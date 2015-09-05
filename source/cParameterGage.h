#pragma once

#include "utility/cRectObj.h"
#include "utility/cColor.h"
#include "cDrawingObject.h"

#include <vector>
#include <list>
#include <tstring_ph.h>


using namespace std;


class cParameterGage :
	public cRectObj
{
public:
	cParameterGage(void);
public:
	virtual ~cParameterGage(void);

protected:
	cDrawingObject DO;
public:
	virtual void Draw(IDirect3DDevice9 *pDev);

	virtual void ShadowDraw(IDirect3DDevice9 *pDev, int x, int y, int alpha);

	int drawmode;
	int edge_width;


	double MaxValue;
	double Value;

	cColor edgeColor;
	cColor forwardColor;
	cColor backColor;
};
