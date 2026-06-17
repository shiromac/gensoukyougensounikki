#include "stdafx.h"

#include "csDungeonFirst.h"

#include "csTitle.h"
#include "stdafx.h"

#include "cDrawingObject.h"
#include "cGameWindow.h"
#include "cSelectWindow.h"
#include "cControlLayer.h"
#include "cCommand.h"

#include "cDungeonSystem.h"
#include "cSaveStore.h"

#include <vector>
#include <list>
#include <tstring_ph.h>
#include <time.h>

using namespace std;






csDungeonFirst::csDungeonFirst(void)
{
	//sg_pDungeonSystem = new cDungeonSystem;
}

csDungeonFirst::~csDungeonFirst(void)
{
	//RELEASE(sg_pDungeonSystem);
}


bool csDungeonFirst::SceneInitialaze(cRenderDevice *pDev)
{
	//sg_pDungeonSystem->InitSystem(pDev);
	{
		//sg_pDungeonSystem->Map().m_pTexture_Mapchip = 
		//	g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev, _T("map\\紅魔館セット.png"));
		
		sg_pDungeonSystem->pFrameTexture = 
			g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev, _T("interface\\frame_default.png"));
	}
	//sg_pDungeonSystem->InitDungeon(pDev);


	//デバッグ用
	//sg_pDungeonSystem->Map().Allmapping();


	pcSaveQuest pquest = sg_pDungeonSystem->pSaveQuest;
	if(pquest == NULL || !pquest->enable())
	{
		pquest = pcSaveQuest((cSaveQuest*)NULL);
		sg_pDungeonSystem->GotoDungeon(_T("preTrial_Dungeon"));
	}
	else
	{
		sg_pDungeonSystem->ContinueDungeon(pquest);
	}
	return 0;
}

void csDungeonFirst::SceneFinalize()
{
	
}

void csDungeonFirst::SceneSystemDraw(cRenderDevice *pDev)
{

}

void csDungeonFirst::SceneDraw(cRenderDevice *pDev)
{

	

	sg_pDungeonSystem->Draw(pDev);
}

void csDungeonFirst::SceneProcess(cRenderDevice *pDev)
{
	if(sg_pDungeonSystem->process(pDev))
	{
	
		//sg_pDungeonSystem->Map().visibleForcusX() += g_pPlayerInput()->X * 0.1;

		//sg_pDungeonSystem->Map().visibleForcusY() += g_pPlayerInput()->Y * 0.1;


	}


}
