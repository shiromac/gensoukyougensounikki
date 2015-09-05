#include "Wiicondefph.h"
//これが定義されているときWiiコンをソリューションに含める。
#ifdef USE_WII_REMOTE_SWITCH


#include "CWiimoteManager.h"
//_ test start
#include <conio.h>
//_ test end

//---------------------------------------------------------
//_ CWiimoteManager class
//---------------------------------------------------------
DWORD CWiimoteManager::_player_num;
CWiimote* CWiimoteManager::_wiimote[CWiimoteManager::MAX_PLAYER];

//----コンストラクタ----//
CWiimoteManager::CWiimoteManager(HWND hWnd)
{
	m_hWnd = hWnd;//ウインドウハンドル取得。コンソールの場合、デスクトップのウィンドウハンドルになる
	_hDeviceHandle = INVALID_HANDLE_VALUE;
	_hEventObject = NULL;
	_hDevInfo = NULL;
	_ReadHandle = INVALID_HANDLE_VALUE;
	_WriteHandle = INVALID_HANDLE_VALUE;
	_MyDeviceDetected = FALSE;
	memset(_wiimote, 0, sizeof(CWiimote*) * CWiimoteManager::MAX_PLAYER);
}

//----デストラクタ----//
CWiimoteManager::~CWiimoteManager()
{
	CloseHandles();//ハンドルを閉じる
	for(int i = 0; i < CWiimoteManager::MAX_PLAYER; i++)
	{
		if(_wiimote[i])
			delete(_wiimote[i]);
	}
}

void CWiimoteManager::FindWiimote()
{
	FindTheHID();
}

CWiimote* CWiimoteManager::GetWiimote(int number)
{
	if((unsigned)number < _player_num && number >= 0 
		&& _wiimote[number] && _wiimote[number]->GetConected())
		return _wiimote[number];
	return NULL;
}

VOID CWiimoteManager::RegisterForDeviceNotifications()
{
	//wiiリモコンが繋がったり外されたりしたときにいろいろ返ってくる。
	//そのときのメッセージは「WM_DEVICECHANGE」

	DEV_BROADCAST_DEVICEINTERFACE DevBroadcastDeviceInterface;
	HDEVNOTIFY DeviceNotificationHandle;

	DevBroadcastDeviceInterface.dbcc_size = sizeof(DevBroadcastDeviceInterface);
	DevBroadcastDeviceInterface.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	DevBroadcastDeviceInterface.dbcc_classguid = _HidGuid;

	//----------------------------------------------------------------------------------------
	//API function: RegisterDeviceNotification デバイスをウィンドウハンドルとともに登録する
	//----------------------------------------------------------------------------------------
	DeviceNotificationHandle = RegisterDeviceNotification(m_hWnd, &DevBroadcastDeviceInterface, DEVICE_NOTIFY_WINDOW_HANDLE);

	if(DeviceNotificationHandle == NULL)
		MessageBox(m_hWnd,_T("DeviceNotificationHandleが無理"),_T("えらら"),MB_OK);

	//本当は終わったあとUnregisterDeviceNotificationを呼ぶ必要がある
}

VOID CWiimoteManager::GetDeviceCapabilities()
{
	//デバイスの能力を検出する
	PHIDP_PREPARSED_DATA	PreparsedData;


	//-------------------------------------------------------------------------//
	//API function: HidD_GetPreparsedData デバイスの能力が入った情報を取得する //
	//-------------------------------------------------------------------------//
	HidD_GetPreparsedData(_hDeviceHandle, &PreparsedData);

	//----------------------------------------------------------------------------------//
	//API function: HidP_GetCaps デバイスの能力を取り出す　これから特性を知ることが可能 //
	//----------------------------------------------------------------------------------//
	HidP_GetCaps(PreparsedData, &_Capabilities);

	//_ test start
	_tprintf(_T("%s%X\n"), _T("Usage: "), _Capabilities.Usage);
	_tprintf(_T("%s%X\n"), _T("Usage Page: "), _Capabilities.UsagePage);
	_tprintf(_T("%s%d\n"), _T("Input Report SByte Length: "), _Capabilities.InputReportByteLength);
	_tprintf(_T("%s%d\n"), _T("Output Report SByte Length: "), _Capabilities.OutputReportByteLength);
	_tprintf(_T("%s%d\n"), _T("Feature Report SByte Length: "), _Capabilities.FeatureReportByteLength);
	_tprintf(_T("%s%d\n"), _T("Number of Link Collection Nodes: "), _Capabilities.NumberLinkCollectionNodes);
	_tprintf(_T("%s%d\n"), _T("Number of Input Button Caps: "), _Capabilities.NumberInputButtonCaps);
	_tprintf(_T("%s%d\n"), _T("Number of InputValue Caps: "), _Capabilities.NumberInputValueCaps);
	_tprintf(_T("%s%d\n"), _T("Number of InputData Indices: "), _Capabilities.NumberInputDataIndices);
	_tprintf(_T("%s%d\n"), _T("Number of Output Button Caps: "), _Capabilities.NumberOutputButtonCaps);
	_tprintf(_T("%s%d\n"), _T("Number of Output Value Caps: "), _Capabilities.NumberOutputValueCaps);
	_tprintf(_T("%s%d\n"), _T("Number of Output Data Indices: "), _Capabilities.NumberOutputDataIndices);
	_tprintf(_T("%s%d\n"), _T("Number of Feature Button Caps: "), _Capabilities.NumberFeatureButtonCaps);
	_tprintf(_T("%s%d\n"), _T("Number of Feature Value Caps: "), _Capabilities.NumberFeatureValueCaps);
	_tprintf(_T("%s%d\n"), _T("Number of Feature Data Indices: "), _Capabilities.NumberFeatureDataIndices);
	//_ test end
/*
	(_T("%s%X"), _T("Usage Page: "), Capabilities.UsagePage);
	(_T("%s%d"), _T("Input Report SByte Length: "), Capabilities.InputReportByteLength);
	(_T("%s%d"), _T("Output Report SByte Length: "), Capabilities.OutputReportByteLength);
	(_T("%s%d"), _T("Feature Report SByte Length: "), Capabilities.FeatureReportByteLength);
	(_T("%s%d"), _T("Number of Link Collection Nodes: "), Capabilities.NumberLinkCollectionNodes);
	(_T("%s%d"), _T("Number of Input Button Caps: "), Capabilities.NumberInputButtonCaps);
	(_T("%s%d"), _T("Number of InputValue Caps: "), Capabilities.NumberInputValueCaps);
	(_T("%s%d"), _T("Number of InputData Indices: "), Capabilities.NumberInputDataIndices);
	(_T("%s%d"), _T("Number of Output Button Caps: "), Capabilities.NumberOutputButtonCaps);
	(_T("%s%d"), _T("Number of Output Value Caps: "), Capabilities.NumberOutputValueCaps);
	(_T("%s%d"), _T("Number of Output Data Indices: "), Capabilities.NumberOutputDataIndices);
	(_T("%s%d"), _T("Number of Feature Button Caps: "), Capabilities.NumberFeatureButtonCaps);
	(_T("%s%d"), _T("Number of Feature Value Caps: "), Capabilities.NumberFeatureValueCaps);
	(_T("%s%d"), _T("Number of Feature Data Indices: "), Capabilities.NumberFeatureDataIndices);
*/
	//これ以上必要ないから開放する
	HidD_FreePreparsedData(PreparsedData);
}

VOID CWiimoteManager::GetReadAndWriteHandles()
{

	//-------------------------------------
	//CreateFile Writeハンドルの作成
	//-------------------------------------
	_WriteHandle=CreateFile(_detailData->DevicePath, 
			GENERIC_WRITE,//writeハンドル
			FILE_SHARE_READ|FILE_SHARE_WRITE, 
			(LPSECURITY_ATTRIBUTES)NULL,
			OPEN_EXISTING, 
			0, 
			NULL);
	
	if(_WriteHandle == INVALID_HANDLE_VALUE)
	{
		MessageBox(m_hWnd,_T("WriteHandleが取れん"),_T("ｴﾛｲｴﾗｰ"),MB_OK);
	}

	//--------------------------------------------------------------------------------
	//CreateFile Readハンドルの作成 ここではオーバーラップ可能なハンドルを作成している
	//--------------------------------------------------------------------------------
	_ReadHandle=CreateFile(_detailData->DevicePath, 
			GENERIC_READ, //read用
			FILE_SHARE_READ|FILE_SHARE_WRITE,
			(LPSECURITY_ATTRIBUTES)NULL, 
			OPEN_EXISTING, 
			FILE_FLAG_OVERLAPPED, 
			NULL);

	if(_ReadHandle == INVALID_HANDLE_VALUE)
	{
		MessageBox(m_hWnd,_T("ReadHandleが取れない"),_T("えらーらー"),MB_OK);
		return;
	}
}

BOOL CWiimoteManager::FindTheHID()
{
	//目的のHIDを探す
	HIDD_ATTRIBUTES						Attributes;
	SP_DEVICE_INTERFACE_DATA			devInfoData;
	bool								LastDevice = FALSE;
	int									MemberIndex = 0;
	LONG								Result;	
	tstring								UsageDescription;

	_Length = 0;
	//_ 以下のコードを手直ししました。（多人数のために
	_detailData = NULL;
	_player_num = 0;
	//_ 以上のコードを手直ししました。（多人数のために
	_hDeviceHandle=INVALID_HANDLE_VALUE;

	//----------------------------------------------------------
	//API function: HidD_GetHidGuid　システムにあるHID一覧？を取得
	//----------------------------------------------------------
	HidD_GetHidGuid(&_HidGuid);	
	
	//----------------------------------------------------------
	//	API function: SetupDiGetClassDevs　現在インストールされているデバイスを取得
	//----------------------------------------------------------
	_hDevInfo=SetupDiGetClassDevs(&_HidGuid, 
			NULL, 
			NULL, 
			DIGCF_PRESENT|DIGCF_INTERFACEDEVICE);

	devInfoData.cbSize = sizeof(devInfoData);


	//現在使用可能なデバイスの検索
	MemberIndex = 0;
	LastDevice = FALSE;

	//--------------------------------------------------------
	//_	先ほど手に入れたHDIクラスデバイスリストハンドル(_hDevInfo)とHIDクラスデ
	//_	バイス(devInfoData)の情報を使用して、SDK API のSetupDi クラスAPI 関数
	//_	SetupDiGetDeviceInterfaceDetail()で、デバイスの詳細情報からWindows 内部
	//_	のデバイス名を取得する。
	//_ ↓
	//_ デバイス名を利用してCreateFile()を呼び出しHIDデバイスをオープンし、ファイルハンドルを取る。
	//--------------------------------------------------------
	//Is it the desired device?
	_MyDeviceDetected = FALSE;
	do
	{

		//----------------------------------------------------------
		//API function: SetupDiEnumDeviceInterfaces デバイスのインデックスからデバイス情報の取得
		//----------------------------------------------------------
		Result=SetupDiEnumDeviceInterfaces(_hDevInfo, 
				0, 
				&_HidGuid, 
				MemberIndex, //読み出すHIDの番号
				&devInfoData);

		if (Result != 0)
		{
			//このデバイスは接続されている

			//----------------------------------------------------------
			//API function: SetupDiGetDeviceInterfaceDetail　二回呼ぶ必要がある
			//----------------------------------------------------------
			Result = SetupDiGetDeviceInterfaceDetail(_hDevInfo, 
					&devInfoData, 
					NULL, 
					0, 
					&_Length, 
					NULL);

			//_ 以下のコードを手直ししました。

			//Allocate memory for the hDevInfo structure, using the returned Length.
			_detailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(_Length);

			//Set cbSize in the detailData structure.
			_detailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

			//Call the function again, this time passing it the returned buffer size.

			Result = SetupDiGetDeviceInterfaceDetail(_hDevInfo, 
					&devInfoData, 
					_detailData, 
					_Length, 
					&_Required, 
					NULL);

			//_ test start
			_tprintf(_T("---device detect---\n\n"));
			_tprintf(_T("DeviceInfoHandle:%p\n"), _hDevInfo);
			_tprintf(_T("Flags:%d, Reserved:%d\n"), devInfoData.Flags, devInfoData.Reserved);
			//_ test end

			//----------------------------------------------------------------------
			//API function: CreateFile ハンドルの作成 ここではデバイスハンドルを作成
			//----------------------------------------------------------------------
			_hDeviceHandle=CreateFile(_detailData->DevicePath, 
					0, //書いたり読んだり出来ない
					FILE_SHARE_READ|FILE_SHARE_WRITE, 
					(LPSECURITY_ATTRIBUTES)NULL,
					OPEN_EXISTING, 
					0, 
					NULL);

			if(_hDeviceHandle == INVALID_HANDLE_VALUE)
			{
				MessageBox(m_hWnd,_T("デバイスハンドル作れんかったでー"),_T("えらーさん"),MB_OK);
				return false;
			}
			
			//構造体のサイズを設定
			Attributes.Size = sizeof(Attributes);

			//----------------------------------------------------------
			//API function: HidD_GetAttributes　デバイスの情報を得る。
			//----------------------------------------------------------
			Result = HidD_GetAttributes(_hDeviceHandle, &Attributes);

			if(Result == FALSE)
				MessageBox(m_hWnd,_T("HidD_GetAttributes無理ｽ"),_T("エラー様"),MB_OK);

			

			if (Attributes.VendorID == VendorID && Attributes.ProductID == ProductID)
			{
				//_ test start
				_tprintf(_T("---wiiremote detect---\n\n"));
				_tprintf(_T("Device Handle:%p\n"), _hDeviceHandle);
				_tprintf(_T("Device Path(not available):%c\n"), _detailData->DevicePath);
				_tprintf(_T("VendorID:0x%X, ProductID:0x%X\n"), Attributes.VendorID, Attributes.ProductID);
				_tprintf(_T("Size:%d, Version:%d\n"), Attributes.Size, Attributes.VersionNumber);
				//_ test end
				//wiiコン発見
				_MyDeviceDetected = TRUE;//ほしいデバイスが見つかった
				_MyDevicePathName = _detailData->DevicePath;

				//デバイスが外されたりしたときの情報を送ってもらうための関数
//				RegisterForDeviceNotifications();

				GetDeviceCapabilities();//デバイスのいろんな状態拾える Capabilitiesは受信したり送ったりするときに使ってる

				GetReadAndWriteHandles();//ハンドルの作成

				if(_hEventObject == 0)
				{
					//--------------------------------------------
					//API function: CreateEvent イベントの作成
					//--------------------------------------------
					_hEventObject = CreateEvent(NULL, TRUE, TRUE, _T(""));
					
					if(_hEventObject == NULL)
						MessageBox(m_hWnd,_T("hEventObjectつくれねぇ"),_T("ぇらー"),MB_OK);
					//Set the members of the overlapped structure.
					memset(&_HIDOverlapped, 0, sizeof(OVERLAPPED));
					_HIDOverlapped.hEvent = _hEventObject;
					_HIDOverlapped.Offset = 0;
					_HIDOverlapped.OffsetHigh = 0;
				}
				//_ 良く見ていないが正しいならばここに来ている気がする。wiiの数を増やしてみる。
				//_ ここのifはもしかしてのテスト→試運転で大丈夫なら消すこと
				if(!_wiimote[_player_num])
				{
					_wiimote[_player_num] = new CWiimote(m_hWnd, _hDeviceHandle, _ReadHandle, _WriteHandle, _hEventObject,
						_Capabilities, _HIDOverlapped, _player_num);
					if(!_wiimote[_player_num]->GetConected())
					{
						delete _wiimote[_player_num];
						_wiimote[_player_num] = NULL;
					}
					else
						_player_num++;
					//_ 後は支障が恐いのでNULLにしておく
					_hDeviceHandle = INVALID_HANDLE_VALUE;
					_ReadHandle = INVALID_HANDLE_VALUE;
					_WriteHandle = INVALID_HANDLE_VALUE;
					_hEventObject = NULL;

				}
				else
					MessageBox(m_hWnd,_T("多分このエラーは起きないと信じたい"),_T("あら、起こった？"),MB_OK);
				//_ test start
				if(!_kbhit())
					Sleep(10);
				//_ test end
			}else{
				//_ test start
				_tprintf(_T("---wiiremote not ditect---\n\n"));
				//_ test end
				//wiiコンじゃないときデバイスハンドルを閉じる
				CloseHandle(_hDeviceHandle);
				_hDeviceHandle = INVALID_HANDLE_VALUE;
			}
			//これ以上必要じゃないからメモリ開放する ハンドル作成時に必要なだけみたい
			free(_detailData);

		}else{
			//SetupDiEnumDeviceInterfacesが0を返したらこれ以上デバイス見るデバイスは無い
			LastDevice=TRUE;
		}
		//次のデバイスを見に行く
		MemberIndex = MemberIndex + 1;//次のデバイスに行く

	}while ((LastDevice == FALSE));

	//_ 以上まで手直ししました。
	
	if (_MyDeviceDetected == FALSE)
		//MessageBox(m_hWnd,_T("デバイスが取れません。"),_T("エラー"),MB_OK);


	//Free the memory reserved for hDevInfo by SetupDiClassDevs.
	SetupDiDestroyDeviceInfoList(_hDevInfo);

	return _MyDeviceDetected;
}

VOID CWiimoteManager::CloseHandles()
{
	//Close open handles.
	if (_hDeviceHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(_hDeviceHandle);
	}

	if (_ReadHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(_ReadHandle);
	}

	if (_WriteHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(_WriteHandle);
	}
}


#endif