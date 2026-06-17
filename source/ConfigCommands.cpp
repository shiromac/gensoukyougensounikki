

#include "stdafx.h"
#include "ConfigCommands.h"

#include "cDrawingObject.h"
#include "cGameWindow.h"
#include "cSelectWindow.h"
#include "cStringWindow.h"
#include "cPadConfigWindow.h"

//パッドコンフィグ
int cCommandConfigPadConfig::Action(cRenderDevice *pDev)
{
	MenuPopupConfigPadConfig(sg_pDungeonSystem->menuControlLayerV(), pDev);
	return true;
}
void MenuPopupConfigPadConfig(cVectorOfControlLayer& vectorOfControlLayer, cRenderDevice *pDev)
{	
	pcControlLayer pccl;
	pcPadConfigWindow pcpw;

	vectorOfControlLayer.push_back(pccl = pcControlLayer(new cControlLayer));

	pccl->Init(pDev);
	pccl->WindowList.push_back(pcpw = pcPadConfigWindow(new cPadConfigWindow));
	pccl->notCancelToPop = true;
	pccl->InputFreeFlag = TRUE;

	pcpw->Init(pDev);
	pcpw->CenterX = (SCREEN_X/2);
	pcpw->CenterY = (SCREEN_Y/2);
}

//BGM音量コンフィグ
int cCommandConfigBGMVolConfig::Action(cRenderDevice *pDev)
{
	MenuPopupConfigBGMVolConfig(sg_pDungeonSystem->menuControlLayerV(), pDev, caption);
	return true;
}
void MenuPopupConfigBGMVolConfig_DidChangeValueCallBack()
{
	g_GameEnv.m_SoundManager.resetVolume();
}
void MenuPopupConfigBGMVolConfig(cVectorOfControlLayer& vectorOfControlLayer, cRenderDevice *pDev, StyleString& caption)
{
	pcControlLayer pccl;
	pcParamaterConfigWindow pcpw;

	vectorOfControlLayer.push_back(pccl = pcControlLayer(new cControlLayer));

	pccl->Init(pDev);
	pccl->WindowList.push_back(pcpw = pcParamaterConfigWindow(new cParamaterConfigWindow));
	pccl->notCancelToPop = true;

	pcpw->Init(pDev,g_GameEnv.m_SoundManager.pBGMVolume(),0,100);
	pcpw->CenterX = (SCREEN_X/2);
	pcpw->CenterY = (SCREEN_Y/2);
	pcpw->Text() = caption;
	pcpw->DidChangeValue = MenuPopupConfigBGMVolConfig_DidChangeValueCallBack;
}

StyleString cCommandConfigBGMVolConfig::shortExplanationText()
{
	map<tstring,StyleString> val;
	val[_T("Volume")] = setStyle(*g_GameEnv.m_SoundManager.pBGMVolume());
	return g_Lang(_T("BGM音量コンフィグExplanation"),val);
}
//SE音量コンフィグ
int cCommandConfigSEVolConfig::Action(cRenderDevice *pDev)
{
	MenuPopupConfigSEVolConfig(sg_pDungeonSystem->menuControlLayerV(), pDev, caption);
	return true;
}
void MenuPopupConfigSEVolConfig(cVectorOfControlLayer& vectorOfControlLayer, cRenderDevice *pDev, StyleString& caption)
{
	pcControlLayer pccl;
	pcParamaterConfigWindow pcpw;

	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));

	pccl->Init(pDev);
	pccl->WindowList.push_back(pcpw = pcParamaterConfigWindow(new cParamaterConfigWindow));
	pccl->notCancelToPop = true;

	pcpw->Init(pDev,g_GameEnv.m_SoundManager.pSEVolume(),0,100);
	pcpw->CenterX = (SCREEN_X/2);
	pcpw->CenterY = (SCREEN_Y/2);
	pcpw->Text() = caption;
}
StyleString cCommandConfigSEVolConfig::shortExplanationText()
{
	map<tstring,StyleString> val;
	val[_T("Volume")] = setStyle(*g_GameEnv.m_SoundManager.pSEVolume());
	return g_Lang(_T("SE音量コンフィグExplanation"),val);
}

//プレイヤーサイン変更
int cCommandConfigSignsConfig::Action(cRenderDevice *pDev)
{
	MenuPopupConfigSignsConfig(sg_pDungeonSystem->menuControlLayerV(), pDev, caption);
	return true;
}
void MenuPopupConfigSignsConfig(cVectorOfControlLayer& vectorOfControlLayer, cRenderDevice *pDev, StyleString& caption)
{
	pcControlLayer pccl;
	pcStringWindow pcstrw;

	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));

	pccl->Init(pDev);
	pccl->notCancelToPop = true;
	pccl->InputFreeFlag = true;
	//pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));
	pccl->WindowList.push_back(pcstrw = pcStringWindow(new cStringWindow));
	pcstrw->Init(pDev);
	pcstrw->targetStr2 = &sg_pDungeonSystem->pSaveData->playersigns;
}
StyleString cCommandConfigSignsConfig::shortExplanationText()
{
	if(sg_pDungeonSystem->pSaveData->playersigns == _T(""))
	{
		sg_pDungeonSystem->pSaveData->playersigns = g_Lang(_T("デフォルトプレイヤー名")).conclete_tstr();
	}
	map<tstring,StyleString> val;
	val[_T("Signs")] = sg_pDungeonSystem->pSaveData->playersigns;
	return g_Lang(_T("プレイヤーサイン変更Explanation"),val);
}

//文字描画の精細さコンフィグ
int cCommandConfigCharEdged::Action(cRenderDevice *pDev)
{
	MenuPopupConfigCharEdged(sg_pDungeonSystem->menuControlLayerV(), pDev, caption);
	return true;
}
void MenuPopupConfigCharEdged(cVectorOfControlLayer& vectorOfControlLayer, cRenderDevice *pDev, StyleString& caption)
{
	pcControlLayer pccl;
	pcParamaterConfigWindow pcpw;

	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));

	pccl->Init(pDev);
	pccl->WindowList.push_back(pcpw = pcParamaterConfigWindow(new cParamaterConfigWindow));
	pccl->notCancelToPop = true;

	pcpw->Init(pDev,&(sg_pDungeonSystem->EV_charEdgedFlag),0,1);
	pcpw->CenterX = (SCREEN_X/2);
	pcpw->CenterY = (SCREEN_Y/2);
	pcpw->Text() = caption;
}
StyleString cCommandConfigCharEdged::shortExplanationText()
{
	map<tstring,StyleString> val;
	val[_T("Value")] = setStyle(sg_pDungeonSystem->EV_charEdgedFlag);
	return g_Lang(_T("文字描画の精細さExplanation"),val);
}

//文字描画の精細さコンフィグ
int cCommandConfigMapThickness::Action(cRenderDevice *pDev)
{
	MenuPopupConfigMapThickness(sg_pDungeonSystem->menuControlLayerV(), pDev, caption);
	return true;
}
void MenuPopupConfigMapThickness(cVectorOfControlLayer& vectorOfControlLayer, cRenderDevice *pDev, StyleString& caption)
{
	pcControlLayer pccl;
	pcParamaterConfigWindow pcpw;

	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));

	pccl->Init(pDev);
	pccl->WindowList.push_back(pcpw = pcParamaterConfigWindow(new cParamaterConfigWindow));
	pccl->notCancelToPop = true;

	pcpw->Init(pDev,&(sg_pDungeonSystem->EV_mapThickness),0,10);
	pcpw->CenterX = (SCREEN_X/2);
	pcpw->CenterY = (SCREEN_Y/2);
	pcpw->Text() = caption;
}
StyleString cCommandConfigMapThickness::shortExplanationText()
{
	map<tstring,StyleString> val;
	val[_T("Value")] = setStyle(sg_pDungeonSystem->EV_mapThickness);
	return g_Lang(_T("ミニマップ表示の濃さExplanation"),val);
}

//コンフィグ
int cCommandConfigConfig::Action(cRenderDevice *pDev)
{
	MenuPopupConfigConfig(sg_pDungeonSystem->menuControlLayerV(), pDev);
	return true;
}
void MenuPopupConfigConfig(cVectorOfControlLayer& vectorOfControlLayer, cRenderDevice *pDev)
{
	pcControlLayer pccl;
	pcSelectWindow pcsw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));

	pccl->Init(pDev);
	pccl->InputFreeFlag = TRUE;

	pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

	pcsw->commandList.push_back(pcCommand(new cCommandConfigPadConfig ));
	pcsw->commandList.push_back(pcCommand(new cCommandConfigBGMVolConfig ));
	pcsw->commandList.push_back(pcCommand(new cCommandConfigSEVolConfig ));
	pcsw->commandList.push_back(pcCommand(new cCommandConfigCharEdged ));
	pcsw->commandList.push_back(pcCommand(new cCommandConfigMapThickness ));
	pcsw->commandList.push_back(pcCommand(new cCommandConfigSignsConfig ));

	pcsw->Init(pDev, 8, pcsw->commandList.size());
	pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(1));
	pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(1));
}
