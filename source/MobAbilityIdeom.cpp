#include "stdafx.h"

#include "MobAbilityIdiom.h"
#include "EffectFunctions.h"

#include "FindUtility.h"

#include "cCharacter.h"
#include "cDroping.h"
#include "cLandform.h"

using namespace MobAbilityIdiom;

bool MobAbilityIdiom::投擲物反射CutIn::operator()(pcCharacter pchara, タイミング timing, cValiableField& valiable) const{

	if(timing == 投擲攻撃接近直前_タイミング)
	{
		//跳ね返し
		if(!valiable.drops.val(変数_対象落ち物)->跳ね返し無効フラグ && 
			!valiable.drops.val(変数_対象落ち物)->投擲貫通())
		{
			valiable.doubles.val(変数_汎用ブール) = 0;//効果発揮フラグ

			map<tstring, StyleString> val;
			val[_T("Chara")] = pchara->FullName();
			val[_T("Item")] = valiable.drops.val(変数_対象落ち物)->FullName();
			g_Langメッセージ(_T("cMob_ID_67_特殊能力メッセージ"),val);
			
			sg_pDungeonSystem->方向転換要請(pchara, valiable.doubles.val(変数_方向)+4);
			sg_pDungeonSystem->投擲要請(pchara, valiable.drops.val(変数_対象落ち物), DRAWTYPE_ATTACKDRAW);
			return true;
		}
	}

	return false;
}

bool MobAbilityIdiom::拾得物修正状態識別CutIn::operator()(pcCharacter pchara, タイミング timing, cValiableField& valiable) const{

	if(timing == 拾い直後_タイミング)
	{
		if(!valiable.drops[変数_対象落ち物]->修正値識別済み() || !valiable.drops[変数_対象落ち物]->状態値識別済み())
		{
			sg_pDungeonSystem->状態のみ識別( valiable.drops[変数_対象落ち物], 0);
			sg_pDungeonSystem->修正のみ識別( valiable.drops[変数_対象落ち物], 0);
		}
	}

	return false;
}

bool MobAbilityIdiom::投擲物魔法ダメージ化CutIn::operator()(pcCharacter pchara, タイミング timing, cValiableField& valiable) const{

	if(投擲物ダメージ化CutIn(damegeValue)(pchara,timing,valiable) || 魔法ダメージ化CutIn(damegeValue)(pchara,timing,valiable)) {
		return true;
	}

	return false;
}

bool MobAbilityIdiom::魔法ダメージ化CutIn::operator()(pcCharacter pchara, タイミング timing, cValiableField& valiable) const{

	if(timing == 魔法接近直前_タイミング)
	{
		valiable.doubles.val(変数_汎用ブール) = 0;//効果発揮フラグ
	
		map<tstring, StyleString> val;
		val[_T("Chara")] = pchara->ShortName();
		g_Langメッセージ(_T("cMob_ID_19_特殊能力4メッセージ"),val);
		
		sg_pDungeonSystem->強制ダメージ要請(pchara,damegeValue,1,1);

		return true;
	}

	return false;
}

bool MobAbilityIdiom::投擲物ダメージ化CutIn::operator()(pcCharacter pchara, タイミング timing, cValiableField& valiable) const{

	if(timing == 投擲攻撃接近直前_タイミング)
	{
		valiable.doubles.val(変数_汎用ブール) = 0;//効果発揮フラグ
	
		map<tstring, StyleString> val;
		val[_T("Chara")] = pchara->ShortName();
		g_Langメッセージ(_T("cMob_ID_19_特殊能力3メッセージ"),val);
		
		sg_pDungeonSystem->強制ダメージ要請(pchara,damegeValue,1,1);

		return true;
	}

	return false;
}

bool MobAbilityIdiom::遠距離ダメージ軽減CutIn::operator()(pcCharacter pchara, タイミング timing, cValiableField& valiable) const{

	if(timing == ダメージ計算防御時_タイミング)
	{
		pcCharacter penemy = valiable.charas[変数_攻撃者];
		if(penemy == NULL) {
			return false;
		}
		if((pchara->足元地形()->place - penemy->足元地形()->place).dif() > 1){
			valiable.doubles[変数_耐性ボーナス_倍率％] += reduceDamegePercent;
			return true;
		}
	}

	return false;
}