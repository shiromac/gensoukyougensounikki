#include "stdafx.h"


#include "cMap.h"


#define MINIMUM_RECT_SIZE 4


#define MINIMUM_ROOM_SIZE 4
#define MARGIN_BETWEEN_RECT_ROOM 2
#define MINIMUM_RECT_SIZE (MINIMUM_ROOM_SIZE + (MARGIN_BETWEEN_RECT_ROOM * 2))


int cMap::makeNewDungeon(int Flag)
{
	MD_rectList.clear();
	MD_roomList.clear();

	MD_rect_split(MD_rect_add(0, 0, MAPWIDTH - 1, MAPHEIGHT - 1));

	MD_room_make();

	MD_data_optimize();
	resetAllLandNeighbor();
	return true;
}

//ãÊâÊÇí«â¡
pcRectObj cMap::MD_rect_add(int lx, int ly, int hx, int hy)
{
	pcRectObj rect;
	rect = pcRectObj(new cRectObj);
	MD_rectList.push_back(rect);

	rect->setLTRB(lx,ly,hx,hy);

	return rect;
}

//êVãKïîâÆóÃàÊ
pcRectObj cMap::MD_room_add(int lx, int ly, int hx, int hy)
{
	pcRectObj rect;
	rect = pcRectObj(new cRectObj);
	MD_roomList.push_back(rect);

	rect->setLTRB(lx,ly,hx,hy);
	return rect;
}

//ïîâÆçÏê¨
void cMap::MD_room_make()
{
	int i;
	pcRectObj rect;
	int x, y, w, h;

	for (i = 0; i < MD_rectList.size(); i++)
	{
		rect = MD_rectList[i];
		w = random_range(MINIMUM_ROOM_SIZE, rect->Right() - rect->Left() - (MARGIN_BETWEEN_RECT_ROOM * 2) + 1);
		h = random_range(MINIMUM_ROOM_SIZE, rect->Bottom() - rect->Top() - (MARGIN_BETWEEN_RECT_ROOM * 2) + 1);
		x = random_range(rect->Left() + MARGIN_BETWEEN_RECT_ROOM, rect->Right() - MARGIN_BETWEEN_RECT_ROOM - w + 1);
		y = random_range(rect->Top() + MARGIN_BETWEEN_RECT_ROOM, rect->Bottom() - MARGIN_BETWEEN_RECT_ROOM - h + 1);
		MD_room_add(x, y, x + w, y + h);
	}
}

//ãÊâÊãÊêÿÇË
void cMap::MD_rect_split(pcRectObj rect_parent)
{
	pcRectObj rect_child;

	//èIóπèåè
	if ((rect_parent->Bottom() - rect_parent->Top() <= MINIMUM_RECT_SIZE * 2) ||
		(rect_parent->Right() - rect_parent->Left() <= MINIMUM_RECT_SIZE * 2))
	{
		return;
	};

	//ï°êª
	rect_child = MD_rect_add(rect_parent->Left(), rect_parent->Top(),
							rect_parent->Right(), rect_parent->Bottom());


	if (random_range(0, 2) < 1)
	{
		int split_coord_y;
		split_coord_y = random_range(rect_parent->Top() + MINIMUM_RECT_SIZE, rect_parent->Bottom() - MINIMUM_RECT_SIZE);
		rect_parent->formBottom(split_coord_y);
		rect_child->formTop(split_coord_y);
		MD_rect_split(rect_parent);
		MD_rect_split(rect_child);
		return;
	}
	else
	{
		int split_coord_x;
		split_coord_x = random_range(rect_parent->Left() + MINIMUM_RECT_SIZE, rect_parent->Right() - MINIMUM_RECT_SIZE);
		rect_parent->formRight(split_coord_x);
		rect_child->formLeft(split_coord_x);
		MD_rect_split(rect_parent);
		MD_rect_split(rect_child);
		return;
	};
}

void cMap::MD_data_optimize()
{
	int i,x,y;



	for(x=0;x<MAPWIDTH;x++)
	{
		for(y=0;y<MAPHEIGHT;y++)
		{
			Land(x,y)->kind = MAPKIND_WALL;
	
	
		}
	}


	for(i=0;i<MD_roomList.size();i++)
	{
		for(x=MD_roomList[i]->Left();x<=MD_roomList[i]->Right();x++)
		{
			for(y=MD_roomList[i]->Top();y<=MD_roomList[i]->Bottom();y++)
			{
				Land(x,y)->kind = MAPKIND_FLOOR;
	
	
			}
		}
	}
}