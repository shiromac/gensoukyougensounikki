#include "stdafx.h"

#include "cDrawingObject_ring.h"

#define DEFOULT_ACCURACY (16)
cDrawingObject_ring::cDrawingObject_ring(void)
{
	Accuracy = DEFOULT_ACCURACY;
}

cDrawingObject_ring::~cDrawingObject_ring(void)
{
}

int cDrawingObject_ring::Draw(cRenderDevice *pDev)
{

	return true;
}
