#include "stdafx.h"
#include "Equipment1.h"
#include "GameIdiom.h"
#include "EffectFunctions.h"
#include "cDungeonSystem.h"
#include "cAttackinformation.h"
#include "MobAbilityIdiom.h"


bool cEquipment_能力仕様フラグID_exist(cValiableField& valiable, int ID)
{
	return valiable.doubles.exist((_T("cEquipment_能力仕様フラグID_") + setStyle(ID)).c_str());
}
double& cEquipment_能力仕様フラグID_dim(cValiableField& valiable, int ID)
{
	return valiable.doubles.dim((_T("cEquipment_能力仕様フラグID_") + setStyle(ID)).c_str());
}
double& cEquipment_能力仕様フラグID_val(cValiableField& valiable, int ID)
{
	return valiable.doubles.val((_T("cEquipment_能力仕様フラグID_") + setStyle(ID)).c_str());
}
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------

void cEquipment_ID_3::能力(const タイミング timing, cValiableField& valiable)
{

	if(timing == 主人公素振り_タイミング)
	{
		if(能力発動条件満たしている_攻撃用() && !cEquipment_能力仕様フラグID_exist(valiable,ID()))
		{
			cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
			if(!(装備者_攻撃用()->aspect % 2))
			{//斜めでない
				int res = sg_pDungeonSystem->穴掘り(sg_pDungeonSystem->キャラ前地形(装備者_攻撃用(),1),true);
				sg_pDungeonSystem->地形整理();
				
				if(res)
				{
					
					穴掘り_count()++;
					if(穴掘り_count() >= 穴掘り_max())
					{
						//効果音未実装



						map<tstring, StyleString> valiable;
						valiable[_T("Item")] = FullName();
						g_Langメッセージ(_T("cEquip_ID_3故障メッセージ"),valiable);
						sg_pDungeonSystem->落ち物破壊要請(me());
					}
				}
			}
		}
	}

}
void cEquipment_ID_3::DataBeginOptimize(int difficulty)
{
	cEquipment::DataBeginOptimize(difficulty);
	穴掘り_max() = random_range(効果量(0), 効果量(1));
}

//------------------------------------------------------------------------------
//びっくり傘
//------------------------------------------------------------------------------

void cEquipment_ID_4::能力(const タイミング timing, cValiableField& valiable)
{

	if(timing == 被攻撃直後時_タイミング)
	{
		if(能力発動条件満たしている_防御用() && !cEquipment_能力仕様フラグID_exist(valiable,ID())
			&& valiable.doubles.exist(変数_直接攻撃フラグ))
		{
			if(発動率() > random())
			{
				if(valiable.charas[変数_攻撃者] == NULL) return;
				cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
				sg_pDungeonSystem->びっくり要請( valiable.charas[変数_攻撃者] ,効果量(1));

			}
		}
	}
	if(timing == 攻撃直後時_タイミング)
	{
		if(能力発動条件満たしている_攻撃用() && !cEquipment_能力仕様フラグID_exist(valiable,ID())
			&& valiable.doubles.exist(変数_直接攻撃フラグ))
		{
			if(発動率() > random())
			{
				if(valiable.charas[変数_防御者] == NULL) return;
				cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
				sg_pDungeonSystem->びっくり要請( valiable.charas[変数_防御者] ,効果量(1));

			}
		}
	}
	else if(timing == 落ち物衝突直後時_タイミング)
	{
		if(valiable.charas[変数_防御者] == NULL) return;
		//valiable.doubles.dim(_T("cEquipment_ID_4能力仕様フラグ")) = 1;
		sg_pDungeonSystem->びっくり要請( valiable.charas[変数_防御者] ,効果量(1));
	}
}
double cEquipment_ID_4::発動率()
{
	return 効果量(0)/100.0;
}
double cEquipment_ID_4::効果ターン()
{
	return 効果量(1);
}
//------------------------------------------------------------------------------
//白楼剣
//------------------------------------------------------------------------------
void cEquipment_ID_5::能力(const タイミング timing, cValiableField& valiable)
{

	if(timing == ダメージ計算攻撃時_タイミング)
	{
		if(能力発動条件満たしている_攻撃用() && !cEquipment_能力仕様フラグID_exist(valiable,ID())
			&& valiable.doubles.exist(変数_直接攻撃フラグ))
		{
			if(発動率() > random())
			{
				if(valiable.charas[変数_防御者] == NULL) return;
				if(GameIdiom::異常状態である(valiable.charas[変数_防御者])) {
					cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
					GameIdiom::全異常状態治療要請( valiable.charas[変数_防御者]);
					valiable.doubles[変数_耐性ボーナス_倍率％] -= 効果量(1);
				}
			}
		}
	}
	else if(timing == 落ち物衝突直後時_タイミング)
	{
		if(valiable.charas[変数_防御者] == NULL) return;
		cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
		GameIdiom::全異常状態治療要請( valiable.charas[変数_防御者]);
	}
}
double cEquipment_ID_5::発動率()
{
	return 効果量(0)/100.0;
}
//------------------------------------------------------------------------------
//大鎌
//------------------------------------------------------------------------------
void cEquipment_ID_8::能力(const タイミング timing, cValiableField& valiable)
{

	if(timing == 装備品衝突消滅直前時_タイミング)
	{
		pcEquipment pEqu = boost::dynamic_pointer_cast<cEquipment>(拠り代());
		if(pEqu == NULL) return;
		valiable.doubles.dim(変数_汎用ブール) = 0;//破壊フラグ
		sg_pDungeonSystem->アイテム劣化(pEqu);
		cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
		if(pEqu->quality() + pEqu->武器力基礎値() <= 0) sg_pDungeonSystem->落ち物破壊要請(me());
	}
}
//------------------------------------------------------------------------------
//妖扇
//------------------------------------------------------------------------------
void cEquipment_ID_9::能力(const タイミング timing, cValiableField& valiable)
{

	if(timing == 装備品防具力基礎値計算時_タイミング)
	{
		cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
		valiable.doubles.val(変数_防具力基礎値ボーナス_定数) += 効果量(1);
	}
	if(timing == 装備品武器力基礎値計算時_タイミング)
	{
		cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
		valiable.doubles.val(変数_武器力基礎値ボーナス_定数) += 効果量(1);
	}
	if(timing == 自然満腹度減少量計算時_タイミング)
	{
		cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
		valiable.doubles.val(変数_汎用ボーナス_倍率) += 効果量(0)/100.0;
	}
}
//------------------------------------------------------------------------------
//ヤタノ鏡
//------------------------------------------------------------------------------
void cEquipment_ID_10::能力(const タイミング timing, cValiableField& valiable)
{

	if(timing == 被攻撃接近直前_タイミング)
	{
		if(能力発動条件満たしている_防御用() && !cEquipment_能力仕様フラグID_exist(valiable,ID()))
		{
			if(valiable.intsets[変数_属性].count(攻撃属性::必中)) return;
			pcCharacter penemy = valiable.charas[変数_攻撃者];
			if(penemy == NULL) return;
			if(!sg_pDungeonSystem->キャラクター敵対判定(装備者_防御用(),penemy)) return;
			if((装備者_防御用()->足元地形()->place - penemy->足元地形()->place).dif() > 1) return;

			if(効果量(0)/100.0 > random())
			{

				set<攻撃属性::攻撃属性> temp;

				sg_pDungeonSystem->攻撃接近(攻撃作成(
					装備者_防御用(),//攻撃者
					penemy->足元地形(),//場所
					valiable.doubles[変数_攻撃力],//攻撃力
					valiable.doubles[変数_定数ダメージフラグ] + valiable.doubles.exist(変数_割合ダメージフラグ)*2,//定数ダメージ
					属性設定back(valiable.intsets[変数_属性]),
					false));//必中

				valiable.doubles[変数_回避力] += 100;

			
				cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;


			}
		}
	}
}
//------------------------------------------------------------------------------
//ヤサカニノ勾玉
//------------------------------------------------------------------------------
void cEquipment_ID_11::能力(const タイミング timing, cValiableField& valiable)
{

	if(timing == アイテム命中判定時_タイミング)
	{
		if(能力発動条件満たしている_防御用() && !cEquipment_能力仕様フラグID_exist(valiable,ID()))
		{

			valiable.doubles[変数_回避力] += 効果量(0);

			cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
		
		}
	}
}

//------------------------------------------------------------------------------
//イナバの杵
//------------------------------------------------------------------------------
void cEquipment_ID_14::能力(const タイミング timing, cValiableField& valiable)
{

	if(timing == 主人公素振り_タイミング)
	{
		if(能力発動条件満たしている_攻撃用() && !cEquipment_能力仕様フラグID_exist(valiable,ID()))
		{
			if(!(valiable.doubles.val(変数_汎用不変定数)))//キャラいない
			{
				pcDroping pdrop = sg_pDungeonSystem->キャラ前地形(装備者_攻撃用(),1)->pOnDrop;

				sg_pDungeonSystem->落ち物破壊要請(pdrop);
				
				if(pdrop != NULL && pdrop->ID() == 7017)
				{//宝箱
					sg_pDungeonSystem->落ち物破壊要請(me());
				}
			}
		}
	}
}
//------------------------------------------------------------------------------
//スノートライデント
//------------------------------------------------------------------------------
void cEquipment_ID_16::能力(const タイミング timing, cValiableField& valiable)
{

	if(timing == 攻撃壁貫通判定_タイミング)
	{
		if(能力発動条件満たしている_攻撃用() && !cEquipment_能力仕様フラグID_exist(valiable,ID()))
		{
			valiable.doubles.val(変数_汎用ブール) = 1;

			cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
		}
	}
}

//------------------------------------------------------------------------------
//漆黒の大カミソリ
//------------------------------------------------------------------------------
void cEquipment_ID_17::能力(const タイミング timing, cValiableField& valiable)
{

	if(timing == 攻撃直後時_タイミング)
	{
		if(能力発動条件満たしている_攻撃用() && !cEquipment_能力仕様フラグID_exist(valiable,ID())
			&& valiable.doubles.exist(変数_直接攻撃フラグ))
		{
			if(効果量(0)/100.0 > random())
			{
				if(valiable.charas[変数_防御者] == NULL) return;

				sg_pDungeonSystem->即死要請(valiable.charas[変数_防御者]);

				cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;

			}
		}
	}
}



//------------------------------------------------------------------------------
//鬼切
//------------------------------------------------------------------------------
void cEquipment_ID_18::能力(const タイミング timing, cValiableField& valiable)
{

	if(timing == ダメージ計算攻撃時_タイミング)
	{
		if(能力発動条件満たしている_攻撃用()
			&& valiable.doubles.exist(変数_直接攻撃フラグ))
		{
			if(valiable.charas.dim(変数_防御者) != NULL 
				&& valiable.charas.dim(変数_防御者)->CharaAttribute().count(キャラ属性::鬼))
			{
				if(!cEquipment_能力仕様フラグID_exist(valiable,ID()))
				{//初
					valiable.doubles[変数_耐性ボーナス_倍率％] -= 効果量(0);
					cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
				}
				else
				{
					valiable.doubles[変数_耐性ボーナス_倍率％] -= 効果量(1);	
				
				}
			}
			
		}
	}
	else if(timing == 装備品衝突直前時_タイミング)
	{
		if(valiable.charas.dim(変数_防御者) != NULL 
			&& valiable.charas.dim(変数_防御者)->CharaAttribute().count(キャラ属性::鬼))
		{
			if(!cEquipment_能力仕様フラグID_exist(valiable,ID()))
			{//初
				valiable.doubles[変数_耐性ボーナス_倍率％] -= 効果量(0);
				cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
			}
			else
			{
				valiable.doubles[変数_耐性ボーナス_倍率％] -= 効果量(1);	
			
			}
		}
	}
}

//------------------------------------------------------------------------------
//神切
//------------------------------------------------------------------------------
void cEquipment_ID_19::能力(const タイミング timing, cValiableField& valiable)
{

	if(timing == ダメージ計算攻撃時_タイミング)
	{
		if(能力発動条件満たしている_攻撃用()
			&& valiable.doubles.exist(変数_直接攻撃フラグ))
		{
			if(valiable.charas.dim(変数_防御者) != NULL 
				&& valiable.charas.dim(変数_防御者)->CharaAttribute().count(キャラ属性::神))
			{
				if(!cEquipment_能力仕様フラグID_exist(valiable,ID()))
				{//初
					valiable.doubles[変数_耐性ボーナス_倍率％] -= 効果量(0);
					cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
				}
				else
				{
					valiable.doubles[変数_耐性ボーナス_倍率％] -= 効果量(1);	
				
				}
			}
			
		}
	}
	else if(timing == 装備品衝突直前時_タイミング)
	{
		if(valiable.charas.dim(変数_防御者) != NULL 
			&& valiable.charas.dim(変数_防御者)->CharaAttribute().count(キャラ属性::神))
		{
			if(!cEquipment_能力仕様フラグID_exist(valiable,ID()))
			{//初
				valiable.doubles[変数_耐性ボーナス_倍率％] -= 効果量(0);
				cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
			}
			else
			{
				valiable.doubles[変数_耐性ボーナス_倍率％] -= 効果量(1);	
			
			}
		}
	}
}

//------------------------------------------------------------------------------
//人切
//------------------------------------------------------------------------------
void cEquipment_ID_20::能力(const タイミング timing, cValiableField& valiable)
{

	if(timing == ダメージ計算攻撃時_タイミング)
	{
		if(能力発動条件満たしている_攻撃用()
			&& valiable.doubles.exist(変数_直接攻撃フラグ))
		{
			if(valiable.charas.dim(変数_防御者) != NULL 
				&& valiable.charas.dim(変数_防御者)->CharaAttribute().count(キャラ属性::人間))
			{
				if(!cEquipment_能力仕様フラグID_exist(valiable,ID()))
				{//初
					valiable.doubles[変数_耐性ボーナス_倍率％] -= 効果量(0);
					cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
				}
				else
				{
					valiable.doubles[変数_耐性ボーナス_倍率％] -= 効果量(1);	
				
				}
			}
			
		}
	}
	else if(timing == 装備品衝突直前時_タイミング)
	{
		if(valiable.charas.dim(変数_防御者) != NULL 
			&& valiable.charas.dim(変数_防御者)->CharaAttribute().count(キャラ属性::人間))
		{
			if(!cEquipment_能力仕様フラグID_exist(valiable,ID()))
			{//初
				valiable.doubles[変数_耐性ボーナス_倍率％] -= 効果量(0);
				cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
			}
			else
			{
				valiable.doubles[変数_耐性ボーナス_倍率％] -= 効果量(1);	
			
			}
		}
	}
}

//------------------------------------------------------------------------------
//妖切
//------------------------------------------------------------------------------
void cEquipment_ID_21::能力(const タイミング timing, cValiableField& valiable)
{

	if(timing == ダメージ計算攻撃時_タイミング)
	{
		if(能力発動条件満たしている_攻撃用()
			&& valiable.doubles.exist(変数_直接攻撃フラグ))
		{
			if(valiable.charas.dim(変数_防御者) != NULL 
				&& valiable.charas.dim(変数_防御者)->CharaAttribute().count(キャラ属性::妖怪))
			{
				if(!cEquipment_能力仕様フラグID_exist(valiable,ID()))
				{//初
					valiable.doubles[変数_耐性ボーナス_倍率％] -= 効果量(0);
					cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
				}
				else
				{
					valiable.doubles[変数_耐性ボーナス_倍率％] -= 効果量(1);	
				
				}
			}
			
		}
	}
	else if(timing == 装備品衝突直前時_タイミング)
	{
		if(valiable.charas.dim(変数_防御者) != NULL 
			&& valiable.charas.dim(変数_防御者)->CharaAttribute().count(キャラ属性::妖怪))
		{
			if(!cEquipment_能力仕様フラグID_exist(valiable,ID()))
			{//初
				valiable.doubles[変数_耐性ボーナス_倍率％] -= 効果量(0);
				cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
			}
			else
			{
				valiable.doubles[変数_耐性ボーナス_倍率％] -= 効果量(1);	
			
			}
		}
	}
}

//------------------------------------------------------------------------------
//妖精切
//------------------------------------------------------------------------------
void cEquipment_ID_22::能力(const タイミング timing, cValiableField& valiable)
{

	if(timing == ダメージ計算攻撃時_タイミング)
	{
		if(能力発動条件満たしている_攻撃用()
			&& valiable.doubles.exist(変数_直接攻撃フラグ))
		{
			if(valiable.charas.dim(変数_防御者) != NULL 
				&& valiable.charas.dim(変数_防御者)->CharaAttribute().count(キャラ属性::妖精))
			{
				if(!cEquipment_能力仕様フラグID_exist(valiable,ID()))
				{//初
					valiable.doubles[変数_耐性ボーナス_倍率％] -= 効果量(0);
					cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
				}
				else
				{
					valiable.doubles[変数_耐性ボーナス_倍率％] -= 効果量(1);	
				
				}
			}
			
		}
	}
	else if(timing == 装備品衝突直前時_タイミング)
	{
		if(valiable.charas.dim(変数_防御者) != NULL 
			&& valiable.charas.dim(変数_防御者)->CharaAttribute().count(キャラ属性::妖精))
		{
			if(!cEquipment_能力仕様フラグID_exist(valiable,ID()))
			{//初
				valiable.doubles[変数_耐性ボーナス_倍率％] -= 効果量(0);
				cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
			}
			else
			{
				valiable.doubles[変数_耐性ボーナス_倍率％] -= 効果量(1);	
			
			}
		}
	}
}
//------------------------------------------------------------------------------
//磁器刀
//------------------------------------------------------------------------------
void cEquipment_ID_24::能力(const タイミング timing, cValiableField& valiable)
{

	if(timing == 攻撃直後時_タイミング)
	{
		if(能力発動条件満たしている_攻撃用()
			&& valiable.doubles.exist(変数_直接攻撃フラグ))
		{
	
			if(!cEquipment_能力仕様フラグID_exist(valiable,ID()))
			{//初
				sg_pDungeonSystem->アイテム劣化(me(),0);
				cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
			}
		
		}
	}
}
//------------------------------------------------------------------------------
//天狗の盾
//------------------------------------------------------------------------------
void cEquipment_ID_26::能力(const タイミング timing, cValiableField& valiable)
{

	if(timing == ダメージ計算防御時_タイミング)
	{
		if(能力発動条件満たしている_防御用())
		{
			/*
			pcCharacter penemy = valiable.charas[変数_攻撃者];
			if(penemy == NULL) return;
			if((装備者_防御用()->足元地形()->place - penemy->足元地形()->place).dif() <= 1) return;

			if(cEquipment_能力仕様フラグID_exist(valiable,ID()))
			{
				valiable.doubles[変数_耐性ボーナス_倍率％] += 効果量(1);
			}
			else
			{
				valiable.doubles[変数_耐性ボーナス_倍率％] += 効果量(0);
				cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
			}
			*/
			if(valiable.doubles.exist(変数_方向))
			{
				int as = safeAspect( valiable.doubles.val(変数_方向) - 装備者_防御用()->aspect);
				if(as >= 3 && as <= 5)
				{
					if(cEquipment_能力仕様フラグID_exist(valiable,ID()))
					{
						cEquipment_能力仕様フラグID_val(valiable,ID()) += 1;
					}
					else
					{
						cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
					}

					if(cEquipment_能力仕様フラグID_val(valiable,ID()) <= 1)
					{
						valiable.doubles[変数_耐性ボーナス_倍率％] += 効果量(0);
					}
					else
					{
						double power = pow(効果量(0)/100.0 , cEquipment_能力仕様フラグID_val(valiable,ID()) - 2);
						valiable.doubles[変数_耐性ボーナス_倍率％] += 効果量(1)*power;
					}
				
				}
				else
				{
					valiable.doubles.dim(変数_耐性ボーナス_倍率％) -= 効果量(2);

					if(!cEquipment_能力仕様フラグID_exist(valiable,ID()))
					{
						valiable.doubles.dim(変数_防御力ボーナス_倍率) -= 1.0;
						cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
					}
				}
			}
			

		}
	}
}
//------------------------------------------------------------------------------
//天狗団扇
//------------------------------------------------------------------------------
void cEquipment_ID_27::能力(const タイミング timing, cValiableField& valiable)
{

	if(timing == 攻撃直後時_タイミング)
	{
		if(能力発動条件満たしている_攻撃用()
			&& valiable.doubles.exist(変数_直接攻撃フラグ))
		{
			if((効果量(0)/100.0) > random())
			{
				if(!cEquipment_能力仕様フラグID_exist(valiable,ID()))
				{//初
					sg_pDungeonSystem->吹き飛ばし要請(装備者_攻撃用(),valiable.charas.dim(変数_防御者),装備者_攻撃用()->aspect,効果量(1),効果量(2));


					cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
				}
			}
		}
	}
	else if(timing == 装備品衝突直前時_タイミング)
	{
		if(!cEquipment_能力仕様フラグID_exist(valiable,ID()))
		{//初
			sg_pDungeonSystem->吹き飛ばし要請(投擲者(),valiable.charas.dim(変数_防御者),aspect,効果量(1),効果量(2));


			cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
		}
	}
}
//------------------------------------------------------------------------------
//のびーるアーム
//------------------------------------------------------------------------------
void cEquipment_ID_28::能力(const タイミング timing, cValiableField& valiable)
{

	if(timing == 直接攻撃位置指定_タイミング)
	{
		if(能力発動条件満たしている_攻撃用())
		{
			pcCharacter pchara = 装備者_攻撃用();
			if(pchara == NULL) return;

			int i,distance = 効果量(0);
			for(i=1;i<=distance;i++)
			{
				if( sg_pDungeonSystem->isCanNeighberAttack(pchara, pchara->aspect,i,pchara->通常攻撃_壁斜めすり抜け(),pchara->通常攻撃_壁すり抜け()))
				{
					pcCharacter penemy = sg_pDungeonSystem->キャラ前地形(pchara,i)->pOnChar;
					if(penemy == NULL) continue;
					if(sg_pDungeonSystem->キャラクター敵対判定(pchara,penemy) 
						)
					{
						valiable.doubles[変数_距離] = i;
						return;
					}
					else
					{
						valiable.doubles[変数_距離] = max(1,i-1);
						return;
					}			
				}
				else
				{
					valiable.doubles[変数_距離] = max(1,i-1);
					return;
				}

			}

			valiable.doubles[変数_距離] = distance;
		}
	}
	else if(timing == 通常攻撃_壁斜めすり抜け判定_タイミング)
	{
		if(能力発動条件満たしている_攻撃用())
		{
			valiable.doubles[変数_汎用ブール] = 1;
		}
	}
	else if(timing == 防御力計算時_タイミング)
	{
		if(能力発動条件満たしている_攻撃用())
		{
			valiable.doubles[変数_防御力ボーナス_倍率] -= 効果量(1)/100.0;
		}
	}
	else if(timing == 直接攻撃命中力計算時_タイミング)
	{
		if(能力発動条件満たしている_攻撃用())
		{
			valiable.doubles[変数_命中力] -= 効果量(2);
		}
	}
}

//------------------------------------------------------------------------------
//卒塔婆
//------------------------------------------------------------------------------
void cEquipment_ID_29::能力(const タイミング timing, cValiableField& valiable)
{

	if(timing == ダメージ計算攻撃時_タイミング)
	{
		if(能力発動条件満たしている_攻撃用()
			&& valiable.doubles.exist(変数_直接攻撃フラグ))
		{
			if(valiable.charas.dim(変数_防御者) != NULL 
				&& valiable.charas.dim(変数_防御者)->CharaAttribute().count(キャラ属性::幽霊))
			{
				if(!cEquipment_能力仕様フラグID_exist(valiable,ID()))
				{//初
					valiable.doubles[変数_耐性ボーナス_倍率％] -= 効果量(0);
					cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
				}
				else
				{
					valiable.doubles[変数_耐性ボーナス_倍率％] -= 効果量(1);	
				
				}
			}
			
		}
	}
	else if(timing == 装備品衝突直前時_タイミング)
	{
		if(valiable.charas.dim(変数_防御者) != NULL 
			&& valiable.charas.dim(変数_防御者)->CharaAttribute().count(キャラ属性::幽霊))
		{
			if(!cEquipment_能力仕様フラグID_exist(valiable,ID()))
			{//初
				valiable.doubles[変数_耐性ボーナス_倍率％] -= 効果量(0);
				cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
			}
			else
			{
				valiable.doubles[変数_耐性ボーナス_倍率％] -= 効果量(1);	
			
			}
		}
	}
}
//------------------------------------------------------------------------------
//レーヴァテイン
//------------------------------------------------------------------------------
void cEquipment_ID_30::能力(const タイミング timing, cValiableField& valiable)
{

	if(timing == 攻撃直後時_タイミング && !valiable.intsets[変数_属性].count(攻撃属性::落ち物))
	{
		if(能力発動条件満たしている_攻撃用()
			&& valiable.doubles.exist(変数_直接攻撃フラグ))
		{
			GameIdiom::火柱召喚(装備者_攻撃用(),sg_pDungeonSystem->キャラ前地形(装備者_攻撃用(),1),効果量(0));
		}
	}
}
//------------------------------------------------------------------------------
//悔悟の棒
//------------------------------------------------------------------------------
void cEquipment_ID_31::能力(const タイミング timing, cValiableField& valiable)
{

	if(timing == 攻撃力計算時_タイミング)
	{
		if(能力発動条件満たしている_攻撃用()
			&& valiable.doubles.exist(変数_直接攻撃フラグ))
		{

			int damage = 装備者_攻撃用()->MHP - 装備者_攻撃用()->HP;
			valiable.doubles[変数_攻撃力ボーナス_定数] += 効果量(0)*damage/100.0;

		}
	}
	else if(timing == 防御力計算時_タイミング)
	{
		if(能力発動条件満たしている_防御用())
		{

			int damage = 装備者_防御用()->MHP - 装備者_防御用()->HP;
			valiable.doubles[変数_防御力ボーナス_定数] += 効果量(1)*damage/100.0;

		}
	}
}
//------------------------------------------------------------------------------
//浄玻璃
//------------------------------------------------------------------------------
void cEquipment_ID_32::能力(const タイミング timing, cValiableField& valiable)
{
	if(timing == 被攻撃直後時_タイミング)
	{
		if(能力発動条件満たしている_防御用())
		{

			pcCharacter penemy = valiable.charas[変数_攻撃者];
			if(penemy == NULL) return;
			if(!sg_pDungeonSystem->キャラクター敵対判定(装備者_防御用(),penemy)) return;
			if((装備者_防御用()->足元地形()->place - penemy->足元地形()->place).dif() > 1) return;


			set<攻撃属性::攻撃属性> temp;

			sg_pDungeonSystem->攻撃接近(攻撃作成(
				装備者_防御用(),//攻撃者
				penemy->足元地形(),//場所
				valiable.doubles[変数_ダメージ]*効果量(0)/100.0,//攻撃力
				1,//定数ダメージ
				属性設定back(valiable.intsets[変数_属性]),
				true));//必中
			
		}
	}
}
//------------------------------------------------------------------------------
//お払い棒
//------------------------------------------------------------------------------
void cEquipment_ID_33::能力(const タイミング timing, cValiableField& valiable)
{
	if(timing == ダメージ計算攻撃時_タイミング)
	{
		if(能力発動条件満たしている_攻撃用()
			&& valiable.doubles.exist(変数_直接攻撃フラグ))
		{
			if(装備者_攻撃用()->defenseequipment != NULL 
				&& 装備者_攻撃用()->defenseequipment->ID() == 拠り代()->ID()
				&& 装備者_攻撃用()->defenseequipment != 拠り代()
				)
			{
				valiable.doubles.val(変数_攻撃力ボーナス_倍率) += 効果量(0)/100.0;
			}
		}
	}
}
//------------------------------------------------------------------------------
//フラワーパラソル
//------------------------------------------------------------------------------
void cEquipment_ID_35::能力(const タイミング timing, cValiableField& valiable)
{
	if(timing == ダメージ計算攻撃時_タイミング)
	{
		if(能力発動条件満たしている_攻撃用()
			&& valiable.doubles.exist(変数_直接攻撃フラグ))
		{
			if(valiable.charas.val(変数_防御者) != NULL)
			{
				if(valiable.charas.val(変数_防御者)->LV > 装備者_攻撃用()->LV)
				{
					valiable.doubles[変数_攻撃力ボーナス_倍率] += 効果量(0)/100.0;
				}
				else
				{
					valiable.doubles[変数_攻撃力ボーナス_倍率] -= 効果量(1)/100.0;
				}

			}
			
		}
	}
}
//------------------------------------------------------------------------------
//鎖分銅
//------------------------------------------------------------------------------
void cEquipment_ID_37::能力(const タイミング timing, cValiableField& valiable)
{
	if(timing == 攻撃直後時_タイミング)
	{
		if(能力発動条件満たしている_攻撃用()
			&& valiable.doubles.exist(変数_直接攻撃フラグ))
		{
			if(効果量(0)/100.0 > random())
			{
				if(valiable.charas.dim(変数_防御者) != NULL)
				{
					sg_pDungeonSystem->脱力要請(valiable.charas.dim(変数_防御者),効果量(1),効果量(2));

				}
			}
		}
	}
}
//------------------------------------------------------------------------------
//ミニ八卦炉
//------------------------------------------------------------------------------
void cEquipment_ID_39::能力(const タイミング timing, cValiableField& valiable)
{
	if(timing == ダメージ計算防御時_タイミング)
	{
		if(能力発動条件満たしている_防御用())
		{
			if(valiable.intsets.val(変数_属性).count(攻撃属性::火))
			{
				if(!cEquipment_能力仕様フラグID_exist(valiable,ID()))
				{//初
					valiable.doubles.val(変数_耐性ボーナス_倍率％) += 効果量(0);
					cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
				}
				else
				{
					valiable.doubles.val(変数_耐性ボーナス_倍率％) += 効果量(1);
				
				}
				
			}
		}
	}
	if(timing == ダメージ計算攻撃時_タイミング)
	{
		if(能力発動条件満たしている_攻撃用())
		{
			攻撃属性::攻撃属性 attackAttri = 攻撃属性::火;
			if(valiable.doubles.exist(変数_直接攻撃フラグ)) {
				valiable.intsets.val(変数_属性).insert(attackAttri);
			}
			else {
				set<int> targetAttackAttri;
				targetAttackAttri.insert((int)attackAttri);
				MobAbilityIdiom::属性攻撃ボーナス％(targetAttackAttri,効果量(2))(装備者_攻撃用(), timing, valiable);
			}
		}
	}

}
//------------------------------------------------------------------------------
//バトン
//------------------------------------------------------------------------------
void cEquipment_ID_40::能力(const タイミング timing, cValiableField& valiable)
{
	if(timing == ダメージ計算防御時_タイミング)
	{
		if(能力発動条件満たしている_防御用() || 能力発動条件満たしている_攻撃用())
		{
			if(valiable.doubles.exist(変数_方向))
			{
				int as = safeAspect( valiable.doubles.val(変数_方向) - 装備者_防御用()->aspect);
				if(as < 3 || as > 5)
				{
					if(cEquipment_能力仕様フラグID_exist(valiable,ID()))
					{
						cEquipment_能力仕様フラグID_val(valiable,ID()) += 1;
					}
					else
					{
						cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
					}

					if(cEquipment_能力仕様フラグID_val(valiable,ID()) <= 1)
					{
						valiable.doubles[変数_耐性ボーナス_倍率％] += 効果量(0);
					}
					else
					{
						double power = pow(効果量(0)/100.0 , cEquipment_能力仕様フラグID_val(valiable,ID()) - 2);
						valiable.doubles[変数_耐性ボーナス_倍率％] += 効果量(1)*power;
					}
				
				}
			}
		}
	}

}
//------------------------------------------------------------------------------
//警策
//------------------------------------------------------------------------------
void cEquipment_ID_41::能力(const タイミング timing, cValiableField& valiable)
{
	if(timing == 攻撃直後時_タイミング)
	{
		if(能力発動条件満たしている_攻撃用()
			&& valiable.doubles.exist(変数_直接攻撃フラグ))
		{
			if(効果量(0)/100.0 > random())
			{
				if(valiable.charas.dim(変数_防御者) != NULL)
				{
					sg_pDungeonSystem->頑強要請(valiable.charas.dim(変数_防御者),効果量(1),効果量(2));

				}
			}
		}
	}
	if(timing == 被攻撃直後時_タイミング)
	{
		if(能力発動条件満たしている_防御用()
			&& valiable.doubles.exist(変数_直接攻撃フラグ))
		{
			if(効果量(0)/100.0 > random())
			{
				if(valiable.charas.dim(変数_防御者) != NULL)
				{
					sg_pDungeonSystem->頑強要請(valiable.charas.dim(変数_防御者),効果量(1),効果量(2));

				}
			}
		}
	}
}
//------------------------------------------------------------------------------
//分厚い本
//------------------------------------------------------------------------------
void cEquipment_ID_43::能力(const タイミング timing, cValiableField& valiable)
{
	if(timing == 脱力追加直前_タイミング)
	{
		if(能力発動条件満たしている_攻撃用())
		{
			valiable.doubles.val(変数_汎用ブール) = 0;
			map<tstring, StyleString> val;
			val[_T("Item")] = 拠り代()->FullName();
			g_Langメッセージ(_T("cEquip_ID_43能力メッセージ"),val);
			valiable.doubles.val(変数_汎用ブール) = 0;
		}
	}
	else if(timing == 軟弱追加直前_タイミング)
	{
		if(能力発動条件満たしている_防御用())
		{
			valiable.doubles.val(変数_汎用ブール) = 0;
			map<tstring, StyleString> val;
			val[_T("Item")] = 拠り代()->FullName();
			g_Langメッセージ(_T("cEquip_ID_43能力メッセージ"),val);
			valiable.doubles.val(変数_汎用ブール) = 0;
		}
	}
}
//------------------------------------------------------------------------------
//猫車
//------------------------------------------------------------------------------
void cEquipment_ID_44::能力(const タイミング timing, cValiableField& valiable)
{
	if(timing == アイテム所持数計算時_タイミング)
	{
		if(能力発動条件満たしている_攻撃用())
		{
			valiable.doubles.dim(変数_汎用実数) += 効果量(0);
		}
	}
}

//------------------------------------------------------------------------------
//宝塔
//------------------------------------------------------------------------------
void cEquipment_ID_45::能力(const タイミング timing, cValiableField& valiable)
{
	if(timing != 能力発動条件満たしている_攻撃用_タイミング && timing != 能力発動条件満たしている_防御用_タイミング)
	{
		if(能力発動条件満たしている_防御用())
		{
			if(!cEquipment_能力仕様フラグID_exist(valiable,ID()))
			{//初
				if(MobAbilityIdiom::超遠距離耐性ボーナスCutIn(効果量(0))(装備者(), timing, valiable))
				{
					cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
				}
			}
			else
			{
				MobAbilityIdiom::超遠距離耐性ボーナスCutIn(効果量(1))(装備者(), timing, valiable);
			}
		}

		if(能力発動条件満たしている_攻撃用()) {
			MobAbilityIdiom::超遠距離攻撃力ボーナスCutIn(効果量(2),0)(装備者(), timing, valiable);
		}
	}
}

//------------------------------------------------------------------------------
//イカリ
//------------------------------------------------------------------------------
void cEquipment_ID_46::能力(const タイミング timing, cValiableField& valiable)
{
	if(timing == 吹き飛ばし判定時_タイミング
		|| timing == 引き寄せ判定時_タイミング
		|| timing == 投げ飛ばし判定時_タイミング
		)
	{
		if(能力発動条件満たしている_攻撃用() || 能力発動条件満たしている_防御用())
		{
			valiable.doubles.val(変数_汎用ブール) = 0;
			map<tstring, StyleString> valiable;
			valiable[_T("Item")] = 拠り代()->FullName();
			g_Langメッセージ(_T("cEquip_ID_46能力メッセージ"),valiable);
		}
	}
}
//------------------------------------------------------------------------------
//大盃
//------------------------------------------------------------------------------
void cEquipment_ID_47::能力(const タイミング timing, cValiableField& valiable)
{
	if(timing == 経験値獲得時_タイミング
		)
	{
		if(能力発動条件満たしている_攻撃用() || 能力発動条件満たしている_防御用())
		{
			valiable.doubles.dim(変数_汎用ボーナス_倍率) += 効果量(0)/100.0;
			
		}
	}
	else if(timing == 防御力計算時_タイミング
		)
	{
		if(能力発動条件満たしている_攻撃用() || 能力発動条件満たしている_防御用())
		{
			valiable.doubles.dim(変数_防御力ボーナス_倍率) -= 効果量(1)/100.0;
			
		}
	}
	
}
//------------------------------------------------------------------------------
//エア巻物
//------------------------------------------------------------------------------
void cEquipment_ID_48::能力(const タイミング timing, cValiableField& valiable)
{
	if(timing == 魔法接近直前_タイミング
		)
	{
		if( 能力発動条件満たしている_防御用() && !cEquipment_能力仕様フラグID_exist(valiable,ID()))
		{
			if(!valiable.drops.val(変数_対象落ち物)->跳ね返し無効フラグ)
			{
				cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
				//跳ね返し
				valiable.doubles.val(変数_汎用ブール) = 0;//効果発揮フラグ
				valiable.drops.val(変数_対象落ち物)->跳ね返し無効フラグ = 1;
				sg_pDungeonSystem->方向転換要請(装備者_防御用(), valiable.doubles.val(変数_方向)+4);
				sg_pDungeonSystem->魔法発射要請(装備者_防御用(), valiable.drops.val(変数_対象落ち物));
			}
		}
	}

}
//------------------------------------------------------------------------------
//制御棒
//------------------------------------------------------------------------------
void cEquipment_ID_49::能力(const タイミング timing, cValiableField& valiable)
{
	if(timing != 能力発動条件満たしている_攻撃用_タイミング && timing != 能力発動条件満たしている_防御用_タイミング)
	{
		if(能力発動条件満たしている_防御用())
		{
			if(!cEquipment_能力仕様フラグID_exist(valiable,ID()))
			{//初
				if(MobAbilityIdiom::定数攻撃耐性ボーナスCutIn(効果量(0))(装備者_防御用(), timing, valiable)) {
					cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
				}
			}
			else
			{
				MobAbilityIdiom::定数攻撃耐性ボーナスCutIn(効果量(1))(装備者_防御用(), timing, valiable);
			}
		}

		if(能力発動条件満たしている_攻撃用())
		{
			MobAbilityIdiom::定数攻撃攻撃力ボーナスCutIn(効果量(2),0)(装備者_攻撃用(), timing, valiable);
		}
	}
}
//------------------------------------------------------------------------------
//氷柱
//------------------------------------------------------------------------------
void cEquipment_ID_50::能力(const タイミング timing, cValiableField& valiable)
{
	if(timing == ダメージ計算防御時_タイミング)
	{
		if(能力発動条件満たしている_防御用())
		{
			if(valiable.intsets.val(変数_属性).count(攻撃属性::冷気))
			{
				if(!cEquipment_能力仕様フラグID_exist(valiable,ID()))
				{//初
					valiable.doubles.val(変数_耐性ボーナス_倍率％) += 効果量(0);
					cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
				}
				else
				{
					valiable.doubles.val(変数_耐性ボーナス_倍率％) += 効果量(1);
				
				}
				
			}
		}
	}
	if(timing == ダメージ計算攻撃時_タイミング)
	{
		if(能力発動条件満たしている_攻撃用())
		{
			攻撃属性::攻撃属性 attackAttri = 攻撃属性::冷気;
			if(valiable.doubles.exist(変数_直接攻撃フラグ)) {
				valiable.intsets.val(変数_属性).insert(attackAttri);
			}
			else {
				set<int> targetAttackAttri;
				targetAttackAttri.insert((int)attackAttri);
				MobAbilityIdiom::属性攻撃ボーナス％(targetAttackAttri,効果量(2))(装備者_攻撃用(), timing, valiable);
			}
		}
	}
}
//------------------------------------------------------------------------------
//道路標識
//------------------------------------------------------------------------------
void cEquipment_ID_51::能力(const タイミング timing, cValiableField& valiable)
{

	if(timing == 被攻撃直後時_タイミング)
	{
		if(能力発動条件満たしている_防御用() && !cEquipment_能力仕様フラグID_exist(valiable,ID())
			&& valiable.doubles.exist(変数_直接攻撃フラグ))
		{
			if(効果量(0)/100.0 > random())
			{
				if(valiable.charas[変数_攻撃者] == NULL) return;
				cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
				sg_pDungeonSystem->封印要請( valiable.charas[変数_攻撃者] ,効果量(1));

			}
		}
	}
	if(timing == 攻撃直後時_タイミング)
	{
		if(能力発動条件満たしている_攻撃用() && !cEquipment_能力仕様フラグID_exist(valiable,ID())
			&& valiable.doubles.exist(変数_直接攻撃フラグ))
		{
			if(効果量(0)/100.0 > random())
			{
				if(valiable.charas[変数_防御者] == NULL) return;
				cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
				sg_pDungeonSystem->封印要請( valiable.charas[変数_防御者] ,効果量(1));

			}
		}
	}
	else if(timing == 落ち物衝突直後時_タイミング)
	{
		if(valiable.charas[変数_防御者] == NULL) return;
		//valiable.doubles.dim(_T("cEquipment_ID_4能力仕様フラグ")) = 1;
		sg_pDungeonSystem->封印要請( valiable.charas[変数_防御者] ,効果量(1));
	}
}
//------------------------------------------------------------------------------
//要石
//------------------------------------------------------------------------------
void cEquipment_ID_52::能力(const タイミング timing, cValiableField& valiable)
{

	if(timing == 通常移動直前_タイミング)
	{
		if(能力発動条件満たしている_攻撃用())
		{
			sg_pDungeonSystem->強制ダメージ要請(装備者_攻撃用(),装備者_攻撃用()->HP*効果量(0)/100.0,false,false);
		}
		else if(能力発動条件満たしている_防御用())
		{
			sg_pDungeonSystem->強制ダメージ要請(装備者_防御用(),装備者_防御用()->HP*効果量(0)/100.0,false,false);	
		}
	}
}
//------------------------------------------------------------------------------
//緋想の剣 
//------------------------------------------------------------------------------
void cEquipment_ID_53::能力(const タイミング timing, cValiableField& valiable)
{

	if(timing == 主人公素振り_タイミング)
	{
		if(!cEquipment_能力仕様フラグID_exist(valiable,ID())
			&& 能力発動条件満たしている_攻撃用())
		{

			EffectFunctions::爆発エフェクト_中(装備者_攻撃用()->placeX,装備者_攻撃用()->placeY,210);

			vector<pcLandform> vpland;

			int a;
			for(a=1;a<8;a++)
			{
				vpland.push_back(sg_pDungeonSystem->キャラ視点方角地形(装備者_攻撃用(), a, 1));
			}

			int i;
			for(i=0;i<vpland.size();i++)
			{

				if(sg_pDungeonSystem->キャラクター敵対判定(装備者_攻撃用(),vpland[i]->pOnChar))
				{
					//ここで攻撃の情報を得て攻撃接近に渡す
					pcAttackinformation pattackinfo = pcAttackinformation(new cAttackinformation);
					
					sg_pDungeonSystem->GetNormalAttackinformation(装備者_攻撃用(),pattackinfo);

					pattackinfo->Landplase = vpland[i];
					pattackinfo->valiable.doubles[変数_方向] = (vpland[i]->place - 装備者_攻撃用()->足元地形()->place).GetAspect();
					pattackinfo->valiable.doubles[変数_距離] = 1;
					sg_pDungeonSystem->攻撃接近(pattackinfo);
			
				}
			}

			cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
			
		}
	}
	else if(timing == 攻撃壁貫通判定_タイミング)
	{
		if(能力発動条件満たしている_攻撃用() && !cEquipment_能力仕様フラグID_exist(valiable,ID()))
		{
			valiable.doubles.val(変数_汎用ブール) = 1;

			cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
		}
	}
}
//------------------------------------------------------------------------------
//御幣
//------------------------------------------------------------------------------
void cEquipment_ID_54::能力(const タイミング timing, cValiableField& valiable)
{

	if(timing == 初期残りスロット計算時_タイミング)
	{
		if(!cEquipment_能力仕様フラグID_exist(valiable,ID())// &&
			//(
			//能力発動条件満たしている_攻撃用() || 能力発動条件満たしている_防御用())
			)
		{
			valiable.doubles.val(変数_汎用ボーナス_定数) += 効果量(0)/valiable.doubles.val(変数_汎用不変定数);
		
			cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
				
		}
	}
}
//------------------------------------------------------------------------------
//必殺の武器
//------------------------------------------------------------------------------
void cEquipment_ID_55::能力(const タイミング timing, cValiableField& valiable)
{
	if(timing == ダメージ計算攻撃時_タイミング)
	{
		if(能力発動条件満たしている_攻撃用()
			&& valiable.doubles.exist(変数_直接攻撃フラグ))
		{
			if(効果量(0)/100.0 > random())
			{

				valiable.doubles.val(変数_攻撃力ボーナス_倍率) += 効果量(1)/100.0;

			}
		}
	}
}
//------------------------------------------------------------------------------
//金の延べ棒
//------------------------------------------------------------------------------
void cEquipment_ID_56::能力(const タイミング timing, cValiableField& valiable)
{
	if(timing == アイテム劣化直前_タイミング)
	{
		if(
			能力発動条件満たしている_攻撃用() || 能力発動条件満たしている_防御用())
		{
			if(valiable.drops.val(変数_対象落ち物) == 拠り代()
				&& valiable.doubles.val(変数_汎用ブール))
			{
				valiable.doubles.val(変数_汎用ブール) = 0;//効果発揮フラグ
				//cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
				map<tstring, StyleString> valiable;
				valiable[_T("Item")] = 拠り代()->FullName();
				g_Langメッセージ(_T("cEquip_ID_56能力メッセージ"),valiable);
			}
		}
	}
}
//------------------------------------------------------------------------------
//マンドラゴラの鉢
//------------------------------------------------------------------------------
void cEquipment_ID_57::能力(const タイミング timing, cValiableField& valiable)
{
	if(timing == 泥棒直前_タイミング)
	{
		if(能力発動条件満たしている_防御用())
		{

			valiable.doubles.val(変数_汎用ブール) = 0;//効果発揮フラグ
			

		}
	}
}
//------------------------------------------------------------------------------
//蛍光刀
//------------------------------------------------------------------------------
void cEquipment_ID_58::能力(const タイミング timing, cValiableField& valiable)
{
	if(timing == 防御力計算時_タイミング)
	{
		if(能力発動条件満たしている_防御用())
		{
			double HPPercent = 0.0;
			pcCharacter pchara = 装備者_防御用();
			if(pchara == NULL) return;

			HPPercent = pchara->HP/(double)pchara->MHP;

			HPPercent = (1-HPPercent) * 効果量(0)/100.0 + HPPercent * 効果量(1)/100.0;
			valiable.doubles.val(変数_防御力ボーナス_倍率) += HPPercent;
		}
	}
	if(timing == 攻撃力計算時_タイミング)
	{
		if(能力発動条件満たしている_攻撃用())
		{
			double HPPercent = 0.0;
			pcCharacter pchara = 装備者_攻撃用();
			if(pchara == NULL) return;

			HPPercent = pchara->HP/(double)pchara->MHP;

			HPPercent = (1-HPPercent) * 効果量(0)/100.0 + HPPercent * 効果量(1)/100.0;
			valiable.doubles.val(変数_攻撃力ボーナス_倍率) += HPPercent;
		}
	}
}
//------------------------------------------------------------------------------
//柄杓
//------------------------------------------------------------------------------
void cEquipment_ID_59::能力(const タイミング timing, cValiableField& valiable)
{
	if(timing == ダメージ計算防御時_タイミング)
	{
		if(能力発動条件満たしている_防御用())
		{
			if(valiable.intsets.val(変数_属性).count(攻撃属性::水))
			{
				if(!cEquipment_能力仕様フラグID_exist(valiable,ID()))
				{//初
					valiable.doubles.val(変数_耐性ボーナス_倍率％) += 効果量(0);
					cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
				}
				else
				{
					valiable.doubles.val(変数_耐性ボーナス_倍率％) += 効果量(1);
				
				}
				
			}
		}
	}
	if(timing == ダメージ計算攻撃時_タイミング)
	{
		if(能力発動条件満たしている_攻撃用())
		{
			攻撃属性::攻撃属性 attackAttri = 攻撃属性::水;
			if(valiable.doubles.exist(変数_直接攻撃フラグ)) {
				valiable.intsets.val(変数_属性).insert(attackAttri);
			}
			else {
				set<int> targetAttackAttri;
				targetAttackAttri.insert((int)attackAttri);
				MobAbilityIdiom::属性攻撃ボーナス％(targetAttackAttri,効果量(2))(装備者_攻撃用(), timing, valiable);
			}
		}
	}
}
//------------------------------------------------------------------------------
//エレキテルの羽衣
//------------------------------------------------------------------------------
void cEquipment_ID_60::能力(const タイミング timing, cValiableField& valiable)
{
	if(timing == ダメージ計算防御時_タイミング)
	{
		if(能力発動条件満たしている_防御用())
		{
			if(valiable.intsets.val(変数_属性).count(攻撃属性::雷))
			{
				if(!cEquipment_能力仕様フラグID_exist(valiable,ID()))
				{//初
					valiable.doubles.val(変数_耐性ボーナス_倍率％) += 効果量(0);
					cEquipment_能力仕様フラグID_dim(valiable,ID()) = 1;
				}
				else
				{
					valiable.doubles.val(変数_耐性ボーナス_倍率％) += 効果量(1);
				
				}
				
			}
		}
	}
	if(timing == ダメージ計算攻撃時_タイミング)
	{
		if(能力発動条件満たしている_攻撃用())
		{
			攻撃属性::攻撃属性 attackAttri = 攻撃属性::雷;
			if(valiable.doubles.exist(変数_直接攻撃フラグ)) {
				valiable.intsets.val(変数_属性).insert(attackAttri);
			}
			else {
				set<int> targetAttackAttri;
				targetAttackAttri.insert((int)attackAttri);
				MobAbilityIdiom::属性攻撃ボーナス％(targetAttackAttri,効果量(2))(装備者_攻撃用(), timing, valiable);
			}
		}
	}
}