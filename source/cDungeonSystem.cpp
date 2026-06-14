#include "stdafx.h"
#include "scriptprocesser/DungeonMakeScript.h"
#include "cDungeonSystem.h"
//#include "utility/mt19937ar.h"
#include "utility/cAltaMatterPlus.h"
#include "Gameobjects.h"
//#include "cShortCuts.h"

#include "FireCommand.h"
#include "FindUtility.h"

#include "csHomeFirst.h"

#include "cDropingDistribution.h"
#include "EffectFunctions.h"

#include "GameIdiom.h"
#include "cFloor.h"
#include "caFade.h"

#include "utility/debug.h"

#include "ceaiActive.h"
#include <time.h>

#include "cSaveStore.h"
#include "cResultWindow.h"

#include "cShortCuts.h"

#include "cHomeResultWindow.h"

#include "cEventManager.h"

double gettimeofday_sec()
{
    time_t tv;
	time(&tv);
    return tv;
}

#include "scriptProcesser/DungeonStoryScript.h"



#include "DungeonInstance.h"

#include <boost/pointer_cast.hpp>

#define TEX_ARROWS_FILENAME _T("interface\\aspectArrows.png")
#define TEX_ARROWS_WIDTH 64
#define TEX_ARROWS_HEIGHT 256


#define MessageWINDOW_CENTERX (SCREEN_X/2)
//#define MessageWINDOW_CENTERY (SCREEN_Y - (GWFONTSIZE +GWFONTOVERMARGIN+GWFONTUNDERMARGIN)*2 - EDGEWIDTH)
#define MessageWINDOW_BOTTOMY (SCREEN_Y)

pcDungeonSystem sg_pDungeonSystem;

#include "cDropingDistribution.h"



cDungeonSystem::cDungeonSystem(void)
{
	pFrameTexture = NULL;

	hiddenInterface_ = false;

	//------------------------------------------------------
	//環境変数初期値
	EV_charEdgedFlag = 1;//文字縁取りモード
}

cDungeonSystem::~cDungeonSystem(void)
{
	menuControlLayerV().clear();
	primaryMenuControlLayerV().clear();
}

double cDungeonSystem::Floortime()
{
	return gettimeofday_sec() - time_Starttime();
}


int cDungeonSystem::InitSystem(IDirect3DDevice9 *pDev)
{
	pDevice_D3D = pDev;

	DataBase.Init(pDev);


	//InitDungeon(pDev);





	pArrowsTexture = g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev,TEX_ARROWS_FILENAME);

	pFrameTexture = 
			g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev, _T("interface\\frame_default.png"));



	//オフスクリーン使う
	g_GameEnv.m_Screen->m_OffScreenEnable = false;
	g_GameEnv.m_Screen->m_LayerEnable = false;
	g_GameEnv.m_Screen->m_MiniEnable = false;
	
	g_GameEnv.m_Screen->m_LayerMode = ALPHA_MODE_NORMAL;
	g_GameEnv.m_Screen->m_LayerColor.ARGB(255,255,255,255);
	
	cColor c;
	c.ARGB(0,0,0,0);

	g_GameEnv.m_Screen->setRenderTarget(pDev, SCREEN_LAYER);
	g_GameEnv.m_Screen->Clear(pDev, c);
	g_GameEnv.m_Screen->setRenderTarget(pDev, SCREEN_OFFSCREEN);
	g_GameEnv.m_Screen->Clear(pDev, c);

	//ターゲット戻しておく
	g_GameEnv.m_Screen->setRenderTarget(pDev, SCREEN_BACKGROUND);




	pSaveData = pcSaveData(new cSaveData);
	pSaveData->Init(pDev);

	//仮
	pDungeonInstance_ = pcDungeonInstance(new cDungeonInstance);

	return true;
}
int cDungeonSystem::InitDungeon(IDirect3DDevice9 *pDev)
{

	int prehome = 0;
	//ここで拠点情報保存
	if(pDungeon() != NULL && 拠点フラグ())
	{
		prehome = 1;
	}



	pDungeonInstance_ = pcDungeonInstance(new cDungeonInstance);

	if(pSaveQuest == NULL)
	{
		pSaveQuest = pcSaveQuest(new cSaveQuest);
		pSaveQuest->Init(pDev,_T("temp"));
		pSaveQuest->floor = 1;
		pSaveQuest->randBase = time(NULL);
		pSaveQuest->DungeonID = tstring(_T("first_Home"));
		pSaveQuest->pPlayer = pcCharacter(new cPlayerChara_ID_2);//tyougoku
		pSaveQuest->pPlayer->Init(pDev,pSaveQuest->pPlayer);
		pSaveQuest->pPlayer->Forse = CHARACTER_FORSE_FRIEND;
		pSaveQuest->money = 0;
		
	}
	else
	{


	}

	//異常終了
	if(!pSaveQuest->GoodEndFlags)
	{
		pSaveQuest->BadEndNum++;
	}
	pSaveQuest->GoodEndFlags = 0;


	//sg_pDungeonSystem->setDungeonData(tstring(_T("Dungeon\\firstDungeonData.id")));
	//情報設定
	random_Seed() = pSaveQuest->randBase;
	pPlayerChara() = pSaveQuest->pPlayer;
	setDungeonData(pSaveQuest->DungeonID);//ダンジョンセット



	GameScreenInterface.money.value = pSaveQuest->money;
	FloorLevel() = pSaveQuest->floor;
	SumTurnCount() = pSaveQuest->Sumturn;
	time_SumFrameCount() = pSaveQuest->SumFrame;
	time_Sumtime() = pSaveQuest->SumFramerealtime/60.0;
	SumEnemyDefeatNum() = pSaveQuest->SumdefeatNum;
	FreeFlags() = pSaveQuest->FreeFlags;
	localFlags() = pSaveQuest->localFlags;
	privateFlags() = pSaveQuest->privateFlags;

	//ここで拠点情報復元
	if(拠点フラグ())
	{//拠点に戻ってきた


		if(pSaveData->lastpPlayer != NULL)
		{//つまり独立モードだったとき復元
			sg_pDungeonSystem->pPlayerChara() = pSaveData->lastpPlayer;
			GameScreenInterface.money.value = pSaveData->lastmoney;
		}
		if(sg_pDungeonSystem->pPlayerChara()->死亡())
		{
			sg_pDungeonSystem->pPlayerChara()->HP = sg_pDungeonSystem->pPlayerChara()->MHP;
		}


		if(pSaveData->pSaveStore()->playerExpStore > 0
			|| pSaveData->pSaveStore()->yakuStore > 0)
		{
			//経験値反映
			pcHomeResultWindow phomeResultWindow(new cHomeResultWindow);

			phomeResultWindow->Init(
				pDevice_D3D,
				pSaveData->playerLevel,
				pSaveData->playerExp,
				pSaveData->pSaveStore()->playerExpStore,
				&pSaveData->NextPlayerLevel,
				pSaveData->pSaveStore()->yaku,
				pSaveData->pSaveStore()->yakuStore
				);


			pcControlLayer pccl;
			//新規コントロールレイヤー
			primaryMenuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
			pccl->Init(pDevice_D3D);

			//新規リストウィンドウ
			pccl->WindowList.push_back(phomeResultWindow);


			pSaveData->AddPlayerLevel(pSaveData->pSaveStore()->playerExpStore);
			pSaveData->pSaveStore()->playerExpStore = 0;
			pSaveData->pSaveStore()->yaku += pSaveData->pSaveStore()->yakuStore;
			pSaveData->pSaveStore()->yakuStore = 0;

		}
	}
	else if(prehome && !独立モード())
	{//リプレイとか独立モードでないとき
		pSaveData->lastpPlayer = NULLCHARA;
		pSaveData->lastmoney = 0;
	}

	//正常終了判定in拠点
	if(拠点フラグ())
	{
		pSaveData->DidGoodEndFlagInBase = pSaveData->GoodEndFlagInBaseTemp;
	}
	pSaveData->GoodEndFlagInBaseTemp = 拠点フラグ();


	pSaveData->save();//セーブ

	DataBase.識別初期化();

	DataBase.ランダム大局識別百分率(pDungeon()->appreciationpos(), pDungeon()->appreciationposkind());

	if(pSaveQuest->DropIDtoAppreciated.size()) DataBase.DropIDtoAppreciatedinstance() = pSaveQuest->DropIDtoAppreciated;
	if(pSaveQuest->DropIDtoMEMO.size()) DataBase.DropIDtoMEMOinstance() = pSaveQuest->DropIDtoMEMO;



	GameOverFlag() = false;






	//RandomInitGen().init_genrand(378555);



	//pPlayerChara() = pcCharacter(new cPlayerChara_ID_2);//まりさ
	//pPlayerChara()->Init(pDevice_D3D,pPlayerChara());
	//pPlayerChara()->Forse = CHARACTER_FORSE_FRIEND;
	//pPlayerChara()->emotion.insert(EMOTION_KAKKO);


	/*
	int i;
	for(i=0;i<32;i++)
	{
		キャラクター生成(_T("のら毛玉"),random()*2,CHARACTER_FORSE_ENEMY);
	}

	int i;
	//デバッグ
	

	vector<int> IDlist;
	cDropingDistribution cDD;

	IDlist.clear();
	cDropingDistribution cTD;
	cTD.setItemGroupPos(0,0,0,0,0,0,0,0,1);
	DataBase.makeDropIDList(IDlist, 属性設定(罠));
	cTD.set(IDlist,1);
	for(i=0;i<15;i++)
	{
		落ち物生成(cTD.get(random()));
	}
	*/


	



	//refreshCharaDropView();
	
	//Map().EraseMiniMap(pPlayerChara()->placeX,pPlayerChara()->placeY);

	//現在地からマッピング(pPlayerChara());
	

	//resetTurnInformation(pDev);


	//デバッグ
	/*
	cDiscreteProbabilityGrouped cDPG;
	cDPG.setnewGroup(3);
	cDPG.set(0,3,11);
	cDPG.set(0,1,12);
	cDPG.set(0,1,13);
	cDPG.setnewGroup(1);
	cDPG.set(1,1,21);
	cDPG.set(1,1,22);
	cDPG.set(1,1,23);
	cDPG.setnewGroup(1);
	cDPG.set(2,3,31);
	cDPG.set(2,1,32);
	cDPG.set(2,1,33);
	int i;
	for(i=0;i<100;i++)
	{
		メッセージ(setStyle( cDPG.get( random() ) ));
	}
	*/



	InitFloor(pDev);



	if(拠点フラグ()){

		int i, size = DataBase.homeEventEnvironments().size();
		for(i=0;i<size;i++)
		{
			if(isReservedNextDungeon()) break;
			if( !DataBase.homeEventEnvironments()[i].function(_T("homeEvent")));
			{
				DataBase.homeEventEnvironments()[i].outputErrorFile(_T("Error_")+setStyle(i).conclete_tstr()+_T("_homeEvent.txt"));
			}
		}
		
	}

	
	int i, size = CharaList().size();
	for(i=0;i<size;i++)
	{
		CharaList()[i]->InitDungeonStart();
	}

	StartFloor(pDev);

	return true;
}

int cDungeonSystem::FinalizeDungeon(IDirect3DDevice9 *pDev)
{
	int i, size = CharaList().size();
	for(i=0;i<size;i++)
	{
		CharaList()[i]->FinalizeDungeonEnd();
	}

	return true;
}

int cDungeonSystem::PlayBGM()
{
	if(floorNowParty())
	{
		AnimationManager().Anime_PlayBGM( _T("party_intro.wav"),_T("party_main.wav"));
	}
	else
	{
		tstring chipset = pDungeon()->pfloor(FloorLevel())->mapchipsetname();

		AnimationManager().Anime_PlayBGM( DataBase.MapChipFormImportDataMusicFile(chipset).first.c_str(),
												DataBase.MapChipFormImportDataMusicFile(chipset).second.c_str());
	}

	return true;
}
int cDungeonSystem::resetedDevice(IDirect3DDevice9 *pDev)
{
	if(pFloorInstance_) Map().ResetMiniMap();
	return true;
}

int cDungeonSystem::InitFloor(IDirect3DDevice9 *pDev)
{
	pFloorInstance_ = pcFloorInstance(new cFloorInstance);

	int i;


	//情報記憶

	pSaveQuest->floor = FloorLevel();
	pSaveQuest->money = GameScreenInterface.money.value;
	pSaveQuest->Sumturn = SumTurnCount();
	pSaveQuest->SumFrame = time_SumFrameCount();
	pSaveQuest->SumFramerealtime = time_Sumtime()*60;
	pSaveQuest->SumdefeatNum =  SumEnemyDefeatNum();
	pSaveQuest->FreeFlags =  FreeFlags();
	pSaveQuest->localFlags = localFlags();
	pSaveQuest->privateFlags = privateFlags();
	pSaveQuest->DropIDtoAppreciated = DataBase.DropIDtoAppreciatedinstance();
	pSaveQuest->DropIDtoMEMO = DataBase.DropIDtoMEMOinstance();
	pSaveQuest->Version_ = g_VersionString();
	if(!拠点フラグ())
	{//拠点じゃない
		if(FloorLevel() == 1)
		{//最初
			pSaveQuest->pFirstConditionOfPlayer = DataBase.NewCopyCharacter(pPlayerChara());
			pSaveQuest->FirstConditionOfMoney = 主人公お金所持量();
		}
		pSaveQuest->save();
		pSaveData->save();//これしないと落ちたとき復帰できない
	}


/*
	vector<unsigned long> init_key;
	for(i=0;i<8;i++)
	{
		init_key.push_back(RandomInitGen().genrand_int32());
	}
	
	pDungeon()->randomgen().init_by_array(&(*init_key.begin()),8);
*/
	pDungeon()->randomgen().init_genrand(pFloor()->floorrandomSeed);
	srand(pFloor()->floorrandomSeed);

	Map().Init(pDev);
	//仮
	Map().setVisibleForcusX(25);
	Map().setVisibleForcusY(25);



	MessageW().Init(pDev,28,4);
	MessageW().CenterX = MessageWINDOW_CENTERX;
	//Message.CenterY = MessageWINDOW_CENTERY;
	MessageW().setBottom(MessageWINDOW_BOTTOMY);



	Map().Renew(pDevice_D3D);



	if(pDungeon() != NULL)
	{
		//std::basic_ofstream<TCHAR> ofs(_T("debug.txt"), ios::app);
		//ofs << _T("before makeNewDungeon") << endl<TCHAR>;/**/
		Map().makeNewDungeon(pFloor());
		tstring chipset = pDungeon()->pfloor(FloorLevel())->mapchipsetname();
		//ofs << _T("before setChipset") << endl<TCHAR>;/**/
		Map().setChipset(chipset);
		//ofs << _T("before DecorationAll") << endl<TCHAR>;/**/
		Map().DecorationAll(chipset);
		//ofs << _T("after DecorationAll") << endl<TCHAR>;/**/
		/*
		if(FloorLevel() == 1)
		{
			g_GameEnv.m_SoundManager.stopBGM();
			g_GameEnv.m_SoundManager.getplayBGMFromFile( DataBase.MapChipFormImportDataMusicFile(chipset).c_str(),NULL);
			
		}
		else if(chipset != pDungeon()->pfloor(FloorLevel()-1)->mapchipsetname())
		{
			g_GameEnv.m_SoundManager.stopBGM();
			g_GameEnv.m_SoundManager.getplayBGMFromFile( DataBase.MapChipFormImportDataMusicFile(chipset).c_str(),NULL);
			
		}
		*/

	}

	AnimationManager().Init(pDevice_D3D);
	AnimationManager().Anime_primaryInterfaceWait();


	if(pDungeon() != NULL)
	{
		PlayBGM();
	}
	

	if(FloorLevel() == 1)
	{
	
		//初期所持アイテム
	
		for(i=0;i<pDungeon()->firstItems().size();i++)
		{
			if(pDungeon()->firstItems()[i].empty()) continue;
			pcDroping pdrop = 落ち物生成_設置なし(pDungeon()->firstItems()[i][0]);
			if(pdrop == NULL) continue;
			pdrop->state() = cDroping::STATE_NORMAL;
			pdrop->quality() = 0;

			遠隔拾得要請_持ち物余白無考慮(pPlayerChara(),pdrop);
		}

	}


	count_minimap() = 0;
	cannotinput() = false;
	aspectTurnprocessswitch() = false;
	DashSwitch() = 0;
	dashflag() = 0;
	count_minimap() = 0;
	isInputWaitingflag() = false;
	cannotinput() = false;


	Resetminimap(pDevice_D3D);

	CutInM().clearMemCutIn();


	GameScreenInterface.FloorLevel = FloorLevel();
	

	//主人公初期設置
	do
	{
		if(pDungeon()->pfloor(FloorLevel())->friendfirstsets().size() > 0)
		{
			const vector<int>& vint = pDungeon()->pfloor(FloorLevel())->friendfirstsets()[0];
			if(vint.size() == 2)
			{
				pcLandform pland = Map().Land(vint[0] + EDGELANDWIDTH, vint[1] + EDGELANDWIDTH);
				初期設置(pPlayerChara(),pland);
				break;
			}
			
		}

		初期設置(pPlayerChara(),FindUtility::各部屋等確率_ランダム地形検索_店回避_配置安全());
		
	}while(0);



	arrangementObject(pDevice_D3D);
	
	refreshCharaDropView();

	//リストインサート
	insertDroplist(pPlayerChara()->holdItem,(FloorLevel() == 1));
	if(拠点フラグ())
	{
		//倉庫
		int i,size = g_pSaveData()->pSaveStore()->storeItem.size();
		for(i=0;i<size;i++)
		{
			insertDroplist(g_pSaveData()->pSaveStore()->storeItem[i]);
		}

		//厄システム
		insertDroplist(g_pSaveData()->pSaveStore()->hinaItem);
		insertDroplist(g_pSaveData()->pSaveStore()->hinaKeepItem);

		//霧雨魔法店
		insertDroplist(g_pSaveData()->pSaveStore()->marisaShopItem);
		
		size = g_pSaveData()->pSaveStore()->marisaShopItem.size();
		for(i=0;i<size;i++)
		{//フラグ外れるので付け直し
			アイテム店フラグ(g_pSaveData()->pSaveStore()->marisaShopItem[i],cDroping::SHOPFLAG_SHOP);
			アイテム割引フラグ(g_pSaveData()->pSaveStore()->marisaShopItem[i], cDroping::SHOPDISCOUNTFLAG_NONE);
		}

	}

	//店資金
	if(FloorLevel() == 1)
	{
		pSaveQuest->ShopFund = pFloor()->ShopFirstFund();
	}
	else
	{
		pSaveQuest->ShopFund = pSaveQuest->ShopFund * pFloor()->ShopPerProfit() + pFloor()->ShopConstProfit();
	}
	//---------------------------------------------------------------
	//初期設置
	//---------------------------------------------------------------



	vector<int> IDlist;
	cDropingDistribution cDD;

	/*
	DataBase.makeDropIDList(IDlist, 属性設定(アイテム));
	cDD.set(IDlist,1);
	for(i=0;i<400;i++)
	{
		落ち物生成(cDD.get(random()));
	}
*/


	int roomnum = Map().RoomList.size();
	int landnum = 0;
	for(i=0;i<roomnum;i++) landnum += Map().RoomList[i]->LandformList.size();
	int trapnum = pDungeon()->pfloor(FloorLevel())->trapfirstsetnum(roomnum,landnum);
	int enemynum = pDungeon()->pfloor(FloorLevel())->enemyfirstsetnum(roomnum,landnum);
	int itemnum = pDungeon()->pfloor(FloorLevel())->itemfirstsetnum(roomnum,landnum);

	//アイテム初期設置固定
	int size = pDungeon()->pfloor(FloorLevel())->itemfirstsets().size();
	for(i=0;i<size;i++)
	{
		const vector<int> & vec = pDungeon()->pfloor(FloorLevel())->itemfirstsets()[i];
		if(vec.size() != 3)
		{
			OnAssert(_T("ドロップアイテム基礎>初期設置"),i,FALSE,_T("形式が間違っています。 form error"));
			continue;
		}
		
		pcLandform pland;
		if(vec[1] == -1) pland = RandomPlace();
		else if(vec[1] == -2) pland = FindUtility::各部屋等確率_ランダム地形検索_店回避_配置安全();
		else pland = Map().Land(vec[1] + EDGELANDWIDTH, vec[2] + EDGELANDWIDTH);
		
		落ち物生成(vec[0],NULL,pland,true);
	}

	//敵初期設置固定
	size = pDungeon()->pfloor(FloorLevel())->charafirstsets().size();
	for(i=0;i<size;i++)
	{
		const vector<int> & vec = pDungeon()->pfloor(FloorLevel())->charafirstsets()[i];
		if(vec.size() != 5)
		{
			OnAssert(_T("出現敵>初期設置"),i,FALSE,_T("形式が間違っています。 form error"));
			continue;
		}

		pcLandform pland;
		if(vec[1] == -1) pland = RandomPlace();
		else if(vec[1] == -2) pland = FindUtility::各部屋等確率_ランダム地形検索_店回避_配置安全();
		else pland = Map().Land(vec[1] + EDGELANDWIDTH, vec[2] + EDGELANDWIDTH);
		
		キャラクター生成_自然湧き(vec[0],vec[3],vec[4],pland);
	}

	//罠初期設置ランダム
	for(i=0;i<trapnum;i++)
	{
		int vint[3];
		/*
		unsigned int trapIDandEtc = pDungeon()->pfloor(FloorLevel())->trapDP().get(random());
		
		vint[0] = trapIDandEtc>>16;
		vint[1] = (trapIDandEtc>>8)&0xFF;
		vint[2] = (trapIDandEtc)&0xFF;
		*/

		罠自然湧きID(vint);
		落ち物生成(vint[0],NULL,NULLLAND,true);

	}

	//敵初期設置ランダム
	for(i=0;i<enemynum;i++)
	{
		int vint[3];
		敵自然湧きID(vint);

		if(sg_pDungeonSystem->CharaList().size() < pDungeon()->pfloor(FloorLevel())->Maxenemynum())
		{
			pcCharacter pcmob = キャラクター生成_自然湧き(vint[0], vint[1], CHARACTER_FORSE_ENEMY, FindUtility::各部屋等確率_ランダム地形検索_配置安全());
		}
	}



	//アイテム初期設置ランダム
	for(i=0;i<itemnum;i++)
	{
		int vint[3];
		アイテム自然湧きID(vint);
		落ち物生成(vint[0],NULL,FindUtility::各部屋等確率_ランダム地形検索_店回避_配置安全(),true);
	}


	//宴会と店
	for(i=0;i<Map().RoomList.size();i++)
	{
		if(Map().RoomList[i]->PartyFlag == 1)
		{
			宴会生成(i);
		}
		if(Map().RoomList[i]->ShopFlag == 1)
		{
			店生成(i);
		}
	}



	GameScreenInterface.Init(pDevice_D3D, boost::dynamic_pointer_cast<cPlayerChara>(pPlayerChara()));
	
	Map().EraseMiniMap(pPlayerChara()->placeX,pPlayerChara()->placeY);

	time_Starttime() = gettimeofday_sec();

	cCondition clearCondition;
	pPlayerChara()->Condition = clearCondition;
	pPlayerChara()->Condition.init(pPlayerChara());


	sg_pDungeonSystem->Map().AllMapvisibleReset();


	{
		cValiableField val;
		val.charas.dim(_T("player")) = pPlayerChara();
		const std::vector<tstring>& contentName = pDungeon()->pfloor(FloorLevel())->extraLuaScriptKeys();
		int i, size = contentName.size();
		for(i=0;i<size;i++)
		{
			DungeonMakeScript_ExtraSetting(pDungeon()->pfloor(FloorLevel())->extraLuaScript(), contentName[i], val);
		}
	}


	宴会発覚(pPlayerChara()->足元地形()->RoomIndex);
	店入出判定(pPlayerChara()->足元地形()->RoomIndex);
	

	if(pSaveQuest->GoodEndFlags)
	{//終了したい
		//sg_pDungeonSystem->pSaveQuest = pcSaveQuest((cSaveQuest*)NULL);
		//g_GameEnv.m_SceneManage->SceneChange(pDev,new csHomeFirst);
		vector<pcDroping> vdrop = pPlayerChara()->holdItem;
		for(i=0;i<vdrop.size();i++)
		{
			removedroping(vdrop[i],true);
		}

		GameScreenInterface.money.value = 0;

		sg_pDungeonSystem->GotoDungeon(_T("first_Home"));
		sg_pDungeonSystem->メニューを閉じる();
	}

	if(!拠点フラグ())
	{
		CutInM().CutInForAllChara(フロア開始_タイミング);
	
	}

	pcaFade panime = pcaFade(new caFade);

	panime->Init(caFade::ANIME_FADE_IN);
	AnimationManager().AddAnime_realtime(panime);

	

	if(拠点フラグ() && FloorLevel() == 1)
	{//拠点に戻ってきた
		GameBaseStart();

	}
	
	pPlayerChara()->Condition.refresh_emotion();

	{

		int i, size = pDungeon()->storyScriptEnvironments().size();
		for(i=0;i<size;i++)
		{
			if( !pDungeon()->storyScriptEnvironments()[i].function(_T("floorInitProcess")) );
			{
				pDungeon()->storyScriptEnvironments()[i].outputErrorFile(_T("Error_")+pDungeon()->DungeonID_+_T("_floorInitProcess.txt"));
			}//DungeonStoryScript_ExtraSetting(psr,keys[i],_T("floorExtraSetting"),valiable);
		}
		
	}

	return true;
}

int cDungeonSystem::StartFloor(IDirect3DDevice9 *pDev)
{

	現在地からマッピング(pPlayerChara());
	resetTurnInformation(pDevice_D3D);

	return true;
}

int cDungeonSystem::cleanfloor()
{
	メニューを閉じる();
	CutInM().clearMemCutIn();
	//allremovefloorcharacter();
	//allremovefloorDroping();

	int i,k,size;

	//店資金
	size = floorshopitemlist().size();
	for(i=0;i<size;i++)
	{
		if(floorshopitemlist()[i]->shopitemflag() == cDroping::SHOPFLAG_SHOP
			&& !(floorshopitemlist()[i]->broken))
		{
			floorshopprofit() += (floorshopitemlist()[i]->単体買値(false) + floorshopitemlist()[i]->単体売値())/2;
		}
	}
	floorshopprofit() += floorshopownermoney();
	pSaveQuest->ShopFund += min( floorshopprofit(), pSaveQuest->ShopFund);//元の資金より多くの利益は得られない

	//仲間
	set<pcCharacter> FriendSet;
	size = pSaveQuest->pFriend.size();
	FriendSet.insert(pPlayerChara());
	for(i=0;i<size;i++)
	{
		FriendSet.insert(pSaveQuest->pFriend[i]);
	}

	//仲間状態リセット
	set<pcCharacter>::iterator friendSetItr = FriendSet.begin();
	for(; FriendSet.end()!=friendSetItr; friendSetItr++)
	{
		精神異常治療要請(*friendSetItr, false);
		身体異常治療要請(*friendSetItr, false);
		呪術異常治療要請(*friendSetItr, false);
		(*friendSetItr)->emotion.clear();
		(*friendSetItr)->visibleemotion.clear();
	}

	//フロア状態リセット
	フロア明かり削除();
	フロア飛ばず削除();
	フロア時間停止削除();
	フロア時間飛ばし削除();

	//厄システム
	size = CharaList().size();
	vector<pcDroping> vpdrop;
	for(i=0;i<size;i++)
	{
		if(FriendSet.count(CharaList()[i])) continue;//フレンドリストにある
		vpdrop = CharaList()[i]->holdItem;
		for(k=0;k<vpdrop.size();k++)
		{
			removedroping(vpdrop[k],true);
		}
	}
	vpdrop = DropList();
	size = vpdrop.size();
	for(i=0;i<size;i++)
	{
		if(vpdrop[i]->Holder() != NULL) continue;//誰かが持ってる
		if(vpdrop[i]->倉庫の中()) continue;

		removedroping(vpdrop[i],true);
	}

	return true;
}
int cDungeonSystem::resetfloor()
{
	/*
	CharaList().clear();
	visibleCharaList().clear();

	
	DropList().clear();
	visibleDropList().clear();
*/

	cleanfloor();
	InitFloor(pDevice_D3D);

	StartFloor(pDevice_D3D);

	insertDroplist(pPlayerChara()->holdItem);



/*
	if(!拠点フラグ())
	{
		CutInM().CutInForAllChara(フロア開始_タイミング);
	
	}
	*/
	暗闇更新();

	return true;
}

const int cDungeonSystem::拠点フラグ()
{
	if(pDungeon() == NULL) return 0;
	return pDungeon()->homeFlag();
}
const int cDungeonSystem::独立モード()
{
	return false;
}

int cDungeonSystem::gameTurnprocess(IDirect3DDevice9 *pDev)
{
/*
	static int s_phase() = GAME_PHASE_INPUT;

	static int s_turn_speed() = GAME_TURN_SPEED_TRIPLE;
*/

	while(1)
	{
		int result = 0;
		int havemove = 0;

		//主人公フェイズ
		if(s_phase() == GAME_PHASE_INPUT)
		{//インプットフェイズ
			

			bool canmove = true;
			if(s_turn_speed() == GAME_TURN_SPEED_TRIPLE)
			{
				canmove = pPlayerChara()->Condition.三倍速である();
			}
			else if(s_turn_speed() == GAME_TURN_SPEED_DOUBLE)
			{
				canmove = pPlayerChara()->Condition.二倍速以上である();
			}
			else if(s_turn_speed() == GAME_TURN_SPEED_SINGLE)
			{
				canmove = !pPlayerChara()->Condition.鈍足で行動不能である();
			}



			if(canmove)
			{


				if(!AnimationManager().Enpty || havemove)
				{//アニメーションが残ってる || 初回?
					AnimationManager().StrongInvocation(pDev);
		

		
					return false;
				}
				else if(!turnEnd_flag())
				{

					/*
					//全キャラターンエンド解除
					int i;
					for(i=0;i<CharaList().size();i++)
					{
						CharaList()[i]->TurnEndFlag = 0;
						CharaList()[i]->TurnEndFlag_half = 0;
					}
					*/

					if(!Inputprocess(pDev))//input
					{//インプットされた
						ターンエンド();
					}
					else
					{
						//アイドリング
						pPlayerChara()->visibleemotion = pPlayerChara()->emotion;
						Map().setVisibleForcusX(pPlayerChara()->visibleplace.x);
						Map().setVisibleForcusY(pPlayerChara()->visibleplace.y);
					}
				}


				if(turnEnd_flag())
				{//ターンエンド
					turnEnd_flag() = false;
					s_AltaInputprocess().process(false);
					havemove = true;



					CutInM().CutIn(pPlayerChara(),アクション終了_タイミング);
					CutInM().CutIn(pPlayerChara()->足元(),アクション終了_タイミング);
					
					if(フロア時間飛ばし状態())
					{
						floortimeskipswitch()--;
						AnimationManager().Anime_Wait(pPlayerChara()->placeX, pPlayerChara()->placeY, 2);
						ターンエンド();
						全キャラ強制ターンエンド要請();
					}
				}
				else
				{
					havemove = false;
				}
			}
			else
			{//動けない
				havemove = true;
			}

			if(havemove)
			{//自分のターンオワリ


				if(フロア時間停止状態())
				{
					floortimestopswitch()--;
					if(!フロア時間停止状態())
					{
						map<tstring, StyleString> valiable;
						g_Langメッセージ(_T("フロア時間停止解除メッセージ"),valiable);
					}
					s_turn_speed() = GAME_TURN_SPEED_SINGLE;
				}
				else
				{


					CutInM().clearMemCutIn();//カットインリストクリア


					s_phase() = GAME_PHASE_ENEMY;
					if(AnimationManager().WeekInvocation(pDev)) return false;//strongが起こったらtrueが返ってくる
				}
			}
			

		}
		else if(s_phase() == GAME_PHASE_ENEMY)
		{//敵フェイズ


			if(s_turn_speed() == GAME_TURN_SPEED_TRIPLE)
			{
				s_turn_speed() = GAME_TURN_SPEED_DOUBLE;
			}
			else if(s_turn_speed() == GAME_TURN_SPEED_DOUBLE)
			{
				s_turn_speed() = GAME_TURN_SPEED_SINGLE;
			}
			else if(s_turn_speed() == GAME_TURN_SPEED_SINGLE)
			{
				s_turn_speed() = GAME_TURN_SPEED_TRIPLE;
			}

			vector<pcCharacter> moveL,attackL;
			//pcMob mob;
			pcCharacter pc;
			int i;
			
			vector<pcCharacter> tempCharaList = CharaList();
			//スケジュール
			for(i=0;i<tempCharaList.size();i++)
			{
				bool canmove = true;


				pc = tempCharaList[i];


				if(pPlayerChara() != pc)
				{//pcはプレイヤーではなかった

					//状態異常更新
					if(s_turn_speed() == GAME_TURN_SPEED_SINGLE)
					{
						pc->Condition.process();
					}

					if(s_turn_speed() == GAME_TURN_SPEED_TRIPLE)
					{
						canmove = pc->Condition.三倍速である();
					}
					else if(s_turn_speed() == GAME_TURN_SPEED_DOUBLE)
					{
						canmove = pc->Condition.二倍速以上である();
					}
					else if(s_turn_speed() == GAME_TURN_SPEED_SINGLE)
					{
						canmove = !pc->Condition.鈍足で行動不能である();

						
					}

					canmove &= !pc->Condition.行動不能である();


					if(canmove)
					{

						pcEnemyAI ai = pc->pAI;
						int req;
						req = ai->Request_Base(s_turn_speed());
						if(req == AI_REQUEST_MOVE)
						{
							moveL.push_back(pc);
						}
						else if(req == AI_REQUEST_ATTACK)
						{
							attackL.push_back(pc);
						}
					}
				}

			}

			for(i=0;i<moveL.size();i++)
			{//動く
				if(moveL[i]->死亡() || moveL[i]->TurnEndFlag || moveL[i]->Condition.行動不能である()) continue;

				pcEnemyAI ai = moveL[i]->pAI;
				if(!ai->Move_Base())
				{//動けんかった
					attackL.push_back(moveL[i]);
				}
				CutInM().CutIn(moveL[i],アクション終了_タイミング);
				CutInM().CutIn(moveL[i]->足元(),アクション終了_タイミング);

				if(moveL[i]->TurnEndFlag_half)
				{
					moveL[i]->TurnEndFlag = 1;
				}
			}


			CutInM().CutInForAllObject(移動フェイズ終了_タイミング);
			


			for(i=0;i<attackL.size();i++)
			{//攻撃
				if(attackL[i]->死亡() || attackL[i]->TurnEndFlag || attackL[i]->Condition.行動不能である()) continue;

				pcEnemyAI ai = attackL[i]->pAI;
				ai->Attack_Base();
				CutInM().CutIn(attackL[i],アクション終了_タイミング);
				CutInM().CutIn(attackL[i]->足元(),アクション終了_タイミング);

			}

			CutInM().CutInForAllObject(攻撃フェイズ終了_タイミング);

			CutInM().CutInForAllObject(セクション終了_タイミング);
			


			//ひるみ処理
			for(i=0;i<CharaList().size();i++)
			{
				if(CharaList()[i]->TurnEndFlag_half) CharaList()[i]->TurnEndFlag = 1;
			}

			//次が主人公の番だったときの処理
			if(s_turn_speed() == GAME_TURN_SPEED_SINGLE)
			{//主人公のコンディション更新
				pPlayerChara()->Condition.process();
			}



			pPlayerChara()->Condition.refresh_emotion();
			pPlayerChara()->Condition.refresh_emotion_anime();
		
			
			if(s_turn_speed() == GAME_TURN_SPEED_SINGLE)
			{//次シングルのとき
				for(i=0;i<CharaList().size();i++)
				{
					pc = CharaList()[i];
					if(pc != pPlayerChara())
					{//敵のコンディション更新
						
						pc->Condition.refresh_predict_emotion();
						pc->Condition.refresh_emotion_anime();

						pc->TurnEndFlag = 0;
						pc->TurnEndFlag_half = 0;
					}
				}
			}
			else
			{
				for(i=0;i<CharaList().size();i++)
				{
					pc = CharaList()[i];
					if(pc != pPlayerChara())
					{//敵のコンディション更新
						
						pc->Condition.refresh_emotion();
						pc->Condition.refresh_emotion_anime();
					}
				}
			}

			if(s_turn_speed() == GAME_TURN_SPEED_SINGLE)
			{//ターンのさいご?
				Turnprocess(pDev);
				CutInM().CutInForAllObject(ターン終了_タイミング);


			}

			if(s_turn_speed() == GAME_TURN_SPEED_SINGLE)
			{//次シングルのとき
				CutInM().CutInForAllObject(主人公ターン開始直前_タイミング);
			}

			s_phase() = GAME_PHASE_INPUT;
			result = AnimationManager().WeekInvocation(pDev);

			//return false;
		}

		//インプットされてない。アニメをやることがある。
		if(result) return false;
		else if(s_AltaInputprocess().on) return true;
	}





	return true;
}

static const int shootAndShortCut_startDrawWindowFrame = 15;

int cDungeonSystem::Draw(IDirect3DDevice9 *pDev)
{
	int i;

	LandDraw(pDev);

	Map().DrawDecoLand(pDev);

	EventDrawUnderObject();

	DropingDraw(pDev);

	CharacterDraw(pDev);

	Map().DrawDecoSky(pDev);

	Map().DrawDark(pDev,pPlayerChara());


	if(!s_AltaofminimapViewprocess().on)
	{
		MiniMapDraw(pDev);


		AnimationManager().Draw(pDev);

		//リアルタイムアニメ
		AnimationManager().Draw_parallel(pDev);

		EventDraw();
	}
	else
	{
	
		AnimationManager().Draw(pDev);

		//リアルタイムアニメ
		AnimationManager().Draw_parallel(pDev);


		EventDraw();

		MiniMapDraw(pDev);
	
	}

	FrameDraw(pDev);

	if(!hiddenInterface_)
	{
		GameScreenInterface.Draw(pDev);
	}

	if(!s_AltaofminimapViewprocess().on)
	{
		for(i=0;i<menuControlLayerV().size();i++)
		{
			menuControlLayerV()[i]->Draw(pDev);
		}
	}
		
	if(menuControlLayerV().empty()) MessageW().Draw(pDev);

	for(i=0;i<primaryMenuControlLayerV().size();i++)
	{
		primaryMenuControlLayerV()[i]->Draw(pDev);
	}

	
	if(s_shootAndShortCutswitch() > shootAndShortCut_startDrawWindowFrame)
	{
		eShortCuts_index aspect;
		if(s_ShortCutSelectSwitch() == -1)
		{
			aspect = eShortCuts_index_NoIndex;
		}
		else
		{
			aspect = (eShortCuts_index)s_ShortCutSelectSwitch();
		}
		pSaveData->ShortCutsManager->drawWindow(aspect);
	}
	

	return true;
}

#define GAME_PROCESSMODE_ANIME (0)
#define GAME_PROCESSMODE_INPUT (1)
#define GAME_PROCESSMODE_MENU (2)
#define GAME_PROCESSMODE_MINIMAP (3)

//trueを返した時入力待ち状態
int cDungeonSystem::process(IDirect3DDevice9 *pDev)
{
	int i;
	//常に行う
	if(nextDungeon != _T(""))
	{
		resetDungeon(nextDungeon);

	}

	Map().process();


	isInputWaitingflag() = false;

	time_FrameCount()++;

	//入力補助
	//if(!g_pPlayerInput()->attack().on) s_AttackDelay() = 1;
	

	count_minimap()++;
	if(count_minimap() >= 60) count_minimap() = 0;


	if(!フロア時間停止状態())
	{
		for(i = 0;i<visibleCharaList().size();i++)
		{	//足踏み
			visibleCharaList()[i]->Process(pDev);
		}
	}
	else
	{
		pPlayerChara()->Process(pDev);
	}

	for(i = 0;i<visibleDropList().size();i++)
	{	
		visibleDropList()[i]->Process(pDev);
	}

	if(menuControlLayerV().empty()) MessageW().process(pDev);
	
	//if(!フロア時間停止状態())
	{
		//リアルタイムアニメ
		AnimationManager().process_parallel(pDev);
	}

	EventManager().Process();
	
	//最も新しいものだけに処理を行う
	primaryMenuControlLayerV().process(pDev);


	//場合によって行うかどうかきまる
/*
	for(;;)
	{
		if(Processmode() == GAME_PROCESSMODE_ANIME)
		{
			//trueを返した時プロセス無し
			if(AnimationManager().process(pDev))
			{//やることない
				//次に飛ばす
				Processmode() = GAME_PROCESSMODE_MINIMAP;
			}
			else
			{//アニメ中
				
				isInputWaitingflag() = false;
				return isInputWaitingflag();
			}
		}
		else if(Processmode() == GAME_PROCESSMODE_MINIMAP)
		{
			if((menuControlLayerV().Last() != NULL && !(menuControlLayerV().Last()->InputFreeFlag))
				|| menuControlLayerV().Last() == NULL )
			{
				
				//trueを返した時処理無し
				if(minimapViewprocess(pDev))
				{//やることない
					//次に飛ばす
					Processmode() = GAME_PROCESSMODE_MENU;
				}
				else
				{
					isInputWaitingflag() = true;
					return isInputWaitingflag();
				}
			}
		}
		else if(Processmode() == GAME_PROCESSMODE_MENU)
		{
			//trueを返した時処理無し
			if(menuCommandprocess(pDev))
			{//やることない
				//次に飛ばす
				Processmode() = GAME_PROCESSMODE_INPUT;
			}
			else
			{
				isInputWaitingflag() = true;
				return isInputWaitingflag();
			}
		}
		else if(Processmode() == GAME_PROCESSMODE_INPUT)
		{
			//trueを返した時処理無し
			if(gameTurnprocess(pDev))
			{//やることない
				//次に飛ばす
				Processmode() = GAME_PROCESSMODE_ANIME;
				isInputWaitingflag() = true;
				return isInputWaitingflag();
			}
			else
			{
				isInputWaitingflag() = true;
				return isInputWaitingflag();
			}
		}


		
	}
	isInputWaitingflag() = true;
	return isInputWaitingflag();

	*/
	int flag = false;
	for(i=0;i<10000;i++)//無限ループ防止
	{
		if(AnimationManager().process(pDev));
		else return false;

		if(!DramaManager().emptyDrama())
		{
			DramaManager().doDrama();
			AnimationManager().StrongInvocation(pDev);
			return false;
		}
		


		if((menuControlLayerV().Last() != NULL && !(menuControlLayerV().Last()->InputFreeFlag))
			|| menuControlLayerV().Last() == NULL )
		{
			if(minimapViewprocess(pDev));
			else
			{
				flag = true;
				break;
			}
		}

		if(menuCommandprocess(pDev));
		else
		{
			if(turnEnd_flag())
			{//アイテム使用
				CutInM().CutIn(pPlayerChara(),アイテム使用直後_タイミング);
			}
			flag = true;
			break;
		}


		if(GameOverFlag())
		{
			GameOverprocess();
			flag = true;
			break;
		}

		if(floormoveswitch())
		{
			movefloor();
			flag = true;
			break;
		}

		//Inputはこの中
		if(gameTurnprocess(pDev))
		{
			flag = true;
			break;
		}
		

		//以降入力待ち状態
	}

	if(flag)
	{
		//毎回初期化
		CutInM().clearMemCutIn();
		
		if(pSaveQuest != NULL) Dungeonprocess(pDungeon()->DungeonID_);	

	}


	OnAssert(_T(__FILE__),__LINE__,(i!=10000),_T("異常に大きなループが検出されました。無限ループの可能性があります。<error> Illegality Large loop"));

	return flag;

	/*
	if(AnimationManager().process(pDev));
	else return false;


	if((menuControlLayerV().Last() != NULL && !(menuControlLayerV().Last()->InputFreeFlag))
		|| menuControlLayerV().Last() == NULL )
	{
		if(minimapViewprocess(pDev));
		else return false;
	}

	if(menuCommandprocess(pDev));
	else return false;





	//Inputはこの中
	if(gameTurnprocess(pDev));
	else return false;

	//以降入力待ち状態


	return true;
	*/
}
void cDungeonSystem::GameOverpreprocess()
{

	/*
		栞を消すところにも類似の処理があるので注意されたし

		注意されたし!
	*/

	int i,k,size;
	//厄システム
	set<pcCharacter> FriendSet;
	size = pSaveQuest->pFriend.size();
	FriendSet.insert(pPlayerChara());
	for(i=0;i<size;i++)
	{
		FriendSet.insert(pSaveQuest->pFriend[i]);
	}
	size = CharaList().size();
	for(i=0;i<size;i++)
	{
		if(FriendSet.count(CharaList()[i])) continue;//フレンドリストにある
		
		for(k=0;k<CharaList()[i]->holdItem.size();k++)
		{
			厄システム挿入要請(pSaveQuest, CharaList()[i]->holdItem[k]);
		}
	}
	vector<pcDroping> vpdrop = DropList();
	size = vpdrop.size();
	for(i=0;i<size;i++)
	{
		if(vpdrop[i]->Holder() != NULL) continue;//誰かが持ってる
		if(vpdrop[i]->倉庫の中()) continue;

		厄システム挿入要請(pSaveQuest, vpdrop[i]);
	}


	if(!GameClearFlag())
	{//ゲームオーバー
		vector<pcDroping> vdrop = pPlayerChara()->holdItem;
		厄システム挿入要請(pSaveQuest, vdrop);
	}
	//もしリプレイでなかったら
	if(!独立モード())
	{
		//厄システム
		//pSaveData->pSaveStore()->yaku += 厄ポイント冒険評価();
		pSaveData->pSaveStore()->yakuStore += 厄ポイント冒険評価(GameClearFlag(), 拠点フラグ(),pSaveQuest,pPlayerChara());
		
		pSaveData->pSaveStore()->hinaItem.insert(pSaveData->pSaveStore()->hinaItem.end(),
			pSaveQuest->hinaPickItem.begin(),pSaveQuest->hinaPickItem.end());

		pSaveQuest->hinaPickItem.clear();

		if(!拠点フラグ())
		{
			//プレイヤー経験値
			double exp = pDungeon()->culcuscore()/20 + 厄ポイント冒険評価(GameClearFlag(), 拠点フラグ(),pSaveQuest,pPlayerChara());
			pSaveData->pSaveStore()->playerExpStore += exp * pDungeon()->playerExpPower() /2;//体験版補正
			//ノーマルプレイ一回の大よその経験値 = 1000
		}
	}

	GameEndSavetyPrepareing();
	pSaveQuest->setUnEnable();
	pSaveQuest->save();

}
void cDungeonSystem::GameOverprocess()
{
	if(GameOverFlag() == 1)
	{
	
		if(!pDungeon()->clearResultSkipFlag())
		{
			MakeResultWindow(GameClearFlag());
		}
		GameOverFlag() = 2;
		AnimationManager().clear();
	}
	if(GameOverFlag() == 2 && menuControlLayerV().empty())
	{
		//仮
		int i;
		if(!GameClearFlag())
		{//ゲームオーバー
			vector<pcDroping> vdrop = pPlayerChara()->holdItem;
			for(i=0;i<vdrop.size();i++)
			{
				removedroping(vdrop[i],true);
			}

			GameScreenInterface.money.value = 0;
		}
		cleanfloor();


		pSaveData->save();//セーブ

		sg_pDungeonSystem->pSaveQuest = pcSaveQuest((cSaveQuest*)NULL);
		//g_GameEnv.m_SceneManage->SceneChange(pDevice_D3D,new csHomeFirst);
		//sg_pDungeonSystem->GotoDungeon(_T("first_Home"));
		sg_pDungeonSystem->メニューを閉じる();
		sg_pDungeonSystem->AnimationManager().AddAnime_GotoDungeon(_T("first_Home"));
		pPlayerChara()->HP = pPlayerChara()->MHP;


		GameOverFlag() = 3;
	}
}

bool cDungeonSystem::isInputWaiting()
{
	return isInputWaitingflag();
}


//trueを返した時処理無し
int cDungeonSystem::Inputprocess(IDirect3DDevice9 *pDev)
{

	if(pPlayerChara()->Condition.行動不能である())
	{//インプット不能フラグ
		s_cannotInputWaitCount()++;
		if(s_cannotInputWaitCount() < 16)
		{
			s_AltaInputprocess().process(true);
			return s_AltaInputprocess().on;
		}
		else
		{
			s_cannotInputWaitCount() = 0;
			s_AltaInputprocess().process(false);
			return s_AltaInputprocess().on;
		}
	}

	if(pPlayerChara()->Condition.臆病状態() || pPlayerChara()->Condition.狂乱状態())
	{
		if(pPlayerChara()->pAI->Request_Base(s_turn_speed()) == AI_REQUEST_MOVE)
		{
			pPlayerChara()->pAI->Move_Base();
		}
		else
		{
			pPlayerChara()->pAI->Attack_Base();
		}
		s_AltaInputprocess().process(false);
		return s_AltaInputprocess().on;
	}


	if(!s_AltaInputprocess().on)
	{//直前の関数返り値がfalse
		resetTurnInformation(pDev);
	}

	if(cannotinput())
	{//インプット不能フラグ
		s_AltaInputprocess().process(true);
		return s_AltaInputprocess().on;
	}

	if(!s_AltaInputprocess().on)
	{//直前の関数返り値がfalse
		CutInM().CutInForAllObject(インプット直前_タイミング);

		//自動振り向き
		if(s_autoTurnFlag() && !キャラクター敵対判定(pPlayerChara(),キャラ前地形(pPlayerChara(),1)->pOnChar))
		{
			int noFirstSwitch = false;
			自動振り向き(noFirstSwitch);
			s_autoTurnFlag() = false;
		}


		s_AltaInputprocess().process(true);
		return s_AltaInputprocess().on;
	}


	//ダッシュ中
	if(Dashprocess(pDev))
	{
		s_AltaInputprocess().process(false);
		return s_AltaInputprocess().on;
	}

	//斜め
	aspectDiagonprocess(pDev);

	//振り向き
	aspectTurnprocess(pDev);

	//スキップ
	if(g_pPlayerInput()->decision().onCount > 15 
		&& g_pPlayerInput()->decision().on
		&& g_pPlayerInput()->cancel().onCount > 15 
		&& g_pPlayerInput()->cancel().on
		&& !pPlayerChara()->Condition.飛ばし危険())
	{

		int stopswitch = s_skipValiable()&0x7F000000;
		if(!stopswitch)
		{

			/*
			int i;
			pPlayerChara()->anime_stamp_step += DEFAULTSTAMPSUMSTEP/8;
			//敵が増えるor敵に隣接
			vector<pcCharacter> pcharalist;
			キャラクター目視可範囲キャラリスト(pPlayerChara(),pcharalist);
			UByte enemycount = 0;
			UByte enemynearcount = 0;
			UByte Dangerflag = 0;
			UByte MaxHp = 0;
			
			for(i=0;i<pcharalist.size();i++)
			{
				if(pcharalist[i]->Forse == CHARACTER_FORSE_ENEMY)
				{
					enemycount++;
				}
			}
			//敵が隣接
			for(i=0;i<pcharalist.size();i++)
			{
				if(pcharalist[i]->Forse == CHARACTER_FORSE_ENEMY)
				{
					if((pcharalist[i]->足元地形()->place - pPlayerChara()->足元地形()->place).dif() <= 1)
					{
						enemynearcount += 1;
					}
				}
			}

			int x,y;
			for(x=pPlayerChara()->足元地形()->placeX-4;x<=pPlayerChara()->足元地形()->placeX+4;x++)
			{
				for(y=pPlayerChara()->足元地形()->placeY-4;y<=pPlayerChara()->足元地形()->placeY+4;y++)
				{
					if(pPlayerChara()->足元地形()->RoomIndex != -1 && Map().Land(x,y)->RoomIndex != -1 && (Map().Land(x,y)->getDarkflag() || Map().Land(x,y)->getDangerflag() ))
					{
						Dangerflag++;
						break;
					}
				}
				if(Dangerflag) break;
			}

			if(pPlayerChara()->HP == pPlayerChara()->MHP)
			{
				MaxHp = 1;
			}
			*/
			int tempdata;
			DashRiskCulcurate(tempdata,DashAspectCulcurate());

			if((s_skipValiable()&0x00FFFFFF) < (tempdata&0x00FFFFFF))
			{//敵増えた
				s_skipValiable() |= 0x01000000;
			}
			else
			{
				s_skipValiable() &= 0x7F000000;
				s_skipValiable() += tempdata;

				pPlayerChara()->emotion.insert(EMOTION_TIMESKIP);
				s_AltaInputprocess().process(false);
				return s_AltaInputprocess().on;
			}
		}
	}
	else
	{
		s_skipValiable() = s_skipValiable()&0x00FFFFFF;
		//s_skipValiable() += 0<<16;
	}
	pPlayerChara()->emotion.erase(EMOTION_TIMESKIP);
	
	if( g_pPlayerInput()->decision().on
		&& g_pPlayerInput()->cancel().on
		)
	{
		/*
		vector<pcCharacter> pcharalist;
		キャラクター目視可範囲キャラリスト(pPlayerChara(),pcharalist);
		UByte enemycount = 0;
		UByte enemynearcount = 0;
		UByte Dangerflag = 0;
		UByte MaxHp = 0;

		int i;
		for(i=0;i<pcharalist.size();i++)
		{
			if(pcharalist[i]->Forse == CHARACTER_FORSE_ENEMY)
			{
				enemycount++;
			}
		}
		//敵が隣接
		for(i=0;i<pcharalist.size();i++)
		{
			if(pcharalist[i]->Forse == CHARACTER_FORSE_ENEMY)
			{
				if((pcharalist[i]->足元地形()->place - pPlayerChara()->足元地形()->place).dif() <= 1)
				{
					enemynearcount += 1;
				}
			}
		}

		int x,y;
		for(x=pPlayerChara()->足元地形()->placeX-4;x<=pPlayerChara()->足元地形()->placeX+4;x++)
		{
			for(y=pPlayerChara()->足元地形()->placeY-4;y<=pPlayerChara()->足元地形()->placeY+4;y++)
			{
				if(pPlayerChara()->足元地形()->RoomIndex != -1 && Map().Land(x,y)->RoomIndex != -1 && (Map().Land(x,y)->getDarkflag() || Map().Land(x,y)->getDangerflag() ))
				{
					Dangerflag++;
					break;
				}
			}
			if(Dangerflag) break;
		}

		if(pPlayerChara()->HP == pPlayerChara()->MHP)
		{
			MaxHp = 1;
		}
		*/
		int tempdata;
		DashRiskCulcurate(tempdata,DashAspectCulcurate());

		s_skipValiable() &= 0x7F000000;
		s_skipValiable() += tempdata;
	}

	//矢飛ばし
	//とショートカット
	if(shootAndShortCutprocess(pDev))
	{
		return s_AltaInputprocess().on;
	}
	

	
	//移動
	if((g_pPlayerInput()->X||g_pPlayerInput()->Y)
		&& !g_pPlayerInput()->turn().on
		&& !g_pPlayerInput()->dash().on
		&& !g_pPlayerInput()->smartdash().on
		&& !s_DashButtonTap())
	{
		cCoordinate aspectcoo;
		int aspect;
		
		aspectcoo.x = g_pPlayerInput()->X;
		aspectcoo.y = g_pPlayerInput()->Y;

		aspect = aspectcoo.GetAspect();


		int moveflag = 泥棒確認移動可_主人公専用(pPlayerChara(),aspect);
		if(moveflag && 移動要請(sg_pDungeonSystem->pPlayerChara(),aspect))
		{
			pcDroping pdrop;

			/*
			if((pdrop = pPlayerChara()->足元()) != NULL)
			{//なんか落ちてる
				pdrop->乗る();
			}
			*/
			//メッセージ(_T("移動\n"));
	
			現在地からマッピング(sg_pDungeonSystem->pPlayerChara());
			s_AttackDelay() = 1;

			s_AltaInputprocess().process(false);
			return s_AltaInputprocess().on;
		}
		else if(moveflag && すれ違い要請(sg_pDungeonSystem->pPlayerChara(),aspect))
		{
			現在地からマッピング(sg_pDungeonSystem->pPlayerChara());
			s_AttackDelay() = 1;

			s_AltaInputprocess().process(false);
			return s_AltaInputprocess().on;
		}

	}
	else if(g_pPlayerInput()->attack().on
		&& !g_pPlayerInput()->cancel().on)
	{
		s_AttackDelay()--;
		if(( s_AttackDelay() < 0 && s_AttackRapidSwitch() )|| g_pPlayerInput()->attack().justOn)
		{
			if(キャラ前地形(pPlayerChara(),1)->pOnChar && キャラ前地形(pPlayerChara(),1)->pOnChar->canTalk())
			{
				キャラ前地形(pPlayerChara(),1)->pOnChar->TalkEvent();
			}
			else
			{
				if(sg_pDungeonSystem->pPlayerChara()->通常攻撃())
				{
					vector<pcCharacter> cautionEnemy;
					int enemyWarning = FALSE;
					キャラクター目視可範囲キャラリスト(pPlayerChara(),cautionEnemy);
					int i,size = cautionEnemy.size();
					for(i=0;i<size;i++)
					{
						if(キャラクター敵対判定(cautionEnemy[i],pPlayerChara())
							&& cautionEnemy[i]->足元地形()->RoomIndex == pPlayerChara()->足元地形()->RoomIndex)
						{
							enemyWarning = TRUE;
							break;
						}
					}

					if(enemyWarning)
					{
						s_AttackDelay() = 10;
					}
					else
					{
						s_AttackDelay() = 2;
					}
					s_AttackRapidSwitch() = 1;
					
					GameIdiom::罠チェック(キャラ前地形(pPlayerChara(),1));
					s_AltaInputprocess().process(false);
					return s_AltaInputprocess().on;
				}
			}
		}
	}
	else
	{
		s_AttackDelay() = 30;
		s_AttackRapidSwitch() = 0;
	}

	/*
	if(g_pPlayerInput()->cancel().on)
	{
		s_AttackDelay() = 13;
	}
	*/
	
	isInputWaitingflag() = true;
	s_AltaInputprocess().process(true);
	return s_AltaInputprocess().on;
}

//一ターンに一度実行される
int cDungeonSystem::Turnprocess(IDirect3DDevice9 *pDev)
{
	
	int i;
	vector<pcCharacter> vpchara = CharaList();
	for(i=0;i<vpchara.size();i++)
	{
		//vpchara[i]->Condition.process();
		if(!vpchara[i]->足元地形()->through(vpchara[i]->水上歩行(), vpchara[i]->壁中歩行(), vpchara[i]->空中歩行()))
		{
			キャラ落下(vpchara[i],vpchara[i]->足元地形());
		}

		if(vpchara[i]->MaxholdNum() < vpchara[i]->holdItem.size())
		{//持ちすぎ
			map<tstring, StyleString> valiable;
			valiable[_T("Chara")] = vpchara[i]->ShortName();
			g_Langメッセージ(_T("アイテム持ちすぎメッセージ"),valiable);
			sg_pDungeonSystem->ころび要請(vpchara[i], vpchara[i]->holdItem.size() - vpchara[i]->MaxholdNum(), true);
		}
	}
	

	//pPlayerChara()->Condition.process();


	if(pDungeon()->pfloor(FloorLevel())->enemyAppear())
	{
		int vint[3];
		敵自然湧きID(vint);
		if(敵の数() < pDungeon()->pfloor(FloorLevel())->Maxenemynum())
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
			pcCharacter pcmob = キャラクター生成_自然湧き(vint[0], vint[1], CHARACTER_FORSE_ENEMY, pland);
		}
	}
	
	if(泥棒状態())
	{
		int shoplevel = 店レベル(pSaveQuest->ShopFund);
		if( random() < (shoplevel+1)*0.03)
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
	else
	{
		int roomindex = pPlayerChara()->足元地形()->RoomIndex;
		if(店請求金額(店長(roomindex)) > 0
			&& (roomindex == -1 || !(Map().RoomList[roomindex]->ShopFlag)))
		{
			泥棒発覚();
		}
	}

	if(フロア飛ばず状態())
	{
		floornothrowswitch()--;
		if(!フロア飛ばず状態())
		{
			map<tstring, StyleString> valiable;
			g_Langメッセージ(_T("フロア飛ばず解除メッセージ"),valiable);
		}
	}

	暗闇更新();

	if(!拠点フラグ())
	{
		SumTurnCount()++;
		TurnCount()++;
		if(TurnCount() == GAME_TURN_CAUTION)
		{
			AnimationManager().
				Anime_PlaySE(_T("quake.wav"),pPlayerChara()->足元地形()->place);
			EffectFunctions::地震(60,8);
			map<tstring, StyleString> valiable;
			g_Langメッセージ(_T("フロア崩壊警告1メッセージ"),valiable);
			AnimationManager().Anime_MessageWait();
		}
		else if(TurnCount() == GAME_TURN_ALERT)
		{
			AnimationManager().
				Anime_PlaySE(_T("quake.wav"),pPlayerChara()->足元地形()->place);
			EffectFunctions::地震(120,16);
			map<tstring, StyleString> valiable;
			g_Langメッセージ(_T("フロア崩壊警告2メッセージ"),valiable);

			フロア明かり追加();
			AnimationManager().Anime_MessageWait();
		}
		else if(TurnCount() == GAME_TURN_GAMEOVER)
		{

			map<tstring, StyleString> valiable;
			valiable[_T("Chara")] = pPlayerChara()->FullName();
			g_Langメッセージ(_T("フロア崩壊メッセージ"),valiable);
			AnimationManager().Anime_MessageWait();

			sg_pDungeonSystem->AnimationManager().
				Anime_PlaySE(_T("quake.wav"),pPlayerChara()->足元地形()->place);
			for(i=0;i<20;i++)
			{
				EffectFunctions::爆発エフェクト_中(
					pPlayerChara()->placeX + 9.0*rand()/RAND_MAX - 4.5,
					pPlayerChara()->placeY + 9.0*rand()/RAND_MAX - 4.5,
					360.0*rand()/RAND_MAX);
			}
			強制ダメージ要請(pPlayerChara(),pPlayerChara()->MHP,0,1);
			GameOver();
		}
	}

	宴会発覚(pPlayerChara()->足元地形()->RoomIndex);
	店入出判定(pPlayerChara()->足元地形()->RoomIndex);

	//スクリプト
	
	int size = pDungeon()->storyScriptEnvironments().size();
	for(i=0;i<size;i++)
	{
		if(!pDungeon()->storyScriptEnvironments()[i].function(_T("dungeonTurnProcess")))
		{
			pDungeon()->storyScriptEnvironments()[i].outputErrorFile(_T("Error_")+pDungeon()->DungeonID_+_T("_dungeonTurnProcess.txt"));
		}
		//DungeonStoryScript_ExtraSetting(psr,keys[i],_T("floorExtraSetting"),valiable);
	}
	

	return true;
}

//trueを返した時メニュー無し
int cDungeonSystem::menuCommandprocess(IDirect3DDevice9 *pDev)
{

	if(menuControlLayerV().size() == 0)
	{//何も表示されて無い
		if(g_pPlayerInput()->menu().justOn && !pPlayerChara()->Condition.行動不能である())
		{//メニューを押された
	

			if(メニュー表示可能())
			{
				メニューを開く();
			}
			return false;
		}



	}
	else
	{
		


		//最も新しいものだけに処理を行う
		menuControlLayerV().process(pDev);


		/*
		if(g_pPlayerInput()->menu().justOn && menuControlLayerV().size() == 1)
		{
			menuControlLayerV().cancel();
		}
		*/


		if(menuControlLayerV().size() == 0)
		{
			//マップ
			Map().MinimapViewOn();
			Resetminimap(pDev);
		}

		//アタック誤爆しないように
		s_AttackDelay() = 30;
		//キャンセル誤爆しないように
		s_DashButtonCancelEffect() = 1;
		return false;
	}

	return true;
}

int cDungeonSystem::refreshCharaDropView()
{
	int i;
	Map().setVisibleForcusX(pPlayerChara()->visibleplace.x);
	Map().setVisibleForcusY(pPlayerChara()->visibleplace.y);
	for(i=0;i<CharaList().size();i++)
	{
		CharaList()[i]->opaque = キャラクター目視可(CharaList()[i]);
		//CharaList()[i]->pri_onland_DrawFlag = キャラクター目視可(CharaList()[i]);
		CharaList()[i]->visibleplace.set(CharaList()[i]->placeX,CharaList()[i]->placeY,0,0);
		CharaList()[i]->visibleaspect = CharaList()[i]->aspect;
	}
	for(i=0;i<DropList().size();i++)
	{
		//DropList()[i]->opaque = 落ち物目視可(DropList()[i]);
		DropList()[i]->opaque = DropList()[i]->isDiscover();
		DropList()[i]->visibleplace.set(DropList()[i]->placeX,DropList()[i]->placeY,0,0);
		DropList()[i]->pri_onland_DrawFlag = (DropList()[i]->is_onland_DrawFlag());
	}

	return true;
}

int cDungeonSystem::EventDraw()
{
	EventManager().Draw();
	return TRUE;
}

int cDungeonSystem::EventDrawUnderObject()
{
	EventManager().DrawUnderObject();
	return TRUE;
}

//インプットの直前に一度初期化する
int cDungeonSystem::resetTurnInformation(IDirect3DDevice9 *pDev)
{

	turnEnd_flag() = FALSE;


	現在地からマッピング(sg_pDungeonSystem->pPlayerChara());





	//visibleCharaList().clear();
	//visibleCharaList() = CharaList();
	int i;
	for(i=0;i<visibleCharaList().size();i++)
	{
		visibleCharaList()[i]->visibleReset();
	}
	
	for(i=0;i<visibleDropList().size();i++)
	{
		visibleDropList()[i]->visibleReset();
	}
	//visibleDropList().clear();
	//visibleDropList() = DropList();

	refreshCharaDropView();

	EventManager().visibleReset();


	Map().MapvisibleReset();

	//方向スイッチ初期化
	aspectTurnprocessswitch() = false;

	//ミニマップ用カウンター初期化
	count_minimap() = 0;

	arrangementObject(pDev);

	Refreshminimap(pDev);
	
	return true;
}

//整理して使われなくなったオブジェクトを消す。
int cDungeonSystem::arrangementObject(IDirect3DDevice9 *pDev)
{
	vector<pcCharacter>::iterator chitr = visibleCharaList().begin();

	for(;chitr != visibleCharaList().end();)
	{
		
		if((*chitr)->死亡())
		{//死んでたらリストからはずす。
			chitr = visibleCharaList().erase(chitr);
			continue;
		}

		/*
		StyleString s = _T("");
		TCHAR d[16];
		_stprintf(d,_T("%d"),chitr->use_count());
		s = s + (*chitr)->ShortName() + _T("：参照数") + d + _T("\n");
		メッセージ(s);
		*/


		chitr++;
	}
	
	vector<pcDroping>::iterator dritr = visibleDropList().begin();


	for(;dritr != visibleDropList().end();)
	{
		
		if((*dritr)->broken)
		{//壊れてたらリストからはずす。
			dritr = visibleDropList().erase(dritr);
			continue;
		}



		dritr++;
	}
	/*
	for(i=0;i<DropList().size();i++)
	{
		DropList()[i]
	}
	*/

	return true;
}



//trueを返した時処理無し
int cDungeonSystem::minimapViewprocess(IDirect3DDevice9 *pDev)
{
	
	s_AltaofminimapViewprocess().process(g_GameEnv.m_Input.PlayerInput.miniMap().on);
	if(s_AltaofminimapViewprocess().justOn)
	{
		s_minimapHiding() = false;
		//マップ
		Map().MinimapViewOn();
		Resetminimap(pDev);
	}
	if(s_AltaofminimapViewprocess().justOff)
	{
		//マップ
		if(menuControlLayerV().empty())
			Map().MinimapViewOn();
		else
			Map().MinimapViewOff();
		Resetminimap(pDev);
	}
	if(s_AltaofminimapViewprocess().on)
	{

		return false;
	}

	return true;
}

int cDungeonSystem::Refreshminimap(IDirect3DDevice9 *pDev)
{

	Map().RefreshMiniMap();

	return true;
}

//ミニマップを消して書き直す。
int cDungeonSystem::Resetminimap(IDirect3DDevice9 *pDev)
{

	Map().ResetMiniMap();

	return true;
}

int cDungeonSystem::minimapDrawDelegate(IDirect3DDevice9 *pDev, const cCoordinate& leftTop, const int boxSize, const int alpha)
{
	EventManager().minimapDraw(leftTop, boxSize, alpha);
	return true;
}


int cDungeonSystem::自動振り向き(int& notfirstflag)
{

	cCoordinate aspectcoo;
	vector<pcCharacter> neicharaList;
	pcCharacter neichara;
	int i;
	if(notfirstflag == false)
	{//初回

		
		for(i=pPlayerChara()->aspect;i<8+pPlayerChara()->aspect;i++)
		{
			aspectcoo.SetAspect(i);
			aspectcoo.x += pPlayerChara()->placeX;
			aspectcoo.y += pPlayerChara()->placeY;
			neichara = Map().Land(aspectcoo)->pOnChar;
			if(neichara != NULL &&
				キャラクター目視可(neichara)
				&&
				isCanNeighberAttack(pPlayerChara(), i,1,pPlayerChara()->通常攻撃_壁斜めすり抜け(),pPlayerChara()->通常攻撃_壁すり抜け()))
			{
				neicharaList.push_back(neichara);
			}
		}
		int maxvalue = -1;
		int maxindex = -1;
		for(i=0;i<neicharaList.size();i++)
		{
			if(neicharaList[i]->CharaAttribute().count(キャラ属性::弾幕)) {
				continue;
			}
			int value = 0;
			if(neicharaList[i]->Forse == CHARACTER_FORSE_ENEMY) value += 100;
			value += 100-(neicharaList[i]->HP*100/(double)neicharaList[i]->MHP);
			if(value > maxvalue)
			{
				maxindex = i;
				maxvalue = value;
			}
		}

		if(maxindex != -1)
		{
			aspectcoo.x = neicharaList[maxindex]->placeX - pPlayerChara()->placeX;
			aspectcoo.y = neicharaList[maxindex]->placeY - pPlayerChara()->placeY;
			方向転換要請_主人公専用(pPlayerChara(), aspectcoo.GetAspect());
		}
		notfirstflag = true;
	}
	else
	{

		for(i=pPlayerChara()->aspect+1;i<8+pPlayerChara()->aspect+1;i++)
		{
			aspectcoo.SetAspect(i);
			aspectcoo.x += pPlayerChara()->placeX;
			aspectcoo.y += pPlayerChara()->placeY;
			neichara = Map().Land(aspectcoo)->pOnChar;
			if(neichara != NULL &&
				キャラクター目視可(neichara)
				&&
				isCanNeighberAttack(pPlayerChara(), i,1,pPlayerChara()->通常攻撃_壁斜めすり抜け(),pPlayerChara()->通常攻撃_壁すり抜け()))
			{
				neicharaList.push_back(neichara);
			}
		}
		if(neicharaList.size())
		{
			aspectcoo.x = neicharaList[0]->placeX - pPlayerChara()->placeX;
			aspectcoo.y = neicharaList[0]->placeY - pPlayerChara()->placeY;
			方向転換要請_主人公専用(pPlayerChara(), aspectcoo.GetAspect());
		}
	}

	return true;
}
//trueを返した時処理無し
int cDungeonSystem::aspectTurnprocess(IDirect3DDevice9 *pDev)
{
	/*
	//ダッシュボタンタップ
	if(g_pPlayerInput()->dash().justOff && (g_pPlayerInput()->dash().onCount < 16 || s_DashButtonTap()))
	{
		if(!s_DashButtonCancelEffect())
			s_DashButtonTap() = !s_DashButtonTap();
		else
			s_DashButtonCancelEffect() = 0;
	}
	*/

	if(s_DashButtonCancelEffect())
	{
		s_DashButtonCancelEffect()++;
		if(s_DashButtonCancelEffect() > 16) s_DashButtonCancelEffect() = 0;
	}


	s_aspectTurnprocess().process(g_pPlayerInput()->turn().on || s_DashButtonTap());


	if(g_pPlayerInput()->turn().justOff)
	{
		s_DashButtonTap() = false;
	}


	if(s_aspectTurnprocess().justOn || g_pPlayerInput()->dash().justOn)
	{//敵の方向を自動で向く
		自動振り向き(aspectTurnprocessswitch());
	}
	else if(s_aspectTurnprocess().on)
	{
		cCoordinate coo;
		coo.x = g_pPlayerInput()->X;
		coo.y = g_pPlayerInput()->Y;



		s_aspectTurndiagonsupport()--;

		if((abs(coo.x)+abs(coo.y)) && s_aspectTurndiagonsupport() < 0)
		{
			方向転換要請_主人公専用(pPlayerChara(), coo.GetAspect());
		
		}
		

		if((abs(coo.x)+abs(coo.y)))
		{
			if(pPlayerChara()->aspect%2
				&& coo.GetAspect()%2)
			{
				s_aspectTurndiagonsupport() = 4;
			}
			else if(pPlayerChara()->aspect%2
				&& !(coo.GetAspect()%2))
			{
			
			}
			else
			{
				s_aspectTurndiagonsupport() = 0;
			}
		}
		

		return false;
	}

	return true;
}
//trueを返した時処理無し
int cDungeonSystem::aspectDiagonprocess(IDirect3DDevice9 *pDev)
{
	s_aspectDiagonprocess().process(g_pPlayerInput()->diagon().on);

	return true;

}
int cDungeonSystem::shootAndShortCutprocess(IDirect3DDevice9 *pDev)
{
	if(pSaveData->ShortCutsManager == NULL)
	{
		return false;
	}
	if(s_shootAndShortCutswitch() == 0)
	{
		//通常モード
		if(g_pPlayerInput()->shot().on //ショットボタン押されている
			&& !(
				(g_pPlayerInput()->cancel().on) //キャンセルボタン押されていない
				|| (g_pPlayerInput()->cancel().on && g_pPlayerInput()->cancel().offCount < 5)
					//キャンセルボタン押された直後ではない
				)
			)
		{
			s_shootAndShortCutswitch() = 1;
			//ショートカットモード
			pSaveData->ShortCutsManager->resetAllShortcutText();

			s_ShortCutSelectSwitch() = -1;
		}
	}
	
	if(s_shootAndShortCutswitch() < 0)
	{
		//ショートカットキャンセルモード
		cCoordinate coo;
		coo.x = g_pPlayerInput()->X;
		coo.y = g_pPlayerInput()->Y;

		if(!(abs(coo.x)+abs(coo.y)) || s_ShortCutSelectOffCount() > 60)
		{
			if(!g_pPlayerInput()->shot().on )
			{
				s_shootAndShortCutswitch() = 0;
				s_ShortCutSelectOffCount() = 0;
				//通常モード
			}
		}
		s_ShortCutSelectOffCount()++;

		s_AltaInputprocess().process(true);
		return true;
	}
	else if(s_shootAndShortCutswitch() > 0)
	{
		//ショートカットモード
		if(g_pPlayerInput()->shot().on)
		{
			s_shootAndShortCutswitch()++;

			if(s_shootAndShortCutswitch() == shootAndShortCut_startDrawWindowFrame)
			{
				pSaveData->ShortCutsManager->refreshAutomaticShortCut(pPlayerChara()->holdItem);
			}
		}

		if(g_pPlayerInput()->cancel().justOn)
		{
			s_shootAndShortCutswitch() = -1;
			s_ShortCutSelectOffCount() = 0;
			//ショートカットキャンセルモード
		}

		if(g_pPlayerInput()->shot().justOff)
		{
			
			if(s_shootAndShortCutswitch() < shootAndShortCut_startDrawWindowFrame)
			{
				//ショット押したばかり
				s_shootAndShortCutswitch() = 0;

				if(	   !g_pPlayerInput()->X
					&& !g_pPlayerInput()->Y
					&& !(s_ShortCutSelectOffCount() > 0))//方向ボタン離したとたんではない
				{
					s_shootAndShortCutswitch() = -1;//ショートカットキャンセルモード
					pcBullet pbul = boost::dynamic_pointer_cast<cBullet>(pPlayerChara()->bulletequipment);
					if(pbul != NULL)
					{
						撃ち要請(pPlayerChara(),pPlayerChara()->bulletequipment);
						s_AltaInputprocess().process(false);
						return true;
					}
				}

			}

			if(	s_ShortCutSelectOffCount() > 0 )//方向ボタン離したとたんではない
			{

				if(s_ShortCutSelectSwitch() != -1)
				{
					if(s_shootAndShortCutswitch() >= shootAndShortCut_startDrawWindowFrame){
						pSaveData->ShortCutsManager->doShortcut((eShortCuts_index)s_ShortCutSelectSwitch());
					}
					else {
						if(!pSaveData->ShortCutsManager->isShortcutAutomatic((eShortCuts_index)s_ShortCutSelectSwitch())) {
							pSaveData->ShortCutsManager->doShortcut((eShortCuts_index)s_ShortCutSelectSwitch());
						}
					}
				}

				//ここでショートカット発動
				
				s_shootAndShortCutswitch() = -1;//ショートカットキャンセルモード
				s_ShortCutSelectOffCount() = 0;
			}

			s_shootAndShortCutswitch() = -1;//ショートカットキャンセルモード
			s_ShortCutSelectOffCount() = 0;
		}

		cCoordinate coo;
		coo.x = g_pPlayerInput()->X;
		coo.y = g_pPlayerInput()->Y;
		
		s_ShortCutdiagonsupport()--;

		

		if((abs(coo.x)+abs(coo.y)))
		{
			//入力がある
			if(s_ShortCutSelectSwitch()%2
				&& coo.GetAspect()%2)
			{
				//斜め選択状態で
				//斜め入力
				s_ShortCutdiagonsupport() = 5;
				s_ShortCutSelectSwitch() = coo.GetAspect();
				s_ShortCutSelectOffCount() = 20;
			}
			else if(s_ShortCutSelectSwitch()%2
				&& !(coo.GetAspect()%2))
			{
				//斜め選択状態で
				//斜め以外入力
				if(s_ShortCutdiagonsupport() <= 0)
				{
					s_ShortCutdiagonsupport() = 0;
					s_ShortCutSelectSwitch() = coo.GetAspect();
					s_ShortCutSelectOffCount() = 20;
				}
			}
			else
			{
				//斜め選択状態でない
				s_ShortCutdiagonsupport() = 0;
				s_ShortCutSelectSwitch() = coo.GetAspect();
				s_ShortCutSelectOffCount() = 20;
			}
		}
		else
		{
			if(s_ShortCutSelectOffCount() > 0)
			{
				s_ShortCutSelectOffCount()--;
			}
			else
			{
				s_ShortCutSelectSwitch() = -1;
				s_ShortCutSelectOffCount() = 0;
			}
		}

		s_AltaInputprocess().process(true);
		return true;
	}


	return false;

}



int cDungeonSystem::LandDraw(IDirect3DDevice9 *pDev)
{

	Map().Draw(pDev);
	return true;
}

int cDungeonSystem::MiniMapDraw(IDirect3DDevice9 *pDev)
{

	if(s_minimapHiding()){
		return false;
	}

	//地図をかく
	Map().DrawMiniMap(pDev);



	return true;
}


//キャラクタ描画順
bool NearChara(pcCharacter a, pcCharacter b){
    return ((a->visibleplace.y + a->anime_position.y)*DEFAULTCHARACTERSIZE*DEFAULTCHARACTERSIZE + a->size()) 
		< ((b->visibleplace.y + b->anime_position.y)*DEFAULTCHARACTERSIZE*DEFAULTCHARACTERSIZE + b->size());
}
bool NearDrop(pcDroping a, pcDroping b){
    return (a->visibleplace.z*DEFAULTCHARACTERSIZE*DEFAULTCHARACTERSIZE - (int)(a->落ちてる())) 
		< (b->visibleplace.z*DEFAULTCHARACTERSIZE*DEFAULTCHARACTERSIZE - (int)(b->落ちてる()));
}


int cDungeonSystem::CharacterDraw(IDirect3DDevice9 *pDev)
{
	if(s_aspectTurnprocess().on)
	{
		Map().DrawGridLand(pDev,s_aspectTurnprocess().onCount/10.0, min(s_aspectTurnprocess().onCount/10.0,1.0)*(-cos(s_aspectTurnprocess().onCount/8.0+1.5)+1.0)/2.0);
	}

	if(s_aspectTurnprocess().on)
	{
		Map().DrawGridChara(pDev,s_aspectTurnprocess().onCount/10.0);
	}

	int i;
	sort( visibleCharaList().begin(), visibleCharaList().end(), ::NearChara  );
    
	for(i = 0;i<visibleCharaList().size();i++)
	{

		if( Map().isOnDisplay(
			visibleCharaList()[i]->visibleplace.x + visibleCharaList()[i]->anime_position.x , 
			visibleCharaList()[i]->visibleplace.y + visibleCharaList()[i]->anime_position.y))
		{

			visibleCharaList()[i]->mapForcus.set(Map().visibleForcusX(),Map().visibleForcusY(),0,0);
			visibleCharaList()[i]->Draw(pDev);
			
		}


		

		
	}

	
	if(hiddenInterface_){
		//矢印書かないで
		return true;
	}
	//矢印描いちゃいます
	if(s_aspectDiagonprocess().on)
	{
		cDrawingObject DO;
		cCoordinate aspect;
		int i;
		for(i=0;i<4;i++)
		{
			aspect.SetAspect(i*2+1);
			DO.setTexture(pArrowsTexture,TEX_ARROWS_WIDTH,TEX_ARROWS_HEIGHT);
			
			DO.Width = DO.Height = MAPTEXBOXSIZE * MAPTEXPOWER/2;
			DO.CenterX = MAPDRAWCENTERX + aspect.x * MAPTEXBOXSIZE * MAPTEXPOWER/2;
			DO.CenterY = MAPDRAWCENTERY + aspect.y * MAPTEXBOXSIZE * MAPTEXPOWER/2;

			DO.m_TexRange.setLTRB(0/2.0,(i*2+1)/8.0,
								1/2.0,(i*2+1+1)/8.0);
			DO.Draw(pDev);
		}
	}
	else if(s_aspectTurnprocess().on)
	{
		cDrawingObject DO;
		cCoordinate aspect;
		aspect.SetAspect(pPlayerChara()->aspect);
		DO.setTexture(pArrowsTexture,TEX_ARROWS_WIDTH,TEX_ARROWS_HEIGHT);
		
		DO.Width = DO.Height = MAPTEXBOXSIZE * MAPTEXPOWER/2;
		DO.CenterX = MAPDRAWCENTERX + aspect.x * MAPTEXBOXSIZE * MAPTEXPOWER/2;
		DO.CenterY = MAPDRAWCENTERY + aspect.y * MAPTEXBOXSIZE * MAPTEXPOWER/2;

		DO.m_TexRange.setLTRB(0/2.0,pPlayerChara()->aspect/8.0,
							1/2.0,(pPlayerChara()->aspect+1)/8.0);
		DO.Draw(pDev);
	}


	return true;
}

int cDungeonSystem::DropingDraw(IDirect3DDevice9 *pDev)
{
	int i;
	sort( visibleDropList().begin(), visibleDropList().end(), ::NearDrop  );
    


	for(i = 0;i<visibleDropList().size();i++)
	{

		if( //visibleDropList()[i]->isDiscover() && 
			visibleDropList()[i]->isShouldDraw()
			&& Map().isOnDisplay(visibleDropList()[i]->visibleplace.x,visibleDropList()[i]->visibleplace.y,1))
		{
			visibleDropList()[i]->mapForcus.set(Map().visibleForcusX(),Map().visibleForcusY(),0,0);
			visibleDropList()[i]->Draw(pDev);
		}
	}
	if(s_aspectTurnprocess().on)
	{
		Map().DrawGridDrop(pDev,s_aspectTurnprocess().onCount/10.0);
	}
	return true;
}
	

int cDungeonSystem::FrameDraw(IDirect3DDevice9 *pDev)
{
	if(pFrameTexture == NULL) return false;


	cDrawingObject DO;

	if(GameScreenInterface.disphero_HP < 0.125*GameScreenInterface.disphero_MHP )
	{
		DO.m_color.ARGB(255,255,128,96);
	}
	else if(GameScreenInterface.disphero_HP < 0.25*GameScreenInterface.disphero_MHP )
	{
		DO.m_color.ARGB(255,255,196,128);
	}


	/*

	DO.setTexture(pFrameTexture,512,1024);
	//左
	DO.setLTRB(0,0,256,768);
	DO.m_TexRange.setLTRB(0/2.0, 0/8.0, 1/2.0, 6/8.0);
	DO.Draw(pDev);

	//右
	DO.setLTRB(800-256,0,800,768);
	DO.m_TexRange.setLTRB(1/2.0, 0/8.0, 2/2.0, 6/8.0);
	DO.Draw(pDev);

	//上
	DO.setLTRB(256,0,256+512,128);
	DO.m_TexRange.setLTRB(0, 6/8.0, 1, 7/8.0);
	DO.Draw(pDev);

	//下
	DO.setLTRB(256,600-128+1,256+512,600);
	DO.m_TexRange.setLTRB(0, 7/8.0, 1, 8/8.0);
	DO.Draw(pDev);

	*/

	DO.setTexture(pFrameTexture,SCREEN_X,SCREEN_Y);
	//左
	DO.setLTRB(0,0,SCREEN_X,SCREEN_Y);
	DO.m_TexRange.setLTRB(0, 0, 1, 1);
	DO.Draw(pDev);

	return true;

}


void cDungeonSystem::MakeResultWindow(int clear)
{


	//結果
	pcSaveResult presult = pcSaveResult(new cSaveResult);
	presult->lastequipItem_.resize(2);
	presult->lastequipItem_[0] = DataBase.NewCopyDroping(pPlayerChara()->attackequipment);
	presult->lastequipItem_[1] = DataBase.NewCopyDroping(pPlayerChara()->defenseequipment);
	アイテム店フラグ(presult->lastequipItem_[0], cDroping::SHOPFLAG_NONE);
	アイテム店フラグ(presult->lastequipItem_[1], cDroping::SHOPFLAG_NONE);
	
	int i;
	for(i=0;i<10;i++)
	{
		int index = MessageW().Messagelogs.size()-1-i;
		if(index < 0) break;
		presult->lastmessage_.push_back(*MessageW().Messagelogs[index]);
	}
	presult->score_ = pDungeon()->culcuscore();
	presult->dungeonName_ = pDungeon()->DungeonName_;
	presult->CharaName_ = pPlayerChara()->FullName();
	presult->LastMHP_ = pPlayerChara()->MHP;
	presult->LastMSP_ = pPlayerChara()->MaxStomach;
	presult->LastLV_ = pPlayerChara()->LV;

	presult->clearflag_ = clear;
	presult->lastfloor_ = FloorLevel()-(clear==GAMECLEAR_CLEARFLAG);
	presult->Sumturn_ = SumTurnCount();
	presult->SumFrame_ = time_FrameCount() + time_SumFrameCount();
	presult->SumFramerealtime_ = Sumtime()*60;
	presult->SumExp_ = pPlayerChara()->EXP;
	presult->SumdefeatNum_ = SumEnemyDefeatNum();
	presult->money_ = GameScreenInterface.money.value;
	presult->BadEndNum_ = pSaveQuest->BadEndNum;

	presult->Date_ = time(NULL);
	presult->PlayerSign_ = pSaveData->playersigns;
	presult->Version_ = g_VersionString();

	presult->pFirstConditionOfPlayer_ = pSaveQuest->pFirstConditionOfPlayer;
	presult->FirstConditionOfMoney_ = pSaveQuest->FirstConditionOfMoney;

	presult->localFlags_ = pSaveQuest->localFlags;
	presult->privateFlags_ = pSaveQuest->privateFlags;

	pcControlLayer pccl;
	pcResultWindow pcrw;
	//新規コントロールレイヤー
	menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDevice_D3D);

	//新規リストウィンドウ
	pccl->WindowList.push_back(pcrw = pcResultWindow(new cResultWindow));


	pcrw->Init(pDevice_D3D,presult);
	pcrw->CenterX = SCREEN_X/2;
	pcrw->CenterY = SCREEN_Y/2;

	DataBase.DungeonRanking(pSaveQuest->DungeonID)->rankingIn(presult);
}

//ゲームオーバー
void cDungeonSystem::GameOver()
{
	cannotinput() = true;
	GameOverFlag() = true;
	GameClearFlag() = false;

	GameOverpreprocess();
}
void cDungeonSystem::GameEndSavetyPrepareing()
{
	sg_pDungeonSystem->pSaveQuest->GoodEndFlags = 1;
	sg_pDungeonSystem->pSaveData->GoodEndFlagInBaseTemp = 1;
}
void cDungeonSystem::GameClear()
{
	cannotinput() = true;
	GameOverFlag() = true;
	GameClearFlag() = GAMECLEAR_CLEARFLAG;

	g_GameEnv.m_SoundManager.stopBGM();

	if(!pDungeon()->clearResultSkipFlag())
	{
		g_GameEnv.m_SoundManager.getplayBGMFromFile(_T("fanfare.wav"), _T(""));
		//sg_pDungeonSystem->AnimationManager().
		//	Anime_PlaySE(_T("fanfare.wav"),pPlayerChara()->足元地形()->place);
	}
	pSaveData->setGlobalFlags_ClearedFlag(pSaveQuest->DungeonID, TRUE);

	GameOverpreprocess();
}
void cDungeonSystem::GamePullout()
{
	cannotinput() = true;
	GameOverFlag() = true;
	GameClearFlag() = GAMECLEAR_PULLOUTFLAG;

	GameOverpreprocess();
}
int cDungeonSystem::ダンジョン引き上げ要請()
{
	if(店請求金額(店長(pPlayerChara()->足元地形()->RoomIndex)) > 0) return false;
	if(泥棒状態()) return false;
	GamePullout();
	return true;
}

void cDungeonSystem::GameBaseEnd()
{
	pSaveData->lastpPlayer = sg_pDungeonSystem->pPlayerChara();
	pSaveData->lastmoney = sg_pDungeonSystem->主人公お金所持量();
	int i, size = DropList().size();
	for(i=0;i < size;i++) {
		if(DropList()[i]->落ちてる() && !(DropList()[i]->broken))
		{
			storeInto(pSaveData->lastfloorItem, DropList()[i]);
		}
	}
	

}
void cDungeonSystem::GameBaseStart()
{

	vector<pcDroping> vpdrop = pSaveData->lastfloorItem;
	int i, size = vpdrop.size();
	for(i=0;i < size;i++) {
		int x = vpdrop[i]->placeX;
		int y = vpdrop[i]->placeY;
		pushbackDropList(vpdrop[i]);
		storeOut(pSaveData->lastfloorItem, vpdrop[i], Map().Land(x,y));
	}
	pSaveData->lastfloorItem.clear();

}
void cDungeonSystem::GameReturnTitle()
{
	sg_pDungeonSystem->GameBaseEnd();

	sg_pDungeonSystem->GameClearFlag() = cDungeonSystem::GAMECLEAR_PULLOUTFLAG;//出向
	sg_pDungeonSystem->GameOverpreprocess();
}
void cDungeonSystem::ターンエンド()
{
	turnEnd_flag() = TRUE;



	s_DashButtonTap() = false;
	s_aspectTurnprocess().process(false);
}



//必ずこれを使う(本当はラッピングすべきだが)
int cDungeonSystem::pushbackCharaList(pcCharacter pchara)
{
	CharaList().push_back(pchara);
	visibleCharaList().push_back(pchara);
	return true;
}

//必ずこれを使う
int cDungeonSystem::pushbackDropList(pcDroping pdrop)
{
	vector<pcDroping>::iterator itr = find(DropList().begin(),DropList().end(),pdrop);
	if(itr == DropList().end()) DropList().push_back(pdrop);

	
	itr = find(visibleDropList().begin(),visibleDropList().end(),pdrop);
	if(itr == visibleDropList().end()) visibleDropList().push_back(pdrop);
	
	return true;
}


bool cDungeonSystem::eventNotification(const pLuaString& eventName, const pLuaString& notificatedObject, const pLuaString& argument)
{
	bool successflag = false;
	int i, size = pDungeon()->storyScriptEnvironments().size();
	for(i=0;i<size;i++)
	{
		if(!strcmp(notificatedObject, "") || notificatedObject == pDungeon()->storyScriptEnvironments()[i].name())
		{
			try
			{
				//::luabind::object globalTable(pDungeon()->storyScriptEnvironments()[i].luaState(),0);
				//::luabind::object eventFunction(globalTable[eventName]);
				//if(eventFunction.is_valid() && luabind::type(eventFunction) == LUA_TFUNCTION)
				{
					if( !pDungeon()->storyScriptEnvironments()[i].function(eventName, argument));
					{
						
						pDungeon()->storyScriptEnvironments()[i].outputErrorFile(_T("Error_")+pDungeon()->DungeonID_+luaString2tstring(eventName)+_T(".txt"));
						
					}//DungeonStoryScript_ExtraSetting(psr,keys[i],_T("floorExtraSetting"),valiable);
					successflag = true;
				}
			}
			catch(luabind::error const& e)
			{
				//return false;
			}

		}
	}
	
	return successflag;
}