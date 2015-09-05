
#include "../stdafx.h"
#include "Wiicon/CWiimoteManager.h"
#include "Wiicon/CWiimote.h"


cInput::cInput(void)
{

}

cInput::~cInput(void)
{
	RELEASE(WiiconInputManager);
	RELEASE(patInputManager);
}

//毎回呼ばれて情報更新
int cInput::UpdateInput()
{
	patInputManager->UpdateState();
	WiiconInputManager->UpdateInput();

	setPlayerInput();
	return 0;
}


//リフレッシュ
void cInput::ClearInput()
{


	WiiconInputManager->ClearInput();

	patInputManager->ClearState();

}

//初期化。始めに一度だけ呼ばれる。
int cInput::InitInput(HWND WindowHandle)
{

	NumOfWiicon = InitInput_WiiconDevice(WindowHandle);

	EnableOfPat = InitInput_PatAndKey(WindowHandle);




	return true;
}

int cInput::InitInput_PatAndKey(HWND WindowHandle)
{
	_TCHAR Temp[64] = _T("");

	patInputManager = new CPatInput(WindowHandle);

	patInputManager->UpdateState();

	wsprintf( Temp, _T("パッドを%dつ発見。\n"), patInputManager->JoyNum());
	OutputDebugString( Temp);

	if(patInputManager->JoyNum() == 0) return false;
	return true;
}

int cInput::InitInput_WiiconDevice(HWND WindowHandle)
{

	WiiconInputManager = new CWiiconInput(WindowHandle);

	
	//これが定義されているときWiiコンをソリューションに含める。
#ifdef USE_WII_REMOTE_SWITCH
	TCHAR Temp[64] = _T("");

	_stprintf( Temp, _T("Wiiコンを%dつ発見。\n"), WiiconInputManager->WiiconNum());
	OutputDebugString( Temp);

	return WiiconInputManager->WiiconNum();
#else
	return 0;
#endif
}

int cInput::setPlayerInput()
{
	int flag;
	
	
	//攻撃
	flag = patInputManager->GetState(0)->Button[0]
		|((WiiconInputManager->wiicon[0])?
		(WiiconInputManager->wiicon[0])->Two.on : 0);
	PlayerInput.setattack().process(flag);


	//振り返り
	flag = patInputManager->GetState(0)->Button[2]
		|((WiiconInputManager->wiicon[0])?
		(WiiconInputManager->wiicon[0])->One.on : 0);
	PlayerInput.setturn().process(flag);

	
	//メニュー
	flag = patInputManager->GetState(0)->Button[3]
		|((WiiconInputManager->wiicon[0])?
		(WiiconInputManager->wiicon[0])->A.on : 0);
	PlayerInput.setmenu().process(flag);

	//ミニマップ
	flag = patInputManager->GetState(0)->Button[6]
		|((WiiconInputManager->wiicon[0])?
		(WiiconInputManager->wiicon[0])->Minus.on : 0);
	PlayerInput.setminiMap().process(flag);


	//斜め
	flag = patInputManager->GetState(0)->Button[4]
		|((WiiconInputManager->wiicon[0])?
		(WiiconInputManager->wiicon[0])->B.on : 0);
	PlayerInput.setdiagon().process(flag);




	//矢飛ばし
	flag = patInputManager->GetState(0)->Button[5]
		|((WiiconInputManager->wiicon[0])?
		(WiiconInputManager->wiicon[0])->Plus.on : 0);
	PlayerInput.setshot().process(flag);



	//ue()
	flag = patInputManager->GetState(0)->Up
		| patInputManager->GetState(0)->Up2
		|((WiiconInputManager->wiicon[0])?
		(WiiconInputManager->wiicon[0])->Right.on : 0);

	PlayerInput.setue().process(flag);


	//shita()
	flag = patInputManager->GetState(0)->Down
		| patInputManager->GetState(0)->Down2
		|((WiiconInputManager->wiicon[0])?
		(WiiconInputManager->wiicon[0])->Left.on : 0);

	PlayerInput.setshita().process(flag);


	//hidari()
	flag = patInputManager->GetState(0)->Left
		| patInputManager->GetState(0)->Left2
		|((WiiconInputManager->wiicon[0])?
		(WiiconInputManager->wiicon[0])->Up.on : 0);

	PlayerInput.sethidari().process(flag);


	//migi()
	flag = patInputManager->GetState(0)->Right
		| patInputManager->GetState(0)->Right2
		|((WiiconInputManager->wiicon[0])?
		(WiiconInputManager->wiicon[0])->Down.on : 0);

	PlayerInput.setmigi().process(flag);


	//X
	PlayerInput.X = PlayerInput.migi().on - PlayerInput.hidari().on;


	//Y
	PlayerInput.Y = PlayerInput.shita().on - PlayerInput.ue().on;

	//斜め縛り
	if(PlayerInput.diagon().on)
	{
		if(abs(PlayerInput.X)+abs(PlayerInput.Y) != 2)
		{//斜めじゃない
			PlayerInput.X = 0;
			PlayerInput.Y = 0;
		}
	}



	//ダッシュ//仮
	/*
	flag = (PlayerInput.migi().on ||
		PlayerInput.hidari().on ||
		PlayerInput.ue().on ||
		PlayerInput.shita().on ) &&
		PlayerInput.cancel().justOn;
		*/
	flag = patInputManager->GetState(0)->Button[1];
	PlayerInput.setdash().process(flag);

	flag = patInputManager->GetState(0)->Button[7];
	PlayerInput.setsmartdash().process(flag);

	return true;
}

int cInput::getrawPadInput(int player, int buttom)
{
	if(MAX_PLAYERS <= player || MAX_BUTTONS <= buttom) return 0;
	return patInputManager->getrawPadInput(player,buttom);
}