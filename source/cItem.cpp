#include "stdafx.h"

#include "cItem.h"

#include "cStringWindow.h"

#include "Language.h"
#include "utility/debug.h"
#define ITEM_CAPTION_MAX_LENGTH (40.0)

cItem::cItem(void)
{
}

cItem::~cItem(void)
{
}

StyleString cItem::FullName()
{
	StyleString FullN;
	StyleString Base = 識別未識別混合道具名();



	//------------------------------------
	//質
	FullN = Base;

	if(state() == cDroping::STATE_NORMAL)	
	{//普通
		
	}
	else if(state() == cDroping::STATE_CURSE)
	{//呪われてる
		//FullN = _T("呪われた") + FullN;
	}
	else if(state() == cDroping::STATE_GOOD)
	{//質のいい
		//FullN = _T("信仰された") + FullN;
	}



	
	//------------------------------------
	//修正値

	if(修正値識別済み())
	{
		FullN += QualityCaption();
	}
	//------------------------------------
	//メモ
	FullN += MemoCaption();

	//------------------------------------
	//オプション

	FullN += OptionCaption();

	//------------------------------------
	//色

	if(状態値識別済み() && state() == cDroping::STATE_CURSE)
	{
		FullN.setSameColor(ITEM_CURSE_COLOR);
	}
	else
	{
		if(大局識別済み())
		{	
			if(state() == cDroping::STATE_GOOD)
			{
				FullN.setSameColor(ITEM_GOOD_COLOR);
			}
			else if(状態値識別済み())
			{

				if(state() == cDroping::STATE_NORMAL || state() == cDroping::STATE_GOOD)	
				{//普通
					if(修正値識別済み())
					{
						if(特殊能力あり())
							FullN.setSameColor(ITEM_MITAMA_COLOR);
						else
							FullN.setSameColor(ITEM_CAPTION_COLOR);
					}
					else
					{
						FullN.setSameColor(ITEM_QUARAPP_COLOR);
					}
				}
				else if(state() == cDroping::STATE_CURSE)
				{//呪われてる
					FullN.setSameColor(ITEM_CURSE_COLOR);
				}
			}
			else
			{
				FullN.setSameColor(ITEM_HALFAPP_COLOR);
			}
		}
		else
		{
			FullN.setSameColor(ITEM_NONAPP_COLOR);
		}
	}


	if(FullN.length() > ITEM_CAPTION_MAX_LENGTH)
	{
		cRenderVector2 size(ITEM_CAPTION_MAX_LENGTH/FullN.length(),1);
		FullN.setSameSize(size);
	}

	StyleString ehs = ExtraHatString();
	if(ehs.c_str() != _T(""))
		return ExtraHatString() + FullN;
	else
		return FullN;
}

StyleString cItem::ExtraHatString()
{

	if(shopitemflag() == SHOPFLAG_SHOP)
	{
		cMoney money;
		money.value = 内包アイテム込み購入支払い値();
		map<tstring, StyleString> valiable;

		valiable[_T("Value")] = money.caption();

		return g_Lang(_T("買い値段Caption"), valiable);
	}
	else if(shopitemflag() == SHOPFLAG_YAKU)
	{
		int value = 厄ポイント単体買値();
		map<tstring, StyleString> valiable;

		valiable[_T("Value")] = setStyle(value);

		return g_Lang(_T("厄ポイントCaption"), valiable);
	}
	return _T("");
}


void cItem::DataBeginOptimize(int difficulty)
{
	cDroping::DataBeginOptimize(difficulty);

	state() = stateBeginDP(difficulty).get(random());
	quality() = qualityBeginDP(difficulty).get(random());

	AppreciationBeginOptimize(difficulty);
}
void cItem::AppreciationBeginOptimize(int difficulty)
{
	untiappreciate();
	if(デフォルト大局識別済み())
	{
		大局Appreciate();
	}
}


cDiscreteProbability cItem::stateBeginDP(int difficulty)
{
	cDiscreteProbability DP;
	//DP.set(7,cDroping::STATE_NORMAL);
	//DP.set(1,cDroping::STATE_GOOD);
	map<int,int> output;
	状態値出現分布(output);
	map<int,int>::iterator itr = output.begin();
	int i = 0;
	for(;itr!=output.end();itr++)
	{
		if(itr->first == 0) DP.set(itr->second, cDroping::STATE_NORMAL);
		else if(itr->first == -1) DP.set(itr->second, cDroping::STATE_CURSE);
		else if(itr->first == 1) DP.set(itr->second, cDroping::STATE_GOOD);
		else OnAssert(_T("状態値出現分布"),i,false,_T(" 書式にエラーが発生しました。スキップします。There is an error of form. Skip it."));
		i++;
	}
	return DP;
}
cDiscreteProbability cItem::qualityBeginDP(int difficulty)
{
	cDiscreteProbability DP;
	map<int,int> output;
	修正値出現分布(output);
	map<int,int>::iterator itr = output.begin();
	for(;itr!=output.end();itr++)
	{
		DP.set(itr->second,itr->first);
	}
	
	return DP;
}


StyleString cItem::QualityCaption()
{
	//------------------------------------
	//修正値
	if(quality() > 0)	
	{//正
		return _T("+") + setStyle(quality());
	}
	else if(quality() < 0)
	{//負
		return setStyle(quality());
	}
	else
	{
		return _T("");
	}
}

StyleString cItem::MemoCaption()
{
	//------------------------------------
	//個別メモ
	if(pmemo->str() != _T(""))	
	{//正
		return _T("(")+pmemo->str()+_T(")");
	}
	else
	{
		return _T("");
	}
}

StyleString cItem::NamingCaption()
{
	//------------------------------------
	//共通メモ
	pcMemoString pm = sg_pDungeonSystem->DataBase.アイテムメモ(ID());
	if( pm != NULL)	
	{//正
		return pm->str();
	}
	else
	{
		return _T("");
	}
}

StyleString cItem::FullNameSub()
{
	return setStyle(_T("No FullNameSub cItem"),0xFFFF0000);
}

StyleString cItem::ShortName()
{
	return setStyle(_T("No ShortName cItem"),0xFFFF0000);
}

StyleString cItem::ShortNameSub()
{
	return setStyle(_T("No ShortNameSub cItem"),0xFFFF0000);
}



void cItem::longExplanationExtra(std::vector<StyleString> &explanations)
{

	if(大局識別済み())
	{
		longExplanationExtraBase(explanations);
	}
	else
	{
		//なし
	}

}

void cItem::longExplanationExtraBase(std::vector<StyleString> &explanations)
{
	cDroping::longExplanationExtraBase(explanations);
	#ifdef _UNRELEASE
	//アンリリースモードのみ
	explanations.push_back(StyleString(_T("")));
	修正値出現分布テキスト_デバグ用(explanations.back());
	状態値出現分布テキスト_デバグ用(explanations.back());


	#endif
}