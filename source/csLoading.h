#pragma once
#include "gamemainsystem\cscenevirtual.h"

#include "cControlLayer.h"
#include "cDrawingObject.h"


class csLoading :
	public cSceneVirtual
{
public:
	csLoading(void);
public:
	virtual ~csLoading(void);

	virtual bool SceneInitialaze(cRenderDevice *pDev);
	virtual void SceneFinalize();

	virtual void SceneSystemDraw(cRenderDevice *pDev);
	virtual void SceneDraw(cRenderDevice *pDev);
	virtual void SceneProcess(cRenderDevice *pDev);

	cDrawingObject sg_wall;
	cDrawingObject sg_wall2;
	cDrawingObject sg_wall3;
};
