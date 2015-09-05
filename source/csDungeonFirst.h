#pragma once
#include "gameMainSystem\cscenevirtual.h"

class csDungeonFirst :
	public cSceneVirtual
{
public:
	csDungeonFirst(void);
public:
	virtual ~csDungeonFirst(void);

		
	virtual bool SceneInitialaze(IDirect3DDevice9 *pDev);
	virtual void SceneFinalize();

	virtual void SceneSystemDraw(IDirect3DDevice9 *pDev);
	virtual void SceneDraw(IDirect3DDevice9 *pDev);
	virtual void SceneProcess(IDirect3DDevice9 *pDev);


};
