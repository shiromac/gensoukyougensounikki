#pragma once
#include "Wiicondefph.h"

#include "CWiimoteManager.h"
#include "CWiimote.h"

#include "cWiiconStateCustum.h"

#define MAX_NUM_OF_PLAYER 4


//これが定義されているときWiiコンをソリューションに含める。
#ifdef USE_WII_REMOTE_SWITCH







class CWiiconInput
{
public:
	CWiiconInput(HWND WindowHandle);
public:
	virtual ~CWiiconInput(void);

	cWiiconStateCustum* wiicon[MAX_NUM_OF_PLAYER];


	int WiiconNum();

	int ClearInput();

	int UpdateInput();

private:
	int NumofWiicon;


	int UpdateBaseInput(int num);



	CWiimoteManager* pWiiConManager;

	CWiimote* pWiiCon[MAX_NUM_OF_PLAYER];

	
};

inline int CWiiconInput::WiiconNum()
{
	return NumofWiicon;
}

#else

//-------------------
//ダミー
//-------------------

class CWiiconInput
{
public:
	CWiiconInput(HWND WindowHandle);
public:
	virtual ~CWiiconInput(void);

	cWiiconStateCustum* wiicon[MAX_NUM_OF_PLAYER];


	int WiiconNum();

	int ClearInput();

	int UpdateInput();

private:
	int NumofWiicon;


	int UpdateBaseInput(int num);

	
};

inline int CWiiconInput::WiiconNum()
{
	return NumofWiicon;
}

#endif	