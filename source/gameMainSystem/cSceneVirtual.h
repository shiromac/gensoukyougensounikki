#pragma once

#include "cRenderBackend.h"


class cSceneVirtual
{
public:
	cSceneVirtual(void);
public:
	virtual ~cSceneVirtual(void);

	virtual bool SceneInitialaze(cRenderDevice *pDev) = 0;
	virtual void SceneFinalize() = 0;

	virtual void SceneSystemDraw(cRenderDevice *pDev) = 0;
	virtual void SceneDraw(cRenderDevice *pDev) = 0;
	virtual void SceneProcess(cRenderDevice *pDev) = 0;

	//シーンより細かいフェイズ管理変数
	int phase_main;
	int phase_sub;
};
