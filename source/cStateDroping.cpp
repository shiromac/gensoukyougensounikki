#include "stdafx.h"

#include "cStateDroping.h"



cStateDroping::cStateDroping(void)
{
}
cStateDroping::cStateDroping(StyleString cstr, StyleString estr)
{
	CaptionStr = cstr;
	ExplanationStr = estr;
}
cStateDroping::~cStateDroping(void)
{
}

StyleString cStateDroping::shortExplanation()
{
	return ExplanationStr;
}
StyleString cStateDroping::longExplanation()
{
	return ExplanationStr;
}

int cStateDroping::RerenderText(SpriteText & sprite,int MaxLetterLength)
{
	StyleString str = CaptionStr;


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
int cStateDroping::Action(IDirect3DDevice9 *pDev)
{

	return true;
}