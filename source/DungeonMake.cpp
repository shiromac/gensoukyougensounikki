#include "stdafx.h"

#include "cFloor.h"
#include "cMap.h"

#include "DungeonMake.h"

#include "utility/debug.h"
#include "utility/cDiscreteProbability.h"

#define SPLIT_HORIZON 0
#define SPLIT_VERTIC 1

#define MINIMUM_ROOM_SIZE 3
#define MARGIN_BETWEEN_RECT_ROOM 2
#define MINIMUM_RECT_SIZE (MINIMUM_ROOM_SIZE + (MARGIN_BETWEEN_RECT_ROOM * 2))

#define DM_INDEX_UNDEFINE (-2)
#define DM_INDEX_WAY (-1)

#define MAPMAKINGEDGELANDWIDTH (EDGELANDWIDTH+5)

#define MINIMUM_ROOM_SIZE 3
#define MINIMUM_ROOM_SIZE 3

#define DIVIDEVALDEF(index,defaultval,minval,maxval) \
if(divideVal.size() < (index)+1) divideVal.resize((index)+1,(defaultval));\
divideVal[(index)] = max((minval),divideVal[(index)]);\
divideVal[(index)] = min((maxval),divideVal[(index)]);


#define VALDEF(index,defaultval,minval,maxval) \
if(Val.size() < (index)+1) Val.resize((index)+1,(defaultval));\
Val[(index)] = max((minval),Val[(index)]);\
Val[(index)] = min((maxval),Val[(index)]);

pcDMMap pdmmap = pcDMMap((cDMMap*)NULL);

int cMap::makeNewDungeon(pcFloor pfloor)
{
	/*
	MD_rectList.clear();
	MD_roomList.clear();
	MD_lineList.clear();
	MD_pairList.clear();
	MD_waterlineList.clear();


	MD_rect_split(MD_rect_add(1, 1, MAPWIDTH - 2, MAPHEIGHT - 2));

	MD_water_make();

	MD_room_make();
	
	
	MD_couple_more();


	MD_pass_make();


	MD_data_optimize();
	*/



	/*
	c4DVector a(0,0,0,0),b(1,0,0,0),c(0,1,0,0),d(1,1,0,0);
	if(cross_Judge_Flat( a, b, c, d))
	{
		return 0;
	
	}
	*/		



	//------------------------------
	//実験場
	//------------------------------
	vector<int> val;
	if(pdmmap == NULL)
	{
		pdmmap = pcDMMap(new cDMMap);
		pdmmap->Init(pdmmap);
	}
	else
	{
		pdmmap->Reset();
	}

	
	val.clear();

	if(pfloor->floorMapGenForm() == FLOORMAPGENFORM_RANDOM)
	{
		int dungeontype = pfloor->dungeonDP().get(random());
		pdmmap->MakeRect(
			dungeontype,
			pfloor->dungeonRandomVal()[dungeontype]);
		val.clear();
		pdmmap->RectOptimaze(pfloor->dungeonFormVal()[_T("部屋数規定")]);
		val.clear();
		pdmmap->Makeroom(pfloor->dungeonFormVal()[_T("部屋サイズ規定")]);
		val.clear();
		pdmmap->RoomOptimaze(pfloor->dungeonFormVal()[_T("部屋形態規定")]);
		val.clear();
		pdmmap->Makelink(pfloor->dungeonFormVal()[_T("通路数規定")]);
		val.clear();
		pdmmap->makeWay(pfloor->dungeonFormVal()[_T("")]);
		val.clear();
		pdmmap->MakeWater(pfloor->dungeonFormVal()[_T("水路量規定")]);
		val.clear();
		pdmmap->MakeAbyss(pfloor->dungeonFormVal()[_T("奈落量規定")]);
		val.clear();
		pdmmap->MakeParty(pfloor->dungeonFormVal()[_T("宴会量規定")]);
		val.clear();
		pdmmap->MakeShop(pfloor->dungeonFormVal()[_T("店量規定")]);

		pdmmap->Output();	
	}
	else if(pfloor->floorMapGenForm() == FLOORMAPGENFORM_CONST)
	{
		pdmmap->decode_floor(pfloor->floormap());
	}
	else if(pfloor->floorMapGenForm() == FLOORMAPGENFORM_SHUFFLE)
	{
		cDiscreteProbability DP;
		map<tstring,double>& possibility = pfloor->sufflefloorpossiblity();
		map<tstring,double>::iterator itr = possibility.begin();
		vector<tstring> namelist;
		int i = 0;
		for(;itr!=possibility.end();++itr)
		{
			namelist.push_back(itr->first);
			DP.set(itr->second, i);
			++i;
		}

		if(DP.empty())
		{
			OnAssert(_T(__FILE__),__LINE__,false,_T("SHUFFLEフロアでひとつも候補がありません！"));
		}
		else
		{
			map<tstring,vector<vector<tstring>>>& sufflefloormap = pfloor->sufflefloormap();
			map<tstring,vector<vector<tstring>>>::iterator finditr = sufflefloormap.find(namelist[DP.get(random())]);
			pdmmap->decode_floor(finditr->second);
		}
	}



	resetAllLandNeighbor();
	
	


	return true;
}

//区画を追加
ps_rect cMap::MD_rect_add(int lx, int ly, int hx, int hy)
{
	ps_rect rect;
	rect = ps_rect(new s_rect);
	MD_rectList.push_back(rect);

	rect->done_split_h = 0;
	rect->done_split_v = 0;
	rect->rect.setLTRB(lx,ly,hx,hy);

	return rect;
}

//新規部屋領域
pcRectObj cMap::MD_room_add(int lx, int ly, int hx, int hy)
{
	pcRectObj rect;
	rect = pcRectObj(new cRectObj);
	MD_roomList.push_back(rect);

	rect->setLTRB(lx,ly,hx,hy);
	return rect;
}

//新規ペア
ps_couple cMap::MD_couple_add(int v_or_h, ps_rect rect0, ps_rect rect1)
{
	ps_couple couple;
	couple = ps_couple(new s_couple);
	MD_pairList.push_back(couple);
	couple->rect0 = rect0;
	couple->rect1 = rect1;
	couple->v_or_h = v_or_h;

	return couple;
}

//部屋作成
void cMap::MD_room_make()
{
	int i;
	cRectObj rect;
	int x, y, w, h;
	int max,min;

	for (i = 0; i < MD_rectList.size(); i++)
	{
		rect = MD_rectList[i]->rect;
		//w = random_range(MINIMUM_ROOM_SIZE, rect.Right() - rect.Left() - (MARGIN_BETWEEN_RECT_ROOM * 2) );
		max = rect.Right() - rect.Left() - (MARGIN_BETWEEN_RECT_ROOM * 2) ;
		min = MINIMUM_ROOM_SIZE;
		if((max - min) == 0) w = min;
		else w = max - pow(max - random_range(min,max), 2)/(max - min);


		//w = rect.Right() - rect.Left() - (MARGIN_BETWEEN_RECT_ROOM * 2);
		//a = random_range(MINIMUM_ROOM_SIZE, rect.Right() - rect.Left() - (MARGIN_BETWEEN_RECT_ROOM * 2) );
		//w = (w+a+1)/2;
		
		//h = random_range(MINIMUM_ROOM_SIZE, rect.Bottom() - rect.Top() - (MARGIN_BETWEEN_RECT_ROOM * 2) );
		max = rect.Bottom() - rect.Top() - (MARGIN_BETWEEN_RECT_ROOM * 2)  ;
		min = MINIMUM_ROOM_SIZE;
		if((max - min) == 0) h = min;
		else h = max - pow(max - random_range(min,max), 2)/(max - min);

		//h = rect.Bottom() - rect.Top() - (MARGIN_BETWEEN_RECT_ROOM * 2);
		//a = random_range(MINIMUM_ROOM_SIZE, rect.Bottom() - rect.Top() - (MARGIN_BETWEEN_RECT_ROOM * 2) );
		//h = (h+a+1)/2;

		if(MD_roomList.size() > 3 && ((w == MINIMUM_ROOM_SIZE && h <= MINIMUM_ROOM_SIZE + 3) || (h == MINIMUM_ROOM_SIZE || w <= MINIMUM_ROOM_SIZE + 3)) && random() < 0.3)
		{//通路化
			w = 0;
			h = 0;
		}

		x = random_range(rect.Left() + MARGIN_BETWEEN_RECT_ROOM, rect.Right() - MARGIN_BETWEEN_RECT_ROOM - w + 1);
		y = random_range(rect.Top() + MARGIN_BETWEEN_RECT_ROOM, rect.Bottom() - MARGIN_BETWEEN_RECT_ROOM - h + 1);
		
		MD_rectList[i]->room = MD_room_add(x, y, x + w, y + h);
	}
}

//区画区切り
void cMap::MD_rect_split(ps_rect rect_parent)
{
	ps_rect rect_child;

	if (rect_parent->rect.Bottom() - rect_parent->rect.Top() <= MINIMUM_RECT_SIZE * 2)
	{
		rect_parent->done_split_v = TRUE;
	}
	if (rect_parent->rect.Right() - rect_parent->rect.Left() <= MINIMUM_RECT_SIZE * 2)
	{
		rect_parent->done_split_h = TRUE;
	}

	if ((MD_rectList.size() > 1) && (random()*64 < 1))
	{//区切り終了率
		rect_parent->done_split_v = TRUE;
		rect_parent->done_split_h = TRUE;
	}
	if ((rect_parent->done_split_v) && (rect_parent->done_split_h)) 
	{
		return;
	}

	//複製
	rect_child = MD_rect_add(rect_parent->rect.Left(), rect_parent->rect.Top(),
							rect_parent->rect.Right(), rect_parent->rect.Bottom());


	if (rect_parent->done_split_v == FALSE)
	{
		int split_coord_y;
		split_coord_y = random_range(rect_parent->rect.Top() + MINIMUM_RECT_SIZE,
											rect_parent->rect.Bottom() - MINIMUM_RECT_SIZE);
		rect_parent->rect.formBottom(split_coord_y);
		rect_child->rect.formTop(split_coord_y);

		rect_parent->done_split_v = TRUE;
		rect_child->done_split_v = TRUE;
		MD_couple_add(SPLIT_VERTIC, rect_parent, rect_child);

		if(random()*2 < 1)
		{
			MD_rect_split(rect_parent);
			MD_rect_split(rect_child);
		}
		else
		{
			MD_rect_split(rect_child);
			MD_rect_split(rect_parent);
		}
		return;
	}
	if (rect_parent->done_split_h == FALSE)
	{
		int split_coord_x;
		split_coord_x = random_range(rect_parent->rect.Left() + MINIMUM_RECT_SIZE,
											rect_parent->rect.Right() - MINIMUM_RECT_SIZE);
		rect_parent->rect.formRight(split_coord_x);
		rect_child->rect.formLeft(split_coord_x);

		rect_parent->done_split_h = TRUE;
		rect_child->done_split_h = TRUE;
		MD_couple_add(SPLIT_HORIZON, rect_parent, rect_child);

		if(random()*2 < 1)
		{
			MD_rect_split(rect_parent);
			MD_rect_split(rect_child);
		}
		else
		{
			MD_rect_split(rect_child);
			MD_rect_split(rect_parent);
		}
		return;
	};





}

void cMap::MD_couple_more()
{

	ps_rect rect;
	ps_rect rectmap[MAPWIDTH][MAPHEIGHT];
	int count=0;

	int i, j, k;
	for (k = 0;k<MD_rectList.size();k++)
	{
		rect = MD_rectList[k];
		for (i = rect->rect.Left(); i < rect->rect.Right(); i++)
		{
			for (j = rect->rect.Top(); j < rect->rect.Bottom(); j++)
			{
      			rectmap[i][j] = rect;
			}
		}
	}
	for (i = 0; i < MAPWIDTH - 2; i++)
	{
		for (j = 0; j < MAPHEIGHT - 2; j++)
		{
			if(rectmap[i][j] == NULL)continue;
			if(rectmap[i + 1][j] == NULL)continue;
			if(rectmap[i][j + 1] == NULL)continue;

			if (rectmap[i][j] != rectmap[i][j + 1]) {
				pcRectObj room1 = rectmap[i][j]->room;
				pcRectObj room2 = rectmap[i][j]->room;

				if (
					(
					(
					(room1->Width*room1->Height == 0)||(room2->Width*room2->Height == 0)
					)&&
					(random_range(0, (count-1)*6) < 2)
					)
					||(random_range(0, count*40) < 2)) {


					for(k = 0;k < MD_pairList.size();k++)
					{
						

						if(
							(	((MD_pairList[k]->rect0->rect.Left()) == (rectmap[i][j]->rect.Left()))	&&
								((MD_pairList[k]->rect1->rect.Left()) == (rectmap[i][j + 1]->rect.Left()))	&&
								((MD_pairList[k]->rect0->rect.Right()) == (rectmap[i][j]->rect.Right()))	&&
								((MD_pairList[k]->rect1->rect.Right()) == (rectmap[i][j + 1]->rect.Right()))	)
							||
							(	((MD_pairList[k]->rect0->rect.Left()) == (rectmap[i][j + 1]->rect.Left())) &&
								((MD_pairList[k]->rect1->rect.Left()) == (rectmap[i][j]->rect.Left()))		&&
								((MD_pairList[k]->rect0->rect.Right()) == (rectmap[i][j + 1]->rect.Right()))	&&
								((MD_pairList[k]->rect1->rect.Right()) == (rectmap[i][j]->rect.Right()))	)
								)
						{
							break;		
						}
					}
					if(k == MD_pairList.size())
					{
						count++;
						MD_couple_add(SPLIT_VERTIC, rectmap[i][j], rectmap[i][j + 1]);
					}
				}
			}
			if (rectmap[i][j] != rectmap[i + 1][j]) {
				pcRectObj room1 = rectmap[i][j]->room;
				pcRectObj room2 = rectmap[i][j]->room;

				if (
					(
					(
					(room1->Width*room1->Height == 0)||(room2->Width*room2->Height == 0)
					)&&
					(random_range(0, (count-1)*6) < 2)
					)
					||(random_range(0, count*40) < 2)) {
					for(k = 0;k < MD_pairList.size();k++)
					{
						

						if(
							(	((MD_pairList[k]->rect0->rect.Left()) == (rectmap[i][j]->rect.Left()))	&&
								((MD_pairList[k]->rect1->rect.Left()) == (rectmap[i + 1][j]->rect.Left()))	&&
								((MD_pairList[k]->rect0->rect.Right()) == (rectmap[i][j]->rect.Right()))	&&
								((MD_pairList[k]->rect1->rect.Right()) == (rectmap[i + 1][j]->rect.Right()))	)
							||
							(	((MD_pairList[k]->rect0->rect.Left()) == (rectmap[i + 1][j]->rect.Left())) &&
								((MD_pairList[k]->rect1->rect.Left()) == (rectmap[i][j]->rect.Left()))		&&
								((MD_pairList[k]->rect0->rect.Right()) == (rectmap[i + 1][j]->rect.Right()))	&&
								((MD_pairList[k]->rect1->rect.Right()) == (rectmap[i][j]->rect.Right()))	)
								)
						{
							break;		
						}
					}
					if(k == MD_pairList.size())
					{
						count++;
						MD_couple_add(SPLIT_HORIZON, rectmap[i][j], rectmap[i + 1][j]);
					}
				}
			}
		}
	}
}

void cMap::MD_water_make()
{
	cCoordinate c0,c1;

	int i;
	
	for(i=0;i<20;i++)
	{
		c0.x = random_range(1, MAPWIDTH - 1);
		c0.y = random_range(1,MAPHEIGHT - 1);

		if(random()*2<1)
		{
			c1.x = random_range(1, MAPWIDTH - 1);
			c1.y = c0.y;
		}
		else
		{
			c1.x = c0.x;
			c1.y = random_range(1,MAPHEIGHT - 1);
		}
		MD_pass_link(c0, c1,&MD_waterlineList);
	}
}

void cMap::MD_pass_make()
{
	int i;
	cCoordinate c0, c1, ct;
	ps_couple couple;

	for(i=0;i<MD_pairList.size();i++)
	{
		couple = MD_pairList[i];

		switch (couple->v_or_h) {
		case SPLIT_HORIZON:
			//if(couple->rect0->rect.Right() == couple->rect1->rect.Left())
			{
				c0.x = couple->rect0->rect.Right();
				c0.y = random_range(couple->rect0->room->Top() + 1, couple->rect0->room->Bottom());
				c1.x = couple->rect1->rect.Left();
				c1.y = random_range(couple->rect1->room->Top() + 1, couple->rect1->room->Bottom());
				
				MD_pass_link(c0, c1);

				ct.x = couple->rect0->room->Right();
				ct.y = c0.y;
				MD_pass_link(ct, c0);

				ct.x = couple->rect1->room->Left();
				ct.y = c1.y;
				MD_pass_link(ct, c1);
			}
			break;
		case SPLIT_VERTIC:
			//if(couple->rect0->rect.Bottom() == couple->rect1->rect.Top())
			{
				c0.x = random_range(couple->rect0->room->Left() + 1, couple->rect0->room->Right());
				c0.y = couple->rect0->rect.Bottom();
				c1.x = random_range(couple->rect1->room->Left() + 1, couple->rect1->room->Right());
				c1.y = couple->rect1->rect.Top();

				MD_pass_link(c0, c1);
			
				ct.x = c0.x;
				ct.y = couple->rect0->room->Bottom();
				MD_pass_link(ct, c0);

				ct.x = c1.x;
				ct.y = couple->rect1->room->Top();
				MD_pass_link(ct, c1);
			}
			break;
		};
	}
}

void cMap::MD_pass_link(cCoordinate start,cCoordinate end, vector<pcRectObj>* linelist)
{
	pcRectObj line = pcRectObj(new cRectObj);

	line->setLTRB(min(start.x, end.x),
					min(start.y, end.y),
					max(start.x, end.x),
					max(start.y, end.y)
		);
	if(line->Width * line->Height)
	{
		return;
	}

	if(linelist != NULL)
	{
		linelist->push_back(line);
	}
	else
	{
		MD_lineList.push_back(line);
	}
	
}


void cMap::MD_data_optimize()
{
	int i,j,k,x,y;
	cCoordinate coo,a;
	pcLandform land;
	int memo[MAPWIDTH][MAPHEIGHT] = {0};

	//壁で埋める
	for(x=0;x<MAPWIDTH;x++)
	{
		for(y=0;y<MAPHEIGHT;y++)
		{
			Land(x,y)->setAttribute(MAPKIND_WALL);
			Land(x,y)->RoomIndex = -1;
	
		}
	}

	//水路掘る
	for(i=0;i<MD_waterlineList.size();i++)
	{
		for(x=MD_waterlineList[i]->Left();x<MD_waterlineList[i]->Right()+1;x++)
		{
			for(y=MD_waterlineList[i]->Top();y<MD_waterlineList[i]->Bottom()+1;y++)
			{
				if(Land(x,y)->getAttribute() == MAPKIND_WALL)
					Land(x,y)->setAttribute(MAPKIND_WATER);
	
			}
		}
	}

	//通路掘る
	for(i=0;i<MD_lineList.size();i++)
	{
		for(x=MD_lineList[i]->Left();x<MD_lineList[i]->Right()+1;x++)
		{
			for(y=MD_lineList[i]->Top();y<MD_lineList[i]->Bottom()+1;y++)
			{
				if(Land(x,y)->getAttribute() == MAPKIND_WALL || Land(x,y)->getAttribute() == MAPKIND_WATER)
					Land(x,y)->setAttribute(MAPKIND_FLOOR2);
	
			}
		}
	}



	//部屋リストクリア
	RoomList.clear();
	//部屋を空ける
	pcRoom room;
	int roomindex = 0;
	for(i=0;i<MD_roomList.size();i++)
	{

		//部屋の大きさが無い
		if(MD_roomList[i]->Height*MD_roomList[i]->Width == 0)
		{
			//通路にする
			Land(MD_roomList[i]->CenterX,MD_roomList[i]->CenterY)->setAttribute(MAPKIND_FLOOR2);
			continue;
		}

		room = pcRoom(new cRoom);
		RoomList.push_back(room);
		room->Index = roomindex;


		for(x=MD_roomList[i]->Left();x<=MD_roomList[i]->Right();x++)
		{
			for(y=MD_roomList[i]->Top();y<=MD_roomList[i]->Bottom();y++)
			{
				Land(x,y)->setAttribute(MAPKIND_FLOOR);
				Land(x,y)->RoomIndex = roomindex;//部屋番号
				//部屋の地形リストに追加
				room->LandformList.push_back(Land(x,y));
	
			}
		}

		for(k = 0;k < room->LandformList.size();k++)
		{
			coo = room->LandformList[k]->place;
			for(j=0;j<8;j++)
			{
				a.SetAspect(j);
				a += coo;
				land = Land(a);//隣接地形
				if(memo[land->place.x][land->place.y] == 1)continue;
				memo[land->place.x][land->place.y] = 1;//複重防止

				if(land->RoomIndex != roomindex )
				{	//部屋の中じゃない
					//部屋の隣接地形リストに追加
					room->LandformNeighborList.push_back(land);
					
				}
	
			}
		}
		
		//出口リストを作る。
		room->resetExitList();

		roomindex++;
	}


	


}


//***************************************************************
//
//　新システム
//
//***************************************************************

const int DEF_RECTUSE_FREE = 0;
const int DEF_RECTUSE_ROOM = 1;
const int DEF_RECTUSE_Way = 2;//通路

const int DEF_ROOMMODE_Way = -1;//通路
const int DEF_ROOMMODE_NORMAL = 0;
const int DEF_ROOMMODE_DIVIDE2WATER = 1;//水路二分部屋
const int DEF_ROOMMODE_DIVIDE4WATER = 2;//水路四分部屋
const int DEF_ROOMMODE_OVAL = 3;//丸
const int DEF_ROOMMODE_NoizeOval = 4;//ノイズ丸
//const int DEF_ROOMMODE_ = 1;

void cDMMap::Init(pcDMMap selfpointer)
{
	selfwp_ = wpcDMMap(selfpointer);
	
	landmap_edge = pcDMLand(new cDMLand);

	int size = MAPWIDTH*MAPHEIGHT;
	int i,x,y;
	for(i=0;i<size;i++)
	{
		landmap_.push_back(pcDMLand(new cDMLand));
	}

	ResetSub_Lands();
}

void cDMMap::ResetSub_Lands()
{
	int i,x,y;

	for(x = 0 ; x < MAPWIDTH; x++)
	{
		for(y = 0 ; y < MAPHEIGHT; y++)
		{
			landmap(x,y)->Init();
			landmap(x,y)->pos.set(x,y,0,0);
			landmap(x,y)->rectFrag = MAPKIND_WALL;
			if(x < EDGELANDWIDTH || x >= MAPWIDTH - EDGELANDWIDTH
				|| y < EDGELANDWIDTH || y >= MAPHEIGHT - EDGELANDWIDTH)
			{//エッジ
				landmap(x,y)->rectFrag = MAPKIND_WALLEGDE;
			}
		}
	}

	landmap_edge->Init();
	landmap_edge->pos.set(-1,-1,0,0);
	landmap_edge->rectFrag = MAPKIND_WALLEGDE;
}

void cDMMap::Reset()
{
	ResetSub_Lands();

	Rects_.clear();
	Room_.clear();
	mustlink.clear();
	linkpair_.clear();
	RoomNum = 0;
}
pcDMLand cDMMap::landmap(int x, int y)
{
	int index = x + y * MAPWIDTH;
	if(index < 0 || index >= landmap_.size()) return landmap_edge;
	return landmap_[index];
}


void cDMMap::MakeRect(int divideMode, vector<double>& divideVal)
{

	switch(divideMode)
	{
	case RANDOMDIVIDE:
		MakeRect_RandomDivideRoop(divideVal);
		break;
	case SIMPLEGRID:
		MakeRect_SimpleGrid(divideVal);
		break;
	case BIGRINGANDGRID:
		MakeRect_BigringAndgrid(divideVal);
		break;
	case NETANDANCHOR:
		MakeRect_NetAndAnchor(divideVal);
		break;
	case HABANDSATELLITE:
		MakeRect_HabAndSatellite(divideVal);
		break;
	case BIGONEROOM:
		MakeRect_BigOneRoom(divideVal);
		break;
	case TWOROOM:
		MakeRect_TwoRoom(divideVal);
		break;
	case FOURROOM:
		MakeRect_FourRoom(divideVal);
		break;
	case RANDOMGROW:
		MakeRect_RandomGrow(divideVal);
		break;
	case RANDOMGRIDUNION:
		MakeRect_RandomGridUnion(divideVal);
		break;
	default:
		OnAssert(_T(__FILE__),__LINE__,false,_T("MakeRect::生成ダンジョンのレクト分割方式が定義されてないインデックスです。<error> [MakeRect] illegality rule_index"));
		MakeRect_RandomDivideRoop(divideVal);
		break;
	}

}


void cDMMap::RectOptimaze(vector<double>& Val)
{
	if(Val.size() < 1) Val.resize(1,0);
	//最低部屋数
	Val[0] = max(1,Val[0]);
	
	if(Val.size() < 2) Val.resize(2,6);
	//最少部屋数期待値(下回る場合もあります)
	Val[1] = max(1,Val[1]);

	if(Val.size() < 3) Val.resize(3,16);
	//最大部屋数期待値(超える場合もあります)
	Val[2] = max(1,Val[2]);


	double roomnumE = random_range(Val[1], Val[2]+1);
	double roomPos = roomnumE/Rects_.size();

	//シャッフルする
	random_shuffle(Rects_.begin(),Rects_.end(),random_int);


	int i;
	int RoomNum = 0;
	for(i=0;i<Rects_.size();i++)
	{
		
		if(Rects_[i]->Flag_ == DEF_RECTUSE_FREE)
		{//自由
			if(RoomNum < Val[0])
			{
				Rects_[i]->Flag_ = DEF_RECTUSE_ROOM;
				RoomNum++;
			}
			else
			{
				if(random() < roomPos)
				{
					Rects_[i]->Flag_ = DEF_RECTUSE_ROOM;
					RoomNum++;
				}
				else
				{
					Rects_[i]->Flag_ = DEF_RECTUSE_Way;
				}
			}
		
		}
		else if(Rects_[i]->Flag_ == DEF_RECTUSE_ROOM)
		{
			RoomNum++;
		}
		else if(Rects_[i]->Flag_ == DEF_RECTUSE_Way)
		{
		
		}
	}

	OnAssert(_T(__FILE__),__LINE__,(RoomNum>0),_T("ダンジョンの部屋の数がひとつもありません。<error> The Dungeon no room"));
}

const int MINROOMSIZE = 4;
//第三過程
void cDMMap::Makeroom(vector<double>& Val)
{
	if(Val.size() < 1) Val.resize(1,0);
	//最小余白マス割合(百分率)(((部屋最大拡張サイズ-部屋拡張のサイズ)/部屋最大拡張サイズ) の最小)
	Val[0] = max(0,Val[0]);
	Val[0] = min(100,Val[0]);
	if(Val.size() < 2) Val.resize(2,90);
	//最大余白マス割合(百分率)(((部屋最大拡張サイズ-部屋拡張のサイズ)/部屋最大拡張サイズ) の最大)
	Val[1] = max(0,Val[1]);
	Val[1] = min(100,Val[1]);
	if(Val.size() < 3) Val.resize(3,20);
	//最大余白マス数((部屋最大拡張サイズ-部屋拡張のサイズ) の最大)
	Val[2] = max(0,Val[2]);
	Val[2] = min(54,Val[2]);

	int i;
	int roomIndex = 0;
	for(i=0;i<Rects_.size();i++)
	{
		pcDMRoom proom = pcDMRoom(new cDMRoom(selfwp_));
		Room_.push_back(proom);
		proom->pArea = Rects_[i];
		Rects_[i]->wproom_ = wpcDMRoom(proom);
		proom->wpSelfroom_ = wpcDMRoom(proom);
		proom->RectIndex = i;

		if(Rects_[i]->Flag_ == DEF_RECTUSE_ROOM)
		{
			proom->RoomMode = DEF_ROOMMODE_NORMAL;
			proom->RoomIndex = roomIndex;
			int w_yuuyo = proom->pArea->Width - MINROOMSIZE - 2;
			int h_yuuyo = proom->pArea->Height - MINROOMSIZE - 2;
			proom->Width = (int)max( (w_yuuyo*(1-random_range(Val[0]/100.0,Val[1]/100.0))) , w_yuuyo - Val[2]) + MINROOMSIZE;
			proom->Height = (int)max( (h_yuuyo*(1-random_range(Val[0]/100.0,Val[1]/100.0))) , h_yuuyo - Val[2]) + MINROOMSIZE;
			roomIndex++;
		}
		else if(Rects_[i]->Flag_ == DEF_RECTUSE_Way)
		{//通路
			proom->RoomMode = DEF_ROOMMODE_Way;
			proom->RoomIndex = DM_INDEX_WAY;
			proom->Width = 1;
			proom->Height = 1;
		}
		else
		{
			OnAssert(_T(__FILE__),__LINE__,false,_T("Makeroom::レクトの用途フラグが不正な値です。<error> [Makeroom] illegality flag"));
		}
		int r_yuuyo = proom->pArea->Width - proom->Width - 2;
		int t_yuuyo = proom->pArea->Height - proom->Height - 2;
		proom->setLeft((int)(proom->pArea->Left() + r_yuuyo*random()+ 1));
		proom->setTop((int)(proom->pArea->Top() + t_yuuyo*random()+ 1));	
	}

	RoomNum = roomIndex;
}


//第四過程
void cDMMap::RoomOptimaze(vector<double>& Val)
{
	if(Val.size() < 1) Val.resize(1,100);
	//通常部屋
	Val[0] = max(0,Val[0]);
	
	if(Val.size() < 2) Val.resize(2,5);
	//水路２分部屋
	Val[1] = max(0,Val[1]);

	if(Val.size() < 3) Val.resize(3,2);
	//水路４分部屋
	Val[2] = max(0,Val[2]);

	if(Val.size() < 4) Val.resize(4,20);
	//丸部屋
	Val[3] = max(0,Val[3]);

	if(Val.size() < 5) Val.resize(5,0);
	//ノイズ円部屋
	Val[4] = max(0,Val[4]);

	int i;
	for(i=0;i<Room_.size();i++)
	{
		if(Room_[i]->RoomMode == DEF_ROOMMODE_Way)
		{
			Room_[i]->RoomOptimaze_Way(Val);
		}
		else
		{
			cDiscreteProbability DP;
			if(Room_[i]->pArea->NeverDivide)
			{//分けないで！
				DP.set(Val[0],DEF_ROOMMODE_NORMAL);
				DP.set(Val[3],DEF_ROOMMODE_OVAL);
				DP.set(Val[4],DEF_ROOMMODE_NoizeOval);
			}
			else
			{
				DP.set(Val[0],DEF_ROOMMODE_NORMAL);
				DP.set(Val[1],DEF_ROOMMODE_DIVIDE2WATER);
				DP.set(Val[2],DEF_ROOMMODE_DIVIDE4WATER);
				DP.set(Val[3],DEF_ROOMMODE_OVAL);
				DP.set(Val[4],DEF_ROOMMODE_NoizeOval);
			}

			int RoomMode = DP.get(random());

			if(RoomMode == DEF_ROOMMODE_NORMAL)
			{
				Room_[i]->RoomOptimaze_Normal(Val);
			}
			else if(RoomMode == DEF_ROOMMODE_DIVIDE2WATER)
			{
				Room_[i]->RoomOptimaze_Div2water(Val);
			}
			else if(RoomMode == DEF_ROOMMODE_DIVIDE4WATER)
			{
				Room_[i]->RoomOptimaze_Div4water(Val);
			}
			else if(RoomMode == DEF_ROOMMODE_OVAL)
			{
				Room_[i]->RoomOptimaze_Oval(Val);
			}
			else if(RoomMode == DEF_ROOMMODE_NoizeOval)
			{
				Room_[i]->RoomOptimaze_NoizeOval(Val);
			}
			else
			{
				OnAssert(_T(__FILE__),__LINE__,false,_T("cDMMap::RoomOptimaze関数内で異常な選択が発生しています。"));
			}
		}

	}


}
void cDMRoom::RoomOptimaze_Normal( vector<double>& Val)
{
	pcDMMap pdmmap = wpmap_.lock();
	pcDMWallround wallround = pcDMWallround(new cDMWallround(wpmap_));
	wallround_.push_back(wallround);

	wallround->WallroundIndex = 0;
	wallround->wproom_ = wpSelfroom_;

	int i;
	for(i = 0 ; i < 8; i++)
	{
		direction_wallround_[i] = wallround;
		direction_wallround_[i]->directionFlag[i] = true;
	}

	int x,y;
	for(x = Left() ; x < Right(); x++)
	{
		for(y = Top() ; y < Bottom(); y++)
		{//ランド最適化
			pdmmap->landmap(x,y)->roomIndex = RoomIndex;
			pdmmap->landmap(x,y)->Landflag = MAPKIND_FLOOR;
			//pdmmap->landmap(x,y)->wallroundIndex = ;
			land_.push_back( pdmmap->landmap(x,y) );
		}
	}

	//pcDMLand pland = land_[0];

	wallround->WallroundOptimaze(land_[0]);

	/*
	cCoordinate coo;
	int aspect = 0;

	for(;;)
	{//端っこサーチ
		coo.SetAspect(aspect);
		pcDMLand pforw = pdmmap->landmap( pland->pos.x + coo.x, pland->pos.y + coo.y);
		if(pforw->roomIndex == DM_INDEX_UNDEFINE)
		{//正面がはしっこ
			break;
		}
		pland = pforw;
	}
	aspect += -2;//左を向く
	for(;;)
	{
		pcDMLand pforw,pright,pdiag,pright2;
		coo.SetAspect(aspect);
		pforw = pdmmap->landmap( pland->pos.x + coo.x, pland->pos.y + coo.y);
		coo.SetAspect(aspect+1);
		pdiag = pdmmap->landmap( pland->pos.x + coo.x, pland->pos.y + coo.y);
		coo.SetAspect(aspect+2);
		pright = pdmmap->landmap( pland->pos.x + coo.x, pland->pos.y + coo.y);
		coo = coo*2;
		pright2 = pdmmap->landmap( pland->pos.x + coo.x, pland->pos.y + coo.y);
		

		if(pright->roomIndex == DM_INDEX_UNDEFINE)
		{//右手が壁

			if(pright->WallofwallroundIndex == wallround->WallroundIndex && pland->wallroundIndex == wallround->WallroundIndex)
			{//一周した
				break;
			}
			
			if(pright->WallofwallroundIndex != wallround->WallroundIndex)
			{
				wallround->wall_.push_back(pright);
				pright->WallofwallroundIndex = wallround->WallroundIndex;
				
				pright->WallofroomIndex = RoomIndex;
				pdiag->WallofroomIndex = RoomIndex;
				pright->WallofrectIndex = RectIndex;
				pdiag->WallofrectIndex = RectIndex;
				
			}

			if(pland->wallroundIndex != wallround->WallroundIndex)
			{
				wallround->land_.push_back(pland);
				
			}


			wallround->walloutland_.push_back(pright2);

			//test
			//pright->Landflag = MAPKIND_WATER;
			//pdiag->Landflag = MAPKIND_WATER;
			//pright2->Landflag = MAPKIND_WATER;

		}
		else
		{//壁から離れたので右に曲がる
			aspect += 2;
			coo.SetAspect(aspect);
			pforw = pdmmap->landmap( pland->pos.x + coo.x, pland->pos.y + coo.y);
			coo.SetAspect(aspect+1);
			pdiag = pdmmap->landmap( pland->pos.x + coo.x, pland->pos.y + coo.y);
			coo.SetAspect(aspect+2);
			pright = pdmmap->landmap( pland->pos.x + coo.x, pland->pos.y + coo.y);
			coo = coo*2;
			pright2 = pdmmap->landmap( pland->pos.x + coo.x, pland->pos.y + coo.y);
		

		}

		if(pforw->roomIndex == DM_INDEX_UNDEFINE)
		{//正面が壁
			//左を向く
			aspect += -2;
			pdiag->WallofwallroundIndex = wallround->WallroundIndex;
			pland->wallroundIndex = wallround->WallroundIndex;
		}
		else
		{//一歩進む
			pland = pforw;
		}

	}
	*/

}

pcDMWallround cDMRoom::direction_wallround(double x,double y)
{
	cCoordinate coo;
	coo.x = x - CenterX;
	coo.y = y - CenterY;
	return direction_wallround_[coo.GetAspect()];
}

void cDMRoom::RoomOptimaze_Way( vector<double>& Val)
{
	pcDMMap pdmmap = wpmap_.lock();
	pcDMWallround wallround = pcDMWallround(new cDMWallround(wpmap_));
	wallround->wproom_ = wpSelfroom_;
	wallround->WallroundIndex = 0;
	int i;
	for(i = 0 ; i < 8; i++)
	{
		direction_wallround_[i] = wallround;
		direction_wallround_[i]->directionFlag[i] = true;
	}
	wallround_.push_back(wallround);
	int x,y;

	pdmmap->landmap(Left(),Top())->roomIndex = RoomIndex;
	pdmmap->landmap(Left(),Top())->Landflag = MAPKIND_FLOOR2;
	
	
	
	land_.push_back( pdmmap->landmap(Left(),Top()) );
	
	
	//wallround->WallroundOptimaze( pdmmap->landmap(Left(),Top()));

	
	wallround->land_.push_back(pdmmap->landmap(Left(),Top()));

	cCoordinate coo;
	int aspect;
	for(aspect =0;aspect<8;aspect++)
	{
		coo.SetAspect(aspect);

		pcDMLand pland = pdmmap->landmap(Left()+coo.x,Top()+coo.y);
		wallround->wall_.push_back(pland);
	

		pland->WallofwallroundIndex = wallround->WallroundIndex;
		pland->WallofroomIndex = wallround->wproom_.lock()->RoomIndex;
		pland->WallofrectIndex = wallround->wproom_.lock()->RectIndex;
	
	}

	for(aspect =0;aspect<8;aspect+=2)
	{
		coo.SetAspect(aspect);
		coo = coo*2;
		wallround->walloutland_.push_back(pdmmap->landmap(Left()+coo.x,Top()+coo.y));
	}


}

void cDMRoom::RoomOptimaze_Div2water(vector<double>& Val)
{
	pcDMWallround wallround;
	pcDMMap pdmmap = wpmap_.lock();
	cCoordinate coo;

	pcDMLand land,landA,landB;

	int x,y = 0;
	for(x = Left() ; x < Right(); x++)
	{
		for(y = Top() ; y < Bottom(); y++)
		{//ランド最適化
			pdmmap->landmap(x,y)->roomIndex = RoomIndex;
			pdmmap->landmap(x,y)->Landflag = MAPKIND_FLOOR;
			//pdmmap->landmap(x,y)->wallroundIndex = ;
			land_.push_back( pdmmap->landmap(x,y) );
		}
	}



	//分割領域決定
	int aspectA, aspectB;
	aspectA = 8*random();
	aspectB = aspectA + 2+2*(int)(3*random());

	int i;

	//水路を引く
	int face;
	
	face = safeAspect( aspectA )/2;

	if(face == 0)
	{//上
		x = random_range(Left()+1,Right()-1);
		y = Top();
	}
	else if(face == 1)
	{//右
		x = Right()-1;
		y = random_range(Top()+1,Bottom()-1);
	}
	else if(face == 2)
	{//下
		x = random_range(Left()+1,Right()-1);
		y = Bottom()-1;
	}
	else if(face == 3)
	{//左
		x = Left();
		y = random_range(Top()+1,Bottom()-1);
	}
	landA = pdmmap->landmap(x,y);
	landA->Landflag = MAPKIND_WATER;
	
	face = safeAspect( aspectB )/2;
	if(face == 0)
	{//上
		x = random_range(Left()+1,Right()-1);
		y = Top();
	}
	else if(face == 1)
	{//右
		x = Right()-1;
		y = random_range(Top()+1,Bottom()-1);
	}
	else if(face == 2)
	{//下
		x = random_range(Left()+1,Right()-1);
		y = Bottom()-1;
	}
	else if(face == 3)
	{//左
		x = Left();
		y = random_range(Top()+1,Bottom()-1);
	}
	landB = pdmmap->landmap(x,y);
	landB->Landflag = MAPKIND_WATER;

	coo.SetAspect((aspectA/2)*2+4);
	landA = pdmmap->landmap(landA->pos.x + coo.x,landA->pos.y + coo.y);
	landA->Landflag = MAPKIND_WATER;

	coo.SetAspect((aspectB/2)*2+4);
	landB = pdmmap->landmap(landB->pos.x + coo.x,landB->pos.y + coo.y);
	landB->Landflag = MAPKIND_WATER;

	makeWaterWay_inRoom(landA,landB);

	//位置関係記述
	wallround = pcDMWallround(new cDMWallround(wpmap_));
	wallround_.push_back(wallround);
	wallround->WallroundIndex = 0;
	wallround->wproom_ = wpSelfroom_;

	for(i = aspectA ; i < aspectB; i++)
	{
		int aspect = safeAspect(i);
		direction_wallround_[aspect] = wallround;
		direction_wallround_[aspect]->directionFlag[aspect] = true;
	}

	face = safeAspect( aspectA )/2;
	if(face == 0)
	{//右上
		land = pdmmap->landmap(Right()-1,Top());
	}
	else if(face == 1)
	{//右下
		land = pdmmap->landmap(Right()-1,Bottom()-1);
	}
	else if(face == 2)
	{//左下
		land = pdmmap->landmap(Left(),Bottom()-1);
	}
	else if(face == 3)
	{//左上
		land = pdmmap->landmap(Left(),Top());
	}
	wallround->WallroundOptimaze(land);



	wallround = pcDMWallround(new cDMWallround(wpmap_));
	wallround_.push_back(wallround);
	wallround->WallroundIndex = 1;
	wallround->wproom_ = wpSelfroom_;

	for(i = aspectB ; i < aspectA+8; i++)
	{
		int aspect = safeAspect(i);
		direction_wallround_[aspect] = wallround;
		direction_wallround_[aspect]->directionFlag[aspect] = true;
	}

	face = safeAspect( aspectB )/2;
	if(face == 0)
	{//右上
		land = pdmmap->landmap(Right()-1,Top());
	}
	else if(face == 1)
	{//右下
		land = pdmmap->landmap(Right()-1,Bottom()-1);
	}
	else if(face == 2)
	{//左下
		land = pdmmap->landmap(Left(),Bottom()-1);
	}
	else if(face == 3)
	{//左上
		land = pdmmap->landmap(Left(),Top());
	}
	wallround->WallroundOptimaze(land);


	/*
	//仮実装
	for(x = Left() ; x < Right(); x++)
	{
		pdmmap->landmap(x,CenterY)->Landflag = MAPKIND_WATER;
	}

	wallround = pcDMWallround(new cDMWallround(wpmap_));
	wallround_.push_back(wallround);

	wallround->WallroundIndex = 0;
	wallround->wproom_ = wpSelfroom_;


	for(i = ASPECT_LEFT ; i < ASPECT_LEFT+4; i++)
	{
		int aspect = safeAspect(i);
		direction_wallround_[aspect] = wallround;
		direction_wallround_[aspect]->directionFlag[aspect] = true;
	}
	//pcDMLand pland = land_[0];

	wallround->WallroundOptimaze(pdmmap->landmap(Left(),Top()));


	wallround = pcDMWallround(new cDMWallround(wpmap_));
	wallround_.push_back(wallround);

	wallround->WallroundIndex = 1;
	wallround->wproom_ = wpSelfroom_;

	
	for(i = ASPECT_RIGHT ; i < ASPECT_RIGHT+4; i++)
	{
		int aspect = safeAspect(i);
		direction_wallround_[aspect] = wallround;
		direction_wallround_[aspect]->directionFlag[aspect] = true;
	}
	//pcDMLand pland = land_[0];

	wallround->WallroundOptimaze(pdmmap->landmap(Left(),Bottom()-1));
	*/

}

void cDMRoom::RoomOptimaze_Div4water(vector<double>& Val)
{
	pcDMWallround wallround;
	pcDMMap pdmmap = wpmap_.lock();
	cCoordinate coo;

	pcDMLand land,landA,landB;

	int x,y;
	for(x = Left() ; x < Right(); x++)
	{
		for(y = Top() ; y < Bottom(); y++)
		{//ランド最適化
			pdmmap->landmap(x,y)->roomIndex = RoomIndex;
			pdmmap->landmap(x,y)->Landflag = MAPKIND_FLOOR;
			//pdmmap->landmap(x,y)->wallroundIndex = ;
			land_.push_back( pdmmap->landmap(x,y) );
		}
	}



	//分割領域決定
	int aspectBase;
	aspectBase = 2*random();


	int i;

	//水路を引く

	//縦
	
	x = random_range(Left()+1,Right()-1);
	y = Top();
	landA = pdmmap->landmap(x,y);
	landA->Landflag = MAPKIND_WATER;

	x = random_range(Left()+1,Right()-1);
	y = Bottom()-1;
	landB = pdmmap->landmap(x,y);
	landB->Landflag = MAPKIND_WATER;

	coo.SetAspect(ASPECT_DOWN);
	landA = pdmmap->landmap(landA->pos.x + coo.x,landA->pos.y + coo.y);
	landA->Landflag = MAPKIND_WATER;

	coo.SetAspect(ASPECT_UP);
	landB = pdmmap->landmap(landB->pos.x + coo.x,landB->pos.y + coo.y);
	landB->Landflag = MAPKIND_WATER;

	makeWaterWay_inRoom(landA,landB);

	//横


	x = Left();
	y = random_range(Top()+1,Bottom()-1);
	landA = pdmmap->landmap(x,y);
	landA->Landflag = MAPKIND_WATER;

	x = Right()-1;
	y = random_range(Top()+1,Bottom()-1);
	landB = pdmmap->landmap(x,y);
	landB->Landflag = MAPKIND_WATER;

	coo.SetAspect(ASPECT_RIGHT);
	landA = pdmmap->landmap(landA->pos.x + coo.x,landA->pos.y + coo.y);
	landA->Landflag = MAPKIND_WATER;

	coo.SetAspect(ASPECT_LEFT);
	landB = pdmmap->landmap(landB->pos.x + coo.x,landB->pos.y + coo.y);
	landB->Landflag = MAPKIND_WATER;

	makeWaterWay_inRoom(landA,landB);



	int face = 0;
	for(face=0;face<4;face++)
	{
		//位置関係記述
		wallround = pcDMWallround(new cDMWallround(wpmap_));
		wallround_.push_back(wallround);
		wallround->WallroundIndex = face;
		wallround->wproom_ = wpSelfroom_;

		for(i = face*2 + aspectBase ; i <  (face+1)*2 + aspectBase; i++)
		{
			int aspect = safeAspect(i);
			direction_wallround_[aspect] = wallround;
			direction_wallround_[aspect]->directionFlag[aspect] = true;
		}

		if(face == 0)
		{//右上
			land = pdmmap->landmap(Right()-1,Top());
		}
		else if(face == 1)
		{//右下
			land = pdmmap->landmap(Right()-1,Bottom()-1);
		}
		else if(face == 2)
		{//左下
			land = pdmmap->landmap(Left(),Bottom()-1);
		}
		else if(face == 3)
		{//左上
			land = pdmmap->landmap(Left(),Top());
		}
		wallround->WallroundOptimaze(land);

	}


}
void cDMRoom::RoomOptimaze_Oval( vector<double>& Val)
{
	pcDMMap pdmmap = wpmap_.lock();
	pcDMWallround wallround = pcDMWallround(new cDMWallround(wpmap_));
	wallround_.push_back(wallround);

	wallround->WallroundIndex = 0;
	wallround->wproom_ = wpSelfroom_;

	int i;
	for(i = 0 ; i < 8; i++)
	{
		direction_wallround_[i] = wallround;
		direction_wallround_[i]->directionFlag[i] = true;
	}

	int x,y;
	for(x = Left() ; x < Right(); x++)
	{
		for(y = Top() ; y < Bottom(); y++)
		{//ランド最適化

			if( pow(x-CenterX+0.5,2)/pow(Width/2,2) + pow(y-CenterY+0.5,2)/pow(Height/2,2) <= 1.0)
			{
				pdmmap->landmap(x,y)->roomIndex = RoomIndex;
				pdmmap->landmap(x,y)->Landflag = MAPKIND_FLOOR;
				//pdmmap->landmap(x,y)->wallroundIndex = ;
				land_.push_back( pdmmap->landmap(x,y) );
			}
		}
	}



	wallround->WallroundOptimaze(land_[0]);



}

void fractalnoize(vector<vector<double>>& noize, const vector<double>& weight, int mapsize)
{
	
	int x,y;
	int size = 2;
	int i = 0;
	double w = 1;
	if(i < weight.size()) w = weight[i];

	noize.resize(size);
	for(x=0;x<size;x++)
	{
		noize[x].resize(size,0.0);
		for(y=0;y<size;y++)
		{
			noize[x][y] = random()*w;
		}
	}

	
	for(i=1;;i++)
	{
		//ぼかし拡大
		size *= 2;
		noize.resize(size);
		for(x=0;x<size;x++)
		{
			noize[x].resize(size,0.0);
		}
	
		noize.resize(size);
		for(x=size/2-1;x>=0;x--)
		{
			for(y=size/2-1;y>=0;y--)
			{
				noize[x*2][y*2] = noize[x][y];
			}
		}
		//横に伸ばす
		for(x=1;x<size-2;x+=2)
		{
			for(y=0;y<size;y+=2)
			{
				noize[x][y] = (noize[x+1][y] + noize[x-1][y])/2;
			}
		}
		for(y=0;y<size;y+=2)
		{
			noize[x][y] = (noize[0][y] + noize[x-1][y])/2;
		}
		
		
		//縦に伸ばす
		for(x=0;x<size;x++)
		{
			for(y=1;y<size-2;y+=2)
			{
				noize[x][y] = (noize[x][y-1] + noize[x][y+1])/2;
			}
			noize[x][y] = (noize[x][y-1] + noize[x][0])/2;
		}
		
		//ノイズ
		double w = 1;
		if(i < weight.size()) w = weight[i];

		for(x=0;x<size;x++)
		{
			for(y=0;y<size;y++)
			{
				noize[x][y] += random()*w;
			}
		}


		//終了条件
		if(size >= mapsize) return;
	}
}

void cDMRoom::RoomOptimaze_NoizeOval( vector<double>& Val)
{
	pcDMMap pdmmap = wpmap_.lock();
	pcDMWallround wallround = pcDMWallround(new cDMWallround(wpmap_));
	wallround_.push_back(wallround);

	wallround->WallroundIndex = 0;
	wallround->wproom_ = wpSelfroom_;

	int i;
	for(i = 0 ; i < 8; i++)
	{
		direction_wallround_[i] = wallround;
		direction_wallround_[i]->directionFlag[i] = true;
	}

	/*
	int x,y;
	for(x = Left() ; x < Right(); x++)
	{
		for(y = Top() ; y < Bottom(); y++)
		{//ランド最適化

			if( pow(x-CenterX+0.5,2)/pow(Width/2,2) + pow(y-CenterY+0.5,2)/pow(Height/2,2) <= 1.0)
			{
				pdmmap->landmap(x,y)->roomIndex = RoomIndex;
				pdmmap->landmap(x,y)->Landflag = MAPKIND_FLOOR;
				//pdmmap->landmap(x,y)->wallroundIndex = ;
				land_.push_back( pdmmap->landmap(x,y) );
			}
		}
	}
	*/



	int left = pArea->Left();
	int right = pArea->Right();
	int top = pArea->Top();
	int bottom = pArea->Bottom();

	int rectedge = 1;
	int size = (Width+Height)/2;
	int avecost = 100;

	int x,y;
	vector<int> v_int(MAPHEIGHT,1000000000);//十分大きな値が初期値
	vector<vector<double>> randomcost;
	vector<double> weight;
	weight.push_back(0.0);
	weight.push_back(0.0);
	weight.push_back(0.2);
	weight.push_back(0.6);
	weight.push_back(0.2);
	weight.push_back(0.0);
	fractalnoize(randomcost,weight,MAPWIDTH);

	vector<vector<int>> mapcost(MAPWIDTH,v_int);
	deque<c4DVector> que;

	for(x = left ; x < right; x++)
	{
		for(y = top ; y < bottom; y++)
		{
			randomcost[x][y] = randomcost[x][y]*avecost*5;
		}
	}
	
	c4DVector v;
	v.set((Left()+Right())/2, (Top()+Bottom())/2, 0, 0);
	que.push_back(v);
	mapcost[v.x][v.y] = v.z;

	for(;;)
	{
		if(que.empty()) break;
		if(que.size() > 4000)
		{//無限ループ防止
			OnAssert(_T(__FILE__),__LINE__,false,_T("RoomOptimaze_NoizeOval関数内で異常に大きなキューが発生しました。"));
			break;
		}

		c4DVector state = que[0];
		que.pop_front();

		if(mapcost[state.x][state.y] < state.z)
		{//コストもっと小さいのがある。
			continue;
		}

		cCoordinate coo;
		int aspect;
		for(aspect=0;aspect<8;aspect+=2)
		{
			coo.SetAspect(aspect);
			c4DVector newstate;
			newstate.set( state.x + coo.x, state.y + coo.y , 0, state.t);
			
			if(newstate.x < left + rectedge || newstate.x >= right - rectedge
				|| newstate.y < top + rectedge || newstate.y >= bottom - rectedge)
			{//端っこ
				continue;
			}
			
			
			newstate.z = state.z;//現在コスト
		
			int addcost = 0;

			if(newstate.t >= 1)
			{//最低保障距離
				addcost = randomcost[newstate.x][newstate.y];

				if(aspect == ASPECT_DOWN || aspect == ASPECT_UP) addcost *= Height*2/(Width+Height);
				else addcost *= Width*2/(Width+Height);
			}
			newstate.z += addcost;
			newstate.t += 1;//開始からの距離

			if(newstate.z < mapcost[newstate.x][newstate.y])//現在コスト
			{//コストが小さい
				mapcost[newstate.x][newstate.y] = newstate.z;
				que.push_back(newstate);
			}

		}
		
	}


	for(x = left ; x < right; x++)
	{
		for(y = top ; y < bottom; y++)
		{
			if(mapcost[x][y]<avecost*(size+1))
			{
				pdmmap->landmap(x,y)->roomIndex = RoomIndex;
				pdmmap->landmap(x,y)->Landflag = MAPKIND_FLOOR;
				//pdmmap->landmap(x,y)->wallroundIndex = ;
				land_.push_back( pdmmap->landmap(x,y) );
			}
		}
	}
	

	wallround->WallroundOptimaze(land_[0]);



}

//水路系
void cDMRoom::makeWaterWay_inRoom(pcDMLand landA, pcDMLand landB)
{
	pcDMMap pdmmap = wpmap_.lock();

	vector<int> v_int(MAPHEIGHT,1000000000);//十分大きな値が初期値
	vector<vector<int>> mapcost(MAPWIDTH,v_int);
	deque<c4DVector> que;

	int i;
	
	

	c4DVector v;
	v.set(landA->pos.x, landA->pos.y, 0, 0);
	que.push_back(v);
	mapcost[v.x][v.y] = v.z;

	/*
	//初期位置
	for(i=0;i<roundA->land_.size();i++)
	{
		c4DVector v;
		v.set(roundA->land_[i]->pos.x, roundA->land_[i]->pos.y, 0, 0);
		que.push_back(v);
		mapcost[v.x][v.y] = v.z;
	}
	*/


	for(;;)
	{
		if(que.empty()) break;
		if(que.size() > 4000)
		{//無限ループ防止
			OnAssert(_T(__FILE__),__LINE__,false,_T("makeWaterWay_inRoom関数内で異常に大きなキューが発生しました。"));
			break;
		}

		c4DVector state = que[0];
		que.pop_front();

		if(mapcost[state.x][state.y] < state.z)
		{//コストもっと小さいのがある。
			continue;
		}

		cCoordinate coo;
		int aspect;
		for(aspect=0;aspect<8;aspect+=2)
		{
			coo.SetAspect(aspect);
			c4DVector newstate;
			newstate.set( state.x + coo.x, state.y + coo.y ,0,0);
			
			if(newstate.x < Left()+1 || newstate.x >= Right()-1
				|| newstate.y < Top()+1 || newstate.y >= Bottom()-1)
			{//端っこ
				continue;
			}
			
			
			newstate.z = state.z;//現在コスト
			
			int addcost = 0;

			if(pdmmap->landmap(newstate.x,newstate.y)->Landflag == MAPKIND_WATER)
			{//
				//addcost = 3;
				addcost = 60;
			}
			else if(pdmmap->landmap(newstate.x,newstate.y)->WallofWater)
			{//部屋の壁
				//addcost = 29;//大きいと迂回したくなる//道が増えない
				addcost = 500;
			}
			else
			{//そのた
				//addcost = 4;//小さいと道が増える
				addcost = 40;
			}

			newstate.z += addcost;

			if(newstate.z < mapcost[newstate.x][newstate.y])//現在コスト
			{//コストが小さい
				mapcost[newstate.x][newstate.y] = newstate.z;
				que.push_back(newstate);
			}

		}
		
	}

	c4DVector goal;
	goal.set(landB->pos.x, landB->pos.y , mapcost[landB->pos.x][landB->pos.y],0);
	/*
	
	for(i=0;i<roundB->land_.size();i++)
	{
		if( goal.z > mapcost[roundB->land_[i]->pos.x][roundB->land_[i]->pos.y])
		{//最低値
			goal.set(roundB->land_[i]->pos.x, roundB->land_[i]->pos.y , mapcost[roundB->land_[i]->pos.x][roundB->land_[i]->pos.y],0);
		}
	}
	*/

	c4DVector state = goal;
	for(i=0;;i++)
	{

		
		cCoordinate coo;
		int aspect;
		c4DVector newstate(-1,-1,1000000000,0);
		c4DVector tempstate = state;

		int aspectstart = (int)(random()*4)*2;
		for(aspect=aspectstart;aspect<aspectstart+8;aspect+=2)
		{
			coo.SetAspect(aspect);
			
			tempstate.set( state.x + coo.x, state.y + coo.y ,0,0);

			if(newstate.z > mapcost[tempstate.x][tempstate.y])//現在コスト
			{//コストが小さい
				newstate.set(tempstate.x, tempstate.y , mapcost[tempstate.x][tempstate.y],0);
			}

		}
		if(newstate.x == -1 || i > 4000)
		{
			OnAssert(_T(__FILE__),__LINE__,false,_T("makeWaterWay_inRoom関数内でコストマップに異常を感知しました。"));
			break;
		}


		if(newstate.z <= 0)
		{
			break;
		}



		//pdmmap->makeWater_line(pdmmap->landmap(state.x,state.y),pdmmap->landmap(newstate.x,newstate.y));
		pdmmap->makeWater_sub(pdmmap->landmap(state.x,state.y));

		state = newstate;


	}

	pdmmap->makeWater_sub(pdmmap->landmap(state.x,state.y));
	

	return;
}


//第五過程
void cDMMap::Makelink(vector<double>& Val)
{
	if(Val.size() < 1) Val.resize(1,100);
	//余分通路最小割合(百分率)
	Val[0] = max(0,Val[0]);
	
	if(Val.size() < 2) Val.resize(2,200);
	//余分通路最大割合(百分率)
	Val[1] = max(1,Val[1]);

	if(Val.size() < 3) Val.resize(3,30);
	//重複通路を許す確率（百分率）
	Val[2] = max(0,Val[2]);

	//行き止まり二リンクを増やす
	VALDEF(3,100,0,100)

	int i;
	for(i=0;i<mustlink.size();i++)
	{//予約組
		Makelink_sub(mustlink[i].first->wproom_.lock(), mustlink[i].second->wproom_.lock(), Val[2]);
	}

	//つながり可能性
	int k,m,q;

	vector<vector<pair<pcDMRect,int>>> linklist(Rects_.size());

	cDiscreteProbability linklistSumPos;

	for(i=0;i<Rects_.size();i++)
	{
		int sumpos = 0;
		for(k=0;k<Rects_.size();k++)
		{
			int x_dis_minus = Rects_[i]->Width/2 + Rects_[k]->Width/2 - abs(Rects_[i]->CenterX - Rects_[k]->CenterX);
			int y_dis_minus = Rects_[i]->Height/2 + Rects_[k]->Height/2 - abs(Rects_[i]->CenterY - Rects_[k]->CenterY);
			if(i != k)
			{
				if(x_dis_minus < 0 || y_dis_minus < 0 )
				{
					continue;
				}
				//隣接してる
				if(x_dis_minus + y_dis_minus == 0) continue;
				linklist[i].push_back(pair<pcDMRect,int>(Rects_[k], x_dis_minus + y_dis_minus));
			}
			for(m=0;m<Room_[i]->wallround_.size();m++)
			{
				for(q=0;q<Room_[k]->wallround_.size();q++)
				{//隣接レクト
					Room_[i]->wallround_[m]->linkpos_.push_back(wpcDMWallround(Room_[k]->wallround_[q]));
				}
				for(q=0;q<Room_[i]->wallround_.size();q++)
				{//自分自身レクト
					if(m != q)
					{
						Room_[i]->wallround_[m]->linkpos_.push_back(wpcDMWallround(Room_[i]->wallround_[q]));
					}
				}
			}

			sumpos += x_dis_minus + y_dis_minus;
		}
		linklistSumPos.set(sumpos,i);
	}

	//余分リンク
	int addlinknum = random_range(Val[0],Val[1])*Room_.size()/100.0 ;

	addlinknum -= mustlink.size();
	if(addlinknum < 0) addlinknum = 0;
	
	//link最低１優先
	for(i=0;i<Room_.size();i++)
	{
		if(addlinknum <= i) break;

		cDiscreteProbability DP;

		if(linklist[i].size() == 0) continue;//隣接なし
		
		for(k=0;k<linklist[i].size();k++)
		{
			DP.set(linklist[i].at(k).second, k);
		}

		int index = DP.get(random());

		Makelink_sub(Room_[i], linklist[i].at(index).first->wproom_.lock(), Val[2]);
	}
	addlinknum -= i;



	for(i=0;i<addlinknum;i++)
	{
		int addindex = linklistSumPos.get(random());

		cDiscreteProbability DP;

		if(linklist[addindex].size() == 0) continue;//隣接なし
		
		for(k=0;k<linklist[addindex].size();k++)
		{
			DP.set(linklist[addindex].at(k).second, k);
		}
	
		int index = DP.get(random());

		Makelink_sub(Room_[addindex], linklist[addindex].at(index).first->wproom_.lock(), Val[2]);
	}


	//---------------------------
	//つながり保障チェック！


	int roop;
	for(roop = 0;;roop++)
	{

		for(i=0;i<Room_.size();i++)
		{
			for(k=0;k<Room_[i]->wallround_.size();k++)
			{
				Room_[i]->wallround_[k]->linkcheck = false;
			}
		}
		Makelink_likecheckroop(Room_[0]->wallround_[0]);
	
		int allchack = true;
		vector<pcDMWallround> notlinkWallround;

		for(i=0;i<Room_.size();i++)
		{
			for(k=0;k<Room_[i]->wallround_.size();k++)
			{
				allchack &= Room_[i]->wallround_[k]->linkcheck;
				if(!(Room_[i]->wallround_[k]->linkcheck)) notlinkWallround.push_back(Room_[i]->wallround_[k]);
			}
		}

		if(allchack) break;//めでたし


		for(i=0;i<notlinkWallround.size();i++)
		{//つながってないリスト
			int isbreak = false;
			for(k=0;k<notlinkWallround[i]->linkpos_.size();k++)
			{
				pcDMWallround roundlink = notlinkWallround[i]->linkpos_[k].lock();
				if(roundlink != NULL && roundlink->linkcheck == true )
				{
					Makelink_sub(notlinkWallround[i], roundlink, Val[2]);
					isbreak = true;
					break;
				}
			}
			if(isbreak) break;
		}

		if(roop > 400)
		{//無限ループ防止
			OnAssert(_T(__FILE__),__LINE__,false,_T("Makelink関数内で異常に大きなループが発生しました。"));
			break;
		}

	}

	
	//行き止まり判定
	for(i=0;i<Room_.size();i++)
	{
		cDiscreteProbability DP;

		if(Room_[i]->RoomMode == DEF_ROOMMODE_Way)
		{
			if(Room_[i]->wallround_[0]->link_.size() <= 1)
			{//行き止まり
				if(Val[3] < random()*100)
				{//行き止まりを許す
					continue;
				}

				for(k=0;k<Room_[i]->wallround_[0]->linkpos_.size();k++)
				{
					int m;
					int t = 1;
					for(m=0;m<Room_[i]->wallround_[0]->link_.size();m++)
					{
						if(Room_[i]->wallround_[0]->linkpos_[k].lock() == Room_[i]->wallround_[0]->link_[m].lock())
						{
							t = 0;
							break;
						}
					}
					if(t)
					{
						DP.set(1, k);
					}
				}

				if(DP.empty())
				{
					for(k=0;k<linklist[i].size();k++)
					{
						DP.set(linklist[i].at(k).second, k);
					}

					int index = DP.get(random());

					Makelink_sub(Room_[i], linklist[i].at(index).first->wproom_.lock(), 100);
				}
				else
				{
					int index = DP.get(random());

					Makelink_sub(Room_[i]->wallround_[0], Room_[i]->wallround_[0]->linkpos_[index].lock(), 100);
				}
			}
		}
	}
	

}

//第六過程
void cDMMap::MakeWater(vector<double>& Val)
{
	if(Val.size() < 1) Val.resize(1,30);
	//水分量少境界
	Val[0] = max(0,Val[0]);
	
	if(Val.size() < 2) Val.resize(2,75);
	//水分量中境界
	Val[1] = max(0,Val[1]);

	if(Val.size() < 3) Val.resize(3,93);
	//水分量多境界
	Val[2] = max(0,Val[2]);



	double watermass = random_range(0,100);

	if(watermass < Val[0])
	{//水なし

	}
	else if(watermass < Val[1])
	{//水路あり

		double per = (watermass-Val[0])*100 / (Val[1]-Val[0]);
		makeWater_Opt_way(per);

	}
	else if(watermass < Val[2])
	{//水溜りあり
	
		double per = (watermass-Val[1])*100 / (Val[2]-Val[1]);
		makeWater_Opt_mach(per);
	}
	else
	{//全面水浸し
		makeWater_Opt_max();
	
	}
}

//第六.5過程
void cDMMap::MakeAbyss(vector<double>& Val)
{
	//閾値最小(百分率)
	VALDEF(0,0,0,100)

	//閾値最大(百分率)
	VALDEF(1,0,0,100)

	//地割れ数最小(百分率)
	VALDEF(2,0,0,100)

	//地割れ数最大(百分率)
	VALDEF(3,0,0,100)


	makeAbyss_Opt_mach(random_range(Val[0],Val[1]+1));

	makeAbyss_Opt_way(random_range(Val[2],Val[3]+1));

}


//第七過程
void cDMMap::MakeParty(vector<double>& Val)
{
	if(Val.size() < 1) Val.resize(1,1);
	//宴会最大数
	Val[0] = max(0,Val[0]);

	if(Val.size() < 2) Val.resize(2,0);
	//宴会発生確率定数


	if(Val.size() < 3) Val.resize(3,0);
	//宴会発生確率マス数平方根係数


	if(Val.size() < 4) Val.resize(4,0);
	//宴会発生確率区画数係数


	int partynum = 0;

	int rectnum = Rects_.size();

	int i;
	for(i=0;i<Room_.size();i++)
	{
		if(Room_[i]->land_.size() < MINROOMSIZE*MINROOMSIZE) continue;//部屋にしては小さすぎる

		int roomsize = sqrt((double)Room_[i]->land_.size());
		
		double pos = Val[1] + Val[2]*roomsize + Val[3]*rectnum;

		if(pos > random() && partynum < Val[0])
		{
			Room_[i]->PartyFlag = 1;
			partynum++;
		}
		else
		{
			Room_[i]->PartyFlag = 0;
		}
	}



}
//第八過程
void cDMMap::MakeShop(vector<double>& Val)
{
	if(Val.size() < 1) Val.resize(1,0.0);
	//店出現確率
	Val[0] = max(0,Val[0]);


	int partynum = 0;

	int rectnum = Rects_.size();


	vector<pcDMRoom> vroom;
	int i;
	for(i=0;i<Room_.size();i++)
	{
		Room_[i]->ShopFlag = 0;
		if(Room_[i]->land_.size() < MINROOMSIZE*MINROOMSIZE) continue;//部屋にしては小さすぎる

		if(Room_[i]->PartyFlag) continue;//そこは宴会部屋
		
		if(Room_[i]->wallround_.size() > 1) continue;//そこは分断部屋

		vroom.push_back(Room_[i]);
		
	}
	
	if(RoomNum <= 1) return;//一部屋しかない

	double pos = Val[0];
	if(pos > random())
	{
		if(!vroom.empty())
		{
			vroom[vroom.size()*random()]->ShopFlag = 1;
		}
	}

}
void cDMMap::makeWater_Opt_way(int percent)
{
	vector<pcDMLand> landlistA, landlistB;
	int x,y,i;


	for(x = MAPMAKINGEDGELANDWIDTH ; x < MAPWIDTH - MAPMAKINGEDGELANDWIDTH; x++)
	{
		landlistA.push_back(landmap(x,MAPMAKINGEDGELANDWIDTH));
		landlistB.push_back(landmap(MAPWIDTH - x, MAPHEIGHT - MAPMAKINGEDGELANDWIDTH - 1));
	}
	for(y = MAPMAKINGEDGELANDWIDTH ; y < MAPHEIGHT - MAPMAKINGEDGELANDWIDTH; y++)
	{
		landlistA.push_back(landmap(MAPMAKINGEDGELANDWIDTH,y));
		landlistB.push_back(landmap(MAPWIDTH - MAPMAKINGEDGELANDWIDTH - 1, MAPHEIGHT - y));
	}

	pcDMLand landA,landB;
	for(i = 0 ; i < percent/5; i++)
	{//100で20本
		int index = min(landlistA.size(),landlistB.size())*random();
		
		if(random() < 0.5)
		{
			landA = landlistA[index];
			landB = landlistB[index];
		}
		else
		{
			landA = landlistB[index];
			landB = landlistA[index];
		}

		makeWater_sub_least(landA,landB);
	}



}
void cDMMap::makeWater_Opt_mach(int percent)
{


	vector<pcDMLand> landlistA, landlistB;
	int x,y,i;

	for(i = 0 ; i < 1+percent/50; i++)
	{//100で2本
		
		x = MAPWIDTH*random();
		y = MAPHEIGHT*random();

		makeWater_sub_pound(landmap(x,y),percent*(1+random())*MAPWIDTH*2.5/200);
	}

	for(x = MAPMAKINGEDGELANDWIDTH ; x < MAPWIDTH - MAPMAKINGEDGELANDWIDTH; x++)
	{
		landlistA.push_back(landmap(x,MAPMAKINGEDGELANDWIDTH));
		landlistB.push_back(landmap(MAPWIDTH - x, MAPHEIGHT - MAPMAKINGEDGELANDWIDTH - 1));
	}
	for(y = MAPMAKINGEDGELANDWIDTH ; y < MAPHEIGHT - MAPMAKINGEDGELANDWIDTH; y++)
	{
		landlistA.push_back(landmap(MAPMAKINGEDGELANDWIDTH,y));
		landlistB.push_back(landmap(MAPWIDTH - MAPMAKINGEDGELANDWIDTH - 1, MAPHEIGHT - y));
	}

	pcDMLand landA,landB;
	for(i = 0 ; i < 20; i++)
	{//20本
		int index = min(landlistA.size(),landlistB.size())*random();
		landA = landlistA[index];
		landB = landlistB[index];


		makeWater_sub_least(landA,landB);
	}

}
void cDMMap::makeWater_Opt_max()
{
	int x,y,i;
	for(x = MAPMAKINGEDGELANDWIDTH ; x < MAPWIDTH-MAPMAKINGEDGELANDWIDTH; x++)
	{
		for(y = MAPMAKINGEDGELANDWIDTH ; y < MAPHEIGHT-MAPMAKINGEDGELANDWIDTH; y++)
		{

			if(landmap(x,y)->Landflag != MAPKIND_FLOOR
				&& landmap(x,y)->Landflag != MAPKIND_FLOOR2)
			{
				landmap(x,y)->Landflag = MAPKIND_WATER;
			}
			
		}
	}
}
void cDMMap::makeAbyss_Opt_way(int num)
{
	vector<pcDMLand> landlistA, landlistB;
	int x,y,i;


	for(x = MAPMAKINGEDGELANDWIDTH ; x < MAPWIDTH - MAPMAKINGEDGELANDWIDTH; x++)
	{
		landlistA.push_back(landmap(x,MAPMAKINGEDGELANDWIDTH));
		landlistB.push_back(landmap(MAPWIDTH - x, MAPHEIGHT - MAPMAKINGEDGELANDWIDTH - 1));
	}
	for(y = MAPMAKINGEDGELANDWIDTH ; y < MAPHEIGHT - MAPMAKINGEDGELANDWIDTH; y++)
	{
		landlistA.push_back(landmap(MAPMAKINGEDGELANDWIDTH,y));
		landlistB.push_back(landmap(MAPWIDTH - MAPMAKINGEDGELANDWIDTH - 1, MAPHEIGHT - y));
	}

	pcDMLand landA,landB;
	for(i = 0 ; i < num; i++)
	{//100で20本
		int index = min(landlistA.size(),landlistB.size())*random();
		
		if(random() < 0.5)
		{
			landA = landlistA[index];
			landB = landlistB[index];
		}
		else
		{
			landA = landlistB[index];
			landB = landlistA[index];
		}

		makeAbyss_sub_least(landA,landB);
	}



}
void cDMMap::makeAbyss_Opt_mach(int percent)
{

	makeAbyss_sub_pound(percent);
}
void cDMMap::makeWater_sub_least(pcDMLand landA, pcDMLand landB)
{
	vector<int> v_int(MAPHEIGHT,1000000000);//十分大きな値が初期値
	vector<vector<int>> mapcost(MAPWIDTH,v_int);
	deque<c4DVector> que;

	int x,y,i;
	vector<vector<int>> randomcost(mapcost);

	for(x = 0 ; x < MAPWIDTH; x++)
	{
		for(y = 0 ; y < MAPHEIGHT; y++)
		{
			randomcost[x][y] = random()*100;
		}
	}
	

	c4DVector v;
	v.set(landA->pos.x, landA->pos.y, 0, 0);
	que.push_back(v);
	mapcost[v.x][v.y] = v.z;

	/*
	//初期位置
	for(i=0;i<roundA->land_.size();i++)
	{
		c4DVector v;
		v.set(roundA->land_[i]->pos.x, roundA->land_[i]->pos.y, 0, 0);
		que.push_back(v);
		mapcost[v.x][v.y] = v.z;
	}
	*/


	for(;;)
	{
		if(que.empty()) break;
		if(que.size() > 4000)
		{//無限ループ防止
			OnAssert(_T(__FILE__),__LINE__,false,_T("makeWater_sub_least関数内で異常に大きなキューが発生しました。"));
			break;
		}

		c4DVector state = que[0];
		que.pop_front();

		if(mapcost[state.x][state.y] < state.z)
		{//コストもっと小さいのがある。
			continue;
		}

		cCoordinate coo;
		int aspect;
		for(aspect=0;aspect<8;aspect+=2)
		{
			coo.SetAspect(aspect);
			c4DVector newstate;
			newstate.set( state.x + coo.x, state.y + coo.y ,0,0);
			
			if(newstate.x < MAPMAKINGEDGELANDWIDTH || newstate.x >= MAPWIDTH-MAPMAKINGEDGELANDWIDTH
				|| newstate.y < MAPMAKINGEDGELANDWIDTH || newstate.y >= MAPHEIGHT-MAPMAKINGEDGELANDWIDTH)
			{//端っこ
				continue;
			}
			
			
			newstate.z = state.z;//現在コスト
			
			int addcost = 0;

			if(landmap(newstate.x,newstate.y)->Landflag == MAPKIND_WATER
				&& landmap(newstate.x,newstate.y)->roomIndex != DM_INDEX_UNDEFINE)
			{//	部屋水路なら
				addcost = 1;
			}
			else if(landmap(newstate.x,newstate.y)->Landflag == MAPKIND_WATER)
			{//	廊下水路なら
				//addcost = 3;
				addcost = 30;
			}
			else if(landmap(newstate.x,newstate.y)->Landflag == MAPKIND_FLOOR)
			{//部屋床
				//絶対通れない。
				addcost = 10000000;
			}
			else if(landmap(newstate.x,newstate.y)->Landflag == MAPKIND_FLOOR2)
			{//通路床
				//addcost = 29;//大きいと迂回したくなる//道が増えない
				addcost = 40;
			}
			else if(landmap(newstate.x,newstate.y)->WallofWater)
			{//水路の壁
				//addcost = 29;//大きいと迂回したくなる//道が増えない
				addcost = 400;
			}
			else if(landmap(newstate.x,newstate.y)->WallofWay
				|| landmap(newstate.x,newstate.y)->WallofroomIndex)
			{//床の横
				addcost = 40;
			}
			else
			{//そのた
				//addcost = 4;//小さいと道が増える
				addcost = 1 + 0.6*randomcost[newstate.x][newstate.y];//randomcostは０～９９
			}

			newstate.z += addcost;

			if(newstate.z < mapcost[newstate.x][newstate.y])//現在コスト
			{//コストが小さい
				mapcost[newstate.x][newstate.y] = newstate.z;
				que.push_back(newstate);
			}

		}
		
	}

	c4DVector goal(0,0,1000000000,0);
	goal.set(landB->pos.x, landB->pos.y , mapcost[landB->pos.x][landB->pos.y],0);
	/*
	
	for(i=0;i<roundB->land_.size();i++)
	{
		if( goal.z > mapcost[roundB->land_[i]->pos.x][roundB->land_[i]->pos.y])
		{//最低値
			goal.set(roundB->land_[i]->pos.x, roundB->land_[i]->pos.y , mapcost[roundB->land_[i]->pos.x][roundB->land_[i]->pos.y],0);
		}
	}
	*/

	int aspectstart = (int)(random()*4)*2;

	c4DVector state = goal;
	for(i=0;;i++)
	{

		
		cCoordinate coo;
		int aspect;
		c4DVector newstate(-1,-1,1000000000,0);
		c4DVector tempstate = state;
		for(aspect=aspectstart;aspect<aspectstart+8;aspect+=2)
		{
			coo.SetAspect(aspect);
			
			tempstate.set( state.x + coo.x, state.y + coo.y ,0,0);

			if(newstate.z > mapcost[tempstate.x][tempstate.y])//現在コスト
			{//コストが小さい
				newstate.set(tempstate.x, tempstate.y , mapcost[tempstate.x][tempstate.y],0);
			}

		}
		if(newstate.x == -1 || i > 4000)
		{
			OnAssert(_T(__FILE__),__LINE__,false,_T("makeWater_sub_least関数内でコストマップに異常を感知しました。"));
			break;
		}


		if(newstate.z <= 0)
		{
			break;
		}



		//makeWater_line(landmap(state.x,state.y),landmap(newstate.x,newstate.y));
		//床は上書きできない！
		if(landmap(state.x,state.y)->Landflag != MAPKIND_FLOOR2 && landmap(state.x,state.y)->Landflag != MAPKIND_FLOOR)
		{
			makeWater_sub(landmap(state.x,state.y));
		}

		state = newstate;


	}

			
	//床は上書きできない！
	if(landmap(state.x,state.y)->Landflag != MAPKIND_FLOOR2 && landmap(state.x,state.y)->Landflag != MAPKIND_FLOOR)
	{
		makeWater_sub(landmap(state.x,state.y));
	}
	if(landA->Landflag != MAPKIND_FLOOR2 && landA->Landflag != MAPKIND_FLOOR)
	{
		makeWater_sub(landA);
	}
	if(landB->Landflag != MAPKIND_FLOOR2 && landB->Landflag != MAPKIND_FLOOR)
	{
		makeWater_sub(landB);
	}


	return;

}


void cDMMap::makeWater_sub_pound(pcDMLand land, int size)
{
	int left = max(land->pos.x-size,MAPMAKINGEDGELANDWIDTH);
	int right = min(land->pos.x+size,MAPWIDTH-MAPMAKINGEDGELANDWIDTH);
	int top = max(land->pos.y-size,MAPMAKINGEDGELANDWIDTH);
	int bottom = min(land->pos.y+size,MAPHEIGHT-MAPMAKINGEDGELANDWIDTH);

	int x,y;
	vector<int> v_int(MAPHEIGHT,1000000000);//十分大きな値が初期値
	vector<vector<int>> randomcost(MAPWIDTH,v_int);
	vector<vector<int>> mapcost(randomcost);
	deque<c4DVector> que;

	for(x = left ; x < right; x++)
	{
		for(y = top ; y < bottom; y++)
		{
			randomcost[x][y] = 100 + random()*1000;
		}
	}
	
	c4DVector v;
	v.set(land->pos.x, land->pos.y, 0, 0);
	que.push_back(v);
	mapcost[v.x][v.y] = v.z;

	for(;;)
	{
		if(que.empty()) break;
		if(que.size() > 4000)
		{//無限ループ防止
			OnAssert(_T(__FILE__),__LINE__,false,_T("makeWater_sub_pound関数内で異常に大きなキューが発生しました。"));
			break;
		}

		c4DVector state = que[0];
		que.pop_front();

		if(mapcost[state.x][state.y] < state.z)
		{//コストもっと小さいのがある。
			continue;
		}

		cCoordinate coo;
		int aspect;
		for(aspect=0;aspect<8;aspect+=2)
		{
			coo.SetAspect(aspect);
			c4DVector newstate;
			newstate.set( state.x + coo.x, state.y + coo.y ,0,0);
			
			if(newstate.x < left || newstate.x >= right
				|| newstate.y < top || newstate.y >= bottom)
			{//端っこ
				continue;
			}
			
			
			newstate.z = state.z;//現在コスト
			
			int addcost = randomcost[newstate.x][newstate.y];


			newstate.z += addcost;

			if(newstate.z < mapcost[newstate.x][newstate.y])//現在コスト
			{//コストが小さい
				mapcost[newstate.x][newstate.y] = newstate.z;
				que.push_back(newstate);
			}

		}
		
	}


	for(x = left ; x < right; x++)
	{
		for(y = top ; y < bottom; y++)
		{
			if(mapcost[x][y]<100*(size+1))
			{
				if(landmap(x,y)->Landflag != MAPKIND_FLOOR
					&& landmap(x,y)->Landflag != MAPKIND_FLOOR2)
				{
					makeWater_sub(landmap(x,y));
					//landmap(x,y)->Landflag = MAPKIND_WATER;
				}
			}
		}
	}
	

}

void cDMMap::makeAbyss_sub_least(pcDMLand landA, pcDMLand landB)
{
	vector<int> v_int(MAPHEIGHT,1000000000);//十分大きな値が初期値
	vector<vector<int>> mapcost(MAPWIDTH,v_int);
	deque<c4DVector> que;

	int x,y,i;
	vector<vector<int>> randomcost(mapcost);

	for(x = 0 ; x < MAPWIDTH; x++)
	{
		for(y = 0 ; y < MAPHEIGHT; y++)
		{
			randomcost[x][y] = random()*100;
		}
	}
	

	c4DVector v;
	v.set(landA->pos.x, landA->pos.y, 0, 0);
	que.push_back(v);
	mapcost[v.x][v.y] = v.z;

	/*
	//初期位置
	for(i=0;i<roundA->land_.size();i++)
	{
		c4DVector v;
		v.set(roundA->land_[i]->pos.x, roundA->land_[i]->pos.y, 0, 0);
		que.push_back(v);
		mapcost[v.x][v.y] = v.z;
	}
	*/


	for(;;)
	{
		if(que.empty()) break;
		if(que.size() > 4000)
		{//無限ループ防止
			OnAssert(_T(__FILE__),__LINE__,false,_T("makeAbyss_sub_least関数内で異常に大きなキューが発生しました。"));
			break;
		}

		c4DVector state = que[0];
		que.pop_front();

		if(mapcost[state.x][state.y] < state.z)
		{//コストもっと小さいのがある。
			continue;
		}

		cCoordinate coo;
		int aspect;
		for(aspect=0;aspect<8;aspect+=2)
		{
			coo.SetAspect(aspect);
			c4DVector newstate;
			newstate.set( state.x + coo.x, state.y + coo.y ,0,0);
			
			if(newstate.x < MAPMAKINGEDGELANDWIDTH || newstate.x >= MAPWIDTH-MAPMAKINGEDGELANDWIDTH
				|| newstate.y < MAPMAKINGEDGELANDWIDTH || newstate.y >= MAPHEIGHT-MAPMAKINGEDGELANDWIDTH)
			{//端っこ
				continue;
			}
			
			
			newstate.z = state.z;//現在コスト
			
			int addcost = 0;

			if(landmap(newstate.x,newstate.y)->Landflag == MAPKIND_WATER
				&& landmap(newstate.x,newstate.y)->roomIndex != DM_INDEX_UNDEFINE)
			{//	部屋水路なら
				addcost = 100;
			}
			else if(landmap(newstate.x,newstate.y)->Landflag == MAPKIND_WATER)
			{//	廊下水路なら
				//addcost = 3;
				addcost = 400;
			}
			else if(landmap(newstate.x,newstate.y)->Landflag == MAPKIND_FLOOR)
			{//部屋床
				//絶対通れない。
				addcost = 10000000;
			}
			else if(landmap(newstate.x,newstate.y)->Landflag == MAPKIND_FLOOR2)
			{//通路床
				//addcost = 29;//大きいと迂回したくなる//道が増えない
				addcost = 400;
			}
			else if(landmap(newstate.x,newstate.y)->WallofWater)
			{//水路の壁
				//addcost = 29;//大きいと迂回したくなる//道が増えない
				addcost = 400;
			}
			else if(landmap(newstate.x,newstate.y)->WallofWay
				|| landmap(newstate.x,newstate.y)->WallofroomIndex)
			{//床の横
				addcost = 40;
			}
			else
			{//そのた
				//addcost = 4;//小さいと道が増える
				addcost = 1 + 0.6*randomcost[newstate.x][newstate.y];//randomcostは０～９９
			}

			newstate.z += addcost;

			if(newstate.z < mapcost[newstate.x][newstate.y])//現在コスト
			{//コストが小さい
				mapcost[newstate.x][newstate.y] = newstate.z;
				que.push_back(newstate);
			}

		}
		
	}

	c4DVector goal(0,0,1000000000,0);
	goal.set(landB->pos.x, landB->pos.y , mapcost[landB->pos.x][landB->pos.y],0);
	/*
	
	for(i=0;i<roundB->land_.size();i++)
	{
		if( goal.z > mapcost[roundB->land_[i]->pos.x][roundB->land_[i]->pos.y])
		{//最低値
			goal.set(roundB->land_[i]->pos.x, roundB->land_[i]->pos.y , mapcost[roundB->land_[i]->pos.x][roundB->land_[i]->pos.y],0);
		}
	}
	*/

	int aspectstart = (int)(random()*4)*2;

	c4DVector state = goal;
	for(i=0;;i++)
	{

		
		cCoordinate coo;
		int aspect;
		c4DVector newstate(-1,-1,1000000000,0);
		c4DVector tempstate = state;
		for(aspect=aspectstart;aspect<aspectstart+8;aspect+=2)
		{
			coo.SetAspect(aspect);
			
			tempstate.set( state.x + coo.x, state.y + coo.y ,0,0);

			if(newstate.z > mapcost[tempstate.x][tempstate.y])//現在コスト
			{//コストが小さい
				newstate.set(tempstate.x, tempstate.y , mapcost[tempstate.x][tempstate.y],0);
			}

		}
		if(newstate.x == -1 || i > 4000)
		{
			OnAssert(_T(__FILE__),__LINE__,false,_T("makeAbyss_sub_least関数内でコストマップに異常を感知しました。"));
			break;
		}


		if(newstate.z <= 0)
		{
			break;
		}



		//makeWater_line(landmap(state.x,state.y),landmap(newstate.x,newstate.y));
		//床は上書きできない！
		if(landmap(state.x,state.y)->Landflag != MAPKIND_FLOOR2 && landmap(state.x,state.y)->Landflag != MAPKIND_FLOOR)
		{
			makeAbyss_sub(landmap(state.x,state.y));
		}

		state = newstate;


	}

			
	//床は上書きできない！
	if(landmap(state.x,state.y)->Landflag != MAPKIND_FLOOR2 && landmap(state.x,state.y)->Landflag != MAPKIND_FLOOR)
	{
		makeAbyss_sub(landmap(state.x,state.y));
	}
	if(landA->Landflag != MAPKIND_FLOOR2 && landA->Landflag != MAPKIND_FLOOR)
	{
		makeAbyss_sub(landA);
	}
	if(landB->Landflag != MAPKIND_FLOOR2 && landB->Landflag != MAPKIND_FLOOR)
	{
		makeAbyss_sub(landB);
	}


	return;

}


void cDMMap::makeAbyss_sub_pound(int percent)
{
	int left = MAPMAKINGEDGELANDWIDTH;
	int right = MAPWIDTH-MAPMAKINGEDGELANDWIDTH;
	int top = MAPMAKINGEDGELANDWIDTH;
	int bottom = MAPHEIGHT-MAPMAKINGEDGELANDWIDTH;

	int x,y;
	vector<int> v_int(MAPHEIGHT,1000000000);//十分大きな値が初期値
	vector<vector<double>> randomcost;
	vector<double> weight;
	weight.push_back(0.08);
	weight.push_back(0.48);
	weight.push_back(0.24);
	weight.push_back(0.12);
	weight.push_back(0.06);
	weight.push_back(0.02);
	fractalnoize(randomcost,weight,MAPWIDTH);

	for(x = left ; x < right; x++)
	{
		for(y = top ; y < bottom; y++)
		{
			randomcost[x][y] = randomcost[x][y]*100;
		}
	}
	


	for(x = left ; x < right; x++)
	{
		for(y = top ; y < bottom; y++)
		{
			if(randomcost[x][y] < percent)
			{
				if(landmap(x,y)->Landflag != MAPKIND_FLOOR
					&& landmap(x,y)->Landflag != MAPKIND_FLOOR2)
				{
					makeAbyss_sub(landmap(x,y));
					//landmap(x,y)->Landflag = MAPKIND_WATER;
				}
			}
		}
	}
	
	

}

pcDMLand cDMMap::makeAbyss_sub(pcDMLand land, int aspect, int Num)
{

	if(Num <= 0) return land;

	cCoordinate coo;
	pcDMLand landfor;//,landright,landleft,landback;
	pcDMLand aspland;
	//landfor = land;
	int i,k;
	for(i=0;i<Num;i++)
	{
		
		//通路床は上書きできない！
		if(land->Landflag == MAPKIND_FLOOR2 || land->Landflag == MAPKIND_FLOOR) continue;


		makeAbyss_sub(land);


		coo.SetAspect(aspect);
		landfor = landmap(land->pos.x + coo.x, land->pos.y + coo.y);
		land = landfor;

	
	}

	if(land->Landflag != MAPKIND_FLOOR2 || land->Landflag == MAPKIND_FLOOR)
	{


		makeAbyss_sub(land);


		coo.SetAspect(aspect);
		landfor = landmap(land->pos.x + coo.x, land->pos.y + coo.y);	
		land = landfor;

	}

	return land;
}
pcDMLand cDMMap::makeAbyss_sub(pcDMLand land)
{

	pcDMLand aspland;
	cCoordinate coo;

	if(land->Landflag == MAPKIND_WALLEGDE) return land;

	land->Landflag = MAPKIND_ABYSS;

	int aspect = 0,k;
	
	for(k=aspect;k<aspect+8;k++)
	{
		coo.SetAspect(k);
		aspland = landmap(land->pos.x + coo.x, land->pos.y + coo.y);
		
	}

	return land;
}

void cDMMap::Makelink_likecheckroop(pcDMWallround round)
{
	round->linkcheck = true;
	int i;
	for(i = 0; i<round->link_.size() ;i++)
	{
		pcDMWallround roundlink = round->link_[i].lock();
		if(roundlink != NULL && roundlink->linkcheck == false ) Makelink_likecheckroop(roundlink);
	}
}


void cDMMap::Makelink_sub(pcDMRoom roomA, pcDMRoom roomB, int multiwaypos)
{
	Makelink_sub(
		roomA->direction_wallround(roomB->CenterX, roomB->CenterY),
		roomB->direction_wallround(roomA->CenterX, roomA->CenterY),
		multiwaypos);
}
void cDMMap::Makelink_sub(pcDMWallround roundA, pcDMWallround roundB, int multiwaypos)
{

	/*
	vector<pair<pcDMWallround, pcDMWallround>>::iterator itr;
	itr = find(linkpair_.begin(), linkpair_.end() , pair<pcDMWallround,pcDMWallround>(roundA,roundB));
	
	if(itr == linkpair_.end())
	{//重複を許さない
		itr = find(linkpair_.begin(), linkpair_.end() , pair<pcDMWallround,pcDMWallround>(roundB,roundA));
	
		if(itr == linkpair_.end())
			linkpair_.push_back(pair<pcDMWallround,pcDMWallround>(roundA,roundB));
	}
	*/
	

	if(	linkpair_.end() == find(linkpair_.begin(), linkpair_.end() , pair<pcDMWallround,pcDMWallround>(roundA,roundB)) &&
		linkpair_.end() == find(linkpair_.begin(), linkpair_.end() , pair<pcDMWallround,pcDMWallround>(roundB,roundA))
	)
	{//重複してない
		linkpair_.push_back(pair<pcDMWallround,pcDMWallround>(roundA,roundB));
		roundA->link_.push_back(wpcDMWallround(roundB));
		roundB->link_.push_back(wpcDMWallround(roundA));
	}
	else
	{//重複してる
		if(	nevermultilink.end() == find(nevermultilink.begin(), nevermultilink.end() , pair<pcDMRect,pcDMRect>(roundA->wproom_.lock()->pArea,roundB->wproom_.lock()->pArea)) &&
			nevermultilink.end() == find(nevermultilink.begin(), nevermultilink.end() , pair<pcDMRect,pcDMRect>(roundB->wproom_.lock()->pArea,roundA->wproom_.lock()->pArea))
			)
		{
			if(multiwaypos > random()*100)
			{
				linkpair_.push_back(pair<pcDMWallround,pcDMWallround>(roundA,roundB));
				roundA->link_.push_back(wpcDMWallround(roundB));
				roundB->link_.push_back(wpcDMWallround(roundA));
	
			}
		}
	}

	//linkpair_.push_back(pair<pcDMWallround,pcDMWallround>(roundA,roundB));

}

void cDMMap::makeWay(vector<double>& Val)
{

	/*
	vector<vector<vector<int>>> Flag(Room_.size());
	vector<pair<pcDMWallround,pcDMWallround>> Waypair;

	int i,k,m,q;

	for(i=0;i<Room_.size();i++)
	{//初期化
		Flag[i].resize(Room_[i]->wallround_.size(), false)
		for(k=0;k<Room_[i]->wallround_.size();k++)
		{
			Flag[i][k].resize(Room_[i]->wallround_[k]->link_.size(), false)
		}
	}

	for(i=0;i<Room_.size();i++)
	{
		for(k=0;k<Room_[i]->wallround_.size();k++)
		{
			for(m=0;m<Room_[i]->wallround_[k]->link_.size();m++)
			{
				if(Flag[i][k][m] == false)
				{
					pcDMWallround round = Room_[i]->wallround_[k];
					pcDMWallround roundlink = Room_[i]->wallround_[k]->link_[m].lock();
					
					if(roundlink == NULL) continue;
					for( q=0; q<roundlink->link_.size(); q++)
					{
						pcDMWallround roundlinklink = roundlink->link_[q].lock()
					}

					roundlink->

					
					Flag[i][k][m] = true;
				}
			}
			
		}
	}
	if()

*/

	int i;

	random_shuffle( linkpair_.begin(),linkpair_.end(),random_int);

	for(i=0;i<linkpair_.size();i++)
	{
		if(random() > 0.5)
		{
			makeWay_sub(linkpair_[i].first, linkpair_[i].second);
		}
		else
		{
			makeWay_sub(linkpair_[i].second, linkpair_[i].first);
		}
	}



}

void cDMMap::makeWay_sub(pcDMWallround roundA, pcDMWallround roundB)
{
	if(roundA->walloutland_.empty() || roundB->walloutland_.empty()) return ;
	pcDMLand landA = roundA->walloutland_[roundA->walloutland_.size()*random()];
	pcDMLand landB = roundB->walloutland_[roundB->walloutland_.size()*random()];

	
	cCoordinate center;//最終的なものとは異なるが目安
	center.Set( roundA->wproom_.lock()->CenterX/2 + roundB->wproom_.lock()->CenterX/2,
		roundA->wproom_.lock()->CenterY/2 + roundB->wproom_.lock()->CenterY/2);


	vector<pcDMLand> landAlist;

	int i;
	for(i=0;i<20;i++)
	{//20回までやり直し
		pcDMLand intro;
		intro = roundA->introExit(center);
		if(intro != NULL) landA = intro;

		intro = roundB->introExit(center);
		if(intro != NULL) landB = intro;

		if(makeWay_sub_nikaku(landA,landB,0,false))
		{
			break;
		}
		if(makeWay_sub_nikaku(landA,landB,2,false))
		{
			break;
		}
	}

	//目標決定した
	pcDMLand land = landA;

	int Moveaspect = -100;

	int k;

	/*
	roundA->exit_.push_back(landA);
	roundA->exitto_.push_back(landB);
	roundB->exit_.push_back(landB);
	roundB->exitto_.push_back(landA);
	*/
	
	pcDMLand land2,land3;
	pcDMLand land2d,land3d;

	int roolsign;
	int base;

	cCoordinate coo;

	cCoordinate landA2center;//外側向き
	landA2center.Set(center.x - landA->pos.x, center.y - landA->pos.y);
	roolsign = (int)(random()*2)*2-1;//-1or1
	base = landA2center.GetAspect() - roolsign;
	for(k=0;k<=8;k+=2)
	{
		coo.SetAspect( safeAspect(base)/2*2 + k*roolsign);//内側向き
		//land2 壁
		land2 = landmap(land->pos.x +coo.x, land->pos.y+ coo.y);
		land2d = landmap(land->pos.x +coo.x*2, land->pos.y+ coo.y*2);
		if(roundA->wproom_.lock()->RectIndex == land2->WallofrectIndex
			&& roundA->WallroundIndex == land2->WallofwallroundIndex
			&& (land2d->Landflag == MAPKIND_FLOOR || land2d->Landflag == MAPKIND_FLOOR2 ))
		{
			//land2->Landflag = MAPKIND_FLOOR2;
			Moveaspect = safeAspect(base)/2*2 + k*roolsign;
			break;
		}
	}
	//coo = coo*2;
	//land2d = landmap(land->pos.x +coo.x, land->pos.y+ coo.y);//部屋の中
		

	land = landB;

	cCoordinate landB2center;
	landB2center.Set(center.x - landB->pos.x, center.y - landB->pos.y);
	roolsign = (int)(random()*2)*2-1;//-1or1
	base = landB2center.GetAspect() - roolsign;
	for(k=0;k<=8;k+=2)
	{
		coo.SetAspect( safeAspect(base)/2*2 + k*roolsign);
		land3 = landmap(land->pos.x +coo.x, land->pos.y+ coo.y);
		land3d = landmap(land->pos.x +coo.x*2, land->pos.y+ coo.y*2);
		if(roundB->wproom_.lock()->RectIndex == land3->WallofrectIndex
			&& roundB->WallroundIndex == land3->WallofwallroundIndex
			&& (land3d->Landflag == MAPKIND_FLOOR || land3d->Landflag == MAPKIND_FLOOR2 ))
		{
			//land3->Landflag = MAPKIND_FLOOR2;
			//Moveaspect = safeAspect(base)/2*2 + k*roolsign;
			break;
		}
	}
	//coo = coo*2;
	//land3d = landmap(land->pos.x +coo.x, land->pos.y+ coo.y);
	

	roundA->exit_.push_back(landA);
	roundA->exitto_.push_back(landB);
	roundB->exit_.push_back(landB);
	roundB->exitto_.push_back(landA);


	if(Moveaspect == -100)
	{//出口がない。（ありえない）
		OnAssert(_T(__FILE__),__LINE__,false,_T("異常検出：出口検索で壁が見つかりません。"));
	}

	
	if(makeWay_sub_nikaku(landA, landB, Moveaspect))
	{
		landA->Landflag = MAPKIND_FLOOR2;
		landB->Landflag = MAPKIND_FLOOR2;
		land2->Landflag = MAPKIND_FLOOR2;
		land3->Landflag = MAPKIND_FLOOR2;
		return;
	}
	if(makeWay_sub_nikaku(landA, landB, Moveaspect+2))
	{
		landA->Landflag = MAPKIND_FLOOR2;
		landB->Landflag = MAPKIND_FLOOR2;
		land2->Landflag = MAPKIND_FLOOR2;
		land3->Landflag = MAPKIND_FLOOR2;
		return;
	}
	
	if(roundA->wproom_.lock()->RoomMode == DEF_ROOMMODE_Way
		|| roundB->wproom_.lock()->RoomMode == DEF_ROOMMODE_Way)
	{
		makeWay_sub_least(roundA,roundB,1);
	}
	else
	{
		makeWay_sub_least(roundA,roundB,0);
	}

	/*
	makeWay_sub(landA, landB, Moveaspect);
	landA->Landflag = MAPKIND_FLOOR2;
	landB->Landflag = MAPKIND_FLOOR2;
	land2->Landflag = MAPKIND_FLOOR2;
	land3->Landflag = MAPKIND_FLOOR2;
	*/

	//land2d->Landflag = MAPKIND_WATER;
	//land3d->Landflag = MAPKIND_WATER;

}
void cDMMap::makeWay_line(pcDMLand land, pcDMLand land2)
{
	cCoordinate coo;
	coo.Set(land2->pos.x - land->pos.x, land2->pos.y - land->pos.y);
	makeWay_sub(land, coo.GetAspect() ,coo.dif());
}
void cDMMap::makeWater_line(pcDMLand land, pcDMLand land2)
{
	cCoordinate coo;
	coo.Set(land2->pos.x - land->pos.x, land2->pos.y - land->pos.y);
	makeWater_sub(land, coo.GetAspect() ,coo.dif());
}
void cDMMap::makeAbyss_line(pcDMLand land, pcDMLand land2)
{
	cCoordinate coo;
	coo.Set(land2->pos.x - land->pos.x, land2->pos.y - land->pos.y);
	makeAbyss_sub(land, coo.GetAspect() ,coo.dif());
}

int cDMMap::makeWay_sub_nikaku(pcDMLand landA, pcDMLand landB, int aspect, int makeWayFlag)
{

	pcDMLand land = landA;
	//二角取り
	cCoordinate apos, bpos;
	apos.Set(landA->pos.x, landA->pos.y);
	bpos.Set(landB->pos.x, landB->pos.y);

	if(landA->pos.x < MAPMAKINGEDGELANDWIDTH || landA->pos.y < MAPMAKINGEDGELANDWIDTH
		|| landA->pos.x >= MAPWIDTH - MAPMAKINGEDGELANDWIDTH || landA->pos.y >= MAPHEIGHT - MAPMAKINGEDGELANDWIDTH
	||landB->pos.x < MAPMAKINGEDGELANDWIDTH || landB->pos.y < MAPMAKINGEDGELANDWIDTH
		|| landB->pos.x >= MAPWIDTH - MAPMAKINGEDGELANDWIDTH || landB->pos.y >= MAPHEIGHT - MAPMAKINGEDGELANDWIDTH)
	{
		return false;
	}

	int ErrorMax = 0;
	int goodaxis = -1;
	cDiscreteProbability axisDP;
	aspect = safeAspect(aspect);
	if(aspect % 4 >= 2)
	{//軸は縦
		vector<int> axis;

		int i;
		for(i=MAPMAKINGEDGELANDWIDTH;i<MAPWIDTH-MAPMAKINGEDGELANDWIDTH;i++)
		{
			axis.push_back(i);
		}

		int ymin;
		int ymax;
		int axmin;
		int axmax;
		int bxmin;
		int bxmax;

		random_shuffle(axis.begin(),axis.end(),random_int);//シャッフル
		for(i=0;i<axis.size();i++)
		{
			int ErrorNum = 0;
			
			int k;
			ymin = min(apos.y , bpos.y);
			ymax = max(apos.y , bpos.y);
			axmin = min(apos.x , axis[i]);
			axmax = max(apos.x , axis[i]);
			bxmin = min(bpos.x , axis[i]);
			bxmax = max(bpos.x , axis[i]);
			
			if(ymax-ymin <= 1) continue;

			for(k = axmin; k <= axmax ; k++)
			{
				pcDMLand land = landmap(k , apos.y);
				if(land->WallofWay
					|| land->WallofwallroundIndex != DM_INDEX_UNDEFINE
					|| land->roomIndex != DM_INDEX_UNDEFINE)
				{
					ErrorNum++;
				}
				if(ErrorNum > ErrorMax) break;
			}
			if(ErrorNum > ErrorMax) continue;
			for(k = bxmin; k <= bxmax ; k++)
			{
				pcDMLand land = landmap(k , bpos.y);
				if(land->WallofWay
					|| land->WallofwallroundIndex != DM_INDEX_UNDEFINE
					|| land->roomIndex != DM_INDEX_UNDEFINE)
				{
					ErrorNum++;
				}
				if(ErrorNum > ErrorMax) break;
			}
			if(ErrorNum > ErrorMax) continue;
			for(k = ymin; k <= ymax ; k++)
			{
				pcDMLand land = landmap(axis[i] , k);
				if(land->WallofWay
					|| land->WallofwallroundIndex != DM_INDEX_UNDEFINE
					|| land->roomIndex != DM_INDEX_UNDEFINE)
				{
					ErrorNum++;
				}
				if(ErrorNum > ErrorMax) break;
			}
			if(ErrorNum > ErrorMax) continue;
			if(ErrorNum <= ErrorMax)
			{
				goodaxis = axis[i];
				double pos = exp(double(MAPWIDTH+MAPHEIGHT-(axmax-axmin+bxmax-bxmin))/2.0);
				axisDP.set(pos, goodaxis);
				//break;
			}
		}

		if(goodaxis != -1)
		{

			goodaxis = axisDP.get(random());

			if(makeWayFlag)
			{

				pcDMLand land1 = landmap(goodaxis,apos.y);
				pcDMLand land2 = landmap(goodaxis,bpos.y);

				makeWay_line(landA, land1);
				makeWay_line(land1, land2);
				makeWay_line(land2, landB);
			}
			return true;
		}

	}
	else
	{//軸は横
		vector<int> axis;

		int i;
		for(i=MAPMAKINGEDGELANDWIDTH;i<MAPHEIGHT-MAPMAKINGEDGELANDWIDTH;i++)
		{
			axis.push_back(i);
		}

		int xmin;
		int xmax;
		int aymin;
		int aymax;
		int bymin;
		int bymax;


		random_shuffle(axis.begin(),axis.end(),random_int);//シャッフル
		for(i=0;i<axis.size();i++)
		{
			int ErrorNum = 0;
			
			int k;
			xmin = min(apos.x , bpos.x);
			xmax = max(apos.x , bpos.x);
			aymin = min(apos.y , axis[i]);
			aymax = max(apos.y , axis[i]);
			bymin = min(bpos.y , axis[i]);
			bymax = max(bpos.y , axis[i]);

			if(xmax-xmin <= 1) continue;

			for(k = aymin; k <= aymax ; k++)
			{
				pcDMLand land = landmap(apos.x , k);
				if(land->WallofWay
					|| land->WallofwallroundIndex != DM_INDEX_UNDEFINE
					|| land->roomIndex != DM_INDEX_UNDEFINE)
				{
					ErrorNum++;
				}
				if(ErrorNum > ErrorMax) break;
			}
			if(ErrorNum > ErrorMax) continue;
			for(k = bymin; k <= bymax ; k++)
			{
				pcDMLand land = landmap(bpos.x , k);
				if(land->WallofWay
					|| land->WallofwallroundIndex != DM_INDEX_UNDEFINE
					|| land->roomIndex != DM_INDEX_UNDEFINE)
				{
					ErrorNum++;
				}
				if(ErrorNum > ErrorMax) break;
			}
			if(ErrorNum > ErrorMax) continue;
			for(k = xmin; k <= xmax ; k++)
			{
				pcDMLand land = landmap( k, axis[i]);
				if(land->WallofWay
					|| land->WallofwallroundIndex != DM_INDEX_UNDEFINE
					|| land->roomIndex != DM_INDEX_UNDEFINE)
				{
					ErrorNum++;
				}
				if(ErrorNum > ErrorMax) break;
			}
			if(ErrorNum > ErrorMax) continue;
			if(ErrorNum <= ErrorMax)
			{
				goodaxis = axis[i];
				double pos = exp(double(MAPWIDTH+MAPHEIGHT-(aymax-aymin+bymax-bymin))/2.0);
				axisDP.set(pos, goodaxis);
				//break;
			}
		}

		if(goodaxis != -1)
		{
			goodaxis = axisDP.get(random());

			if(makeWayFlag)
			{
				pcDMLand land1 = landmap(apos.x,goodaxis);
				pcDMLand land2 = landmap(bpos.x,goodaxis);

				makeWay_line(landA, land1);
				makeWay_line(land1, land2);
				makeWay_line(land2, landB);
			}
			return true;
		}
	}


	return false;


}

void cDMMap::makeWay_sub_least(pcDMWallround roundA, pcDMWallround roundB, int ModeWay)
{
	vector<int> v_int(MAPHEIGHT,1000000000);//十分大きな値が初期値
	vector<vector<int>> mapcost(MAPWIDTH,v_int);
	deque<c4DVector> que;

	
	
	int i = roundA->land_.size()*random();
	c4DVector v;
	v.set(roundA->land_[i]->pos.x, roundA->land_[i]->pos.y, 0, 0);
	que.push_back(v);
	mapcost[v.x][v.y] = v.z;

	/*
	//初期位置
	for(i=0;i<roundA->land_.size();i++)
	{
		c4DVector v;
		v.set(roundA->land_[i]->pos.x, roundA->land_[i]->pos.y, 0, 0);
		que.push_back(v);
		mapcost[v.x][v.y] = v.z;
	}
	*/


	for(;;)
	{
		if(que.empty()) break;
		if(que.size() > 4000)
		{//無限ループ防止
			OnAssert(_T(__FILE__),__LINE__,false,_T("makeWay_sub_least関数内で異常に大きなキューが発生しました。"));
			break;
		}

		c4DVector state = que[0];
		que.pop_front();

		if(mapcost[state.x][state.y] < state.z)
		{//コストもっと小さいのがある。
			continue;
		}

		cCoordinate coo;
		int aspect;
		for(aspect=0;aspect<8;aspect+=2)
		{
			coo.SetAspect(aspect);
			c4DVector newstate;
			newstate.set( state.x + coo.x, state.y + coo.y ,0,0);
			
			if(newstate.x < MAPMAKINGEDGELANDWIDTH || newstate.x >= MAPWIDTH-MAPMAKINGEDGELANDWIDTH
				|| newstate.y < MAPMAKINGEDGELANDWIDTH || newstate.y >= MAPHEIGHT-MAPMAKINGEDGELANDWIDTH)
			{//端っこ
				continue;
			}
			
			
			newstate.z = state.z;//現在コスト
			
			int addcost = 0;

			if(landmap(newstate.x,newstate.y)->Landflag == MAPKIND_FLOOR)
			{//	部屋床なら
				addcost = 3;
			}
			else if(landmap(newstate.x,newstate.y)->Landflag == MAPKIND_FLOOR2)
			{//	廊下床なら
				//addcost = 3;
				addcost = 5;
			}
			else if(landmap(newstate.x,newstate.y)->Landflag == MAPKIND_WATER)
			{//水路
				//この時点の水路は部屋の中で、絶対通れない。(例外あり)
				addcost = 40000;
			}
			else if(landmap(newstate.x,newstate.y)->WallofwallroundIndex != DM_INDEX_UNDEFINE)
			{//部屋の壁
				//addcost = 29;//大きいと迂回したくなる//道が増えない
				addcost = 91;
			}
			else if(landmap(newstate.x,newstate.y)->WallofWay)
			{//通路の壁
				//addcost = 29;//大きいと迂回したくなる//道が増えない
				addcost = 80;
			}
			else
			{//そのた
				//addcost = 4;//小さいと道が増える
				addcost = 4;
			}

			if(ModeWay)
			{
				if(
				roundA->land_[0] == landmap(newstate.x,newstate.y)
				|| roundB->land_[0] == landmap(newstate.x,newstate.y)
				)
				{
					addcost += 10000;
				}
				else if(landmap(newstate.x,newstate.y)->WallofwallroundIndex != DM_INDEX_UNDEFINE)
				{//部屋の壁
					//addcost = 29;//大きいと迂回したくなる//道が増えない
					addcost += 91;
				}
			}

			newstate.z += addcost;

			if(newstate.z < mapcost[newstate.x][newstate.y])//現在コスト
			{//コストが小さい
				mapcost[newstate.x][newstate.y] = newstate.z;
				que.push_back(newstate);
			}
			
		}
		
	}

	c4DVector goal(0,0,1000000000,0);
	i = roundB->land_.size()*random();
	goal.set(roundB->land_[i]->pos.x, roundB->land_[i]->pos.y , mapcost[roundB->land_[i]->pos.x][roundB->land_[i]->pos.y],0);
	/*
	
	for(i=0;i<roundB->land_.size();i++)
	{
		if( goal.z > mapcost[roundB->land_[i]->pos.x][roundB->land_[i]->pos.y])
		{//最低値
			goal.set(roundB->land_[i]->pos.x, roundB->land_[i]->pos.y , mapcost[roundB->land_[i]->pos.x][roundB->land_[i]->pos.y],0);
		}
	}
	*/

	int aspectstart = (int)(random()*4)*2;

	c4DVector state = goal;
	for(i=0;;i++)
	{

		
		cCoordinate coo;
		int aspect;
		c4DVector newstate(-1,-1,1000000000,0);
		c4DVector tempstate = state;
		for(aspect=aspectstart;aspect<aspectstart+8;aspect+=2)
		{
			coo.SetAspect(aspect);
			
			tempstate.set( state.x + coo.x, state.y + coo.y ,0,0);
			

			if(newstate.z > mapcost[tempstate.x][tempstate.y])//現在コスト
			{//コストが小さい
				newstate.set(tempstate.x, tempstate.y , mapcost[tempstate.x][tempstate.y],0);
			}
		}
		if(newstate.x == -1 || i > 4000)
		{
			OnAssert(_T(__FILE__),__LINE__,false,_T("makeWay_sub_least関数内でコストマップに異常を感知しました。"));
			break;
		}


		if(newstate.z <= 0)
		{
			break;
		}



		makeWay_line(landmap(state.x,state.y),landmap(newstate.x,newstate.y));


		state = newstate;


	}

	

	return;
}

	//仮実装	
void cDMMap::makeWay_sub(pcDMLand landA, pcDMLand landB, int aspect)
{


	pcDMLand land;
	
	//aspectはaからのあすぺくと

	cCoordinate coo, center;
	//pcDMLand land;
	center.Set( landA->pos.x/2 + landB->pos.x/2, landA->pos.y/2 + landB->pos.y/2);
	c4DVector v,centerv(center.x,center.y,0,0);



	land = landA;
	
	c4DVector coov;


	v = centerv - landA->pos;
	coo.SetAspect(aspect);
	coov.set(coo.x,coo.y,0,0);
	if(v*coov < 0)
	{
		aspect = aspect+4;
		land = makeWay_sub(land, aspect, -v*coov);
	}
	else
	{
		land = makeWay_sub(land, aspect, v*coov);
	}


	
	v = landB->pos - land->pos;
	aspect = aspect+2;
	coo.SetAspect(aspect);
	coov.set(coo.x,coo.y,0,0);
	if(v*coov < 0)
	{
		aspect = aspect+4;
		land = makeWay_sub(land, aspect, -v*coov);
	}
	else
	{
		land = makeWay_sub(land, aspect, v*coov);
	}

	v = landB->pos - land->pos;
	aspect = aspect+2;
	coo.SetAspect(aspect);
	coov.set(coo.x,coo.y,0,0);
	if(v*coov < 0)
	{
		aspect = aspect+4;
		land = makeWay_sub(land, aspect, -v*coov);
	}
	else
	{
		land = makeWay_sub(land, aspect, v*coov);
	}
	
	

}
pcDMLand cDMMap::makeWay_sub(pcDMLand land, int aspect, int Num)
{
	
	if(Num <= 0) return land;

	cCoordinate coo;
	pcDMLand landfor;//,landright,landleft,landback;
	pcDMLand aspland;
	//landfor = land;
	int i,k;
	for(i=0;i<Num;i++)
	{
		
		
		if(land->Landflag == MAPKIND_FLOOR ) continue;

		coo.SetAspect(aspect);
		landfor = landmap(land->pos.x + coo.x, land->pos.y + coo.y);
		
		/*
		coo.SetAspect(aspect + 2);
		landright = landmap(land->pos.x + coo.x, land->pos.y + coo.y);
		coo.SetAspect(aspect - 2);
		landleft = landmap(land->pos.x + coo.x, land->pos.y + coo.y);
		coo.SetAspect(aspect + 4);
		landback = landmap(land->pos.x + coo.x, land->pos.y + coo.y);
		*/

		land->Landflag = MAPKIND_FLOOR2;
		land->WallofWay = false;
		
		for(k=aspect;k<aspect+8;k++)
		{
			coo.SetAspect(k);
			aspland = landmap(land->pos.x + coo.x, land->pos.y + coo.y);
			aspland->WallofWay = true && (aspland->Landflag != MAPKIND_FLOOR2) && (aspland->Landflag != MAPKIND_FLOOR);
		}



		/*
		landright->WallofWay = true && (landright->Landflag != MAPKIND_FLOOR2) && (landright->Landflag != MAPKIND_FLOOR);
		landleft->WallofWay = true && (landleft->Landflag != MAPKIND_FLOOR2) && (landleft->Landflag != MAPKIND_FLOOR);
		landback->WallofWay = true && (landback->Landflag != MAPKIND_FLOOR2) && (landback->Landflag != MAPKIND_FLOOR);
		landfor->WallofWay = true && (landfor->Landflag != MAPKIND_FLOOR2) && (landfor->Landflag != MAPKIND_FLOOR);

		*/


		//test
		//if(landright->WallofWay && landright->Landflag != MAPKIND_FLOOR2) landright->Landflag = MAPKIND_WATER;
		//if(landleft->WallofWay && landleft->Landflag != MAPKIND_FLOOR2) landleft->Landflag = MAPKIND_WATER;
		//if(landback->WallofWay && landback->Landflag != MAPKIND_FLOOR2) landback->Landflag = MAPKIND_WATER;
		//if(landfor->WallofWay && landfor->Landflag != MAPKIND_FLOOR2) landfor->Landflag = MAPKIND_WATER;

		land = landfor;

	
	}

	if(land->Landflag != MAPKIND_FLOOR )
	{

		coo.SetAspect(aspect);
		landfor = landmap(land->pos.x + coo.x, land->pos.y + coo.y);
		
		/*
		coo.SetAspect(aspect + 2);
		landright = landmap(land->pos.x + coo.x, land->pos.y + coo.y);
		coo.SetAspect(aspect - 2);
		landleft = landmap(land->pos.x + coo.x, land->pos.y + coo.y);
		coo.SetAspect(aspect + 4);
		landback = landmap(land->pos.x + coo.x, land->pos.y + coo.y);
		*/

		land->Landflag = MAPKIND_FLOOR2;
		land->WallofWay = false;
		
		for(k=aspect;k<aspect+8;k++)
		{
			coo.SetAspect(k);
			aspland = landmap(land->pos.x + coo.x, land->pos.y + coo.y);
			aspland->WallofWay = true && (aspland->Landflag != MAPKIND_FLOOR2) && (aspland->Landflag != MAPKIND_FLOOR);
		}



		/*
		landright->WallofWay = true && (landright->Landflag != MAPKIND_FLOOR2) && (landright->Landflag != MAPKIND_FLOOR);
		landleft->WallofWay = true && (landleft->Landflag != MAPKIND_FLOOR2) && (landleft->Landflag != MAPKIND_FLOOR);
		landback->WallofWay = true && (landback->Landflag != MAPKIND_FLOOR2) && (landback->Landflag != MAPKIND_FLOOR);
		landfor->WallofWay = true && (landfor->Landflag != MAPKIND_FLOOR2) && (landfor->Landflag != MAPKIND_FLOOR);

		*/


		//test
		//if(landright->WallofWay && landright->Landflag != MAPKIND_FLOOR2) landright->Landflag = MAPKIND_WATER;
		//if(landleft->WallofWay && landleft->Landflag != MAPKIND_FLOOR2) landleft->Landflag = MAPKIND_WATER;
		//if(landback->WallofWay && landback->Landflag != MAPKIND_FLOOR2) landback->Landflag = MAPKIND_WATER;
		//if(landfor->WallofWay && landfor->Landflag != MAPKIND_FLOOR2) landfor->Landflag = MAPKIND_WATER;


	}


	return land;
}
pcDMLand cDMMap::makeWater_sub(pcDMLand land, int aspect, int Num)
{

	if(Num <= 0) return land;

	cCoordinate coo;
	pcDMLand landfor;//,landright,landleft,landback;
	pcDMLand aspland;
	//landfor = land;
	int i,k;
	for(i=0;i<Num;i++)
	{
		
		//通路床は上書きできない！
		if(land->Landflag == MAPKIND_FLOOR2 ) continue;


		makeWater_sub(land);


		coo.SetAspect(aspect);
		landfor = landmap(land->pos.x + coo.x, land->pos.y + coo.y);
		land = landfor;

	
	}

	if(land->Landflag != MAPKIND_FLOOR2 )
	{


		makeWater_sub(land);


		coo.SetAspect(aspect);
		landfor = landmap(land->pos.x + coo.x, land->pos.y + coo.y);	
		land = landfor;

	}

	return land;
}
pcDMLand cDMMap::makeWater_sub(pcDMLand land)
{

	pcDMLand aspland;
	cCoordinate coo;

	if(land->Landflag == MAPKIND_WALLEGDE) return land;

	land->Landflag = MAPKIND_WATER;
	land->WallofWater = false;

	int aspect = 0,k;
	
	for(k=aspect;k<aspect+8;k++)
	{
		coo.SetAspect(k);
		aspland = landmap(land->pos.x + coo.x, land->pos.y + coo.y);
		aspland->WallofWater = true 
			&& (aspland->Landflag != MAPKIND_WATER)
			&& (aspland->roomIndex != DM_INDEX_UNDEFINE);
		
		//test
		//if(aspland->WallofWater && aspland->Landflag != MAPKIND_WATER) aspland->Landflag = MAPKIND_WATER;
	
	}

	return land;
}
pcDMLand cDMWallround::introExit(cCoordinate center)
{
	random_shuffle(walloutland_.begin(), walloutland_.end(), random_int);

	pcDMMap pdmmap = wpmap_.lock();
	pcDMRoom pdmroom = wproom_.lock();
	vector<pcDMLand> landlist;

	if(wproom_.lock()->RoomMode == DEF_ROOMMODE_Way)
	{
		int i = 0;
	}
	int i,k,m;
	for(i=0;i<walloutland_.size();i++)
	{
		pcDMLand land = walloutland_[i];
		int check = false;

		
		//方向判定
		if(pdmroom->RoomMode != DEF_ROOMMODE_Way)
		{
			
			//方向判定
			c4DVector exitvec, exittocenter;
			exittocenter.set(center.x - land->pos.x, center.y - land->pos.y,0,0);
			exitvec.set(land->pos.x - pdmroom->CenterX, land->pos.y - pdmroom->CenterY,0,0);
			
			if(exittocenter*exitvec <= 0)
			{//方向あってない
				continue;
			}

		}

		for(m=0;m<exit_.size();m++)
		{
			cCoordinate dis;
			dis.Set( exit_[m]->pos.x - land->pos.x, exit_[m]->pos.y - land->pos.y);
			if((pdmroom->RoomMode != DEF_ROOMMODE_Way && dis.dif() <= 1)
				|| pdmroom->RoomMode == DEF_ROOMMODE_Way && dis.dif() == 0)
			{
				check = true;//他の出口に隣接してる！
				break;
			}

			if(land->WallofWay)
			{
				check = true;//通路の壁！
				break;
			}
			
			if(land->pos.x < MAPMAKINGEDGELANDWIDTH || land->pos.y < MAPMAKINGEDGELANDWIDTH
				|| land->pos.x >= MAPWIDTH - MAPMAKINGEDGELANDWIDTH || land->pos.y >= MAPHEIGHT - MAPMAKINGEDGELANDWIDTH)
			{//はしっこすぎ
				check = true;
				break;
			}

			/*
			if(land_.size() > 4)
			{//部屋の大きさが4より大きいとき
				int q;
				int w_count = 0;
				for(q=0;q<8;q+=2)
				{
					cCoordinate coo;
					pcDMLand land2;
					coo.SetAspect(q);
					land2 = pdmmap->landmap(land->pos.x +coo.x, land->pos.y+ coo.y);
					if(land->roomIndex == land2->WallofroomIndex && land->wallroundIndex == land2->WallofwallroundIndex)
					{
						w_count++;
					}
				}
				if(w_count >= 2)
				{//角（２つ以上の壁に隣接している）
					check = true;
					break;
				}
			}
			*/
			if(pdmroom->RoomMode != DEF_ROOMMODE_Way)
			{

				//交差判定
				c4DVector land2center,goal;
				land2center.set(center.x - land->pos.x, center.y - land->pos.y,0,0);
				goal = land2center*2 + land->pos;

				if(cross_Judge_Flat( land->pos, goal, exit_[m]->pos, exitto_[m]->pos))
				{//交差してる
					check = true;
					break;
				}

				
			}
		}

		if(!check)
		{
			//条件にぴったり！
			landlist.push_back(land);

		}
	}

	pcDMLand introland;

	for(i=0;i<landlist.size();i++)
	{
		if(introExit_check(center, landlist[i],0)) return landlist[i];
	}
	for(i=0;i<landlist.size();i++)
	{
		if(introExit_check(center, landlist[i],1)) return landlist[i];
		if(introExit_check(center, landlist[i],-1)) return landlist[i];
	}
	for(i=0;i<landlist.size();i++)
	{
		if(introExit_check(center, landlist[i],2)) return landlist[i];
		if(introExit_check(center, landlist[i],-2)) return landlist[i];
	}

	if(wproom_.lock()->RoomMode == DEF_ROOMMODE_Way)
	{
		for(i=0;i<landlist.size();i++)
		{
			if(introExit_check(center, landlist[i],3)) return landlist[i];
			if(introExit_check(center, landlist[i],-3)) return landlist[i];
		}
		for(i=0;i<landlist.size();i++)
		{
			if(introExit_check(center, landlist[i],4)) return landlist[i];
		}
	}
	/*
	for(i=0;i<landlist.size();i++)
	{
		if(introExit_check(center, landlist[i],3)) return landlist[i];
		if(introExit_check(center, landlist[i],-3)) return landlist[i];
	}
	*/
	return (pcDMLand)((cDMLand*)NULL);//出口候補なし
}

//方向があってるかチェック
bool cDMWallround::introExit_check(cCoordinate center, pcDMLand land, int aspect_lean)
{
	int m;

	pcDMMap pdmmap = wpmap_.lock();

	cCoordinate coo;
	coo.Set( -land->pos.x + center.x, -land->pos.y + center.y);
	int aspect = coo.GetAspect();
	pcDMLand land2,land3;

	coo.SetAspect(aspect+aspect_lean + 4);//仕様変更により方向が逆
	
	land2 = pdmmap->landmap(land->pos.x +coo.x, land->pos.y+ coo.y);
	//coo = coo*2;
	//land3 = pdmmap->landmap(land->pos.x +coo.x, land->pos.y+ coo.y);
	if(wproom_.lock()->RoomIndex == land2->WallofroomIndex 
		&& WallroundIndex == land2->WallofwallroundIndex)
	{
		/*
		int check = false;
		for(m=0;m<exit_.size();m++)
		{
			cCoordinate dis;
			dis.Set( exit_[m]->pos.x - land->pos.x, exit_[m]->pos.y - land->pos.y);
			if(dis.dif() == 1)
			{
				check = true;//他の出口に隣接してる！
				break;
			}
			//交差判定
			c4DVector land2center,goal;
			land2center.set(center.x - land->pos.x, center.y - land->pos.y,0,0);
			goal = land2center*2 + land->pos;


			if(cross_Judge_Flat( land->pos, goal, exit_[m]->pos, exitto_[m]->pos))
			{//交差してる
				check = true;
				break;
			}
		}

		

		if(!check)
		{
			//条件にぴったり！
			return land;

		}
		*/

		return true;
	}

	return false;
}

void cDMWallround::WallroundOptimaze(pcDMLand pland)
{
	pcDMMap pdmmap = wpmap_.lock();
	
	cCoordinate coo;
	int aspect = 0;

	for(;;)
	{//端っこサーチ
		coo.SetAspect(aspect);
		pcDMLand pforw = pdmmap->landmap( pland->pos.x + coo.x, pland->pos.y + coo.y);
		if(pforw->roomIndex == DM_INDEX_UNDEFINE || pforw->Landflag == MAPKIND_WATER)
		{//正面がはしっこ
			break;
		}
		pland = pforw;
	}
	aspect += -2;//左を向く

	int roop;
	for(roop=0;;roop++)
	{
		pcDMLand pforw,pright,pdiag,pright2;
		coo.SetAspect(aspect);
		pforw = pdmmap->landmap( pland->pos.x + coo.x, pland->pos.y + coo.y);
		coo.SetAspect(aspect+1);
		pdiag = pdmmap->landmap( pland->pos.x + coo.x, pland->pos.y + coo.y);
		coo.SetAspect(aspect+2);
		pright = pdmmap->landmap( pland->pos.x + coo.x, pland->pos.y + coo.y);
		coo = coo*2;
		pright2 = pdmmap->landmap( pland->pos.x + coo.x, pland->pos.y + coo.y);
		

		if(pright->roomIndex == DM_INDEX_UNDEFINE || pright->Landflag == MAPKIND_WATER)
		{//右手が壁

			if(pright->WallofwallroundIndex == WallroundIndex && pland->wallroundIndex == WallroundIndex)
			{//一周した
				break;
			}
			
			if(pright->WallofwallroundIndex != WallroundIndex)
			{
				wall_.push_back(pright);
				pright->WallofwallroundIndex = WallroundIndex;
				pdiag->WallofwallroundIndex = WallroundIndex;
				
				pright->WallofroomIndex = wproom_.lock()->RoomIndex;
				pdiag->WallofroomIndex = wproom_.lock()->RoomIndex;
		
				pright->WallofrectIndex = wproom_.lock()->RectIndex;
				pdiag->WallofrectIndex = wproom_.lock()->RectIndex;	
				
			}

			if(pland->wallroundIndex != WallroundIndex)
			{
				land_.push_back(pland);
				
			}


			if(pright->Landflag != MAPKIND_WATER)
			{
				//出口候補になる壁の外
				walloutland_.push_back(pright2);

				//test
				//pright2->Landflag = MAPKIND_WATER;
			}
			//test
			//pright->Landflag = MAPKIND_WATER;
			//pdiag->Landflag = MAPKIND_WATER;
			

		}
		else
		{//壁から離れたので右に曲がる
			aspect += 2;
			coo.SetAspect(aspect);
			pforw = pdmmap->landmap( pland->pos.x + coo.x, pland->pos.y + coo.y);
			coo.SetAspect(aspect+1);
			pdiag = pdmmap->landmap( pland->pos.x + coo.x, pland->pos.y + coo.y);
			coo.SetAspect(aspect+2);
			pright = pdmmap->landmap( pland->pos.x + coo.x, pland->pos.y + coo.y);
			coo = coo*2;
			pright2 = pdmmap->landmap( pland->pos.x + coo.x, pland->pos.y + coo.y);
		

		}

		if(pforw->roomIndex == DM_INDEX_UNDEFINE || pforw->Landflag == MAPKIND_WATER)
		{//正面が壁
			//左を向く
			aspect += -2;
			pdiag->WallofwallroundIndex = WallroundIndex;
			pland->wallroundIndex = WallroundIndex;
		}
		else
		{//一歩進む
			pland = pforw;
		}


		if(roop>4000)
		{
			OnAssert(_T(__FILE__),__LINE__,false,_T("cDMWallround::WallroundOptimaze関数内で異常に大きなループが発生しました。"));
			break;
		}

	}
}

void cDMMap::Output()
{
	int x,y;

	/*
	//壁で埋める//保険
	for(x=0;x<MAPWIDTH;x++)
	{
		for(y=0;y<MAPHEIGHT;y++)
		{
			landmap(x,y)->ConcreteLand()->setAttribute(MAPKIND_WALL);
			landmap(x,y)->ConcreteLand()->RoomIndex = -1;
	
		}
	}
	sg_pDungeonSystem->Map().Land(-1,-1)->setAttribute(MAPKIND_WALLEGDE)
	*/



	//部屋リストクリア
	sg_pDungeonSystem->Map().RoomList.clear();
	int i,k,m;
	for(i=0;i<RoomNum;i++)
	{
		pcRoom room;
		room = pcRoom(new cRoom);
		room->Index = i;

		sg_pDungeonSystem->Map().RoomList.push_back(room);

		
	}

	
	for(i=0;i<Room_.size();i++)
	{
		if(Room_[i]->RoomIndex < 0) continue;
		sg_pDungeonSystem->Map().RoomList[Room_[i]->RoomIndex]->PartyFlag = Room_[i]->PartyFlag;
		sg_pDungeonSystem->Map().RoomList[Room_[i]->RoomIndex]->ShopFlag = Room_[i]->ShopFlag;
	}
	


	landmap_edge->Landflag = MAPKIND_WALLEGDE;
	landmap_edge->roomIndex = -1;
	for(x = 0 ; x < MAPWIDTH; x++)
	{
		for(y = 0 ; y < MAPHEIGHT; y++)
		{

			landmap(x,y)->OptimazeLand();
			
		}
	}
	landmap_edge->OptimazeLand();


	//端
	for(x=0;x<MAPWIDTH;x++)
	{
		for(y=0;y<MAPHEIGHT;y++)
		{
			if(x < MAPMAKINGEDGELANDWIDTH || x >= MAPWIDTH - MAPMAKINGEDGELANDWIDTH
				|| y < MAPMAKINGEDGELANDWIDTH || y >= MAPHEIGHT - MAPMAKINGEDGELANDWIDTH)
			{
				landmap(x,y)->ConcreteLand()->setAttribute(MAPKIND_WALLEGDE);
				landmap(x,y)->ConcreteLand()->RoomIndex = -1;
			}
		}
	}

	/*
	for(x=0;x<MAPWIDTH;x++)
	{
		landmap(x,0)->ConcreteLand()->setAttribute(MAPKIND_WALLEGDE);
		landmap(x,0)->ConcreteLand()->RoomIndex = -1;
		landmap(x,MAPHEIGHT-1)->ConcreteLand()->setAttribute(MAPKIND_WALLEGDE);
		landmap(x,MAPHEIGHT-1)->ConcreteLand()->RoomIndex = -1;
	}
	for(y=0;y<MAPHEIGHT;y++)
	{
		landmap(0,y)->ConcreteLand()->setAttribute(MAPKIND_WALLEGDE);
		landmap(0,y)->ConcreteLand()->RoomIndex = -1;
		landmap(MAPWIDTH-1,y)->ConcreteLand()->setAttribute(MAPKIND_WALLEGDE);
		landmap(MAPWIDTH-1,y)->ConcreteLand()->RoomIndex = -1;
	}
	*/
	/*
	//部屋を空ける
	pcRoom room;
	int roomindex = 0;
	int i;
	int memo[MAPWIDTH][MAPHEIGHT] = {0};

	for(i=0;i<Room_.size();i++)
	{

		if(Room_[i]->RoomMode == DEF_ROOMMODE_Way)
		{
			sg_pDungeonSystem->Map().Land(Room_[i]->Left(),Room_[i]->Top())->setAttribute(MAPKIND_FLOOR2);
			continue;
		}


		room = pcRoom(new cRoom);
		sg_pDungeonSystem->Map().RoomList.push_back(room);
		room->Index = roomindex;


		int j,k;
		for(j=0;j<Room_[i]->land_.size();j++)
		{
			room->LandformList.push_back(Room_[i]->land_[j]->ConcreteLand());
		}

		for(j=0;j<Room_[i]->wallround_.size();j++)
		{
			for(k=0;k<Room_[i]->wallround_[j]->wall_.size();k++)
			{
				room->LandformList.push_back(Room_[i]->wallround_[j]->wall_[k]->ConcreteLand());
			}
		}

		cCoordinate coo, a;

		for(k = 0;k < room->LandformList.size();k++)
		{
			coo = room->LandformList[k]->place;
			for(j=0;j<8;j++)
			{
				
				pcLandform land;
				a.SetAspect(j);
				a += coo;
				land = sg_pDungeonSystem->Map().Land(a);//隣接地形
				if(memo[land->place.x][land->place.y] == 1)continue;
				memo[land->place.x][land->place.y] = 1;//複重防止

				if(land->RoomIndex != roomindex )
				{	//部屋の中じゃない
					//部屋の隣接地形リストに追加
					room->LandformNeighborList.push_back(land);
					
				}

			}
		}


		//出口リストを作る。
		room->resetExitList();

		roomindex++;
	}

*/

	//int memo[MAPWIDTH][MAPHEIGHT] = {0};

	int j;
	int roomindex = 0;
	for(i=0;i<RoomNum;i++)
	{
		
		cCoordinate coo, a;
		pcRoom room;
		room = sg_pDungeonSystem->Map().RoomList[i];
		/*
		for(k = 0;k < room->LandformList.size();k++)
		{
			coo = room->LandformList[k]->place;
			for(j=0;j<8;j++)
			{
				
				pcLandform land;
				a.SetAspect(j);
				a += coo;
				land = sg_pDungeonSystem->Map().Land(a);//隣接地形
				if(memo[land->place.x][land->place.y] == 1)continue;
				memo[land->place.x][land->place.y] = 1;//複重防止

				if(land->RoomIndex != roomindex )
				{	//部屋の中じゃない
					//部屋の隣接地形リストに追加
					room->LandformNeighborList.push_back(land);
					
				}

			}
		}
		*/

		//付近リストを作る。
		room->resetNeighborList();
		//出口リストを作る。
		room->resetExitList();
		//roomindex++;
	}


}

pcLandform cDMLand::ConcreteLand()
{
	return sg_pDungeonSystem->Map().Land(pos.x, pos.y);
}
void cDMLand::OptimazeLand()
{

	/*//チェック用
	if(WallofroomIndex != -1)
	{
		Landflag = MAPKIND_WATER;
	}
*/

	if(roomIndex != DM_INDEX_WAY && roomIndex != DM_INDEX_UNDEFINE)
	{//通路ではない

		//if(roomIndex < RoomNum)
		//{
		if(ConcreteLand()->through(0,0,0))
		{
			sg_pDungeonSystem->Map().RoomList[roomIndex]->LandformList.push_back(ConcreteLand());
		}
		else
		{
			sg_pDungeonSystem->Map().RoomList[roomIndex]->isolatedLandformList.push_back(ConcreteLand());
		}
			
		//}
		//else
		//{
		//	OnAssert(_T(__FILE__),__LINE__,false,_T("cDMMap::Output　ルームインデックス異常"));
		//}
	}


	ConcreteLand()->setAttribute(Landflag);
	
	ConcreteLand()->RoomIndex = roomIndex;
	if(ConcreteLand()->RoomIndex == DM_INDEX_UNDEFINE) ConcreteLand()->RoomIndex = DM_INDEX_WAY;

}
cDMLand::cDMLand()
{

}
void cDMLand::Init()
{
	rectFrag = 0;
	roomIndex = DM_INDEX_UNDEFINE;
	wallroundIndex = DM_INDEX_UNDEFINE;
	WallofrectIndex = DM_INDEX_UNDEFINE;
	WallofroomIndex = DM_INDEX_UNDEFINE;
	WallofwallroundIndex = DM_INDEX_UNDEFINE;
	WallofWay = 0;
	WallofWater = 0;
	Landflag = 0;
}


//-----------------------------------
//ディバイドループ
//-----------------------------------
const int MINRECTSIZE = 6;

void cDMMap::MakeRect_RandomDivideRoop(vector<double>& divideVal)
{
	//変数0目標分割数
	if(divideVal.size() < 1)
	{//目標分割量（分割数とは一致しない）
		divideVal.resize(1,10);
	}
	//変数1最低分割数
	if(divideVal.size() < 2)
	{//最低分割数
		divideVal.resize(2,5);
	}
	pcDMRect newprect = pcDMRect(new cDMRect(selfwp_));
	newprect->Width = MAPWIDTH - MAPMAKINGEDGELANDWIDTH*2;
	newprect->Height = MAPHEIGHT - MAPMAKINGEDGELANDWIDTH*2;
	newprect->setLeft(MAPMAKINGEDGELANDWIDTH);
	newprect->setTop(MAPMAKINGEDGELANDWIDTH);
	Rects_.push_back(newprect);

	MakeRect_RandomDivideRoop_rectcount = 1;
	//divideVal[0] = 40;

	MakeRect_RandomDivideRoop_split(newprect,divideVal);

	int i;
	for(i=0;i<Rects_.size();i++)
	{
		Rects_[i]->Flag_ = DEF_RECTUSE_FREE;
		Rects_[i]->NeverDivide = FALSE;
	}
	return;
}
void cDMMap::MakeRect_RandomDivideRoop_split(pcDMRect prect, vector<double>& divideVal)
{
	int	amariWidth = max(prect->Width - MINRECTSIZE*2,0);
	int amariHeight = max(prect->Height - MINRECTSIZE*2,0);

	if(amariWidth+amariHeight == 0) return;//もう分割できない

	//大きさ
	double large = log(prect->Width * prect->Height) - log((double)MAPWIDTH*MAPHEIGHT/divideVal[0]);
	
	//適当なランダム打ち切り
	if(MakeRect_RandomDivideRoop_rectcount >= divideVal[1]
		&& ( -large)/log((double)MAPWIDTH*MAPHEIGHT/divideVal[0]) +0.5 > random()) return;

	cDiscreteProbability DP;
	DP.set(amariWidth,0);
	DP.set(amariHeight,1);

	if(DP.get(random()))
	{//Height
		int divine = prect->Top() + MINRECTSIZE + amariHeight*random();
		pcDMRect newprect = pcDMRect(new cDMRect(*prect));
		prect->formBottom(divine);
		newprect->formTop(divine);
		Rects_.push_back(newprect);

		MakeRect_RandomDivideRoop_rectcount++;

		if(random() > 0.5)
		{
			MakeRect_RandomDivideRoop_split(prect,divideVal);
			MakeRect_RandomDivideRoop_split(newprect,divideVal);
		}
		else
		{
			MakeRect_RandomDivideRoop_split(newprect,divideVal);
			MakeRect_RandomDivideRoop_split(prect,divideVal);
		}
	}
	else
	{//Width
		int divine = prect->Left() + MINRECTSIZE + amariWidth*random();
		pcDMRect newprect = pcDMRect(new cDMRect(*prect));
		prect->formRight(divine);
		newprect->formLeft(divine);
		Rects_.push_back(newprect);

		MakeRect_RandomDivideRoop_rectcount++;

		if(random() > 0.5)
		{
			MakeRect_RandomDivideRoop_split(prect,divideVal);
			MakeRect_RandomDivideRoop_split(newprect,divideVal);
		}
		else
		{
			MakeRect_RandomDivideRoop_split(newprect,divideVal);
			MakeRect_RandomDivideRoop_split(prect,divideVal);
		}
	}

}

void cDMMap::MakeRect_BigringAndgrid(vector<double>& divideVal)
{
	int width = MAPWIDTH - MAPMAKINGEDGELANDWIDTH*2;
	int height = MAPHEIGHT - MAPMAKINGEDGELANDWIDTH*2;

	if(divideVal.size() < 1) divideVal.resize(1,6);
	//分割数X軸最小
	divideVal[0] = max(3,divideVal[0]);//最小
	divideVal[0] = min(width/MINRECTSIZE,divideVal[0]);//最大
	if(divideVal.size() < 2) divideVal.resize(2,6);
	//分割数X軸最大
	divideVal[1] = max(3,divideVal[1]);//最小
	divideVal[1] = min(width/MINRECTSIZE,divideVal[1]);//最大

	if(divideVal.size() < 3) divideVal.resize(3,4);
	//分割数Y軸最小
	divideVal[2] = max(3,divideVal[2]);//最小
	divideVal[2] = min(height/MINRECTSIZE,divideVal[2]);//最小
	if(divideVal.size() < 4) divideVal.resize(4,4);
	//分割数Y軸最大
	divideVal[3] = max(3,divideVal[3]);//最小
	divideVal[3] = min(height/MINRECTSIZE,divideVal[3]);//最小


	int i,k;
	int divideX = random_range(divideVal[0],divideVal[1]+1), divideY = random_range(divideVal[2],divideVal[3]+1);
	
	int	amariWidth = max(width - MINRECTSIZE*divideX,0);
	int amariHeight = max(height - MINRECTSIZE*divideY,0);

	double SumrandX = 0, SumrandY = 0;
	vector<double> randX(1,0),randY(1,0);
	for(i=0;i<divideX;i++)
	{
		SumrandX += random();
		randX.push_back(SumrandX);
	}
	for(i=0;i<divideY;i++)
	{
		SumrandY += random();
		randY.push_back(SumrandY);
	}

	for(i=0;i<=divideX;i++)
	{
		randX[i] = randX[i]/SumrandX * amariWidth;
	}
	for(i=0;i<=divideY;i++)
	{
		randY[i] = randY[i]/SumrandY * amariHeight;
	}


	vector<pcDMRect> rectv(divideY);
	vector<vector<pcDMRect>> rectmap(divideX,rectv);

	for(i=0;i<divideX;i++)
	{
		for(k=0;k<divideY;k++)
		{
			/*
			int left = 1+ width * (i)/divideX;
			int right = 1+ width * (i+1)/divideX;
			int top = 1+ height * (k)/divideY;
			int bottom = 1+ height * (k+1)/divideY;
			*/
			int left = MAPMAKINGEDGELANDWIDTH + randX[i] + (i)*MINRECTSIZE;
			int right = MAPMAKINGEDGELANDWIDTH + randX[i+1] + (i+1)*MINRECTSIZE;
			int top = MAPMAKINGEDGELANDWIDTH + randY[k] + (k)*MINRECTSIZE;
			int bottom = MAPMAKINGEDGELANDWIDTH + randY[k+1] + (k+1)*MINRECTSIZE;


			pcDMRect newprect = pcDMRect(new cDMRect(selfwp_));
			newprect->Width = right-left;
			newprect->Height = bottom-top;
			newprect->setLeft(left);
			newprect->setTop(top);
			newprect->Flag_ = DEF_RECTUSE_ROOM;//
			newprect->NeverDivide = FALSE;//自由

			if(i == 0 || i == divideX-1 
				|| k == 0 || k == divideY-1 )
			{//端っこ
				newprect->Flag_ = DEF_RECTUSE_Way;//
				newprect->NeverDivide = TRUE;//
			}

			rectmap[i][k] = newprect;
			Rects_.push_back(newprect);
		}
	}


	for(i=0;i<divideX-1;i++)
	{
		mustlink.push_back(pair<pcDMRect,pcDMRect>(rectmap[i][0],rectmap[i+1][0]));
		mustlink.push_back(pair<pcDMRect,pcDMRect>(rectmap[i][divideY-1],rectmap[i+1][divideY-1]));
		nevermultilink.push_back(pair<pcDMRect,pcDMRect>(rectmap[i][0],rectmap[i+1][0]));
		nevermultilink.push_back(pair<pcDMRect,pcDMRect>(rectmap[i][divideY-1],rectmap[i+1][divideY-1]));
	}
	for(k=0;k<divideY-1;k++)
	{
		mustlink.push_back(pair<pcDMRect,pcDMRect>(rectmap[0][k],rectmap[0][k+1]));
		mustlink.push_back(pair<pcDMRect,pcDMRect>(rectmap[divideX-1][k],rectmap[divideX-1][k+1]));
		nevermultilink.push_back(pair<pcDMRect,pcDMRect>(rectmap[0][k],rectmap[0][k+1]));
		nevermultilink.push_back(pair<pcDMRect,pcDMRect>(rectmap[divideX-1][k],rectmap[divideX-1][k+1]));
	}
}

void cDMMap::MakeRect_NetAndAnchor(vector<double>& divideVal)
{
	int width = MAPWIDTH - MAPMAKINGEDGELANDWIDTH*2;
	int height = MAPHEIGHT - MAPMAKINGEDGELANDWIDTH*2;

	if(divideVal.size() < 1) divideVal.resize(1,6);
	//分割数X軸最小
	divideVal[0] = max(3,divideVal[0]);//最小
	divideVal[0] = min(width/MINRECTSIZE,divideVal[0]);//最大
	if(divideVal.size() < 2) divideVal.resize(2,6);
	//分割数X軸最大
	divideVal[1] = max(3,divideVal[1]);//最小
	divideVal[1] = min(width/MINRECTSIZE,divideVal[1]);//最大

	if(divideVal.size() < 3) divideVal.resize(3,4);
	//分割数Y軸最小
	divideVal[2] = max(3,divideVal[2]);//最小
	divideVal[2] = min(height/MINRECTSIZE,divideVal[2]);//最小
	if(divideVal.size() < 4) divideVal.resize(4,4);
	//分割数Y軸最大
	divideVal[3] = max(3,divideVal[3]);//最小
	divideVal[3] = min(height/MINRECTSIZE,divideVal[3]);//最小


	int i,k;
	int divideX = random_range(divideVal[0],divideVal[1]+1), divideY = random_range(divideVal[2],divideVal[3]+1);

	int	amariWidth = max(width - MINRECTSIZE*divideX,0);
	int amariHeight = max(height - MINRECTSIZE*divideY,0);

	double SumrandX = 0, SumrandY = 0;
	vector<double> randX(1,0),randY(1,0);
	for(i=0;i<divideX;i++)
	{
		SumrandX += random();
		randX.push_back(SumrandX);
	}
	for(i=0;i<divideY;i++)
	{
		SumrandY += random();
		randY.push_back(SumrandY);
	}

	for(i=0;i<=divideX;i++)
	{
		randX[i] = randX[i]/SumrandX * amariWidth;
	}
	for(i=0;i<=divideY;i++)
	{
		randY[i] = randY[i]/SumrandY * amariHeight;
	}

	vector<pcDMRect> rectv(divideY);
	vector<vector<pcDMRect>> rectmap(divideX,rectv);

	for(i=0;i<divideX;i++)
	{
		for(k=0;k<divideY;k++)
		{
			/*
			int left = 1+ width * (i)/divideX;
			int right = 1+ width * (i+1)/divideX;
			int top = 1+ height * (k)/divideY;
			int bottom = 1+ height * (k+1)/divideY;
			*/
			int left = MAPMAKINGEDGELANDWIDTH + randX[i] + (i)*MINRECTSIZE;
			int right = MAPMAKINGEDGELANDWIDTH + randX[i+1] + (i+1)*MINRECTSIZE;
			int top = MAPMAKINGEDGELANDWIDTH + randY[k] + (k)*MINRECTSIZE;
			int bottom = MAPMAKINGEDGELANDWIDTH + randY[k+1] + (k+1)*MINRECTSIZE;


			pcDMRect newprect = pcDMRect(new cDMRect(selfwp_));
			newprect->Width = right-left;
			newprect->Height = bottom-top;
			newprect->setLeft(left);
			newprect->setTop(top);
			newprect->Flag_ = DEF_RECTUSE_Way;//
			newprect->NeverDivide = TRUE;//

			if(i == 0 || i == divideX-1 
				|| k == 0 || k == divideY-1 )
			{//端っこ
				newprect->Flag_ = DEF_RECTUSE_ROOM;//
				newprect->NeverDivide = FALSE;//自由
			}

			rectmap[i][k] = newprect;
			Rects_.push_back(newprect);
		}
	}


	for(i=0;i<divideX-1;i++)
	{
		for(k=1;k<divideY-1;k++)
		{
			mustlink.push_back(pair<pcDMRect,pcDMRect>(rectmap[i][k],rectmap[i+1][k]));
			nevermultilink.push_back(pair<pcDMRect,pcDMRect>(rectmap[i][k],rectmap[i+1][k]));
		}
	}
	for(i=1;i<divideX-1;i++)
	{
		for(k=0;k<divideY-1;k++)
		{
			mustlink.push_back(pair<pcDMRect,pcDMRect>(rectmap[i][k],rectmap[i][k+1]));
			nevermultilink.push_back(pair<pcDMRect,pcDMRect>(rectmap[i][k],rectmap[i][k+1]));
		}
	}
}


//ハブと衛星
void cDMMap::MakeRect_HabAndSatellite(vector<double>& divideVal)
{
	int width = MAPWIDTH - MAPMAKINGEDGELANDWIDTH*2;
	int height = MAPHEIGHT - MAPMAKINGEDGELANDWIDTH*2;
	int i,k;


	if(divideVal.size() < 1) divideVal.resize(1,6);
	//衛星数/2最小

	if(divideVal.size() < 2) divideVal.resize(2,6);
	//衛星数/2最大



	int divide = random_range(divideVal[0],divideVal[1]+1);

	if(random() < 0.5)
	{
		divide = max(1,divide);//最小
		divide = min(height/MINRECTSIZE,divide);//最大

		int divideX = 3;
		int divideY = divide;

		int	amariWidth = max(width - MINRECTSIZE*divideX,0);
		int amariHeight = max(height - MINRECTSIZE*divideY,0);

		double SumrandX = 0, SumrandY = 0;
		vector<double> randX(1,0),randY(1,0);
		for(i=0;i<divideX;i++)
		{
			SumrandX += random();
			randX.push_back(SumrandX);
		}
		for(i=0;i<divideY;i++)
		{
			SumrandY += random();
			randY.push_back(SumrandY);
		}

		for(i=0;i<=divideX;i++)
		{
			randX[i] = randX[i]/SumrandX * amariWidth;
		}
		for(i=0;i<=divideY;i++)
		{
			randY[i] = randY[i]/SumrandY * amariHeight;
		}

		pcDMRect mainprect;
		i=1;
		{//メイン中間層
			int left = MAPMAKINGEDGELANDWIDTH + randX[i] + (i)*MINRECTSIZE;
			int right = MAPMAKINGEDGELANDWIDTH + randX[i+1] + (i+1)*MINRECTSIZE;
			int top = MAPMAKINGEDGELANDWIDTH ;
			int bottom = MAPMAKINGEDGELANDWIDTH + height;

			pcDMRect newprect = pcDMRect(new cDMRect(selfwp_));
			newprect->Width = right-left;
			newprect->Height = bottom-top;
			newprect->setLeft(left);
			newprect->setTop(top);
			newprect->Flag_ = DEF_RECTUSE_ROOM;//
			newprect->NeverDivide = FALSE;//自由

			mainprect = newprect;

			Rects_.push_back(newprect);
		}


		for(i=0;i<divideX;i++)
		{
			if(i != 1)
			{
				for(k=0;k<divideY;k++)
				{
					/*
					int left = 1+ width * (i)/divideX;
					int right = 1+ width * (i+1)/divideX;
					int top = 1+ height * (k)/divideY;
					int bottom = 1+ height * (k+1)/divideY;
					*/
					int left = MAPMAKINGEDGELANDWIDTH + randX[i] + (i)*MINRECTSIZE;
					int right = MAPMAKINGEDGELANDWIDTH + randX[i+1] + (i+1)*MINRECTSIZE;
					int top = MAPMAKINGEDGELANDWIDTH + randY[k] + (k)*MINRECTSIZE;
					int bottom = MAPMAKINGEDGELANDWIDTH + randY[k+1] + (k+1)*MINRECTSIZE;

					pcDMRect newprect = pcDMRect(new cDMRect(selfwp_));
					newprect->Width = right-left;
					newprect->Height = bottom-top;
					newprect->setLeft(left);
					newprect->setTop(top);
					newprect->Flag_ = DEF_RECTUSE_ROOM;//
					newprect->NeverDivide = FALSE;//自由
					

					Rects_.push_back(newprect);

					mustlink.push_back(pair<pcDMRect,pcDMRect>(mainprect, newprect));
					
				}
			}

		}

		/*
		int divideX = 3;

		for(i=0;i<divideX;i++)
		{
			int left,right,top,bottom;
			pcDMRect newprect;

			left = 1+ width * (i)/divideX;
			right = 1+ width * (i+1)/divideX;
			top = 1;
			bottom = 1+ height;

			newprect = pcDMRect(new cDMRect(selfwp_));
			newprect->Width = right-left;
			newprect->Height = bottom-top;
			newprect->setLeft(left);
			newprect->setTop(top);
			Rects_.push_back(newprect);

			
			if(i != 1)
			{
				MakeRect_RandomDivideRoop_split(newprect,divideVal);
			}
			else
			{
			
				newprect->Flag_ = DEF_RECTUSE_ROOM;//
			}
		}
		*/
	}
	else
	{
		divide = max(1,divide);//最小
		divide = min(width/MINRECTSIZE,divide);//最大

		int divideX = divide;
		int divideY = 3;

		int	amariWidth = max(width - MINRECTSIZE*divideX,0);
		int amariHeight = max(height - MINRECTSIZE*divideY,0);

		double SumrandX = 0, SumrandY = 0;
		vector<double> randX(1,0),randY(1,0);
		for(i=0;i<divideX;i++)
		{
			SumrandX += random();
			randX.push_back(SumrandX);
		}
		for(i=0;i<divideY;i++)
		{
			SumrandY += random();
			randY.push_back(SumrandY);
		}

		for(i=0;i<=divideX;i++)
		{
			randX[i] = randX[i]/SumrandX * amariWidth;
		}
		for(i=0;i<=divideY;i++)
		{
			randY[i] = randY[i]/SumrandY * amariHeight;
		}

		pcDMRect mainprect;
		k=1;
		{//メイン中間層
			int left = MAPMAKINGEDGELANDWIDTH ;
			int right = MAPMAKINGEDGELANDWIDTH + width;
			int top = MAPMAKINGEDGELANDWIDTH + randY[k] + (k)*MINRECTSIZE;
			int bottom = MAPMAKINGEDGELANDWIDTH + randY[k+1] + (k+1)*MINRECTSIZE;

			pcDMRect newprect = pcDMRect(new cDMRect(selfwp_));
			newprect->Width = right-left;
			newprect->Height = bottom-top;
			newprect->setLeft(left);
			newprect->setTop(top);

			newprect->Flag_ = DEF_RECTUSE_FREE;//
			newprect->NeverDivide = FALSE;//自由

			mainprect = newprect;

			Rects_.push_back(newprect);
		}

		for(k=0;k<divideY;k++)
		{
		
			if(k != 1)
			{
				for(i=0;i<divideX;i++)
				{	
					/*
					int left = 1+ width * (i)/divideX;
					int right = 1+ width * (i+1)/divideX;
					int top = 1+ height * (k)/divideY;
					int bottom = 1+ height * (k+1)/divideY;
					*/
					int left = MAPMAKINGEDGELANDWIDTH + randX[i] + (i)*MINRECTSIZE;
					int right = MAPMAKINGEDGELANDWIDTH + randX[i+1] + (i+1)*MINRECTSIZE;
					int top = MAPMAKINGEDGELANDWIDTH + randY[k] + (k)*MINRECTSIZE;
					int bottom = MAPMAKINGEDGELANDWIDTH + randY[k+1] + (k+1)*MINRECTSIZE;

					pcDMRect newprect = pcDMRect(new cDMRect(selfwp_));
					newprect->Width = right-left;
					newprect->Height = bottom-top;
					newprect->setLeft(left);
					newprect->setTop(top);
					newprect->Flag_ = DEF_RECTUSE_ROOM;//
					newprect->NeverDivide = FALSE;//自由
					
					mustlink.push_back(pair<pcDMRect,pcDMRect>(mainprect, newprect));

					Rects_.push_back(newprect);
				}
			}

		}
		/*
		int divideY = 3;

		for(i=0;i<divideY;i++)
		{
			int left,right,top,bottom;
			pcDMRect newprect;

			left = 1;
			right = 1+ width;
			top = 1 + height * (i)/divideY;
			bottom = 1+ height * (i+1)/divideY;

			newprect = pcDMRect(new cDMRect(selfwp_));
			newprect->Width = right-left;
			newprect->Height = bottom-top;
			newprect->setLeft(left);
			newprect->setTop(top);
			Rects_.push_back(newprect);

			if(i != 1)
			{
				MakeRect_RandomDivideRoop_split(newprect,divideVal);
			}
			else
			{
				newprect->Flag_ = DEF_RECTUSE_ROOM;//
			}
			
		}
		*/
	}

	/*
	for(i=0;i<Rects_.size();i++)
	{
		if(Rects_[i]->Flag_ != DEF_RECTUSE_ROOM)
		{
			Rects_[i]->Flag_ = DEF_RECTUSE_FREE;
		}
		Rects_[i]->NeverDivide = FALSE;
	}
	*/
}


void cDMMap::MakeRect_SimpleGrid(vector<double>& divideVal)
{
	int width = MAPWIDTH - MAPMAKINGEDGELANDWIDTH*2;
	int height = MAPHEIGHT - MAPMAKINGEDGELANDWIDTH*2;

	if(divideVal.size() < 1) divideVal.resize(1,6);
	//分割数X軸最小
	divideVal[0] = max(1,divideVal[0]);//最小
	divideVal[0] = min(width/MINRECTSIZE,divideVal[0]);//最大
	if(divideVal.size() < 2) divideVal.resize(2,6);
	//分割数X軸最大
	divideVal[1] = max(1,divideVal[1]);//最小
	divideVal[1] = min(width/MINRECTSIZE,divideVal[1]);//最大

	if(divideVal.size() < 3) divideVal.resize(3,4);
	//分割数Y軸最小
	divideVal[2] = max(1,divideVal[2]);//最小
	divideVal[2] = min(height/MINRECTSIZE,divideVal[2]);//最小
	if(divideVal.size() < 4) divideVal.resize(4,4);
	//分割数Y軸最大
	divideVal[3] = max(1,divideVal[3]);//最小
	divideVal[3] = min(height/MINRECTSIZE,divideVal[3]);//最小


	int i,k;
	int divideX = random_range(divideVal[0],divideVal[1]+1), divideY = random_range(divideVal[2],divideVal[3]+1);

	int	amariWidth = max(width - MINRECTSIZE*divideX,0);
	int amariHeight = max(height - MINRECTSIZE*divideY,0);

	double SumrandX = 0, SumrandY = 0;
	vector<double> randX(1,0),randY(1,0);
	for(i=0;i<divideX;i++)
	{
		SumrandX += random();
		randX.push_back(SumrandX);
	}
	for(i=0;i<divideY;i++)
	{
		SumrandY += random();
		randY.push_back(SumrandY);
	}

	for(i=0;i<=divideX;i++)
	{
		randX[i] = randX[i]/SumrandX * amariWidth;
	}
	for(i=0;i<=divideY;i++)
	{
		randY[i] = randY[i]/SumrandY * amariHeight;
	}

	vector<pcDMRect> rectv(divideY);
	vector<vector<pcDMRect>> rectmap(divideX,rectv);

	for(i=0;i<divideX;i++)
	{
		for(k=0;k<divideY;k++)
		{
			/*
			int left = 1+ width * (i)/divideX;
			int right = 1+ width * (i+1)/divideX;
			int top = 1+ height * (k)/divideY;
			int bottom = 1+ height * (k+1)/divideY;
			*/
			int left = MAPMAKINGEDGELANDWIDTH + randX[i] + (i)*MINRECTSIZE;
			int right = MAPMAKINGEDGELANDWIDTH + randX[i+1] + (i+1)*MINRECTSIZE;
			int top = MAPMAKINGEDGELANDWIDTH + randY[k] + (k)*MINRECTSIZE;
			int bottom = MAPMAKINGEDGELANDWIDTH + randY[k+1] + (k+1)*MINRECTSIZE;

			pcDMRect newprect = pcDMRect(new cDMRect(selfwp_));
			newprect->Width = right-left;
			newprect->Height = bottom-top;
			newprect->setLeft(left);
			newprect->setTop(top);
			newprect->Flag_ = DEF_RECTUSE_FREE;//
			newprect->NeverDivide = FALSE;//自由


			rectmap[i][k] = newprect;
			Rects_.push_back(newprect);
		}
	}


}

void cDMMap::MakeRect_BigOneRoom(vector<double>& divideVal)
{
	if(divideVal.size() < 1) divideVal.resize(1,0.0);
	//分割部屋を許す確率
	divideVal[0] = max(0,divideVal[0]);//最小
	divideVal[0] = min(1,divideVal[0]);//最大

	int width = MAPWIDTH - MAPMAKINGEDGELANDWIDTH*2;
	int height = MAPHEIGHT - MAPMAKINGEDGELANDWIDTH*2;

	int left = MAPMAKINGEDGELANDWIDTH;
	int right = left + width;
	int top = MAPMAKINGEDGELANDWIDTH;
	int bottom = top + height;

	pcDMRect newprect = pcDMRect(new cDMRect(selfwp_));
	newprect->Width = right-left;
	newprect->Height = bottom-top;
	newprect->setLeft(left);
	newprect->setTop(top);
	newprect->Flag_ = DEF_RECTUSE_ROOM;//
	newprect->NeverDivide = (divideVal[0] > random())? FALSE: TRUE;


	Rects_.push_back(newprect);

}
void cDMMap::MakeRect_TwoRoom(vector<double>& divideVal)
{
	if(divideVal.size() < 1) divideVal.resize(1,0.0);
	//分割部屋を許す確率
	divideVal[0] = max(0,divideVal[0]);//最小
	divideVal[0] = min(1,divideVal[0]);//最大

	int width = MAPWIDTH - MAPMAKINGEDGELANDWIDTH*2;
	int height = MAPHEIGHT - MAPMAKINGEDGELANDWIDTH*2;


	int left = MAPMAKINGEDGELANDWIDTH;
	int right = left + width;
	int top = MAPMAKINGEDGELANDWIDTH;
	int bottom = top + height;

	pcDMRect prect = pcDMRect(new cDMRect(selfwp_));
	prect->Width = right-left;
	prect->Height = bottom-top;
	prect->setLeft(left);
	prect->setTop(top);


	int	amariWidth = max(width - MINRECTSIZE*2,0);
	int amariHeight = max(height - MINRECTSIZE*2,0);


	cDiscreteProbability DP;
	DP.set(amariWidth,0);
	DP.set(amariHeight,1);

	if(DP.get(random()))
	{//Height
		int divine = prect->Top() + MINRECTSIZE + amariHeight/2;
		pcDMRect newprect = pcDMRect(new cDMRect(*prect));
		prect->formBottom(divine);
		newprect->formTop(divine);
		Rects_.push_back(newprect);
		Rects_.push_back(prect);

		newprect->Flag_ = DEF_RECTUSE_ROOM;//
		newprect->NeverDivide = (divideVal[0] > random())? FALSE: TRUE;

		prect->Flag_ = DEF_RECTUSE_ROOM;//
		prect->NeverDivide = (divideVal[0] > random())? FALSE: TRUE;
	}
	else
	{//Width
		int divine = prect->Left() + MINRECTSIZE + amariWidth/2;
		pcDMRect newprect = pcDMRect(new cDMRect(*prect));
		prect->formRight(divine);
		newprect->formLeft(divine);
		Rects_.push_back(newprect);
		Rects_.push_back(prect);
	
		newprect->Flag_ = DEF_RECTUSE_ROOM;//
		newprect->NeverDivide = (divideVal[0] > random())? FALSE: TRUE;

		prect->Flag_ = DEF_RECTUSE_ROOM;//
		prect->NeverDivide = (divideVal[0] > random())? FALSE: TRUE;

	}

}

void cDMMap::MakeRect_FourRoom(vector<double>& divideVal)
{
	int width = MAPWIDTH - MAPMAKINGEDGELANDWIDTH*2;
	int height = MAPHEIGHT - MAPMAKINGEDGELANDWIDTH*2;

	int i,k;
	int divideX = 2, divideY = 2;

	int	amariWidth = max(width - MINRECTSIZE*divideX,0);
	int amariHeight = max(height - MINRECTSIZE*divideY,0);

	double SumrandX = 0, SumrandY = 0;
	vector<double> randX(1,0),randY(1,0);
	for(i=0;i<divideX;i++)
	{
		SumrandX += 1.0;//固定
		randX.push_back(SumrandX);
	}
	for(i=0;i<divideY;i++)
	{
		SumrandY += 1.0;//固定
		randY.push_back(SumrandY);
	}

	for(i=0;i<=divideX;i++)
	{
		randX[i] = randX[i]/SumrandX * amariWidth;
	}
	for(i=0;i<=divideY;i++)
	{
		randY[i] = randY[i]/SumrandY * amariHeight;
	}

	vector<pcDMRect> rectv(divideY);
	vector<vector<pcDMRect>> rectmap(divideX,rectv);

	for(i=0;i<divideX;i++)
	{
		for(k=0;k<divideY;k++)
		{
			/*
			int left = 1+ width * (i)/divideX;
			int right = 1+ width * (i+1)/divideX;
			int top = 1+ height * (k)/divideY;
			int bottom = 1+ height * (k+1)/divideY;
			*/
			int left = MAPMAKINGEDGELANDWIDTH + randX[i] + (i)*MINRECTSIZE;
			int right = MAPMAKINGEDGELANDWIDTH + randX[i+1] + (i+1)*MINRECTSIZE;
			int top = MAPMAKINGEDGELANDWIDTH + randY[k] + (k)*MINRECTSIZE;
			int bottom = MAPMAKINGEDGELANDWIDTH + randY[k+1] + (k+1)*MINRECTSIZE;

			pcDMRect newprect = pcDMRect(new cDMRect(selfwp_));
			newprect->Width = right-left;
			newprect->Height = bottom-top;
			newprect->setLeft(left);
			newprect->setTop(top);
			newprect->Flag_ = DEF_RECTUSE_ROOM;//
			newprect->NeverDivide = (divideVal[0] > random())? FALSE: TRUE;//自由


			rectmap[i][k] = newprect;
			Rects_.push_back(newprect);
		}
	}

	mustlink.push_back(pair<pcDMRect,pcDMRect>(rectmap[0][0], rectmap[0][1]));
	mustlink.push_back(pair<pcDMRect,pcDMRect>(rectmap[0][0], rectmap[1][0]));
	mustlink.push_back(pair<pcDMRect,pcDMRect>(rectmap[1][0], rectmap[1][1]));
	mustlink.push_back(pair<pcDMRect,pcDMRect>(rectmap[0][1], rectmap[1][1]));
}



void cDMMap::MakeRect_RandomGrow(vector<double>& divideVal)
{
	//最低分割数
	DIVIDEVALDEF(0,5,1,10000);
	//最大分割数
	DIVIDEVALDEF(1,10,1,10000);

	int dividenum = random_range(divideVal[0], divideVal[1]+1);

	int seedsetable[MAPWIDTH][MAPHEIGHT] = {0};
	int notempty[MAPWIDTH][MAPHEIGHT] = {0};
	vector<pcDMRect> growrects;
	vector<pair<int,int>> randomcoordinate;
	//種を撒く
	int x,y;
	for(x=MAPMAKINGEDGELANDWIDTH;x<MAPWIDTH-MAPMAKINGEDGELANDWIDTH-MINRECTSIZE+1;x++)
	{
		for(y=MAPMAKINGEDGELANDWIDTH;y<MAPHEIGHT-MAPMAKINGEDGELANDWIDTH-MINRECTSIZE+1;y++)
		{
			randomcoordinate.push_back(pair<int,int>(x,y));
		}
	}
	std::random_shuffle(randomcoordinate.begin(), randomcoordinate.end(), random_int);
	int i;
	int rectcount = 0;
	int ix,iy;
	for(i=0;i<randomcoordinate.size();i++)
	{
		ix = randomcoordinate[i].first;
		iy = randomcoordinate[i].second;
		if(seedsetable[ix][iy] == 0)
		{
			//種巻き
			for(x=ix-MINRECTSIZE+1;x<ix+MINRECTSIZE;x++)
			{
				if(x < MAPMAKINGEDGELANDWIDTH || x >= MAPWIDTH-MAPMAKINGEDGELANDWIDTH-MINRECTSIZE+1)
				{
					continue;
				}
				for(y=iy-MINRECTSIZE+1;y<iy+MINRECTSIZE;y++)
				{
					if(y < MAPMAKINGEDGELANDWIDTH || y >= MAPHEIGHT-MAPMAKINGEDGELANDWIDTH-MINRECTSIZE+1)
					{
						continue;
					}
					seedsetable[x][y] = 1;
				}
			}
			for(x=ix;x<ix+MINRECTSIZE;x++)
			{
				if(x < MAPMAKINGEDGELANDWIDTH || x >= MAPWIDTH-MAPMAKINGEDGELANDWIDTH+1)
				{
					continue;
				}
				for(y=iy;y<iy+MINRECTSIZE;y++)
				{
					if(y < MAPMAKINGEDGELANDWIDTH || y >= MAPHEIGHT-MAPMAKINGEDGELANDWIDTH+1)
					{
						continue;
					}
					notempty[x][y] = 1;
				}
			}
			int left = ix;
			int right = left + MINRECTSIZE;
			int top = iy;
			int bottom = top + MINRECTSIZE;

			pcDMRect newprect = pcDMRect(new cDMRect(selfwp_));
			newprect->Width = right-left;
			newprect->Height = bottom-top;
			newprect->setLeft(left);
			newprect->setTop(top);
			newprect->Flag_ = DEF_RECTUSE_FREE;//
			newprect->NeverDivide = FALSE;//自由


			Rects_.push_back(newprect);
			growrects.push_back(newprect);

			rectcount++;

			if(rectcount >= dividenum) break;
		}

	}

	vector<int> priority;
	priority.push_back(0);
	priority.push_back(1);
	priority.push_back(2);
	priority.push_back(3);

	int count;
	int k,m;
	int width;
	int flag,notextendflag = 0,extendflag = 0;
	vector<int> neverextend(growrects.size(),0);
	//分割成長フェイズ
	for(count=0;;count++)
	{
		if(count > 4000)
		{//無限ループ防止
			OnAssert(_T(__FILE__),__LINE__,false,_T("MakeRect_RandomGrow関数内で異常に大きなループが発生しました。"));
			break;
		}

		notextendflag = 1;
		for(i=0;i<growrects.size();i++)
		{
			std::random_shuffle(priority.begin(), priority.end(), random_int);
			if(neverextend[i])
			{
				continue;
			}
			extendflag = 0;
			for(k=0;k<priority.size();k++)
			{
				c4DVector v;
				if(priority[k] == 0)
				{//上
					ix = growrects[i]->Left();
					iy = growrects[i]->Top()-1;
					if(ix < MAPMAKINGEDGELANDWIDTH || ix >= MAPWIDTH-MAPMAKINGEDGELANDWIDTH
						|| iy < MAPMAKINGEDGELANDWIDTH || iy >= MAPHEIGHT-MAPMAKINGEDGELANDWIDTH)
					{
						continue;
					}

					width = growrects[i]->Width;
					flag = 0;
					for(m=0;m<width;m++)
					{
						if(notempty[ix+m][iy])
						{//空でない
							flag = 1;
							break;
						}
					}
					if(flag) continue;

					for(m=0;m<width;m++)
					{
						notempty[ix+m][iy] = 1;
					}

					//拡張可能
					growrects[i]->formTop(iy);

					extendflag = 1;
					break;
				}
				else if(priority[k] == 1)
				{//下
					ix = growrects[i]->Left();
					iy = growrects[i]->Bottom();
					if(ix < MAPMAKINGEDGELANDWIDTH || ix >= MAPWIDTH-MAPMAKINGEDGELANDWIDTH
						|| iy < MAPMAKINGEDGELANDWIDTH || iy >= MAPHEIGHT-MAPMAKINGEDGELANDWIDTH)
					{
						continue;
					}

					width = growrects[i]->Width;
					flag = 0;
					for(m=0;m<width;m++)
					{
						if(notempty[ix+m][iy])
						{//空でない
							flag = 1;
							break;
						}
					}
					if(flag) continue;

					for(m=0;m<width;m++)
					{
						notempty[ix+m][iy] = 1;
					}

					//拡張可能
					growrects[i]->formBottom(iy+1);
				
					extendflag = 1;
					break;
				}
				else if(priority[k] == 2)
				{//左
					ix = growrects[i]->Left()-1;
					iy = growrects[i]->Top();
					if(ix < MAPMAKINGEDGELANDWIDTH || ix >= MAPWIDTH-MAPMAKINGEDGELANDWIDTH
						|| iy < MAPMAKINGEDGELANDWIDTH || iy >= MAPHEIGHT-MAPMAKINGEDGELANDWIDTH)
					{
						continue;
					}

					width = growrects[i]->Height;
					flag = 0;
					for(m=0;m<width;m++)
					{
						if(notempty[ix][iy+m])
						{//空でない
							flag = 1;
							break;
						}
					}
					if(flag) continue;

					for(m=0;m<width;m++)
					{
						notempty[ix][iy+m] = 1;
					}

					//拡張可能
					growrects[i]->formLeft(ix);

					extendflag = 1;
					break;
				}
				else if(priority[k] == 3)
				{//右
					ix = growrects[i]->Right();
					iy = growrects[i]->Top();
					if(ix < MAPMAKINGEDGELANDWIDTH || ix >= MAPWIDTH-MAPMAKINGEDGELANDWIDTH
						|| iy < MAPMAKINGEDGELANDWIDTH || iy >= MAPHEIGHT-MAPMAKINGEDGELANDWIDTH)
					{
						continue;
					}

					width = growrects[i]->Height;
					flag = 0;
					for(m=0;m<width;m++)
					{
						if(notempty[ix][iy+m])
						{//空でない
							flag = 1;
							break;
						}
					}
					if(flag) continue;

					for(m=0;m<width;m++)
					{
						notempty[ix][iy+m] = 1;
					}

					//拡張可能
					growrects[i]->formRight(ix+1);

					extendflag = 1;
					break;
				}
			}
			
			if(!extendflag)
			{//拡張してない
				neverextend[i] = 1;
			}
			else
			{//拡張した
				notextendflag = 0;
			}

		}

		if(notextendflag) break;
	}
}

void cDMMap::MakeRect_RandomGridUnion(vector<double>& divideVal)
{
	int width = MAPWIDTH - MAPMAKINGEDGELANDWIDTH*2;
	int height = MAPHEIGHT - MAPMAKINGEDGELANDWIDTH*2;

	//分割数X軸最小
	DIVIDEVALDEF(0,5,1,width/MINRECTSIZE);
	//分割数X軸最大
	DIVIDEVALDEF(1,5,1,width/MINRECTSIZE);

	//分割数X軸最小
	DIVIDEVALDEF(2,5,1,height/MINRECTSIZE);
	//分割数X軸最大
	DIVIDEVALDEF(3,5,1,height/MINRECTSIZE);

	//統合回数最小
	DIVIDEVALDEF(4,2,0,10000);
	//統合回数最大
	DIVIDEVALDEF(5,8,1,10000);

	//最低分割区域数
	DIVIDEVALDEF(6,5,1,10000);

	int i,k;
	int divideX = random_range(divideVal[0],divideVal[1]+1), divideY = random_range(divideVal[2],divideVal[3]+1);

	int	amariWidth = max(width - MINRECTSIZE*divideX,0);
	int amariHeight = max(height - MINRECTSIZE*divideY,0);

	double SumrandX = 0, SumrandY = 0;
	vector<double> randX(1,0),randY(1,0);
	for(i=0;i<divideX;i++)
	{
		SumrandX += random();
		randX.push_back(SumrandX);
	}
	for(i=0;i<divideY;i++)
	{
		SumrandY += random();
		randY.push_back(SumrandY);
	}

	for(i=0;i<=divideX;i++)
	{
		randX[i] = randX[i]/SumrandX * amariWidth;
	}
	for(i=0;i<=divideY;i++)
	{
		randY[i] = randY[i]/SumrandY * amariHeight;
	}

	vector<pcDMRect> rectv(divideY);
	vector<vector<pcDMRect>> rectmap(divideX,rectv);

	vector<cRectObj> rectrangev(divideY);
	vector<vector<cRectObj>> rectrange(divideX,rectrangev);

	vector<pcDMRect> growrects;

	//初期設定
	for(i=0;i<divideX;i++)
	{
		for(k=0;k<divideY;k++)
		{
			/*
			int left = 1+ width * (i)/divideX;
			int right = 1+ width * (i+1)/divideX;
			int top = 1+ height * (k)/divideY;
			int bottom = 1+ height * (k+1)/divideY;
			*/
			int left = MAPMAKINGEDGELANDWIDTH + randX[i] + (i)*MINRECTSIZE;
			int right = MAPMAKINGEDGELANDWIDTH + randX[i+1] + (i+1)*MINRECTSIZE;
			int top = MAPMAKINGEDGELANDWIDTH + randY[k] + (k)*MINRECTSIZE;
			int bottom = MAPMAKINGEDGELANDWIDTH + randY[k+1] + (k+1)*MINRECTSIZE;

			pcDMRect newprect = pcDMRect(new cDMRect(selfwp_));
			newprect->Width = right-left;
			newprect->Height = bottom-top;
			newprect->setLeft(left);
			newprect->setTop(top);
			newprect->Flag_ = DEF_RECTUSE_FREE;//
			newprect->NeverDivide = FALSE;//自由

			rectrange[i][k].setLTRB(i,k,i+1,k+1);
			rectmap[i][k] = newprect;
		}
	}

	int rectnum = divideX*divideY;

	int unionnum = random_range(divideVal[4],divideVal[5]+1);

	cRectObj range;
	cDiscreteProbability dp;
	dp.set(divideX-1,0);
	dp.set(divideY-1,1);
	for(i=0;i<unionnum;i++)
	{
		int linkaX,linkbX,linkaY,linkbY;
		if(dp.get(random()) == 0)
		{//X
			int linkindex = random_range(0,divideY);
			linkaY = linkbY = linkindex;
			linkaX = random_range(0,divideX-1);
			linkbX = linkaX + 1;
		}
		else
		{//Y
			int linkindex = random_range(0,divideX);
			linkaX = linkbX = linkindex;
			linkaY = random_range(0,divideY-1);
			linkbY = linkaY + 1;
		}

		range.setTop(min(rectrange[linkaX][linkaY].Top(),rectrange[linkbX][linkbY].Top()));
		range.setLeft(min(rectrange[linkaX][linkaY].Left(),rectrange[linkbX][linkbY].Left()));
		range.formBottom(max(rectrange[linkaX][linkaY].Bottom(),rectrange[linkbX][linkbY].Bottom()));
		range.formRight(max(rectrange[linkaX][linkaY].Right(),rectrange[linkbX][linkbY].Right()));
		
		if(range.Width * range.Height != 2)
		{//1 , 1同士しかつながらない
			continue;
		}

		pcDMRect UnionedRect = rectmap[range.Left()][range.Top()];

		int x,y;
		int lossrectcount = 0;
		for(x=range.Left();x<range.Right();x++)
		{
			for(y=range.Top();y<range.Bottom();y++)
			{
				if(UnionedRect != rectmap[x][y])
				{
					lossrectcount++;
				}
			}
		}


		rectnum -= lossrectcount;
		if(rectnum < divideVal[6]) break;
	
		UnionedRect->setTop(rectmap[range.Left()][range.Top()]->Top());
		UnionedRect->setLeft(rectmap[range.Left()][range.Top()]->Left());
		UnionedRect->formBottom(rectmap[range.Right()-1][range.Bottom()-1]->Bottom());
		UnionedRect->formRight(rectmap[range.Right()-1][range.Bottom()-1]->Right());

		if(lossrectcount)
		{
			for(x=range.Left();x<range.Right();x++)
			{
				for(y=range.Top();y<range.Bottom();y++)
				{
					rectrange[x][y] = range;
					rectmap[x][y] = UnionedRect;
				}
			}

		}


	}

	set<pcDMRect> rectset;

	for(i=0;i<divideX;i++)
	{
		for(k=0;k<divideY;k++)
		{
			rectset.insert(rectmap[i][k]);
		}
	}

	set<pcDMRect>::iterator itr;
	for(itr=rectset.begin();itr!=rectset.end();itr++)
	{
		Rects_.push_back(*itr);
	}
}
//---------------------------------------------------------
//固定系
//---------------------------------------------------------
void cDMMap::decode_floor(const vector<vector<tstring>>& vvstr)
{
	int x,y;
	int ix,iy;
	set<int> temproomindexs;
	for(y=0;y<MAPHEIGHT;y++)
	{
		for(x=0;x<MAPWIDTH;x++)
		{
			landmap(x,y)->Landflag = MAPKIND_WALLEGDE;
			landmap(x,y)->roomIndex = -1;
		}
	}
	for(y=EDGELANDWIDTH;y<MAPHEIGHT-EDGELANDWIDTH;y++)
	{
		iy = y - EDGELANDWIDTH;
		if(!(vvstr.size() > iy)) continue;
		for(x=EDGELANDWIDTH;x<MAPWIDTH-EDGELANDWIDTH;x++)
		{

			ix = x - EDGELANDWIDTH;
			if(!(vvstr[iy].size() > ix)) continue;

			if(vvstr[iy][ix] == _T("WL"))
			{//壁
				landmap(x,y)->Landflag = MAPKIND_WALL;
				landmap(x,y)->roomIndex = -1;
			}
			else if(vvstr[iy][ix] == _T("WE"))
			{//壁端
				landmap(x,y)->Landflag = MAPKIND_WALLEGDE;
				landmap(x,y)->roomIndex = -1;
			}
			else if(vvstr[iy][ix] == _T("WT"))
			{//水
				landmap(x,y)->Landflag = MAPKIND_WATER;
				landmap(x,y)->roomIndex = -1;
			}
			else if(vvstr[iy][ix] == _T("EM"))
			{//奈落
				landmap(x,y)->Landflag = MAPKIND_ABYSS;
				landmap(x,y)->roomIndex = -1;
			}
			else if(vvstr[iy][ix] == _T("WY"))
			{//奈落
				landmap(x,y)->Landflag = MAPKIND_FLOOR2;
				landmap(x,y)->roomIndex = -1;
			}
			else
			{
				int index = tstrtoi(vvstr[iy][ix]);
				if(index == -1)
				{//通路
					landmap(x,y)->Landflag = MAPKIND_FLOOR2;
					landmap(x,y)->roomIndex = -1;
				}
				else
				{
					temproomindexs.insert(index);
					landmap(x,y)->Landflag = MAPKIND_FLOOR;
					landmap(x,y)->roomIndex = index;
				}
			}
		}
	}
	landmap_edge->Landflag = MAPKIND_WALLEGDE;
	landmap_edge->roomIndex = -1;

	//部屋インデックス詰め
	RoomNum = temproomindexs.size();
	map<int,int> roomindextable;
	set<int>::iterator itr = temproomindexs.begin();
	int i;
	for(i=0;itr!=temproomindexs.end();(i++,itr++))
	{
		roomindextable.insert(pair<int,int>(*itr,i));
	}
	for(x=0;x<MAPWIDTH;x++)
	{
		for(y=0;y<MAPHEIGHT;y++)
		{
			if(landmap(x,y)->roomIndex != -1)
			{
				landmap(x,y)->roomIndex = roomindextable[landmap(x,y)->roomIndex];
			}
		}
	}

	//部屋リストクリア
	sg_pDungeonSystem->Map().RoomList.clear();
	int k,m;
	for(i=0;i<RoomNum;i++)
	{
		pcRoom room;
		room = pcRoom(new cRoom);
		room->Index = i;
		sg_pDungeonSystem->Map().RoomList.push_back(room);



	}

	/*
	for(i=0;i<Room_.size();i++)
	{
		//てすと
		for(k=0;k<Room_[i]->wallround_.size();k++)
		{
			for(m=0;m<Room_[i]->wallround_[k]->wall_.size();m++)
			{
				Room_[i]->wallround_[k]->wall_[m]->Landflag = MAPKIND_WATER;
			}
		}
	}
	*/

	for(x = 0 ; x < MAPWIDTH; x++)
	{
		for(y = 0 ; y < MAPHEIGHT; y++)
		{

			landmap(x,y)->OptimazeLand();
			
		}
	}
	landmap_edge->OptimazeLand();



	/*
	//部屋を空ける
	pcRoom room;
	int roomindex = 0;
	int i;
	int memo[MAPWIDTH][MAPHEIGHT] = {0};

	for(i=0;i<Room_.size();i++)
	{

		if(Room_[i]->RoomMode == DEF_ROOMMODE_Way)
		{
			sg_pDungeonSystem->Map().Land(Room_[i]->Left(),Room_[i]->Top())->setAttribute(MAPKIND_FLOOR2);
			continue;
		}


		room = pcRoom(new cRoom);
		sg_pDungeonSystem->Map().RoomList.push_back(room);
		room->Index = roomindex;


		int j,k;
		for(j=0;j<Room_[i]->land_.size();j++)
		{
			room->LandformList.push_back(Room_[i]->land_[j]->ConcreteLand());
		}

		for(j=0;j<Room_[i]->wallround_.size();j++)
		{
			for(k=0;k<Room_[i]->wallround_[j]->wall_.size();k++)
			{
				room->LandformList.push_back(Room_[i]->wallround_[j]->wall_[k]->ConcreteLand());
			}
		}

		cCoordinate coo, a;

		for(k = 0;k < room->LandformList.size();k++)
		{
			coo = room->LandformList[k]->place;
			for(j=0;j<8;j++)
			{
				
				pcLandform land;
				a.SetAspect(j);
				a += coo;
				land = sg_pDungeonSystem->Map().Land(a);//隣接地形
				if(memo[land->place.x][land->place.y] == 1)continue;
				memo[land->place.x][land->place.y] = 1;//複重防止

				if(land->RoomIndex != roomindex )
				{	//部屋の中じゃない
					//部屋の隣接地形リストに追加
					room->LandformNeighborList.push_back(land);
					
				}

			}
		}


		//出口リストを作る。
		room->resetExitList();

		roomindex++;
	}

*/

	
	//int memo[MAPWIDTH][MAPHEIGHT] = {0};

	int j;
	int roomindex = 0;
	for(i=0;i<RoomNum;i++)
	{
		
		cCoordinate coo, a;
		pcRoom room;
		room = sg_pDungeonSystem->Map().RoomList[i];
		/*
		for(k = 0;k < room->LandformList.size();k++)
		{
			coo = room->LandformList[k]->place;
			for(j=0;j<8;j++)
			{
				
				pcLandform land;
				a.SetAspect(j);
				a += coo;
				land = sg_pDungeonSystem->Map().Land(a);//隣接地形
				if(memo[land->place.x][land->place.y] == 1)continue;
				memo[land->place.x][land->place.y] = 1;//複重防止

				if(land->RoomIndex != roomindex )
				{	//部屋の中じゃない
					//部屋の隣接地形リストに追加
					room->LandformNeighborList.push_back(land);
					
				}

			}
		}
		*/
		//付近リストを作る。
		room->resetNeighborList();
		//出口リストを作る。
		room->resetExitList();
		//roomindex++;
	}



}