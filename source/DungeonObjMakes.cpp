#include "stdafx.h"

#include "cDungeonSystem.h"
#include "cFloor.h"
#include "csDungeonFirst.h"

#include "utility/debug.h"
#include "cSaveStore.h"
#include "cResultWindow.h"
#include "Gameobjects.h"
#include <time.h>
#include "ceaiShopOwner.h"
#include "FindUtility.h"

/*pcCharacter cDungeonSystem::キャラクター生成(tstring name,int CLASS,int Forse,pcLandform land)
{
	pcCharacter pchara = DataBase.GetSampleCharacter(name);
	if(pchara == NULLCHARA) return NULLCHARA;
	return キャラクター生成(pchara->ID(), CLASS, Forse, land);
}
*/
pcCharacter cDungeonSystem::キャラクター生成(int ID,int CLASS,int Forse,pcLandform land, const CreateCharacterOptions& options)
{

	if(land == NULLLAND)
	{
		land = RandomPlace();
	}
	if(land == NULLLAND)
	{//置くところがひとつもない
		OnAssert(_T(__FILE__),__LINE__,false,_T("キャラクターの配置に失敗しました。キャラクターを配置可能な場所が存在しません。設定を確認してください。<error> A character cannot put on land."));
	}


	pcCharacter pcmob;


	pcmob = DataBase.GetNewCharacter(ID);
	if(pcmob == NULLCHARA) return NULLCHARA;
	pcmob->Forse = Forse;
	pcmob->CLASS = CLASS;

	if(options & CreateCharacterOptionsOverDrived) {
		pcmob->onOverDrive();
	}



	if(!初期設置(pcmob,land))
	{//失敗
		return NULLCHARA;
	}
	else
	{	
		pcmob->Init(pDevice_D3D,pcmob);
		pcmob->settingInit();

		AnimationManager().Anime_DisplayChange(&(pcmob->VisibleCLASS),CLASS);

		double opeque = pcmob->opaque;
		pcmob->opaque = 0;
		AnimationManager().Anime_DisplayChange(
				&pcmob->opaque,
				opeque);

		return pcmob;
	}
}

pcCharacter cDungeonSystem::主人公交代(int ID)
{
	pcCharacter beforeChara = sg_pDungeonSystem->pPlayerChara();
	pcLandform beforeLand = beforeChara->足元地形();
	int level = beforeChara->LV;
	unlinkcharacter(beforeChara);
	sg_pDungeonSystem->pPlayerChara() = キャラクター生成(ID, 0, CHARACTER_FORSE_FRIEND, beforeLand);
	sg_pDungeonSystem->pPlayerChara()->LV = level;

	vector<pcDroping> beforeCharaHoldItem = beforeChara->holdItem;
	int i,size = beforeCharaHoldItem.size();
	for(i=0;i<size;i++) {
		泥棒要請(pPlayerChara(),beforeCharaHoldItem[i],false,true);
	}
	removecharacter(beforeChara);
	ターンエンド();
	sg_pDungeonSystem->GameScreenInterface.Init(pDevice_D3D, sg_pDungeonSystem->pPlayerChara());
	//sg_pDungeonSystem->GameScreenInterface.Resetdisp(pDevice_D3D);
	return sg_pDungeonSystem->pPlayerChara();
}

bool cDungeonSystem::主人公交代(pcCharacter pchara)
{
	pcCharacter beforeChara = sg_pDungeonSystem->pPlayerChara();
	sg_pDungeonSystem->pPlayerChara() = pchara;

	vector<pcDroping> beforeCharaHoldItem = beforeChara->holdItem;
	int i,size = beforeCharaHoldItem.size();
	for(i=0;i<size;i++) {
		泥棒要請(pPlayerChara(),beforeCharaHoldItem[i],false,true);
	}
	ターンエンド();
	sg_pDungeonSystem->GameScreenInterface.Init(pDevice_D3D, sg_pDungeonSystem->pPlayerChara());
	//sg_pDungeonSystem->GameScreenInterface.Resetdisp(pDevice_D3D);
	return true;
}
//居眠り付き
pcCharacter cDungeonSystem::キャラクター生成_自然湧き(int ID,int CLASS,int Forse,pcLandform land)
{
	CreateCharacterOptions option = CreateCharacterOptionsNoOption;
	if( pFloor()->overdriveMaxAppearEnemyNum() > オーバードライブ敵の数()
		&& pFloor()->overdriveEnemyPercent() > random()*100) {
		option = CreateCharacterOptionsOverDrived;
	}

	pcCharacter pchara = キャラクター生成(ID, CLASS, Forse, land, option);

	if(pchara)
	{
		pchara->naturalSpawnInit();
	}

	return pchara;
}



pcDroping cDungeonSystem::落ち物生成(tstring name,const int dpack_int[],pcLandform land, int firstsetting)
{

	return 落ち物生成(DataBase.GetSampleDroping(name)->ID(),
		dpack_int,
		land,
		firstsetting);
}
pcDroping cDungeonSystem::落ち物生成(int ID,const int dpack_int[],pcLandform land, int firstsetting)
{
	if(land == NULLLAND)
	{
		land = RandomPlace();
	}
	if(land == NULLLAND)
	{//置くところがひとつもない
		OnAssert(_T(__FILE__),__LINE__,false,_T("アイテムの配置に失敗しました。アイテムを配置可能な場所が存在しません。設定を確認してください。<error> A item cannot put on land."));
	}


	pcDroping pdrop = 落ち物生成_設置なし( ID, dpack_int, firstsetting);
	if(pdrop == NULL) return NULLDROP;
	/*
	pcDroping pdrop;
	pdrop = DataBase.GetNewDroping(ID);
	if(pdrop == NULL) return NULLDROP;
	pdrop->Init(pDevice_D3D,pdrop);

	//難易度別未実装
	pdrop->DataBeginOptimize(0);

	if(dpack_int != NULL)
	{
		int i;
		for(i=0;i<DEF_DROP_DATAPACK_NUM;i++)
		{
			pdrop->dpack.integer[i] = dpack_int[i];
		}
	}

	*/

	if(pdrop->大切())
	{
		if(!初期設置_大切(pdrop,land))
		{//失敗
			return NULLDROP;
		}
		else
		{
			pdrop->settingInit();

			int pri_onland_DrawFlag = pdrop->pri_onland_DrawFlag;
			pdrop->pri_onland_DrawFlag = 0;
			AnimationManager().Anime_DisplayChange(
				&pdrop->pri_onland_DrawFlag,
				pri_onland_DrawFlag);

			return pdrop;
		}
	}
	else
	{
		if(!初期設置(pdrop,land,true,firstsetting))
		{//失敗
			return NULLDROP;
		}
		else
		{
			pdrop->settingInit();

			int pri_onland_DrawFlag = pdrop->pri_onland_DrawFlag;
			pdrop->pri_onland_DrawFlag = 0;
			AnimationManager().Anime_DisplayChange(
				&pdrop->pri_onland_DrawFlag,
				pri_onland_DrawFlag);

			return pdrop;
		}
	}
}


pcDroping cDungeonSystem::落ち物生成_設置なし(tstring name,const int dpack_int[], int firstsetting)
{
	return 落ち物生成_設置なし(DataBase.GetSampleDroping(name)->ID(),
		dpack_int,
		firstsetting);
}

pcDroping cDungeonSystem::落ち物生成_設置なし(int ID,const int dpack_int[], int firstsetting)
{
	pcDroping pdrop;
	
	pdrop = DataBase.GetNewDroping(ID);
	if(pdrop == NULL) return NULLDROP;
	pdrop->Init(pDevice_D3D,pdrop);

	//難易度別未実装
	pdrop->DataBeginOptimize(0);

	if(拠点フラグ())
	{
		識別(pdrop,false);
	}

	if(dpack_int != NULL)
	{
		int i;
		for(i=0;i<DEF_DROP_DATAPACK_NUM;i++)
		{
			pdrop->dpack.integer[i] = dpack_int[i];
		}
	}

	//ドロップリストにセット。
	pushbackDropList(pdrop);

	return pdrop;

}
void cDungeonSystem::宴会生成(int roomindex, int partyFlag)
{
	if(partyFlag == 1)
	{//通常宴会
		if(roomindex < 0 || roomindex >= Map().RoomList.size()) return;
		pcRoom proom = Map().RoomList[roomindex];

		if(proom->LandformList.empty()) return;

		proom->PartyFlag = partyFlag;
	
		
		pcParty pparty = pDungeon()->pfloor(FloorLevel())->getParty();
		if(pparty != NULL)
		{
			int landnum = proom->LandformList.size();
			int trapnum = pparty->trapfirstsetnum(landnum,FloorLevel());
			int enemynum = pparty->enemyfirstsetnum(landnum,FloorLevel());
			int itemnum = pparty->itemfirstsetnum(landnum,FloorLevel());

			int i;

			//罠初期設置ランダム
			for(i=0;i<trapnum;i++)
			{
				int vint[3];
				unsigned int trapIDandEtc = pparty->trapDP().get(random());

				vint[0] = trapIDandEtc>>16;
				vint[1] = (trapIDandEtc>>8)&0xFF;
				vint[2] = (trapIDandEtc)&0xFF;

				落ち物生成(vint[0],NULL,proom->LandformList[proom->LandformList.size()*random()],true);

			}

			//アイテム初期設置ランダム
			for(i=0;i<itemnum;i++)
			{
				int vint[3];
				cDropingDistribution iDP = pparty->dropitemDP();

				vint[0] = iDP.get(random());

				落ち物生成(vint[0],NULL,proom->LandformList[proom->LandformList.size()*random()],true);
			}

			//敵初期設置ランダム
			for(i=0;i<enemynum;i++)
			{
				int vint[3];
				unsigned int enemyIDandPower = pparty->enemyDP().get(random());

				vint[0] = enemyIDandPower>>16;
				vint[1] = (enemyIDandPower>>8)&0xFF;
				vint[2] = (enemyIDandPower)&0xFF;

				pcCharacter pcmob = キャラクター生成(vint[0], vint[1], CHARACTER_FORSE_ENEMY, proom->LandformList[proom->LandformList.size()*random()]);
				if(pcmob != NULL)
				{
					pcmob->Condition.深い居眠り追加();
				}
			}
		}
	
	}


}


void cDungeonSystem::宴会発覚(int roomindex)
{
	if(roomindex < 0 || Map().RoomList.size() <= roomindex) return;

	if(Map().RoomList[roomindex]->PartyFlag == 0) return;
	
	vector<pcLandform> landlist = Map().RoomList[roomindex]->allLandformList();
	
	if(Map().RoomList[roomindex]->PartyFlag == 1)
	{//通常宴会
		int size = landlist.size();
		int ememychara = 0;
		int i;
		for(i=0;i<size;i++)
		{
			if(landlist[i]->pOnChar != NULL)
			{
				landlist[i]->pOnChar->Condition.とても強い刺激();
				//sg_pDungeonSystem->強制ターンエンド要請(landlist[i]->pOnChar);
				ememychara++;
			}
		}

		ememychara--;//自分の分を引く
		map<tstring,StyleString> valiable;
		if(ememychara > 25)
		{
			g_Langメッセージ(_T("通常宴会大規模メッセージ"),valiable);
		}
		else if(ememychara < 1)
		{
			g_Langメッセージ(_T("通常宴会無規模メッセージ"),valiable);
		}
		else if(ememychara < 5)
		{
			g_Langメッセージ(_T("通常宴会小規模メッセージ"),valiable);
		}
		else
		{
			g_Langメッセージ(_T("通常宴会メッセージ"),valiable);
		}

		floorNowParty() = true;//宴会突入
		//ここでBGM変更
		PlayBGM();

		メッセージ決定待ち();
		メッセージ消去();
	
		Map().RoomList[roomindex]->PartyFlag = 0;
	}


}

//----------------------------------------------------------------------
//店
//----------------------------------------------------------------------
int cDungeonSystem::店入出判定(int roomindex)
{
	if(!泥棒状態() && floorshoplastroom() != roomindex)
	{
		if( (int)(Map().RoomList.size()) > floorshoplastroom())
		{
			if(floorshoplastroom() != -1 && Map().RoomList[floorshoplastroom()]->ShopFlag)
			{
				店出る(roomindex);
			}

			if(roomindex != -1 && Map().RoomList[roomindex]->ShopFlag)
			{
				店入る(roomindex);
			}
		}
		floorshoplastroom() = roomindex;
	}
	return true;
}
pcCharacter cDungeonSystem::店長(int roomindex)
{
	//if(roomindex == -1) return NULLCHARA;
	
	vector<pcCharacter> vpchara = CharaList();
	int i,size = vpchara.size();
	for(i=0;i<size;i++)
	{
		if(vpchara[i]->ID() == 2020)// && vpchara[i]->足元地形()->RoomIndex == roomindex)
			return vpchara[i];
	}
	return NULLCHARA;
}
int cDungeonSystem::店入る(int roomindex)
{
	map<tstring, StyleString> val;
	g_Langメッセージ(_T("入店メッセージ"),val);
	
	g_GameEnv.m_SoundManager.getplayBGMFromFile( _T("shop_intro.wav"),_T("shop_main.wav"));
	
	return true;
}
int cDungeonSystem::店出る(int roomindex)
{
	if(店請求金額(店長(roomindex)) > 0)
	{
		泥棒発覚();
	}
	else
	{
		map<tstring, StyleString> val;
		g_Langメッセージ(_T("退店メッセージ"),val);
		PlayBGM();
	}

	return true;
}

int cDungeonSystem::店退出確認()
{
	pcControlLayer pccl;
	pcSelectWindow pcsw;

	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));

	pccl->Init(sg_pDungeonSystem->pDevice_D3D);
	pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));


	pcsw->commandList.push_back(pcCommand(new cCommand_ShopExit(g_Lang(_T("店を出る"))) ));
	pcsw->commandList.push_back(pcCommand(new cCommandCancel(g_Lang(_T("やめる"))) ));

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
	

	return true;
}
int cDungeonSystem::店レベル(double fund)
{
	return max(sqrt(fund/1000),0);
}
int cDungeonSystem::店ツケ可能金額()
{
	return pSaveQuest->ShopFund * 0.05;
}
int cDungeonSystem::店主持ち金()
{
	return floorshopownermoney();
}
int cDungeonSystem::店アイテム数(int level)
{
	return min(25,7+2*level);
}
int cDungeonSystem::店売却アイテムリスト(vector<pcDroping>& output, pcCharacter pShopowner)
{

	vector<pcDroping> vpdrop;
	キャラクター目視可範囲落ち物リスト(pShopowner,vpdrop);
	int value=0;
	int i,size = vpdrop.size();
	int roomIndex;
	for(i=0;i<size;i++)
	{
		roomIndex = vpdrop[i]->現在地形()->RoomIndex;
		if(!(vpdrop[i]->broken)
			&& (vpdrop[i]->Holder() == NULL || vpdrop[i]->Holder() == pShopowner)
			&& !(vpdrop[i]->Holder() == NULL &&(roomIndex == -1 || !Map().RoomList[roomIndex]->ShopFlag))
			&& !(vpdrop[i]->shopitemflag())
			&& vpdrop[i]->売却可())
		{
			//壊れていない
			//誰かももってないまたは店主が持ってる
			//誰かももってないとき店の外でない
			//店売りでない
			//店売り可
			output.push_back(vpdrop[i]);
		}

	}
	

	return true;
}
int cDungeonSystem::店購入アイテムリスト(vector<pcDroping>& output, pcCharacter pShopowner)
{
	output = floorshopitemlist();
	/*
	int i,size = floorshopitemlist().size();
	for(i=0;i<size;i++)
	{
		int roomIndex = floorshopitemlist()[i]->現在地形()->RoomIndex;
		

		if(floorshopitemlist()[i]->broken)
		{//壊れている。
			if(!(floorshopitemlist()[i]->broken & cDroping::BREAKFLAG_LOSS))
			{//ロスでない
				output.push_back(floorshopitemlist()[i]);
			}
		}
		else if(floorshopitemlist()[i]->Parent() != NULL)
		{//どっかの中
			output.push_back(floorshopitemlist()[i]);
		}
		else if(floorshopitemlist()[i]->Holder() != NULL)
		{//誰かが持ってる
			if(floorshopitemlist()[i]->Holder() != pShopowner)
			{//店主でない
				output.push_back(floorshopitemlist()[i]);
			}
		}
		else if(roomIndex == -1 || !(Map().RoomList[roomIndex]->ShopFlag))
		{//店の外
			output.push_back(floorshopitemlist()[i]);
		}
	}
	return true;
	*/

	return true;
}

int cDungeonSystem::店購入総価値差額再計算(pcCharacter pShopowner)
{
	int value=0;
	vector<pcDroping> vpdrop;

	店購入アイテムリスト(vpdrop,pShopowner);//NULL OK
	int i,size = vpdrop.size();
	for(i=0;i<size;i++)
	{
		int temp = 店購入価値差額再計算(vpdrop[i],pShopowner);//NULL OK
		if(temp < 0)
			value += temp;
	}

	return value;
}
int cDungeonSystem::店売却総価値差額再計算(pcCharacter pShopowner)
{

	vector<pcDroping> vpdrop;
	店売却アイテムリスト(vpdrop, pShopowner);
	int value=0;
	int i,size = vpdrop.size();
	for(i=0;i<size;i++)
	{
		int temp = 店売却価値差額再計算(vpdrop[i],pShopowner);
		if(temp > 0)
			value += temp;
	}

	return value;
}
int cDungeonSystem::店購入価値差額再計算(pcDroping pdrop, pcCharacter pShopowner)
{
	int prevalue;
	int value=0;
	if(floorshopitemvalueBuy().count(pdrop))
	{
		prevalue = floorshopitemvalueBuy()[pdrop];
		value = 店購入価値再計算(pdrop, pShopowner) - prevalue;//NULL OK
	}
	else
	{
		value = 店購入価値再計算(pdrop, pShopowner);//NULL OK
	}
	return value;
}
int cDungeonSystem::店売却価値差額再計算(pcDroping pdrop, pcCharacter pShopowner)
{
	int prevalue;
	int value=0;
	if(floorshopitemvalueSell().count(pdrop))
	{
		prevalue = floorshopitemvalueSell()[pdrop];
		value = pdrop->単体売値() - prevalue;
	}
	else
	{
		value = 店売却価値再計算(pdrop, pShopowner);
	}
	return value;
}
	
int cDungeonSystem::店購入価値再計算(pcDroping pdrop, pcCharacter pShopowner)
{
	int roomIndex = pdrop->現在地形()->RoomIndex;
	

	if(pdrop->broken)
	{//壊れている。
		if(pdrop->broken & cDroping::BREAKFLAG_LOSS)
		{//ロス
			return pdrop->単体買値(拠点フラグ());
		}
		else
		{
			return 0;
		}
	}
	else if(pdrop->Parent() != NULL && pdrop->Parent()->内包アイテムは実体がない())
	{//どっかの中
		return 0;
	}
	else if(pdrop->Holder() != NULL)
	{//誰かが持ってる
		if(pdrop->Holder() == pShopowner)
		{//店主
			return pdrop->単体買値(拠点フラグ());
		}
		else
		{
			return 0;
		}
	}
	else if(roomIndex == -1 || !(Map().RoomList[roomIndex]->ShopFlag))
	{//店の外
		return 0;
	}
	else if(!pdrop->売却可())
	{// 店売り不可
		return 0;
	}
	else
	{
		return pdrop->単体買値(拠点フラグ());
	}
}
int cDungeonSystem::店売却価値再計算(pcDroping pdrop, pcCharacter pShopowner)
{
	int roomIndex = pdrop->現在地形()->RoomIndex;
	

	if(pdrop->broken)
	{//壊れている。
		return 0;
	}
	else if(pdrop->Parent() != NULL && pdrop->Parent()->内包アイテムは実体がない())
	{//どっかの中 で　ねだんないほう含まれる
		return 0;
	}
	else if(pdrop->Holder() != NULL && pdrop->Holder() != pShopowner)
	{//誰かが持ってる
		return 0;
	}
	else if(roomIndex == -1 || pdrop->shopitemflag())
	{//店の外 // 店売り
		return 0;
	}
	else if(!pdrop->売却可())
	{// 店売り不可
		return 0;
	}
	else
	{
		return pdrop->単体売値();
	}
}	
int cDungeonSystem::店アイテム価値再計算(pcDroping pdrop, pcCharacter pShopowner)
{
	floorshopitemvalueBuy()[pdrop] = pdrop->単体買値(拠点フラグ());
	floorshopitemvalueSell()[pdrop] = pdrop->単体売値();

	if(!pdrop->内包アイテムは実体がない())
	{
		int i;
		for(i=0;i<pdrop->includedItem.size();i++)
		{
			店アイテム価値再計算(pdrop->includedItem[i], pShopowner);
		}
	}
	return true;
}	
int cDungeonSystem::店請求金額(pcCharacter pShopowner)
{
	//if(pShopowner == NULL) return 0;ヌルＯＫ
	return -店購入総価値差額再計算(pShopowner);
}
int cDungeonSystem::店支払い金額(pcCharacter pShopowner)
{
	if(pShopowner == NULL) return 0;
	return 店売却総価値差額再計算(pShopowner);
}

int cDungeonSystem::店アイテム購入清算(pcCharacter pShopowner)
{
	if(pShopowner == NULL) return false;

	vector<pcDroping> vpdrop,buydroplist;
	店購入アイテムリスト(vpdrop,pShopowner);
	int value=0;
	int i,size = vpdrop.size();
	for(i=0;i<size;i++)
	{
		if(店購入価値差額再計算(vpdrop[i],pShopowner) < 0)
		{
			buydroplist.push_back(vpdrop[i]);
		}
	}
	size = buydroplist.size();
	for(i=0;i<size;i++)
	{
		店アイテム購入(buydroplist[i], pShopowner);
	}
	return true;
	/*
	vector<pcDroping> vpdrop;
	int value=0;
	int i,size = floorshopitemlist().size();
	for(i=0;i<size;i++)
	{
		int roomIndex = floorshopitemlist()[i]->現在地形()->RoomIndex;
		

		if(floorshopitemlist()[i]->broken)
		{//壊れている。
			if(floorshopitemlist()[i]->broken & cDroping::BREAKFLAG_LOSS)
			{//ロス
				
			}
			else
			{
				vpdrop.push_back(floorshopitemlist()[i]);
				value += floorshopitemlist()[i]->単体買値(拠点フラグ());
			}
		}
		else if(floorshopitemlist()[i]->Parent() != NULL)
		{//どっかの中
			value += 0;
		}
		else if(floorshopitemlist()[i]->Holder() != NULL)
		{//誰かが持ってる
			if(floorshopitemlist()[i]->Holder()->ID() == 2020)
			{//店主
				
			}
			else
			{
				vpdrop.push_back(floorshopitemlist()[i]);
				value += floorshopitemlist()[i]->単体買値(拠点フラグ());
			}
		}
		else if(roomIndex == -1 || !(Map().RoomList[roomIndex]->ShopFlag))
		{//店の外
			vpdrop.push_back(floorshopitemlist()[i]);
			value += floorshopitemlist()[i]->単体買値(拠点フラグ());
		}
		else
		{
			
		}
	}

	int loss = 店請求金額() - value;

	if(loss > 0)
	{
		size = vpdrop.size();
		for(i=0;i<size;i++)
		{
			店アイテム購入(vpdrop[i]);
		}

		map<tstring, StyleString> val;
		cMoney moneystr;
		moneystr.value = loss;
		val[_T("Value")] = moneystr.caption();
		g_Langメッセージ(_T("店買う雑費メッセージ"),val);
		if(sg_pDungeonSystem->pSaveQuest->ShopDebt > 0)
		{//つけあり
			int tukepey = min(sg_pDungeonSystem->GameScreenInterface.money.value - loss ,sg_pDungeonSystem->pSaveQuest->ShopDebt);
			if(tukepey > 0)
			{//ツケが払える
				loss += tukepey;
			}
		}
		else if(sg_pDungeonSystem->pSaveQuest->ShopDebt < 0)
		{//後払いあり
			int tukepey = min(loss, -sg_pDungeonSystem->pSaveQuest->ShopDebt);
			if(tukepey > 0)
			{//後払いが払える
				loss -= tukepey;
				pSaveQuest->ShopDebt += tukepey;
			}
		}

		if(sg_pDungeonSystem->GameScreenInterface.money.value - loss >= 0)
		{//足りる
			主人公お金増減(-loss);
			floorshopownermoney() += loss;
		}
		else
		{//つければ足りる(はず)
			int temp = sg_pDungeonSystem->GameScreenInterface.money.value;
			主人公お金増減(-GameScreenInterface.money.value);
			floorshopownermoney() += temp;
			pSaveQuest->ShopDebt += loss - temp;
		}
	}
	floorshopitemvalue() = 店総価値再計算();
	return true;
	*/
}
int cDungeonSystem::店アイテム売却清算(pcCharacter pShopowner)
{
	if(pShopowner == NULL) return false;

	vector<pcDroping> vpdrop,buydroplist;
	店売却アイテムリスト(vpdrop,pShopowner);
	int value=0;
	int i,size = vpdrop.size();
	for(i=0;i<size;i++)
	{
		if(店売却価値差額再計算(vpdrop[i],pShopowner) > 0)
		{
			buydroplist.push_back(vpdrop[i]);
		}
	}
	size = buydroplist.size();
	for(i=0;i<size;i++)
	{
		店アイテム売却(buydroplist[i], pShopowner);
	}
	for(i=0;i<size;i++)
	{
		店アイテム価値再計算(buydroplist[i], pShopowner);
		
		アイテム店フラグ(buydroplist[i], cDroping::SHOPFLAG_SHOP);
		アイテム割引フラグ(buydroplist[i], cDroping::SHOPDISCOUNTFLAG_NONE);
	}
	
	return true;
	/*
	vector<pcDroping> vpdrop,vpdrop2;
	キャラクター目視可範囲落ち物リスト(pShopowner,vpdrop);

	int i,size = vpdrop.size();
	for(i=0;i<size;i++)
	{
		int roomIndex = vpdrop[i]->現在地形()->RoomIndex;
		

		if(vpdrop[i]->broken)
		{//壊れている。

		}
		else if(vpdrop[i]->Parent() != NULL)
		{//どっかの中
			
		}
		else if(vpdrop[i]->Holder() != NULL && vpdrop[i]->Holder() != pShopowner)
		{//誰かが持ってる
	
		}
		else if(roomIndex == -1 || vpdrop[i]->shopitemflag())
		{//店の外 // 店売り
	
		}
		else if(!vpdrop[i]->売却可())
		{// 店売り不可
	
		}
		else
		{
			vpdrop2.push_back(vpdrop[i]);
		}
	}

	size = vpdrop2.size();
	for(i=0;i<size;i++)
	{
		店アイテム売却(vpdrop2[i]);
	}
	floorshopitemvalue() = 店総価値再計算();
	return true;
	*/
}
int cDungeonSystem::店アイテム購入(pcDroping pdrop, pcCharacter pShopowner)
{
	int money = -店購入価値差額再計算(pdrop, pShopowner);

	map<tstring, StyleString> val;
	cMoney moneystr;
	moneystr.value = money;
	val[_T("Value")] = moneystr.caption();
	val[_T("Item")] = pdrop->FullName();

	店金支払い(money);

	if(money == floorshopitemvalueBuy()[pdrop])
	{//全額購入
		アイテム店フラグ(pdrop, cDroping::SHOPFLAG_NONE);
		アイテム割引フラグ(pdrop, cDroping::SHOPDISCOUNTFLAG_NONE);

		
		floorshopitemvalueBuy().erase(pdrop);
		floorshopitemvalueSell().erase(pdrop);

		vector<pcDroping>::iterator itr = floorshopitemlist().end();
		itr = find(floorshopitemlist().begin(),floorshopitemlist().end(),pdrop);
		if(itr != floorshopitemlist().end()) floorshopitemlist().erase(itr);



		g_Langメッセージ(_T("店買うメッセージ"),val);
	}
	else
	{

		g_Langメッセージ(_T("店損失買うメッセージ"),val);

		店アイテム価値再計算(pdrop, pShopowner);
	}
	

	return true;
}
int cDungeonSystem::店アイテム売却(pcDroping pdrop, pcCharacter pShopowner)
{
	int money = 店売却価値差額再計算(pdrop, pShopowner);
	map<tstring, StyleString> val;
	cMoney moneystr;
	moneystr.value = money;	
	val[_T("Value")] = moneystr.caption();
	val[_T("Item")] = pdrop->FullName();

	if(money == pdrop->単体売値())
	{
		g_Langメッセージ(_T("店売るメッセージ"),val);
		floorshopitemlist().push_back(pdrop);
	}
	else
	{
		g_Langメッセージ(_T("店利益売るメッセージ"),val);
	}

	店金受け取り(money);


	return true;
}
int cDungeonSystem::店金支払い(int value)
{
	int money = value;
	if(sg_pDungeonSystem->pSaveQuest->ShopDebt > 0)
	{//つけあり
		int tukepey = min(sg_pDungeonSystem->GameScreenInterface.money.value - money ,sg_pDungeonSystem->pSaveQuest->ShopDebt);
		if(tukepey > 0)
		{//ツケが払える
			money += tukepey;
		}
	}
	else if(sg_pDungeonSystem->pSaveQuest->ShopDebt < 0)
	{//後払いあり
		int tukepey = min(money, -sg_pDungeonSystem->pSaveQuest->ShopDebt);
		if(tukepey > 0)
		{//後払いが払える
			money -= tukepey;
			pSaveQuest->ShopDebt += tukepey;
		}
	}

	if(sg_pDungeonSystem->GameScreenInterface.money.value - money >= 0)
	{//足りる
		主人公お金増減(-money);
		floorshopownermoney() += money;
	}
	else
	{//つければ足りる(はず)
		int temp = sg_pDungeonSystem->GameScreenInterface.money.value;
		主人公お金増減(-GameScreenInterface.money.value);
		floorshopownermoney() += temp;
		pSaveQuest->ShopDebt += money - temp;
	}

	return true;
}
int cDungeonSystem::店金受け取り(int value)
{
	int money = value;

	if(sg_pDungeonSystem->pSaveQuest->ShopDebt > 0)
	{//つけあり
		int tukepey = min(money ,sg_pDungeonSystem->pSaveQuest->ShopDebt);
		if(tukepey > 0)
		{//ツケが払える
			money -= tukepey;
			sg_pDungeonSystem->pSaveQuest->ShopDebt -= tukepey;

		}
	}

	if(floorshopownermoney() - money >= 0)
	{//足りる
		主人公お金増減(money);
		floorshopownermoney() -= money;
	}
	else
	{//足りない
		int temp = money - floorshopownermoney();
		主人公お金増減(money - temp);
		floorshopownermoney() = 0;
		sg_pDungeonSystem->pSaveQuest->ShopDebt -= temp;
	}

	return true;
}

int cDungeonSystem::店後払い支払い()
{
	int tukepey = min(floorshopownermoney() ,-sg_pDungeonSystem->pSaveQuest->ShopDebt);
	
	map<tstring, StyleString> val;
	cMoney moneystr;
	moneystr.value = tukepey;
	val[_T("Value")] = moneystr.caption();
	g_Langメッセージ(_T("店後払いメッセージ"),val);

	主人公お金増減(tukepey);
	floorshopownermoney() -= tukepey;
	sg_pDungeonSystem->pSaveQuest->ShopDebt += tukepey;
	return true;
}
void cDungeonSystem::店生成(int roomindex, int shopFlag)
{
	if(shopFlag == 1)
	{//通常宴会
		if(roomindex < 0 || roomindex >= Map().RoomList.size()) return;
		pcRoom proom = Map().RoomList[roomindex];

		if(proom->LandformList.empty()) return;
		int shopLevel = 店レベル(pSaveQuest->ShopFund);

		proom->ShopFlag = shopFlag;
		pcLandform pland;
		int templandcount, maxspace = 0;

		int itemnum = 店アイテム数(shopLevel);
		int a,i,k,size = proom->LandformList.size();
		for(i=0;i<size;i++)
		{
			templandcount = 0;
			for(a=0;a<25;a++)
			{
				if(代理地形( proom->LandformList[i], a)->kind_surface() == MAPKIND_FLOOR)
				{
					templandcount++;
				}
			}
			if(templandcount > maxspace)
			{
				maxspace = templandcount;
				pland = proom->LandformList[i];
			}
		}

		pcCharacter pcmob = キャラクター生成(2020, shopLevel, CHARACTER_FORSE_OTHER, pland);

		//アイテム初期設置ランダム
		for(i=0;i<itemnum;i++)
		{
			pcDroping pdrop;
			int vint[3];
			for(k=0;k<256;k++)
			{
				アイテム店湧きID(vint);
				if(pSaveQuest->ShopFund / itemnum > DataBase.DropPrice(vint[0]))
				{
					break;
				}
			}
			if(k!=256)
			{//見つかってる
				pdrop = 落ち物生成(vint[0],NULL,pland,true);
				if(pdrop == NULL) continue;
				店アイテム追加(pdrop,pcmob);
				floorshopprofit() -= (pdrop->単体買値(false) + pdrop->単体売値())/2;
			}
		}

		floorshopownermoney() = (int)(pSaveQuest->ShopFund*0.25);
		floorshopprofit() -= floorshopownermoney();
	}


}
void cDungeonSystem::店アイテム追加(pcDroping pdrop, pcCharacter pShopowner)
{
	アイテム店フラグ(pdrop, cDroping::SHOPFLAG_SHOP);
	アイテム割引フラグ(pdrop, cDroping::SHOPDISCOUNTFLAG_NONE);
	floorshopitemlist().push_back(pdrop);
	店アイテム価値再計算(pdrop,pShopowner);
}
void cDungeonSystem::泥棒発覚()
{
	if(!泥棒状態())
	{
		floorstealswitch() = 1;
		map<tstring, StyleString> val;
		g_Langメッセージ(_T("泥棒発覚メッセージ"),val);
		if(店長(0) != NULL) 店長(0)->Forse = CHARACTER_FORSE_ENEMY;
		if(pSaveQuest->ShopDebt < 0) pSaveQuest->ShopDebt = 0;

		AnimationManager().Anime_PlayBGM( _T("doro_intro.wav"),_T("doro_main.wav"));

		int i,k;

		int shoplevel = 店レベル(pSaveQuest->ShopFund);
		for(k=0;k<(shoplevel+3)*3;k++)
		{
			int vint[3];
			if(random() < 0.2)
			{
				vint[0] = 2020;
			}
			else
			{
				vint[0] = 2024;
			}
			vint[1] = shoplevel;
			if(sg_pDungeonSystem->CharaList().size() < 128)
			{
				pcLandform pland = FindUtility::視界外優先_各部屋等確率_ランダム地形検索_配置安全(pPlayerChara()->足元地形());
				for(i=0;i<16;i++)
				{
					if((pPlayerChara()->足元地形()->place - pland->place).dif() > 4)
					{
						break;
					}
					pland = FindUtility::視界外優先_各部屋等確率_ランダム地形検索_配置安全(pPlayerChara()->足元地形());
				}
				pcCharacter pcmob = キャラクター生成(vint[0], vint[1], CHARACTER_FORSE_ENEMY, pland);
			}
		}
	}
}
bool cDungeonSystem::泥棒状態()
{
	return floorstealswitch();
}

//------------------------------------------------------------------------
//ダンジョン
//------------------------------------------------------------------------

//ダンジョンに行く
class cCommandDungeons_goto : public cCommand
{
public:
	cCommandDungeons_goto(const int num, const tstring& dungeonID, const tstring& savefileName)
	{
		caption = g_Lang(_T("はじめから"));
		dungeonID_ = dungeonID;
		savefileName_ = savefileName;
		num_ = num;
	};
	cCommandDungeons_goto(const int num, const tstring& dungeonID, const tstring& savefileName, pcSaveQuest pQuest)
	{
		if(pQuest == NULL) caption = g_Lang(_T("はじめから"));
		else caption = g_Lang(_T("つづきから"));
		dungeonID_ = dungeonID;
		pQuest_ = pQuest;
		savefileName_ = savefileName;
		num_ = num;
	};
	virtual ~cCommandDungeons_goto(void){};
	tstring dungeonID_;
	tstring savefileName_;
	pcSaveQuest pQuest_;
	int num_;

	virtual bool isCanFire()
	{
		return (
			(sg_pDungeonSystem->pPlayerChara()->holdItem.size()
			|| sg_pDungeonSystem->主人公お金所持量() > 0));
	};
	virtual int Action(IDirect3DDevice9 *pDev)
	{

		if(pQuest_ != NULL)
		{
			if(isCanFire())
			{
				return false;
			}
			//sg_pDungeonSystem->ContinueDungeon(pQuest_);
			sg_pDungeonSystem->メニューを閉じる();
			sg_pDungeonSystem->AnimationManager().AddAnime_ContinueDungeon(pQuest_);
		}
		else
		{
			map<tstring, int> localFlags_ = cSaveData::initialLocalFlags(dungeonID_,num_);

			//sg_pDungeonSystem->GotoDungeon(dungeonID_,localFlags_,savefileName_);
			sg_pDungeonSystem->AnimationManager().Anime_PlaySE(_T("step.wav"), sg_pDungeonSystem->pPlayerChara()->足元地形()->place);
			sg_pDungeonSystem->メニューを閉じる();
			sg_pDungeonSystem->AnimationManager().AddAnime_GotoDungeon(dungeonID_,num_,localFlags_,savefileName_);
		}
		//sg_pDungeonSystem->pSaveQuest = pQuest_;
		//g_GameEnv.m_SceneManage->SceneChange(pDev,new csDungeonFirst);
		sg_pDungeonSystem->メニューを閉じる();
		return true;
	};
	virtual StyleString shortExplanationText()
	{
		if(pQuest_ != NULL)
		{
			if(isCanFire())
			{
				return g_Lang(_T("CommandDungeons_goto_shortExplanationText_Warning"));
			}
			else
			{
				return pQuest_->shortExplanationText();
			}
		}

		return _T("");
	};

};
//グローバルフラグスイッチ
class cCommand_globalFlagSwitch : public cCommand
{
public:
	cCommand_globalFlagSwitch(const tstring& captionName, const tstring& flagName, const tstring& flagExplanationName, bool defaultValue)
	{
		captionName_ = captionName;
		flagName_ = flagName;
		flagExplanationName_ = flagExplanationName;
		map<tstring, int>& globalFlags = sg_pDungeonSystem->pSaveData->globalFlags;
		map<tstring, int>::iterator itr = globalFlags.find(flagName);
		if(globalFlags.end() == itr)
		{
			globalFlags[flagName] = (int)defaultValue;
		}

	};
	virtual ~cCommand_globalFlagSwitch(void){};
	tstring captionName_;
	tstring flagName_;
	tstring flagExplanationName_;
	virtual void resetCaption()
	{
		map<tstring,StyleString> val;
		if(sg_pDungeonSystem->pSaveData->globalFlags[flagName_])
		{
			val[_T("BoolValue")] = g_Lang(_T("BoolValueTRUE"));
		}
		else
		{
			val[_T("BoolValue")] = g_Lang(_T("BoolValueFALSE"));
		}
		caption = g_Lang(captionName_.c_str(), val);
	};
	virtual int Action(IDirect3DDevice9 *pDev)
	{
		sg_pDungeonSystem->pSaveData->globalFlags[flagName_] = !(sg_pDungeonSystem->pSaveData->globalFlags[flagName_]);
		sg_pDungeonSystem->メニューリレンダ();


		return false;
	};
	virtual StyleString shortExplanationText()
	{
		map<tstring,StyleString> val;
		if(sg_pDungeonSystem->pSaveData->globalFlags[flagName_])
		{
			val[_T("BoolValue")] = g_Lang(_T("BoolValueTRUE"));
		}
		else
		{
			val[_T("BoolValue")] = g_Lang(_T("BoolValueFALSE"));
		}
		return g_Lang(flagExplanationName_.c_str(),val);
	};
	
};
//本当に本当に消す
class cCommandDungeons_erase_erase_erase : public cCommand
{
public:
	cCommandDungeons_erase_erase_erase(pcSaveQuest pQuest)
	{
		pQuest_ = pQuest;
		caption = g_Lang(_T("本当に本当に消す"));
	};
	virtual ~cCommandDungeons_erase_erase_erase(void){};
	pcSaveQuest pQuest_;
	virtual int Action(IDirect3DDevice9 *pDev)
	{
		/*
			ゲームオーバーにも類似の処理があるので注意されたし

			注意されたし!

		*/
		int i,k,size;
		//厄システム
		vector<pcCharacter> FriendSet;
		size = pQuest_->pFriend.size();
		for(i=0;i<size;i++)
		{
			FriendSet.push_back(pQuest_->pFriend[i]);
		}
		size = FriendSet.size();
		for(i=0;i<size;i++)
		{
			cDungeonSystem::厄システム挿入要請(pQuest_, FriendSet[i]->holdItem);
		}

		vector<pcDroping> vdrop = pQuest_->pPlayer->holdItem;
		cDungeonSystem::厄システム挿入要請(pQuest_, vdrop);

		sg_pDungeonSystem->pSaveData->pSaveStore()->yakuStore += cDungeonSystem::厄ポイント冒険評価(true, false, pQuest_, pQuest_->pPlayer);
		
		sg_pDungeonSystem->pSaveData->pSaveStore()->hinaItem.insert(
			sg_pDungeonSystem->pSaveData->pSaveStore()->hinaItem.end(),
			pQuest_->hinaPickItem.begin(),pQuest_->hinaPickItem.end());

		pQuest_->hinaPickItem.clear();

		sg_pDungeonSystem->pSaveData->pSaveStore()->playerExpStore += cDungeon::culcuscore(pQuest_)/20 + cDungeonSystem::厄ポイント冒険評価(false, false,pQuest_,pQuest_->pPlayer);
			
		pQuest_->setUnEnable();
		pQuest_->save();

		sg_pDungeonSystem->メニューを一枚閉じる();
		sg_pDungeonSystem->メニューを一枚閉じる();
		sg_pDungeonSystem->メニューを一枚閉じる();

		return false;

	};
	virtual StyleString shortExplanationText()
	{
		return g_Lang(_T("本当に本当に消すShortExplanation"));
	};
	
};
//やっぱりやめる
class cCommandDungeons_erase_erase_cancel : public cCommand
{
public:
	cCommandDungeons_erase_erase_cancel()
	{
		caption = g_Lang(_T("やっぱりやめる"));
	};
	virtual ~cCommandDungeons_erase_erase_cancel(void){};
	virtual int Action(IDirect3DDevice9 *pDev)
	{
		sg_pDungeonSystem->メニューを一枚閉じる();
		sg_pDungeonSystem->メニューを一枚閉じる();
		return false;
	};
	virtual StyleString shortExplanationText()
	{
		return _T("");
	};
	
};
//本当に消す
class cCommandDungeons_erase_erase : public cCommand
{
public:
	cCommandDungeons_erase_erase(pcSaveQuest pQuest)
	{
		pQuest_ = pQuest;
		caption = g_Lang(_T("本当に消す"));
	};
	virtual ~cCommandDungeons_erase_erase(void){};
	pcSaveQuest pQuest_;
	virtual int Action(IDirect3DDevice9 *pDev)
	{
		
		pcControlLayer pccl;
		pcSelectWindow pcsw;

		//新規コントロールレイヤー
		sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
		pccl->Init(pDev);

		pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

		pcsw->commandList.push_back(pcCommand(new cCommandDungeons_erase_erase_cancel() ));
		pcsw->commandList.push_back(pcCommand(new cCommandDungeons_erase_erase_erase(pQuest_) ));

		pcsw->Init(pDev, 8, pcsw->commandList.size());
		pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(4));
		pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(4));

		return false;
	};
	virtual StyleString shortExplanationText()
	{
		return g_Lang(_T("本当に消すShortExplanation"));
	};
	
};
//やっぱりやめる
class cCommandDungeons_erase_cancel : public cCommand
{
public:
	cCommandDungeons_erase_cancel()
	{
		caption = g_Lang(_T("やっぱりやめる"));
	};
	virtual ~cCommandDungeons_erase_cancel(void){};
	virtual int Action(IDirect3DDevice9 *pDev)
	{
		sg_pDungeonSystem->メニューを一枚閉じる();
		return false;
	};
	virtual StyleString shortExplanationText()
	{
		return _T("");
	};
	
};
//しおりを消す
class cCommandDungeons_erase : public cCommand
{
public:
	cCommandDungeons_erase(pcSaveQuest pQuest)
	{
		pQuest_ = pQuest;
		caption = g_Lang(_T("しおりを消す"));
	};
	virtual ~cCommandDungeons_erase(void){};
	pcSaveQuest pQuest_;
	virtual int Action(IDirect3DDevice9 *pDev)
	{
		
		pcControlLayer pccl;
		pcSelectWindow pcsw;

		//新規コントロールレイヤー
		sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
		pccl->Init(pDev);

		pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

		pcsw->commandList.push_back(pcCommand(new cCommandDungeons_erase_cancel() ));
		pcsw->commandList.push_back(pcCommand(new cCommandDungeons_erase_erase(pQuest_) ));

		pcsw->Init(pDev, 8, pcsw->commandList.size());
		pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(3));
		pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(3));

		return false;
	};
	virtual StyleString shortExplanationText()
	{
		return g_Lang(_T("しおりを消すShortExplanation"));
	};
	
};
//ダンジョンのしおり
class cCommandDungeons_shiori : public cCommand
{
public:
	cCommandDungeons_shiori( const tstring& dungeonID, int num, const tstring& suffix)
	{
		StyleString captionStr(_T("しおり"));
		captionStr.addstring(num);
		num_ = num;
		caption = g_Lang(captionStr.c_str());//しおり1～3
		dungeonID_ = dungeonID;

		pcSaveQuest pQuest;
		pQuest_ = pcSaveQuest(new cSaveQuest);
		pQuest_->saveFileNum = num;
		questSaveFileString_ = dungeonID;
		questSaveFileString_ += suffix;
		pQuest_->Init(sg_pDungeonSystem->pDevice_D3D, questSaveFileString_);
		pQuest_->load();
	};
	virtual ~cCommandDungeons_shiori(void){};
	tstring dungeonID_;
	tstring questSaveFileString_;
	pcSaveQuest pQuest_;
	int num_;
	virtual int Action(IDirect3DDevice9 *pDev)
	{
		
		StyleString errormassage;
		if(!sg_pDungeonSystem->isPlayerCharacterAccordingStartRegulation(dungeonID_,errormassage))
		{
			return false;
		}
		if(!sg_pDungeonSystem->isRegureCharacter(dungeonID_, sg_pDungeonSystem->pPlayerChara())
			&& (sg_pDungeonSystem->pSaveData->globalFlags_ClearedFlag(dungeonID_,FALSE) == FALSE)//未クリア
			)
		{
			return false;
		}

		pcControlLayer pccl;
		pcSelectWindow pcsw;

		//新規コントロールレイヤー
		sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
		pccl->Init(pDev);

		pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));


		if(pQuest_->enable())
		{
			pcsw->commandList.push_back(pcCommand(new cCommandDungeons_goto(num_, dungeonID_, questSaveFileString_, pQuest_) ));
			pcsw->commandList.push_back(pcCommand(new cCommandDungeons_erase(pQuest_) ));
		
		}
		else
		{
			pcsw->commandList.push_back(pcCommand(new cCommandDungeons_goto(num_, dungeonID_, questSaveFileString_) ));
			do
			{
				pcScriptRLayer dungeonform = sg_pDungeonSystem->DataBase.DungeonImportDataFile(dungeonID_);
				
				if(dungeonform == NULL) break;
				dungeonform = dungeonform->pmember(_T("動的識別不要フラグ"));
				
				int value = 0;//デフォルト
				if(dungeonform)
				{
					value = tstrtoi(dungeonform->topscript());
				}
				if(!value)
				{
					pcsw->commandList.push_back(pcCommand(new cCommand_globalFlagSwitch(_T("識別補助"),
					cSaveData::globalFlagsKey_AppreciationSupportKey(dungeonID_,num_),
					_T("識別補助ShortExplanation"),
					true) ));
				}
			}
			while(0);

			if(sg_pDungeonSystem->pSaveData->globalFlags_ClearedFlag(dungeonID_,FALSE)
				&& sg_pDungeonSystem->isRegureCharacter(dungeonID_, sg_pDungeonSystem->pPlayerChara()))
			{
				pcsw->commandList.push_back(pcCommand(new cCommand_globalFlagSwitch(_T("ストーリー設定"),
					cSaveData::globalFlagsKey_StoryEventKey(dungeonID_,num_),
					_T("ストーリー設定ShortExplanation"),
					false) ));
			}
		}

		pcsw->Init(pDev, 12, pcsw->commandList.size());
		pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(2));
		pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(2));

		return true;
	};
	virtual StyleString shortExplanationText()
	{

		if(pQuest_ != NULL && pQuest_->enable())
		{
			return pQuest_->shortExplanationText();
		}
		if(!sg_pDungeonSystem->isRegureCharacter(dungeonID_, sg_pDungeonSystem->pPlayerChara())
			&& (sg_pDungeonSystem->pSaveData->globalFlags_ClearedFlag(dungeonID_,FALSE) == FALSE)//未クリア
			)
		{
			return g_Lang(_T("未クリア非規定キャラShortExplanation"));
		}
		StyleString errormassage;
		if(!sg_pDungeonSystem->isPlayerCharacterAccordingStartRegulation(dungeonID_,errormassage))
		{
			return errormassage;
		}

		return _T("");
	};
	
};
//結果表示
class cCommandDungeons_rankingShowResult : public cCommand
{
public:
	cCommandDungeons_rankingShowResult(void){caption = g_Lang(_T("入る"));};
	cCommandDungeons_rankingShowResult(tstring s, cSaveResult& result)
	{
		caption = s;
		result_ = result;
	};
	virtual ~cCommandDungeons_rankingShowResult(void){};
	cSaveResult result_;
	virtual int Action(IDirect3DDevice9 *pDev)
	{
		pcControlLayer pccl;
		pcResultWindow pcrw;
		//新規コントロールレイヤー
		sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
		pccl->Init(pDev);

		//新規リストウィンドウ
		pccl->WindowList.push_back(pcrw = pcResultWindow(new cResultWindow));


		pcrw->Init(pDev,result_);
		pcrw->CenterX = SCREEN_X/2;
		pcrw->CenterY = SCREEN_Y/2;
		return true;
	};
};
//ランキング
class cCommandDungeons_rankingSA : public cCommand
{
public:
	cCommandDungeons_rankingSA(void){caption = g_Lang(_T("スコアアタックランキング"));};
	cCommandDungeons_rankingSA(tstring s)
	{
		caption = g_Lang(_T("スコアアタックランキング"));
		dungeonID = s;
	};
	virtual ~cCommandDungeons_rankingSA(void){};
	tstring dungeonID;
	virtual int Action(IDirect3DDevice9 *pDev)
	{
		vector<cSaveResult> vranking = sg_pDungeonSystem->DataBase.DungeonRanking(dungeonID)->SARanking;


		
		pcControlLayer pccl;
		pcSelectWindow pcsw;

		//新規コントロールレイヤー
		sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
		pccl->Init(pDev);

		pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

		int i;
		for(i=0;i<vranking.size();i++)
		{
			StyleString caption;
			caption.addstring(vranking[i].score_);
			caption.addstring(_T(" ["));
			caption.addstring(vranking[i].PlayerSign_);
			caption.addstring(_T("]("));
			struct tm *timeObject;
			timeObject = localtime(&vranking[i].Date_);
			TCHAR str[256];
			_tcsftime(str, 255, _T("%y/%m/%d %H:%M:%S"), timeObject);
			caption.addstring(str);
			caption.addstring(_T(")"));
			pcsw->commandList.push_back(pcCommand(new cCommandDungeons_rankingShowResult(caption.conclete_tstr(),vranking[i]) ));
		}

		if(pcsw->commandList.empty())
		{
			pcsw->commandList.push_back(pcCommand(new cCommandNull(g_Lang(_T("記録なし"))) ));
		}

		pcsw->Init(pDev, 24, pcsw->commandList.size());
		pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(3));
		pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(3));
		return true;
	};
};
class cCommandDungeons_freeRankingSA : public cCommandDungeons_rankingSA
{
public:
	cCommandDungeons_freeRankingSA(void){caption = g_Lang(_T("フリーキャラスコアアタックランキング"));};
	cCommandDungeons_freeRankingSA(tstring s)
	{
		caption = g_Lang(_T("フリーキャラスコアアタックランキング"));
		dungeonID = s;
	};
	virtual ~cCommandDungeons_freeRankingSA(void){};
	tstring dungeonID;
	virtual int Action(IDirect3DDevice9 *pDev)
	{
		vector<cSaveResult> vranking = sg_pDungeonSystem->DataBase.DungeonRanking(dungeonID)->FreeStyle_SARanking;


		
		pcControlLayer pccl;
		pcSelectWindow pcsw;

		//新規コントロールレイヤー
		sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
		pccl->Init(pDev);

		pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

		int i;
		for(i=0;i<vranking.size();i++)
		{
			StyleString caption;
			caption.addstring(vranking[i].score_);
			caption.addstring(_T(" ["));
			caption.addstring(vranking[i].PlayerSign_);
			caption.addstring(_T("]("));
			struct tm *timeObject;
			timeObject = localtime(&vranking[i].Date_);
			TCHAR str[256];
			_tcsftime(str, 255, _T("%y/%m/%d %H:%M:%S"), timeObject);
			caption.addstring(str);
			caption.addstring(_T(")"));
			pcsw->commandList.push_back(pcCommand(new cCommandDungeons_rankingShowResult(caption.conclete_tstr(),vranking[i]) ));
		}

		if(pcsw->commandList.empty())
		{
			pcsw->commandList.push_back(pcCommand(new cCommandNull(g_Lang(_T("記録なし"))) ));
		}

		pcsw->Init(pDev, 24, pcsw->commandList.size());
		pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(3));
		pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(3));
		return true;
	};
};
//ランキング
class cCommandDungeons_rankingRTA : public cCommand
{
public:
	cCommandDungeons_rankingRTA(void){caption = g_Lang(_T("リアルタイムアタックランキング"));};
	cCommandDungeons_rankingRTA(tstring s)
	{
		caption = g_Lang(_T("リアルタイムアタックランキング"));
		dungeonID = s;
	};
	virtual ~cCommandDungeons_rankingRTA(void){};
	tstring dungeonID;
	virtual int Action(IDirect3DDevice9 *pDev)
	{
		vector<cSaveResult> vranking = sg_pDungeonSystem->DataBase.DungeonRanking(dungeonID)->RTARanking;


		
		pcControlLayer pccl;
		pcSelectWindow pcsw;

		//新規コントロールレイヤー
		sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
		pccl->Init(pDev);

		pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

		int i;
		for(i=0;i<vranking.size();i++)
		{
			StyleString caption;
			caption.addstring(((int)vranking[i].SumFramerealtime_)/(60*60*60),_T("%d"));
			caption.addstring(_T(":"));
			caption.addstring((((int)vranking[i].SumFramerealtime_/60)/60)%60,_T("%02d"));
			caption.addstring(_T(":"));
			caption.addstring(((int)vranking[i].SumFramerealtime_/60)%60,_T("%02d"));
			caption.addstring(_T(" ["));
			caption.addstring(vranking[i].PlayerSign_);
			caption.addstring(_T("]("));
			struct tm *timeObject;
			timeObject = localtime(&vranking[i].Date_);
			TCHAR str[256];
			_tcsftime(str, 255, _T("%y/%m/%d %H:%M:%S"), timeObject);
			caption.addstring(str);
			caption.addstring(_T(")"));
			pcsw->commandList.push_back(pcCommand(new cCommandDungeons_rankingShowResult(caption.conclete_tstr(),vranking[i]) ));
		}

		if(pcsw->commandList.empty())
		{
			pcsw->commandList.push_back(pcCommand(new cCommandNull(g_Lang(_T("記録なし"))) ));
		}

		pcsw->Init(pDev, 24, pcsw->commandList.size());
		pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(3));
		pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(3));

		return true;
	};
};
//ランキング
class cCommandDungeons_freeRankingRTA : public cCommand
{
public:
	cCommandDungeons_freeRankingRTA(void){caption = g_Lang(_T("フリーキャラリアルタイムアタックランキング"));};
	cCommandDungeons_freeRankingRTA(tstring s)
	{
		caption = g_Lang(_T("フリーキャラリアルタイムアタックランキング"));
		dungeonID = s;
	};
	virtual ~cCommandDungeons_freeRankingRTA(void){};
	tstring dungeonID;
	virtual int Action(IDirect3DDevice9 *pDev)
	{
		vector<cSaveResult> vranking = sg_pDungeonSystem->DataBase.DungeonRanking(dungeonID)->FreeStyle_RTARanking;


		
		pcControlLayer pccl;
		pcSelectWindow pcsw;

		//新規コントロールレイヤー
		sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
		pccl->Init(pDev);

		pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

		int i;
		for(i=0;i<vranking.size();i++)
		{
			StyleString caption;
			caption.addstring(((int)vranking[i].SumFramerealtime_)/(60*60*60),_T("%d"));
			caption.addstring(_T(":"));
			caption.addstring((((int)vranking[i].SumFramerealtime_/60)/60)%60,_T("%02d"));
			caption.addstring(_T(":"));
			caption.addstring(((int)vranking[i].SumFramerealtime_/60)%60,_T("%02d"));
			caption.addstring(_T(" ["));
			caption.addstring(vranking[i].PlayerSign_);
			caption.addstring(_T("]("));
			struct tm *timeObject;
			timeObject = localtime(&vranking[i].Date_);
			TCHAR str[256];
			_tcsftime(str, 255, _T("%y/%m/%d %H:%M:%S"), timeObject);
			caption.addstring(str);
			caption.addstring(_T(")"));
			pcsw->commandList.push_back(pcCommand(new cCommandDungeons_rankingShowResult(caption.conclete_tstr(),vranking[i]) ));
		}

		if(pcsw->commandList.empty())
		{
			pcsw->commandList.push_back(pcCommand(new cCommandNull(g_Lang(_T("記録なし"))) ));
		}

		pcsw->Init(pDev, 24, pcsw->commandList.size());
		pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(3));
		pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(3));

		return true;
	};
};
//ランキング
class cCommandDungeons_rankingTTA : public cCommand
{
public:
	cCommandDungeons_rankingTTA(void){caption = g_Lang(_T("ターンタイムアタックランキング"));};
	cCommandDungeons_rankingTTA(tstring s)
	{
		caption = g_Lang(_T("ターンタイムアタックランキング"));
		dungeonID = s;
	};
	virtual ~cCommandDungeons_rankingTTA(void){};
	tstring dungeonID;
	virtual int Action(IDirect3DDevice9 *pDev)
	{
		vector<cSaveResult> vranking = sg_pDungeonSystem->DataBase.DungeonRanking(dungeonID)->TTARanking;


		
		pcControlLayer pccl;
		pcSelectWindow pcsw;

		//新規コントロールレイヤー
		sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
		pccl->Init(pDev);

		pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

		int i;
		for(i=0;i<vranking.size();i++)
		{
			StyleString caption;
			caption.addstring(vranking[i].Sumturn_);
			caption.addstring(_T(" ["));
			caption.addstring(vranking[i].PlayerSign_);
			caption.addstring(_T("]("));
			struct tm *timeObject;
			timeObject = localtime(&vranking[i].Date_);
			TCHAR str[256];
			_tcsftime(str, 255, _T("%y/%m/%d %H:%M:%S"), timeObject);
			caption.addstring(str);
			caption.addstring(_T(")"));
			pcsw->commandList.push_back(pcCommand(new cCommandDungeons_rankingShowResult(caption.conclete_tstr(),vranking[i]) ));
		}

		if(pcsw->commandList.empty())
		{
			pcsw->commandList.push_back(pcCommand(new cCommandNull(g_Lang(_T("記録なし"))) ));
		}

		pcsw->Init(pDev, 24, pcsw->commandList.size());
		pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(3));
		pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(3));
		return true;
	};
};
//ランキング
class cCommandDungeons_freeRankingTTA : public cCommand
{
public:
	cCommandDungeons_freeRankingTTA(void){caption = g_Lang(_T("フリーキャラターンタイムアタックランキング"));};
	cCommandDungeons_freeRankingTTA(tstring s)
	{
		caption = g_Lang(_T("フリーキャラターンタイムアタックランキング"));
		dungeonID = s;
	};
	virtual ~cCommandDungeons_freeRankingTTA(void){};
	tstring dungeonID;
	virtual int Action(IDirect3DDevice9 *pDev)
	{
		vector<cSaveResult> vranking = sg_pDungeonSystem->DataBase.DungeonRanking(dungeonID)->FreeStyle_TTARanking;


		
		pcControlLayer pccl;
		pcSelectWindow pcsw;

		//新規コントロールレイヤー
		sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
		pccl->Init(pDev);

		pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

		int i;
		for(i=0;i<vranking.size();i++)
		{
			StyleString caption;
			caption.addstring(vranking[i].Sumturn_);
			caption.addstring(_T(" ["));
			caption.addstring(vranking[i].PlayerSign_);
			caption.addstring(_T("]("));
			struct tm *timeObject;
			timeObject = localtime(&vranking[i].Date_);
			TCHAR str[256];
			_tcsftime(str, 255, _T("%y/%m/%d %H:%M:%S"), timeObject);
			caption.addstring(str);
			caption.addstring(_T(")"));
			pcsw->commandList.push_back(pcCommand(new cCommandDungeons_rankingShowResult(caption.conclete_tstr(),vranking[i]) ));
		}

		if(pcsw->commandList.empty())
		{
			pcsw->commandList.push_back(pcCommand(new cCommandNull(g_Lang(_T("記録なし"))) ));
		}

		pcsw->Init(pDev, 24, pcsw->commandList.size());
		pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(3));
		pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(3));
		return true;
	};
};
//ダンジョンランキング
class cCommandDungeons_rankings : public cCommand
{
public:
	cCommandDungeons_rankings(void){caption = g_Lang(_T("ダンジョンランキング"));};
	cCommandDungeons_rankings(tstring s)
	{
		caption = g_Lang(_T("ダンジョンランキング"));
		dungeonID = s;
	};
	virtual ~cCommandDungeons_rankings(void){};
	tstring dungeonID;
	virtual int Action(IDirect3DDevice9 *pDev)
	{
		
		pcControlLayer pccl;
		pcSelectWindow pcsw;

		//新規コントロールレイヤー
		sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
		pccl->Init(pDev);

		pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));


		pcsw->commandList.push_back(pcCommand(new cCommandDungeons_rankingSA(dungeonID) ));	
		pcsw->commandList.push_back(pcCommand(new cCommandDungeons_rankingRTA(dungeonID) ));
		pcsw->commandList.push_back(pcCommand(new cCommandDungeons_rankingTTA(dungeonID) ));
		pcsw->commandList.push_back(pcCommand(new cCommandDungeons_freeRankingSA(dungeonID) ));	
		pcsw->commandList.push_back(pcCommand(new cCommandDungeons_freeRankingRTA(dungeonID) ));
		pcsw->commandList.push_back(pcCommand(new cCommandDungeons_freeRankingTTA(dungeonID) ));
		pcsw->Init(pDev, 12, pcsw->commandList.size());
		pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(2));
		pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(2));

		return true;
	};
	/*
	virtual StyleString shortExplanationText()
	{
	}
	*/
};
//ダンジョン選択
class cCommandDungeons : public cCommand
{
public:
	cCommandDungeons(void){caption = g_Lang(_T("ダンジョン名"));};
	cCommandDungeons(tstring s)
	{
		caption = sg_pDungeonSystem->DataBase.DungeonImportDataName(s);
		dungeonID = s;
	};
	virtual ~cCommandDungeons(void){};
	tstring dungeonID;
	virtual int Action(IDirect3DDevice9 *pDev)
	{
		
		pcControlLayer pccl;
		pcSelectWindow pcsw;

		//新規コントロールレイヤー
		sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
		pccl->Init(pDev);

		pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

		

		pcsw->commandList.push_back(pcCommand(new cCommandDungeons_shiori(dungeonID, 1, _T("_1"))));
		pcsw->commandList.push_back(pcCommand(new cCommandDungeons_shiori(dungeonID, 2, _T("_2"))));
		pcsw->commandList.push_back(pcCommand(new cCommandDungeons_shiori(dungeonID, 3, _T("_3"))));
		/*
		pcSaveQuest pQuest;
		pQuest = pcSaveQuest(new cSaveQuest);
		pQuest->Init(pDev, dungeonID+_T("_temp"));
		pQuest->load();

		
		map<tstring, int> localFlags_;

		if(pQuest->enable())
		{
			pcsw->commandList.push_back(pcCommand(new cCommandDungeons_goto(localFlags_, dungeonID, dungeonID + _T("_temp"), pQuest) ));
		}
		else
		{
			pcsw->commandList.push_back(pcCommand(new cCommandDungeons_goto(localFlags_, dungeonID, dungeonID + _T("_temp")) ));
		}
		*/

		pcsw->commandList.push_back(pcCommand(new cCommandDungeons_rankings(dungeonID) ));
		pcsw->Init(pDev, 12, pcsw->commandList.size());
		pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(2));
		pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(2));

		return true;
	};
	
	virtual StyleString shortExplanationText()
	{
		/*
		map<tstring,StyleString> val;
		pcScriptRLayer pst = sg_pDungeonSystem->DataBase.DungeonImportDataFile(dungeonID);
		if(pst) pst = pst->pmember(_T("最大階層"));
		if(pst) val[_T("MaxFloor")] = pst->topscript();
		else val[_T("MaxFloor")] = _T("Not Defined");
		return g_Lang(_T("ダンジョンshortExplanationText"),val);
		*/
		return sg_pDungeonSystem->StartRegulationMassage(dungeonID);
	}
	
};


void cDungeonSystem::SelectDungeon(const vector<tstring>& Dungeons)
{
	pcControlLayer pccl;
	pcSelectWindow pcsw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDevice_D3D);

	pcsw = pcSelectWindow(new cSelectWindow);

	int i;
	for(i=0;i<Dungeons.size();i++)
	{
#ifndef _UNRELEASE
		if(sg_pDungeonSystem->pSaveData->globalFlags_AppearedFlag(Dungeons[i],FALSE))
#endif
		{
			pcsw->commandList.push_back(pcCommand(new cCommandDungeons(Dungeons[i]) ));
		}
	}


	if(!pcsw->commandList.empty())
	{
		pcsw->Init(pDevice_D3D, 12, pcsw->commandList.size());
		pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(2));
		pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(2));
		pcsw->setSelectMemoryKey((tstring)_T("dungeonSelect_base_exit"));

		pccl->WindowList.push_back(pcsw);

	}

}
void cDungeonSystem::GotoDungeon(const tstring& DungeonID)
{
	map<tstring,int> privateFlags;
	tstring savefileName(DungeonID + _T("_temp"));
	GotoDungeon(DungeonID,-1,privateFlags,savefileName);
}
void cDungeonSystem::GotoDungeon(const tstring& DungeonID, const int saveFileNum, const map<tstring,int>& privateFlags, const tstring& savefileName)
{
	nextDungeon = DungeonID;

	pcSaveQuest pQuest;
	pQuest = pcSaveQuest(new cSaveQuest);
	pQuest->Init(pDevice_D3D,savefileName);
	pQuest->floor = 1;
	pQuest->randBase = time(NULL);
	pQuest->privateFlags = privateFlags;
	pQuest->DungeonID = DungeonID;
	pQuest->saveFileNum = saveFileNum;
	if(pPlayerChara() != NULL) pQuest->pPlayer = pPlayerChara();
	else
	{
		pQuest->pPlayer = pcCharacter(new cPlayerChara_ID_2);//tyougoku
		pQuest->pPlayer->Init(pDevice_D3D,pQuest->pPlayer);
		pQuest->pPlayer->Forse = CHARACTER_FORSE_FRIEND;
	}
	pQuest->money = sg_pDungeonSystem->GameScreenInterface.money.value;
	
	pQuest->pPlayer->LV = 1;
	pQuest->pPlayer->EXP = 0;
	pQuest->pPlayer->SetMHP();
	pQuest->pPlayer->HP = pQuest->pPlayer->MHP;
	pQuest->pPlayer->Stomach = pQuest->pPlayer->MaxStomach = 100;
	pQuest->money = GameScreenInterface.money.value;
	//pQuest->save();

	sg_pDungeonSystem->pSaveQuest = pQuest;



	if(拠点フラグ())
	{//拠点から何処かにいく
		GameBaseEnd();
	}

	//sg_pDungeonSystem->InitDungeon(pDevice_D3D);
	
}

void cDungeonSystem::ContinueDungeon(pcSaveQuest continueQuest)
{
	if(continueQuest == NULL) return;
	
	nextDungeon = continueQuest->DungeonID;

	sg_pDungeonSystem->pSaveQuest = continueQuest;
	if(pSaveQuest->pPlayer == NULL)
	{
		pSaveQuest->pPlayer = pcCharacter(new cPlayerChara_ID_2);//tyougoku
		pSaveQuest->pPlayer->Init(pDevice_D3D,pSaveQuest->pPlayer);
		pSaveQuest->pPlayer->Forse = CHARACTER_FORSE_FRIEND;
		pSaveQuest->pPlayer->LV = 1;
		pSaveQuest->pPlayer->EXP = 0;
		pSaveQuest->pPlayer->SetMHP();
		pSaveQuest->pPlayer->HP = pSaveQuest->pPlayer->MHP;
		pSaveQuest->pPlayer->Stomach = 100;
		pSaveQuest->pPlayer->MaxStomach = 100;
		pSaveQuest->money = GameScreenInterface.money.value;
	}

	
	if(拠点フラグ())
	{//拠点から何処かにいく
		GameBaseEnd();
	}

}


void cDungeonSystem::resetDungeon(const tstring DungeonID)
{
	nextDungeon = _T("");

	sg_pDungeonSystem->InitDungeon(pDevice_D3D);

}

class cCommandSimple :
	public cCommand
{
public:
	cCommandSimple(void){};
	cCommandSimple(StyleString s, int value, int* pointer)
	{
		caption = s;
		pointer_ = pointer;
		value_ = value;
	};
public:
	virtual ~cCommandSimple(void){};
	virtual int Action(IDirect3DDevice9 *pDev)
	{
		*pointer_ = value_;
		sg_pDungeonSystem->メニューを閉じる();
	return true;
	};

protected:
	int* pointer_;
	int value_;

};

void cDungeonSystem::Dungeonprocess(const tstring& DungeonsID)
{

	{

		int i, size = pDungeon()->storyScriptEnvironments().size();
		for(i=0;i<size;i++)
		{
			if(!pDungeon()->storyScriptEnvironments()[i].function(_T("dungeonProcess")))
			{
				pDungeon()->storyScriptEnvironments()[i].outputErrorFile(_T("Error_")+DungeonsID+_T("_dungeonProcess.txt"));
			}
			//DungeonStoryScript_ExtraSetting(psr,keys[i],_T("floorExtraSetting"),valiable);
		}
		
	}

	if(DungeonsID == _T("first_Home"))
	{
		//sg_pDungeonSystem->Map().visibleForcusX() += g_pPlayerInput()->X * 0.1;

		//sg_pDungeonSystem->Map().visibleForcusY() += g_pPlayerInput()->Y * 0.1;
		static bool sFlag = false;
		if(sg_pDungeonSystem->menuControlLayerV().size() == 0)
		{//何も表示されて無い
			
			if(!sFlag && sg_pDungeonSystem->pPlayerChara()->足元地形()->RoomIndex == -1)
			{//通路
				sFlag = true;

				/*
				pcControlLayer pccl;
				pcSelectWindow pcsw;

				sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));

				pccl->Init(pDev);
				pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

				pcsw->commandList.push_back(pcCommand(new cCommandStart2 ));
				pcsw->commandList.push_back(pcCommand(new cCommandEnd2 ));
				pcsw->Init(pDev, 5, pcsw->commandList.size());
				pcsw->setLeft(32);
				pcsw->setTop(32);

				pcsw->playsound_decide();
				*/
				
				vector<tstring> dungeons;
#ifdef _UNRELEASE
				dungeons.push_back(_T("intro_Dungeon"));
#endif
				dungeons.push_back(_T("tutorial_Dungeon"));
				
//#ifdef _UNRELEASE
//				dungeons.push_back(_T("preTrial_Dungeon"));
//#endif
//#ifdef _DEBUG
//				dungeons.push_back(_T("preTrial_Dungeon"));
//#endif
				//dungeons.push_back(_T("koumakan_Dungeon"));
				//dungeons.push_back(_T("koumakan_Dungeon_Luna"));
				//dungeons.push_back(_T("koumakan_Dungeon_Extra"));
				dungeons.push_back(_T("red_Dungeon"));
				dungeons.push_back(_T("blue_Dungeon"));
				dungeons.push_back(_T("green_Dungeon"));
				dungeons.push_back(_T("purple_Dungeon"));
#ifdef _UNRELEASE
				//dungeons.push_back(_T("red_extra_Dungeon"));
				//dungeons.push_back(_T("blue_extra_Dungeon"));
				//dungeons.push_back(_T("green_extra_Dungeon"));
				//dungeons.push_back(_T("purple_extra_Dungeon"));
				//dungeons.push_back(_T("event_dream_Dungeon"));
				//dungeons.push_back(_T("event_ending_Dungeon"));
				dungeons.push_back(_T("ten_match_Dungeon"));
#endif
				dungeons.push_back(_T("last_Dungeon"));
#ifdef _UNRELEASE
				dungeons.push_back(_T("99F_Dungeon"));
				dungeons.push_back(_T("99F_ura_Dungeon"));
				//dungeons.push_back(_T("R10_99F_Dungeon"));
				dungeons.push_back(_T("test_Dungeon"));
#endif
				sg_pDungeonSystem->SelectDungeon(dungeons);
				
			}
			else
			{
				if(sg_pDungeonSystem->pPlayerChara()->足元地形()->RoomIndex != -1)
				{
					sFlag = false;
				}
			}
			
		}
	}
	else if(DungeonsID == _T("tutorial_Dungeon"))
	{
	/*
		if(FreeFlags().empty())
		{
			FreeFlags().resize(40,0);
			

			pcControlLayer pccl;
			pcSelectWindow pcsw;
			pcGameWindow pcgw;

			sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));

			pccl->Init(pDevice_D3D);
			pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));
			pccl->WindowList.push_back(pcgw = pcGameWindow(new cGameWindow));


			pcgw->Init(pDevice_D3D, 24, 1);
			pcgw->Text() = g_Story(_T("tutorial選択"));
			pcgw->setLeft(GameScreenInterface.menuPosLeft(1));
			pcgw->setTop(GameScreenInterface.menuPosTop(2));

			vector<tstring> choise;
			pcScriptRLayer psrl = g_systemSR.pdata()->pmember((tstring)_T("Story"));
			if(psrl != NULL)
			{
				psrl = psrl->pmember((tstring)_T("tutorial選択肢"));
				if(psrl != NULL)
				{
					choise = psrl->script();
				}		
			}
			if(choise.size() < 2)
			{
				choise.resize(2);
				choise[0] = _T("Yes");
				choise[1] = _T("No");
			}
	
			pcsw->commandList.push_back(pcCommand(new cCommandSimple(choise[0],1,&(FreeFlags()[0])) ));
			pcsw->commandList.push_back(pcCommand(new cCommandSimple(choise[1],0,&(FreeFlags()[0])) ));
			pcsw->Init(pDevice_D3D, 3, pcsw->commandList.size());
			pcsw->setRight(GameScreenInterface.menuPosRight(3));
			pcsw->setTop(GameScreenInterface.menuPosTop(4));

			pcsw->playsound_decide();
				
		}
		else if(FreeFlags()[0] && !MessageW().isStockingMassage())
		{
			map<tstring,StyleString> val;

			if(!FreeFlags()[1])
			{
				FreeFlags()[1] = 1;
				val[_T("Button")] = setStyle(1+g_GameEnv.m_Input.patInputManager->padconfigI2B[0]);
				g_Storyメッセージ(_T("tutorial挨拶"),val);
				AnimationManager().Anime_MessageWait();
				g_Storyメッセージ(_T("tutorial挨拶2"),val);
		
			}
			else if(!FreeFlags()[2] && 2 < SumTurnCount())
			{
				FreeFlags()[2] = 1;
				g_Storyメッセージ(_T("tutorial移動"),val);
				AnimationManager().Anime_MessageWait();
				
			}
			else if(!FreeFlags()[3] && 4 < SumTurnCount()
				&& キャラ前地形(pPlayerChara(),1)->pOnChar != NULL)
			{
				FreeFlags()[3] = 1;
				val[_T("Button")] = setStyle(1+g_GameEnv.m_Input.patInputManager->padconfigI2B[0]);
				g_Storyメッセージ(_T("tutorial攻撃"),val);
				AnimationManager().Anime_MessageWait();
				
			}


			if(!FreeFlags()[4])
			{
				int a,flag = 0;
				for(a=1;a<8;a++)
				{
					if(キャラ視点方角地形(pPlayerChara(),a,1)->pOnChar != NULL
						&& isCanNeighberAttack(pPlayerChara(),pPlayerChara()->aspect+a,1,0,0))
					{
						flag = 1;
						break;
					}
				}
				if( 20 < SumTurnCount()
					&& flag)
				{
					FreeFlags()[4] = 1;
					val[_T("Button")] = setStyle(1+g_GameEnv.m_Input.patInputManager->padconfigI2B[2]);
					g_Storyメッセージ(_T("tutorial振り向き"),val);
					AnimationManager().Anime_MessageWait();
				}
			}
		
		
			else if(!FreeFlags()[5] && 100 < SumTurnCount())
			{
				FreeFlags()[5] = 1;
				val[_T("Button")] = setStyle(1+g_GameEnv.m_Input.patInputManager->padconfigI2B[1]);
				g_Storyメッセージ(_T("tutorialダッシュ"),val);
				AnimationManager().Anime_MessageWait();
				
			}
			else if(!FreeFlags()[6] && 200 < SumTurnCount()
				&& キャラ前地形(pPlayerChara(),1)->pOnChar != NULL)
			{
				FreeFlags()[6] = 1;
				val[_T("Button")] = setStyle(1+g_GameEnv.m_Input.patInputManager->padconfigI2B[7]);
				g_Storyメッセージ(_T("tutorialスマートダッシュ"),val);
				AnimationManager().Anime_MessageWait();
			}
			else if(!FreeFlags()[7] && 1200 < SumTurnCount()
				&& キャラ前地形(pPlayerChara(),1)->pOnChar != NULL)
			{
				FreeFlags()[7] = 1;
				g_Storyメッセージ(_T("tutorialラストスペル"),val);
				AnimationManager().Anime_MessageWait();
				
			}
		
		
		

			if(!FreeFlags()[8])
			{
				int a,flag = 0;
				for(a=0;a<8;a++)
				{
					if(キャラ視点方角地形(pPlayerChara(),a,1)->pOnDrop != NULL
						&& キャラ視点方角地形(pPlayerChara(),a,1)->pOnDrop->属性.count(落ち物属性::アイテム))
					{
						flag = 1;
						break;
					}
				}
				if( 3 < SumTurnCount()
					&& flag)
				{
					FreeFlags()[8] = 1;
					val[_T("Button")] = setStyle(1+g_GameEnv.m_Input.patInputManager->padconfigI2B[1]);
					g_Storyメッセージ(_T("tutorialアイテム発見"),val);
					AnimationManager().Anime_MessageWait();
				}
			}

			if(!FreeFlags()[9] && 3 < SumTurnCount()
				&& pPlayerChara()->holdItem.size() > 1)
			{
				FreeFlags()[9] = 1;
				val[_T("Button")] = setStyle(1+g_GameEnv.m_Input.patInputManager->padconfigI2B[3]);
				g_Storyメッセージ(_T("tutorialアイテム取得"),val);
				AnimationManager().Anime_MessageWait();
			}
		
		
			if(!FreeFlags()[10] && 3 < SumTurnCount()
				&& pPlayerChara()->足元() != NULL
				&& pPlayerChara()->足元()->属性.count(落ち物属性::罠))
			{
				FreeFlags()[10] = 1;
				g_Storyメッセージ(_T("tutorial罠乗り"),val);
				AnimationManager().Anime_MessageWait();
			}
		
			if(!FreeFlags()[11])
			{
				int a,flag = 0;
				for(a=0;a<pPlayerChara()->holdItem.size();a++)
				{
					if(!pPlayerChara()->holdItem[a]->大局識別済み()
						)
					{
						flag = 1;
						break;
					}
				}
				if( 400 < SumTurnCount()
					&& flag)
				{
					FreeFlags()[11] = 1;
					g_Storyメッセージ(_T("tutorialアイテム未識別"),val);
					AnimationManager().Anime_MessageWait();
				}
			}
		
		
		
			if(!FreeFlags()[12])
			{
				int a,flag = 0;
				for(a=0;a<pPlayerChara()->holdItem.size();a++)
				{
					if(pPlayerChara()->holdItem[a]->state() == cDroping::STATE_CURSE
						)
					{
						flag = 1;
						break;
					}
				}
				if( 400 < SumTurnCount()
					&& flag)
				{
					FreeFlags()[12] = 1;
					g_Storyメッセージ(_T("tutorialアイテム呪い"),val);
					AnimationManager().Anime_MessageWait();
				}
			}
		
			if(!FreeFlags()[13])
			{
				int a,flag = 0;
				for(a=0;a<pPlayerChara()->holdItem.size();a++)
				{
					if(pPlayerChara()->holdItem[a]->state() == cDroping::STATE_GOOD
						)
					{
						flag = 1;
						break;
					}
				}
				if( 400 < SumTurnCount()
					&& flag)
				{
					FreeFlags()[13] = 1;
					g_Storyメッセージ(_T("tutorialアイテム信仰"),val);
					AnimationManager().Anime_MessageWait();
				}
			}
		
		
			if(!FreeFlags()[14])
			{
				if( 3 < SumTurnCount()
					&& MessageW().Messagelogs.size() > 30)
				{
					FreeFlags()[14] = 1;
					val[_T("Button")] = setStyle(1+g_GameEnv.m_Input.patInputManager->padconfigI2B[3]);
					g_Storyメッセージ(_T("tutorialメッセージ履歴"),val);
					AnimationManager().Anime_MessageWait();
				}
			}
		
			if(!FreeFlags()[15])
			{
				int a,flag = 0;
				for(a=0;a<8;a++)
				{
					if(キャラ視点方角地形(pPlayerChara(),a,1)->pOnDrop != NULL
						&& キャラ視点方角地形(pPlayerChara(),a,1)->pOnDrop->属性.count(落ち物属性::階段))
					{
						flag = 1;
						break;
					}
				}
				if( 3 < SumTurnCount()
					&& flag)
				{
					FreeFlags()[15] = 1;
					g_Storyメッセージ(_T("tutorial階段発見"),val);
					AnimationManager().Anime_MessageWait();
					FreeFlags()[16] = 1;
					g_Storyメッセージ(_T("tutorial階段"),val);
					AnimationManager().Anime_MessageWait();

				}
			}
			
			//if(!FreeFlags()[16] && 3 < SumTurnCount()
			//	&& pPlayerChara()->足元() != NULL
			//	&& pPlayerChara()->足元()->属性.count(落ち物属性::階段))
			//{
			//	FreeFlags()[16] = 1;
			//	g_Storyメッセージ(_T("tutorial階段"),val);
			//	AnimationManager().Anime_MessageWait(true);
			//}
			//

			if(!FreeFlags()[17] && 250 < SumTurnCount()
				)
			{
				FreeFlags()[17] = 1;
				val[_T("Button")] = setStyle(1+g_GameEnv.m_Input.patInputManager->padconfigI2B[6]);
				g_Storyメッセージ(_T("tutorialマップ"),val);
				AnimationManager().Anime_MessageWait();
			}
			if(!FreeFlags()[18] && 150 < SumTurnCount()
				)
			{
				FreeFlags()[18] = 1;
				val[_T("Button")] = setStyle(1+g_GameEnv.m_Input.patInputManager->padconfigI2B[4]);
				g_Storyメッセージ(_T("tutorial斜め"),val);
				AnimationManager().Anime_MessageWait();
			}

			if(!FreeFlags()[19] && 3 < SumTurnCount() &&
				pPlayerChara()->bulletequipment != NULL)
			{
				FreeFlags()[19] = 1;
				val[_T("Button")] = setStyle(1+g_GameEnv.m_Input.patInputManager->padconfigI2B[5]);
				g_Storyメッセージ(_T("tutorial弾幕装備"),val);
				AnimationManager().Anime_MessageWait();
			}

			if(!FreeFlags()[20] && 450 < SumTurnCount())
			{
				FreeFlags()[20] = 1;
				val[_T("Button")] = setStyle(1+g_GameEnv.m_Input.patInputManager->padconfigI2B[5]);
				g_Storyメッセージ(_T("tutorial足踏み"),val);
				AnimationManager().Anime_MessageWait();
			}

			if(!FreeFlags()[21] && 30 < SumTurnCount() && 
				pPlayerChara()->HP != pPlayerChara()->MHP)
			{
				FreeFlags()[21] = 1;
				g_Storyメッセージ(_T("tutorialHP"),val);
				AnimationManager().Anime_MessageWait();
			}

			if(!FreeFlags()[22] && 30 < SumTurnCount() &&
				pPlayerChara()->Stomach < pPlayerChara()->MaxStomach/2)
			{
				FreeFlags()[22] = 1;
				g_Storyメッセージ(_T("tutorial満腹度"),val);
				AnimationManager().Anime_MessageWait();
			}


			if(!FreeFlags()[23])
			{
				int a,flag = 0;
				for(a=0;a<pPlayerChara()->holdItem.size();a++)
				{
					if(pPlayerChara()->holdItem[a]->属性.count(落ち物属性::装備品)
						)
					{
						flag = 1;
						break;
					}
				}
				if( 3 < SumTurnCount()
					&& flag)
				{
					FreeFlags()[23] = 1;
					g_Storyメッセージ(_T("tutorial装備品"),val);
					AnimationManager().Anime_MessageWait();
				}
			}

			if(!FreeFlags()[24])
			{
				int a,flag = 0;
				for(a=0;a<pPlayerChara()->holdItem.size();a++)
				{
					if(pPlayerChara()->holdItem[a]->属性.count(落ち物属性::スペカ)
						)
					{
						flag = 1;
						break;
					}
				}
				if( 3 < SumTurnCount()
					&& flag)
				{
					FreeFlags()[24] = 1;
					g_Storyメッセージ(_T("tutorialスペル"),val);
					AnimationManager().Anime_MessageWait();
				}
			}

			if(!FreeFlags()[25])
			{
				int a,flag = 0;
				for(a=0;a<pPlayerChara()->holdItem.size();a++)
				{
					if(pPlayerChara()->holdItem[a]->属性.count(落ち物属性::飲み物)
						)
					{
						flag = 1;
						break;
					}
				}
				if( 3 < SumTurnCount()
					&& flag)
				{
					FreeFlags()[25] = 1;
					g_Storyメッセージ(_T("tutorial飲み物"),val);
					AnimationManager().Anime_MessageWait();
				}
			}

			if(!FreeFlags()[26])
			{
				int a,flag = 0;
				for(a=0;a<pPlayerChara()->holdItem.size();a++)
				{
					if(pPlayerChara()->holdItem[a]->属性.count(落ち物属性::撃ち物)
						)
					{
						flag = 1;
						break;
					}
				}
				if( 3 < SumTurnCount()
					&& flag)
				{
					FreeFlags()[26] = 1;
					g_Storyメッセージ(_T("tutorial弾幕"),val);
					AnimationManager().Anime_MessageWait();
				}
			}


			if(!FreeFlags()[27])
			{
				int a,flag = 0;
				for(a=0;a<pPlayerChara()->holdItem.size();a++)
				{
					if(pPlayerChara()->holdItem[a]->属性.count(落ち物属性::読み物)
						)
					{
						flag = 1;
						break;
					}
				}
				if( 3 < SumTurnCount()
					&& flag)
				{
					FreeFlags()[27] = 1;
					g_Storyメッセージ(_T("tutorial本"),val);
					AnimationManager().Anime_MessageWait();
				}
			}

			if(!FreeFlags()[28])
			{
				int a,flag = 0;
				for(a=0;a<pPlayerChara()->holdItem.size();a++)
				{
					if(pPlayerChara()->holdItem[a]->属性.count(落ち物属性::入れ物)
						)
					{
						flag = 1;
						break;
					}
				}
				if( 3 < SumTurnCount()
					&& flag)
				{
					FreeFlags()[28] = 1;
					g_Storyメッセージ(_T("tutorial箱"),val);
					AnimationManager().Anime_MessageWait();
				}
			}

			if(!FreeFlags()[29])
			{
				if( 1000 < SumTurnCount())
				{
					FreeFlags()[29] = 1;
					g_Storyメッセージ(_T("tutorial合成"),val);
					AnimationManager().Anime_MessageWait();
				}
			}

			if(!FreeFlags()[30])
			{
				if( 1000 < SumTurnCount()
					&& !pPlayerChara()->死亡() && pPlayerChara()->HP <= pPlayerChara()->MHP/4)
				{
					FreeFlags()[30] = 1;
					g_Storyメッセージ(_T("tutorialピンチ"),val);
					AnimationManager().Anime_MessageWait();
				}
			}

			if(!FreeFlags()[31])
			{
				int a,flag = 0;
				for(a=0;a<8;a++)
				{
					if(キャラ視点方角地形(pPlayerChara(),a,1)->pOnChar != NULL
						&& isCanNeighberAttack(pPlayerChara(),pPlayerChara()->aspect+a,1,0,0))
					{
						flag++;
					}
				}
				if( 3 < SumTurnCount()
					&& flag >= 2)
				{
					FreeFlags()[31] = 1;
					g_Storyメッセージ(_T("tutorial囲まれ"),val);
					AnimationManager().Anime_MessageWait();
				}
			}

			if(!FreeFlags()[32])
			{
				if( 3 < SumTurnCount()
					&& キャラ前地形(pPlayerChara(),1)->pOnChar != NULL
						&& !isCanNeighberAttack(pPlayerChara(),pPlayerChara()->aspect,1,0,0))
				{
					FreeFlags()[32] = 1;
					g_Storyメッセージ(_T("tutorial角攻撃"),val);
					AnimationManager().Anime_MessageWait();
				}
			}
			
		}
		*/
	}


}

bool cDungeonSystem::isPlayerCharacterAccordingStartRegulation(const tstring& DungeonsID, StyleString& error)
{
	pDungeonStoryScript_LuaEnvironment pluaEnv = sg_pDungeonSystem->DataBase.DungeonRegulationScriptEnvironment(DungeonsID);
	if(pluaEnv)
	{
		bool returnValue = false;
		pluaEnv->function_returnable(_T("isPlayerCharacterAccordingStartRegulation"),returnValue, boost::ref(error));
		return returnValue;
	}
	return true;
}
bool cDungeonSystem::isRegureCharacter(const tstring& DungeonsID, pcCharacter chara)
{
	pDungeonStoryScript_LuaEnvironment pluaEnv = sg_pDungeonSystem->DataBase.DungeonRegulationScriptEnvironment(DungeonsID);
	if(pluaEnv)
	{
		bool returnValue = false;
		pluaEnv->function_returnable(_T("isRegureCharacter"), returnValue, chara);
		return returnValue;
	}
	return true;
}
StyleString cDungeonSystem::StartRegulationMassage(const tstring& DungeonsID)
{
	pDungeonStoryScript_LuaEnvironment pluaEnv = sg_pDungeonSystem->DataBase.DungeonRegulationScriptEnvironment(DungeonsID);
	if(pluaEnv)
	{
		StyleString returnValue;
		pluaEnv->function_returnable(_T("StartRegulationMassage"), returnValue);
		return returnValue;
	}
	return _T("");
}