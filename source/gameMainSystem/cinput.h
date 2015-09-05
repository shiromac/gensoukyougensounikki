#pragma once
#include "Wiicon/WiiconInput.h"

#include "Wiicon/cWiiconStateCustum.h"

#include "cPlayerInput.h"

#include "patInput.h"



#define MAX_NUM_OF_PLAYER 4

class cInput
{
public:
	cInput(void);
public:
	virtual ~cInput(void);

	//patとkey
	CPatInput* patInputManager;
	//Wiicon


	CWiiconInput* WiiconInputManager;

	int InitInput(HWND WindowHandle);

private:
	int InitInput_WiiconDevice(HWND WindowHandle);//発見したWiiconの数を返す
	int InitInput_PatAndKey(HWND WindowHandle);//パットがあるかどうか返す


	int setPlayerInput();

public:
	int NumOfWiicon;
	int EnableOfPat;

	
	int UpdateInput();

	void ClearInput();

	int m_player_num;



	//コンフィグ用、重い
	int getrawPadInput(int player, int buttom);


	cPlayerInput PlayerInput;//一人用なので一つ

};

