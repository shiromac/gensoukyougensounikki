#include "stdafx.h"

#include "caECustom.h"

#include "caEchipS.h"



#define FILENAME_FIRETEX _T("effect\\fire_chip.png")
#define SIZE_FIRETEX (256)

//ptは0~7まで

#define BOXSIZE 64
//-------------------------------------------------
//単エフェクト
//-------------------------------------------------
caECustom::caECustom(void)
{
	End = false;
	preprocessed = 0;
}
caECustom::caECustom(int x,int y,int z)
{
	place.set(x,y,z,0);
	End = false;
	preprocessed = 0;
}

caECustom::~caECustom(void)
{
}


int caECustom::preprocess(IDirect3DDevice9 *pDev)
{




	
	return true;
}

int caECustom::process(IDirect3DDevice9 *pDev)
{


	if(preprocessed==0)
	{//初回

		preprocess(pDev);
		preprocessed = 1;
	}

	chipprocess(pDev);

	if(chipEmpty())
	{//オワリ
		End = true;
	}
	

	
	return true;
}

int caECustom::Draw(IDirect3DDevice9 *pDev)
{
	chipDraw(pDev);


	return true;
}


bool caECustom::isOnScreen()
{

	if(	sg_pDungeonSystem->Map().isOnDisplay(place.x, place.y, 1) )
	{
		return true;
	}
	

	return false;
}
