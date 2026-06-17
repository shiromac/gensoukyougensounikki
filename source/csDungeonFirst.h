#pragma once
#include "gameMainSystem\cscenevirtual.h"

class csDungeonFirst :
	public cSceneVirtual
{
public:
	csDungeonFirst(void);
public:
	virtual ~csDungeonFirst(void);

		
	virtual bool SceneInitialaze(cRenderDevice *pDev);
	virtual void SceneFinalize();

	virtual void SceneSystemDraw(cRenderDevice *pDev);
	virtual void SceneDraw(cRenderDevice *pDev);
	virtual void SceneProcess(cRenderDevice *pDev);


};
