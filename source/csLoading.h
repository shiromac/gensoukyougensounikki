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

	virtual bool SceneInitialaze(IDirect3DDevice9 *pDev);
	virtual void SceneFinalize();

	virtual void SceneSystemDraw(IDirect3DDevice9 *pDev);
	virtual void SceneDraw(IDirect3DDevice9 *pDev);
	virtual void SceneProcess(IDirect3DDevice9 *pDev);

	cDrawingObject sg_wall;
	cDrawingObject sg_wall2;
	cDrawingObject sg_wall3;
};
