#include "stdafx.h"

#include "cDungeonSystem.h"

#include "firecommand.h"
#include "Gameobjects.h"
#include "EffectFunctions.h"
#include "FindUtility.h"
#include <boost/pointer_cast.hpp>

int cDungeonSystem::回復要請(pcCharacter pchara, int recovery, int Messageflag)
{
	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = true;//効果発揮フラグ
	valf.doubles.dim(変数_汎用実数) = recovery;
	valf.doubles.dim(変数_メッセージフラグ) = Messageflag;
	valf.charas.dim(変数_対象者) = pchara;
	CutInM().CutIn(pchara,回復直前_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール) && valf.doubles.dim(変数_汎用実数) > 0)
	{

		int beforeHP = pchara->HP,afterHP;

		RecoverCharacter(pchara,recovery,Messageflag);
		afterHP = pchara->HP;

		if(Messageflag && pchara == pPlayerChara())
		{//メッセージを表示する
			map<tstring, StyleString> valiable;

			valiable[_T("Value")] = setStyle(afterHP-beforeHP,RECOVER_COLOR);
			valiable[_T("Chara")] = pchara->ShortName();
		
			g_Langメッセージ(_T("HP回復メッセージ"),valiable);

			//メッセージ(pchara->ShortName() +_T(" のHPが ")+setStyle(afterHP-beforeHP,RECOVER_COLOR)+_T(" 回復した。\n"));
		}

		return true;
	}
	return false;
}
int cDungeonSystem::HP設定要請(pcCharacter pchara, int afterHP, int Messageflag)
{

	int beforeHP = pchara->HP;

	if(beforeHP == afterHP) return false;

	if(beforeHP < afterHP)
	{
		int recovery = afterHP-beforeHP;
		RecoverCharacter(pchara,recovery);

		afterHP = pchara->HP;

		if(Messageflag && pchara == pPlayerChara())
		{//メッセージを表示する
			map<tstring, StyleString> valiable;

			valiable[_T("Value")] = setStyle(afterHP-beforeHP,RECOVER_COLOR);
			valiable[_T("Chara")] = pchara->ShortName();
		
			g_Langメッセージ(_T("HP回復メッセージ"),valiable);

			//メッセージ(pchara->ShortName() +_T(" のHPが ")+setStyle(afterHP-beforeHP,RECOVER_COLOR)+_T(" 回復した。\n"));
		}

	}
	else
	{
		int damage = beforeHP-afterHP;

		afterHP = pchara->HP;

		DamageCharacter(pchara,damage,0);
		if(Messageflag && pchara == pPlayerChara())
		{//メッセージを表示する
			map<tstring, StyleString> valiable;

			//攻撃メッセージと共用
			valiable[_T("DeferName")] = pchara->ShortName();
			valiable[_T("Damege")] = setStyle(damage,DAMAGE_COLOR);
		
			g_Langメッセージ(_T("攻撃被ダメージメッセージ"),valiable);
			
			//メッセージ(pchara->ShortName() +_T(" は ")+setStyle(damage,DAMAGE_COLOR)+_T(" のダメージを受けた。\n"));
		}


	}
	
	return true;
}

int cDungeonSystem::最大HP拡張要請(pcCharacter pchara, int extention, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	int beforeMHP = pchara->MHP,afterMHP;
	HPextentionCharacter(pchara, extention);
	afterMHP = pchara->MHP;

	if(Messageflag && pchara == pPlayerChara())
	{//メッセージを表示する
		map<tstring, StyleString> valiable;

		valiable[_T("Value")] = setStyle(afterMHP-beforeMHP,RECOVER_COLOR);
		valiable[_T("Chara")] = pchara->ShortName();
	
		g_Langメッセージ(_T("最大HP拡張メッセージ"),valiable);

		//メッセージ(pchara->ShortName() +_T(" の最大HPが ")+setStyle(afterMHP-beforeMHP,RECOVER_COLOR)+_T(" 上がった。\n"));
	}

	return true;
}
int cDungeonSystem::最大HP縮小要請(pcCharacter pchara, int reduction, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	int beforeMHP = pchara->MHP,afterMHP;
	HPreductionCharacter(pchara, reduction);
	afterMHP = pchara->MHP;

	if(Messageflag && pchara == pPlayerChara())
	{//メッセージを表示する
		map<tstring, StyleString> valiable;

		valiable[_T("Value")] = setStyle(-(afterMHP-beforeMHP),RECOVER_COLOR);
		valiable[_T("Chara")] = pchara->ShortName();
	
		g_Langメッセージ(_T("最大HP縮小メッセージ"),valiable);

		//メッセージ(pchara->ShortName() +_T(" の最大HPが ")+setStyle(afterMHP-beforeMHP,RECOVER_COLOR)+_T(" 上がった。\n"));
	}

	return true;
}

int cDungeonSystem::満腹度回復要請(pcCharacter pchara, double recovery, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	StmRecoverCharacter(pchara,recovery);

	if(Messageflag && pchara == pPlayerChara())
	{//メッセージを表示する
		if(pchara->Stomach == pchara->MaxStomach)
		{
			map<tstring, StyleString> valiable;

			valiable[_T("Chara")] = pchara->ShortName();
		
			g_Langメッセージ(_T("満腹度回復メッセージ_満腹"),valiable);
			//メッセージ(pchara->ShortName() +_T(" はお腹がいっぱいになった。\n"));
		}
		else
		{
			map<tstring, StyleString> valiable;

			valiable[_T("Chara")] = pchara->ShortName();
		
			g_Langメッセージ(_T("満腹度回復メッセージ_否満腹"),valiable);
			//メッセージ(pchara->ShortName() +_T(" はお腹が少しふくれた。\n"));
		}
	}

	return true;
}
int cDungeonSystem::満腹度設定要請(pcCharacter pchara, double afterSP, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	if(pchara->Stomach == afterSP) return false;

	if(pchara->Stomach < afterSP)
	{
		int recovery = afterSP - pchara->Stomach;
		StmRecoverCharacter(pchara,recovery);

		if(Messageflag && pchara == pPlayerChara())
		{//メッセージを表示する
			if(pchara->Stomach == pchara->MaxStomach)
			{
				map<tstring, StyleString> valiable;

				valiable[_T("Chara")] = pchara->ShortName();
			
				g_Langメッセージ(_T("満腹度回復メッセージ_満腹"),valiable);
				//メッセージ(pchara->ShortName() +_T(" はお腹がいっぱいになった。\n"));
			}
			else
			{
				map<tstring, StyleString> valiable;

				valiable[_T("Chara")] = pchara->ShortName();
			
				g_Langメッセージ(_T("満腹度回復メッセージ_否満腹"),valiable);
				//メッセージ(pchara->ShortName() +_T(" はお腹が少しふくれた。\n"));
			}
		}

	}
	else
	{
		int hunger = pchara->Stomach - afterSP;
		hungerCharacter(pchara,hunger);
		if(Messageflag && pchara == pPlayerChara())
		{//メッセージを表示する
			map<tstring, StyleString> valiable;

			valiable[_T("Value")] = setStyle((int)hunger,HUNGER_COLOR);
			valiable[_T("Chara")] = pchara->ShortName();
		
			g_Langメッセージ(_T("満腹度減少メッセージ"),valiable);
			
			//メッセージ(pchara->ShortName() +_T(" の満腹度が ")+setStyle(hunger,HUNGER_COLOR)+_T(" 減少した。\n"));
		}

	}


	return true;
}
int cDungeonSystem::最大満腹度拡張要請(pcCharacter pchara, int extention, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	int beforeMaxStomach = pchara->MaxStomach,afterMaxStomach;
	StmextentionCharacter(pchara, extention);
	afterMaxStomach = pchara->MaxStomach;

	if(Messageflag && pchara == pPlayerChara())
	{//メッセージを表示する
		map<tstring, StyleString> valiable;

		valiable[_T("Value")] = setStyle(afterMaxStomach-beforeMaxStomach,RECOVER_COLOR);
		valiable[_T("Chara")] = pchara->ShortName();
	
		g_Langメッセージ(_T("最大満腹度拡張メッセージ"),valiable);
		
		//メッセージ(pchara->ShortName() +_T(" の最大満腹度が ")+setStyle(afterMaxStomach-beforeMaxStomach,RECOVER_COLOR)+_T(" 上がった。\n"));
	}
	return true;
}
int cDungeonSystem::最大満腹度縮小要請(pcCharacter pchara, int reduction, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	int beforeMaxStomach = pchara->MaxStomach,afterMaxStomach;
	StmextentionCharacter(pchara, -reduction);
	afterMaxStomach = pchara->MaxStomach;

	if(Messageflag && pchara == pPlayerChara())
	{//メッセージを表示する
		map<tstring, StyleString> valiable;

		valiable[_T("Value")] = setStyle(beforeMaxStomach-afterMaxStomach,RECOVER_COLOR);
		valiable[_T("Chara")] = pchara->ShortName();
	
		g_Langメッセージ(_T("最大満腹度縮小メッセージ"),valiable);
		
	}
	return true;
}


int cDungeonSystem::速度増加要請(pcCharacter pchara, int turn, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	//if(!(pchara->雑魚属性()))//(pchara->Forse == CHARACTER_FORSE_FRIEND)
	//{
		pchara->Condition.速度増加(turn);
	//}
	//else
	//{//味方以外
	//	pchara->Condition.速度増加(GAME_TURN_GAMEOVER);//永続
	//}
	

	if(Messageflag && pchara == pPlayerChara())
	{//メッセージを表示する
		map<tstring, StyleString> valiable;

		valiable[_T("Chara")] = pchara->ShortName();
	
		g_Langメッセージ(_T("速度増加メッセージ"),valiable);
		
		//メッセージ(pchara->ShortName() +_T(" の素早さが 上がった！\n"));
		//sg_pDungeonSystem->全キャラ強制ターンエンド要請();
	}
	return true;
}

int cDungeonSystem::速度減少要請(pcCharacter pchara, int turn, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
	CutInM().CutIn(pchara,速度減少直前_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール))
	{

		if(!(pchara->雑魚属性()))//(pchara->Forse == CHARACTER_FORSE_FRIEND)
		{
			pchara->Condition.速度減少(turn);
		}
		else
		{//味方以外
			pchara->Condition.速度減少(GAME_TURN_GAMEOVER);//永続
		}

		if(Messageflag && pchara == pPlayerChara())
		{//メッセージを表示する
			map<tstring, StyleString> valiable;

			valiable[_T("Chara")] = pchara->ShortName();
		
			g_Langメッセージ(_T("速度減少メッセージ"),valiable);
			
			//メッセージ(pchara->ShortName() +_T(" の素早さが 下がった。\n"));
		}
		return true;
	}
	return false;
}
int cDungeonSystem::眠り要請(pcCharacter pchara, int turn, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
	CutInM().CutIn(pchara,眠り追加直前_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール))
	{

		pchara->Condition.眠り追加(turn);


		if(Messageflag)
		{//メッセージを表示する
			map<tstring, StyleString> valiable;

			valiable[_T("Chara")] = pchara->ShortName();
		
			g_Langメッセージ(_T("眠りメッセージ"),valiable);

		}
		return true;
	}
	return false;
}
int cDungeonSystem::バクスイ要請(pcCharacter pchara, int turn, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
	CutInM().CutIn(pchara,バクスイ追加直前_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール))
	{

		pchara->Condition.バクスイ追加(turn);

		if(Messageflag)
		{//メッセージを表示する
			map<tstring, StyleString> valiable;

			valiable[_T("Chara")] = pchara->ShortName();
		
			g_Langメッセージ(_T("バクスイメッセージ"),valiable);
		}

		return true;
	}
	return false;
}
int cDungeonSystem::びっくり要請(pcCharacter pchara, int turn, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
	CutInM().CutIn(pchara,びっくり追加直前_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール))
	{

		pchara->Condition.びっくり追加(turn);
		pchara->Condition.とても強い刺激();

		if(Messageflag && キャラクター目視可(pchara))
		{//メッセージを表示する
			map<tstring, StyleString> valiable;

			valiable[_T("Chara")] = pchara->ShortName();
		
			g_Langメッセージ(_T("びっくりメッセージ"),valiable);
		}

		return true;
	}
	return false;
}
int cDungeonSystem::金縛り要請(pcCharacter pchara, int turn, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
	CutInM().CutIn(pchara,金縛り追加直前_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール))
	{

		pchara->Condition.金縛り追加(turn);

		if(Messageflag)
		{//メッセージを表示する
			map<tstring, StyleString> valiable;

			valiable[_T("Chara")] = pchara->ShortName();
		
			g_Langメッセージ(_T("金縛りメッセージ"),valiable);
			
		}

		return true;
	}
	return false;
}

int cDungeonSystem::泥酔要請(pcCharacter pchara, int turn, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
	CutInM().CutIn(pchara,泥酔追加直前_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール))
	{

		pchara->Condition.泥酔追加(turn);

		if(Messageflag)
		{//メッセージを表示する
			map<tstring, StyleString> valiable;

			valiable[_T("Chara")] = pchara->ShortName();
		
			g_Langメッセージ(_T("泥酔メッセージ"),valiable);
			
		}

		return true;
	}
	return false;
}
int cDungeonSystem::封印要請(pcCharacter pchara, int turn, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
	CutInM().CutIn(pchara,封印追加直前_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール))
	{

		//if(!(pchara->雑魚属性()))//(pchara->Forse == CHARACTER_FORSE_FRIEND)
		//{
			pchara->Condition.封印追加(turn);
		//}
		//else
		//{//味方以外
		//	if(turn > 0) pchara->Condition.封印追加(GAME_TURN_GAMEOVER);//永続
		//	else pchara->Condition.封印追加(0);
		//}

		pchara->Condition.擬態追加(-1);
		pchara->Condition.無敵追加(-1);
		pchara->Condition.みがわり追加(-1,NULLCHARA);

		if(Messageflag)
		{//メッセージを表示する
			map<tstring, StyleString> valiable;

			valiable[_T("Chara")] = pchara->ShortName();
		
			g_Langメッセージ(_T("封印メッセージ"),valiable);
			
		}

		return true;
	}
	return false;
}
int cDungeonSystem::空振り要請(pcCharacter pchara, int turn, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
	CutInM().CutIn(pchara,空振り追加直前_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール))
	{

		pchara->Condition.空振り追加(turn);


		if(Messageflag)
		{//メッセージを表示する
			map<tstring, StyleString> valiable;

			valiable[_T("Chara")] = pchara->ShortName();
		
			g_Langメッセージ(_T("空振りメッセージ"),valiable);
			
		}
		return true;	
	}
	return false;
}
int cDungeonSystem::貧乏要請(pcCharacter pchara, int turn, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
	CutInM().CutIn(pchara,貧乏追加直前_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール))
	{

		pchara->Condition.貧乏追加(turn);

		if(Messageflag)
		{//メッセージを表示する
			map<tstring, StyleString> valiable;

			valiable[_T("Chara")] = pchara->ShortName();
		
			g_Langメッセージ(_T("貧乏メッセージ"),valiable);
			
		}

		return true;
	}
	return false;
}
int cDungeonSystem::錯乱要請(pcCharacter pchara, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
	CutInM().CutIn(pchara,錯乱追加直前_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール))
	{
		if(Messageflag)
		{//メッセージを表示する
			map<tstring, StyleString> valiable;

			valiable[_T("Chara")] = pchara->ShortName();
		
			g_Langメッセージ(_T("錯乱メッセージ"),valiable);
			
		}

		pchara->emotion.insert(EMOTION_PIYO);

		AnimationManager().Anime_DisplayChange(
			&pchara->visibleemotion,
			pchara->emotion);

		sg_pDungeonSystem->AnimationManager().
			Anime_PlaySE(_T("confuse.wav"),pchara->足元地形()->place);

		if(Map().isOnDisplay(pchara->placeX,pchara->placeY,0)) AnimationManager().Anime_Wait(pchara->placeX,pchara->placeY,30);

		
		pchara->emotion.erase(EMOTION_PIYO);

		AnimationManager().Anime_DisplayChange(
			&pchara->visibleemotion,
			pchara->emotion);

		if(pchara != pPlayerChara())
		{
			泥酔要請(pchara,10,false);
		}
		else if(!pchara->holdItem.empty())
		{
			pcDroping pdrop = pchara->holdItem[pchara->holdItem.size()*random()];

			do
			{
				pcSpell pcspl = boost::dynamic_pointer_cast<cSpell>(pdrop);
				if(pcspl != NULL)
				{
					if(pcspl->ID() != 3029)
					{
						if(pcspl->装備されている())
						{
							宣言強制要請(pchara,pcspl);		
						}
						else
						{
							if(random() > 0.5)
							{
								Spell装備(pDevice_D3D,pchara->最大装備スペル数()*random(),pcspl);
							}
							else
							{
								宣言強制要請(pchara,pcspl);	
							}
						}
					
						break;
					}
				}
				pcEquipment pcequ = boost::dynamic_pointer_cast<cEquipment>(pdrop);
				if(pcequ != NULL)
				{
					if(pcequ->装備されている_攻撃用() || pcequ->装備されている_防御用())
					{
						Equipmentはずす(pDevice_D3D,pcequ);		
					}
					else
					{
						Equipment装備(pDevice_D3D,EQUIP_PART_MAXSIZE*random(),pcequ);
					}
				
					break;
				}
				pcFood pcfood = boost::dynamic_pointer_cast<cFood>(pdrop);
				if(pcfood != NULL)
				{
					食べる(pDevice_D3D,pcfood);
				
					break;
				}
				pcDrink pcdrk = boost::dynamic_pointer_cast<cDrink>(pdrop);
				if(pcdrk != NULL)
				{
					
					飲む(pDevice_D3D,pcdrk);
					break;
				}
				pcBook pcBok = boost::dynamic_pointer_cast<cBook>(pdrop);
				if(pcBok != NULL)
				{
					読む(pDevice_D3D,pcBok);
					break;
				}
				pcBox pcBox = boost::dynamic_pointer_cast<cBox>(pdrop);
				if(pcBox != NULL)
				{
					if(pcBox->開け用())
					{
						開ける(pDevice_D3D,pcBox);
					}
					else
					{
						vector<pcDroping> vDrop = FindUtility::装備品以外の表面手持ちアイテム店売り除く(pchara);
						入れる(pDevice_D3D,vDrop[vDrop.size()*random()],pcBox);
					}
				
					break;
				}
				pcBullet pcBlt = boost::dynamic_pointer_cast<cBullet>(pdrop);
				if(pcBlt != NULL)
				{
					撃つ(pDevice_D3D,pcBlt);
				
					break;
				}

				方向転換要請(pchara,random()*8);
				投擲要請(pchara,pdrop);

			}
			while(0);
		}
		else
		{
		
		
		}

		if(pPlayerChara() == pchara)
			ターンエンド();
		else
			強制ターンエンド要請(pchara);



		return true;
	}
	return false;
}
int cDungeonSystem::臆病要請(pcCharacter pchara, int turn, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
	CutInM().CutIn(pchara,臆病追加直前_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール))
	{

		pchara->Condition.臆病追加(turn);

		if(Messageflag && turn >= 0)
		{//メッセージを表示する
			map<tstring, StyleString> valiable;

			valiable[_T("Chara")] = pchara->ShortName();
		
			g_Langメッセージ(_T("臆病メッセージ"),valiable);
			
		}

		return true;
	}
	return false;
}
int cDungeonSystem::嫉妬要請(pcCharacter pchara, int turn, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
	CutInM().CutIn(pchara,嫉妬追加直前_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール))
	{

		pchara->Condition.嫉妬追加(turn);

		if(Messageflag && turn >= 0)
		{//メッセージを表示する
			map<tstring, StyleString> valiable;

			valiable[_T("Chara")] = pchara->ShortName();
		
			g_Langメッセージ(_T("嫉妬メッセージ"),valiable);
			
		}

		return true;
	}
	return false;
}
int cDungeonSystem::狂乱要請(pcCharacter pchara, int turn, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
	CutInM().CutIn(pchara,狂乱追加直前_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール))
	{

		pchara->Condition.狂乱追加(turn);

		if(Messageflag && turn >= 0)
		{//メッセージを表示する
			map<tstring, StyleString> valiable;

			valiable[_T("Chara")] = pchara->ShortName();
		
			g_Langメッセージ(_T("狂乱メッセージ"),valiable);
			
		}

		return true;
	}
	return false;
}
int cDungeonSystem::無意識要請(pcCharacter pchara, int turn, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
	CutInM().CutIn(pchara,無意識追加直前_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール))
	{

		pchara->Condition.無意識追加(turn);

		if(Messageflag && turn >= 0)
		{//メッセージを表示する
			map<tstring, StyleString> valiable;

			valiable[_T("Chara")] = pchara->ShortName();
		
			g_Langメッセージ(_T("無意識メッセージ"),valiable);
			
		}

		return true;
	}
	return false;
}
int cDungeonSystem::氷付け要請(pcCharacter pchara, int turn, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
	CutInM().CutIn(pchara,氷付け追加直前_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール))
	{

		pchara->Condition.氷付け追加(turn);

		if(Messageflag && turn >= 0)
		{//メッセージを表示する
			map<tstring, StyleString> valiable;

			valiable[_T("Chara")] = pchara->ShortName();
		
			g_Langメッセージ(_T("氷付けメッセージ"),valiable);
			
			EffectFunctions::氷付けエフェクト(pchara->placeX,pchara->placeY);
		}

		return true;
	}
	return false;
}
int cDungeonSystem::無敵要請(pcCharacter pchara, int turn, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	pchara->Condition.無敵追加(turn);

	if(Messageflag && turn >= 0)
	{//メッセージを表示する
		map<tstring, StyleString> valiable;

		valiable[_T("Chara")] = pchara->ShortName();
	
		g_Langメッセージ(_T("無敵メッセージ"),valiable);

		EffectFunctions::パワーアップエフェクト(pchara->placeX,pchara->placeY,55);
		
	}

	return true;

}
int cDungeonSystem::脱力要請(pcCharacter pchara, int power, int turn, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
	valf.doubles.dim(変数_強度) = power;
	valf.doubles.dim(変数_ターン) = turn;
	CutInM().CutIn(pchara,脱力追加直前_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール))
	{


		pchara->Condition.脱力追加(power,turn);

		if(Messageflag && power > 0)
		{//メッセージを表示する
			map<tstring, StyleString> valiable;

			valiable[_T("Chara")] = pchara->ShortName();
		
			g_Langメッセージ(_T("脱力メッセージ"),valiable);
		
			EffectFunctions::パワーダウンエフェクト(pchara->placeX,pchara->placeY,10);

		}

		return true;
	}
	return false;

}
int cDungeonSystem::元気要請(pcCharacter pchara, int power, int turn, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
	valf.doubles.dim(変数_強度) = power;
	valf.doubles.dim(変数_ターン) = turn;
	CutInM().CutIn(pchara,元気追加直前_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール))
	{

		pchara->Condition.元気追加(power,turn);

		if(Messageflag && power > 0)
		{//メッセージを表示する
			map<tstring, StyleString> valiable;

			valiable[_T("Chara")] = pchara->ShortName();
		
			g_Langメッセージ(_T("元気メッセージ"),valiable);
			
			EffectFunctions::パワーアップエフェクト(pchara->placeX,pchara->placeY,10);

		}

		return true;
	}
	return false;
}
int cDungeonSystem::軟弱要請(pcCharacter pchara, int power, int turn, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
	valf.doubles.dim(変数_強度) = power;
	valf.doubles.dim(変数_ターン) = turn;
	CutInM().CutIn(pchara,軟弱追加直前_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール))
	{
		pchara->Condition.軟弱追加(power,turn);

		if(Messageflag && power > 0)
		{//メッセージを表示する
			map<tstring, StyleString> valiable;

			valiable[_T("Chara")] = pchara->ShortName();
		
			g_Langメッセージ(_T("軟弱メッセージ"),valiable);
			
			EffectFunctions::パワーダウンエフェクト(pchara->placeX,pchara->placeY,260);
		}

		return true;
	}
	return false;
}
int cDungeonSystem::頑強要請(pcCharacter pchara, int power, int turn, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
	valf.doubles.dim(変数_強度) = power;
	valf.doubles.dim(変数_ターン) = turn;
	CutInM().CutIn(pchara,頑強追加直前_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール))
	{
		pchara->Condition.頑強追加(power,turn);

		if(Messageflag && power > 0)
		{//メッセージを表示する
			map<tstring, StyleString> valiable;

			valiable[_T("Chara")] = pchara->ShortName();
		
			g_Langメッセージ(_T("頑強メッセージ"),valiable);
		
			EffectFunctions::パワーアップエフェクト(pchara->placeX,pchara->placeY,260);
		}

		return true;
	}
	return false;
}
int cDungeonSystem::鳥目要請(pcCharacter pchara, int turn, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
	CutInM().CutIn(pchara,鳥目追加直前_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール))
	{
		pchara->Condition.鳥目追加(turn);

		if(Messageflag)
		{//メッセージを表示する
			map<tstring, StyleString> valiable;

			valiable[_T("Chara")] = pchara->ShortName();
		
			g_Langメッセージ(_T("鳥目メッセージ"),valiable);
			
		}

		return true;
	}
	return false;
}
int cDungeonSystem::健康要請(pcCharacter pchara, int turn, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
	CutInM().CutIn(pchara,健康追加直前_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール))
	{
		pchara->Condition.健康追加(turn);

		if(Messageflag)
		{//メッセージを表示する
			map<tstring, StyleString> valiable;

			valiable[_T("Chara")] = pchara->ShortName();
		
			g_Langメッセージ(_T("健康メッセージ"),valiable);
			
		}

		return true;
	}
	return false;
}
int cDungeonSystem::病気要請(pcCharacter pchara, int turn, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
	CutInM().CutIn(pchara,病気追加直前_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール))
	{
		pchara->Condition.病気追加(turn);

		if(Messageflag)
		{//メッセージを表示する
			map<tstring, StyleString> valiable;

			valiable[_T("Chara")] = pchara->ShortName();
		
			g_Langメッセージ(_T("病気メッセージ"),valiable);
			
		}

		return true;
	}
	return false;
}
int cDungeonSystem::死の誘い要請(pcCharacter pchara, pcCharacter subject, int turn, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
	CutInM().CutIn(pchara,死の誘い追加直前_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール))
	{
		if(!(pchara->雑魚属性()))//(pchara->Forse == CHARACTER_FORSE_FRIEND)
		{
			pchara->Condition.死の誘い追加(turn,subject);
		}
		else
		{//味方以外
			pchara->Condition.死の誘い追加(turn,NULLCHARA);
		}
		

		if(Messageflag)
		{//メッセージを表示する
			map<tstring, StyleString> valiable;

			valiable[_T("Chara")] = pchara->ShortName();
			valiable[_T("Turn")] = turn;
		
			g_Langメッセージ(_T("死の誘いメッセージ"),valiable);
			
		}

		return true;
	}
	return false;
}
int cDungeonSystem::身代わり要請(pcCharacter pchara, pcCharacter subject, int turn, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;


	pchara->Condition.みがわり追加(turn,subject);
	
	

	if(Messageflag)
	{//メッセージを表示する
		map<tstring, StyleString> valiable;

		valiable[_T("Chara")] = pchara->ShortName();
		valiable[_T("Turn")] = turn;
	
		g_Langメッセージ(_T("みがわりメッセージ"),valiable);
		
	}

	return true;

}
int cDungeonSystem::変身要請(pcCharacter pchara, pcCharacter subject, int turn, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;


	pchara->Condition.変身追加(turn,subject);
	
	

	if(Messageflag)
	{//メッセージを表示する
		map<tstring, StyleString> valiable;

		valiable[_T("Chara")] = pchara->ShortName();
		valiable[_T("Turn")] = turn;
	
		g_Langメッセージ(_T("変身メッセージ"),valiable);
		
	}

	return true;

}
int cDungeonSystem::擬態要請(pcCharacter pchara, int turn, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;


	pchara->Condition.擬態追加(turn);
	
	

	if(Messageflag)
	{//メッセージを表示する
		map<tstring, StyleString> valiable;

		valiable[_T("Chara")] = pchara->ShortName();
		
		g_Langメッセージ(_T("擬態メッセージ"),valiable);
		
	}

	return true;

}
int cDungeonSystem::精神異常治療要請(pcCharacter pchara, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	pchara->Condition.とても強い刺激();
	pchara->Condition.バクスイ追加(-1);
	pchara->Condition.眠り追加(-1);
	pchara->Condition.びっくり追加(-1);
	pchara->Condition.金縛り追加(-1);
	pchara->Condition.封印追加(-1);
	pchara->Condition.空振り追加(-1);
	pchara->Condition.貧乏追加(-1);
	pchara->Condition.臆病追加(-1);
	pchara->Condition.嫉妬追加(-1);
	pchara->Condition.狂乱追加(-1);
	pchara->Condition.無意識追加(-1);

	return true;
}
bool cDungeonSystem::精神異常状態(pcCharacter pchara)
{
	if(pchara == NULL || pchara->死亡()) return false;

	if(pchara->Condition.眠りで行動不能である()) return true;
	if(pchara->Condition.びっくりで行動不能である()) return true;
	if(pchara->Condition.金縛りで行動不能である()) return true;
	if(pchara->Condition.封印状態()) return true;
	if(pchara->Condition.空振り状態()) return true;
	if(pchara->Condition.貧乏状態()) return true;
	if(pchara->Condition.臆病状態()) return true;
	if(pchara->Condition.嫉妬状態()) return true;
	if(pchara->Condition.狂乱状態()) return true;
	if(pchara->Condition.無意識状態()) return true;

	return false;
}
int cDungeonSystem::身体異常治療要請(pcCharacter pchara, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	pchara->Condition.脱力初期化();
	pchara->Condition.軟弱初期化();

	pchara->Condition.泥酔追加(-1);
	pchara->Condition.氷付け追加(-1);
	pchara->Condition.鳥目追加(-1);
	pchara->Condition.病気追加(-1);
	pchara->Condition.健康追加(-1);
	return true;
}
bool cDungeonSystem::身体異常状態(pcCharacter pchara)
{
	if(pchara == NULL || pchara->死亡()) return false;

	if(pchara->Condition.力度数() != 0) return true;
	if(pchara->Condition.守度数() != 0) return true;

	if(pchara->Condition.泥酔状態()) return true;
	if(pchara->Condition.氷付け状態()) return true;
	if(pchara->Condition.鳥目状態()) return true;
	if(pchara->Condition.病気状態()) return true;
	if(pchara->Condition.健康状態()) return true;
	return false;
}
int cDungeonSystem::呪術異常治療要請(pcCharacter pchara, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	pchara->Condition.死の誘い追加(-1,NULLCHARA);
	pchara->Condition.みがわり追加(-1,NULLCHARA);
	pchara->Condition.擬態追加(-1);

	return true;
}
bool cDungeonSystem::呪術異常状態(pcCharacter pchara)
{
	if(pchara == NULL || pchara->死亡()) return false;

	if(pchara->Condition.死の誘い状態()) return true;
	if(pchara->Condition.みがわり状態()) return true;
	if(pchara->Condition.擬態状態()) return true;

	return false;
}
int cDungeonSystem::速度異常治療要請(pcCharacter pchara, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;
	int i;
	for(i=0;i<4;i++)
	{
		if(pchara->Condition.速度度数() > pchara->Condition.デフォルト速度度数())
		{
			速度減少要請(pchara,pchara->Condition.速度残りターン数());
		}
		if(pchara->Condition.速度度数() < pchara->Condition.デフォルト速度度数())
		{
			速度増加要請(pchara,pchara->Condition.速度残りターン数());
		}
		if(pchara->Condition.速度度数() == pchara->Condition.デフォルト速度度数())
		{
			break;
		}
	}
	return true;
}
bool cDungeonSystem::速度異常状態(pcCharacter pchara)
{
	if(pchara == NULL || pchara->死亡()) return false;

	if(pchara->Condition.速度度数() != pchara->Condition.デフォルト速度度数()) return true;
	return false;
}
int cDungeonSystem::悪性異常状態治療要請(pcCharacter pchara, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	pchara->Condition.とても強い刺激();
	pchara->Condition.バクスイ追加(-1);
	pchara->Condition.眠り追加(-1);
	pchara->Condition.びっくり追加(-1);
	pchara->Condition.金縛り追加(-1);
	pchara->Condition.封印追加(-1);
	pchara->Condition.空振り追加(-1);
	pchara->Condition.貧乏追加(-1);
	pchara->Condition.臆病追加(-1);
	//pchara->Condition.嫉妬追加(-1);
	pchara->Condition.狂乱追加(-1);
	pchara->Condition.無意識追加(-1);

	if(pchara->Condition.力度数() < 0) {
		pchara->Condition.脱力初期化();
	}
	if(pchara->Condition.守度数() < 0) {
		pchara->Condition.軟弱初期化();
	}

	pchara->Condition.泥酔追加(-1);
	pchara->Condition.氷付け追加(-1);
	pchara->Condition.鳥目追加(-1);
	pchara->Condition.病気追加(-1);
	//pchara->Condition.健康追加(-1);

	pchara->Condition.死の誘い追加(-1,NULLCHARA);
	pchara->Condition.みがわり追加(-1,NULLCHARA);
	//pchara->Condition.擬態追加(-1);

	if(pchara->Condition.速度度数() < pchara->Condition.デフォルト速度度数())
	{
		sg_pDungeonSystem->速度異常治療要請(pchara, false);
	}

	return true;
}
int cDungeonSystem::良性異常状態治療要請(pcCharacter pchara, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	pchara->Condition.嫉妬追加(-1);

	if(pchara->Condition.力度数() > 0) {
		pchara->Condition.脱力初期化();
	}
	if(pchara->Condition.守度数() > 0) {
		pchara->Condition.軟弱初期化();
	}

	pchara->Condition.健康追加(-1);

	pchara->Condition.擬態追加(-1);

	if(pchara->Condition.速度度数() > pchara->Condition.デフォルト速度度数())
	{
		sg_pDungeonSystem->速度異常治療要請(pchara, false);
	}

	return true;
}

int cDungeonSystem::全異常状態治療要請(pcCharacter pchara, int Messageflag)
{
	良性異常状態治療要請(pchara, Messageflag);
	悪性異常状態治療要請(pchara, Messageflag);
	return true;
}

int cDungeonSystem::やりすごし要請(pcCharacter pchara, int turn, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	呪術異常治療要請(pchara);

	pchara->Condition.やりすごし追加(turn);

	//キャラクター生成(3000,0,CHARACTER_FORSE_OTHER);
	return true;
}
int cDungeonSystem::目薬要請(pcCharacter pchara, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	pchara->Condition.目薬追加();

	if(Messageflag)
	{//メッセージを表示する
		map<tstring, StyleString> valiable;

		valiable[_T("Chara")] = pchara->ShortName();
	
		g_Langメッセージ(_T("目薬メッセージ"),valiable);
		
	}

	return true;
}

int cDungeonSystem::強制ダメージ要請(pcCharacter pchara, int damage, int Messageflag, int animationflag)
{
	if(pchara == NULL || pchara->死亡() || pchara->LastSpelling) return false;

	if(Messageflag && pchara == pPlayerChara())
	{//メッセージを表示する
		map<tstring, StyleString> valiable;

		//攻撃メッセージと共用
		valiable[_T("DeferName")] = pchara->ShortName();
		valiable[_T("Damege")] = setStyle(damage,DAMAGE_COLOR);
	
		g_Langメッセージ(_T("攻撃被ダメージメッセージ"),valiable);
		
		//メッセージ(pchara->ShortName() +_T(" は ")+setStyle(damage,DAMAGE_COLOR)+_T(" のダメージを受けた。\n"));
	}

	DamageCharacter(pchara,damage,animationflag);
	return true;
}

int cDungeonSystem::即死要請(pcCharacter pchara, int Messageflag, int animationflag)
{
	if(pchara == NULL || pchara->死亡() || pchara->LastSpelling) return false;

	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
	CutInM().CutIn(pchara,即死要請直前_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール))
	{
		if(pchara->雑魚属性())
		{
			if(Messageflag)
			{//メッセージを表示する
				map<tstring, StyleString> valiable;

				valiable[_T("Chara")] = pchara->ShortName();

				g_Langメッセージ(_T("即死メッセージ"),valiable);
				
			}

			DieCharacter(pchara, Messageflag);
		}
		else
		{
			if(Messageflag)
			{//メッセージを表示する
				map<tstring, StyleString> valiable;

				valiable[_T("Chara")] = pchara->ShortName();

				g_Langメッセージ(_T("半即死メッセージ"),valiable);
				
			}

			DamageCharacter(pchara,ceil(pchara->HP/2.0),animationflag);
		}
	}
	return true;
}

int cDungeonSystem::満腹度減少要請(pcCharacter pchara, double hunger, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	if(Messageflag)
	{
		sg_pDungeonSystem->AnimationManager().
			Anime_PlaySE(_T("hunger.wav"),pchara->足元地形()->place);

		if(pchara == pPlayerChara())
		{//メッセージを表示する
			map<tstring, StyleString> valiable;

			valiable[_T("Value")] = setStyle((int)hunger,HUNGER_COLOR);
			valiable[_T("Chara")] = pchara->ShortName();
		
			g_Langメッセージ(_T("満腹度減少メッセージ"),valiable);
			
			//メッセージ(pchara->ShortName() +_T(" の満腹度が ")+setStyle(hunger,HUNGER_COLOR)+_T(" 減少した。\n"));
		}
	}
	hungerCharacter(pchara,hunger);
	return true;
}

int cDungeonSystem::経験値獲得(pcCharacter pchara, int point, int Messageflag)
{
	if(pchara == NULL) return false;

	return EXTcharge(pchara,point,Messageflag);

}
int cDungeonSystem::レベルダウン(pcCharacter pchara, int downlv, int Messageflag)
{
	if(pchara == NULL) return false;

	return leveldownCharacter(pchara, downlv);
}
int cDungeonSystem::レベルアップ(pcCharacter pchara, int uplv, int Messageflag)
{
	if(pchara == NULL) return false;

	return levelupCharacter(pchara, uplv);
}

int cDungeonSystem::強制ひるみ要請(pcCharacter pchara)
{//強制的に（実質）等速以下にする。
	if(pchara == NULL) return false;

	return pchara->TurnEndFlag_half = 1;
}

int cDungeonSystem::強制ターンエンド要請(pcCharacter pchara)
{
	if(pchara == NULL) return false;

	return pchara->TurnEndFlag = 1;
}
int cDungeonSystem::全キャラ強制ターンエンド要請()
{

	int i;
	for(i=0;i<CharaList().size();i++)
	{
		強制ターンエンド要請(CharaList()[i]);
	}
	return true;
}
//経験値を得る
int cDungeonSystem::EXTcharge(pcAttackinformation pattackinfo)
{
	
	cValiableField valf;
	valf.doubles.dim(変数_汎用実数) = pattackinfo->defenser->HaveEXP();//
	valf.charas.dim(変数_攻撃者) = pattackinfo->attacker;
	valf.charas.dim(変数_防御者) = pattackinfo->defenser;
	valf.doubles.dim(変数_汎用ブール) = (double)(pattackinfo->attacker != NULL && pattackinfo->defenser != pattackinfo->attacker);
	CutInM().CutIn(pPlayerChara(),経験値獲得判定時_タイミング,valf);
	
	if( valf.doubles.val(変数_汎用ブール) != 0.0)
	{

		if(pattackinfo->defenser->HaveEXP() <= 0)
		{
			return true;
		}

		double ext = valf.doubles.val(変数_汎用実数);
		
		if(valf.charas.dim(変数_攻撃者)->LV - 10 >= valf.charas.dim(変数_防御者)->LV){
			ext /= 4;
		}
		else if(valf.charas.dim(変数_攻撃者)->LV - 5 >= valf.charas.dim(変数_防御者)->LV){
			ext /= 2;
		}

		ext = ceil(ext);
		if (ext < 1) {
			ext = 1;
		}

		//自分の場合メッセージが出る。
		EXTcharge(valf.charas.val(変数_攻撃者), ext, (valf.charas.val(変数_攻撃者) == pPlayerChara()));
		//EXTcharge(pattackinfo->attacker, pattackinfo->defenser->HaveEXP(), (pattackinfo->attacker == pPlayerChara()));
		return true;
	}
	return false;
}
//経験値を得る
int cDungeonSystem::EXTcharge(pcCharacter pchara, int point, int Messageflag)
{
	if(pchara == NULL) return false;
	
	cValiableField valf;
	valf.doubles.dim(変数_汎用ボーナス_倍率) = 1.0;//
	valf.doubles.dim(変数_汎用実数) = point;//
	CutInM().CutIn(pchara,経験値獲得時_タイミング,valf);

	point = valf.doubles.val(変数_汎用実数) * valf.doubles.val(変数_汎用ボーナス_倍率);
	
	if(Messageflag && point > 0)
	{//メッセージ

		map<tstring, StyleString> valiable;

		valiable[_T("Value")] = setStyle(point,EXP_COLOR);
		valiable[_T("Chara")] = pchara->ShortName();
	
		g_Langメッセージ(_T("経験値獲得メッセージ"),valiable);
		
		////if(pattackinfo->attacker != pPlayerChara())
		//{//自分以外の場合は誰が得たか明記使用と思ったけど止めた。
		//	m = m + pattackinfo->attacker->ShortName()+_T("は ");
		//}
		//m = m +setStyle(ext,EXP_COLOR)+_T(" ポイントの ")+setStyle(_T("経験値")/*,EXP_COLOR*/)+_T(" を得た。\n");
		//メッセージ(m);
		

	}



	int lvup = pchara->addEXP(point);//経験値取得

	if(lvup)
	{//レベルアップ
		levelupCharacter(pchara, lvup);
	}

	return true;
}

//レベルアップ。
int cDungeonSystem::levelupCharacter(pcCharacter pchara, int uplv)
{
	if(pchara == NULL) return false;

	StyleString m;


	map<tstring, StyleString> valiable;


	if(!pchara->雑魚属性())
	{
		int i;
		int flag = 0;
		for(i=0;i<uplv;i++)
		{
			flag |= pchara->LvUP();
		}

		if(!flag) return false;

		
		valiable[_T("LevelStr")] = setStyle(pchara->LVStr(),LV_COLOR);
		valiable[_T("Level")] = setStyle(pchara->LV,LV_COLOR);
		valiable[_T("Chara")] = pchara->ShortName();
		g_Langメッセージ(_T("レベルアップメッセージ"),valiable);

	}
	else
	{
		valiable[_T("Chara")] = pchara->FullName();
		int i;
		int flag = 0;
		for(i=0;i<uplv;i++)
		{
			flag |= pchara->LvUP();
		}

		if(!flag) return false;

		valiable[_T("NewChara")] = pchara->FullName();
		valiable[_T("LevelStr")] = setStyle(pchara->LVStr(),LV_COLOR);
		g_Langメッセージ(_T("レベルアップメッセージ敵"),valiable);

	}
	
	if(pchara->Forse == CHARACTER_FORSE_FRIEND)
	{
		AnimationManager().Anime_PlaySE(_T("LvUP2.wav"),pchara->足元地形()->place);
	}

	/*
	if(pchara->Forse == CHARACTER_FORSE_FRIEND)
	{//味方の場合
		m = m + pchara->ShortName()+_T("は ");
		m = m +setStyle(_T("レベル "),LV_COLOR)+setStyle(lv,LV_COLOR)+_T(" に 上がった！\n");
		メッセージ(m);
	}
	else
	{
		m = m + pchara->FullName()+_T("は ");
		m = m +setStyle(_T("パワー "),LV_COLOR)+setStyle(lv,LV_COLOR)+_T(" に 上がった！\n");
		メッセージ(m);
	}
	*/

	if(pPlayerChara() == pchara)
	{//インターフェイス用

		AnimationManager().Anime_DisplayChange(
			&GameScreenInterface.disphero_LV,
			pchara->LV);

		//インターフェイス用
		AnimationManager().Anime_DisplayChange(
			&GameScreenInterface.disphero_HP,
			pchara->HP);
		AnimationManager().Anime_DisplayChange(
			&GameScreenInterface.disphero_MHP,
			pchara->MHP);
	}

	AnimationManager().Anime_DisplayChange(
		&pchara->VisibleCLASS,
		pchara->CLASS);


	return true;
}
//レベルダウン。
int cDungeonSystem::leveldownCharacter(pcCharacter pchara, int downlv)
{
	if(pchara == NULL) return false;

	StyleString m;


	map<tstring, StyleString> valiable;
	

	if(!pchara->雑魚属性())
	{
		int i;
		int flag = 0;
		for(i=0;i<downlv;i++)
		{
			flag |= pchara->LvDown();
		}

		if(!flag) return false;
		
		valiable[_T("LevelStr")] = setStyle(pchara->LVStr(),LV_COLOR);
		valiable[_T("Level")] = setStyle(pchara->LV,LV_COLOR);
		valiable[_T("Chara")] = pchara->ShortName();
		g_Langメッセージ(_T("レベルダウンメッセージ"),valiable);

	}
	else
	{
		valiable[_T("Chara")] = pchara->FullName();
		int i;
		int flag = 0;
		for(i=0;i<downlv;i++)
		{
			flag |= pchara->LvDown();
		}

		if(!flag) return false;

		valiable[_T("NewChara")] = pchara->FullName();
		valiable[_T("LevelStr")] = setStyle(pchara->LVStr(),LV_COLOR);
		g_Langメッセージ(_T("レベルダウンメッセージ敵"),valiable);

	}



	if(pPlayerChara() == pchara)
	{//インターフェイス用
		AnimationManager().Anime_DisplayChange(
			&GameScreenInterface.disphero_LV,
			pchara->LV);

		//インターフェイス用
		AnimationManager().Anime_DisplayChange(
			&GameScreenInterface.disphero_HP,
			pchara->HP);
		AnimationManager().Anime_DisplayChange(
			&GameScreenInterface.disphero_MHP,
			pchara->MHP);
	}

	AnimationManager().Anime_DisplayChange(
		&pchara->VisibleCLASS,
		pchara->CLASS);


	return true;
}



//ダメージ処理
int cDungeonSystem::DamageCharacter(pcCharacter pchara, int damage, int animationflag, double effective)
{
	if(pchara == NULL) return false;
	if(pchara->死亡()) return false;//ダメージを与えるまもなく死亡している
	if(pchara->LastSpelling) return false;//ラストスペル中

	pchara->HP -= damage;
	pchara->Condition.damagedHP();

	if(pchara->HP < 0)
	{
		pchara->HP = 0;
	}

	if(pPlayerChara() == pchara)
	{//インターフェイス用
		AnimationManager().Anime_DisplayChange(
			&GameScreenInterface.disphero_HP,
			pchara->HP);
	}

	if(animationflag)
	{
		AnimationManager().Anime_Damagecharacter(pchara,damage,effective);
	}

	//死亡
	if(pchara->死亡())
	{
		DieCharacter(pchara);

	}

	return true;
}



//死亡処理
int cDungeonSystem::DieCharacter(pcCharacter pchara, int Messageflag)
{
	if(pchara == NULL) return false;

	StyleString m;

	cValiableField val;
	val.doubles.dim(変数_死亡回避) = 0;
	CutInM().CutIn(pchara,死亡直前_ラストスペル_タイミング,val);//死亡直前


	if(val.doubles[変数_死亡回避] != 0) return false;//死亡回避


	val.doubles[変数_死亡回避] = 0;
	CutInM().CutIn(pchara,死亡直前_タイミング,val);//死亡直前

	if(val.doubles[変数_死亡回避] != 0) return false;//死亡回避


	pchara->HP = 0;
		
	if(pPlayerChara() == pchara)
	{//インターフェイス用
		AnimationManager().Anime_DisplayChange(
			&GameScreenInterface.disphero_HP,
			pchara->HP);
	}
	
	if(!pchara->CharaAttribute().count(キャラ属性::弾幕))
	{
			
		//死亡アニメ
		AnimationManager().Anime_Retirecharacter(pchara);
	}
	else
	{
		AnimationManager().Anime_DisplayChange(
		&pchara->opaque,
		0);
	}

	if(Messageflag)
	{
		map<tstring, StyleString> valiable;
		valiable[_T("Chara")] = pchara->FullName();

		if(!pchara->CharaAttribute().count(キャラ属性::弾幕))
		{
			
		

			if(pchara->Forse == CHARACTER_FORSE_FRIEND)
			{//やられたのが味方の場合

				valiable[_T("Dead")] = setStyle(g_Lang(_T("表現_力尽きた")).conclete_tstr(),DENGER_COLOR);
			
				g_Langメッセージ(_T("死亡メッセージ_味方"),valiable);
				
				//m = m + pchara->FullName()+_T("は ") + setStyle(_T("力尽きた・・・\n"),DENGER_COLOR);
			
				AnimationManager().Anime_MessageWait(false);
			}
			else if(pchara->Forse == CHARACTER_FORSE_ENEMY)
			{
					
				g_Langメッセージ(_T("死亡メッセージ_敵"),valiable);
				
				//m = m + pchara->FullName()+_T("を やっつけた！\n");
				//AnimationManager().Anime_MessageWait(true);

				if(pchara->DefeatCountable())
				{
					SumEnemyDefeatNum()++;
				}
			}
			else
			{
				g_Langメッセージ(_T("死亡メッセージ_その他"),valiable);
				
				//m = m + pchara->FullName()+_T("は たおされた。\n");
				AnimationManager().Anime_MessageWait(true);
			}
			//メッセージ(m);
		}
	}

	if(pchara != pPlayerChara())
	{
		//死亡アイテムドロップ

		vector<int> IDs = pchara->死亡ドロップアイテムIDs();
		int index, size = IDs.size();
		for(index = 0; index < size; index++ ) {
			int id = IDs[index];
			pcDroping pdrop = 落ち物生成_設置なし(id);
			遠隔拾得要請(pchara,pdrop);
		}

		ころび要請(pchara,pchara->holdItem.size(),1,0);
	}

	//キャラ消去
	removecharacter(pchara);

	return true;
}
int cDungeonSystem::強制退場要請(pcCharacter pchara, int Messageflag, int animationflag)
{

	if(pchara != pPlayerChara())
	{
		//死亡アイテムドロップ

		vector<int> IDs = pchara->死亡ドロップアイテムIDs();
		int index, size = IDs.size();
		for(index = 0; index < size; index++ ) {
			int id = IDs[index];
			pcDroping pdrop = 落ち物生成_設置なし(id);
			遠隔拾得要請(pchara,pdrop);
		}

		ころび要請(pchara,pchara->holdItem.size(),1,0);
	}
	//キャラ消去
	removecharacter(pchara);
	AnimationManager().Anime_DisplayChange(
			&pchara->opaque,
			0);
	return true;
}


//満腹度減少処理
int cDungeonSystem::hungerCharacter(pcCharacter pchara, double hunger)
{
	if(pchara == NULL) return false;

	pchara->Stomach -= hunger;

	if(pchara->Stomach < 0)
	{
		pchara->Stomach = 0;
	}


	if(pPlayerChara() == pchara)
	{//インターフェイス用
		AnimationManager().Anime_DisplayChange(
			&GameScreenInterface.disphero_Stomach,
			pchara->Stomach);
	}

	return true;
}



int cDungeonSystem::RecoverCharacter(pcCharacter pchara, int recovery, int animationflag)
{
	if(pchara == NULL) return false;

	pchara->HP += recovery;
	if(pchara->MHP < pchara->HP) pchara->HP = pchara->MHP;

	if(animationflag)
	{
		if(pchara->Forse != CHARACTER_FORSE_ENEMY ||
			(sg_pDungeonSystem->キャラクター目視可(pchara)))
		{
			AnimationManager().Anime_Recovercharacter(pchara,recovery);
		}
	}

	if(pPlayerChara() == pchara)
	{//インターフェイス用
		AnimationManager().Anime_DisplayChange(
			&GameScreenInterface.disphero_HP,
			pchara->HP);
	}

	return true;
}

//最大HP拡張処理
int cDungeonSystem::HPextentionCharacter(pcCharacter pchara, int extention)
{
	if(pchara == NULL) return false;

	pchara->HP += extention;
	pchara->MHP += extention;

	if(pchara->MHP < 1)
	{
		pchara->MHP = 1;
	}
	else if(pchara->MHP > pchara->MaxMHP())
	{
		pchara->MHP = pchara->MaxMHP();
	}

	if(pchara->HP > pchara->MHP)
	{
		pchara->HP = pchara->MHP;
	}

	if(pPlayerChara() == pchara)
	{//インターフェイス用
		AnimationManager().Anime_DisplayChange(
			&GameScreenInterface.disphero_HP,
			pchara->HP);
		AnimationManager().Anime_DisplayChange(
			&GameScreenInterface.disphero_MHP,
			pchara->MHP);
	}


	return true;
}
//最大HP縮小処理
int cDungeonSystem::HPreductionCharacter(pcCharacter pchara, int reduction)
{
	if(pchara == NULL) return false;

	pchara->MHP -= reduction;

	if(pchara->MHP < 1)
	{
		pchara->MHP = 1;
	}
	else if(pchara->MHP > pchara->MaxMHP())
	{
		pchara->MHP = pchara->MaxMHP();
	}

	if(pchara->HP > pchara->MHP)
	{
		pchara->HP = pchara->MHP;
	}

	if(pPlayerChara() == pchara)
	{//インターフェイス用
		AnimationManager().Anime_DisplayChange(
			&GameScreenInterface.disphero_HP,
			pchara->HP);
		AnimationManager().Anime_DisplayChange(
			&GameScreenInterface.disphero_MHP,
			pchara->MHP);
	}


	return true;
}
//満腹度回復処理
int cDungeonSystem::StmRecoverCharacter(pcCharacter pchara, int recovery)
{
	if(pchara == NULL) return false;

	pchara->Stomach += recovery;
	if(pchara->MaxStomach < pchara->Stomach) pchara->Stomach = pchara->MaxStomach;


	if(pPlayerChara() == pchara)
	{//インターフェイス用
		AnimationManager().Anime_DisplayChange(
			&GameScreenInterface.disphero_Stomach,
			pchara->Stomach);
	}

	return true;
}

//最大満腹度拡張処理
int cDungeonSystem::StmextentionCharacter(pcCharacter pchara, int extention)
{
	if(pchara == NULL) return false;

	pchara->Stomach += extention;
	pchara->MaxStomach += extention;

	if(pchara->MaxStomach > pchara->MaxMaxStomach())
	{
		pchara->MaxStomach = pchara->MaxMaxStomach();
		if(pchara->Stomach > pchara->MaxStomach)
		{
			pchara->Stomach = pchara->MaxStomach;
		}
	}

	if(pchara->MaxStomach < 1)
	{
		pchara->MaxStomach = 1;
		if(pchara->Stomach > pchara->MaxStomach)
		{
			pchara->Stomach = pchara->MaxStomach;
		}
	}

	if(pPlayerChara() == pchara)
	{//インターフェイス用
		AnimationManager().Anime_DisplayChange(
			&GameScreenInterface.disphero_Stomach,
			pchara->Stomach);
		AnimationManager().Anime_DisplayChange(
			&GameScreenInterface.disphero_MaxStomach,
			pchara->MaxStomach);
	}


	return true;
}

