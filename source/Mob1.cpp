#include "stdafx.h"
#include "cTrap.h"
#include "Mob1.h"
#include "Event1.h"
#include "OtherChara1.h"

#include "EffectFunctions.h"
#include "GameIdiom.h"
#include "FindUtility.h"
#include "MobAbilityIdiom.h"

#include "ceaiNegative.h"
#include "ceaiEscape.h"
#include "ceaiLoot.h"
#include "ceaiShopOwner.h"
#include "ceaiRoomKeeper.h"
#include "cSaveStore.h"
inline double pow(int a,int b)
{
	return pow((double)a,(double)b);
}

bool cMob_能力仕様フラグID_exist(cValiableField& valiable, int ID)
{
	return valiable.doubles.exist((_T("cMob_能力仕様フラグID_") + setStyle(ID)).c_str());
}
double& cMob_能力仕様フラグID_dim(cValiableField& valiable, int ID)
{
	return valiable.doubles.dim((_T("cMob_能力仕様フラグID_") + setStyle(ID)).c_str());
}
double& cMob_能力仕様フラグID_val(cValiableField& valiable, int ID)
{
	return valiable.doubles.val((_T("cMob_能力仕様フラグID_") + setStyle(ID)).c_str());
}
/*
#define DEF_SetPropaty(identify,\
f1,f2,f3,f4,f5,\
eb,ed,ep,e1,e2,e3,e4,e5,\
hb,hd,hp,h1,h2,h3,h4,h5,\
ab,ad,ap,a1,a2,a3,a4,a5,\
db,dd,dp,d1,d2,d3,d4,d5,\
sb,sd,sp,s1,s2,s3,s4,s5\
) \
const int cMob_appearFloor_ID_##identify##[] = {f1,f2,f3,f4,f5};\
int cMob_ID_##identify##::appearFloor(int power)\
{\
	return cMob_appearFloor_ID_##identify##[power];\
}\
const int cMob_attaP_ID_##identify##[] = {a1,a2,a3,a4,a5};\
const int cMob_deffP_ID_##identify##[] = {d1,d2,d3,d4,d5};\
const int cMob_SetMHP_ID_##identify##[] = {h1,h2,h3,h4,h5};\
const int cMob_HaveEXP_ID_##identify##[] = {e1,e2,e3,e4,e5};\
const int cMob_PerOfspecialAttack_ID_##identify##[] = {s1,s2,s3,s4,s5};\
int cMob_ID_##identify##::attaP()\
{\
	int p = cMob_attaP_ID_##identify##[CLASS];\
	if(-1 == p) p = ab*(ad*appearFloor(CLASS)+pow(ap,appearFloor(CLASS)));\
	return p;\
}\
int cMob_ID_##identify##::deffP()\
{\
	int p = cMob_deffP_ID_##identify##[CLASS];\
	if(-1 == p) p = db*(dd*appearFloor(CLASS)+pow(dp,appearFloor(CLASS)));\
	return p;\
}\
int cMob_ID_##identify##::SetMHP()\
{\
	int p = cMob_SetMHP_ID_##identify##[CLASS];\
	if(-1 == p) p = hb*(hd*appearFloor(CLASS)+pow(hp,appearFloor(CLASS)));\
	return MHP = p;\
}\
int cMob_ID_##identify##::HaveEXP()\
{\
	int p = cMob_HaveEXP_ID_##identify##[CLASS];\
	if(-1 == p) p = eb*(ed*appearFloor(CLASS)+pow(ep,appearFloor(CLASS)));\
	return p;\
}\
int cMob_ID_##identify##::PerOfspecialAttack()\
{\
	int p = cMob_PerOfspecialAttack_ID_##identify##[CLASS];\
	if(-1 == p) p = sb*(sd*appearFloor(CLASS)+pow(sp,appearFloor(CLASS)));\
	return p;\
}
*/

//ノラケダマ


/*
DEF_SetPropaty(0,//ID
	0,	1,	10,	30,	60,//出現階
	2,	1.5,	1.12,	-1,	-1,	-1,	-1,	-1,//経験値
	5,	.5,		1,		-1,	-1,	-1,	-1,	-1,//最大HP
	3,	.5,		1,		-1,	-1,	-1,	-1,	-1,//攻撃力
	1,	.3,		1.01,	-1,	-1,	-1,	-1,	-1,//防御力
	0,	0,		1,		-1,	-1,	-1,	-1,	-1//特殊攻撃率
)
*/

/*
int cMob_ID_0::特殊攻撃()
{
	//二回攻撃
	pcCharacter me = sg_pDungeonSystem->システム用キャラ検索(this);
	sg_pDungeonSystem->直接攻撃要請(pcCharacter(me));
	sg_pDungeonSystem->直接攻撃要請(pcCharacter(me));
	return true;
}
*/

//ザコヨウセイ


/*
DEF_SetPropaty(1,//ID
	1,	5,	15,	25,	50,//出現階
	2,	1.5,	1.13,	-1,	-1,	-1,	-1,	-1,//経験値
	5,	.6,		1,		-1,	-1,	-1,	-1,	-1,//最大HP
	4,	.4,		1,		-1,	-1,	-1,	-1,	-1,//攻撃力
	1,	.2,		1,		-1,	-1,	-1,	-1,	-1,//防御力
	0,	0,		1,		-1,	-1,	-1,	-1,	-1//特殊攻撃率
)
*/

//ヨウセイメイド


/*
DEF_SetPropaty(2,//ID
	5,	12,	20,	35,	55,//出現階
	2,	1.5,	1.13,	-1,	-1,	-1,	-1,	-1,//経験値
	5,	.5,		1,		-1,	-1,	-1,	-1,	-1,//最大HP
	5,	.4,		1,		-1,	-1,	-1,	-1,	-1,//攻撃力
	1,	.3,		1.01,	-1,	-1,	-1,	-1,	-1,//防御力
	0,	0,		1,		-1,	-1,	-1,	-1,	-1//特殊攻撃率
)
*/
//-----------------------------------------------------------------
//妖精メイド
pcEnemyAI cMob_ID_2::Get_kindofAI()
{
	return pcEnemyAI(new ceaiNegative);
}
int cMob_ID_2::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);
		if((nowco - youco).dif() >= 2 && (nowco - youco).dif() <= ValiableConstant1() && sg_pDungeonSystem->八方判定(nowco,youco))
		{
			return true;
		}
	}
	return false;
}
int cMob_ID_2::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{	
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);

		sg_pDungeonSystem->方向転換要請(me(),(youco-nowco).GetAspect());

		sg_pDungeonSystem->AnimationManager().
			Anime_PlaySE(_T("shoot.wav"),me()->足元地形()->place);

		pcDroping newpdrop = sg_pDungeonSystem->落ち物生成_設置なし(ValiableConstant2());
		//一本
		newpdrop->quality() = 0;
		newpdrop->state() = cDroping::STATE_NORMAL;
		newpdrop->discover();

		return sg_pDungeonSystem->投擲要請(me(),newpdrop,DRAWTYPE_ATTACKDRAW,ValiableConstant1());
	}

	return false;
}

//-----------------------------------------------------------------
//ダイヨウセイ
int cMob_ID_3::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara)
		&& pchara->足元地形()->RoomIndex != -1)
	{

		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);
		if((nowco - youco).dif() >= 2)
		{
			return true;
		}
	}
	return false;
}
int cMob_ID_3::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{	
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);
		cCoordinate aspco;
		aspco.SetAspect((youco-nowco).GetAspect());

		pcLandform pland = sg_pDungeonSystem->Map().Land(youco+aspco);
		if(pland->caninto(0,0,0))
		{
			sg_pDungeonSystem->ワープ要請(me(),pland);
		}
		else
		{
			sg_pDungeonSystem->ワープ要請(me(),pchara->足元地形());
		}

		EffectFunctions::煙エフェクト1(me()->placeX,me()->placeY);
		return true;
	}

	return false;
}
void cMob_ID_3::パッシブ能力(タイミング timing, cValiableField& valiable)
{

	if(timing == 攻撃直後時_タイミング)
	{
		if(ValiableConstant1()/100.0 > random())
		{
			//sg_pDungeonSystem->移動要請(me(),me()->aspect+4,me()->aspect);
			sg_pDungeonSystem->吹き飛ばし要請(me(),me(),me()->aspect+4,1,0);
		}
	}
}
//-----------------------------------------------------------------
//レミリア
int cMob_ID_4::AI特殊攻撃選択(cValiableField& valiable)
{

	/*ケイマトビルーチン
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);
		cCoordinate aspco;
		//int aspect = (youco-nowco).GetAspect();
		
		if((nowco - youco).dif() >= 2)
		{
	
			int a;
			cCoordinate nearest;
			int nearestdif = 1000000000;
			for(a=0;a<8;a++)
			{
				aspco.SetAspect(a);
				cCoordinate aspco2;
				aspco2.SetAspect((a/2)*2 + 6);
				cCoordinate keima = nowco+aspco*2 + aspco2;
				if(sg_pDungeonSystem->Map().Land(keima)->through(0,0))
				{
					cCoordinate dif = (keima - youco);
					if(nearestdif > dif.dif() * 1000 + abs(dif.x) + abs(dif.y))
					{
						nearestdif = dif.dif() * 1000 + abs(dif.x) + abs(dif.y);
						nearest = keima;
					}
				}
			}
			if(nearestdif != 1000000000)
			{
				return true;
			}
			
		}
	}
	*/
	/*
	if(sg_pDungeonSystem->s_turn_speed() < ValiableConstant3() - ValiableConstant4())
	{//手加減
		return false;//不許可
	}

	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);
		cValiableField val;
		if(sg_pDungeonSystem->八方判定(nowco,youco) && (nowco - youco).dif() > 1
			&&sg_pDungeonSystem->isCanNeighberAttack(me(),(youco - nowco).GetAspect(),(youco - nowco).dif(),0,0))
		{
			return true;
		}
	}

	*/

	//隣接キャラが居る
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);
		if((nowco - youco).dif() == 1 && me()->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
		{
			return true;
		}
	}
	return false;
}
int cMob_ID_4::特殊攻撃効果(cValiableField& valiable)
{

	/*ケイマトビルーチン
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{	
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);
		cCoordinate aspco;
		//int aspect = (youco-nowco).GetAspect();
		
		if((nowco - youco).dif() >= 2)
		{
			int a;
			cCoordinate nearest;
			int nearestdif = 1000000000;
			for(a=0;a<8;a++)
			{
				aspco.SetAspect(a);
				cCoordinate aspco2;
				aspco2.SetAspect((a/2)*2 + 6);
				cCoordinate keima = nowco+aspco*2 + aspco2;
				if(sg_pDungeonSystem->Map().Land(keima)->through(0,0))
				{
					cCoordinate dif = (keima - youco);
					if(nearestdif > dif.dif() * 1000 + abs(dif.x) + abs(dif.y))
					{
						nearestdif = dif.dif() * 1000 + abs(dif.x) + abs(dif.y);
						nearest = keima;
					}
				}
			}
			sg_pDungeonSystem->投げ飛ばし要請(me(),me(),sg_pDungeonSystem->Map().Land(nearest),ValiableConstant1(),false,1.7);
				
			return true;
		}
	}
	*/
	/*
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{	
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);

		sg_pDungeonSystem->方向転換要請(me(),(youco-nowco).GetAspect());

		pcLandform pland = sg_pDungeonSystem->キャラ前地形(me(),ValiableConstant2());
		sg_pDungeonSystem->吹き飛ばし要請(me(),me(),me()->aspect,ValiableConstant2(),0);

		if(pland != me()->足元地形() && sg_pDungeonSystem->キャラ前地形(me(),1)->pOnChar != NULL
			 && me()->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
		{
			multiset<攻撃属性::攻撃属性> 属性;

			sg_pDungeonSystem->攻撃接近(攻撃作成(
				me(),//Attacker
				sg_pDungeonSystem->キャラ前地形(me(),1),//Landplase
				ValiableConstant1(),//attackPower
				1,//damageflag
				属性,//attribute
				true
				));
			sg_pDungeonSystem->強制ターンエンド要請(me());
		}
		return true;
	}
	*/
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{
		AImemory[0] = 1;
	}
	return false;
}
void cMob_ID_4::パッシブ能力(タイミング timing, cValiableField& valiable)
{

	/*if(timing == ダメージ計算防御時_タイミング)
	{

		if(valiable.intsets.val(変数_属性).count(攻撃属性::水))
		{
			if(Condition.速度度数() >= Condition.デフォルト速度度数())
				sg_pDungeonSystem->速度減少要請(me(),10);
		}
	}
	else*/ if(timing == 攻撃直後時_タイミング)
	{

		if(AImemory[0] == 1)
		{
			AImemory[0] = 0;
			sg_pDungeonSystem->回復要請(me(),
				valiable.doubles.val(変数_ダメージ)*ValiableConstant3()/100.0,true);
			sg_pDungeonSystem->満腹度減少要請(me(),UseSPOfspecialAttack(),false);
			
		}
	}
}
//-----------------------------------------------------------------
//フランドール
int cMob_ID_5::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);
		if((nowco - youco).dif() == 1 && me()->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
		{
			return true;
		}
	}
	return false;
}
int cMob_ID_5::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{	
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);

		sg_pDungeonSystem->方向転換要請(me(),(youco-nowco).GetAspect());

		pcLandform pland = sg_pDungeonSystem->キャラ前地形(me(),1);

		//ここにアニメを入れる
		EffectFunctions::爆発エフェクト_中(pland->placeX,pland->placeY,20);


		int a;

		map<tstring, StyleString> val;
		val[_T("Chara")] = FullName();
		g_Langメッセージ(_T("cMob_ID_5_特殊攻撃メッセージ"),val);

		GameIdiom::爆発(pland,me(),ValiableConstant1(),0,false,false,true,false);
		
		return true;
	}

	return false;
}
void cMob_ID_5::パッシブ能力(タイミング timing, cValiableField& valiable)
{

	if(timing == 被攻撃直後時_タイミング)
	{
		if(ValiableConstant3() <= Stomach && !死亡())
		{//消費SP十分
			if(ValiableConstant2()/100.0 > random())
			{//分身
				sg_pDungeonSystem->満腹度減少要請(me(),ValiableConstant3(),false);
				map<tstring, StyleString> val;
				//val[_T("Drop")] = valiable.drops.val(変数_対象落ち物)->FullName();
				val[_T("Chara")] = FullName();
				g_Langメッセージ(_T("cMob_ID_5_特殊能力メッセージ"),val);
				sg_pDungeonSystem->分身要請(me());
			}
		}
	}
	/*else if(timing == ダメージ計算防御時_タイミング)
	{

		if(valiable.intsets.val(変数_属性).count(攻撃属性::水))
		{
			if(Condition.速度度数() >= Condition.デフォルト速度度数())
				sg_pDungeonSystem->速度減少要請(me(),10);
		}
	}*/
}
//-----------------------------------------------------------------
//サクヤ
pcLandform cMob_ID_6::AIお気に入り地形(cValiableField& valiable)
{
	if(足元地形()->RoomIndex == -1) return NULLLAND;

	vector<pcCharacter> vpchara;
	sg_pDungeonSystem->キャラクター目視可範囲キャラリスト(me(),vpchara);



	int i;
	for(i=0;i<vpchara.size();i++)
	{
		if(vpchara[i]->ID() == 2023//メイリン
			&& vpchara[i]->Condition.眠りで行動不能である())
		{
			return vpchara[i]->足元地形();
		}
	}

	return NULLLAND;
}
int cMob_ID_6::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL)
	{
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);

		if(sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
		{

			if((nowco - youco).dif() == 1 && me()->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
			{
				return true;
			}

					
			if((nowco - youco).dif() >= ValiableConstant2() && (nowco - youco).dif() <= ValiableConstant3())
			{
				return true;
			}
		}
		else if(!sg_pDungeonSystem->キャラクター敵対判定(me(),pchara)
			&& pchara->ID() == 2023)
		{
			if((nowco - youco).dif() == 1 && me()->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
			{
				return true;
			}
		
		}

	}
	return false;
}
int cMob_ID_6::特殊攻撃効果(cValiableField& valiable)
{
	
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL)
	{
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);

		if((nowco - youco).dif() == 1 && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
		{	
			cCoordinate nowco(me()->placeX,me()->placeY);
			cCoordinate youco(pchara->placeX,pchara->placeY);

			sg_pDungeonSystem->方向転換要請(me(),(youco-nowco).GetAspect());

			int i;
			for(i=0;i<ValiableConstant1();i++)
			{
				if(!Condition.行動不能である() && me()->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
				{
					通常攻撃();
				}
			}
			
			return true;
		}
		else if( (nowco - youco).dif() == 1 && !sg_pDungeonSystem->キャラクター敵対判定(me(),pchara)
			&& pchara->ID() == 2023)
		{

			if((nowco - youco).dif() == 1 && me()->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
			{
				GameIdiom::悪性異常状態治療要請(pchara);
			}
		}

		if((nowco - youco).dif() >= ValiableConstant2() && (nowco - youco).dif() <= ValiableConstant3())
		{
			int i;
			for(i=0;i<ValiableConstant4();i++)
			{
				me()->pAI->u_目標点に移動(youco.x,youco.y);
				cCoordinate nowco(me()->placeX,me()->placeY);
				if((nowco - youco).dif() <= 1 || (nowco - youco).dif() - 1< ValiableConstant2())
				{
					break;
				}
			}
			return true;
		}
	}
	return false;
}
void cMob_ID_6::パッシブ能力(タイミング timing, cValiableField& valiable)
{
/*
	if(timing == 投擲攻撃接近直前_タイミング)
	{
		valiable.doubles.val(変数_汎用ブール) = 0;//効果発揮フラグ
		
		pcCharacter pchara = valiable.drops.val(変数_対象落ち物)->投擲者();

		if(pchara != NULL)
		{
			cCoordinate nowco(me()->placeX,me()->placeY);
			cCoordinate youco(pchara->placeX,pchara->placeY);
			cCoordinate aspco;
			aspco.SetAspect((youco-nowco).GetAspect());

			pcLandform pland = sg_pDungeonSystem->Map().Land(youco+aspco);

			map<tstring, StyleString> val;
			//val[_T("Drop")] = valiable.drops.val(変数_対象落ち物)->FullName();
			val[_T("Chara")] = FullName();
			g_Langメッセージ(_T("cMob_ID_6_特殊能力メッセージ"),val);

			if(pland->caninto(0,0,0))
			{
				sg_pDungeonSystem->ワープ要請(me(),pland);
			}
			else
			{
				sg_pDungeonSystem->ワープ要請(me(),pchara->足元地形());
			}

			EffectFunctions::煙エフェクト1(me()->placeX,me()->placeY);
		}
	}
*/

}
//-----------------------------------------------------------------
//ルーミア
void cMob_ID_7::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	if(timing == 暗闇更新_タイミング)
	{
		sg_pDungeonSystem->暗闇付与(足元地形(),ValiableConstant1());
	}
}
//-----------------------------------------------------------------
//ニトリ
void cMob_ID_8::配置処理()
{
	didGouseiNum = 0;
	sg_pDungeonSystem->擬態要請(me(),GAME_TURN_GAMEOVER,false);
}
int cMob_ID_8::MaxholdNum()
{
	return ValiableConstant1() - didGouseiNum;
}
int cMob_ID_8::AIセミパッシブ特殊攻撃選択(cValiableField& valiable)
{//SP消費なし優先度高
	if(AImemory[0] != 0)
	{
		if(ValiableConstant4()/100.0 < random())
		{
			int a;
			pcLandform pland;
			pcCharacter pchara;
			for(a=0;a<8;a++)
			{
				pland = sg_pDungeonSystem->隣接地形(足元地形(),a,1);
				pcCharacter pchara = pland->pOnChar;
				if(pchara != NULL)
				{
					cCoordinate nowco(me()->placeX,me()->placeY);
					cCoordinate youco(pchara->placeX,pchara->placeY);
					cValiableField val;
					if(sg_pDungeonSystem->キャラクター敵対判定(me(),pchara) && me()->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
					{//隣接
						return true;
					}

				}
			}
		}
	}
	return false;
}
int cMob_ID_8::セミパッシブ特殊攻撃効果(cValiableField& valiable)
{//SP消費なし優先度高
	if(AImemory[0] != 0)
	{
		if(ValiableConstant4()/100.0 < random())
		{
			int a;
			pcLandform pland;
			pcCharacter pchara;
			for(a=0;a<8;a++)
			{
				pland = sg_pDungeonSystem->隣接地形(足元地形(),a,1);
				pcCharacter pchara = pland->pOnChar;
				if(pchara != NULL)
				{
					cCoordinate nowco(me()->placeX,me()->placeY);
					cCoordinate youco(pchara->placeX,pchara->placeY);

					sg_pDungeonSystem->方向転換要請(me(),(youco-nowco).GetAspect());

					cValiableField val;
					if(sg_pDungeonSystem->キャラクター敵対判定(me(),pchara) && me()->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
					{//隣接
						return 通常攻撃();
					}

				}
			}
		}
	}
	return false;
}
int cMob_ID_8::AI特殊攻撃選択(cValiableField& valiable)
{

	if(AImemory[0] == 0)
	{
		pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

		if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
		{
			cCoordinate nowco(me()->placeX,me()->placeY);
			cCoordinate youco(pchara->placeX,pchara->placeY);
			cValiableField val;
			if((nowco - youco).dif() <= 1 && me()->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
			{//隣接
				return true;
			}

		}

	}


	return false;
}
int cMob_ID_8::特殊攻撃効果(cValiableField& valiable)
{
	if(AImemory[0] == 0)
	{
		pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;
	
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);
		cValiableField val;

		sg_pDungeonSystem->方向転換要請(me(),(youco-nowco).GetAspect());
		pcLandform pland = sg_pDungeonSystem->キャラ前地形(me(),1);
		pchara = pland->pOnChar;
		
		if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
		{


			if((nowco - youco).dif() <= 1)
			{//隣接
				vector<pcDroping> vpdrop = FindUtility::装備品以外の表面手持ちアイテム店売り除く(pchara);
					
				if(!vpdrop.empty())
				{
					pcDroping pdrop = vpdrop[vpdrop.size()*random()];
					if(持ち物余白あり(pdrop))
					{
						if(sg_pDungeonSystem->泥棒要請(me(),pdrop))
						{
							
							sg_pDungeonSystem->元気要請(me(),ValiableConstant2(),ValiableConstant3(),false);
							sg_pDungeonSystem->頑強要請(me(),ValiableConstant2(),ValiableConstant3(),false);
							//sg_pDungeonSystem->速度増加要請(me(),10,false);
							sg_pDungeonSystem->擬態要請(me(),-1,false);

							AImemory[0] = 1;
							pAI = pcEnemyAI(new ceaiEscape);
							pAI->Init(me());
							return true;
						}
					}
				}
			}

		}


		return false;
	}
	else
	{
	
	}

	return false;
}
void cMob_ID_8::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	if(timing == 投擲攻撃接近直前_タイミング)
	{

		if(sg_pDungeonSystem->落ち物拾得可(valiable.drops.val(変数_対象落ち物),me()))
		{
			valiable.doubles.val(変数_汎用ブール) = 0;//効果発揮フラグ

			sg_pDungeonSystem->遠隔拾得要請(me(),valiable.drops.val(変数_対象落ち物));
			
			EffectFunctions::煙エフェクト1(me()->placeX,me()->placeY);
		
			//sg_pDungeonSystem->アイテム合成(holdItem);
			sg_pDungeonSystem->元気要請(me(),ValiableConstant2(),ValiableConstant3(),false);
			sg_pDungeonSystem->頑強要請(me(),ValiableConstant2(),ValiableConstant3(),false);
			//sg_pDungeonSystem->速度増加要請(me(),10,false);
			sg_pDungeonSystem->擬態要請(me(),-1,false);
			
			AImemory[0] = 1;
			pAI = pcEnemyAI(new ceaiEscape);
			pAI->Init(me());

			//合成
			int beforeItemNum = holdItem.size();
			sg_pDungeonSystem->アイテム合成(holdItem);
			int afterItemNum = holdItem.size();

			if(beforeItemNum > afterItemNum) {
				didGouseiNum += beforeItemNum - afterItemNum;
			}
		}

	}
	else if(timing == ダメージ計算攻撃時_タイミング)
	{
		valiable.intsets.val(変数_属性).insert(攻撃属性::水);
	}
	/*else if(timing == ダメージ計算防御時_タイミング)
	{
		if(valiable.intsets.val(変数_属性).count(攻撃属性::電気))
		{
			if(Condition.速度度数() >= Condition.デフォルト速度度数())
				sg_pDungeonSystem->速度減少要請(me(),10);
		}
	}*/
	else if(timing == 死亡直前_タイミング)
	{
		//sg_pDungeonSystem->アイテム合成(holdItem);
	}
	else if(timing == 攻撃直後時_タイミング)
	{
		sg_pDungeonSystem->擬態要請(me(),-1,false);
	}

}
//-----------------------------------------------------------------
//エイキ
int cMob_ID_9::AIセミパッシブ特殊攻撃選択(cValiableField& valiable)
{
	return (AImemory[0]);
}
int cMob_ID_9::セミパッシブ特殊攻撃効果(cValiableField& valiable)
{
	map<tstring, StyleString> val;
	val[_T("Chara")] = ShortName();
	g_Langメッセージ(_T("説教メッセージ"),val);
	return true;
}
int cMob_ID_9::AI特殊攻撃選択(cValiableField& valiable)
{
	if(AImemory[0] == 0)
	{
		pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

		if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
		{
			cCoordinate nowco(me()->placeX,me()->placeY);
			cCoordinate youco(pchara->placeX,pchara->placeY);
			if((nowco - youco).dif() == 1 && me()->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
			{
				return true;
			}
		}

	}
	return false;
}
int cMob_ID_9::特殊攻撃効果(cValiableField& valiable)
{
	if(AImemory[0] == 0)
	{
		pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

		if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
		{	
			cCoordinate nowco(me()->placeX,me()->placeY);
			cCoordinate youco(pchara->placeX,pchara->placeY);

			sg_pDungeonSystem->方向転換要請(me(),(youco-nowco).GetAspect());

			pcLandform pland = sg_pDungeonSystem->キャラ前地形(me(),1);

			if(pland->pOnChar != NULL)
			{

				map<tstring, StyleString> valiable;
				valiable[_T("Chara")] = pland->pOnChar->ShortName();
				g_Langメッセージ(_T("cMob_ID_9_特殊攻撃メッセージ"),valiable);
					
				//AImemory[0] = 1;
				
				return sg_pDungeonSystem->びっくり要請(pland->pOnChar,ValiableConstant1());
			}
		}
	}
	return false;
}
void cMob_ID_9::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	if(timing == びっくり追加直前_タイミング)
	{
		valiable.doubles[変数_汎用ブール] = 0;//拒否
	}
	/*
	if(AImemory[0])
	{
		if(timing == 直接攻撃回避力計算時_タイミング)
		{
			valiable.doubles[変数_回避力] += ValiableConstant4();
		}
	}
	*/
	
}
//-----------------------------------------------------------------
//ヒナ
int cMob_ID_10::AIセミパッシブ特殊攻撃選択(cValiableField& valiable)
{
	//爆弾岩的な
	if(HP < MHP/2) return true;
	else
	{
		AImemory[0] = AImemory[1] = 0;
		emotion.erase(EMOTION_ASERI);
		return false;
	}
}
int cMob_ID_10::セミパッシブ特殊攻撃効果(cValiableField& valiable)
{
	if(HP < MHP/4 && AImemory[0] && AImemory[1])
	{
		
		pcLandform pland;
		int a;

		if(sg_pDungeonSystem->即死要請(me(),0,1))
		{
			//ここにアニメを入れる
			EffectFunctions::爆発エフェクト_中(me()->placeX,me()->placeY,260);


			for(a=0;a<8;a++)
			{
				pland = sg_pDungeonSystem->キャラ視点方角地形(me(),a,1);
				if(pland->pOnChar != NULL)
				{//厄爆発
					pcCharacter pchara = pland->pOnChar;

					map<tstring, StyleString> val;
					//val[_T("Drop")] = valiable.drops.val(変数_対象落ち物)->FullName();
					val[_T("Chara")] = FullName();
					g_Langメッセージ(_T("cMob_ID_10_特殊攻撃メッセージ"),val);

					if(pchara->雑魚属性())
					{
						sg_pDungeonSystem->速度減少要請(pchara, 10);
					}
					else
					{
						int i;
						if(!pchara->holdItem.empty())
						{
							for(i=0;i<ValiableConstant1();i++)
							{
								int index = pchara->holdItem.size()*random();

								sg_pDungeonSystem->アイテム呪い(pchara->holdItem[index]);
								
							}
						}
					}
				}
			}
		}
		
		return true;
		
	}
	else
	{//停止
		AImemory[0] = 1;
		emotion.insert(EMOTION_ASERI);

		sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(
			&visibleemotion,
			emotion);

		return true;
	}

}
int cMob_ID_10::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);
		if((nowco - youco).dif() == 1 && me()->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
		{//隣接かつ攻撃可能地形
			int i;
			for(i=0;i<pchara->holdItem.size();i++)
			{
				if(pchara->holdItem[i]->state() == cDroping::STATE_CURSE)
				{
					return true;
				}
			}
		}
	}
	return false;
}
int cMob_ID_10::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;
	
	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{	
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);

		sg_pDungeonSystem->方向転換要請(me(),(youco-nowco).GetAspect());

		pcLandform pland = sg_pDungeonSystem->キャラ前地形(me(),1);

		if(pland->pOnChar != NULL)
		{
			pcCharacter pchara = pland->pOnChar;
			int i;
			for(i=0;i<pchara->holdItem.size();i++)
			{
				if(pchara->holdItem[i]->state() == cDroping::STATE_CURSE)
				{
					int res = sg_pDungeonSystem->アイテム祝福(pchara->holdItem[i]);
					sg_pDungeonSystem->元気要請(me(),ValiableConstant2(),ValiableConstant3());
					sg_pDungeonSystem->頑強要請(me(),ValiableConstant2(),ValiableConstant3());
					return res;
				}
			}
		}
	}

	return false;
}
void cMob_ID_10::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	if(timing == 被攻撃直後時_タイミング)
	{
		if(AImemory[0] == 1)
		{
			AImemory[1] = 1;//殴られた
		}
	}
}
//-----------------------------------------------------------------
//ユユコ
pcLandform cMob_ID_11::AIお気に入り地形(cValiableField& valiable)
{
	if(ValiableConstant2() > random()*100)
	{
		return sg_pDungeonSystem->RandomPlace();
	}
	return NULLLAND;
}
int cMob_ID_11::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);
		if((nowco - youco).dif() == 1 && me()->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
		{
			return true;
		}
	}
	return false;
}
int cMob_ID_11::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{	
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);

		sg_pDungeonSystem->方向転換要請(me(),(youco-nowco).GetAspect());

		pcLandform pland = sg_pDungeonSystem->キャラ前地形(me(),1);

		if(pland->pOnChar != NULL)
		{
			map<tstring, StyleString> valiable;
			valiable[_T("CharaA")] = me()->ShortName();
			valiable[_T("CharaB")] = pland->pOnChar->ShortName();
			g_Langメッセージ(_T("cMob_ID_11_特殊攻撃メッセージ"),valiable);
			return sg_pDungeonSystem->死の誘い要請(pland->pOnChar,me(),ValiableConstant1());
		}
	}

	return false;
}
void cMob_ID_11::パッシブ能力(タイミング timing, cValiableField& valiable)
{

	if(timing == 即死要請直前_タイミング)
	{
		valiable.doubles[変数_汎用ブール] = 0;//拒否
		
	}
	else if(timing == 死の誘い追加直前_タイミング)
	{
		valiable.doubles[変数_汎用ブール] = 0;//拒否
		
	}
}
//-----------------------------------------------------------------
//ヨウム
int cMob_ID_12::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);
		if((nowco - youco).dif() == 1 && me()->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
		{
			return true;
		}
	}
	return false;
}
int cMob_ID_12::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{	
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);

		sg_pDungeonSystem->方向転換要請(me(),(youco-nowco).GetAspect());

		pcLandform pland = sg_pDungeonSystem->キャラ前地形(me(),1);

		if(pland->pOnChar != NULL)
		{
			vector<pcDroping> vpdrop;

			if(ValiableConstant2() && pchara->attackequipment != NULL) vpdrop.push_back(pchara->attackequipment);
			if(ValiableConstant3() && pchara->defenseequipment != NULL) vpdrop.push_back(pchara->defenseequipment);

			if(ValiableConstant4())
			{
				int k;
				for(k=0;k<pchara->最大装備スペル数();k++)
				{
					if(pchara->spellequipment[k] != NULL) vpdrop.push_back(pchara->spellequipment[k]);
				}
			}
			
				
			if(!vpdrop.empty())
			{
				pcDroping pdrop = vpdrop[vpdrop.size()*random()];
				if(sg_pDungeonSystem->アイテム弾き飛ばし要請(me(),pdrop,ValiableConstant1()))
				{

					return true;
				}
			}
		}
	}

	return false;
}
//-----------------------------------------------------------------
//チルノ
int cMob_ID_13::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);
		if((nowco - youco).dif() == 1 && me()->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
		{
			return true;
		}
	}
	return false;
}
int cMob_ID_13::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{	
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);

		sg_pDungeonSystem->方向転換要請(me(),(youco-nowco).GetAspect());

		pcLandform pland = sg_pDungeonSystem->キャラ前地形(me(),1);

		if(pland->pOnChar != NULL)
		{
			特殊攻撃_アニメ(valiable);
			return sg_pDungeonSystem->氷付け要請(pland->pOnChar,ValiableConstant1());
		}
	}

	return false;
}
void cMob_ID_13::パッシブ能力(タイミング timing, cValiableField& valiable)
{

	if(timing == ダメージ計算攻撃時_タイミング)
	{
		valiable.intsets.val(変数_属性).insert(攻撃属性::冷気);
	}
	/*else if(timing == ダメージ計算防御時_タイミング)
	{
		if(valiable.intsets.val(変数_属性).count(攻撃属性::冷気))
		{
			valiable.doubles.val(変数_耐性ボーナス_倍率％) += ValiableConstant2();
		}
		if(valiable.intsets.val(変数_属性).count(攻撃属性::火))
		{
			if(Condition.速度度数() >= Condition.デフォルト速度度数())
				sg_pDungeonSystem->速度減少要請(me(),10);
		}
	}*/
	else if(timing == 氷付け追加直前_タイミング)
	{
		valiable.doubles[変数_汎用ブール] = 0;//拒否
		
	}
}
	
//-----------------------------------------------------------------
//マリサ
pcEnemyAI cMob_ID_14::Get_kindofAI()
{
	return pcEnemyAI(new ceaiLoot);
}

double cMob_ID_14::targetPriorityHimanageDrop(pcCharacter me, pcDroping object)
{
	if(me->足元地形()->RoomIndex == -1) {
		return 0.0;
	}

	if(!object->落ちてる()) {
		return 0.0;
	}

	if( ! object->属性.count(落ち物属性::アイテム)) {
		return 0.0;
	}

	if(object->shopitemflag() != cDroping::SHOPFLAG_NONE) {
		return 0.0;
	}

	pcLandform lastPutLand = sg_pDungeonSystem->Map().RoomList[me->足元地形()->RoomIndex]->plastputland;
	
	if(lastPutLand && lastPutLand->pOnDrop == object) {
		return 10000;
	}

	cCoordinate distance(object->placeX - me->placeX, object->placeY - me->placeY);

	if(distance.dif() == 0) {
		return 1000000;
	}

	double priority = (100 - distance.dif())*100;


	return priority;
}
pcLandform cMob_ID_14::AIお気に入り地形(cValiableField& valiable)
{

	if(AImemory[0] == 0)
	{
		vector<pcDroping> vpDrop;
		sg_pDungeonSystem->キャラクター目視可範囲落ち物リスト(me(),vpDrop);

		pcDroping pHimanageDrop = FindUtility::ターゲット落ち物検索(vpDrop, me(), targetPriorityHimanageDrop);

		if(pHimanageDrop) {
			return pHimanageDrop->現在地形();
		}
	}
	return NULLLAND;
}
int cMob_ID_14::AIセミパッシブ特殊攻撃選択(cValiableField& valiable)
{
	if(AImemory[0] == 0)
	{
		if(me()->pAI->u_敵隣接判定())
		{
			return (ValiableConstant1()/100.0 > random());//様子見
		}
	}
	return false;	
}
int cMob_ID_14::セミパッシブ特殊攻撃効果(cValiableField& valiable)
{

	map<tstring, StyleString> val;
	val[_T("Chara")] = FullName();
	g_Langメッセージ(_T("様子見メッセージ"),val);
	
	return true;

}
double cMob_ID_14::targetPriority(pcCharacter me, pcCharacter object)
{
	double priority = 0;
	cCoordinate distance(object->placeX - me->placeX, object->placeY - me->placeY);

	priority += (100 - distance.dif())*10000;

	if(me == object ||
		!sg_pDungeonSystem->キャラクター敵対判定(me,object))
	{
		priority = 0;
	}
	return priority;
}
int cMob_ID_14::AI特殊攻撃選択(cValiableField& valiable)
{
	if(!holdItem.empty())
	{
		vector<pcCharacter> vpchara;
		sg_pDungeonSystem->キャラクター目視可範囲キャラリスト(me(),vpchara);

		pcCharacter pchara = FindUtility::ターゲットキャラクター検索(vpchara, me(), targetPriority);

		if(pchara && FindUtility::キャラの距離(me(),pchara) <= ValiableConstant3())
		{
			return true;
		}
	}

	if(AImemory[0] == 0)
	{
		pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;
		pcDroping pdrop = valiable.lands[変数_攻撃場所]->pOnDrop;
			
		if(pdrop != NULL)
		{
			
			cCoordinate nowco(me()->placeX,me()->placeY);
			cCoordinate youco(pdrop->placeX,pdrop->placeY);

			/*
			if((nowco - youco).dif() <= 1 && me()->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
			{//隣接
				return true;
			}
			*/

			if((nowco - youco).dif() == 0)
			{//足元
				vector<pcCharacter> vpchara;
				sg_pDungeonSystem->キャラクター目視可範囲キャラリスト(me(),vpchara);

				pcCharacter pchara = FindUtility::ターゲットキャラクター検索(vpchara, me(), targetPriority);

				if(pchara && FindUtility::キャラの距離(me(),pchara) <= ValiableConstant3())
				{
					return true;
				}
			}
		
		}	
		
		if(me()->pAI->u_敵隣接判定())
		{
			int out_tgt_x,out_tgt_y;
			if(me()->pAI->u_敵サーチ_距離優先(out_tgt_x,out_tgt_y)) {
				cCoordinate nowco(me()->placeX,me()->placeY);
				cCoordinate youco(out_tgt_x,out_tgt_y);
				cValiableField val;
				if((nowco - youco).dif() <= 1 && me()->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
				{//隣接
					return true;
				}
			}

		}


	}

	return false;
}
int cMob_ID_14::特殊攻撃効果(cValiableField& valiable)
{
	if(!holdItem.empty())
	{
		pcDroping pdrop = holdItem[0];
		
		vector<pcCharacter> vpchara;
		sg_pDungeonSystem->キャラクター目視可範囲キャラリスト(me(),vpchara);

		pcCharacter pchara = FindUtility::ターゲットキャラクター検索(vpchara, me(), targetPriority);

		if(pchara && FindUtility::キャラの距離(me(),pchara) <= ValiableConstant3())
		{
			GameIdiom::キャラの方を向く(me(),pchara);
			sg_pDungeonSystem->投擲要請(me(),pdrop,THROW_HIMANAGE,ValiableConstant3());
			return true;
		}
	}

	if(AImemory[0] == 0)
	{
		pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;
		pcDroping pdrop = valiable.lands[変数_攻撃場所]->pOnDrop;
	
		if(pdrop != NULL)
		{
			cCoordinate nowco(me()->placeX,me()->placeY);
			cCoordinate youco(pdrop->placeX,pdrop->placeY);


			if((nowco - youco).dif() == 0)
			{//足元


				
				vector<pcCharacter> vpchara;
				sg_pDungeonSystem->キャラクター目視可範囲キャラリスト(me(),vpchara);

				pcCharacter pchara = FindUtility::ターゲットキャラクター検索(vpchara, me(), targetPriority);

				if(pchara && FindUtility::キャラの距離(me(),pchara) <= ValiableConstant3())
				{
					GameIdiom::キャラの方を向く(me(),pchara);
					sg_pDungeonSystem->投擲要請(me(),pdrop,THROW_HIMANAGE,ValiableConstant3());
					return true;
				}
				/*
				cCoordinate nowco(me()->placeX,me()->placeY);
				cCoordinate youco(pdrop->placeX,pdrop->placeY);
				sg_pDungeonSystem->方向転換要請(me(),(youco-nowco).GetAspect());
				pcLandform pland = sg_pDungeonSystem->キャラ前地形(me(),1);
				
				pdrop = pland->pOnDrop;

				if((nowco - youco).dif() == 0) pdrop = 足元();

				cValiableField val;
				if((nowco - youco).dif() <= 1 && me()->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
				{//隣接
					if(pdrop->shopitemflag() == cDroping::SHOPFLAG_NONE && 
						sg_pDungeonSystem->泥棒要請(me(),pdrop)
						)
					{
						GameIdiom::高飛び(me(),me()->足元地形());
						AImemory[0] = 1;
						pAI = pcEnemyAI(new ceaiEscape);
						pAI->Init(me());
						return true;
					}
				}
				*/
			}
		}

		if(me()->pAI->u_敵隣接判定())
		{
			int out_tgt_x,out_tgt_y;
			if(me()->pAI->u_敵サーチ_距離優先(out_tgt_x,out_tgt_y)) {
				cCoordinate nowco(me()->placeX,me()->placeY);
				cCoordinate youco(out_tgt_x,out_tgt_y);
				sg_pDungeonSystem->方向転換要請(me(),(youco-nowco).GetAspect());
				pcLandform pland = sg_pDungeonSystem->キャラ前地形(me(),1);
				pchara = pland->pOnChar;
				if(pchara != NULL)
				{
					cValiableField val;
					if((nowco - youco).dif() <= 1 && me()->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
					{//隣接
						vector<pcDroping> vpdrop;
						int i,size = pchara->holdItem.size();
						for(i=0;i<size;i++)
						{
							// 2012 10 02 仕様変更
							if(pchara->holdItem[i] == pchara->attackequipment) continue;
							if(pchara->holdItem[i] == pchara->defenseequipment) continue;

							if(!ValiableConstant2())
							{

								//if(pchara->holdItem[i] == pchara->bulletequipment) continue;
								int k;
								for(k=0;k<pchara->最大装備スペル数();k++)
								{
									if(pchara->holdItem[i] == pchara->spellequipment[k]) break;
								}
								if(k != pchara->最大装備スペル数()) continue;//最後までいなかった

							}
							if(pchara->holdItem[i]->shopitemflag() != cDroping::SHOPFLAG_NONE) continue;

							vpdrop.push_back(pchara->holdItem[i]);
						}
							
						if(!vpdrop.empty())
						{
							if(sg_pDungeonSystem->泥棒要請(me(),vpdrop[vpdrop.size()*random()]))
							{
								GameIdiom::高飛び(me(),me()->足元地形());
								AImemory[0] = 1;
								pAI = pcEnemyAI(new ceaiEscape);
								pAI->Init(me());
								return true;
							}
						}
					}
				}
			}

		}


		return false;
	}

	return false;
}

//-----------------------------------------------------------------
//パチュリー
pcEnemyAI cMob_ID_15::Get_kindofAI()
{
	return pcEnemyAI(new ceaiNegative);
}
int cMob_ID_15::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);
		cValiableField val;
		if(sg_pDungeonSystem->八方判定(nowco,youco) && (nowco - youco).dif() <= AI最大間合い(val))
		{
			return true;
		}
	}
	return false;
}
int cMob_ID_15::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{	
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);

		sg_pDungeonSystem->方向転換要請(me(),(youco-nowco).GetAspect());

		pcDroping newpdrop = sg_pDungeonSystem->落ち物生成_設置なし(6018);
		//一本
		newpdrop->quality() = 0;
		newpdrop->state() = cDroping::STATE_NORMAL;
		newpdrop->discover();

		sg_pDungeonSystem->AnimationManager().
			Anime_PlaySE(_T("shoot.wav"),me()->足元地形()->place);

		newpdrop->dpack.integer[6] = ValiableConstant2();

		return sg_pDungeonSystem->魔法発射要請(me(),newpdrop);
	}

	return false;
}
void cMob_ID_15::パッシブ能力(タイミング timing, cValiableField& valiable)
{

	/*if(timing == ダメージ計算防御時_タイミング)
	{
		if(valiable.intsets.val(変数_属性).count(攻撃属性::冷気)
			|| valiable.intsets.val(変数_属性).count(攻撃属性::火)
			|| valiable.intsets.val(変数_属性).count(攻撃属性::水)
			|| valiable.intsets.val(変数_属性).count(攻撃属性::爆発)
			|| valiable.intsets.val(変数_属性).count(攻撃属性::気)
			|| valiable.intsets.val(変数_属性).count(攻撃属性::電気))
		{
			valiable.doubles.val(変数_耐性ボーナス_倍率％) += ValiableConstant1();
		}
	}
	else */if(timing == 魔法接近直前_タイミング)
	{
		//跳ね返し
		if(!valiable.drops.val(変数_対象落ち物)->跳ね返し無効フラグ)
		{
			valiable.doubles.val(変数_汎用ブール) = 0;//効果発揮フラグ

			map<tstring, StyleString> val;
			val[_T("Chara")] = FullName();
			g_Langメッセージ(_T("cMob_ID_15_特殊能力メッセージ"),val);
			
			sg_pDungeonSystem->方向転換要請(me(), valiable.doubles.val(変数_方向)+4);
			sg_pDungeonSystem->魔法発射要請(me(), valiable.drops.val(変数_対象落ち物));
		}
	}
}
//-----------------------------------------------------------------
//コアクマ
int cMob_ID_16::AIセミパッシブ特殊攻撃選択(cValiableField& valiable)
{
	//爆弾岩的な
	if(HP < MHP/2) return true;
	else
	{
		AImemory[0] = AImemory[1] = 0;
		emotion.erase(EMOTION_ASERI);
		return false;
	}
}
int cMob_ID_16::セミパッシブ特殊攻撃効果(cValiableField& valiable)
{
	if(HP < MHP/4 && AImemory[0] && AImemory[1])
	{
		pcLandform pland;
		int a;

		if(sg_pDungeonSystem->即死要請(me(),0,1))
		{
			//ここにアニメを入れる
			EffectFunctions::爆発エフェクト_中(me()->placeX,me()->placeY,130);
			
			map<tstring, StyleString> val;
			val[_T("Chara")] = FullName();
			g_Langメッセージ(_T("cMob_ID_16_特殊攻撃メッセージ"),val);
			
			for(a=0;a<8;a++)
			{
				pland = sg_pDungeonSystem->キャラ視点方角地形(me(),a,1);

				multiset<攻撃属性::攻撃属性> 属性;

				sg_pDungeonSystem->攻撃接近(攻撃作成(
					NULLCHARA,//Attacker
					pland,//Landplase
					ValiableConstant1(),//attackPower
					2,//damageflag
					属性,//attribute
					true
					));
			
			}
		}
		
		return true;
		
	}
	else
	{//停止
		AImemory[0] = 1;//ピンチ
		emotion.insert(EMOTION_ASERI);

		sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(
			&visibleemotion,
			emotion);

		return true;
	}

}
void cMob_ID_16::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	if(timing == 被攻撃直後時_タイミング)
	{
		if(AImemory[0] == 1)
		{
			AImemory[1] = 1;//殴られた
		}
	}
}
	
//-----------------------------------------------------------------
//ウドンゲ
int cMob_ID_17::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);
		if((nowco - youco).dif() == 1 && me()->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
		{
			return true;
		}
	}
	return false;
}
int cMob_ID_17::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{	
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);

		sg_pDungeonSystem->方向転換要請(me(),(youco-nowco).GetAspect());

		pcLandform pland = sg_pDungeonSystem->キャラ前地形(me(),1);

		if(pland->pOnChar != NULL)
		{
			特殊攻撃_アニメ(valiable);
			return sg_pDungeonSystem->錯乱要請(pland->pOnChar);
		}
	}

	return false;
}
//-----------------------------------------------------------------
//アヤ
int cMob_ID_18::AI特殊攻撃選択(cValiableField& valiable)
{
	if(sg_pDungeonSystem->s_turn_speed() < ValiableConstant3() - ValiableConstant4())
	{
		return false;
	}

	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);
		cValiableField val;
		if(sg_pDungeonSystem->八方判定(nowco,youco) && (nowco - youco).dif() <= AI最大間合い(val))
		{
			return true;
		}
	}
	return false;
}
int cMob_ID_18::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{	
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);

		sg_pDungeonSystem->方向転換要請(me(),(youco-nowco).GetAspect());

		pcDroping newpdrop = sg_pDungeonSystem->落ち物生成_設置なし(6019);
		//一本
		newpdrop->quality() = 0;
		newpdrop->state() = cDroping::STATE_NORMAL;
		newpdrop->discover();

		newpdrop->dpack.integer[6] = ValiableConstant2();
		newpdrop->dpack.integer[7] = ValiableConstant1();

		sg_pDungeonSystem->AnimationManager().
			Anime_PlaySE(_T("shoot.wav"),me()->足元地形()->place);

		return sg_pDungeonSystem->魔法発射要請(me(),newpdrop);
	}

	return false;
}
//-----------------------------------------------------------------
//レイム
int cMob_ID_19::AI特殊攻撃選択(cValiableField& valiable)
{
	int size = sg_pDungeonSystem->CharaList().size();
	cCoordinate coo;
	pcCharacter pchara;

	int i;

	for(i=0;i<size;i++)
	{//敵キャラリスト作成
		pchara = sg_pDungeonSystem->CharaList()[i];

		coo.x = pchara->placeX - me()->placeX;
		coo.y = pchara->placeY - me()->placeY;
		if(sg_pDungeonSystem->キャラクター敵対判定(me(),pchara)
			&& coo.dif() <= ValiableConstant2())
		{
			return true;
		}
	}


	return false;
}
int cMob_ID_19::特殊攻撃効果(cValiableField& valiable)
{
	int size = sg_pDungeonSystem->CharaList().size();
	multimap<int,pcCharacter> sortMap;
	int i;
	int maxForcevalue = 0;
	pcCharacter pchara;
	cCoordinate coo;

	for(i=0;i<size;i++)
	{//敵キャラリスト作成
		pchara = sg_pDungeonSystem->CharaList()[i];

		coo.x = pchara->placeX - me()->placeX;
		coo.y = pchara->placeY - me()->placeY;
		if(sg_pDungeonSystem->キャラクター敵対判定(me(),pchara)
			&& coo.dif() <= ValiableConstant2())
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
			sortMap.insert(pair<int,pcCharacter>(coo.dif(),pchara));
		}
	}

	multimap<int,pcCharacter>::iterator itr = sortMap.begin();
	if(sortMap.size())
	{//最も近い敵をサーチする。
		for(;itr != sortMap.end();itr++)
		{

			cValiableField val;
			double temp;
			val.doubles.dim(tstring(_T("toPlaceX")),temp = itr->second->placeX);
			val.doubles.dim(tstring(_T("toPlaceY")),temp = itr->second->placeY);

			特殊攻撃_アニメ(val);
			//EffectFunctions::夢想封印(me()->placeX,me()->placeY,itr->second->placeX,itr->second->placeY,(360*rand())/RAND_MAX);


			multiset<攻撃属性::攻撃属性> 属性;
			//属性.insert(攻撃属性::気);
			
			sg_pDungeonSystem->攻撃接近(攻撃作成(
				me(),//攻撃者
				itr->second->足元地形(),//場所
				ValiableConstant1(),//攻撃力
				false,//定数ダメージでない
				属性,
				true//必中
				));

			return true;
		}
	}


	return false;
}
void cMob_ID_19::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	if(timing == 投擲攻撃接近直前_タイミング)
	{
		if(holdItem.size() == 0)
		{//初回のみ
			if(valiable.drops.val(変数_対象落ち物)->属性.count(落ち物属性::お金))
			{
				valiable.doubles.val(変数_汎用ブール) = 0;//効果発揮フラグ
				
				map<tstring, StyleString> val;
				val[_T("Drop")] = valiable.drops.val(変数_対象落ち物)->FullName();
				val[_T("Chara")] = ShortName();
				g_Langメッセージ(_T("cMob_ID_19_特殊能力メッセージ"),val);
				
				
				if(sg_pDungeonSystem->遠隔拾得要請(me(), valiable.drops.val(変数_対象落ち物))
					&& holdItem.size() >= 1)
				{
					if(holdItem[0]->quality() >= MHP)
					{
						g_Langメッセージ(_T("cMob_ID_19_特殊能力2メッセージ"),val);
						sg_pDungeonSystem->速度減少要請(me(),10,true);
					}
				}
			}
			else if(ValiableConstant3())
			{
				if(MobAbilityIdiom::投擲物ダメージ化CutIn(ValiableConstant3())(me(), timing, valiable)) {
					sg_pDungeonSystem->満腹度減少要請(me(),UseSPOfspecialAttack());
				}
			}
		}
	}
}
//-----------------------------------------------------------------
//リンノスケ
pcEnemyAI cMob_ID_20::Get_kindofAI()
{
	return pcEnemyAI(new ceaiShopOwner);
}
bool cMob_ID_20::すれ違い許可(pcCharacter pchara)
{
	return cMob::すれ違い許可(pchara) && !((sg_pDungeonSystem->店請求金額(me()) > 0) &&
		(足元() != NULL &&
			(
				足元()->属性.count(落ち物属性::階段)
				|| 足元()->ID() == 9015
			)
		));
}
int cMob_ID_20::canTalk()
{
	return !(sg_pDungeonSystem->キャラクター敵対判定(me(),sg_pDungeonSystem->pPlayerChara()));
}
int cMob_ID_20::TalkEvent()
{
	cMob::TalkEvent();
	pcControlLayer pccl;
	pcSelectWindow pcsw;

	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));

	pccl->Init(sg_pDungeonSystem->pDevice_D3D);
	pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));
	
	if(me()->足元地形()->RoomIndex == -1 || !(sg_pDungeonSystem->Map().RoomList[me()->足元地形()->RoomIndex]->ShopFlag))
	{//ここ店じゃないよ！
		pcsw->commandList.push_back(pcCommand(new cCommand_ShopSellHold(me(),g_Lang(_T("持ち物を売る"))) ));
	}
	else
	{
		bool yuka_ari = false;
		if(sg_pDungeonSystem->店支払い金額(me()) > 0)
		{
			pcsw->commandList.push_back(pcCommand(new cCommand_ShopSell(me(),g_Lang(_T("床のアイテムを売る"))) ));
			yuka_ari = true;
		}

		if(sg_pDungeonSystem->店請求金額(me()) > 0)
		{
			pcsw->commandList.push_back(pcCommand(new cCommand_ShopBuy(me(),g_Lang(_T("商品を買う"))) ));
		}
		
		if(!yuka_ari)
		{
			pcsw->commandList.push_back(pcCommand(new cCommand_ShopSellHold(me(),g_Lang(_T("持ち物を売る"))) ));
		}

	}

	if(me()->holdItem.size())
	{
		pcsw->commandList.push_back(pcCommand(new cCommand_ShopBuyHold(me(),g_Lang(_T("店主の持ち物を買う"))) ));
	}

	if(sg_pDungeonSystem->pSaveQuest->ShopDebt < 0)
	{//後払い金
		pcsw->commandList.push_back(pcCommand(new cCommand_ShopChargeMoney(g_Lang(_T("後払い金を請求する"))) ));
	}

	pcsw->commandList.push_back(pcCommand(new cCommandCancel(g_Lang(_T("やめる"))) ));

	int strsize = 0;
	int i;
	for(i=0;i<pcsw->commandList.size();i++)
	{
		strsize = max(strsize,pcsw->commandList[i]->caption.length());
	}
	strsize = max(strsize,3);
	strsize = min(strsize,20);


	pcsw->Init(sg_pDungeonSystem->pDevice_D3D, strsize, pcsw->commandList.size());
	pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(1));
	pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(1));
	

	pcsw->playsound_decide();

	pcGameWindow pcgw;
	pccl->WindowList.push_back(pcgw = pcGameWindow(new cGameWindow));
	pcgw->Init(sg_pDungeonSystem->pDevice_D3D, 14, 2);
	pcgw->setRight(sg_pDungeonSystem->GameScreenInterface.menuPosRight(0));
	pcgw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(1));
	cMoney moneystr;
	map<tstring, StyleString> val;
	moneystr.value = sg_pDungeonSystem->店主持ち金();
	val[_T("Value")] = moneystr.caption();
	pcgw->Text() = g_Lang(_T("店主のもち金"),val);
	pcgw->Text() += _T("\n");
	if(sg_pDungeonSystem->pSaveQuest->ShopDebt < 0)
	{
		moneystr.value = -sg_pDungeonSystem->pSaveQuest->ShopDebt;
		val[_T("Value")] = moneystr.caption();
		pcgw->Text() += g_Lang(_T("貸し金"),val);
	}
	else
	{
		moneystr.value = sg_pDungeonSystem->pSaveQuest->ShopDebt;
		val[_T("Value")] = moneystr.caption();
		pcgw->Text() += g_Lang(_T("ツケ"),val);
	}
	pcgw->enable = true;

	return true;
}
void cMob_ID_20::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	if(timing == 魔法接近直前_タイミング)
	{
		if(ValiableConstant1())
		{
			//跳ね返し
			if(!valiable.drops.val(変数_対象落ち物)->跳ね返し無効フラグ)
			{
				valiable.doubles.val(変数_汎用ブール) = 0;//効果発揮フラグ

				map<tstring, StyleString> val;
				val[_T("Chara")] = FullName();
				g_Langメッセージ(_T("cMob_ID_20_特殊能力1メッセージ"),val);
				
				sg_pDungeonSystem->方向転換要請(me(), valiable.doubles.val(変数_方向)+4 +1 + 7*random());
				sg_pDungeonSystem->魔法発射要請(me(), valiable.drops.val(変数_対象落ち物));
			}
		}
	}
	else if(timing == 投擲攻撃接近直前_タイミング)
	{
		if(ValiableConstant2())
		{
			if(!valiable.drops.val(変数_対象落ち物)->属性.count(落ち物属性::スペカ) &&
				!valiable.drops.val(変数_対象落ち物)->跳ね返し無効フラグ)
			{
				valiable.doubles.val(変数_汎用ブール) = 0;//効果発揮フラグ
				
				pcCharacter pchara = valiable.drops.val(変数_対象落ち物)->投擲者();

				map<tstring, StyleString> val;
				val[_T("Item")] = valiable.drops.val(変数_対象落ち物)->FullName();
				val[_T("Chara")] = FullName();
				g_Langメッセージ(_T("cMob_ID_20_特殊能力2メッセージ"),val);
	
				if(sg_pDungeonSystem->キャラクター敵対判定(sg_pDungeonSystem->pPlayerChara(), pchara))
				{
					sg_pDungeonSystem->方向転換要請(me(), valiable.doubles.val(変数_方向)+4);
				}
				else
				{
					sg_pDungeonSystem->方向転換要請(me(), valiable.doubles.val(変数_方向)+4 +1 + 7*random());
				}
				sg_pDungeonSystem->投擲要請(me(), valiable.drops.val(変数_対象落ち物));
			
			}
		}
	}
	else if(timing == 即死要請直前_タイミング)
	{
		if(ValiableConstant3())
		{
			valiable.doubles[変数_汎用ブール] = 0;//拒否
		}
	}
	else if(timing == ダメージ時_タイミング)
	{
		if(ValiableConstant4())
		{
			//valiable.doubles.val(変数_ダメージ) = min(valiable.doubles.val(変数_ダメージ),ValiableConstant1());
			valiable.doubles.val(変数_ダメージ) = 1;
		}
	}
	else if(timing == ターン終了_タイミング)
	{
		if(holdItem.size())
		{
			if(sg_pDungeonSystem->落ち物配置安全(足元地形()))
			{
				if(me()->足元地形()->RoomIndex != -1 && (sg_pDungeonSystem->Map().RoomList[me()->足元地形()->RoomIndex]->ShopFlag))
				{//ここ店だ！
					int i, size = holdItem.size();
					for(i=0;i<size;i++)
					{
						if(sg_pDungeonSystem->足元設置要請(me(),holdItem[i]))
						{
							break;
						}
					}
				}

			}
			/*
			pcLandform pland = sg_pDungeonSystem->配置安全地形(足元地形(),holdItem[0],true);
			if(pland != NULL)
			{
				sg_pDungeonSystem->ころび要請(me(),1,true,false);
			}
			*/
		}
		int i,flag = 0;
		vector<pcDroping> dropoutputlist;
		sg_pDungeonSystem->キャラクター目視可範囲落ち物リスト(me(), dropoutputlist);
		int size = dropoutputlist.size();
		for(i=0;i<size;i++)
		{
			if(dropoutputlist[i]->属性.count(落ち物属性::階段))
			{
				if(dropoutputlist[i]->現在地形()->RoomIndex != -1 &&
					sg_pDungeonSystem->Map().RoomList[dropoutputlist[i]->現在地形()->RoomIndex]->ShopFlag)
				{
					flag = 1;break;
				}
			}
			else if(dropoutputlist[i]->ID() == 9015)//スキマ
			{
				if(dropoutputlist[i]->現在地形()->RoomIndex != -1 &&
					sg_pDungeonSystem->Map().RoomList[dropoutputlist[i]->現在地形()->RoomIndex]->ShopFlag)
				{
					flag = 1;break;
				}
			}
		}
		if(flag)
		{
			if(Condition.速度度数() <= Condition.デフォルト速度度数())
				sg_pDungeonSystem->速度増加要請(me(),10);
		}
	}
	else if(timing == 被攻撃直後時_タイミング)
	{
		pAI->addEnemy(valiable.charas.val(変数_攻撃者));
	}
}
//-----------------------------------------------------------------
//黄金毛玉

void cMob_ID_21::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	if(timing == ダメージ時_タイミング)
	{
		//valiable.doubles.val(変数_ダメージ) = min(valiable.doubles.val(変数_ダメージ),ValiableConstant1());
		valiable.doubles.val(変数_ダメージ) = ValiableConstant1();
	}
	else if( ValiableConstant2() >= 1 &&
	(
		timing == 眠り追加直前_タイミング
		 || timing == バクスイ追加直前_タイミング
		 || timing == びっくり追加直前_タイミング
		 || timing == 金縛り追加直前_タイミング
		 || timing == 封印追加直前_タイミング
		 || timing == 空振り追加直前_タイミング
		 || timing == 貧乏追加直前_タイミング
		 || timing == 錯乱追加直前_タイミング
		 || timing == 臆病追加直前_タイミング
		 || timing == 嫉妬追加直前_タイミング
		 || timing == 無意識追加直前_タイミング
		 || timing == 狂乱追加直前_タイミング
		 || timing == 泥酔追加直前_タイミング
		 || timing == 氷付け追加直前_タイミング
		 || timing == 脱力追加直前_タイミング
		 || timing == 元気追加直前_タイミング
		 || timing == 軟弱追加直前_タイミング
		 || timing == 頑強追加直前_タイミング
		 || timing == 鳥目追加直前_タイミング
		 || timing == 死の誘い追加直前_タイミング
		)&& !cMob_能力仕様フラグID_exist(valiable,ID()))
	{
		valiable.doubles[変数_汎用ブール] = 0;//拒否
		cMob_能力仕様フラグID_dim(valiable,ID()) = 1;
	}
	else if(timing == 死亡直前_タイミング)
	{
		pcDroping pdrop = sg_pDungeonSystem->落ち物生成_設置なし(ValiableConstant3());
		sg_pDungeonSystem->遠隔拾得要請_持ち物余白無考慮(me(),pdrop);
	}
}

//-----------------------------------------------------------------
//サナエ
int cMob_ID_22::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{
		
		if(FindUtility::キャラ８方向射線で範囲以内(me(),pchara,AI最大間合い(valiable)))
		{
			return true;
		}
		/*
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);
		if((nowco - youco).dif() == 1 && me()->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
		{
			return true;
		}
		*/
	}
	return false;
}
int cMob_ID_22::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{	
		if(FindUtility::キャラ８方向射線で範囲以内(me(),pchara,AI最大間合い(valiable)))
		{
			GameIdiom::キャラの方を向く(me(),pchara);

			vector<int> IDlist;
			if(ValiableConstant1()) IDlist.push_back(ValiableConstant1());
			if(ValiableConstant2()) IDlist.push_back(ValiableConstant2());
			if(ValiableConstant3()) IDlist.push_back(ValiableConstant3());
			if(ValiableConstant4()) IDlist.push_back(ValiableConstant4());
			if(!IDlist.empty())
			{
				pcDroping newpdrop = sg_pDungeonSystem->落ち物生成_設置なし(IDlist[IDlist.size()*random()]);
				//一本
				newpdrop->state() = cDroping::STATE_NORMAL;
				newpdrop->discover();

				sg_pDungeonSystem->AnimationManager().
					Anime_PlaySE(_T("shoot.wav"),me()->足元地形()->place);

				return sg_pDungeonSystem->魔法発射要請(me(),newpdrop);
			}
		}
		/*
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);

		sg_pDungeonSystem->方向転換要請(me(),(youco-nowco).GetAspect());

		pcLandform pland = sg_pDungeonSystem->キャラ前地形(me(),1);

		if(pland->pOnChar != NULL)
		{
			
			pcCharacter pchara = pland->pOnChar;
			if(pchara->雑魚属性())
			{
				sg_pDungeonSystem->封印要請(pchara,ValiableConstant3());
				return true;
			}
			else
			{
		
				vector<pcDroping> vpdrop;

				if(pchara->attackequipment != NULL) vpdrop.push_back(pchara->attackequipment);
				if(pchara->defenseequipment != NULL) vpdrop.push_back(pchara->defenseequipment);


				if(ValiableConstant2())
				{
					int k;
					for(k=0;k<pchara->最大装備スペル数();k++)
					{
						if(pchara->spellequipment[k] != NULL) vpdrop.push_back(pchara->spellequipment[k]);
					}
				}

				
					
				if(!vpdrop.empty())
				{
					pcDroping pdrop = vpdrop[vpdrop.size()*random()];

					特殊攻撃_アニメ(valiable);

					int i;
					for(i=0;i<ValiableConstant1();i++)
					{
						sg_pDungeonSystem->アイテム劣化(pdrop);
					}
					return true;
				}

			}
		}
		*/
	}

	return false;
}
//-----------------------------------------------------------------
//メイリン
int cMob_ID_23::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	/*
	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);
		if((nowco - youco).dif() >= 2 && (nowco - youco).dif() <= ValiableConstant1() && sg_pDungeonSystem->八方判定(nowco,youco))
		{
			return true;
		}
	}
	*///気の矢発射

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);
		if((nowco - youco).dif() == 1 && me()->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
		{
			return true;
		}
	}
	return false;
}
int cMob_ID_23::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	/*
	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{	
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);

		sg_pDungeonSystem->方向転換要請(me(),(youco-nowco).GetAspect());

		pcDroping newpdrop = sg_pDungeonSystem->落ち物生成_設置なし(2000);
		//一本
		newpdrop->quality() = 0;
		newpdrop->state() = cDroping::STATE_NORMAL;
		newpdrop->discover();

		return sg_pDungeonSystem->投擲要請(me(),newpdrop,DRAWTYPE_ATTACKDRAW,ValiableConstant1());
	}
	*///気の矢発射

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);
		
		sg_pDungeonSystem->方向転換要請(me(),(youco-nowco).GetAspect());

		pcLandform pland = sg_pDungeonSystem->キャラ前地形(me(),1);

		if(pland->pOnChar != NULL && (nowco - youco).dif() == 1 && me()->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
		{
			AImemory[0] = 1;
			sg_pDungeonSystem->直接攻撃要請(me());
			return true;
		}

	}

	return false;
}
void cMob_ID_23::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	/*
	if(timing == 投擲攻撃接近直前_タイミング)
	{
		if(!valiable.doubles.val(変数_貫通))
		{

			valiable.doubles.val(変数_汎用ブール) = 0;//効果発揮フラグ
			
			map<tstring, StyleString> val;
			val[_T("Drop")] = valiable.drops.val(変数_対象落ち物)->FullName();
			val[_T("Chara")] = FullName();
			g_Langメッセージ(_T("cMob_ID_23_特殊能力メッセージ"),val);

			sg_pDungeonSystem->方向転換要請(me(), valiable.doubles.val(変数_方向)+4);
			sg_pDungeonSystem->投擲要請(me(), valiable.drops.val(変数_対象落ち物));
		}
	}
	*/
	/*if(timing == ダメージ計算防御時_タイミング)
	{
		if(valiable.intsets.val(変数_属性).count(攻撃属性::気))
		{
			valiable.doubles.val(変数_耐性ボーナス_倍率％) += ValiableConstant2();
		}
		if(valiable.intsets.val(変数_属性).count(攻撃属性::落ち物))
		{
			valiable.doubles.val(変数_耐性ボーナス_倍率％) -= ValiableConstant3();
		}
	}
	else */if(timing == ダメージ計算攻撃時_タイミング && AImemory[0])
	{
		AImemory[0] = 0;
		valiable.doubles.val(変数_耐性ボーナス_倍率％) -= ValiableConstant1();
	}
	
}

//-----------------------------------------------------------------
//イク
int cMob_ID_25::AI特殊攻撃選択(cValiableField& valiable)
{
	//pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;
	vector<pcCharacter> vpchara;
	sg_pDungeonSystem->キャラクター目視可範囲キャラリスト(me(),vpchara);
	int i;
	for(i=0;i<vpchara.size();i++)
	{
		if(me() != vpchara[i])
		{
			return true;
		}
	}
	return false;
}
int cMob_ID_25::特殊攻撃効果(cValiableField& valiable)
{
	//pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;
	vector<pcCharacter> vpchara,vpobject;
	sg_pDungeonSystem->キャラクター目視可範囲キャラリスト(me(),vpchara);
	int i;
	for(i=0;i<vpchara.size();i++)
	{
		if(me() != vpchara[i])
		{
			vpobject.push_back(vpchara[i]);
		}
	}
	if(!vpobject.empty())
	{
		pcCharacter pchara = vpobject.at(vpobject.size()*random());
		if(pchara != NULL)
		{

			/*
			valiable.lands[変数_攻撃場所] = pchara->足元地形();
			特殊攻撃_アニメ(valiable);
			return sg_pDungeonSystem->びっくり要請(pchara,1);
			*/

			valiable.lands[変数_攻撃場所] = pchara->足元地形();
			特殊攻撃_アニメ(valiable);
			multiset<攻撃属性::攻撃属性> 属性;
			属性.insert(攻撃属性::雷);
			
			sg_pDungeonSystem->攻撃接近(攻撃作成(
				me(),//攻撃者
				pchara->足元地形(),//場所
				ValiableConstant2(),//攻撃力
				定数ダメージフラグ,//定数ダメージ
				属性,
				true//必中
				));

		}
	}
	return false;
}
void cMob_ID_25::パッシブ能力(タイミング timing, cValiableField& valiable)
{

	if(timing == ダメージ計算攻撃時_タイミング)
	{
		valiable.intsets.val(変数_属性).insert(攻撃属性::電気);
	}
	/*else if(timing == ダメージ計算防御時_タイミング)
	{
		if(valiable.intsets.val(変数_属性).count(攻撃属性::電気))
		{
			valiable.doubles.val(変数_耐性ボーナス_倍率％) += ValiableConstant1();
		}
		if(valiable.intsets.val(変数_属性).count(攻撃属性::冷気))
		{
			if(Condition.速度度数() >= Condition.デフォルト速度度数())
				sg_pDungeonSystem->速度減少要請(me(),10);
		}
	}*/
}
int cMob_ID_25::特殊攻撃_アニメ(cValiableField& valiable)
{
	EffectFunctions::落雷エフェクト(valiable.lands[変数_攻撃場所]->placeX, valiable.lands[変数_攻撃場所]->placeY);
	return true;
}
//-----------------------------------------------------------------
//シズハ
pcEnemyAI cMob_ID_26::Get_kindofAI()
{
	return pcEnemyAI(new ceaiEscape);
}
int cMob_ID_26::AI特殊攻撃選択(cValiableField& valiable)
{
	//エスケープの場合は移動先に敵がいないので検索する必要がある。
	pcCharacter pchara;

	int a;
	for(a=0;a<8;a++)
	{
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco;
		youco.SetAspect(a+me()->aspect);
		youco += nowco;

		pchara = sg_pDungeonSystem->Map().Land(youco)->pOnChar;

		if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
		{

			if((nowco - youco).dif() == 1 && me()->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
			{
				return true;
			}
		}
	}
	return false;
}
int cMob_ID_26::特殊攻撃効果(cValiableField& valiable)
{
	//エスケープの場合は移動先に敵がいないので検索する必要がある。
	pcCharacter pchara;

	int a;
	for(a=0;a<8;a++)
	{
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco;
		youco.SetAspect(a+me()->aspect);
		youco += nowco;

		pchara = sg_pDungeonSystem->Map().Land(youco)->pOnChar;

		if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
		{	
			cCoordinate nowco(me()->placeX,me()->placeY);
			cCoordinate youco(pchara->placeX,pchara->placeY);

			sg_pDungeonSystem->方向転換要請(me(),(youco-nowco).GetAspect());

			pcLandform pland = sg_pDungeonSystem->キャラ前地形(me(),1);

			if(pland->pOnChar != NULL)
			{
				特殊攻撃_アニメ(valiable);
				return sg_pDungeonSystem->満腹度減少要請(pland->pOnChar,ValiableConstant2());
			}
		}

	}
	
	return false;
}
void cMob_ID_26::パッシブ能力(タイミング timing, cValiableField& valiable)
{

	if(timing == 死亡直前_タイミング)
	{
		pcDroping pdrop = sg_pDungeonSystem->落ち物生成_設置なし(ValiableConstant1());
		sg_pDungeonSystem->遠隔拾得要請_持ち物余白無考慮(me(),pdrop);
	}
	/*else if(timing == ダメージ計算防御時_タイミング)
	{
		if(valiable.intsets.val(変数_属性).count(攻撃属性::冷気))
		{
			if(Condition.速度度数() >= Condition.デフォルト速度度数())
				sg_pDungeonSystem->速度減少要請(me(),10);
		}
	}*/
}
//-----------------------------------------------------------------
//スイカ
int cMob_ID_27::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);
		if((nowco - youco).dif() == 1 && me()->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
		{
			return true;
		}
	}
	return false;
}
int cMob_ID_27::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{	
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);

		sg_pDungeonSystem->方向転換要請(me(),(youco-nowco).GetAspect());

		pcLandform pland = sg_pDungeonSystem->キャラ前地形(me(),1);

		if(pland->pOnChar != NULL)
		{
			特殊攻撃_アニメ(valiable);
			int eff = 0;

			int i;
			for(i=0;i<8;i++)
			{
				pland = sg_pDungeonSystem->キャラ視点方角地形(me(), i, 1);
				eff += sg_pDungeonSystem->泥酔要請(pland->pOnChar,ValiableConstant1());
			}
			return true;
		}
	}

	return false;
}
void cMob_ID_27::パッシブ能力(タイミング timing, cValiableField& valiable)
{

	if(timing == 被攻撃直後時_タイミング)
	{
		if(ValiableConstant3() <= Stomach && !死亡())
		{//消費SP十分
			if(ValiableConstant2()/100.0 > random())
			{//分身
				sg_pDungeonSystem->満腹度減少要請(me(),ValiableConstant3(),false);
				map<tstring, StyleString> val;
				//val[_T("Drop")] = valiable.drops.val(変数_対象落ち物)->FullName();
				val[_T("Chara")] = FullName();
				g_Langメッセージ(_T("cMob_ID_27_特殊能力メッセージ"),val);
				sg_pDungeonSystem->分身要請(me());
			}
		}
	}
	/*else if(timing == ダメージ計算防御時_タイミング)
	{

		if(valiable.intsets.val(変数_属性).count(攻撃属性::水))
		{
			if(Condition.速度度数() >= Condition.デフォルト速度度数())
				sg_pDungeonSystem->速度減少要請(me(),10);
		}
	}*/
}

//-----------------------------------------------------------------
//スワコ
int cMob_ID_28::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);
		if((nowco - youco).dif() == 1 && me()->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
		{
			return true;
		}
	}
	return false;
}
int cMob_ID_28::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{	
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);

		sg_pDungeonSystem->方向転換要請(me(),(youco-nowco).GetAspect());

		pcLandform pland = sg_pDungeonSystem->キャラ前地形(me(),1);

		if(pland->pOnChar != NULL)
		{
				
			pcCharacter pchara = pland->pOnChar;

			map<tstring, StyleString> val;
			//val[_T("Drop")] = valiable.drops.val(変数_対象落ち物)->FullName();
			val[_T("Chara")] = FullName();

			if(pchara->雑魚属性())
			{
				sg_pDungeonSystem->速度減少要請(pchara, 10);
			}
			else
			{
				int i;
				if(!pchara->holdItem.empty())
				{
					int index = pchara->holdItem.size()*random();
					sg_pDungeonSystem->アイテム呪い(pchara->holdItem[index]);
				}
			}

			特殊攻撃_アニメ(valiable);
			sg_pDungeonSystem->脱力要請(me(),ValiableConstant1(),ValiableConstant2(),false);
			sg_pDungeonSystem->軟弱要請(me(),ValiableConstant1(),ValiableConstant2(),false);

			return true;
		}
	}

	return false;
}
void cMob_ID_28::パッシブ能力(タイミング timing, cValiableField& valiable)
{

	/*if(timing == ダメージ計算防御時_タイミング)
	{
		if(valiable.intsets.val(変数_属性).count(攻撃属性::水))
		{
			valiable.doubles.val(変数_耐性ボーナス_倍率％) += ValiableConstant3();
		}
		if(valiable.intsets.val(変数_属性).count(攻撃属性::冷気))
		{
			if(Condition.速度度数() >= Condition.デフォルト速度度数())
				sg_pDungeonSystem->速度減少要請(me(),10);
		}
	}*/
}

//-----------------------------------------------------------------
//テンシ
int cMob_ID_29::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);
		if((nowco - youco).dif() == 1 && me()->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
		{
			return true;
		}
	}
	return false;
}
int cMob_ID_29::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{	
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);

		sg_pDungeonSystem->方向転換要請(me(),(youco-nowco).GetAspect());

		pcLandform pland = sg_pDungeonSystem->キャラ前地形(me(),1);

		if(pland->pOnChar != NULL)
		{
			特殊攻撃_アニメ(valiable);

			int a;
			for(a=0;a<8;a++)
			{
				pland = sg_pDungeonSystem->キャラ視点方角地形(me(),a,1);
				if(pland->pOnChar != NULL)
				{
					/*
					pcCharacter pcharafuki = pland->pOnChar;
					if(sg_pDungeonSystem->吹き飛ばし要請(me(),pcharafuki,a+me()->aspect,ValiableConstant2(),ValiableConstant1()))
					{
						sg_pDungeonSystem->ころび要請(pcharafuki,ValiableConstant3());
					}
					*/
					pcCharacter pcharafuki = pland->pOnChar;
					sg_pDungeonSystem->投げ飛ばし要請(me(),pcharafuki,
						sg_pDungeonSystem->隣接地形(pland, 8*random(), ValiableConstant2()),
						 ValiableConstant1(), true, 2.0);
						 
				}
			}

			return true;
		}
	}
	
	return false;
}
int cMob_ID_29::特殊攻撃_アニメ(cValiableField& valiable)
{
	EffectFunctions::Mob29エフェクト(me()->placeX,me()->placeY);
	return false;
}
void cMob_ID_29::パッシブ能力(タイミング timing, cValiableField& valiable)
{


	if(timing == 被攻撃直後時_タイミング)
	{

		sg_pDungeonSystem->頑強要請(me(),ValiableConstant3(),ValiableConstant4(),true);
		
	}
}
//-----------------------------------------------------------------
//パルスィ
int cMob_ID_30::AI特殊攻撃選択(cValiableField& valiable)
{
	/*
	//キャラ検索する。
	pcCharacter pchara;

	int a;
	for(a=0;a<8;a++)
	{
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco;
		youco.SetAspect(a+me()->aspect);
		youco += nowco;

		pchara = sg_pDungeonSystem->Map().Land(youco)->pOnChar;

		if(pchara != NULL && !sg_pDungeonSystem->キャラクター敵対判定(me(),pchara)
			&& !pchara->Condition.嫉妬状態())
		{//みかた

			if((nowco - youco).dif() == 1 && me()->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
			{
				return true;
			}
		}
	}
	*/
	return false;
}
int cMob_ID_30::特殊攻撃効果(cValiableField& valiable)
{
	/*
	//エスケープの場合は移動先に敵がいないので検索する必要がある。
	pcCharacter pchara;

	int a;
	for(a=0;a<8;a++)
	{
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco;
		youco.SetAspect(a+me()->aspect);
		youco += nowco;

		pchara = sg_pDungeonSystem->Map().Land(youco)->pOnChar;

		if(pchara != NULL && !sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
		{	
			cCoordinate nowco(me()->placeX,me()->placeY);
			cCoordinate youco(pchara->placeX,pchara->placeY);

			sg_pDungeonSystem->方向転換要請(me(),(youco-nowco).GetAspect());

			pcLandform pland = sg_pDungeonSystem->キャラ前地形(me(),1);

			if(pland->pOnChar != NULL)
			{
				特殊攻撃_アニメ(valiable);
				if(sg_pDungeonSystem->嫉妬要請(pland->pOnChar,ValiableConstant1()))
				{
					pland->pOnChar->Condition.とても強い刺激();
					return true;
				}
			}
		}

	}
	*/
	return false;
}
void cMob_ID_30::パッシブ能力(タイミング timing, cValiableField& valiable)
{


	if(timing == 被攻撃直後時_タイミング)
	{
		if(me()->Condition.力係数() >= 2.0 && !me()->Condition.嫉妬状態())
		{
			sg_pDungeonSystem->元気要請(me(), 0, -1, 0);
			sg_pDungeonSystem->嫉妬要請(me(), ValiableConstant3(), 1);
		}
		sg_pDungeonSystem->元気要請(me(),ValiableConstant2(),ValiableConstant3(),true);
		//sg_pDungeonSystem->頑強要請(me(),ValiableConstant2(),ValiableConstant3(),false);
		
	}
}

//-----------------------------------------------------------------
//ミノリコ
int cMob_ID_31::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);
		if((nowco - youco).dif() == 1 && me()->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
		{
			return true;
		}
	}
	return false;
}
int cMob_ID_31::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{	
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);

		sg_pDungeonSystem->方向転換要請(me(),(youco-nowco).GetAspect());

		pcLandform pland = sg_pDungeonSystem->キャラ前地形(me(),1);

		cValiableField valf;
		valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
		sg_pDungeonSystem->CutInM().CutIn(pchara,イモライズ直前_タイミング,valf);

		valiable.lands[変数_攻撃場所] = pland;
		if(pland->pOnChar != NULL && valf.doubles.val(変数_汎用ブール))
		{
			
			
			pcCharacter pchara = pland->pOnChar;
			if(pchara->雑魚属性())
			{
				特殊攻撃_アニメ(valiable);

				if(sg_pDungeonSystem->即死要請(pchara,ValiableConstant3()))
				{
					sg_pDungeonSystem->落ち物生成(ValiableConstant1(),NULL,pland);
				}
				return true;
			}
			else
			{
		
				vector<pcDroping> vpdrop = FindUtility::装備品以外の表面手持ちアイテム店売り除く(pchara);
				if(!vpdrop.empty())
				{
					pcDroping pdrop = vpdrop[vpdrop.size()*random()];

					特殊攻撃_アニメ(valiable);

					pcDroping newpdrop = sg_pDungeonSystem->落ち物生成_設置なし(ValiableConstant1());
					if(newpdrop != NULL)
					{
						map<tstring, StyleString> val;
						val[_T("ItemA")] = pdrop->FullName();
						val[_T("ItemB")] = newpdrop->FullName();
						g_Langメッセージ(_T("cMob_ID_31_特殊能力メッセージ"),val);
						sg_pDungeonSystem->落ち物変化(pdrop,newpdrop);
						
						return true;
					}
				}

			}

			
		}
	}

	
	return false;
}
void cMob_ID_31::パッシブ能力(タイミング timing, cValiableField& valiable)
{


	if(timing == 投擲攻撃接近直前_タイミング)
	{
		if(valiable.drops.val(変数_対象落ち物)->属性.count(落ち物属性::食べ物))
		{
			map<tstring, StyleString> val;
			val[_T("Chara")] = me()->FullName();
			val[_T("Item")] = valiable.drops.val(変数_対象落ち物)->FullName();
			g_Langメッセージ(_T("cMob_ID_31_特殊能力2メッセージ"),val);
					
			sg_pDungeonSystem->即死要請(me(),false,true);
			valiable.doubles.val(変数_汎用ブール) = 0;//接近回避
			sg_pDungeonSystem->落ち物破壊要請(valiable.drops.val(変数_対象落ち物));
		}
	}
	/*if(timing == ダメージ計算防御時_タイミング)
	{
		if(valiable.intsets.val(変数_属性).count(攻撃属性::冷気))
		{
			if(Condition.速度度数() >= Condition.デフォルト速度度数())
				sg_pDungeonSystem->速度減少要請(me(),10);
		}
	}*/
}
int cMob_ID_31::特殊攻撃_アニメ(cValiableField& valiable)
{

	EffectFunctions::Mob31エフェクト(valiable.lands[変数_攻撃場所]->placeX,valiable.lands[変数_攻撃場所]->placeY);

	return true;
}

//-----------------------------------------------------------------
//モミジ
pcLandform cMob_ID_32::AIお気に入り地形(cValiableField& valiable)
{
	if(足元地形()->RoomIndex == -1) return NULLLAND;

	vector<pcCharacter> vpchara;
	sg_pDungeonSystem->キャラクター目視可範囲キャラリスト(me(),vpchara);

	int i;
	for(i=0;i<vpchara.size();i++)
	{
		if(!sg_pDungeonSystem->キャラクター敵対判定(me(),vpchara[i])
			&& vpchara[i]->Condition.眠りで行動不能である())
		{
			return vpchara[i]->足元地形();
		}
	}

	return NULLLAND;
}
int cMob_ID_32::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && !sg_pDungeonSystem->キャラクター敵対判定(me(),pchara)
		&& pchara->Condition.眠りで行動不能である())
	{
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);
		if((nowco - youco).dif() == 1 && me()->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
		{
			return true;
		}
	
	}

	return false;
}
int cMob_ID_32::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;


	if(pchara != NULL && !sg_pDungeonSystem->キャラクター敵対判定(me(),pchara)
		&& pchara->Condition.眠りで行動不能である())
	{
		GameIdiom::悪性異常状態治療要請(pchara);
	
	}
	return false;
}
void cMob_ID_32::パッシブ能力(タイミング timing, cValiableField& valiable)
{

	if(timing == ダメージ計算防御時_タイミング)
	{
		if(valiable.doubles.exist(変数_方向))
		{
			int as = safeAspect( valiable.doubles.val(変数_方向) - me()->aspect);
			if(as >= 3 && as <= 5)
			{
				valiable.doubles.dim(変数_耐性ボーナス_倍率％) += ValiableConstant1();
			
			}
			else
			{
				valiable.doubles.dim(変数_耐性ボーナス_倍率％) -= ValiableConstant2();

			}
		}
	}

}

//-----------------------------------------------------------------
//ユウギ
int cMob_ID_33::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);
		if((nowco - youco).dif() == 1 && me()->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
		{
			int i;
			vector<pcLandform> vpland;

			vector<pcDroping>& vpdrop = sg_pDungeonSystem->DropList();
			for(i=0;i<vpdrop.size();i++)
			{
				if(vpdrop[i]->落ちてる() && vpdrop[i]->属性.count(落ち物属性::罠))
				{
					youco.Set(vpdrop[i]->placeX,vpdrop[i]->placeY);

					if((nowco - youco).dif() <= ValiableConstant1())
					{
						vpland.push_back(vpdrop[i]->現在地形());
					}
				}
			}
			vector<pcCharacter>& vpchara = sg_pDungeonSystem->CharaList();
			for(i=0;i<vpchara.size();i++)
			{
				if(vpchara[i] != pchara && vpchara[i] != me())
				{
					youco.Set(vpchara[i]->placeX,vpchara[i]->placeY);

					if((nowco - youco).dif() <= ValiableConstant1())
					{
						vpland.push_back(vpchara[i]->足元地形());
					}
				}
			}
			if(vpland.size())
			{
				AImemory[0] = 0;
				return true;
			}
		}
	
		nowco.Set(me()->placeX,me()->placeY);
		youco.Set(pchara->placeX,pchara->placeY);
		if((nowco - youco).dif() <= ValiableConstant1())
		{
			//味方を投げることが出来るか検討する
			int a;
			for(a=0;a<8;a++)
			{
				youco.SetAspect(a+me()->aspect);
				youco += nowco;
			
				pcCharacter pthrowchara = sg_pDungeonSystem->Map().Land(youco)->pOnChar;

				if(pthrowchara != NULL && pchara != pthrowchara)
				{//誰でもいいけど投げれる
					if(pthrowchara->CharaAttribute().count(キャラ属性::弾幕))
					{
						continue;
					}
					AImemory[0] = 1;
					return true;
				}
			}
		}
	
	}

	return false;
}
int cMob_ID_33::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{
		if(AImemory[0])
		{
			//味方を投げることが出来る
			cCoordinate nowco(me()->placeX,me()->placeY);
			cCoordinate youco(pchara->placeX,pchara->placeY);
			
			int a;
			for(a=0;a<8;a++)
			{
				youco.SetAspect(a+me()->aspect);
				youco += nowco;
			
				pcCharacter pthrowchara = sg_pDungeonSystem->Map().Land(youco)->pOnChar;

				if(pthrowchara != NULL)
				{//誰でもいいけど投げれる

					if(pthrowchara->CharaAttribute().count(キャラ属性::弾幕))
					{
						continue;
					}

					sg_pDungeonSystem->方向転換要請(me(),(youco-nowco).GetAspect());


					pcLandform ptgtland = pchara->足元地形();

					特殊攻撃_アニメ(valiable);

					sg_pDungeonSystem->投げ飛ばし要請(me(),pthrowchara,ptgtland,ValiableConstant2(),true,0.8);
					return true;
				}
			}

		}
		else
		{
			cCoordinate nowco(me()->placeX,me()->placeY);
			cCoordinate youco(pchara->placeX,pchara->placeY);

			sg_pDungeonSystem->方向転換要請(me(),(youco-nowco).GetAspect());

			pcLandform pland = sg_pDungeonSystem->キャラ前地形(me(),1);

			valiable.lands[変数_攻撃場所] = pland;
			if(pland->pOnChar != NULL)
			{
				
				pchara = pland->pOnChar;

				pcLandform ptgtland;
				int i;
				vector<pcLandform> vpland;

				vector<pcDroping>& vpdrop = sg_pDungeonSystem->DropList();
				for(i=0;i<vpdrop.size();i++)
				{
					if(vpdrop[i]->落ちてる() && vpdrop[i]->属性.count(落ち物属性::罠))
					{
						youco.Set(vpdrop[i]->placeX,vpdrop[i]->placeY);

						if((nowco - youco).dif() <= ValiableConstant1())
						{
							vpland.push_back(vpdrop[i]->現在地形());
						}
					}
				}
				vector<pcCharacter>& vpchara = sg_pDungeonSystem->CharaList();
				for(i=0;i<vpchara.size();i++)
				{
					if(vpchara[i] != pchara && vpchara[i] != me())
					{
						youco.Set(vpchara[i]->placeX,vpchara[i]->placeY);

						if((nowco - youco).dif() <= ValiableConstant1())
						{
							vpland.push_back(vpchara[i]->足元地形());
						}
					}
				}
				
				ptgtland = vpland[vpland.size()*random()];

				特殊攻撃_アニメ(valiable);

				sg_pDungeonSystem->投げ飛ばし要請(me(),pchara,ptgtland,ValiableConstant2(),true,0.5);
				return true;
			}

		}
	}


	return false;
}
int cMob_ID_33::特殊攻撃_アニメ(cValiableField& valiable)
{
	pcLandform pland = sg_pDungeonSystem->キャラ前地形(me(),1);
	c4DVector start,end;
	//投げアニメーション2
	start.set(me()->placeX,me()->placeY,0,0);
	end.set(pland->placeX,pland->placeY,0,0);
	end = (end+start)/2;
	sg_pDungeonSystem->AnimationManager().Anime_Throw(me(),start,end,ROUTE_LOW,DRAWTYPE_NORMAL,0.8);
	
	sg_pDungeonSystem->AnimationManager().Anime_Wait(pland->placeX,pland->placeY,15);
	sg_pDungeonSystem->AnimationManager().Anime_Throw(me(),end,start,ROUTE_LOW,DRAWTYPE_NORMAL,1.7);

	return true;
}
//-----------------------------------------------------------------
//キスメ
int cMob_ID_34::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	return sg_pDungeonSystem->キャラクター敵対判定(me(),pchara);
}
int cMob_ID_34::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(sg_pDungeonSystem->キャラクター敵対判定(me(),pchara)
		&& FindUtility::キャラの距離(me(),pchara) >= 2)
	{
		vector<pcLandform> landlist;
		桂馬地形リスト(me(), landlist);
		pcLandform pland = FindUtility::ターゲット地形検索(landlist, me(), &地形評価);

		if(pland){
			GameIdiom::キャラの方を向く(me(),pchara);

			sg_pDungeonSystem->投げ飛ばし要請(me(),me(),pland,ValiableConstant1(),false,1.7);
					
			return true;
		}
	}
	return false;
}
double cMob_ID_34::地形評価(pcCharacter me, pcLandform land)
{
	double value = 100;
	int x,y;
	if(!land->through(false,false,false))
	{
		return 0;
	}
	if(me->pAI->u_敵サーチ_キャラ優先度優先(x, y))
	{
		cCoordinate place(x,y);
		value -= (place - land->place).dif();
	}
	pcCharacter pchara = land->pOnChar;
	if(pchara && sg_pDungeonSystem->キャラクター敵対判定(me,pchara))
	{
		value += pchara->被攻撃優先度()*100;
	}
	return value;
}
void cMob_ID_34::桂馬地形リスト(pcCharacter me, vector<pcLandform>& landlist)
{
	int x = me->placeX, y = me->placeY; 
	landlist.push_back(sg_pDungeonSystem->Map().Land(x-2,y+1));
	landlist.push_back(sg_pDungeonSystem->Map().Land(x-2,y-1));
	landlist.push_back(sg_pDungeonSystem->Map().Land(x+2,y+1));
	landlist.push_back(sg_pDungeonSystem->Map().Land(x+2,y-1));
	landlist.push_back(sg_pDungeonSystem->Map().Land(x+1,y-2));
	landlist.push_back(sg_pDungeonSystem->Map().Land(x-1,y-2));
	landlist.push_back(sg_pDungeonSystem->Map().Land(x+1,y+2));
	landlist.push_back(sg_pDungeonSystem->Map().Land(x-1,y+2));
}
//-----------------------------------------------------------------
//オリン
int cMob_ID_35::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	return sg_pDungeonSystem->キャラクター敵対判定(me(),pchara);
}
int cMob_ID_35::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(sg_pDungeonSystem->キャラクター敵対判定(me(),pchara)
		&& FindUtility::キャラの距離(me(),pchara) >= 2)
	{
		vector<pcLandform> landlist;
		ジャンプ地形リスト(me(), landlist);
		pcLandform pland = FindUtility::ターゲット地形検索(landlist, me(), &地形評価);

		GameIdiom::キャラの方を向く(me(),pchara);

		sg_pDungeonSystem->投げ飛ばし要請(me(),me(),pland,ValiableConstant1(),false,1.9);
				
		return true;
	}
	return false;
}
double cMob_ID_35::地形評価(pcCharacter me, pcLandform land)
{
	double value = 100;
	int x,y;
	if(me->pAI->u_敵サーチ_キャラ優先度優先(x, y))
	{
		cCoordinate place(x,y);
		value -= (place - land->place).dif();
	}
	pcCharacter pchara = land->pOnChar;
	if(pchara && sg_pDungeonSystem->キャラクター敵対判定(me,pchara))
	{
		value += pchara->被攻撃優先度()*100;
	}
	return value;
}
void cMob_ID_35::ジャンプ地形リスト(pcCharacter me, vector<pcLandform>& landlist)
{
	int tempaspect;
	for(tempaspect = 0; tempaspect < 8; tempaspect++)
	{
		landlist.push_back(sg_pDungeonSystem->キャラ視点方角地形(me,tempaspect,2));
	}
}
void cMob_ID_35::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	if (死亡直前_タイミング == timing)
	{
		cEvent_OnryouSpawn::pNewEvent(cEvent::PRIORITY_MIDDLE, placeX, placeY, CLASS)->startEvent();
	}
}

//-----------------------------------------------------------------
//ヤマメ
int cMob_ID_36::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	return FindUtility::キャラが敵で直接攻撃可能か(me(),pchara);
}
int cMob_ID_36::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(FindUtility::キャラが敵で直接攻撃可能か(me(),pchara))
	{
		GameIdiom::キャラの方を向く(me(),pchara);
		pchara = sg_pDungeonSystem->キャラ前地形(me(),1)->pOnChar;
		if(pchara)
		{
			sg_pDungeonSystem->軟弱要請(pchara,ValiableConstant1(),ValiableConstant2());
			if(ValiableConstant3() > 0) {
				sg_pDungeonSystem->病気要請(pchara,ValiableConstant4());
			}
		}

		return true;
	}
	return false;
}
//-----------------------------------------------------------------
//サトリ
int cMob_ID_37::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	return FindUtility::キャラが敵で直接攻撃可能か(me(),pchara);
}
int cMob_ID_37::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(FindUtility::キャラが敵で直接攻撃可能か(me(),pchara))
	{
		GameIdiom::キャラの方を向く(me(),pchara);
		if(ValiableConstant1())//コイシ
		{
			召喚(CCHARACTER_cMob_BASE_ID_NUM+38,pchara);
		}
		if(ValiableConstant2())//ウツホ
		{
			召喚(CCHARACTER_cMob_BASE_ID_NUM+39,pchara);
		}
		if(ValiableConstant3())//オリン
		{
			召喚(CCHARACTER_cMob_BASE_ID_NUM+35,pchara);
		}
		return true;
	}
	return false;
}
int cMob_ID_37::召喚(int ID, pcCharacter enemy)
{
	pcCharacter friendchara = sg_pDungeonSystem->キャラクター生成(ID,me()->CLASS,me()->Forse,sg_pDungeonSystem->キャラ視点方角地形(me(),4,1));
	if(friendchara)
	{
		map<tstring, StyleString> val;
		val[_T("Chara")] = ShortName();
		val[_T("Chara2")] = friendchara->ShortName();
		g_Langメッセージ(_T("cMob_ID_37_特殊能力メッセージ"),val);

		//サトリの召喚した敵はお腹いっぱい
		//friendchara->Stomach = me()->Stomach - UseSPOfspecialAttack();
		sg_pDungeonSystem->強制ターンエンド要請(friendchara);
		GameIdiom::キャラの方を向く(friendchara,enemy);
		return true;
	}
	return false;
}
void cMob_ID_37::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	if (直接攻撃回避力計算時_タイミング == timing)
	{
		valiable.doubles[変数_回避力] += ValiableConstant4();
	}
}
//-----------------------------------------------------------------
//コイシ
void cMob_ID_38::配置処理()
{
	sg_pDungeonSystem->擬態要請(me(),GAME_TURN_GAMEOVER,false);
}
pcLandform cMob_ID_38::AIお気に入り地形(cValiableField& valiable)
{
	if(ValiableConstant2() > random()*100)
	{
		return sg_pDungeonSystem->RandomPlace();
	}
	return NULLLAND;
}
int cMob_ID_38::AI特殊攻撃選択(cValiableField& valiable)
{
	//キャラ検索する。
	pcCharacter pchara;

	int a;
	for(a=0;a<8;a++)
	{
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco;
		youco.SetAspect(a+me()->aspect);
		youco += nowco;

		pchara = sg_pDungeonSystem->Map().Land(youco)->pOnChar;

		if(pchara != NULL && !sg_pDungeonSystem->キャラクター敵対判定(me(),pchara)
			&& !pchara->Condition.無意識状態())
		{//みかた

			if((nowco - youco).dif() == 1 && me()->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
			{
				return true;
			}
		}
	}
	return false;
}
int cMob_ID_38::特殊攻撃効果(cValiableField& valiable)
{
	
	//エスケープの場合は移動先に敵がいないので検索する必要がある。
	pcCharacter pchara;

	int a;
	for(a=0;a<8;a++)
	{
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco;
		youco.SetAspect(a+me()->aspect);
		youco += nowco;

		pchara = sg_pDungeonSystem->Map().Land(youco)->pOnChar;

		if(pchara != NULL && !sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
		{	
			cCoordinate nowco(me()->placeX,me()->placeY);
			cCoordinate youco(pchara->placeX,pchara->placeY);

			sg_pDungeonSystem->方向転換要請(me(),(youco-nowco).GetAspect());

			pcLandform pland = sg_pDungeonSystem->キャラ前地形(me(),1);

			if(pland->pOnChar != NULL)
			{
				特殊攻撃_アニメ(valiable);
				if(sg_pDungeonSystem->無意識要請(pland->pOnChar,ValiableConstant1()))
				{
					pland->pOnChar->Condition.とても強い刺激();
					return true;
				}
			}
		}

	}
	return false;
}
//-----------------------------------------------------------------
//うつほ
int cMob_ID_39::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{
		int zure = FindUtility::キャラ八方ズレ数(me(),pchara);
		cValiableField val;
		if(zure <= 1 && FindUtility::キャラの距離(me(),pchara) <= AI最大間合い(val))
		{
			return true;
		}
	}
	return false;
}
int cMob_ID_39::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{
		int zure = FindUtility::キャラ八方ズレ数(me(),pchara);
		cValiableField val;
		if(zure <= 1 && FindUtility::キャラの距離(me(),pchara) <= AI最大間合い(val))
		{
			GameIdiom::キャラの方を向く(me(),pchara);

			sg_pDungeonSystem->AnimationManager().
				Anime_PlaySE(_T("shoot.wav"),me()->足元地形()->place);

			pcDroping newpdrop = sg_pDungeonSystem->落ち物生成_設置なし(2027);
			//一本
			newpdrop->quality() = 0;
			newpdrop->state() = cDroping::STATE_NORMAL;
			newpdrop->discover();

			newpdrop->dpack.integer[6] = ValiableConstant1();

			return sg_pDungeonSystem->投擲要請(me(),newpdrop,DRAWTYPE_ATTACKDRAW,10);
		}
	}

	return false;
}
void cMob_ID_39::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	if(timing == 死亡直前_タイミング)
	{
		if(足元() == NULL)
		{
			pcDroping pdrop = sg_pDungeonSystem->落ち物生成_設置なし(9002,NULL,false);
			if(pdrop != NULL)
			{
				pdrop->discover();
				pcTrap ptrap = boost::dynamic_pointer_cast<cTrap>(pdrop);
				if(ptrap)
				{
					ptrap->stepcount()++;
				}
				pcEvent pevent = cEvent_ItemSpowning::pNewEvent(cEvent::PRIORITY_MIDDLE, me()->足元地形(), pdrop);
				pevent->startEvent();
			}
		}
	}
}
//けーね
int cMob_ID_40::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	return FindUtility::キャラが敵で直接攻撃可能か(me(),pchara);
}
int cMob_ID_40::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(FindUtility::キャラが敵で直接攻撃可能か(me(),pchara))
	{
		GameIdiom::キャラの方を向く(me(),pchara);
		pchara = sg_pDungeonSystem->キャラ前地形(me(),1)->pOnChar;
		if(pchara)
		{
			if(random()*100 < ValiableConstant3())
			{
				//レベルダウン

				map<tstring, StyleString> val;
				val[_T("Chara")] = me()->ShortName();
				val[_T("Chara2")] = pchara->ShortName();
				g_Langメッセージ(_T("cMob_ID_40_特殊能力メッセージ"),val);
				
				int i,rekkanum = ValiableConstant1();
				for(i=0;i<rekkanum;i++)
				{
					sg_pDungeonSystem->レベルダウン(pchara);
				}
			}
			else
			{
				//装備劣化
				vector<pcDroping> vpdrop;
				if(pchara->attackequipment) vpdrop.push_back(pchara->attackequipment);
				if(pchara->defenseequipment) vpdrop.push_back(pchara->defenseequipment);
				if(!vpdrop.empty())
				{

					pcDroping pdrop = vpdrop[vpdrop.size()*random()];

					map<tstring, StyleString> val;
					val[_T("Chara")] = me()->ShortName();
					val[_T("Item")] = pdrop->FullName();
					g_Langメッセージ(_T("cMob_ID_40_特殊能力2メッセージ"),val);

					int i,rekkanum = ValiableConstant2();
					for(i=0;i<rekkanum;i++)
					{
						sg_pDungeonSystem->アイテム劣化(pdrop);
					}
					
				}
				
			}
			EffectFunctions::煙エフェクト1(pchara->placeX, pchara->placeY);
		}	
	}

	return true;
}
//-----------------------------------------------------------------
//アリス
int cMob_ID_41::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	return FindUtility::キャラが敵で直接攻撃可能か(me(),pchara);
}
int cMob_ID_41::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(FindUtility::キャラが敵で直接攻撃可能か(me(),pchara))
	{
		GameIdiom::キャラの方を向く(me(),pchara);
		通常攻撃();
		int i;
		bool isSpown = false;
		map<tstring, StyleString> val;
		for(i=0;i<ValiableConstant1();i++)
		{
			pcCharacter friendchara = sg_pDungeonSystem->キャラクター生成(CCHARACTER_cMob_BASE_ID_NUM+80,me()->CLASS,me()->Forse,sg_pDungeonSystem->キャラ視点方角地形(me(),4,0));
			if(friendchara)
			{
				slaveCharacter.push_back(friendchara);
				isSpown = true;
				val[_T("Chara2")] = friendchara->ShortName();
				sg_pDungeonSystem->強制ターンエンド要請(friendchara);
			}
		}
		if(isSpown)
		{
			
			val[_T("Chara")] = ShortName();
			g_Langメッセージ(_T("cMob_ID_41_特殊能力メッセージ"),val);
			return true;
		}
	}
	return false;
}
void cMob_ID_41::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	if(timing == ダメージ計算攻撃時_タイミング)
	{
		valiable.intsets.val(変数_属性).insert(攻撃属性::必中);
	}
	if(timing == 死亡直前_タイミング)
	{
		int i, size = slaveCharacter.size();
		for(i=0;i<size;i++)
		{
			sg_pDungeonSystem->即死要請(slaveCharacter[i], 0, 1);
		}
	}
}
//-----------------------------------------------------------------
//オンリョウ
double cMob_ID_42::targetPriority(pcCharacter me, pcCharacter object)
{
	double priority = 0;
	cCoordinate distance(object->placeX - me->placeX, object->placeY - me->placeY);

	priority += (100 - distance.dif())*10000;

	if(me == object ||
		sg_pDungeonSystem->キャラクター敵対判定(me,object) ||
		 object->CharaAttribute().count(キャラ属性::弾幕))
	{
		priority = 0;
	}
	return priority;
}
pcLandform cMob_ID_42::AIお気に入り地形(cValiableField& valiable)
{

	vector<pcCharacter> vpchara = FindUtility::透明でない部屋内と隣接味方リスト(me());


	pcCharacter pchara = FindUtility::ターゲットキャラクター検索(vpchara, me(), targetPriority);

	if(pchara)
	{
		return pchara->足元地形();
	}

	return NULLLAND;
}
int cMob_ID_42::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(FindUtility::キャラが味方で直接攻撃可能か(me(),pchara))
	{
		return true;
	}

	return false;
}
int cMob_ID_42::特殊攻撃効果(cValiableField& valiable)
{
	vector<pcCharacter> vpchara;
	sg_pDungeonSystem->キャラクター目視可範囲キャラリスト(me(),vpchara);

	pcCharacter pchara = FindUtility::ターゲットキャラクター検索(vpchara, me(), targetPriority);

	if(pchara == NULL)
	{
		return false;
	}

	if(FindUtility::キャラが味方で直接攻撃可能か(me(),pchara))
	{
		cValiableField val;
		double temp;
		val.doubles.dim(tstring(_T("toPlaceX")),temp = pchara->placeX);
		val.doubles.dim(tstring(_T("toPlaceY")),temp = pchara->placeY);

		特殊攻撃_アニメ(val);
		EffectFunctions::煙エフェクト1( pchara->placeX, pchara->placeY);
		int i = 0;
		for(i=0;i<ValiableConstant1();i++)
		{
			sg_pDungeonSystem->レベルアップ(pchara);
		}

		sg_pDungeonSystem->強制退場要請(me());
		return true;
	}

	return false;
}
void cMob_ID_42::パッシブ能力(タイミング timing, cValiableField& valiable)
{

}
pcEnemyAI cMob_ID_42::Get_kindofAI()
{
	return pcEnemyAI(new ceaiEscape);
}
//-----------------------------------------------------------------
//チェン
int cMob_ID_43::AI特殊攻撃選択(cValiableField& valiable)
{
	if(sg_pDungeonSystem->s_turn_speed() < ValiableConstant3() - ValiableConstant4())
	{//手加減
		return false;//不許可
	}

	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);
		cValiableField val;
		if(sg_pDungeonSystem->八方判定(nowco,youco) && (nowco - youco).dif() > 1
			&&sg_pDungeonSystem->isCanNeighberAttack(me(),(youco - nowco).GetAspect(),(youco - nowco).dif(),0,0))
		{
			return true;
		}
	}
	return false;
}
int cMob_ID_43::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{	
		GameIdiom::キャラの方を向く(me(),pchara);

		pcLandform pland = sg_pDungeonSystem->キャラ前地形(me(),ValiableConstant2());
		sg_pDungeonSystem->吹き飛ばし要請(me(),me(),me()->aspect,ValiableConstant2(),0);
		cCoordinate nowco(me()->placeX,me()->placeY);
		cCoordinate youco(pchara->placeX,pchara->placeY);
		if(pland != me()->足元地形() && sg_pDungeonSystem->キャラ前地形(me(),1)->pOnChar != NULL
			 && me()->pAI->u_隣接攻撃通用判定((youco - nowco).GetAspect()))
		{
			multiset<攻撃属性::攻撃属性> 属性;

			sg_pDungeonSystem->攻撃接近(攻撃作成(
				me(),//Attacker
				sg_pDungeonSystem->キャラ前地形(me(),1),//Landplase
				ValiableConstant1(),//attackPower
				1,//damageflag
				属性,//attribute
				true
				));
			sg_pDungeonSystem->強制ターンエンド要請(me());
		}
		return true;
	}
	return false;
}
void cMob_ID_43::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	if(timing == ダメージ計算防御時_タイミング)
	{
		if(valiable.intsets.val(変数_属性).count(攻撃属性::水))
		{
			sg_pDungeonSystem->封印要請(me(),GAME_TURN_GAMEOVER,1);
		}
	}
}
//-----------------------------------------------------------------
//ラン
int cMob_ID_44::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	return FindUtility::キャラが敵で直接攻撃可能か(me(),pchara);
}
int cMob_ID_44::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(FindUtility::キャラが敵で直接攻撃可能か(me(),pchara))
	{
		GameIdiom::キャラの方を向く(me(),pchara);
		pcCharacter friendchara = sg_pDungeonSystem->キャラクター生成(CCHARACTER_cMob_BASE_ID_NUM+43,me()->CLASS,me()->Forse,sg_pDungeonSystem->キャラ視点方角地形(me(),4,1));
		if(friendchara)
		{

			map<tstring, StyleString> val;
			val[_T("Chara")] = ShortName();
			val[_T("Chara2")] = friendchara->ShortName();
			g_Langメッセージ(_T("cMob_ID_44_特殊能力メッセージ"),val);
			sg_pDungeonSystem->強制ターンエンド要請(friendchara);
			return true;
		}
	}
	return false;
}
void cMob_ID_44::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	if(timing == ダメージ計算防御時_タイミング)
	{
		if(valiable.intsets.val(変数_属性).count(攻撃属性::水))
		{
			sg_pDungeonSystem->封印要請(me(),GAME_TURN_GAMEOVER,1);
		}
	}
}
//-----------------------------------------------------------------
//ユカリ
pcLandform cMob_ID_45::AIお気に入り地形(cValiableField& valiable)
{
	if(ValiableConstant1() > random()*100)
	{
		return sg_pDungeonSystem->RandomPlace();
	}
	return NULLLAND;
}
int cMob_ID_45::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	return FindUtility::キャラが敵で直接攻撃可能か(me(),pchara);
}
int cMob_ID_45::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(FindUtility::キャラが敵で直接攻撃可能か(me(),pchara))
	{
		GameIdiom::キャラの方を向く(me(),pchara);
		pcCharacter friendchara = sg_pDungeonSystem->キャラクター生成(CCHARACTER_cMob_BASE_ID_NUM+44,me()->CLASS,me()->Forse,sg_pDungeonSystem->キャラ視点方角地形(me(),4,1));
		if(friendchara)
		{

			map<tstring, StyleString> val;
			val[_T("Chara")] = ShortName();
			val[_T("Chara2")] = friendchara->ShortName();
			g_Langメッセージ(_T("cMob_ID_44_特殊能力メッセージ"),val);
			sg_pDungeonSystem->強制ターンエンド要請(friendchara);
			return true;
		}
	}
	return false;
}
void cMob_ID_45::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	if(timing == ダメージ計算防御時_タイミング)
	{
		if(valiable.intsets.val(変数_属性).count(攻撃属性::水))
		{
			sg_pDungeonSystem->封印要請(me(),GAME_TURN_GAMEOVER,1);
		}
	}
}
//-----------------------------------------------------------------
//エイリン
double cMob_ID_46::targetPriority(pcCharacter me, pcCharacter object)
{
	double priority = 0;
	cCoordinate distance(object->placeX - me->placeX, object->placeY - me->placeY);
	
	priority += (object->MHP - object->HP);
	if(object->ID() == CCHARACTER_cMob_BASE_ID_NUM+47)
	{//カグヤ優先
		priority *= 2;
	}

	priority += (100 - distance.dif())*10000;

	if(me == object ||
		sg_pDungeonSystem->キャラクター敵対判定(me,object) ||
		(object->HP == object->MHP)
		|| object->CharaAttribute().count(キャラ属性::弾幕))
	{
		priority = 0;
	}
	return priority;
}
pcLandform cMob_ID_46::AIお気に入り地形(cValiableField& valiable)
{

	vector<pcCharacter> vpchara;
	sg_pDungeonSystem->キャラクター目視可範囲キャラリスト(me(),vpchara);

	pcCharacter pchara = FindUtility::ターゲットキャラクター検索(vpchara, me(), targetPriority);

	if(pchara)
	{
		return pchara->足元地形();
	}

	return NULLLAND;
}
int cMob_ID_46::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;
	if(pchara)
	{

		if(FindUtility::キャラが味方で直接攻撃可能か(me(),pchara) && (pchara->HP != pchara->MHP))
		{
			return true;
		}
		if(ValiableConstant3() > random()*100
			&& FindUtility::キャラの距離(me(),pchara) == 2
			&& sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
		{
			return true;
		}
	}
	return false;
}
int cMob_ID_46::特殊攻撃効果(cValiableField& valiable)
{
	
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;
	if(pchara)
	{

		if(FindUtility::キャラが味方で直接攻撃可能か(me(),pchara))
		{
			GameIdiom::キャラの方を向く(me(),pchara);
			pcCharacter pchara = sg_pDungeonSystem->キャラ前地形(me(),1)->pOnChar;
			if(pchara)
			{
				sg_pDungeonSystem->回復要請(pchara,ValiableConstant1(),true);
				return true;
			}
		}
		
		if(FindUtility::キャラの距離(me(),pchara) == 2
			&& sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
		{
			GameIdiom::キャラの方を向く(me(),pchara);
			pcDroping newpdrop = sg_pDungeonSystem->落ち物生成_設置なし(ValiableConstant4());
			//一本
			newpdrop->quality() = 0;
			newpdrop->state() = cDroping::STATE_NORMAL;
			newpdrop->discover();
			//sg_pDungeonSystem->無条件投擲要請(me(),pchara->足元地形(),newpdrop,me()->aspect,THROW_HIMANAGE,2);
			sg_pDungeonSystem->投擲要請(me(),newpdrop,THROW_HIMANAGE,2);
			return true;
		}
	}
	return true;
}
void cMob_ID_46::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	if(timing == ターン終了_タイミング)
	{
		int damage = me()->MHP - me()->HP;
		if(damage > 0)
		{
			sg_pDungeonSystem->回復要請(me(),min(ValiableConstant2(),damage),true);
		}
	}
	else if(timing == 脱力追加直前_タイミング)
	{
		valiable.doubles[変数_汎用ブール] = 0;//拒否
	}
	else if(timing == 軟弱追加直前_タイミング)
	{
		valiable.doubles[変数_汎用ブール] = 0;//拒否
	}
	else if(timing == 飲み効果直前_タイミング)
	{
		if(valiable.drops[変数_対象落ち物]->属性.count(落ち物属性::薬))
		{
			valiable.doubles[変数_汎用ブール] = 0;//拒否
			map<tstring, StyleString> valiable;
			valiable[_T("Chara")] = me()->ShortName();
			g_Langメッセージ(_T("cSpell_ID_10効果メッセージ"),valiable);
		}
	}
	else if(timing == 飲み物衝突効果直前_タイミング)
	{
		if(valiable.drops[変数_対象落ち物]->属性.count(落ち物属性::薬))
		{
			valiable.doubles[変数_汎用ブール] = 0;//拒否
			map<tstring, StyleString> valiable;
			valiable[_T("Chara")] = me()->ShortName();
			g_Langメッセージ(_T("cSpell_ID_10効果メッセージ"),valiable);
		}
	}
}
//-----------------------------------------------------------------
//カグヤ
int cMob_ID_47::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	return FindUtility::キャラが敵で直接攻撃可能か(me(),pchara);
}
int cMob_ID_47::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(FindUtility::キャラが敵で直接攻撃可能か(me(),pchara))
	{
		GameIdiom::キャラの方を向く(me(),pchara);
		pcCharacter friendchara = sg_pDungeonSystem->キャラクター生成(CCHARACTER_cMob_BASE_ID_NUM+46,me()->CLASS,me()->Forse,sg_pDungeonSystem->キャラ視点方角地形(me(),4,1));
		if(friendchara)
		{

			map<tstring, StyleString> val;
			val[_T("Chara")] = ShortName();
			val[_T("Chara2")] = friendchara->ShortName();
			g_Langメッセージ(_T("cMob_ID_47_特殊能力メッセージ"),val);

			//friendchara->Stomach = me()->Stomach - UseSPOfspecialAttack();
			sg_pDungeonSystem->強制ターンエンド要請(friendchara);
			GameIdiom::キャラの方を向く(friendchara,pchara);
			return true;
		}
	}
	return false;
}
void cMob_ID_47::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	if(timing == 魔法接近直前_タイミング)
	{
		//跳ね返し
		if(!valiable.drops.val(変数_対象落ち物)->跳ね返し無効フラグ)
		{
			valiable.doubles.val(変数_汎用ブール) = 0;//効果発揮フラグ

			map<tstring, StyleString> val;
			val[_T("Chara")] = FullName();
			g_Langメッセージ(_T("cMob_ID_15_特殊能力メッセージ"),val);
			
			sg_pDungeonSystem->方向転換要請(me(), valiable.doubles.val(変数_方向)+4);
			sg_pDungeonSystem->魔法発射要請(me(), valiable.drops.val(変数_対象落ち物));
		}
	}
}
pcEnemyAI cMob_ID_47::Get_kindofAI()
{
	return pcEnemyAI(new ceaiRoomKeeper);
}
//テイ
int cMob_ID_48::AI特殊攻撃選択(cValiableField& valiable)
{
	if(足元() == NULL && me()->足元地形()->isRoom())
	{
		return true;
	}
	return false;
}
int cMob_ID_48::特殊攻撃効果(cValiableField& valiable)
{
	int vint[3];

	sg_pDungeonSystem->罠自然湧きID(vint);
	pcDroping pdrop = sg_pDungeonSystem->落ち物生成(vint[0],NULL,me()->足元地形(),true);
	if(pdrop != NULL && ValiableConstant1()/100.0 > random())
	{
		pdrop->discover();
		pcTrap ptrap = boost::dynamic_pointer_cast<cTrap>(pdrop);
		if(ptrap)
		{
			ptrap->stepcount()++;
		}
	}
	EffectFunctions::煙エフェクト1(me()->placeX, me()->placeY);


	return true;
}
void cMob_ID_48::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	if(timing == 死亡直前_タイミング)
	{
		if(足元() == NULL)
		{
			pcDroping pdrop;
			if(ValiableConstant4()/100.0 > random())
			{
				pdrop = sg_pDungeonSystem->落ち物生成_設置なし(ValiableConstant2(),NULL,false);
			}
			else
			{
				pdrop = sg_pDungeonSystem->落ち物生成_設置なし(ValiableConstant3(),NULL,false);
			}

			if(pdrop != NULL)
			{
				pdrop->discover();
				pcTrap ptrap = boost::dynamic_pointer_cast<cTrap>(pdrop);
				if(ptrap)
				{
					ptrap->stepcount()++;
				}
			}
			pcEvent pevent = cEvent_ItemSpowning::pNewEvent(cEvent::PRIORITY_MIDDLE, me()->足元地形(), pdrop);
			pevent->startEvent();
		}
	}
}
//-----------------------------------------------------------------
//モコウ
void cMob_ID_49::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	if(timing == 死亡直前_タイミング)
	{
		if(me()->Stomach >= 100.0)
		{
			pcCharacter pchara = sg_pDungeonSystem->キャラクター生成(ID(),CLASS,Forse,
				FindUtility::部屋外優先_各部屋等確率_ランダム地形検索_配置安全(me()->足元地形()));
			sg_pDungeonSystem->満腹度減少要請(pchara,100,false);
		}
	}
	else if(timing == ターン終了_タイミング)
	{
		int damage = me()->MHP - me()->HP;
		if(damage > 0)
		{
			sg_pDungeonSystem->回復要請(me(),min(ValiableConstant1(),damage),true);
		}
	}
	else if(timing == ダメージ計算攻撃時_タイミング)
	{
		valiable.intsets.val(変数_属性).insert(攻撃属性::火);
	}
}
//-----------------------------------------------------------------
//リグル
/*
pcLandform cMob_ID_50::AIお気に入り地形(cValiableField& valiable)
{
	if(足元地形()->RoomIndex == -1) return NULLLAND;

	vector<pcCharacter> vpchara;
	sg_pDungeonSystem->キャラクター目視可範囲キャラリスト(me(),vpchara);


	int i;
	for(i=0;i<vpchara.size();i++)
	{
		if(vpchara[i]->Condition.眠りで行動不能である())
		{
			return vpchara[i]->足元地形();
		}
	}

	return NULLLAND;
}
*/
int cMob_ID_50::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	return FindUtility::キャラが敵で直接攻撃可能か(me(),pchara);
}
int cMob_ID_50::特殊攻撃効果(cValiableField& valiable)
{
	
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(FindUtility::キャラが敵で直接攻撃可能か(me(),pchara))
	{
		GameIdiom::キャラの方を向く(me(),pchara);
		pcCharacter friendchara = sg_pDungeonSystem->キャラクター生成(ID(),CLASS,me()->Forse,sg_pDungeonSystem->キャラ前地形(me(),1));
		if(friendchara)
		{
			map<tstring, StyleString> val;
			val[_T("Chara")] = ShortName();
			g_Langメッセージ(_T("cMob_ID_50_特殊能力メッセージ"),val);

			friendchara->Stomach = me()->Stomach - UseSPOfspecialAttack();
			sg_pDungeonSystem->強制ターンエンド要請(friendchara);
			GameIdiom::キャラの方を向く(friendchara,pchara);
		}
	}
	return true;
}
void cMob_ID_50::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	if(timing == ターン終了_タイミング)
	{
		int a;
		for(a=0;a<8;a++)
		{
			pcCharacter pchara = sg_pDungeonSystem->キャラ視点方角地形(me(),a,1)->pOnChar;
			if(pchara && ValiableConstant1() > 100*random())
			{
				if(FindUtility::キャラが味方で直接攻撃可能か(me(),pchara) && pchara->Condition.眠りで行動不能である())
				{
					EffectFunctions::特殊能力発揮エフェクト(me()->placeX,me()->placeY,1.7);
					EffectFunctions::煙エフェクト1(pchara->placeX, pchara->placeY);
					GameIdiom::悪性異常状態治療要請(pchara,false);
				}
			}
		}
	}
}

//ミスティア
int cMob_ID_51::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	return FindUtility::キャラが敵で直接攻撃可能か(me(),pchara);
}
int cMob_ID_51::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(FindUtility::キャラが敵で直接攻撃可能か(me(),pchara) && !pchara->Condition.鳥目状態())
	{
		GameIdiom::キャラの方を向く(me(),pchara);
		pchara = sg_pDungeonSystem->キャラ前地形(me(),1)->pOnChar;
		if(pchara)
		{
			特殊攻撃_アニメ(valiable);
			sg_pDungeonSystem->鳥目要請(pchara,ValiableConstant1(),false);
			map<tstring, StyleString> val;
			val[_T("Chara")] = pchara->ShortName();
			g_Langメッセージ(_T("cMob_ID_51_特殊能力メッセージ"),val);
		}
	}
	return true;
}
//-----------------------------------------------------------------
void cMob_ID_prismriver::setfriend(pcCharacter friendchara1, pcCharacter friendchara2, pcCharacter friendchara3)
{
	pcMob_ID_prismriver pchara1 = boost::dynamic_pointer_cast<cMob_ID_prismriver>(friendchara1);
	pcMob_ID_prismriver pchara2 = boost::dynamic_pointer_cast<cMob_ID_prismriver>(friendchara2);
	pcMob_ID_prismriver pchara3 = boost::dynamic_pointer_cast<cMob_ID_prismriver>(friendchara3);
	if(pchara1 && pchara2 && pchara3)
	{
		pchara1->friendcharas.push_back(wpcCharacter(pchara2));
		pchara1->friendcharas.push_back(wpcCharacter(pchara3));
		pchara2->friendcharas.push_back(wpcCharacter(pchara1));
		pchara2->friendcharas.push_back(wpcCharacter(pchara3));
		pchara3->friendcharas.push_back(wpcCharacter(pchara1));
		pchara3->friendcharas.push_back(wpcCharacter(pchara2));
		pchara1->mode_together = true;
		pchara2->mode_together = true;
		pchara3->mode_together = true;

		if(pchara1->Condition.眠りで行動不能である())
		{
			pchara2->Condition.深い居眠り追加();
			pchara3->Condition.深い居眠り追加();
		}
		else
		{
			pchara2->Condition.とても強い刺激();
			pchara3->Condition.とても強い刺激();
		}
	}
}
bool cMob_ID_prismriver::checkFriendLive()
{
	int i,size = friendcharas.size();
	for(i=0;i<size;i++)
	{
		pcCharacter pchara = friendcharas[i].lock();
		if(!(pchara) || pchara->HP <= 0 )
		{
			return false;
		}
	}
	return true;
}
void cMob_ID_prismriver::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	if(timing == 被攻撃接近直後_タイミング)
	{
		int i,size = friendcharas.size();
		for(i=0;i<size;i++)
		{
			pcCharacter pchara = friendcharas[i].lock();
			if(pchara && !pchara->死亡() )
			{
				if(pchara->Condition.眠りで行動不能である())
				{
					pchara->Condition.とても強い刺激();
				}
			}
		}
	}
}
pcLandform cMob_ID_prismriver::AIお気に入り地形(cValiableField& valiable)
{
	if(mode_together && checkFriendLive())
	{
		int i,size = friendcharas.size();
		for(i=0;i<size;i++)
		{
			cCoordinate coo(friendcharas[i].lock()->placeX - me()->placeX, friendcharas[i].lock()->placeY - me()->placeY);
			if(coo.dif() > 1)
			{
				return friendcharas[i].lock()->足元地形();
			}
		}
	}
	return NULLLAND;
}
//メルラン
void cMob_ID_52::配置処理()
{
}
int cMob_ID_52::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;
	if(pchara && FindUtility::キャラが敵で直接攻撃可能か(me(),pchara))
	{
		return (pchara->Condition.鈍足以下である() || !pchara->Condition.狂乱状態());
	}
	return false;
}
int cMob_ID_52::特殊攻撃効果(cValiableField& valiable)
{
	if(AI特殊攻撃選択(valiable))
	{
		pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;
		GameIdiom::キャラの方を向く(me(),pchara);
		pchara = sg_pDungeonSystem->キャラ前地形(me(),1)->pOnChar;
		if(pchara)
		{
			特殊攻撃_アニメ(valiable);
			if(pchara->Condition.鈍足以下である())
			{
				sg_pDungeonSystem->速度増加要請(pchara,-1,true);
			}
			if(!pchara->Condition.狂乱状態())
			{
				sg_pDungeonSystem->狂乱要請(pchara,ValiableConstant1(),true);
			}
			return true;
		}
	}
	return false;
}
pcEnemyAI cMob_ID_52::Get_kindofAI()
{
	return pcEnemyAI(new ceaiRoomKeeper);
}
//リリカ
void cMob_ID_53::配置処理()
{
}
int cMob_ID_53::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;
	if(pchara && FindUtility::キャラが敵で直接攻撃可能か(me(),pchara))
	{
		return (pchara->Condition.鈍足以下である() || pchara->Condition.狂乱状態());
	}
	return false;
}
int cMob_ID_53::特殊攻撃効果(cValiableField& valiable)
{
	if(AI特殊攻撃選択(valiable))
	{
		pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;
		GameIdiom::キャラの方を向く(me(),pchara);
		pchara = sg_pDungeonSystem->キャラ前地形(me(),1)->pOnChar;
		if(pchara)
		{
			特殊攻撃_アニメ(valiable);
			if(pchara->Condition.鈍足以下である())
			{
				sg_pDungeonSystem->速度増加要請(pchara,-1,true);
			}
			if(pchara->Condition.狂乱状態())
			{
				sg_pDungeonSystem->狂乱要請(pchara,-1,true);
			}
			return true;
		}
	}
	return false;
}
pcEnemyAI cMob_ID_53::Get_kindofAI()
{
	return pcEnemyAI(new ceaiRoomKeeper);
}
//ルナサ
void cMob_ID_54::配置処理()
{
	cMob_ID_prismriver::setfriend(me(),
		sg_pDungeonSystem->キャラクター生成(CCHARACTER_cMob_BASE_ID_NUM+52,me()->CLASS,me()->Forse,me()->足元地形() ),
		sg_pDungeonSystem->キャラクター生成(CCHARACTER_cMob_BASE_ID_NUM+53,me()->CLASS,me()->Forse,me()->足元地形() ));
}
int cMob_ID_54::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;
	if(pchara && FindUtility::キャラが敵で直接攻撃可能か(me(),pchara))
	{
		return (!pchara->Condition.鈍足以下である() || pchara->Condition.狂乱状態());
	}
	return false;
}
int cMob_ID_54::特殊攻撃効果(cValiableField& valiable)
{
	if(AI特殊攻撃選択(valiable))
	{
		pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;
		GameIdiom::キャラの方を向く(me(),pchara);
		pchara = sg_pDungeonSystem->キャラ前地形(me(),1)->pOnChar;
		if(pchara)
		{
			特殊攻撃_アニメ(valiable);
			if(!pchara->Condition.鈍足以下である())
			{
				sg_pDungeonSystem->速度減少要請(pchara,ValiableConstant1(),true);
			}
			if(pchara->Condition.狂乱状態())
			{
				sg_pDungeonSystem->狂乱要請(pchara,-1,true);
			}
			return true;
		}
	}
	return false;
}
pcEnemyAI cMob_ID_54::Get_kindofAI()
{
	return pcEnemyAI(new ceaiRoomKeeper);
}
//メディスン
int cMob_ID_55::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	return FindUtility::キャラが敵で直接攻撃可能か(me(),pchara);
}
int cMob_ID_55::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(FindUtility::キャラが敵で直接攻撃可能か(me(),pchara))
	{
		GameIdiom::キャラの方を向く(me(),pchara);
		pchara = sg_pDungeonSystem->キャラ前地形(me(),1)->pOnChar;
		if(pchara)
		{
			特殊攻撃_アニメ(valiable);
			sg_pDungeonSystem->脱力要請(pchara,ValiableConstant1(),ValiableConstant2(),true);
			if(ValiableConstant3() > 0) sg_pDungeonSystem->速度減少要請(pchara,ValiableConstant3(),true);
		}
	}
	return true;
}
void cMob_ID_55::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	if(timing == 脱力追加直前_タイミング)
	{
		valiable.doubles.val(変数_汎用ブール) = 0;
	}
}
//ユウカ
int cMob_ID_56::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	return FindUtility::キャラが敵で直接攻撃可能か(me(),pchara);
}
int cMob_ID_56::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(FindUtility::キャラが敵で直接攻撃可能か(me(),pchara))
	{
		GameIdiom::キャラの方を向く(me(),pchara);
		AImemory[0] = 1;
		sg_pDungeonSystem->直接攻撃要請(me());
		return true;
	}
	return true;
}
void cMob_ID_56::配置処理()
{
	sg_pDungeonSystem->狂乱要請(me(), GAME_TURN_GAMEOVER, 0);
}
void cMob_ID_56::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	if(timing == 経験値獲得時_タイミング)
	{
		valiable.doubles[変数_汎用ボーナス_倍率] = 0.0;//経験値入らない
		if(me()->Condition.力係数() >= 2.0 && !me()->Condition.無意識状態())
		{
			sg_pDungeonSystem->元気要請(me(), 0, -1, 0);
			sg_pDungeonSystem->無意識要請(me(), ValiableConstant3(), 1);
		}
		sg_pDungeonSystem->元気要請(me(),ValiableConstant2(),ValiableConstant3(),false);
	}
	if(timing == ダメージ計算攻撃時_タイミング && AImemory[0])
	{
		AImemory[0] = 0;
		valiable.doubles.val(変数_耐性ボーナス_倍率％) -= ValiableConstant1();
	}
}

//コマチ
int cMob_ID_57::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;
	if(pchara)
	{
		if(FindUtility::キャラが敵で直接攻撃可能か(me(), pchara))
		{
			return true;
		}
		if(FindUtility::キャラ８方向射線で範囲以内(me(),pchara, ValiableConstant1()) && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
		{
			return true;
		}
	}
	return false;
}
int cMob_ID_57::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;
	if(pchara)
	{

		if(FindUtility::キャラが敵で直接攻撃可能か(me(), pchara))
		{
			GameIdiom::キャラの方を向く(me(),pchara);
			pchara = sg_pDungeonSystem->キャラ前地形(me(),1)->pOnChar;
			if(pchara)
			{
				multiset<攻撃属性::攻撃属性> 属性;
				
				sg_pDungeonSystem->攻撃接近(攻撃作成(
					me(),//攻撃者
					pchara->足元地形(),//場所
					ValiableConstant2(),//攻撃力
					割合ダメージフラグ,//
					属性,
					true//必中
					));

				特殊攻撃_アニメ(valiable);
				return true;
			}
		}

		if(FindUtility::キャラ８方向射線で範囲以内(me(),pchara, ValiableConstant1()) && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
		{
			GameIdiom::キャラの方を向く(me(),pchara);

			pcDroping newpdrop = sg_pDungeonSystem->落ち物生成_設置なし(6011);//引き寄せ
			//一本
			newpdrop->quality() = 0;
			newpdrop->state() = cDroping::STATE_NORMAL;
			newpdrop->discover();

			sg_pDungeonSystem->AnimationManager().
				Anime_PlaySE(_T("shoot.wav"),me()->足元地形()->place);

			sg_pDungeonSystem->魔法発射要請(me(),newpdrop);

			通常攻撃();

			return true;
		}
	}
	return false;
}
//ナズーリン
pcLandform cMob_ID_58::AIお気に入り地形(cValiableField& valiable)
{
	vector<pcDroping> vpdrop;
	sg_pDungeonSystem->キャラクター目視可範囲落ち物リスト(me(),vpdrop);
	pcDroping target = FindUtility::ターゲット落ち物検索(vpdrop, me(), targetPriority);
	if(target)
	{
		return target->現在地形();
	}
	return NULLLAND;
}
double cMob_ID_58::targetPriority(pcCharacter me, pcDroping object)
{
	double priority = 0;

	if(object && object->落ちてる() && object->ID() == CDROPING_cEquipment_BASE_ID_NUM+45)
	{
		cCoordinate distance(object->placeX - me->placeX, object->placeY - me->placeY);
		priority += (100 - distance.dif())*10000;
	}
	return priority;
}
//---------------------------------------------------------------
//コガサ
pcLandform cMob_ID_59::AIお気に入り地形(cValiableField& valiable)
{
	if(特殊攻撃可能())
	{

		if(足元地形()->RoomIndex == -1) return NULLLAND;
		pcLandform pland = sg_pDungeonSystem->Map().RoomList[足元地形()->RoomIndex]->plastputland;
		if(pland != NULL && pland->pOnDrop != NULL && pland->pOnDrop->属性.count(落ち物属性::アイテム) &&
			pland->pOnDrop->shopitemflag() == cDroping::SHOPFLAG_NONE)
		{
			return sg_pDungeonSystem->Map().RoomList[足元地形()->RoomIndex]->plastputland;
		}
	}
	return NULLLAND;
}
int cMob_ID_59::AI特殊攻撃選択(cValiableField& valiable)
{
	pcDroping pdrop = 足元();
	if(pdrop == NULL) return false;

	if(pdrop->属性.count(落ち物属性::アイテム))
	{
		return true;
	}

	return false;
}
int cMob_ID_59::特殊攻撃効果(cValiableField& valiable)
{
	if(AI特殊攻撃選択(valiable))
	{
		pcDroping pdrop = 足元();
		if(pdrop != NULL)
		{
			EffectFunctions::煙エフェクト1(pdrop->placeX,pdrop->placeY);
			sg_pDungeonSystem->アイテム隠れ(me(),pdrop);
			return true;
		}
	}
	return false;
}
void cMob_ID_59::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	if(アイテム隠れ解除直後時_タイミング == timing)
	{
		pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;
		if(pchara)
		{
			
			EffectFunctions::煙エフェクト1(me()->placeX,me()->placeY);
			GameIdiom::キャラの方を向く(me(),pchara);
			multiset<攻撃属性::攻撃属性> attri;
			attri.insert(攻撃属性::水); attri.insert(攻撃属性::水); attri.insert(攻撃属性::水);
			GameIdiom::広範囲攻撃(valiable.lands[変数_攻撃場所], me(), 0, 0, attri, 0);
			sg_pDungeonSystem->びっくり要請(pchara,1,1);
			sg_pDungeonSystem->強制ターンエンド要請(me());
		}
	}
	else if(timing == ダメージ計算攻撃時_タイミング)
	{
		valiable.intsets.val(変数_属性).insert(攻撃属性::水);
	}
}
//-----------------------------------------------------------------
//イチリン
int cMob_ID_60::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;
	if(pchara)
	{
		if(sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
		{
			return FindUtility::キャラ中距離攻撃地形射線通り(me(),pchara,ValiableConstant1());
		}
	}
	return false;
}
int cMob_ID_60::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;
	if(pchara && AI特殊攻撃選択(valiable))
	{
		GameIdiom::キャラの方を向く(me(),pchara);
		int distance = FindUtility::キャラ中距離攻撃地形攻撃距離(me(), me()->aspect, ValiableConstant1());
		pcLandform pland = sg_pDungeonSystem->キャラ前地形(me(),distance);
		pchara = pland->pOnChar;
		cValiableField val;
		double temp;
		val.doubles.dim(tstring(_T("distanceGrid")),temp = distance);
		val.doubles.dim(tstring(_T("toPlaceX")),temp = pland->placeX);
		val.doubles.dim(tstring(_T("toPlaceY")),temp = pland->placeY);
		特殊攻撃_アニメ(val);

		multiset<攻撃属性::攻撃属性> 属性;
		pcAttackinformation pca = 攻撃作成(
			me(),//Attacker
			sg_pDungeonSystem->キャラ前地形(me(),distance),//Landplase
			me()->GetAttackPower(),//attackPower
			0,//damageflag
			属性,//attribute
			false
			);
		pca->valiable.doubles.dim(変数_命中力) = me()->GetHitPowerBase();
		sg_pDungeonSystem->攻撃接近(pca);
			
		return true;
	}
	return false;
}
void cMob_ID_60::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	if(timing == 被攻撃接近直後_タイミング)
	{
		if(me()->HP > 0 && random()*100 < ValiableConstant2())
		{
			double HPper = min(1.0, me()->HP/(double)me()->MHP + ValiableConstant3()/100.0);
			sg_pDungeonSystem->強制退場要請(me(),0,0);
			pcCharacter friendchara = sg_pDungeonSystem->キャラクター生成(CCHARACTER_cMob_BASE_ID_NUM+61,me()->CLASS,me()->Forse,sg_pDungeonSystem->キャラ視点方角地形(me(),2,1));
			sg_pDungeonSystem->強制ターンエンド要請(friendchara);
			sg_pDungeonSystem->HP設定要請(friendchara, HPper * friendchara->HP, 0);
			pcCharacter friendchara2 = sg_pDungeonSystem->キャラクター生成(CCHARACTER_cMob_BASE_ID_NUM+62,me()->CLASS,me()->Forse,sg_pDungeonSystem->キャラ視点方角地形(me(),6,1));
			sg_pDungeonSystem->強制ターンエンド要請(friendchara2);
			sg_pDungeonSystem->HP設定要請(friendchara2, HPper * friendchara2->HP, 0);
			map<tstring, StyleString> val;
			val[_T("Chara")] = ShortName();
			g_Langメッセージ(_T("cMob_ID_60_特殊能力メッセージ"),val);
			return;
		}
	}
}
//イチリンノミ
pcLandform cMob_ID_61::AIお気に入り地形(cValiableField& valiable)
{
	vector<pcCharacter> vpchara;
	sg_pDungeonSystem->キャラクター目視可範囲キャラリスト(me(),vpchara);
	int i;
	for(i=0;i<vpchara.size();i++)
	{
		if(vpchara[i]->ID() == 2062)//ウンザン
		{
			return vpchara[i]->足元地形();
		}
	}
	return NULLLAND;
}
pcEnemyAI cMob_ID_61::Get_kindofAI()
{
	return pcEnemyAI(new ceaiEscape);
}
//ウンザン
pcEnemyAI cMob_ID_62::Get_kindofAI()
{
	return pcEnemyAI(new ceaiEscape);
}
int cMob_ID_62::AI特殊攻撃選択(cValiableField& valiable)
{
	vector<pcCharacter> vpchara;
	sg_pDungeonSystem->キャラクター目視可範囲キャラリスト(me(),vpchara);
	int i;
	for(i=0;i<vpchara.size();i++)
	{
		if(vpchara[i]->ID() == 2061)//イチリンノミ
		{
			if(FindUtility::キャラが味方で直接攻撃可能か(me(), vpchara[i]))
			{
				return true;
			}
		}
	}
	return false;
}
int cMob_ID_62::特殊攻撃効果(cValiableField& valiable)
{
	vector<pcCharacter> vpchara;
	sg_pDungeonSystem->キャラクター目視可範囲キャラリスト(me(),vpchara);
	int i;
	for(i=0;i<vpchara.size();i++)
	{
		if(vpchara[i]->ID() == 2061)//イチリンノミ
		{
			if(FindUtility::キャラが味方で直接攻撃可能か(me(), vpchara[i]))
			{
				double HPper = max(vpchara[i]->HP/(double)vpchara[i]->MHP, me()->HP/(double)me()->MHP);
				int cla = max(vpchara[i]->CLASS, me()->CLASS);
				pcLandform pland = vpchara[i]->足元地形();
				sg_pDungeonSystem->強制退場要請(me(),0,0);
				sg_pDungeonSystem->強制退場要請(vpchara[i],0,0);
				
				pcCharacter friendchara = sg_pDungeonSystem->キャラクター生成(CCHARACTER_cMob_BASE_ID_NUM+60, cla, me()->Forse, pland);
				sg_pDungeonSystem->強制ターンエンド要請(friendchara);
				sg_pDungeonSystem->HP設定要請(friendchara, HPper * friendchara->HP, 0);
			
				map<tstring, StyleString> val;
				val[_T("Chara")] = FullName();
				val[_T("Chara2")] = vpchara[i]->FullName();
				g_Langメッセージ(_T("cMob_ID_62_特殊能力メッセージ"),val);
				return true;
			}
		}
	}


	return false;
}
//---------------------------------------------------------------
//ムラサ
void cMob_ID_63::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	if(timing == 吹き飛ばし判定時_タイミング
		|| timing == 引き寄せ判定時_タイミング
		|| timing == 投げ飛ばし判定時_タイミング
		)
	{
		valiable.doubles.val(変数_汎用ブール) = 0;
		map<tstring, StyleString> valiable;
		valiable[_T("Chara")] = FullName();
		g_Langメッセージ(_T("cMob_ID_63_特殊能力メッセージ"),valiable);
	}
	else if(timing == ダメージ計算攻撃時_タイミング)
	{
		valiable.intsets.val(変数_属性).insert(攻撃属性::水);
	}
}
//---------------------------------------------------------------
//トラマル
int cMob_ID_64::AIセミパッシブ特殊攻撃選択(cValiableField& valiable)//SP消費なし優先度高
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(FindUtility::キャラの距離(me(),pchara) == 2 && FindUtility::キャラ八方ズレ数(me(),pchara) == 0)
	{
		return true;
	}

	return false;
}
int cMob_ID_64::セミパッシブ特殊攻撃効果(cValiableField& valiable)//SP消費なし優先度高
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara && sg_pDungeonSystem->キャラクター敵対判定(me(), pchara) && FindUtility::キャラの距離(me(),pchara) == 2 && FindUtility::キャラ八方ズレ数(me(),pchara) == 0)
	{
		GameIdiom::キャラの方を向く(me(),pchara);
		int distance = 2;
		pcLandform pland = sg_pDungeonSystem->キャラ前地形(me(),distance);
		cValiableField val;
		double temp;
		val.doubles.dim(tstring(_T("distanceGrid")),temp = distance);
		val.doubles.dim(tstring(_T("toPlaceX")),temp = pland->placeX);
		val.doubles.dim(tstring(_T("toPlaceY")),temp = pland->placeY);
		特殊攻撃_アニメ(val);

		multiset<攻撃属性::攻撃属性> 属性;
		pcAttackinformation pca = 攻撃作成(
			me(),//Attacker
			sg_pDungeonSystem->キャラ前地形(me(),1),//Landplase
			me()->GetAttackPower(),//attackPower
			0,//damageflag
			属性,//attribute
			false
			);
		sg_pDungeonSystem->攻撃接近(pca);
		pca = 攻撃作成(
			me(),//Attacker
			sg_pDungeonSystem->キャラ前地形(me(),2),//Landplase
			me()->GetAttackPower(),//attackPower
			0,//damageflag
			属性,//attribute
			false
			);
		sg_pDungeonSystem->攻撃接近(pca);
		return true;
	}
	return false;
}
int cMob_ID_64::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(FindUtility::キャラが敵で直接攻撃可能か(me(),pchara))
	{
		return true;
	}

	return false;
}
int cMob_ID_64::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(FindUtility::キャラが敵で直接攻撃可能か(me(),pchara))
	{
		GameIdiom::キャラの方を向く(me(),pchara);
		if(ValiableConstant1())//イチリン
		{
			召喚(CCHARACTER_cMob_BASE_ID_NUM+60,pchara);
		}
		if(ValiableConstant2())//ナズ
		{
			召喚(CCHARACTER_cMob_BASE_ID_NUM+58,pchara);
		}
		if(ValiableConstant3())//ムラサ
		{
			召喚(CCHARACTER_cMob_BASE_ID_NUM+63,pchara);
		}
		return true;
	}
	return false;
}
int cMob_ID_64::召喚(int ID, pcCharacter enemy)
{
	pcCharacter friendchara = sg_pDungeonSystem->キャラクター生成(ID,me()->CLASS,me()->Forse,sg_pDungeonSystem->キャラ視点方角地形(me(),4,1));
	if(friendchara)
	{
		map<tstring, StyleString> val;
		val[_T("Chara")] = ShortName();
		val[_T("Chara2")] = friendchara->ShortName();
		g_Langメッセージ(_T("cMob_ID_64_特殊能力メッセージ"),val);

		friendchara->Stomach = me()->Stomach - UseSPOfspecialAttack();
		sg_pDungeonSystem->強制ターンエンド要請(friendchara);
		GameIdiom::キャラの方を向く(friendchara,enemy);
		return true;
	}
	return false;
}
void cMob_ID_64::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	/*
	if(timing == ダメージ計算攻撃時_タイミング)
	{
		valiable.intsets.val(変数_属性).insert(攻撃属性::気);
	}
	*/
}
//---------------------------------------------------------------
//ヒジリ
int cMob_ID_65::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{
		if(FindUtility::キャラの距離(me(),pchara) <= 4)//視界内
		{
			return true;
		}
	}
	return false;
}
int cMob_ID_65::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{
		if(FindUtility::キャラの距離(me(),pchara) <= 4)//視界内
		{
			map<tstring, StyleString> val;
			val[_T("Chara")] = FullName();
			g_Langメッセージ(_T("cMob_ID_65_特殊能力メッセージ"),val);
			sg_pDungeonSystem->頑強要請(me(),ValiableConstant1(),ValiableConstant2());
			sg_pDungeonSystem->元気要請(me(),ValiableConstant1(),ValiableConstant2());
			return true;
		}
	}

	return false;
}
void cMob_ID_65::パッシブ能力(タイミング timing, cValiableField& valiable)
{

	if(timing == 魔法接近直前_タイミング)
	{
		//跳ね返し
		if(!valiable.drops.val(変数_対象落ち物)->跳ね返し無効フラグ)
		{
			valiable.doubles.val(変数_汎用ブール) = 0;//効果発揮フラグ

			map<tstring, StyleString> val;
			val[_T("Chara")] = FullName();
			g_Langメッセージ(_T("cMob_ID_15_特殊能力メッセージ"),val);
			
			sg_pDungeonSystem->方向転換要請(me(), valiable.doubles.val(変数_方向)+4);
			sg_pDungeonSystem->魔法発射要請(me(), valiable.drops.val(変数_対象落ち物));
		}
	}
}
//---------------------------------------------------------------
//ヌエ
pcLandform cMob_ID_66::AIお気に入り地形(cValiableField& valiable)
{
	if(me()->Condition.変身状態())
	{
		return NULLLAND;
	}
	vector<pcCharacter> vpchara = FindUtility::透明でない部屋内と隣接味方リスト(me());

	int mindif = 50;
	pcCharacter nearchara = NULLCHARA;
	int i;
	for(i=0;i<vpchara.size();i++)
	{
		if(!sg_pDungeonSystem->キャラクター敵対判定(me(), vpchara[i]))
		{
			int dif = FindUtility::キャラの距離(me(), vpchara[i]);
			if(dif < mindif)
			{
				mindif = dif;
				nearchara = vpchara[i];
			}
		}
	}
	if(nearchara)
	{
		nearchara->足元地形();
	}
	return NULLLAND;
}
int cMob_ID_66::AI特殊攻撃選択(cValiableField& valiable)
{
	if(me()->Condition.変身状態())
	{
		return false;
	}
	vector<pcCharacter> vpchara = FindUtility::透明でない部屋内と隣接味方リスト(me());

	int i;
	for(i=0;i<vpchara.size();i++)
	{
		if(FindUtility::キャラが味方で直接攻撃可能か(me(), vpchara[i]))
		{
			return true;
		}
	}
	return false;
}
int cMob_ID_66::特殊攻撃効果(cValiableField& valiable)
{
	if(me()->Condition.変身状態())
	{
		return false;
	}
	vector<pcCharacter> vpchara = FindUtility::透明でない部屋内と隣接味方リスト(me());

	int i;
	for(i=0;i<vpchara.size();i++)
	{
		if(FindUtility::キャラが味方で直接攻撃可能か(me(), vpchara[i]))
		{
			sg_pDungeonSystem->変身要請(me(),vpchara[i], GAME_TURN_GAMEOVER, 0);
		}
	}
	return false;
}
void cMob_ID_66::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	if(timing == 被攻撃接近直後_タイミング)
	{
		if(me()->Condition.変身状態())
		{
			EffectFunctions::煙エフェクト1(me()->placeX,me()->placeY);
			sg_pDungeonSystem->変身要請(me(),NULLCHARA,-1,false);
		}
	}
}
//---------------------------------------------------------------
//カナコ
int cMob_ID_67::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{
		int zure = FindUtility::キャラ八方ズレ数(me(),pchara);
		cValiableField val;
		if(zure <= 0 && FindUtility::キャラの距離(me(),pchara) <= ValiableConstant2())
		{
			return true;
		}
	}
	return false;
}
int cMob_ID_67::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
	{
		int zure = FindUtility::キャラ八方ズレ数(me(),pchara);
		cValiableField val;
		if(zure <= 0 && FindUtility::キャラの距離(me(),pchara) <= ValiableConstant2())
		{
			GameIdiom::キャラの方を向く(me(),pchara);

			sg_pDungeonSystem->AnimationManager().
				Anime_PlaySE(_T("shoot.wav"),me()->足元地形()->place);

			pcDroping newpdrop = sg_pDungeonSystem->落ち物生成_設置なし(2026);
			//一本
			newpdrop->quality() = 0;
			newpdrop->state() = cDroping::STATE_NORMAL;
			newpdrop->discover();

			newpdrop->dpack.integer[6] = ValiableConstant1();
			newpdrop->dpack.integer[7] = ValiableConstant3();

			return sg_pDungeonSystem->投擲要請(me(),newpdrop,DRAWTYPE_ATTACKDRAW,ValiableConstant2());
		}
	}

	return false;
}
void cMob_ID_67::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	MobAbilityIdiom::投擲物反射CutIn()(me(), timing, valiable);
}
//---------------------------------------------------------------
//レティ
int cMob_ID_68::AI特殊攻撃選択(cValiableField& valiable)
{
	//pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;
	vector<pcCharacter> vpchara;
	sg_pDungeonSystem->キャラクター目視可範囲キャラリスト(me(),vpchara);
	int i;
	for(i=0;i<vpchara.size();i++)
	{
		if(me() != vpchara[i])
		{
			return true;
		}
	}
	return false;
}
int cMob_ID_68::特殊攻撃効果(cValiableField& valiable)
{
	//pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;
	vector<pcCharacter> vpchara,vpobject;
	sg_pDungeonSystem->キャラクター目視可範囲キャラリスト(me(),vpchara);
	int i;
	for(i=0;i<vpchara.size();i++)
	{
		if(me() != vpchara[i])
		{
			vpobject.push_back(vpchara[i]);
		}
	}
	if(!vpobject.empty())
	{
		pcCharacter pchara = vpobject.at(vpobject.size()*random());
		if(pchara != NULL)
		{

			valiable.lands[変数_攻撃場所] = pchara->足元地形();
			valiable.doubles.dim(_T("toPlaceX")) = pchara->placeX;
			valiable.doubles.dim(_T("toPlaceY")) = pchara->placeY;
			特殊攻撃_アニメ(valiable);
			multiset<攻撃属性::攻撃属性> 属性;
			属性.insert(攻撃属性::冷);
			
			sg_pDungeonSystem->攻撃接近(攻撃作成(
				me(),//攻撃者
				pchara->足元地形(),//場所
				ValiableConstant2(),//攻撃力
				定数ダメージフラグ,//定数ダメージ
				属性,
				true//必中
				));

			sg_pDungeonSystem->氷付け要請(pchara,ValiableConstant1());
			return true;
		}
	}
	return false;
}
void cMob_ID_68::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	if(timing == ダメージ計算攻撃時_タイミング)
	{
		valiable.intsets.val(変数_属性).insert(攻撃属性::冷);
	}
	else if(timing == 氷付け追加直前_タイミング)
	{
		valiable.doubles.val(変数_汎用ブール) = 0;//効果発揮フラグ
	}
}
//---------------------------------------------------------------
//リリー
int cMob_ID_69::AI特殊攻撃選択(cValiableField& valiable)
{
	vector<pcCharacter> vpchara = FindUtility::フロア敵索敵_敵リスト(me(), ValiableConstant1());

	if(vpchara.size() >= 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}
int cMob_ID_69::特殊攻撃効果(cValiableField& valiable)
{
	vector<pcCharacter> vpchara = FindUtility::フロア敵索敵_敵リスト(me(), ValiableConstant1());

	pcCharacter pchara = FindUtility::敵リストから優先敵索敵(vpchara, me());

	if(pchara == NULL) return false;
	EffectFunctions::煙エフェクト1(me()->placeX, me()->placeY);
	sg_pDungeonSystem->ワープ要請(me(), sg_pDungeonSystem->キャラ視点方角地形(pchara,4,1));
	GameIdiom::キャラの方を向く(me(),pchara);
	EffectFunctions::煙エフェクト1(me()->placeX, me()->placeY);
	map<tstring, StyleString> val;
	val[_T("Chara")] = ShortName();
	g_Langメッセージ(_T("cMob_ID_69_特殊能力メッセージ"),val);

	return true;
}
//---------------------------------------------------------------
//ハタテ
bool cMob_ID_70::AI移動許可(cValiableField& valiable)
{
	vector<pcCharacter> vpchara;
	pcCharacter pchara;
	sg_pDungeonSystem->キャラクター目視可範囲キャラリスト(me(), vpchara);

	int i, size = (int)vpchara.size();
	for(i=0;i<size;i++)
	{
		pchara = vpchara[i];
		int distance = FindUtility::キャラの距離(me(),pchara);
		if(pchara && distance >= 2 && distance <= ValiableConstant1() && sg_pDungeonSystem->キャラクター敵対判定(me(),pchara))
		{
			return false;
		}
	}
	return true;
}
int cMob_ID_70::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	int distance = FindUtility::キャラの距離(me(),pchara);
	if(pchara && distance >= 2 && distance <= ValiableConstant1())
	{
		return true;
	}
	return false;
}
int cMob_ID_70::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	int distance = FindUtility::キャラの距離(me(),pchara);
	if(pchara && sg_pDungeonSystem->キャラクター敵対判定(me(), pchara) && distance >= 2 && distance <= ValiableConstant1())
	{
		GameIdiom::キャラの方を向く(me(),pchara);
		cValiableField val;
		double temp;
		c4DVector v(pchara->placeX - me()->placeX,pchara->placeY - me()->placeY,0,0);
		val.doubles.dim(tstring(_T("angle")),temp = degree(v));
		val.doubles.dim(tstring(_T("toPlaceX")),temp = pchara->placeX);
		val.doubles.dim(tstring(_T("toPlaceY")),temp = pchara->placeY);
		特殊攻撃_アニメ(val);

		multiset<攻撃属性::攻撃属性> 属性;
		pcAttackinformation pca = 攻撃作成(
			me(),//Attacker
			pchara->足元地形(),//Landplase
			me()->GetAttackPower(),//attackPower
			0,//damageflag
			属性,//attribute
			false
			);
		sg_pDungeonSystem->攻撃接近(pca);
		return true;

	}
	return false;
}
pcEnemyAI cMob_ID_70::Get_kindofAI()
{
	return pcEnemyAI(new ceaiRoomKeeper);
}
//-----------------------------------------------------------------
void cMob_ID_sangessei::setfriend(pcCharacter friendchara1, pcCharacter friendchara2, pcCharacter friendchara3)
{
	pcMob_ID_sangessei pchara1 = boost::dynamic_pointer_cast<cMob_ID_sangessei>(friendchara1);
	pcMob_ID_sangessei pchara2 = boost::dynamic_pointer_cast<cMob_ID_sangessei>(friendchara2);
	pcMob_ID_sangessei pchara3 = boost::dynamic_pointer_cast<cMob_ID_sangessei>(friendchara3);
	if(pchara1 && pchara2 && pchara3)
	{
		pchara1->friendcharas.push_back(wpcCharacter(pchara2));
		pchara1->friendcharas.push_back(wpcCharacter(pchara3));
		pchara2->friendcharas.push_back(wpcCharacter(pchara1));
		pchara2->friendcharas.push_back(wpcCharacter(pchara3));
		pchara3->friendcharas.push_back(wpcCharacter(pchara1));
		pchara3->friendcharas.push_back(wpcCharacter(pchara2));
		pchara1->mode_together = true;
		pchara2->mode_together = true;
		pchara3->mode_together = true;

		if(pchara1->Condition.眠りで行動不能である())
		{
			pchara2->Condition.深い居眠り追加();
			pchara3->Condition.深い居眠り追加();
		}
		else
		{
			pchara2->Condition.とても強い刺激();
			pchara3->Condition.とても強い刺激();
		}
	}
}
bool cMob_ID_sangessei::checkFriendLive()
{
	int i,size = friendcharas.size();
	for(i=0;i<size;i++)
	{
		pcCharacter pchara = friendcharas[i].lock();
		if(!(pchara) || pchara->HP <= 0 )
		{
			return false;
		}
	}
	return true;
}
void cMob_ID_sangessei::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	if(timing == ターン終了_タイミング)
	{
		if(mode_together && !checkFriendLive())
		{
			sg_pDungeonSystem->擬態要請(me(),-1,false);
			sg_pDungeonSystem->びっくり要請(me(),1,false);
			mode_together = false;
		}
	}
	if(timing == 攻撃可判定_タイミング)
	{
		if(mode_together && !checkFriendLive())
		{
			valiable.doubles.val(変数_汎用ブール) *= 0;//不許可
		}
	}
	if(timing == 被攻撃接近直後_タイミング)
	{
		int i,size = friendcharas.size();
		for(i=0;i<size;i++)
		{
			pcCharacter pchara = friendcharas[i].lock();
			if(pchara && !pchara->死亡() )
			{
				if(pchara->Condition.眠りで行動不能である())
				{
					pchara->Condition.とても強い刺激();
				}
			}
		}
	}
}
pcLandform cMob_ID_sangessei::AIお気に入り地形(cValiableField& valiable)
{
	if(mode_together && checkFriendLive())
	{
		int i,size = friendcharas.size();
		for(i=0;i<size;i++)
		{
			cCoordinate coo(friendcharas[i].lock()->placeX - me()->placeX, friendcharas[i].lock()->placeY - me()->placeY);
			if(coo.dif() > 1)
			{
				return friendcharas[i].lock()->足元地形();
			}
		}
	}
	return NULLLAND;
}
//サニー
void cMob_ID_72::配置処理()
{
	sg_pDungeonSystem->擬態要請(me(),GAME_TURN_GAMEOVER,false);
	cMob_ID_sangessei::setfriend(me(),
		sg_pDungeonSystem->キャラクター生成(CCHARACTER_cMob_BASE_ID_NUM+73,me()->CLASS,me()->Forse,me()->足元地形() ),
		sg_pDungeonSystem->キャラクター生成(CCHARACTER_cMob_BASE_ID_NUM+74,me()->CLASS,me()->Forse,me()->足元地形() ));

}
//ルナ
void cMob_ID_73::配置処理()
{
	sg_pDungeonSystem->擬態要請(me(),GAME_TURN_GAMEOVER,false);
}
//スター
void cMob_ID_74::配置処理()
{
	sg_pDungeonSystem->擬態要請(me(),GAME_TURN_GAMEOVER,false);
}

//キモケーネ
int cMob_ID_75::AI特殊攻撃選択(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	return FindUtility::キャラが敵で直接攻撃可能か(me(),pchara);
}
int cMob_ID_75::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;
	pcLandform pland = valiable.lands[変数_攻撃場所];

	if(FindUtility::キャラが敵で直接攻撃可能か(me(),pchara))
	{
		GameIdiom::キャラの方を向く(me(),pchara);

		pcCharacter frontChara = sg_pDungeonSystem->キャラ前地形(me(),1)->pOnChar;
		

		特殊攻撃_アニメ(valiable);

		if(frontChara) {
			if(sg_pDungeonSystem->吹き飛ばし要請(me(),frontChara,me()->aspect,ValiableConstant1(),ValiableConstant2()))
			{
				if(sg_pDungeonSystem->ころび要請(frontChara,ValiableConstant3()))
				{
					map<tstring, StyleString> val;
					val[_T("Chara")] = frontChara->ShortName();
					g_Langメッセージ(_T("cMob_ID_75_特殊能力メッセージ"),val);
				}
			
			}
		}
	
	}

	return true;
}
//-----------------------------------------------------------------
//カセン
void cMob_ID_76::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	if(timing == 魔法接近直前_タイミング || timing == 投擲攻撃接近直前_タイミング)
	{
		pcDroping pdrop = valiable.drops.val(変数_対象落ち物);
		pcCharacter pchara = pdrop->投擲者();
		if(pchara || sg_pDungeonSystem->キャラクター敵対判定(me(),pchara)) {
			if(UseSPOfspecialAttack() <= Stomach) {
				sg_pDungeonSystem->満腹度減少要請(me(),UseSPOfspecialAttack(),false);
				GameIdiom::キャラの方を向く(me(),pchara);
				int summonID = 0;
				if(random() > 0.5) {
					summonID = CCHARACTER_cMob_BASE_ID_NUM+77;
				}
				else {
					summonID = CCHARACTER_cMob_BASE_ID_NUM+78;
				}
				pcCharacter friendchara = sg_pDungeonSystem->キャラクター生成(summonID,CLASS,me()->Forse,sg_pDungeonSystem->キャラ前地形(me(),1));
				if(friendchara)
				{
					map<tstring, StyleString> val;
					val[_T("Chara")] = ShortName();
					val[_T("Chara2")] = friendchara->ShortName();
					g_Langメッセージ(_T("cMob_ID_76_特殊能力メッセージ"),val);

					sg_pDungeonSystem->強制ターンエンド要請(friendchara);
					GameIdiom::キャラの方を向く(friendchara,pchara);
				}
			}
		}
	}
}
//-----------------------------------------------------------------
//オオワシ
void cMob_ID_77::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	MobAbilityIdiom::攻撃時自分ノックバックCutIn()(me(), timing, valiable);
}

//-----------------------------------------------------------------
//ライジュウ
void cMob_ID_78::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	if(timing == ダメージ計算攻撃時_タイミング)
	{
		valiable.intsets.val(変数_属性).insert(攻撃属性::電気);
	}
}

//-----------------------------------------------------------------
//人形
int cMob_ID_80::AI特殊攻撃選択(cValiableField& valiable)
{
	if(AImemory[0] == 0)
	{
		pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;
		if(FindUtility::キャラの距離(me(),pchara) <= 1)
		{
			return true;	
		}
	}
	return false;
}

int cMob_ID_80::特殊攻撃効果(cValiableField& valiable)
{
	pcCharacter pchara = valiable.lands[変数_攻撃場所]->pOnChar;

	if(AImemory[0] == 0)
	{
		if(FindUtility::キャラが敵で直接攻撃可能か(me(),pchara))
		{
			AImemory[0] = 1;//爆発フラグ

			emotion.insert(EMOTION_ASERI);

			sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(
				&visibleemotion,
				emotion);
			return true;
		}
	
	}
	return false;
}
int cMob_ID_80::AIセミパッシブ特殊攻撃選択(cValiableField& valiable)
{
	if(AImemory[0] != 0)
	{
		return true;
	}
	return false;
}
int cMob_ID_80::セミパッシブ特殊攻撃効果(cValiableField& valiable)
{
	if(AImemory[0] != 0)
	{
		EffectFunctions::爆発エフェクト_中(me()->placeX,me()->placeY,20);
			
		GameIdiom::爆発(me()->足元地形(), me(), ValiableConstant1(), 1, false, false, true, false);
		sg_pDungeonSystem->即死要請(me(),0,1);
		return true;
	}
	return false;
}
//-----------------------------------------------------------------
//ミセニトリ
void cMob_ID_81::配置処理()
{
	shop_use_count_ = 0;
}
pcEnemyAI cMob_ID_81::Get_kindofAI()
{
	return pcEnemyAI(new ceaiRoomKeeper);
}
bool cMob_ID_81::すれ違い許可(pcCharacter pchara)
{
	return cMob::すれ違い許可(pchara) && !((sg_pDungeonSystem->店請求金額(me()) > 0) &&
		(足元() != NULL &&
			(
				足元()->属性.count(落ち物属性::階段)
				|| 足元()->ID() == 9015
			)
		));
}
int cMob_ID_81::canTalk()
{
	return !(sg_pDungeonSystem->キャラクター敵対判定(me(),sg_pDungeonSystem->pPlayerChara()));
}
bool cMob_ID_81::isCanUseShop()
{
	return (ValiableConstant1() > shop_use_count_);
}
int cMob_ID_81::強化資金()
{
	return ValiableConstant2() * (shop_use_count_ + 1);
}
int cMob_ID_81::合成資金()
{
	return ValiableConstant3() * (shop_use_count_ + 1);
}
int cMob_ID_81::TalkEvent()
{
	
	GameIdiom::キャラの方を向く(me(), sg_pDungeonSystem->pPlayerChara());

	if(isCanUseShop())
	{
		//使用可能

		pcControlLayer pccl;
		pcSelectWindow pcsw;

		sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));

		pccl->Init(sg_pDungeonSystem->pDevice_D3D);
		pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

		pcCommand pcommand = pcCommand(new cCommand_NitoriFactory_reinforce(強化資金(), g_Lang(_T("装備品を強化する")) ) );
		pcommand->delegate_ = wpcCommandDelegateObject(boost::static_pointer_cast<cCommandDelegateObject>(boost::dynamic_pointer_cast<cMob_ID_81>(me())));
		pcommand->delegateID_ = delegateID_reinforce;
		pcsw->commandList.push_back(pcommand);

		pcommand = pcCommand(new cCommand_NitoriFactory_combine(合成資金(),g_Lang(_T("アイテムを合成する")) ) );
		pcommand->delegate_ = wpcCommandDelegateObject(boost::static_pointer_cast<cCommandDelegateObject>(boost::dynamic_pointer_cast<cMob_ID_81>(me())));
		pcommand->delegateID_ = delegateID_combine;
		pcsw->commandList.push_back(pcommand);
		
		int strsize = 0;
		int i;
		for(i=0;i<pcsw->commandList.size();i++)
		{
			strsize = max(strsize,pcsw->commandList[i]->caption.length());
		}
		strsize = max(strsize,3);
		strsize = min(strsize,20);

		pcsw->Init(sg_pDungeonSystem->pDevice_D3D, strsize, pcsw->commandList.size());
		pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(2));
		pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(2));

		pcsw->playsound_decide();
		
	}
	else
	{
		g_Langメッセージ(_T("ニトリ製作所資材不足メッセージ"),std::map<tstring, StyleString >());
		sg_pDungeonSystem->メニューを閉じる();
	}
	return true;
}
void cMob_ID_81::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	if(timing == 被攻撃直後時_タイミング)
	{
		pAI->addEnemy(valiable.charas.val(変数_攻撃者));
	}
}
void cMob_ID_81::didEndCommand(cCommand& caller) {
	if (caller.delegateID_ == delegateID_reinforce) {
		shop_use_count_++;
	}
	else if (caller.delegateID_ == delegateID_combine) {
		shop_use_count_++;
	}
}