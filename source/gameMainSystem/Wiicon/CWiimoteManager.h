#include "Wiicondefph.h"
//これが定義されているときWiiコンをソリューションに含める。
#ifdef USE_WII_REMOTE_SWITCH

//_	
//_	CWiimoteManager.h
//_	最終更新日　07/02/27
//_	まだまだ無駄なところ多いと思います。
//_ 部室で直したレポート番号を付け加えること。
//_ http://www.madlabo.com/mad/gid/research/usb/usb_dev.pdf
/*	___ これから直すべきこと ___
	Report 20について
	IRセンサーの動きをよくする。
	もし、多人数化がうまく機能するならば、自分がいなくなったときは探しにいく関数を作る。
	または、そういうスレッドを１個作っちまうのもいいかもしれない。
	この場合、一つが戦線離脱して他のWiiRemoteで参戦してくる可能性も否めない。
	自分がいなくなった→今、報告されている以外のWiiRemoteを探す必要がある。
*/

#ifndef _CWiimoteManager_h_
#define _CWiimoteManager_h_

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

// 主にタグとかで
#include "CWiimote.h"



//**********************************************************//
//基本的にレジスタ書き込み、メモリ書き込みは一回しか行わない//
//**********************************************************//
//---------------------------------------------------------
//_ CWiimoteManager class
//---------------------------------------------------------
class CWiimoteManager
{

private:
	//wiiリモコンのベンダーIDとプロダクトID
	static const int VendorID = 0x057e;
	static const int ProductID = 0x0306;
	static const int MAX_PLAYER = 4;	//_ wiiを繋げれる数の最大数（根拠はない
	//_ 付け加えたデータ群です。
	static DWORD		_player_num;			//_ 繋がっている数を記録します。
	static CWiimote*	_wiimote[CWiimoteManager::MAX_PLAYER];

	//Application global variables 
//_	DWORD								_ActualBytesRead;
//_	DWORD								_BytesRead;
	HIDP_CAPS							_Capabilities;
//_	DWORD								_cbBytesRead;
	PSP_DEVICE_INTERFACE_DETAIL_DATA	_detailData;			//_ 頑張ればローカルでいける
//_	DWORD								_dwError;
//_	TCHAR								_FeatureReport[256];

	HANDLE								_hDeviceHandle;
	HANDLE								_hEventObject;			//_ CWiimoteで作れたなら…
	HANDLE								_hDevInfo;				//_ ローカルでいける
	HANDLE								_ReadHandle;
	HANDLE								_WriteHandle;

	GUID								_HidGuid;				//_ 多分ローカルでいける
	OVERLAPPED							_HIDOverlapped;			//_ CWiimoteで作れたなら…
	ULONG								_Length;				//_ ローカルでいける
//_	LPOVERLAPPED						_lpOverLap;				
	BOOL								_MyDeviceDetected;
	tstring								_MyDevicePathName;		//_ ローカルでいける 
//_	DWORD								_ReportType;			
	ULONG								_Required;				//_ ローカルでいけそう
//_	tstring								_ValueToDisplay;

	HWND								m_hWnd;//ウインドウハンドル
private:
	VOID RegisterForDeviceNotifications();//デバイスの登録　現在使用していない
	VOID GetReadAndWriteHandles();//ハンドルの取得
	VOID GetDeviceCapabilities();//デバイスの能力の取得
	BOOL FindTheHID();//デバイスの発見
	VOID CloseHandles();//ハンドルを閉じる
public:
	void FindWiimote();
	CWiimote* GetWiimote(int number);
	int GetWiimoteNum(){
		return _player_num;
	}

	CWiimoteManager(HWND hWnd = GetDesktopWindow());
	~CWiimoteManager(void);
};

#endif

#endif