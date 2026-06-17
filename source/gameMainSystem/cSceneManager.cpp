#include "../stdafx.h"

#include "cSceneManager.h"
#include "../csLoading.h"

cSceneManager::cSceneManager(void)
{
		//普段NULL
	m_NextScene = pcSceneVirtual((cSceneVirtual*)NULL);
}

cSceneManager::~cSceneManager(void)
{

}

bool cSceneManager::initialize(cRenderDevice *pDev)
{
	//初期化

	m_NowScene = pcSceneVirtual(new csLoading);//初期シーン

	//RELEASE(g_GameEnv.m_Resourse);
	//g_GameEnv.m_Resourse = new cResourseManage(&g_GameEnv.FilePackLoad());

	return m_NowScene->SceneInitialaze(pDev);//初期化
}
void cSceneManager::finalize()
{
	//終了処理
	m_NowScene->SceneFinalize();


	//RELEASE(g_GameEnv.m_Resourse);


}

void cSceneManager::SceneChange(cRenderDevice *pDev, cSceneVirtual* nextScene)
{
	m_NextScene = pcSceneVirtual(nextScene);
}

void cSceneManager::SceneCheck(cRenderDevice *pDev)
{

	if(m_NextScene != NULL)
	{
		bool error;

		//終了処理
		m_NowScene->SceneFinalize();



		g_GameEnv.m_Input.ClearInput();


		m_NowScene = m_NextScene;
		error = m_NowScene->SceneInitialaze(pDev);//初期化

		if(error) OutputDebugString( _T("SceneInitialaze初期化でerror\n"));




		m_NextScene = pcSceneVirtual((cSceneVirtual*)NULL);
	}
}

