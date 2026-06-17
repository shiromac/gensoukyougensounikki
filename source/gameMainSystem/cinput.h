#pragma once
#include "cPlayerInput.h"

#ifdef __EMSCRIPTEN__
#include <vector>
typedef void* HWND;
#ifndef MAX_BUTTONS
#define MAX_BUTTONS 32
#endif
class CPatInput
{
public:
	std::vector<int> padconfigI2B;
	CPatInput(void) : padconfigI2B(MAX_BUTTONS)
	{
		for(int i = 0; i < MAX_BUTTONS; ++i) padconfigI2B[i] = i;
	}
};
#else
#include "Wiicon/WiiconInput.h"
#include "Wiicon/cWiiconStateCustum.h"
#include "patInput.h"
#endif



#define MAX_NUM_OF_PLAYER 4

struct cInputState
{
	int attack;
	int turn;
	int menu;
	int miniMap;
	int diagon;
	int shot;
	int ue;
	int shita;
	int hidari;
	int migi;
	int dash;
	int smartdash;

	cInputState()
		: attack(0), turn(0), menu(0), miniMap(0), diagon(0), shot(0),
		  ue(0), shita(0), hidari(0), migi(0), dash(0), smartdash(0)
	{
	}
};

class cInput
{
public:
	cInput(void);
public:
	virtual ~cInput(void);

	//pat and key
	CPatInput* patInputManager;
#ifndef __EMSCRIPTEN__
	//Wiicon


	CWiiconInput* WiiconInputManager;
#endif

	int InitInput(HWND WindowHandle);

private:
#ifndef __EMSCRIPTEN__
	int InitInput_WiiconDevice(HWND WindowHandle);//発見したWiiconの数を返す
	int InitInput_PatAndKey(HWND WindowHandle);//パットがあるかどうか返す
#endif

	void readPlatformInput(cInputState& state);
	int applyPlayerInput(const cInputState& state);
	int setPlayerInput();

public:
	int NumOfWiicon;
	int EnableOfPat;

	
	int UpdateInput();
	int setInputState(const cInputState& state);

	void ClearInput();

	int m_player_num;



	//コンフィグ用、重い
	int getrawPadInput(int player, int buttom);


private:
#ifdef __EMSCRIPTEN__
	cInputState platformInputState_;
#endif

public:
	cPlayerInput PlayerInput;//一人用なので一つ

};

