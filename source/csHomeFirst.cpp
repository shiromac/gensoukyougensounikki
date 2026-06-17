#include "stdafx.h"

#include "csHomeFirst.h"
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

#include "cVectorOfControlLayer.h"

using namespace std;


/*
//はじめから
class cCommandStart2 : public cCommand
{
public:
	cCommandStart2(void){caption = g_Lang(_T("はじめから"));};
	cCommandStart2(tstring s){caption = s;};
	virtual ~cCommandStart2(void){};
	virtual int Action(cRenderDevice *pDev);
	{
		sg_pDungeonSystem->pSaveQuest =  pcSaveQuest((cSaveQuest*)NULL);
		g_GameEnv.m_SceneManage->SceneChange(pDev,new csDungeonFirst);
		sg_pDungeonSystem->メニューを閉じる();

	};
};

//おわる
class cCommandEnd2 : public cCommand
{
public:
	cCommandEnd2(void){caption = g_Lang(_T("やめる"));};
	cCommandEnd2(tstring s){caption = s;};
	virtual ~cCommandEnd2(void){};
	virtual int Action(cRenderDevice *pDev);
	{
		sg_pDungeonSystem->メニューを閉じる();
	};
};
*/



csHomeFirst::csHomeFirst(void)
{
	//sg_pDungeonSystem = new cDungeonSystem;
}

csHomeFirst::~csHomeFirst(void)
{
	//RELEASE(sg_pDungeonSystem);
}


bool csHomeFirst::SceneInitialaze(cRenderDevice *pDev)
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



	sg_pDungeonSystem->GotoDungeon(_T("first_Home"));

	return 0;
}

void csHomeFirst::SceneFinalize()
{
	
	//ここで拠点情報保存
	if(sg_pDungeonSystem->拠点フラグ())
	{//拠点から何処かにいく//ていうか終了する
		sg_pDungeonSystem->GameReturnTitle();
	}
	
}

void csHomeFirst::SceneSystemDraw(cRenderDevice *pDev)
{

}

void csHomeFirst::SceneDraw(cRenderDevice *pDev)
{

	

	sg_pDungeonSystem->Draw(pDev);
}

void csHomeFirst::SceneProcess(cRenderDevice *pDev)
{


	if(sg_pDungeonSystem->process(pDev))
	{
		/*
		//sg_pDungeonSystem->Map().visibleForcusX() += g_pPlayerInput()->X * 0.1;

		//sg_pDungeonSystem->Map().visibleForcusY() += g_pPlayerInput()->Y * 0.1;
		static bool sFlag = false;
		if(sg_pDungeonSystem->menuControlLayerV().size() == 0)
		{//何も表示されて無い
			
			if(!sFlag && sg_pDungeonSystem->pPlayerChara()->足元地形()->RoomIndex == -1)
			{//通路
				sFlag = true;

				
				//pcControlLayer pccl;
				//pcSelectWindow pcsw;

				//sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));

				//pccl->Init(pDev);
				//pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

				//pcsw->commandList.push_back(pcCommand(new cCommandStart2 ));
				//pcsw->commandList.push_back(pcCommand(new cCommandEnd2 ));
				//pcsw->Init(pDev, 5, pcsw->commandList.size());
				//pcsw->setLeft(32);
				//pcsw->setTop(32);

				pcsw->playsound_decide();
				
				
				vector<tstring> dungeons;
				dungeons.push_back(_T("preTrial_Dungeon"));
				sg_pDungeonSystem->SelectDungeon(dungeons);
				
			}
			else
			{
				if(sg_pDungeonSystem->pPlayerChara()->足元地形()->RoomIndex != -1)
				{
					sFlag = false;
				}
			}
			
		}
		*/
	}


}
