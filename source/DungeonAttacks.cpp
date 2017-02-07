#include "stdafx.h"

#include "cDungeonSystem.h"

#include "EffectFunctions.h"
#include "Gameobjects.h"
#include "../GensouGage.h"
#include "../cFloor.h"



int cDungeonSystem::直接攻撃要請(pcCharacter pchara)
{
	if(pchara == NULLCHARA) return false;

	if(pchara->Condition.混乱状態である())
	{
		方向転換要請(pchara, random()*8);
	}

	return 直接攻撃要請_強制混乱なし(pchara);
}
int cDungeonSystem::直接攻撃要請_強制混乱なし(pcCharacter pchara)
{

	if(直接攻撃可(pchara))
	{//攻撃出来る

		if(pchara == pPlayerChara()) {
			cValiableField val;
			val.doubles.dim(変数_汎用不変定数) = (キャラ前地形(pPlayerChara(),1)->pOnChar != NULL);//目の前に敵がいた
			CutInM().CutIn(sg_pDungeonSystem->pPlayerChara(),主人公素振り_タイミング,val);
		}

		Attackcharacter(pchara);


		return true;
	}
	else 
	{//攻撃できなかった

		return false;
	}

}

pcAttackinformation cDungeonSystem::直接攻撃攻撃情報取得(pcCharacter pchara)
{
	pcAttackinformation pattackinfo = pcAttackinformation(new cAttackinformation);
	
	GetNormalAttackinformation(pchara,pattackinfo);

	return pattackinfo;
}
int cDungeonSystem::直接攻撃可(pcCharacter pchara)
{
	cValiableField valf;
	pcLandform land;
	cCoordinate placeaspect;
	placeaspect.SetAspect(pchara->aspect);
	land = Map().Land(pchara->placeX + placeaspect.x, pchara->placeY + placeaspect.y);
	valf.doubles.dim(変数_汎用ブール) = 1;//許可
	valf.charas.dim(変数_攻撃者) = pchara;
	CutInM().CutIn(land->pOnDrop,隣接地形被攻撃可判定_タイミング,valf);
	CutInM().CutIn(land,隣接地形被攻撃可判定_タイミング,valf);
	CutInM().CutIn(pchara,攻撃可判定_タイミング,valf);
	
	return valf.doubles.val(変数_汎用ブール);
}
	
int cDungeonSystem::投擲攻撃接近(pcCharacter pchara, pcDroping pdrop, int passthroughflag)
{
	if(pchara == NULL) return false;


	pchara->Condition.とても強い刺激();

	if(pchara->持ち物衝突可(pdrop))
	{

		cValiableField valf;
		valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
		valf.drops.dim(変数_対象落ち物) = pdrop;
		valf.doubles.dim(変数_方向) = pdrop->aspect;
		valf.doubles.dim(変数_貫通) = passthroughflag;
		CutInM().CutIn(pchara,投擲攻撃接近直前_タイミング,valf);
		if(valf.doubles.val(変数_汎用ブール))
		{

			cValiableField valf;
			valf.doubles.dim(変数_命中力) = pdrop->命中力();
			valf.doubles.dim(変数_回避力) = pchara->GetItemDodgePowerBase();
			CutInM().CutIn(pchara,アイテム命中判定時_タイミング,valf);
			
			if(pdrop->命中力必中() || isThrowHit(valf.doubles[変数_命中力],valf.doubles[変数_回避力]))
			{//ヒット
				map<tstring, StyleString> valiable;

				valiable[_T("Chara")] = pchara->ShortName();
				valiable[_T("Drop")] = pdrop->FullName();
				g_Langメッセージ(_T("落ちもの衝突メッセージ"),valiable);
				
				//メッセージ(pdrop->FullName() + _T(" は ") + land->pOnChar->FullName() +_T(" に当たった。\n"));

						
				if(pdrop->衝突(pchara))
				{//使用に成功した

					cValiableField val;
					val.charas.dim(変数_防御者) = pchara;
					CutInM().CutIn(pdrop,落ち物衝突直後時_タイミング,val);

					/*
					if(pdrop->Holder() == NULL)
					{//誰も持ってない
						sg_pDungeonSystem->落ち物破壊要請(pdrop);
						
					}
					*/

					if(pdrop->Holder() != NULL)
					{//誰かが持ってる
						return true;
						
					}
					if(pdrop->broken)
					{//誰かが持ってる
						return true;
						
					}

					if(!passthroughflag)
					{
						落ち物落下(pdrop,pdrop->現在地形());
					}

					return true;
				}
				
			}
			else
			{
				map<tstring, StyleString> valiable;
				valiable[_T("Drop")] = pdrop->FullName();
				g_Langメッセージ(_T("落ちもの衝突ミスメッセージ"),valiable);
			}
			
			if(!passthroughflag)
			{
				//使用に失敗した
				c4DVector start,end;

				//落ちアニメーション
				start.set(pdrop->placeX,pdrop->placeY,THROW_BASE_HEIGHT,0);
				end.set(pdrop->placeX,pdrop->placeY,0,0);
				AnimationManager().Anime_Throw(pdrop,start,end,ROUTE_HIGH);

				落ち物落下(pdrop,pdrop->現在地形());
			}
			
			return true;

		}
		else
		{
			if(pdrop->Holder() == NULL)
				落ち物落下(pdrop,pdrop->現在地形());
		}
	}


	
	return false;
}

int cDungeonSystem::魔法接近(pcCharacter pchara, pcDroping pdrop)
{
	pcBook pbook = boost::dynamic_pointer_cast<cBook>(pdrop);
	if(pbook == NULL)
	{
		return false;
	}
	
	pchara->Condition.とても強い刺激();

	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
	valf.drops.dim(変数_対象落ち物) = pdrop;
	valf.doubles.dim(変数_方向) = pdrop->aspect;
	CutInM().CutIn(pchara,魔法接近直前_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール))
	{

		pbook->衝突(pchara);
		

		return true;
	}
	return false;
}

//使ってない・・・
int cDungeonSystem::落ち物接近(pcLandform pland, pcDroping pdrop)
{
	pcCharacter pchara = pland->pOnChar;
	
	if(pchara == NULL) return false;

	return pdrop->衝突(pchara);

}



int cDungeonSystem::攻撃接近(pcAttackinformation pattackinfo)
{	//地形データの位置に攻撃を行う。
	//この状態で(攻撃者)//攻撃力//攻撃属性が決定されて無いといけない


	

	if(pattackinfo->Landplase->pOnChar != NULL)
	{
		pattackinfo->defenser = pcCharacter(pattackinfo->Landplase->pOnChar);

		if(pattackinfo->defenser != NULL)
			pattackinfo->valiable.doubles.dim(変数_回避力) = pattackinfo->defenser->GetDodgePowerBase();

		pattackinfo->valiable.charas.dim(変数_攻撃者) = pattackinfo->attacker;
		pattackinfo->valiable.charas.dim(変数_防御者) = pattackinfo->defenser;

		CutInM().CutIn(pattackinfo->Landplase->pOnChar, 被攻撃接近直前_タイミング, pattackinfo->valiable);

		if(isAttackHit(pattackinfo))
		{
			//ヒットしたら
			AttackHit(pattackinfo);
			
		}
		else
		{
			//ミスしたら
			AttackMiss(pattackinfo);
		}

		if(pattackinfo->Landplase->pOnChar != NULL)
		{
			pattackinfo->Landplase->pOnChar->Condition.とても強い刺激();
		}
		//アイテム誘爆
		CutInM().CutIn(pattackinfo->Landplase->pOnDrop, 被攻撃接近直後_タイミング, pattackinfo->valiable);
		
		CutInM().CutIn(pattackinfo->Landplase->pOnChar, 被攻撃接近直後_タイミング, pattackinfo->valiable);

		return true;
	}


	//アイテム誘爆
	CutInM().CutIn(pattackinfo->Landplase->pOnDrop, 被攻撃接近直後_タイミング, pattackinfo->valiable);

	CutInM().CutIn(pattackinfo->Landplase->pOnChar, 被攻撃接近直後_タイミング, pattackinfo->valiable);


	return false;
}


bool cDungeonSystem::isCanNeighberAttack(pcCharacter pchara, int aspect, int distance, int diagonthrough, int wallthrough)
{
	if(wallthrough) return true;

	pcLandform land;
	cCoordinate placeaspect;

	placeaspect.SetAspect(aspect);

	distance = min(distance,max(MAPWIDTH,MAPHEIGHT));

	int i;


	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 0;
	CutInM().CutIn(pchara, 攻撃壁貫通判定_タイミング, valf);
	

	for(i=1;i<=distance;i++)
	{

		land = Map().Land(pchara->placeX + i*placeaspect.x, pchara->placeY + i*placeaspect.y);

		if(abs(placeaspect.x)+abs(placeaspect.y) > 1)
		{//斜め


			pcLandform sublandX,sublandY;
		
			sublandX = Map().Land(land->placeX - placeaspect.x, land->placeY);
			sublandY = Map().Land(land->placeX, land->placeY - placeaspect.y);


			if(valf.doubles.val(変数_汎用ブール) ||
				(!land->iswall() && ((sublandX->diagonthrough(pchara->水上歩行(),pchara->壁中歩行(),pchara->空中歩行()) && sublandY->diagonthrough(pchara->水上歩行(),pchara->壁中歩行(),pchara->空中歩行())) || diagonthrough )
				))
			{
				continue;
			}
			else
			{
				return false;
			}
		}
		else
		{

			if(valf.doubles.val(変数_汎用ブール) || !land->iswall())
			{
				continue;
			}
			else
			{
				return false;
			}
		}

	}

	return true;
}




//攻撃の情報を得る
int cDungeonSystem::GetNormalAttackinformation(pcCharacter pchara,pcAttackinformation &pcattackinfo)
{


	pcattackinfo->attacker = pchara;
	pcattackinfo->valiable.doubles[変数_攻撃力] = pchara->GetAttackPower();
	pcattackinfo->valiable.doubles.dim(変数_命中力) = pchara->GetHitPowerBase();
	pcattackinfo->valiable.doubles.dim(変数_方向) = pchara->aspect;
	pcattackinfo->valiable.doubles.dim(変数_距離) = 1;
	
	int aspect = pcattackinfo->valiable.doubles.dim(変数_方向);
	int distance = pcattackinfo->valiable.doubles.dim(変数_距離);
	pcattackinfo->Landplase = pchara->GetAttackLand(aspect,distance);
	pcattackinfo->valiable.doubles[変数_方向] = aspect;
	pcattackinfo->valiable.doubles[変数_距離] = distance;
	
	return true;
}


//攻撃する
int cDungeonSystem::Attackcharacter(pcCharacter pchara)
{
	
	//アニメ
	pchara->通常攻撃_アニメ();
	

	//ここで攻撃の情報を得て攻撃接近に渡す
	pcAttackinformation pattackinfo = pcAttackinformation(new cAttackinformation);
	
	GetNormalAttackinformation(pchara,pattackinfo);

	CutInM().CutIn(pchara, 直接攻撃直前_タイミング,pattackinfo->valiable);
	
	pattackinfo->valiable.doubles.dim(変数_直接攻撃フラグ) = 1;
	if(isCanNeighberAttack(pchara,pchara->aspect,pattackinfo->valiable.doubles[変数_距離],0,0))
	{//仮
		攻撃接近(pattackinfo);
	}
	CutInM().CutIn(pattackinfo->Landplase, 被攻撃接近直後_タイミング, pattackinfo->valiable);
	CutInM().CutIn(pattackinfo->Landplase->pOnDrop, 被攻撃接近直後_タイミング, pattackinfo->valiable);


	return true;
}

//攻撃があたったかどうか
int cDungeonSystem::isAttackHit(pcAttackinformation pattackinfo)
{
	double HitPower,DodgePower;
	if(pattackinfo->valiable.doubles.exist(変数_命中力))
	{
		HitPower = pattackinfo->valiable.doubles[変数_命中力];
	}
	else
	{
		HitPower = 100;
	}
	if(pattackinfo->valiable.doubles.exist(変数_回避力))
	{
		DodgePower = pattackinfo->valiable.doubles[変数_回避力];
	}
	else
	{
		DodgePower = 0;
	}

	if(pattackinfo->valiable.intsets[変数_属性].count(攻撃属性::必中)) return true;
	


	if((HitPower - DodgePower > random()*100.0))
	{

		//あたった
		return true;
	}
	else
	{
		return false;
	}
}

int cDungeonSystem::isThrowHit(double hitpower, double dodgepower)
{


	if(hitpower - dodgepower > random()*100.0)
	{

		//あたった
		return true;
	}
	else
	{
		return false;
	}
}


int cDungeonSystem::AttackHit(pcAttackinformation pattackinfo)
{
	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("hit.wav"),pattackinfo->defenser->足元地形()->place);

	
	//cValiableField val = pattackinfo->valiable;
	//val.doubles.dim(変数_攻撃力) = pattackinfo->valiable.doubles[変数_攻撃力];
	//val.doubles.dim(変数_攻撃力) = pattackinfo->valiable.doubles[変数_攻撃力];
	pattackinfo->valiable.charas.dim(変数_攻撃者) = pattackinfo->attacker;
	pattackinfo->valiable.charas.dim(変数_防御者) = pattackinfo->defenser;
	pattackinfo->valiable.doubles.dim(変数_攻撃力ボーナス_倍率) = 1.0;
	pattackinfo->valiable.doubles.dim(変数_攻撃力ボーナス_定数) = 0.0;
	pattackinfo->valiable.doubles.dim(変数_防御力ボーナス_倍率) = 1.0;
	pattackinfo->valiable.doubles.dim(変数_防御力ボーナス_定数) = 0.0;
	pattackinfo->valiable.doubles.dim(変数_耐性ボーナス_倍率％) = 0.0;
	pattackinfo->valiable.doubles.dim(変数_防御効果ありフラグ) = 0;
	
	//カットイン
	CutInM().CutIn(pattackinfo->attacker, ダメージ計算攻撃時_タイミング, pattackinfo->valiable);
	CutInM().CutIn(pattackinfo->defenser, ダメージ計算防御時_タイミング, pattackinfo->valiable);

	CutInM().CutIn(pattackinfo->attacker, ダメージ計算攻撃時優先度低_タイミング, pattackinfo->valiable);
	//pattackinfo->valiable = val;

	if(pattackinfo->defenser != NULL)
	{
		pattackinfo->valiable.doubles.dim(変数_防御力) = pattackinfo->defenser->GetDefencePower();
	}
	else pattackinfo->valiable.doubles.dim(変数_防御力) = 0;
			
	//ダメージ計算
	int damage = CalculateDamege(pattackinfo);
	
	//(攻撃者)防御者決定済み

	if(pattackinfo->defenser != NULL && pattackinfo->valiable.doubles.val(変数_防御効果ありフラグ))
	{
		pattackinfo->defenser->通常防御_アニメ();
	}

	if(pattackinfo->valiable.intsets[変数_属性].count(攻撃属性::火))
	{
		EffectFunctions::ダメージ火エフェクト(pattackinfo->defenser->placeX, pattackinfo->defenser->placeY);
	}
	if(pattackinfo->valiable.intsets[変数_属性].count(攻撃属性::水))
	{
		EffectFunctions::ダメージ水エフェクト(pattackinfo->defenser->placeX, pattackinfo->defenser->placeY);
	}
	if(pattackinfo->valiable.intsets[変数_属性].count(攻撃属性::電気))
	{
		EffectFunctions::ダメージ電気エフェクト(pattackinfo->defenser->placeX, pattackinfo->defenser->placeY);
	}
	if(pattackinfo->valiable.intsets[変数_属性].count(攻撃属性::冷気))
	{
		EffectFunctions::ダメージ冷気エフェクト(pattackinfo->defenser->placeX, pattackinfo->defenser->placeY);
	}
	if(pattackinfo->valiable.intsets[変数_属性].count(攻撃属性::爆発))
	{
		EffectFunctions::ダメージ爆発エフェクト(pattackinfo->defenser->placeX, pattackinfo->defenser->placeY);
	}
	if(pattackinfo->valiable.intsets[変数_属性].count(攻撃属性::気))
	{
		EffectFunctions::ダメージ気エフェクト(pattackinfo->defenser->placeX, pattackinfo->defenser->placeY);
	}


	pattackinfo->valiable.doubles.dim(変数_ダメージ) = damage;//許可
	CutInM().CutIn(pattackinfo->defenser,ダメージ時_タイミング,pattackinfo->valiable);
	damage = pattackinfo->valiable.doubles.val(変数_ダメージ);
	
	StyleString m;

	map<tstring, StyleString> valiable;
	
	if(pattackinfo->valiable.doubles.exist(変数_ダメージキャップ))
	{
		if(damage > pattackinfo->valiable.doubles[変数_ダメージキャップ])
		{
			damage = pattackinfo->valiable.doubles[変数_ダメージキャップ];
		}
	}

	valiable[_T("DeferName")] = pattackinfo->defenser->ShortName();
	valiable[_T("Damege")] = setStyle(damage,DAMAGE_COLOR);

	if(pattackinfo->valiable.intsets[変数_属性].count(攻撃属性::敵即死))
	{//即死
		if(pattackinfo->defenser != NULL
			&& pattackinfo->defenser->Forse == CHARACTER_FORSE_ENEMY
			&& pattackinfo->defenser->雑魚属性())
		{
			damage = pattackinfo->defenser->HP;
		}
	}

	if(pattackinfo->defenser->Condition.無敵状態である())
	{
		damage = 0;
		g_Langメッセージ(_T("攻撃無ダメージメッセージ"),valiable);
	}
	else if(pattackinfo->attacker != NULL)
	{

		
		if(pattackinfo->defenser == sg_pDungeonSystem->pPlayerChara())
		{//防御者が自分の場合
			if(pattackinfo->attacker != NULL && pattackinfo->attacker != pattackinfo->defenser)
			{
				if(キャラクター目視可(pattackinfo->attacker))
				{
					valiable[_T("AtkerName")] = pattackinfo->attacker->ShortName();
				}
				else
				{
					valiable[_T("AtkerName")] = g_Lang(_T("不明敵名称"));
				}
				
				g_Langメッセージ(_T("攻撃自分ダメージ敵目視メッセージ"),valiable);
				
				//m = m + pattackinfo->defenser->FullName()+_T("に ")+setStyle(damage,DAMAGE_COLOR)+_T(" の")+setStyle(_T("ダメージ")/*,DAMAGE_COLOR*/)+_T("を与えた。\n");
				//メッセージ(m);
			}
			else
			{
				g_Langメッセージ(_T("攻撃自分ダメージメッセージ"),valiable);
			}
		}
		else
		{
			

			//m = m + pattackinfo->defenser->ShortName()+_T("は ");
			if(pattackinfo->attacker != NULL && キャラクター目視可(pattackinfo->attacker))
			{
				valiable[_T("AtkerName")] = pattackinfo->attacker->ShortName();
				g_Langメッセージ(_T("攻撃ダメージ敵目視メッセージ"),valiable);
				//m = m+ pattackinfo->attacker->FullName()+_T("から ");
			}
			else
			{
				g_Langメッセージ(_T("攻撃ダメージメッセージ"),valiable);
			}
			//m = m+setStyle(damage,DAMAGE_COLOR)+_T(" の")+setStyle(_T("ダメージ")/*,DAMAGE_COLOR*/)+_T("を受けた。\n");
			//メッセージ(m);
			
		}
	}
	else
	{
		//m = m + pattackinfo->defenser->FullName()+_T("は ");
		//if(pattackinfo->attacker != NULL && キャラクター目視可(pattackinfo->attacker))
		//{
			//m = m+ pattackinfo->attacker->FullName()+_T("から ");
		//}
		//m = m+setStyle(damage,DAMAGE_COLOR)+_T(" の")+setStyle(_T("ダメージ")/*,DAMAGE_COLOR*/)+_T("を受けた。\n");
		//メッセージ(m);
		g_Langメッセージ(_T("攻撃被ダメージメッセージ"),valiable);

	}

	DamageCharacter(pattackinfo->defenser, damage, true, -pattackinfo->valiable.doubles.dim(変数_耐性ボーナス_倍率％)/100.0 + 1.0);

	if(pattackinfo->defenser->死亡())
	{//経験値
		if(pattackinfo->attacker == sg_pDungeonSystem->pPlayerChara())
		{

			if (pattackinfo->defenser->CharaAttribute().count(キャラ属性::弾幕)) {
				double power = 1;
				瞬間幻想度％加算(power);
			}
			else {
				double power = 5;
				恒常幻想度％加算(power);
			}
		}

		EXTcharge(pattackinfo);
		
		CutInM().CutIn(pattackinfo->attacker, 敵倒し直後時_タイミング, pattackinfo->valiable);

	}

	

	//val.doubles.dim(変数_攻撃力) = pattackinfo->valiable.doubles[変数_攻撃力];
	//val.doubles.dim(変数_攻撃力) = pattackinfo->valiable.doubles[変数_攻撃力];


	//カットイン
	CutInM().CutIn(pattackinfo->attacker, 攻撃直後時_タイミング, pattackinfo->valiable);
	CutInM().CutIn(pattackinfo->defenser, 被攻撃直後時_タイミング, pattackinfo->valiable);



	return true;
}

//攻撃が外れた
int cDungeonSystem::AttackMiss(pcAttackinformation pattackinfo)
{

	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("miss.wav"),pattackinfo->defenser->足元地形()->place);


	if(pattackinfo->attacker != NULL && キャラクター目視可(pattackinfo->attacker))
	{
		//(攻撃者)防御者決定済み
		map<tstring, StyleString> valiable;
		valiable[_T("AtkerName")] = pattackinfo->attacker->ShortName();
		g_Langメッセージ(_T("攻撃ミスメッセージ"),valiable);
	}

	return true;
}



//ダメージ計算
int cDungeonSystem::CalculateDamege(pcAttackinformation pattackinfo)
{
	//攻撃者 //防御者 //
	int damage = 0;



	if(pattackinfo->valiable.doubles.exist(変数_定数ダメージフラグ) && pattackinfo->valiable.doubles[変数_定数ダメージフラグ])
	{
		damage = pattackinfo->valiable.doubles[変数_攻撃力];
		damage *= pattackinfo->valiable.doubles[変数_攻撃力ボーナス_倍率];
		damage += pattackinfo->valiable.doubles[変数_攻撃力ボーナス_定数];
	}
	else if(pattackinfo->valiable.doubles.exist(変数_割合ダメージフラグ) && pattackinfo->valiable.doubles[変数_割合ダメージフラグ])
	{
		if(pattackinfo->defenser != NULL)
		{
			damage = pattackinfo->defenser->HP * pattackinfo->valiable.doubles[変数_割合ダメージ] / 100.0;
		}
		damage *= pattackinfo->valiable.doubles[変数_攻撃力ボーナス_倍率];
		damage += pattackinfo->valiable.doubles[変数_攻撃力ボーナス_定数];
	
	}
	else
	{	
		double at = pattackinfo->valiable.doubles[変数_攻撃力] * (1+(0.2*random()-0.1)) + (2*random()-1);
		at *= pattackinfo->valiable.doubles[変数_攻撃力ボーナス_倍率];
		at += pattackinfo->valiable.doubles[変数_攻撃力ボーナス_定数];

		pattackinfo->valiable.doubles[変数_攻撃力] = at;
		
		double df = pattackinfo->valiable.doubles[変数_防御力] * (1+(0.2*random()-0.1));
		df *= pattackinfo->valiable.doubles[変数_防御力ボーナス_倍率];
		df += pattackinfo->valiable.doubles[変数_防御力ボーナス_定数];
			
		pattackinfo->valiable.doubles[変数_防御力] = df;
		
		double k = pow(0.9875,df);

		//ダメージ式
		damage = ( (at - 0.458*df*k)*k )*0.9 + at*0.1;

		//damage = damage*random_range(0.9,1.1);
		if(df > 0) pattackinfo->valiable.doubles.val(変数_防御効果ありフラグ) = 1;
	}

	if(pattackinfo->valiable.doubles.exist(変数_耐性ボーナス_倍率％))
	{
		damage *= (100 - pattackinfo->valiable.doubles.val(変数_耐性ボーナス_倍率％))/100.0;

		if(pattackinfo->valiable.doubles.val(変数_耐性ボーナス_倍率％) > 0) pattackinfo->valiable.doubles.val(変数_防御効果ありフラグ) = 1;
	}

	if(damage <= 0)
	{
		damage = 1;
	}


	return damage;
}
void cDungeonSystem::恒常幻想度％加算(double power)
{
	pGensouGage()->add_long_live_power(power);
}
void cDungeonSystem::瞬間幻想度％加算(double power)
{
	pGensouGage()->add_short_live_power(power);
	pGensouGage()->keep_short_live_power();
}
double cDungeonSystem::幻想度％()
{
	return pGensouGage()->sum_power();
}