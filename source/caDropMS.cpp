#include "stdafx.h"

#include "caDropMS.h"

#define ANIME_THROW_SPEED (0.2) 

caThrow::caThrow(void)
{
	End = false;
	flag = 0;
	route = ROUTE_LOW;//低空
	progress = 0;
	cameraTracing = false;
}

caThrow::~caThrow(void)
{
}

int caThrow::Init(pcAnimation me)
{
	cAnimation::Init(me);

	double distance = 0;
	c4DVector v = end - start;



	//start.z = end.z = THROW_BASE_HEIGHT;


	if(route == ROUTE_HIGH)
	{
		distance += 2;
	}

	distance += max(abs(v.x), abs(v.y));

	Displayin = 0;

	int i;
	c4DVector pos;

	double d;
	for(i=0;i<=32;i++)
	{
		d = i/32.0;
		pos = (1-d)*start + d*end;
		if(	sg_pDungeonSystem->Map().isOnDisplay(pos.x, pos.y, 2) )
		{
			//初期値
			progress = i/32.0*distance;
			break;
		}
	}

	//全体値
	entiredistance = distance;


	if(drawtype == DRAWTYPE_ATTACKDRAW)
	{
		cCoordinate coo;
		coo.x = v.x;
		coo.y = v.y;
		pThrowed->visibleaspect = coo.GetAspect();
	}

	pThrowed->mapForcus.x = sg_pDungeonSystem->Map().visibleForcusX();
	pThrowed->mapForcus.y = sg_pDungeonSystem->Map().visibleForcusY();


	return true;
}

int caThrow::preprocess()
{
	//値待避
	t_shouldDraw = pThrowed->pri_onland_DrawFlag;
	pThrowed->pri_onland_DrawFlag = false;
	return true;
}

int caThrow::process(cRenderDevice *pDev)
{

	c4DVector position;
	double x;

	if(flag==0)
	{//初回

		/*
		if(sg_pDungeonSystem->八方判定(start,end))
		{

		}
		else
		{
			route = ROUTE_HIGH;//空中
		}
		*/
		
		preprocess();
		flag = 1;
	}



	
	//x=[0,1]
	x = progress/entiredistance;

	pThrowed->mapForcus.x = sg_pDungeonSystem->Map().visibleForcusX();
	pThrowed->mapForcus.y = sg_pDungeonSystem->Map().visibleForcusY();



	if(route == ROUTE_HIGH)
	{//空中
		position = (1-x)*start + x*end;
		position.z += -4*(THROW_PEAK_HEIGHT)*(x-0.5)*(x-0.5) + THROW_PEAK_HEIGHT;
	}
	else
	{
		position = (1-x)*start + x*end;
		//position.z = THROW_BASE_HEIGHT;
	}
	

	pThrowed->visibleplace = position;

	if(cameraTracing) {
		sg_pDungeonSystem->Map().setVisibleForcusX(position.x);
		sg_pDungeonSystem->Map().setVisibleForcusY(position.y);
	}

	if((Displayin && !sg_pDungeonSystem->Map().isOnDisplay(position.x, position.y, 1) )
		|| x >= 1)
	{//範囲外に
		//値復帰
		pThrowed->pri_onland_DrawFlag = t_shouldDraw;
		pThrowed->visibleplace = end;



		End = true;
	}
	
	if(sg_pDungeonSystem->Map().isOnDisplay(position.x, position.y, 1))
	{
		Displayin = 1;
	}
	

	//進行
	if(drawtype == DRAWTYPE_NORMAL)
	{
		progress += ANIME_THROW_SPEED * speed;
	}
	else
	{
		progress += ANIME_THROW_SPEED*2 * speed;
	}
	
	if(drawtype == DRAWTYPE_ATTACKDRAW)
	{
		cCoordinate coo;
		c4DVector v = end - start;
		coo.x = v.x;
		coo.y = v.y;
		pThrowed->visibleaspect = coo.GetAspect();
	}

	return true;
}

int caThrow::Draw(cRenderDevice *pDev)
{
	if(flag!=0)
	{//ぷりプロセスした
		if(drawtype == DRAWTYPE_NORMAL)
		{
			pThrowed->Draw(pDev);
		}
		else if(drawtype == DRAWTYPE_ATTACKDRAW)
		{
			pThrowed->aspectedAttackDraw(pDev);
		}
	}
	return true;
}


bool caThrow::isOnScreen()
{
	double d;
	int i;
	c4DVector v;

	for(i=0;i<=128;i++)
	{
		d = i/128.0;
		v = (1-d)*start + (d)*end;
		if(	sg_pDungeonSystem->Map().isOnDisplay(v.x, v.y, 1) )
		{
			return true;
		}
	}

	return false;
}