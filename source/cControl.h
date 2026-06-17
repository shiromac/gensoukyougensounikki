#pragma once
#include "utility\cRectObj.h"

#include "gameMainSystem/cRenderBackend.h"

class cControl :
	public cRectObj
{
public:
	cControl(void);
public:
	virtual ~cControl(void);

	virtual int process(cRenderDevice *pDev){return true;};

};
