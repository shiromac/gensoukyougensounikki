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

	//IDirect3DTexture9* pTex;

	virtual int process(IDirect3DDevice9 *pDev);
	virtual int Draw(IDirect3DDevice9 *pDev);

	virtual bool isOnScreen();


};

