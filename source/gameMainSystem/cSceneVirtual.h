#pragma once

#include <d3d9.h>
#include <d3dx9.h>

class cSceneVirtual
{
public:
	cSceneVirtual(void);
public:
	virtual ~cSceneVirtual(void);

	virtual bool SceneInitialaze(IDirect3DDevice9 *pDev) = 0;
	virtual void SceneFinalize() = 0;

	virtual void SceneSystemDraw(IDirect3DDevice9 *pDev) = 0;
	virtual void SceneDraw(IDirect3DDevice9 *pDev) = 0;
	virtual void SceneProcess(IDirect3DDevice9 *pDev) = 0;

	//シーンより細かいフェイズ管理変数
	int phase_main;
	int phase_sub;
};
