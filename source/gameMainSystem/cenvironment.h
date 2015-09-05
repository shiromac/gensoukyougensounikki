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

#include <d3d9.h>
#include <d3dx9.h>

//基本的に唯一に作られる（グローバル）。
class cEnvironment
{
public:
	cEnvironment(void);
public:
	virtual ~cEnvironment(void);

	virtual void init(LPDIRECT3DDEVICE9 pDev);

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
	vector<IDirect3DTexture9*> pmTextureVforGlobalIndex;
	

	bool AppEnd;//終了フラグ

	bool ResetDevice(LPDIRECT3DDEVICE9 pDev);//デバイスのリセット

	CFilePackLoad& FilePackLoad(){return FilePackLoad_;};
protected:
	//パックテクスチャ
	CFilePackLoad FilePackLoad_;
};

extern cEnvironment g_GameEnv;//ゲーム環境


/*
//これを使ってテクスチャの参照をする。
inline IDirect3DTexture9* g_pTexture(int indexOrName)
{
	return g_GameEnv.pmTextureVforGlobalIndex[indexOrName];
}
inline IDirect3DTexture9** g_ppTexture(int indexOrName)
{
	return &(g_GameEnv.pmTextureVforGlobalIndex[indexOrName]);
}

//これを使ってテクスチャを読み込む
inline IDirect3DTexture9* loadGlobalTexture(IDirect3DDevice9* pDevice, TCHAR* filename,int indexOrName)
{
	return *g_ppTexture(indexOrName) = g_GameEnv.m_GlobalResourse->getTextureFromFile(pDevice, filename);
}

inline IDirect3DTexture9* loadTexture(IDirect3DDevice9* pDevice, TCHAR* filename,int indexOrName)
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

