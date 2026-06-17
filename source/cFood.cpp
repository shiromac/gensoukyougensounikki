
#include "cFood.h"

#include "cDungeonSystem.h"

#include "Language.h"


#define FOOD_BASICVALUESTR (_T("Food基本値"))

cFood::cFood(void)
{
}

cFood::~cFood(void)
{
}




int cFood::GetmenuCaption(vector<tstring> &CaptionList)
{
	if(倉庫の中())
	{
		CaptionList.push_back(_T("説明"));
		CaptionList.push_back(_T("倉庫から出す"));
		return true;
	}

	if(Holder() == NULL)
	{//はじめ
		if(Parent() == NULL)
		{
			if(sg_pDungeonSystem->pPlayerChara()->holdItem.size() >= sg_pDungeonSystem->pPlayerChara()->MaxholdNum())
				CaptionList.push_back(_T("交換"));
			else
				CaptionList.push_back(_T("拾う"));
		}
	}

	CaptionList.push_back(_T("食べる"));
	CaptionList.push_back(_T("投げる"));

	if(Holder() == NULL)
	{
		
	}
	else if(Holder()->足元() == NULL)
	{
		CaptionList.push_back(_T("置く"));
	}
		

	if(Holder() != NULL && Holder()->足元() != NULL && !内包されている())
	{//落ちてる
		if(Holder()->足元()->属性.count(落ち物属性::アイテム)) {
			CaptionList.push_back(_T("交換"));
		}
		else if(Holder()->足元()->属性.count(落ち物属性::罠) &&
			!Holder()->足元()->罠用_効果目標か(sg_pDungeonSystem->pPlayerChara())) {
			CaptionList.push_back(_T("交換"));
		}
	}

	CaptionList.push_back(_T("説明"));

	return true;
}

/*
//コマンドを解決する;
int cFood::FireCommand(cRenderDevice *pDev, tstring verb, vector<pcDroping> &ObjectList)
{

	if(verb == _T("食べる"))
	{
		食べる(pDev);
	}


	if(verb == _T("説明")) 説明(pDev);
	if(verb == _T("拾う")) 拾う(pDev);
	if(verb == _T("置く")) 置く(pDev);
	if(verb == _T("投げる")) 投げる(pDev);
	if(verb == _T("交換")) 交換(pDev);

	return true;
}


int cFood::食べる(cRenderDevice *pDev)
{
	int result = 0;


	if(state() == cDroping::STATE_NORMAL)
	{
		result = 食べる_メッセージ_通常(sg_pDungeonSystem->pPlayerChara());
	}
	else if(state() == cDroping::STATE_CURSE)
	{
		result = 食べる_メッセージ_呪い(sg_pDungeonSystem->pPlayerChara());
	}
	else if(state() == cDroping::STATE_GOOD)
	{
		result = 食べる_メッセージ_祝福(sg_pDungeonSystem->pPlayerChara());
	}
	if(result)
	{//使用に成功した
		sg_pDungeonSystem->メニューを閉じる();
		sg_pDungeonSystem->ターンエンド();
	}

	result = 食べ要請(sg_pDungeonSystem->pPlayerChara());




	return true;
}
int cFood::食べ要請(pcCharacter pchara)
{

	int result = 0;
	if(state() == cDroping::STATE_NORMAL)
	{
		result = 食べる_効果_通常(pchara);
	}
	else if(state() == cDroping::STATE_CURSE)
	{
		result = 食べる_効果_呪い(pchara);
	}
	else if(state() == cDroping::STATE_GOOD)
	{
		result = 食べる_効果_祝福(pchara);
	}

	if(result)
	{//使用に成功した
		sg_pDungeonSystem->落ち物破壊要請(me());
	}

	return result;
}
*/
double cFood::デフォルト効果量状態倍率_祝福()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value((tstring)FOOD_BASICVALUESTR, (tstring)_T("デフォルト効果量状態倍率_祝福"), 2.0);
}
int cFood::食べる_効果_通常(pcCharacter pchara)
{
	sg_pDungeonSystem->メッセージ((StyleString)_T("cFood 食べる_効果_通常() 効果が設定されていません。\n"));
	return true;
}
int cFood::食べる_メッセージ_通常(pcCharacter pchara)
{
	return 食べるメッセージ定型(pchara);
}

int cFood::食べる_効果_呪い(pcCharacter pchara)
{
	//効果なし
	return false;
}
int cFood::食べる_メッセージ_呪い(pcCharacter pchara)
{
	sg_pDungeonSystem->メッセージ(g_Lang(_T("Food呪いによる食べ不可メッセージ")));
	return true;
}

int cFood::食べる_効果_祝福(pcCharacter pchara)
{
	食べる_効果_通常(pchara);
	return 食べる_効果_通常(pchara);
}
int cFood::食べる_メッセージ_祝福(pcCharacter pchara)
{
	return 食べる_メッセージ_通常(pchara);
}


int cFood::食べるメッセージ定型(pcCharacter pchara)
{
	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("eat.wav"),pchara->足元地形()->place);

	map<tstring, StyleString> valiable;
	valiable[_T("Item")] = FullName();
	g_Langメッセージ(_T("Food食べメッセージ"),valiable);
		

	return true;
}


int cFood::衝突(pcCharacter pchara)
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

int cFood::衝突時効果(pcCharacter pchara)
{

	if(state() == cDroping::STATE_NORMAL)
	{
		return 食べる_効果_通常(pchara);
	}
	else if(state() == cDroping::STATE_GOOD)
	{
		return 食べる_効果_祝福(pchara);
	}
	else
	{
		return cDroping::衝突(pchara);
	}
}

int cFood::衝突時_メッセージ(pcCharacter pchara)
{
	if(state() == cDroping::STATE_NORMAL)
	{
		return 食べる_メッセージ_通常(pchara);
	}
	else if(state() == cDroping::STATE_GOOD)
	{
		return 食べる_メッセージ_祝福(pchara);
	}
	else
	{
		return 食べる_メッセージ_呪い(pchara);
	}

}

double cFood::火時性質変化率()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		(tstring)_T("Food基本値"),(tstring)_T("火時性質変化率"),0.15);
}

void cFood::CutIn(タイミング timing, cValiableField& valiable)
{
	cItem::CutIn(timing,valiable);

	if(timing == 被攻撃接近直後_タイミング)
	{
		if(valiable.intsets.val(変数_属性).count(攻撃属性::火)
			&& random() < 火時性質変化率() && !属性.count(落ち物属性::耐火))
		{
			cValiableField newvaliable;
			newvaliable.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
			sg_pDungeonSystem->CutInM().CutIn(Holder(),食べ物加熱直前_タイミング,newvaliable);
			if(newvaliable.doubles.val(変数_汎用ブール))
			{

				pcDroping pdrop = sg_pDungeonSystem->落ち物生成_設置なし(加熱後変化ID());
				
				map<tstring, StyleString> valiable;
				valiable[_T("Item1")] = FullName();
				valiable[_T("Item2")] = pdrop->FullName();
				g_Langメッセージ(_T("Food焼けメッセージ"),valiable);
		
				sg_pDungeonSystem->落ち物変化(me(),pdrop);
			}
		}


	}
}