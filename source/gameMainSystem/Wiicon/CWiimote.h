#ifndef _CWiimote_h_
#define _CWiimote_h_


#include "Wiicondefph.h"
//これが定義されているときWiiコンをソリューションに含める。
#ifdef USE_WII_REMOTE_SWITCH





//_	
//_	CWiimote.h
//_	最終更新日　07/02/27
//_	

#pragma comment(lib, "hid.lib")
#pragma comment(lib, "setupapi.lib")


#include <windows.h>
#include <String>
#include <iostream>
#include <sstream>

using namespace std;

//DDKより
extern _T("C") 
{

//	This file is in the Windows DDK available from Microsoft.
	#include "hidsdi.h"
	#include <setupapi.h>
	#include <dbt.h>
}


struct tag_wiiButton
{
	BOOL One;
	BOOL Two;
	BOOL B;
	BOOL A;
	BOOL Minus;
	BOOL Home;
	BOOL Left;
	BOOL Right;
	BOOL Down;
	BOOL Up;
	BOOL Plus;
};

struct tag_wiiMotion
{
	UCHAR X;
	UCHAR Y;
	UCHAR Z;
};

struct tag_wiiIRPosition
{
	int X;
	int Y;
	bool enable;
};

struct tag_wiiNunchuk
{
	UCHAR analogX;//アナログスティックX
	UCHAR analogY;//アナログスティックY
	UCHAR motionX;//加速度X
	UCHAR motionY;//加速度Y
	UCHAR motionZ;//加速度Z
	BOOL buttonC;//Cボタン
	BOOL buttonZ;//Zボタン
};

struct tag_baseGravity
{
	UCHAR G0;//0Gの時の値
	UCHAR G1;//1Gの時の値
};

struct tag_angles
{
	float yaw;
	float pitch;
	float roll;
};

//---------------------------------------------------------
//_ CWiimote class
//---------------------------------------------------------
class CWiimote
{
public:
	CWiimote(HWND hWnd, HANDLE hDeviceH, HANDLE ReadH, HANDLE WriteH, HANDLE hEventH, HIDP_CAPS& Capabilities,
		OVERLAPPED& HIDOverlapped, int number);
	~CWiimote();
private:
	UCHAR								_InputReport[256];
	UCHAR								_OutputReport[256];
	OVERLAPPED							_HIDOverlapped;
	DWORD								_NumberOfBytesRead;

	HANDLE								_hDeviceHandle;
	HANDLE								_hEventObject;
	HANDLE								_ReadHandle;
	HANDLE								_WriteHandle;

	HIDP_CAPS							_Capabilities;
	BOOL								_MyDeviceDetected;

	HWND								m_hWnd;//ウインドウハンドル
	UCHAR								m_ReportID;//レポートのID
	bool								m_onceFlag;
	HANDLE								m_hDrawEvent;
	HANDLE								m_hDrawTh;
private:

public:
	tag_baseGravity m_MotionBaseX;//重力加速度用
	tag_baseGravity m_MotionBaseY;//重力加速度用
	tag_baseGravity m_MotionBaseZ;//重力加速度用
private:
	VOID CloseHandles();//ハンドルを閉じる
	VOID WriteOutputReport();//レポート送信
	VOID ReadInputReport();//レポート受信

	// OutputReportに入れる。
	VOID WiiWriteOutputReport(UCHAR len, ...);
	//レポート番号のセット
	VOID SetReportID(UCHAR reportID);

	// デバック用
	VOID WiiReadMemory();
	// デバッグ用(非常に危険)
	VOID WiiWriteMemory();
	static unsigned WINAPI AnotherThread(void* p);
	VOID CreateThread();
	VOID DeleteThread();
public:
	bool								m_loopflag;
	tag_wiiButton m_buttons;//ボタンの状態が格納されてる
	tag_wiiMotion m_motions;//モーション状態が格納されとる
	tag_wiiIRPosition m_irPosition1;//IRの位置情報の一個目
	tag_wiiIRPosition m_irPosition2;//IRの位置情報の二個目
	tag_wiiIRPosition m_irCenter;//位置の中心
	tag_angles			m_angles;				// 角度
	tag_wiiNunchuk m_Nunchuk;//ヌンチャク情報
	BOOL  m_bNunchukPlugged;//ヌンチャクが着いているかどうか
	DOUBLE m_dBattery;//バッテリーの残量

  
	//Input情報
	VOID WiiButtonsAndMotionsAndIR();
	VOID WiiButtonsAndMotionsAndIRAndNunchuk();
	// 角度取得
	VOID WiiGetAngle();

	// 振動OFF
	VOID WiiFeedbackOff();
	// 振動ON
	VOID WiiFeedbackOn();

	//バッテリー残量(Sleepをかけているため、メインループ内ではまわさないこと)
//	DOUBLE WiiGetBatteries();
	//ヌンチャクがあるか(Sleepをかけているため、メインループ内ではまわさないこと)  
//	BOOL WiiNunchukPlugged();

	//----------------------------------------------------------------------------------------------//
	//上記二つの関数を一個にまとめた。理由はレポートID(0x20)が安定しないから                        //
	//この関数は糞重いから気をつけること あと、他の関数に影響を与えるから極力使うところを絞ること   //
	//----------------------------------------------------------------------------------------------//
	VOID WiiGetControllerStatus();//バッテリーの残量とヌンチャクが装着されているかどうかチェック

	// LED表示
	VOID WiiLED(BOOL left,BOOL centerLeft,BOOL centerRight,BOOL right);
	// 加速度校正（未実装
	VOID WiiMotionCalibration();
	VOID WiiDebugSound(bool init_flag);
	// 今、取れているかどうかの確認
	BOOL GetConected();
};





#else

//------------------------------------
//ダミー
//------------------------------------
class CWiimote
{
public:
	CWiimote(){};
	~CWiimote(){};
};
#endif

#endif