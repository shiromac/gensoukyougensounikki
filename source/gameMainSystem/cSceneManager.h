#pragma once

#include "cSceneVirtual.h"


#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cSceneVirtual> pcSceneVirtual;


class cSceneManager
{
public:
	cSceneManager(void);
public:
	virtual ~cSceneManager(void);

	bool initialize(cRenderDevice *pDev);
	void finalize();

	void SceneChange(cRenderDevice *pDev, cSceneVirtual* nextScene);

	void SceneCheck(cRenderDevice *pDev);

	pcSceneVirtual m_NowScene;
protected:
	//ïÅíiNULL
	pcSceneVirtual m_NextScene;



};
