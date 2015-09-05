#include "stdafx.h"

#include "cMoneyBag.h"

#include "cDungeonSystem.h"

#define DEF_ATTACKICON_INDEXSIZE 8.0


#define RUNOUTGOOD_POSSIBILITY (0.6)

cMoneyBag::cMoneyBag(void)
{

}

cMoneyBag::~cMoneyBag(void)
{
}

//消去処理の準備
void cMoneyBag::Release()
{

}


void cMoneyBag::Init(IDirect3DDevice9 *pDev,pcDroping self)
{
	cItem::Init(pDev,self);
}

/*
cDiscreteProbability cMoneyBag::stateBeginDP(int difficulty)
{
	cDiscreteProbability DP;
	DP.set(10,cDroping::STATE_NORMAL);
	DP.set(1,cDroping::STATE_GOOD);
	return DP;
}
*/
cDiscreteProbability cMoneyBag::qualityBeginDP(int difficulty)
{
	cDiscreteProbability DP;

	int min = 効果量基礎(0);
	int max = 効果量基礎(1);
	int i;
	for(i=min;i<=max;i++)
	{
		DP.set(1,i);
	}
	return DP;
}


tstring cMoneyBag::FullNameBase()
{
	cMoney mo;
	mo.value = quality();

	return mo.caption();
}


void cMoneyBag::DrawStateIconSub(IDirect3DDevice9 *pDev,int x,int y)
{
	cDrawingObject DO;

	int upperline = y-8;
	int bottomline = y+8;
	int liftline = x+8;
	int rightline = x+8+16;


	DO.Width = 16;
	DO.Height = 16;

	//本体
	DO.m_color.ARGB(255,255,255,255);

	DO.setTexture(m_pTexture_StateIcon,64,64);




		
}



StyleString cMoneyBag::QualityCaption()
{
	//------------------------------------
	//修正値

	return _T("");

}


int cMoneyBag::GetmenuCaption(vector<tstring> &CaptionList)
{
	if(倉庫の中())
	{
		CaptionList.push_back(_T("説明"));
		CaptionList.push_back(_T("倉庫から出す"));
		return true;
	}

	if(Holder() == NULL)
	{
		if(Parent() == NULL) CaptionList.push_back(_T("拾う"));
		CaptionList.push_back(_T("投げる"));
	}
	else
	{

		
		CaptionList.push_back(_T("投げる"));

		if(Holder()->足元() == NULL)
		{
			CaptionList.push_back(_T("置く"));
		}
		else
		{
			if(!内包されている() && Holder()->足元()->属性.count(落ち物属性::アイテム)) CaptionList.push_back(_T("交換"));
			else if(!内包されている() &&
					Holder()->足元()->属性.count(落ち物属性::罠) &&
					!Holder()->足元()->罠用_効果目標か(sg_pDungeonSystem->pPlayerChara())) CaptionList.push_back(_T("交換"));
		}

	}

	CaptionList.push_back(_T("説明"));


	return true;
}

/*
//コマンドを解決する;
int cMoneyBag::FireCommand(IDirect3DDevice9 *pDev, tstring verb, vector<pcDroping> &ObjectList)
{


	if(verb == _T("説明")) 説明(pDev);
	if(verb == _T("拾う")) 拾う(pDev);
	if(verb == _T("置く")) 置く(pDev);
	if(verb == _T("投げる")) 投げる(pDev);
	if(verb == _T("交換")) 交換(pDev);

	return true;
}
*/


int cMoneyBag::衝突(pcCharacter pchara)
{
	int result = 0;

	衝突時_メッセージ(pchara);


	result = 衝突時効果(pchara);


	if(result)
	{//使用に成功した
		sg_pDungeonSystem->落ち物破壊要請(me());
	}
	
	return result;
}

int cMoneyBag::ダメージ効果(pcCharacter pchara, pcLandform pland, int power)
{

	//sg_pDungeonSystem->AnimationManager().
	//	Anime_PlaySE(_T("flame.wav"),pchara->足元地形()->place);
	//ここにエフェクト


	sg_pDungeonSystem->攻撃接近(攻撃作成(
		pchara,//攻撃者
		pland,//場所
		power,//攻撃力
		true,//定数ダメージ
		攻撃属性(),
		true
		));


	return true;
}
multiset<攻撃属性::攻撃属性> cMoneyBag::攻撃属性()
{
	multiset<攻撃属性::攻撃属性> 属性;
	return 属性;
}


int cMoneyBag::衝突時効果(pcCharacter pchara)
{
	int power = amount();
		
	return ダメージ効果(投擲者()
		, sg_pDungeonSystem->キャラ足元地形(pchara)//地形
		, power//威力
		);
}



int cMoneyBag::衝突時_メッセージ(pcCharacter pchara)
{
	map<tstring, StyleString> valiable;
	valiable[_T("Item")] = FullName();
	g_Langメッセージ(_T("Money衝突時メッセージ"),valiable);
	//sg_pDungeonSystem->メッセージ((FullName()+_T(" が当たった。\n")));

	return true;
}


int cMoneyBag::被まとめ要請(pcCharacter pchara)
{
	if(被まとめる(pchara))
	{
		return true;
	}

	return false;
}

int cMoneyBag::被まとめる(pcCharacter pchara)
{

	if(pchara == sg_pDungeonSystem->pPlayerChara())
	{
		sg_pDungeonSystem->主人公お金拾得要請(me());

		sg_pDungeonSystem->落ち物破壊要請(me());
		return true;
	}

	return false;
}

int cMoneyBag::まとめ要請(pcDroping pdrop)
{

	if(pdrop->属性.count(落ち物属性::お金) && me() != pdrop)
	{//まとめられる
		まとめる(pdrop);
	}
	else
	{
		return false;
	}
	return true;
}
int cMoneyBag::まとめる(pcDroping pdrop)
{
	quality() += pdrop->quality();

	int st = 0;

	if(state() == cDroping::STATE_CURSE) st--;
	if(pdrop->state() == cDroping::STATE_CURSE) st--;

	if(state() == cDroping::STATE_GOOD) st++;
	if(pdrop->state() == cDroping::STATE_GOOD) st++;

	if(st < 0) state() = cDroping::STATE_CURSE;
	else if(st > 1) state() = cDroping::STATE_GOOD;
	else state() = cDroping::STATE_NORMAL;

	sg_pDungeonSystem->落ち物破壊要請(pdrop);

	if(Holder() != NULL && Holder() == sg_pDungeonSystem->pPlayerChara())
	{
		map<tstring, StyleString> valiable;
		valiable[_T("ItemA")] = pdrop->FullName();
		valiable[_T("ItemB")] = FullName();
		g_Langメッセージ(_T("Moneyまとめメッセージ"),valiable);
	}
			

	return true;
}


bool cMoneyBag::stategoodRunoutprocess()
{
	if(RUNOUTGOOD_POSSIBILITY < random())
	{

		map<tstring, StyleString> valiable;
		valiable[_T("Item")] = FullName();
		g_Langメッセージ(_T("Money神力切れメッセージ"),valiable);
	
		//sg_pDungeonSystem->メッセージ(
		//	FullName() + _T(" の神力がなくなった！\n"));
		state() = cDroping::STATE_NORMAL;	

		return true;
	}

	return false;
}

