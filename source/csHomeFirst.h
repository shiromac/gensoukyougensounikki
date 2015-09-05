#pragma once
#include "gameMainSystem\cscenevirtual.h"

class csHomeFirst :
	public cSceneVirtual
{
public:
	csHomeFirst(void);
public:
	virtual ~csHomeFirst(void);

		
	virtual bool SceneInitialaze(IDirect3DDevice9 *pDev);
	virtual void SceneFinalize();

	virtual void SceneSystemDraw(IDirect3DDevice9 *pDev);
	virtual void SceneDraw(IDirect3DDevice9 *pDev);
	virtual void SceneProcess(IDirect3DDevice9 *pDev);


};
