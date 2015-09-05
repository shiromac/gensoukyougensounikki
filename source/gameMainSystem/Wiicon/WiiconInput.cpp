#include "../../stdafx.h"

#include "WiiconInput.h"

//これが定義されているときWiiコンをソリューションに含める。
#ifdef USE_WII_REMOTE_SWITCH





CWiiconInput::CWiiconInput(HWND WindowHandle)
{

	int i;

	pWiiConManager = NULL;
	pWiiConManager = new CWiimoteManager(WindowHandle);

	for(i=0;i<MAX_NUM_OF_PLAYER;i++)
	{
		pWiiCon[i] = NULL;
	}

	if(pWiiConManager)
	{
		pWiiConManager->FindWiimote();

		NumofWiicon = pWiiConManager->GetWiimoteNum();


	

		//new inputclass
		
		for(i=0;i<MAX_NUM_OF_PLAYER;i++)
		{
			wiicon[i] = NULL;
		}

		for(i=0;i<NumofWiicon;i++)
		{
			pWiiCon[i] = pWiiConManager->GetWiimote(i);
			if(pWiiCon[i])
			{
				wiicon[i] = new cWiiconStateCustum;

				//初期化
				wiicon[i]->playerNo = i;
				wiicon[i]->InitWiiconState(pWiiCon[i]);

				//pPlayer[i]->enable = true;
				//pWiiCon[i]->WiiFeedbackOn();
				//pWiiCon[i]->WiiFeedbackOff();
			}
			else
			{
				//pPlayer[i]->enable = false;
			}
		}
	}


}


CWiiconInput::~CWiiconInput(void)
{
	int i;
	RELEASE(pWiiConManager);

	for(i=0;i<MAX_NUM_OF_PLAYER;i++)
	{
		RELEASE(wiicon[i]);
	}
}


//毎回呼ばれて情報更新
int CWiiconInput::UpdateInput()
{
	int i;
	for(i=0;i<NumofWiicon;i++)
	{
		if(wiicon[i]!=NULL )//&& pPlayer[i]->enable)
		{
			//(pWiiCon[i])->WiiButtonsAndMotionsAndIR();
			UpdateBaseInput(i);
			(wiicon[i])->ResetAdvanceInput(i);
		}
	}

	return 0;
}

//入力を初期化
int CWiiconInput::ClearInput()
{
	int i;
	for(i=0;i<NumofWiicon;i++)
	{

		if(wiicon[i]) (wiicon[i])->FeedbackOff();

	}

	return 0;
}

//毎回呼ばれて情報更新を各コントローラー別に行う
int CWiiconInput::UpdateBaseInput(int num)
{


	
	(wiicon[num])->IRpointer_center_raw.set( (pWiiCon[num])->m_irCenter.X, 
								(pWiiCon[num])->m_irCenter.Y
								,0,0);
	(wiicon[num])->IRpointer_1_raw.set( (pWiiCon[num])->m_irPosition1.X, 
								(pWiiCon[num])->m_irPosition1.Y
								,0,0);
	(wiicon[num])->IRpointer_2_raw.set( (pWiiCon[num])->m_irPosition2.X, 
								(pWiiCon[num])->m_irPosition2.Y
								,0,0);


	(wiicon[num])->IRpointer_1_exist.process( (pWiiCon[num])->m_irPosition1.enable );
	(wiicon[num])->IRpointer_2_exist.process( (pWiiCon[num])->m_irPosition2.enable );
	
	(wiicon[num])->motion_raw.set( (pWiiCon[num])->m_motions.X, 
								(pWiiCon[num])->m_motions.Y,
								(pWiiCon[num])->m_motions.Z
								,0);

	(wiicon[num])->A.process( (bool)((pWiiCon[num])->m_buttons.A) );
	(wiicon[num])->B.process( (bool)((pWiiCon[num])->m_buttons.B) );
	
	
	(wiicon[num])->Plus.process( (bool)((pWiiCon[num])->m_buttons.Plus) );
	(wiicon[num])->Minus.process( (bool)((pWiiCon[num])->m_buttons.Minus) );
	(wiicon[num])->Home.process( (bool)((pWiiCon[num])->m_buttons.Home) );

	(wiicon[num])->One.process( (bool)((pWiiCon[num])->m_buttons.One) );
	(wiicon[num])->Two.process( (bool)((pWiiCon[num])->m_buttons.Two) );

	(wiicon[num])->Up.process( (bool)((pWiiCon[num])->m_buttons.Up) );
	(wiicon[num])->Down.process( (bool)((pWiiCon[num])->m_buttons.Down) );
	(wiicon[num])->Left.process( (bool)((pWiiCon[num])->m_buttons.Left) );
	(wiicon[num])->Right.process( (bool)((pWiiCon[num])->m_buttons.Right) );
	
	
	
	
	return 0;
}

#else
//-------------------
//ダミー
//-------------------
CWiiconInput::CWiiconInput(HWND WindowHandle)
{
	int i;
	for(i=0;i<MAX_NUM_OF_PLAYER;i++)
	{
		wiicon[i] = NULL;
	}
}


CWiiconInput::~CWiiconInput(void)
{
}

//毎回呼ばれて情報更新
int CWiiconInput::UpdateInput()
{
	return 0;
}

//入力を初期化
int CWiiconInput::ClearInput()
{
	return 0;
}

//毎回呼ばれて情報更新を各コントローラー別に行う
int CWiiconInput::UpdateBaseInput(int num)
{
	return 0;
}

#endif