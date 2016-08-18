#include "stdafx.h"
#include "Trap1.h"

#include "cDungeonSystem.h"
#include "cAttackinformation.h"
#include "FindUtility.h"
#include "GameIdiom.h"
#include "EffectFunctions.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//天狗風のワナ
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cTrap_ID_0::効果()
{

	EffectFunctions::上吹き飛ばし風エフェクト(me()->placeX,me()->placeY);
	return true;
}
int cTrap_ID_0::効果(pcCharacter pchara)
{

	map<tstring, StyleString> valiable;
	valiable[_T("Chara")] = pchara->ShortName();
	g_Langメッセージ(_T("cTrap_ID_0効果メッセージ"),valiable);

	EffectFunctions::上吹き飛ばし風エフェクト(me()->placeX,me()->placeY);

	GameIdiom::高飛び(pchara,me()->現在地形());
	return true;
}
int cTrap_ID_0::効果(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	map<tstring, StyleString> valiable;
	valiable[_T("Chara")] = pdrop->FullName();
	g_Langメッセージ(_T("cTrap_ID_0効果メッセージ"),valiable);

	EffectFunctions::上吹き飛ばし風エフェクト(me()->placeX,me()->placeY);
	
	pcCharacter pchara = pdrop->乗りキャラ();
	if(pchara != NULLCHARA) GameIdiom::高飛び(pchara,me()->現在地形());
	GameIdiom::高飛び(pdrop,me()->現在地形());
	return true;//pdropを破壊したかったらtrue;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//天狗風のワナ[丈夫]
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void cTrap_ID_1::DataBeginOptimize(int difficulty)
{
	cTrap::DataBeginOptimize(difficulty);

	discover();//常に見えてる
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//プチフレアの罠
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cTrap_ID_2::効果()
{
	EffectFunctions::爆発エフェクト_中(me()->placeX,me()->placeY,20);

	GameIdiom::爆発(me()->現在地形(), NULLCHARA, 効果量(0), 割合ダメージフラグ, TRUE, TRUE, TRUE, TRUE);
	//sg_pDungeonSystem->攻撃接近();

	return true;
}
int cTrap_ID_2::効果(pcCharacter pchara)
{
	return 効果();
}
int cTrap_ID_2::効果(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	効果();
	sg_pDungeonSystem->落ち物破壊要請(pdrop);
	return true;
}
void cTrap_ID_2::CutIn(タイミング timing, cValiableField& valiable)
{
	cTrap::CutIn(timing,valiable);

	if(timing == 被攻撃接近直後_タイミング)
	{
		if(valiable.intsets[変数_属性].count(攻撃属性::爆発) && Holder() == NULL)
		{
			if(発動要請())
			{
				map<tstring, StyleString> valiable;
				valiable[_T("Trap")] = FullName();
				g_Langメッセージ(_T("cTrap_ID_2効果メッセージ"),valiable);

			}

		}
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//メガフレアの罠
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//プチフレアの罠と同じ

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//春眠の罠
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cTrap_ID_4::効果()
{
	//エフェクト未実装

	return true;
}
int cTrap_ID_4::効果(pcCharacter pchara)
{
	return sg_pDungeonSystem->眠り要請(pchara, 効果量(0));
}
int cTrap_ID_4::効果(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	効果();
	return false;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//気の矢の罠
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cTrap_ID_5::効果()
{
	return 効果(random()*8);
}
int cTrap_ID_5::効果(pcCharacter pchara)
{
	return 効果(pchara->aspect);
}
int cTrap_ID_5::効果(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	if(pdrop->aspect >= 0 && pdrop->aspect <= 7)
	{
		効果(pdrop->aspect);
	}
	else
	{
		効果(random()*8);
	}
	return false;
}

int cTrap_ID_5::効果(int aspect)
{
	pcDroping newpdrop = sg_pDungeonSystem->落ち物生成_設置なし(2000);
	//一本
	newpdrop->quality() = 0;
	newpdrop->state() = cDroping::STATE_NORMAL;
	newpdrop->discover();

	pcLandform pland = sg_pDungeonSystem->射程内障害物検索(現在地形(),aspect+2,-1,true,false,false,false);

	return sg_pDungeonSystem->無条件投擲要請(NULLCHARA,pland,newpdrop,aspect-2,DRAWTYPE_ATTACKDRAW,効果量(0));
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//徹の矢の罠
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cTrap_ID_6::効果()
{
	return 効果(random()*8);
}
int cTrap_ID_6::効果(pcCharacter pchara)
{
	return 効果(pchara->aspect);
}
int cTrap_ID_6::効果(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	if(pdrop->aspect >= 0 && pdrop->aspect <= 7)
	{
		効果(pdrop->aspect);
	}
	else
	{
		効果(random()*8);
	}
	return false;

}

int cTrap_ID_6::効果(int aspect)
{
	pcDroping newpdrop = sg_pDungeonSystem->落ち物生成_設置なし(2001);
	//一本
	newpdrop->quality() = 0;
	newpdrop->state() = cDroping::STATE_NORMAL;
	newpdrop->discover();

	pcLandform pland = sg_pDungeonSystem->射程内障害物検索(現在地形(),aspect+2,-1,true,false,false,false);

	return sg_pDungeonSystem->無条件投擲要請(NULLCHARA,pland,newpdrop,aspect-2,DRAWTYPE_ATTACKDRAW,効果量(0));
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//毒の矢の罠
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cTrap_ID_7::効果()
{
	return 効果(random()*8);
}
int cTrap_ID_7::効果(pcCharacter pchara)
{
	return 効果(pchara->aspect);
}
int cTrap_ID_7::効果(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	if(pdrop->aspect >= 0 && pdrop->aspect <= 7)
	{
		効果(pdrop->aspect);
	}
	else
	{
		効果(random()*8);
	}

	return false;
}

int cTrap_ID_7::効果(int aspect)
{
	pcDroping newpdrop = sg_pDungeonSystem->落ち物生成_設置なし(2003);
	//一本
	newpdrop->quality() = 0;
	newpdrop->state() = cDroping::STATE_NORMAL;
	newpdrop->discover();

	pcLandform pland = sg_pDungeonSystem->射程内障害物検索(現在地形(),aspect+2,-1,true,false,false,false);

	return sg_pDungeonSystem->無条件投擲要請(NULLCHARA,pland,newpdrop,aspect-2,DRAWTYPE_ATTACKDRAW,効果量(0));
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//御柱の罠
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cTrap_ID_8::効果()
{
	return 効果(random()*8);
}
int cTrap_ID_8::効果(pcCharacter pchara)
{
	return 効果(pchara->aspect);
}
int cTrap_ID_8::効果(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	if(pdrop->aspect >= 0 && pdrop->aspect <= 7)
	{
		効果(pdrop->aspect);
	}
	else
	{
		効果(random()*8);
	}

	return false;
}

int cTrap_ID_8::効果(int aspect)
{
	pcDroping newpdrop = sg_pDungeonSystem->落ち物生成_設置なし(2016);
	//一本
	newpdrop->quality() = 0;
	newpdrop->state() = cDroping::STATE_NORMAL;
	newpdrop->discover();

	pcLandform pland = sg_pDungeonSystem->射程内障害物検索(現在地形(),aspect+2,-1,true,false,false,false);

	return sg_pDungeonSystem->無条件投擲要請(NULLCHARA,pland,newpdrop,aspect-2,DRAWTYPE_ATTACKDRAW,効果量(0));
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//つるべおとしの罠
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cTrap_ID_9::効果()
{
	pcLandform pland = 現在地形();

	multiset<攻撃属性::攻撃属性> temp;
	double power = sg_pDungeonSystem->FloorLevel() * 効果量(0);
	if(pland->pOnChar)
	{
		power = pland->pOnChar->LV * 効果量(0);
	}
	if(power > 効果量(1)) power = 効果量(1);

	EffectFunctions::つるべ落としエフェクト(pland->placeX,pland->placeY);

	return sg_pDungeonSystem->攻撃接近(攻撃作成(
		NULLCHARA,//攻撃者
		pland,//場所
		power,//攻撃力
		1,//定数ダメージ
		temp,
		true));//必中
}
int cTrap_ID_9::効果(pcCharacter pchara)
{
	return 効果();
}
int cTrap_ID_9::効果(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	効果();
	return false;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//泥酔の罠
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cTrap_ID_10::効果()
{
	pcLandform pland = 現在地形();
	if(pland->pOnChar)
	{
		return sg_pDungeonSystem->泥酔要請(pland->pOnChar,効果量(0));
	}
	else
	{
		return false;
	}
}
int cTrap_ID_10::効果(pcCharacter pchara)
{
	int res = 効果();
	if(res)
	{

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Trap効果無しメッセージ"),valiable);
	}

	return res;
}
int cTrap_ID_10::効果(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	効果();
	return false;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//雨の罠
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cTrap_ID_11::効果()
{
	return true;
}
int cTrap_ID_11::効果(pcCharacter pchara)
{
	
	int res = 0;

	if(pchara->雑魚属性())
	{
		res |= sg_pDungeonSystem->脱力要請(pchara,効果量(0),効果量(1));
	}
	else
	{
		効果();

		res |= 効果_pri(pchara->attackequipment);
		if(pchara->attackequipment != pchara->defenseequipment)
		{
			res |= 効果_pri(pchara->defenseequipment);
		}
	}

	if(res)
	{

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Trap効果無しメッセージ"),valiable);
	}
	return true;
}
int cTrap_ID_11::効果(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	効果();

	効果_pri(pdrop);
	return false;
}
int cTrap_ID_11::効果_pri(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	if(pdrop == NULL) return false;

	if(pdrop->属性.count(落ち物属性::装備品))
	{
		return sg_pDungeonSystem->アイテム劣化(pdrop);
	}
	return true;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//そうびはずしの罠
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cTrap_ID_12::効果()
{
	return true;
}
int cTrap_ID_12::効果(pcCharacter pchara)
{
	int res = 0;

	if(pchara->雑魚属性())
	{
		res |= sg_pDungeonSystem->封印要請(pchara,効果量(0));
	}
	else
	{
		効果();
			
		/*
		map<tstring, StyleString> valiable;
		valiable(_T("Chara")) = pchara->Fullname();
		g_Langメッセージ(_T("cTrap_ID_12効果メッセージ"),valiable);
		*/
		int res = 0;

		res |= sg_pDungeonSystem->装備強制はずし(pchara->attackequipment);
		res |= sg_pDungeonSystem->装備強制はずし(pchara->defenseequipment);

		int i,size = pchara->最大装備スペル数();
		for(i=0;i<size;i++)
		{
			res |= sg_pDungeonSystem->装備強制はずし(pchara->spellequipment[i]);
		}
	}
	if(res)
	{

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Trap効果無しメッセージ"),valiable);
	}
	return res;
}
int cTrap_ID_12::効果(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	効果();

	return false;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//そうびまわしの罠
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cTrap_ID_13::効果()
{
	return true;
}
int cTrap_ID_13::効果(pcCharacter pchara)
{
	if(pchara->雑魚属性())
	{
		sg_pDungeonSystem->空振り要請(pchara,効果量(0));
	}
	else
	{
		効果();

		sg_pDungeonSystem->装備回し要請(pchara,1);
	}

	return true;
}
int cTrap_ID_13::効果(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	効果();


	return false;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//モンスターの罠
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cTrap_ID_14::効果()
{
	pcLandform pland = 現在地形();

	vector<pcLandform> vpland;

	if(pland->RoomIndex == -1)
	{//通路
		vpland.push_back(pland);
	}
	else
	{
		vpland = sg_pDungeonSystem->Map().RoomList[pland->RoomIndex]->allLandformList();
	}

	int res = 0;
	int i;
	for(i=0;i<vpland.size();i++)
	{
		res |= 効果_pri(vpland[i]->pOnDrop);
	}


	return res;
}
int cTrap_ID_14::効果(pcCharacter pchara)
{
	int res = 効果();
	if(res)
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("cTrap_ID_14効果メッセージ"),valiable);
	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Trap効果無しメッセージ"),valiable);
	}

	return res;
}
int cTrap_ID_14::効果(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	int res2,res = 効果();
	res |= res2 = 効果_pri(pdrop);
	if(res)
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("cTrap_ID_14効果メッセージ"),valiable);
	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Trap効果無しメッセージ"),valiable);
	}

	return res2;
}
int cTrap_ID_14::効果_pri(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	if(pdrop == NULL) return false;
	if(!pdrop->属性.count(落ち物属性::アイテム)) return false;
	if(!(pdrop->shopitemflag() == cDroping::SHOPFLAG_NONE)) return false;

	int vint[3];
	sg_pDungeonSystem->敵自然湧きID(vint);
	if(vint[0] == 0) return false;

	pcLandform pland = pdrop->現在地形();
	 
	sg_pDungeonSystem->落ち物破壊要請(pdrop);

	sg_pDungeonSystem->強制ひるみ要請(sg_pDungeonSystem->キャラクター生成(vint[0],vint[1],CHARACTER_FORSE_ENEMY,pland));
	return true;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//スキマの罠
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cTrap_ID_15::効果()
{
	//エフェクト未実装
	return true;
}
int cTrap_ID_15::効果(pcCharacter pchara)
{
	効果();

	map<tstring, StyleString> valiable;
	valiable[_T("Chara")] = pchara->ShortName();
	g_Langメッセージ(_T("cTrap_ID_15効果メッセージ"),valiable);
	
	multiset<攻撃属性::攻撃属性> temp;
	sg_pDungeonSystem->攻撃接近(攻撃作成(
		NULLCHARA,//攻撃者
		pchara->足元地形(),//場所
		効果量(0),//攻撃力
		2,//ワリアイダメージ
		temp,
		true));//必中

	if(pchara == sg_pDungeonSystem->pPlayerChara())
	{

		if(!sg_pDungeonSystem->拠点フラグ() && !sg_pDungeonSystem->ボス戦中判定()) {
			sg_pDungeonSystem->メッセージ決定待ち();

			sg_pDungeonSystem->フロア更新要請();
		}
	}
	else
	{
		if(!pchara->CharaAttribute().count(キャラ属性::ボス)) {
			sg_pDungeonSystem->強制退場要請(pchara);
		}
	}
	return true;
}
int cTrap_ID_15::効果(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	効果();
	sg_pDungeonSystem->落ち物破壊要請(pdrop);

	return true;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//のろいの罠
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cTrap_ID_16::効果()
{
	//エフェクト未実装
	return true;
}
int cTrap_ID_16::効果(pcCharacter pchara)
{

	if(pchara->雑魚属性())
	{
		sg_pDungeonSystem->死の誘い要請(pchara, sg_pDungeonSystem->pPlayerChara(), 効果量(0));
	}
	else
	{
		効果();

		if(pchara->holdItem.empty())
		{
			map<tstring, StyleString> valiable;
			g_Langメッセージ(_T("Trap効果無しメッセージ"),valiable);
			return false;
		}

		pcDroping pdrop = pchara->holdItem[pchara->holdItem.size()*random()];

		if(sg_pDungeonSystem->アイテム呪い(pdrop))
		{

		}
		else
		{
			map<tstring, StyleString> valiable;
			g_Langメッセージ(_T("Trap効果無しメッセージ"),valiable);
		}
	}
	return true;
}
int cTrap_ID_16::効果(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	効果();
	sg_pDungeonSystem->アイテム呪い(pdrop);

	return false;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//幸運の罠
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cTrap_ID_17::効果()
{
	//エフェクト未実装
	return true;
}
int cTrap_ID_17::効果(pcCharacter pchara)
{
	if(pchara->雑魚属性())
	{
		sg_pDungeonSystem->無敵要請(pchara, 効果量(0));
	}
	else
	{
		効果();

		if(pchara->holdItem.empty())
		{
			map<tstring, StyleString> valiable;
			g_Langメッセージ(_T("Trap効果無しメッセージ"),valiable);
			return false;
		}

		pcDroping pdrop = pchara->holdItem[pchara->holdItem.size()*random()];

		sg_pDungeonSystem->アイテム祝福(pdrop);
	}
	return true;
}
int cTrap_ID_17::効果(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	効果();
	sg_pDungeonSystem->アイテム祝福(pdrop);

	return false;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//貧乏の罠
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cTrap_ID_18::効果()
{
	//エフェクト未実装
	return true;
}
int cTrap_ID_18::効果(pcCharacter pchara)
{
	効果();

	if(!sg_pDungeonSystem->貧乏要請(pchara,効果量(0)))
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Trap効果無しメッセージ"),valiable);
		return false;
	}

	return true;
}
int cTrap_ID_18::効果(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	効果();

	return false;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//虎ばさみの罠
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cTrap_ID_19::効果()
{
	//エフェクト未実装
	return true;
}
int cTrap_ID_19::効果(pcCharacter pchara)
{
	効果();

	if(!sg_pDungeonSystem->氷付け要請(pchara,効果量(0)))
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Trap効果無しメッセージ"),valiable);
		return false;
	}

	return true;
}
int cTrap_ID_19::効果(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	効果();

	return false;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//召喚の罠
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cTrap_ID_20::効果()
{
	int size = 効果量(0);//敵出現数
	
	int i,vint[3];
	sg_pDungeonSystem->敵自然湧きID(vint);
		
	pcLandform pland = 現在地形();

	for(i=0;i<size;i++)
	{
		pcCharacter pchara = sg_pDungeonSystem->キャラクター生成(vint[0],vint[1],CHARACTER_FORSE_ENEMY,pland);
		if(pchara != NULL)
		{
			EffectFunctions::煙エフェクト1(pchara->placeX, pchara->placeY);
			pchara->Condition.とても強い刺激();
			//sg_pDungeonSystem->眠り要請(pchara,0,0);
			sg_pDungeonSystem->強制ひるみ要請(pchara);
		}
	}

	return true;
}
int cTrap_ID_20::効果(pcCharacter pchara)
{
	if(!効果())
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Trap効果無しメッセージ"),valiable);
		return false;
	}

	return true;
}
int cTrap_ID_20::効果(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	効果();

	return false;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//罠の罠
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cTrap_ID_21::効果()
{
	int vint[3];

	int eff = 0;

	int i,size = 効果量(0);
	for(i=0;i<size;i++)
	{
		sg_pDungeonSystem->罠自然湧きID(vint);
		pcDroping pdrop = sg_pDungeonSystem->落ち物生成(vint[0]);

		if(pdrop != NULL)
		{
			pdrop->disappear();
			eff++;
		}
	}

	if(eff)
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("cTrap_ID_21効果メッセージ"),valiable);
	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Trap効果無しメッセージ"),valiable);
	}
	return eff;
}
int cTrap_ID_21::効果(pcCharacter pchara)
{
	return 効果();
}
int cTrap_ID_21::効果(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	効果();

	return false;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//三歩必殺の罠
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cTrap_ID_22::効果()
{

	EffectFunctions::三歩必殺(現在地形()->placeX,現在地形()->placeY);
	pcLandform pland = 現在地形();
	int size = 25;
	int i;
	for(i = 0; i<size ;i++)
	{
		pcLandform pland2 = sg_pDungeonSystem->代理地形(pland,i);
		if( pland2->placeX == -1 || pland2->placeY == -1)
		{//エッジ
			continue;
		}

	
		multiset<攻撃属性::攻撃属性> temp;
		double power = 1;
		if(pland2->pOnChar)
		{
			power = pland2->pOnChar->MHP * 効果量(0) / 100.0;
		}
		if(power > 効果量(1)) power = 効果量(1);
		sg_pDungeonSystem->攻撃接近(攻撃作成(
			NULLCHARA,//攻撃者
			pland2,//場所
			power,//攻撃力
			1,//定数ダメージ
			temp,
			true));//必中
	}
	return true;
}
int cTrap_ID_22::効果(pcCharacter pchara)
{
	if(!効果())
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Trap効果無しメッセージ"),valiable);
		return false;
	}

	return true;
}
int cTrap_ID_22::効果(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	効果();

	return false;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//クランの罠
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cTrap_ID_23::効果()
{
	
	vector<pcDroping> vpdrop = sg_pDungeonSystem->DropList();
	vector<pcCharacter> vpchara = sg_pDungeonSystem->CharaList();

	int i;
	for(i=0;i<vpchara.size();i++)
	{
		sg_pDungeonSystem->ワープ要請(vpchara[i],FindUtility::各部屋等確率_ランダム地形検索_配置安全());
	}
	for(i=0;i<vpchara.size();i++)
	{
		multiset<攻撃属性::攻撃属性> temp;
		temp.insert(攻撃属性::水);
		double power = 効果量(0);
		sg_pDungeonSystem->攻撃接近(攻撃作成(
			NULLCHARA,//攻撃者
			vpchara[i]->足元地形(),//場所
			power,//攻撃力
			1,//定数ダメージ
			temp,
			true));//必中
	}

	for(i=0;i<vpdrop.size();i++)
	{
		if(vpdrop[i]->Holder() == NULL
			&& !vpdrop[i]->大切()
			&& vpdrop[i]->属性.count(落ち物属性::アイテム)
			&& vpdrop[i]->shopitemflag() == cDroping::SHOPFLAG_NONE)
		{
			sg_pDungeonSystem->落ち物ワープ(vpdrop[i],FindUtility::各部屋等確率_ランダム地形検索_配置安全());
		}
	}

	return true;
}
int cTrap_ID_23::効果(pcCharacter pchara)
{
	if(!効果())
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Trap効果無しメッセージ"),valiable);
		return false;
	}

	return true;
}
int cTrap_ID_23::効果(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	効果();

	return false;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//デロの罠
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cTrap_ID_24::効果()
{
	//エフェクト未実装

	pcCharacter pchara = 乗りキャラ();

	if(pchara == NULL) return true;
	
	cValiableField valf;

	if(pchara->雑魚属性())
	{
		sg_pDungeonSystem->即死要請(pchara,false,true);
		pcDroping pdrop = sg_pDungeonSystem->落ち物生成_設置なし(4003);
		sg_pDungeonSystem->落ち物落下(pdrop,現在地形());
		valf.doubles.dim(変数_汎用ブール) = 1;
		return true;
	}
	else
	{
		valf.doubles.dim(変数_汎用ブール) = 1;//効果が発生するか
		sg_pDungeonSystem->CutInM().CutIn(pchara,食べ物劣化直前_タイミング,valf);
		if(valf.doubles.val(変数_汎用ブール))
		{
			
			valf.doubles.dim(変数_汎用ブール) = 0;//効果が発生したか
			sg_pDungeonSystem->CutInM().CutIn(pchara,食べ物劣化_タイミング,valf);
		}
		//pcDroping pdrop2 = sg_pDungeonSystem->落ち物生成_設置なし(4003);
		//sg_pDungeonSystem->落ち物変化(pdrop,pdrop2);
	}

	return valf.doubles.val(変数_汎用ブール);
}
int cTrap_ID_24::効果(pcCharacter pchara)
{
	


	if(!効果())
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Trap効果無しメッセージ"),valiable);
		return false;
	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("cTrap_ID_24効果メッセージ"),valiable);
		return true;
	}


	return true;
}
int cTrap_ID_24::効果(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	効果();

	if(pdrop->属性.count(落ち物属性::食べ物))
	{
		sg_pDungeonSystem->アイテム劣化(pdrop);
		return true;
	}

	return false;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//ころびの罠
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cTrap_ID_25::効果()
{
	//エフェクト未実装
	return true;
}
int cTrap_ID_25::効果(pcCharacter pchara)
{
	if(pchara->雑魚属性())
	{
		sg_pDungeonSystem->びっくり要請(pchara, 効果量(2));
	}

	効果();

	/*
	if(pchara->holdItem.empty())
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Trap効果無しメッセージ"),valiable);
		return false;
	}
	*/

	int res = 0;
	
	if(sg_pDungeonSystem->ころび要請(pchara,効果量(1)))
	{
		map<tstring, StyleString> valiable;
		valiable[_T("Chara")] = pchara->ShortName();
		g_Langメッセージ(_T("cTrap_ID_25効果メッセージ"),valiable);
		return false;

		multiset<攻撃属性::攻撃属性> temp;
		double power = 効果量(0);
		sg_pDungeonSystem->攻撃接近(攻撃作成(
			NULLCHARA,//攻撃者
			pchara->足元地形(),//場所
			power,//攻撃力
			1,//定数ダメージ
			temp,
			true));//必中

		res = 1;
	}

	if(!res)
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Trap効果無しメッセージ"),valiable);
		return false;
	}
	else
	{

	}
	return true;

}
int cTrap_ID_25::効果(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	効果();

	return false;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//うつの罠
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cTrap_ID_26::効果()
{
	//エフェクト未実装

	return true;
}
int cTrap_ID_26::効果(pcCharacter pchara)
{
	return sg_pDungeonSystem->速度減少要請(pchara, 効果量(0));
}
int cTrap_ID_26::効果(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	効果();
	return false;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//警報の罠
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cTrap_ID_27::効果()
{
	//エフェクト未実装
	vector<pcCharacter> vpchara = sg_pDungeonSystem->CharaList();

	int i;
	for(i=0;i<vpchara.size();i++)
	{
		vpchara[i]->Condition.とても強い刺激();
	}

	map<tstring, StyleString> valiable;
	g_Langメッセージ(_T("cTrap_ID_27効果メッセージ"),valiable);

	return true;
}
int cTrap_ID_27::効果(pcCharacter pchara)
{
	効果();
	return true;
}
int cTrap_ID_27::効果(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	効果();
	return false;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//青竹踏みの罠
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cTrap_ID_28::効果()
{
	//エフェクト未実装



	return true;
}
int cTrap_ID_28::効果(pcCharacter pchara)
{
	効果();

	sg_pDungeonSystem->健康要請(pchara,効果量(0),false);
	GameIdiom::悪性異常状態治療要請(pchara);

	map<tstring, StyleString> valiable;
	valiable[_T("Chara")] = pchara->ShortName();
	g_Langメッセージ(_T("cTrap_ID_28効果メッセージ"),valiable);

	return true;
}
int cTrap_ID_28::効果(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	効果();
	return false;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//二の足の罠
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cTrap_ID_29::効果()
{
	//エフェクト未実装



	return true;
}
int cTrap_ID_29::効果(pcCharacter pchara)
{
	効果();

	sg_pDungeonSystem->臆病要請(pchara,効果量(0),true);


	return true;
}
int cTrap_ID_29::効果(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	効果();
	return false;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//宵闇の罠
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cTrap_ID_30::効果()
{
	//エフェクト未実装

	sw() = !sw();

	return true;
}
int cTrap_ID_30::効果(pcCharacter pchara)
{
	if(pchara->雑魚属性())
	{
		sg_pDungeonSystem->鳥目要請(pchara, 効果量(1));
	}

	効果();
	return true;
}
int cTrap_ID_30::効果(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	効果();
	return false;
}
void cTrap_ID_30::CutIn(タイミング timing, cValiableField& valiable)
{
	cTrap::CutIn(timing,valiable);
	if(timing == 暗闇更新_タイミング && sw())
	{
		sg_pDungeonSystem->暗闇付与(現在地形(),効果量(0));
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//宴会の罠
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cTrap_ID_31::効果()
{
	//エフェクト未実装
	int roomIndex = 現在地形()->RoomIndex;
	int eff = 0;

	if(roomIndex == -1)
	{//通路
		GameIdiom::高飛び(me(),現在地形());
	}
	/*
	else {
		pcRoom proom = sg_pDungeonSystem->Map().RoomList[roomIndex];
		if(すでに宴会) {
			return false;
		}
	}
	*/

	roomIndex = 現在地形()->RoomIndex;

	if(roomIndex != -1)
	{
		sg_pDungeonSystem->宴会生成(roomIndex);
		eff = 1;
	}

	return eff;
}
int cTrap_ID_31::効果(pcCharacter pchara)
{

	効果();
	return true;
}
int cTrap_ID_31::効果(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	効果();
	return false;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//病気の罠
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cTrap_ID_32::効果()
{
	//エフェクト未実装
	int eff = 0;

	return eff;
}
int cTrap_ID_32::効果(pcCharacter pchara)
{

	効果();

	sg_pDungeonSystem->病気要請(pchara,効果量(0),true);

	return true;
}
int cTrap_ID_32::効果(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	効果();
	return false;
}
