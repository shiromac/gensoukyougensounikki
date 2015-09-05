#include "stdafx.h"
#include "Drink1.h"
#include "GameIdiom.h"
#include "cDungeonSystem.h"
#include "cAttackinformation.h"
#include "gameMainSystem/filemanage/cScriptRLayer.h"
#include "cFloor.h"
#include "cDropingDistribution.h"

bool cDrink_能力仕様フラグID_exist(cValiableField& valiable, int ID)
{
	return valiable.doubles.exist((_T("cDrink_能力仕様フラグID_") + setStyle(ID)).c_str());
}
double& cDrink_能力仕様フラグID_dim(cValiableField& valiable, int ID)
{
	return valiable.doubles.dim((_T("cDrink_能力仕様フラグID_") + setStyle(ID)).c_str());
}
double& cDrink_能力仕様フラグID_val(cValiableField& valiable, int ID)
{
	return valiable.doubles.val((_T("cDrink_能力仕様フラグID_") + setStyle(ID)).c_str());
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//水道水
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cDrink_ID_0::効果(pcCharacter pchara, int recover, int extend)
{
	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("recover.wav"),pchara->足元地形()->place);

	if(pchara->HP == pchara->MHP)
	{
		sg_pDungeonSystem->最大HP拡張要請(pchara,extend);	
	}
	else
	{
		sg_pDungeonSystem->回復要請(pchara,recover);	
	}

	return true;
}
int cDrink_ID_0::飲む_効果_通常(pcCharacter pchara)
{
	return 効果(pchara
		, 効果量(0)//回復量
		, 効果量(1)//HP拡張量
		);
}
/*
int cDrink_ID_0::飲む_効果_祝福(pcCharacter pchara)
{
	return 飲む_効果_通常(pchara);
}
*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//神主殺し
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cDrink_ID_1::効果(pcCharacter pchara, pcLandform pland, int power)
{
	return GameIdiom::燃え(pchara, pland, power);
}
int cDrink_ID_1::飲む_効果_通常(pcCharacter pchara)
{
	return 効果(pchara
		, sg_pDungeonSystem->キャラ前地形(pchara,1)//地形
		, random_range(効果量(0),効果量(1))//威力
		);
}
int cDrink_ID_1::飲む_メッセージ_通常(pcCharacter pchara)
{
	飲むメッセージ定型(pchara);

	map<tstring, StyleString> valiable;
	valiable[_T("Chara")] = pchara->ShortName();
	g_Langメッセージ(_T("cDrink_ID_1効果メッセージ"),valiable);


	//sg_pDungeonSystem->メッセージ(
	//	pchara->ShortName() + _T(" は火を噴いた！\n"));

	return true;
}
/*
int cDrink_ID_1::飲む_効果_祝福(pcCharacter pchara)
{
	return 効果(pchara
		, sg_pDungeonSystem->キャラ前地形(pchara,1)//地形
		, random_range(150,179)*効果量係数()//威力
		);
	return true;
}
*/
int cDrink_ID_1::衝突時効果(pcCharacter pchara)
{
	int power;
	/*
	if(state() == cDroping::STATE_NORMAL)
	{
		power = random_range(40,49)*効果量係数();
	}
	else if(state() == cDroping::STATE_GOOD)
	{
		power = random_range(80,99)*効果量係数();
	}
	else
	{
		power = 1;
	}
	*/
	power = random_range(効果量(2),効果量(3));
	return 効果(投擲者()
		, sg_pDungeonSystem->キャラ足元地形(pchara)//地形
		, power//威力
		);
}
/*
int cDrink_ID_1::衝突時_メッセージ(pcCharacter pchara)
{
	if(state() == cDroping::STATE_CURSE)
	{//呪い
		飲む_メッセージ_呪い(pchara);
	}
	else
	{
		map<tstring, StyleString> valiable;
		valiable[_T("Chara")] = pchara->ShortName();
		valiable[_T("Item")] = FullName();
		g_Langメッセージ(_T("Drink衝突時ばら撒きメッセージ"),valiable);

		//sg_pDungeonSystem->メッセージ( pchara->FullName()+_T("は ")+
		//	FullName() + _T(" を頭からかぶった！\n"));
	}
	return true;
}
*/

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//脱力水
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cDrink_ID_2::効果(pcCharacter pchara, int turn)
{
	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("slowdown.wav"),pchara->足元地形()->place);

	sg_pDungeonSystem->速度減少要請(pchara, turn);
		

	return true;
}
int cDrink_ID_2::飲む_効果_通常(pcCharacter pchara)
{
	return 効果(pchara
		, 効果量(0)//ターン数
		);
}
int cDrink_ID_2::飲む_効果_祝福(pcCharacter pchara)
{
	return (効果(pchara
		, 効果量(0)//ターン数
		),
			効果(pchara
		, 効果量(0)//ターン数
		));
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//天狗水
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cDrink_ID_3::効果(pcCharacter pchara, int turn)
{
	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("speedup.wav"),pchara->足元地形()->place);

	sg_pDungeonSystem->速度増加要請(pchara, turn);
		

	return true;
}
int cDrink_ID_3::飲む_効果_通常(pcCharacter pchara)
{
	return 効果(pchara
		, 効果量(0)//回復量
		);
}
int cDrink_ID_3::飲む_効果_祝福(pcCharacter pchara)
{
	return (効果(pchara
		, 効果量(0)//回復量
		),
			効果(pchara
		, 効果量(0)//回復量
		));
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//眠り薬
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cDrink_ID_4::効果(pcCharacter pchara, int turn)
{
	//効果音未実装

	sg_pDungeonSystem->眠り要請(pchara, turn);
		

	return true;
}
int cDrink_ID_4::飲む_効果_通常(pcCharacter pchara)
{
	return 効果(pchara
		, 効果量(0)//ターン
		);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//回復中
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void cDrink_ID_6::能力(const タイミング timing, cValiableField& valiable)
{
	if(timing == 被攻撃直後時_タイミング)
	{
		if(能力発動条件満たしている_防御用() && !cDrink_能力仕様フラグID_exist(valiable,ID())
			&& !装備者_防御用()->死亡())
		{
			sg_pDungeonSystem->回復要請(装備者_防御用(),
				効果量(2),false);
			
			cDrink_能力仕様フラグID_dim(valiable,ID()) = 1;
		}
	}
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//回復大
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cDrink_ID_7::効果(pcCharacter pchara, double recoverper)
{
	if(pchara == NULL) return false;

	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("recover.wav"),pchara->足元地形()->place);


	sg_pDungeonSystem->回復要請(pchara,pchara->MHP*recoverper/100.0);	
	

	return true;
}
int cDrink_ID_7::飲む_効果_通常(pcCharacter pchara)
{
	return 効果(pchara
		, 効果量(0)//回復%
		);
}
void cDrink_ID_7::能力(const タイミング timing, cValiableField& valiable)
{
	if(timing == 攻撃直後時_タイミング)
	{
		if(能力発動条件満たしている_攻撃用() && !cDrink_能力仕様フラグID_exist(valiable,ID()))
		{
			sg_pDungeonSystem->回復要請(装備者_攻撃用(),
				valiable.doubles.val(変数_ダメージ)*効果量(1)/100.0,false);
			
			cDrink_能力仕様フラグID_dim(valiable,ID()) = 1;
		}
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//空
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int cDrink_ID_8::衝突時効果(pcCharacter pchara)
{
	multiset<攻撃属性::攻撃属性> 属性;
	属性.insert(攻撃属性::落ち物);
	if(跳ね返し無効フラグ)属性.insert(攻撃属性::跳ね返し無効);
	sg_pDungeonSystem->攻撃接近(攻撃作成(
		投擲者(),//攻撃者
		pchara->足元地形(),//場所
		1,//攻撃力
		true,//定数ダメージ
		属性,
		true
		));

	return true;
}
int cDrink_ID_8::GetmenuCaption(vector<tstring> &CaptionList)
{
	cDrink::GetmenuCaption(CaptionList);

	pcCharacter pchara;
	if(落ちてる())
	{
		pchara = 乗りキャラ();
	}
	else
	{
		pchara = Holder();
	}

	int i,index = -1;
	for(i=0;i<CaptionList.size();i++)
	{
		if(CaptionList[i] == _T("飲む"))
		{
			index = i;
		}
	}//飲むを探す

	
	if(index == -1) return true;

	//飲むを消す
	if((pchara != NULL) && sg_pDungeonSystem->キャラ前地形(pchara,1)->driable())
	{
		CaptionList[index] = _T("汲む");
	}
	else
	{
		CaptionList.erase(CaptionList.begin()+index);//飲むを消す
	}

	CaptionList.insert((CaptionList.begin()+index),_T("カクテル"));


	return true;
}
void cDrink_ID_8::AppreciationBeginOptimize(int difficulty)
{
	cDrink::AppreciationBeginOptimize(difficulty);
	sg_pDungeonSystem->識別(me(),0);
}
int cDrink_ID_8::汲む_効果_通常(pcCharacter pchara, pcDroping& pdrop)
{
	if(効果量(0) > random()*100)
	{
		cDropingDistribution iDP = sg_pDungeonSystem->pFloor()->dropitemDP();

		int IDgroup = me()->ID()/1000;
		int i;
		int dp[10];
		int sum = 0;
		for(i=0;i<10;i++)
		{
			dp[i] = (IDgroup == i+1);
			sum += dp[i];
		}
		iDP.changeItemGroupPos(
			dp[0],
			dp[1],
			dp[2],
			dp[3],
			dp[4],
			dp[5],
			dp[6],
			dp[7],
			dp[8]
			);

		int id = iDP.get(random());

		pdrop = sg_pDungeonSystem->落ち物生成_設置なし(id);
		pcDrink pcDrk = boost::dynamic_pointer_cast<cDrink>(pdrop);
		if(pcDrk != NULL)
		{
			pcDrk->reuse()++;
			return true;
		}
	}

	pdrop = sg_pDungeonSystem->落ち物生成_設置なし(5009);
	pcDrink pcDrk = boost::dynamic_pointer_cast<cDrink>(pdrop);
	if(pcDrk != NULL)
	{
		pcDrk->reuse()++;
		return true;
	}
	return false;
}
int cDrink_ID_8::飲む_効果_通常(pcCharacter pchara)
{
	
	map<tstring, StyleString> valiable;
	g_Langメッセージ(_T("cDrink_ID_8効果メッセージ"),valiable);


	if(pchara == NULL) return false;
	return true;
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//生水
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void cDrink_ID_9::AppreciationBeginOptimize(int difficulty)
{
	cDrink::AppreciationBeginOptimize(difficulty);
	sg_pDungeonSystem->識別(me(),0);
}
int cDrink_ID_9::効果(pcCharacter pchara, int power)
{
	//効果音未実装

	sg_pDungeonSystem->満腹度減少要請(pchara, power);
		
	return true;
}
int cDrink_ID_9::飲む_効果_通常(pcCharacter pchara)
{
	return 効果(pchara
		, 効果量(0)
		);
}
int cDrink_ID_9::衝突時効果(pcCharacter pchara)
{
	multiset<攻撃属性::攻撃属性> 属性;
	属性.insert(攻撃属性::水);
	属性.insert(攻撃属性::水);
	属性.insert(攻撃属性::水);//三倍

	sg_pDungeonSystem->攻撃接近(攻撃作成(
		投擲者(),//攻撃者
		pchara->足元地形(),//場所
		1,//攻撃力
		true,//定数ダメージ
		属性,
		true
		));
	return true;
}
void cDrink_ID_9::CutIn(タイミング timing, cValiableField& valiable)
{
	cDrink::CutIn(timing, valiable);

	if(timing == 被攻撃接近直後_タイミング)
	{
		if(valiable.intsets[変数_属性].count(攻撃属性::火))
		{
			加熱();
		}
	}
	else if(timing == フロア終了_タイミング)
	{
		if(Parent() == NULL)
		{
			冷却();
		}
	}
}
void cDrink_ID_9::冷却()
{
	temperature() = 0;
}

void cDrink_ID_9::加熱()
{
	//if(temperature() == 1)
	{
		pcDroping pdrop = sg_pDungeonSystem->落ち物生成_設置なし(5011);
		pcDrink pdrink = boost::dynamic_pointer_cast<cDrink>(pdrop);
		if(pdrink != NULL) pdrink->reuse() = reuse();
		sg_pDungeonSystem->落ち物変化(me(),pdrop);
	}
	//temperature() = 1;
}
tstring cDrink_ID_9::FullNameBase()
{
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.DropImportLanguage(ID(),
		(tstring)_T("FullNameBase"));
	if(psr == NULL || psr->script().empty()) return NoDataString();

	int index = (temperature() == 1);
	index = min(index,psr->script().size()-1);
	return psr->script()[index];
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//水
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cDrink_ID_10::効果(pcCharacter pchara, int power)
{
	//効果音未実装

	sg_pDungeonSystem->満腹度回復要請(pchara, power);
		
	return true;
}
int cDrink_ID_10::飲む_効果_祝福(pcCharacter pchara)
{
	//効果音未実装

	sg_pDungeonSystem->アイテム祝福(pchara->attackequipment);
	sg_pDungeonSystem->アイテム祝福(pchara->defenseequipment);

	return true;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//熱湯
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void cDrink_ID_11::AppreciationBeginOptimize(int difficulty)
{
	cDrink::AppreciationBeginOptimize(difficulty);
	sg_pDungeonSystem->識別(me(),0);
}
int cDrink_ID_11::効果(pcCharacter pchara, int power)
{
	//効果音未実装

	multiset<攻撃属性::攻撃属性> 属性;
	属性.insert(攻撃属性::水);
	属性.insert(攻撃属性::水);
	属性.insert(攻撃属性::水);//三倍
	属性.insert(攻撃属性::火);
	属性.insert(攻撃属性::火);
	属性.insert(攻撃属性::火);//三倍

	sg_pDungeonSystem->攻撃接近(攻撃作成(
		NULLCHARA,//攻撃者
		pchara->足元地形(),//場所
		power,//攻撃力
		true,//定数ダメージ
		属性,
		true
		));

	return true;
}
int cDrink_ID_11::飲む_効果_通常(pcCharacter pchara)
{
	return 効果(pchara
		, 効果量(0)//ターン
		);
}
int cDrink_ID_11::衝突時効果(pcCharacter pchara)
{
	multiset<攻撃属性::攻撃属性> 属性;
	属性.insert(攻撃属性::水);
	属性.insert(攻撃属性::火);

	sg_pDungeonSystem->攻撃接近(攻撃作成(
		投擲者(),//攻撃者
		pchara->足元地形(),//場所
		効果量(0),//攻撃力
		true,//定数ダメージ
		属性,
		true
		));
	return true;
}
void cDrink_ID_11::CutIn(タイミング timing, cValiableField& valiable)
{
	cDrink::CutIn(timing, valiable);

	if(timing == 被攻撃接近直後_タイミング)
	{
		if(valiable.intsets[変数_属性].count(攻撃属性::火))
		{
			加熱();
		}
	}
	
	else if(timing == フロア終了_タイミング)
	{
		if(Parent() == NULL)
		{
			冷却();
		}
	}
	
}
void cDrink_ID_11::冷却()
{
	pcDroping pdrop = sg_pDungeonSystem->落ち物生成_設置なし(5010);
	pcDrink pdrink = boost::dynamic_pointer_cast<cDrink>(pdrop);
	if(pdrink != NULL) pdrink->reuse() = reuse();
	sg_pDungeonSystem->落ち物変化(me(),pdrop);
}

void cDrink_ID_11::加熱()
{

}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//酒精
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cDrink_ID_12::効果(pcCharacter pchara, int turn)
{
	
	sg_pDungeonSystem->泥酔要請(pchara,turn);

	return true;
}
int cDrink_ID_12::飲む_効果_通常(pcCharacter pchara)
{
	return 効果(pchara
		, 効果量(0)//ターン
		);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//レベルアップする酒（仮名）
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cDrink_ID_13::効果(pcCharacter pchara, int power)
{
	
	sg_pDungeonSystem->レベルアップ(pchara,power);

	return true;
}
int cDrink_ID_13::飲む_効果_通常(pcCharacter pchara)
{
	return 効果(pchara
		, 効果量(0)//ターン
		);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//レベルダウンする酒（仮名）
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cDrink_ID_14::効果(pcCharacter pchara, int power)
{
	//効果音未実装
	sg_pDungeonSystem->レベルダウン(pchara,power);

	return true;
}
int cDrink_ID_14::飲む_効果_通常(pcCharacter pchara)
{
	return 効果(pchara
		, 効果量(0)//ターン
		);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//高飛びする酒（仮名）
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cDrink_ID_15::効果(pcCharacter pchara)
{
	//効果音未実装
	GameIdiom::高飛び(pchara, pchara->足元地形());

	return true;
}
int cDrink_ID_15::飲む_効果_通常(pcCharacter pchara)
{
	return 効果(pchara
		);
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//胃拡張（仮名）
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cDrink_ID_18::効果(pcCharacter pchara, int power)
{
	//効果音未実装
	sg_pDungeonSystem->最大満腹度拡張要請(pchara,power);

	return true;
}
int cDrink_ID_18::飲む_効果_通常(pcCharacter pchara)
{
	return 効果(pchara, 効果量(0)
		);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//胃縮小（仮名）
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cDrink_ID_19::効果(pcCharacter pchara, int power)
{
	//効果音未実装
	sg_pDungeonSystem->最大満腹度縮小要請(pchara,power);

	return true;
}
int cDrink_ID_19::飲む_効果_通常(pcCharacter pchara)
{
	return 効果(pchara, 効果量(0)
		);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//脱力（仮名）
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cDrink_ID_20::効果(pcCharacter pchara, int power, int turn)
{
	//効果音未実装
	sg_pDungeonSystem->脱力要請(pchara,power,turn);

	return true;
}
int cDrink_ID_20::飲む_効果_通常(pcCharacter pchara)
{
	return 効果(pchara, 効果量(0), 効果量(1)
		);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//元気（仮名）
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cDrink_ID_21::効果(pcCharacter pchara, int power, int turn)
{
	//効果音未実装
	sg_pDungeonSystem->元気要請(pchara,power,turn);

	return true;
}
int cDrink_ID_21::飲む_効果_通常(pcCharacter pchara)
{
	return 効果(pchara, 効果量(0), 効果量(1)
		);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//目薬（仮名）
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cDrink_ID_22::効果(pcCharacter pchara)
{
	//効果音未実装
	sg_pDungeonSystem->目薬要請(pchara);

	return true;
}
int cDrink_ID_22::飲む_効果_通常(pcCharacter pchara)
{
	return 効果(pchara
		);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//命
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cDrink_ID_23::効果(pcCharacter pchara, int power)
{
	//効果音未実装
	sg_pDungeonSystem->最大HP拡張要請(pchara, power);

	return true;
}
int cDrink_ID_23::飲む_効果_通常(pcCharacter pchara)
{
	return 効果(pchara
		,効果量(0)
		);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//貧乏
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cDrink_ID_24::効果(pcCharacter pchara, int power)
{
	//効果音未実装
	sg_pDungeonSystem->貧乏要請(pchara, power);

	return true;
}
int cDrink_ID_24::飲む_効果_通常(pcCharacter pchara)
{
	return 効果(pchara
		,効果量(0)
		);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//異常状態治し
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cDrink_ID_25::効果(pcCharacter pchara)
{
	//効果音未実装
	sg_pDungeonSystem->精神異常治療要請(pchara);
	sg_pDungeonSystem->身体異常治療要請(pchara);

	map<tstring, StyleString> valiable;
	valiable[_T("Chara")] = pchara->ShortName();
	g_Langメッセージ(_T("cDrink_ID_25効果メッセージ"),valiable);

	return true;
}
int cDrink_ID_25::飲む_効果_通常(pcCharacter pchara)
{
	return 効果(pchara
		);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//液体窒素
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cDrink_ID_26::効果(pcCharacter pchara, int power)
{
	//効果音未実装
	sg_pDungeonSystem->氷付け要請(pchara,power);


	return true;
}
int cDrink_ID_26::飲む_効果_通常(pcCharacter pchara)
{
	return 効果(pchara,
		効果量(0));
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//神便鬼毒酒
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cDrink_ID_27::飲む_効果_通常(pcCharacter pchara)
{
	if(pchara->CharaAttribute().count(キャラ属性::鬼))
	{
	
		sg_pDungeonSystem->脱力要請(pchara,効果量(2),効果量(1));
		sg_pDungeonSystem->軟弱要請(pchara,効果量(3),効果量(1));
		sg_pDungeonSystem->金縛り要請(pchara,効果量(1));
	}
	else
	{
		sg_pDungeonSystem->頑強要請(pchara,効果量(0),効果量(1));
	}

	return true;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//封印酒
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cDrink_ID_28::飲む_効果_通常(pcCharacter pchara)
{

	sg_pDungeonSystem->封印要請(pchara,効果量(0));

	return true;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//国士無双の薬
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cDrink_ID_29::飲む_効果_通常(pcCharacter pchara)
{

	sg_pDungeonSystem->無敵要請(pchara,効果量(0));

	return true;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//嫉妬のカクテル
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cDrink_ID_30::飲む_効果_通常(pcCharacter pchara)
{

	sg_pDungeonSystem->嫉妬要請(pchara,効果量(0));

	return true;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//ハイビール
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cDrink_ID_31::飲む_効果_通常(pcCharacter pchara)
{
	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("recover.wav"),pchara->足元地形()->place);

	if(pchara->HP == pchara->MHP)
	{
		sg_pDungeonSystem->最大HP拡張要請(pchara,効果量(1));	
	}
	else
	{
		sg_pDungeonSystem->回復要請(pchara,効果量(0));	
	}
	return true;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//エクスビール
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cDrink_ID_32::飲む_効果_通常(pcCharacter pchara)
{
	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("recover.wav"),pchara->足元地形()->place);

	if(pchara->HP == pchara->MHP)
	{
		sg_pDungeonSystem->最大HP拡張要請(pchara,効果量(1));	
	}
	else
	{
		sg_pDungeonSystem->回復要請(pchara,pchara->MHP*効果量(0)/100.0);
	}
	return true;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//おしるこ
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cDrink_ID_33::飲む_効果_通常(pcCharacter pchara)
{
	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("eat.wav"),pchara->足元地形()->place);

	sg_pDungeonSystem->満腹度回復要請(pchara,効果量(0));

	return true;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//カレーライス
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cDrink_ID_34::飲む_効果_通常(pcCharacter pchara)
{
	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("eat.wav"),pchara->足元地形()->place);

	sg_pDungeonSystem->満腹度回復要請(pchara,効果量(0));

	return true;
}
