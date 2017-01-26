#include "../stdafx.h"

#include "cNumField.h"

#include "../cDrawingObject.h"

cNumField::cNumField(void):
colorBlendMode(cDrawingObject::COLOR_BLEND_MULTIPLE),
	drawMode(cDrawingObject::DRAW_MODE_NORMAL),
	arrangeX(ARRANGEX_RIGHT),
	arrangeY(ARRANGEY_BOTTOM),
	Value_(0),
	percentSign(false),
	plusSign(false),
	distanceQuant(true)
{
}

cNumField::~cNumField(void)
{

}

void cNumField::Init(IDirect3DDevice9 *pDev)
{
	pTexture_ = g_GameEnv.m_GlobalResourse->getTextureFromFile(
					pDev,_T("interface\\num.png"));

	distance = 0;
	Fontindex = 0;
	FontHeightSize = 32*2;
	FontWidthSize = 16*2;
}



void cNumField::GetFigure(vector<int>& figures)
{
	figures.clear();

	if(percentSign) {
		figures.push_back(13);//%記号
	}

	int i;
	INT64 tmp;
	int minus = 0;

	tmp = Value_;
	if(tmp < 0) 
	{
		tmp = -tmp;
		minus = 1;
	}
	for(i=0;i<CNF_MAX_FIGURE;i++)
	{
		figures.push_back(tmp % 10);
		tmp /= 10;

		if(tmp <= 0)
		{
			break;
		}
	}
	if(minus)
	{
		figures.push_back(10);//マイナス記号
	}
	else if(plusSign)
	{
		figures.push_back(11);//プラス記号
	}
}

void cNumField::Draw(IDirect3DDevice9 *pDev)
{

	int i;
	cDrawingObject DO;

	vector<int> Figures;

	GetFigure(Figures);


	DO.setTexture(pTexture_, 512,256);


	DO.Height = FontHeightSize;
	DO.Width = FontWidthSize;

	int Xdif = 0;

	if(arrangeY == ARRANGEY_BOTTOM)
	{
		DO.setBottom(position.y);
	}
	else if(arrangeY == ARRANGEY_CENTER)
	{
		DO.CenterY = position.y;
	}
	else if(arrangeY == ARRANGEY_TOP)
	{
		DO.setTop(position.y);
	}

	Xdif = -DO.Width -distance;
	int size = Figures.size();
	if(arrangeX == ARRANGEX_RIGHT)
	{
		DO.setRight(position.x);
	}
	else if(arrangeX == ARRANGEX_CENTER)
	{
		DO.CenterX = position.x - Xdif*(size-1)/2.0;
	}
	else if(arrangeX == ARRANGEX_LEFT)
	{
		DO.setLeft(position.x - Xdif*(size-1));
	}



	DO.m_color = color;
	DO.AddingDraw = (cDrawingObject::DrawMode)drawMode;
	DO.colorblendmode = (cDrawingObject::ColorBlendMode)colorBlendMode;
	
	
	if(distanceQuant) {
		distance = floor(distance);
	}

	for(i=0;i<Figures.size();i++)
	{
		
		DO.m_TexRange.setLTRB((double)(Figures[i]) / 16,
								(Fontindex) / 4.0,
								(double)(Figures[i]+1) / 16,
								(Fontindex+1) / 4.0);

		DO.Draw(pDev);

		DO.CenterX -= DO.Width + distance;
	}

}