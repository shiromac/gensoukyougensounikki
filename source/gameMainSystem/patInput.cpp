#include "../stdafx.h"


#include "patInput.h"

#define JOYAXIS_RANGE 1000
#define JOYAXIS_MARGIN 200




//==============================================================
// CInputStateクラス

// コンストラクタ
CPatInputState::CPatInputState() {
	Clear();
}

// 状態のクリア
void CPatInputState::Clear() {
	Up=Down=Left=Right=false;
	Up2=Down2=Left2=Right2=false;
	for (int i=0; i<MAX_BUTTONS; i++) Button[i]=false;
	AnalogX=AnalogY=0;
	AnalogX2=AnalogY2=0;
}




//==============================================================
// 列挙用のコールバック関数
BOOL CALLBACK EnumJoysticksCallbackWrapper(const DIDEVICEINSTANCE* did, VOID* ref)
{
	CPatInput* input=(CPatInput*)ref;
	return input->EnumJoysticksCallback(did);
}

// 
BOOL CALLBACK CPatInput::EnumJoysticksCallback(const DIDEVICEINSTANCE* did) 
{
	LPDIRECTINPUTDEVICE8 joystick=NULL;
	if (!FAILED(DI->CreateDevice(did->guidInstance, &joystick, NULL))) {//Creates and initializes an instance of a device based on a given GUID, and obtains an IDirectInputDevice8 interface. 
		JoySticks.push_back(joystick);//The element added to the end of the vector
	}
	return DIENUM_CONTINUE;
}

// 軸の範囲を設定する
BOOL CALLBACK EnumObjectsCallbackWrapper(
	const DIDEVICEOBJECTINSTANCE* dido, VOID* ref
) {
	LPDIRECTINPUTDEVICE8 joystick=(LPDIRECTINPUTDEVICE8)ref;
	if (dido->dwType & DIDFT_AXIS) {
		DIPROPRANGE dip;
		dip.diph.dwSize=sizeof(DIPROPRANGE); 
		dip.diph.dwHeaderSize=sizeof(DIPROPHEADER); 
		dip.diph.dwHow=DIPH_BYID; 
		dip.diph.dwObj=dido->dwType;
		dip.lMin=-JOYAXIS_RANGE; 
		dip.lMax=JOYAXIS_RANGE;
		joystick->SetProperty(DIPROP_RANGE, &dip.diph); 
	}	
	return DIENUM_CONTINUE;
}




//==============================================================
// コンストラクタ
CPatInput::CPatInput(HWND window) {
	// DirectInputインタフェースの作成
	DI=NULL;
	//Creates a Microsoft DirectInput object and returns an IDirectInput8 or later interface.
	if (FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DI, NULL))) 
		return;

	// キーボードデバイスの取得と初期化
	Keyboard=NULL;
	if (FAILED(DI->CreateDevice(GUID_SysKeyboard, &Keyboard, NULL)) ||
		FAILED(Keyboard->SetDataFormat(&c_dfDIKeyboard)) ||//デバイスのデータ形式を設定
		FAILED(Keyboard->SetCooperativeLevel(window, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))//フォアグラウンド非排他アクセス権
	) { FreeResources(); return; }//失敗したら開放

	// ジョイスティックデバイスの取得と初期化
	if (FAILED(DI->EnumDevices(DI8DEVCLASS_GAMECTRL, 
			EnumJoysticksCallbackWrapper, this, DIEDFL_ATTACHEDONLY))//利用可能なデバイスを列挙
	) { FreeResources(); return; }//失敗したら開放
	for (int i=0, n=JoySticks.size(); i<n; i++) {//ジョイスティックの数だけ繰り返す
		if (FAILED(JoySticks[i]->SetDataFormat(&c_dfDIJoystick2)) ||
			FAILED(JoySticks[i]->SetCooperativeLevel(window, DISCL_FOREGROUND | DISCL_EXCLUSIVE)) ||
			FAILED(JoySticks[i]->EnumObjects(EnumObjectsCallbackWrapper, JoySticks[i], DIDFT_ALL))
		) { FreeResources(); return; }//失敗したら開放
	}

	int i;
	for(i=0;i<MAX_BUTTONS;i++) padconfigI2B.push_back(i);
}




//==============================================================
// デストラクタ
CPatInput::~CPatInput() {
	FreeResources();
}




//==============================================================
// 資源の解放
void CPatInput::FreeResources() {
	if (Keyboard) {
		Keyboard->Unacquire();
		Keyboard->Release();
	}
	for (int i=0, n=JoySticks.size(); i<n; i++) {
		JoySticks[i]->Unacquire();
		JoySticks[i]->Release();
	}
	if (DI) DI->Release();
}




//==============================================================
// 入力状態を最新にする
#define KEYDOWN(key) ((key_state[key]&0x80)!=0)
#define BTNDOWN(btn) ((joy_state.rgbButtons[btn]&0x80)!=0)

double culcurateDegree(const int inputX, const int inputY)
{
	double result_d = 0, r_cos;
	double length = sqrt((double)inputX*inputX + inputY*inputY);
	if(length != 0)
	{
		double unit_x = inputX/length, unit_y = inputY/length;
		r_cos = unit_x;
		result_d = 180*acos(r_cos)/V_PI;
		if(inputY<0) {
			result_d = 360 - result_d;
		}
	}
	else
	{
		result_d = -360;
	}
	return result_d;
}

void analogPadStickInput(const int inputX, const int inputY, int* outputX, int* outputY) {
	
	double length = sqrt((double)inputX*inputX + inputY*inputY);
	if(length < 600) {
		*outputX = 0;
		*outputY = 0;
		return;
	}
	double degree = culcurateDegree(inputX, inputY);
	double degree_unit = 360.0/16.0;
	if (degree < 1*degree_unit || degree > 15*degree_unit) {
		*outputX = 0;
		*outputY = 1;
		return;
	}
	else if (degree < 3*degree_unit) {
		*outputX = 1;
		*outputY = 1;
		return;
	}
	else if (degree < 5*degree_unit) {
		*outputX = 1;
		*outputY = 0;
		return;
	}
	else if (degree < 7*degree_unit) {
		*outputX = 1;
		*outputY = -1;
		return;
	}
	else if (degree < 9*degree_unit) {
		*outputX = 0;
		*outputY = -1;
		return;
	}
	else if (degree < 11*degree_unit) {
		*outputX = -1;
		*outputY = -1;
		return;
	}
	else if (degree < 13*degree_unit) {
		*outputX = -1;
		*outputY = 0;
		return;
	}
	else {
		*outputX = -1;
		*outputY = 1;
		return;
	}
}

void CPatInput::UpdateState() {
	ClearState();
		
	// キーボード
	if (Keyboard) {
		UByte key_state[256];
		if (!FAILED(Keyboard->Acquire()) &&//Obtains access to the input device.
			!FAILED(Keyboard->GetDeviceState(sizeof(key_state), key_state))) //デバイスから直接データを取得
		{
			CPatInputState& s=State[0];

			s.Up|=KEYDOWN(DIK_UP);
			s.Down|=KEYDOWN(DIK_DOWN);
			s.Left|=KEYDOWN(DIK_LEFT);
			s.Right|=KEYDOWN(DIK_RIGHT);
			s.Button[0]|=KEYDOWN(DIK_Z);
			s.Button[1]|=KEYDOWN(DIK_X);
			s.Button[2]|=KEYDOWN(DIK_C);
			s.Button[3]|=KEYDOWN(DIK_V) | KEYDOWN(DIK_A) | KEYDOWN(DIK_RETURN);
			s.Button[4]|=KEYDOWN(DIK_LSHIFT);
			s.Button[5]|=KEYDOWN(DIK_S);
			s.Button[6]|=KEYDOWN(DIK_SPACE);
			s.Button[7]|=KEYDOWN(DIK_D);
			
			/*s.Button[7]|=KEYDOWN(DIK_F);
			s.Button[8]|=KEYDOWN(DIK_Q);
			s.Button[9]|=KEYDOWN(DIK_W);
			s.Button[10]|=KEYDOWN(DIK_E);
			s.Button[11]|=KEYDOWN(DIK_R);
			s.Button[12]|=KEYDOWN(DIK_SPACE);
			s.Button[13]|=KEYDOWN(DIKEYBOARD_LSHIFT);
			*/
			s.AnalogY+=(s.Up? -1 : (s.Down? 1 : 0));
			s.AnalogX+=(s.Left? -1 : (s.Right? 1 : 0));

			s.Up2|=KEYDOWN(DIK_I);
			s.Down2|=KEYDOWN(DIK_K);
			s.Left2|=KEYDOWN(DIK_J);
			s.Right2|=KEYDOWN(DIK_L);
			s.AnalogY2+=(s.Up2? -1 : (s.Down2? 1 : 0));
			s.AnalogX2+=(s.Left2? -1 : (s.Right2? 1 : 0));
		}
	}

	// ジョイスティック
	for (int i=0, n=JoySticks.size(); i<n; i++) {
		DIJOYSTATE2 joy_state;
		JoySticks[i]->Poll();
		if (!FAILED(JoySticks[i]->Acquire()) &&
			!FAILED(JoySticks[i]->GetDeviceState(sizeof(joy_state), &joy_state))) 
		{
			CPatInputState& s=State[i];
			int outputX, outputY;
			analogPadStickInput(joy_state.lY, joy_state.lX, &outputX, &outputY);
			s.Up|=(outputY < 0);
			s.Down|=(outputY > 0);
			s.Left|=(outputX < 0);
			s.Right|=(outputX > 0);
			/*
			s.Up|=(joy_state.lY<-JOYAXIS_MARGIN);
			s.Down|=(joy_state.lY>JOYAXIS_MARGIN);
			s.Left|=(joy_state.lX<-JOYAXIS_MARGIN);
			s.Right|=(joy_state.lX>JOYAXIS_MARGIN);
			*/
			s.Button[0]|=((joy_state.rgbButtons[padconfigI2B[0]]&0x80)!=0);
			s.Button[1]|=((joy_state.rgbButtons[padconfigI2B[1]]&0x80)!=0);
			s.Button[2]|=((joy_state.rgbButtons[padconfigI2B[2]]&0x80)!=0);
			s.Button[3]|=((joy_state.rgbButtons[padconfigI2B[3]]&0x80)!=0);
			s.Button[4]|=((joy_state.rgbButtons[padconfigI2B[4]]&0x80)!=0);
			s.Button[5]|=((joy_state.rgbButtons[padconfigI2B[5]]&0x80)!=0);
			s.Button[6]|=((joy_state.rgbButtons[padconfigI2B[6]]&0x80)!=0);
			s.Button[7]|=((joy_state.rgbButtons[padconfigI2B[7]]&0x80)!=0);

			// Y方向のアナログ入力
			int jy=joy_state.lY;
			if (jy<=-JOYAXIS_RANGE+JOYAXIS_MARGIN) s.AnalogY+=-1;
			else if (-JOYAXIS_MARGIN<=jy && jy<=JOYAXIS_MARGIN) s.AnalogY+=0;
			else if (JOYAXIS_RANGE-JOYAXIS_MARGIN<=jy) s.AnalogY+=1; 
			else if (jy<0) s.AnalogY+=(float)(jy+JOYAXIS_MARGIN)/(JOYAXIS_RANGE-JOYAXIS_MARGIN*2);
			else s.AnalogY+=(float)(jy-JOYAXIS_MARGIN)/(JOYAXIS_RANGE-JOYAXIS_MARGIN*2);

			// X方向のアナログ入力
			int jx=joy_state.lX;
			if (jx<=-JOYAXIS_RANGE+JOYAXIS_MARGIN) s.AnalogX+=-1; else
			if (-JOYAXIS_MARGIN<=jx && jx<=JOYAXIS_MARGIN) s.AnalogX+=0; else
			if (JOYAXIS_RANGE-JOYAXIS_MARGIN<=jx) s.AnalogX+=1;
			else if (jx<0) s.AnalogX+=(float)(jx+JOYAXIS_MARGIN)/(JOYAXIS_RANGE-JOYAXIS_MARGIN*2);
			else s.AnalogX+=(float)(jx-JOYAXIS_MARGIN)/(JOYAXIS_RANGE-JOYAXIS_MARGIN*2);
		}
	}
	
}

int CPatInput::getrawPadInput(int player, int buttom)
{
	int i = player;
	CPatInputState s;

	if( i >= JoySticks.size()) return 0;
	// ジョイスティック

	DIJOYSTATE2 joy_state;
	JoySticks[i]->Poll();
	if (!FAILED(JoySticks[i]->Acquire()) &&
		!FAILED(JoySticks[i]->GetDeviceState(sizeof(joy_state), &joy_state))) 
	{
		

		s.Up|=(joy_state.lY<-JOYAXIS_MARGIN);
		s.Down|=(joy_state.lY>JOYAXIS_MARGIN);
		s.Left|=(joy_state.lX<-JOYAXIS_MARGIN);
		s.Right|=(joy_state.lX>JOYAXIS_MARGIN);
		
		int k;
		for(k=0;k<MAX_BUTTONS;k++)
			s.Button[k] = ((joy_state.rgbButtons[k]&0x80)!=0);

		// Y方向のアナログ入力
		int jy=joy_state.lY;
		if (jy<=-JOYAXIS_RANGE+JOYAXIS_MARGIN) s.AnalogY+=-1;
		else if (-JOYAXIS_MARGIN<=jy && jy<=JOYAXIS_MARGIN) s.AnalogY+=0;
		else if (JOYAXIS_RANGE-JOYAXIS_MARGIN<=jy) s.AnalogY+=1; 
		else if (jy<0) s.AnalogY+=(float)(jy+JOYAXIS_MARGIN)/(JOYAXIS_RANGE-JOYAXIS_MARGIN*2);
		else s.AnalogY+=(float)(jy-JOYAXIS_MARGIN)/(JOYAXIS_RANGE-JOYAXIS_MARGIN*2);

		// X方向のアナログ入力
		int jx=joy_state.lX;
		if (jx<=-JOYAXIS_RANGE+JOYAXIS_MARGIN) s.AnalogX+=-1; else
		if (-JOYAXIS_MARGIN<=jx && jx<=JOYAXIS_MARGIN) s.AnalogX+=0; else
		if (JOYAXIS_RANGE-JOYAXIS_MARGIN<=jx) s.AnalogX+=1;
		else if (jx<0) s.AnalogX+=(float)(jx+JOYAXIS_MARGIN)/(JOYAXIS_RANGE-JOYAXIS_MARGIN*2);
		else s.AnalogX+=(float)(jx-JOYAXIS_MARGIN)/(JOYAXIS_RANGE-JOYAXIS_MARGIN*2);
	}
	
	return s.Button[buttom];
}


//==============================================================
// 何も入力されていない状態にする
void CPatInput::ClearState() {
	for (int i=0; i<MAX_PLAYERS; i++) {
		State[i].Clear();
	}
}


