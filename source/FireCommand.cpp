#include "stdafx.h"

#include "Gameobjects.h"
#include "Language.h"

#include "FireCommand.h"
#include "cDropingFunc.h"
#include "cCommandDroping.h"
#include "cStringWindow.h"
#include "cTrap.h"
#include "cSaveStore.h"
#include "cMoney.h"
#include <assert.h>
#include <boost/pointer_cast.hpp>

#include "utility/debug.h"

int FireCommand(cRenderDevice *pDev, tstring verb, vector<pcDroping> &ObjectList, pcDroping pdrop)
{
	
	//共通
	if(verb == _T("説明")) 説明(pDev,pdrop);
	if(verb == _T("拾う")) 拾う(pDev);
	if(verb == _T("置く")) 置く(pDev,pdrop);
	if(verb == _T("投げる")) 投げる(pDev,pdrop);


	if(verb == _T("交換"))
	{
		if(pdrop->Holder() == NULL)
		{
			if(ObjectList.size())
				交換(pDev, ObjectList[0]);
		}
		else 交換(pDev,pdrop);
	}

	if(verb == _T("メモ")) メモ(pDev,pdrop);
	if(verb == _T("名前")) 名前(pDev,pdrop);

	//準共通
	if(verb == _T("装備"))
	{
		装備(pDev,pdrop);
	}

	if(verb == _T("はずす"))
	{
		はずす(pDev,pdrop);
	}




	int i;



	//book
	if(verb == _T("読む"))
	{
		読む(pDev,pdrop);
	}


	//box
	if(verb == _T("見る"))
	{
		見る(pDev,pdrop);
	}
	if(verb == _T("のぞく"))
	{
		のぞく(pDev,pdrop);
	}
	if(verb == _T("開ける"))
	{
		開ける(pDev,pdrop);
	}
	if(verb == _T("入れる"))
	{
		if(ObjectList.size())
			return 入れる(pDev, ObjectList[0],pdrop);
	}
	if(verb == _T("倉庫に入れる"))
	{
		if(ObjectList.size())
			return 倉庫に入れる(pDev, ObjectList[0],pdrop);
	}
	if(verb == _T("倉庫から出す"))
	{
		if(ObjectList.size())
			return 倉庫から出す(pDev, ObjectList[0],pdrop);
		else
			return 倉庫から出す(pDev, pdrop,NULLDROP);
	}
	if(verb == _T("出す"))
	{
		//if(ObjectList.size())
		//	出す(pDev, ObjectList[0],pdrop);
		出す(pDev, pdrop, pdrop->Parent());
	}
	if(verb == _T("まとめて出す"))
	{
		//if(ObjectList.size())
		//	出す(pDev, ObjectList[0],pdrop);
		まとめて出す(pDev, ObjectList);
	}

	if(verb == _T("入れ替え"))
	{
		if(ObjectList.size() >= 2)
			入れ替え(pDev, ObjectList[0], ObjectList[1],pdrop);
	}

	//bullet
	if(verb == _T("撃つ"))
	{
		撃つ(pDev,pdrop);
	}



	//drink
	if(verb == _T("飲む"))
	{
		飲む(pDev,pdrop);
	}
	if(verb == _T("汲む"))
	{
		汲む(pDev,pdrop);
	}
	if(verb == _T("カクテル"))
	{
		カクテル(pDev,ObjectList,pdrop);
	}

	//equipment
	if(verb == _T("攻撃用に装備"))
	{
		Equipment装備(pDev,EQUIP_PART_ATTACK,pdrop);
	}
	if(verb == _T("防御用に装備"))
	{
		Equipment装備(pDev,EQUIP_PART_DEFENSE,pdrop);
	}
	if(verb == _T("能力"))
	{
		能力(pDev,pdrop);
	}

	//food
	if(verb == _T("食べる"))
	{
		食べる(pDev,pdrop);
	}

	//spell
	if(verb == _T("宣言"))
	{
		宣言(pDev,ObjectList,pdrop);
	}
	if(verb == _T("破壊"))
	{
		破壊(pDev,pdrop);
	}
	for(i=0;i<SPELL_EQUIP_MAXNUM;i++)
	{
		pcSpell pcSpe = boost::dynamic_pointer_cast<cSpell>(pdrop);

		if(pcSpe != NULL)
		{
			if(verb == pcSpe->装備コマンド名称(i))
			{
				Spell装備(pDev,i,pdrop);
			}
		}
	}
	if(verb == _T("スキル"))
	{
		スキル(pDev,ObjectList,pdrop);
	}

	//stair
	if(verb == _T("踏む"))
	{
		踏む(pDev,pdrop);
	}

	if(verb == _T("上る"))
	{
		上る(pDev,pdrop);
	}
	if(verb == _T("下りる"))
	{
		下りる(pDev,pdrop);
	}
	if(verb == _T("やめる"))
	{
		やめる(pDev,pdrop);
	}
	if(verb == _T("一旦中断する"))
	{
		一旦中断する(pDev);
	}

	if(verb == _T("キープする"))
	{
		キープする(pDev,pdrop);
	}
	if(verb == _T("キープをやめる"))
	{
		キープをやめる(pDev,pdrop);
	}
	if(verb == _T("ポイント交換"))
	{
		ポイント交換(pDev,pdrop);
	}
	if(verb == _T("買う"))
	{
		買う(pDev,pdrop);
	}
	if(verb == _T("売る"))
	{
		売る(pDev,pdrop);
	}
	if(verb == _T("店買う"))
	{
		店買う(pDev,pdrop);
	}
	if(verb == _T("店売る"))
	{
		店売る(pDev,pdrop);
	}
	if(verb == _T("まとめて買う"))
	{
		まとめて買う(pDev, ObjectList);
	}
	if(verb == _T("まとめて売る"))
	{
		まとめて売る(pDev, ObjectList);
	}
	if(verb == _T("まとめて店買う"))
	{
		まとめて店買う(pDev, ObjectList);
	}
	if(verb == _T("まとめて店売る"))
	{
		まとめて店売る(pDev, ObjectList);
	}
	return true;
}


//Fireできるときtrue;
int isCanFireCommand(pcDroping subject, vector<pcDroping> &ObjectList, tstring verb)
{

	if(verb == _T("入れる")
		//|| verb == _T("出す")
		
		
		)
	{
		if(ObjectList.size() < 1)
		{
			return false;
		}

	}

	if(verb == _T("入れ替え")
		
		)
	{
		if(ObjectList.size() < 2)
		{
			return false;
		}

	}

	if(verb == _T("交換")
		&& subject->Holder() == NULL
		)
	{
		if(ObjectList.size() < 1)
		{
			return false;
		}

	}

	if(verb == _T("宣言"))
	{
		pcSpell pcSpe = boost::dynamic_pointer_cast<cSpell>(subject);

		if(pcSpe != NULL)
		{
			return pcSpe->オブジェクト宣言可能(ObjectList.size());
		}
	}

	if(verb == _T("スキル"))
	{
		pcSpell pcSpe = boost::dynamic_pointer_cast<cSpell>(subject);

		if(pcSpe != NULL)
		{
			return pcSpe->オブジェクトスキル可能(ObjectList.size());
		}
	}

	if(verb == _T("カクテル"))
	{
		if(ObjectList.size() < 2)
		{
			return false;
		}
	}

	return true;

}


void GetmenuFuncObjectsList(pcDroping subject, vector<vector<pcDroping>> &ObjectListlist, vector<StyleString>& listExplain, bool &MultiObj, int objectsize, tstring verb)
{

	if(verb == _T("入れる"))
	{
		if(objectsize == 0)
		{//一つ目


			ObjectListlist.push_back(sg_pDungeonSystem->pPlayerChara()->holdItem);
			listExplain.push_back(g_Lang(_T("何を？_[複数選択可] (持ち物)")));

			MultiObj = true;
			if(sg_pDungeonSystem->pPlayerChara()->足元() != NULL)
			{
				vector<pcDroping> 足元リスト;
				足元リスト.push_back(sg_pDungeonSystem->pPlayerChara()->足元());
				ObjectListlist.push_back(足元リスト);
			}

			listExplain.push_back(g_Lang(_T("何を？_[複数選択可] (足元)")));
			
			/*
			if(subject->倉庫の中())
			{
				vector< vector<pcDroping> > & storeItem = g_pSaveData()->pSaveStore()->storeItem;
				int i,size = storeItem.size();

				StyleString caption;
				for(i=0;i<size;i++)
				{
					ObjectListlist.push_back(sg_pDungeonSystem->pPlayerChara()->holdItem);
					caption.clear();
					caption += g_Lang(_T("何を？_[複数選択可] (倉庫)"));
					caption += _T(" ");
					caption += setStyle(i+1);
					caption += _T(". ");
					if(g_pSaveData()->pSaveStore()->storeMemo[i]->getDataSize() > 0)
					{
						caption +=  _T(" (");
						caption += g_pSaveData()->pSaveStore()->storeMemo[i]->str();
						caption +=  _T(") ");
					}
					listExplain.push_back(caption);
				}
			}
			*/
			
			return;
		}

	}

	if(verb == _T("入れ替え"))
	{
		if(objectsize == 0)
		{

			MultiObj = false;
			ObjectListlist.push_back(subject->includedItem);
			listExplain.push_back(g_Lang(_T("どれと？ (中身)")));
			return;
		}
		else if(objectsize == 1)
		{
			MultiObj = false;

			ObjectListlist.push_back(sg_pDungeonSystem->pPlayerChara()->holdItem);
			listExplain.push_back(g_Lang(_T("どれを？ (持ち物)")));

			if(sg_pDungeonSystem->pPlayerChara()->足元() != NULL)
			{
				vector<pcDroping> 足元リスト;
				足元リスト.push_back(sg_pDungeonSystem->pPlayerChara()->足元());
				ObjectListlist.push_back(足元リスト);
			}

			listExplain.push_back(g_Lang(_T("どれを？ (足元)")));
			return;
		}
	}

	if(verb == _T("交換"))
	{
		if(objectsize == 0)
		{//一つ目


			ObjectListlist.push_back(sg_pDungeonSystem->pPlayerChara()->holdItem);
			listExplain.push_back(g_Lang(_T("どれと？ (持ち物)")));

			MultiObj = false;
			return;
		}

	}

	if(verb == _T("カクテル"))
	{
		if(objectsize == 0)
		{//一つ目


			ObjectListlist.push_back(sg_pDungeonSystem->pPlayerChara()->holdItem);
			listExplain.push_back(g_Lang(_T("どれと？ (持ち物)")));
			if(sg_pDungeonSystem->pPlayerChara()->足元() != NULL)
			{
				vector<pcDroping> 足元リスト;
				足元リスト.push_back(sg_pDungeonSystem->pPlayerChara()->足元());
				ObjectListlist.push_back(足元リスト);
			}

			listExplain.push_back(g_Lang(_T("どれを？ (足元)")));

			MultiObj = false;
			return;
		}
		if(objectsize == 1)
		{//二つ目


			ObjectListlist.push_back(sg_pDungeonSystem->pPlayerChara()->holdItem);
			listExplain.push_back(g_Lang(_T("どれを？ (持ち物)")));
			if(sg_pDungeonSystem->pPlayerChara()->足元() != NULL)
			{
				vector<pcDroping> 足元リスト;
				足元リスト.push_back(sg_pDungeonSystem->pPlayerChara()->足元());
				ObjectListlist.push_back(足元リスト);
			}

			listExplain.push_back(g_Lang(_T("どれを？ (足元)")));

			MultiObj = false;
			return;
		}
	}

	if(verb == _T("宣言"))
	{
		pcSpell pcSpe = boost::dynamic_pointer_cast<cSpell>(subject);

		if(pcSpe != NULL)
		{


			pcSpe->宣言オブジェクト候補(ObjectListlist, listExplain, MultiObj, objectsize, verb);

			return;
		}
	}

	if(verb == _T("スキル"))
	{
		pcSpell pcSpe = boost::dynamic_pointer_cast<cSpell>(subject);

		if(pcSpe != NULL)
		{


			pcSpe->スキルオブジェクト候補(ObjectListlist, listExplain, MultiObj, objectsize, verb);

			return;
		}
	}
	//return _T("");

	/*
	if(verb == _T("出す"))
	{
		if(objectsize == 0)
		{
			MultiObj = true;
			ObjectList = includedItem;
			return _T("どれを？_[複数選択可]");
		}

	}
	*/

}


//---------------------------------------------
//共通
int 説明(cRenderDevice *pDev, pcDroping pdrop)
{
	pcControlLayer pccl;
	pcGameWindow pcgw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	//新規ゲームウィンドウ
	pccl->WindowList.push_back(pcgw = pcGameWindow(new cGameWindow));
	pcgw->Init(pDev, 28, 16);


	pcgw->Text() = pdrop->longExplanation();
	pdrop->longExplanationExtra(pcgw->vText);

	pcgw->CenterX = SCREEN_X/2;
	pcgw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(1));
	
	return true;
}

int 置く(cRenderDevice *pDev, pcDroping pdrop)
{

	if(sg_pDungeonSystem->足元設置要請(sg_pDungeonSystem->pPlayerChara(), pdrop))
	{
		map<tstring, StyleString> valiable;
		valiable[_T("Item")] = pdrop->FullName();
		g_Langメッセージ(_T("アイテム置くメッセージ"),valiable);
				
		sg_pDungeonSystem->アイテム強制マッピング(pdrop);

		//sg_pDungeonSystem->メッセージ(_T("足元に")+pdrop->FullName()+_T("を置いた。\n"));
		sg_pDungeonSystem->メニューを閉じる();
		sg_pDungeonSystem->ターンエンド();
		return true;
	}

	sg_pDungeonSystem->メニューを閉じる();

	return false;
}

int 拾う(cRenderDevice *pDev)
{
	pcDroping pdrop = sg_pDungeonSystem->pPlayerChara()->足元();
	if(pdrop == NULL) return false;
	if(sg_pDungeonSystem->足元拾得要請(sg_pDungeonSystem->pPlayerChara()))
	{
		
		sg_pDungeonSystem->AnimationManager().
			Anime_PlaySE(_T("pickup.wav"),sg_pDungeonSystem->pPlayerChara()->足元地形()->place);
		if(!pdrop->broken)
		{//まとめられて無い
			map<tstring, StyleString> valiable;
			valiable[_T("Item")] = pdrop->FullName();
			g_Langメッセージ(_T("アイテム拾うメッセージ"),valiable);
			
			//sg_pDungeonSystem->メッセージ(pdrop->FullName() + _T("  を拾った。\n"));
		}
		sg_pDungeonSystem->メニューリフレッシュ();
		


		return true;
	}
	
	if(!pdrop->被拾得可())
	{
		map<tstring, StyleString> valiable;
		valiable[_T("Item")] = pdrop->FullName();
		g_Langメッセージ(_T("アイテム拾う失敗拾得不可メッセージ"),valiable);
		sg_pDungeonSystem->メニューを閉じる();
	}
	else if(!sg_pDungeonSystem->pPlayerChara()->持ち物余白あり())
	{
		map<tstring, StyleString> valiable;
		valiable[_T("Item")] = pdrop->FullName();
		g_Langメッセージ(_T("アイテム拾う失敗持ち物余白なしメッセージ"),valiable);
		sg_pDungeonSystem->メニューを閉じる();
		//sg_pDungeonSystem->メッセージ(_T("持ち物がいっぱいで ") + pdrop->FullName() + _T("は拾えなかった。\n"));
	}
	else
	{
		map<tstring, StyleString> valiable;
		valiable[_T("Item")] = pdrop->FullName();
		g_Langメッセージ(_T("アイテム拾う失敗メッセージ"),valiable);
		sg_pDungeonSystem->メニューを閉じる();
		//sg_pDungeonSystem->メッセージ(pdrop->FullName() + _T("は拾えなかった。\n"));
	}
	return false;
}

int 投げる(cRenderDevice *pDev, pcDroping pdrop)
{
	sg_pDungeonSystem->メニューを閉じる();
	sg_pDungeonSystem->AnimationManager().
			Anime_PlaySE(_T("throw.wav"),pdrop->現在地形()->place);
		
	if(sg_pDungeonSystem->投擲要請(sg_pDungeonSystem->pPlayerChara(), pdrop))
	{
		sg_pDungeonSystem->ターンエンド();
		return true;
	}
	else if(!pdrop->被拾得可())
	{
		map<tstring, StyleString> valiable;
		valiable[_T("Item")] = pdrop->FullName();
		g_Langメッセージ(_T("アイテム拾う失敗拾得不可メッセージ"),valiable);
	}

	sg_pDungeonSystem->メニューを閉じる();
	return false;
}

int 交換(cRenderDevice *pDev, pcDroping pdrop)
{
	pcDroping pdrop2 = sg_pDungeonSystem->pPlayerChara()->足元();
	if(sg_pDungeonSystem->足元交換要請(sg_pDungeonSystem->pPlayerChara(), pdrop))
	{
		map<tstring, StyleString> valiable;
		valiable[_T("ItemA")] = pdrop->FullName();
		valiable[_T("ItemB")] = pdrop2->FullName();
		g_Langメッセージ(_T("アイテム交換メッセージ"),valiable);
		
		//sg_pDungeonSystem->メッセージ(_T("足元に")+pdrop->FullName()+_T("を置いて、")
		//	+pdrop2->FullName() + _T("と交換した。\n"));
		sg_pDungeonSystem->アイテム強制マッピング(pdrop);
		
		sg_pDungeonSystem->メニューを閉じる();
		sg_pDungeonSystem->ターンエンド();
		return true;
	}

	map<tstring, StyleString> valiable;
	valiable[_T("ItemA")] = pdrop->FullName();
	valiable[_T("ItemB")] = pdrop2->FullName();
	g_Langメッセージ(_T("アイテム交換失敗メッセージ"),valiable);
	sg_pDungeonSystem->メニューを閉じる();

	return false;
}
int メモ(cRenderDevice *pDev, pcDroping pdrop)
{
	pcControlLayer pccl;
	//pcSelectWindow pcsw;
	pcStringWindow pcstrw;


	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));

	pccl->Init(pDev);
	pccl->InputFreeFlag = true;
	pccl->notCancelToPop = true;
	//pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));
	pccl->WindowList.push_back(pcstrw = pcStringWindow(new cStringWindow));
	pcstrw->targetStr = pdrop->pmemo;//これを先にすると初期文字がターゲットになる
	pcstrw->Init(pDev);
	
	return false;
}
int 名前(cRenderDevice *pDev, pcDroping pdrop)
{
	pcControlLayer pccl;
	//pcSelectWindow pcsw;
	pcStringWindow pcstrw;


	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));

	pccl->Init(pDev);
	pccl->InputFreeFlag = true;
	pccl->notCancelToPop = true;
	//pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));
	pccl->WindowList.push_back(pcstrw = pcStringWindow(new cStringWindow));
	
	pcMemoString pmemo = sg_pDungeonSystem->DataBase.アイテムメモ(pdrop->ID());
	if(pmemo == NULL)
	{
		pmemo = pcMemoString(new cMemoString);
		sg_pDungeonSystem->DataBase.アイテムメモ追加(pdrop->ID(), pmemo);
	}
	
	pcstrw->targetStr = pmemo;//これを先にすると初期文字がターゲットになる
	pcstrw->Init(pDev);


	return false;
}


//準共通
int 装備(cRenderDevice *pDev, pcDroping pdrop)
{
	pcBullet pcBul = boost::dynamic_pointer_cast<cBullet>(pdrop);
	pcEquipment pcEqu = boost::dynamic_pointer_cast<cEquipment>(pdrop);
	pcSpell pcSpe = boost::dynamic_pointer_cast<cSpell>(pdrop);


	if(pcBul != NULL)
	{
		return Bullet装備(pDev,pdrop);
	}
	else if(pcEqu != NULL)
	{
		if(pcEqu->両手装備())
		{
			Equipment装備(pDev,EQUIP_PART_ATTACK,pdrop);//EQUIP_PART_ATTACKは意味なし
		}
		else
		{
			Equipment装備選択(pDev,pdrop);
		}
	}
	else if(pcSpe != NULL)
	{
		Spell装備選択(pDev,pdrop);
	}
	else
	{
		assert(!_T("error_装備品以外を装備"));
	}
	
	return false;
}
int はずす(cRenderDevice *pDev, pcDroping pdrop)
{
	pcBullet pcBul = boost::dynamic_pointer_cast<cBullet>(pdrop);
	pcEquipment pcEqu = boost::dynamic_pointer_cast<cEquipment>(pdrop);
	pcSpell pcSpl = boost::dynamic_pointer_cast<cSpell>(pdrop);


	if(pcEqu != NULL)
	{
		return Equipmentはずす(pDev,pdrop);
	}
	else if(pcBul != NULL)
	{
		return Bulletはずす(pDev,pdrop);
	}
	else if(pcSpl != NULL)
	{
		return Spellはずす(pDev,pdrop);
	}
	else
	{
		assert(!_T("error_装備品以外をはずす"));
	}
	
	return false;
}


//---------------------------------------------
//book
int 読む(cRenderDevice *pDev, pcDroping pdrop)
{
	int result = true;
	pcBook pcbk = boost::dynamic_pointer_cast<cBook>(pdrop);
	if(pcbk == NULL)
	{
		assert(!_T("error_本以外を読む"));
		return false;
	}

	cValiableField val;
	val.doubles.dim(変数_汎用ブール) = result;
	sg_pDungeonSystem->CutInM().CutIn(sg_pDungeonSystem->pPlayerChara(),読み直前_タイミング,val);
	if(!val.doubles.dim(変数_汎用ブール)) return false;


	if(pdrop->state() == cDroping::STATE_NORMAL)
	{
		result = pcbk->読む_メッセージ_通常(sg_pDungeonSystem->pPlayerChara());
	}
	else if(pdrop->state() == cDroping::STATE_CURSE)
	{
		result = pcbk->読む_メッセージ_呪い(sg_pDungeonSystem->pPlayerChara());
	}
	else if(pdrop->state() == cDroping::STATE_GOOD)
	{
		result = pcbk->読む_メッセージ_祝福(sg_pDungeonSystem->pPlayerChara());
	}
	else
	{
		OnAssert(_T(__FILE__),__LINE__,false,_T(" state()に異常があります。"));
	}

	if(result)
	{//使用に成功した
		sg_pDungeonSystem->メニューを閉じる();
		sg_pDungeonSystem->ターンエンド();
	


		if(pdrop->quality() > 0)
		{//残量あり
			pcbk->溜めエフェクト(sg_pDungeonSystem->pPlayerChara());
			result = 読み要請(sg_pDungeonSystem->pPlayerChara(),pdrop);
		}
		else
		{//残量なし
			//効果なし
			pcbk->弾切れエフェクト(sg_pDungeonSystem->pPlayerChara());
			
			if(!pcbk->修正値識別済み())
			{
				sg_pDungeonSystem->修正のみ識別(pdrop,false);
			}
			result = true;
		}

	}

	sg_pDungeonSystem->メニューを閉じる();

	return true;
}
int 読み要請(pcCharacter pchara, pcDroping pdrop)
{
	pcBook pcbk = boost::dynamic_pointer_cast<cBook>(pdrop);
	if(pcbk == NULL)
	{
		assert(!_T("error_本以外を読む"));
		return false;
	}
	int result = 0;



	if(pdrop->state() == cDroping::STATE_NORMAL)
	{
		result = pcbk->読む_効果_通常(pchara);
	}
	else if(pdrop->state() == cDroping::STATE_CURSE)
	{
		result = pcbk->読む_効果_呪い(pchara);
	}
	else if(pdrop->state() == cDroping::STATE_GOOD)
	{
		result = pcbk->読む_効果_祝福(pchara);
	}

	if(result)
	{//使用に成功した
		

		if(pcbk->state() == cDroping::STATE_GOOD)
		{
			pcbk->stategoodRunoutprocess();
		}

	}

	return result;
}

//---------------------------------------------
//Box
int 入れる(cRenderDevice *pDev, pcDroping pdrop, pcDroping pbox)
{
	int result = true;
	pcBox pcbx = boost::dynamic_pointer_cast<cBox>(pbox);
	if(pcbx == NULL)
	{
		assert(!_T("error_箱以外を入れる"));
		return false;
	}


	if(result)
	{
		result = 入れ要請(pdrop, pbox);
	}

	if(result)
	{//使用に成功した
		//sg_pDungeonSystem->メニューを閉じる();
		sg_pDungeonSystem->ターンエンド();
	}

	sg_pDungeonSystem->メニューを閉じる();//失敗しても閉じる

	return result;
}

int 入れ要請(pcDroping pdrop, pcDroping pbox)
{
	pcBox pcbx = boost::dynamic_pointer_cast<cBox>(pbox);
	if(pcbx == NULL)
	{
		assert(!_T("error_箱以外を入れる"));
		return false;
	}

	if(pcbx->残容量() > 0)
	{//まだ入る

		
		
		if(pdrop->被拾得可() && pcbx->箱入れ可(pdrop))
		{//入れ物じゃない

			pdrop->被箱入れ準備();
			if(pdrop->被箱入れ可())
			{
				if(pbox != pdrop)
				{//自分自身でない
					int result = 0;

					if(pbox->state() == cDroping::STATE_NORMAL)
					{
						result = pcbx->入れる_メッセージ_通常(pdrop);
					}
					else if(pbox->state() == cDroping::STATE_CURSE)
					{
						result = pcbx->入れる_メッセージ_呪い(pdrop);
					}
					else if(pbox->state() == cDroping::STATE_GOOD)
					{
						result = pcbx->入れる_メッセージ_祝福(pdrop);
					}

					if(pcbx->cannotopen())
					{
						return false;
					}

					if(result)
					{
						if(!sg_pDungeonSystem->落ち物入れ要請(pbox,pdrop))
						{
							return false;
						}
					}



					if(pbox->state() == cDroping::STATE_NORMAL)
					{
						result = pcbx->入れる_効果_通常(pdrop);
					}
					else if(pbox->state() == cDroping::STATE_CURSE)
					{
						result = pcbx->入れる_効果_呪い(pdrop);
					}
					else if(pbox->state() == cDroping::STATE_GOOD)
					{
						result = pcbx->入れる_効果_祝福(pdrop);
					}
				
					if(result)
					{//使用に成功した
						//sg_pDungeonSystem->落ち物破壊要請(me());
					}
					return result;
					
				}
				else
				{
					map<tstring, StyleString> valiable;
					valiable[_T("Box")] = pbox->FullName();
					valiable[_T("Item")] = pdrop->FullName();
					sg_pDungeonSystem->メッセージ(g_Lang(_T("Box入れ要請拒否メッセージ"),valiable));
				}
			}
			else
			{
			
				map<tstring, StyleString> valiable;
				valiable[_T("Box")] = pbox->FullName();
				valiable[_T("Item")] = pdrop->FullName();

				if(pdrop->属性.count(落ち物属性::装備品)
					&& (pdrop->装備されている_攻撃用() || pdrop->装備されている_防御用()))
				{
					sg_pDungeonSystem->メッセージ(g_Lang(_T("Box装備入れ要請拒否メッセージ"),valiable));
				}
				else
				{
					sg_pDungeonSystem->メッセージ(g_Lang(_T("Box入れ要請拒否メッセージ"),valiable));
				}
			
			}
		}
		else
		{//入れ拒否された

			map<tstring, StyleString> valiable;
			valiable[_T("Box")] = pbox->FullName();
			valiable[_T("Item")] = pdrop->FullName();

			if(pdrop->属性.count(落ち物属性::装備品)
				&& (pdrop->装備されている_攻撃用() || pdrop->装備されている_防御用()))
			{
				sg_pDungeonSystem->メッセージ(g_Lang(_T("Box装備入れ要請拒否メッセージ"),valiable));
			}
			else
			{
				sg_pDungeonSystem->メッセージ(g_Lang(_T("Box入れ要請拒否メッセージ"),valiable));
			}
		}
	}
	else
	{//もう入らない
		map<tstring, StyleString> valiable;
		valiable[_T("Box")] = pbox->FullName();
		valiable[_T("Item")] = pdrop->FullName();
		sg_pDungeonSystem->メッセージ(g_Lang(_T("Box入れ要請拒否2メッセージ"),valiable));
	}
	sg_pDungeonSystem->メニューを閉じる();
	return false;
}
int 倉庫に入れる(cRenderDevice *pDev, pcDroping pdrop, pcDroping pstore)
{
	int result = true;
	pcStoreDroping psd = boost::dynamic_pointer_cast<cStoreDroping>(pstore);
	if(psd == NULL)
	{
		assert(!_T("error_倉庫以外に入れる"));
		return false;
	}


	result = sg_pDungeonSystem->倉庫入れ要請(psd->store_, psd->capacity_, pdrop);

	if(!result)
	{
		map<tstring, StyleString> valiable;
		valiable[_T("Item")] = pdrop->FullName();
		sg_pDungeonSystem->メッセージ(g_Lang(_T("倉庫入れ要請拒否メッセージ"),valiable));
	
		return false;
	}

	map<tstring, StyleString> valiable;
	valiable[_T("Item")] = pdrop->FullName();
	sg_pDungeonSystem->メッセージ(g_Lang(_T("倉庫入れメッセージ"),valiable));

	sg_pDungeonSystem->メニューを閉じる();//失敗しても閉じる

	return result;
}
int 倉庫にまとめて入れる(cRenderDevice *pDev, vector<pcDroping>& pdroplist, pcDroping pstore)
{
	int i;
	for(i=0;i<pdroplist.size();i++)
	{
		if(!倉庫に入れる(pDev, pdroplist[i], pstore)) break;
	}

	return true;
}

int 見る(cRenderDevice *pDev, pcDroping pbox)
{
	pcBox pcbx = boost::dynamic_pointer_cast<cBox>(pbox);
	if(pcbx == NULL)
	{
		assert(!_T("error_箱以外を見る"));
		return false;
	}

	int result = 0;


	if(pbox->state() == cDroping::STATE_NORMAL)
	{
		result = pcbx->見る_メッセージ_通常();
	}
	else if(pbox->state() == cDroping::STATE_CURSE)
	{
		result = pcbx->見る_メッセージ_呪い();
	}
	else if(pbox->state() == cDroping::STATE_GOOD)
	{
		result = pcbx->見る_メッセージ_祝福();
	}



	if(result)
	{
		result = 見る要請(pbox);
	}
	else
	{
		sg_pDungeonSystem->メニューを閉じる();
	}

	if(result)
	{//使用に成功した
		//sg_pDungeonSystem->メニューを閉じる();
		//sg_pDungeonSystem->ターンエンド();
	}


	return true;
}
int 見る要請(pcDroping pbox)
{
	pcBox pcbx = boost::dynamic_pointer_cast<cBox>(pbox);
	if(pcbx == NULL)
	{
		assert(!_T("error_箱以外を見る"));
		return false;
	}

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
	if(pbox->includedItem.size() != 0)
	{//なにかある
		pclw->pDroplist = pbox->includedItem;
	}
	else
	{//なにもない
		pclw->pDroplist.push_back(pcDroping(new cDropingNothing));
	}
	pclw->Init(sg_pDungeonSystem->pDevice_D3D, 20, min(10,max(pclw->pDroplist.size(),pcbx->capacity())));
	pclw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(2));
	pclw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(3));


	pclw->multiverbs.push_back(_T("まとめて出す"));
	pclw->MultiSelect = true;



	int result = 0;
	if(pbox->state() == cDroping::STATE_NORMAL)
	{
		result = pcbx->見る_効果_通常();
	}
	else if(pbox->state() == cDroping::STATE_CURSE)
	{
		result = pcbx->見る_効果_呪い();
	}
	else if(pbox->state() == cDroping::STATE_GOOD)
	{
		result = pcbx->見る_効果_祝福();
	}

	if(result)
	{//使用に成功した
		//sg_pDungeonSystem->落ち物破壊要請(me());
	}

	return result;
}

int のぞく(cRenderDevice *pDev, pcDroping pbox)
{
	pcBox pcbx = boost::dynamic_pointer_cast<cBox>(pbox);
	if(pcbx == NULL)
	{
		assert(!_T("error_箱以外を見る"));
		return false;
	}

	int result = 0;


	if(pbox->state() == cDroping::STATE_NORMAL)
	{
		result = pcbx->見る_メッセージ_通常();
	}
	else if(pbox->state() == cDroping::STATE_CURSE)
	{
		result = pcbx->見る_メッセージ_呪い();
	}
	else if(pbox->state() == cDroping::STATE_GOOD)
	{
		result = pcbx->見る_メッセージ_祝福();
	}


	if(result)
	{
		result = のぞく要請(pbox);
	}

	if(result)
	{//使用に成功した
		//sg_pDungeonSystem->メニューを閉じる();
		//sg_pDungeonSystem->ターンエンド();
	}

	return true;
}


int のぞく要請(pcDroping pbox)
{
	pcBox pcbx = boost::dynamic_pointer_cast<cBox>(pbox);
	if(pcbx == NULL)
	{
		assert(!_T("error_箱以外を見る"));
		return false;
	}

	//----------------------------------
	//使用不可能なリスト
	//----------------------------------

	vector<pcDroping> Objects = pbox->includedItem;

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
	pclw->Init(sg_pDungeonSystem->pDevice_D3D, 20,  min(10,max(pclw->pDroplist.size(),pcbx->capacity())));
	pclw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(2));
	pclw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(1));




	int result = 0;
	if(pbox->state() == cDroping::STATE_NORMAL)
	{
		result = pcbx->見る_効果_通常();
	}
	else if(pbox->state() == cDroping::STATE_CURSE)
	{
		result = pcbx->見る_効果_呪い();
	}
	else if(pbox->state() == cDroping::STATE_GOOD)
	{
		result = pcbx->見る_効果_祝福();
	}

	if(result)
	{//使用に成功した
		//sg_pDungeonSystem->落ち物破壊要請(me());
	}

	return result;
}

int 開ける(cRenderDevice *pDev, pcDroping pbox)
{
	pcBox pcbx = boost::dynamic_pointer_cast<cBox>(pbox);
	if(pcbx == NULL)
	{
		assert(!_T("error_箱以外を開ける"));
		return false;
	}

	int result = 0;


	if(pbox->state() == cDroping::STATE_NORMAL)
	{
		result = pcbx->開ける_メッセージ_通常();
	}
	else if(pbox->state() == cDroping::STATE_CURSE)
	{
		result = pcbx->開ける_メッセージ_呪い();
	}
	else if(pbox->state() == cDroping::STATE_GOOD)
	{
		result = pcbx->開ける_メッセージ_祝福();
	}


	if(result)
	{
		result = 開け要請(pbox);
	}
	if(result == 2) return true;//混酒の箱の例外処理

	if(result)
	{//使用に成功した
		sg_pDungeonSystem->メニューを閉じる();
		sg_pDungeonSystem->ターンエンド();
	}
	sg_pDungeonSystem->メニューを閉じる();

	return true;
}
int 開け要請(pcDroping pbox)
{
	pcBox pcbx = boost::dynamic_pointer_cast<cBox>(pbox);
	if(pcbx == NULL)
	{
		assert(!_T("error_箱以外を開ける"));
		return false;
	}



	int result = 0;
	if(pbox->state() == cDroping::STATE_NORMAL)
	{
		result = pcbx->開ける_効果_通常();
	}
	else if(pbox->state() == cDroping::STATE_CURSE)
	{
		result = pcbx->開ける_効果_呪い();
	}
	else if(pbox->state() == cDroping::STATE_GOOD)
	{
		result = pcbx->開ける_効果_祝福();
	}

	if(result)
	{//使用に成功した
		//sg_pDungeonSystem->落ち物破壊要請(pbox);
	}

	return result;
}

int 倉庫から出す(cRenderDevice *pDev, pcDroping pdrop, pcDroping pstore)
{
	int result = true;
	pcStoreDroping psd = boost::dynamic_pointer_cast<cStoreDroping>(pstore);
	if(psd == NULL)
	{
		//ない
		vector<vector<pcDroping>>& vstore = g_pSaveData()->pSaveStore()->storeItem;
		vector<vector<pcDroping>>::iterator tgtstore = vstore.end();
		int i,size = vstore.size();
		for(i=0;i<size;i++)
		{
			vector<pcDroping>& store = vstore[i];
			//倉庫検索
			vector<pcDroping>::iterator pdpitr = store.begin();
			for(;pdpitr != store.end();)
			{
				if((*pdpitr) == pdrop)
				{//検索ヒット //しないかもしれない
					tgtstore = vstore.begin()+i;
					break;
				}
				pdpitr++;
			}

			if(tgtstore != vstore.end()) break;
		}
		if(tgtstore == vstore.end())
		{
			assert(!_T("error_倉庫から出す"));
			return false;
		}

		result = sg_pDungeonSystem->倉庫出し要請(*tgtstore, pdrop);

	}
	else
	{

		result = sg_pDungeonSystem->倉庫出し要請(psd->store_, pdrop);
	}

	if(!result)
	{
		map<tstring, StyleString> valiable;
		valiable[_T("Item")] = pdrop->FullName();
		sg_pDungeonSystem->メッセージ(g_Lang(_T("倉庫出し要請拒否メッセージ"),valiable));
	
		return false;
	}

	map<tstring, StyleString> valiable;
	valiable[_T("Item")] = pdrop->FullName();
	sg_pDungeonSystem->メッセージ(g_Lang(_T("倉庫出しメッセージ"),valiable));


	sg_pDungeonSystem->メニューを閉じる();//失敗しても閉じる

	return result;
}
int 倉庫からまとめて出す(cRenderDevice *pDev, vector<pcDroping>& pdroplist, pcDroping pstore)
{
	int i;
	for(i=0;i<pdroplist.size();i++)
	{
		if(!倉庫から出す(pDev, pdroplist[i], pstore)) break;
	}

	return true;
}

int 出す(cRenderDevice *pDev, pcDroping pdrop, pcDroping pbox)
{
	int result = true;

	pcCharacter receiver = sg_pDungeonSystem->pPlayerChara();
	


	if(result)
	{
		result = 出し要請(pdrop,receiver,pbox);
	}

	if(result)
	{//使用に成功した
		
		sg_pDungeonSystem->ターンエンド();
	}
	sg_pDungeonSystem->メニューを閉じる();
	

	return result;
}

//まとめて
int まとめて出す(cRenderDevice *pDev, vector<pcDroping>& pdroplist)
{

	int i;
	for(i=0;i<pdroplist.size();i++)
	{
		if(!出す(pDev, pdroplist[i], pdroplist[i]->Parent())) break;
	}

	return true;
}



int 出し要請(pcDroping pdrop, pcCharacter receiver, pcDroping pbox)
{
	pcBox pcbx = boost::dynamic_pointer_cast<cBox>(pbox);
	if(pcbx == NULL)
	{
		assert(!_T("error_箱以外を出す"));
		return false;
	}

	if(receiver->持ち物余白あり())
	{

		int result = 0;
			
		if(pbox->state() == cDroping::STATE_NORMAL)
		{
			result = pcbx->出す_メッセージ_通常(pdrop,receiver);
		}
		else if(pbox->state() == cDroping::STATE_CURSE)
		{
			result = pcbx->出す_メッセージ_呪い(pdrop,receiver);
		}
		else if(pbox->state() == cDroping::STATE_GOOD)
		{
			result = pcbx->出す_メッセージ_祝福(pdrop,receiver);
		}


		if(result)
		{
			sg_pDungeonSystem->落ち物出し要請(pdrop,receiver);
		}




		if(pbox->state() == cDroping::STATE_NORMAL)
		{
			result = pcbx->出す_効果_通常(pdrop,receiver);
		}
		else if(pbox->state() == cDroping::STATE_CURSE)
		{
			result = pcbx->出す_効果_呪い(pdrop,receiver);
		}
		else if(pbox->state() == cDroping::STATE_GOOD)
		{
			result = pcbx->出す_効果_祝福(pdrop,receiver);
		}

		if(result)
		{//使用に成功した
			//sg_pDungeonSystem->落ち物破壊要請(me());
		}

		return result;
	}
	else
	{
		sg_pDungeonSystem->メッセージ(g_Lang(_T("Box出し要請拒否メッセージ")));
	}
	return false;
}
int 入れ替え(cRenderDevice *pDev, pcDroping pdrop, pcDroping pdrop2, pcDroping pbox)
{
	pcBox pcbx = boost::dynamic_pointer_cast<cBox>(pbox);
	if(pcbx == NULL)
	{
		assert(!_T("error_箱以外を出す"));
		return false;
	}
	int result = true;

	//pdropは出す方
	if(pdrop2->被箱入れ可() && pcbx->箱入れ可(pdrop2))
	{//入れ物じゃない
		if(pbox != pdrop2)
		{//自分自身でない

			if(result)
			{
				result = 入れ替え要請(pdrop,pdrop2,pbox);
			}

			if(result)
			{//使用に成功した
				
				sg_pDungeonSystem->ターンエンド();
			}
		}
		else
		{
			//入れ要請と共用
			map<tstring, StyleString> valiable;
			valiable[_T("Box")] = pbox->FullName();
			valiable[_T("Item")] = pdrop2->FullName();
			sg_pDungeonSystem->メッセージ(g_Lang(_T("Box入れ要請拒否メッセージ"),valiable));
			//sg_pDungeonSystem->メッセージ(_T("これは入れられない。\n"));
		}
	}
	else
	{
		//入れ要請と共用
		map<tstring, StyleString> valiable;
		valiable[_T("Box")] = pbox->FullName();
		valiable[_T("Item")] = pdrop2->FullName();
		sg_pDungeonSystem->メッセージ(g_Lang(_T("Box入れ要請拒否メッセージ"),valiable));
		//sg_pDungeonSystem->メッセージ(_T("これは入れられない。\n"));
	}
	sg_pDungeonSystem->メニューを閉じる();

	return true;
}
int 入れ替え要請(pcDroping pdrop, pcDroping pdrop2, pcDroping pbox)
{
	pcBox pcbx = boost::dynamic_pointer_cast<cBox>(pbox);
	if(pcbx == NULL)
	{
		assert(!_T("error_箱以外を出す"));
		return false;
	}
	int result = true;

	sg_pDungeonSystem->落ち物入れ替え要請(pdrop,pdrop2);



	if(pbox->state() == cDroping::STATE_NORMAL)
	{
		result = pcbx->入れ替え_メッセージ_通常(pdrop,pdrop2);
	}
	else if(pbox->state() == cDroping::STATE_CURSE)
	{
		result = pcbx->入れ替え_メッセージ_呪い(pdrop,pdrop2);
	}
	else if(pbox->state() == cDroping::STATE_GOOD)
	{
		result = pcbx->入れ替え_メッセージ_祝福(pdrop,pdrop2);
	}



	if(pbox->state() == cDroping::STATE_NORMAL)
	{
		result = pcbx->入れ替え_効果_通常(pdrop,pdrop2);
	}
	else if(pbox->state() == cDroping::STATE_CURSE)
	{
		result = pcbx->入れ替え_効果_呪い(pdrop,pdrop2);
	}
	else if(pbox->state() == cDroping::STATE_GOOD)
	{
		result = pcbx->入れ替え_効果_祝福(pdrop,pdrop2);
	}

	if(result)
	{//使用に成功した
		//sg_pDungeonSystem->落ち物破壊要請(me());
	}

	return result;
}

//---------------------------------------------
//Bullet
int Bullet装備(cRenderDevice *pDev, pcDroping pdrop)
{
	pcBullet pcBul = boost::dynamic_pointer_cast<cBullet>(pdrop);
	if(pcBul == NULL)
	{
		assert(!_T("error_弾以外を装備"));
		return false;
	}

	int result = 0;


	if(pdrop->state() == cDroping::STATE_NORMAL)
	{
		result = pcBul->装備_メッセージ_通常(sg_pDungeonSystem->pPlayerChara());
	}
	else if(pdrop->state() == cDroping::STATE_CURSE)
	{
		result = pcBul->装備_メッセージ_呪い(sg_pDungeonSystem->pPlayerChara());
	}
	else if(pdrop->state() == cDroping::STATE_GOOD)
	{
		result = pcBul->装備_メッセージ_祝福(sg_pDungeonSystem->pPlayerChara());
	}


	result = Bullet装備要請(sg_pDungeonSystem->pPlayerChara(), pdrop);


	if(result)
	{//使用に成功した
		sg_pDungeonSystem->メニューを閉じる();
		//sg_pDungeonSystem->ターンエンド();
	}

	sg_pDungeonSystem->メニューを閉じる();
	return true;
}
int Bullet装備要請(pcCharacter pchara, pcDroping pdrop)
{
	pcBullet pcBul = boost::dynamic_pointer_cast<cBullet>(pdrop);
	if(pcBul == NULL)
	{
		assert(!_T("error_弾以外を装備"));
		return false;
	}

	if(!pcBul->装備可())
	{//装備できない
		return false;
	}


	int result = 0;
	if(pdrop->state() == cDroping::STATE_NORMAL)
	{
		result = pcBul->装備_効果_通常(pchara);
	}
	else if(pdrop->state() == cDroping::STATE_CURSE)
	{
		result = pcBul->装備_効果_呪い(pchara);
	}
	else if(pdrop->state() == cDroping::STATE_GOOD)
	{
		result = pcBul->装備_効果_祝福(pchara);
	}

	if(result)
	{//装備に成功した
		
	}

	return result;
}
int Bulletはずす(cRenderDevice *pDev, pcDroping pdrop)
{
	pcBullet pcBul = boost::dynamic_pointer_cast<cBullet>(pdrop);
	if(pcBul == NULL)
	{
		assert(!_T("error_弾以外をはずす"));
		return false;
	}

	int result = 0;


	if(pdrop->state() == cDroping::STATE_NORMAL)
	{
		result = pcBul->はずす_メッセージ_通常(sg_pDungeonSystem->pPlayerChara());
	}
	else if(pdrop->state() == cDroping::STATE_CURSE)
	{
		result = pcBul->はずす_メッセージ_呪い(sg_pDungeonSystem->pPlayerChara());
	}
	else if(pdrop->state() == cDroping::STATE_GOOD)
	{
		result = pcBul->はずす_メッセージ_祝福(sg_pDungeonSystem->pPlayerChara());
	}


	result = Bullet装備はずし要請(sg_pDungeonSystem->pPlayerChara(), pdrop);


	if(result)
	{//使用に成功した
		sg_pDungeonSystem->メニューを閉じる();
	}

	sg_pDungeonSystem->メニューを閉じる();
	return true;
}
int Bullet装備はずし要請(pcCharacter pchara, pcDroping pdrop)
{
	pcBullet pcBul = boost::dynamic_pointer_cast<cBullet>(pdrop);
	if(pcBul == NULL)
	{
		assert(!_T("error_弾以外をはずす"));
		return false;
	}

	int result = 0;
	if(pdrop->state() == cDroping::STATE_NORMAL)
	{
		result = pcBul->はずす_効果_通常(pchara);
	}
	else if(pdrop->state() == cDroping::STATE_CURSE)
	{
		result = pcBul->はずす_効果_呪い(pchara);
	}
	else if(pdrop->state() == cDroping::STATE_GOOD)
	{
		result = pcBul->はずす_効果_祝福(pchara);
	}

	if(result)
	{//使用に成功した
		sg_pDungeonSystem->メニューを閉じる();
	}


	return result;
}

int 撃つ(cRenderDevice *pDev, pcDroping pdrop)
{
	pcBullet pcBul = boost::dynamic_pointer_cast<cBullet>(pdrop);
	if(pcBul == NULL)
	{
		assert(!_T("error_弾以外を撃つ"));
		return false;
	}

	int result = 0;


	if(pdrop->state() == cDroping::STATE_NORMAL)
	{
		result = pcBul->撃つ_メッセージ_通常(sg_pDungeonSystem->pPlayerChara());
	}
	else if(pdrop->state() == cDroping::STATE_CURSE)
	{
		result = pcBul->撃つ_メッセージ_呪い(sg_pDungeonSystem->pPlayerChara());
	}
	else if(pdrop->state() == cDroping::STATE_GOOD)
	{
		result = pcBul->撃つ_メッセージ_祝福(sg_pDungeonSystem->pPlayerChara());
	}

	if(result)
	{
		result = 撃ち要請(sg_pDungeonSystem->pPlayerChara(), pdrop);

	}
	if(result)
	{//使用に成功した
		sg_pDungeonSystem->メニューを閉じる();
		sg_pDungeonSystem->ターンエンド();
	}

	sg_pDungeonSystem->メニューを閉じる();
	return true;

}
int 撃ち要請(pcCharacter pchara, pcDroping pdrop)
{
	pcBullet pcBul = boost::dynamic_pointer_cast<cBullet>(pdrop);
	if(pcBul == NULL)
	{
		assert(!_T("error_弾以外を撃つ"));
		return false;
	}

	int result = 0;
	if(pdrop->state() == cDroping::STATE_NORMAL)
	{
		result = pcBul->撃つ_効果_通常(pchara);
	}
	else if(pdrop->state() == cDroping::STATE_CURSE)
	{
		result = pcBul->撃つ_効果_呪い(pchara);
	}
	else if(pdrop->state() == cDroping::STATE_GOOD)
	{
		result = pcBul->撃つ_効果_祝福(pchara);
	}

	if(result)
	{//使用に成功した
		
		if(pcBul->quantity() <= 0)
		{
			//sg_pDungeonSystem->落ち物破壊要請(pdrop);
		}
		else
		{
			if(pdrop->state() == cDroping::STATE_GOOD)
			{
				pcBul->stategoodRunoutprocess();
			}
		}
	}

	return result;
}

//---------------------------------------------
//Drink
int 飲む(cRenderDevice *pDev, pcDroping pdrop)
{
	pcDrink pcDrk = boost::dynamic_pointer_cast<cDrink>(pdrop);
	if(pcDrk == NULL)
	{
		assert(!_T("error_飲み物以外を飲む"));
		return false;
	}

	int result = 0;


	if(pdrop->state() == cDroping::STATE_NORMAL)
	{
		result = pcDrk->飲む_メッセージ_通常(sg_pDungeonSystem->pPlayerChara());
	}
	else if(pdrop->state() == cDroping::STATE_CURSE)
	{
		result = pcDrk->飲む_メッセージ_呪い(sg_pDungeonSystem->pPlayerChara());
	}
	else if(pdrop->state() == cDroping::STATE_GOOD)
	{
		result = pcDrk->飲む_メッセージ_祝福(sg_pDungeonSystem->pPlayerChara());
	}

	if(result)
	{//使用に成功した

		sg_pDungeonSystem->メニューを閉じる();
		sg_pDungeonSystem->ターンエンド();

		
		if(!pdrop->名称識別済み())
		{
			sg_pDungeonSystem->識別(pdrop,true);
		}
		else if(!pdrop->完全識別済み())
		{
			sg_pDungeonSystem->識別(pdrop,MASSAGE_IFCHANGE);
		}

		result = 飲み要請(sg_pDungeonSystem->pPlayerChara(),pdrop);
	}




	if(result)
	{//使用に成功した
		pcDroping emptypdrop = sg_pDungeonSystem->落ち物生成_設置なし(5008);
		//sg_pDungeonSystem->落ち物変化(pdrop,emptypdrop);
		sg_pDungeonSystem->落ち物破壊要請(pdrop);
		emptypdrop->discover();

		if(pcDrk->reuse())
		{
			map<tstring,StyleString> val;
			val[_T("Item")] = emptypdrop->FullName();
			sg_pDungeonSystem->メッセージ(g_Lang(_T("Drink破壊メッセージ"),val));
			sg_pDungeonSystem->落ち物破壊要請(emptypdrop);
			
		}
		else
		{
			sg_pDungeonSystem->落ち物設置(emptypdrop, sg_pDungeonSystem->pPlayerChara()->足元地形());
			
			
			//
		}
	}

	sg_pDungeonSystem->メニューを閉じる();

	return true;
}
int 飲み要請(pcCharacter pchara, pcDroping pdrop)
{
	pcDrink pcDrk = boost::dynamic_pointer_cast<cDrink>(pdrop);
	if(pcDrk == NULL)
	{
		assert(!_T("error_飲み物以外を飲む"));
		return false;
	}

	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;
	valf.drops.dim(変数_対象落ち物) = pdrop;
	sg_pDungeonSystem->CutInM().CutIn(pchara,飲み効果直前_タイミング,valf);
		
	int result = 0;
	if(pdrop->state() == cDroping::STATE_NORMAL)
	{

		if(valf.doubles.dim(変数_汎用ブール))
			result = pcDrk->飲む_効果_通常(pchara);
		else
			result = true;//飲んだことにはなる

	}
	else if(pdrop->state() == cDroping::STATE_CURSE)
	{
		result = pcDrk->飲む_効果_呪い(pchara);
	}
	else if(pdrop->state() == cDroping::STATE_GOOD)
	{
		if(valf.doubles.dim(変数_汎用ブール))
			result = pcDrk->飲む_効果_祝福(pchara);
		else
			result = true;//飲んだことにはなる

	}

	if(result)
	{//使用に成功した


		sg_pDungeonSystem->満腹度回復要請(pchara,5,false);


	}

	return result;
}
int 汲む(cRenderDevice *pDev, pcDroping pdrop)
{
	pcDrink pcDrk = boost::dynamic_pointer_cast<cDrink>(pdrop);
	if(pcDrk == NULL)
	{
		assert(!_T("error_飲み物以外を汲む"));
		return false;
	}

	int result = 0;



	result = 汲み要請(sg_pDungeonSystem->pPlayerChara(),pdrop);
	
	if(result)
	{//使用に成功した
		sg_pDungeonSystem->メニューを閉じる();
		sg_pDungeonSystem->ターンエンド();
	}

	sg_pDungeonSystem->メニューを閉じる();
	return true;
}

int 汲み要請(pcCharacter pchara, pcDroping pdrop)
{
	pcDrink pcDrk = boost::dynamic_pointer_cast<cDrink>(pdrop);
	if(pcDrk == NULL)
	{
		assert(!_T("error_飲み物以外を汲む"));
		return false;
	}

	pcDroping dipdrop;

	int result = 0;
	if(pdrop->state() == cDroping::STATE_NORMAL)
	{
		result = pcDrk->汲む_効果_通常(pchara,dipdrop);
	}
	else if(pdrop->state() == cDroping::STATE_CURSE)
	{
		result = pcDrk->汲む_効果_呪い(pchara,dipdrop);
	}
	else if(pdrop->state() == cDroping::STATE_GOOD)
	{
		result = pcDrk->汲む_効果_祝福(pchara,dipdrop);
	}

	if(dipdrop){
		sg_pDungeonSystem->落ち物変化(pdrop,dipdrop);
		dipdrop->quality() = pdrop->quality();
	}

	if(pdrop->state() == cDroping::STATE_NORMAL)
	{
		result = pcDrk->汲む_メッセージ_通常(sg_pDungeonSystem->pPlayerChara(),dipdrop);
	}
	else if(pdrop->state() == cDroping::STATE_CURSE)
	{
		result = pcDrk->汲む_メッセージ_呪い(sg_pDungeonSystem->pPlayerChara(),dipdrop);
	}
	else if(pdrop->state() == cDroping::STATE_GOOD)
	{
		result = pcDrk->汲む_メッセージ_祝福(sg_pDungeonSystem->pPlayerChara(),dipdrop);
	}

	if(result)
	{//使用に成功した

	}

	return result;
}

int カクテル(cRenderDevice *pDev ,vector<pcDroping> &ObjectList, pcDroping pdrop)
{
	pcDrink pcDrk = boost::dynamic_pointer_cast<cDrink>(pdrop);
	if(pcDrk == NULL)
	{
		assert(!_T("error_飲み物以外をカクテル"));
		return false;
	}

	int i;
	for(i=0;i<ObjectList.size();i++)
	{
		if(ObjectList[i] == pdrop) {
			map<tstring, StyleString> valiable;
			valiable[_T("Item")] = pdrop->FullName();
			g_Langメッセージ(_T("カクテル不能自身メッセージ"),valiable);
			sg_pDungeonSystem->メニューを閉じる();
			return false;
		}
		pcDrink pcObjectDrk = boost::dynamic_pointer_cast<cDrink>(ObjectList[i]);
		if(pcObjectDrk == NULL)
		{
			map<tstring, StyleString> valiable;
			valiable[_T("Item")] = ObjectList[i]->FullName();
			g_Langメッセージ(_T("カクテル不能メッセージ"),valiable);
			sg_pDungeonSystem->メニューを閉じる();
			return false;
		}
		else if(pcObjectDrk->reuse())
		{
			map<tstring, StyleString> valiable;
			valiable[_T("Item")] = ObjectList[i]->FullName();
			g_Langメッセージ(_T("カクテル不能つめかえメッセージ"),valiable);
			sg_pDungeonSystem->メニューを閉じる();
			return false;
		}
		int k;
		for(k=0;k<ObjectList.size();k++)
		{
			if(i != k && ObjectList[i] == ObjectList[k]) {
				map<tstring, StyleString> valiable;
				g_Langメッセージ(_T("カクテル不能重複メッセージ"),valiable);
				sg_pDungeonSystem->メニューを閉じる();
				return false;
			}
		}
	}

	int result = 0;


	result = カクテル要請(sg_pDungeonSystem->pPlayerChara(), ObjectList, pdrop);

	if(result == 2) return true;//混酒の箱の例外処理

	if(result)
	{//使用に成功した
		sg_pDungeonSystem->メニューを閉じる();
		sg_pDungeonSystem->ターンエンド();
	}

	sg_pDungeonSystem->メニューを閉じる();
	return true;
}
int カクテル要請(pcCharacter pchara ,vector<pcDroping> &ObjectList, pcDroping pdrop)
{
	pcDrink pcDrk = boost::dynamic_pointer_cast<cDrink>(pdrop);
	if(pcDrk == NULL)
	{
		assert(!_T("error_飲み物以外をカクテル"));
		return false;
	}

	int result = 0;

	result = pcDrk->カクテル_効果(pchara,ObjectList);

	if(result)
	{//使用に成功した

	}

	return result;
}

//---------------------------------------------
//Equipment
int Equipment装備(cRenderDevice *pDev, int part, pcDroping pdrop)
{
	pcEquipment pcEqu = boost::dynamic_pointer_cast<cEquipment>(pdrop);
	if(pcEqu == NULL)
	{
		assert(!_T("error_装備品以外を装備"));
		return false;
	}
	
	int result = 0;

	cValiableField val;
	val.doubles.dim(変数_汎用ブール) = TRUE;
	val.drops.dim(変数_対象落ち物) = pdrop;
	val.doubles.dim(_T("part")) = part;
	val.doubles.dim(_T("both_hands")) = (int)pcEqu->両手装備();
	sg_pDungeonSystem->CutInM().CutIn(sg_pDungeonSystem->pPlayerChara(),装備直前_タイミング,val);
	if(!val.doubles.dim(変数_汎用ブール)) return false;

	if(pdrop->state() == cDroping::STATE_NORMAL)
	{
		result = pcEqu->装備_メッセージ_通常(sg_pDungeonSystem->pPlayerChara(),part);
	}
	else if(pdrop->state() == cDroping::STATE_CURSE)
	{
		result = pcEqu->装備_メッセージ_呪い(sg_pDungeonSystem->pPlayerChara(),part);
	}
	else if(pdrop->state() == cDroping::STATE_GOOD)
	{
		result = pcEqu->装備_メッセージ_祝福(sg_pDungeonSystem->pPlayerChara(),part);
	}


	result = Equipment装備要請(sg_pDungeonSystem->pPlayerChara(),part,pdrop);


	if(result)
	{//使用に成功した
		sg_pDungeonSystem->メニューを閉じる();
		sg_pDungeonSystem->ターンエンド();
	}

	sg_pDungeonSystem->メニューを閉じる();
	return true;
}
int Equipment装備選択(cRenderDevice *pDev, pcDroping pdrop)
{
	pcEquipment pcEqu = boost::dynamic_pointer_cast<cEquipment>(pdrop);
	if(pcEqu == NULL)
	{
		assert(!_T("error_装備品以外を装備"));
		return false;
	}

	vector<tstring> captions;

	//GetmenuCaption(captions);
	//if(pcEqu->装備可_攻撃用()) captions.push_back(_T("攻撃用に装備"));
	//if(pcEqu->装備可_防御用()) captions.push_back(_T("防御用に装備"));
	captions.push_back(_T("攻撃用に装備"));
	captions.push_back(_T("防御用に装備"));

	if(captions.size() == 0) return false;

	pcControlLayer pccl;
	pcSelectWindow pcsw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));
	

	int i;
	for(i=0;i<captions.size();i++)
	{
		cDropingFunc *drfn;
		pcsw->commandList.push_back(pcCommand(drfn = new cDropingFunc(captions[i])));
		drfn->Subject = pdrop;
		drfn->Verb = captions[i];

		drfn->shortExplanation = pdrop->GetmenuExplain(drfn->Verb);
	}

	pcsw->Init(pDev, 6, pcsw->commandList.size());
	pcsw->setRight(sg_pDungeonSystem->GameScreenInterface.menuPosRight(2));
	pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(2));
	
	return true;
}
int Equipment装備要請(pcCharacter pchara, int part, pcDroping pdrop)
{
	pcEquipment pcEqu = boost::dynamic_pointer_cast<cEquipment>(pdrop);
	if(pcEqu == NULL)
	{
		assert(!_T("error_装備品以外を装備"));
		return false;
	}

	if(part == EQUIP_PART_ATTACK && !pcEqu->装備可_攻撃用())
	{//装備できない
		return false;
	}
	if(part == EQUIP_PART_DEFENSE && !pcEqu->装備可_防御用())
	{//装備できない
		return false;
	}

	int result = 0;
	if(pdrop->state() == cDroping::STATE_NORMAL)
	{
		result = pcEqu->装備_効果_通常(pchara,part);
	}
	else if(pdrop->state() == cDroping::STATE_CURSE)
	{
		result = pcEqu->装備_効果_呪い(pchara,part);
	}
	else if(pdrop->state() == cDroping::STATE_GOOD)
	{
		result = pcEqu->装備_効果_祝福(pchara,part);
	}

	if(result)
	{//装備に成功した
		
	}
	else
	{//失敗した
		if(pchara == sg_pDungeonSystem->pPlayerChara())
		{
			sg_pDungeonSystem->メッセージ(g_Lang(_T("Equip装備要請拒否メッセージ")));
			//sg_pDungeonSystem->メッセージ(_T("今の装備がはずせないので装備できない!\n"));
		}
	}

	return result;
}

int Equipmentはずす(cRenderDevice *pDev, pcDroping pdrop)
{
	pcEquipment pcEqu = boost::dynamic_pointer_cast<cEquipment>(pdrop);
	if(pcEqu == NULL)
	{
		assert(!_T("error_装備品以外をはずす"));
		return false;
	}

	int result = 0;


	if(pdrop->state() == cDroping::STATE_NORMAL)
	{
		result = pcEqu->はずす_メッセージ_通常(sg_pDungeonSystem->pPlayerChara());
	}
	else if(pdrop->state() == cDroping::STATE_CURSE)
	{
		result = pcEqu->はずす_メッセージ_呪い(sg_pDungeonSystem->pPlayerChara());
	}
	else if(pdrop->state() == cDroping::STATE_GOOD)
	{
		result = pcEqu->はずす_メッセージ_祝福(sg_pDungeonSystem->pPlayerChara());
	}


	result = Equipment装備はずし要請(sg_pDungeonSystem->pPlayerChara(),pdrop);


	if(result)
	{//使用に成功した
		sg_pDungeonSystem->メニューを閉じる();
		sg_pDungeonSystem->ターンエンド();
	}

	sg_pDungeonSystem->メニューを閉じる();
	return true;

}
int Equipment装備はずし要請(pcCharacter pchara, pcDroping pdrop)
{
	pcEquipment pcEqu = boost::dynamic_pointer_cast<cEquipment>(pdrop);
	if(pcEqu == NULL)
	{
		assert(!_T("error_装備品以外をはずす"));
		return false;
	}

	int result = 0;
	if(pdrop->state() == cDroping::STATE_NORMAL)
	{
		result = pcEqu->はずす_効果_通常(pchara);
	}
	else if(pdrop->state() == cDroping::STATE_CURSE)
	{
		result = pcEqu->はずす_効果_呪い(pchara);
	}
	else if(pdrop->state() == cDroping::STATE_GOOD)
	{
		result = pcEqu->はずす_効果_祝福(pchara);
	}

	if(result)
	{//使用に成功した

	}

	return result;

}
int 能力(cRenderDevice *pDev, pcDroping pdrop)
{
	pcEquipment pcEqu = boost::dynamic_pointer_cast<cEquipment>(pdrop);
	if(pcEqu == NULL)
	{
		assert(!_T("error_装備品以外の能力"));
		return false;
	}


	pcControlLayer pccl;
	pcSelectWindow pcsw;
	pcGameWindow pcgw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));
	

	//拠り代
	cDropingFunc *drfn;
	if(pcEqu->能力スロット())
	{
		pcsw->commandList.push_back(pcCommand(drfn = new cDropingFunc()));
		drfn->caption = pcEqu->能力説明Caption();
		drfn->shortExplanation = pcEqu->能力説明();
	}

	int i;
	for(i=0;i<pcEqu->includedItem.size();i++)
	{
		pcsw->commandList.push_back(pcCommand(drfn = new cDropingFunc()));
		//pcco->Subject = pcEqu->FullName();
		drfn->caption = pcEqu->includedItem[i]->能力説明Caption();
		drfn->shortExplanation = pcEqu->includedItem[i]->shortExplanation();
	}

	pcsw->Init(pDev, 20, 10);
	pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(2));
	pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(3));

	pccl->WindowList.push_back(pcgw = pcGameWindow(new cGameWindow));
	pcgw->Text() = pcEqu->能力タイトルCaption();
	
	pcgw->Init(pDev, 20, 1);
	pcgw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(2));
	pcgw->setBottom(sg_pDungeonSystem->GameScreenInterface.menuPosTop(3));

	return true;
}

//---------------------------------------------
//Food
int 食べる(cRenderDevice *pDev, pcDroping pdrop)
{
	pcFood pcfd = boost::dynamic_pointer_cast<cFood>(pdrop);
	if(pcfd == NULL)
	{
		assert(!_T("error_食べ物以外を食べる"));
		return false;
	}

	int result = 0;


	if(pdrop->state() == cDroping::STATE_NORMAL)
	{
		result = pcfd->食べる_メッセージ_通常(sg_pDungeonSystem->pPlayerChara());
	}
	else if(pdrop->state() == cDroping::STATE_CURSE)
	{
		result = pcfd->食べる_メッセージ_呪い(sg_pDungeonSystem->pPlayerChara());
	}
	else if(pdrop->state() == cDroping::STATE_GOOD)
	{
		result = pcfd->食べる_メッセージ_祝福(sg_pDungeonSystem->pPlayerChara());
	}
	if(result)
	{//使用に成功した
		sg_pDungeonSystem->メニューを閉じる();
		sg_pDungeonSystem->ターンエンド();
	}

	result = 食べ要請(sg_pDungeonSystem->pPlayerChara(),pdrop);



	sg_pDungeonSystem->メニューを閉じる();

	return true;
}
int 食べ要請(pcCharacter pchara, pcDroping pdrop)
{
	pcFood pcfd = boost::dynamic_pointer_cast<cFood>(pdrop);
	if(pcfd == NULL)
	{
		assert(!_T("error_食べ物以外を食べる"));
		return false;
	}

	int result = 0;
	if(pdrop->state() == cDroping::STATE_NORMAL)
	{
		result = pcfd->食べる_効果_通常(pchara);
	}
	else if(pdrop->state() == cDroping::STATE_CURSE)
	{
		result = pcfd->食べる_効果_呪い(pchara);
	}
	else if(pdrop->state() == cDroping::STATE_GOOD)
	{
		result = pcfd->食べる_効果_祝福(pchara);
	}

	if(result)
	{//使用に成功した
		sg_pDungeonSystem->落ち物破壊要請(pdrop);
	}

	return result;
}

//---------------------------------------------
//Spell
int Spell装備(cRenderDevice *pDev, int part, pcDroping pdrop)
{
	pcSpell pcSpl = boost::dynamic_pointer_cast<cSpell>(pdrop);
	if(pcSpl == NULL)
	{
		assert(!_T("error_装備品以外を装備"));
		return false;
	}

	int result = 0;


	if(pdrop->state() == cDroping::STATE_NORMAL)
	{
		result = pcSpl->装備_メッセージ_通常(sg_pDungeonSystem->pPlayerChara(),part);
	}
	else if(pdrop->state() == cDroping::STATE_CURSE)
	{
		result = pcSpl->装備_メッセージ_呪い(sg_pDungeonSystem->pPlayerChara(),part);
	}
	else if(pdrop->state() == cDroping::STATE_GOOD)
	{
		result = pcSpl->装備_メッセージ_祝福(sg_pDungeonSystem->pPlayerChara(),part);
	}


	result = Spell装備要請(sg_pDungeonSystem->pPlayerChara(),part,pdrop);


	if(result)
	{//使用に成功した

		sg_pDungeonSystem->ターンエンド();
	}
	sg_pDungeonSystem->メニューを閉じる();
	return true;
}
int Spell装備選択(cRenderDevice *pDev, pcDroping pdrop)
{
	pcSpell pcSpl = boost::dynamic_pointer_cast<cSpell>(pdrop);
	if(pcSpl == NULL)
	{
		assert(!_T("error_装備品以外を装備"));
		return false;
	}

	vector<tstring> captions;

	//GetmenuCaption(captions);
	int i;
	for(i=0;i<SPELL_EQUIP_MAXNUM && i< sg_pDungeonSystem->pPlayerChara()->最大装備スペル数();i++)
	{
		captions.push_back(pcSpl->装備コマンド名称(i));
	}
	if(captions.size() == 0) return false;

	pcControlLayer pccl;
	pcSelectWindow pcsw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));
	


	for(i=0;i<captions.size();i++)
	{
		cDropingFunc *drfn;
		pcsw->commandList.push_back(pcCommand(drfn = new cDropingFunc(captions[i])));
		drfn->Subject = pdrop;
		drfn->Verb = captions[i];

		drfn->shortExplanation = pcSpl->GetmenuExplain(drfn->Verb);
	}

	pcsw->Init(pDev, 6, pcsw->commandList.size());
	pcsw->setRight(sg_pDungeonSystem->GameScreenInterface.menuPosRight(2));
	pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(2));
	
	return true;
}
int Spell装備要請(pcCharacter pchara, int part, pcDroping pdrop)
{
	pcSpell pcSpl = boost::dynamic_pointer_cast<cSpell>(pdrop);
	if(pcSpl == NULL)
	{
		assert(!_T("error_装備品以外を装備"));
		return false;
	}

	if(pcSpl->装備されている())
	{//装備できない
		return false;
	}


	int result = 0;
	if(pdrop->state() == cDroping::STATE_NORMAL)
	{
		result = pcSpl->装備_効果_通常(pchara,part);
	}
	else if(pdrop->state() == cDroping::STATE_CURSE)
	{
		result = pcSpl->装備_効果_呪い(pchara,part);
	}
	else if(pdrop->state() == cDroping::STATE_GOOD)
	{
		result = pcSpl->装備_効果_祝福(pchara,part);
	}

	if(result)
	{//装備に成功した
		
	}

	return result;
}

int Spellはずす(cRenderDevice *pDev, pcDroping pdrop)
{
	pcSpell pcSpl = boost::dynamic_pointer_cast<cSpell>(pdrop);
	if(pcSpl == NULL)
	{
		assert(!_T("error_装備品以外をはずす"));
		return false;
	}

	int result = 0;


	if(pdrop->state() == cDroping::STATE_NORMAL)
	{
		result = pcSpl->はずす_メッセージ_通常(sg_pDungeonSystem->pPlayerChara());
	}
	else if(pdrop->state() == cDroping::STATE_CURSE)
	{
		result = pcSpl->はずす_メッセージ_呪い(sg_pDungeonSystem->pPlayerChara());
	}
	else if(pdrop->state() == cDroping::STATE_GOOD)
	{
		result = pcSpl->はずす_メッセージ_祝福(sg_pDungeonSystem->pPlayerChara());
	}


	result = Spell装備はずし要請(sg_pDungeonSystem->pPlayerChara(),pdrop);


	if(result)
	{//使用に成功した
		sg_pDungeonSystem->メニューを閉じる();
	}

	sg_pDungeonSystem->メニューを閉じる();

	return true;
}
int Spell装備はずし要請(pcCharacter pchara, pcDroping pdrop)
{
	pcSpell pcSpl = boost::dynamic_pointer_cast<cSpell>(pdrop);
	if(pcSpl == NULL)
	{
		assert(!_T("error_装備品以外をはずす"));
		return false;
	}

	int result = 0;
	if(pdrop->state() == cDroping::STATE_NORMAL)
	{
		result = pcSpl->はずす_効果_通常(pchara);
	}
	else if(pdrop->state() == cDroping::STATE_CURSE)
	{
		result = pcSpl->はずす_効果_呪い(pchara);
	}
	else if(pdrop->state() == cDroping::STATE_GOOD)
	{
		result = pcSpl->はずす_効果_祝福(pchara);
	}

	if(result)
	{//使用に成功した
		sg_pDungeonSystem->メニューを閉じる();
	}

	return result;
}


int 破壊(cRenderDevice *pDev, pcDroping pdrop)
{
	pcSpell pcSpl = boost::dynamic_pointer_cast<cSpell>(pdrop);
	if(pcSpl == NULL)
	{
		assert(!_T("error_装備品以外を破壊"));
		return false;
	}

	int result = 0;


	if(pdrop->state() == cDroping::STATE_NORMAL)
	{
		result = pcSpl->破壊_メッセージ_通常(sg_pDungeonSystem->pPlayerChara());
	}
	else if(pdrop->state() == cDroping::STATE_CURSE)
	{
		result = pcSpl->破壊_メッセージ_呪い(sg_pDungeonSystem->pPlayerChara());
	}
	else if(pdrop->state() == cDroping::STATE_GOOD)
	{
		result = pcSpl->破壊_メッセージ_祝福(sg_pDungeonSystem->pPlayerChara());
	}


	result = 破壊要請(sg_pDungeonSystem->pPlayerChara(),pdrop);


	if(result)
	{//使用に成功した
		sg_pDungeonSystem->メニューを閉じる();
	}

	sg_pDungeonSystem->メニューを閉じる();

	return true;
}
int 破壊要請(pcCharacter pchara, pcDroping pdrop)
{
	pcSpell pcSpl = boost::dynamic_pointer_cast<cSpell>(pdrop);
	if(pcSpl == NULL)
	{
		assert(!_T("error_装備品以外を破壊"));
		return false;
	}

	int result = 0;
	if(pdrop->state() == cDroping::STATE_NORMAL)
	{
		result = pcSpl->破壊_効果_通常(pchara);
	}
	else if(pdrop->state() == cDroping::STATE_CURSE)
	{
		result = pcSpl->破壊_効果_呪い(pchara);
	}
	else if(pdrop->state() == cDroping::STATE_GOOD)
	{
		result = pcSpl->破壊_効果_祝福(pchara);
	}

	if(result)
	{//使用に成功した
		sg_pDungeonSystem->メニューを閉じる();
	}

	return result;
}


int 宣言(cRenderDevice *pDev ,vector<pcDroping> &ObjectList, pcDroping pdrop)
{
	pcSpell pcSpl = boost::dynamic_pointer_cast<cSpell>(pdrop);
	if(pcSpl == NULL)
	{
		assert(!_T("error_スペル以外を宣言"));
		return false;
	}

	int result = 1;

	cValiableField val;
	val.doubles.dim(変数_汎用ブール) = result;
	sg_pDungeonSystem->CutInM().CutIn(sg_pDungeonSystem->pPlayerChara(),宣言直前_タイミング,val);
	if(!val.doubles.dim(変数_汎用ブール)) return false;

	if(pdrop->state() == cDroping::STATE_NORMAL)
	{
		result = pcSpl->宣言_メッセージ_通常(sg_pDungeonSystem->pPlayerChara(), ObjectList);
	}
	else if(pdrop->state() == cDroping::STATE_CURSE)
	{
		result = pcSpl->宣言_メッセージ_呪い(sg_pDungeonSystem->pPlayerChara(), ObjectList);
	}
	else if(pdrop->state() == cDroping::STATE_GOOD)
	{
		result = pcSpl->宣言_メッセージ_祝福(sg_pDungeonSystem->pPlayerChara(), ObjectList);
	}

	if(result)
	{//使用に成功した
		sg_pDungeonSystem->メニューを閉じる();
		sg_pDungeonSystem->ターンエンド();

		if(!pdrop->名称識別済み())
		{
			sg_pDungeonSystem->識別(pdrop,true);
		}
		else if(!pdrop->完全識別済み())
		{
			sg_pDungeonSystem->識別(pdrop,false);
		}

	}
	sg_pDungeonSystem->メニューを閉じる();

	result = 宣言要請(sg_pDungeonSystem->pPlayerChara(), ObjectList, pdrop);




	return true;
}
int 宣言要請(pcCharacter pchara ,vector<pcDroping> &ObjectList, pcDroping pdrop)
{
	pcSpell pcspl = boost::dynamic_pointer_cast<cSpell>(pdrop);
	if(pcspl == NULL)
	{
		assert(!_T("error_スペル以外を宣言"));
		return false;
	}




	int result = 0;
	if(pdrop->state() == cDroping::STATE_NORMAL)
	{
		result = pcspl->宣言_効果_通常(pchara, ObjectList);
	}
	else if(pdrop->state() == cDroping::STATE_CURSE)
	{
		result = pcspl->宣言_効果_呪い(pchara, ObjectList);
	}
	else if(pdrop->state() == cDroping::STATE_GOOD)
	{
		result = pcspl->宣言_効果_祝福(pchara, ObjectList);
	}

	if(result)
	{//使用に成功した

		if(pdrop->state() == cDroping::STATE_GOOD)
		{
			pcspl->stategoodRunoutprocess();
		}
		else
		{
			sg_pDungeonSystem->落ち物破壊要請(pdrop);
		}
	}

	return result;
}
int 宣言強制要請(pcCharacter pchara , pcDroping pdrop)
{
	pcSpell pcspl = boost::dynamic_pointer_cast<cSpell>(pdrop);
	if(pcspl == NULL)
	{
		assert(!_T("error_スペル以外を宣言"));
		return false;
	}

	vector<pcDroping> ObjectList;
	if(!pcspl->宣言オブジェクト自動選択(ObjectList)) return false;//オブジェクトが取れなかった

	pcspl->宣言メッセージ定型(pchara, ObjectList);
	return 宣言要請(pchara,ObjectList,pdrop);
}

int スキル(cRenderDevice *pDev ,vector<pcDroping> &ObjectList, pcDroping pdrop)
{
	pcSpell pcSpl = boost::dynamic_pointer_cast<cSpell>(pdrop);
	if(pcSpl == NULL)
	{
		assert(!_T("error_スペル以外をスキル"));
		return false;
	}

	int result = 1;

	cValiableField val;
	val.doubles.dim(変数_汎用ブール) = result;
	sg_pDungeonSystem->CutInM().CutIn(sg_pDungeonSystem->pPlayerChara(),スキル直前_タイミング,val);
	if(!val.doubles.dim(変数_汎用ブール)) return false;

	if(pcSpl->スキルコスト満腹度(sg_pDungeonSystem->pPlayerChara()) > sg_pDungeonSystem->pPlayerChara()->Stomach)
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spellスキル満腹度不足メッセージ"),valiable);
		sg_pDungeonSystem->メニューを閉じる();
		return false;
	}

	result = pcSpl->スキル_メッセージ(sg_pDungeonSystem->pPlayerChara(), ObjectList);


	if(result)
	{//使用に成功した
		sg_pDungeonSystem->メニューを閉じる();
		sg_pDungeonSystem->ターンエンド();

	}
	sg_pDungeonSystem->メニューを閉じる();

	result = スキル要請(sg_pDungeonSystem->pPlayerChara(), ObjectList, pdrop);

	return true;
}
int スキル要請(pcCharacter pchara ,vector<pcDroping> &ObjectList, pcDroping pdrop)
{
	pcSpell pcspl = boost::dynamic_pointer_cast<cSpell>(pdrop);
	if(pcspl == NULL)
	{
		assert(!_T("error_スペル以外をスキル"));
		return false;
	}
	int result;

	result = pcspl->スキル使用(pchara, ObjectList);

	return result;
}


//---------------------------------------------
//trap
int 踏む(cRenderDevice *pDev, pcDroping pdrop)
{
	return 踏み要請(pdrop);
}
int 踏み要請(pcDroping pdrop)
{
	pcTrap pctr = boost::dynamic_pointer_cast<cTrap>(pdrop);
	if(pctr == NULL)
	{
		assert(!_T("error_ワナ以外を踏む"));
		return false;
	}

	int result = pctr->発動要請();

	sg_pDungeonSystem->メニューを閉じる();
	sg_pDungeonSystem->ターンエンド();

	return result;
}

//---------------------------------------------
//stair
int 上る(cRenderDevice *pDev, pcDroping pdrop)
{
	sg_pDungeonSystem->AnimationManager().
				Anime_PlaySE(_T("step.wav"),sg_pDungeonSystem->pPlayerChara()->足元地形()->place);
	return 昇降要請();
}
int 下りる(cRenderDevice *pDev, pcDroping pdrop)
{
	sg_pDungeonSystem->AnimationManager().
				Anime_PlaySE(_T("step.wav"),sg_pDungeonSystem->pPlayerChara()->足元地形()->place);
	return 昇降要請();
}
int やめる(cRenderDevice *pDev, pcDroping pdrop)
{
	sg_pDungeonSystem->メニューを閉じる();
	return true;
}
int 一旦中断する(cRenderDevice *pDev)
{
	sg_pDungeonSystem->RequestFloorSuspend();
	sg_pDungeonSystem->メニューを閉じる();
	sg_pDungeonSystem->フロア更新要請();
	return true;
}
int 昇降要請()
{
	sg_pDungeonSystem->メニューを閉じる();
	sg_pDungeonSystem->フロア更新要請();
	return true;
}


//---------------------------------------------
//Shop
int キープする(cRenderDevice *pDev, pcDroping pdrop)
{
	pcShopDroping psd = boost::dynamic_pointer_cast<cShopDroping>(pdrop);
	if(psd == NULL)
	{
		assert(!_T("cShopDroping以外をキープする"));
		return false;
	}

	if(g_pSaveData()->pSaveStore()->hinaKeepItem.size() >= g_pSaveData()->LevelHinaKeepNum())
	{//キープできない
	
		map<tstring,StyleString> val;
		g_Langメッセージ(_T("キープ拒否メッセージ"),val);
		sg_pDungeonSystem->メニューを閉じる();
		return false;
	}

	vector<pcDroping>& vpdrop = g_pSaveData()->pSaveStore()->hinaItem;
	vector<pcDroping>::iterator itr = g_pSaveData()->pSaveStore()->hinaItem.begin();

	itr = std::find(vpdrop.begin(),vpdrop.end(),psd->item_);

	g_pSaveData()->pSaveStore()->hinaKeepItem.push_back(*itr);
	sg_pDungeonSystem->アイテム店フラグ((*itr), cDroping::SHOPFLAG_YAKU);

	vpdrop.erase(itr);

	map<tstring,StyleString> val;
	val[_T("Item")] = pdrop->FullName();
	g_Langメッセージ(_T("キープするメッセージ"),val);

	sg_pDungeonSystem->メニューを閉じる();
	return true;
}
int キープをやめる(cRenderDevice *pDev, pcDroping pdrop)
{
	pcShopDroping psd = boost::dynamic_pointer_cast<cShopDroping>(pdrop);
	if(psd == NULL)
	{
		assert(!_T("cShopDroping以外をキープをやめる"));
		return false;
	}

	vector<pcDroping>& vpdrop = g_pSaveData()->pSaveStore()->hinaKeepItem;
	vector<pcDroping>::iterator itr = g_pSaveData()->pSaveStore()->hinaKeepItem.begin();

	itr = std::find(vpdrop.begin(),vpdrop.end(),psd->item_);

	g_pSaveData()->pSaveStore()->hinaItem.push_back(*itr);
	sg_pDungeonSystem->アイテム店フラグ((*itr), cDroping::SHOPFLAG_YAKU);


	vpdrop.erase(itr);

	map<tstring,StyleString> val;
	val[_T("Item")] = pdrop->FullName();
	g_Langメッセージ(_T("キープをやめるメッセージ"),val);

	sg_pDungeonSystem->メニューを閉じる();
	return true;
}
int ポイント交換(cRenderDevice *pDev, pcDroping pdrop)
{
	pcShopDroping psd = boost::dynamic_pointer_cast<cShopDroping>(pdrop);
	if(psd == NULL)
	{
		assert(!_T("cShopDroping以外をポイント交換"));
		return false;
	}
	if(sg_pDungeonSystem->pSaveData->pSaveStore()->yaku < psd->item_->厄ポイント単体買値())
	{
		//厄ポイント不足
		return false;
	}

	
	sg_pDungeonSystem->pSaveData->pSaveStore()->yaku -= psd->item_->厄ポイント単体買値();

	vector<pcDroping>& vpdrop = g_pSaveData()->pSaveStore()->hinaKeepItem;
	vector<pcDroping>::iterator itr;
	itr = std::find(vpdrop.begin(),vpdrop.end(),psd->item_);
	if(itr == vpdrop.end())
	{//みつからない
	
		vector<pcDroping>& vpdrop = g_pSaveData()->pSaveStore()->hinaItem;
		vector<pcDroping>::iterator itr;
		itr = std::find(vpdrop.begin(),vpdrop.end(),psd->item_);
		//見つかるはず
		sg_pDungeonSystem->遠隔拾得要請_持ち物余白無考慮(sg_pDungeonSystem->pPlayerChara(),*itr);
		sg_pDungeonSystem->アイテム店フラグ((*itr), cDroping::SHOPFLAG_NONE);
		sg_pDungeonSystem->アイテム割引フラグ((*itr), cDroping::SHOPDISCOUNTFLAG_NONE);


		vpdrop.erase(itr);
	}
	else
	{//みつけた
		sg_pDungeonSystem->遠隔拾得要請_持ち物余白無考慮(sg_pDungeonSystem->pPlayerChara(),*itr);
		sg_pDungeonSystem->アイテム店フラグ((*itr), cDroping::SHOPFLAG_NONE);
		sg_pDungeonSystem->アイテム割引フラグ((*itr), cDroping::SHOPDISCOUNTFLAG_NONE);

		vpdrop.erase(itr);
	}

	map<tstring,StyleString> val;
	val[_T("Item")] = pdrop->FullName();
	g_Langメッセージ(_T("ポイント交換メッセージ"),val);

	sg_pDungeonSystem->メニューを閉じる();

	return true;
}
int 買う(cRenderDevice *pDev, pcDroping pdrop)
{
	//現在霧雨魔法店専用
	pcShopDroping psd = boost::dynamic_pointer_cast<cShopDroping>(pdrop);
	if(psd == NULL)
	{
		assert(!_T("cShopDroping以外を買う"));
		return false;
	}
	map<tstring, StyleString> val;
	if(sg_pDungeonSystem->主人公お金所持量() < psd->item_->内包アイテム込み買値(sg_pDungeonSystem->拠点フラグ()))
	{
		//不足
		sg_pDungeonSystem->メニューを閉じる();
		g_Langメッセージ(_T("霧雨魔法店買う不足メッセージ"),val);
		return true;
	}

	pcCharacter seller = psd->item_->Holder();

	sg_pDungeonSystem->主人公お金増減( - psd->item_->内包アイテム込み買値(sg_pDungeonSystem->拠点フラグ()));

	sg_pDungeonSystem->泥棒要請(sg_pDungeonSystem->pPlayerChara(), psd->item_, false, true);
	sg_pDungeonSystem->アイテム店フラグ(psd->item_, cDroping::SHOPFLAG_NONE);
	sg_pDungeonSystem->アイテム割引フラグ(psd->item_, cDroping::SHOPDISCOUNTFLAG_NONE);
	
	sg_pDungeonSystem->メニューを閉じる();

	cMoney moneystr( psd->item_->内包アイテム込み買値(sg_pDungeonSystem->拠点フラグ()) );
	val[_T("Value")] = moneystr.caption();
	val[_T("Item")] = psd->item_->FullName();

	sg_pDungeonSystem->CutInM().CutIn(seller, 売買直後_タイミング);
	sg_pDungeonSystem->CutInM().CutIn(sg_pDungeonSystem->pPlayerChara(), 売買直後_タイミング);
	g_Langメッセージ(_T("霧雨魔法店買う完了メッセージ"),val);

	return true;
}
int 売る(cRenderDevice *pDev, pcDroping pdrop)
{
	//現在霧雨魔法店専用
	pcShopDroping psd = boost::dynamic_pointer_cast<cShopDroping>(pdrop);
	if(psd == NULL)
	{
		assert(!_T("cShopDroping以外を売る"));
		return false;
	}

	sg_pDungeonSystem->主人公お金増減( psd->item_->内包アイテム込み売値());

	pcCharacter buyer = psd->sellchara_;
	
	map<tstring, StyleString> val;
	cMoney moneystr(psd->item_->内包アイテム込み売値());
	val[_T("Value")] = moneystr.caption();
	val[_T("Item")] = psd->item_->FullName();

	sg_pDungeonSystem->泥棒要請(psd->sellchara_, psd->item_, false, true);
	sg_pDungeonSystem->アイテム店フラグ(psd->item_, cDroping::SHOPFLAG_SHOP);
	sg_pDungeonSystem->アイテム割引フラグ(psd->item_, cDroping::SHOPDISCOUNTFLAG_NONE);
	
	sg_pDungeonSystem->メニューを閉じる();

	sg_pDungeonSystem->CutInM().CutIn(sg_pDungeonSystem->pPlayerChara(), 売買直後_タイミング);
	sg_pDungeonSystem->CutInM().CutIn(buyer, 売買直後_タイミング);

	g_Langメッセージ(_T("霧雨魔法店売る完了メッセージ"),val);

	return true;
}

int 店買う(cRenderDevice *pDev, pcDroping pdrop)
{
	pcShopDroping psd = boost::dynamic_pointer_cast<cShopDroping>(pdrop);
	if(psd == NULL)
	{
		assert(!_T("cShopDroping以外を店買う"));
		return false;
	}

	pcCharacter pchara = psd->item_->Holder();
	int money = psd->item_->内包アイテム込み買値(sg_pDungeonSystem->拠点フラグ());
	map<tstring, StyleString> val;
	if(sg_pDungeonSystem->GameScreenInterface.money.value + sg_pDungeonSystem->店ツケ可能金額() - sg_pDungeonSystem->pSaveQuest->ShopDebt - money >= 0)
	{//つければ足りる
		sg_pDungeonSystem->泥棒要請(sg_pDungeonSystem->pPlayerChara(), psd->item_, false, true);
		sg_pDungeonSystem->店アイテム購入清算(pchara);
		g_Langメッセージ(_T("店買う完了メッセージ"),val);
		sg_pDungeonSystem->メニューを閉じる();
	}
	else
	{//つけても足りない
		g_Langメッセージ(_T("店買う不足メッセージ"),val);
		sg_pDungeonSystem->メニューを閉じる();
	}

	return true;
}
int 店売る(cRenderDevice *pDev, pcDroping pdrop)
{
	pcShopDroping psd = boost::dynamic_pointer_cast<cShopDroping>(pdrop);
	if(psd == NULL)
	{
		assert(!_T("cShopDroping以外を売る"));
		return false;
	}
	map<tstring, StyleString> val;
	if(!sg_pDungeonSystem->落ち物放棄可(psd->item_->Holder(), psd->item_))
	{
		val[_T("Item")] = pdrop->FullName();
		g_Langメッセージ(_T("店売る不可メッセージ"),val);
		sg_pDungeonSystem->メニューを閉じる();
		return false;
	}

	sg_pDungeonSystem->泥棒要請(psd->sellchara_, psd->item_, false, true);

	sg_pDungeonSystem->店アイテム売却清算(psd->sellchara_);
	g_Langメッセージ(_T("店売る完了メッセージ"),val);
	sg_pDungeonSystem->メニューを閉じる();


	return true;
}
int まとめて買う(cRenderDevice *pDev, vector<pcDroping>& pdroplist)
{

	int i;
	for(i=0;i<pdroplist.size();i++)
	{
		if(!買う(pDev, pdroplist[i])) break;
	}

	return true;
}


int まとめて売る(cRenderDevice *pDev, vector<pcDroping>& pdroplist)
{

	int i;
	for(i=0;i<pdroplist.size();i++)
	{
		if(!売る(pDev, pdroplist[i])) break;
	}

	return true;
}

int まとめて店買う(cRenderDevice *pDev, vector<pcDroping>& pdroplist)
{

	int i;
	for(i=0;i<pdroplist.size();i++)
	{
		if(!店買う(pDev, pdroplist[i])) break;
	}

	return true;
}


int まとめて店売る(cRenderDevice *pDev, vector<pcDroping>& pdroplist)
{

	int i;
	for(i=0;i<pdroplist.size();i++)
	{
		if(!店売る(pDev, pdroplist[i])) break;
	}

	return true;
}

