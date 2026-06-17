#pragma once
#include "cAnimation.h"

#include <vector>
#include <list>
#include <string>
using namespace std;



class caMassageWait :
	public cAnimation
{
public:
	caMassageWait(void);
public:
	virtual ~caMassageWait(void);

	int preprocessed;

	//cRenderTexture* pTex;

	virtual int process(cRenderDevice *pDev);
	virtual int Draw(cRenderDevice *pDev);

	virtual bool isOnScreen();


};

