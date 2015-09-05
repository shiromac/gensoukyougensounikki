#include "stdafx.h"

#include "caMove.h"

caMove::caMove()
{
	End = false;
	count = 0;
	speed = 1;
}

caMove::~caMove()
{
}


int caMove::process(IDirect3DDevice9 *pDev)
{
	//---------------------------------------
	//	checkpointに始点（中間点）終点の順にいれてから処理を行う
	//---------------------------------------
	count += speed;
	
	int c = count/ANIME_MOVE_STEP;
	pcLandform start, end;

	double d = (count - c*ANIME_MOVE_STEP)/ANIME_MOVE_STEP;



	if( !End && (checkpoint.size() > c+1))
	{


		start = checkpoint[c];
		end = checkpoint[c+1];

		pGobj->visibleaspect = Aspect[c];

		
		pGobj->visibleplace.x = start->placeX + -d*start->placeX + (d)*end->placeX;
		pGobj->visibleplace.y = start->placeY + -d*start->placeY + (d)*end->placeY;

		pGobj->opaque = (1-d)*Sight[c] + (d)*Sight[c+1];
	}
	else
	{
		pGobj->visibleplace.x = checkpoint[checkpoint.size()-1]->placeX;
		pGobj->visibleplace.y = checkpoint[checkpoint.size()-1]->placeY;
		pGobj->opaque = Sight[Sight.size()-1];
	
		End = true;
	
	}
	
	return true;
}

bool caMove::isOnScreen()
{
	int i;
	for(i=0;i<checkpoint.size();i++)
	{
		if( (Sight[i] > 0.5) &&
			sg_pDungeonSystem->Map().isOnDisplay(checkpoint[i]->placeX,checkpoint[i]->placeY,1) )
		{
			return true;
		}
	}

	return false;
}