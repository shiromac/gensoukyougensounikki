#include "stdafx.h"
#include "cDungeonSystem.h"
#include "cRoom.h"

cRoom::cRoom(void)
{
	Index = -1;
	allisolated = 0;
	PartyFlag = 0;
	ShopFlag = 0;
}

cRoom::~cRoom(void)
{
}

//出口を再検索
//resetNeighborListをやってから();
void cRoom::resetExitList()
{
	int i;
	LandformExitList.clear();
	for(i=0;i<LandformNeighborList.size();i++)
	{
		if(LandformNeighborList[i]->through(0,0,0))
		{
			LandformExitList.push_back(LandformNeighborList[i]);
		}
	}

}

void cRoom::resetNeighborList()
{
	LandformNeighborList.clear();
	LandformEdgeList.clear();
	int k;
	int j;
	cCoordinate coo, a;
	bool memo[MAPWIDTH][MAPHEIGHT] = {0};
	bool memo2[MAPWIDTH][MAPHEIGHT] = {0};

	vector<pcLandform> landlist = allLandformList();

	for(k = 0;k < landlist.size();k++)
	{
		coo = landlist[k]->place;
		for(j=0;j<8;j++)
		{
			pcLandform land;
			a.SetAspect(j);
			a += coo;


			land = sg_pDungeonSystem->Map().Land(a);//隣接地形
			if(land->place.x < 0 || land->place.x >= MAPWIDTH
				|| land->place.y < 0 || land->place.y >= MAPHEIGHT)
			{//端っこ
				continue;
			}

			if(memo[land->place.x][land->place.y] == 1)continue;
			memo[land->place.x][land->place.y] = 1;//複重防止

			if(land->RoomIndex != Index )
			{	//部屋の中じゃない
				//部屋の隣接地形リストに追加
				LandformNeighborList.push_back(land);
				
				if(memo2[landlist[k]->place.x][landlist[k]->place.y] == 0)
				{
					memo2[landlist[k]->place.x][landlist[k]->place.y] = 1;//複重防止
					//部屋の地形縁リストに追加
					LandformEdgeList.push_back(landlist[k]);
				}
				
			}
		}
	}
	
	for(k = 0;k < landlist.size();k++)
	{
		if(memo2[landlist[k]->place.x][landlist[k]->place.y] == 0)
		{
			//部屋の地形非縁リストに追加
			LandformNonEdgeList.push_back(landlist[k]);
		}
	}
}

//地形領域リスト水路、浮島などの通常侵入不可領域を含めたすべての地形
vector<pcLandform> cRoom::allLandformList()
{
	vector<pcLandform> llist = LandformList;
	llist.insert(llist.end(),isolatedLandformList.begin(), isolatedLandformList.end());
	return llist;
}

//地形リストの整合性を取る
//地形のインデックスを優先する
int cRoom::stabilize()
{

	vector<pcLandform> newLandformList, newisolatedLandformList;
	
	int i;
	int size;

	size = LandformList.size();
	for(i=0;i<size;i++)
	{
		if(LandformList[i]->RoomIndex == Index)
		{
			newLandformList.push_back(LandformList[i]);
		}
	}

	size = isolatedLandformList.size();
	for(i=0;i<size;i++)
	{
		if(isolatedLandformList[i]->RoomIndex == Index)
		{
			newisolatedLandformList.push_back(isolatedLandformList[i]);
		}
	}

	LandformList = newLandformList;
	isolatedLandformList = newisolatedLandformList;

	return true;
}

//部屋の統合
int cRoom::compose(cRoom& anotherroom)
{
	if(this == &anotherroom) return false;
	
	
	int i;
	int size;

	size = anotherroom.LandformList.size();
	for(i=0;i<size;i++)
	{
		anotherroom.LandformList[i]->RoomIndex = Index;
	}
	LandformList.insert(LandformList.end(), anotherroom.LandformList.begin(), anotherroom.LandformList.end());
	anotherroom.LandformList.clear();

	size = anotherroom.isolatedLandformList.size();
	for(i=0;i<size;i++)
	{
		anotherroom.isolatedLandformList[i]->RoomIndex = Index;
	}
	isolatedLandformList.insert(isolatedLandformList.end(), anotherroom.isolatedLandformList.begin(), anotherroom.isolatedLandformList.end());
	anotherroom.isolatedLandformList.clear();


	PartyFlag |= anotherroom.PartyFlag;
	ShopFlag |= anotherroom.ShopFlag;
	return true;
}

//Change
int cRoom::ChangeIndex(int toindex)
{
	int size;
	int i;

	Index = toindex;

	size = LandformList.size();
	for(i=0;i<size;i++)
	{
		LandformList[i]->RoomIndex = Index;
	}

	size = isolatedLandformList.size();
	for(i=0;i<size;i++)
	{
		isolatedLandformList[i]->RoomIndex = Index;
	}


	return true;
}