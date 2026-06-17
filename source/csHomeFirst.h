#pragma once
#include "gameMainSystem\cscenevirtual.h"

class csHomeFirst :
	public cSceneVirtual
{
public:
	csHomeFirst(void);
public:
	virtual ~csHomeFirst(void);

		
	virtual bool SceneInitialaze(cRenderDevice *pDev);
	virtual void SceneFinalize();

	virtual void SceneSystemDraw(cRenderDevice *pDev);
	virtual void SceneDraw(cRenderDevice *pDev);
	virtual void SceneProcess(cRenderDevice *pDev);


};
