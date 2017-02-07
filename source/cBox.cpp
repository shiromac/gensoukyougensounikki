#include "stdafx.h"

#include "cBox.h"

#include "Language.h"


#include "cDungeonSystem.h"
#include "cCommandDroping.h"

#define WEAK_CRASH_BREAKOUT_PROBABILITY (0.3)

cBox::cBox(void)
{
}

cBox::~cBox(void)
{
}


StyleString cBox::識別未識別混合道具名()
{
	if(大局識別済み())
		return FullNameBase();
	else
	{
		if(NamingCaption() == _T(""))
			return sg_pDungeonSystem->DataBase.未識別名(ID());
		else
			return g_Lang(_T("箱:")) + NamingCaption();
	}
}
StyleString cBox::ExtraHatString()
{
	StyleString ehs = cItem::ExtraHatString();

	if(cannotopen())
	{
		ehs += g_Lang(_T("フタ開かずCaption"));
	}
	return ehs;
}

/*
int cBox::値段補正値()
{
	int value = 値段基礎値()*(1.0 + 0.1*quality());

	int i, size = includedItem.size();
	for(i=0;i<size;i++)
	{
		value += includedItem[i]->値段補正値();
	}
	
	return value;
}
*/

/*
cDiscreteProbability cBox::stateBeginDP(int difficulty)
{
	cDiscreteProbability DP;
	DP.set(10,cDroping::STATE_NORMAL);
	DP.set(1,cDroping::STATE_GOOD);
	return DP;
}
cDiscreteProbability cBox::qualityBeginDP(int difficulty)
{
	cDiscreteProbability DP;
	DP.set(4,3);
	DP.set(3,4);
	DP.set(2,5);
	DP.set(1,6);
	return DP;
}
*/



int cBox::GetmenuCaption(vector<tstring> &CaptionList)
{
	if(倉庫の中())
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

		if(!開け用())
		{
			CaptionList.push_back(_T("入れる"));
		}

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

	if(開け用())
	{
		CaptionList.push_back(_T("開ける"));
	}
	else
	{
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
		else
		{
			CaptionList.push_back(_T("のぞく"));
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
	}

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
		if(!内包されている() && Holder()->足元()->属性.count(落ち物属性::アイテム)) CaptionList.push_back(_T("交換"));
		else if(!内包されている() &&
			Holder()->足元()->属性.count(落ち物属性::罠) &&
			!Holder()->足元()->罠用_効果目標か(sg_pDungeonSystem->pPlayerChara())) CaptionList.push_back(_T("交換"));
	}

	CaptionList.push_back(_T("メモ"));
	
	if(!大局識別済み()) CaptionList.push_back(_T("名前"));

	CaptionList.push_back(_T("説明"));

	return true;
}
StyleString cBox::GetmenuExplain(tstring &Caption)
{
	if(Caption == _T("開ける"))
	{
		return g_Lang(_T("Box開けるShortExplanation"));
	}

	return _T("");
}

//Fireできるときtrue;
/*
int cBox::isCanFireCommand( vector<pcDroping> &ObjectList, tstring verb)
{

	if(verb == _T("入れる") || verb == _T("出す"))
	{
		if(ObjectList.size() < 1)
		{
			return false;
		}

	}

	if(verb == _T("入れ替え"))
	{
		if(ObjectList.size() < 2)
		{
			return false;
		}

	}


	return true;
}
*/

//オブジェクト(目的語)リストを得る;
//案内を返す
/*
StyleString cBox::GetmenuFuncObjectsList(vector<pcDroping> &ObjectList, bool &MultiObj,int objectsize, tstring verb)
{
	if(verb == _T("入れる"))
	{
		if(objectsize == 0)
		{

			ObjectList = sg_pDungeonSystem->pPlayerChara()->holdItem;

			MultiObj = true;
			if(sg_pDungeonSystem->pPlayerChara()->足元() != NULL)
				ObjectList.push_back(sg_pDungeonSystem->pPlayerChara()->足元());
			return _T("何を？_[複数選択可]");
		}

	}

	if(verb == _T("入れ替え"))
	{
		if(objectsize == 0)
		{
			MultiObj = false;
			ObjectList = includedItem;
			return _T("どれと？");
		}
		else if(objectsize == 1)
		{
			MultiObj = false;
			
			ObjectList = sg_pDungeonSystem->pPlayerChara()->holdItem;

			if(sg_pDungeonSystem->pPlayerChara()->足元() != NULL)
				ObjectList.push_back(sg_pDungeonSystem->pPlayerChara()->足元());
			
			return _T("どれを？");
		}
	}

		if(verb == _T("出す"))
	{
		if(objectsize == 0)
		{
			MultiObj = true;
			ObjectList = includedItem;
			return _T("どれを？_[複数選択可]");
		}

	}

	return _T("");
}
*/

/*
//コマンドを解決する;
int cBox::FireCommand(IDirect3DDevice9 *pDev, tstring verb, vector<pcDroping> &ObjectList)
{

	if(verb == _T("見る"))
	{
		見る(pDev);
	}
	if(verb == _T("入れる"))
	{
		if(ObjectList.size())
			入れる(pDev, ObjectList[0]);
	}
	if(verb == _T("出す"))
	{
		if(ObjectList.size())
			出す(pDev, ObjectList[0]);
	}
	if(verb == _T("入れ替え"))
	{
		if(ObjectList.size() >= 2)
			入れ替え(pDev, ObjectList[0], ObjectList[1]);
	}

	if(verb == _T("説明")) 説明(pDev);
	if(verb == _T("拾う")) 拾う(pDev);
	if(verb == _T("置く")) 置く(pDev);
	if(verb == _T("投げる")) 投げる(pDev);
	if(verb == _T("交換")) 交換(pDev);

	return true;
}
*/

StyleString cBox::QualityCaption()
{
	//------------------------------------
	//修正値

	//return _T("[") + setStyle(残容量()) +_T("/") +setStyle(capacity())+_T("]");
	return _T("[") + setStyle(残容量()) +_T("]");
}

/*
//----------------------------------------------------------
//入れる
//----------------------------------------------------------
int cBox::入れる(IDirect3DDevice9 *pDev, pcDroping pdrop)
{
	int result = true;



	if(result)
	{
		result = 入れ要請(pdrop);
	}

	if(result)
	{//使用に成功した
		sg_pDungeonSystem->メニューを閉じる();
		sg_pDungeonSystem->ターンエンド();
	}



	return true;
}


int cBox::入れ要請(pcDroping pdrop)
{

	if(残容量() > 0)
	{//まだ入る

		if(pdrop->被箱入れ可())
		{//入れ物じゃない
			if(me() != pdrop)
			{//自分自身でない
				int result = 0;

				if(state() == cDroping::STATE_NORMAL)
				{
					result = 入れる_メッセージ_通常(pdrop);
				}
				else if(state() == cDroping::STATE_CURSE)
				{
					result = 入れる_メッセージ_呪い(pdrop);
				}
				else if(state() == cDroping::STATE_GOOD)
				{
					result = 入れる_メッセージ_祝福(pdrop);
				}

				if(result)
				{
					if(!sg_pDungeonSystem->落ち物入れ要請(me(),pdrop))
					{
						return false;
					}
				}



				if(state() == cDroping::STATE_NORMAL)
				{
					result = 入れる_効果_通常(pdrop);
				}
				else if(state() == cDroping::STATE_CURSE)
				{
					result = 入れる_効果_呪い(pdrop);
				}
				else if(state() == cDroping::STATE_GOOD)
				{
					result = 入れる_効果_祝福(pdrop);
				}
			
				if(result)
				{//使用に成功した
					//sg_pDungeonSystem->落ち物破壊要請(me());
				}
				return result;
				
			}
			else
			{
				sg_pDungeonSystem->メッセージ(_T("これは入れられない。\n"));
			}
		}
		else
		{
			sg_pDungeonSystem->メッセージ(_T("これは入れられない。\n"));
		}
	}
	else
	{
		sg_pDungeonSystem->メッセージ(_T("これ以上は入らない。\n"));
	}
	return false;
}

*/

int cBox::入れる_効果_通常(pcDroping pdrop)
{
	//sg_pDungeonSystem->メッセージ(_T("cBox 入れる_効果_通常() 効果が設定されていません。\n"));
	return !cannotopen();
}
int cBox::入れる_メッセージ_通常(pcDroping pdrop)
{
	return 入れるメッセージ定型(pdrop);
}

int cBox::入れる_効果_呪い(pcDroping pdrop)
{
	//効果なし
	return !cannotopen();//ターンは過ぎる
}
int cBox::入れる_メッセージ_呪い(pcDroping pdrop)
{
	sg_pDungeonSystem->メッセージ(g_Lang(_T("Box呪いによる入れ不可メッセージ")));
	sg_pDungeonSystem->状態のみ識別(me(),false);
	return false;//入れるのに失敗する
}

int cBox::入れる_効果_祝福(pcDroping pdrop)
{
	return 入れる_効果_通常(pdrop);
}
int cBox::入れる_メッセージ_祝福(pcDroping pdrop)
{
	return 入れる_メッセージ_通常(pdrop);
}


int cBox::入れるメッセージ定型(pcDroping pdrop)
{
	if(cannotopen())
	{
		sg_pDungeonSystem->メッセージ(g_Lang(_T("Box電気による入れ不可メッセージ")));
		return false;
	}
	else
	{
		//sg_pDungeonSystem->AnimationManager().
			//Anime_PlaySE(_T("drink.wav"),pchara->足元地形()->place);
		
		map<tstring, StyleString> valiable;
		valiable[_T("ItemA")] = pdrop->FullName();
		valiable[_T("ItemB")] = FullName();
		g_Langメッセージ(_T("Box入れメッセージ"),valiable);
			
		//sg_pDungeonSystem->メッセージ
		//		(pdrop->FullName()+g_Lang(_T("Box入れメッセージ_前半"))+FullName()+g_Lang(_T("Box入れメッセージ_後半")));
		return true;
	}
}




/*
//----------------------------------------------------------
//見る
//----------------------------------------------------------
int cBox::見る(IDirect3DDevice9 *pDev)
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
int cBox::見る要請()
{
	//----------------------------------
	//使用不可能なリスト
	//----------------------------------

	vector<pcDroping> Objects = includedItem;

	int i;

	pcControlLayer pccl;
	pcListWindow pclw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(sg_pDungeonSystem->pDevice_D3D);


	//Object
	pccl->WindowList.push_back(pclw = pcListWindow(new cListWindow));

	pcDroping pdrop;
	if(Objects.size() != 0)
	{//なにかある
		for(i=0;i<Objects.size();i++)
		{
			//新規
			pcDropingFunc pcdf = pcDropingFunc(new cDropingFunc);
			pcdf->Subject = Objects[i];
			pcdf->Verb = _T("説明");
			pcdf->Objects.push_back(Objects[i]);

			//パック
			pdrop = DropingFuncToDroping(pcdf);

			pclw->pDroplist.push_back(pdrop);
		}	
	}
	else
	{//なにもない
		pclw->pDroplist.push_back(pcDroping(new cDropingNothing));
	}
	pclw->Init(sg_pDungeonSystem->pDevice_D3D, 20,  min(10,max(pclw->pDroplist.size(),capacity())));
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
int cBox::見る_効果_通常()
{
	//sg_pDungeonSystem->メッセージ(_T("cBox 見る_効果_通常() 効果が設定されていません。\n"));
	return !cannotopen();
}
int cBox::見る_メッセージ_通常()
{
	return 見るメッセージ定型();
}

int cBox::見る_効果_呪い()
{
	//効果なし
	return false;
}
int cBox::見る_メッセージ_呪い()
{
	sg_pDungeonSystem->メッセージ(g_Lang(_T("Box呪いによる見る不可メッセージ")));
	return false;
}

int cBox::見る_効果_祝福()
{
	return 見る_効果_通常();
}
int cBox::見る_メッセージ_祝福()
{
	return 見る_メッセージ_通常();
}


int cBox::見るメッセージ定型()
{
	//sg_pDungeonSystem->AnimationManager().
	//	Anime_PlaySE(_T("drink.wav"),pchara->足元地形()->place);
	//sg_pDungeonSystem->メッセージ
	//		(FullName()+_T(" を見た。\n"));
	if(cannotopen())
	{
		sg_pDungeonSystem->メッセージ(g_Lang(_T("Box電気による入れ不可メッセージ")));
		return false;
	}
	return true;
}

int cBox::開ける_効果_通常()
{
	//sg_pDungeonSystem->メッセージ(_T("cBox 開ける_効果_通常() 効果が設定されていません。\n"));
	return !cannotopen();
}
int cBox::開ける_メッセージ_通常()
{
	return 開けるメッセージ定型();
}

int cBox::開ける_効果_呪い()
{
	//効果なし
	return false;
}
int cBox::開ける_メッセージ_呪い()
{
	sg_pDungeonSystem->メッセージ(g_Lang(_T("Box呪いによる開ける不可メッセージ")));
	return false;
}

int cBox::開ける_効果_祝福()
{
	return 開ける_効果_通常();
}
int cBox::開ける_メッセージ_祝福()
{
	return 開ける_メッセージ_通常();
}


int cBox::開けるメッセージ定型()
{
	if(cannotopen())
	{
		sg_pDungeonSystem->メッセージ(g_Lang(_T("Box電気による入れ不可メッセージ")));
		return false;
	}

	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("open.wav"),me()->現在地形()->place);
	

	map<tstring, StyleString> valiable;
	valiable[_T("Item")] = FullName();
	g_Langメッセージ(_T("Box開けるメッセージ"),valiable);
	return true;
}

int cBox::出す_効果_通常(pcDroping pdrop, pcCharacter receiver)
{
	//sg_pDungeonSystem->メッセージ(_T("cBox 出す_効果_通常() 効果が設定されていません。\n"));
	return !cannotopen();
}
int cBox::出す_メッセージ_通常(pcDroping pdrop, pcCharacter receiver)
{
	return 出すメッセージ定型(pdrop,receiver);
}

int cBox::出す_効果_呪い(pcDroping pdrop, pcCharacter receiver)
{
	//効果なし
	return false;
}
int cBox::出す_メッセージ_呪い(pcDroping pdrop, pcCharacter receiver)
{
	sg_pDungeonSystem->メッセージ(g_Lang(_T("Box呪いによる出し不可メッセージ")));
	return false;
}

int cBox::出す_効果_祝福(pcDroping pdrop, pcCharacter receiver)
{
	return 出す_効果_通常(pdrop,receiver);
}
int cBox::出す_メッセージ_祝福(pcDroping pdrop, pcCharacter receiver)
{
	return 出す_メッセージ_通常(pdrop,receiver);
}


int cBox::出すメッセージ定型(pcDroping pdrop, pcCharacter receiver)
{
	if(cannotopen())
	{
		sg_pDungeonSystem->メッセージ(g_Lang(_T("Box電気による入れ不可メッセージ")));
		return false;
	}
	//sg_pDungeonSystem->AnimationManager().
	//	Anime_PlaySE(_T("drink.wav"),pchara->足元地形()->place);
	map<tstring, StyleString> valiable;
	valiable[_T("ItemA")] = pdrop->FullName();
	valiable[_T("ItemB")] = FullName();
	g_Langメッセージ(_T("Box出しメッセージ"),valiable);
	
	//sg_pDungeonSystem->メッセージ
	//		(pdrop->FullName()+g_Lang(_T("Box出しメッセージ_前半"))+FullName()+g_Lang(_T("Box出しメッセージ_後半")));
			
	return true;
}

/*
int cBox::入れ替え(IDirect3DDevice9 *pDev, pcDroping pdrop, pcDroping pdrop2)
{
	int result = true;

	//pdropは出す方
	if(pdrop2->被箱入れ可())
	{//入れ物じゃない
		if(me() != pdrop2)
		{//自分自身でない

			if(result)
			{
				result = 入れ替え要請(pdrop,pdrop2);
			}

			if(result)
			{//使用に成功した
				sg_pDungeonSystem->メニューを閉じる();
				sg_pDungeonSystem->ターンエンド();
			}
		}
		else
		{
			sg_pDungeonSystem->メッセージ(_T("これは入れられない。\n"));
		}
	}
	else
	{
		sg_pDungeonSystem->メッセージ(_T("これは入れられない。\n"));
	}

	return true;
}


int cBox::入れ替え要請(pcDroping pdrop, pcDroping pdrop2)
{

	sg_pDungeonSystem->落ち物入れ替え要請(pdrop,pdrop2);

	int result = 0;

	if(state() == cDroping::STATE_NORMAL)
	{
		result = 入れ替え_メッセージ_通常(pdrop,pdrop2);
	}
	else if(state() == cDroping::STATE_CURSE)
	{
		result = 入れ替え_メッセージ_呪い(pdrop,pdrop2);
	}
	else if(state() == cDroping::STATE_GOOD)
	{
		result = 入れ替え_メッセージ_祝福(pdrop,pdrop2);
	}



	if(state() == cDroping::STATE_NORMAL)
	{
		result = 入れ替え_効果_通常(pdrop,pdrop2);
	}
	else if(state() == cDroping::STATE_CURSE)
	{
		result = 入れ替え_効果_呪い(pdrop,pdrop2);
	}
	else if(state() == cDroping::STATE_GOOD)
	{
		result = 入れ替え_効果_祝福(pdrop,pdrop2);
	}

	if(result)
	{//使用に成功した
		//sg_pDungeonSystem->落ち物破壊要請(me());
	}

	return result;
}
*/

	//破壊される時true;
int cBox::入れ替え_効果_通常(pcDroping pdrop, pcDroping pdrop2)
{
	//sg_pDungeonSystem->メッセージ(_T("cBox 入れ替え_効果_通常() 効果が設定されていません。\n"));
	return !cannotopen();
}
int cBox::入れ替え_効果_呪い(pcDroping pdrop, pcDroping pdrop2)
{
	//効果なし
	return false;
}
int cBox::入れ替え_効果_祝福(pcDroping pdrop, pcDroping pdrop2)
{
	return 入れ替え_効果_通常(pdrop,pdrop2);
}
int cBox::入れ替え_メッセージ_通常(pcDroping pdrop, pcDroping pdrop2)
{
	return 入れ替えメッセージ定型(pdrop,pdrop2);
}
int cBox::入れ替え_メッセージ_呪い(pcDroping pdrop, pcDroping pdrop2)
{
	sg_pDungeonSystem->メッセージ((StyleString)_T("Box呪いによる入れ替え不可メッセージ"));
	return false;
}
int cBox::入れ替え_メッセージ_祝福(pcDroping pdrop, pcDroping pdrop2)
{
	return 入れ替え_メッセージ_通常(pdrop,pdrop2);
}
int cBox::入れ替えメッセージ定型(pcDroping pdrop, pcDroping pdrop2)
{
	if(cannotopen())
	{
		sg_pDungeonSystem->メッセージ(g_Lang(_T("Box電気による入れ不可メッセージ")));
		return false;
	}

	//sg_pDungeonSystem->AnimationManager().
	//	Anime_PlaySE(_T("drink.wav"),pchara->足元地形()->place);
	map<tstring, StyleString> valiable;
	valiable[_T("ItemA")] = pdrop->FullName();
	valiable[_T("ItemB")] = pdrop2->FullName();
	g_Langメッセージ(_T("Box入れ替えメッセージ"),valiable);
	
	//sg_pDungeonSystem->メッセージ
	//		(g_Lang(_T("Box入れ替えメッセージ_い"))+pdrop->FullName()+g_Lang(_T("Box入れ替えメッセージ_ろ"))+pdrop2->FullName()+g_Lang(_T("Box入れ替えメッセージ_は")));

	return true;
}


int cBox::衝突(pcCharacter pchara)
{
	int result = 0;

	衝突時_メッセージ(pchara);


	result = 衝突時効果(pchara);
	
	return result;
}

int cBox::衝突時効果(pcCharacter pchara)
{

	sg_pDungeonSystem->CutInM().CutIn(me(), 被衝撃_強_タイミング);

	return true;
}

int cBox::衝突時_メッセージ(pcCharacter pchara)
{


	return true;
}

/*
double cBox::CutInValue(int timing)
{
	double result = cItem::CutInValue(timing);

	return result;
}

int cBox::CutInEvent(int timing)
{
	cItem::CutInEvent(timing);

	if(timing == 被衝撃_強_タイミング)
	{
		BreakOut();
	}
	if(timing == 被衝撃_弱_タイミング)
	{
		if(random() < WEAK_CRASH_BREAKOUT_PROBABILITY)
		{
			BreakOut();
		}
	}
	return true;
}
*/
void cBox::CutIn(タイミング timing, cValiableField& valiable)
{
	cItem::CutIn(timing,valiable);

	if(timing == 被衝撃_強_タイミング)
	{
		BreakOut();
	}
	else if(timing == 被衝撃_弱_タイミング)
	{
		if(random() < 弱衝撃時破壊率())
		{
			BreakOut();
		}
	}
	else if(timing == コマンド挿入_子アイテム_タイミング)
	{
		if(出し可())
		{
			valiable.strvectors[変数_挿入コマンド].push_back(_T("出す"));
			valiable.intvectors[変数_挿入コマンド位置].push_back(1);
		}
	}
	else if(timing == 被攻撃接近直後_タイミング)
	{
		if(valiable.intsets.val(変数_属性).count(攻撃属性::電気)
			&& random() < 電気時帯静電気率() && !属性.count(落ち物属性::耐静電気)
			&& !cannotopen())
		{
			cValiableField newvaliable;
			newvaliable.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
			sg_pDungeonSystem->CutInM().CutIn(Holder(),箱帯静電気直前_タイミング,newvaliable);
			if(newvaliable.doubles.val(変数_汎用ブール))
			{
				map<tstring, StyleString> val;
				val[_T("Item")] = FullName();
				g_Langメッセージ(_T("Box電気付与メッセージ"),val);
				cannotopen() = 1;
			}
		}


	}
	else if(timing == フロア開始_タイミング)
	{
		cannotopen() = 0;
	}

	if(出し可())
	{
		int k;
		vector<pcDroping> vdrop = includedItem;
		for(k=0;k<vdrop.size();k++)
		{
			if(vdrop[k] == NULL) continue;
			sg_pDungeonSystem->CutInM().CutIn(vdrop[k],timing,valiable);
		}
	}
	
}
int cBox::異常状態解除()
{
	if (cannotopen()) {
		map<tstring, StyleString> val;
		val[_T("Item")] = FullName();
		g_Langメッセージ(_T("Box電気解消メッセージ"),val);
		cannotopen() = 0;
		return true;
	}
	return false;
}
double cBox::弱衝撃時破壊率()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		(tstring)_T("Box基本値"),(tstring)_T("弱衝撃時破壊率"),WEAK_CRASH_BREAKOUT_PROBABILITY);
}
double cBox::電気時帯静電気率()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		(tstring)_T("Box基本値"),(tstring)_T("電気時帯静電気率"),0.15);
}
int cBox::消費時幻想度加算量()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		(tstring)_T("Box基本値"),(tstring)_T("消費時幻想度加算量"),0);
}
int cBox::BreakOut()
{
	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("boxbreak.wav"),現在地形()->place);
	
	map<tstring, StyleString> valiable;
	valiable[_T("Item")] = FullName();
	g_Langメッセージ(_T("Box破壊メッセージ"),valiable);
	//sg_pDungeonSystem->メッセージ(FullName() + g_Lang(_T("Box破壊メッセージ")));
	
	sg_pDungeonSystem->落ち物ばら撒き要請(me());



	sg_pDungeonSystem->落ち物破壊要請(me());

	return true;
}


//スモールビュー
int cBox::Getsmollview(pcGameWindow& smollview)
{

	if(capacity() <= 0)
	{
		return false;
	}

	pcListWindow pclw;
	smollview = (pclw = pcListWindow(new cListWindow));

	pcDroping pdrop;
	if(includedItem.size())
	{//なにかある
		pclw->pDroplist = includedItem;
	}
	else
	{//なにもない
		pclw->pDroplist.push_back(pcDroping(new cDropingNothing));
	}
	pclw->enable = false;
	pclw->Init(sg_pDungeonSystem->pDevice_D3D, 10, capacity());
	pclw->setRight(SCREEN_X);
	pclw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(3));


	return true;
}


//--------------------------------------------------------
//実行群
//--------------------------------------------------------
