#include "stdafx.h"
#include "Bullet1.h"
#include "EffectFunctions.h"
#include "GameIdiom.h"
#include "cDungeonSystem.h"
#include "cAttackinformation.h"
#include "cObjectchara.h"

bool cBullet_能力仕様フラグID_exist(cValiableField& valiable, int ID)
{
	return valiable.doubles.exist((_T("cBullet_能力仕様フラグID_") + setStyle(ID)).c_str());
}
double& cBullet_能力仕様フラグID_dim(cValiableField& valiable, int ID)
{
	return valiable.doubles.dim((_T("cBullet_能力仕様フラグID_") + setStyle(ID)).c_str());
}
double& cBullet_能力仕様フラグID_val(cValiableField& valiable, int ID)
{
	return valiable.doubles.val((_T("cBullet_能力仕様フラグID_") + setStyle(ID)).c_str());
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int cBullet_ID_0::衝突時効果(pcCharacter pchara)
{
	int res = cBullet::衝突時効果(pchara);
	sg_pDungeonSystem->動的識別(me());
	return res;
}

//------------------------------------------------------------------------------
int cBullet_ID_1::衝突時効果(pcCharacter pchara)
{
	int res = cBullet::衝突時効果(pchara);
	sg_pDungeonSystem->動的識別(me());
	return res;
}

//------------------------------------------------------------------------------
int cBullet_ID_2::衝突時効果(pcCharacter pchara)
{
	int res = 効果(pchara->足元地形());
	sg_pDungeonSystem->動的識別(me());
	return res;
}
int cBullet_ID_2::効果(pcLandform pland)
{
	EffectFunctions::爆発エフェクト_中(me()->placeX,me()->placeY,20);
	double power = 威力計算();
	if(explosion_power() != 0)
	{
		power = (double)explosion_power();
	}
	return GameIdiom::爆発(
		pland,
		投擲者(),
		power,
		攻撃属性_定数ダメージ(),
		FALSE,
		FALSE,
		TRUE,
		TRUE
		);
}
void cBullet_ID_2::CutIn(タイミング timing, cValiableField& valiable)
{
	cBullet::CutIn(timing,valiable);

	if(timing == 被衝撃_強_タイミング)
	{
		if(!broken)
		{
			効果(me()->現在地形());
			sg_pDungeonSystem->動的識別(me());
			sg_pDungeonSystem->落ち物破壊要請(me());
		}
	}

}
//------------------------------------------------------------------------------
//毒
int cBullet_ID_3::衝突時効果(pcCharacter pchara)
{
	int res = cBullet::衝突時効果(pchara);
	sg_pDungeonSystem->脱力要請(pchara,効果量(0),効果量(1));
	sg_pDungeonSystem->動的識別(me());
	return res;
}
//------------------------------------------------------------------------------
//銀
int cBullet_ID_4::衝突時効果(pcCharacter pchara)
{
	int res = cBullet::衝突時効果(pchara);
	sg_pDungeonSystem->動的識別(me());
	return res;
}
//------------------------------------------------------------------------------
//軟弱
int cBullet_ID_5::衝突時効果(pcCharacter pchara)
{
	int res = cBullet::衝突時効果(pchara);
	sg_pDungeonSystem->軟弱要請(pchara,効果量(0),効果量(1));
	sg_pDungeonSystem->動的識別(me());
	return res;
}
//------------------------------------------------------------------------------
//火の矢
int cBullet_ID_6::衝突時効果(pcCharacter pchara)
{
	int res = cBullet::衝突時効果(pchara);

	sg_pDungeonSystem->動的識別(me());
	return res;
}
//------------------------------------------------------------------------------
//水の矢
int cBullet_ID_7::衝突時効果(pcCharacter pchara)
{
	int res = cBullet::衝突時効果(pchara);

	sg_pDungeonSystem->動的識別(me());
	return res;
}
//------------------------------------------------------------------------------
//石つぶて
int cBullet_ID_8::衝突時効果(pcCharacter pchara)
{
	int res = cBullet::衝突時効果(pchara);

	sg_pDungeonSystem->動的識別(me());
	return res;
}
//------------------------------------------------------------------------------
//ナゲナイフ
int cBullet_ID_9::衝突時効果(pcCharacter pchara)
{
	int res = cBullet::衝突時効果(pchara);

	sg_pDungeonSystem->動的識別(me());
	return res;
}
void cBullet_ID_9::CutIn(タイミング timing, cValiableField& valiable)
{
	cBullet::CutIn(timing,valiable);

	if(timing == 投擲反射回数計算時ドロップ専用_タイミング)
	{
		valiable.doubles[変数_汎用実数] += 効果量(0);
	}

}
//------------------------------------------------------------------------------
//
int cBullet_ID_10::衝突時効果(pcCharacter pchara)
{
	int res = cBullet::衝突時効果(pchara);

	sg_pDungeonSystem->動的識別(me());
	return res;
}
//------------------------------------------------------------------------------
//必中の大玉
int cBullet_ID_11::衝突時効果(pcCharacter pchara)
{
	int res = cBullet::衝突時効果(pchara);

	sg_pDungeonSystem->動的識別(me());
	return res;
}
void cBullet_ID_11::能力(const タイミング timing, cValiableField& valiable)
{
	if(timing == 直接攻撃直前_タイミング)
	{
		if(能力発動条件満たしている_攻撃用())
		{
			if(ceil(効果量(0)*能力効果量倍率())/100.0 > random())
			{
				valiable.intsets.val(変数_属性).insert(攻撃属性::必中);
			}
		}
	}

}
//------------------------------------------------------------------------------
//三連投げナイフ
int cBullet_ID_12::衝突時効果(pcCharacter pchara)
{
	int res = cBullet::衝突時効果(pchara);

	sg_pDungeonSystem->動的識別(me());
	return res;
}
int cBullet_ID_12::撃つ_効果_通常(pcCharacter pchara)
{
	int res = cBullet::撃つ_効果_通常(pchara);
	sg_pDungeonSystem->方向転換要請(pchara,pchara->aspect + 1);
	if(quantity() > 0) cBullet::撃つ_効果_通常(pchara);
	sg_pDungeonSystem->方向転換要請(pchara,pchara->aspect - 2);
	if(quantity() > 0) cBullet::撃つ_効果_通常(pchara);
	sg_pDungeonSystem->方向転換要請(pchara,pchara->aspect + 1);
	return res;
}
void cBullet_ID_12::CutIn(タイミング timing, cValiableField& valiable)
{
	cBullet::CutIn(timing,valiable);

	if(timing == 投擲反射回数計算時ドロップ専用_タイミング)
	{
		valiable.doubles[変数_汎用実数] += 効果量(0);
	}

}
//------------------------------------------------------------------------------
//蝶球
int cBullet_ID_13::衝突時効果(pcCharacter pchara)
{
	int res = cBullet::衝突時効果(pchara);

	sg_pDungeonSystem->動的識別(me());
	return res;
}
int cBullet_ID_13::撃つ_効果_通常(pcCharacter pchara)
{
	int res = cBullet::撃つ_効果_通常(pchara);
	sg_pDungeonSystem->方向転換要請(pchara,pchara->aspect + 1);
	if(quantity() > 0) cBullet::撃つ_効果_通常(pchara);
	sg_pDungeonSystem->方向転換要請(pchara,pchara->aspect - 2);
	if(quantity() > 0) cBullet::撃つ_効果_通常(pchara);
	sg_pDungeonSystem->方向転換要請(pchara,pchara->aspect + 1);
	return res;
}
//------------------------------------------------------------------------------
int cBullet_ID_14::衝突時効果(pcCharacter pchara)
{
	int res = 0;
	if(!broken)
	{
		res = 効果(pchara->足元地形());
		sg_pDungeonSystem->動的識別(me());
		sg_pDungeonSystem->落ち物破壊要請(me());
	}
	return res;
}
int cBullet_ID_14::効果(pcLandform pland)
{
	EffectFunctions::爆発エフェクト_中(me()->placeX,me()->placeY,20);
	double power = 威力計算();
	return GameIdiom::爆発(
		pland,
		投擲者(),
		power,
		攻撃属性_定数ダメージ(),
		FALSE,
		FALSE,
		TRUE
		);
}
void cBullet_ID_14::CutIn(タイミング timing, cValiableField& valiable)
{
	cBullet::CutIn(timing,valiable);

	if(timing == 被衝撃_強_タイミング || timing == 落ち物ひまなげ落下直後時_タイミング)
	{
		if(!broken)
		{
			効果(me()->現在地形());
			sg_pDungeonSystem->動的識別(me());
			sg_pDungeonSystem->落ち物破壊要請(me());
		}
	}
}
//------------------------------------------------------------------------------
int cBullet_ID_15::衝突時効果(pcCharacter pchara)
{
	int res = cBullet::衝突時効果(pchara);

	sg_pDungeonSystem->動的識別(me());
	return res;
}
//------------------------------------------------------------------------------
int cBullet_ID_16::衝突時効果(pcCharacter pchara)
{
	sg_pDungeonSystem->吹き飛ばし要請(投擲者(),pchara,aspect,効果量(0),0);

	int res = cBullet::衝突時効果(pchara);

	sg_pDungeonSystem->動的識別(me());
	return res;
}
//------------------------------------------------------------------------------
int cBullet_ID_17::衝突時効果(pcCharacter pchara)
{
	int res = cBullet::衝突時効果(pchara);

	sg_pDungeonSystem->動的識別(me());
	return res;
}
void cBullet_ID_17::CutIn(タイミング timing, cValiableField& valiable)
{
	cBullet::CutIn(timing,valiable);

	if(timing == 投擲反射回数計算時ドロップ専用_タイミング)
	{
		valiable.doubles[変数_汎用実数2] += 効果量(0);
	}

}
//------------------------------------------------------------------------------
int cBullet_ID_18::衝突時効果(pcCharacter pchara)
{
	double power = 1.0;
	if(pchara->CharaAttribute().count(キャラ属性::鬼))
	{
		power = 効果量(0)/100.0;
		sg_pDungeonSystem->脱力要請(pchara,効果量(1),効果量(3));
		sg_pDungeonSystem->軟弱要請(pchara,効果量(2),効果量(3));
	}

	return ダメージ効果(投擲者()
		, sg_pDungeonSystem->キャラ足元地形(pchara)//地形
		, 威力計算()*power//威力
		);
}
//------------------------------------------------------------------------------
void cBullet_ID_19::CutIn(タイミング timing, cValiableField& valiable)
{
	cBullet::CutIn(timing,valiable);

	//装備していなくても良い
	if(Parent() == NULL)
	{//じか持ち
		if(timing == ころび直前_タイミング && !cBullet_能力仕様フラグID_exist(valiable,ID()))
		{
			valiable.doubles[変数_汎用ブール] = 0;
			cBullet_能力仕様フラグID_dim(valiable,ID()) = 1;
		
			if(効果量(0) > random()*100)
			{

				if(quality() < 1)
				{//破壊
					map<tstring, StyleString> valiable;
					valiable[_T("Item")] = FullName();
					g_Langメッセージ(_T("Bullet護符消滅メッセージ"),valiable);
				
					sg_pDungeonSystem->落ち物破壊要請(me());
				}
				else
				{
					map<tstring, StyleString> valiable;
					valiable[_T("Item")] = FullName();
					g_Langメッセージ(_T("Bullet護符効果メッセージ"),valiable);
				
					quality()--;
				}
			}
		}

	}

}
//------------------------------------------------------------------------------
int cBullet_ID_20::衝突時効果(pcCharacter pchara)
{
	int res = cBullet::衝突時効果(pchara);

	sg_pDungeonSystem->動的識別(me());
	return res;
}
//------------------------------------------------------------------------------
//うさぎ飛び弾
int cBullet_ID_21::衝突時効果(pcCharacter pchara)
{
	pcLandform pland = sg_pDungeonSystem->キャラ前地形(pchara,射程距離());
	
	投擲者_weakpointer = wpcCharacter(pchara);

	int res = sg_pDungeonSystem->投げ飛ばし要請(pchara,pchara,pland,威力計算(),true);

	sg_pDungeonSystem->動的識別(me());
	return res;
}
int cBullet_ID_21::撃つ_効果_通常(pcCharacter pchara)
{
	pcDroping newpdrop = sg_pDungeonSystem->落ち物生成_設置なし(ID(),dpack.integer);

	sg_pDungeonSystem->アイテム店フラグ(newpdrop,cDroping::SHOPFLAG_NONE);//売り物でない
	sg_pDungeonSystem->アイテム割引フラグ(newpdrop, cDroping::SHOPDISCOUNTFLAG_NONE);
	//一本
	newpdrop->quality() = 0;


	本数消費();
	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("shoot.wav"),現在地形()->place);
	溜めエフェクト(pchara);
	
	return newpdrop->衝突時効果(pchara);
}
//------------------------------------------------------------------------------
//保冷
void cBullet_ID_22::CutIn(タイミング timing, cValiableField& valiable)
{
	cBullet::CutIn(timing,valiable);

	//装備していなくても良い
	if(Parent() == NULL)
	{//じか持ち
		if(timing == 食べ物劣化直前_タイミング && !cBullet_能力仕様フラグID_exist(valiable,ID()))
		{
			valiable.doubles[変数_汎用ブール] = 0;
			cBullet_能力仕様フラグID_dim(valiable,ID()) = 1;
			if(効果量(0) > random()*100)
			{
				if(quality() < 1)
				{//破壊
					map<tstring, StyleString> valiable;
					valiable[_T("Item")] = FullName();
					g_Langメッセージ(_T("Bullet護符消滅メッセージ"),valiable);
				
					sg_pDungeonSystem->落ち物破壊要請(me());
				}
				else
				{
					map<tstring, StyleString> valiable;
					valiable[_T("Item")] = FullName();
					g_Langメッセージ(_T("Bullet護符効果メッセージ"),valiable);
				
					quality()--;
				}
			}
		}

	}

}
//------------------------------------------------------------------------------
//水難
void cBullet_ID_23::CutIn(タイミング timing, cValiableField& valiable)
{
	cBullet::CutIn(timing,valiable);

	//装備していなくても良い
	if(Parent() == NULL)
	{//じか持ち
		if(timing == 本濡れ直前_タイミング && !cBullet_能力仕様フラグID_exist(valiable,ID()))
		{
			valiable.doubles[変数_汎用ブール] = 0;
			cBullet_能力仕様フラグID_dim(valiable,ID()) = 1;
		
			if(効果量(0) > random()*100)
			{
				if(quality() < 1)
				{//破壊
					map<tstring, StyleString> valiable;
					valiable[_T("Item")] = FullName();
					g_Langメッセージ(_T("Bullet護符消滅メッセージ"),valiable);
				
					sg_pDungeonSystem->落ち物破壊要請(me());
				}
				else
				{
					map<tstring, StyleString> valiable;
					valiable[_T("Item")] = FullName();
					g_Langメッセージ(_T("Bullet護符効果メッセージ"),valiable);
				
					quality()--;
				}
			}
		}

	}

}
//------------------------------------------------------------------------------
//静電気除け
void cBullet_ID_24::CutIn(タイミング timing, cValiableField& valiable)
{
	cBullet::CutIn(timing,valiable);

	//装備していなくても良い
	if(Parent() == NULL)
	{//じか持ち
		if(timing == 箱帯静電気直前_タイミング && !cBullet_能力仕様フラグID_exist(valiable,ID()))
		{
			valiable.doubles[変数_汎用ブール] = 0;
			cBullet_能力仕様フラグID_dim(valiable,ID()) = 1;
			if(効果量(0) > random()*100)
			{
				if(quality() < 1)
				{//破壊
					map<tstring, StyleString> valiable;
					valiable[_T("Item")] = FullName();
					g_Langメッセージ(_T("Bullet護符消滅メッセージ"),valiable);
				
					sg_pDungeonSystem->落ち物破壊要請(me());
				}
				else
				{
					map<tstring, StyleString> valiable;
					valiable[_T("Item")] = FullName();
					g_Langメッセージ(_T("Bullet護符効果メッセージ"),valiable);
				
					quality()--;
				}
			}
		}

	}

}
//------------------------------------------------------------------------------
//冷凍除け
void cBullet_ID_25::CutIn(タイミング timing, cValiableField& valiable)
{
	cBullet::CutIn(timing,valiable);

	//装備していなくても良い
	if(Parent() == NULL)
	{//じか持ち
		if(timing == 水筒冷凍直前_タイミング && !cBullet_能力仕様フラグID_exist(valiable,ID()))
		{
			valiable.doubles[変数_汎用ブール] = 0;
			cBullet_能力仕様フラグID_dim(valiable,ID()) = 1;

			if(効果量(0) > random()*100)
			{
				if(quality() < 1)
				{//破壊
					map<tstring, StyleString> valiable;
					valiable[_T("Item")] = FullName();
					g_Langメッセージ(_T("Bullet護符消滅メッセージ"),valiable);
				
					sg_pDungeonSystem->落ち物破壊要請(me());
				}
				else
				{
					map<tstring, StyleString> valiable;
					valiable[_T("Item")] = FullName();
					g_Langメッセージ(_T("Bullet護符効果メッセージ"),valiable);
				
					quality()--;
				}
			}	
		}

	}

}
//------------------------------------------------------------------------------
int cBullet_ID_26::加算攻撃力()
{
	return cBullet::加算攻撃力() + attack_power();
}
int cBullet_ID_26::衝突時効果(pcCharacter pchara)
{
	sg_pDungeonSystem->吹き飛ばし要請(投擲者(),pchara,aspect,push_power(),0);

	int res = cBullet::衝突時効果(pchara);

	sg_pDungeonSystem->動的識別(me());
	return res;
}
void cBullet_ID_26::CutIn(タイミング timing, cValiableField& valiable)
{
	cBullet::CutIn(timing,valiable);

	if(timing == 落ち物落下直後時_タイミング)
	{
		if(!broken)
		{
			sg_pDungeonSystem->動的識別(me());
			sg_pDungeonSystem->落ち物破壊要請(me());
		}
	}
	if(timing == セクション終了_タイミング)
	{
		pcDroping newdrop = sg_pDungeonSystem->落ち物生成_設置なし(2016);
		
		if(newdrop != NULL)
		{
			newdrop->state() = state();
			newdrop->quality() = quality();
			if(状態値識別済み()) sg_pDungeonSystem->状態のみ識別(newdrop,0);
			if(修正値識別済み()) sg_pDungeonSystem->修正のみ識別(newdrop,0);
			sg_pDungeonSystem->落ち物変化(me(),newdrop);
		}
		pcCharacter pchara = newdrop->Holder();
		if(pchara) {
			sg_pDungeonSystem->落ち物まとめ要請(newdrop,pchara);
		}
	}
}
//------------------------------------------------------------------------------
int cBullet_ID_27::衝突時効果(pcCharacter pchara)
{
	int res = 効果(pchara->足元地形());
	sg_pDungeonSystem->動的識別(me());
	return res;
}
int cBullet_ID_27::効果(pcLandform pland)
{
	EffectFunctions::爆発エフェクト_中(me()->placeX,me()->placeY,20);
	double power = 威力計算();
	if(explosion_power() != 0)
	{
		power = (double)explosion_power();
	}
	return GameIdiom::爆発(
		pland,
		投擲者(),
		power,
		攻撃属性_定数ダメージ(),
		FALSE,
		FALSE,
		TRUE,
		TRUE
		);
}
void cBullet_ID_27::CutIn(タイミング timing, cValiableField& valiable)
{
	cBullet::CutIn(timing,valiable);

	if(timing == 被衝撃_強_タイミング)
	{
		if(!broken)
		{
			効果(me()->現在地形());
			sg_pDungeonSystem->動的識別(me());
			sg_pDungeonSystem->落ち物破壊要請(me());
		}
	}
	if(timing == 落ち物落下直後時_タイミング)
	{
		if(!broken)
		{
			sg_pDungeonSystem->動的識別(me());
			sg_pDungeonSystem->落ち物破壊要請(me());
		}
	}
	if(timing == セクション終了_タイミング)
	{
		pcDroping newdrop = sg_pDungeonSystem->落ち物生成_設置なし(2002);
		
		if(newdrop != NULL)
		{
			newdrop->state() = state();
			newdrop->quality() = quality();
			if(状態値識別済み()) sg_pDungeonSystem->状態のみ識別(newdrop,0);
			if(修正値識別済み()) sg_pDungeonSystem->修正のみ識別(newdrop,0);
			sg_pDungeonSystem->落ち物変化(me(),newdrop);
		}
		pcCharacter pchara = newdrop->Holder();
		if(pchara) {
			sg_pDungeonSystem->落ち物まとめ要請(newdrop,pchara);
		}
	}
}
//------------------------------------------------------------------------------
//スケープゴート弾
int cBullet_ID_28::衝突時効果(pcCharacter pchara)
{
	const int bulletnum = quantity();
	int bullet_count = 0;

	const int MaxDistance = 4;

	for(int distance = 1; distance < MaxDistance ;distance++) {
		for(int aspect = 0; aspect < 8 ;aspect++) {

			int aspect_res = aspect-4 + pchara->aspect;

			pcLandform pland = sg_pDungeonSystem->キャラ視点方角地形(pchara,aspect-4,distance);
			
			if(sg_pDungeonSystem->キャラ配置安全(pland)) {
			
				効果(pland, aspect_res);

				bullet_count++;
				if(bullet_count >= bulletnum) {
					break;
				}
			}
		}
	}
	
	sg_pDungeonSystem->動的識別(me());
	return true;
}
int cBullet_ID_28::効果(pcLandform pland, int aspect)
{
	pcCharacter pchara = 弾幕::弾幕召喚(
				pland,//場所
				威力計算(),//HP
				CHARACTER_FORSE_FRIEND,//Forse
				0,//speed
				弾幕::直進,//弾幕Type
				弾幕::ミドリ,//弾幕色
				aspect,//aspect
				NULLCHARA//targetenemy
				);

	if(pchara) {
		sg_pDungeonSystem->身代わり要請(pchara, sg_pDungeonSystem->pPlayerChara(), GAME_TURN_GAMEOVER, false);
		sg_pDungeonSystem->動的識別(me());
	}
	return (pchara != NULLCHARA);
}
int cBullet_ID_28::撃つ_効果_通常(pcCharacter pchara)
{
	pcLandform forwordLand = sg_pDungeonSystem->キャラ視点方角地形(pchara,0,1);

	本数消費();
	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("shoot.wav"),現在地形()->place);
	溜めエフェクト(pchara);
	
	return 効果(forwordLand, pchara->aspect);
}
//------------------------------------------------------------------------------
//壁弾幕
int cBullet_ID_29::衝突時効果(pcCharacter pchara)
{
	
	投擲者_weakpointer = wpcCharacter(pchara);

	const int bulletnum = quantity() * 効果量(1);
	int bullet_count = 0;

	const int MaxDistance = 4;

	for(int distance = 1; distance < MaxDistance ;distance++) {
		for(int aspect = 0; aspect < 8 ;aspect++) {

			int aspect_res = aspect-4 + pchara->aspect;

			pcLandform pland = sg_pDungeonSystem->キャラ視点方角地形(pchara,aspect-4,distance);
			
			if(sg_pDungeonSystem->キャラ配置安全(pland)) {
			
				pcCharacter pchara = 弾幕::弾幕召喚(
					pland,//場所
					威力計算(),//HP
					CHARACTER_FORSE_FRIEND,//Forse
					0,//speed
					弾幕::追尾,//弾幕Type
					弾幕::ミドリ,//弾幕色
					random()*8,//aspect
					NULLCHARA//targetenemy
					);

				bullet_count++;
				if(bullet_count >= bulletnum) {
					break;
				}
			}
		}
	}

	sg_pDungeonSystem->動的識別(me());
	return true;
}
int cBullet_ID_29::効果(pcLandform pland, int aspect)
{
	bool res = false;
	int width = 効果量(0);
	vector<pcLandform> land_list = sg_pDungeonSystem->横列地形列挙(pland, aspect, width);
	int size = land_list.size();
	for(int distance = 0; distance < size ;distance++) {

		pcCharacter pchara = 弾幕::弾幕召喚(
					land_list[distance],//場所
					威力計算(),//HP
					CHARACTER_FORSE_FRIEND,//Forse
					0,//speed
					弾幕::直進,//弾幕Type
					弾幕::ミドリ,//弾幕色
					aspect,//aspect
					NULLCHARA//targetenemy
					);
		res |= (pchara != NULLCHARA);
	}

	if(res) {
		sg_pDungeonSystem->動的識別(me());
	}

	return res;
}
int cBullet_ID_29::撃つ_効果_通常(pcCharacter pchara)
{
	pcLandform forwordLand = sg_pDungeonSystem->キャラ視点方角地形(pchara,0,1);

	本数消費();
	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("shoot.wav"),現在地形()->place);
	溜めエフェクト(pchara);
	
	return 効果(forwordLand, pchara->aspect);
}