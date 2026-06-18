
#include "stdafx.h"
#include "Language.h"

#include "csTitle.h"
#include "csHomeFirst.h"
#include "csDungeonFirst.h"

#include "cVectorOfControlLayer.h"

#include "cDrawingObject.h"
#include "cGameWindow.h"
#include "cSelectWindow.h"
#include "cStringWindow.h"

#include "cCommand.h"

#include <vector>
#include <list>
#include <tstring_ph.h>



#include "cPadConfigWindow.h"
using namespace std;



cVectorOfControlLayer sg_ControlLayerV;


cSaveConfig g_SaveConfig;

class cCommandAbc : public cCommand
{
public:
	cCommandAbc(void){caption = _T("");};
	cCommandAbc(tstring s){caption = s;};
	virtual ~cCommandAbc(void){};
	virtual int Action(cRenderDevice *pDev)
	{
		pcControlLayer pccl;
		//pcSelectWindow pcsw;
		pcStringWindow pcstrw;


		sg_ControlLayerV.push_back(pccl = pcControlLayer(new cControlLayer));

		pccl->Init(pDev);
		pccl->notCancelToPop = true;
		pccl->InputFreeFlag = true;
		//pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));
		pccl->WindowList.push_back(pcstrw = pcStringWindow(new cStringWindow));
		pcstrw->Init(pDev);
		/*
		pcsw->commandList.push_back(pcCommand(new cCommandAbc(_T("A"))));
		pcsw->commandList.push_back(pcCommand(new cCommandAbc(_T("B"))));
		pcsw->commandList.push_back(pcCommand(new cCommandAbc(_T("C"))));
		pcsw->Init(pDev, 3, pcsw->commandList.size());

		pcsw->setLeft(sg_ControlLayerV.size()*64);
		pcsw->setTop(0);
		*/
		return true;
	};
};

//はじめから
class cTitleSelectWindow : public cSelectWindow
{
public:
	virtual void playsound_decide(){}
};

class cCommandStart : public cCommand
{
public:
	cCommandStart(void){caption = g_Lang(_T("はじめる"));};
	cCommandStart(tstring s){caption = s;};
	virtual ~cCommandStart(void){};
	virtual int Action(cRenderDevice *pDev)
	{
		g_GameEnv.m_SceneManage->SceneChange(pDev,new csHomeFirst);
		return true;
	};
};

//つづきから
class cCommandContinue : public cCommand
{
public:
	cCommandContinue(void){caption = g_Lang(_T("つづきから"));};
	cCommandContinue(tstring s){caption = s;};
	virtual ~cCommandContinue(void){};
	virtual int Action(cRenderDevice *pDev)
	{
		
		pcControlLayer pccl;
		pcGameWindow pcgw;

		sg_ControlLayerV.push_back(pccl = pcControlLayer(new cControlLayer));
		
		pccl->Init(pDev);
		pccl->WindowList.push_back(pcgw = pcGameWindow(new cGameWindow));
		
		pcgw->Init(pDev, 6, 1);
		pcgw->Text()= _T("まだむりです");
		pcgw->setLeft(0);
		pcgw->setTop(0);
		return true;
	};
};

//おわる
class cCommandEnd : public cCommand
{
public:
	cCommandEnd(void){caption = g_Lang(_T("おわる"));};
	cCommandEnd(tstring s){caption = s;};
	virtual ~cCommandEnd(void){};
	virtual int Action(cRenderDevice *pDev)
	{
		g_GameEnv.AppEnd = true;
		return true;
	};
};


//パッドコンフィグ
class cCommandPadConfig : public cCommand
{
public:
	cCommandPadConfig(void){caption = g_Lang(_T("パッドコンフィグ"));};
	cCommandPadConfig(tstring s){caption = s;};
	virtual ~cCommandPadConfig(void){};
	virtual int Action(cRenderDevice *pDev)
	{
		
		pcControlLayer pccl;
		pcPadConfigWindow pcpw;

		sg_ControlLayerV.push_back(pccl = pcControlLayer(new cControlLayer));

		pccl->Init(pDev);
		pccl->WindowList.push_back(pcpw = pcPadConfigWindow(new cPadConfigWindow));
		pccl->notCancelToPop = true;

		pcpw->Init(pDev);
		pcpw->CenterX = (SCREEN_X/2);
		pcpw->CenterY = (SCREEN_Y/2);
		return true;
	};
};

//BGM音量コンフィグ
class cCommandBGMVolConfig : public cCommand
{
public:
	cCommandBGMVolConfig(void){caption = g_Lang(_T("BGM音量コンフィグ"));};
	cCommandBGMVolConfig(tstring s){caption = s;};
	virtual ~cCommandBGMVolConfig(void){};
	virtual int Action(cRenderDevice *pDev)
	{
		
		pcControlLayer pccl;
		pcParamaterConfigWindow pcpw;

		sg_ControlLayerV.push_back(pccl = pcControlLayer(new cControlLayer));

		pccl->Init(pDev);
		pccl->WindowList.push_back(pcpw = pcParamaterConfigWindow(new cParamaterConfigWindow));
		pccl->notCancelToPop = true;

		pcpw->Init(pDev,g_GameEnv.m_SoundManager.pBGMVolume(),0,100);
		pcpw->CenterX = (SCREEN_X/2);
		pcpw->CenterY = (SCREEN_Y/2);
		pcpw->Text() = caption;
		return true;
	};
	virtual StyleString shortExplanationText()
	{
		map<tstring,StyleString> val;
		val[_T("Volume")] = setStyle(*g_GameEnv.m_SoundManager.pBGMVolume());
		return g_Lang(_T("BGM音量コンフィグExplanation"),val);
	};
};
//SE音量コンフィグ
class cCommandSEVolConfig : public cCommand
{
public:
	cCommandSEVolConfig(void){caption = g_Lang(_T("SE音量コンフィグ"));};
	cCommandSEVolConfig(tstring s){caption = s;};
	virtual ~cCommandSEVolConfig(void){};
	virtual int Action(cRenderDevice *pDev)
	{
		
		pcControlLayer pccl;
		pcParamaterConfigWindow pcpw;

		sg_ControlLayerV.push_back(pccl = pcControlLayer(new cControlLayer));

		pccl->Init(pDev);
		pccl->WindowList.push_back(pcpw = pcParamaterConfigWindow(new cParamaterConfigWindow));
		pccl->notCancelToPop = true;

		pcpw->Init(pDev,g_GameEnv.m_SoundManager.pSEVolume(),0,100);
		pcpw->CenterX = (SCREEN_X/2);
		pcpw->CenterY = (SCREEN_Y/2);
		pcpw->Text() = caption;
		return true;
	};
	virtual StyleString shortExplanationText()
	{
		map<tstring,StyleString> val;
		val[_T("Volume")] = setStyle(*g_GameEnv.m_SoundManager.pSEVolume());
		return g_Lang(_T("SE音量コンフィグExplanation"),val);
	};
};


//プレイヤーサイン変更
class cCommandSignsConfig : public cCommand
{
public:
	cCommandSignsConfig(void){caption = g_Lang(_T("プレイヤーサイン変更"));};
	cCommandSignsConfig(tstring s){caption = s;};
	virtual ~cCommandSignsConfig(void){};
	virtual int Action(cRenderDevice *pDev)
	{
		
		pcControlLayer pccl;
		pcStringWindow pcstrw;


		sg_ControlLayerV.push_back(pccl = pcControlLayer(new cControlLayer));

		pccl->Init(pDev);
		pccl->notCancelToPop = true;
		pccl->InputFreeFlag = true;
		//pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));
		pccl->WindowList.push_back(pcstrw = pcStringWindow(new cStringWindow));
		pcstrw->Init(pDev);
		pcstrw->targetStr2 = &sg_pDungeonSystem->pSaveData->playersigns;
		return true;
	};
	virtual StyleString shortExplanationText()
	{
		if(sg_pDungeonSystem->pSaveData->playersigns == _T(""))
		{
			sg_pDungeonSystem->pSaveData->playersigns = g_Lang(_T("デフォルトプレイヤー名")).conclete_tstr();
		}
		map<tstring,StyleString> val;
		val[_T("Signs")] = sg_pDungeonSystem->pSaveData->playersigns;
		return g_Lang(_T("プレイヤーサイン変更Explanation"),val);
	}
};

//文字描画の精細さコンフィグ
class cCommandCharEdged : public cCommand
{
public:
	cCommandCharEdged(void){caption = g_Lang(_T("文字描画の精細さ"));};
	cCommandCharEdged(tstring s){caption = s;};
	virtual ~cCommandCharEdged(void){};
	virtual int Action(cRenderDevice *pDev)
	{
		
		pcControlLayer pccl;
		pcParamaterConfigWindow pcpw;

		sg_ControlLayerV.push_back(pccl = pcControlLayer(new cControlLayer));

		pccl->Init(pDev);
		pccl->WindowList.push_back(pcpw = pcParamaterConfigWindow(new cParamaterConfigWindow));
		pccl->notCancelToPop = true;

		pcpw->Init(pDev,&(sg_pDungeonSystem->EV_charEdgedFlag),0,1);
		pcpw->CenterX = (SCREEN_X/2);
		pcpw->CenterY = (SCREEN_Y/2);
		pcpw->Text() = caption;
		return true;
	};
	virtual StyleString shortExplanationText()
	{
		map<tstring,StyleString> val;
		val[_T("Value")] = setStyle(sg_pDungeonSystem->EV_charEdgedFlag);
		return g_Lang(_T("文字描画の精細さExplanation"),val);
	};
};


//コンフィグ
class cCommandConfig : public cCommand
{
public:
	cCommandConfig(void){caption = g_Lang(_T("コンフィグ"));};
	cCommandConfig(tstring s){caption = s;};
	virtual ~cCommandConfig(void){};
	virtual int Action(cRenderDevice *pDev)
	{
		
		pcControlLayer pccl;
		pcSelectWindow pcsw;

		//新規コントロールレイヤー
		sg_ControlLayerV.push_back(pccl = pcControlLayer(new cControlLayer));

		pccl->Init(pDev);

		pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

		pcsw->commandList.push_back(pcCommand(new cCommandPadConfig ));
		pcsw->commandList.push_back(pcCommand(new cCommandBGMVolConfig ));
		pcsw->commandList.push_back(pcCommand(new cCommandSEVolConfig ));
		pcsw->commandList.push_back(pcCommand(new cCommandCharEdged ));
		pcsw->commandList.push_back(pcCommand(new cCommandSignsConfig ));

		pcsw->Init(pDev, 8, pcsw->commandList.size());
		pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(1));
		pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(1));

		return true;
	};
};


csTitle::csTitle(void)
{
	sg_ControlLayerV.clear();


}

csTitle::~csTitle(void)
{
	sg_ControlLayerV.clear();

	//RELEASE(sg_pDungeonSystem);
}


bool csTitle::SceneInitialaze(cRenderDevice *pDev)
{
	cRenderTexture* pTexture;
	//pTexture = g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev,_T("effect\\magiccircle.png"));
	pTexture = g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev,_T("interface\\title.png"));

	sg_wall.setTexture(pTexture,800,600);
	sg_wall.m_TexRange.setLTRB(0,0,1,1);
	sg_wall3.setTexture(pTexture,800,600);
	sg_wall3.m_TexRange.setLTRB(0,0,1,1);

	pTexture = g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev,_T("interface\\タイトル素材.png"));

	sg_wall2.setTexture(pTexture,512,512);
	sg_wall2.m_TexRange.setLTRB(0,0,0.5,1);
	sg_wall2.CenterX = 400+8;
	sg_wall2.CenterY = 300-3;
	sg_wall2.Width = 256;
	sg_wall2.Height = 512;

	pTexture = g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev,_T("interface\\titleExplanation.png"));

	sg_button.setTexture(pTexture,256,128);
	sg_button.m_TexRange.setLTRB(0,0,1,1);
	sg_button.CenterX = 400+8;
	sg_button.CenterY = 300+250;
	sg_button.Width = 256;
	sg_button.Height = 128;

	g_GameEnv.m_SoundManager.getplayBGMFromFile(_T("title.wav"), _T("title_roop.wav"));


	return true;
}

void csTitle::SceneFinalize()
{
	g_SaveConfig.save();

}

void csTitle::SceneSystemDraw(cRenderDevice *pDev)
{

}

void csTitle::SceneDraw(cRenderDevice *pDev)
{


	int i;

	static double roop = 0;
	static int noize = 0;
	static bool control_view = false;
	static double button_alpha = 0.0;
	static int button_count = 0;

	sg_wall.setLTRB(0,0,800,600);
	sg_wall.m_color.ARGB(255,50,200,50);
	//sg_wall.Rotation+=0.5;
	//sg_wall.Draw(pDev);
	//sg_wall.Rotation+=0.5;

	sg_wall3.setLTRB(0,0,800,600);
	sg_wall3.m_color.ARGB(255,200,50,200);
	sg_wall3.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;


	//if(noize < 0)
	//{
		double a = 1*(sin(roop));
		a = max(0,a);
		//sg_wall.CenterX += a;
		//sg_wall.CenterY += rand()%8 - 4;
		sg_wall3.ScaleX = 1 + a*0.008;
		//sg_wall3.CenterY += rand()%8 - 4;
		//noize += rand()%128;
	//}
	//--noize;
	sg_wall.Draw(pDev);
	sg_wall3.Draw(pDev);

	roop += 0.02;
	sg_wall2.m_color.alpha = 64*sin(roop)+64;
	sg_wall2.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;
	sg_wall2.Draw(pDev);

	if(!control_view)
	{
		button_count++;
	}
	else
	{
		button_count = 0;
		button_alpha = 0;
	}

	if(button_count > 240)
	{
		button_alpha += 1;
		sg_button.m_color.alpha = button_alpha;
		sg_button.Draw(pDev);
	}

	control_view = false;
	
	for(i=0;i<sg_ControlLayerV.size();i++)
	{
		sg_ControlLayerV[i]->Draw(pDev);
		control_view = true;
	}
	
}

void csTitle::SceneProcess(cRenderDevice *pDev)
{



	if(sg_ControlLayerV.size() == 0)
	{//何も表示されて無い
		if(g_pPlayerInput()->decision().justOn)
		{//決定を押された
	

			pcControlLayer pccl;
			pcSelectWindow pcsw;

			sg_ControlLayerV.push_back(pccl = pcControlLayer(new cControlLayer));

			pccl->Init(pDev);
			pccl->WindowList.push_back(pcsw = pcSelectWindow(new cTitleSelectWindow));

			pcsw->commandList.push_back(pcCommand(new cCommandStart ));
			//pcsw->commandList.push_back(pcCommand(new cCommandContinue ));
			pcsw->commandList.push_back(pcCommand(new cCommandConfig));
			pcsw->commandList.push_back(pcCommand(new cCommandEnd ));
			pcsw->Init(pDev, 5, pcsw->commandList.size());
			pcsw->setLeft(32);
			pcsw->setTop(32);

			pcsw->playsound_decide();
		}
	}
	else
	{

	
		sg_ControlLayerV.process(pDev);
		
		g_GameEnv.m_SoundManager.resetVolume();//コンフィグ即反映
	
	}
}