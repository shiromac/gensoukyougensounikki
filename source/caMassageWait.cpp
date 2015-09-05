#include "stdafx.h"

#include "caMassageWait.h"



caMassageWait::caMassageWait()
{
	End = false;
	preprocessed = 0;
}

caMassageWait::~caMassageWait()
{
}


int caMassageWait::process(IDirect3DDevice9 *pDev)
{

	if(preprocessed == 0)
	{
		//pTex = g_GameEnv.m_Resourse->getTextureFromFile(pDev,TEXT("interface\\cursor.png"));

		sg_pDungeonSystem->Massage.Wait();
		preprocessed = 1;
	
	}
	
	sg_pDungeonSystem->Massage.VisibleCount = 2;


	if(g_pPlayerInput()->decision.justOn)
	{
		sg_pDungeonSystem->Massage.unWait();
		g_GameEnv.m_Input.UpdateInput();
		End = true;
	}


	
	
	return true;
}

int caMassageWait::Draw(IDirect3DDevice9 *pDev)
{
	return true;
}

bool caMassageWait::isOnScreen()
{
	return true;
}
