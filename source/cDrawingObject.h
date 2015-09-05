#pragma once
#include "gamemainsystem\cobject.h"
#include "utility\cRectObj.h"
#include "utility\cColor.h"
#include <d3d9.h>

#include <vector>

class cDrawableObject :
	public cObject
{
public:
	cDrawableObject(void);
public:
	virtual ~cDrawableObject(void);

	virtual bool process(){return true;};//いらない関数ｗ

protected:
	virtual bool culculateRealTexRange();
	IDirect3DTexture9* m_pTexture;
	cRectObj m_RealTexRange;
	int m_TexSizeX;
	int m_TexSizeY;

	virtual int SetRenderMode(IDirect3DDevice9 *pDev);
public:
	cColor m_color;
	std::vector<cColor> vm_color;//4隅の色を変える

	cRectObj m_TexRange;

	enum ColorBlendMode
	{
		COLOR_BLEND_MULTIPLE = 0,
		COLOR_BLEND_ADDITION = 1,
		COLOR_BLEND_FILL = 2,
	};
	ColorBlendMode colorblendmode;
	enum DrawMode
	{
		DRAW_MODE_ADDITION = 1,
		DRAW_MODE_NORMAL = 0,
		DRAW_MODE_SUBSTRACTION = -1,
	};
	DrawMode AddingDraw;

	void setTexture(IDirect3DTexture9* pTexture,int TexSizeX, int TexSizeY);
	void setTexture(IDirect3DTexture9* pTexture);

	virtual int Draw(IDirect3DDevice9 *pDev){return false;};


	virtual int getTexSizeX(){return m_TexSizeX;};
	virtual int getTexSizeY(){return m_TexSizeY;};
};

class cDrawingObject :
	public cDrawableObject
{
public:
	cDrawingObject(void);
public:
	virtual ~cDrawingObject(void);

	virtual bool process(){return true;};//いらない関数ｗ


public:	
	//特殊効果
	double Rotation;//回転(度数法)
	double ScaleX;//大きさX
	double ScaleY;//大きさY
	double Rotation2;//回転2(度数法)

	virtual int Draw(IDirect3DDevice9 *pDev);

	//基礎しかサポートし無い高速ドロー
	//(テクスチャサポートしない)
	//(加算サポートしない)
	virtual int EasyDraw(IDirect3DDevice9 *pDev);
};
