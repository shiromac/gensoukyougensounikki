#pragma once

#include "cResourseManage.h"
#include "cScreen.h"
#include "cSceneManager.h"
#include "cInput.h"
//#include "cPlayerData.h"
#include "../cSoundManager.h"

#include "../TexSeBgmFilepack/CFilePack.h"


#include <vector>
using namespace std;


//基本的に唯一に作られる（グローバル）。
class cEnvironment
{
public:
	cEnvironment(void);
public:
	virtual ~cEnvironment(void);

	virtual void init(cRenderDevice* pDev);

	//シーンごとのリソース
	//cResourseManage*	m_Resourse;

	//終了まで開放しないリソース
	cResourseManage*	m_GlobalResourse;

	//スクリーン
	cScreen*			m_Screen;

	cSceneManager*		m_SceneManage;


	//cPlayerData PlayerInfo[4];


	cInput		m_Input;


	cSoundManager		m_SoundManager;
	


	//めんどいので直接参照しないように
	vector<cRenderTexture*> pmTextureVforGlobalIndex;
	

	bool AppEnd;//終了フラグ

	bool ResetDevice(cRenderDevice* pDev);//デバイスのリセット

	CFilePackLoad& FilePackLoad(){return FilePackLoad_;};
protected:
	//パックテクスチャ
	CFilePackLoad FilePackLoad_;
};

extern cEnvironment g_GameEnv;//ゲーム環境


/*
//これを使ってテクスチャの参照をする。
inline cRenderTexture* g_pTexture(int indexOrName)
{
	return g_GameEnv.pmTextureVforGlobalIndex[indexOrName];
}
inline cRenderTexture** g_ppTexture(int indexOrName)
{
	return &(g_GameEnv.pmTextureVforGlobalIndex[indexOrName]);
}

//これを使ってテクスチャを読み込む
inline cRenderTexture* loadGlobalTexture(cRenderDevice* pDevice, TCHAR* filename,int indexOrName)
{
	return *g_ppTexture(indexOrName) = g_GameEnv.m_GlobalResourse->getTextureFromFile(pDevice, filename);
}

inline cRenderTexture* loadTexture(cRenderDevice* pDevice, TCHAR* filename,int indexOrName)
{
	return *g_ppTexture(indexOrName) = g_GameEnv.m_GlobalResourse->getTextureFromFile(pDevice, filename);
}
*/
//インプット
inline cPlayerInput* g_pPlayerInput()
{
	return &(g_GameEnv.m_Input.PlayerInput);
}

#define MAX_TEX_INDEXSIZE 1000

