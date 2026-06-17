#include "stdafx.h"

#include "caMessageWait.h"

#include "debug.h"


caMessageWait::caMessageWait()
{
	End = false;
	preprocessed = 0;
	preendflag = 0;
	anykey = 0;
}

caMessageWait::~caMessageWait()
{
}


int caMessageWait::process(cRenderDevice *pDev)
{

	if(preprocessed == 0)
	{
		//pTex = g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev,_T("interface\\cursor.png"));

		sg_pDungeonSystem->MessageW().Wait();
		preprocessed = 1;
	
	}
	
	//sg_pDungeonSystem->MessageW().VisibleCount = 2;
	if(preendflag == 0)
	{
		if(sg_pDungeonSystem->primaryMenuControlLayerV().empty())
		{
			preendflag = 1;
		}
	}
	else if(preendflag == 1)
	{
		if((g_pPlayerInput()->decision().justOn || (g_pPlayerInput()->decision().onCount > 30 && !g_pPlayerInput()->cancel().on && !(g_pPlayerInput()->cancel().offCount > 15))
		|| (anykey && g_pPlayerInput()->anykey().on))
		&& sg_pDungeonSystem->primaryMenuControlLayerV().empty())
	
		{
			sg_pDungeonSystem->MessageW().unWait();
			//g_GameEnv.m_Input.UpdateInput();
			sg_pDungeonSystem->s_AttackDelay() = 30;
			preendflag = 2;
		}
	}
	else if(preendflag == 2)
	{
		preendflag = 3;
	}
	else if(preendflag == 3)
	{
		End = true;
	}
	else
	{
		OnAssert(_T(__FILE__),__LINE__,_T("preendflag error"));
	}



	
	return true;
}

int caMessageWait::Draw(cRenderDevice *pDev)
{
	return true;
}

bool caMessageWait::isOnScreen()
{
	return true;
}



caPrimaryInterfaceWait::caPrimaryInterfaceWait()
{
	End = false;
	preprocessed = 0;
}

caPrimaryInterfaceWait::~caPrimaryInterfaceWait()
{
}


int caPrimaryInterfaceWait::process(cRenderDevice *pDev)
{

	if(preprocessed == 0)
	{

		preprocessed = 1;
	
	}
	else if(preprocessed == 1)
	{
		if(sg_pDungeonSystem->primaryMenuControlLayerV().empty())
		{
			preprocessed = 2;
		}
	}
	else if(preprocessed == 2)
	{
		End = true;
	}
	else
	{
		OnAssert(_T(__FILE__),__LINE__,_T("preendflag error"));
	}

	return true;
}

int caPrimaryInterfaceWait::Draw(cRenderDevice *pDev)
{
	return true;
}

bool caPrimaryInterfaceWait::isOnScreen()
{
	return true;
}





caMessageClear::caMessageClear()
{
	End = false;
}

caMessageClear::~caMessageClear()
{
}


int caMessageClear::process(cRenderDevice *pDev)
{


	sg_pDungeonSystem->MessageW().textclear(sg_pDungeonSystem->pDevice_D3D);


	End = true;


	
	return true;
}

int caMessageClear::Draw(cRenderDevice *pDev)
{
	return true;
}

bool caMessageClear::isOnScreen()
{
	return true;
}
