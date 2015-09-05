#include "stdafx.h"

#include "cDungeonSystem.h"

#include "cFloor.h"

#include "utility/debug.h"

bool cDungeonSystem::現在地からマッピング(pcCharacter pchara)
{
	return ある地点からマッピング(Map().Land(pchara->placeX, pchara->placeY));
}
bool cDungeonSystem::ある地点からマッピング(pcLandform pland)
{
	if(pland->RoomIndex <= -1)
	{//通路
		Map().mapping(pland->placeX, pland->placeY);
	}
	else
	{//部屋
		int i;
		pcRoom room = Map().RoomList[pland->RoomIndex];
		vector<pcLandform> vpland = room->allLandformList();
		for(i=0;i<vpland.size();i++)
		{
			Map().mapping(
				vpland[i]->placeX,
				vpland[i]->placeY);
		}
		/*
		for(i=0;i<room->LandformNeighborList.size();i++)
		{
			Map().pointmapping(
				room->LandformNeighborList[i]->placeX,
				room->LandformNeighborList[i]->placeY);
		}
		*/
	}

	return true;
}

bool cDungeonSystem::全マッピング()
{
	sg_pDungeonSystem->Map().Allmapping();
	return true;
}
bool cDungeonSystem::全マッピング解除()
{
	sg_pDungeonSystem->Map().EraseMiniMap(pPlayerChara()->placeX,pPlayerChara()->placeY);
	return true;
}
const int cDungeonSystem::フロア明かり状態()
{
	return floorlightswitch();
}
const int cDungeonSystem::フロア明かり状態ビジブル()
{
	return floorlightswitchvisible();
}
void cDungeonSystem::フロア明かり追加()
{
	floorlightswitch() = 1;//明かり状態
	AnimationManager().Anime_DisplayChange(&floorlightswitchvisible(), floorlightswitch());
}

void cDungeonSystem::フロア明かり削除()
{
	floorlightswitch() = 0;//明かり状態
	AnimationManager().Anime_DisplayChange(&floorlightswitchvisible(), floorlightswitch());
}
const int cDungeonSystem::フロア飛ばず状態()
{
	return (bool)(floornothrowswitch());
}
void cDungeonSystem::フロア飛ばず追加(int turn)
{
	floornothrowswitch() = turn;//飛ばず状態
}

void cDungeonSystem::フロア飛ばず削除()
{
	floornothrowswitch() = 0;//飛ばず状態
}

const int cDungeonSystem::フロア時間停止状態()
{
	return (bool)(floortimestopswitch());
}
void cDungeonSystem::フロア時間停止追加(int turn)
{
	floortimestopswitch() = turn;
}

void cDungeonSystem::フロア時間停止削除()
{
	floortimestopswitch() = 0;
}

const int cDungeonSystem::フロア時間飛ばし状態()
{
	return (bool)(floortimeskipswitch());
}
void cDungeonSystem::フロア時間飛ばし追加(int turn)
{
	floortimeskipswitch() = turn;
}

void cDungeonSystem::フロア時間飛ばし削除()
{
	floortimeskipswitch() = 0;
}

bool cDungeonSystem::八方判定(c4DVector from, c4DVector to)
{

	c4DVector v = to - from;

	if(v.x*v.y == 0)
	{
		return true;
	}

	if(abs(v.x/v.y) == 1)
	{
		return true;
	}

	return false;
}
bool cDungeonSystem::八方判定(cCoordinate from, cCoordinate to)
{

	cCoordinate v = to - from;

	if(v.x*v.y == 0)
	{
		return true;
	}

	if(abs(v.x) == abs(v.y))
	{
		return true;
	}

	return false;
}


bool cDungeonSystem::アイテムマッピング(pcDroping pdrop)
{
	//描画変更
	if(pdrop->isDiscover()) return true;
	int result = pdrop->OnMapping();
	//AnimationManager().Anime_DisplayChange(&(pdrop->opaque), result);
	return result;
}

bool cDungeonSystem::アイテム強制マッピング(pcDroping pdrop)
{
	//描画変更
	int result = pdrop->discover();
	//AnimationManager().Anime_DisplayChange(&(pdrop->opaque), result);
	return result;
}

void cDungeonSystem::setDungeonData(const tstring& DungeonID)
{
	/*
	tstring str;
	if(DungeonID == _T(""))
	{
		str = DataBase.DungeonImportDataFile(tstring(_T("first_Home")));
	}
	else
		str = DataBase.DungeonImportDataFile(DungeonID);
	if(str == _T("")) str = DungeonID;

	cScriptReader dungeon(IMPORTDATADIR + str);
	dungeon.load();
	dungeon.outputerrormassage(str +tstring(_T("_ImportDungeonFileData_Errorlog.txt")));
	*/
	pDungeon() = pcDungeon(new cDungeon);

	RandomInitGen().init_genrand(random_Seed());

	pDungeon()->decode(DataBase.DungeonImportDataFile(DungeonID));

	pDungeon()->DungeonID_ = DungeonID;
	pDungeon()->DungeonName_ = DataBase.DungeonImportDataName(DungeonID);

}
pcFloor cDungeonSystem::pFloor()
{
	return pDungeon()->pfloor(FloorLevel());
}

bool cDungeonSystem::穴掘り(pcLandform pland, int messageflag)
{
	if(pland->breakable())
	{
		return landchange(pland, MAPKIND_FLOOR2);
	}
	else if(pland->kind_surface() == MAPKIND_WALL && messageflag)
	{//見た目壁なのに掘れない
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("穴掘り失敗メッセージ"),valiable);
	}

	return false;
}
bool cDungeonSystem::水掘り(pcLandform pland, int messageflag)
{
	if(pland->breakable())
	{
		return landchange(pland, MAPKIND_WATER);
	}
	else if(pland->kind_surface() == MAPKIND_WALL && messageflag)
	{//見た目壁なのに掘れない
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("穴掘り失敗メッセージ"),valiable);
	}

	return false;
}
bool cDungeonSystem::水埋め(pcLandform pland, int messageflag)
{
	if(pland->driable())
	{
		return landchange(pland, MAPKIND_FLOOR2);
	}


	return false;
}
bool cDungeonSystem::部屋つぶし(pcLandform pland, int range, int messageflag)
{
	if(pland->RoomIndex == -1) return false;
	int eraseRoomIndex = pland->RoomIndex;
	pcRoom proom = Map().RoomList[eraseRoomIndex];

	
	vector<pcLandform> vpland = proom->allLandformList();
	
	/*
	int i;
	for(i=0;i<vpland.size();i++)
	{//地形塗りつぶし
		if(vpland[i]->kind_surface() == MAPKIND_FLOOR)
		{
			landchange(vpland[i], MAPKIND_FLOOR2);
		}

		vpland[i]->RoomIndex = -1;
	}
	*/
	int i;
	for(i=0;i<vpland.size();i++)
	{//地形塗りつぶし
		if((pland->place - vpland[i]->place).dif() < range)
		{
			if(vpland[i]->kind_surface() == MAPKIND_FLOOR)
			{
				landchange(vpland[i], MAPKIND_FLOOR2);
			}

			vpland[i]->RoomIndex = -1;
		}
	}



	//柱立て
	vpland = proom->LandformNonEdgeList;
	for(i=0;i<vpland.size();i++)
	{

		if((pland->placeX - vpland[i]->placeX + MAPWIDTH)%2 == 1
			&& (pland->placeY - vpland[i]->placeY + MAPWIDTH)%2 == 1
			&& vpland[i]->kind_surface() == MAPKIND_FLOOR2
			&& (pland->place - vpland[i]->place).dif() < range)
		{
			landchange(vpland[i], MAPKIND_FRAGILEWALL);
			if(vpland[i]->pOnChar != NULL) キャラ落下(vpland[i]->pOnChar,vpland[i]);
			if(vpland[i]->pOnDrop != NULL) 落ち物落下(vpland[i]->pOnDrop,vpland[i]);
		}
	}


	proom->stabilize();
	if(proom->allLandformList().empty())
	{//部屋なくなっちまっただ
		Map().RoomList.erase(Map().RoomList.begin() + eraseRoomIndex);
		for(i=eraseRoomIndex;i<Map().RoomList.size();i++)
		{//インデックス変更
			Map().RoomList[i]->ChangeIndex(i);
		}
	}

	地形整理();
	return false;
}
bool cDungeonSystem::大部屋(int messageflag)
{
	int i;
	int partyFlag = 0;
	int shopFlag = 0;
	for(i=0;i<Map().RoomList.size();i++)
	{
		partyFlag |= Map().RoomList[i]->PartyFlag;
		shopFlag |= Map().RoomList[i]->ShopFlag;
	}

	
	Map().RoomList.clear();
	Map().RoomList.push_back(pcRoom(new cRoom));
	Map().RoomList[0]->Index = 0;

	Map().RoomList[0]->PartyFlag = partyFlag;
	Map().RoomList[0]->ShopFlag = shopFlag;
	int x,y;
	for(x = 1; x < MAPWIDTH -1; x++)
	{
		for(y = 1; y < MAPHEIGHT -1; y++)
		{
			pcLandform pland = Map().Land(x,y);
			if(pland->getAttribute() != MAPKIND_WALLEGDE)
			{
				landchange(pland, MAPKIND_FLOOR);
				pland->RoomIndex = 0;
				Map().RoomList[0]->LandformList.push_back(pland);
			}
			else
			{
				pland->RoomIndex = -1;
			}
		}
	}

	if(messageflag)
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("大部屋メッセージ"),valiable);
	}

	for(i=0;i<Map().RoomList.size();i++)
	{
		Map().RoomList[i]->resetNeighborList();
		Map().RoomList[i]->resetExitList();
	}

	地形整理();
	return true;
}
bool cDungeonSystem::landchange(pcLandform pland, int kind)
{
	if(pland == NULL) return false;

	pland->setAttribute(kind);

	Map().changedLand.push_back(pland);
	AnimationManager().Anime_DisplayChange(pland->private_visiblekind_pointer(), kind);

	Resetminimap(pDevice_D3D);
	return true;
}
bool cDungeonSystem::地形整理()
{
	//部屋拡張、出口追加
	int memo[MAPWIDTH][MAPHEIGHT] = {0};
	deque<c4DVector> que;
	vector<c4DVector> list;
	int roomnum = Map().RoomList.size();

	const int minsizeofroom = 4;
	int x,y,i,k;

	for(x=0;x<MAPWIDTH-minsizeofroom;x++)
	{
		for(y=0;y<MAPHEIGHT-minsizeofroom;y++)
		{
			int breakflag = 0;
			int roomindex = -1;
			int isallroom = 1;
			for(i=0;i<minsizeofroom;i++)
			{
				for(k=0;k<minsizeofroom;k++)
				{
					if(Map().Land(x+i,y+k)->RoomIndex == -1)
					{
						isallroom = 0;
					}
					else
					{
						roomindex = Map().Land(x+i,y+k)->RoomIndex;
					}

					//if((Map().Land(x+i,y+k)->RoomIndex == -1) && !(Map().Land(x+i,y+k)->through(0,0)))
					if(!(Map().Land(x+i,y+k)->through(0,0,0)))
					{
						breakflag = 1;
						break;
					}
				}
				if(breakflag) break;


			}

			if(!breakflag)
			{//空きあり
				memo[x][y] = roomindex;
				if(!isallroom)
				{//空きありかつすべてルームでない＝拡張性がある
					if(roomindex == -1)
					{//全通路(新部屋の可能性)
						roomindex = roomnum;
						roomnum++;
					}
					c4DVector v;
					v.set(x,y,roomindex,0);
					que.push_back(v);
					list.push_back(v);//部屋拡張チェックリスト
				}
				
			}
			else
			{//空きなし
				memo[x][y] = -2;
			}
		}
	}

	/*
	//高速化のためのプリプロセス//未完成バグあり
	if(list.size() > 10)
	{
		deque<c4DVector> neoque;
		//vector<c4DVector> neolist = list;

		vector<c4DVector> group;
		vector<c4DVector> edgegroup;
		int grouproomindex = 400000;
		int flag[MAPWIDTH][MAPHEIGHT] = {0};
		int count = 0;

		int size = list.size();
		for(i=0;i<size;i++)
		{
			c4DVector firststate = list[i];
		

			for(;;)
			{
				if(neoque.empty()) break;
				if(count++ > 400000)
				{//無限ループ防止
					OnAssert(_T(__FILE__),__LINE__,false,_T("地形整理関数内 プリプロセスで異常に大きなキューが発生しました。"));
					break;
				}


				c4DVector state = neoque.front();
				neoque.pop_front();

				if(memo[(int)state.x][(int)state.y] == -2) continue;

				if(flag[(int)state.x][(int)state.y] == 0)
				{//未考証


					//考証
					group.push_back(state);
					flag[(int)state.x][(int)state.y] = 1;//考証済み

					cCoordinate coo;
					int aspect;
					int edge = 0;
			
					for(aspect=0;aspect<8;aspect+=2)
					{
						coo.SetAspect(aspect);
						c4DVector newstate;
						newstate.set( state.x + coo.x, state.y + coo.y ,state.z,0);
						grouproomindex = min((int)state.z,grouproomindex);
						
						if(newstate.x < 0 || newstate.x >= MAPWIDTH-minsizeofroom
							|| newstate.y < 0 || newstate.y >= MAPHEIGHT-minsizeofroom)
						{//端っこ
							continue;
						}

						

						if(memo[(int)newstate.x][(int)newstate.y] == -2)
						{//隣接が壁
							edge = 1;
						}
						else
						{
							que.push_back(newstate);
						}
					}

					if(edge)
					{//隣接が壁
						edgegroup.push_back(state);
					}
				}
			}

			for(k=0;k<group.size();k++)
			{
				group[k].z = grouproomindex;
			}
			for(k=0;k<edgegroup.size();k++)
			{
				edgegroup[k].z = grouproomindex;
				neoque.push_back(edgegroup[k]);
			}


		}

		que = neoque;


	}
	*/

	//つながり伝播
	for(;;)
	{
		if(que.empty()) break;
		if(que.size() > 400000)
		{//無限ループ防止
			OnAssert(_T(__FILE__),__LINE__,false,_T("地形整理関数内 つながり伝播で異常に大きなキューが発生しました。"));
			break;
		}

		c4DVector state = que.front();
		que.pop_front();

		if(memo[(int)state.x][(int)state.y] == -2)
		{//壁
			continue;
		}


		if(memo[(int)state.x][(int)state.y] != -1)
		{//オオキナ通路でない
			//部屋
			if(memo[(int)state.x][(int)state.y] != state.z)
			{//別の部屋
				//簡易部屋つなぎ判定
				if(memo[(int)state.x][(int)state.y] < state.z)
				{//部屋番号若い者優先
					continue;
				}

			}
			else
			{//同じ部屋
				continue;
			}
		}
	

		memo[(int)state.x][(int)state.y] = state.z;
		list.push_back(state);//部屋拡張チェックリスト

		cCoordinate coo;
		int aspect;
		
		/*
		for(x=1-minsizeofroom;x<minsizeofroom;x++)
		{
			for(y=1-minsizeofroom;y<minsizeofroom;y++)
			{
				coo.Set(x,y);
				c4DVector newstate;
				newstate.set( state.x + coo.x, state.y + coo.y ,state.z,0);
				
				if(newstate.x < 0 || newstate.x >= MAPWIDTH-minsizeofroom
					|| newstate.y < 0 || newstate.y >= MAPHEIGHT-minsizeofroom)
				{//端っこ
					continue;
				}

				que.push_back(newstate);
			}
		}
		*/
		
		for(x=1-2;x<2;x++)
		{
			for(y=1-2;y<2;y++)
			{
				coo.Set(x,y);
				c4DVector newstate;
				newstate.set( state.x + coo.x, state.y + coo.y ,state.z,0);
				
				if(newstate.x < 0 || newstate.x >= MAPWIDTH-minsizeofroom
					|| newstate.y < 0 || newstate.y >= MAPHEIGHT-minsizeofroom)
				{//端っこ
					continue;
				}

				que.push_back(newstate);
			}
		}
	}

	
	//部屋インデックス詰め
	set<int> temproomindexs;
	for(i=0;i<Map().RoomList.size();i++)
	{
		temproomindexs.insert(i);//これだけはあるはず
	}
	for(i=0;i<list.size();i++)
	{
		temproomindexs.insert(memo[(int)list[i].x][(int)list[i].y]);
	}
	int RoomNum = temproomindexs.size();
	if(RoomNum != Map().RoomList.size())
	{//新しい部屋あり
		map<int,int> roomindextable;
		set<int>::iterator itr = temproomindexs.begin();
		int i;
		for(i=0;itr!=temproomindexs.end();(i++,itr++))
		{
			roomindextable.insert(pair<int,int>(*itr,i));
		}
		for(i=0;i<list.size();i++)
		{
			if(memo[(int)list[i].x][(int)list[i].y] != -2)
			{
				memo[(int)list[i].x][(int)list[i].y] = roomindextable[memo[(int)list[i].x][(int)list[i].y]];
			}
		}


		for(i=Map().RoomList.size();i<RoomNum;i++)
		{//新部屋
			pcRoom room;
			room = pcRoom(new cRoom);
			room->Index = i;
			Map().RoomList.push_back(room);
		}

	}


	//地形整理

	for(i=0;i<list.size();i++)
	{
		for(x=0;x<minsizeofroom;x++)
		{
			for(y=0;y<minsizeofroom;y++)
			{
				pcLandform pland = Map().Land((int)list[i].x + x, (int)list[i].y + y);
				if(pland->RoomIndex == -1)
				{
					if(pland->getAttribute() == MAPKIND_FLOOR2)
					{
						pland->RoomIndex = memo[(int)list[i].x][(int)list[i].y];
						Map().RoomList[pland->RoomIndex]->LandformList.push_back(pland);

						landchange(pland, MAPKIND_FLOOR);
						

					}	
				}
				else
				{
					if(pland->RoomIndex != memo[(int)list[i].x][(int)list[i].y])
					{//部屋の統合
						int index = min(pland->RoomIndex,memo[(int)list[i].x][(int)list[i].y]);
						int maxindex = max(pland->RoomIndex,memo[(int)list[i].x][(int)list[i].y]);
						Map().RoomList[index]->compose(*(Map().RoomList[maxindex]));
						Map().RoomList[maxindex] = Map().RoomList[index];
					}
				}
			}
		}
	
	}


	//部屋リスト詰め


	//部屋インデックス詰め
	temproomindexs.clear();
	int size = Map().RoomList.size();
	for(i=0;i<size;i++)
	{
		temproomindexs.insert(Map().RoomList[i]->Index);
	}
	RoomNum = temproomindexs.size();
	if(RoomNum != Map().RoomList.size())
	{//部屋短縮あり
		map<int,int> roomindextable;
		set<int>::iterator itr = temproomindexs.begin();
		int i;
		for(i=0;itr!=temproomindexs.end();(i++,itr++))
		{
			roomindextable.insert(pair<int,int>(i,*itr));//逆順
		}

		vector<pcRoom> vproom;
		int size = roomindextable.size();
		for(i=0;i<size;i++)
		{
			vproom.push_back(Map().RoomList[roomindextable[i]]);
			Map().RoomList[roomindextable[i]]->ChangeIndex(i);
		}

		Map().RoomList = vproom;

		for(i=0;i<vproom.size();i++)
		{//地形インデックス再入力//安全だが冗長
			vector<pcLandform> vpland = vproom[i]->allLandformList();
			int k;
			for(k=0;k<vpland.size();k++)
			{
				vpland[k]->RoomIndex = i;
			}
		}
	}




	for(i=0;i<Map().RoomList.size();i++)
	{
		Map().RoomList[i]->resetNeighborList();
		Map().RoomList[i]->resetExitList();
	}

	tstring chipset = pFloor()->mapchipsetname();
	Map().ReDecoration(chipset,Map().changedLand);
	Map().changedLand.clear();

	return true;
}

bool cDungeonSystem::暗闇更新()
{
	set<pcLandform>::iterator itr = DarkLandList().begin();
	for(;itr != DarkLandList().end();itr++)
	{
		(*itr)->setDarkflag(false);
	}

	CutInM().CutInForAllChara(暗闇更新_タイミング);
	CutInM().CutInForAllDrop(暗闇更新_タイミング);
	return true;
}
bool cDungeonSystem::暗闇付与(pcLandform pland, int range)
{
	int x,y;
	for(x = pland->placeX - range; x<= pland->placeX + range;x++)
	{
		if(x < 0 || x >= MAPWIDTH) continue;
		for(y = pland->placeY - range; y<= pland->placeY + range;y++)
		{
			if(y < 0 || y >= MAPHEIGHT) continue;

			DarkLandList().insert(Map().Land(x,y));
			Map().Land(x,y)->setDarkflag(true);
		}
	}
	

	return true;
}