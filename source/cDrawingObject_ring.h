#pragma once
#include "cDrawingObject.h"
#include <d3d9.h>


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


	void setTexture(IDirect3DTexture9* pTexture,int TexSizeX, int TexSizeY);

	virtual int Draw(IDirect3DDevice9 *pDev);

};
