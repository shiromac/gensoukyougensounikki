#include "stdafx.h"
#include "ceaiShopOwner.h"
#include "cSaveStore.h"
#include "cStringWindow.h"
#include "cListWindowAdv.h"
#include "cDropingFunc.h"
#include "cCommandDroping.h"
#define MEM_TGTLANDX 0
#define MEM_TGTLANDY 1
#define MEM_MOVEMODE 2




ceaiShopOwner::ceaiShopOwner(void)
{

}

ceaiShopOwner::~ceaiShopOwner(void)
{

}

int ceaiShopOwner::Init(pcCharacter self)
{
	cEnemyAI::Init(self);

	mode = MODE_NOTHING;

	return true;
}

int ceaiShopOwner::Request(int phase)
{

	if(sg_pDungeonSystem->泥棒状態())
	{//泥棒状態
		return SimpleActiveRequest(phase);
	}

	if(me()->足元地形()->RoomIndex == -1 || !(sg_pDungeonSystem->Map().RoomList[me()->足元地形()->RoomIndex]->ShopFlag))
	{//ここ店じゃないよ！
		return SimpleActiveRequest(phase);
	}

	if(mode == MODE_NOTHING)
	{
		u_目標点作成(tgtLandX,tgtLandY);
		mode = MODE_WALK;
	}
	else
	{
		u_目標点作成(tgtLandX,tgtLandY);
		mode = MODE_WALK;
	}

	{
		int i,size;
		int flag = 0;
		do
		{
			vector<pcDroping> dropoutputlist;
			sg_pDungeonSystem->キャラクター目視可範囲落ち物リスト(me(), dropoutputlist);
			size = dropoutputlist.size();
			for(i=0;i<size;i++)
			{
				if(dropoutputlist[i]->属性.count(落ち物属性::階段))
				{
					tgtLandX = dropoutputlist[i]->placeX;
					tgtLandY = dropoutputlist[i]->placeY;
					if(sg_pDungeonSystem->Map().Land(tgtLandX,tgtLandY)->RoomIndex != -1 &&
						sg_pDungeonSystem->Map().RoomList[sg_pDungeonSystem->Map().Land(tgtLandX,tgtLandY)->RoomIndex]->ShopFlag)
					{
						mode = MODE_WALK;
						flag = 1;break;
					}
				}
				else if(dropoutputlist[i]->ID() == 9015)//スキマ
				{
					tgtLandX = dropoutputlist[i]->placeX;
					tgtLandY = dropoutputlist[i]->placeY;
					if(sg_pDungeonSystem->Map().Land(tgtLandX,tgtLandY)->RoomIndex != -1 &&
						sg_pDungeonSystem->Map().RoomList[sg_pDungeonSystem->Map().Land(tgtLandX,tgtLandY)->RoomIndex]->ShopFlag)
					{
						mode = MODE_WALK;
						flag = 1;break;
					}
				}
			}
			if(flag) break;

			if(u_敵サーチ_キャラ優先度優先(tgtLandX,tgtLandY))
			{//成功
				if(sg_pDungeonSystem->Map().Land(tgtLandX,tgtLandY)->RoomIndex != -1 &&
					sg_pDungeonSystem->Map().RoomList[sg_pDungeonSystem->Map().Land(tgtLandX,tgtLandY)->RoomIndex]->ShopFlag)
				{
					mode = MODE_WALK;
					break;
				}
			}

			vector<pcCharacter> outputlist;
			sg_pDungeonSystem->キャラクター目視可範囲キャラリスト(me(), outputlist);
			size = outputlist.size();
			for(i=0;i<size;i++)
			{
				if(outputlist[i] == sg_pDungeonSystem->pPlayerChara())
				{
					tgtLandX = outputlist[i]->placeX;
					tgtLandY = outputlist[i]->placeY;
					if(sg_pDungeonSystem->Map().Land(tgtLandX,tgtLandY)->RoomIndex != -1 &&
						sg_pDungeonSystem->Map().RoomList[sg_pDungeonSystem->Map().Land(tgtLandX,tgtLandY)->RoomIndex]->ShopFlag)
					{
						mode = MODE_WALK;
						flag = 1;break;
					}
				}
			}
			if(flag) break;

			cValiableField val;
			pcLandform pland;
			if((pland = me()->AIお気に入り地形(val)) != NULL)
			{//お気に入り地形発見
				tgtLandX = pland->placeX;
				tgtLandY = pland->placeY;

				if(sg_pDungeonSystem->Map().Land(tgtLandX,tgtLandY)->RoomIndex != -1 &&
					sg_pDungeonSystem->Map().RoomList[sg_pDungeonSystem->Map().Land(tgtLandX,tgtLandY)->RoomIndex]->ShopFlag)
				{
					mode = MODE_WALK;
					break;
				}
			}
		}while(0);
	}


	//cMap* pmap = &sg_pDungeonSystem->Map();
	//if(mode == MODE_ATTACK)//追尾モード
	{
		cValiableField val;
		val.doubles.dim(変数_フェイズ) = phase;
		val.lands.dim(変数_攻撃場所) = sg_pDungeonSystem->Map().Land(tgtLandX,tgtLandY);
		val.doubles.dim(変数_汎用実数) = 0.0;
		if( me()->AI特殊攻撃選択pre(val) )
		{
			flagSpecialAttack = 1;
			valueSpecialAttack = val.doubles.val(変数_汎用実数);
			return AI_REQUEST_ATTACK;
		}
		else
		{
			valueSpecialAttack = val.doubles.val(変数_汎用実数);
			flagSpecialAttack = 0;
		}
	}

	if(u_敵隣接判定())
	{
		return AI_REQUEST_ATTACK;
	}


	return AI_REQUEST_MOVE;
}

int ceaiShopOwner::Move()
{
	if(sg_pDungeonSystem->泥棒状態())
	{//泥棒状態
		return SimpleActiveMove();
	}
	if(me()->足元地形()->RoomIndex == -1 || !(sg_pDungeonSystem->Map().RoomList[me()->足元地形()->RoomIndex]->ShopFlag))
	{//ここ店じゃないよ！
		return SimpleActiveMove();
	}

	if(u_敵隣接判定())
	{
		return false;
	}


	//cMap* pmap = &sg_pDungeonSystem->Map();
	multimap<int,int> sortMap;
	cCoordinate coo,mep,aspectcoo;
	int aspect;
	mep.x = me()->placeX;
	mep.y = me()->placeY;
	coo.x = tgtLandX;
	coo.y = tgtLandY;

	aspectcoo.x = ((coo-mep).x)?(coo-mep).x/abs((coo-mep).x):0;
	aspectcoo.y = ((coo-mep).y)?(coo-mep).y/abs((coo-mep).y):0;

	if(aspectcoo.dif() == 0) return false;
	aspect = aspectcoo.GetAspect();

	vector<pcLandform> vpedgeland;
	pcLandform pland = sg_pDungeonSystem->Map().Land(mep);
	if(pland->RoomIndex != -1)
	{
		vpedgeland = sg_pDungeonSystem->Map().RoomList[pland->RoomIndex]->LandformEdgeList;
	}

	int nomove = 0;
	nomove += (coo-mep).dif() * 128*128;

	int i;
	cCoordinate relative;

	for(i=0;i<8;i++)
	{
		aspectcoo.SetAspect(i);
		relative = coo-(mep+aspectcoo);
		int value_minus = 0;
		if(relative.dif() > (coo-mep).dif())continue;
		if(!u_隣接移動通用判定(i))continue;
		value_minus += relative.dif() * 128*128;
		value_minus += (abs(relative.x)+abs(relative.y)) * 128;
		value_minus += aspect_AngleAspect(me()->aspect,i);
		pland = sg_pDungeonSystem->Map().Land(mep+aspectcoo);	
		value_minus += (pland->pOnDrop != NULL) * 128;
		vector<pcLandform>::iterator itr = find(vpedgeland.begin(), vpedgeland.end(),pland);
		value_minus += (itr != vpedgeland.end()) * 128*128*128;
		if(sg_pDungeonSystem->Map().Land(mep+aspectcoo)->RoomIndex == -1) value_minus += 128*128*128*16;
		else if(relative.dif() == 0) value_minus = 0;
		sortMap.insert(pair<int,int>(value_minus,i));
	}

	multimap<int,int>::iterator itr = sortMap.begin();
	if(!sortMap.empty())
	{
		if(nomove < itr->first)
		{
			return true;
		}
		for(;itr != sortMap.end();itr++)
		{
			if(sg_pDungeonSystem->隣接地形移動可(me(),itr->second))
			{
				sg_pDungeonSystem->移動要請(me(),itr->second);
				return true;
			}
		}
	}
	return false;

}

int ceaiShopOwner::Attack()
{
	if(sg_pDungeonSystem->泥棒状態())
	{//泥棒状態
		return SimpleActiveAttack();
	}
	if(me()->足元地形()->RoomIndex == -1 || !(sg_pDungeonSystem->Map().RoomList[me()->足元地形()->RoomIndex]->ShopFlag))
	{//ここ店じゃないよ！
		return SimpleActiveAttack();
	}


	return SimpleActiveAttack();

}

//商品を買う
int cCommand_ShopBuy::Action(cRenderDevice *pDev)
{
	int money = sg_pDungeonSystem->店請求金額(powner_);
	map<tstring, StyleString> val;
	if(sg_pDungeonSystem->GameScreenInterface.money.value + sg_pDungeonSystem->店ツケ可能金額() - sg_pDungeonSystem->pSaveQuest->ShopDebt - money >= 0)
	{//つければ足りる
		sg_pDungeonSystem->店アイテム購入清算(powner_);
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
StyleString cCommand_ShopBuy::shortExplanationText()
{
	StyleString sstr;
	map<tstring, StyleString> val;
	int money = sg_pDungeonSystem->店請求金額(powner_);
	cMoney moneystr;
	moneystr.value = money;
	val[_T("Value")] = moneystr.caption();

	int atobarai = max(0,-sg_pDungeonSystem->pSaveQuest->ShopDebt);
	if(sg_pDungeonSystem->GameScreenInterface.money.value - money + atobarai >= 0)
	{//足りる
		sstr += g_Lang(_T("店買うshortExplanation"),val);
	}
	else if(sg_pDungeonSystem->GameScreenInterface.money.value + sg_pDungeonSystem->店ツケ可能金額() - sg_pDungeonSystem->pSaveQuest->ShopDebt - money >= 0)
	{//つければ足りる
		sstr += g_Lang(_T("店買う不足許可shortExplanation"),val);
	}
	else
	{//つけても足りない
		sstr += g_Lang(_T("店買うshortExplanation"),val);
	}

	if(sg_pDungeonSystem->pSaveQuest->ShopDebt > 0)
	{//つけあり
		int tukepey = min(sg_pDungeonSystem->GameScreenInterface.money.value - money ,sg_pDungeonSystem->pSaveQuest->ShopDebt);
		if(tukepey > 0)
		{//ツケが払える
			moneystr.value = tukepey;
			val[_T("Value")] = moneystr.caption();
			sstr += g_Lang(_T("店買うツケshortExplanation"),val);
		}
	}
	else if(sg_pDungeonSystem->pSaveQuest->ShopDebt < 0)
	{//後払いあり
		int tukepey = min(money, -sg_pDungeonSystem->pSaveQuest->ShopDebt);
		if(tukepey > 0)
		{//後払いが払える
			moneystr.value = tukepey;
			val[_T("Value")] = moneystr.caption();
			sstr += g_Lang(_T("店買う後払いshortExplanation"),val);
		}
	}
	return sstr;
}

//床のアイテムを売る
int cCommand_ShopSell::Action(cRenderDevice *pDev)
{
	map<tstring, StyleString> val;
	sg_pDungeonSystem->店アイテム売却清算(powner_);
	g_Langメッセージ(_T("店売る完了メッセージ"),val);
	sg_pDungeonSystem->メニューを閉じる();

	return true;
}
StyleString cCommand_ShopSell::shortExplanationText()
{
	StyleString sstr;
	map<tstring, StyleString> val;
	int money = sg_pDungeonSystem->店支払い金額(powner_);
	cMoney moneystr;
	moneystr.value = money;
	val[_T("Value")] = moneystr.caption();

	if(sg_pDungeonSystem->店主持ち金() - money >= 0)
	{//足りる
		sstr += g_Lang(_T("店売るshortExplanation"),val);
	}
	else
	{//足りない
		int after = money - sg_pDungeonSystem->店主持ち金();
		moneystr.value = after;
		val[_T("Value2")] = moneystr.caption();
		sstr += g_Lang(_T("店売る不足許可shortExplanation"),val);
	}


	if(sg_pDungeonSystem->pSaveQuest->ShopDebt > 0)
	{//つけあり
		int tukepey = min(money ,sg_pDungeonSystem->pSaveQuest->ShopDebt);
		if(tukepey > 0)
		{//ツケが払える
			moneystr.value = tukepey;
			val[_T("Value")] = moneystr.caption();
			sstr += g_Lang(_T("店売るツケshortExplanation"),val);
		}
	}
	return sstr;
}

//持ち物を売る
int cCommand_ShopSellHold::Action(cRenderDevice *pDev)
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

		int i,size = Objects.size();
		for(i=0;i<size;i++)
		{
			captions.clear();
			captions.push_back(_T("店売る"));
			if(!Objects[i]->大局識別済み()) captions.push_back(_T("名前"));
			captions.push_back(_T("説明"));
			pcShopDroping psd = pcShopDroping(new cShopDroping(Objects[i],captions));
			psd->Init(pDev,psd);
			psd->sellchara_ = powner_;
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
	pcladw->multiverbs.push_back(_T("まとめて店売る"));

	return true;
}
StyleString cCommand_ShopSellHold::shortExplanationText()
{
	StyleString sstr;
	return sstr;
}

//持ち物を買う
int cCommand_ShopBuyHold::Action(cRenderDevice *pDev)
{
	//----------------------------------
	//買う
	//----------------------------------

	vector<pcDroping>& Objects = powner_->holdItem;
	
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

		int i,size = Objects.size();
		for(i=0;i<size;i++)
		{
			captions.clear();
			captions.push_back(_T("店買う"));
			if(!Objects[i]->大局識別済み()) captions.push_back(_T("名前"));
			captions.push_back(_T("説明"));
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
	pcladw->pvpDroplistName.push_back(g_Lang(_T("店主の持ち物")).conclete_tstr());


	pcladw->Init(pDev, 20, 10);
	pcladw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(2));
	pcladw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(3));
	pcladw->MultiSelect = TRUE;
	pcladw->multiverbs.push_back(_T("まとめて店買う"));

	return true;
}
StyleString cCommand_ShopBuyHold::shortExplanationText()
{
	StyleString sstr;
	return sstr;
}

//後払い金を請求する
int cCommand_ShopChargeMoney::Action(cRenderDevice *pDev)
{
	map<tstring, StyleString> val;
	if(sg_pDungeonSystem->店主持ち金() >= 0)
	{//つければ足りる
		sg_pDungeonSystem->店後払い支払い();
		sg_pDungeonSystem->メニューを閉じる();
	}
	else
	{//つけても足りない
		sg_pDungeonSystem->メニューを閉じる();
	}
	return true;
}
StyleString cCommand_ShopChargeMoney::shortExplanationText()
{
	StyleString sstr;
	map<tstring, StyleString> val;
	if(sg_pDungeonSystem->pSaveQuest->ShopDebt < 0)
	{//後払いあり
		int tukepey = min(sg_pDungeonSystem->店主持ち金(), -sg_pDungeonSystem->pSaveQuest->ShopDebt);
		cMoney moneystr;
		moneystr.value = tukepey;
		val[_T("Value")] = moneystr.caption();
		if(tukepey > 0)
		{//ツケが払える
			sstr += g_Lang(_T("店後払い請求shortExplanation"),val);
		}
		else
		{
			sstr += g_Lang(_T("店後払い請求不可shortExplanation"),val);
		}
	}
	return sstr;
}
//店を出る
int cCommand_ShopExit::Action(cRenderDevice *pDev)
{
	sg_pDungeonSystem->移動要請(sg_pDungeonSystem->pPlayerChara(),sg_pDungeonSystem->pPlayerChara()->aspect);
	sg_pDungeonSystem->ターンエンド();
	sg_pDungeonSystem->メニューを閉じる();
	return true;
}
StyleString cCommand_ShopExit::shortExplanationText()
{
	return g_Lang(_T("店退出確認shortExplanation"));
}