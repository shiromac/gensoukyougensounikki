#include "stdafx.h"
#include "Book1.h"
#include "EffectFunctions.h"
#include "cDungeonSystem.h"
#include "cAttackinformation.h"
#include "GameIdiom.h"

//------------------------------------------------------------------------------
//メランコリーブック
//------------------------------------------------------------------------------
int cBook_ID_0::効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect,int flag)
{
	if(pchara != NULL)
	{

		//sg_pDungeonSystem->メッセージ(pchara->ShortName() +_T(" は気が重くなった。\n"));
		if(sg_pDungeonSystem->速度減少要請(pchara, 効果量(0)))//効果ターン
		{
			map<tstring, StyleString> valiable;
			valiable[_T("Chara")] = pchara->ShortName();
			g_Langメッセージ(_T("cBook_ID_0効果メッセージ"),valiable);

			sg_pDungeonSystem->動的識別(me());
			return true;
		}
		else
		{
			map<tstring, StyleString> valiable;
			valiable[_T("Chara")] = pchara->ShortName();
			g_Langメッセージ(_T("cBook_ID_0効果無しメッセージ"),valiable);

			//sg_pDungeonSystem->メッセージ(_T("しかし ") + pchara->ShortName() +_T(" には効果が無いようだ。\n"));
		}
	}
	return false;
}

//------------------------------------------------------------------------------
//健脚の魔導書
//------------------------------------------------------------------------------
int cBook_ID_1::効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect,int flag)
{
	if(pchara != NULL)
	{
		//sg_pDungeonSystem->メッセージ(pchara->ShortName() +_T(" は足が軽くなった。\n"));
		if(sg_pDungeonSystem->速度増加要請(pchara, 効果量(0)))//効果ターン
		{
			map<tstring, StyleString> valiable;
			valiable[_T("Chara")] = pchara->ShortName();
			g_Langメッセージ(_T("cBook_ID_1効果メッセージ"),valiable);

			sg_pDungeonSystem->動的識別(me());
			return true;
		}
		else
		{
			map<tstring, StyleString> valiable;
			valiable[_T("Chara")] = pchara->ShortName();
			g_Langメッセージ(_T("cBook_ID_1効果無しメッセージ"),valiable);

			//sg_pDungeonSystem->メッセージ(_T("しかし ") + pchara->ShortName() +_T(" には効果が無いようだ。\n"));
		}
	}
	return false;
}
//------------------------------------------------------------------------------
//千里眼の魔導書
//------------------------------------------------------------------------------
int cBook_ID_2::効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect,int flag)
{

	int i;
	int size = vpland.size();
	for(i=0;i<size;i++)
	{
		sg_pDungeonSystem->ある地点からマッピング(vpland[i]);
		GameIdiom::罠チェック(vpland[i]);
	}

	if(pchara != NULL)
	{

		map<tstring, StyleString> valiable;
		valiable[_T("Chara")] = pchara->ShortName();
		g_Langメッセージ(_T("cBook_ID_2効果メッセージ"),valiable);

		
		if(pchara == sg_pDungeonSystem->pPlayerChara())
		{
			valiable.clear();
			g_Langメッセージ(_T("cBook_ID_2効果メッセージ2"),valiable);
		}
		sg_pDungeonSystem->動的識別(me());

		return true;
	}
	return false;
}
int cBook_ID_2::反射回数()
{
	return 効果量(0);
}

//------------------------------------------------------------------------------
//トンネル
//------------------------------------------------------------------------------
int cBook_ID_3::効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect,int flag)
{


	if(pchara != NULL)
	{
		sg_pDungeonSystem->攻撃接近(攻撃作成(
			投擲者(),//攻撃者
			pchara->足元地形(),//場所
			効果量(0),//攻撃力
			1,//定数ダメージ
			攻撃属性(),
			true));
		

		sg_pDungeonSystem->動的識別(me());
	}
	else
	{
		if(vpland.empty()) return false;
		if(aspect % 2) return false;
		pcLandform pland = vpland.back();
		if(!(sg_pDungeonSystem->隣接地形(pland,aspect+4,1)->through(0,0,0))) return false;
		cCoordinate pos,vec;
		pos = pland->place;
		vec.SetAspect(aspect);
		int i;
		int res = 0;
		int size = max(MAPWIDTH,MAPHEIGHT);
		for(i=0;i<size;i++)//無限ループ防止
		{
			if(pland->breakable())
			{
				res |= sg_pDungeonSystem->穴掘り(pland);
			}
			else
			{
				break;
			}

			pos += vec;
			pland = sg_pDungeonSystem->Map().Land(pos);
		}

		if(res)
		{
			sg_pDungeonSystem->地形整理();
			sg_pDungeonSystem->動的識別(me());
		}
	}
	return true;
}
//------------------------------------------------------------------------------
//火柱
//------------------------------------------------------------------------------
int cBook_ID_4::効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect,int flag)
{
	int i;
	int res = 0;
	int size = min(効果量(0),vpland.size());

	if(state() == cDroping::STATE_GOOD)
	{
		for(i=0;i<size;i++)
		{
			res |= GameIdiom::火柱召喚(投擲者(),vpland[i],10);
		}
	}


	if(pchara != NULL)
	{
		res |= GameIdiom::火柱召喚(投擲者(),pchara->足元地形(),10);

	}

	if(res)
	{
		sg_pDungeonSystem->動的識別(me());
	}

	return res;
}
//------------------------------------------------------------------------------
//水脈
//------------------------------------------------------------------------------
int cBook_ID_5::効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect,int flag)
{

	if(pchara != NULL)
	{
		sg_pDungeonSystem->攻撃接近(攻撃作成(
			投擲者(),//攻撃者
			pchara->足元地形(),//場所
			効果量(0),//攻撃力
			1,//定数ダメージ
			攻撃属性(),
			true));
		

		sg_pDungeonSystem->動的識別(me());
	}
	else
	{
		if(vpland.empty()) return false;
		if(aspect % 2) return false;
		pcLandform pland = vpland.back();
		cCoordinate pos,vec;
		pos = pland->place;
		vec.SetAspect(aspect);
		int i;
		int res = 0;
		int size = max(MAPWIDTH,MAPHEIGHT);
		for(i=0;i<size;i++)//無限ループ防止
		{
			if(pland->breakable())
			{
				res |= sg_pDungeonSystem->水掘り(pland);
			}
			else
			{
				break;
			}

			pos += vec;
			pland = sg_pDungeonSystem->Map().Land(pos);
		}

		if(res)
		{
			sg_pDungeonSystem->地形整理();
			sg_pDungeonSystem->動的識別(me());
		}
	}
	return true;
}
//------------------------------------------------------------------------------
//奇跡
//------------------------------------------------------------------------------
int cBook_ID_6::効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect,int flag)
{

	if(pchara != NULL)
	{

		EffectFunctions::上吹き飛ばし風エフェクト(pchara->placeX,pchara->placeY,true);
		GameIdiom::高飛び(pchara, pchara->足元地形());
		

		sg_pDungeonSystem->動的識別(me());
	}
	else
	{
		if(vpland.empty()) return false;
		if(aspect % 2) return false;
		pcLandform pland = vpland.back();

		int effectflag = 0;
		int i;
		for(i=0;i<vpland.size();i++)
		{
			if(vpland[i]->driable())
			{
				pland = vpland[i];
				effectflag = 1;
				break;
			}
			else if(vpland[i]->breakable())
			{
				pland = vpland[i];
				effectflag = 0;
				break;
			}
		}
		cCoordinate pos,vec;
		pos = pland->place;
		vec.SetAspect(aspect);

		int res = 0;
		int size = max(MAPWIDTH,MAPHEIGHT);
		for(i=0;i<size;i++)//無限ループ防止
		{

			if(effectflag && pland->driable())
			{
				res |= sg_pDungeonSystem->水埋め(pland);
			}
			else if(!effectflag && pland->breakable())
			{
				res |= sg_pDungeonSystem->水掘り(pland);
			}
			else
			{
				break;
			}

			pos += vec;
			pland = sg_pDungeonSystem->Map().Land(pos);
		}

		if(res)
		{
			sg_pDungeonSystem->地形整理();
			sg_pDungeonSystem->動的識別(me());
		}
	}
	return true;
}
//------------------------------------------------------------------------------
//回復
//------------------------------------------------------------------------------
int cBook_ID_7::効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect,int flag)
{

	if(pchara != NULL)
	{
		sg_pDungeonSystem->AnimationManager().
			Anime_PlaySE(_T("recover.wav"),pchara->足元地形()->place);
		

		if(pchara->CharaAttribute().count(キャラ属性::幽霊))
		{
			sg_pDungeonSystem->攻撃接近(攻撃作成(
			投擲者(),//攻撃者
			pchara->足元地形(),//場所
			効果量(0),//攻撃力
			1,//定数ダメージ
			攻撃属性(),
			true));
		}
		else sg_pDungeonSystem->回復要請(pchara,効果量(0));
		

		sg_pDungeonSystem->動的識別(me());
	}
	return true;
}
//------------------------------------------------------------------------------
//幸せ
//------------------------------------------------------------------------------
int cBook_ID_8::効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect,int flag)
{

	if(pchara != NULL)
	{
		if(state() == cDroping::STATE_GOOD)
		{
			EffectFunctions::煙エフェクト1(pchara->placeX,pchara->placeY);
			sg_pDungeonSystem->レベルアップ(pchara,1);
		}
		else
		{
			if(pchara->雑魚属性())
			{
				EffectFunctions::煙エフェクト1(pchara->placeX,pchara->placeY);
				sg_pDungeonSystem->レベルアップ(pchara,1);
			}
			else
			{
				EffectFunctions::煙エフェクト1(pchara->placeX,pchara->placeY);
				sg_pDungeonSystem->経験値獲得(pchara,効果量(0),1);
			}
		}

		sg_pDungeonSystem->動的識別(me());
	}
	return true;
}

//------------------------------------------------------------------------------
//不幸
//------------------------------------------------------------------------------
int cBook_ID_9::効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect,int flag)
{

	if(pchara != NULL)
	{
		if(sg_pDungeonSystem->レベルダウン(pchara,効果量(0)))
		{
			EffectFunctions::煙エフェクト1(pchara->placeX,pchara->placeY);
			sg_pDungeonSystem->動的識別(me());
		}
	}
	return true;
}

//------------------------------------------------------------------------------
//ふきとばし
//------------------------------------------------------------------------------
int cBook_ID_10::効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect,int flag)
{

	if(pchara != NULL)
	{
		if(sg_pDungeonSystem->吹き飛ばし要請(投擲者(),pchara,aspect,効果量(0),効果量(1)))
		{
			sg_pDungeonSystem->動的識別(me());
		}
	}
	else if(pdrop != NULL)
	{
		if(sg_pDungeonSystem->吹き飛ばし要請(投擲者(),pdrop,aspect,効果量(0)))
		{
			sg_pDungeonSystem->動的識別(me());
		}
	}
	return true;
}
//------------------------------------------------------------------------------
//ひきよせ
//------------------------------------------------------------------------------
int cBook_ID_11::効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect,int flag)
{

	if(pchara != NULL)
	{
		if(sg_pDungeonSystem->引き寄せ要請(投擲者(),pchara,aspect+4,効果量(0)))
		{
			if(投擲者()->Forse == CHARACTER_FORSE_FRIEND)
			{
				sg_pDungeonSystem->動的識別(me());
			}
		}
	}
	else if(pdrop != NULL)
	{
		if(sg_pDungeonSystem->引き寄せ要請(投擲者(),pdrop,aspect+4,効果量(0)))
		{
			if(投擲者()->Forse == CHARACTER_FORSE_FRIEND)
			{
				sg_pDungeonSystem->動的識別(me());
			}
		}
	}
	return true;
}
//------------------------------------------------------------------------------
//場所替え
//------------------------------------------------------------------------------
int cBook_ID_12::効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect,int flag)
{

	if(pchara != NULL)
	{
		if(sg_pDungeonSystem->場所替え要請(投擲者(),pchara))
		{
			EffectFunctions::煙エフェクト1(pchara->placeX,pchara->placeY);
			sg_pDungeonSystem->動的識別(me());
		}
	}
	return true;
}
//------------------------------------------------------------------------------
//金縛り
//------------------------------------------------------------------------------
int cBook_ID_13::効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect,int flag)
{

	if(pchara != NULL)
	{
		if(sg_pDungeonSystem->金縛り要請(pchara,効果量(0)))
		{
			sg_pDungeonSystem->動的識別(me());
		}
	}
	return true;
}
//------------------------------------------------------------------------------
//泥酔
//------------------------------------------------------------------------------
int cBook_ID_14::効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect,int flag)
{

	if(pchara != NULL)
	{
		if(sg_pDungeonSystem->泥酔要請(pchara,効果量(0)))
		{
			sg_pDungeonSystem->動的識別(me());
		}
	}
	return true;
}
//------------------------------------------------------------------------------
//封印
//------------------------------------------------------------------------------
int cBook_ID_15::効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect,int flag)
{

	if(pchara != NULL)
	{
		if(sg_pDungeonSystem->封印要請(pchara,効果量(0)))
		{
			sg_pDungeonSystem->動的識別(me());
		}
	}
	return true;
}
//------------------------------------------------------------------------------
//空振り
//------------------------------------------------------------------------------
int cBook_ID_16::効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect,int flag)
{

	if(pchara != NULL)
	{
		if(sg_pDungeonSystem->空振り要請(pchara,効果量(0)))
		{
			sg_pDungeonSystem->動的識別(me());
		}
	}
	return true;
}
//------------------------------------------------------------------------------
//貧乏
//------------------------------------------------------------------------------
int cBook_ID_17::効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect,int flag)
{

	if(pchara != NULL)
	{
		if(sg_pDungeonSystem->貧乏要請(pchara,効果量(0)))
		{
			sg_pDungeonSystem->動的識別(me());
		}
	}
	return true;
}
//------------------------------------------------------------------------------
//パチュリーの魔導書（敵専用）
//------------------------------------------------------------------------------
void cBook_ID_18::AppreciationBeginOptimize(int difficulty)
{
	cBook::AppreciationBeginOptimize(difficulty);
	sg_pDungeonSystem->識別(me(),0);
}
int cBook_ID_18::効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect,int flag)
{

	if(pchara != NULL)
	{
		multiset<攻撃属性::攻撃属性> attr;
		sg_pDungeonSystem->攻撃接近(攻撃作成(
		投擲者(),//攻撃者
		pchara->足元地形(),//場所
		power(),//攻撃力
		1,//定数ダメージ
		attr,
		true));
	}
	return true;
}
//------------------------------------------------------------------------------
//アヤの風の魔導書（敵専用）
//------------------------------------------------------------------------------
void cBook_ID_19::AppreciationBeginOptimize(int difficulty)
{
	cBook::AppreciationBeginOptimize(difficulty);
	sg_pDungeonSystem->識別(me(),0);
}
int cBook_ID_19::効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect,int flag)
{

	if(pchara != NULL)
	{
		if(sg_pDungeonSystem->吹き飛ばし要請(投擲者(),pchara,aspect,distancepower(),damagepower()))
		{

		}
	}
	else if(pdrop != NULL)
	{
		if(sg_pDungeonSystem->吹き飛ばし要請(投擲者(),pdrop,aspect,distancepower()))
		{

		}
	}
	return true;
}