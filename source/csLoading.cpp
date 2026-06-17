
#include "stdafx.h"
#include "Language.h"

#include "csLoading.h"
#include "csTitle.h"

#include "cVectorOfControlLayer.h"

#include "cSoundManager.h"
#include "cGameWindow.h"

#include "cCommand.h"
#include "utility/debug.h"

#include <vector>
#include <list>
#include <tstring_ph.h>

#include "cSaveStore.h"

using namespace std;

#ifndef __EMSCRIPTEN__
#include <windows.h>
#include <process.h>
#endif



#ifndef __EMSCRIPTEN__
#define GGN_LOADING_THREAD_CALL __stdcall
#else
#define GGN_LOADING_THREAD_CALL
#endif

unsigned GGN_LOADING_THREAD_CALL LoadingThread(void * pDev_void);

unsigned GGN_LOADING_THREAD_CALL LoadingThread(void * pDev_void)
{
	cRenderDevice *pDev = static_cast<cRenderDevice *>(pDev_void);
	
	
	if(sg_pDungeonSystem == NULL)
	{
		g_InitSystemData((LANGUAGE_IMPORTDATADIR()+ _T("systems.id")));

		sg_pDungeonSystem = pcDungeonSystem(new cDungeonSystem);
		sg_pDungeonSystem->InitSystem(pDev);
		{
			/*
			sg_pDungeonSystem->Map().m_pTexture_Mapchip = 
				g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev, _T("map\\紅魔館セット.png"));
			
			sg_pDungeonSystem->pFrameTexture = 
				g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev, _T("interface\\frame1.png"));
			*/
		}
		//sg_pDungeonSystem->InitDungeon(pDev);

		int error = 0;
		cScriptReader SR(g_GraphicAssetPath(_T("graphicpass.id")));
#ifdef __EMSCRIPTEN__
		error |= SR.load();
#elif defined(_UNRELEASE)
		error |= SR.loadAndpacked();
#else
		error |= SR.loadpacked();
		if(error) exit(0);//ゲーム終了
#endif
		vector<tstring> passes;
		//getPass(SR.pdata(),SR.scriptdir(),passes);
		//ロードしないほうがいいです

		g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev, WINDOWTEXPATH);
		g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev, CURSORTEXPATH);
		g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev, PAGETEXPATH);

		/*
		int i;
		for(i=0;i<passes.size();i++)
		{
			g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev, passes[i].c_str());

		}
		*/
	}





	g_SaveConfig.Init(pDev,_T("config.dat"));




	return 0;
}

#ifndef __EMSCRIPTEN__
unsigned g_thID;
HANDLE g_hTh;
#endif


csLoading::csLoading(void)
{

}

csLoading::~csLoading(void)
{

}


bool csLoading::SceneInitialaze(cRenderDevice *pDev)
{
	/*
	g_hTh = (HANDLE)_beginthreadex(NULL, 0, LoadingThread, pDev, 0, &g_thID);
	if(g_hTh == 0)
	{
		OnAssert(_T(__FILE__),__LINE__,false,_T("ローディング失敗"));
		return false;
	}
	*/
	
	g_GameEnv.m_Screen->m_OffScreenEnable = false;
	g_GameEnv.m_Screen->m_LayerEnable = false;
	g_GameEnv.m_Screen->m_MiniEnable = false;

	g_GameEnv.m_Screen->setRenderTarget(pDev, SCREEN_BACKGROUND);


	cRenderTexture* pTexture;
	//pTexture = g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev,_T("effect\\magiccircle.png"));
	pTexture = g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev,_T("interface\\title.png"));

	sg_wall.setTexture(pTexture,800,600);
	sg_wall.m_TexRange.setLTRB(0,0,1,1);
	sg_wall.m_color.ARGB(255,200,200,200);
	sg_wall.setLTRB(0,0,800,600);

	pTexture = g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev,_T("interface\\nowloadingnow.png"));

	sg_wall2.setTexture(pTexture);
	sg_wall2.m_TexRange.setLTRB(0,0,1,1);
	sg_wall2.CenterX = 400;
	sg_wall2.CenterY = 300;
	sg_wall2.Width = 256;
	sg_wall2.Height = 64;

	pTexture = g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev,_T("interface\\loading.png"));
	
	sg_wall3.setTexture(pTexture);
	sg_wall3.m_TexRange.setLTRB(0,0,1,1);
	sg_wall3.CenterX = 400;
	sg_wall3.CenterY = 300;
	sg_wall3.Width = 64;
	sg_wall3.Height = 64;

	sg_wall3.colorblendmode = cDrawableObject::COLOR_BLEND_ADDITION;
	sg_wall3.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

	return true;
}

void csLoading::SceneFinalize()
{
#ifndef __EMSCRIPTEN__
	if(g_hTh != NULL)
	{
		CloseHandle(g_hTh);
	}
#endif
}

void csLoading::SceneSystemDraw(cRenderDevice *pDev)
{

}
static int s_drawed = 0;
void csLoading::SceneDraw(cRenderDevice *pDev)
{
	sg_wall.Draw(pDev);

	sg_wall3.Draw(pDev);
	sg_wall2.Draw(pDev);

	s_drawed += 1;
}

void csLoading::SceneProcess(cRenderDevice *pDev)
{
	sg_wall3.Rotation += 2;

#ifndef __EMSCRIPTEN__
	DWORD dwExCode;
	GetExitCodeThread(g_hTh, &dwExCode);
	if(dwExCode != STILL_ACTIVE && s_drawed > 3)
#else
	if(s_drawed > 3)
#endif
	{//終了した
		
		LoadingThread(pDev);

		g_GameEnv.m_SceneManage->SceneChange(pDev,new csTitle);
	}


}