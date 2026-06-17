#include "stdafx.h"

#include "FindUtility.h"
#include "cDungeonSystem.h"

#include "cCharacter.h"
#include "cDroping.h"
#include "cLandform.h"

using namespace FindUtility;

pcLandform FindUtility::各部屋等確率_ランダム地形検索_配置安全()
{
	vector<pcRoom> vproom;

	int i;
	int size = sg_pDungeonSystem->Map().RoomList.size();
	for(i=0;i<size;i++)
	{
		//if(!(Map().RoomList[i]->allisolated))
		//{//隔離されてない
			vproom.push_back(sg_pDungeonSystem->Map().RoomList[i]);
		//}
	}

	if(vproom.empty())
	{//部屋なし
		vector<pcLandform> LandformList;

		int x,y;
		for(x=0;x<MAPWIDTH;x++)
		{
			for(y=0;y<MAPHEIGHT;y++)
			{
				if(sg_pDungeonSystem->Map().Land(x,y)->caninto(0,0,0))
				{
					LandformList.push_back(sg_pDungeonSystem->Map().Land(x,y));
				}
			}
		}
		if(LandformList.empty()) return NULLLAND;
		int li = random() * LandformList.size();

		return LandformList[li];
	}
	int ri = random() * vproom.size();
	
	pcRoom room =  vproom[ri];

	int li = random() * room->LandformList.size();

	return room->LandformList[li];
}

pcLandform FindUtility::各部屋等確率_ランダム地形検索_店回避_配置安全()
{
	vector<pcRoom> vproom;

	int i;
	int size = sg_pDungeonSystem->Map().RoomList.size();
	for(i=0;i<size;i++)
	{
		if(!(sg_pDungeonSystem->Map().RoomList[i]->ShopFlag))
		{//店でない
			vproom.push_back(sg_pDungeonSystem->Map().RoomList[i]);
		}
	}

	if(vproom.empty())
	{//部屋なし
		size = sg_pDungeonSystem->Map().RoomList.size();
		for(i=0;i<size;i++)
		{
			vproom.push_back(sg_pDungeonSystem->Map().RoomList[i]);
		}
	}

	if(vproom.empty())
	{//部屋なし
		vector<pcLandform> LandformList;

		int x,y;
		for(x=0;x<MAPWIDTH;x++)
		{
			for(y=0;y<MAPHEIGHT;y++)
			{
				if(sg_pDungeonSystem->Map().Land(x,y)->caninto(0,0,0))
				{
					LandformList.push_back(sg_pDungeonSystem->Map().Land(x,y));
				}
			}
		}
		if(LandformList.empty()) return NULLLAND;
		int li = random() * LandformList.size();

		return LandformList[li];
	}
	int ri = random() * vproom.size();
	
	pcRoom room =  vproom[ri];

	int li = random() * room->LandformList.size();

	return room->LandformList[li];
}
pcLandform FindUtility::部屋外優先_各部屋等確率_ランダム地形検索_配置安全(pcLandform pland)
{
	vector<pcRoom> vproom;

	int i;
	int size = sg_pDungeonSystem->Map().RoomList.size();
	for(i=0;i<size;i++)
	{
		vproom.push_back(sg_pDungeonSystem->Map().RoomList[i]);
	}
	if(vproom.empty())
	{//部屋なし
		vector<pcLandform> LandformList;

		int x,y;
		for(x=0;x<MAPWIDTH;x++)
		{
			for(y=0;y<MAPHEIGHT;y++)
			{
				if(sg_pDungeonSystem->Map().Land(x,y)->caninto(0,0,0))
				{
					LandformList.push_back(sg_pDungeonSystem->Map().Land(x,y));
				}
			}
		}
		if(LandformList.empty()) return NULLLAND;
		int li = random() * LandformList.size();

		return LandformList[li];
	}

	int roomnum = vproom.size();
	if(roomnum <= 1)
	{//部屋数が１
		int landindex = vproom[0]->LandformList.size()*random();
		return vproom[0]->LandformList[landindex];
	}
	else
	{//部屋数が２以上
		int roomindex = (roomnum -1)*random();
		if(roomindex >= pland->RoomIndex) roomindex++;
		int landindex = vproom[roomindex]->LandformList.size()*random();
		return vproom[roomindex]->LandformList[landindex];
	}
}
pcLandform FindUtility::部屋外優先_各部屋等確率_ランダム地形検索_非配置安全(pcLandform pland)
{
	vector<pcRoom> vproom;

	int i;
	int size = sg_pDungeonSystem->Map().RoomList.size();
	for(i=0;i<size;i++)
	{
		vproom.push_back(sg_pDungeonSystem->Map().RoomList[i]);
	}
	if(vproom.empty())
	{//部屋なし
		vector<pcLandform> LandformList;

		int x,y;
		for(x=0;x<MAPWIDTH;x++)
		{
			for(y=0;y<MAPHEIGHT;y++)
			{
				if(sg_pDungeonSystem->Map().Land(x,y)->caninto(0,0,0))
				{
					LandformList.push_back(sg_pDungeonSystem->Map().Land(x,y));
				}
			}
		}
		if(LandformList.empty()) return NULLLAND;
		int li = random() * LandformList.size();

		return LandformList[li];
	}
	int roomnum = vproom.size();
	if(roomnum <= 1)
	{//部屋数が１
		vector<pcLandform> landlist = vproom[0]->allLandformList();
		vector<pcLandform> safelandlist;
		int i;
		for(i=0;i<landlist.size();i++)
		{
			if(landlist[i]->caninto(0,0,0))
			{
				safelandlist.push_back(landlist[i]);
			}
		}
		if(safelandlist.empty()) return NULLLAND;
		int landindex = safelandlist.size()*random();
		return safelandlist[landindex];
	}
	else
	{//部屋数が２以上

		int roomindex = (roomnum -1)*random();
		if(roomindex >= pland->RoomIndex) roomindex++;
		vector<pcLandform> landlist = vproom[roomindex]->allLandformList();
		
		vector<pcLandform> safelandlist;
		int i;
		for(i=0;i<landlist.size();i++)
		{
			if(landlist[i]->caninto(0,0,0))
			{
				safelandlist.push_back(landlist[i]);
			}
		}
		if(safelandlist.empty()) return NULLLAND;
		int landindex = safelandlist.size()*random();
		return safelandlist[landindex];
	}
}

pcLandform FindUtility::視界外優先_各部屋等確率_ランダム地形検索_非配置安全(pcLandform pland)
{
	vector<pcRoom> vproom;

	int i;
	int size = sg_pDungeonSystem->Map().RoomList.size();
	for(i=0;i<size;i++)
	{
		vproom.push_back(sg_pDungeonSystem->Map().RoomList[i]);
	}
	if(vproom.empty())
	{//部屋なし
		vector<pcLandform> LandformList;

		int x,y;
		for(x=0;x<MAPWIDTH;x++)
		{
			for(y=0;y<MAPHEIGHT;y++)
			{
				if(sg_pDungeonSystem->Map().Land(x,y)->caninto(0,0,0))
				{
					LandformList.push_back(sg_pDungeonSystem->Map().Land(x,y));
				}
			}
		}
		if(LandformList.empty()) return NULLLAND;
		int li = random() * LandformList.size();

		return LandformList[li];
	}

	int roomnum = vproom.size();
	if(roomnum <= 1)
	{//部屋数が１
		vector<pcLandform> prelandlist = vproom[0]->allLandformList();
		vector<pcLandform> landlist;
		int i;
		int size = prelandlist.size();
		for(i=0;i<size;i++)
		{
			if((pland->place - prelandlist[i]->place).dif() > 4)
			{
				landlist.push_back(prelandlist[i]);
			}
		}
		if(landlist.empty())
		{
			int landindex = prelandlist.size()*random();
			return prelandlist[landindex];
		}


		return landlist[landlist.size()*random()];
	}
	else
	{//部屋数が２以上
		int roomindex = (roomnum -1)*random();
		if(roomindex >= pland->RoomIndex) roomindex++;
		vector<pcLandform> landlist = vproom[roomindex]->allLandformList();
		int landindex = landlist.size()*random();
		return landlist[landindex];
	}
}

pcLandform FindUtility::視界外優先_各部屋等確率_ランダム地形検索_配置安全(pcLandform pland)
{
	vector<pcRoom> vproom;

	int i;
	int size = sg_pDungeonSystem->Map().RoomList.size();
	for(i=0;i<size;i++)
	{
		vproom.push_back(sg_pDungeonSystem->Map().RoomList[i]);
	}
	if(vproom.empty())
	{//部屋なし
		vector<pcLandform> LandformList;

		int x,y;
		for(x=0;x<MAPWIDTH;x++)
		{
			for(y=0;y<MAPHEIGHT;y++)
			{
				if(sg_pDungeonSystem->Map().Land(x,y)->caninto(0,0,0))
				{
					LandformList.push_back(sg_pDungeonSystem->Map().Land(x,y));
				}
			}
		}
		if(LandformList.empty()) return NULLLAND;
		int li = random() * LandformList.size();

		return LandformList[li];
	}

	int roomnum = vproom.size();
	if(roomnum <= 1)
	{//部屋数が１
		vector<pcLandform> landlist;
		int i;
		int size =vproom[0]->LandformList.size();
		for(i=0;i<size;i++)
		{
			if((pland->place - vproom[0]->LandformList[i]->place).dif() > 4)
			{
				landlist.push_back(vproom[0]->LandformList[i]);
			}
		}
		if(landlist.empty())
		{
			int landindex = vproom[0]->LandformList.size()*random();
			return vproom[0]->LandformList[landindex];
		}


		return landlist[landlist.size()*random()];
	}
	else
	{//部屋数が２以上
		int roomindex = (roomnum -1)*random();
		if(roomindex >= pland->RoomIndex) roomindex++;
		int landindex = vproom[roomindex]->LandformList.size()*random();
		return vproom[roomindex]->LandformList[landindex];
	}
}

vector<pcCharacter> FindUtility::フロア敵索敵_敵リスト(pcCharacter pchara, const int range)
{
	vector<pcCharacter> pcharalist;
	const vector<pcCharacter>& vpchara = sg_pDungeonSystem->CharaList();
	int i, size = vpchara.size();
	for(i=0;i<size;i++)
	{
		if(sg_pDungeonSystem->キャラクター目視可(pchara, vpchara[i], range)
			|| sg_pDungeonSystem->キャラクター敵対判定(pchara, vpchara[i]))
		{
			pcharalist.push_back(vpchara[i]);
		}
	}

	return pcharalist;
}

vector<pcCharacter> FindUtility::部屋内と隣接敵リスト(pcCharacter pchara)
{
	vector<pcCharacter> pcharalist;
	const vector<pcCharacter>& vpchara = sg_pDungeonSystem->CharaList();
	int i, size = vpchara.size();
	for(i=0;i<size;i++)
	{

		if(sg_pDungeonSystem->キャラクター敵対判定(pchara, vpchara[i]))
		{
			if(sg_pDungeonSystem->キャラが同室または出口または隣接か(pchara, vpchara[i]))
			{
				pcharalist.push_back(vpchara[i]);
			}
		}
	}

	return pcharalist;
}

vector<pcCharacter> FindUtility::透明でない部屋内と隣接味方リスト(pcCharacter pchara)
{
	vector<pcCharacter> pcharalist;
	const vector<pcCharacter>& vpchara = sg_pDungeonSystem->CharaList();
	int i, size = vpchara.size();
	for(i=0;i<size;i++)
	{

		if(!sg_pDungeonSystem->キャラクター敵対判定(pchara, vpchara[i]))
		{
			if(sg_pDungeonSystem->キャラが同室または出口または隣接か(pchara, vpchara[i]))
			{
				if(!vpchara[i]->Condition.透明状態である())
				{
					pcharalist.push_back(vpchara[i]);
				}
			}
		}
	}

	return pcharalist;
}

pcCharacter FindUtility::敵リストから優先敵索敵(const vector<pcCharacter>& vpchara, pcCharacter fromchara)
{
	int size = vpchara.size();
	multimap<int,pcCharacter> sortMap;
	int i;
	int maxForcevalue = 0;
	pcCharacter pchara;
	cCoordinate coo;

	for(i=0;i<size;i++)
	{//敵キャラリスト作成
		pchara = vpchara[i];

		//coo.x = pchara->placeX - fromchara->placeX;
		//coo.y = pchara->placeY - fromchara->placeY;
		if(sg_pDungeonSystem->キャラクター敵対判定(fromchara,pchara))
		{
			if(maxForcevalue < pchara->被攻撃優先度())
			{//優先度高い
				maxForcevalue = pchara->被攻撃優先度();
				sortMap.clear();
			}
			else if(maxForcevalue > pchara->被攻撃優先度())
			{//優先度低い
				continue;
			}
			sortMap.insert(pair<int,pcCharacter>(random(),pchara));
		}
	}

	multimap<int,pcCharacter>::iterator itr = sortMap.begin();
	if(sortMap.size())
	{//最も近い敵をサーチする。->ランダムに変更
		if(sortMap.end() != itr)
		{

			return itr->second;
		}
	}

	return NULLCHARA;
}


bool FindUtility::キャラが敵で直接攻撃可能か(pcCharacter me, pcCharacter enemy)
{

	if(enemy != NULL && sg_pDungeonSystem->キャラクター敵対判定(me,enemy))
	{
		cCoordinate nowco(me->placeX,me->placeY);
		cCoordinate youco(enemy->placeX,enemy->placeY);
		if((nowco - youco).dif() == 1 && me->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
		{
			return true;
		}
	}
	return false;
}
bool FindUtility::キャラが味方で直接攻撃可能か(pcCharacter me, pcCharacter chara)
{

	if(chara != NULL && !sg_pDungeonSystem->キャラクター敵対判定(me,chara))
	{
		cCoordinate nowco(me->placeX,me->placeY);
		cCoordinate youco(chara->placeX,chara->placeY);
		if((nowco - youco).dif() == 1 && me->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
		{
			return true;
		}
	}
	return false;
}


bool FindUtility::キャラ８方向射線で範囲以内(pcCharacter me, pcCharacter enemy, const int range)
{
	
	if(enemy != NULL)
	{
		cCoordinate nowco(me->placeX,me->placeY);
		cCoordinate youco(enemy->placeX,enemy->placeY);

		if((nowco - youco).dif() <= range && sg_pDungeonSystem->八方判定(nowco,youco))
		{
			return true;
		}
	}
	return false;
}
int FindUtility::キャラの距離(pcCharacter me, pcCharacter you)
{
	
	if(you != NULL && me != NULL)
	{
		cCoordinate nowco(me->placeX,me->placeY);
		cCoordinate youco(you->placeX,you->placeY);

		return (nowco - youco).dif();
	}
	return -1;
}
vector<pcDroping> FindUtility::装備品以外の表面手持ちアイテム店売り除く(pcCharacter pchara)
{
	vector<pcDroping> vpdrop;
	int i, size = pchara->holdItem.size();
	for(i=0;i<size;i++)
	{
		if(pchara->holdItem[i] == pchara->attackequipment) continue;
		if(pchara->holdItem[i] == pchara->defenseequipment) continue;
		//if(pchara->holdItem[i] == pchara->bulletequipment) continue;
		int k;
		for(k=0;k<pchara->最大装備スペル数();k++)
		{
			if(pchara->holdItem[i] == pchara->spellequipment[k]) break;
		}
		if(k != pchara->最大装備スペル数()) continue;//最後までいなかった

		if(pchara->holdItem[i]->shopitemflag() != cDroping::SHOPFLAG_NONE) continue;

		vpdrop.push_back(pchara->holdItem[i]);
	}
	return vpdrop;
}

int FindUtility::キャラの方向(pcCharacter me, pcCharacter you)
{
	if(you != NULL && me != NULL)
	{
		cCoordinate nowco(me->placeX,me->placeY);
		cCoordinate youco(you->placeX,you->placeY);

		return (youco - nowco).GetAspect();
	}
	return -1;
}
int FindUtility::キャラ八方ズレ数(pcCharacter me, pcCharacter you)
{
	
	if(you != NULL && me != NULL)
	{
		cCoordinate nowco(me->placeX,me->placeY);
		cCoordinate youco(you->placeX,you->placeY);

		cCoordinate diffarence = youco - nowco;

		int distance = abs(diffarence.x);
		distance = min(distance, abs(diffarence.y));
		distance = min(distance, abs(diffarence.x - diffarence.y));
		distance = min(distance, abs(diffarence.y + diffarence.x));
		return distance;
	}
	return -1;
}
bool FindUtility::キャラ中距離攻撃地形射線通り(pcCharacter me, pcCharacter you, int distance)
{
	
	if(!キャラ８方向射線で範囲以内(me, you, distance))
	{
		return false;
	}
	if(you != NULL && me != NULL)
	{
		int aspect = キャラの方向(me, you);

		return sg_pDungeonSystem->isCanNeighberAttack(me, aspect, distance, 0, 0);
	}
	return false;
}
	//敵味方区別なし、地形のみ見る、壁斜め通らない
int FindUtility::キャラ中距離攻撃地形攻撃距離(pcCharacter me, int aspect, int distance)
{
	int i;
	for(i=1;i<=distance;i++)
	{
		if(!sg_pDungeonSystem->isCanNeighberAttack(me, aspect, i, 0, 0))
		{
			return i-1;
		}
		if(sg_pDungeonSystem->隣接地形(me->足元地形(), aspect, i)->pOnChar)
		{
			return i;
		}
	}
	return distance;
}

//double priorityFunction(mechara,objectchara)
//0未満だった場合はNULLCHARAを返す
pcCharacter FindUtility::ターゲットキャラクター検索(const vector<pcCharacter>& charalist, pcCharacter me, double(*priorityFunction)(pcCharacter, pcCharacter))
{
	if(charalist.empty()) return NULLCHARA;

	pcCharacter maxchara = charalist[0];
	double max_priority = priorityFunction(me, charalist[0]);
	double temp_priority;
	int i, size = charalist.size();
	for(i=1;i<size;i++)
	{
		temp_priority = priorityFunction(me, charalist[i]);
		if(max_priority < temp_priority)
		{
			max_priority = temp_priority;
			maxchara = charalist[i];
		}
	}

	if(max_priority <= 0) return NULLCHARA;
	return maxchara;
}

pcDroping FindUtility::ターゲット落ち物検索(const vector<pcDroping>& droplist, pcCharacter me, double(*priorityFunction)(pcCharacter, pcDroping))
{
	if(droplist.empty()) return NULLDROP;

	pcDroping maxdrop = droplist[0];
	double max_priority = priorityFunction(me, droplist[0]);
	double temp_priority;
	int i, size = droplist.size();
	for(i=1;i<size;i++)
	{
		temp_priority = priorityFunction(me, droplist[i]);
		if(max_priority < temp_priority)
		{
			max_priority = temp_priority;
			maxdrop = droplist[i];
		}
	}

	if(max_priority <= 0) return NULLDROP;
	return maxdrop;
}

pcLandform FindUtility::ターゲット地形検索(const vector<pcLandform>& landlist, pcCharacter me, double(*priorityFunction)(pcCharacter, pcLandform))
{
	if(landlist.empty()) return NULLLAND;

	pcLandform maxland = landlist[0];
	double max_priority = priorityFunction(me, landlist[0]);
	double temp_priority;
	int i, size = landlist.size();
	for(i=1;i<size;i++)
	{
		temp_priority = priorityFunction(me, landlist[i]);
		if(max_priority < temp_priority)
		{
			max_priority = temp_priority;
			maxland = landlist[i];
		}
	}

	if(max_priority <= 0) return NULLLAND;
	return maxland;
}
