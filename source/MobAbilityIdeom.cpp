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

bool MobAbilityIdiom::フロア開始時所持アイテムランダム識別CutIn::operator()(pcCharacter pchara, タイミング timing, cValiableField& valiable) const{

	if(timing == フロア開始_タイミング)
	{
		vector<pcDroping> itemList = FindUtility::全てのアイテム一覧(pchara);
		itemList = FindUtility::dropFiltering(itemList, FindUtility::完全に鑑定されていないアイテムか);
		itemList = FindUtility::randomSelect(itemList, count);
		int i, size = itemList.size();
		for(i=0; i<size; i++) {
			sg_pDungeonSystem->識別(itemList[i],true);
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

bool MobAbilityIdiom::呪い無効化CutIn::operator()(pcCharacter pchara, タイミング timing, cValiableField& valiable) const{

	if(timing == アイテム呪い直前_タイミング)
	{
		valiable.doubles.val(変数_汎用ブール) = 0;
		return true;
	}

	return false;
}
bool MobAbilityIdiom::悪性異常状態無効化CutIn::operator()(pcCharacter pchara, タイミング timing, cValiableField& valiable) const{

	if(
			//速度系
			timing == 速度減少直前_タイミング
			//精神系
		 || timing == 眠り追加直前_タイミング
		 || timing == バクスイ追加直前_タイミング
		 || timing == びっくり追加直前_タイミング
		 || timing == 金縛り追加直前_タイミング
		 || timing == 封印追加直前_タイミング
		 || timing == 空振り追加直前_タイミング
		 || timing == 貧乏追加直前_タイミング
		 || timing == 錯乱追加直前_タイミング
		 || timing == 臆病追加直前_タイミング
		 //|| timing == 嫉妬追加直前_タイミング
		 || timing == 狂乱追加直前_タイミング
		 || timing == 無意識追加直前_タイミング
			//身体系
		 || timing == 泥酔追加直前_タイミング
		 || timing == 氷付け追加直前_タイミング
		 || timing == 脱力追加直前_タイミング
		 //|| timing == 元気追加直前_タイミング
		 || timing == 軟弱追加直前_タイミング
		 //|| timing == 頑強追加直前_タイミング
		 || timing == 鳥目追加直前_タイミング
		 //|| timing == 健康追加直前_タイミング
		 || timing == 病気追加直前_タイミング
			//呪術系
	     || timing == 死の誘い追加直前_タイミング)

	{
		valiable.doubles.val(変数_汎用ブール) = 0;
		return true;
	}

	return false;
}

bool MobAbilityIdiom::攻撃時自分ノックバックCutIn::operator()(pcCharacter pchara, タイミング timing, cValiableField& valiable) const{

	if(timing == 攻撃直後時_タイミング)
	{
		if(valiable.doubles.exist(変数_直接攻撃フラグ))
		{
			sg_pDungeonSystem->吹き飛ばし要請(pchara, pchara, pchara->aspect+4, 1, 0);
			return true;
		}
	}

	return false;
}

bool MobAbilityIdiom::攻撃時眠り付与CutIn::operator()(pcCharacter pchara, タイミング timing, cValiableField& valiable) const{

	if(timing == 攻撃直後時_タイミング)
	{
		if(valiable.doubles.exist(変数_直接攻撃フラグ))
		{
			if(percent/100.0 > random())
			{
				if(valiable.charas.dim(変数_防御者) != NULL)
				{
					sg_pDungeonSystem->眠り要請(valiable.charas.dim(変数_防御者),turn);

				}
			}
		}
	}
	return false;
}

bool MobAbilityIdiom::常時攻撃力ボーナスCutIn::operator()(pcCharacter pchara, タイミング timing, cValiableField& valiable) const{

	if(timing == 攻撃力計算時_タイミング)
	{
		valiable.doubles[変数_攻撃力ボーナス_倍率] += addPercent / 100.0;
		valiable.doubles[変数_攻撃力ボーナス_定数] += addConst;
		return true;
	}
	return false;
}

bool MobAbilityIdiom::常時防御力ボーナスCutIn::operator()(pcCharacter pchara, タイミング timing, cValiableField& valiable) const{

	if(timing == 防御力計算時_タイミング)
	{
		valiable.doubles[変数_防御力ボーナス_倍率] += addPercent / 100.0;
		valiable.doubles[変数_防御力ボーナス_定数] += addConst;
		return true;
	}
	return false;
}
bool MobAbilityIdiom::属性耐性ボーナス％::operator()(pcCharacter pchara, タイミング timing, cValiableField& valiable) const{

	if(timing == ダメージ計算防御時_タイミング)
	{
		bool match = false;
		set<int>::const_iterator itr = attribsute.begin();
		for(;itr != attribsute.end(); itr++) {
			if(valiable.intsets.val(変数_属性).count(*itr) > 0)
			{
				match = true;
				break;
			}
		}

		if(match)
		{
			valiable.doubles.val(変数_耐性ボーナス_倍率％) += addPercent;
			return true;
		}
	}
	return false;
}

const static int 超遠距離Distance = 4;

bool MobAbilityIdiom::超遠距離攻撃力ボーナスCutIn::operator()(pcCharacter pchara, タイミング timing, cValiableField& valiable) const{

	if(timing == ダメージ計算攻撃時_タイミング)
	{
		pcCharacter attack = valiable.charas[変数_攻撃者];
		pcCharacter defence = valiable.charas[変数_防御者];
		if(attack != NULLCHARA && defence != NULLCHARA && FindUtility::キャラの距離(attack,defence) >= 超遠距離Distance) {
			valiable.doubles[変数_攻撃力ボーナス_倍率] += addPercent / 100.0;
			valiable.doubles[変数_攻撃力ボーナス_定数] += addConst;
			return true;
		}
	}
	return false;
}

bool MobAbilityIdiom::超遠距離防御力ボーナスCutIn::operator()(pcCharacter pchara, タイミング timing, cValiableField& valiable) const{

	if(timing == ダメージ計算防御時_タイミング)
	{
		pcCharacter attack = valiable.charas[変数_攻撃者];
		pcCharacter defence = valiable.charas[変数_防御者];
		if(attack != NULLCHARA && defence != NULLCHARA && FindUtility::キャラの距離(attack,defence) >= 超遠距離Distance) {
			valiable.doubles[変数_防御力ボーナス_倍率] += addPercent / 100.0;
			valiable.doubles[変数_防御力ボーナス_定数] += addConst;
			return true;
		}
	}
	return false;
}

bool MobAbilityIdiom::超遠距離耐性ボーナスCutIn::operator()(pcCharacter pchara, タイミング timing, cValiableField& valiable) const{

	if(timing == ダメージ計算防御時_タイミング)
	{
		pcCharacter attack = valiable.charas[変数_攻撃者];
		pcCharacter defence = valiable.charas[変数_防御者];
		if(attack != NULLCHARA && defence != NULLCHARA && FindUtility::キャラの距離(attack,defence) >= 超遠距離Distance) {
			valiable.doubles[変数_耐性ボーナス_倍率％] += addPercent;
			return true;
		}
	}
	return false;
}