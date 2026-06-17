#pragma once
#include "gamemainsystem\cscenevirtual.h"

#include "cControlLayer.h"

#include "cSaveStore.h"
#include "cDrawingObject.h"
class csTitle :
	public cSceneVirtual
{
public:
	csTitle(void);
public:
	virtual ~csTitle(void);

	virtual bool SceneInitialaze(cRenderDevice *pDev);
	virtual void SceneFinalize();

	virtual void SceneSystemDraw(cRenderDevice *pDev);
	virtual void SceneDraw(cRenderDevice *pDev);
	virtual void SceneProcess(cRenderDevice *pDev);

	cDrawingObject sg_wall;
	cDrawingObject sg_wall2;
	cDrawingObject sg_wall3;

	cDrawingObject sg_button;
};

extern cSaveConfig g_SaveConfig;