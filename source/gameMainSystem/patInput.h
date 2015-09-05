#ifndef INPUTH
#define INPUTH


#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <vector>
using namespace std;

//プレイヤー人数の最大値
#define MAX_PLAYERS 4
#define MAX_BUTTONS 32





class CPatInputState {
private:
public:
	bool Up, Down, Left, Right, Button[MAX_BUTTONS],
		Up2, Down2, Left2, Right2;
	float AnalogX, AnalogY, AnalogX2, AnalogY2;
	CPatInputState();
	void Clear();
};




class CPatInput {
private:

	CPatInputState State[MAX_PLAYERS];
	LPDIRECTINPUT8 DI;
	LPDIRECTINPUTDEVICE8 Keyboard;
	void FreeResources();
	vector<LPDIRECTINPUTDEVICE8> JoySticks;//反復子

public:

	CPatInput(HWND hwnd);
	~CPatInput();
	void UpdateState();
	const CPatInputState* GetState(int player) {
		return &State[player];
	}
	const int JoyNum(){return JoySticks.size();}
	void ClearState();

	BOOL CALLBACK EnumJoysticksCallback(
		const DIDEVICEINSTANCE* did);

	//コンフィグ用、重い
	int getrawPadInput(int player, int buttom);

	
	vector<SByte> padconfigI2B;
};




#endif
