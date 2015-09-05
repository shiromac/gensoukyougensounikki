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

	virtual bool SceneInitialaze(IDirect3DDevice9 *pDev);
	virtual void SceneFinalize();

	virtual void SceneSystemDraw(IDirect3DDevice9 *pDev);
	virtual void SceneDraw(IDirect3DDevice9 *pDev);
	virtual void SceneProcess(IDirect3DDevice9 *pDev);

	cDrawingObject sg_wall;
	cDrawingObject sg_wall2;
	cDrawingObject sg_wall3;

	cDrawingObject sg_button;
};

extern cSaveConfig g_SaveConfig;