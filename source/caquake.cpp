#include "stdafx.h"

#include "caquake.h"

const double PI = 3.14159265358979;
caQuake::caQuake(void)
{
	count = 0;
	time_frame = 60;
	amplitude = 32;
}

caQuake::~caQuake(void)
{
}


int caQuake::Init()
{

	preprocess = 0;
	return true;
}
int caQuake::process(IDirect3DDevice9 *pDev)
{
	if(!preprocess)
	{
		ForcusX = sg_pDungeonSystem->Map().visibleForcusX();
		ForcusY = sg_pDungeonSystem->Map().visibleForcusY();

		preprocess = 1;
	}

	count++;

	if( !End &&  count < time_frame)
	{
		double d = sin(((double)count/time_frame) * PI);
		countX = (double)(rand() - RAND_MAX/2) /RAND_MAX /5;
		countY = (double)(rand() - RAND_MAX/2) /RAND_MAX /5;
		sg_pDungeonSystem->Map().setVisibleForcusX(ForcusX + d*(amplitude/64.0 * sin(countX)));
		sg_pDungeonSystem->Map().setVisibleForcusY(ForcusY + d*(amplitude/64.0 * sin(countY)));
	}
	else
	{
		sg_pDungeonSystem->Map().setVisibleForcusX(ForcusX);
		sg_pDungeonSystem->Map().setVisibleForcusY(ForcusY);


		End = true;
	}
	
	return true;
}
int caQuake::Draw(IDirect3DDevice9 *pDev)
{
	return true;
}
bool caQuake::isOnScreen()
{

	return true;
}