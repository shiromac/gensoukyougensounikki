#include "stdafx.h"
#include "cDungeonSystem.h"
#include "OtherChara1.h"
#include "Language.h"
#include "cDropingFunc.h"
#include "cCommandDroping.h"
#include "cSaveStore.h"
#include "cStringWindow.h"
#include "cListWindowAdv.h"
#include "cNumSetWindow.h"
#include "cDropingDistribution.h"
#include "ScriptDecodeFunction.h"
#include "fireCommand.h"
#include "cEquipment.h"
#include "GameIdiom.h"

#define DEF_COMMANDCLASS(name,deftext) \
class cCommand##name## : public cCommand\
{\
public:\
	cCommand##name##(void){caption = deftext;};\
	cCommand##name##(StyleString s){caption = s;};\
	virtual ~cCommand##name##(void){};\
	virtual int Action(IDirect3DDevice9 *pDev);\



DEF_COMMANDCLASS(_StoreManege,g_Lang(_T("倉庫の管理")))
	//virtual StyleString shortExplanationText();
};
class cCommand_Store : public cCommand
{
public:
	virtual ~cCommand_Store(void){};
	virtual int Action(IDirect3DDevice9 *pDev);
	virtual StyleString shortExplanationText();
	cCommand_Store(StyleString s, int storeindex, pcMemoString memo, int capacity)
	:storeindex_(storeindex),memo_(memo),capacity_(capacity){caption = s;};
	int storeindex_;
	pcMemoString memo_;
	int capacity_;
	virtual void resetCaption();
};
DEF_COMMANDCLASS(_StoreLook,g_Lang(_T("倉庫をのぞく")))
	//virtual StyleString shortExplanationText();
	cCommand_StoreLook(StyleString s, int storeindex)
	:storeindex_(storeindex){caption = s;};
	int storeindex_;
};
DEF_COMMANDCLASS(_inStore,g_Lang(_T("倉庫に入れる")))
	//virtual StyleString shortExplanationText();
	cCommand_inStore(StyleString s, int storeindex, int capacity)
	:storeindex_(storeindex),capacity_(capacity){caption = s;};
	int storeindex_;
	int capacity_;
};
DEF_COMMANDCLASS(_inAllStore,g_Lang(_T("倉庫に全部入れる")))
	//virtual StyleString shortExplanationText();
	cCommand_inAllStore(StyleString s, int storeindex, int capacity)
	:storeindex_(storeindex),capacity_(capacity){caption = s;};
	int storeindex_;
	int capacity_;
};
DEF_COMMANDCLASS(_outStore,g_Lang(_T("倉庫から出す")))
	cCommand_outStore(StyleString s, int storeindex)
	:storeindex_(storeindex){caption = s;};
	//virtual StyleString shortExplanationText();
	int storeindex_;
};
DEF_COMMANDCLASS(_StoreMemo,g_Lang(_T("メモ")))
	cCommand_StoreMemo(StyleString s, int storeindex, pcMemoString memo)
	:storeindex_(storeindex),memo_(memo){caption = s;};
	//virtual StyleString shortExplanationText();
	int storeindex_;
	pcMemoString memo_;
};
DEF_COMMANDCLASS(_StoreErase,g_Lang(_T("倉庫を消す")))
	cCommand_StoreErase(StyleString s, int storeindex)
	:storeindex_(storeindex){caption = s;};
	//virtual StyleString shortExplanationText();
	int storeindex_;
};
DEF_COMMANDCLASS(_StoreMake,g_Lang(_T("倉庫を作る")))
};
DEF_COMMANDCLASS(_StoreCapacityExtend,g_Lang(_T("倉庫の容量を増やす")))
	virtual StyleString shortExplanationText();
};

int cCommand_StoreManege::Action(IDirect3DDevice9 *pDev)
{
	//----------------------------------
	//倉庫のかんり
	//----------------------------------
	pcControlLayer pccl;
	pcSelectWindow pcsw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

	vector<vector<pcDroping>> & storeItem = g_pSaveData()->pSaveStore()->storeItem;
	int i,size = storeItem.size();
	
	
	if(size == 0)
	{
		size = 1;
		storeItem.resize(size);
		g_pSaveData()->pSaveStore()->storeMemo.push_back(pcMemoString(new cMemoString));
	}
	

	int mass = 0;
	for(i=0;i<size;i++)
	{
		mass += sg_pDungeonSystem->ItemSize(g_pSaveData()->pSaveStore()->storeItem[i]);
	}
	int maxcapacity = g_pSaveData()->LevelStoreSpaceNum();//仮


	for(i=0;i<size;i++)
	{
		StyleString tstr;
		pcCommand pcom;
		pcsw->commandList.push_back(pcom = pcCommand(new cCommand_Store(tstr,
			i,
			g_pSaveData()->pSaveStore()->storeMemo[i], 
			maxcapacity - mass + sg_pDungeonSystem->ItemSize(g_pSaveData()->pSaveStore()->storeItem[i]))));
		pcom->resetCaption();
	}
	if(size < 20)
	{
		pcsw->commandList.push_back(pcCommand(new cCommand_StoreMake));
	}
	pcsw->commandList.push_back(pcCommand(new cCommand_StoreCapacityExtend));

	pcsw->Init(pDev, 20, min(10,pcsw->commandList.size()));
	pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(2));
	pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(2));


	return true;
}
void cCommand_Store::resetCaption()
{
	caption.clear();

	caption += g_Lang(_T("倉庫"));
	caption += _T(" ");
	caption += setStyle(storeindex_+1);
	caption += _T(". ");
	if(g_pSaveData()->pSaveStore()->storeMemo[storeindex_]->getDataSize() > 0)
	{
		caption +=  _T(" (");
		caption += g_pSaveData()->pSaveStore()->storeMemo[storeindex_]->str();
		caption +=  _T(") ");
	}

	caption +=  _T("<");
	caption += setStyle((int)sg_pDungeonSystem->ItemSize(g_pSaveData()->pSaveStore()->storeItem[storeindex_]));
	caption +=  _T(">");
}
int cCommand_Store::Action(IDirect3DDevice9 *pDev)
{
	pcControlLayer pccl;
	pcSelectWindow pcsw;

	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));

	pccl->Init(sg_pDungeonSystem->pDevice_D3D);
	pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

	pcsw->commandList.push_back(pcCommand(new cCommand_outStore(g_Lang(_T("倉庫から出す")),storeindex_) ));
	pcsw->commandList.push_back(pcCommand(new cCommand_inStore(g_Lang(_T("倉庫に入れる")),storeindex_,capacity_)));
	pcsw->commandList.push_back(pcCommand(new cCommand_inAllStore(g_Lang(_T("倉庫に全部入れる")),storeindex_,capacity_)));
	pcsw->commandList.push_back(pcCommand(new cCommand_StoreLook(g_Lang(_T("倉庫をのぞく")),storeindex_)));
	pcsw->commandList.push_back(pcCommand(new cCommand_StoreMemo(g_Lang(_T("メモ")),storeindex_,memo_) ));
	
	if(storeindex_ > 0 && g_pSaveData()->pSaveStore()->storeItem[storeindex_].empty())
	{
		pcsw->commandList.push_back(pcCommand(new cCommand_StoreErase(g_Lang(_T("倉庫を消す")),storeindex_) ));
	}

	int strsize = 0;
	int i;
	for(i=0;i<pcsw->commandList.size();i++)
	{
		strsize = max(strsize,pcsw->commandList[i]->caption.length());
	}
	strsize = max(strsize,3);
	strsize = min(strsize,20);

	pcsw->Init(sg_pDungeonSystem->pDevice_D3D, strsize, pcsw->commandList.size());
	pcsw->setRight(sg_pDungeonSystem->GameScreenInterface.menuPosRight(1));
	pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(2));


	pcsw->playsound_decide();
	return true;
}

StyleString cCommand_Store::shortExplanationText()
{
	map<tstring,StyleString> val;
	val[_T("Value")] = setStyle(-sg_pDungeonSystem->ItemSize(g_pSaveData()->pSaveStore()->storeItem[storeindex_]) + capacity_);
	return g_Lang(_T("倉庫Explanation"),val);
}
int cCommand_StoreLook::Action(IDirect3DDevice9 *pDev)
{

	//----------------------------------
	//倉庫をみる
	//----------------------------------
	vector<pcDroping>& Objects = g_pSaveData()->pSaveStore()->storeItem[storeindex_];
	
	pcStoreDroping pstore = pcStoreDroping(new cStoreDroping(g_pSaveData()->pSaveStore()->storeItem[storeindex_]));
	
	pcControlLayer pccl;
	pcListWindowAdv pcladw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	pccl->WindowList.push_back(pcladw = pcListWindowAdv(new cListWindowAdv));

	static vector<pcDroping> エスケープリスト;

	pcDroping pdrop;
	if(Objects.size() != 0)
	{//なにかある
		pcladw->vpDroplist.push_back(Objects);
		pcladw->pvpDroplist_forSort.push_back(&Objects);
	}
	else
	{//なにもない
		エスケープリスト.clear();
		エスケープリスト.push_back(pcDroping(new cDropingNothing));
		pcladw->pvpDroplist_forSort.push_back(&エスケープリスト);
		pcladw->vpDroplist.push_back(エスケープリスト);
	}
	StyleString sstr;
	sstr += g_Lang(_T("倉庫"));
	sstr += _T(" ");
	sstr += setStyle(storeindex_+1);
	sstr += _T(". ");
	if(g_pSaveData()->pSaveStore()->storeMemo[storeindex_]->getDataSize() > 0)
	{
		sstr +=  _T(" (");
		sstr += g_pSaveData()->pSaveStore()->storeMemo[storeindex_]->str();
		sstr +=  _T(") ");
	}
	pcladw->pvpDroplistName.push_back(sstr.conclete_tstr());


	pcladw->Init(pDev, 20, 10);
	pcladw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(2));
	pcladw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(3));
	pcladw->MultiSelect = FALSE;

	return true;
}

int cCommand_inStore::Action(IDirect3DDevice9 *pDev)
{


	//----------------------------------
	//持ち物を倉庫に入れる
	//----------------------------------
	vector<pcDroping> Objects = sg_pDungeonSystem->pPlayerChara()->holdItem;

	pcStoreDroping pstore = pcStoreDroping(new cStoreDroping(g_pSaveData()->pSaveStore()->storeItem[storeindex_]));
	pstore->capacity_ = capacity_;

	pcControlLayer pccl;
	pcListWindowAdv pcladw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	pccl->WindowList.push_back(pcladw = pcListWindowAdv(new cListWindowAdv));

	static vector<pcDroping> エスケープリスト;

	pcDroping pdrop;
	if(Objects.size() != 0)
	{//なにかある
		int i;
		pcladw->vpDroplist.resize(1);
		for(i=0;i<Objects.size();i++)
		{
			//新規
			pcDropingFunc pcdf = pcDropingFunc(new cDropingFunc);
			pcdf->Subject = pstore;
			pcdf->Verb = _T("倉庫に入れる");
			pcdf->Objects.push_back(Objects[i]);

			//パック
			pdrop = DropingFuncToDroping(pcdf);

			pcladw->vpDroplist[0].push_back(pdrop);
		}	
		//pcladw->pvpDroplist_forSort.push_back(&Objects);
	}
	else
	{//なにもない
		エスケープリスト.clear();
		エスケープリスト.push_back(pcDroping(new cDropingNothing));
		pcladw->pvpDroplist_forSort.push_back(&エスケープリスト);
		pcladw->vpDroplist.push_back(エスケープリスト);
	}
	pcladw->pvpDroplistName.push_back(g_Lang(_T("何を？_[複数選択可] (持ち物)")).conclete_tstr());


	pcladw->Init(pDev, 20, 10);
	pcladw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(2));
	pcladw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(3));
	pcladw->MultiSelect = TRUE;


	return true;
}

int cCommand_inAllStore::Action(IDirect3DDevice9 *pDev)
{


	//----------------------------------
	//持ち物を倉庫に入れる
	//----------------------------------
	vector<pcDroping> Objects = sg_pDungeonSystem->pPlayerChara()->holdItem;

	pcStoreDroping pstore = pcStoreDroping(new cStoreDroping(g_pSaveData()->pSaveStore()->storeItem[storeindex_]));
	pstore->capacity_ = capacity_;

	pcDroping pdrop;
	if(Objects.size() != 0)
	{//なにかある
		int i;
		for(i=0;i<Objects.size();i++)
		{
			if(!倉庫に入れる(pDev,Objects[i],pstore))
			{
				break;
			}
		}
	}
	else
	{
		map<tstring, StyleString> valiable;
		sg_pDungeonSystem->メッセージ(g_Lang(_T("倉庫全部入れ失敗メッセージ"),valiable));
		sg_pDungeonSystem->メニューを閉じる();
	}

	return true;
}
int cCommand_outStore::Action(IDirect3DDevice9 *pDev)
{

	//----------------------------------
	//持ち物を倉庫から出す
	//----------------------------------
	vector<pcDroping> Objects = g_pSaveData()->pSaveStore()->storeItem[storeindex_];

	pcStoreDroping pstore = pcStoreDroping(new cStoreDroping(g_pSaveData()->pSaveStore()->storeItem[storeindex_]));
	
	pcControlLayer pccl;
	pcListWindowAdv pcladw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	pccl->WindowList.push_back(pcladw = pcListWindowAdv(new cListWindowAdv));

	static vector<pcDroping> エスケープリスト;

	pcDroping pdrop;
	if(Objects.size() != 0)
	{//なにかある
		int i;
		pcladw->vpDroplist.resize(1);
		for(i=0;i<Objects.size();i++)
		{
			//新規
			pcDropingFunc pcdf = pcDropingFunc(new cDropingFunc);
			pcdf->Subject = pstore;
			pcdf->Verb = _T("倉庫から出す");
			pcdf->Objects.push_back(Objects[i]);

			//パック
			pdrop = DropingFuncToDroping(pcdf);

			pcladw->vpDroplist[0].push_back(pdrop);
		}	
		//pcladw->pvpDroplist_forSort.push_back(&Objects);
	}
	else
	{//なにもない
		エスケープリスト.clear();
		エスケープリスト.push_back(pcDroping(new cDropingNothing));
		pcladw->pvpDroplist_forSort.push_back(&エスケープリスト);
		pcladw->vpDroplist.push_back(エスケープリスト);
	}
	pcladw->pvpDroplistName.push_back(g_Lang(_T("何を？_[複数選択可] (倉庫)")).conclete_tstr());


	pcladw->Init(pDev, 20, 10);
	pcladw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(2));
	pcladw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(3));
	pcladw->MultiSelect = TRUE;


	return true;
}
int cCommand_StoreMemo::Action(IDirect3DDevice9 *pDev)
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
	pcstrw->targetStr = memo_;//これを先にすると初期文字がターゲットになる
	pcstrw->Init(pDev);
	
	sg_pDungeonSystem->メニューリレンダ();

	return false;
}
int cCommand_StoreErase::Action(IDirect3DDevice9 *pDev)
{
	if(g_pSaveData()->pSaveStore()->storeItem[storeindex_].size() > 0)
	{
		return false;
	}

	g_pSaveData()->pSaveStore()->storeItem.erase(g_pSaveData()->pSaveStore()->storeItem.begin() + storeindex_);
	g_pSaveData()->pSaveStore()->storeMemo.erase(g_pSaveData()->pSaveStore()->storeMemo.begin() + storeindex_);

	sg_pDungeonSystem->メニューを一枚閉じる();
	sg_pDungeonSystem->メニューを一枚閉じる();
	return true;
}

int cCommand_StoreMake::Action(IDirect3DDevice9 *pDev)
{

	vector<pcDroping> vpdrop;
	g_pSaveData()->pSaveStore()->storeItem.push_back(vpdrop);
	g_pSaveData()->pSaveStore()->storeMemo.push_back(pcMemoString(new cMemoString));
	
	sg_pDungeonSystem->メニューを一枚閉じる();
	return true;
}
StyleString cCommand_StoreCapacityExtend::shortExplanationText()
{

	map<tstring,StyleString> val;
	val[_T("ExtendLevel")] = setStyle(sg_pDungeonSystem->pSaveData->StoreSpaceExtendCount());
	val[_T("NowCapacity")] = setStyle(sg_pDungeonSystem->pSaveData->LevelStoreSpaceNum());
	val[_T("NextCapacity")] = setStyle(sg_pDungeonSystem->pSaveData->StoreSpaceNumForExtendNum(sg_pDungeonSystem->pSaveData->StoreSpaceExtendCount()+1));
	cMoney price(sg_pDungeonSystem->pSaveData->StoreSpaceExtendNeedMoney());
	val[_T("Price")] = price.caption();
	if(sg_pDungeonSystem->pSaveData->StoreSpaceExtendCount() >= sg_pDungeonSystem->pSaveData->StoreSpaceExtendCountMax())
	{//最大
		return g_Lang(_T("倉庫の容量を増やすLevelMaxExplanation"),val);
	}
	else
	{
		return g_Lang(_T("倉庫の容量を増やすExplanation"),val);
	}
}
static void cCommand_StoreCapacityExtend_fadeBlack() {
	sg_pDungeonSystem->EventManager().fadeIn(cColor(255,0,0,0), 40);
}
static void cCommand_StoreCapacityExtend_fadeOut() {
	sg_pDungeonSystem->EventManager().fadeOut(40);
}
int cCommand_StoreCapacityExtend::Action(IDirect3DDevice9 *pDev)
{
	if(sg_pDungeonSystem->pSaveData->StoreSpaceExtendCount() >= sg_pDungeonSystem->pSaveData->StoreSpaceExtendCountMax())
	{//最大
		return true;
	}

	if(sg_pDungeonSystem->主人公お金所持量() < sg_pDungeonSystem->pSaveData->StoreSpaceExtendNeedMoney())
	{
		//資金不足
		g_Langメッセージ(_T("警備メイド倉庫拡張資金不足メッセージ"),std::map<tstring, StyleString >());
		sg_pDungeonSystem->メニューを閉じる();
		return true;
	}

	sg_pDungeonSystem->主人公お金増減( - sg_pDungeonSystem->pSaveData->StoreSpaceExtendNeedMoney());
	sg_pDungeonSystem->pSaveData->StoreSpaceExtend();
	sg_pDungeonSystem->メニューを閉じる();
	
	g_Langメッセージ(_T("警備メイド倉庫拡張メッセージ_1"),std::map<tstring, StyleString >());
	sg_pDungeonSystem->メッセージ決定待ち();
	sg_pDungeonSystem->メッセージ消去();
	sg_pDungeonSystem->AnimationManager().Anime_Function(&cCommand_StoreCapacityExtend_fadeBlack);
	cCoordinate place(sg_pDungeonSystem->pPlayerChara()->placeX,sg_pDungeonSystem->pPlayerChara()->placeY);
	sg_pDungeonSystem->AnimationManager().Anime_Wait(place.x, place.y, 40);
	sg_pDungeonSystem->AnimationManager().Anime_PlaySE(_T("flame.wav"), place);
	sg_pDungeonSystem->AnimationManager().Anime_Wait(place.x, place.y, 30);
	sg_pDungeonSystem->AnimationManager().Anime_PlaySE(_T("freeze.wav"), place);
	sg_pDungeonSystem->AnimationManager().Anime_Wait(place.x, place.y, 30);
	sg_pDungeonSystem->AnimationManager().Anime_PlaySE(_T("makecocktail.wav"), place);
	sg_pDungeonSystem->AnimationManager().Anime_Wait(place.x, place.y, 60);
	sg_pDungeonSystem->AnimationManager().Anime_Function(&cCommand_StoreCapacityExtend_fadeOut);
	g_Langメッセージ(_T("警備メイド倉庫拡張メッセージ_2"),std::map<tstring, StyleString >());

	return true;
}
DEF_COMMANDCLASS(_BankManege,g_Lang(_T("貯金の管理")))
	//virtual StyleString shortExplanationText();
};
DEF_COMMANDCLASS(_BankIn,g_Lang(_T("もち金を預ける")))
	//virtual StyleString shortExplanationText();
};
DEF_COMMANDCLASS(_BankOut,g_Lang(_T("貯金を引き出す")))
	//virtual StyleString shortExplanationText();
};
DEF_COMMANDCLASS(_BankInAction,g_Lang(_T("もち金を預ける(アクション)")))
	//virtual StyleString shortExplanationText();
	unsigned long int money;
};
DEF_COMMANDCLASS(_BankOutAction,g_Lang(_T("もち金を引き出す(アクション)")))
	//virtual StyleString shortExplanationText();
	unsigned long int money;
};

int cCommand_BankManege::Action(IDirect3DDevice9 *pDev)
{
	//----------------------------------
	//貯金の管理
	//----------------------------------
	pcControlLayer pccl;
	pcSelectWindow pcsw;

	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));

	pccl->Init(sg_pDungeonSystem->pDevice_D3D);
	pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

	pcsw->commandList.push_back(pcCommand(new cCommand_BankIn ));
	pcsw->commandList.push_back(pcCommand(new cCommand_BankOut ));

	int strsize = 0;
	int i;
	for(i=0;i<pcsw->commandList.size();i++)
	{
		strsize = max(strsize,pcsw->commandList[i]->caption.length());
	}
	strsize = max(strsize,3);
	strsize = min(strsize,20);

	pcsw->Init(sg_pDungeonSystem->pDevice_D3D, strsize, pcsw->commandList.size());
	pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(2));
	pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(2));



	pcGameWindow pcgw;
	pccl->WindowList.push_back(pcgw = pcGameWindow(new cGameWindow));
	pcgw->Init(sg_pDungeonSystem->pDevice_D3D, 14, 2);
	pcgw->setRight(sg_pDungeonSystem->GameScreenInterface.menuPosRight(0));
	pcgw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(1));
	cMoney moneystr;
	pcgw->Text() = g_Lang(_T("貯金"));
	pcgw->Text() += _T("\n");

	moneystr.value = sg_pDungeonSystem->pSaveData->pSaveStore()->bank;
	pcgw->Text() += setStyle( moneystr.caption(),0xFFFFEE00);

	pcgw->enable = true;


	return true;
}
int cCommand_BankIn::Action(IDirect3DDevice9 *pDev)
{
	//----------------------------------
	//もち金を預ける
	//----------------------------------
	pcControlLayer pccl;
	pcNumSetWindow pcnw;

	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));

	pccl->Init(sg_pDungeonSystem->pDevice_D3D);
	pccl->WindowList.push_back(pcnw = pcNumSetWindow(new cNumSetWindow));

	cCommand_BankInAction* pcommand;
	pcnw->setCommand(pcCommand(pcommand = new cCommand_BankInAction ));
	
	pcommand->money = sg_pDungeonSystem->主人公お金所持量();
	pcnw->Init(sg_pDungeonSystem->pDevice_D3D,
				&(pcommand->money),
				0,
				min(sg_pDungeonSystem->主人公お金所持量(), sg_pDungeonSystem->主人公お金所持量最大() - sg_pDungeonSystem->主人公お金所持量()),
				10);

	cMoney moneystr;
	pcnw->Text() = g_Lang(_T("預ける金額"));
	pcnw->setKetaCaption(moneystr.caption_onlymeasureS(),0);
	pcnw->setKetaCaption(moneystr.caption_onlymeasureB(),2);

	pcnw->CenterX = SCREEN_X/2;
	pcnw->CenterY = SCREEN_Y/2;

	return true;
}
int cCommand_BankInAction::Action(IDirect3DDevice9 *pDev)
{
	//----------------------------------
	//もち金を預ける
	//----------------------------------

	cMoney moneystr;
	moneystr.value = money;
	map<tstring,StyleString> val;
	val[_T("Value")] = moneystr.caption();
	
	g_Langメッセージ(_T("もち金を預けるメッセージ"),val);

	sg_pDungeonSystem->主人公お金増減(-(int)money);
	sg_pDungeonSystem->pSaveData->pSaveStore()->bank += money;

	sg_pDungeonSystem->メニューを閉じる();

	return true;
}
int cCommand_BankOut::Action(IDirect3DDevice9 *pDev)
{
	//----------------------------------
	//貯金を引き出す
	//----------------------------------
	pcControlLayer pccl;
	pcNumSetWindow pcnw;

	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));

	pccl->Init(sg_pDungeonSystem->pDevice_D3D);
	pccl->WindowList.push_back(pcnw = pcNumSetWindow(new cNumSetWindow));

	cCommand_BankOutAction* pcommand;
	pcnw->setCommand(pcCommand(pcommand = new cCommand_BankOutAction ));
	
	pcommand->money = 0;
	pcnw->Init(sg_pDungeonSystem->pDevice_D3D,
				&(pcommand->money),
				0,
				sg_pDungeonSystem->pSaveData->pSaveStore()->bank,
				10);

	cMoney moneystr;
	pcnw->Text() = g_Lang(_T("引き出す金額"));
	pcnw->setKetaCaption(moneystr.caption_onlymeasureS(),0);
	pcnw->setKetaCaption(moneystr.caption_onlymeasureB(),2);

	pcnw->CenterX = SCREEN_X/2;
	pcnw->CenterY = SCREEN_Y/2;

	return true;
}
int cCommand_BankOutAction::Action(IDirect3DDevice9 *pDev)
{
	//----------------------------------
	//貯金を引き出す
	//----------------------------------

	cMoney moneystr;
	moneystr.value = money;
	map<tstring,StyleString> val;
	val[_T("Value")] = moneystr.caption();
	
	g_Langメッセージ(_T("貯金を引き出すメッセージ"),val);

	sg_pDungeonSystem->主人公お金増減(money);
	sg_pDungeonSystem->pSaveData->pSaveStore()->bank -= money;

	sg_pDungeonSystem->メニューを閉じる();

	return true;
}
int cOtherChara_ID_0::TalkEvent()
{
	cOtherChara::TalkEvent();

	pcControlLayer pccl;
	pcSelectWindow pcsw;

	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));

	pccl->Init(sg_pDungeonSystem->pDevice_D3D);
	pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

	pcsw->commandList.push_back(pcCommand(new cCommand_StoreManege ));
	pcsw->commandList.push_back(pcCommand(new cCommand_BankManege ));
	

	int strsize = 0;
	int i;
	for(i=0;i<pcsw->commandList.size();i++)
	{
		strsize = max(strsize,pcsw->commandList[i]->caption.length());
	}
	strsize = max(strsize,3);
	strsize = min(strsize,20);

	pcsw->Init(sg_pDungeonSystem->pDevice_D3D, strsize, pcsw->commandList.size());
	pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(1));
	pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(1));


	pcsw->playsound_decide();

	return true;
}
//--------------------------------------------------------------------
//ヒナ
//--------------------------------------------------------------------

DEF_COMMANDCLASS(_yakuChange,g_Lang(_T("厄ポイントの交換")))
	//virtual StyleString shortExplanationText();
};
DEF_COMMANDCLASS(_Uncurse,g_Lang(_T("解呪サービス")))
	//virtual StyleString shortExplanationText();
};

DEF_COMMANDCLASS(_yakuChange_PickItem,g_Lang(_T("掘り出し物")))
	//virtual StyleString shortExplanationText();
};
DEF_COMMANDCLASS(_yakuChange_KeepItem,g_Lang(_T("キープリスト")))
	virtual StyleString shortExplanationText();
};
int cCommand_Uncurse::Action(IDirect3DDevice9 *pDev)
{
	//----------------------------------
	//解呪サービス
	//----------------------------------
	pcCharacter pchara = sg_pDungeonSystem->pPlayerChara();

	vector<pcDroping> vpdrop = sg_pDungeonSystem->DropList();

	int i,eff = 0;

	if(pchara->足元() != NULL) vpdrop.push_back(pchara->足元());

	for(i=0;i<vpdrop.size();i++)
	{
		if(vpdrop[i]->state() == cDroping::STATE_CURSE)
		{
			sg_pDungeonSystem->アイテム祝福(vpdrop[i],false);
			eff++;
		}
	}

	GameIdiom::呪術悪性異常状態治療要請(pchara);

	if(eff)
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("cSpell_ID_5効果メッセージ"),valiable);
	}
	else
	{
		map<tstring, StyleString> valiable;
		g_Langメッセージ(_T("Spell効果無しメッセージ"),valiable);
	}
	sg_pDungeonSystem->メニューを閉じる();
	return true;
}
int cCommand_yakuChange::Action(IDirect3DDevice9 *pDev)
{
	//----------------------------------
	//厄ポイントの交換
	//----------------------------------
	pcControlLayer pccl;
	pcSelectWindow pcsw;

	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));

	pccl->Init(sg_pDungeonSystem->pDevice_D3D);
	pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

	pcsw->commandList.push_back(pcCommand(new cCommand_yakuChange_PickItem));
	pcsw->commandList.push_back(pcCommand(new cCommand_yakuChange_KeepItem));
	
	int strsize = 0;
	int i;
	for(i=0;i<pcsw->commandList.size();i++)
	{
		strsize = max(strsize,pcsw->commandList[i]->caption.length());
	}
	strsize = max(strsize,3);
	strsize = min(strsize,20);

	pcsw->Init(sg_pDungeonSystem->pDevice_D3D, strsize, pcsw->commandList.size());
	pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(2));
	pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(2));


	pcsw->playsound_decide();
	return true;
}
int cCommand_yakuChange_PickItem::Action(IDirect3DDevice9 *pDev)
{
	//----------------------------------
	//掘り出し物
	//----------------------------------

	vector<pcDroping>& Objects = g_pSaveData()->pSaveStore()->hinaItem;
	
	pcControlLayer pccl;
	pcListWindowAdv pcladw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	pccl->WindowList.push_back(pcladw = pcListWindowAdv(new cListWindowAdv));

	static vector<pcDroping> エスケープリスト;

	pcDroping pdrop;
	if(Objects.size() != 0)
	{//なにかある
		pcladw->vpDroplist.resize(pcladw->vpDroplist.size() + 1);
		vector<tstring> captions;
		captions.push_back(_T("説明"));
		captions.push_back(_T("ポイント交換"));
		captions.push_back(_T("キープする"));
		int i,size = Objects.size();
		for(i=0;i<size;i++)
		{
			pcShopDroping psd = pcShopDroping(new cShopDroping(Objects[size-i-1],captions));
			psd->Init(pDev,psd);
			pcladw->vpDroplist.back().push_back(psd);
		}
	}
	else
	{//なにもない
		エスケープリスト.clear();
		エスケープリスト.push_back(pcDroping(new cDropingNothing));
		pcladw->pvpDroplist_forSort.push_back(&エスケープリスト);
		pcladw->vpDroplist.push_back(エスケープリスト);
	}
	pcladw->pvpDroplistName.push_back(g_Lang(_T("掘り出し物")).conclete_tstr());


	pcladw->Init(pDev, 20, 10);
	pcladw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(2));
	pcladw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(3));
	pcladw->MultiSelect = FALSE;

	pcGameWindow pcgw;
	pccl->WindowList.push_back(pcgw = pcGameWindow(new cGameWindow));
	pcgw->Init(pDev, 12, 1);
	pcgw->setRight(sg_pDungeonSystem->GameScreenInterface.menuPosRight(0));
	pcgw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(0));
	pcgw->Text() = g_Lang(_T("厄ポイント")) + _T("  ") + setStyle((sg_pDungeonSystem->pSaveData->pSaveStore()->yaku), _T("%12.0f"), 0xFFEE00FF);
	pcgw->enable = true;
	return true;
}
int cCommand_yakuChange_KeepItem::Action(IDirect3DDevice9 *pDev)
{
	//----------------------------------
	//キープリスト
	//----------------------------------

	vector<pcDroping>& Objects = g_pSaveData()->pSaveStore()->hinaKeepItem;
	
	pcControlLayer pccl;
	pcListWindowAdv pcladw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	pccl->WindowList.push_back(pcladw = pcListWindowAdv(new cListWindowAdv));

	static vector<pcDroping> エスケープリスト;

	pcDroping pdrop;
	if(Objects.size() != 0)
	{//なにかある
		pcladw->vpDroplist.resize(pcladw->vpDroplist.size() + 1);
		vector<tstring> captions;
		captions.push_back(_T("説明"));
		captions.push_back(_T("ポイント交換"));
		captions.push_back(_T("キープをやめる"));
		int i,size = Objects.size();
		for(i=0;i<size;i++)
		{
			pcShopDroping psd = pcShopDroping(new cShopDroping(Objects[i],captions));
			psd->Init(pDev,psd);
			pcladw->vpDroplist.back().push_back(psd);
		}
	}
	else
	{//なにもない
		エスケープリスト.clear();
		エスケープリスト.push_back(pcDroping(new cDropingNothing));
		pcladw->pvpDroplist_forSort.push_back(&エスケープリスト);
		pcladw->vpDroplist.push_back(エスケープリスト);
	}
	pcladw->pvpDroplistName.push_back(g_Lang(_T("キープリスト")).conclete_tstr());


	pcladw->Init(pDev, 20, 10);
	pcladw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(2));
	pcladw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(3));
	pcladw->MultiSelect = FALSE;

	pcGameWindow pcgw;
	pccl->WindowList.push_back(pcgw = pcGameWindow(new cGameWindow));
	pcgw->Init(pDev, 12, 1);
	pcgw->setRight(sg_pDungeonSystem->GameScreenInterface.menuPosRight(0));
	pcgw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(0));
	pcgw->Text() = g_Lang(_T("厄ポイント")) + _T("  ") + setStyle((sg_pDungeonSystem->pSaveData->pSaveStore()->yaku), _T("%12.0f"), 0xFFEE00FF);
	pcgw->enable = true;
	return true;
}
StyleString cCommand_yakuChange_KeepItem::shortExplanationText()
{
	map<tstring,StyleString> val;
	val[_T("Value")] = g_pSaveData()->LevelHinaKeepNum();
	return g_Lang(_T("キープリストShortExplanation"),val);
}
int cOtherChara_ID_1::TalkEvent()
{
	cOtherChara::TalkEvent();
	pcControlLayer pccl;
	pcSelectWindow pcsw;

	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));

	pccl->Init(sg_pDungeonSystem->pDevice_D3D);
	pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

	pcsw->commandList.push_back(pcCommand(new cCommand_yakuChange ));
	pcsw->commandList.push_back(pcCommand(new cCommand_Uncurse ));

	int strsize = 0;
	int i;
	for(i=0;i<pcsw->commandList.size();i++)
	{
		strsize = max(strsize,pcsw->commandList[i]->caption.length());
	}
	strsize = max(strsize,3);
	strsize = min(strsize,20);

	pcsw->Init(sg_pDungeonSystem->pDevice_D3D, strsize, pcsw->commandList.size());
	pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(1));
	pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(1));


	pcsw->playsound_decide();

	return true;
}

//--------------------------------------------------------------------
//マリサ
//--------------------------------------------------------------------
DEF_COMMANDCLASS(_KirisameShop,g_Lang(_T("霧雨魔法店")))
	virtual StyleString shortExplanationText(){return g_Lang(_T("霧雨魔法店shortExplanationText"));};
	cCommand_KirisameShop(pcCharacter pchara, StyleString s):self_(pchara){caption = s;};
	pcCharacter self_;
};
DEF_COMMANDCLASS(_KirisameShop_Buy,g_Lang(_T("買う")))
	//virtual StyleString shortExplanationText();
	cCommand_KirisameShop_Buy(pcCharacter pchara, StyleString s):self_(pchara){caption = s;};
	pcCharacter self_;
};
DEF_COMMANDCLASS(_KirisameShop_Sell,g_Lang(_T("売る")))
	//virtual StyleString shortExplanationText();
	cCommand_KirisameShop_Sell(pcCharacter pchara, StyleString s):self_(pchara){caption = s;};
	pcCharacter self_;
};
int cCommand_KirisameShop::Action(IDirect3DDevice9 *pDev)
{
	pcControlLayer pccl;
	pcSelectWindow pcsw;

	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));

	pccl->Init(sg_pDungeonSystem->pDevice_D3D);
	pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

	pcsw->commandList.push_back(pcCommand(new cCommand_KirisameShop_Buy(self_ ,g_Lang(_T("買う")) ) ));
	pcsw->commandList.push_back(pcCommand(new cCommand_KirisameShop_Sell(self_ ,g_Lang(_T("売る")) ) ));
	
	int strsize = 0;
	int i;
	for(i=0;i<pcsw->commandList.size();i++)
	{
		strsize = max(strsize,pcsw->commandList[i]->caption.length());
	}
	strsize = max(strsize,3);
	strsize = min(strsize,20);

	pcsw->Init(sg_pDungeonSystem->pDevice_D3D, strsize, pcsw->commandList.size());
	pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(2));
	pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(2));


	pcsw->playsound_decide();

	return true;
}
int cCommand_KirisameShop_Buy::Action(IDirect3DDevice9 *pDev)
{
	//----------------------------------
	//買う
	//----------------------------------

	vector<pcDroping>& Objects = self_->holdItem;
	
	pcControlLayer pccl;
	pcListWindowAdv pcladw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	pccl->WindowList.push_back(pcladw = pcListWindowAdv(new cListWindowAdv));

	static vector<pcDroping> エスケープリスト;

	pcDroping pdrop;
	if(Objects.size() != 0)
	{//なにかある
		pcladw->vpDroplist.resize(pcladw->vpDroplist.size() + 1);
		vector<tstring> captions;
		captions.push_back(_T("買う"));
		captions.push_back(_T("説明"));
		int i,size = Objects.size();
		for(i=0;i<size;i++)
		{
			pcShopDroping psd = pcShopDroping(new cShopDroping(Objects[i],captions));
			psd->Init(pDev,psd);
			pcladw->vpDroplist.back().push_back(psd);
		}
	}
	else
	{//なにもない
		エスケープリスト.clear();
		エスケープリスト.push_back(pcDroping(new cDropingNothing));
		pcladw->pvpDroplist_forSort.push_back(&エスケープリスト);
		pcladw->vpDroplist.push_back(エスケープリスト);
	}
	pcladw->pvpDroplistName.push_back(g_Lang(_T("霧雨魔法店")).conclete_tstr());


	pcladw->Init(pDev, 20, 10);
	pcladw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(2));
	pcladw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(3));
	pcladw->MultiSelect = TRUE;
	pcladw->multiverbs.push_back(_T("まとめて買う"));

	return true;
}
int cCommand_KirisameShop_Sell::Action(IDirect3DDevice9 *pDev)
{
	//----------------------------------
	//売る
	//----------------------------------

	vector<pcDroping>& Objects = sg_pDungeonSystem->pPlayerChara()->holdItem;
	
	pcControlLayer pccl;
	pcListWindowAdv pcladw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	pccl->WindowList.push_back(pcladw = pcListWindowAdv(new cListWindowAdv));

	static vector<pcDroping> エスケープリスト;

	pcDroping pdrop;
	if(Objects.size() != 0)
	{//なにかある
		pcladw->vpDroplist.resize(pcladw->vpDroplist.size() + 1);
		vector<tstring> captions;
		captions.push_back(_T("売る"));
		captions.push_back(_T("説明"));
		int i,size = Objects.size();
		for(i=0;i<size;i++)
		{
			pcShopDroping psd = pcShopDroping(new cShopDroping(Objects[i],captions));
			psd->Init(pDev,psd);
			psd->sellchara_ = self_;
			pcladw->vpDroplist.back().push_back(psd);
		}
	}
	else
	{//なにもない
		エスケープリスト.clear();
		エスケープリスト.push_back(pcDroping(new cDropingNothing));
		pcladw->pvpDroplist_forSort.push_back(&エスケープリスト);
		pcladw->vpDroplist.push_back(エスケープリスト);
	}
	pcladw->pvpDroplistName.push_back(g_Lang(_T("持ち物")).conclete_tstr());


	pcladw->Init(pDev, 20, 10);
	pcladw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(2));
	pcladw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(3));
	pcladw->MultiSelect = TRUE;
	pcladw->multiverbs.push_back(_T("まとめて売る"));

	return true;
}

void cOtherChara_ID_2::CutIn(タイミング timing, cValiableField& valiable)
{
	cOtherChara::CutIn(timing,valiable);

	if(timing == 死亡直前_タイミング)
	{
		sg_pDungeonSystem->pSaveData->pSaveStore()->marisaShopItem = holdItem;
		
		vector<pcDroping> vpdrop = holdItem;
		int i;
		for(i=0;i<vpdrop.size();i++)
		{
			sg_pDungeonSystem->落ち物非破壊非設置放棄要請(vpdrop[i]);
		}
	}
	else if(timing == 売買直後_タイミング)
	{
		sg_pDungeonSystem->pSaveData->pSaveStore()->marisaShopItem = holdItem;
	}
}

void cOtherChara_ID_2::settingInit()
{
	cOtherChara::settingInit();
	cDropingDistribution dropDistribution;

	pcScriptRLayer homeDataBase(sg_pDungeonSystem->DataBase.HomeDataBase());
	if(homeDataBase == NULL)
	{
		return;
	}
	
	pcScriptRLayer marisaShop(homeDataBase->pmember(_T("MarisaShop")));
	if(marisaShop == NULL)
	{
		return;
	}
	
	DropingDistributionDecode(marisaShop,dropDistribution);

	if(sg_pDungeonSystem->pSaveData->pSaveStore()->marisaShopLastSiireLevel != sg_pDungeonSystem->pSaveData->playerLevel
		|| sg_pDungeonSystem->pSaveData->pSaveStore()->marisaShopLastSiireExp < sg_pDungeonSystem->pSaveData->playerExp - 1000)
	{
		//再仕入れ
		sg_pDungeonSystem->pSaveData->pSaveStore()->marisaShopLastSiireLevel = sg_pDungeonSystem->pSaveData->playerLevel;
		sg_pDungeonSystem->pSaveData->pSaveStore()->marisaShopLastSiireExp = sg_pDungeonSystem->pSaveData->playerExp;

		vector<pcDroping> vecDroping = sg_pDungeonSystem->pSaveData->pSaveStore()->marisaShopItem;
		int i,size = vecDroping.size();
		for(i=0;i<size;i++)
		{
			sg_pDungeonSystem->落ち物破壊要請(vecDroping[i],true);
		}

		sg_pDungeonSystem->pSaveData->pSaveStore()->marisaShopItem.clear();
	}
	
	vector<pcDroping>& marisaShopDroping = sg_pDungeonSystem->pSaveData->pSaveStore()->marisaShopItem;

	int i;
	for(i=0;;i++)
	{
		if(marisaShopDroping.size() >= 10) break;

		int id = dropDistribution.get(random());

		pcDroping pdrop = sg_pDungeonSystem->落ち物生成_設置なし(id);

		sg_pDungeonSystem->アイテム店フラグ(pdrop, cDroping::SHOPFLAG_SHOP);

		marisaShopDroping.push_back(pdrop);
	}

	
	int size = marisaShopDroping.size();
	for(i=0;i<size;i++)
	{
		sg_pDungeonSystem->遠隔拾得要請_持ち物余白無考慮(me(), marisaShopDroping[i]);
	}
	
}

int cOtherChara_ID_2::TalkEvent()
{
	cOtherChara::TalkEvent();
	pcControlLayer pccl;
	pcSelectWindow pcsw;

	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));

	pccl->Init(sg_pDungeonSystem->pDevice_D3D);
	pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

	pcsw->commandList.push_back(pcCommand(new cCommand_KirisameShop(me(),g_Lang(_T("霧雨魔法店"))) ));
	
	int strsize = 0;
	int i;
	for(i=0;i<pcsw->commandList.size();i++)
	{
		strsize = max(strsize,pcsw->commandList[i]->caption.length());
	}
	strsize = max(strsize,3);
	strsize = min(strsize,20);

	pcsw->Init(sg_pDungeonSystem->pDevice_D3D, strsize, pcsw->commandList.size());
	pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(1));
	pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(1));


	pcsw->playsound_decide();

	return true;
}
//--------------------------------------------------------------------
//ニトリ
//--------------------------------------------------------------------
DEF_COMMANDCLASS(_NitoriFactory, g_Lang(_T("ニトリ製作所")))
	//virtual StyleString shortExplanationText();
	virtual void didEndCommand(cCommand& caller);
	enum {
		delegateID_reinforce = 0,
		delegateID_combine = 1,
	};
};
DEF_COMMANDCLASS(_NitoriFactory_reinforce_item, _T("装備品を強化する->装備品名"))
	//virtual StyleString shortExplanationText();
	cCommand_NitoriFactory_reinforce_item(pcDroping item, int reinforce_fee, StyleString s):item_(item), reinforce_fee_(reinforce_fee){caption = s;};
	pcDroping item_;
	int reinforce_fee_;
	virtual pcDroping includeObjectiveDroping(pcDroping objectpdrop) {
		if(objectpdrop->属性.count(落ち物属性::装備品)) {
			return pcObjectiveDroping(new cObjectiveDroping( objectpdrop,
					pcCommand(new cCommand_NitoriFactory_reinforce_item(objectpdrop, reinforce_fee_, objectpdrop->FullName()))));
		}
		return NULLDROP;
	};
};
DEF_COMMANDCLASS(_NitoriFactory_combine_item1, _T("アイテムを合成する->アイテム名１"))
	//virtual StyleString shortExplanationText();
	cCommand_NitoriFactory_combine_item1(int combine_fee, pcDroping item1, StyleString s):combine_fee_(combine_fee), item1_(item1){caption = s;};
	virtual pcDroping includeObjectiveDroping(pcDroping objectpdrop) {
		return pcObjectiveDroping(new cObjectiveDroping( objectpdrop,
			pcCommand(new cCommand_NitoriFactory_combine_item1(combine_fee_, objectpdrop, objectpdrop->FullName()))));
	};
	pcDroping item1_;
	int combine_fee_;
};
DEF_COMMANDCLASS(_NitoriFactory_combine_item1_item2, _T("アイテムを合成する->アイテム名１->アイテム名２"))
	//virtual StyleString shortExplanationText();
	cCommand_NitoriFactory_combine_item1_item2(int combine_fee, pcDroping item1, pcDroping item2, StyleString s):combine_fee_(combine_fee), item1_(item1),item2_(item2){caption = s;};
	virtual pcDroping includeObjectiveDroping(pcDroping objectpdrop) {
		if(objectpdrop == item1_)
		{
			return NULLDROP;
		}
		return pcObjectiveDroping(new cObjectiveDroping( objectpdrop,
			pcCommand(new cCommand_NitoriFactory_combine_item1_item2(combine_fee_, item1_, objectpdrop, objectpdrop->FullName()))));
	};
	pcDroping item1_;
	pcDroping item2_;
	int combine_fee_;
};
DEF_COMMANDCLASS(_NitoriFactory_remodeling, g_Lang(_T("装備品を改造する")))
	virtual StyleString shortExplanationText(){return g_Lang(_T("装備品を改造するshortExplanationText"));};
	//virtual StyleString shortExplanationText();
};
DEF_COMMANDCLASS(_NitoriFactory_remodeling_item, g_Lang(_T("装備品を改造する->装備品名")))
	cCommand_NitoriFactory_remodeling_item(pcDroping item, StyleString s):item_(item){caption = s;};
	virtual StyleString shortExplanationText(){
		pcEquipment pequip = boost::shared_dynamic_cast<cEquipment>(item_);
		std::map<tstring, StyleString > val;
		if(pequip){
			cMoney money(pequip->改造費());
			val[_T("Money")] = money.caption();
			val[_T("Num")] = setStyle(pequip->overspec()+1);
		}
		else
		{
			val[_T("Money")] = _T("？");
			val[_T("Num")] = _T("？");
		}
		return g_Lang(_T("装備品を改造する->装備品名shortExplanationText"),val);
	}
	pcDroping item_;
};
int cCommand_NitoriFactory::Action(IDirect3DDevice9 *pDev)
{
	if(cOtherChara_ID_5::isCanUseShop())
	{
		//使用可能

		pcControlLayer pccl;
		pcSelectWindow pcsw;

		sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));

		pccl->Init(sg_pDungeonSystem->pDevice_D3D);
		pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

		pcCommand pcommand = pcCommand(new cCommand_NitoriFactory_reinforce(cOtherChara_ID_5::強化資金(), g_Lang(_T("装備品を強化する")) ) );
		pcommand->delegate_ = selfAsDelegate();
		pcommand->delegateID_ = delegateID_reinforce;
		pcsw->commandList.push_back(pcommand);

		pcommand = pcCommand(new cCommand_NitoriFactory_combine(cOtherChara_ID_5::合成資金(),g_Lang(_T("アイテムを合成する")) ) );
		pcommand->delegate_ = selfAsDelegate();
		pcommand->delegateID_ = delegateID_combine;
		pcsw->commandList.push_back(pcommand);
		
		pcsw->commandList.push_back(pcCommand(new cCommand_NitoriFactory_remodeling(g_Lang(_T("装備品を改造する")) ) ));

		int strsize = 0;
		int i;
		for(i=0;i<pcsw->commandList.size();i++)
		{
			strsize = max(strsize,pcsw->commandList[i]->caption.length());
		}
		strsize = max(strsize,3);
		strsize = min(strsize,20);

		pcsw->Init(sg_pDungeonSystem->pDevice_D3D, strsize, pcsw->commandList.size());
		pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(2));
		pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(2));

		pcsw->playsound_decide();
		
	}
	else
	{
		g_Langメッセージ(_T("ニトリ製作所資材不足メッセージ"),std::map<tstring, StyleString >());
		sg_pDungeonSystem->メニューを閉じる();
	}
	return true;
}
void cCommand_NitoriFactory::didEndCommand(cCommand& caller) {
	if (caller.delegateID_ == delegateID_reinforce) {
		cOtherChara_ID_5::ShopUsedSave();
	}
	else if (caller.delegateID_ == delegateID_combine) {
		cOtherChara_ID_5::ShopUsedSave();
	}
}
int cCommand_NitoriFactory_reinforce::Action(IDirect3DDevice9 *pDev)
{
	//----------------------------------
	//強化
	//----------------------------------
	if(sg_pDungeonSystem->主人公お金所持量() < reinforce_fee_)
	{
		//資金不足
		g_Langメッセージ(_T("ニトリ製作所資金不足メッセージ"),std::map<tstring, StyleString >());
		sg_pDungeonSystem->メニューを閉じる();
		return true;
	}

	vector<pcDroping>& Objects = sg_pDungeonSystem->pPlayerChara()->holdItem;
	
	pcControlLayer pccl;
	pcListWindowAdv pcladw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	pccl->WindowList.push_back(pcladw = pcListWindowAdv(new cListWindowAdv));

	static vector<pcDroping> エスケープリスト;

	pcDroping pdrop;
	if(Objects.size() != 0)
	{//なにかある
		pcladw->vpDroplist.resize(pcladw->vpDroplist.size() + 1);
		int i,size = Objects.size();
		for(i=0;i<size;i++)
		{
			if(Objects[i]->属性.count(落ち物属性::装備品))
			{
				pcCommand pcommand = pcCommand(new cCommand_NitoriFactory_reinforce_item(Objects[i], reinforce_fee_, Objects[i]->FullName()));
				pcommand->delegate_ = selfAsDelegate();
				pcladw->vpDroplist.back().push_back( pcObjectiveDroping(new cObjectiveDroping( Objects[i],
					pcommand)));
			}

			if(Objects[i]->内包落ち物対象可())
			{
				int k,size_include = Objects[i]->includedItem.size();
				
				for(k=0;k<size_include;k++)
				{
					pcDroping pdrop_include = Objects[i]->includedItem[k];
					if(pdrop_include->属性.count(落ち物属性::装備品))
					{
						pcCommand pcommand = pcCommand(new cCommand_NitoriFactory_reinforce_item(pdrop_include, reinforce_fee_, pdrop_include->FullName()));
						pcommand->delegate_ = selfAsDelegate();
						pcladw->vpDroplist.back().push_back( pcObjectiveDroping(new cObjectiveDroping( pdrop_include,
							pcommand)));
					}
				}
			}
		}
	}
	if(pcladw->vpDroplist.empty())
	{//なにもない
		エスケープリスト.clear();
		エスケープリスト.push_back(pcDroping(new cDropingNothing(g_Lang(_T("ニトリ強化可能アイテム無し")).conclete_tstr())));
		pcladw->pvpDroplist_forSort.push_back(&エスケープリスト);
		pcladw->vpDroplist.push_back(エスケープリスト);
	}
	pcladw->pvpDroplistName.push_back(g_Lang(_T("持っている装備品")).conclete_tstr());


	pcladw->Init(pDev, 20, 10);
	pcladw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(2));
	pcladw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(3));
	pcladw->MultiSelect = FALSE;

	return true;

}
int cCommand_NitoriFactory_reinforce_item::Action(IDirect3DDevice9 *pDev)
{
	sg_pDungeonSystem->主人公お金増減( - reinforce_fee_);
	sg_pDungeonSystem->アイテム強化(item_,false);
	std::map<tstring, StyleString > val;
	val[_T("Item")] = item_->FullName();
	g_Langメッセージ(_T("ニトリ強化完了メッセージ"),val);

	pcCommandDelegateObject shared_delegate = delegate_.lock();
	if(shared_delegate) {
		shared_delegate->didEndCommand(*this);
	}

	sg_pDungeonSystem->メニューを閉じる();
	return true;
}
int cCommand_NitoriFactory_combine::Action(IDirect3DDevice9 *pDev)
{
	//----------------------------------
	//合成
	//----------------------------------
	if(sg_pDungeonSystem->主人公お金所持量() < combine_fee_)
	{
		//資金不足
		g_Langメッセージ(_T("ニトリ製作所資金不足メッセージ"),std::map<tstring, StyleString >());
		sg_pDungeonSystem->メニューを閉じる();
		return true;
	}

	vector<pcDroping>& Objects = sg_pDungeonSystem->pPlayerChara()->holdItem;
	
	pcControlLayer pccl;
	pcListWindowAdv pcladw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	pccl->WindowList.push_back(pcladw = pcListWindowAdv(new cListWindowAdv));

	static vector<pcDroping> エスケープリスト;

	pcDroping pdrop;
	if(Objects.size() != 0)
	{//なにかある
		pcladw->vpDroplist.resize(pcladw->vpDroplist.size() + 1);
		int i,size = Objects.size();
		for(i=0;i<size;i++)
		{
			pcCommand pcommand = pcCommand(new cCommand_NitoriFactory_combine_item1(combine_fee_, Objects[i], Objects[i]->FullName()));
			pcommand->delegate_ = selfAsDelegate();
			pcladw->vpDroplist.back().push_back( pcObjectiveDroping(new cObjectiveDroping( Objects[i],
				pcommand)));
		}
	}
	if(pcladw->vpDroplist.empty())
	{//なにもない
		エスケープリスト.clear();
		エスケープリスト.push_back(pcDroping(new cDropingNothing(g_Lang(_T("ニトリ合成可能アイテム無し")).conclete_tstr())));
		pcladw->pvpDroplist_forSort.push_back(&エスケープリスト);
		pcladw->vpDroplist.push_back(エスケープリスト);
	}
	pcladw->pvpDroplistName.push_back(g_Lang(_T("合成アイテム１")).conclete_tstr());


	pcladw->Init(pDev, 20, 10);
	pcladw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(2));
	pcladw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(3));
	pcladw->MultiSelect = FALSE;

	return true;
}
int cCommand_NitoriFactory_combine_item1::Action(IDirect3DDevice9 *pDev)
{
	//----------------------------------
	//合成
	//----------------------------------

	vector<pcDroping>& Objects = sg_pDungeonSystem->pPlayerChara()->holdItem;
	
	pcControlLayer pccl;
	pcListWindowAdv pcladw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	pccl->WindowList.push_back(pcladw = pcListWindowAdv(new cListWindowAdv));

	static vector<pcDroping> エスケープリスト;

	pcDroping pdrop;
	if(Objects.size() != 0)
	{//なにかある
		pcladw->vpDroplist.resize(pcladw->vpDroplist.size() + 1);
		int i,size = Objects.size();
		for(i=0;i<size;i++)
		{
			if(item1_ != Objects[i]) {
				pcCommand pcommand = pcCommand(new cCommand_NitoriFactory_combine_item1_item2(combine_fee_, item1_, Objects[i], Objects[i]->FullName()));
				pcommand->delegate_ = selfAsDelegate();
				pcladw->vpDroplist.back().push_back( pcObjectiveDroping(new cObjectiveDroping( Objects[i],
					pcommand)));
			}
		}
	}
	if(pcladw->vpDroplist.empty())
	{//なにもない
		エスケープリスト.clear();
		エスケープリスト.push_back(pcDroping(new cDropingNothing(g_Lang(_T("ニトリ合成可能アイテム無し")).conclete_tstr())));
		pcladw->pvpDroplist_forSort.push_back(&エスケープリスト);
		pcladw->vpDroplist.push_back(エスケープリスト);
	}
	pcladw->pvpDroplistName.push_back(g_Lang(_T("合成アイテム２")).conclete_tstr());


	pcladw->Init(pDev, 20, 10);
	pcladw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(2));
	pcladw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(3));
	pcladw->MultiSelect = FALSE;

	return true;
}
int cCommand_NitoriFactory_combine_item1_item2::Action(IDirect3DDevice9 *pDev)
{

	vector<pcDroping> vpdrop;
	vpdrop.push_back(item1_);
	vpdrop.push_back(item2_);
	sg_pDungeonSystem->アイテム合成(vpdrop,1,1,0);

	if(vpdrop.size() >= 2 && vpdrop[1] && vpdrop[1]->broken)
	{
		sg_pDungeonSystem->主人公お金増減( - combine_fee_ );
		std::map<tstring, StyleString > val;
		if(!vpdrop.empty() && vpdrop[0]) {
			val[_T("Item")] = vpdrop[0]->FullName();
		}
		else {
			val[_T("Item")] = _T("？");
		}
		g_Langメッセージ(_T("ニトリ合成完了メッセージ"),val);

		pcCommandDelegateObject shared_delegate = delegate_.lock();
		if(shared_delegate) {
			shared_delegate->didEndCommand(*this);
		}
	}
	else
	{
		std::map<tstring, StyleString > val;
		g_Langメッセージ(_T("ニトリ合成失敗メッセージ"),val);
	}

	sg_pDungeonSystem->メニューを閉じる();
	return true;
}
int cCommand_NitoriFactory_remodeling::Action(IDirect3DDevice9 *pDev)
{
	//----------------------------------
	//改造
	//----------------------------------

	vector<pcDroping>& Objects = sg_pDungeonSystem->pPlayerChara()->holdItem;
	
	pcControlLayer pccl;
	pcListWindowAdv pcladw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	pccl->WindowList.push_back(pcladw = pcListWindowAdv(new cListWindowAdv));

	static vector<pcDroping> エスケープリスト;

	pcDroping pdrop;
	if(Objects.size() != 0)
	{//なにかある
		pcladw->vpDroplist.resize(pcladw->vpDroplist.size() + 1);
		int i,size = Objects.size();
		for(i=0;i<size;i++)
		{
			pcEquipment pequip = boost::dynamic_pointer_cast<cEquipment>(Objects[i]);
			if(pequip && pequip->改造可能())
			{
				pcladw->vpDroplist.back().push_back( pcObjectiveDroping(new cObjectiveDroping( Objects[i],
					pcCommand(new cCommand_NitoriFactory_remodeling_item(Objects[i], Objects[i]->FullName())))));
			}

			if(Objects[i]->内包落ち物対象可())
			{
				int k,size_include = Objects[i]->includedItem.size();
				
				for(k=0;k<size_include;k++)
				{
					pcEquipment pequip_include = boost::dynamic_pointer_cast<cEquipment>(Objects[i]->includedItem[k]);
					if(pequip_include && pequip_include->改造可能())
					{
						pcladw->vpDroplist.back().push_back( pcObjectiveDroping(new cObjectiveDroping( pequip_include,
							pcCommand(new cCommand_NitoriFactory_remodeling_item(pequip_include, pequip_include->FullName())))));
					}
				}
			}
		}
	}
	if(pcladw->vpDroplist.empty())
	{//なにもない
		エスケープリスト.clear();
		エスケープリスト.push_back(pcDroping(new cDropingNothing(g_Lang(_T("ニトリ改造可能アイテム無し")).conclete_tstr())));
		pcladw->pvpDroplist_forSort.push_back(&エスケープリスト);
		pcladw->vpDroplist.push_back(エスケープリスト);
	}
	pcladw->pvpDroplistName.push_back(g_Lang(_T("改造できる装備品")).conclete_tstr());


	pcladw->Init(pDev, 20, 10);
	pcladw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(2));
	pcladw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(3));
	pcladw->MultiSelect = FALSE;

	return true;
}
int cCommand_NitoriFactory_remodeling_item::Action(IDirect3DDevice9 *pDev)
{
	if(sg_pDungeonSystem->主人公お金所持量() < cOtherChara_ID_5::改造資金(item_))
	{
		//資金不足
		if(cOtherChara_ID_5::改造資金(item_) > 20000)
		{
			g_Langメッセージ(_T("ニトリ製作所高額資金不足メッセージ"),std::map<tstring, StyleString >());
		}
		else
		{
			g_Langメッセージ(_T("ニトリ製作所資金不足メッセージ"),std::map<tstring, StyleString >());
		}
		sg_pDungeonSystem->メニューを閉じる();
		return true;
	}

	sg_pDungeonSystem->主人公お金増減( - cOtherChara_ID_5::改造資金(item_));

	sg_pDungeonSystem->メニューを閉じる();
	pcEquipment pequip = boost::dynamic_pointer_cast<cEquipment>(item_);
	if(pequip)
	{
		pequip->改造する();
		std::map<tstring, StyleString > val;
		val[_T("Item")] = item_->FullName();
		g_Langメッセージ(_T("ニトリ改造完了メッセージ"),val);
		cOtherChara_ID_5::ShopUsedSave();
	}

	return true;
}
int cOtherChara_ID_5::TalkEvent()
{
	cOtherChara::TalkEvent();
	pcControlLayer pccl;
	pcSelectWindow pcsw;

	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));

	pccl->Init(sg_pDungeonSystem->pDevice_D3D);
	pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

	pcsw->commandList.push_back(pcCommand(new cCommand_NitoriFactory(g_Lang(_T("ニトリ製作所"))) ));
	
	int strsize = 0;
	int i;
	for(i=0;i<pcsw->commandList.size();i++)
	{
		strsize = max(strsize,pcsw->commandList[i]->caption.length());
	}
	strsize = max(strsize,3);
	strsize = min(strsize,20);

	pcsw->Init(sg_pDungeonSystem->pDevice_D3D, strsize, pcsw->commandList.size());
	pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(1));
	pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(1));


	pcsw->playsound_decide();

	return true;
}
int cOtherChara_ID_5::強化資金()
{
	return 100;
}
int cOtherChara_ID_5::合成資金()
{
	return 200;
}
int cOtherChara_ID_5::改造資金(pcDroping pdrop)
{
	pcEquipment pequip = boost::dynamic_pointer_cast<cEquipment>(pdrop);
	if(pequip)
	{
		return pequip->改造費();
	}
	return 999999999;
}
int cOtherChara_ID_5::御霊削除資金()
{
	return 500;
}
void cOtherChara_ID_5::ShopUsedSave()
{
	mapUtility::setMapValue(sg_pDungeonSystem->pSaveData->globalFlags, cOtherChara_ID_5::lastUseTimeLavelKey(), sg_pDungeonSystem->pSaveData->playerLevel);
	mapUtility::setMapValue(sg_pDungeonSystem->pSaveData->globalFlags, cOtherChara_ID_5::lastUseTimeExpKey(), sg_pDungeonSystem->pSaveData->playerExp);

}
bool cOtherChara_ID_5::isCanUseShop()
{
#ifdef _UNRELEASE
	return true;
#endif
	return (mapUtility::getMapValue(sg_pDungeonSystem->pSaveData->globalFlags, cOtherChara_ID_5::lastUseTimeLavelKey(), 0) != sg_pDungeonSystem->pSaveData->playerLevel
		|| mapUtility::getMapValue(sg_pDungeonSystem->pSaveData->globalFlags, cOtherChara_ID_5::lastUseTimeExpKey(), 0) < sg_pDungeonSystem->pSaveData->playerExp - 1000);
}

//--------------------------------------------------------------------
//あきゅう
//--------------------------------------------------------------------
DEF_COMMANDCLASS(_Hizumi_Dungeon, g_Lang(_T("ひずみのダンジョン")))
	virtual StyleString shortExplanationText(){
		return g_Lang(_T("ひずみのダンジョンshortExplanationText"));
	};
};
int cCommand_Hizumi_Dungeon::Action(IDirect3DDevice9 *pDev)
{
	vector<tstring> dungeons;
	dungeons.push_back(_T("red_extra_Dungeon"));
	dungeons.push_back(_T("blue_extra_Dungeon"));
	sg_pDungeonSystem->SelectDungeon(dungeons);
	return true;
}
/*
int cOtherChara_ID_8::TalkEvent()
{
	cOtherChara::TalkEvent();
	pcControlLayer pccl;
	pcSelectWindow pcsw;

	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));

	pccl->Init(sg_pDungeonSystem->pDevice_D3D);
	pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

	pcsw->commandList.push_back(pcCommand(new cCommand_NitoriFactory(g_Lang(_T("ひずみのダンジョン"))) ));
	
	int strsize = 0;
	int i;
	for(i=0;i<pcsw->commandList.size();i++)
	{
		strsize = max(strsize,pcsw->commandList[i]->caption.length());
	}
	strsize = max(strsize,3);
	strsize = min(strsize,20);

	pcsw->Init(sg_pDungeonSystem->pDevice_D3D, strsize, pcsw->commandList.size());
	pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(1));
	pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(1));


	pcsw->playsound_decide();

	return true;
}
*/