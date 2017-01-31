#include "stdafx.h"
#include "cSaveStore.h"
#include "cDungeonSystem.h"
#include "gameMainSystem/cEnvironment.h"
#include "gameMainSystem/filemanage/cDataConverter.h"
#include "Gameobjects.h"
#include "cStringWindow.h"
#include <boost/pointer_cast.hpp>

#include "cShortCuts.h"

#include <time.h>

#include <Windows.h>

#pragma comment(lib, "version.lib")

#define PTFIRSTVIRSION _T("PT_sp 0, 0, 0, 0")
tstring g_VersionString()
{
	tstring ver = _T("1ST");
	//ver += FILEVERSION;
	TCHAR path[1024];
	GetModuleFileName(0,path,1024);

	DWORD dwZero = 0;
	DWORD dwVerInfoSize = GetFileVersionInfoSize(path, &dwZero);
	if(dwVerInfoSize == 0) {
		return _T("error");
	}

	unsigned char *pBlock;
	pBlock = new unsigned char[dwVerInfoSize];
	if(pBlock == NULL) return FALSE;
	GetFileVersionInfo(path, dwZero, dwVerInfoSize, pBlock);

	//バージョンを取得する為のバッファ
	void *pvVersion;
	UINT VersionLen;

	//言語のコードページが分かっている場合は、それを直接指定すればよい
	//プロダクトバージョン
	if(VerQueryValue(pBlock, TEXT("\\StringFileInfo\\041104b0\\ProductVersion"), &pvVersion, &VersionLen)){
		//printf("ProductVersion:%d:%s\n", VersionLen, pvVersion);
		int i;
		for(i=0;i<VersionLen;i++)
		{
			ver += *((TCHAR*)pvVersion+i);
		}
	}

	delete [] pBlock;
	return ver;
}

pcSaveData g_pSaveData()
{
	return sg_pDungeonSystem->pSaveData;
}

cSaveData::cSaveData(void)
{
	playersigns = g_Lang(_T("デフォルトプレイヤー名")).conclete_tstr();
	playerLevel = 1;
	playerExp = 0;

	lastmoney = 0;
	
	GoodEndFlagInBaseTemp = 1;
	DidGoodEndFlagInBase = 1;
}

cSaveData::~cSaveData(void)
{
	save();
}
void cSaveData::save()//明示的にセーブ
{
	sg_pDungeonSystem->厄システム整理();

	playersave_.vv_int().resize(1);
	playersave_.vv_int()[0].resize(5);
	SUBSTITUTION_L2R(playerLevel, playersave_.vv_int()[0][0]);
	SUBSTITUTION_L2R(playerExp, playersave_.vv_int()[0][1]);

	//追加１
	SUBSTITUTION_L2R(lastmoney, playersave_.vv_int()[0][2]);

	//追加５
	SUBSTITUTION_L2R(GoodEndFlagInBaseTemp, playersave_.vv_int()[0][3]);
	SUBSTITUTION_L2R(DidGoodEndFlagInBase, playersave_.vv_int()[0][4]);

	playersave_.vv_char().resize(6);
	cDataConverter::ConvertT2VecC(playersigns,playersave_.vv_char()[0]);
	ConvertT2VecC(pSaveStore_,playersave_.vv_char()[1]);

	//追加１
	ConvertT2VecC(lastpPlayer,playersave_.vv_char()[2]);

	//追加２
	cDataConverter::ConvertMTT2VecVecC2VecC(savekey, playersave_.vv_char()[3]);
	
	//追加３
	cDataConverter::ConvertVT2VecVecC2VecC(lastfloorItem, playersave_.vv_char()[4]);
	
	//追加４
	cDataConverter::ConvertMTT2VecVecC2VecC(globalFlags, playersave_.vv_char()[5]);
	
	playersave_.save();
}

void cSaveData::Init(IDirect3DDevice9 *pDev)
{
	
	ShortCutsManager = pcShortCutsManager(new cShortCutsManager);
	pSaveStore_ = pcSaveStore(new cSaveStore);
	pSaveStore_->Init(pDev,_T("save.dat"));

	playersave_.savename() = SAVEDATADIRCTORY _T("player.dat");
	playersave_.load();
	
	if(playersave_.vv_int().size() < 1) return;
	if(playersave_.vv_int()[0].size() < 2) return;
	SUBSTITUTION_R2L(playerLevel, playersave_.vv_int()[0][0]);
	SUBSTITUTION_R2L(playerExp, playersave_.vv_int()[0][1]);

	if(playersave_.vv_int()[0].size() >= 3)
	{//追加１
		SUBSTITUTION_R2L(lastmoney, playersave_.vv_int()[0][2]);
	}

	if(playersave_.vv_int()[0].size() >= 5)
	{//追加４
		SUBSTITUTION_R2L(GoodEndFlagInBaseTemp, playersave_.vv_int()[0][3]);
		SUBSTITUTION_R2L(DidGoodEndFlagInBase, playersave_.vv_int()[0][4]);
	}

	if(playersave_.vv_char().size() < 1) return;
	cDataConverter::BackDecodeVecC2T(playersigns,playersave_.vv_char()[0]);

	if(playersave_.vv_char().size() < 2) return;
	BackDecodeVecC2T(pSaveStore_,playersave_.vv_char()[1]);

	if(playersave_.vv_char().size() >= 3)
	{//追加１
		BackDecodeVecC2T(lastpPlayer,playersave_.vv_char()[2]);
	}
	
	if(playersave_.vv_char().size() >= 4)
	{//追加２
		cDataConverter::BackDecodeVecC2VecVecC2MTT(savekey, playersave_.vv_char()[3]);
	}

	if(playersave_.vv_char().size() >= 5)
	{//追加２
		cDataConverter::BackDecodeVecC2VecVecC2VT(lastfloorItem, playersave_.vv_char()[4]);
	}

	if(playersave_.vv_char().size() >= 6)
	{//追加３
		cDataConverter::BackDecodeVecC2VecVecC2MTT(globalFlags, playersave_.vv_char()[5]);
	}

}
double cSaveData::LevelShopDiscount()
{
	return (1.0+30.0/(30.0+(playerLevel-1)))/2.0;
}
int cSaveData::LevelHinaKeepNum()
{
	return 2+playerLevel;
}
int cSaveData::LevelStoreSpaceNum()
{
	return StoreSpaceNumForExtendNum(StoreSpaceExtendCount());
	//if(playerLevel < 10) return 100;
	//else if(playerLevel < 20) return 400;
	//else return 1000;
}
int cSaveData::StoreSpaceNumForExtendNum(int extendNum)
{
	return 100 + extendNum*50;
}
int cSaveData::StoreSpaceExtendCount()
{
	return mapUtility::getMapValue(globalFlags,(tstring)_T("BaseProperty:StoreSpaceExtendCount"),0);
}
int cSaveData::StoreSpaceExtendCountMax()
{
	return 8;
}
int cSaveData::StoreSpaceExtendNeedMoney()
{
	return (StoreSpaceExtendCount()+1)*2000 * LevelShopDiscount();
}
void cSaveData::StoreSpaceExtend()
{
	int nowStoreSpaceExtend = StoreSpaceExtendCount();
	return mapUtility::setMapValue(globalFlags,(tstring)_T("BaseProperty:StoreSpaceExtendCount"),nowStoreSpaceExtend+1);
}
int cSaveData::NextPlayerLevel(int now_level)
{
	if(now_level > 100)
	{
		now_level = 100;
	}
	return 200*(now_level);
}
int cSaveData::AddPlayerLevel(double addExp)
{
	int up_level = 0;
	playerExp += addExp;
	for(;;)
	{
		if(playerExp >= NextPlayerLevel(playerLevel))
		{
			//レベルアップ
			playerExp -= NextPlayerLevel(playerLevel);
			playerLevel += 1;
			up_level += 1;
			continue;
		}
		break;
	}
	return up_level;
}
map<tstring, int> cSaveData::initialLocalFlags(const tstring& dungeonID, const int num)
{
	map<tstring, int> localFlags;

	//初めから
	localFlags[cSaveQuest::privateFlagKey_AppreciationSupportKey()] = 
		sg_pDungeonSystem->pSaveData->globalFlags[cSaveData::globalFlagsKey_AppreciationSupportKey(dungeonID,num)];
	
	if(sg_pDungeonSystem->pSaveData->globalFlags_ClearedFlag(dungeonID,FALSE))
	{
		localFlags[cSaveQuest::privateFlagKey_StoryEventKey()] = 
			mapUtility::getMapValue( sg_pDungeonSystem->pSaveData->globalFlags,
									cSaveData::globalFlagsKey_StoryEventKey(dungeonID,num),
									FALSE
			);
		}
	else
	{
		//クリアしてない
		localFlags[cSaveQuest::privateFlagKey_StoryEventKey()] = TRUE;
	
	}

	return localFlags;
}
tstring cSaveData::globalFlagsKey_AppreciationSupportKey(const tstring& dungeonID, int num)
{//識別補助
	return _T("Dungeon:")+dungeonID+_T(":")+setStyle(num,_T("%d")).conclete_tstr() +_T(":")+cSaveQuest::privateFlagKey_AppreciationSupportKey();
}
tstring cSaveData::globalFlagsKey_StoryEventKey(const tstring& dungeonID, int num)
{//ストーリー
	return _T("Dungeon:")+dungeonID+_T(":")+setStyle(num,_T("%d")).conclete_tstr()+_T(":")+cSaveQuest::privateFlagKey_StoryEventKey();
}
tstring cSaveData::globalFlagsKey_ClearedFlagKey(const tstring& dungeonID)
{//クリアしたことがあるか
	return _T("Dungeon:")+dungeonID+_T(":Cleared");
}
tstring cSaveData::globalFlagsKey_AppearedFlagKey(const tstring& dungeonID)
{//クリアしたことがあるか
	return _T("Dungeon:")+dungeonID+_T(":Appeared");
}
const int mapUtility::getMapValue(map<tstring,int>& mapObject, tstring& key, int defaultValue)
{
	map<tstring, int>::iterator itr = mapObject.find(key);
	if(mapObject.end() == itr)
	{
		mapObject[key] = defaultValue;
		return defaultValue;
	}
	return itr->second;
}
void mapUtility::setMapValue(map<tstring,int>& mapObject, tstring& key, int setValue)
{
	mapObject[key] = setValue;
}
//-------------------------------------------------------
cSaveStore::cSaveStore(void)
{
	bank = 0.0;
	yaku = 0.0;
	yakuStore = 0;
	playerExpStore = 0;
	marisaShopLastSiireLevel = 0;
	marisaShopLastSiireExp = 0;
}

cSaveStore::~cSaveStore(void)
{
	
	sg_pDungeonSystem->DataBase.savememoDic(*memodata_);

}


void cSaveStore::Init(IDirect3DDevice9 *pDev, tstring savefile)
{


	saveclass_dic_.Reserve(memodata_);
	saveclass_dic_.Init(SAVEDATADIRCTORY _T("dictionary_")+savefile);

	if(!memodata_->empty()) sg_pDungeonSystem->DataBase.loadmemoDic(*memodata_);

}
int cSaveStore::save(pcSaveClass data) const
{
	data->vv_double().resize(1);
	data->vv_double()[0].resize(6);
	SUBSTITUTION_L2R(bank, data->vv_double()[0][0]);
	SUBSTITUTION_L2R(yaku, data->vv_double()[0][1]);
	//120308追加
	SUBSTITUTION_L2R(yakuStore, data->vv_double()[0][2]);
	SUBSTITUTION_L2R(playerExpStore, data->vv_double()[0][3]);
	//120314追加
	SUBSTITUTION_L2R(marisaShopLastSiireLevel, data->vv_double()[0][4]);
	SUBSTITUTION_L2R(marisaShopLastSiireExp, data->vv_double()[0][5]);

	data->vv_char().resize(5);
	cDataConverter::ConvertVVT2VecVecVecC2VecC(storeItem,data->vv_char()[0]);
	cDataConverter::ConvertVT2VecVecC2VecC(storeMemo,data->vv_char()[1]);
	cDataConverter::ConvertVT2VecVecC2VecC(hinaItem,data->vv_char()[2]);
	cDataConverter::ConvertVT2VecVecC2VecC(hinaKeepItem,data->vv_char()[3]);

	//120314追加
	cDataConverter::ConvertVT2VecVecC2VecC(marisaShopItem, data->vv_char()[4]);
	return SUCCESS;
}
int cSaveStore::load(const pcSaveClass data)
{
	if(data->vv_double().size() < 1) return ERROR_FM_LOAD_DECODE_ERROR;
	if(data->vv_double()[0].size() < 2) return ERROR_FM_LOAD_DECODE_ERROR;
	
	SUBSTITUTION_R2L(bank, data->vv_double()[0][0]);
	SUBSTITUTION_R2L(yaku, data->vv_double()[0][1]);
	//120308追加
	if(data->vv_double()[0].size() >= 4)
	{
		SUBSTITUTION_R2L(yakuStore, data->vv_double()[0][2]);
		SUBSTITUTION_R2L(playerExpStore, data->vv_double()[0][3]);
	}

	//120314追加
	if(data->vv_double()[0].size() >= 6)
	{
		SUBSTITUTION_R2L(marisaShopLastSiireLevel, data->vv_double()[0][4]);
		SUBSTITUTION_R2L(marisaShopLastSiireExp, data->vv_double()[0][5]);
	}

	if(data->vv_char().size() < 4) return ERROR_FM_LOAD_DECODE_ERROR;
	cDataConverter::BackDecodeVecC2VecVecVecC2VVT(storeItem,data->vv_char()[0]);
	cDataConverter::BackDecodeVecC2VecVecC2VT(storeMemo,data->vv_char()[1]);
	cDataConverter::BackDecodeVecC2VecVecC2VT(hinaItem,data->vv_char()[2]);
	cDataConverter::BackDecodeVecC2VecVecC2VT(hinaKeepItem,data->vv_char()[3]);

	//120314追加
	if(data->vv_char().size() >= 5)
	{
		cDataConverter::BackDecodeVecC2VecVecC2VT(marisaShopItem, data->vv_char()[4]);
	}

	return SUCCESS;
}
//-------------------------------------------------------
cSaveQuest::cSaveQuest(void)
{
	floor = -1;
	randBase = 0;

	money = 0;
	Sumturn = 0;
	SumFrame = 0;
	SumFramerealtime = 0;
	SumdefeatNum = 0;
	GoodEndFlags = 1;
	BadEndNum = 0;
	saveFileNum = 0;

	ShopFund = 0.0;
	ShopDebt = 0.0;

	GensouLongLivePower = 0.0;

	FirstConditionOfMoney = 0;
}

cSaveQuest::~cSaveQuest(void)
{

}
void cSaveQuest::setUnEnable()
{
	floor = -1;
}
int cSaveQuest::enable()
{
	int flag = (floor != -1);
	
#ifdef _UNRELEASE

#else
	if(key != sg_pDungeonSystem->pSaveData->savekey[savefile_->savename()])
	{
		flag = 0;
	}
#endif

	return flag;
}
void cSaveQuest::Init(IDirect3DDevice9 *pDev, tstring savefile)
{
	
	savefile_ = pcSaveClass(new cSaveClass);
	savefile_->savename() = SAVEDATADIRCTORY + savefile + _T("_save.dat");
	saveFileID = savefile;



}
int cSaveQuest::save()
{
	{//key計算
		key = clock() ^ time(NULL);
		sg_pDungeonSystem->pSaveData->savekey[savefile_->savename()] = key;
	}
	
	savefile_->vv_int().resize(2);
	savefile_->vv_int()[0].resize(11);
	SUBSTITUTION_L2R(floor, savefile_->vv_int()[0][0]);
	SUBSTITUTION_L2R(money, savefile_->vv_int()[0][1]);
	SUBSTITUTION_L2R(Sumturn, savefile_->vv_int()[0][2]);
	SUBSTITUTION_L2R(SumFrame, savefile_->vv_int()[0][3]);
	SUBSTITUTION_L2R(SumFramerealtime, savefile_->vv_int()[0][4]);
	SUBSTITUTION_L2R(SumdefeatNum, savefile_->vv_int()[0][5]);
	SUBSTITUTION_L2R(GoodEndFlags, savefile_->vv_int()[0][6]);
	SUBSTITUTION_L2R(BadEndNum, savefile_->vv_int()[0][7]);

	//追加分3
	SUBSTITUTION_L2R(key, savefile_->vv_int()[0][8]);

	//追加分4
	SUBSTITUTION_L2R(FirstConditionOfMoney, savefile_->vv_int()[0][9]);

	//追加分6
	SUBSTITUTION_L2R(saveFileNum, savefile_->vv_int()[0][10]);


	SUBSTITUTION_L2R(FreeFlags, savefile_->vv_int()[1]);

	savefile_->vv_char().resize(12);//サイズによって忘れず変更すること
	cDataConverter::ConvertT2VecC(DungeonID,savefile_->vv_char()[0]);
	cDataConverter::ConvertT2VecC(randBase,savefile_->vv_char()[1]);
	ConvertT2VecC(pPlayer,savefile_->vv_char()[2]);
	cDataConverter::ConvertVT2VecVecC2VecC(pFriend,savefile_->vv_char()[3]);
	cDataConverter::ConvertMTT2VecVecC2VecC(DropIDtoMEMO,savefile_->vv_char()[4]);
	cDataConverter::ConvertMTT2VecVecC2VecC(DropIDtoAppreciated,savefile_->vv_char()[5]);
	
	//追加分
	cDataConverter::ConvertT2VecC(Version_,savefile_->vv_char()[6]);

	//追加分2
	cDataConverter::ConvertVT2VecVecC2VecC(hinaPickItem,savefile_->vv_char()[7]);
	
	savefile_->vv_double().resize(1);
	savefile_->vv_double()[0].resize(3);
	SUBSTITUTION_L2R(ShopFund,savefile_->vv_double()[0][0]);
	SUBSTITUTION_L2R(ShopDebt,savefile_->vv_double()[0][1]);

	//追加分7
	SUBSTITUTION_L2R(GensouLongLivePower,savefile_->vv_double()[0][2]);

	//追加分4
	ConvertT2VecC(pFirstConditionOfPlayer,savefile_->vv_char()[8]);
	

	//追加分5
	cDataConverter::ConvertMTT2VecVecC2VecC(localFlags, savefile_->vv_char()[9]);
	cDataConverter::ConvertMTT2VecVecC2VecC(privateFlags, savefile_->vv_char()[10]);

	//追加分6
	cDataConverter::ConvertT2VecC(saveFileID, savefile_->vv_char()[10]);

	return savefile_->save();
	
	return SUCCESS;
}
int cSaveQuest::load()
{
	
	savefile_->load();

	//int
	if(savefile_->vv_int().size() < 2) return ERROR_FM_LOAD_DECODE_ERROR;
	if(savefile_->vv_int()[0].size() < 8) return ERROR_FM_LOAD_DECODE_ERROR;
	SUBSTITUTION_R2L(floor, savefile_->vv_int()[0][0]);
	SUBSTITUTION_R2L(money, savefile_->vv_int()[0][1]);
	SUBSTITUTION_R2L(Sumturn, savefile_->vv_int()[0][2]);
	SUBSTITUTION_R2L(SumFrame, savefile_->vv_int()[0][3]);
	SUBSTITUTION_R2L(SumFramerealtime, savefile_->vv_int()[0][4]);
	SUBSTITUTION_R2L(SumdefeatNum, savefile_->vv_int()[0][5]);
	SUBSTITUTION_R2L(GoodEndFlags, savefile_->vv_int()[0][6]);
	SUBSTITUTION_R2L(BadEndNum, savefile_->vv_int()[0][7]);
	
	if(savefile_->vv_int()[0].size() > 8)
	{//追加分3
		SUBSTITUTION_R2L(key, savefile_->vv_int()[0][8]);
	}

	if(savefile_->vv_int()[0].size() > 9)
	{//追加４
		SUBSTITUTION_R2L(FirstConditionOfMoney, savefile_->vv_int()[0][9]);
	}
	if(savefile_->vv_int()[0].size() > 10)
	{//追加6
		SUBSTITUTION_R2L(saveFileNum, savefile_->vv_int()[0][10]);
	}
	else
	{
		FirstConditionOfMoney = 0;
	}

	SUBSTITUTION_R2L(FreeFlags, savefile_->vv_int()[1]);

	//double
	if(savefile_->vv_double().size() >= 1 ) 
	{
		if (savefile_->vv_double()[0].size() >= 2 )
		{
			SUBSTITUTION_R2L(ShopFund,savefile_->vv_double()[0][0]);
			SUBSTITUTION_R2L(ShopDebt,savefile_->vv_double()[0][1]);
		}

		//追加分7
		if (savefile_->vv_double()[0].size() >= 3 )
		{
			SUBSTITUTION_R2L(GensouLongLivePower,savefile_->vv_double()[0][2]);
		}
	}

	//char
	if(savefile_->vv_char().size() < 6) return ERROR_FM_LOAD_DECODE_ERROR;
	cDataConverter::BackDecodeVecC2T(DungeonID,savefile_->vv_char()[0]);
	cDataConverter::BackDecodeVecC2T(randBase,savefile_->vv_char()[1]);
	BackDecodeVecC2T(pPlayer,savefile_->vv_char()[2]);
	cDataConverter::BackDecodeVecC2VecVecC2VT(pFriend,savefile_->vv_char()[3]);
	cDataConverter::BackDecodeVecC2VecVecC2MTT(DropIDtoMEMO,savefile_->vv_char()[4]);
	cDataConverter::BackDecodeVecC2VecVecC2MTT(DropIDtoAppreciated,savefile_->vv_char()[5]);

	//追加分
	if(savefile_->vv_char().size() < 7)
	{
		Version_ = PTFIRSTVIRSION;
	}
	else cDataConverter::BackDecodeVecC2T(Version_,savefile_->vv_char()[6]);

	//追加分2
	if(savefile_->vv_char().size() < 8)
	{
		
	}
	else cDataConverter::BackDecodeVecC2VecVecC2VT(hinaPickItem,savefile_->vv_char()[7]);



	if(savefile_->vv_char().size() > 8)
	{//追加分4
		BackDecodeVecC2T(pFirstConditionOfPlayer,savefile_->vv_char()[8]);
	}

	if(savefile_->vv_char().size() > 10)
	{//追加分5
		cDataConverter::BackDecodeVecC2VecVecC2MTT(localFlags, savefile_->vv_char()[9]);
		cDataConverter::BackDecodeVecC2VecVecC2MTT(privateFlags, savefile_->vv_char()[10]);
	}
	if(savefile_->vv_char().size() > 11)
	{//追加分6
		cDataConverter::BackDecodeVecC2T(saveFileID, savefile_->vv_char()[11]);
	}

	return SUCCESS;
}
StyleString cSaveQuest::shortExplanationText()
{
	map<tstring,StyleString> val;

	val[_T("Chara")] = pPlayer->FullName();

	val[_T("Floor")] = setStyle(floor,_T("%3d"));
	
	double sumtime = SumFramerealtime/60;
	val[_T("RealTimeSec")].addstring(((int)sumtime)%60,_T("%2d"));
	val[_T("RealTimeMin")].addstring((((int)sumtime)/60)%60,_T("%2d"));
	val[_T("RealTimeHour")].addstring(((int)sumtime)/60/60,_T("%3d"));

	val[_T("Turn")] = setStyle(Sumturn,_T("%6d"));
	val[_T("LV")] = setStyle(pPlayer->LV,_T("%3d"));
	val[_T("HP")] = setStyle(pPlayer->HP,_T("%3d"));
	val[_T("MHP")] = setStyle(pPlayer->MHP,_T("%3d"));
	
	if(pPlayer->attackequipment != NULL)
		val[_T("ArmName")] = pPlayer->attackequipment->FullName();
	else
		val[_T("ArmName")] = _T("");

	if(pPlayer->defenseequipment != NULL)
		val[_T("ShdName")] = pPlayer->defenseequipment->FullName();
	else
		val[_T("ShdName")] = _T("");

	if(pFirstConditionOfPlayer != NULL)
	{
		val[_T("FirstItemNum")] = setStyle(sg_pDungeonSystem->ItemSize(pFirstConditionOfPlayer->holdItem),_T("%3d"));
	}
	else
	{
		val[_T("FirstItemNum")] = _T("0");
	}

	cMoney mny;
	mny.value = FirstConditionOfMoney;
	val[_T("FirstMoney")].addstring(mny.caption());

	return g_Lang(_T("つづきからshortExplanationText"),val);
}
tstring cSaveQuest::privateFlagKey_AppreciationSupportKey()
{//識別補助
	return _T("AppreciationSupport");
}

tstring cSaveQuest::privateFlagKey_StoryEventKey()
{//ストーリー
	return _T("StoryEvent");
}
//-------------------------------------------------------
cSaveConfig::cSaveConfig(void)
{

	inited = 0;


}

cSaveConfig::~cSaveConfig(void)
{
	
	save();
}
void cSaveConfig::save()
{
	if(inited)
	{
		pSoundVolume_->at(0) = g_GameEnv.m_SoundManager.getSEVolume();
		pSoundVolume_->at(1) = g_GameEnv.m_SoundManager.getBGMVolume();


		pGhaphicsLight_->at(0) = sg_pDungeonSystem->EV_charEdgedFlag;

		pMapThickness_->at(0) = sg_pDungeonSystem->EV_mapThickness;

		*ppadconfig_ = g_GameEnv.m_Input.patInputManager->padconfigI2B;
		saveclass_.save();
	}
}

void cSaveConfig::Init(IDirect3DDevice9 *pDev, tstring savefile)
{

	saveclass_.Reserve(pSoundVolume_);
	saveclass_.Reserve(ppadconfig_);
	saveclass_.Reserve(pGhaphicsLight_);
	saveclass_.Reserve(pMapThickness_);
	saveclass_.Init(SAVEDATADIRCTORY+savefile);

	if(!ppadconfig_->empty())
	{
		g_GameEnv.m_Input.patInputManager->padconfigI2B = *ppadconfig_;
	}
	else
	{
		*ppadconfig_ = g_GameEnv.m_Input.patInputManager->padconfigI2B;
	}
	if(pSoundVolume_->size() < 2)
	{
		pSoundVolume_->resize(2);
		pSoundVolume_->at(0) = 80;
		pSoundVolume_->at(1) = 80;
	}
	g_GameEnv.m_SoundManager.setSEVolume(pSoundVolume_->at(0));
	g_GameEnv.m_SoundManager.setBGMVolume(pSoundVolume_->at(1));
	if(pGhaphicsLight_->size() < 1)
	{
		pGhaphicsLight_->resize(1);
		pGhaphicsLight_->at(0) = 1;
	}
	sg_pDungeonSystem->EV_charEdgedFlag = pGhaphicsLight_->at(0);

	if(pMapThickness_->size() < 1)
	{
		pMapThickness_->resize(1);
		pMapThickness_->at(0) = 6;
	}
	sg_pDungeonSystem->EV_mapThickness = pMapThickness_->at(0);


	inited = 1;
}
//-------------------------------------------------------
cSaveResult::cSaveResult(void)
{


	score_ = 0;

	LastMHP_ = 0;
	LastMSP_ = 0;
	LastLV_ = 0;

	clearflag_ = 0;
	lastfloor_ = 0;
	Sumturn_ = 0;
	SumFrame_ = 0;
	SumFramerealtime_ = 0;
	SumExp_ = 0;
	SumdefeatNum_ = 0;
	money_ = 0;

	BadEndNum_ = 0;//異常終了カウンタ

	Date_ = 0;

	FirstConditionOfMoney_ = 0;

}
cSaveResult::cSaveResult(const cSaveResult& rother)
{
	
	if(&rother == this) return;
	lastmessage_ = rother.lastmessage_;
	int i;
	for(i=0;i<rother.lastequipItem_.size();i++)
	{
		lastequipItem_.push_back(sg_pDungeonSystem->DataBase.NewCopyDroping(rother.lastequipItem_[i]));
	}


	dungeonName_ = rother.dungeonName_;
	CharaName_ = rother.CharaName_;
	score_ = rother.score_;

	LastMHP_ = rother.LastMHP_;
	LastMSP_ = rother.LastMSP_;
	LastLV_ = rother.LastLV_;

	clearflag_ = rother.clearflag_;
	lastfloor_ = rother.lastfloor_;
	Sumturn_ = rother.Sumturn_;
	SumFrame_ = rother.SumFrame_;
	SumFramerealtime_ = rother.SumFramerealtime_;
	SumExp_ = rother.SumExp_;
	SumdefeatNum_ = rother.SumdefeatNum_;
	money_ = rother.money_;
	BadEndNum_ = rother.BadEndNum_;

	Date_ = rother.Date_;
	PlayerSign_ = rother.PlayerSign_;
	Version_ = rother.Version_;

	pFirstConditionOfPlayer_ = sg_pDungeonSystem->DataBase.NewCopyCharacter(rother.pFirstConditionOfPlayer_);
	FirstConditionOfMoney_ = rother.FirstConditionOfMoney_;

	localFlags_ = rother.localFlags_;
	privateFlags_ = rother.privateFlags_;
}
int cSaveResult::save(pcSaveClass data) const
{

	data->vv_int().resize(1);
	data->vv_int()[0].resize(14);
	SUBSTITUTION_L2R(score_, data->vv_int()[0][0]);
	SUBSTITUTION_L2R(LastMHP_, data->vv_int()[0][1]);
	SUBSTITUTION_L2R(LastMSP_, data->vv_int()[0][2]);
	SUBSTITUTION_L2R(LastLV_, data->vv_int()[0][3]);
	SUBSTITUTION_L2R(clearflag_, data->vv_int()[0][4]);
	SUBSTITUTION_L2R(lastfloor_, data->vv_int()[0][5]);
	SUBSTITUTION_L2R(Sumturn_, data->vv_int()[0][6]);
	SUBSTITUTION_L2R(SumFrame_, data->vv_int()[0][7]);
	SUBSTITUTION_L2R(SumFramerealtime_, data->vv_int()[0][8]);
	SUBSTITUTION_L2R(SumExp_, data->vv_int()[0][9]);
	SUBSTITUTION_L2R(SumdefeatNum_, data->vv_int()[0][10]);
	SUBSTITUTION_L2R(money_, data->vv_int()[0][11]);
	SUBSTITUTION_L2R(BadEndNum_, data->vv_int()[0][12]);

	//追加２
	SUBSTITUTION_L2R(FirstConditionOfMoney_, data->vv_int()[0][13]);

	data->vv_char().resize(10);
	cDataConverter::ConvertT2VecC(Date_,data->vv_char()[0]);
	cDataConverter::ConvertT2VecC(PlayerSign_,data->vv_char()[1]);
	ConvertT2VecC(CharaName_,data->vv_char()[2]);
	ConvertT2VecC(dungeonName_,data->vv_char()[3]);
	cDataConverter::ConvertVT2VecVecC2VecC(lastequipItem_,data->vv_char()[4]);
	cDataConverter::ConvertVT2VecVecC2VecC(lastmessage_,data->vv_char()[5]);
	cDataConverter::ConvertT2VecC(Version_,data->vv_char()[6]);

	//追加2
	ConvertT2VecC(pFirstConditionOfPlayer_,data->vv_char()[7]);

	//追加3
	cDataConverter::ConvertMTT2VecVecC2VecC(localFlags_,data->vv_char()[8]);
	cDataConverter::ConvertMTT2VecVecC2VecC(privateFlags_,data->vv_char()[9]);
	return SUCCESS;
}
int cSaveResult::load(const pcSaveClass data)
{


	if(data->vv_int().size() < 1) return ERROR_FM_LOAD_DECODE_ERROR;
	if(data->vv_int()[0].size() < 13) return ERROR_FM_LOAD_DECODE_ERROR;
	SUBSTITUTION_R2L(score_, data->vv_int()[0][0]);
	SUBSTITUTION_R2L(LastMHP_, data->vv_int()[0][1]);
	SUBSTITUTION_R2L(LastMSP_, data->vv_int()[0][2]);
	SUBSTITUTION_R2L(LastLV_, data->vv_int()[0][3]);
	SUBSTITUTION_R2L(clearflag_, data->vv_int()[0][4]);
	SUBSTITUTION_R2L(lastfloor_, data->vv_int()[0][5]);
	SUBSTITUTION_R2L(Sumturn_, data->vv_int()[0][6]);
	SUBSTITUTION_R2L(SumFrame_, data->vv_int()[0][7]);
	SUBSTITUTION_R2L(SumFramerealtime_, data->vv_int()[0][8]);
	SUBSTITUTION_R2L(SumExp_, data->vv_int()[0][9]);
	SUBSTITUTION_R2L(SumdefeatNum_, data->vv_int()[0][10]);
	SUBSTITUTION_R2L(money_, data->vv_int()[0][11]);
	SUBSTITUTION_R2L(BadEndNum_, data->vv_int()[0][12]);
	if(data->vv_int()[0].size() > 13)
	{//追加2
		SUBSTITUTION_R2L(FirstConditionOfMoney_, data->vv_int()[0][13]);
	}
	else FirstConditionOfMoney_ = 0;

	if(data->vv_char().size() < 6) return ERROR_FM_LOAD_DECODE_ERROR;
	cDataConverter::BackDecodeVecC2T(Date_,data->vv_char()[0]);
	cDataConverter::BackDecodeVecC2T(PlayerSign_,data->vv_char()[1]);
	BackDecodeVecC2T(CharaName_,data->vv_char()[2]);
	BackDecodeVecC2T(dungeonName_,data->vv_char()[3]);
	cDataConverter::BackDecodeVecC2VecVecC2VT(lastequipItem_,data->vv_char()[4]);
	cDataConverter::BackDecodeVecC2VecVecC2VT(lastmessage_,data->vv_char()[5]);
	
	//追加分1
	if(data->vv_char().size() < 7)//不等号注意
	{
		Version_ = PTFIRSTVIRSION;
	}
	else cDataConverter::BackDecodeVecC2T(Version_,data->vv_char()[6]);

	//追加分2
	if(data->vv_char().size() > 7)
	{
		BackDecodeVecC2T(pFirstConditionOfPlayer_,data->vv_char()[7]);
	}

	//追加分3
	if(data->vv_char().size() > 9)
	{
		cDataConverter::BackDecodeVecC2VecVecC2MTT(localFlags_,data->vv_char()[8]);
		cDataConverter::BackDecodeVecC2VecVecC2MTT(privateFlags_,data->vv_char()[9]);
	}

	return SUCCESS;
}

cSaveResult::~cSaveResult(void)
{
	
}


void cSaveResult::Init(IDirect3DDevice9 *pDev)
{


}
//-------------------------------------------------------
cSaveRanking::cSaveRanking(void)
{
	//FreeResult.resize(10);



}
cSaveRanking::~cSaveRanking(void)
{
	
	save(savefile_);
	savefile_->save();
	
}
void cSaveRanking::Init(IDirect3DDevice9 *pDev,const tstring DungeonID)
{
	
	savefile_ = pcSaveClass(new cSaveClass);
	savefile_->savename() = SAVEDATADIRCTORY + DungeonID + _T("_ranking.dat");
	savefile_->load();

	load(savefile_);
	DungeonID_ = DungeonID;
	
}
int cSaveRanking::rankingIn(pcSaveResult result)
{
	if(sg_pDungeonSystem->isRegureCharacter(DungeonID_, result->pFirstConditionOfPlayer_))
	{
		return rankingInInstance(result, RTARanking, TTARanking, SARanking);
	}
	else
	{
		return rankingInInstance(result, FreeStyle_RTARanking, FreeStyle_TTARanking, FreeStyle_SARanking);
	}
}

int cSaveRanking::rankingInInstance(pcSaveResult result, vector<cSaveResult>& RTARankingInstance, vector<cSaveResult>& TTARankingInstance, vector<cSaveResult>& SARankingInstance)
{
	int rankin = 0;
	
	int i;
	if(result->clearflag_ == cDungeonSystem::GAMECLEAR_CLEARFLAG)
	{
		for(i=0;i<RTARankingInstance.size();i++)
		{
			if(RTARankingInstance[i].SumFramerealtime_ > result->SumFramerealtime_)
			{
				RTARankingInstance.insert((RTARankingInstance.begin()+i),*result);
				rankin |= (i+1) << 0;
				break;
			}
		}
		if(RTARankingInstance.size() == i && i < RankingNum) RTARankingInstance.push_back(*result);
		if(RTARankingInstance.size() > RankingNum) RTARankingInstance.pop_back();


		for(i=0;i<TTARankingInstance.size();i++)
		{
			if(TTARankingInstance[i].Sumturn_ > result->Sumturn_)
			{
				TTARankingInstance.insert((TTARankingInstance.begin()+i),*result);
				rankin |= (i+1) << 8;
				break;
			}
		}
		if(TTARankingInstance.size() == i && i < RankingNum) TTARankingInstance.push_back(*result);
		if(TTARankingInstance.size() > RankingNum) TTARankingInstance.pop_back();
	}

	for(i=0;i<SARankingInstance.size();i++)
	{
		if(SARankingInstance[i].score_ < result->score_)
		{
			SARankingInstance.insert((SARankingInstance.begin()+i),*result);
			rankin |= (i+1) << 16;
			break;
		}
	}
	if(SARankingInstance.size() == i && i < RankingNum) SARankingInstance.push_back(*result);
	if(SARankingInstance.size() > RankingNum) SARankingInstance.pop_back();

	//すぐにセーブ
	save(savefile_);
	savefile_->save();

	
	return rankin;
}
int cSaveRanking::save(pcSaveClass data)
{
	
	//data->vv_char().resize(4);
	data->vv_char().resize(7);

	cDataConverter::ConvertVT2VecVecC2VecC(RTARanking,data->vv_char()[0]);
	cDataConverter::ConvertVT2VecVecC2VecC(TTARanking,data->vv_char()[1]);
	cDataConverter::ConvertVT2VecVecC2VecC(SARanking,data->vv_char()[2]);
	cDataConverter::ConvertVT2VecVecC2VecC(FreeResult,data->vv_char()[3]);

	cDataConverter::ConvertVT2VecVecC2VecC(FreeStyle_RTARanking,data->vv_char()[4]);
	cDataConverter::ConvertVT2VecVecC2VecC(FreeStyle_TTARanking,data->vv_char()[5]);
	cDataConverter::ConvertVT2VecVecC2VecC(FreeStyle_SARanking,data->vv_char()[6]);
	return true;
}
int cSaveRanking::load(const pcSaveClass data)
{
	
	//if(data->vv_char().size() < 4) return false;
	if(data->vv_char().size() >= 4)
	{
		cDataConverter::BackDecodeVecC2VecVecC2VT(RTARanking,data->vv_char()[0]);
		cDataConverter::BackDecodeVecC2VecVecC2VT(TTARanking,data->vv_char()[1]);
		cDataConverter::BackDecodeVecC2VecVecC2VT(SARanking,data->vv_char()[2]);
		cDataConverter::BackDecodeVecC2VecVecC2VT(FreeResult,data->vv_char()[3]);
	}
	if(data->vv_char().size() >= 7)
	{
		cDataConverter::BackDecodeVecC2VecVecC2VT(FreeStyle_RTARanking,data->vv_char()[4]);
		cDataConverter::BackDecodeVecC2VecVecC2VT(FreeStyle_TTARanking,data->vv_char()[5]);
		cDataConverter::BackDecodeVecC2VecVecC2VT(FreeStyle_SARanking,data->vv_char()[6]);
	}
	return true;
}


//-------------------------------------------------------
int ConvertT2VecC(const StyleString& tmpl, vector<SByte>& data)
{
	data.clear();
	vector<vector<SByte>> vvdata(5);
	cDataConverter::ConvertT2VecC(tmpl.conclete_tstr(),vvdata[0]);
	cDataConverter::Convert2VecC(tmpl.colors,vvdata[1]);
	cDataConverter::Convert2VecC(tmpl.sizes,vvdata[2]);
	cDataConverter::Convert2VecC(tmpl.sector,vvdata[3]);
	cDataConverter::Convert2VecC(tmpl.flags,vvdata[4]);

	cDataConverter::VecVecC2VecC(vvdata,data);
	return SUCCESS;
}
int DecodeVecC2T(const vector<SByte>& data, StyleString& tmpl)
{
	vector<vector<SByte>> vvdata;
	cDataConverter::VecC2VecVecC(data,vvdata);
	if(vvdata.size() != 5) return ERROR_FM_LOAD_DECODE_ERROR;

	tstring tstr;
	cDataConverter::DecodeVecC2T(vvdata[0],tstr);
	tmpl = tstr;
	cDataConverter::DecodeVecC(vvdata[1],tmpl.colors);
	cDataConverter::DecodeVecC(vvdata[2],tmpl.sizes);
	cDataConverter::DecodeVecC(vvdata[3],tmpl.sector);
	cDataConverter::DecodeVecC(vvdata[4],tmpl.flags);
	return SUCCESS;

}

int ConvertT2VecC(const pcDroping& tmpl, vector<SByte>& data)
{
	data.clear();
	vector<vector<SByte>> vvdata(1);
	if(tmpl == NULL)
	{
		int ID = 0;
		cDataConverter::ConvertT2VecC(ID,vvdata[0]);
		int presize = sizeof(int);
		vvdata[0].resize(presize+DEF_DROP_DATAPACK_UCHAR_NUM);
		vector<SByte>::iterator itr = vvdata[0].begin() + presize;
		int i;
		for(i=0;i<DEF_DROP_DATAPACK_UCHAR_NUM;i++,itr++)
		{
			*itr = static_cast<SByte>((UByte)0);
		}
	}
	else
	{
		cDataConverter::ConvertT2VecC(tmpl->ID(),vvdata[0]);
		int presize = sizeof(int);
		vvdata[0].resize(presize+DEF_DROP_DATAPACK_UCHAR_NUM);
		vector<SByte>::iterator itr = vvdata[0].begin() + presize;
		int i;
		for(i=0;i<DEF_DROP_DATAPACK_UCHAR_NUM;i++,itr++)
		{
			*itr = static_cast<SByte>(tmpl->dpack.uchar[i]);
		}
		vector<SByte> tempSByteVector;
		cDataConverter::ConvertT2VecC((int)tmpl->placeX,tempSByteVector);
		vvdata[0].insert(vvdata[0].end(),tempSByteVector.begin(),tempSByteVector.end());
		cDataConverter::ConvertT2VecC((int)tmpl->placeY,tempSByteVector);
		vvdata[0].insert(vvdata[0].end(),tempSByteVector.begin(),tempSByteVector.end());

		//内包アイテムあり
		if(!tmpl->includedItem.empty())
		{
			vvdata.resize(tmpl->includedItem.size()+1);
			for(i=0;i<tmpl->includedItem.size();i++)
			{
				ConvertT2VecC(tmpl->includedItem[i],vvdata[i+1]);
			}
		}
	}

	cDataConverter::VecVecC2VecC(vvdata,data);
	return SUCCESS;
}
int DecodeVecC2T(const vector<SByte>& data, pcDroping& tmpl)
{
	int presize = sizeof(int);
	vector<vector<SByte>> vvdata;
	cDataConverter::VecC2VecVecC(data,vvdata);
	if(vvdata.size() < 1)
	{
		tmpl = NULLDROP;
		return ERROR_FM_LOAD_DECODE_ERROR;
	}

	if(vvdata[0].size() < presize + DEF_DROP_DATAPACK_UCHAR_NUM) return ERROR_FM_LOAD_DECODE_ERROR;
	int i;
	vector<SByte> IDdata(vvdata[0].begin(), vvdata[0].begin()+presize);

	int ID;
	cDataConverter::DecodeVecC2T(IDdata,ID);
	tmpl = sg_pDungeonSystem->DataBase.GetNewDroping(ID);
	
	if(tmpl != NULL)
	{
		tmpl->Init(sg_pDungeonSystem->pDevice_D3D,tmpl);
		vector<SByte>::const_iterator itr = vvdata[0].begin() + presize;
		for(i=0;i<DEF_DROP_DATAPACK_UCHAR_NUM;i++,itr++)
		{
			tmpl->dpack.uchar[i] = static_cast<UByte>(*itr);
		}
	}
	else return SUCCESS;

	//120330追加
	if(vvdata[0].size() >= presize + DEF_DROP_DATAPACK_UCHAR_NUM + sizeof(int)*2)
	{
		vector<SByte> tempSByteVector;
		tempSByteVector.assign(vvdata[0].begin() + presize + DEF_DROP_DATAPACK_UCHAR_NUM,
								vvdata[0].begin() + presize + DEF_DROP_DATAPACK_UCHAR_NUM+sizeof(int));
		cDataConverter::BackDecodeVecC2T((int)tmpl->placeX,tempSByteVector);
		tempSByteVector.assign(vvdata[0].begin() + presize + DEF_DROP_DATAPACK_UCHAR_NUM+sizeof(int),
								vvdata[0].begin() + presize + DEF_DROP_DATAPACK_UCHAR_NUM+sizeof(int)*2);
		cDataConverter::BackDecodeVecC2T((int)tmpl->placeY,tempSByteVector);
	}

	//内包アイテムあり
	int includesize = vvdata.size() - 1;
	if(includesize > 0)
	{
		pcDroping pdrop;
		for(i=0;i<includesize;i++)
		{
			DecodeVecC2T(vvdata[i+1],pdrop);
			sg_pDungeonSystem->入れ子復元(tmpl,pdrop);
		}
	}

	return SUCCESS;
}

const int CHARACTER_EQUIPPART_ATTACK = 1;
const int CHARACTER_EQUIPPART_DEFENSE = 2;
const int CHARACTER_EQUIPPART_BULLET = 3;
const int CHARACTER_EQUIPPART_SPELL(int part){return part+16;};

int ConvertT2VecC(const pcCharacter& tmpl, vector<SByte>& data)
{
	data.clear();
	vector<vector<SByte>> vvdata(4);
	if(tmpl == NULL)
	{
		int ID = 0;
		cDataConverter::ConvertT2VecC(ID,vvdata[0]);
	}
	else
	{
		cDataConverter::ConvertT2VecC(tmpl->ID(),vvdata[0]);
		
		cSaveClass sc;

		sc.vv_int().resize(2);
		sc.vv_int()[0].resize(7);
		SUBSTITUTION_L2R(tmpl->Forse, sc.vv_int()[0][0]);
		SUBSTITUTION_L2R(tmpl->HP, sc.vv_int()[0][1]);
		SUBSTITUTION_L2R(tmpl->MHP, sc.vv_int()[0][2]);
		SUBSTITUTION_L2R(tmpl->LV, sc.vv_int()[0][3]);
		SUBSTITUTION_L2R(tmpl->CLASS, sc.vv_int()[0][4]);
		SUBSTITUTION_L2R(tmpl->VisibleCLASS, sc.vv_int()[0][5]);
		SUBSTITUTION_L2R(tmpl->EXP, sc.vv_int()[0][6]);

		sc.vv_int()[1].resize(AI_MEMORY_NUM);
		int i;
		for(i=0;i<AI_MEMORY_NUM;i++)
		{
			SUBSTITUTION_L2R(tmpl->AImemory[i], sc.vv_int()[1][i]);
		}

		sc.vv_double().resize(1);
		sc.vv_double()[0].resize(2);
		SUBSTITUTION_L2R(tmpl->Stomach, sc.vv_double()[0][0]);
		SUBSTITUTION_L2R(tmpl->MaxStomach, sc.vv_double()[0][1]);

		sc.convertData(vvdata[1]);

		cDataConverter::ConvertVT2VecVecC2VecC(tmpl->holdItem,vvdata[2]);

		vvdata[3].resize(tmpl->holdItem.size());
		int k;
		for(i=0;i<tmpl->holdItem.size();i++)
		{
			if(tmpl->holdItem[i] == tmpl->attackequipment) vvdata[3][i] = CHARACTER_EQUIPPART_ATTACK;
			else if(tmpl->holdItem[i] == tmpl->defenseequipment) vvdata[3][i] = CHARACTER_EQUIPPART_DEFENSE;
			else if(tmpl->holdItem[i] == tmpl->bulletequipment) vvdata[3][i] = CHARACTER_EQUIPPART_BULLET;
			
			for(k=0;k<tmpl->最大装備スペル数();k++)
			{
				if(tmpl->holdItem[i] == tmpl->spellequipment[k]) vvdata[3][i] = CHARACTER_EQUIPPART_SPELL(k);
			}
		}
	}

	cDataConverter::VecVecC2VecC(vvdata,data);
	return SUCCESS;
}
int DecodeVecC2T(const vector<SByte>& data, pcCharacter& tmpl)
{
	vector<vector<SByte>> vvdata;
	cDataConverter::VecC2VecVecC(data,vvdata);
	if(vvdata.size() < 4) return ERROR_FM_LOAD_DECODE_ERROR;

	int ID;
	cDataConverter::BackDecodeVecC2T(ID,vvdata[0]);
	tmpl = sg_pDungeonSystem->DataBase.GetNewCharacter(ID);

	if(tmpl != NULL)
	{
		tmpl->Init(sg_pDungeonSystem->pDevice_D3D,tmpl);
		cDataConverter::ConvertT2VecC(tmpl->ID(),vvdata[0]);
		tmpl->Condition.init(tmpl);
		
		cSaveClass sc;
		sc.decodeData(vvdata[1]);

		if(sc.vv_int().size() < 2) return ERROR_FM_LOAD_DECODE_ERROR;
		if(sc.vv_int()[0].size() < 7) return ERROR_FM_LOAD_DECODE_ERROR;
		SUBSTITUTION_R2L(tmpl->Forse, sc.vv_int()[0][0]);
		SUBSTITUTION_R2L(tmpl->HP, sc.vv_int()[0][1]);
		SUBSTITUTION_R2L(tmpl->MHP, sc.vv_int()[0][2]);
		SUBSTITUTION_R2L(tmpl->LV, sc.vv_int()[0][3]);
		SUBSTITUTION_R2L(tmpl->CLASS, sc.vv_int()[0][4]);
		SUBSTITUTION_R2L(tmpl->VisibleCLASS, sc.vv_int()[0][5]);
		SUBSTITUTION_R2L(tmpl->EXP, sc.vv_int()[0][6]);

		if(sc.vv_int()[1].size() < AI_MEMORY_NUM) return ERROR_FM_LOAD_DECODE_ERROR;
		int i;
		for(i=0;i<AI_MEMORY_NUM;i++)
		{
			SUBSTITUTION_R2L(tmpl->AImemory[i], sc.vv_int()[1][i]);
		}

		if(sc.vv_double().size() < 1) return ERROR_FM_LOAD_DECODE_ERROR;
		if(sc.vv_double()[0].size() < 2) return ERROR_FM_LOAD_DECODE_ERROR;
		SUBSTITUTION_R2L(tmpl->Stomach, sc.vv_double()[0][0]);
		SUBSTITUTION_R2L(tmpl->MaxStomach, sc.vv_double()[0][1]);



		vector<pcDroping> vdrop;
		cDataConverter::BackDecodeVecC2VecVecC2VT(vdrop,vvdata[2]);
		int holdsize = vdrop.size();
		if(holdsize > 0)
		{
			for(i=0;i<holdsize;i++)
			{
				sg_pDungeonSystem->入れ子復元(tmpl,vdrop[i]);
			}
		}

		if(vvdata[3].size() < holdsize) return ERROR_FM_LOAD_DECODE_ERROR;
		int k;
		for(i=0;i<holdsize;i++)
		{
			if(!vvdata[3][i]) continue;
			if(vvdata[3][i] == CHARACTER_EQUIPPART_ATTACK)
			{
				pcEquipment pcEqu = boost::dynamic_pointer_cast<cEquipment>(tmpl->holdItem[i]);
				if(pcEqu != NULL) pcEqu->equip(tmpl, EQUIP_PART_ATTACK);
			}
			else if(vvdata[3][i] == CHARACTER_EQUIPPART_DEFENSE)
			{
				pcEquipment pcEqu = boost::dynamic_pointer_cast<cEquipment>(tmpl->holdItem[i]);
				if(pcEqu != NULL) pcEqu->equip(tmpl, EQUIP_PART_DEFENSE);
			}
			else if(vvdata[3][i] == CHARACTER_EQUIPPART_BULLET)
			{
				pcBullet pcBlt = boost::dynamic_pointer_cast<cBullet>(tmpl->holdItem[i]);
				if(pcBlt != NULL) pcBlt->equip(tmpl);
			}
			
			for(k=0;k<tmpl->最大装備スペル数();k++)
			{
				if(vvdata[3][i] == CHARACTER_EQUIPPART_SPELL(k))
				{
					pcSpell pcSpl = boost::dynamic_pointer_cast<cSpell>(tmpl->holdItem[i]);
					if(pcSpl != NULL) pcSpl->equip(tmpl, k);
				}
			}
		}
	}

	
	return SUCCESS;
}

int ConvertT2VecC(const cSaveResult& tmpl, vector<SByte>& data)
{
	pcSaveClass psave = pcSaveClass(new cSaveClass);
	tmpl.save(psave);
	return psave->convertData(data);
}
int DecodeVecC2T(const vector<SByte>& data, cSaveResult& tmpl)
{
	pcSaveClass psave = pcSaveClass(new cSaveClass);
	psave->decodeData(data);
	return tmpl.load(psave);
}

int ConvertT2VecC(const pcMemoString& tmpl, vector<SByte>& data)
{
	tmpl->save(data);
	return SUCCESS;
}
int DecodeVecC2T(const vector<SByte>& data, pcMemoString& tmpl)
{
	tmpl = pcMemoString(new cMemoString);
	tmpl->load(data);
	return SUCCESS;
}
int ConvertT2VecC(const pcSaveStore& tmpl, vector<SByte>& data)
{
	pcSaveClass psave = pcSaveClass(new cSaveClass);
	tmpl->save(psave);
	return psave->convertData(data);
}
int DecodeVecC2T(const vector<SByte>& data, pcSaveStore& tmpl)
{
	pcSaveClass psave = pcSaveClass(new cSaveClass);
	psave->decodeData(data);
	return tmpl->load(psave);
}
