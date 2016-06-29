#include "stdafx.h"

#include "MobAbilityIdiom.h"
#include "EffectFunctions.h"

#include "FindUtility.h"

#include "cCharacter.h"
#include "cDroping.h"
#include "cLandform.h"

using namespace MobAbilityIdiom;


bool MobAbilityIdiom::投擲物反射CutIn(pcCharacter pchara, タイミング timing, cValiableField& valiable) {

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
			sg_pDungeonSystem->投擲要請(pchara, valiable.drops.val(変数_対象落ち物));
			return true;
		}
	}

	return false;
}