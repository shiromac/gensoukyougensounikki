#pragma once
#include "cDrawingObject.h"


class cDrawingObject_Rod :
	public cDrawableObject
{
public:
	cDrawingObject_Rod(void);
public:
	virtual ~cDrawingObject_Rod(void);

	virtual bool process(){return true;};//いらない関数ｗ


public:	
	double startX;
	double startY;
	double endX;
	double endY;

	double halfWidth;

	virtual int Draw(IDirect3DDevice9 *pDev);

	//基礎しかサポートし無い高速ドロー
	//(テクスチャサポートしない)
	//(加算サポートしない)
	virtual int EasyDraw(IDirect3DDevice9 *pDev);
};
