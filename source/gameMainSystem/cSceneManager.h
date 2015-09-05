#pragma once

#include "cSceneVirtual.h"


#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cSceneVirtual> pcSceneVirtual;

#include <d3d9.h>
#include <d3dx9.h>

class cSceneManager
{
public:
	cSceneManager(void);
public:
	virtual ~cSceneManager(void);

	bool initialize(IDirect3DDevice9 *pDev);
	void finalize();

	void SceneChange(IDirect3DDevice9 *pDev, cSceneVirtual* nextScene);

	void SceneCheck(IDirect3DDevice9 *pDev);

	pcSceneVirtual m_NowScene;
protected:
	//ïÅíiNULL
	pcSceneVirtual m_NextScene;



};
