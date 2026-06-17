#pragma once
#include "cDrawingObject.h"
#include "gameMainSystem/cRenderBackend.h"


class cDrawingObject_ring :
	public cDrawableObject
{
public:
	cDrawingObject_ring(void);
public:
	virtual ~cDrawingObject_ring(void);

	virtual bool process(){return true;};//いらない関数ｗ


	//精度（ポリゴン数/2）
	int Accuracy;


	void setTexture(cRenderTexture* pTexture,int TexSizeX, int TexSizeY);

	virtual int Draw(cRenderDevice *pDev);

};
