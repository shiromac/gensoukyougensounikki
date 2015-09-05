#include "cObjectChara.h"

#include "gameMainSystem/filemanage/cScriptRLayer.h"
#include "gameMainSystem/filemanage/cScriptReader.h"

#include "cDungeonSystem.h"
#include "EffectFunctions.h"

#include "Event1.h"

cObjectChara::cObjectChara(void)
{
	EXP = 0;
	LV = 1;
	MHP = 0;
	HP = 0;
	CLASS = 0;

	SetMHP();
}

cObjectChara::~cObjectChara(void)
{
}


void cObjectChara_ID_1::CutIn(タイミング timing, cValiableField& valiable)
{
	cObjectChara::CutIn(timing,valiable);

	if(timing == ターン終了_タイミング)
	{
		if(!Condition.みがわり状態())
		{
			sg_pDungeonSystem->即死要請(me(),0,1);
		}
	}
}

void cObjectChara_Damaku::CutIn(タイミング timing, cValiableField& valiable)
{
	cObjectChara::CutIn(timing,valiable);
	if(timing == 直接攻撃直前_タイミング)
	{
		valiable.doubles[変数_攻撃力] = me()->MHP;
		valiable.doubles.dim(変数_定数ダメージフラグ) = 1;
	}
	else if(timing == 攻撃直後時_タイミング)
	{
		sg_pDungeonSystem->強制退場要請(me());
	}
	else if(timing == ターン終了_タイミング)
	{
		if(me()->AImemory[3] > 2 && sg_pDungeonSystem->s_turn_speed() == GAME_TURN_SPEED_SINGLE)
		{
			sg_pDungeonSystem->即死要請(me(),false);
		}
	}
}
int cObjectChara_Damaku::通常攻撃_アニメ()
{
	EffectFunctions::弾幕体当たり(placeX, placeY, me(), aspect);
	return true;
}

namespace 弾幕{
	pcCharacter 弾幕召喚(pcLandform land, int HP, int Forse, int speed, 弾幕Type type, 弾幕色 color, int aspect, pcCharacter targetenemy)
	{
		if(HP <= 0) HP = 1;
		pcCharacter pchara;
		pchara = sg_pDungeonSystem->キャラクター生成(CCHARACTER_cObjectChara_BASE_ID_NUM+2 +(int)弾幕色_NUM*(int)type+color,
			0,Forse,land);

		if(pchara)
		{
			pchara->HP = 1;
			pchara->MHP = HP;
			pchara->aspect = aspect;
			if(targetenemy)
			{
				pcLandform enemyland = targetenemy->足元地形();
				cCoordinate coo(enemyland->placeX - land->placeX,
								enemyland->placeY - land->placeY);

				pchara->aspect = coo.GetAspect();


			}

			if(speed == -2)
			{
				sg_pDungeonSystem->速度減少要請(pchara,GAME_TURN_GAMEOVER,false);
				sg_pDungeonSystem->速度減少要請(pchara,GAME_TURN_GAMEOVER,false);	
			}
			else if(speed == -1)
			{
				sg_pDungeonSystem->速度減少要請(pchara,GAME_TURN_GAMEOVER,false);
			}
			else if(speed == 1)
			{
				sg_pDungeonSystem->速度増加要請(pchara,GAME_TURN_GAMEOVER,false);
			}
			else if(speed == 2)
			{
				sg_pDungeonSystem->速度増加要請(pchara,GAME_TURN_GAMEOVER,false);
				sg_pDungeonSystem->速度増加要請(pchara,GAME_TURN_GAMEOVER,false);
			}
			//sg_pDungeonSystem->強制ターンエンド要請(pchara);

			return pchara;
		}
		return NULLCHARA;
	}

	pcEvent レーザー召喚(
		pcLandform land,
		int aspect,
		int length,
		int power,
		int Forse,
		cColor color,
		int readyTurn,
		int attackTurn,
		bool isAttackFriend,
		bool isStopAtWall
		)
	{
		pcEvent pevent = cEvent_BossLazer::pNewEvent(cEvent::PRIORITY_HIGH, land,
			aspect,
			length,
			power,
			Forse,
			color,
			readyTurn,
			attackTurn,
			isAttackFriend,
			isStopAtWall);
	
		pevent->startEvent();
		return pevent;
	}
};
