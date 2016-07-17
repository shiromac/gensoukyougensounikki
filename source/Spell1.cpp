#include "stdafx.h"
#include "Spell1.h"
#include "EffectFunctions.h"
#include "SpellEffects.h"
#include "GameIdiom.h"
#include "FindUtility.h"
#include "cDungeonSystem.h"
#include "cAttackinformation.h"
#include "MobAbilityIdiom.h"

bool cSpell_能力仕様フラグID_exist(cValiableField& valiable, int ID)
{
	return valiable.doubles.exist((_T("cSpell_能力仕様フラグID_") + setStyle(ID)).c_str());
}
double& cSpell_能力仕様フラグID_dim(cValiableField& valiable, int ID)
{
	return valiable.doubles.dim((_T("cSpell_能力仕様フラグID_") + setStyle(ID)).c_str());
}
double& cSpell_能力仕様フラグID_val(cValiableField& valiable, int ID)
{
	return valiable.doubles.val((_T("cSpell_能力仕様フラグID_") + setStyle(ID)).c_str());
}

bool cSpell::CutInMobAbilityIdiom(const MobAbilityIdiom::CutInFunctionObject& functor, pcCharacter pchara, タイミング timing, cValiableField& valiable, bool enabledSpellDamage, bool enabledActiveIdentify, bool guardDuplicateAbility)
{
	if(!guardDuplicateAbility || !cSpell_能力仕様フラグID_exist(valiable,ID())) {
		if(functor(装備者(), timing, valiable)) {
			if(guardDuplicateAbility) {
				cSpell_能力仕様フラグID_dim(valiable,ID()) = 1;
			}
			if(enabledSpellDamage) {
				Breakcrashprocess(効果時腕輪ダメージ());
			}
			if(enabledActiveIdentify) {
				sg_pDungeonSystem->動的識別(me());
			}
			return true;
		}
	}
	return false;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//覚識「森近霖之助」
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cSpell_ID_0::効果(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{

	if(ObjectList.size())
	{
		sg_pDungeonSystem->識別(ObjectList[0], true);
	
		sg_pDungeonSystem->動的識別(me());
	}
	return true;
}
int cSpell_ID_0::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	SpellEffects::EffectcSpell_ID_0(pchara->placeX,pchara->placeY);
	
	効果(pchara, ObjectList);
	return true;//必ずtrueを返す
}
int cSpell_ID_0::宣言_効果_祝福(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	return 宣言_効果_通常(pchara,ObjectList);
}
void cSpell_ID_0::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);
	
	if(装備されている())
	{
		if( MobAbilityIdiom::フロア開始時所持アイテムランダム識別CutIn(効果量(0))(装備者(), timing, valiable) )
		{
			sg_pDungeonSystem->動的識別(me());
			Breakcrashprocess(効果時腕輪ダメージ());
		}
	}

}
int cSpell_ID_0::スキル効果(pcCharacter pchara, vector<pcDroping> &ObjectList)
{
	if(ObjectList.size() <= 0) return false;
	
	pcDroping pdrop = ObjectList[0];

	if(!pdrop->修正値識別済み() || pdrop->状態値識別済み())
	{
		sg_pDungeonSystem->状態のみ識別( pdrop, 0);
		sg_pDungeonSystem->修正のみ識別( pdrop, 0);
		sg_pDungeonSystem->動的識別(me());
		
		return true;
	}
	return false;
}
	
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//リグル
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cSpell_ID_1::効果(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	sg_pDungeonSystem->全マッピング();
	/*
	vector<pcDroping> vpdrop = sg_pDungeonSystem->DropList();
	int i, size = vpdrop.size();
	for(i=0;i<size;i++)
	{
		if(!vpdrop[i]->属性.count(落ち物属性::罠))
		{
			sg_pDungeonSystem->アイテム強制マッピング(vpdrop[i]);
		}
	}
	*/
	sg_pDungeonSystem->フロア明かり追加();

	map<tstring, StyleString> valiable;
	g_Langメッセージ(_T("cSpell_ID_1効果メッセージ"),valiable);


	return true;
}
int cSpell_ID_1::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);

	効果(pchara, ObjectList);
	return true;//必ずtrueを返す
}
int cSpell_ID_1::宣言_効果_祝福(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	return 宣言_効果_通常(pchara,ObjectList);
}
void cSpell_ID_1::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);
	
	if(装備されている())
	{
		if(timing == キャラクター目視範囲計算時_タイミング)
		{
			valiable.doubles[変数_汎用実数] += 効果量(0);
			sg_pDungeonSystem->動的識別(me());
		}
		else if(timing == 敵起こしやすさ倍率計算時_タイミング)
		{
			valiable.doubles[変数_汎用実数] *= 効果量(1);
		}
	}
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//tei
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cSpell_ID_2::効果()
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
		g_Langメッセージ(_T("cSpell_ID_2効果メッセージ"),valiable);
	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_2::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);

	効果();
	return true;//必ずtrueを返す
}
void cSpell_ID_2::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);

	if(装備されている() && timing == 罠発動率計算時_タイミング)
	{
		valiable.doubles[変数_汎用実数] *= 0;
		Breakcrashprocess(効果時腕輪ダメージ());
		sg_pDungeonSystem->動的識別(me());
	}

}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//reimu
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void cSpell_ID_3::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);

	if(装備されている())
	{
		int damage = 装備者()->MHP/効果量(1);
		if(damage < 1) {
			damage = 1;
		}
		if(MobAbilityIdiom::投擲物魔法ダメージ化CutIn(damage)(装備者(),timing,valiable)) {
			Breakcrashprocess(効果時腕輪ダメージ());
			sg_pDungeonSystem->動的識別(me());
		}

		if(MobAbilityIdiom::遠距離ダメージ軽減CutIn(効果量(2))(装備者(),timing,valiable)) {
			Breakcrashprocess(効果時腕輪ダメージ());
			sg_pDungeonSystem->動的識別(me());
		}
	}

}
int cSpell_ID_3::効果(pcCharacter pchara)
{

	int eff = 0;

	vector<pcCharacter> vpchara = sg_pDungeonSystem->CharaList();
	
	int i;
	for(i = 0; i<vpchara.size() ;i++)
	{
		if( sg_pDungeonSystem->キャラクター敵対判定(pchara, vpchara[i])
			|| sg_pDungeonSystem->キャラクター敵対判定(sg_pDungeonSystem->pPlayerChara(),vpchara[i]))
		{
			sg_pDungeonSystem->封印要請(vpchara[i],効果量(0),0);
			eff++;
		}
	}

	if(eff)
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("cSpell_ID_3効果メッセージ"),valiable);
	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_3::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);

	効果(pchara);
	return true;//必ずtrueを返す
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//marisa
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void cSpell_ID_4::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);

	if(装備されている() && timing == 射撃攻撃力計算時_タイミング)
	{
		valiable.doubles[変数_攻撃力ボーナス_倍率] += 効果量(2)/100.0;
		//Breakcrashprocess(効果時腕輪ダメージ());
		sg_pDungeonSystem->動的識別(me());
	}

}
int cSpell_ID_4::効果(pcCharacter pchara, int aspect)
{

	int eff = 0;


	pcLandform pbaseland = pchara->足元地形();
	int size = 効果量(1);
	int i;
	for(i = 0; i<size ;i++)
	{

		int k;
		for(k = -1; k <= 1 ;k++)
		{
			pcLandform pland = sg_pDungeonSystem->隣接地形(pbaseland,aspect+k,1);

			if( pland->placeX == -1 || pland->placeY == -1)
			{//エッジ
				break;
			}

			int a;
			for(a=0;a<8;a+=2)
			{
				if(sg_pDungeonSystem->隣接地形(pland,a,1)->through(0,0,0))
				{
					sg_pDungeonSystem->穴掘り(pland,0);
					break;
				}
			}
		
			multiset<攻撃属性::攻撃属性> temp;
			double power = 1;
			if(pland->pOnChar)
			{
				power = pland->pOnChar->MHP * 効果量(0) / 100.0;
			}
			sg_pDungeonSystem->攻撃接近(攻撃作成(
				pchara,//攻撃者
				pland,//場所
				power,//攻撃力
				1,//定数ダメージ
				temp,
				true));//必中
		}
		
		pbaseland = sg_pDungeonSystem->隣接地形(pbaseland,aspect,1);
	}

	sg_pDungeonSystem->地形整理();

	return eff;
}
int cSpell_ID_4::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	SpellEffects::EffectcSpell_ID_4(pchara->placeX,pchara->placeY,pchara->aspect);

	効果(pchara, pchara->aspect);
	return true;//必ずtrueを返す
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//hina
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void cSpell_ID_5::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);

	if(装備されている() && timing == 被攻撃接近直前_タイミング && !cSpell_能力仕様フラグID_exist(valiable,ID()))
	{//回転防御
		if(装備者()->Condition.貧乏状態()) return;//首が回らない
		if(valiable.intsets[変数_属性].count(攻撃属性::必中)) return;

		int a;
		for(a=0;a<8;a++)
		{
			pcCharacter penemy = sg_pDungeonSystem->キャラ視点方角地形(装備者(),a,1)->pOnChar;
			if(penemy == NULL) continue;
			if(!sg_pDungeonSystem->キャラクター敵対判定(装備者(),penemy)) continue;
			if(valiable.charas[変数_攻撃者] == penemy) continue;

			//効果発動
			int a2;
			for(a2=0;a2<8;a2++)
			{
				sg_pDungeonSystem->方向転換要請(装備者(),装備者()->aspect+1);
				sg_pDungeonSystem->AnimationManager().Anime_Wait(装備者()->placeX,装備者()->placeY,3);
			}
			
			set<攻撃属性::攻撃属性> temp;

			sg_pDungeonSystem->攻撃接近(攻撃作成(
				valiable.charas[変数_攻撃者],//攻撃者
				sg_pDungeonSystem->キャラ視点方角地形(装備者(),a,1),//場所
				valiable.doubles[変数_攻撃力],//攻撃力
				valiable.doubles[変数_定数ダメージフラグ] + valiable.doubles.exist(変数_割合ダメージフラグ)*2,//定数ダメージ
				属性設定back(valiable.intsets[変数_属性]),
				false));//必中

			valiable.doubles[変数_回避力] += 100;
			cSpell_能力仕様フラグID_dim(valiable,ID()) = 1;
			sg_pDungeonSystem->動的識別(me());
			Breakcrashprocess(効果時腕輪ダメージ());
			break;
		}

	}
	if(装備されている() && !cSpell_能力仕様フラグID_exist(valiable,ID()))
	{
		if(MobAbilityIdiom::呪い無効化CutIn()(装備者(), timing, valiable)) {
			cSpell_能力仕様フラグID_dim(valiable,ID()) = 1;
			sg_pDungeonSystem->動的識別(me());
		}
	}
}
int cSpell_ID_5::効果(pcCharacter pchara)
{

	vector<pcDroping> vpdrop = sg_pDungeonSystem->DropList();

	int i,eff = 0;

	if(pchara->足元() != NULL) vpdrop.push_back(pchara->足元());

	for(i=0;i<vpdrop.size();i++)
	{
		if(vpdrop[i]->state() == cDroping::STATE_CURSE)
		{
			sg_pDungeonSystem->アイテム祝福(vpdrop[i],false);
			eff++;
		}
	}

	sg_pDungeonSystem->呪術異常治療要請(pchara);

	if(eff)
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("cSpell_ID_5効果メッセージ"),valiable);
	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_5::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);

	効果(pchara);
	return true;//必ずtrueを返す
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//youmuヨウム
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void cSpell_ID_6::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);

	if(装備されている() && timing == 攻撃力計算時_タイミング)
	{
		pcDroping pdrop = 装備者()->attackequipment;
		if(pdrop == NULL) return;
		if(pdrop->属性.count(落ち物属性::剣))
		{
			valiable.doubles[変数_攻撃力ボーナス_倍率] += 効果量(2)/100.0;
			//Breakcrashprocess(効果時腕輪ダメージ());
			sg_pDungeonSystem->動的識別(me());
		}
	}

}
int cSpell_ID_6::効果(pcCharacter pchara, int aspect)
{
	int eff = 0;
	eff = 1;

	pcLandform pland = pchara->足元地形();
	int size = 効果量(1);
	int i;
	for(i = 0; i<size ;i++)
	{
		pland = sg_pDungeonSystem->隣接地形(pland,aspect,1);
		if( pland->placeX == -1 || pland->placeY == -1)
		{//エッジ
			break;
		}

		int a;
		int flag = 0;
		if(pland->iswall())
		{//壁
			for(a=0;a<8;a+=2)
			{
				if(sg_pDungeonSystem->隣接地形(pland,a,1)->through(0,0,0))
				{
					if(sg_pDungeonSystem->穴掘り(pland,0))
					{//ほれた
						flag = 1;
						break;
					}
				}
			}

			if(!flag)
			{//壁なのに掘れなかった
				break;
			}
		}
		

		if(pland->pOnChar == NULL || sg_pDungeonSystem->キャラクター敵対判定(pchara,pland->pOnChar))
		{
			pcAttackinformation pai = sg_pDungeonSystem->直接攻撃攻撃情報取得(pchara);
			sg_pDungeonSystem->攻撃接近(攻撃作成(
				pchara,//攻撃者
				pland,//場所
				pai->valiable.doubles[変数_攻撃力] + 効果量(0),//攻撃力
				0,//定数ダメージ
				属性設定back(pai->valiable.intsets[変数_属性]),
				true));//必中	
		}


		sg_pDungeonSystem->ワープ要請(pchara,pland);
	}
	

	sg_pDungeonSystem->地形整理();

	return eff;
}
int cSpell_ID_6::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	SpellEffects::EffectcSpell_ID_6(pchara->placeX,pchara->placeY,pchara->aspect);
	map<tstring, StyleString> valiable;
	valiable[_T("Chara")] = pchara->ShortName();
	g_Langメッセージ(_T("cSpell_ID_6効果メッセージ"),valiable);
	

	効果(pchara, pchara->aspect);
	return true;//必ずtrueを返す
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//れいせんレイセン
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void cSpell_ID_7::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);

	if(装備されている())
	{
		if(timing == 攻撃直後時_タイミング 
			&& !cSpell_能力仕様フラグID_exist(valiable,ID()))
		{
			if(valiable.charas[変数_防御者] != NULL
				&& 効果量(1)/100.0 > random())
			{
				
				sg_pDungeonSystem->空振り要請(valiable.charas[変数_防御者],効果量(2));
				
				//Breakcrashprocess(効果時腕輪ダメージ());
				sg_pDungeonSystem->動的識別(me());
				cSpell_能力仕様フラグID_dim(valiable,ID()) = 1;
			
			}
		}
		else if((
			/*
			timing == 眠り追加直前_タイミング
			 || timing == バクスイ追加直前_タイミング
			 || timing == びっくり追加直前_タイミング
			 || timing == 金縛り追加直前_タイミング
			 || timing == 封印追加直前_タイミング
			 || timing == 空振り追加直前_タイミング
			 || timing == 貧乏追加直前_タイミング
			 || */timing == 錯乱追加直前_タイミング/*
 			 || timing == 臆病追加直前_タイミング
 			 || timing == 嫉妬追加直前_タイミング
 			 || timing == 狂乱追加直前_タイミング*/
			)&& !cSpell_能力仕様フラグID_exist(valiable,ID()))
		{
			valiable.doubles[変数_汎用ブール] = 0;//拒否
			Breakcrashprocess(効果時腕輪ダメージ());
			sg_pDungeonSystem->動的識別(me());
			cSpell_能力仕様フラグID_dim(valiable,ID()) = 1;
		}
	}
}
int cSpell_ID_7::効果(pcCharacter pchara)
{

	int eff = 0;

	vector<pcCharacter> vpchara;
	sg_pDungeonSystem->キャラクター部屋キャラリスト(pchara,vpchara);

	int size = vpchara.size();
	int i;
	for(i = 0; i<size ;i++)
	{
		if(sg_pDungeonSystem->キャラクター敵対判定(pchara,vpchara[i])
			|| sg_pDungeonSystem->キャラクター敵対判定(sg_pDungeonSystem->pPlayerChara(),vpchara[i]))
		{
			sg_pDungeonSystem->空振り要請(vpchara[i],効果量(0));
			eff++;
		}
	}

	if(eff)
	{

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_7::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);

	効果(pchara);
	return true;//必ずtrueを返す
}
 int cSpell_ID_7::スキル効果(pcCharacter pchara, vector<pcDroping> &ObjectList)
 {

	pcLandform pland = sg_pDungeonSystem->キャラ前地形(装備者(),1);

	if(pland->pOnChar != NULL)
	{

		return sg_pDungeonSystem->錯乱要請(pland->pOnChar);
	}
	return true;
 }
	
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//パチュリー
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void cSpell_ID_8::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);

	if(装備されている())
	{
		if(timing == ダメージ計算攻撃時優先度低_タイミング )
		{
			int count = 0;
			count += valiable.intsets[変数_属性].count(攻撃属性::爆発);
			count += valiable.intsets[変数_属性].count(攻撃属性::火);
			count += valiable.intsets[変数_属性].count(攻撃属性::水);
			count += valiable.intsets[変数_属性].count(攻撃属性::冷気);
			count += valiable.intsets[変数_属性].count(攻撃属性::気);
			count += valiable.intsets[変数_属性].count(攻撃属性::電気);

			Breakcrashprocess(効果時腕輪ダメージ());
			if(count) sg_pDungeonSystem->動的識別(me());
			valiable.doubles[変数_攻撃力ボーナス_倍率] += sqrt((double)count)*効果量(1)/100.0;
		}
	}
}
int cSpell_ID_8::効果(pcCharacter pchara)
{

	int eff = 0;

	vector<pcCharacter> vpchara;
	sg_pDungeonSystem->キャラクター部屋キャラリスト(pchara,vpchara);

	int size = vpchara.size();
	int i;
	for(i = 0; i<size ;i++)
	{
		if(sg_pDungeonSystem->キャラクター敵対判定(pchara,vpchara[i])
			|| sg_pDungeonSystem->キャラクター敵対判定(sg_pDungeonSystem->pPlayerChara(),vpchara[i]))
		{

			eff++;
			multiset<攻撃属性::攻撃属性> temp;
			temp.insert(攻撃属性::爆発);
			temp.insert(攻撃属性::火);
			temp.insert(攻撃属性::水);
			temp.insert(攻撃属性::冷気);
			temp.insert(攻撃属性::気);
			temp.insert(攻撃属性::電気);
			sg_pDungeonSystem->攻撃接近(攻撃作成(
				pchara,//攻撃者
				vpchara[i]->足元地形(),//場所
				効果量(0),//攻撃力
				1,//定数ダメージ
				temp,
				true));//必中
		}
	}

	if(eff)
	{

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_8::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);

	効果(pchara);
	return true;//必ずtrueを返す
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//カナコ
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void cSpell_ID_9::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);

	if(装備されている())
	{
		if(timing == 直接攻撃直前_タイミング)
		{
			if(効果量(0)/100.0 > random())
			{
				pcDroping pdrop = 装備者()->attackequipment;
				if(pdrop != NULL) sg_pDungeonSystem->アイテム祝福(pdrop);
				//Breakcrashprocess(効果時腕輪ダメージ());
				sg_pDungeonSystem->動的識別(me());
			}
		}
		else if(timing == 被攻撃接近直前_タイミング )
		{
			if(効果量(0)/100.0 > random())
			{
				pcDroping pdrop = 装備者()->defenseequipment;
				if(pdrop != NULL) sg_pDungeonSystem->アイテム祝福(pdrop);
				//Breakcrashprocess(効果時腕輪ダメージ());
				sg_pDungeonSystem->動的識別(me());
			}
		}
		
		CutInMobAbilityIdiom(MobAbilityIdiom::投擲物反射CutIn(), 装備者(), timing, valiable,
			true,//bool enabledSpellDamage,
			true,//bool enabledActiveIdentify,
			true);//bool guardDuplicateAbility)
	}
}
int cSpell_ID_9::効果(pcCharacter pchara)
{

	int eff = 1;

	if(pchara->足元() != NULL)
	{
		pcDroping pdropstep = pchara->足元();
		int dpack_int[DEF_DROP_DATAPACK_NUM] = {0};
		pcDroping pdrop = sg_pDungeonSystem->落ち物生成_設置なし(11001,dpack_int);
		sg_pDungeonSystem->落ち物すり替え(pdropstep,pdrop);
		sg_pDungeonSystem->落ち物設置(pdropstep,pchara->足元地形());

	}
	else
	{
		int dpack_int[DEF_DROP_DATAPACK_NUM] = {0};
		pcDroping pdrop = sg_pDungeonSystem->落ち物生成(11001,dpack_int,pchara->足元地形());
	}

	if(eff)
	{

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_9::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	if(sg_pDungeonSystem->ボス戦中判定())
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("cSpell_ID_8効果不能メッセージ"),valiable);
		return false;
	}
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);

	効果(pchara);
	return true;//必ずtrueを返す
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//エーリン
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void cSpell_ID_10::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);

	if(装備されている())
	{
		if(timing == 自然回復量計算時_タイミング)
		{
			valiable.doubles.val(変数_回復力ボーナス_倍率) += 効果量(0)/100.0;
			Breakcrashprocess(効果時腕輪ダメージ());
			sg_pDungeonSystem->動的識別(me());
		}
		/*
		else if(timing == 自然満腹度減少量計算時_タイミング)
		{
			valiable.doubles.val(変数_汎用ボーナス_倍率) += 効果量(1)/100.0;
			//Breakcrashprocess(効果時腕輪ダメージ());
			sg_pDungeonSystem->動的識別(me());
		}
		*/
		else if(timing == 脱力追加直前_タイミング && !cSpell_能力仕様フラグID_exist(valiable,ID()))
		{
			valiable.doubles[変数_汎用ブール] = 0;//拒否
			Breakcrashprocess(効果時腕輪ダメージ());
			sg_pDungeonSystem->動的識別(me());
			cSpell_能力仕様フラグID_dim(valiable,ID()) = 1;	
		}
		else if(timing == 軟弱追加直前_タイミング && !cSpell_能力仕様フラグID_exist(valiable,ID()))
		{
			valiable.doubles[変数_汎用ブール] = 0;//拒否
			Breakcrashprocess(効果時腕輪ダメージ());
			sg_pDungeonSystem->動的識別(me());
			cSpell_能力仕様フラグID_dim(valiable,ID()) = 1;	
		}
		else if(timing == 飲み効果直前_タイミング && !cSpell_能力仕様フラグID_exist(valiable,ID()))
		{
			if(valiable.drops[変数_対象落ち物]->属性.count(落ち物属性::薬))
			{
				valiable.doubles[変数_汎用ブール] = 0;//拒否
				Breakcrashprocess(効果時腕輪ダメージ());
				sg_pDungeonSystem->動的識別(me());
				cSpell_能力仕様フラグID_dim(valiable,ID()) = 1;
				map<tstring, StyleString> valiable;
				valiable[_T("Chara")] = 装備者()->ShortName();
				g_Langメッセージ(_T("cSpell_ID_10効果メッセージ"),valiable);
			}
		}
		else if(timing == 飲み物衝突効果直前_タイミング && !cSpell_能力仕様フラグID_exist(valiable,ID()))
		{
			if(valiable.drops[変数_対象落ち物]->属性.count(落ち物属性::薬))
			{
				valiable.doubles[変数_汎用ブール] = 0;//拒否
				Breakcrashprocess(効果時腕輪ダメージ());
				sg_pDungeonSystem->動的識別(me());
				cSpell_能力仕様フラグID_dim(valiable,ID()) = 1;
				map<tstring, StyleString> valiable;
				valiable[_T("Chara")] = 装備者()->ShortName();
				g_Langメッセージ(_T("cSpell_ID_10効果メッセージ"),valiable);
			}
		}
	}
}
int cSpell_ID_10::効果(pcCharacter pchara)
{

	int eff = 0;

	eff |= sg_pDungeonSystem->回復要請(pchara,pchara->MHP);
	eff |= sg_pDungeonSystem->精神異常治療要請(pchara);
	eff |= sg_pDungeonSystem->身体異常治療要請(pchara);

	if(eff)
	{
		
	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_10::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);

	効果(pchara);
	return true;//必ずtrueを返す
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//スワコ
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void cSpell_ID_11::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);


	if(装備されている())
	{
		if(timing == 主人公水上歩行可能判定時_タイミング)
		{
			valiable.doubles.val(変数_汎用ブール) = 1;
			//Breakcrashprocess(効果時腕輪ダメージ());
			//sg_pDungeonSystem->動的識別(me());
		}
		/*
		if(timing == アイテム呪い直前_タイミング)
		{
			if(valiable.drops.val(変数_対象落ち物)->state() == cDroping::STATE_NORMAL)
			{
				valiable.doubles.val(変数_汎用ブール) = 0;
				Breakcrashprocess(効果時腕輪ダメージ());
				sg_pDungeonSystem->動的識別(me());
			}
		}
		*/
	}
}
int cSpell_ID_11::効果(pcCharacter pchara, vector<pcDroping> &ObjectList)
{
	int eff = 0;
	if(!ObjectList.empty())
	{
		pcDroping pdrop = ObjectList[0];

		if(pdrop == NULL) eff = 0;
		else
		{
			if(pdrop->state() == cDroping::STATE_CURSE)
			{
				sg_pDungeonSystem->アイテム祝福(pdrop);
				sg_pDungeonSystem->アイテム祝福(pdrop);
			}
			else
			{
				sg_pDungeonSystem->アイテム呪い(pdrop);
			}
			eff = 1;
		}
	}

	if(eff)
	{
		
	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_11::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);

	効果(pchara,ObjectList);
	return true;//必ずtrueを返す
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//ルーミア
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void cSpell_ID_12::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);


	if(装備されている())
	{
		if(timing == 敵起こしやすさ倍率計算時_タイミング)
		{
			valiable.doubles[変数_汎用実数] *= 0;
		}
	}
}
int cSpell_ID_12::効果(pcCharacter pchara)
{
	int eff = 0;

	eff |= sg_pDungeonSystem->全マッピング解除();
	//eff |= sg_pDungeonSystem->部屋つぶし(pchara->足元地形(),効果量(0));

	pcDroping pdrop = sg_pDungeonSystem->落ち物生成(9030,NULL,pchara->足元地形());
	pdrop->dpack.integer[6] = 1;
	pdrop->discover();
	
	if(eff)
	{
		map<tstring, StyleString> valiable;
		valiable[_T("Chara")] = pchara->ShortName();
		g_Langメッセージ(_T("cSpell_ID_12効果メッセージ"),valiable);
	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_12::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);
	効果(pchara);
	return true;//必ずtrueを返す
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//ユユコ
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void cSpell_ID_13::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);


	if(装備されている())
	{
		/*
		if(timing == 自然満腹度減少量計算時_タイミング)
		{
			valiable.doubles.val(変数_汎用ボーナス_倍率) += 効果量(1)/100.0;
			//Breakcrashprocess(効果時腕輪ダメージ());
			sg_pDungeonSystem->動的識別(me());
		}
		*/
		if(timing == ダメージ時_タイミング)
		{
			valiable.doubles.val(変数_ダメージ) = min(valiable.doubles.val(変数_ダメージ),装備者()->MHP/効果量(2));
			Breakcrashprocess(効果時腕輪ダメージ());
			sg_pDungeonSystem->動的識別(me());
		}
		else if(timing == 即死要請直前_タイミング && !cSpell_能力仕様フラグID_exist(valiable,ID()))
		{
			valiable.doubles[変数_汎用ブール] = 0;//拒否
			Breakcrashprocess(効果時腕輪ダメージ());
			sg_pDungeonSystem->動的識別(me());
			cSpell_能力仕様フラグID_dim(valiable,ID()) = 1;	
		}
		else if(timing == 死の誘い追加直前_タイミング && !cSpell_能力仕様フラグID_exist(valiable,ID()))
		{
			valiable.doubles[変数_汎用ブール] = 0;//拒否
			Breakcrashprocess(効果時腕輪ダメージ());
			sg_pDungeonSystem->動的識別(me());
			cSpell_能力仕様フラグID_dim(valiable,ID()) = 1;	
		}
	}
}
int cSpell_ID_13::効果(pcCharacter pchara)
{
	int eff = 0;

	vector<pcCharacter> vpchara;
	sg_pDungeonSystem->キャラクター部屋キャラリスト(pchara,vpchara);

	int size = vpchara.size();
	int i;
	for(i = 0; i<size ;i++)
	{
		if(sg_pDungeonSystem->キャラクター敵対判定(pchara,vpchara[i])
			|| sg_pDungeonSystem->キャラクター敵対判定(sg_pDungeonSystem->pPlayerChara(),vpchara[i]))
		{
			sg_pDungeonSystem->死の誘い要請(vpchara[i],pchara,効果量(0));
			eff++;
		}
	}

	return eff;
	
	if(eff)
	{

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_13::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);
	効果(pchara);
	return true;//必ずtrueを返す
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//アリス
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void cSpell_ID_14::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);


	if(装備されている())
	{
		/*
		if(timing == スペルダメージ直前_タイミング)
		{
			if(valiable.drops.val(変数_対象落ち物) != me())
			{
				valiable.doubles.val(変数_汎用ブール) = 0;
				Breakcrashprocess(valiable.doubles.val(変数_ダメージ));
				//Breakcrashprocess(効果時腕輪ダメージ());
				//sg_pDungeonSystem->動的識別(me());
			}
		}
		*/
		if(timing == 直接攻撃直前_タイミング)
		{
			valiable.intsets.val(変数_属性).insert(攻撃属性::必中);
			Breakcrashprocess(効果時腕輪ダメージ());
		}
	}
}
int cSpell_ID_14::効果(pcCharacter pchara)
{
	int eff = 0;

	int size = 効果量(0);
	
	pcLandform pland = pchara->足元地形();

	int i;
	for(i=0;i<size;i++)
	{
		pcCharacter pmiga = sg_pDungeonSystem->キャラクター生成(3001,0,CHARACTER_FORSE_FRIEND,pland);
		if(pmiga != NULL)
		{
			pmiga->Condition.とても強い刺激();
			sg_pDungeonSystem->身代わり要請(pmiga,pchara,効果量(1),0);
			sg_pDungeonSystem->眠り要請(pmiga,効果量(1),0);
			sg_pDungeonSystem->無敵要請(pmiga,効果量(1),0);
			eff++;
		}
	}
	
	if(eff)
	{

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_14::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);
	効果(pchara);
	return true;//必ずtrueを返す
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//チルノ
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void cSpell_ID_15::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);


	if(装備されている())
	{
		if(timing == ダメージ計算攻撃時_タイミング)
		{
			valiable.intsets.val(変数_属性).insert(攻撃属性::冷気);
			//Breakcrashprocess(効果時腕輪ダメージ());
			sg_pDungeonSystem->動的識別(me());
		}
		else if(timing == ダメージ計算防御時_タイミング)
		{
			if(valiable.intsets.val(変数_属性).count(攻撃属性::冷気))
			{
				valiable.doubles.val(変数_耐性ボーナス_倍率％) += 効果量(0);
				//Breakcrashprocess(効果時腕輪ダメージ());
				sg_pDungeonSystem->動的識別(me());
			}
			if(valiable.intsets.val(変数_属性).count(攻撃属性::火))
			{
				sg_pDungeonSystem->脱力要請(装備者(),1,効果量(3));
				sg_pDungeonSystem->軟弱要請(装備者(),1,効果量(3));
			}
		}
		else if(timing == 攻撃直後時_タイミング)
		{
			if((効果量(1)/100.0) > random())
			{
				if(!cSpell_能力仕様フラグID_exist(valiable,ID()))
				{//初
					sg_pDungeonSystem->氷付け要請(valiable.charas.val(変数_防御者),効果量(2));

					cSpell_能力仕様フラグID_dim(valiable,ID()) = 1;
				}
			}
		}
		else if(timing == 氷付け追加直前_タイミング && !cSpell_能力仕様フラグID_exist(valiable,ID()))
		{
			valiable.doubles[変数_汎用ブール] = 0;//拒否
			Breakcrashprocess(効果時腕輪ダメージ());
			//sg_pDungeonSystem->動的識別(me());
			cSpell_能力仕様フラグID_dim(valiable,ID()) = 1;	
		}
		else if(timing == 水筒冷凍直前_タイミング && !cSpell_能力仕様フラグID_exist(valiable,ID()))
		{
			valiable.doubles[変数_汎用ブール] = 0;//拒否
			Breakcrashprocess(効果時腕輪ダメージ());
			//sg_pDungeonSystem->動的識別(me());
			cSpell_能力仕様フラグID_dim(valiable,ID()) = 1;	
		}
	}
}
int cSpell_ID_15::効果(pcCharacter pchara)
{
	int eff = 0;

	sg_pDungeonSystem->フロア飛ばず追加(効果量(4));
	eff = 1;
	
	if(eff)
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("cSpell_ID_15効果メッセージ"),valiable);

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_15::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);
	効果(pchara);
	return true;//必ずtrueを返す
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//ニトリ
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void cSpell_ID_16::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);


	if(装備されている())
	{
		
		if(timing == ターン終了_タイミング)
		{
			if(!装備者()->Condition.擬態状態())
			{
				sg_pDungeonSystem->落ち物破壊要請(me());
				//sg_pDungeonSystem->擬態要請(装備者(),0,false);
			}
		}
		
		else if(timing == スペル装備直後_タイミング || timing == フロア開始_タイミング)
		{
			sg_pDungeonSystem->擬態要請(装備者(),効果量(0),true);
		}
		else if(timing == スペル装備解除直前_タイミング)
		{
			sg_pDungeonSystem->擬態要請(装備者(),0,false);
		}

		/*
		if(timing == 主人公水上歩行可能判定時_タイミング)
		{
			valiable.doubles.val(変数_汎用ブール) = 1;
			//Breakcrashprocess(効果時腕輪ダメージ());
			//sg_pDungeonSystem->動的識別(me());
		}
		else if(timing == アクション終了_タイミング)
		{
			if(装備者()->足元地形()->iswater())
			{
				sg_pDungeonSystem->強制ダメージ要請(装備者(),max(1,装備者()->MHP/効果量(1)),0,0);
				Breakcrashprocess(効果時腕輪ダメージ());
				sg_pDungeonSystem->動的識別(me());
			}
		}
		else if(timing == 飛ばし危険判定時_タイミング)
		{
			if(装備者()->足元地形()->iswater())
			{
				valiable.doubles.val(変数_汎用ブール) = 1;
			}
		}
		*/
		/*
		if(timing == ダメージ計算攻撃時_タイミング)
		{
			valiable.intsets.val(変数_属性).insert(攻撃属性::水);
			//Breakcrashprocess(効果時腕輪ダメージ());
			sg_pDungeonSystem->動的識別(me());
		}
		else if(timing == ダメージ計算防御時_タイミング)
		{
			if(valiable.intsets.val(変数_属性).count(攻撃属性::水));
			{
				valiable.doubles.val(変数_耐性ボーナス_倍率％) += 効果量(0);
				//Breakcrashprocess(効果時腕輪ダメージ());
				sg_pDungeonSystem->動的識別(me());
			}
			if(valiable.intsets.val(変数_属性).count(攻撃属性::電気))
			{
				sg_pDungeonSystem->速度減少要請(装備者(),効果量(2));
			}
		}
		*/
	}
}
int cSpell_ID_16::効果(pcCharacter pchara, vector<pcDroping> &ObjectList)
{
	int eff = 0;
/*
	vector<pcCharacter> vpchara;
	sg_pDungeonSystem->キャラクター部屋キャラリスト(pchara,vpchara);

	int size = vpchara.size();
	int i;
	for(i = 0; i<size ;i++)
	{
		sg_pDungeonSystem->擬態要請(vpchara[i],効果量(0));
		if(sg_pDungeonSystem->キャラクター敵対判定(pchara,vpchara[i])
			|| sg_pDungeonSystem->キャラクター敵対判定(sg_pDungeonSystem->pPlayerChara(),vpchara[i]))
		{
			sg_pDungeonSystem->泥酔要請(vpchara[i],効果量(0));
			
		}
		eff++;
	}

*/
	if(ObjectList.size())
	{
		if(ObjectList[0]->属性.count(落ち物属性::装備品))
		{
			sg_pDungeonSystem->アイテム強化(ObjectList[0]);
		
			sg_pDungeonSystem->動的識別(me());

			eff++;
		}
	}

	if(eff)
	{
		//map<tstring, StyleString> valiable;
		//g_Langメッセージ(_T("cSpell_ID_15効果メッセージ"),valiable);

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_16::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);
	効果(pchara, ObjectList);
	return true;//必ずtrueを返す
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//レミリア
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void cSpell_ID_17::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);


	if(装備されている())
	{
		if(timing == 攻撃直後時_タイミング)
		{
			int reco = valiable.doubles.val(変数_ダメージ)*効果量(1)/100;
			reco = max(1,reco);
			sg_pDungeonSystem->回復要請(装備者(),reco,false);
			Breakcrashprocess(効果時腕輪ダメージ());
			sg_pDungeonSystem->動的識別(me());
		}
		else if(timing == ダメージ計算防御時_タイミング)
		{
			if(valiable.intsets.val(変数_属性).count(攻撃属性::水))
			{
				sg_pDungeonSystem->脱力要請(装備者(),1,効果量(2));
				sg_pDungeonSystem->軟弱要請(装備者(),1,効果量(2));
			}
		}
	}
}
int cSpell_ID_17::効果(pcCharacter pchara)
{
	int eff = 0;


	pcLandform pland = pchara->足元地形();

	int size = 効果量(0);
	int i;
	for(i = 0; i<size ;i++)
	{

		int a;
		for(a=0;a<8;a+=2)
		{
			pcLandform pland2 = sg_pDungeonSystem->隣接地形(pland,a,i+1);
		
			if(pland2->pOnChar != NULL && !sg_pDungeonSystem->キャラクター敵対判定(pchara,pland2->pOnChar)
				&& !sg_pDungeonSystem->キャラクター敵対判定(sg_pDungeonSystem->pPlayerChara(),pland2->pOnChar))
			{//味方
				
			}
			else
			{
				GameIdiom::火柱召喚(pchara,pland2,10);
				eff++;
			}
				
		}

	}


	if(eff)
	{
		//map<tstring, StyleString> valiable;
		//g_Langメッセージ(_T("cSpell_ID_15効果メッセージ"),valiable);

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_17::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);
	効果(pchara);
	return true;//必ずtrueを返す
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//フラン
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void cSpell_ID_18::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);


	if(装備されている())
	{
		if(timing == 被攻撃直後時_タイミング && !cSpell_能力仕様フラグID_exist(valiable,ID()))
		{

			if(valiable.charas[変数_攻撃者] != NULL)
			{
				sg_pDungeonSystem->攻撃接近(攻撃作成(
					装備者(),//攻撃者
					valiable.charas[変数_攻撃者]->足元地形(),//場所
					valiable.doubles[変数_ダメージ]*効果量(1)/100.0,//攻撃力
					1,//定数ダメージ
					属性設定back(valiable.intsets[変数_属性]),
					true));//必中
			

				cSpell_能力仕様フラグID_dim(valiable,ID()) = 1;
				sg_pDungeonSystem->動的識別(me());
				Breakcrashprocess(効果時腕輪ダメージ());
			}
			
		}
		else if(timing == ダメージ計算防御時_タイミング)
		{
			if(valiable.intsets.val(変数_属性).count(攻撃属性::水))
			{
				sg_pDungeonSystem->脱力要請(装備者(),1,効果量(2));
				sg_pDungeonSystem->軟弱要請(装備者(),1,効果量(2));
			}
		}
	}
}
int cSpell_ID_18::効果(pcCharacter pchara)
{
	int eff = 0;


	if(!sg_pDungeonSystem->拠点フラグ())
	{
		eff |= sg_pDungeonSystem->大部屋();
	}
	/*
	vector<pcCharacter> vpchara = sg_pDungeonSystem->CharaList();

	int size = vpchara.size();
	int i;
	for(i = 0; i<size ;i++)
	{
		eff |= sg_pDungeonSystem->強制ダメージ要請(vpchara[i],効果量(0)*vpchara[i]->HP/100.0);
		vpchara[i]->Condition.とても強い刺激();
	}
	*/


	if(eff)
	{
		//map<tstring, StyleString> valiable;
		//g_Langメッセージ(_T("cSpell_ID_15効果メッセージ"),valiable);

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_18::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);
	効果(pchara);
	return true;//必ずtrueを返す
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//中国
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void cSpell_ID_19::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);


	if(装備されている())
	{
		if(timing == ダメージ計算防御時_タイミング)
		{
			if(valiable.intsets.val(変数_属性).count(攻撃属性::気))
			{
				valiable.doubles.val(変数_耐性ボーナス_倍率％) += 効果量(3);
				//Breakcrashprocess(効果時腕輪ダメージ());
				sg_pDungeonSystem->動的識別(me());
			}
			if(valiable.intsets.val(変数_属性).count(攻撃属性::落ち物))
			{
				valiable.doubles.val(変数_耐性ボーナス_倍率％) -= 効果量(4);
				//Breakcrashprocess(効果時腕輪ダメージ());
				sg_pDungeonSystem->動的識別(me());
			}
		}
		else if(timing == 自然満腹度減少量計算時_タイミング)
		{
			valiable.doubles.val(変数_汎用ボーナス_倍率) -= 効果量(2)/100.0;
			//Breakcrashprocess(効果時腕輪ダメージ());
			sg_pDungeonSystem->動的識別(me());
		}
	}
}
int cSpell_ID_19::効果(pcCharacter pchara)
{
	int eff = 0;


	eff |= sg_pDungeonSystem->元気要請( pchara, 効果量(1), 効果量(0));
	eff |= sg_pDungeonSystem->頑強要請( pchara, 効果量(1), 効果量(0));

	if(eff)
	{
		//map<tstring, StyleString> valiable;
		//g_Langメッセージ(_T("cSpell_ID_15効果メッセージ"),valiable);

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_19::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);
	効果(pchara);
	return true;//必ずtrueを返す
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//momijiモミジ
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void cSpell_ID_20::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);


	if(装備されている())
	{
		if(timing == キャラクター目視範囲計算時_タイミング)
		{

			valiable.doubles.val(変数_汎用ブール) = 1;
			//Breakcrashprocess(効果時腕輪ダメージ());
			sg_pDungeonSystem->動的識別(me());
			
		}
	}
}
int cSpell_ID_20::効果(pcCharacter pchara)
{
	int eff = 0;

/*
	vector<pcCharacter> vpchara;
	sg_pDungeonSystem->キャラクター部屋キャラリスト(pchara,vpchara);

	int size = vpchara.size();
	int i;
	for(i = 0; i<size ;i++)
	{

		if(sg_pDungeonSystem->キャラクター敵対判定(pchara,vpchara[i])
			|| sg_pDungeonSystem->キャラクター敵対判定(sg_pDungeonSystem->pPlayerChara(),vpchara[i]))
		{
			GameIdiom::高飛び(vpchara[i],FindUtility::部屋外優先_各部屋等確率_ランダム地形検索_配置安全(vpchara[i]->足元地形()));
			eff++;
		}
		
	}
*/

	int size = 効果量(0);//敵出現数
	
	int i,vint[3];

		
	pcLandform pland = 現在地形();

	for(i=0;i<size;i++)
	{
		sg_pDungeonSystem->敵自然湧きID(vint);

		pcCharacter pchara = sg_pDungeonSystem->キャラクター生成(vint[0],vint[1],CHARACTER_FORSE_ENEMY,pland);
		if(pchara != NULL)
		{
			pchara->Condition.とても強い刺激();
			//sg_pDungeonSystem->眠り要請(pchara,0,0);
			sg_pDungeonSystem->強制ひるみ要請(pchara);
			eff++;
		}
	}


	if(eff)
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("cSpell_ID_20効果メッセージ"),valiable);

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_20::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);
	効果(pchara);
	return true;//必ずtrueを返す
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//サクヤ
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void cSpell_ID_21::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);


	if(装備されている())
	{
		if(timing == 投擲反射回数計算時_タイミング)
		{

			valiable.doubles.val(変数_汎用実数) += 効果量(1);
			valiable.doubles.val(変数_汎用実数2) += 効果量(2);
			//Breakcrashprocess(効果時腕輪ダメージ());
			sg_pDungeonSystem->動的識別(me());
		}
		else if(timing == 投擲貫通判定時_タイミング)
		{

			valiable.doubles.val(変数_汎用実数) += 効果量(3);//射程
			//Breakcrashprocess(効果時腕輪ダメージ());
			sg_pDungeonSystem->動的識別(me());
		}
	}
}
int cSpell_ID_21::効果(pcCharacter pchara)
{
	int eff = 0;


	sg_pDungeonSystem->フロア時間停止追加(効果量(0) + 1);
	eff = 1;

	if(eff)
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("cSpell_ID_21効果メッセージ"),valiable);

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_21::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);
	効果(pchara);
	return true;//必ずtrueを返す
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//カグヤ
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void cSpell_ID_22::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);


	if(装備されている())
	{
		if(timing == アイテム劣化直前_タイミング)
		{

			valiable.doubles.val(変数_汎用ブール) = 0;
			Breakcrashprocess(効果時腕輪ダメージ());
			sg_pDungeonSystem->動的識別(me());
		}
		if(timing == 箱帯静電気直前_タイミング
			|| timing == 本濡れ直前_タイミング
			|| timing == 水筒冷凍直前_タイミング
			|| timing == 食べ物劣化直前_タイミング)
		{

			valiable.doubles.val(変数_汎用ブール) = 0;
			Breakcrashprocess(効果時腕輪ダメージ());
			sg_pDungeonSystem->動的識別(me());
		}
	}
}
int cSpell_ID_22::効果(pcCharacter pchara)
{
	int eff = 0;


	sg_pDungeonSystem->フロア時間飛ばし追加(効果量(0));
	eff = 1;

	vector<pcDroping> vpdrop = pchara->holdItem;
	int i,size = vpdrop.size();
	for(i=0;i<size;i++)
	{
		eff |= vpdrop[i]->異常状態解除_内容込み();
	}

	if(eff)
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("cSpell_ID_22効果メッセージ"),valiable);

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_22::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	SpellEffects::EffectcSpell_ID_22(pchara->placeX,pchara->placeY,効果量(0)*2);
	効果(pchara);
	return true;//必ずtrueを返す
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//ユウギ
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void cSpell_ID_23::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);


	if(装備されている())
	{
		if(timing == 投擲貫通判定時_タイミング)
		{

			valiable.doubles.val(変数_汎用ブール) = 1;
			valiable.doubles.val(変数_汎用実数) += 効果量(0);
			Breakcrashprocess(効果時腕輪ダメージ());
			sg_pDungeonSystem->動的識別(me());
		}
	}
}
int cSpell_ID_23::効果(pcCharacter pchara)
{
	int eff = 0;


	sg_pDungeonSystem->無敵要請(pchara,効果量(0));
	eff = 1;

	if(eff)
	{

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_23::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);
	効果(pchara);
	return true;//必ずtrueを返す
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//けいねケイネ
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void cSpell_ID_24::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);


	if(装備されている())
	{
		if(timing == 装備強制はずし直前_タイミング)
		{

			valiable.doubles.val(変数_汎用ブール) = 0;
			Breakcrashprocess(効果時腕輪ダメージ());
			sg_pDungeonSystem->動的識別(me());
		}
	}
}
int cSpell_ID_24::効果(pcCharacter pchara)
{
	int eff = 0;

	if(pchara->attackequipment != NULL)
	{
		if(pchara->attackequipment->quality() < 0)
		{
			pchara->attackequipment->quality() = 0;
		}
	}
	eff |= sg_pDungeonSystem->装備強制はずし(pchara->attackequipment);
	
	if(pchara->defenseequipment != NULL)
	{
		if(pchara->defenseequipment->quality() < 0)
		{
			pchara->defenseequipment->quality() = 0;
		}
	}
	eff |= sg_pDungeonSystem->装備強制はずし(pchara->defenseequipment);

	int i,size = pchara->最大装備スペル数();
	for(i=0;i<size;i++)
	{
		if(pchara->spellequipment[i] != NULL)
		{
			if(pchara->spellequipment[i]->quality() < 0)
			{
				pchara->spellequipment[i]->quality() = 0;
			}
		}
		pcSpell pSpe = boost::dynamic_pointer_cast<cSpell>(pchara->spellequipment[i]);
		if(pSpe != NULL) pSpe->破損回復(pSpe->破損している());
		eff |= sg_pDungeonSystem->装備強制はずし(pchara->spellequipment[i]);

	}

	vector<pcDroping> vpdrop = pchara->holdItem;
	size = vpdrop.size();
	for(i=0;i<size;i++)
	{
		eff |= vpdrop[i]->異常状態解除_内容込み();
	}

	if(eff)
	{

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_24::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);
	効果(pchara);
	return true;//必ずtrueを返す
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//ゆかり
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void cSpell_ID_25::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);


	if(装備されている())
	{
		if(timing == 主人公壁中歩行可能判定時_タイミング)
		{
			valiable.doubles.val(変数_汎用ブール) = 1;
			//Breakcrashprocess(効果時腕輪ダメージ());
			//sg_pDungeonSystem->動的識別(me());
		}
		else if(timing == アクション終了_タイミング)
		{
			if(装備者()->足元地形()->iswall())
			{
				sg_pDungeonSystem->強制ダメージ要請(装備者(),max(1,装備者()->MHP/効果量(0)),0,0);
				Breakcrashprocess(効果時腕輪ダメージ());
				sg_pDungeonSystem->動的識別(me());
			}
		}
		else if(timing == 飛ばし危険判定時_タイミング)
		{
			if(装備者()->足元地形()->iswall())
			{
				valiable.doubles.val(変数_汎用ブール) = 1;
			}
		}
	}
}
int cSpell_ID_25::効果(pcCharacter pchara)
{
	int eff = 0;

	map<tstring, StyleString> valiable;
	valiable[_T("Chara")] = pchara->ShortName();
	g_Langメッセージ(_T("cSpell_ID_25効果メッセージ"),valiable);


	eff |= sg_pDungeonSystem->HP設定要請(pchara, pchara->MHP - pchara->HP +1);
	eff |= sg_pDungeonSystem->満腹度設定要請(pchara, pchara->MaxStomach - pchara->Stomach +1);
	
	if(pchara->Condition.速度度数() != 0)
	{
		if(pchara->Condition.速度度数() > 0)
		{
			int i = pchara->Condition.速度度数();
			for(;i>0;i--)
			{
				sg_pDungeonSystem->速度減少要請(pchara,pchara->Condition.速度残りターン数());
				sg_pDungeonSystem->速度減少要請(pchara,pchara->Condition.速度残りターン数());
			}
			
		}
		else
		{
			int i = -pchara->Condition.速度度数();
			for(;i>0;i--)
			{
				sg_pDungeonSystem->速度増加要請(pchara,pchara->Condition.速度残りターン数());
				sg_pDungeonSystem->速度増加要請(pchara,pchara->Condition.速度残りターン数());
			}
		}
		eff |= 1;
	}

	if(pchara->Condition.力度数() != 0)
	{
		int turn = pchara->Condition.力残りターン数();
		int power = pchara->Condition.力度数();
		if(pchara->Condition.力度数() > 0)
		{
			sg_pDungeonSystem->脱力要請(pchara,power,-1,false);
			sg_pDungeonSystem->脱力要請(pchara,power,turn);
		}
		else
		{
			sg_pDungeonSystem->元気要請(pchara,-power,-1,false);
			sg_pDungeonSystem->元気要請(pchara,-power,turn);
		}
	
		eff |= 1;
	}

	if(pchara->Condition.守度数() != 0)
	{
		int turn = pchara->Condition.守残りターン数();
		int power = pchara->Condition.守度数();
		if(pchara->Condition.守度数() > 0)
		{
			sg_pDungeonSystem->軟弱要請(pchara,power,-1,false);
			sg_pDungeonSystem->軟弱要請(pchara,power,turn);
		}
		else
		{
			sg_pDungeonSystem->頑強要請(pchara,-power,-1,false);
			sg_pDungeonSystem->頑強要請(pchara,-power,turn);
		}
	
		eff |= 1;
	}

	if(eff)
	{


	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_25::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);
	効果(pchara);
	return true;//必ずtrueを返す
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//チェン
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void cSpell_ID_26::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);


	if(装備されている())
	{
		if(timing == 被攻撃直後時_タイミング)
		{
			if(!cSpell_能力仕様フラグID_exist(valiable,ID()) && (効果量(0)/100.0) > random())
			{

				pcCharacter pchara = 装備者();
				c4DVector start,end;
				//とびアニメーション
				start.set(pchara->placeX,pchara->placeY,0,0);
				end.set(pchara->placeX,pchara->placeY,10,0);
				sg_pDungeonSystem->AnimationManager().Anime_Throw(pchara,start,end,ROUTE_HIGH);
				sg_pDungeonSystem->ワープ要請(pchara,FindUtility::部屋外優先_各部屋等確率_ランダム地形検索_配置安全(pchara->足元地形()));

				cSpell_能力仕様フラグID_dim(valiable,ID()) = 1;
				Breakcrashprocess(効果時腕輪ダメージ());
				sg_pDungeonSystem->動的識別(me());
			}

		}
	}
}
int cSpell_ID_26::効果(pcCharacter pchara)
{
	int eff = 0;
	//eff |= ;

	pcLandform pland = FindUtility::部屋外優先_各部屋等確率_ランダム地形検索_配置安全(pchara->足元地形());
	

	c4DVector start,end;
	//とびアニメーション
	start.set(pchara->placeX,pchara->placeY,0,0);
	end.set(pchara->placeX,pchara->placeY,10,0);
	sg_pDungeonSystem->AnimationManager().Anime_Throw(pchara,start,end,ROUTE_HIGH);

	eff |= sg_pDungeonSystem->ワープ要請(pchara,pland);

	vector<pcCharacter> vpchara = sg_pDungeonSystem->CharaList();

	int size = vpchara.size();
	int i;
	for(i = 0; i<size ;i++)
	{
		if(vpchara[i] != pchara && vpchara[i]->Forse == CHARACTER_FORSE_FRIEND)
		{
			//とびアニメーション
			start.set(vpchara[i]->placeX,vpchara[i]->placeY,0,0);
			end.set(vpchara[i]->placeX,vpchara[i]->placeY,10,0);
			sg_pDungeonSystem->AnimationManager().Anime_Throw(vpchara[i],start,end,ROUTE_HIGH);

			eff |= sg_pDungeonSystem->ワープ要請(vpchara[i],pland);
		}
	}


	if(eff)
	{

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_26::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);
	効果(pchara);
	return true;//必ずtrueを返す
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//スイカ
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void cSpell_ID_27::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);


	if(装備されている())
	{
		if(timing == 飲み効果直前_タイミング)
		{
			if(valiable.drops[変数_対象落ち物]->属性.count(落ち物属性::酒))
			{
				int i,size = 効果量(0);
				for(i=0;i<size;i++)
				{
					sg_pDungeonSystem->アイテム強化(valiable.drops[変数_対象落ち物]);
				}
				Breakcrashprocess(効果時腕輪ダメージ());
				sg_pDungeonSystem->動的識別(me());
			}
		}

	}
}
int cSpell_ID_27::効果(pcCharacter pchara)
{
	int eff = 0;
	//eff |= ;

	//未実装
	if(pchara->足元地形()->RoomIndex == -1)
	{//通路
		GameIdiom::高飛び(pchara,pchara->足元地形());
	}

	if(pchara->足元地形()->RoomIndex != -1)
	{
		sg_pDungeonSystem->宴会生成(pchara->足元地形()->RoomIndex);
		eff = 1;
	}

	if(eff)
	{

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_27::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);
	効果(pchara);
	return true;//必ずtrueを返す
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//コイシ
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void cSpell_ID_28::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);


	if(装備されている())
	{
		/*
		if(timing == スペル装備直後_タイミング)
		{
			tempmem() = 装備者()->Forse;
			if(tempmem() == CHARACTER_FORSE_FRIEND)
			{
				装備者()->Forse = CHARACTER_FORSE_ENEMY;
			}
			else
			{
				装備者()->Forse = CHARACTER_FORSE_FRIEND;
			}
			sg_pDungeonSystem->狂乱要請(装備者(),GAME_TURN_GAMEOVER,true);
		}
		else if(timing == スペル装備解除直前_タイミング)
		{
			装備者()->Forse = tempmem();
			sg_pDungeonSystem->狂乱要請(装備者(),-1,true);
		}
		else if(timing == ターン終了_タイミング)
		{
			Breakcrashprocess(効果時腕輪ダメージ());
		}
		*/
		if(timing == スペル装備直後_タイミング)
		{
			sg_pDungeonSystem->精神異常治療要請(装備者(),true);
		}
		else if((
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
 			 || timing == 狂乱追加直前_タイミング
 			 || timing == 無意識追加直前_タイミング
			)&& !cSpell_能力仕様フラグID_exist(valiable,ID()))
		{
			valiable.doubles[変数_汎用ブール] = 0;//拒否
			Breakcrashprocess(効果時腕輪ダメージ());
			if(timing != びっくり追加直前_タイミング)
			{
				sg_pDungeonSystem->動的識別(me());
			}
			cSpell_能力仕様フラグID_dim(valiable,ID()) = 1;
		}
	}
}
int cSpell_ID_28::効果(pcCharacter pchara)
{
	int eff = 0;
	//eff |= ;

	//未実装

	eff = sg_pDungeonSystem->狂乱要請(pchara,効果量(0));
	

	if(eff)
	{

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_28::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);
	効果(pchara);
	return true;//必ずtrueを返す
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//結句
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cSpell_ID_29::効果(pcCharacter pchara)
{
	int eff = 0;
	//eff |= ;

	//未実装

	if(pchara == sg_pDungeonSystem->pPlayerChara()
		&& !sg_pDungeonSystem->拠点フラグ())
	{
		sg_pDungeonSystem->ダンジョン引き上げ要請();
		eff = true;
	}
	

	if(eff)
	{

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_29::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);
	効果(pchara);
	return true;//必ずtrueを返す
}

int cSpell_ID_29::GetmenuCaption(vector<tstring> &CaptionList)
{
	cSpell::GetmenuCaption(CaptionList);
	
	vector<tstring>::iterator itr = find(CaptionList.begin(), CaptionList.end(), _T("装備"));
	if(itr != CaptionList.end())
	{
		CaptionList.erase(itr);
	}
	return true;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//うつほ
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cSpell_ID_30::効果(pcCharacter pchara)
{
	int eff = 0;
	//eff |= ;

	EffectFunctions::爆発エフェクト_中(me()->現在地形()->placeX,me()->現在地形()->placeY,20);

	GameIdiom::爆発(me()->現在地形(), NULLCHARA, 効果量(0), 割合ダメージフラグ, TRUE, TRUE, TRUE, TRUE);
	eff = 1;

	if(eff)
	{

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_30::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);
	効果(pchara);
	return true;//必ずtrueを返す
}

void cSpell_ID_30::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);


	if(装備されている())
	{
		if(timing == ダメージ計算防御時_タイミング)
		{
			if(valiable.intsets.val(変数_属性).count(攻撃属性::爆))
			{
				valiable.doubles.val(変数_耐性ボーナス_倍率％) += 効果量(1);
				//Breakcrashprocess(効果時腕輪ダメージ());
				sg_pDungeonSystem->動的識別(me());
			}
		}
		if(timing == ダメージ計算攻撃時_タイミング)
		{
			valiable.intsets.val(変数_属性).insert(攻撃属性::爆);
			//Breakcrashprocess(効果時腕輪ダメージ());
			sg_pDungeonSystem->動的識別(me());
		}
	}
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//パルスィ
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cSpell_ID_31::効果(pcCharacter pchara)
{
	int eff = 0;
	vector<pcCharacter> findCharacters;
	findCharacters = FindUtility::部屋内と隣接敵リスト(pchara);
	int i,size = findCharacters.size();
	for(i=0;i<size;i++)
	{
		eff |= sg_pDungeonSystem->嫉妬要請(findCharacters[i], 効果量(0));
	}

	if(eff)
	{

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_31::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);
	効果(pchara);
	return true;//必ずtrueを返す
}

void cSpell_ID_31::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);


	if(装備されている())
	{
		if(timing == 被攻撃直後時_タイミング)
		{
			if(効果量(1) > random()*100)
			{
				sg_pDungeonSystem->元気要請(装備者(),効果量(3),効果量(2));
				sg_pDungeonSystem->動的識別(me());
			}
		}
	}
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//サナエ
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cSpell_ID_32::効果(pcCharacter pchara)
{
	int eff = 0;
	bool didTryFlag = false;

	if(!didTryFlag && pchara->HP <= pchara->MHP*0.5)
	{
		eff |= sg_pDungeonSystem->回復要請(pchara, pchara->MHP);
		didTryFlag = true;
	}
	
	if(!didTryFlag && pchara->Stomach <= 0)
	{
		pcDroping pdrop = sg_pDungeonSystem->落ち物生成_設置なし(4001, NULL);
		sg_pDungeonSystem->落ち物落下(pdrop, pchara->足元地形());
		didTryFlag = true;
		
		map<tstring, StyleString> valiable;
		valiable[_T("Drop")] = pdrop->FullName();
		g_Langメッセージ(_T("cSpell_ID_32効果メッセージ"),valiable);
		eff = 1;
	}

	if(!didTryFlag)
	{
		int aspect;
		int enemycount = 0;
		for(aspect = 0; aspect<8; aspect++)
		{
			pcLandform pland = sg_pDungeonSystem->隣接地形(pchara->足元地形(), aspect, 1);
			pcCharacter neirChara = pland->pOnChar;
			if(neirChara != NULL)
			{
				if(sg_pDungeonSystem->キャラクター敵対判定(neirChara,pchara))
				{
					enemycount++;
				}
			}
		}
		if(enemycount >= 2)
		{
			for(aspect = 0; aspect<8; aspect++)
			{
				pcLandform pland = sg_pDungeonSystem->隣接地形(pchara->足元地形(), aspect, 1);
				pcCharacter neirChara = pland->pOnChar;
				if(neirChara != NULL)
				{
					eff |= sg_pDungeonSystem->金縛り要請(neirChara,効果量(1));
				}
			}
			didTryFlag = true;
		}
	}

	if(!didTryFlag)
	{
		if(pchara->attackequipment)
		{
			if(pchara->attackequipment->state() == STATE_CURSE)
			{
				eff |= sg_pDungeonSystem->アイテム祝福(pchara->attackequipment);
				didTryFlag = true;
			}
		}
		if(pchara->defenseequipment)
		{
			if(pchara->defenseequipment->state() == STATE_CURSE)
			{
				eff |= sg_pDungeonSystem->アイテム祝福(pchara->defenseequipment);
				didTryFlag = true;
			}
		}

		int spellIndex = 0;
		for(spellIndex = 0; spellIndex < pchara->最大装備スペル数(); spellIndex++)
		{
			if(pchara->spellequipment[spellIndex])
			{
				if(pchara->spellequipment[spellIndex]->state() == STATE_CURSE)
				{
					eff |= sg_pDungeonSystem->アイテム祝福(pchara->spellequipment[spellIndex]);
					didTryFlag = true;
				}
			}
		}
	}

	if(!didTryFlag)
	{
		if(sg_pDungeonSystem->精神異常状態(pchara)||
			sg_pDungeonSystem->身体異常状態(pchara)||
			sg_pDungeonSystem->呪術異常状態(pchara)||
			sg_pDungeonSystem->速度異常状態(pchara) )
		{
			eff |= sg_pDungeonSystem->精神異常治療要請(pchara);
			eff |= sg_pDungeonSystem->身体異常治療要請(pchara);
			eff |= sg_pDungeonSystem->呪術異常治療要請(pchara);
			eff |= sg_pDungeonSystem->速度異常状態(pchara);
			didTryFlag = true;
		}

	}

	if(!didTryFlag)
	{
		vector<pcDroping> vpdrop;
		if(pchara->attackequipment) vpdrop.push_back(pchara->attackequipment);
		if(pchara->defenseequipment) vpdrop.push_back(pchara->defenseequipment);
		int spellIndex = 0;
		for(spellIndex = 0; spellIndex < pchara->最大装備スペル数(); spellIndex++)
		{
			if(pchara->spellequipment[spellIndex])
			{
				vpdrop.push_back(pchara->spellequipment[spellIndex]);
			}
		}

		if(!vpdrop.empty())
		{
			eff |= sg_pDungeonSystem->アイテム呪い(vpdrop[vpdrop.size()*random()]);
		}

	}
	if(eff)
	{

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_32::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);
	効果(pchara);
	return true;//必ずtrueを返す
}

void cSpell_ID_32::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);


	if(装備されている())
	{
		if(timing == 経験値獲得時_タイミング)
		{
			valiable.doubles.val(変数_汎用ボーナス_倍率) += 効果量(0)/100.0;
		}
	}
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//イク
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cSpell_ID_34::効果(pcCharacter pchara)
{
	int eff = 0;
	vector<pcCharacter> findCharacters;
	findCharacters = FindUtility::部屋内と隣接敵リスト(pchara);
	int i,size = findCharacters.size();
	for(i=0;i<size;i++)
	{
		EffectFunctions::落雷エフェクト(findCharacters[i]->placeX, findCharacters[i]->placeY);
	
		multiset<攻撃属性::攻撃属性> attackAttri;
		attackAttri.insert(攻撃属性::雷);
		attackAttri.insert(攻撃属性::雷);//２倍
		eff |= sg_pDungeonSystem->攻撃接近(攻撃作成(
					pchara,//攻撃者
					findCharacters[i]->足元地形(),//場所
					効果量(0),//攻撃力
					1,//定数ダメージ
					attackAttri,
					true));//必中
	}

	if(eff)
	{

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_34::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);
	効果(pchara);
	return true;//必ずtrueを返す
}

void cSpell_ID_34::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);


	if(装備されている())
	{
		if(timing == ダメージ計算防御時_タイミング)
		{
			if(valiable.intsets.val(変数_属性).count(攻撃属性::雷))
			{
				valiable.doubles.val(変数_耐性ボーナス_倍率％) += 効果量(1);
				//Breakcrashprocess(効果時腕輪ダメージ());
				sg_pDungeonSystem->動的識別(me());
			}
		}
		if(timing == ダメージ計算攻撃時_タイミング)
		{
			valiable.intsets.val(変数_属性).insert(攻撃属性::雷);
			//Breakcrashprocess(効果時腕輪ダメージ());
			sg_pDungeonSystem->動的識別(me());
		}
		else if(timing == 攻撃直後時_タイミング)
		{
			if((効果量(1)/100.0) > random())
			{
				if(!cSpell_能力仕様フラグID_exist(valiable,ID()))
				{//初
					sg_pDungeonSystem->金縛り要請(valiable.charas.val(変数_防御者),効果量(2));

					cSpell_能力仕様フラグID_dim(valiable,ID()) = 1;
				}
			}
		}
		else if(timing == 箱帯静電気直前_タイミング && !cSpell_能力仕様フラグID_exist(valiable,ID()))
		{
			valiable.doubles[変数_汎用ブール] = 0;//拒否
			Breakcrashprocess(効果時腕輪ダメージ());
			//sg_pDungeonSystem->動的識別(me());
			cSpell_能力仕様フラグID_dim(valiable,ID()) = 1;	
		}
		else if(timing == 金縛り追加直前_タイミング && !cSpell_能力仕様フラグID_exist(valiable,ID()))
		{
			valiable.doubles[変数_汎用ブール] = 0;//拒否
			Breakcrashprocess(効果時腕輪ダメージ());
			//sg_pDungeonSystem->動的識別(me());
			cSpell_能力仕様フラグID_dim(valiable,ID()) = 1;	
		}
	}
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//コガサ
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cSpell_ID_36::効果(pcCharacter pchara)
{
	int eff = 0;
	//eff |= ;
	vector<pcCharacter> findCharacters;
	findCharacters = sg_pDungeonSystem->CharaList();
	int i,size = findCharacters.size();
	for(i=0;i<size;i++)
	{
		multiset<攻撃属性::攻撃属性> attackAttri;
		attackAttri.insert(攻撃属性::水);
		attackAttri.insert(攻撃属性::水);
		attackAttri.insert(攻撃属性::水);//３倍
		eff |= sg_pDungeonSystem->攻撃接近(攻撃作成(
				NULLCHARA,//攻撃者
				findCharacters[i]->足元地形(),//場所
				効果量(0),//攻撃力
				0,//定数ダメージ
				attackAttri,
				true));//必中
	}

	if(eff)
	{

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_36::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);
	効果(pchara);
	return true;//必ずtrueを返す
}

void cSpell_ID_36::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);


	if(装備されている())
	{
		if(timing == 攻撃直後時_タイミング)
		{
			if((効果量(1)/100.0) > random())
			{
				if(!cSpell_能力仕様フラグID_exist(valiable,ID()))
				{//初
					sg_pDungeonSystem->びっくり要請(valiable.charas.val(変数_防御者),効果量(2));

					cSpell_能力仕様フラグID_dim(valiable,ID()) = 1;
				}
			}
		}
		else if(timing == びっくり追加直前_タイミング && !cSpell_能力仕様フラグID_exist(valiable,ID()))
		{
			valiable.doubles[変数_汎用ブール] = 0;//拒否
			Breakcrashprocess(効果時腕輪ダメージ());
			//sg_pDungeonSystem->動的識別(me());
			cSpell_能力仕様フラグID_dim(valiable,ID()) = 1;	
		}
	}
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//トラマル
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cSpell_ID_38::効果(pcDroping pdrop )
{
	int eff = 0;
	//eff |= ;

	map<tstring, StyleString> valiable;
	valiable[_T("Drop")] = pdrop->FullName();
	if(pdrop != me()) {
		eff = sg_pDungeonSystem->アイテム倉庫行き要請(pdrop);
	}

	if(eff)
	{
		g_Langメッセージ(_T("cSpell_ID_38効果メッセージ"),valiable);
	}
	else
	{
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_38::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);
	効果(ObjectList[0]);
	return true;//必ずtrueを返す
}

void cSpell_ID_38::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);


	if(装備されている())
	{
		if(timing == ターン終了_タイミング)
		{
			if(効果量(0) > random()*10000.0)
			{
				vector<pcDroping> vpdrop = FindUtility::装備品以外の表面手持ちアイテム店売り除く(装備者());
				if(!vpdrop.empty()){
					sg_pDungeonSystem->落ち物落し物(vpdrop[vpdrop.size()*random()], 装備者()->足元地形());
				}
			}
		}
		else if(timing == スペル装備直後_タイミング)
		{
			if(装備者()->ID() == 2020)//店主
			{
				int i, size = 効果量(1)*sg_pDungeonSystem->店アイテム数(装備者()->CLASS)/100;
				for(i=0;i<size;i++)
				{
					int vint[3];
					sg_pDungeonSystem->アイテム店湧きID(vint);
					
					pcDroping pdrop = sg_pDungeonSystem->落ち物生成_設置なし(vint[0],NULL,true);
					if(sg_pDungeonSystem->遠隔拾得要請_持ち物余白無考慮(装備者(),pdrop))
					{
						sg_pDungeonSystem->店アイテム追加(pdrop,装備者());
					}
				}
				sg_pDungeonSystem->落ち物破壊要請(me());
			}
		}
	}
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//ヌエ
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cSpell_ID_40::効果(pcDroping pdrop)
{
	int eff = 0;
	
	eff |= sg_pDungeonSystem->カテゴリ未識別化(pdrop);

	if(eff)
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("cSpell_ID_40効果メッセージ"),valiable);
	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_40::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);
	効果(ObjectList[0]);
	return true;//必ずtrueを返す
}

void cSpell_ID_40::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);


	if(装備されている())
	{
		if(timing == 攻撃直後時_タイミング)
		{
			if(効果量(0) > random()*100)
			{
				pcCharacter enemy = valiable.charas.val(変数_防御者);
				int a = random()*8;
				//眠り(5)、びっくり(1)、金縛り(50)、封印(20)、空振り(20)、貧乏(20)、臆病(20)、狂乱(20)
				if(a == 0)
				{
					sg_pDungeonSystem->眠り要請(enemy, 5);
					sg_pDungeonSystem->動的識別(me());
				}
				else if(a == 1)
				{
					sg_pDungeonSystem->びっくり要請(enemy, 1);
				}
				else if(a == 2)
				{
					sg_pDungeonSystem->金縛り要請(enemy, 50);
				}
				else if(a == 3)
				{
					sg_pDungeonSystem->封印要請(enemy, 20);
					sg_pDungeonSystem->動的識別(me());
				}
				else if(a == 4)
				{
					sg_pDungeonSystem->空振り要請(enemy, 20);
				}
				else if(a == 5)
				{
					sg_pDungeonSystem->貧乏要請(enemy, 20);
					sg_pDungeonSystem->動的識別(me());
				}
				else if(a == 6)
				{
					sg_pDungeonSystem->臆病要請(enemy, 20);
					sg_pDungeonSystem->動的識別(me());
				}
				else if(a == 7)
				{
					sg_pDungeonSystem->狂乱要請(enemy, 20);
					sg_pDungeonSystem->動的識別(me());
				}
				//Breakcrashprocess(効果時腕輪ダメージ());
				sg_pDungeonSystem->動的識別(me());
			}
		}
	}
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//ムラサ
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cSpell_ID_43::効果(pcCharacter pchara)
{
	int eff = 0;
	
	int x,y;
	for(x = 0;x < MAPWIDTH; x++)
	{
		for(y = 0; y < MAPHEIGHT; y++)
		{
			pcLandform pland = sg_pDungeonSystem->Map().Land(x,y);
			eff |= sg_pDungeonSystem->水掘り(pland,false);
		}
	}


	if(eff)
	{
		sg_pDungeonSystem->地形整理();
	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_43::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);
	効果(pchara);
	return true;//必ずtrueを返す
}

void cSpell_ID_43::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);


	if(装備されている())
	{
		if(timing == ダメージ計算攻撃時_タイミング)
		{
			valiable.intsets.val(変数_属性).insert(攻撃属性::水);
			//Breakcrashprocess(効果時腕輪ダメージ());
			sg_pDungeonSystem->動的識別(me());
		}
		else if(timing == ダメージ計算防御時_タイミング)
		{
			if(valiable.intsets.val(変数_属性).count(攻撃属性::水))
			{
				valiable.doubles.val(変数_耐性ボーナス_倍率％) += 効果量(0);
				//Breakcrashprocess(効果時腕輪ダメージ());
				sg_pDungeonSystem->動的識別(me());
			}
		}
		else if(
			(timing == 吹き飛ばし判定時_タイミング ||
				timing == 引き寄せ判定時_タイミング ||
				timing == 投げ飛ばし判定時_タイミング)
			&& !cSpell_能力仕様フラグID_exist(valiable,ID()))
		{
			valiable.doubles[変数_汎用ブール] = 0;//拒否
			Breakcrashprocess(効果時腕輪ダメージ());
			//sg_pDungeonSystem->動的識別(me());
			cSpell_能力仕様フラグID_dim(valiable,ID()) = 1;	
		}
		else if(timing == 本濡れ直前_タイミング && !cSpell_能力仕様フラグID_exist(valiable,ID()))
		{
			valiable.doubles[変数_汎用ブール] = 0;//拒否
			Breakcrashprocess(効果時腕輪ダメージ());
			//sg_pDungeonSystem->動的識別(me());
			cSpell_能力仕様フラグID_dim(valiable,ID()) = 1;	
		}
	}
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//ユウカ
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cSpell_ID_44::効果(pcCharacter pchara)
{
	int eff = 0;
	
	eff |= sg_pDungeonSystem->狂乱要請(pchara,効果量(0));
	eff |= sg_pDungeonSystem->元気要請(pchara,効果量(1),効果量(3));
	eff |= sg_pDungeonSystem->軟弱要請(pchara,効果量(2),効果量(3));

	if(eff)
	{

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_44::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);
	効果(pchara);
	return true;//必ずtrueを返す
}

void cSpell_ID_44::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);


	if(装備されている())
	{
		if(timing == ダメージ計算攻撃時_タイミング)
		{
			if(効果量(4) > random()*100)
			{
				valiable.doubles.val(変数_耐性ボーナス_倍率％) -= 100;
			
				sg_pDungeonSystem->動的識別(me());
			}
		}
	}
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//レティ
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void cSpell_ID_46::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);


	if(装備されている())
	{
		if(timing == ダメージ計算攻撃時_タイミング)
		{
			valiable.intsets.val(変数_属性).insert(攻撃属性::冷気);
			//Breakcrashprocess(効果時腕輪ダメージ());
			sg_pDungeonSystem->動的識別(me());
		}
		else if(timing == ダメージ計算防御時_タイミング)
		{
			if(valiable.intsets.val(変数_属性).count(攻撃属性::冷気))
			{
				valiable.doubles.val(変数_耐性ボーナス_倍率％) += 効果量(2);
				//Breakcrashprocess(効果時腕輪ダメージ());
				sg_pDungeonSystem->動的識別(me());
			}
			if(valiable.intsets.val(変数_属性).count(攻撃属性::火))
			{
				sg_pDungeonSystem->脱力要請(装備者(),1,効果量(3));
				sg_pDungeonSystem->軟弱要請(装備者(),1,効果量(3));
			}
		}
		else if(timing == 氷付け追加直前_タイミング && !cSpell_能力仕様フラグID_exist(valiable,ID()))
		{
			valiable.doubles[変数_汎用ブール] = 0;//拒否
			Breakcrashprocess(効果時腕輪ダメージ());
			//sg_pDungeonSystem->動的識別(me());
			cSpell_能力仕様フラグID_dim(valiable,ID()) = 1;	
		}
		else if(timing == 水筒冷凍直前_タイミング && !cSpell_能力仕様フラグID_exist(valiable,ID()))
		{
			valiable.doubles[変数_汎用ブール] = 0;//拒否
			Breakcrashprocess(効果時腕輪ダメージ());
			//sg_pDungeonSystem->動的識別(me());
			cSpell_能力仕様フラグID_dim(valiable,ID()) = 1;	
		}
		else if(timing == 食べ物劣化直前_タイミング && !cSpell_能力仕様フラグID_exist(valiable,ID()))
		{
			valiable.doubles[変数_汎用ブール] = 0;//拒否
			Breakcrashprocess(効果時腕輪ダメージ());
			sg_pDungeonSystem->動的識別(me());
			cSpell_能力仕様フラグID_dim(valiable,ID()) = 1;	
		}
	}
}
int cSpell_ID_46::効果(pcCharacter pchara)
{
	int eff = 0;

	vector<pcCharacter> findCharacters;
	findCharacters = FindUtility::部屋内と隣接敵リスト(pchara);
	int i,size = findCharacters.size();
	for(i=0;i<size;i++)
	{
		eff |= sg_pDungeonSystem->氷付け要請(findCharacters[i],効果量(0));
		multiset<攻撃属性::攻撃属性> attackAttri;
		attackAttri.insert(攻撃属性::冷気);
		attackAttri.insert(攻撃属性::冷気);
		attackAttri.insert(攻撃属性::冷気);
		eff |= sg_pDungeonSystem->攻撃接近(攻撃作成(
					pchara,//攻撃者
					findCharacters[i]->足元地形(),//場所
					効果量(1),//攻撃力
					1,//定数ダメージ
					attackAttri,
					true));//必中
	}
	
	if(eff)
	{
		//map<tstring, StyleString> valiable;
		//g_Langメッセージ(_T("cSpell_ID_15効果メッセージ"),valiable);

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_46::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);
	効果(pchara);
	return true;//必ずtrueを返す
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//リリー
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cSpell_ID_48::効果(pcCharacter pchara)
{
	int eff = 0;
	//eff |= ;
	vector<pcCharacter> findCharacters;
	findCharacters = FindUtility::部屋内と隣接敵リスト(pchara);
	int i,size = findCharacters.size();
	for(i=0;i<size;i++)
	{
		eff |= sg_pDungeonSystem->バクスイ要請(findCharacters[i],効果量(0));
	}

	if(eff)
	{

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_48::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);
	効果(pchara);
	return true;//必ずtrueを返す
}

void cSpell_ID_48::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);


	if(装備されている())
	{
		if(timing == ターン終了_タイミング)
		{
			pcCharacter equipmentChara = 装備者();
			vector<pcCharacter> vpchara = FindUtility::フロア敵索敵_敵リスト(equipmentChara, MAPWIDTH);

			if(!vpchara.empty() && 効果量(1)/100.0 > random())
			{
				pcCharacter enemyChara = FindUtility::敵リストから優先敵索敵(vpchara, equipmentChara);

				if(enemyChara == NULL)
				{
					return;
				}
				EffectFunctions::煙エフェクト1(equipmentChara->placeX, equipmentChara->placeY);
				sg_pDungeonSystem->ワープ要請(equipmentChara, sg_pDungeonSystem->キャラ視点方角地形(enemyChara,random()*8,1));
				GameIdiom::キャラの方を向く(equipmentChara,enemyChara);
				EffectFunctions::煙エフェクト1(equipmentChara->placeX, equipmentChara->placeY);
				map<tstring, StyleString> val;
				val[_T("Chara")] = equipmentChara->ShortName();
				g_Langメッセージ(_T("cMob_ID_69_特殊能力メッセージ"),val);
				sg_pDungeonSystem->動的識別(me());
			}

		}
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//エイキ
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cSpell_ID_49::効果(pcCharacter pchara)
{
	int eff = 0;
	//eff |= ;
	vector<pcCharacter> findCharacters;
	findCharacters = FindUtility::部屋内と隣接敵リスト(pchara);
	int i,size = findCharacters.size();
	for(i=0;i<size;i++)
	{
		if(効果量(0) > random()*100)
		{
			eff |= sg_pDungeonSystem->即死要請(findCharacters[i]);
			EffectFunctions::スペルエフェクト(findCharacters[i]->placeX,findCharacters[i]->placeY,ID(),EffectFunctions::SpellEffectPart_class::SpellEffectPart_Effected);
		}
		else {
			EffectFunctions::スペルエフェクト(findCharacters[i]->placeX,findCharacters[i]->placeY,ID(),EffectFunctions::SpellEffectPart_class::SpellEffectPart_NotEffected);
		}
	}

	if(eff)
	{

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_49::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);
	効果(pchara);
	return true;//必ずtrueを返す
}

void cSpell_ID_49::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);


	if(装備されている())
	{
		if((
			timing == びっくり追加直前_タイミング
			)&& !cSpell_能力仕様フラグID_exist(valiable,ID()))
		{
			valiable.doubles[変数_汎用ブール] = 0;//拒否
			Breakcrashprocess(効果時腕輪ダメージ());
			cSpell_能力仕様フラグID_dim(valiable,ID()) = 1;
		}
		else if(timing == ダメージ計算攻撃時_タイミング)
		{
			pcCharacter defender = valiable.charas.val(変数_防御者);
			if(defender->Condition.眠りで行動不能である())
			{
				valiable.doubles.val(変数_耐性ボーナス_倍率％) -= 効果量(1);
			}
			//Breakcrashprocess(効果時腕輪ダメージ());
			sg_pDungeonSystem->動的識別(me());
		}
	}
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//ミノリコ
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cSpell_ID_51::効果(pcCharacter pchara)
{
	int eff = 0;
	//eff |= ;

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
		pcDroping beforeDrop = vpland[i]->pOnDrop;
		if(beforeDrop && beforeDrop != me())
		{
			if(beforeDrop->属性.count(落ち物属性::アイテム) && !beforeDrop->大切())
			{
				pcDroping pdrop2 = sg_pDungeonSystem->落ち物生成_設置なし(4013);
				eff |= sg_pDungeonSystem->落ち物変化(beforeDrop,pdrop2);
				EffectFunctions::煙エフェクト1(pdrop2->placeX, pdrop2->placeY);
			}
		}
	}

	if(eff)
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("cSpell_ID_51効果メッセージ"),valiable);
	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_51::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);
	効果(pchara);
	return true;//必ずtrueを返す
}

void cSpell_ID_51::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);


	if(装備されている())
	{
		if((
			timing == イモライズ直前_タイミング
			)&& !cSpell_能力仕様フラグID_exist(valiable,ID()))
		{
			valiable.doubles[変数_汎用ブール] = 0;//拒否
			Breakcrashprocess(効果時腕輪ダメージ());
			cSpell_能力仕様フラグID_dim(valiable,ID()) = 1;
		}
		else if(timing == 敵倒し直後時_タイミング)
		{
			pcCharacter defender = valiable.charas.val(変数_防御者);
			if(効果量(0) > random()*100)
			{
				sg_pDungeonSystem->落ち物生成(4012,NULL,defender->足元地形(),false);
				Breakcrashprocess(効果時腕輪ダメージ());
				sg_pDungeonSystem->動的識別(me());
				EffectFunctions::煙エフェクト1(defender->placeX, defender->placeY);
			}

		}
	}
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//メディスン
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cSpell_ID_53::効果(pcCharacter pchara)
{
	int eff = 0;
	//eff |= ;
	vector<pcCharacter> findCharacters;
	findCharacters = FindUtility::部屋内と隣接敵リスト(pchara);
	int i,size = findCharacters.size();
	for(i=0;i<size;i++)
	{
		multiset<攻撃属性::攻撃属性> attackAttri;
		attackAttri.insert(攻撃属性::毒);
		eff |= sg_pDungeonSystem->攻撃接近(攻撃作成(
					pchara,//攻撃者
					findCharacters[i]->足元地形(),//場所
					効果量(2),//攻撃力
					1,//定数ダメージ
					attackAttri,
					true));//必中
		if(!findCharacters[i]->死亡())
		{
			eff |= sg_pDungeonSystem->脱力要請(findCharacters[i], 効果量(1), 効果量(0));
		}
	}

	if(eff)
	{

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_53::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);
	効果(pchara);
	return true;//必ずtrueを返す
}

void cSpell_ID_53::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);


	if(装備されている())
	{
		if(timing == 脱力追加直前_タイミング)
		{
			valiable.doubles[変数_汎用ブール] = 0;//拒否
			Breakcrashprocess(効果時腕輪ダメージ());
			sg_pDungeonSystem->動的識別(me());
			sg_pDungeonSystem->元気要請(装備者(),valiable.doubles[変数_強度],valiable.doubles[変数_ターン]);
			sg_pDungeonSystem->回復要請(装備者(),valiable.doubles[変数_強度]*valiable.doubles[変数_ターン]*効果量(3)/100.0);
		}
		if(timing == 軟弱追加直前_タイミング)
		{
			valiable.doubles[変数_汎用ブール] = 0;//拒否
			Breakcrashprocess(効果時腕輪ダメージ());
			sg_pDungeonSystem->動的識別(me());
			sg_pDungeonSystem->頑強要請(装備者(),valiable.doubles[変数_強度],valiable.doubles[変数_ターン]);
			sg_pDungeonSystem->回復要請(装備者(),valiable.doubles[変数_強度]*valiable.doubles[変数_ターン]*効果量(3)/100.0);
		}
	}
}
void cSpell_ID_54::LastSpellAfterProcess(pcCharacter equipedChara)
{
	sg_pDungeonSystem->回復要請(equipedChara, equipedChara->MHP*効果量(2)/100.0);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//モコウ
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cSpell_ID_54::効果(pcCharacter pchara)
{
	int eff = 0;
	//eff |= ;
	vector<pcCharacter> findCharacters;
	findCharacters = FindUtility::部屋内と隣接敵リスト(pchara);
	int i,size = findCharacters.size();
	for(i=0;i<size;i++)
	{
		multiset<攻撃属性::攻撃属性> attackAttri;
		attackAttri.insert(攻撃属性::火);
		attackAttri.insert(攻撃属性::火);//２倍
		eff |= sg_pDungeonSystem->攻撃接近(攻撃作成(
					pchara,//攻撃者
					findCharacters[i]->足元地形(),//場所
					効果量(0),//攻撃力
					1,//定数ダメージ
					attackAttri,
					true));//必中
	}

	if(eff)
	{

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_54::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);
	効果(pchara);
	return true;//必ずtrueを返す
}

void cSpell_ID_54::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);


	if(装備されている())
	{
		if(timing == ダメージ計算防御時_タイミング)
		{
			if(valiable.intsets.val(変数_属性).count(攻撃属性::火))
			{
				valiable.doubles.val(変数_耐性ボーナス_倍率％) += 効果量(1);
				//Breakcrashprocess(効果時腕輪ダメージ());
				sg_pDungeonSystem->動的識別(me());
			}
		}
		else if(timing == ダメージ計算攻撃時_タイミング)
		{
			valiable.intsets.val(変数_属性).insert(攻撃属性::火);
			//Breakcrashprocess(効果時腕輪ダメージ());
			sg_pDungeonSystem->動的識別(me());
		}
		else if(timing == 食べ物加熱直前_タイミング && !cSpell_能力仕様フラグID_exist(valiable,ID()))
		{
			valiable.doubles[変数_汎用ブール] = 0;//拒否
			Breakcrashprocess(効果時腕輪ダメージ());
			sg_pDungeonSystem->動的識別(me());
			cSpell_能力仕様フラグID_dim(valiable,ID()) = 1;	
		}
	}
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//ヒジリ
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cSpell_ID_55::効果(pcCharacter pchara)
{
	int eff = 0;
	//eff |= ;
	eff |= sg_pDungeonSystem->頑強要請(pchara, 効果量(1), 効果量(0));
	eff |= sg_pDungeonSystem->速度増加要請(pchara, 効果量(0));

	if(eff)
	{

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_55::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	EffectFunctions::地震(60, 18);
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);
	効果(pchara);
	return true;//必ずtrueを返す
}

void cSpell_ID_55::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);
	

	if(装備されている())
	{
		if(timing == スペル装備直後_タイミング)
		{
			int UpMHP = 装備者()->MHP*効果量(3)/100.0;
			int before = 装備者()->MHP;
			sg_pDungeonSystem->最大HP拡張要請(装備者(),UpMHP,false);
			int after = 装備者()->MHP;
			up_maxhp() = after - before;
			sg_pDungeonSystem->動的識別(me());
		}
		else if(timing == スペル装備解除直前_タイミング)
		{
			sg_pDungeonSystem->最大HP縮小要請(装備者(),up_maxhp(),false);
			up_maxhp() = 0;
		}
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//テンシ
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cSpell_ID_56::効果(pcCharacter pchara)
{
	int eff = 0;
	//eff |= ;
	vector<pcCharacter> findCharacters;
	findCharacters = sg_pDungeonSystem->CharaList();
	int i,size = findCharacters.size();
	for(i=0;i<size;i++)
	{
		if(findCharacters[i] != pchara)
		{
			multiset<攻撃属性::攻撃属性> temp;
			eff |= sg_pDungeonSystem->攻撃接近(攻撃作成(
					pchara,//攻撃者
					findCharacters[i]->足元地形(),//場所
					効果量(0),//攻撃力
					1,//定数ダメージ
					temp,
					true));//必中
		}
	}

	if(eff)
	{

	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	return eff;
}
int cSpell_ID_56::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	EffectFunctions::地震(60, 18);
	//SpellEffects::EffectcSpell_ID_1(pchara->placeX,pchara->placeY);
	効果(pchara);
	return true;//必ずtrueを返す
}

void cSpell_ID_56::CutIn(タイミング timing, cValiableField& valiable)
{
	cSpell::CutIn(timing,valiable);


	if(装備されている())
	{
		if(timing == 被攻撃接近直前_タイミング)
		{
			if(効果量(1) > random()*100)
			{
				sg_pDungeonSystem->頑強要請(装備者(), 効果量(3), 効果量(2));
				
				sg_pDungeonSystem->動的識別(me());
			}
		}
	}
}
