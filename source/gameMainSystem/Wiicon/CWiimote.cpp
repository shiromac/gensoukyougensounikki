#include "CWiimote.h"


//これが定義されているときWiiコンをソリューションに含める。
#ifdef USE_WII_REMOTE_SWITCH

#include <process.h>

//---------------------------------------------------------
//_ CWiimote class
//---------------------------------------------------------

CWiimote::CWiimote(HWND hWnd, HANDLE hDeviceH, HANDLE ReadH, HANDLE WriteH, HANDLE hEventH,
				   HIDP_CAPS &Capabilities, OVERLAPPED &HIDOverlapped, int number)
{
	m_hWnd = hWnd;
	_hDeviceHandle = hDeviceH;
	_ReadHandle = ReadH;
	_WriteHandle = WriteH;
	_hEventObject = hEventH;
	_Capabilities = Capabilities;
	_HIDOverlapped = HIDOverlapped;
	m_onceFlag = false;
	_MyDeviceDetected = true;
	m_ReportID = 0x31;
	SetReportID(m_ReportID);//reportIDの変更
	number += 1;
	WiiFeedbackOn();
	WiiLED(number == 1, number == 2, number == 3, number == 4);
	ZeroMemory(&m_irCenter,sizeof(tag_wiiIRPosition));//IRの中身を全てfalseにする
	ZeroMemory(&m_irPosition1,sizeof(tag_wiiIRPosition));//IRの中身を全てfalseにする
	ZeroMemory(&m_irPosition2,sizeof(tag_wiiIRPosition));//IRの中身を全てfalseにする
	WiiButtonsAndMotionsAndIR();//モーションデータがほしい
	WiiMotionCalibration();//規定値の設定
	// 別スレッドを立てる
	CreateThread();
}


VOID CWiimote::SetReportID(UCHAR reportID)
{
	if(_MyDeviceDetected==FALSE)
	{
		return;
		//_MyDeviceDetected=FindTheHID();
	}
	ZeroMemory(_OutputReport,sizeof(_OutputReport));
	_OutputReport[0] = 0x12;
	_OutputReport[2] = reportID;
	WriteOutputReport();
}


CWiimote::~CWiimote()
{
	WiiFeedbackOff();
	DeleteThread();
	CloseHandles();//ハンドルを閉じる
}

VOID CWiimote::CloseHandles()
{
	//Close open handles.
	if (_hDeviceHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(_hDeviceHandle);
		_hDeviceHandle = INVALID_HANDLE_VALUE;
	}

	if (_ReadHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(_ReadHandle);
		_ReadHandle = INVALID_HANDLE_VALUE;
	}

	if (_WriteHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(_WriteHandle);
		_WriteHandle = INVALID_HANDLE_VALUE;
	}
}



VOID CWiimote::WriteOutputReport()
{
	//Send a report to the device.

	DWORD	BytesWritten = 0;
	ULONG	Result;

	if(_OutputReport[0] == 0)
		return;//何もしない場合

	if (_WriteHandle != INVALID_HANDLE_VALUE)
	{
		//----------------------------------------------------
		//API Function: WriteFile 情報をデバイスに書き込む
		//----------------------------------------------------
		Result = WriteFile(_WriteHandle, 
				_OutputReport, 
				_Capabilities.OutputReportByteLength, 
				&BytesWritten,//何バイト書き込んだか？ 
				NULL);

	
	}
//	DWORD err = GetLastError();//最新のエラー

	if (!Result)
	{
		//書き込み失敗時、ハンドルが死んでると判断
		CloseHandles();
		//MessageBox(m_hWnd,_T("デバイスにwriteできませんでした。コントローラーの状態を確認してください"),_T("エラー"),MB_OK);
		_MyDeviceDetected = FALSE;
	}
}

VOID CWiimote::ReadInputReport()
{

	// Retrieve an Input report from the device.

	DWORD	Result;
	
	if(_ReadHandle != INVALID_HANDLE_VALUE)
	{
		//--------------------------------------
		//API call:ReadFile デバイスの情報を読む
		//--------------------------------------
		ReadFile(_ReadHandle, 
			_InputReport,
			_Capabilities.InputReportByteLength, 
			&_NumberOfBytesRead,
			(LPOVERLAPPED) 
			&_HIDOverlapped); 
		//---------------------------------------------------------
		//API call:GetOverlappedResult オーバーラップのデータを読む
		//---------------------------------------------------------
		Result = GetOverlappedResult( _ReadHandle, &_HIDOverlapped, &_NumberOfBytesRead, FALSE );
		if(Result != S_OK && GetLastError() != ERROR_IO_PENDING && GetLastError() != ERROR_IO_INCOMPLETE)
		{
			_tprintf(_T("GetOverlappedResult失敗\n"));
//			MessageBox(m_hWnd,_T("GetOverlappedResult失敗"),_T("えらこ"),MB_OK);
			OutputDebugString(_T("GetOverlappedResult失敗\n"));
		}
	}


	//------------------------------------------------------------------------------
	//API call:WaitForSingleObject　タイムアウトするまでデバイスから応答があるか待つ
	//------------------------------------------------------------------------------
	Result = WaitForSingleObject(_hEventObject, 500);//マジックナンバー使用 何も押さないとかの時にタイムアウトとか発生する。微妙なところ

	
	if(Result != WAIT_OBJECT_0)
	{
		// 反応がないと仮定する→こいつはだめということにする
		//何らかのエラー
		//MessageBox(m_hWnd,_T("readのタイムアウト？困ったやつだ"),_T("えらりんこ"),MB_OK);

		//-------------------------------------------
		//API call: CancelIo ReadFileをキャンセルする
		//-------------------------------------------
		Result = CancelIo(_ReadHandle);

		CloseHandles();//ハンドルを閉じる
		_MyDeviceDetected = FALSE;
	}

	//---------------------------------------
	//API call: ResetEvent イベントのリセット
	//---------------------------------------
	ResetEvent(_hEventObject);
	
}

//-------------------
//ブルブルする
//------------------
VOID CWiimote::WiiFeedbackOn()
{
	if(_MyDeviceDetected==FALSE)
	{
		return;
//_		_MyDeviceDetected=FindTheHID();
	}
	
	ZeroMemory(_OutputReport,sizeof(_OutputReport));

	_OutputReport[0] = 0x13;
	_OutputReport[1] = 0x05;
	WriteOutputReport();
	
}

//-------------------
//ブルブルとめる
//-------------------
VOID CWiimote::WiiFeedbackOff()
{
	if(_MyDeviceDetected==FALSE)
	{
		return;
//_		_MyDeviceDetected=FindTheHID();
	}
	ZeroMemory(_OutputReport,sizeof(_OutputReport));

	_OutputReport[0] = 0x13;
	_OutputReport[1] = 0x04;

	WriteOutputReport();
}

//--------------------------
//Wiiコン本体の四つのLED制御
//--------------------------
VOID CWiimote::WiiLED(BOOL left,BOOL centerLeft,BOOL centerRight,BOOL right)
{
	if(_MyDeviceDetected==FALSE)
	{
		return;
//_		_MyDeviceDetected=FindTheHID();
	}
	ZeroMemory(_OutputReport,sizeof(_OutputReport));

	//点灯位置決定
	TCHAR Num=0;
	if(left)
		Num += 0x10;
	if(centerLeft)
		Num += 0x20;
	if(centerRight)
		Num += 0x40;
	if(right)
		Num += 0x80;

	_OutputReport[0] = 0x11;
	_OutputReport[1] = Num;

	WriteOutputReport();
}



//----------------------------------------------
//バッテリーの残量をパーセントで＋ヌンチャクが接続のチェック(失敗率高い)
//-----------------------------------------------
VOID CWiimote::WiiGetControllerStatus()
{
	if(_MyDeviceDetected==FALSE)
	{
		return;
//_		_MyDeviceDetected=FindTheHID();
	}
	ZeroMemory(_InputReport,sizeof(_InputReport));
	ZeroMemory(_OutputReport,sizeof(_OutputReport));


	SetReportID(0x31);//reportIDの変更
	Sleep(10);//reportIDが変わるまで待つ

	_OutputReport[0] = 0x15;//statusの要求
	WriteOutputReport();

	_InputReport[0] = 0x20;
	_InputReport[3] = 0x02;
	ReadInputReport();//送った結果を受け取る

	if(_InputReport[0] != 0x20)
		return;//失敗

	//----------注意！2.00だと理論値。1.98くらいが本当の最大値っぽい--------
	m_dBattery = _InputReport[6]/2.00;
	
	if(_InputReport[3] & 0x02)
	{
//		cout << _T("ヌンチャク発見") << endl;
//		OutputDebugString(_T("ヌンチャク発見\n"));
		m_bNunchukPlugged = TRUE;
	}else{
		m_bNunchukPlugged = FALSE;
	}
	//0x38
	//0x3e 0x37はだめっぽい

	SetReportID(m_ReportID);//reportIDを元に戻す

}


//-------------------------------------------------------------------------
//偽の規定値の設定　本当はユーザに3回リモコンを動かしてもらわないとできない
//-------------------------------------------------------------------------
VOID CWiimote::WiiMotionCalibration()
{
	//メモリの0x16番地から0x18の3バイト、0x1aから0xcの3バイトを書き換える物（普通は一回書き込めば二度と書き換えない？）
	//メモリの0x20番地からのところはバックアップ用に使えるかも
	//この関数でやることは、メモリから読み出して規定値を発見するんじゃなくて、現在の傾きから推測する。
	//イカサマなので十分関数を読んで使用すること。
	
	//現在、Bボタンが上に向いて、机の上のような水平なところに置いてあると仮定する。

	//現在の傾きから推測する
	m_MotionBaseX.G0 = m_motions.X;//0G
	m_MotionBaseY.G0 = m_motions.Y;//0G
	m_MotionBaseZ.G1 = 0xff - m_motions.Z;//1G

	m_MotionBaseX.G1 = m_MotionBaseZ.G1;
	m_MotionBaseY.G1 = m_MotionBaseZ.G1;
	m_MotionBaseZ.G0 = m_MotionBaseX.G0;
}


VOID CWiimote::WiiReadMemory()
{
	if(_MyDeviceDetected==FALSE)
	{
//_		_MyDeviceDetected=FindTheHID();
		return;
	}
	ZeroMemory(_InputReport,sizeof(_InputReport));
	ZeroMemory(_OutputReport,sizeof(_OutputReport));

	_OutputReport[0] = 0x17;
	_OutputReport[1] = 0x02;
	_OutputReport[4] = 0x20;//開始アドレス？
	_OutputReport[6] = 0x0F;//サイズ

	WriteOutputReport();
	
	_InputReport[0] = 0x21;
	ReadInputReport();//送った結果を受け取る

	if(_InputReport[0] == 0x21)
	{

  		cout << _InputReport[0] << endl;

	}else{
		//MessageBox(m_hWnd,_T("Readレポート無理無理無理メモリ読めない"),_T("えらでぃん"),MB_OK);
	}

}

//---------------------------------------
//ボタン＋モーション＋赤外線の検知
//---------------------------------------
VOID CWiimote::WiiButtonsAndMotionsAndIR()
{
	if(_MyDeviceDetected==FALSE)
	{
//_		_MyDeviceDetected=FindTheHID();
		return;
	}

	if(!m_onceFlag)
	{
		//一回だけしか実行しないもの
		//WiiWriteOutputReport(3, 0x12, 0x00, 0x33);
		WiiWriteOutputReport(2, 0x13, 0x04);
		Sleep(20);
		WiiWriteOutputReport(2, 0x1A, 0x04);
		Sleep(20);
		WiiWriteOutputReport(7, 0x16, 0x04, 0xB0, 0xFF, 0x30, 0x01, 0x01);//レジスタへの書き込み
		Sleep(20);
		WiiWriteOutputReport(13, 0x16, 0x04, 0xB0, 0xFF, 0x00, 0x07, 0x02, 0x00, 0x00, 0x71, 0x01, 0x00, 0xC8);
		Sleep(20);
		WiiWriteOutputReport(8, 0x16, 0x04, 0xB0, 0xFF, 0x07, 0x02, 0x00, 0x36);
		Sleep(20);
		WiiWriteOutputReport(8, 0x16, 0x04, 0xB0, 0xFF, 0x1A, 0x02, 0x35, 0x03);
		Sleep(20);
		WiiWriteOutputReport(7, 0x16, 0x04, 0xB0, 0xFF, 0x33, 0x01, 0x03);
		Sleep(20);
		WiiWriteOutputReport(7, 0x16, 0x04, 0xB0, 0xFF, 0x30, 0x01, 0x08);
		Sleep(20);
		WiiWriteOutputReport(3, 0x12, 0x00, 0x33);
		m_onceFlag = true;
	}

	ZeroMemory(_InputReport,sizeof(_InputReport));

	ReadInputReport();
	if((_InputReport[0] & 0xF0) != 0x30) //m_ReportID)
		return;//失敗:レポートIDが違っていたとき

	ZeroMemory(&m_buttons,sizeof(m_buttons));//ボタンの中身を全てfalseにする
	
	if(_InputReport[2] & 0x01)
		m_buttons.Two = TRUE;
	if(_InputReport[2] & 0x02)
		m_buttons.One = TRUE;
	if(_InputReport[2] & 0x04)
		m_buttons.B = TRUE;
	if(_InputReport[2] & 0x08)
		m_buttons.A = TRUE;
	if(_InputReport[2] & 0x10)
		m_buttons.Minus = TRUE;
//	if(_InputReport[2] & 0x20)//現在解析されていない部分
//	if(_InputReport[2] & 0x40)//現在解析されていない部分
	if(_InputReport[2] & 0x80)
		m_buttons.Home = TRUE;
	if(_InputReport[1] & 0x01)
		m_buttons.Left = TRUE;
	if(_InputReport[1] & 0x02)
		m_buttons.Right = TRUE;
	if(_InputReport[1] & 0x04)
		m_buttons.Down = TRUE;
	if(_InputReport[1] & 0x08)
		m_buttons.Up = TRUE;
	if(_InputReport[1] & 0x10)
		m_buttons.Plus = TRUE;
//	if(_InputReport[1] & 0x20)//現在解析されていない部分
//	if(_InputReport[1] & 0x40)//現在解析されていない部分
//	if(_InputReport[1] & 0x80)//現在解析されていない部分

	ZeroMemory(&m_motions,sizeof(m_motions));//モーションの中身を全てfalseにする

	//加速度センサー
	//x軸
//	cout << _T("X軸:") << (int)_InputReport[3] << endl;;
	m_motions.X = (UCHAR)_InputReport[3];

	//y軸
//	cout << _T("Y軸:") << (int)_InputReport[4] << endl;;
	m_motions.Y = (UCHAR)_InputReport[4];

	//z軸
//	cout << _T("Z軸:") << (int)_InputReport[5] << endl;;
	m_motions.Z = (UCHAR)_InputReport[5];


    if(_InputReport[0] & 0x02)//IRが送られてきたとき
	{

		// 一つ目
		UINT rest = (_InputReport[8] & 0xf0) >> 4;
		SHORT newX = ((rest & 0x03) << 8) + _InputReport[6];//x
		SHORT newY = ((rest & 0x0c) << 6) + _InputReport[7];//y

		if((newX != 1023) && (newY != 1023))
		{
			m_irPosition1.X = newX;//一個目の点
			m_irPosition1.Y = newY;
			m_irPosition1.enable = true; 
		}
		else
			m_irPosition1.enable = false;

		newX = 0;
		newY = 0;
		// 二つ目

		rest = (_InputReport[11] & 0xf0) >> 4;
		newX = ((rest & 0x03) << 8) + _InputReport[9];//x
		newY = ((rest & 0x0c) << 6) + _InputReport[10];

		if((newX != 1023) && (newY != 1023)) 
		{
			m_irPosition2.X = newX;//2個目の点
			m_irPosition2.Y = newY;	
			m_irPosition2.enable = true; 
		}
		else
			m_irPosition2.enable = false; 

		m_irCenter.X = (SHORT)((m_irPosition1.X + m_irPosition2.X)/(double)2);//中央値
		m_irCenter.Y = (SHORT)((m_irPosition1.Y + m_irPosition2.Y)/(double)2);

	}

}
VOID CWiimote::WiiButtonsAndMotionsAndIRAndNunchuk()
{
	static bool onceFlag = false;
	if(!onceFlag)
	{
		//一回だけしか実行しないもの
		WiiWriteOutputReport(6, 0x16, 0x04, 0xA4, 0x00, 0x40, 0x01);//レジスタ書き込み
		onceFlag = true;
	}

	WiiButtonsAndMotionsAndIR();//WiiButtonsAndMotionsAndIR()の結果を利用する
	if(_InputReport[0] != m_ReportID)
		return;//失敗:レポートIDが違っていたとき
	
	ZeroMemory(&m_Nunchuk,sizeof(tag_wiiNunchuk));//ヌンチャクの中身をすべてfalseにする


	//暗号化されているからそれを解く
	m_Nunchuk.analogX = (_InputReport[16] ^ 0x17) + 0x17;
	m_Nunchuk.analogY = (_InputReport[17] ^ 0x17) + 0x17;
	m_Nunchuk.motionX = (_InputReport[18] ^ 0x17) + 0x17;
	m_Nunchuk.motionY = (_InputReport[19] ^ 0x17) + 0x17;
	m_Nunchuk.motionZ = (_InputReport[20] ^ 0x17) + 0x17;
	UCHAR button = (_InputReport[21] ^ 0x17) + 0x17;
	m_Nunchuk.buttonC = ((button & 0x02) >> 1) ^ 0x01;
	m_Nunchuk.buttonZ = (button & 0x01) ^ 0x01;

}


VOID CWiimote::WiiWriteMemory()
{
	if(_MyDeviceDetected==FALSE)
	{
//_		_MyDeviceDetected=FindTheHID();
		return;
	}
	ZeroMemory(_OutputReport,sizeof(_OutputReport));

	_OutputReport[0] = 0x16;//memWrite

	_OutputReport[4] = 0x16;//開始バイト
	_OutputReport[5] = 0x01;//バイト数
	_OutputReport[6] = 0x7d;
//	_OutputReport[7] = 0x7c;
//	_OutputReport[8] = 0x7c;
//	_OutputReport[6] = 0x05;
//	_OutputReport[7] = 0x05;
//	_OutputReport[8] = 0x05;


	WriteOutputReport();   

	if(_OutputReport[0] == 0x16)
	{
		MessageBox(m_hWnd,_T("メモリ書き込み成功"),_T("せいこうせいこう"),MB_OK);
	}

}


//_ OutputReportにいちいち書くのが面倒なので作ってみた
VOID CWiimote::WiiWriteOutputReport(UCHAR len, ...)
{
	va_list args;

	ZeroMemory(_OutputReport, sizeof(_OutputReport));
	va_start(args, len);
	for(int i = 0; i < len; i++)
	{
		_OutputReport[i] = va_arg(args, UCHAR);
	}
	va_end(args);
	WriteOutputReport();
}

BOOL CWiimote::GetConected()
{
	return _MyDeviceDetected;
}

//_ Debug状態ではあるが、Soundをつけてみた。
VOID CWiimote::WiiDebugSound(bool init_flag)
{
	static int sound_volume = 0x66;
	if(init_flag)
	{
		//WiiWriteOutputReport(3, 0x14, 0x00, 0x04);
		WiiWriteOutputReport(2, 0x14, 0x04);
		WiiWriteOutputReport(2, 0x19, 0x04);
		WiiWriteOutputReport(7, 0x16, 0x04, 0xA2, 0x00, 0x09, 0x01, 0x01);
		WiiWriteOutputReport(7, 0x16, 0x04, 0xA2, 0x00, 0x01, 0x01, 0x08);
		WiiWriteOutputReport(7, 0x16, 0x04, 0xA2, 0x00, 0x01, 0x01, 0x00);
		WiiWriteOutputReport(7, 0x16, 0x04, 0xA2, 0x00, 0x02, 0x01, 0x00);
		WiiWriteOutputReport(7, 0x16, 0x04, 0xA2, 0x00, 0x03, 0x01, 0x00);
		WiiWriteOutputReport(7, 0x16, 0x04, 0xA2, 0x00, 0x04, 0x01, 0x0D);
		WiiWriteOutputReport(7, 0x16, 0x04, 0xA2, 0x00, 0x05, 0x01, sound_volume);
		WiiWriteOutputReport(7, 0x16, 0x04, 0xA2, 0x00, 0x06, 0x01, 0x00);
		WiiWriteOutputReport(7, 0x16, 0x04, 0xA2, 0x00, 0x07, 0x01, 0x00);
		//WiiWriteOutputReport(7, 0x16, 0x04, 0xA2, 0x00, 0x08, 0x01, 0x00);
		WiiWriteOutputReport(7, 0x16, 0x04, 0xA2, 0x00, 0x08, 0x01, 0x01);
		WiiWriteOutputReport(2, 0x19, 0x00);
		_tprintf(_T("\n来たよ\n"));
	}
	if(m_buttons.Minus)
	{
		sound_volume = (sound_volume < 5) ? 0 : sound_volume - 5;
		WiiWriteOutputReport(7, 0x16, 0x04, 0xA2, 0x00, 0x05, 0x01, sound_volume);
	}
	else if(m_buttons.Plus)
	{
		sound_volume = (sound_volume > 250) ? 255 : sound_volume + 5;
		WiiWriteOutputReport(7, 0x16, 0x04, 0xA2, 0x00, 0x05, 0x01, sound_volume);
	}
	else if(1)
	{
		// Sound Data送信
		WiiWriteOutputReport(22, 0x18, (0x14 << 3), 0x13, 0xCC, 0x53, 0x77,
			0x13, 0xCC, 0x53, 0x77, 0x13, 0xCC, 0x53, 0x77, 0x13, 0xCC, 0x53, 0x77
		, 0x13, 0xCC, 0x53, 0x77);
	}
}

VOID CWiimote::WiiGetAngle()
{
	int y_g0 = m_MotionBaseY.G0;
	int y_g1 = m_MotionBaseY.G1;
	int gm = y_g0 * 2 - y_g1;
	m_angles.pitch = (float)(m_MotionBaseY.G1 - m_motions.Y) / (m_MotionBaseY.G1 - gm) * ((m_motions.Z < m_MotionBaseZ.G0) ? -1 : 1);
	gm = m_MotionBaseX.G0 * 2 - m_MotionBaseX.G1;
	m_angles.roll = (float)(m_MotionBaseX.G1 - m_motions.X) / (m_MotionBaseX.G1 - gm);
	m_angles.yaw = 0;//(float)m_motions.Z / m_MotionBaseZ.G0 + (m_motions.Z - m_MotionBaseZ.G0 > 0);
	_tprintf(_T("pitch %f\nroll %f\nyaw(not) %f\n"), m_angles.pitch, m_angles.roll, m_angles.yaw);
}


void CWiimote::CreateThread()
{
	DWORD thID;
	m_loopflag = true;
	// まず、サブスレッドを作成
	m_hDrawTh = (HANDLE)_beginthreadex(NULL, 0, &AnotherThread, this, CREATE_SUSPENDED, (unsigned int*)&thID);
	if (m_hDrawTh == 0) {
        ;//ERROR_EXIT_EX(_T("スレッド作成失敗"));
        return;
    }
	// 次にイベントをつくってやる
	m_hDrawEvent = CreateEventA(NULL, TRUE, FALSE, NULL);
	// 作ったら、優先順位を決めよう。
	SetThreadPriority(m_hDrawTh, THREAD_PRIORITY_NORMAL);
	// 非シグナル状態にします
	ResetEvent(m_hDrawEvent);
	ResumeThread(m_hDrawTh);
	return;
}

void CWiimote::DeleteThread()
{
	// 終わったら、イベントをシグナル状態にする。
	SetEvent(m_hDrawEvent);
	m_loopflag = false;
    WaitForSingleObject(m_hDrawTh, INFINITE);        
	if(!CloseHandle(m_hDrawTh))
		;//ERROR_EXIT_EX(_T("エラー"));
	if(!CloseHandle(m_hDrawEvent))
		;//ERROR_EXIT_EX(_T("エラー。"));
	m_hDrawTh = NULL;
	m_hDrawEvent = NULL;
}

unsigned WINAPI CWiimote::AnotherThread(void* p)
{
	CWiimote* p_cw = (CWiimote*)p;
	while(p_cw->m_loopflag)
	{
		p_cw->WiiButtonsAndMotionsAndIR();
		Sleep(1);
	}
	return 0;
}

#endif