#pragma once

#include "cControlLayer.h"

#include <vector>
#include <tstring_ph.h>


using namespace std;

class cVectorOfControlLayer :
	public 	vector<pcControlLayer>
{
public:
	cVectorOfControlLayer(void);
public:
	virtual ~cVectorOfControlLayer(void);

	virtual pcControlLayer Last();


	virtual void cancel();
	virtual void allCancel();


	virtual void process(IDirect3DDevice9 *pDev);
	
	virtual int AllRelease();

};

