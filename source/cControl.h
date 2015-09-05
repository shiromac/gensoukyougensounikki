#pragma once
#include "utility\cRectObj.h"

#include <d3d9.h>
#include <d3dx9.h>

class cControl :
	public cRectObj
{
public:
	cControl(void);
public:
	virtual ~cControl(void);

	virtual int process(IDirect3DDevice9 *pDev){return true;};

};
