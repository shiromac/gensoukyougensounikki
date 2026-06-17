#include "stdafx.h"

#include "cParameterGage.h"


cParameterGage::cParameterGage(void)
{
	edge_width = 0;
	MaxValue = 0;
	Value = 0;
	drawmode = 0;
}

cParameterGage::~cParameterGage(void)
{


}

void cParameterGage::Draw(cRenderDevice *pDev)
{



	//エッジ
	if(edge_width)
	{
		DO.setTexture(NULL,0,0);
		DO.m_color = edgeColor;
		DO.AddingDraw = (cDrawableObject::DrawMode)drawmode;

		/*
		DO.setLTRB( Left() - edge_width, Top() - edge_width,
					Right() + edge_width, Bottom() + edge_width);
		DO.Draw(pDev);
		*/

		DO.setLTRB( Left() - edge_width, Top() - edge_width,
					Left()				, Bottom() + edge_width);
		DO.Draw(pDev);
		DO.setLTRB( Right()				, Top() - edge_width,
					Right() + edge_width, Bottom() + edge_width);
		DO.Draw(pDev);
		DO.setLTRB( Left()				, Top() - edge_width,
					Right()				, Top());
		DO.Draw(pDev);
		DO.setLTRB( Left()				, Bottom(),
					Right()				, Bottom() + edge_width);
		DO.Draw(pDev);
	}

	double separation = Value*Width/MaxValue;
	
	//前
	DO.setTexture(NULL,0,0);
	DO.m_color = forwardColor;
	DO.setLTRB( Left(), Top(),
				Left() + separation, Bottom());
	DO.Draw(pDev);

	//後ろ
	DO.m_color = backColor;
	DO.setLTRB( Left() + separation, Top(),
				Right(), Bottom());
	DO.Draw(pDev);


}



void cParameterGage::ShadowDraw(cRenderDevice *pDev, int x, int y, int alpha)
{

		DO.setTexture(NULL,0,0);
		DO.m_color.ARGB(alpha,0,0,0);
		DO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;
		
		DO.setLTRB( Left() - edge_width, Top() - edge_width,
					Right() + edge_width, Bottom() + edge_width);
		DO.Draw(pDev);


}
