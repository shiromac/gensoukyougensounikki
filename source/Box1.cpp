#include "stdafx.h"
#include "Box1.h"
#include "cDrink.h"
#include "cFloor.h"
#include "cDropingDistribution.h"
#include "cDungeonSystem.h"
#include "cAttackinformation.h"
#include "cCocktailWindow.h"
#include "GameIdiom.h"
#include "FindUtility.h"
#include "EffectFunctions.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//空箱
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
int cBox_ID_0::見る(IDirect3DDevice9 *pDev)
{
	int result = 0;


	if(state() == cDroping::STATE_NORMAL)
	{
		result = 見る_メッセージ_通常();
	}
	else if(state() == cDroping::STATE_CURSE)
	{
		result = 見る_メッセージ_呪い();
	}
	else if(state() == cDroping::STATE_GOOD)
	{
		result = 見る_メッセージ_祝福();
	}


	if(result)
	{
		result = 見る要請();
	}

	if(result)
	{//使用に成功した
		//sg_pDungeonSystem->メニューを閉じる();
		//sg_pDungeonSystem->ターンエンド();
	}

	return true;
}

int cBox_ID_0::見る要請()
{

	//----------------------------------
	//使用可能なリスト
	//----------------------------------

	pcControlLayer pccl;
	pcListWindow pclw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(sg_pDungeonSystem->pDevice_D3D);

	pccl->WindowList.push_back(pclw = pcListWindow(new cListWindow));

	pcDroping pdrop;
	if(me()->includedItem.size() != 0)
	{//なにかある
		pclw->pDroplist = me()->includedItem;
	}
	else
	{//なにもない
		pclw->pDroplist.push_back(pcDroping(new cDropingNothing));
	}
	pclw->Init(sg_pDungeonSystem->pDevice_D3D, 20, min(10,max(pclw->pDroplist.size(),capacity())));
	pclw->setLeft(40);
	pclw->setTop(64);


	int result = 0;
	if(state() == cDroping::STATE_NORMAL)
	{
		result = 見る_効果_通常();
	}
	else if(state() == cDroping::STATE_CURSE)
	{
		result = 見る_効果_呪い();
	}
	else if(state() == cDroping::STATE_GOOD)
	{
		result = 見る_効果_祝福();
	}

	if(result)
	{//使用に成功した
		//sg_pDungeonSystem->落ち物破壊要請(me());
	}

	return result;
}
*/

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//------------------------------------------------------------------------------
//ありがたい箱
//------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int cBox_ID_1::入れる_効果_通常(pcDroping pdrop)
{
	pdrop->被祝福要請();

	sg_pDungeonSystem->動的識別(me());

	return true;
}

//弁当箱
bool cBox_ID_2::箱入れ可(pcDroping pdrop)
{
	return (pdrop->属性.count(落ち物属性::食べ物) > 0);
}

//水筒箱
bool cBox_ID_3::箱入れ可(pcDroping pdrop)
{
	return (pdrop->属性.count(落ち物属性::飲み物) > 0);
}
//本箱
bool cBox_ID_4::箱入れ可(pcDroping pdrop)
{
	return (pdrop->属性.count(落ち物属性::読み物) > 0);
}
//弾幕箱
bool cBox_ID_5::箱入れ可(pcDroping pdrop)
{
	return (pdrop->属性.count(落ち物属性::撃ち物) > 0);
}
//スペル箱
bool cBox_ID_6::箱入れ可(pcDroping pdrop)
{
	return (pdrop->属性.count(落ち物属性::スペカ) > 0);
}
//武具箱
bool cBox_ID_7::箱入れ可(pcDroping pdrop)
{
	return (pdrop->属性.count(落ち物属性::装備品) > 0);
}
//箱の箱
bool cBox_ID_8::箱入れ可(pcDroping pdrop)
{
	return (pdrop->ID() != ID());
}
//貯金箱
bool cBox_ID_9::箱入れ可(pcDroping pdrop)
{
	return (pdrop->属性.count(落ち物属性::お金) > 0);
}

//賽銭箱
bool cBox_ID_10::箱入れ可(pcDroping pdrop)
{
	return (pdrop->属性.count(落ち物属性::お金) > 0);
}
int cBox_ID_10::入れる_効果_通常(pcDroping pdrop)
{
	int i;
	for(i=0;i<includedItem.size();i++)
	{
		if(includedItem[i]->まとめ要請(pdrop))
		{//まとめ成功
			capacity()--;
			return true;
			sg_pDungeonSystem->動的識別(me());
		}
	}
	return true;
}
int cBox_ID_10::衝突時効果(pcCharacter pchara)
{
	int i;
	for(i=0;i<includedItem.size();i++)
	{
		includedItem[i]->衝突時効果(pchara);
	}
	cBox::衝突時効果(pchara);
	return true;
}
//救急箱
int cBox_ID_11::入れる_効果_通常(pcDroping pdrop)
{
	sg_pDungeonSystem->落ち物破壊要請(pdrop);
	capacity()--;

	pcCharacter pchara = Holder();
	if(pchara == NULL) pchara = 乗りキャラ();

	if(pchara != NULL)
	{
		sg_pDungeonSystem->AnimationManager().
			Anime_PlaySE(_T("recover.wav"),pchara->足元地形()->place);


		sg_pDungeonSystem->回復要請(pchara, pchara->MHP * (効果量(0) + (効果量(1) - 効果量(0))*random())/100.0 +1);
		sg_pDungeonSystem->動的識別(me());
	}
	
	return true;
}
bool cBox_ID_11::箱入れ可(pcDroping pdrop)
{
	return !(pdrop->内包されている());
}
//流転の箱
int cBox_ID_12::入れる_効果_通常(pcDroping pdrop)
{

	cDropingDistribution iDP = sg_pDungeonSystem->pFloor()->dropitemDP();

	int IDgroup = pdrop->ID()/1000;
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

	pcDroping newdrop = sg_pDungeonSystem->落ち物生成_設置なし(id);
	
	if(newdrop != NULL)
	{
		newdrop->state() = pdrop->state();
		newdrop->quality() = pdrop->quality();
		if(pdrop->状態値識別済み()) sg_pDungeonSystem->状態のみ識別(newdrop,0);
		if(pdrop->修正値識別済み()) sg_pDungeonSystem->修正のみ識別(newdrop,0);

		pdrop->shopitemflag() = cDroping::SHOPFLAG_CANNOTYAKU;//厄システム挿入不可			
		sg_pDungeonSystem->落ち物変化(pdrop,newdrop);
		//sg_pDungeonSystem->落ち物破壊要請(pdrop);

		//sg_pDungeonSystem->落ち物入れ要請(me(),newdrop);

	}
	else
	{
	
	}

	return true;
}
//変化の箱
int cBox_ID_13::入れる_効果_通常(pcDroping pdrop)
{
	int id[3];

	sg_pDungeonSystem->アイテム自然湧きID(id);

	pcDroping newdrop = sg_pDungeonSystem->落ち物生成_設置なし(id[0]);
	
	if(newdrop != NULL)
	{
		sg_pDungeonSystem->落ち物変化(pdrop,newdrop);

		//sg_pDungeonSystem->落ち物破壊要請(pdrop);

		//sg_pDungeonSystem->落ち物入れ要請(me(),newdrop);

	}
	else
	{
	
	}

	return true;
}
//合成の箱
int cBox_ID_14::入れる_効果_通常(pcDroping pdrop)
{
	int size = includedItem.size();
	sg_pDungeonSystem->アイテム合成(includedItem,1,1,1);
	size -= includedItem.size();
	capacity() -= size;

	return true;
}
//小さなつづら
int cBox_ID_15::開ける_効果_通常()
{
	sg_pDungeonSystem->識別(me(),MASSAGE_IFCHANGE);

	map<tstring, StyleString> valiable;
	g_Langメッセージ(_T("cBox_ID_15開けるメッセージ"),valiable);

	int size = 効果量(0);
	int i;
	pcLandform pland;
	pcDroping pbox = sg_pDungeonSystem->落ち物生成_設置なし(7000);
	pbox->quality() = size;
	sg_pDungeonSystem->名称のみ識別(pbox,0);

	if(pbox)
	{
		sg_pDungeonSystem->落ち物変化(me(),pbox);

		for(i=0;i<size;i++)
		{
			int vint[3];
			sg_pDungeonSystem->アイテム自然湧きID(vint);
			pcDroping pdrop = sg_pDungeonSystem->落ち物生成_設置なし(vint[0]);
			if(pdrop != NULL)
				sg_pDungeonSystem->落ち物入れ要請(pbox,pdrop);
		}

	}

	return true;
}
//大きなつづら
int cBox_ID_16::開ける_効果_通常()
{

	pcLandform pland;
	map<tstring, StyleString> valiable;
	g_Langメッセージ(_T("cBox_ID_16開けるメッセージ"),valiable);

	if(Holder() != NULL)
	{
		pland = Holder()->足元地形();
	}
	else
	{
		pland = 現在地形();
	}

	pcDroping pdrop = sg_pDungeonSystem->落ち物生成_設置なし(7000);//空箱
	sg_pDungeonSystem->名称のみ識別(pdrop,0);
	if(pdrop)
	{
		sg_pDungeonSystem->落ち物変化(me(),pdrop);
	}

	return 効果(pland);
}
int cBox_ID_16::BreakOut()
{
	効果(現在地形());
	
	return cBox::BreakOut();
}
int cBox_ID_16::効果(pcLandform pland)
{
	int i;
	
	sg_pDungeonSystem->識別(me(),MASSAGE_IFCHANGE);


	int size = 効果量(0);
	


	for(i=0;i<size;i++)
	{
		int vint[3];
		sg_pDungeonSystem->敵自然湧きID(vint);
		pcCharacter pchara = sg_pDungeonSystem->キャラクター生成(vint[0],vint[1],CHARACTER_FORSE_ENEMY,pland);
		if(pchara != NULL)
		{
			pchara->Condition.とても強い刺激();
			sg_pDungeonSystem->眠り要請(pchara,1,0);
		}
	}



	return true;
}
//宝箱
void cBox_ID_17::DataBeginOptimize(int difficulty)
{
	cBox::DataBeginOptimize(difficulty);

	int size = capacity();
	int i;
	pcLandform pland;


	for(i=0;i<size;i++)
	{
		int vint[3];
		sg_pDungeonSystem->アイテムレア湧きID(vint,効果量(0)/100.0);
		pcDroping pdrop = sg_pDungeonSystem->落ち物生成_設置なし(vint[0]);
		if(pdrop != NULL)
			sg_pDungeonSystem->落ち物入れ要請(me(),pdrop);
	}


}
void cBox_ID_17::AppreciationBeginOptimize(int difficulty)
{
	cBox::AppreciationBeginOptimize(difficulty);
	sg_pDungeonSystem->識別(me(),0);
}

int cBox_ID_17::開ける_メッセージ_通常()
{
	map<tstring, StyleString> valiable;
	g_Langメッセージ(_T("cBox_ID_17開けるメッセージ"),valiable);
	return false;
}
int cBox_ID_17::開ける_メッセージ_呪い()
{
	return 開けるメッセージ定型();
}
int cBox_ID_17::開ける_効果_通常()
{
	return false;
}
int cBox_ID_17::開ける_効果_呪い()
{
	sg_pDungeonSystem->識別(me(),MASSAGE_IFCHANGE);

	int size = capacity();
	int i;
	pcLandform pland;
	pcDroping pbox = sg_pDungeonSystem->落ち物生成_設置なし(7000);
	pbox->quality() = size;
	sg_pDungeonSystem->名称のみ識別(pbox,0);

	if(pbox)
	{
		sg_pDungeonSystem->落ち物変化(me(),pbox);

		vector<pcDroping> vpdrop = me()->includedItem;
		for(i=0;i<vpdrop.size();i++)
		{
			sg_pDungeonSystem->落ち物出し要請(vpdrop[i],pbox);
		}

	}

	return true;
}
void cBox_ID_17::CutIn(タイミング timing, cValiableField& valiable)
{


	if(timing == 被攻撃接近直後_タイミング)
	{

		if(valiable.intsets[変数_属性].count(攻撃属性::アイテム破壊))
		{

			GameIdiom::高飛び(me(),現在地形());
			sg_pDungeonSystem->落ち物ばら撒き要請(me());

		}
	
	}
	if(timing == 被破壊直前_タイミング)
	{
		sg_pDungeonSystem->落ち物ばら撒き要請(me());
	}

	cBox::CutIn(timing,valiable);

}
//パンドラの箱
int cBox_ID_18::開ける_効果_通常()
{

	pcLandform pland;
	map<tstring, StyleString> valiable;
	g_Langメッセージ(_T("cBox_ID_18開けるメッセージ"),valiable);

	if(Holder() != NULL)
	{
		pland = Holder()->足元地形();
	}
	else
	{
		pland = 現在地形();
	}

	int i;
	int size = 効果量(0);//アイテム出現数
	pcDroping pbox = sg_pDungeonSystem->落ち物生成_設置なし(7000);
	pbox->quality() = size;
	sg_pDungeonSystem->名称のみ識別(pbox,0);

	if(pbox)
	{
		sg_pDungeonSystem->落ち物変化(me(),pbox);

		for(i=0;i<size;i++)
		{
			int vint[3];
			sg_pDungeonSystem->アイテム自然湧きID(vint);
			pcDroping pdrop = sg_pDungeonSystem->落ち物生成_設置なし(vint[0]);
			if(pdrop != NULL)
				sg_pDungeonSystem->落ち物入れ要請(pbox,pdrop);
		}

	}


	return 効果(pland);
}
int cBox_ID_18::BreakOut()
{
	効果(現在地形());
	
	return cBox::BreakOut();
}
int cBox_ID_18::効果(pcLandform pland)
{
	int i;
	
	sg_pDungeonSystem->識別(me(),MASSAGE_IFCHANGE);

	pcCharacter pchara = pland->pOnChar;

	if(pchara != NULL)
	{
		sg_pDungeonSystem->脱力要請(pchara,効果量(2),効果量(3));
		sg_pDungeonSystem->軟弱要請(pchara,効果量(4),効果量(5));
		GameIdiom::高飛び(pchara,pland);
		pland = pchara->足元地形();
	}

	int size = 効果量(1);//敵出現数
	

	for(i=0;i<size;i++)
	{
		int vint[3];
		sg_pDungeonSystem->敵自然湧きID(vint);
		pcCharacter pchara = sg_pDungeonSystem->キャラクター生成(vint[0],vint[1],CHARACTER_FORSE_ENEMY,pland);
		if(pchara != NULL)
		{
			pchara->Condition.とても強い刺激();
			sg_pDungeonSystem->眠り要請(pchara,1,0);
		}
	}



	return true;
}
//玉手箱
int cBox_ID_19::開ける_効果_通常()
{

	pcLandform pland;
	map<tstring, StyleString> valiable;
	g_Langメッセージ(_T("cBox_ID_19開けるメッセージ"),valiable);

	if(Holder() != NULL)
	{
		pland = Holder()->足元地形();
	}
	else
	{
		pland = 現在地形();
	}


	return 効果(pland);
}
int cBox_ID_19::BreakOut()
{
	効果(現在地形());
	
	return cBox::BreakOut();
}
int cBox_ID_19::効果(pcLandform pland)
{
	int i;
	
	sg_pDungeonSystem->識別(me(),MASSAGE_IFCHANGE);

	EffectFunctions::煙エフェクト1(pland->placeX,pland->placeY);

	int size = 1;
	pcDroping pbox = sg_pDungeonSystem->落ち物生成_設置なし(7000);
	pbox->quality() = size;
	sg_pDungeonSystem->名称のみ識別(pbox,0);

	if(pbox && (Holder() || Parent() || 落ちてる()))
	{
		sg_pDungeonSystem->落ち物変化(me(),pbox);
	}

	pcCharacter pchara = pland->pOnChar;

	if(pchara != NULL)
	{
		sg_pDungeonSystem->レベルアップ(pchara);
		sg_pDungeonSystem->軟弱要請(pchara,効果量(0),効果量(1));
	}

	return true;
}
//復活の箱
int cBox_ID_20::開ける_効果_通常()
{

	pcLandform pland;
	map<tstring, StyleString> valiable;
	g_Langメッセージ(_T("cBox_ID_20開けるメッセージ"),valiable);

	if(Holder() != NULL)
	{
		pland = Holder()->足元地形();
	}
	else
	{
		pland = 現在地形();
	}


	return 効果(pland);
}
int cBox_ID_20::効果(pcLandform pland)
{
	int i;
	
	sg_pDungeonSystem->識別(me(),MASSAGE_IFCHANGE);

	int size = 1;
	pcDroping pbox = sg_pDungeonSystem->落ち物生成_設置なし(7000);
	pbox->quality() = size;
	sg_pDungeonSystem->名称のみ識別(pbox,0);

	if(pbox)
	{
		sg_pDungeonSystem->落ち物変化(me(),pbox);
	}


	return true;
}
void cBox_ID_20::CutIn(タイミング timing, cValiableField& valiable)
{
	cBox::CutIn(timing,valiable);

	if(timing == 死亡直前_タイミング)
	{
		if(!内包されている())
		{
			if(valiable.doubles[変数_死亡回避] == 0 && !(state() == cDroping::STATE_CURSE))
			{//まだ死亡回避していない

				valiable.doubles[変数_死亡回避] = 1;//味方なら死亡回避
				
				sg_pDungeonSystem->名称のみ識別(me(),0);

				sg_pDungeonSystem->全キャラ強制ターンエンド要請();
			
				pcLandform pland;
				map<tstring, StyleString> valiable;
				valiable[_T("Item")] = FullName();
				g_Langメッセージ(_T("cBox_ID_20効果メッセージ"),valiable);

				if(Holder() != NULL)
				{
					sg_pDungeonSystem->回復要請(Holder(),max(Holder()->MHP * 効果量(0)/100,効果量(1)), 0);
					sg_pDungeonSystem->満腹度回復要請(Holder(),Holder()->MaxStomach);
				}
				
				if(state() == cDroping::STATE_GOOD)
				{
					state() = cDroping::STATE_NORMAL;
				}
				else
				{

					pcDroping pbox = sg_pDungeonSystem->落ち物生成_設置なし(7000);
					pbox->quality() = 1;
					sg_pDungeonSystem->名称のみ識別(pbox,0);

					if(pbox)
					{
						sg_pDungeonSystem->落ち物変化(me(),pbox);
					}

				}


			}
		}
	}

}
//強化の箱
int cBox_ID_21::入れる_効果_通常(pcDroping pdrop)
{
	int i,size = 各種効果量対応(pdrop->ID()/1000,pdrop->使用難度());
	for(i=0;i<size;i++)
	{
		sg_pDungeonSystem->アイテム強化(pdrop);
	}
	
	sg_pDungeonSystem->修正のみ識別(pdrop,0);
	return true;
}
double cBox_ID_21::各種効果量対応(const int index, const int difficulty)
{
	if(index == CDROPING_cBook_BASE_ID_NUM/1000)
	{
		if(difficulty == 1) return 効果量(3);
		else if(difficulty == 2) return 効果量(4);
		else if(difficulty == 3) return 効果量(5);
		else if(difficulty == 4) return 効果量(6);
		else if(difficulty == 5) return 効果量(7);
		else return 効果量(7);
	}
	else if(index == CDROPING_cDrink_BASE_ID_NUM/1000)
	{
		return 効果量(2);
	}
	else if(index == CDROPING_cEquipment_BASE_ID_NUM/1000)
	{
		return 効果量(0);
	}
	else if(index == CDROPING_cSpell_BASE_ID_NUM/1000)
	{
		return 効果量(1);
	}
	else return 0;
}
//弱化の箱
int cBox_ID_22::入れる_効果_通常(pcDroping pdrop)
{
	pdrop->quality() -= 各種効果量対応(pdrop->ID()/1000,pdrop->使用難度());
	sg_pDungeonSystem->修正のみ識別(pdrop,0);
	return true;
}
double cBox_ID_22::各種効果量対応(const int index, const int difficulty)
{
	if(index == CDROPING_cBook_BASE_ID_NUM/1000)
	{
		if(difficulty == 1) return 効果量(3);
		else if(difficulty == 2) return 効果量(4);
		else if(difficulty == 3) return 効果量(5);
		else if(difficulty == 4) return 効果量(6);
		else if(difficulty == 5) return 効果量(7);
		else return 効果量(7);
	}
	else if(index == CDROPING_cDrink_BASE_ID_NUM/1000)
	{
		return 効果量(2);
	}
	else if(index == CDROPING_cEquipment_BASE_ID_NUM/1000)
	{
		return 効果量(0);
	}
	else if(index == CDROPING_cSpell_BASE_ID_NUM/1000)
	{
		return 効果量(1);
	}
	else return 0;
}
//ヤリスゴシの箱
int cBox_ID_23::開ける_効果_通常()
{
	pcCharacter pchara;
	if(Holder() != NULL)
	{
		pchara = Holder();
	}
	else
	{
		pchara = 現在地形()->pOnChar;
	}

	map<tstring, StyleString> valiable;
	valiable[_T("Item")] = FullName();
	g_Langメッセージ(_T("cBox_ID_23効果メッセージ"),valiable);

	if(pchara != NULL) sg_pDungeonSystem->やりすごし要請(pchara,効果量(0));
	sg_pDungeonSystem->識別(me(),0);
	sg_pDungeonSystem->落ち物破壊要請(me());
	return true;
}
int cBox_ID_23::衝突時効果(pcCharacter pchara)
{
	sg_pDungeonSystem->やりすごし要請(pchara,効果量(0));
	sg_pDungeonSystem->金縛り要請(pchara,効果量(0),0);
	sg_pDungeonSystem->識別(me(),0);
	sg_pDungeonSystem->落ち物破壊要請(me());
	//cBox::衝突時効果(pchara);
	return true;
}

//混酒の箱
void cBox_ID_24::DataBeginOptimize(int difficulty)
{
	cBox::DataBeginOptimize(difficulty);

	int size = capacity();
	int i;
	pcLandform pland;



}
void cBox_ID_24::AppreciationBeginOptimize(int difficulty)
{
	cBox::AppreciationBeginOptimize(difficulty);
	sg_pDungeonSystem->識別(me(),0);
	int i;
	for(i=0;i<cDrink::MATERIALNUM;i++)
	{
		material(i) = (効果量(0)+1)*random();
	}
}
bool cBox_ID_24::箱入れ可(pcDroping pdrop)
{
	return (pdrop->属性.count(落ち物属性::飲み物) > 0);
}
void cBox_ID_24::SumMaterial(vector<int>& vmaterial)
{
	vmaterial.clear();
	vmaterial.resize(cDrink::MATERIALNUM+1,0);

	int i,k;
	for(i=0;i<cDrink::MATERIALNUM;i++)
	{
		vmaterial[i] = material(i);
	}

	pcDrink pcDrk;
	for(k=0;k<includedItem.size();k++)
	{
		pcDrk = boost::dynamic_pointer_cast<cDrink>(includedItem[k]);
		if(pcDrk == NULL) continue;
		if(pcDrk->名称識別済み())
		{
			for(i=0;i<cDrink::MATERIALNUM;i++)
			{
				vmaterial[i] += pcDrk->マテリアル(i);
			}
		}
		else
		{
			for(i=0;i<cDrink::MATERIALNUM;i++)
			{
				vmaterial[i] += 1;
			}
		}
		
		vmaterial[i] += !(pcDrk->reuse());
	}

}
//スモールビュー
int cBox_ID_24::Getsmollview(pcGameWindow& smollview)
{

	vector<int> vint;
	SumMaterial(vint);

	if(capacity() <= 0)
	{
		return false;
	}

	pcGameWindow pcgw;
	smollview = (pcgw = pcGameWindow(new cGameWindow));

	getMaterialText(vint, pcgw->Text());


	pcgw->Init(sg_pDungeonSystem->pDevice_D3D, 4, 5);
	pcgw->setRight(SCREEN_X);
	pcgw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(3));
	pcgw->enable = false;

	return true;
}
void getMaterialText(const vector<int>& vmaterial, StyleString& output)
{
	StyleString sstr;
	int i;
	for(i=0;i<cDrink::MATERIALNUM+1;i++)
	{
		sstr = g_Lang((_T("飲み物マテリアル") + setStyle(i+1)).c_str()) + _T(" ") + setStyle(vmaterial[i]);

		switch(i)
		{
		case 0:
			sstr.setSameColor(0xFFFF77FF);
			break;
		case 1:
			sstr.setSameColor(0xFF44FF44);
			break;
		case 2:
			sstr.setSameColor(0xFFFFAA44);
			break;
		case 3:
			sstr.setSameColor(0xFF44FFFF);
			break;
		case 4:
			sstr.setSameColor(0xFFFFFFFF);
			break;
		}
		output += sstr;
		output += _T("\n");
	}

}
void getMaterialTextShort(const vector<int>& vmaterial, StyleString& output)
{
	StyleString sstr;
	int i;
	output += _T("(");
	for(i=0;i<cDrink::MATERIALNUM+1;i++)
	{
		sstr = setStyle(vmaterial[i]);

		switch(i)
		{
		case 0:
			sstr.setSameColor(0xFFFF77FF);
			break;
		case 1:
			sstr.setSameColor(0xFF44FF44);
			break;
		case 2:
			sstr.setSameColor(0xFFFFAA44);
			break;
		case 3:
			sstr.setSameColor(0xFF44FFFF);
			break;
		case 4:
			sstr.setSameColor(0xFFFFFFFF);
			break;
		}
		output += sstr;
		output += _T(",");
	}
	output += _T(")");
}
int cBox_ID_24::GetmenuCaption(vector<tstring> &CaptionList)
{
	if(Holder() == NULL)
	{
		//if(Parent() == NULL) CaptionList.push_back(_T("拾う"));
	}

	if(includedItem.size())
	{
		if(内容物使用可())
		{
			if(state() == cDroping::STATE_CURSE)
			{
				CaptionList.push_back(_T("のぞく"));
			}
			else
			{
				CaptionList.push_back(_T("見る"));
			}
		}
		else
		{
			CaptionList.push_back(_T("のぞく"));
		}
	}
	//if(state() != cDroping::STATE_CURSE)
	//{
		CaptionList.push_back(_T("入れる"));
		if(出し可() && includedItem.size())
		{
			//CaptionList.push_back(_T("出す"));
			CaptionList.push_back(_T("入れ替え"));
		}
	//}


	if(!used()) CaptionList.push_back(_T("開ける"));


	CaptionList.push_back(_T("投げる"));

	if(Holder() == NULL)
	{
		
	}
	else if(Holder()->足元() == NULL)
	{
		CaptionList.push_back(_T("置く"));
	}
	else
	{
		if(!内包されている()) CaptionList.push_back(_T("交換"));
	}

	CaptionList.push_back(_T("メモ"));
	
	if(!大局識別済み()) CaptionList.push_back(_T("名前"));

	CaptionList.push_back(_T("説明"));

	return true;
}
int cBox_ID_24::開ける_効果_通常()
{
	pcControlLayer pccl;
	pcCocktailWindow pccw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(sg_pDungeonSystem->pDevice_D3D);
	pccl->notCancelToPop = true;

	pccl->WindowList.push_back(pccw = pcCocktailWindow(new cCocktailWindow));


	pccw->Init(sg_pDungeonSystem->pDevice_D3D, me());
	
	return 2;//混酒の箱の例外処理
}

//鑑定の箱
int cBox_ID_25::入れる_効果_通常(pcDroping pdrop)
{
	int res = 0;
	if(!pdrop->完全識別済み()) res = 1;
	sg_pDungeonSystem->識別(pdrop);

	if(res) sg_pDungeonSystem->動的識別(me());
	
	return true;//つねにtrue
}

//お払い箱
int cBox_ID_26::入れる_効果_通常(pcDroping pdrop)
{
	if(pdrop->state() == cDroping::STATE_CURSE)
	{
		sg_pDungeonSystem->アイテム祝福(pdrop);
		sg_pDungeonSystem->動的識別(me());
	}
	else
	{
		pcCharacter pchara = Holder();
		if(pchara == NULL) pchara = 乗りキャラ();
		sg_pDungeonSystem->落ち物破壊要請(pdrop);
		capacity()--;
		GameIdiom::悪性異常状態治療要請(pchara);
		sg_pDungeonSystem->AnimationManager().
			Anime_PlaySE(_T("recover.wav"),pchara->足元地形()->place);

		sg_pDungeonSystem->動的識別(me());
	}
	
	
	return true;
}

//思い出の小箱
int cBox_ID_27::入れる_効果_通常(pcDroping pdrop)
{
	if(!sg_pDungeonSystem->拠点フラグ())
	{
		sg_pDungeonSystem->アイテム店フラグ(pdrop, cDroping::SHOPFLAG_SHOULDYAKU);
		sg_pDungeonSystem->アイテム割引フラグ(pdrop, cDroping::SHOPDISCOUNTFLAG_FREE);
		sg_pDungeonSystem->厄システム挿入要請(sg_pDungeonSystem->pSaveQuest,pdrop);
		sg_pDungeonSystem->落ち物破壊要請(pdrop);
		capacity()--;
	}
	return true;
}

bool cBox_ID_27::箱入れ可(pcDroping pdrop)
{
	if(sg_pDungeonSystem->拠点フラグ())
	{
		return false;
	}
	if(pdrop->shopitemflag() == cDroping::SHOPFLAG_NONE)
	{
		return true;
	}
	return false;
}

//霊撃の箱
int cBox_ID_28::入れる_効果_通常(pcDroping pdrop)
{
	sg_pDungeonSystem->落ち物破壊要請(pdrop);
	capacity()--;

	pcCharacter pchara = Holder();
	if(pchara == NULL) pchara = 乗りキャラ();

	if(pchara != NULL)
	{
		sg_pDungeonSystem->AnimationManager().
			Anime_PlaySE(_T("shoot.wav"),pchara->足元地形()->place);

		EffectFunctions::ボスエフェクト発散(pchara->placeX, pchara->placeY, cColor(255,255,255,255), cColor(255,255,255,255));

		int a;
		for(a=0;a<8;a++)
		{

			pcLandform pland = sg_pDungeonSystem->キャラ視点方角地形(pchara, a, 1);
			pcCharacter enemy = pland->pOnChar;
			if(enemy != NULL)
			{
				sg_pDungeonSystem->吹き飛ばし要請(pchara, enemy, pchara->aspect + a, 効果量(2), 効果量(3));
			}
			sg_pDungeonSystem->びっくり要請(enemy, 効果量(1));
		}


		const vector<pcCharacter> vpchara = sg_pDungeonSystem->CharaList();
		int i, size = vpchara.size();
		for(i=0;i<size;i++)
		{
			if(sg_pDungeonSystem->キャラクター敵対判定(pchara, vpchara[i]))
			{
				if(FindUtility::キャラの距離(pchara, vpchara[i]) <= 4)
				{
					multiset<攻撃属性::攻撃属性> 属性;
					pcAttackinformation pca = 攻撃作成(
						pchara,//Attacker
						vpchara[i]->足元地形(),//Landplase
						効果量(0),//attackPower
						1,//damageflag
						属性,//attribute
						true
						);
					sg_pDungeonSystem->攻撃接近(pca);
				}
			}
		}

		sg_pDungeonSystem->動的識別(me());
	}
	
	return true;
}
bool cBox_ID_28::箱入れ可(pcDroping pdrop)
{
	return !(pdrop->内包されている());
}