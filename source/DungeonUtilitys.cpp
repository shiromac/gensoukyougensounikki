#include "stdafx.h"

#include "cDungeonSystem.h"
#include "cAnimation.h"
#include "cSaveStore.h"

#include "Language.h"
#include "cFloor.h"

#include "utility/debug.h"
double random()
{
	return sg_pDungeonSystem->pDungeon()->randomgen().random();
};
//[0,max)
int random_int(int max)
{
	return sg_pDungeonSystem->pDungeon()->randomgen().genrand_real2()*max;
};


//Range[min,max)
double random_range(double min,double max)
{
	return min + sg_pDungeonSystem->pDungeon()->randomgen().genrand_real2()*(max - min);
};

//全ての部屋の中からランダムで一つ地形を得る。
pcLandform cDungeonSystem::RandomPlace()
{

	cDiscreteProbability dp;

	int i;
	for(i=0;i<Map().RoomList.size();i++)
	{
		if(!(Map().RoomList[i]->allisolated))
		{//隔離されてない
			dp.set(Map().RoomList[i]->LandformList.size(),i);
		}
	}

	if(dp.empty())
	{//部屋なし
		vector<pcLandform> LandformList;

		int x,y;
		for(x=0;x<MAPWIDTH;x++)
		{
			for(y=0;y<MAPHEIGHT;y++)
			{
				if(Map().Land(x,y)->caninto(0,0,0))
				{
					LandformList.push_back(Map().Land(x,y));
				}
			}
		}
		if(LandformList.empty()) return NULLLAND;
		int li = random() * LandformList.size();

		return LandformList[li];
	}

	pcRoom room = Map().RoomList[dp.get(random())];

	int li = random() * room->LandformList.size();

	return room->LandformList[li];
	/*
	int ri = random() * Map().RoomList.size();
	
	pcRoom room = Map().RoomList[ri];

	int li = random() * room->LandformList.size();

	return room->LandformList[li];
	*/
}

int cDungeonSystem::メッセージ(const StyleString& Message)
{
	return AnimationManager().Anime_Message(Message);
}
int cDungeonSystem::メッセージ消去()
{
	return AnimationManager().Anime_MessageClear();
}
int cDungeonSystem::メッセージ決定待ち()
{
	return AnimationManager().Anime_MessageWait();
}
int cDungeonSystem::メッセージ何かボタン入力待ち()
{
	return AnimationManager().Anime_MessageWait(true);
}
void cDungeonSystem::メニューを開く()
{

	pcControlLayer pccl;
	pcSelectWindow pcsw;

	menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));

	pccl->Init(pDevice_D3D);
	pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

	pcsw->commandList.push_back(pcCommand(new cCommand_menuItem ));
	pcsw->commandList.push_back(pcCommand(new cCommand_menuFoot ));

	pcCommand charaSkillCommond = pPlayerChara()->CharaSkillCommond();
	if(charaSkillCommond)
	{
		pcsw->commandList.push_back(charaSkillCommond);
	}

	pcsw->commandList.push_back(pcCommand(new cCommand_menuStates ));
	pcsw->commandList.push_back(pcCommand(new cCommand_menuOther ));
	pcsw->Init(pDevice_D3D, 3, pcsw->commandList.size());
	pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(1));
	pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(1));


	pcsw->playsound_decide();

	//情報
	/*
	pcGameWindow pcgw;
	pccl->countStringWindow = pcgw = pcGameWindow(new cGameWindow);

	pcgw->Init(pDev,24,4);
	pcgw->CenterX = MessageWINDOW_CENTERX;
	pcgw->CenterY = MessageWINDOW_CENTERY;
	*/


	//マップ
	Map().MinimapViewOff();
	Resetminimap(pDevice_D3D);

	MessageW().textclear(pDevice_D3D);

}

void cDungeonSystem::メニューを一枚閉じる()
{
	if(primaryMenuControlLayerV().size())
	{
		primaryMenuControlLayerV().pop_back();
	}
	else
	{
		if(menuControlLayerV().size())
			menuControlLayerV().pop_back();
	}
}
void cDungeonSystem::メニューを閉じる()
{
	menuControlLayerV().AllRelease();
}
void cDungeonSystem::メニューリフレッシュ()
{//未実装（半実装）
	menuControlLayerV().AllRelease();
}
void cDungeonSystem::メニューリレンダ()
{
	int i,k;
	if(pDungeonInstance_ == NULL) return;
	for(i=0;i<menuControlLayerV().size();i++)
	{
		for(k=0;k<menuControlLayerV()[i]->WindowList.size();k++)
		{
			menuControlLayerV()[i]->WindowList[k]->ReRendarText();
		}
	}
	
}

bool cDungeonSystem::メニュー表示可能()
{
	return true;
}

int cDungeonSystem::キャラクター目視可(pcCharacter penemychara)
{//真下のやつにわたす
	//pPlayerChara()->allsight = 0;
	if(フロア明かり状態()) pPlayerChara()->allsight = 1;
	return キャラクター目視可(pPlayerChara(),penemychara);
}
int cDungeonSystem::キャラクター目視可(pcCharacter frompchara, pcCharacter topchara)
{

	int sightrange = frompchara->sightrange;
	if (frompchara->allsight)
	{
		sightrange = max(MAPHEIGHT,MAPWIDTH);
	}
	bool flag = キャラクター目視可(frompchara, topchara, sightrange);
	return  flag;
}
int cDungeonSystem::キャラクター目視可(pcCharacter frompchara, pcCharacter topchara, const int range)
{
	pcLandform here = Map().Land(frompchara->placeX,frompchara->placeY);
	pcLandform there = Map().Land(topchara->placeX,topchara->placeY);

	if(there->pOnChar != topchara && !topchara->死亡())
	{//アイテムの裏に隠れてるときなど
		return false;
	}
	
	bool flag = Map().isOnSight(here,there,range,frompchara->Condition.鳥目状態()) && !( topchara->Condition.透明状態である() && キャラクター敵対判定(frompchara, topchara) && !frompchara->Condition.目薬状態());
	return  flag;
}
	
int cDungeonSystem::落ち物目視可(pcDroping pdrop)
{//真下のやつにわたす
	return 落ち物目視可(pPlayerChara(),pdrop);
}
int cDungeonSystem::落ち物目視可(pcCharacter frompchara, pcDroping pdrop)
{
	pcLandform here = Map().Land(frompchara->placeX,frompchara->placeY);
	pcLandform there = pdrop->現在地形();

	int sightrange = frompchara->sightrange;

	return pdrop->isDiscover() && Map().isOnSight(here,there,sightrange,frompchara->Condition.鳥目状態());
}

int cDungeonSystem::地形目視可(pcCharacter frompchara, pcLandform toLand)
{
	pcLandform here = Map().Land(frompchara->placeX,frompchara->placeY);
	
	return Map().isOnSight(here,toLand);
}
int cDungeonSystem::地形目視可(pcLandform fromLand, pcLandform toLand)
{	
	return Map().isOnSight(fromLand,toLand);
}
bool cDungeonSystem::キャラが同室または出口または隣接か(pcCharacter me, pcCharacter enemy)
{	
	return Map().isOnSameRoomOrExit(me->足元地形(), enemy->足元地形());
}

pcLandform cDungeonSystem::配置安全地形(pcLandform target,pcCharacter pchara, int firstsetting)
{
	int i;
	pcLandform land;
	for(i=0;i<25;i++)
	{
		land = alternativeLand(target,i);
		if(キャラ配置安全(land) && (!firstsetting || i == 0 || land->RoomIndex != -1))
		{
			return land;
		}
	}

	return NULLLAND;
}
pcLandform cDungeonSystem::配置安全地形(pcLandform target,pcDroping pdrop, int firstsetting)
{
	int i;
	pcLandform land;
	for(i=0;i<25;i++)
	{
		land = alternativeLand(target,i);
		if(落ち物配置安全(land) && (!firstsetting || i == 0 || land->RoomIndex != -1))
		{
			if(firstsetting)
			{
				if(land->neighbor(0) == MAPKIND_FLOOR2
					|| land->neighbor(2) == MAPKIND_FLOOR2
					|| land->neighbor(4) == MAPKIND_FLOOR2
					|| land->neighbor(6) == MAPKIND_FLOOR2)
				{//通路隣接
					continue;
				}
			}
			return land;
		}
	}

	return NULLLAND;
}
bool cDungeonSystem::キャラ配置安全(pcLandform target)
{
	return ((target->pOnChar == NULL) && (target->through(0,0,0)));
}
bool cDungeonSystem::キャラ配置安全(pcLandform target,pcCharacter pchara)
{
	return (pchara && (target->pOnChar == NULL) && (target->through(pchara->水上歩行(),pchara->壁中歩行(),pchara->空中歩行())));
}
bool cDungeonSystem::落ち物配置安全(pcLandform target)
{
	return ((target->pOnDrop == NULL) && (target->through(0,0,0)));
}

pcLandform cDungeonSystem::存在安全地形(pcLandform target,pcCharacter pchara)
{
	if(pchara == NULL) return NULLLAND;

	int i;
	pcLandform land;
	if(target != NULLLAND)
	{
		for(i=0;i<25;i++)
		{
			land = alternativeLand(target,i);
			if((land->pOnChar == NULL || land->pOnChar == pchara) && (land->through(pchara->水上歩行(),pchara->壁中歩行(),pchara->空中歩行())))
			{
				return land;
			}
		}
	}

	vector<pcLandform> landlist;
	int x,y;
	for(x=0;x<MAPWIDTH;x++)
	{
		for(y=0;y<MAPHEIGHT;y++)
		{
			land = Map().Land(x,y);
			if((land->pOnChar == NULL || land->pOnChar == pchara) && (land->through(pchara->水上歩行(),pchara->壁中歩行(),pchara->空中歩行())))
			{
				landlist.push_back(land);
			}
		}
	}

	if(!landlist.empty())
	{
		int landindex = landlist.size()*random();
		return landlist[landindex];
	}

	return NULLLAND;
}
pcLandform cDungeonSystem::存在安全地形(pcLandform target,pcDroping pdrop)
{
	int i;
	pcLandform land;
	for(i=0;i<25;i++)
	{
		land = alternativeLand(target,i);
		if((land->pOnDrop == NULL) && (land->canputdrop()))
		{
			return land;
		}
	}

	return NULLLAND;
}

//3マス以上はなれたところに関して関与しない。(false)
bool cDungeonSystem::壁はさみ判定(pcLandform start, pcLandform end)
{
	cCoordinate startplace = start->place;
	cCoordinate endplace = end->place;

	cCoordinate vec = endplace - startplace;

	if(vec.dif() <= 1 || start->iswall()) return false;//スタートが壁なら壁はさみで無い

	if(vec.dif() == 2)
	{
		if(八方判定(startplace,endplace))
		{//八方
			cCoordinate midplace = (startplace + endplace)/2;
			//間が壁だったら壁はさみ
			return Map().Land(midplace)->iswall();
		}
		else
		{
			vec.x = ((vec.x<0)?-1:1);
			vec.y = ((vec.y<0)?-1:1);//斜め
			cCoordinate midplace = startplace + vec;

			if(! Map().Land(midplace)->iswall()) return false;
			
			midplace = endplace - vec;

			if(! Map().Land(midplace)->iswall()) return false;

			return true;
		}
	}

	return false;
	
}

pcLandform cDungeonSystem::キャラ足元地形(pcCharacter pchara)
{
	return Map().Land(pchara->placeX,pchara->placeY);
}

pcLandform cDungeonSystem::キャラ前地形(pcCharacter pchara,int distance)
{
	cCoordinate asp;
	asp.SetAspect(pchara->aspect);
	return Map().Land(pchara->placeX + asp.x*distance ,pchara->placeY + asp.y*distance);
}
pcLandform cDungeonSystem::キャラ視点方角地形(pcCharacter pchara, int plusaspect, int distance)
{
	cCoordinate asp;
	asp.SetAspect(pchara->aspect + plusaspect);
	return Map().Land(pchara->placeX + asp.x*distance ,pchara->placeY + asp.y*distance);
}

pcLandform cDungeonSystem::隣接地形(pcLandform outLand, int aspect, int distance)
{
	if(outLand == NULL) return NULLLAND;
	cCoordinate asp;
	asp.SetAspect(aspect);
	return Map().Land(outLand->placeX + asp.x*distance ,outLand->placeY + asp.y*distance);
}


bool cDungeonSystem::壁反射判定且検索(pcLandform &outLand, int &outaspect, pcLandform comeLand, int comeaspect)
{
	if(!(comeaspect%2)) return false;
	
	if(comeLand->placeX == -1 && comeLand->placeY == -1)
	{//エッジ
		return false;
	}
	if(!comeLand->iswall())
	{//壁で無い
		return false;
	}

	cCoordinate coo;

	coo.SetAspect(comeaspect-3);//左側
	pcLandform leftland = Map().Land(comeLand->placeX + coo.x,comeLand->placeY + coo.y);

	coo.SetAspect(comeaspect+3);//右側
	pcLandform rightland = Map().Land(comeLand->placeX + coo.x,comeLand->placeY + coo.y);

	if(!leftland->iswall())
	{//壁で無い
		
		outLand = rightland;
		outaspect = safeAspect(comeaspect-2);
		return true;
	}

	if(!rightland->iswall())
	{//壁で無い
		
		outLand = leftland;
		outaspect = safeAspect(comeaspect+2);
		return true;
	}


	return false;
}
bool cDungeonSystem::壁曲がり判定且検索(pcLandform &outLand, int &outaspect, pcLandform comeLand, int comeaspect)
{
	if((comeaspect%2)) return false;
	
	if(comeLand->placeX == -1 && comeLand->placeY == -1)
	{//エッジ
		return false;
	}
	if(!comeLand->iswall())
	{//壁で無い
		return false;
	}

	cCoordinate coo;

	coo.SetAspect(comeaspect-3);//左側
	pcLandform leftland = Map().Land(comeLand->placeX + coo.x,comeLand->placeY + coo.y);

	coo.SetAspect(comeaspect+3);//右側
	pcLandform rightland = Map().Land(comeLand->placeX + coo.x,comeLand->placeY + coo.y);
	
	if(!rightland->iswall())
	{//壁で無い
		coo.SetAspect(comeaspect-4);//後ろ
		outLand = Map().Land(comeLand->placeX + coo.x,comeLand->placeY + coo.y);
		outaspect = safeAspect(comeaspect+2);
		return true;
	}

	if(!leftland->iswall())
	{//壁で無い
		coo.SetAspect(comeaspect-4);//後ろ
		outLand = Map().Land(comeLand->placeX + coo.x,comeLand->placeY + coo.y);
		outaspect = safeAspect(comeaspect-2);
		return true;
	}



	return false;
}

pcLandform cDungeonSystem::射程内障害物検索(pcLandform start, int aspect, int range, bool stopWall, bool stopChara, bool stopDrop, bool stopNonthrough)
{
	vector<pcLandform> landpass;
	return 射程内障害物検索(landpass, start, aspect, range, stopWall, stopChara, stopDrop, stopNonthrough);
}
pcLandform cDungeonSystem::射程内障害物検索(vector<pcLandform>& landpass, pcLandform start, int aspect, int range, bool stopWall, bool stopChara, bool stopDrop, bool stopNonthrough)
{
	pcLandform land = start;

	if(range < 0) range = max(MAPWIDTH,MAPHEIGHT);

	cCoordinate now = start->place,vec;
	vec.SetAspect(aspect);

	int i;
	for(i=0;i<range;i++)
	{
		now += vec;
		land = Map().Land(now);
		
		landpass.push_back(land);

		if(!(land->place == now))
		{//端っこ
			return Map().Land(now-vec);
		}

		if(stopWall && land->iswall())
		{
			return land;
		}
	
		if(stopChara && land->pOnChar != NULL)
		{
			return land;
		}

		if(stopDrop && land->pOnDrop != NULL)
		{
			return land;
		}
		
		if(stopNonthrough && !(land->through(0,0,0)))
		{
			return land;
		}
	}

	return land;
}

pcLandform cDungeonSystem::障害物検索(pcLandform start, int aspect, bool stopWall, bool stopChara, bool stopDrop)
{
	return 射程内障害物検索(start, aspect, max(MAPWIDTH,MAPHEIGHT), stopWall, stopChara, stopDrop);
}
pcLandform cDungeonSystem::落下可能地点検索(pcLandform start, pcDroping pdrop)
{
	pcLandform pland;
	int i;
	for(i=0;i<25;i++)
	{
		pland = alternativeLand(start,i);
		if(!pland->iswall() && pland->pOnDrop == NULL && pland->設置可())
		{
			//壁はさみ判定
			if(!壁はさみ判定(start,pland))
			{
				return pland;
			}
		}
	}
	
	return NULLLAND;
}
pcLandform cDungeonSystem::落下可能地点検索(pcLandform start, pcCharacter pchara)
{
	pcLandform pland;
	int i;
	for(i=0;i<25;i++)
	{
		pland = alternativeLand(start,i);
		if(!pland->iswall() && (pland->pOnChar == NULL || pland->pOnChar == pchara) && pland->through(pchara->水上歩行(),pchara->壁中歩行(),pchara->空中歩行()))
		{
			//壁はさみ判定
			if(!壁はさみ判定(start,pland))
			{
				return pland;
			}
		}
	}
	
	return NULLLAND;
}


bool cDungeonSystem::キャラクター敵対判定(pcCharacter pcharaA,pcCharacter pcharaB)
{
	if(pcharaA == NULL || pcharaB == NULL) return false;
	if(pcharaA == pcharaB) return false;

	if(pcharaB->Condition.やりすごし状態()) return false;

	if(
			(
				(pcharaA->Forse != pcharaB->Forse) &&
				(pcharaA->Forse != CHARACTER_FORSE_OTHER) &&
				(pcharaB->Forse != CHARACTER_FORSE_OTHER)
			)
			||
				(pcharaB->Condition.みがわり状態() && pcharaA->Forse == CHARACTER_FORSE_ENEMY)
			|| pcharaA->pAI->isOppose(pcharaB)
			|| (pcharaA->Forse != CHARACTER_FORSE_FRIEND && pcharaA->Condition.狂乱状態() 
				&& pcharaA != pPlayerChara()
				)
		)
	{
		return true;
	}
	return false;
}

bool cDungeonSystem::ボス戦中判定()
{
	vector<pcCharacter>& vpChara = CharaList();
	int i, size = vpChara.size();
	for(i=0;i<size;i++)
	{
		pcCharacter& pChara = vpChara[i];
		if(pChara->CharaAttribute().count(キャラ属性::ボス))
		{
			//壁はさみ判定
			if(キャラクター敵対判定(pPlayerChara(),pChara))
			{
				return true;
			}
		}
	}
	return false;
}

//代理地形
pcLandform cDungeonSystem::alternativeLand(pcLandform main, int orderNum)
{
	cCoordinate coo,Mplace;
	Mplace.x = main->placeX;
	Mplace.y = main->placeY;
	switch(orderNum)
	{
	case 1:
		coo.Set(1,0);break;
	case 2:
		coo.Set(-1,0);break;
	case 3:
		coo.Set(0,-1);break;
	case 4:
		coo.Set(0,1);break;
	case 5:
		coo.Set(1,-1);break;
	case 6:
		coo.Set(-1,-1);break;
	case 7:
		coo.Set(1,1);break;
	case 8:
		coo.Set(-1,1);break;
	case 9:
		coo.Set(2,0);break;
	case 10:
		coo.Set(-2,0);break;
	case 11:
		coo.Set(0,-2);break;
	case 12:
		coo.Set(0,2);break;
	case 13:
		coo.Set(2,-1);break;
	case 14:
		coo.Set(-2,-1);break;
	case 15:
		coo.Set(2,1);break;
	case 16:
		coo.Set(-2,1);break;
	case 17:
		coo.Set(1,-2);break;
	case 18:
		coo.Set(-1,-2);break;
	case 19:
		coo.Set(1,2);break;
	case 20:
		coo.Set(-1,2);break;
	case 21:
		coo.Set(2,-2);break;
	case 22:
		coo.Set(-2,-2);break;
	case 23:
		coo.Set(2,2);break;
	case 24:
		coo.Set(-2,2);break;
	default:
		coo.Set(0,0);break;
	}
	return Map().Land(Mplace+coo);
}

int cDungeonSystem::キャラクター目視可範囲キャラリスト(pcCharacter frompchara, vector<pcCharacter>& outputlist)
{
	outputlist.clear();
	int i;
	for(i=0;i<CharaList().size();i++)
	{
		if(キャラクター目視可(frompchara,CharaList()[i]))
		{
			outputlist.push_back(CharaList()[i]);
		}
	}
	return true;
}
int cDungeonSystem::キャラクター目視可範囲落ち物リスト(pcCharacter frompchara, vector<pcDroping>& outputlist)
{
	outputlist.clear();
	int i;
	for(i=0;i<DropList().size();i++)
	{
		if(落ち物目視可(frompchara,DropList()[i]))
		{
			outputlist.push_back(DropList()[i]);
		}
	}
	return true;
}
int cDungeonSystem::キャラクター部屋キャラリスト(pcCharacter frompchara, vector<pcCharacter>& outputlist)
{
	outputlist.clear();
	int i;
	
	for(i=0;i<CharaList().size();i++)
	{
		if(Map().isOnSameRoomOrExit(frompchara->足元地形(),CharaList()[i]->足元地形()))
		{
			outputlist.push_back(CharaList()[i]);
		}
	}

	return true;
}
int cDungeonSystem::地形目視可範囲出口リスト(pcCharacter frompchara, vector<pcLandform>& outputlist)
{
	outputlist.clear();
	int i;
	int roomindex = Map().Land(frompchara->placeX,frompchara->placeY)->RoomIndex;
	
	if(roomindex == -1)
	{//通路
		for(i=0;i<8;i++)
		{//八方
			if(sg_pDungeonSystem->隣接地形移動可(frompchara,i))
			{
				cCoordinate coo;
				coo.SetAspect(i);
				coo.x += frompchara->placeX;
				coo.y += frompchara->placeY;
				outputlist.push_back(Map().Land(coo));
			}
		}
	}
	else
	{
		if(Map().RoomList.size() <= roomindex)
		{
			OnAssert(_T(__FILE__),__LINE__,false,_T("地形目視可範囲出口リスト　で無効なルームインデックスが参照されました。"));
			return true;
		}
		for(i=0;i<Map().RoomList[roomindex]->LandformExitList.size();i++)
		{
			if(地形目視可(frompchara,Map().RoomList[roomindex]->LandformExitList[i]))
			{//出口リスト
				outputlist.push_back(Map().RoomList[roomindex]->LandformExitList[i]);
			}
		}
	}
	return true;
}

int cDungeonSystem::キャラクター隣接地形リスト(pcCharacter frompchara, vector<pcLandform>& outputlist)
{
	cCoordinate place,ascoo;
	int i;

	place.x = frompchara->placeX;
	place.y = frompchara->placeY;

	for(i=0;i<8;i++)
	{
		ascoo.SetAspect(i);
		outputlist.push_back(Map().Land(place+ascoo));
	}

	return true;
}






//キャラのポインタからラッピングずみを検索
pcCharacter cDungeonSystem::システム用キャラ検索(pcCharacter pchara)
{
	int i;
	for(i=0;i<CharaList().size();i++)
	{
		if(CharaList()[i] == pchara) return CharaList()[i];
	}
	return NULLCHARA;
}

int cDungeonSystem::敵の数()
{
	int count = 0;
	int i,size = sg_pDungeonSystem->CharaList().size();
	pcCharacter pchara;
	for(i=0;i<size;i++)
	{
		pchara = sg_pDungeonSystem->CharaList()[i];
		if(pchara->Forse == CHARACTER_FORSE_ENEMY && !pchara->CharaAttribute().count(キャラ属性::弾幕))
		{
			
			++count;
		}
	}
	return count;
}

tstring cDungeonSystem::漢数字(int digit, int flag)
{

	switch(digit)
	{
	case 0:
		return g_Lang(_T("零")).conclete_tstr();
	case 1:
		return g_Lang(_T("壱")).conclete_tstr();
	case 2:
		return g_Lang(_T("弐")).conclete_tstr();
	case 3:
		return g_Lang(_T("参")).conclete_tstr();
	case 4:
		return g_Lang(_T("肆")).conclete_tstr();
	case 5:
		return g_Lang(_T("伍")).conclete_tstr();
	case 6:
		return g_Lang(_T("陸")).conclete_tstr();
	case 7:
		return g_Lang(_T("漆")).conclete_tstr();
	case 8:
		return g_Lang(_T("捌")).conclete_tstr();
	case 9:
		return g_Lang(_T("玖")).conclete_tstr();
	default:
		return g_Lang(_T("多")).conclete_tstr();
	}
}

bool cDungeonSystem::isNotGoodTerminateFlagInBase()
{
	return !pSaveData->DidGoodEndFlagInBase;
}
