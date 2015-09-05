#include "stdafx.h"

#include "cMessageDroping.h"



cMessageDroping::cMessageDroping(void)
{
}
cMessageDroping::cMessageDroping(psstring pstr)
{
	pmessagestr = pstr;
}
cMessageDroping::~cMessageDroping(void)
{
}

StyleString cMessageDroping::shortExplanation()
{
	return *pmessagestr;
}
StyleString cMessageDroping::longExplanation()
{
	return *pmessagestr;
}

int cMessageDroping::RerenderText(SpriteText & sprite,int MaxLetterLength)
{
	StyleString str = pmessagestr->straightString();


	if(str.sector.size() > 0)
	{//セクションが一つ以上

		//文字が長いと縮小
		sprite.FitTextWidth(str,MaxLetterLength*GWFONTSIZE);

		/*
		if(str.length() > MaxLetterLength*2)
		{	
			str.sizes[0].x = (double)MaxLetterLength*2*0.9/str.length();
			str.setSameSize(str.sizes[0]);
		}
		*/

	}

	
	sprite.DrawText(str);

	return true;
}
//メニューで選択時に最初に呼ばれる
int cMessageDroping::Action(IDirect3DDevice9 *pDev)
{

	return true;
}