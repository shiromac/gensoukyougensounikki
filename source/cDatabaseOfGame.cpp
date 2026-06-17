#include "stdafx.h"

#include "cDatabaseOfGame.h"
#include "Gameobjects.h"

#include "Language.h"

#include "utility/debug.h"

#include "gameMainSystem/filemanage/cScriptRLayer.h"
#include "gameMainSystem/filemanage/cScriptReader.h"
#include "gameMainSystem/filemanage/cDataConverter.h"
#include <LuaUtility/LuaScript.h>

#include "cStringWindow.h"

#include "cSaveStore.h"

#include "cMapChipRule.h"

#include <boost/preprocessor.hpp>
#include <algorithm>

//データパックしない
//#define _UNPACK




#define DEF_GetCharaFunc_insert(z, n, data) \
GetCharaFunc.insert(pair<int, pcCharacter (*)() >\
(CCHARACTER_##data##_BASE_ID_NUM + n,\
		def_GetInstanceOf##data##_ID_##n));


#define DEF_GetDropFunc_insert(z, n, data) \
GetDropFunc.insert(pair<int, pcDroping (*)() >\
(CDROPING_##data##_BASE_ID_NUM + n,\
		def_GetInstanceOf##data##_ID_##n));



//new関数連続作成
BOOST_PP_REPEAT(PLAYERCHARA_NUM, DEF_FUNC_new_cCharacterClass, cPlayerChara)
BOOST_PP_REPEAT(MOB_NUM, DEF_FUNC_new_cCharacterClass, cMob)
BOOST_PP_REPEAT(OBJECTCHARA_NUM, DEF_FUNC_new_cCharacterClass, cObjectChara)
BOOST_PP_REPEAT(OTHERCHARA_NUM, DEF_FUNC_new_cCharacterClass, cOtherChara)
BOOST_PP_REPEAT(BOSS_NUM, DEF_FUNC_new_cCharacterClass, cBoss)
BOOST_PP_REPEAT(MOB_NUM, DEF_FUNC_new_cOtherChara_Event_Class, cOtherChara_Event)

BOOST_PP_REPEAT(EQUIPMENT_NUM, DEF_FUNC_new_cDropingClass, cEquipment)
BOOST_PP_REPEAT(BULLET_NUM, DEF_FUNC_new_cDropingClass, cBullet)

BOOST_PP_REPEAT(SPELL_NUM, DEF_FUNC_new_cDropingClass, cSpell)

BOOST_PP_REPEAT(FOOD_NUM, DEF_FUNC_new_cDropingClass, cFood)
BOOST_PP_REPEAT(DRINK_NUM, DEF_FUNC_new_cDropingClass, cDrink)

BOOST_PP_REPEAT(BOOK_NUM, DEF_FUNC_new_cDropingClass, cBook)

BOOST_PP_REPEAT(BOX_NUM, DEF_FUNC_new_cDropingClass, cBox)

BOOST_PP_REPEAT(MONEYBAG_NUM, DEF_FUNC_new_cDropingClass, cMoneyBag)

BOOST_PP_REPEAT(TRAP_NUM, DEF_FUNC_new_cDropingClass, cTrap)

BOOST_PP_REPEAT(STAIR_NUM, DEF_FUNC_new_cDropingClass, cStair)
BOOST_PP_REPEAT(THING_NUM, DEF_FUNC_new_cDropingClass, cThing)

cDatabaseOfGame::cDatabaseOfGame(void)
{

}

cDatabaseOfGame::~cDatabaseOfGame(void)
{
	CharaDataBase.clear();
	DropDataBase.clear();
	GetCharaFunc.clear();
	GetDropFunc.clear();

	CharaFindNametoID.clear();

	DropFindNametoID.clear();

}

int cDatabaseOfGame::Init(cRenderDevice *pDev)
{
	//最初に
	InitImportData();

	cCondition::InitChipstr();

	//playerchara

	//キャラ確保関数リスト作成
	BOOST_PP_REPEAT(PLAYERCHARA_NUM, DEF_GetCharaFunc_insert, cPlayerChara)
	BOOST_PP_REPEAT(MOB_NUM, DEF_GetCharaFunc_insert, cMob)
	BOOST_PP_REPEAT(OBJECTCHARA_NUM, DEF_GetCharaFunc_insert, cObjectChara)
	BOOST_PP_REPEAT(OTHERCHARA_NUM, DEF_GetCharaFunc_insert, cOtherChara)
	BOOST_PP_REPEAT(BOSS_NUM, DEF_GetCharaFunc_insert, cBoss)
	BOOST_PP_REPEAT(MOB_NUM, DEF_GetCharaFunc_insert, cOtherChara_Event)


	//キャラ見本リスト作成
	map<int, pcCharacter (*)() >::iterator itr;
	itr = GetCharaFunc.begin();
	//CharaDataBase.clear();
	//CharaFindNametoID.clear();
	for(;itr!=GetCharaFunc.end();itr++)
	{
		pcCharacter pchara = itr->second();
		pchara->InitSub(pDev);
		CharaDataBase.insert(pair<int, pcCharacter>(pchara->ID(),pchara));

		CharaFindNametoID.insert(pair<tstring, int>(pchara->uniqueName(),pchara->ID()));
		CharaFindNametoID.insert(pair<tstring, int>(pchara->usefulName(),pchara->ID()));
		//CharaFindNametoID.insert(pair<tstring, int>(pchara->ShortName(),pchara->ID()));
		//CharaFindNametoID.insert(pair<tstring, int>(pchara->ShortNameSub(),pchara->ID()));
		//CharaFindNametoID.insert(pair<tstring, int>(pchara->FullName(),pchara->ID()));
		//CharaFindNametoID.insert(pair<tstring, int>(pchara->FullNameSub(),pchara->ID()));
		//CharaFindNametoID.insert(pair<tstring, int>(pchara->titleName(),pchara->ID()));
	}


	//落ち物確保関数リスト作成
	BOOST_PP_REPEAT(EQUIPMENT_NUM, DEF_GetDropFunc_insert, cEquipment)
	BOOST_PP_REPEAT(BULLET_NUM, DEF_GetDropFunc_insert, cBullet)
 	BOOST_PP_REPEAT(SPELL_NUM, DEF_GetDropFunc_insert, cSpell)
	BOOST_PP_REPEAT(FOOD_NUM, DEF_GetDropFunc_insert, cFood)
	BOOST_PP_REPEAT(DRINK_NUM, DEF_GetDropFunc_insert, cDrink)
 	BOOST_PP_REPEAT(BOOK_NUM, DEF_GetDropFunc_insert, cBook)
 	BOOST_PP_REPEAT(BOX_NUM, DEF_GetDropFunc_insert, cBox)
	BOOST_PP_REPEAT(MONEYBAG_NUM, DEF_GetDropFunc_insert, cMoneyBag)
	BOOST_PP_REPEAT(TRAP_NUM, DEF_GetDropFunc_insert, cTrap)


	BOOST_PP_REPEAT(STAIR_NUM, DEF_GetDropFunc_insert, cStair)
 	BOOST_PP_REPEAT(THING_NUM, DEF_GetDropFunc_insert, cThing)

	//落ち物見本リスト作成
	map<int, pcDroping (*)() >::iterator itr2;
	itr2 = GetDropFunc.begin();
	//DropDataBase.clear();
	//DropFindNametoID.clear();
	for(;itr2!=GetDropFunc.end();itr2++)
	{
		pcDroping pdrop = itr2->second();
		pdrop->Init(pDev,pdrop);
		DropDataBase.insert(pair<int, pcDroping>(pdrop->ID(),pdrop));

		DropFindNametoID.insert(pair<tstring, int>(pdrop->usefulName(),pdrop->ID()));
		DropFindNametoID.insert(pair<tstring, int>(pdrop->ShortNameBase(),pdrop->ID()));
		DropFindNametoID.insert(pair<tstring, int>(pdrop->FullNameBase(),pdrop->ID()));

	}


	InitDropUnknownNamePool();

	InitDropItemMemoPool();

	ImportDungeonFile();
	ImportMapFile();
	ImportHomeDataBase();

	return true;
}
int cDatabaseOfGame::ImportHomeDataBase()
{
	int error = 0;
	cScriptReader SR(IMPORTDATADIR + _T("HomeDataBase.id"));

#ifdef _UNRELEASE
#ifndef _UNPACK
	error |= SR.loadAndpacked();
#else
	error |= SR.loadpacked();
	if(error) exit(0);//ゲーム終了
#endif
#else
	error |= SR.loadpacked();
	if(error) exit(0);//ゲーム終了
#endif

	SR.outputerrormassage(tstring(_T("HomeDataBase_log.txt")));

	tstring strstory(_T("拠点イベント"));
	pcScriptRLayer dungeonstory = SR.pdata()->pmember(strstory);
	if(dungeonstory != NULL)
	{
		dungeonstory->getLuaScriptKeys(homeEventKeys_);
		cValiableField valiable;

		int i, size = homeEventKeys_.size();
		homeEventEnvironments_.resize(size);
		for(i=0;i<size;i++)
		{
			if(!homeEventEnvironments_[i].loadDungeonStoryScript(dungeonstory,homeEventKeys_[i],valiable))
			{
				OnAssert(_T(__FILE__),__LINE__,false,_T("ストーリースクリプトのロードに失敗しました　error. failed to load \"ストーリースクリプト\"："),homeEventKeys_[i].c_str());
			}
		}
		
	}

	HomeDataBase_ = SR.pdata();

	return true;
}
pcScriptRLayer cDatabaseOfGame::HomeDataBase()
{
	return HomeDataBase_;
}
int cDatabaseOfGame::ImportDungeonFile()
{
	int error = 0;
	cScriptReader SR(IMPORTDATADIR + _T("Dungeon.id"));


#ifdef _UNRELEASE
#ifndef _UNPACK
	error |= SR.loadAndpacked();
#else
	error |= SR.loadpacked();
	if(error) exit(0);//ゲーム終了
#endif
#else
	error |= SR.loadpacked();
	if(error) exit(0);//ゲーム終了
#endif

	SR.outputerrormassage(tstring(_T("Dungeon_log.txt")));

	pcScriptRLayer player = SR.pdata()->pmember(_T("DungeonList"));
	if(player != NULL)
	{
		map<tstring, pcScriptRLayer>::iterator itr = player->membermap().begin();
		for(;itr!=player->membermap().end();itr++)
		{
			pcScriptRLayer pName = itr->second->pmember(_T("Name"));
			if(pName != NULL)
			{
				DungeonImportDataName_.insert(pair<tstring,tstring>(itr->first,pName->topscript()));
			}
			pcScriptRLayer pFilename = itr->second->pmember(_T("FormFile"));
			if(pFilename != NULL)
			{
				DungeonImportDataFile_.insert(pair<tstring,pcScriptRLayer>(itr->first,pFilename));
			}

			pcSaveRanking prank = pcSaveRanking(new cSaveRanking);
			prank->Init(sg_pDungeonSystem->pDevice_D3D,itr->first);
			DungeonRanking_.insert(pair<tstring,pcSaveRanking>(itr->first,prank));
		}
	
	}

	map<tstring, pcScriptRLayer>::iterator itr = DungeonImportDataFile_.begin();
	for(;itr!=DungeonImportDataFile_.end();itr++)
	{
		pcScriptRLayer data = itr->second;
		//-----------------------------------------------------
		//レギュレーションスクリプト
		tstring strregulation(_T("レギュレーションスクリプト"));
		pcScriptRLayer dungeonregulation = data->pmember(strregulation);
		if(dungeonregulation != NULL)
		{
			std::vector<tstring> regulationScriptKeys;
			dungeonregulation->getLuaScriptKeys(regulationScriptKeys);

			cValiableField valiable;

			if(!regulationScriptKeys.empty())
			{
				pDungeonStoryScript_LuaEnvironment regulationScriptEnvironment = pDungeonStoryScript_LuaEnvironment(new DungeonStoryScript_LuaEnvironment(tString2luastring(regulationScriptKeys[0])));
				if(!regulationScriptEnvironment->loadDungeonStoryScript(dungeonregulation,regulationScriptKeys[0],valiable))
				{
					OnAssert(_T(__FILE__),__LINE__,false,_T("レギュレーションスクリプトのロードに失敗しました　error. failed to load \"レギュレーションスクリプト\"："),regulationScriptKeys[0].c_str());
				}
				DungeonRegulationScriptEnvironment_.insert(pair<tstring,pDungeonStoryScript_LuaEnvironment>(itr->first,regulationScriptEnvironment));
			}
		}
	}
	return true;
}
int cDatabaseOfGame::ImportMapFile()
{
	int error = 0;
	cScriptReader SR(IMPORTDATADIR + _T("Mapchip.id"));


#ifdef _UNRELEASE
#ifndef _UNPACK
	error |= SR.loadAndpacked();
#else
	error |= SR.loadpacked();
	if(error) exit(0);//ゲーム終了
#endif
#else
	error |= SR.loadpacked();
	if(error) exit(0);//ゲーム終了
#endif

	SR.outputerrormassage(tstring(_T("Mapchip_log.txt")));

	pcScriptRLayer player = SR.pdata()->pmember(_T("MapchipList"));
	if(player != NULL)
	{
		MapImportDataFile_ = player->membermap();
		
		map<tstring, pcScriptRLayer>::iterator itr = player->membermap().begin();
		
		for(;itr!=player->membermap().end();itr++)
		{

			//music
			tstring Music_in, Music_roop;
			pcScriptRLayer pMusic = itr->second->pmember(_T("Music_in"));
			if(pMusic != NULL)
			{
				Music_in = pMusic->topscript();
			}
			pMusic = itr->second->pmember(_T("Music_roop"));
			if(pMusic != NULL)
			{
				Music_roop = pMusic->topscript();
			}
			MapChipFormImportDataMusicFile_.insert(pair<tstring,pair<tstring,tstring>>(itr->first,pair<tstring,tstring>(Music_in,Music_roop)));
		
			//Mainchip
			pcScriptRLayer pMapchipFile = itr->second->pmember(_T("Mainchip"));
			if(pMapchipFile != NULL)
			{
				MapChipFormImportDataMapchipFile_.insert(pair<tstring,tstring>(itr->first,pMapchipFile->topscript()));
			}
			//Decochip
			pcScriptRLayer pDecochipFile = itr->second->pmember(_T("Decochip"));
			if(pMapchipFile != NULL)
			{
				MapChipFormImportDataMapchipDecoFile_.insert(pair<tstring,tstring>(itr->first,pDecochipFile->topscript()));
			}
		
			//DecoRule
			pcScriptRLayer pDecoRule = itr->second->pmember(_T("DecorationRule"));
			if(pDecoRule != NULL)
			{
				map<tstring, pcScriptRLayer>::iterator itrdeco = pDecoRule->membermap().begin();
		
				vector<pcMapChipRule> vrule;
				for(;itrdeco!=pDecoRule->membermap().end();itrdeco++)
				{
					pcMapChipRule pmcr = pcMapChipRule(new cMapChipRule);
					if(!pmcr->Init(itrdeco->second))
					{
						OnAssert(itrdeco->second->name().c_str(),0,false,_T("DecorationRule内でエラーが発生しました。occur an error in DecorationRule"));
					}
					vrule.push_back(pmcr);
				}
				MapImportDataChipDecoRule_.insert(pair<tstring,vector<pcMapChipRule>>(itr->first,vrule));
				
			}
			//BaseChipAnimation
			pcScriptRLayer pBaseAnime = itr->second->pmember(_T("BaseChipAnimation"));
			if(pBaseAnime != NULL)
			{
				pcMapChipAnimeAll pmcaa = pcMapChipAnimeAll(new cMapChipAnimeAll);
				pmcaa->Init(pBaseAnime);
				MapImportDataChipAnime_.insert(pair<tstring,pcMapChipAnimeAll>(itr->first,pmcaa));
			}
			else
			{
				pcMapChipAnimeAll pmcaa = pcMapChipAnimeAll(new cMapChipAnimeAll);
				MapImportDataChipAnime_.insert(pair<tstring,pcMapChipAnimeAll>(itr->first,pmcaa));
			}
		}
	
	}

	return true;
}

tstring cDatabaseOfGame::DungeonImportDataName(tstring ID)
{

	return g_Lang((DungeonImportDataName_[ID]).c_str()).conclete_tstr();
}
pcScriptRLayer cDatabaseOfGame::DungeonImportDataFile(tstring ID)
{

	return DungeonImportDataFile_[ID];
}
pcSaveRanking cDatabaseOfGame::DungeonRanking(tstring ID)
{

	return DungeonRanking_[ID];
}
pDungeonStoryScript_LuaEnvironment cDatabaseOfGame::DungeonRegulationScriptEnvironment(tstring ID)
{
	map<tstring,pDungeonStoryScript_LuaEnvironment>::iterator itr = DungeonRegulationScriptEnvironment_.find(ID);
	if (itr == DungeonRegulationScriptEnvironment_.end())
	{
		return pDungeonStoryScript_LuaEnvironment((DungeonStoryScript_LuaEnvironment*)NULL);
	}
	return itr->second;
}

pair<tstring,tstring> cDatabaseOfGame::MapChipFormImportDataMusicFile(tstring ID)
{
	return MapChipFormImportDataMusicFile_[ID];
}
tstring cDatabaseOfGame::MapChipFormImportDataMapchipFile(tstring ID)
{
	return MapChipFormImportDataMapchipFile_[ID];
}
tstring cDatabaseOfGame::MapChipFormImportDataMapchipDecoFile(tstring ID)
{
	return MapChipFormImportDataMapchipDecoFile_[ID];
}
pcScriptRLayer cDatabaseOfGame::MapImportDataFile(tstring ID)
{
	return MapImportDataFile_[ID];
}
vector<pcMapChipRule>& cDatabaseOfGame::MapImportDataChipDecoRule(tstring ID)
{
	return MapImportDataChipDecoRule_[ID];
}
pcMapChipAnimeAll cDatabaseOfGame::MapImportDataChipAnimeAll(tstring ID)
{
	return MapImportDataChipAnime_[ID];
}

pcCharacter cDatabaseOfGame::GetNewCharacter(int ID)
{
	map<int, pcCharacter (*)() >::iterator itr;
 
	itr = GetCharaFunc.find(ID);

	if(itr != GetCharaFunc.end())
	{
		return itr->second();
	}
	else
	{
		return NULLCHARA;
	}
}
pcCharacter cDatabaseOfGame::GetNewCharacter(tstring str)
{
	return GetNewCharacter(CharaFindNametoID[str]);
}

pcDroping cDatabaseOfGame::GetNewDroping(int ID)
{
	map<int, pcDroping (*)() >::iterator itr;
 
	itr = GetDropFunc.find(ID);

	if(itr != GetDropFunc.end())
	{
		return itr->second();
	}
	else
	{
		return NULLDROP;
	}
}

pcDroping cDatabaseOfGame::GetNewDroping(tstring str)
{
	return GetNewDroping(DropFindNametoID[str]);
}

pcDroping cDatabaseOfGame::NewCopyDroping(pcDroping pdrop)
{
	if(pdrop == NULL) return NULLDROP;
	pcDroping newdrop = GetNewDroping(pdrop->ID());
	newdrop->Init(sg_pDungeonSystem->pDevice_D3D, newdrop);
	int i;
	for(i=0;i<DEF_DROP_DATAPACK_NUM;i++)
	{
		newdrop->dpack.integer[i] = pdrop->dpack.integer[i];
	}
	for(i=0;i<pdrop->includedItem.size();i++)
	{
		sg_pDungeonSystem->入れ子復元(newdrop, NewCopyDroping(pdrop->includedItem[i]));
	}
	return newdrop;
}
pcCharacter cDatabaseOfGame::NewCopyCharacter(pcCharacter pchara)
{
	if(pchara == NULL) return NULLCHARA;
	pcCharacter newchara = GetNewCharacter(pchara->ID());
	newchara->Init(sg_pDungeonSystem->pDevice_D3D, newchara);
	newchara->Condition.init(newchara);

	newchara->Forse = pchara->Forse;
	newchara->HP = pchara->HP;
	newchara->MHP = pchara->MHP;
	newchara->LV = pchara->LV;
	newchara->CLASS = pchara->CLASS;
	newchara->VisibleCLASS = pchara->VisibleCLASS;
	newchara->EXP = pchara->EXP;
	newchara->Stomach = pchara->Stomach;
	newchara->MaxStomach = pchara->MaxStomach;
	
	int i,k;
	for(i=0;i<AI_MEMORY_NUM;i++)
	{
		newchara->AImemory[i] = pchara->AImemory[i];
	}

	int holdsize = pchara->holdItem.size();

	for(i=0;i<holdsize;i++)
	{
		sg_pDungeonSystem->入れ子復元(newchara,NewCopyDroping(pchara->holdItem[i]));
	}
	
	for(i=0;i<holdsize;i++)
	{
		if(pchara->holdItem[i] == pchara->attackequipment)
		{
			pcEquipment pcEqu = boost::dynamic_pointer_cast<cEquipment>(newchara->holdItem[i]);
			if(pcEqu != NULL) pcEqu->equip(newchara, EQUIP_PART_ATTACK);
		}
		else if(pchara->holdItem[i] == pchara->defenseequipment)
		{
			pcEquipment pcEqu = boost::dynamic_pointer_cast<cEquipment>(newchara->holdItem[i]);
			if(pcEqu != NULL) pcEqu->equip(newchara, EQUIP_PART_DEFENSE);
		}
		else if(pchara->holdItem[i] == pchara->bulletequipment)
		{
			pcBullet pcBlt = boost::dynamic_pointer_cast<cBullet>(newchara->holdItem[i]);
			if(pcBlt != NULL) pcBlt->equip(newchara);
		}
		
		for(k=0;k<newchara->最大装備スペル数();k++)
		{
			if(pchara->holdItem[i] == pchara->spellequipment[k])
			{
				pcSpell pcSpl = boost::dynamic_pointer_cast<cSpell>(newchara->holdItem[i]);
				if(pcSpl != NULL) pcSpl->equip(newchara, k);
			}
		}
	}
	
	return newchara;
}


int cDatabaseOfGame::makeAllCharaList(vector<pcCharacter>& list)
{
	map<int, pcCharacter>::iterator itr;
	itr = CharaDataBase.begin();
	for(;itr!=CharaDataBase.end();itr++)
	{
		list.push_back(itr->second);
	}

	return true;
}

int cDatabaseOfGame::makeAllDropList(vector<pcDroping>& list)
{
	map<int, pcDroping>::iterator itr;
	itr = DropDataBase.begin();
	for(;itr!=DropDataBase.end();itr++)
	{
		list.push_back(itr->second);
	}

	return true;
}


pcCharacter cDatabaseOfGame::GetSampleCharacter(int ID)
{
	map<int, pcCharacter>::iterator itr;
 
	itr = CharaDataBase.find(ID);

	if(itr != CharaDataBase.end())
	{
		return itr->second;
	}
	else
	{
		return NULLCHARA;
	}
}
pcCharacter cDatabaseOfGame::GetSampleCharacter(tstring str)
{
	return GetSampleCharacter(CharaFindNametoID[str]);
}

pcDroping cDatabaseOfGame::GetSampleDroping(int ID)
{
	map<int, pcDroping>::iterator itr;
 
	itr = DropDataBase.find(ID);

	if(itr != DropDataBase.end())
	{
		return itr->second;
	}
	else
	{
		return NULLDROP;
	}
}
pcDroping cDatabaseOfGame::GetSampleDroping(tstring str)
{
	return GetSampleDroping(DropFindNametoID[str]);
}




//-------------------------------------
//検索
//-------------------------------------

int cDatabaseOfGame::makeDropIDList(vector<int>& IDlist, set<int> condition_and)
{
	IDlist.clear();
	int i;

	map<int, pcDroping>::iterator itr;
	itr = DropDataBase.begin();
	for(;itr!=DropDataBase.end();itr++)
	{//全ドロップループ
		

		int match = true;

		set<int>::iterator sitr;
		sitr = condition_and.begin();
		for(;sitr!=condition_and.end();sitr++)
		{//全条件ループ
			
			if(itr->second->属性.count(*sitr))
			{
			
			}
			else
			{
				match = false;
				break;
			}
		}

		if(match)
		{
			IDlist.push_back(itr->second->ID());
		}

	}

	return true;
}

//-------------------------------------
//アイテム系

int cDatabaseOfGame::ImportDropFileData(tstring type)
{
	int error = 0;
	cScriptReader SR(IMPORTDATADIR+type);


#ifdef _UNRELEASE
#ifndef _UNPACK
	error |= SR.loadAndpacked();
#else
	error |= SR.loadpacked();
	if(error) exit(0);//ゲーム終了
#endif
#else
	error |= SR.loadpacked();
	if(error) exit(0);//ゲーム終了
#endif

	SR.outputerrormassage(type +tstring(_T("_ImportDropFileData_Errorlog.txt")));
	if(error) return false;//エラー

	//親レイヤー保存
	DropImportDataParentLayer_ = SR.pdata();

	/*
	vector<tstring> tablestr;
	tablestr.push_back(_T("EquipmentTable"));
	tablestr.push_back(_T("BookTable"));
	tablestr.push_back(_T("DrinkTable"));
	tablestr.push_back(_T("BulletTable"));
	tablestr.push_back(_T("BoxTable"));
	tablestr.push_back(_T("SpellTable"));
	tablestr.push_back(_T("FoodTable"));
	tablestr.push_back(_T("TrapTable"));
	//表デコード
	int i;
	for(i=0;i<tablestr.size();i++)
	{
		pcScriptRLayer psr = DropImportDataParentLayer_->pmember(tablestr[i]);
		if(psr == NULL) continue;
		afterdecode_table(psr->script(),_T(";"),DropImportDataParentLayer_);
	}
	*/

	//メンバ(ID,レイヤー)を取得する
	map<tstring,pcScriptRLayer>::iterator itr = SR.data().membermap().begin();
	DropImportDataPool.clear();
	for(;itr != SR.data().membermap().end();itr++)
	{
		int id = _tstoi (itr->first.c_str());
		DropImportDataPool.insert(pair<int, pcScriptRLayer>(id, itr->second));
	}
	return true;
}

int cDatabaseOfGame::InitImportData()
{
	DropImportDataPool.clear();
	cScriptReader SR;

	//仕様変更により一度しか呼んではいけない
	ImportDropFileData(_T("Item.id"));

	ImportDropFileLanguage(_T("ItemLanguage.id"));

	ImportCharaFileData(_T("Chara.id"));

	ImportCharaFileLanguage(_T("CharaLanguage.id"));

	ImportEffectFile(_T("Effect.id"));

	//インポート終わってから実行
	DecodeDropFileData();
	DecodeCharaFileData();
	return true;
}

int cDatabaseOfGame::ReImportData()
{
	InitImportData();

	//キャラ見本リスト作成
	map<int, pcCharacter (*)() >::iterator itr;
	itr = GetCharaFunc.begin();
	for(;itr!=GetCharaFunc.end();itr++)
	{
		pcCharacter pchara = itr->second();
		pchara->InitSub(sg_pDungeonSystem->pDevice_D3D);
	}

	//落ち物見本リスト作成
	map<int, pcDroping (*)() >::iterator itr2;
	itr2 = GetDropFunc.begin();
	for(;itr2!=GetDropFunc.end();itr2++)
	{
		pcDroping pdrop = itr2->second();
		pdrop->Init(sg_pDungeonSystem->pDevice_D3D,pdrop);
	}


	return true;
}

double cDatabaseOfGame::def_decode_value(pcScriptRLayer psr, double defaultval, int index)
{
	if(psr == NULL) return defaultval;

	if(psr->script().size() <= index)
	{//未定義
		return defaultval;
	}

	/*
	double res;
	TCHAR* e;
	const tstring & code = psr->script().at(index);

	res = _tcstod(code.c_str(),&e);
	
	if(!(code.length() != 0))
	{
		OnAssert(_T(__FILE__),__LINE__, false , psr->nameAllPass().c_str(),
			((tstring)_T(" index ")+ setStyle(index) +_T(" に　空欄　が設定されています。<error> There is a blank space.")).c_str());
	}
	if(!((_tcslen(e) == 0) && ( !(res == 0 && _tcslen(e) == 0) || (code == _T("0")))))
	{
		OnAssert(_T(__FILE__),__LINE__, false , psr->nameAllPass().c_str(),
			((tstring)_T(" index ")+ setStyle(index) +_T(" に　解釈不能な数値 「") + code + _T("」 が設定されています。<error> Cannot understand it.")).c_str());
	}
	return res;
	*/
	return tstrtod(psr->script().at(index),psr->nameAllPass(),index);

}
int cDatabaseOfGame::def_decode_value_size(pcScriptRLayer psr)
{
	if(psr == NULL) return 0;

	return psr->script().size();
}

void cDatabaseOfGame::def_decode_Dropattri(pcScriptRLayer psr, set<落ち物属性::落ち物属性>& setint)
{
	if(psr == NULL) return;
	int size = psr->script().size();
	int i;
	for(i=0;i<size;i++)
	{
		setint.insert(dropattrS2I(psr->script()[i]));
	}
}
void cDatabaseOfGame::def_decode_Charaattri(pcScriptRLayer psr, set<キャラ属性::キャラ属性リスト>& setint)
{
	if(psr == NULL) return;
	int size = psr->script().size();
	int i;
	for(i=0;i<size;i++)
	{
		setint.insert(charattrS2I(psr->script()[i]));
	}
}
void cDatabaseOfGame::def_decode_Attackattri(pcScriptRLayer psr, multiset<攻撃属性::攻撃属性>& setint)
{
	if(psr == NULL) return;
	int size = psr->script().size();
	int i;
	for(i=0;i<size;i++)
	{
		setint.insert(attackattrS2I(psr->script()[i]));
	}
}


#define DEF_STR_TO_PRISE(sty,lty) \
	DropPricePool.insert(pair<tstring,int>(tstring(_T(#sty))+tstring(_T("_SP")),PRICE_##lty##_SP));\
	DropPricePool.insert(pair<tstring,int>(tstring(_T(#sty))+tstring(_T("_SSS")),PRICE_##lty##_SSS));\
	DropPricePool.insert(pair<tstring,int>(tstring(_T(#sty))+tstring(_T("_SS")),PRICE_##lty##_SS));\
	DropPricePool.insert(pair<tstring,int>(tstring(_T(#sty))+tstring(_T("_S")),PRICE_##lty##_S));\
	DropPricePool.insert(pair<tstring,int>(tstring(_T(#sty))+tstring(_T("_A")),PRICE_##lty##_A));\
	DropPricePool.insert(pair<tstring,int>(tstring(_T(#sty))+tstring(_T("_B")),PRICE_##lty##_B));\
	DropPricePool.insert(pair<tstring,int>(tstring(_T(#sty))+tstring(_T("_C")),PRICE_##lty##_C));\
	DropPricePool.insert(pair<tstring,int>(tstring(_T(#sty))+tstring(_T("_D")),PRICE_##lty##_D));\
	DropPricePool.insert(pair<tstring,int>(tstring(_T(#sty))+tstring(_T("_E")),PRICE_##lty##_E));\
	DropPricePool.insert(pair<tstring,int>(tstring(_T(#sty))+tstring(_T("_F")),PRICE_##lty##_F));\
	DropPricePool.insert(pair<tstring,int>(tstring(_T(#sty))+tstring(_T("_E")),PRICE_##lty##_G));\
	DropPricePool.insert(pair<tstring,int>(tstring(_T(#sty))+tstring(_T("_F")),PRICE_##lty##_H));


#define DEF_DropAttrlistInsert(str) \
dropattrStr2Intlist.insert(pair<tstring,落ち物属性::落ち物属性>(_T(#str),落ち物属性::str));\
dropattrInt2Strlist.insert(pair<落ち物属性::落ち物属性,tstring>(落ち物属性::str,_T(#str)));

#define DEF_AttackAttrlistInsert(str) \
attackattrStr2Intlist.insert(pair<tstring,攻撃属性::攻撃属性>(_T(#str),攻撃属性::str));\
attackattrInt2Strlist.insert(pair<攻撃属性::攻撃属性,tstring>(攻撃属性::str,_T(#str)));

int cDatabaseOfGame::DecodeDropFileData()
{

	/*
	vector<tstring> tablestr;
	tablestr.push_back(_T("EquipmentTable"));
	//表デコード
	int i;
	for(i=0;i<tablestr.size();i++)
	{
		pcScriptRLayer psr = DropImportLanguageParentLayer_->pmember(tablestr[i]);
		if(psr == NULL) continue;
		vector<vector<tstring>> vvstr;
		split(psr->script(),_T("|"),vvstr);

		vector<tstring> columnstr;//列要素
		int x,y;
		for(x=0;x<vvstr[0].size();x++)
		{
			columnstr.push_back(trimstring(vvstr[0][x]));
		}

		for(y=1;y<vvstr.size();y++)
		{
			tstring idstr = trimstring(vvstr[y][0]);
			int id = tstrtoi(idstr);
			pcScriptRLayer idpsr = DropImportLanguagePool[id];
			
			int size = min(columnstr.size(),vvstr[y].size());
			for(x=1;x<size;x++)
			{
				idpsr->addMember(columnstr[x],idpsr)->script().push_back(trimstring(vvstr[y][x]));
			}
		}
	}
*/

	DropImportPricePool.clear();
	DropImportRarityPool.clear();

	map<tstring,int> DropPricePool;
	map<tstring,double> DropRarityPool;
	DEF_STR_TO_PRISE(BOK,BOOK)
	DEF_STR_TO_PRISE(BOX,BOX)
	DEF_STR_TO_PRISE(BLT,BULLET)
	DEF_STR_TO_PRISE(DRK,DRINK)
	DEF_STR_TO_PRISE(EQU,EQUIPMENT)
	DEF_STR_TO_PRISE(FOD,FOOD)
	DEF_STR_TO_PRISE(SPL,SPELL)

	DropRarityPool.insert(pair<tstring,double>(_T("SP"),RARITY_SP));
	DropRarityPool.insert(pair<tstring,double>(_T("SSS"),RARITY_SSS));
	DropRarityPool.insert(pair<tstring,double>(_T("SS"),RARITY_SS));
	DropRarityPool.insert(pair<tstring,double>(_T("S"),RARITY_S));
	DropRarityPool.insert(pair<tstring,double>(_T("A"),RARITY_A));
	DropRarityPool.insert(pair<tstring,double>(_T("B"),RARITY_B));
	DropRarityPool.insert(pair<tstring,double>(_T("C"),RARITY_C));
	DropRarityPool.insert(pair<tstring,double>(_T("D"),RARITY_D));
	DropRarityPool.insert(pair<tstring,double>(_T("E"),RARITY_E));
	DropRarityPool.insert(pair<tstring,double>(_T("F"),RARITY_F));
	DropRarityPool.insert(pair<tstring,double>(_T("G"),RARITY_G));
	DropRarityPool.insert(pair<tstring,double>(_T("H"),RARITY_H));

	map<int,pcScriptRLayer>::iterator itr = DropImportDataPool.begin();
	pcScriptRLayer sr;	
	
	tstring pricestr = _T("値段基礎値");
	tstring raritystr = _T("レア度");
	tstring usefulnamestr = _T("usefulNameBase");
	for(;itr!=DropImportDataPool.end();itr++)
	{
		int id = itr->first;

		//値段
		sr = itr->second;
		sr = sr->pmember(pricestr);
		if(sr != NULL)
		{
			tstring scr = sr->topscript();
			map<tstring,int>::iterator priceitr = DropPricePool.find( scr );
			if(priceitr != DropPricePool.end())
			{//リストにある(定義値)
				DropImportPricePool.insert( pair<int,int>(id, priceitr->second));
			}
			else
			{//リストにない(即値)
				DropImportPricePool.insert( pair<int,int>(id, _tstoi( scr.c_str() )));
			}
		}

		//レア度
		sr = itr->second;
		sr = sr->pmember(raritystr);
		if(sr != NULL)
		{
			tstring scr = sr->topscript();
			map<tstring,double>::iterator rarityitr = DropRarityPool.find( scr );
			if(rarityitr != DropRarityPool.end())
			{//リストにある(定義値)
				DropImportRarityPool.insert( pair<int,double>(id, rarityitr->second));
			}
			else
			{//リストにない(即値)
				DropImportRarityPool.insert( pair<int,double>(id, _tstof( scr.c_str() )));
			}
		}

		//usefulname
		sr = itr->second;
		sr = sr->pmember(usefulnamestr);
		if(sr != NULL)
		{
			DropIDtousefulName_[id] = sr->topscript();
		}
	}

	//属性
	DEF_DropAttrlistInsert(無効エラー)
	DEF_DropAttrlistInsert(物体)
	DEF_DropAttrlistInsert(アイテム)
	DEF_DropAttrlistInsert(飲み物)
	DEF_DropAttrlistInsert(食べ物)
	DEF_DropAttrlistInsert(装備品)
	DEF_DropAttrlistInsert(撃ち物)
	DEF_DropAttrlistInsert(読み物)
	DEF_DropAttrlistInsert(スペカ)
	DEF_DropAttrlistInsert(入れ物)
	DEF_DropAttrlistInsert(お金)
	DEF_DropAttrlistInsert(階段)
	DEF_DropAttrlistInsert(火柱)
	DEF_DropAttrlistInsert(罠)
	DEF_DropAttrlistInsert(酒)
	DEF_DropAttrlistInsert(薬)
	DEF_DropAttrlistInsert(火消し)
	DEF_DropAttrlistInsert(剣)
	DEF_DropAttrlistInsert(割れ物)
	DEF_DropAttrlistInsert(不燃性)
	DEF_DropAttrlistInsert(耐冷凍)
	DEF_DropAttrlistInsert(耐静電気)
	DEF_DropAttrlistInsert(耐濡れ)
	DEF_DropAttrlistInsert(耐火)
#ifdef _UNRELEASE
	if(落ち物属性::DROPATRI_NUM != dropattrStr2Intlist.size())
	{
		assert(!_T("属性デコード内で要素数とDROPATRI_NUMの間に相違を検出しました。ソースを確認してください。"));
	}
#endif

	//属性
	DEF_AttackAttrlistInsert(無効エラー)
	DEF_AttackAttrlistInsert(アイテム破壊)
	DEF_AttackAttrlistInsert(必中)
	DEF_AttackAttrlistInsert(敵即死)
	DEF_AttackAttrlistInsert(衝)
	DEF_AttackAttrlistInsert(爆)
	DEF_AttackAttrlistInsert(爆発)
	DEF_AttackAttrlistInsert(火)
	DEF_AttackAttrlistInsert(水)
	DEF_AttackAttrlistInsert(冷)
	DEF_AttackAttrlistInsert(冷気)
	DEF_AttackAttrlistInsert(気)
	DEF_AttackAttrlistInsert(雷)
	DEF_AttackAttrlistInsert(電気)
	DEF_AttackAttrlistInsert(毒)
	DEF_AttackAttrlistInsert(貫通)
	DEF_AttackAttrlistInsert(魔法)
	DEF_AttackAttrlistInsert(落ち物)
	DEF_AttackAttrlistInsert(跳ね返し無効)
#ifdef _UNRELEASE
	if(攻撃属性::ATTACKATRI_NUM != attackattrStr2Intlist.size())
	{
		assert(!_T("属性デコード内で要素数とATTACKATRI_NUMの間に相違を検出しました。ソースを確認してください。"));
	}
#endif



	return true;
}

int cDatabaseOfGame::ImportDropFileLanguage(tstring type)
{
	int error = 0;
	cScriptReader SR(LANGUAGE_IMPORTDATADIR()+type);

	error |= SR.load();

	SR.outputerrormassage(type +tstring(_T("_ImportDropFileLanguage_Errorlog.txt")));
	if(error) return false;//エラー

	//親レイヤー保存
	DropImportLanguageParentLayer_ = SR.pdata();

	//メンバ(ID,レイヤー)を取得する
	map<tstring,pcScriptRLayer>::iterator itr = SR.data().membermap().begin();
	DropImportLanguagePool.clear();
	for(;itr != SR.data().membermap().end();itr++)
	{
		int id = _tstoi (itr->first.c_str());
		DropImportLanguagePool.insert(pair<int, pcScriptRLayer>(id, itr->second));
	}
	return true;
}

pcScriptRLayer cDatabaseOfGame::DropImportData(int ID)
{
	map<int,pcScriptRLayer>::iterator itr;
	itr = DropImportDataPool.find(ID);
	if(DropImportDataPool.end() == itr) return NULLOFcScriptRLayer;
	return itr->second;
}
pcScriptRLayer cDatabaseOfGame::DropImportData(int ID, const tstring& dataname)
{
	pcScriptRLayer psr;
	psr = DropImportData(ID);
	if(psr == NULL) return NULLOFcScriptRLayer;
	psr = psr->pmember(dataname);
	if(psr == NULL) return NULLOFcScriptRLayer;
	return psr;
}
pcScriptRLayer cDatabaseOfGame::DropImportData(int ID, const tstring& dataname, const tstring& dataname2)
{
	pcScriptRLayer psr;
	psr = DropImportData(ID,dataname);
	if(psr == NULL) return NULLOFcScriptRLayer;
	psr = psr->pmember(dataname2);
	if(psr == NULL) return NULLOFcScriptRLayer;
	return psr;
}
pcScriptRLayer cDatabaseOfGame::DropImportData(const tstring& dataname)
{
	pcScriptRLayer psr = DropImportDataParentLayer_;
	psr = psr->pmember(dataname);
	return psr;
}
pcScriptRLayer cDatabaseOfGame::DropImportData(const tstring& dataname, const tstring& dataname2)
{
	pcScriptRLayer psr = DropImportDataParentLayer_;
	psr = psr->pmember(dataname);
	if(psr == NULL) return NULLOFcScriptRLayer;
	psr = psr->pmember(dataname2);
	return psr;
}


double cDatabaseOfGame::DropImportData_Value(int ID, const tstring& dataname, double defaultval, int index)
{
	return def_decode_value(DropImportData(ID, dataname), defaultval, index);
}
double cDatabaseOfGame::DropImportData_Value(int ID, const tstring& dataname, const tstring& dataname2, double defaultval, int index)
{
	return def_decode_value(DropImportData(ID, dataname, dataname2), defaultval, index);
}
double cDatabaseOfGame::DropImportData_Value(const tstring& dataname, double defaultval, int index)
{
	return def_decode_value(DropImportData(dataname), defaultval, index);
}
double cDatabaseOfGame::DropImportData_Value(const tstring& dataname, const tstring& dataname2, double defaultval, int index)
{
	return def_decode_value(DropImportData(dataname,dataname2), defaultval, index);
}


int cDatabaseOfGame::DropImportData_ValueSize(int ID, const tstring& dataname)
{
	return def_decode_value_size(DropImportData(ID, dataname));
}
int cDatabaseOfGame::DropImportData_ValueSize(int ID, const tstring& dataname, const tstring& dataname2)
{
	return def_decode_value_size(DropImportData(ID, dataname, dataname2));
}
int cDatabaseOfGame::DropImportData_ValueSize(const tstring& dataname)
{
	return def_decode_value_size(DropImportData(dataname));
}
int cDatabaseOfGame::DropImportData_ValueSize(const tstring& dataname, const tstring& dataname2)
{
	return def_decode_value_size(DropImportData(dataname, dataname2));
}

void cDatabaseOfGame::DropImportData_MapIntToInt(int ID, const tstring& dataname, map<int,int>& mapii)
{
	mapii.clear();
	pcScriptRLayer datalayer = DropImportData(ID, dataname);
	vector<vector<int>> vvi;

	if(datalayer != NULL)
	{
		split( datalayer->script(), _T("|"), vvi, dataname);

		int i;
		int size = vvi.size();
		for(i=0;i<size;i++)
		{
			if(vvi[i].size() != 2)
			{
				OnAssert(dataname.c_str(),i,false,_T(" 書式にエラーが発生しました。スキップします。There is an error of form. Skip it."));
				continue;
			}
			mapii.insert(pair<int,int>(vvi[i][0],vvi[i][1]));
		}
	}

	if(mapii.empty())
	{
		mapii.insert(pair<int,int>(0,1));
	}

	return;
}

tstring cDatabaseOfGame::DropusefulName(int ID)
{
	map<int,tstring>::iterator itr;
	itr = DropIDtousefulName_.find(ID);
	if(DropIDtousefulName_.end() == itr) return _T("");
	return itr->second;
}
int cDatabaseOfGame::compareDropusefulName(int IDa, int IDb)
{
	map<int,tstring>::iterator itra, itrb;
	itra = DropIDtousefulName_.find(IDa);
	if(DropIDtousefulName_.end() == itra) return -1;
	itrb = DropIDtousefulName_.find(IDb);
	if(DropIDtousefulName_.end() == itrb) return 1;
	return itra->second.compare(itrb->second);
}
int cDatabaseOfGame::DropPrice(int ID)
{
	map<int,int>::iterator itr;
	itr = DropImportPricePool.find(ID);
	if(DropImportPricePool.end() == itr) return 0;
	return itr->second;
}
double cDatabaseOfGame::DropRarity(int ID)
{
	map<int,double>::iterator itr;
	itr = DropImportRarityPool.find(ID);
	if(DropImportRarityPool.end() == itr) return 0;
	return itr->second;
}


pcScriptRLayer cDatabaseOfGame::DropImportLanguage(int ID)
{
	map<int,pcScriptRLayer>::iterator itr;
	itr = DropImportLanguagePool.find(ID);
	if(DropImportLanguagePool.end() == itr) return NULLOFcScriptRLayer;
	return itr->second;
}
pcScriptRLayer cDatabaseOfGame::DropImportLanguage(int ID, const tstring& dataname)
{
	pcScriptRLayer psr;
	psr = DropImportLanguage(ID);
	if(psr == NULL) return NULLOFcScriptRLayer;
	psr = psr->pmember(dataname);
	if(psr == NULL) return NULLOFcScriptRLayer;
	return psr;
}
pcScriptRLayer cDatabaseOfGame::DropImportLanguage(int ID, const tstring& dataname, const tstring& dataname2)
{
	pcScriptRLayer psr;
	psr = DropImportLanguage(ID,dataname);
	if(psr == NULL) return NULLOFcScriptRLayer;
	psr = psr->pmember(dataname2);
	if(psr == NULL) return NULLOFcScriptRLayer;
	return psr;
}
pcScriptRLayer cDatabaseOfGame::DropImportLanguage(const tstring& dataname)
{
	pcScriptRLayer psr = DropImportLanguageParentLayer_;
	psr = psr->pmember(dataname);
	return psr;
}
pcScriptRLayer cDatabaseOfGame::DropImportLanguage(const tstring& dataname, const tstring& dataname2)
{
	pcScriptRLayer psr = DropImportLanguageParentLayer_;
	psr = psr->pmember(dataname);
	if(psr == NULL) return NULLOFcScriptRLayer;
	psr = psr->pmember(dataname2);
	return psr;
}


//-------------------------------------
//キャラクター系
int cDatabaseOfGame::ImportCharaFileData(tstring type)
{
	int error = 0;
	cScriptReader SR(IMPORTDATADIR+type);


#ifdef _UNRELEASE
#ifndef _UNPACK
	error |= SR.loadAndpacked();
#else
	error |= SR.loadpacked();
	if(error) exit(0);//ゲーム終了
#endif
#else
	error |= SR.loadpacked();
	if(error) exit(0);//ゲーム終了
#endif


	SR.outputerrormassage(type +tstring(_T("_ImportCharaFileData_Errorlog.txt")));
	if(error) return false;//エラー

	CharaImportDataParentLayer_ = SR.pdata();

	/*
	vector<tstring> tablestr;
	tablestr.push_back(_T("EnemyTable"));
	//表デコード
	int i;
	for(i=0;i<tablestr.size();i++)
	{
		pcScriptRLayer psr = CharaImportDataParentLayer_->pmember(tablestr[i]);
		if(psr == NULL) continue;
		afterdecode_table(psr->script(),_T(";"),CharaImportDataParentLayer_);
	}
	*/

	//メンバ(ID,レイヤー)を取得する
	map<tstring,pcScriptRLayer>::iterator itr = SR.data().membermap().begin();
	CharaImportDataPool.clear();
	for(;itr != SR.data().membermap().end();itr++)
	{
		int id = _tstoi (itr->first.c_str());
		CharaImportDataPool.insert(pair<int, pcScriptRLayer>(id, itr->second));
	}

	map<int,pcScriptRLayer>::iterator mobitr = CharaImportDataPool.lower_bound(CCHARACTER_cMob_BASE_ID_NUM);
	map<int,pcScriptRLayer>::iterator mobitrEnd = CharaImportDataPool.upper_bound(CCHARACTER_cMob_BASE_ID_NUM+1000-1);
	vector<int> idList;
	for(;mobitr != mobitrEnd; mobitr++)
	{
		idList.push_back(mobitr->first);
	}
	int i, size = idList.size();
	for(i=0; i < size; i++)
	{
		int newID = idList[i] - CCHARACTER_cMob_BASE_ID_NUM + CCHARACTER_cOtherChara_Event_BASE_ID_NUM;
		CharaImportDataPool.insert(pair<int, pcScriptRLayer>(newID, CharaImportDataPool[idList[i]]));
	}

	return true;
}

int cDatabaseOfGame::ImportCharaFileLanguage(tstring type)
{
	int error = 0;
	cScriptReader SR(LANGUAGE_IMPORTDATADIR()+type);

	error |= SR.load();

	SR.outputerrormassage(type +tstring(_T("_ImportCharaFileLanguage_Errorlog.txt")));
	if(error) return false;//エラー


	//メンバ(ID,レイヤー)を取得する
	map<tstring,pcScriptRLayer>::iterator itr = SR.data().membermap().begin();
	CharaImportLanguagePool.clear();
	for(;itr != SR.data().membermap().end();itr++)
	{
		int id = _tstoi (itr->first.c_str());
		CharaImportLanguagePool.insert(pair<int, pcScriptRLayer>(id, itr->second));
	}
	return true;
}

//--------------------------------------------------
//エフェクト
int cDatabaseOfGame::ImportEffectFile(const tstring& type)
{

	int error = 0;
	cScriptReader SR(IMPORTDATADIR + type);

#ifdef _UNRELEASE
#ifndef _UNPACK
	error |= SR.loadAndpacked();
#else
	error |= SR.loadpacked();
	if(error) exit(0);//ゲーム終了
#endif
#else
	error |= SR.loadpacked();
	if(error) exit(0);//ゲーム終了
#endif


	SR.outputerrormassage(tstring(_T("ImportEffectFile_Errorlog.txt")));
	if(error) return false;//エラー

	EffectImportPool_ = SR.pdata();

	if(EffectImportPool_ == NULL)
	{
		return false;
	}

	pcScriptRLayer EffectImportPool_Equipment = EffectImportPool_->pmember(_T("Equipment"));
	if(EffectImportPool_Equipment != NULL)
	{
		EffectImportPool_Equipment_Attack_ = EffectImportPool_Equipment->pmember(_T("Attack"));
		EffectImportPool_Equipment_Damage_ = EffectImportPool_Equipment->pmember(_T("Damage"));
	}

	EffectImportPool_Spell_ = EffectImportPool_->pmember(_T("Spell"));

	EffectImportPool_Trap_ = EffectImportPool_->pmember(_T("Trap"));

	pcScriptRLayer EffectImportPool_Chara = EffectImportPool_->pmember(_T("Chara"));
	if(EffectImportPool_Chara != NULL)
	{
		EffectImportPool_Chara_Attack_ = EffectImportPool_Chara->pmember(_T("Attack"));
		EffectImportPool_Chara_Special_ = EffectImportPool_Chara->pmember(_T("Special"));
	}

	EffectImportPool_Other_ = EffectImportPool_->pmember(_T("Other"));


	return true;
}

inline tstring EffectImport_Name_Inline(const tstring& preFilename, int ID)
{
	tstring filename = preFilename;
	filename += setStyle(ID).conclete_tstr();
	return filename += _T(".lua");
}
tstring cDatabaseOfGame::EffectImport_Name_Equipment_Attack(int ID)
{
	return EffectImport_Name_Inline(_T("EffectEquipAttack"),ID);
}
tstring cDatabaseOfGame::EffectImport_Name_Equipment_Damage(int ID)
{
	return EffectImport_Name_Inline(_T("EffectEquipDamage"),ID);
}
tstring cDatabaseOfGame::EffectImport_Name_Spell(int ID)
{
	return EffectImport_Name_Inline(_T("EffectSpell"),ID);
}
tstring cDatabaseOfGame::EffectImport_Name_Trap(int ID)
{
	return EffectImport_Name_Inline(_T("EffectTrap"),ID);
}
tstring cDatabaseOfGame::EffectImport_Name_Chara_Attack(int ID)
{
	return EffectImport_Name_Inline(_T("EffectCharaAttack"),ID);
}
tstring cDatabaseOfGame::EffectImport_Name_Chara_Special(int ID)
{
	return EffectImport_Name_Inline(_T("EffectCharaSpecial"),ID);
}
pcScriptRLayer cDatabaseOfGame::EffectImport_Equipment_Attack()
{
	return EffectImportPool_Equipment_Attack_;
}
pcScriptRLayer cDatabaseOfGame::EffectImport_Equipment_Damage()
{
	return EffectImportPool_Equipment_Damage_;
}
pcScriptRLayer cDatabaseOfGame::EffectImport_Spell()
{
	return EffectImportPool_Spell_;
}
pcScriptRLayer cDatabaseOfGame::EffectImport_Trap()
{
	return EffectImportPool_Trap_;
}
pcScriptRLayer cDatabaseOfGame::EffectImport_Chara_Attack()
{
	return EffectImportPool_Chara_Attack_;
}
pcScriptRLayer cDatabaseOfGame::EffectImport_Chara_Special()
{
	return EffectImportPool_Chara_Special_;
}
pcScriptRLayer cDatabaseOfGame::EffectImport_Other()
{
	return EffectImportPool_Other_;
}
/*
enum 
{
	DEFPAL_出現階,
	DEFPAL_MHP,
	DEFPAL_経験値,
	DEFPAL_攻撃力,
	DEFPAL_防御力,
	DEFPAL_特殊使用率,
}

tstring StrpalOfChara
{
	_T("出現階"),
	_T("MHP"),
	_T("経験値"),
	変数_攻撃力,
	_T("防御力"),
	_T("特殊使用率"),
}
*/

#define DEF_CharaAttrlistInsert(str) \
charaattrStr2Intlist.insert(pair<tstring,キャラ属性::キャラ属性リスト>(_T(#str),キャラ属性::str));\
charaattrInt2Strlist.insert(pair<キャラ属性::キャラ属性リスト,tstring>(キャラ属性::str,_T(#str)));

int cDatabaseOfGame::DecodeCharaFileData()
{
	DEF_CharaAttrlistInsert(無効エラー)
	DEF_CharaAttrlistInsert(人間)
	DEF_CharaAttrlistInsert(妖怪)
	DEF_CharaAttrlistInsert(幽霊)
	DEF_CharaAttrlistInsert(神)
	DEF_CharaAttrlistInsert(鬼)
	DEF_CharaAttrlistInsert(妖精)
	DEF_CharaAttrlistInsert(天狗)
	DEF_CharaAttrlistInsert(閻魔)
	DEF_CharaAttrlistInsert(鳥)
	DEF_CharaAttrlistInsert(獣)
	DEF_CharaAttrlistInsert(一文字)
	DEF_CharaAttrlistInsert(虫)
	DEF_CharaAttrlistInsert(ザコ)
	DEF_CharaAttrlistInsert(毛玉)
	DEF_CharaAttrlistInsert(弾幕)

	DEF_CharaAttrlistInsert(紅魔館)
	DEF_CharaAttrlistInsert(永遠亭)
	DEF_CharaAttrlistInsert(八雲一家)
	DEF_CharaAttrlistInsert(吸血鬼)
	DEF_CharaAttrlistInsert(殺人鬼)
	DEF_CharaAttrlistInsert(門番)
	DEF_CharaAttrlistInsert(魔法使い)
	DEF_CharaAttrlistInsert(病弱)
	DEF_CharaAttrlistInsert(カリスマ)
	DEF_CharaAttrlistInsert(泥棒)
	DEF_CharaAttrlistInsert(ボス)


#ifdef _UNRELEASE
	if(キャラ属性::CHARAATRI_NUM != charaattrStr2Intlist.size())
	{
		assert(!_T("属性デコード内で要素数とCHARAATRI_NUMの間に相違を検出しました。ソースを確認してください。"));
	}
#endif

	pcScriptRLayer sr;	

	map<int,pcScriptRLayer>::iterator itr = CharaImportDataPool.begin();

	for(;itr!=CharaImportDataPool.end();itr++)
	{
		int id = itr->first;//キャラID
		
		set<キャラ属性::キャラ属性リスト> charaattri;

		
		sr = itr->second;
		sr = sr->pmember(tstring(_T("属性")));
		if(sr != NULL)
		{
			int i;
			int size = sr->script().size();
			for(i=0;i<size;i++)
			{
				キャラ属性::キャラ属性リスト attr;
				if((attr = charattrS2I(sr->script()[i])) != キャラ属性::無効エラー)
				{
					charaattri.insert(attr);
				}
				else
				{//無効な識別子
					//assert(!((tstring)((setStyle(_T("属性デコード内に無効な識別子を発見しました。>ID"))+i+_T(">")+sr->script()[i])).c_str()));
					assert(!_T("属性デコード内に無効な識別子を発見しました。>ID"));
				}
			}
		}
		CharaImportAttriPool.insert( pair<int,set<キャラ属性::キャラ属性リスト>>(id, charaattri));
		
		#define def_DecodeCharaAttackAttriPool_routine(CHARAATTACKATTRINAME,CHARAATTACKATTRISTRING) \
		{\
			set<攻撃属性::攻撃属性> attackattri;\
			sr = itr->second;\
			sr = sr->pmember(tstring(_T(#CHARAATTACKATTRISTRING)));\
			if(sr != NULL)\
			{\
				int i;\
				int size = sr->script().size();\
				for(i=0;i<size;i++)\
				{\
					攻撃属性::攻撃属性 attr;\
					if((attr = attackattrS2I(sr->script()[i])) != 攻撃属性::無効エラー)\
					{\
						attackattri.insert(attr);\
					}\
					else\
					{\
						assert(!_T(#CHARAATTACKATTRISTRING) _T( "デコード内に無効な識別子を発見しました。"));\
					}\
				}\
			}\
			CharaImport_##CHARAATTACKATTRINAME##Pool.insert( pair<int,set<攻撃属性::攻撃属性>>(id, attackattri));\
		}

		def_DecodeCharaAttackAttriPool_routine(CharaStrongDamageBigAttri,強特攻耐性)
		def_DecodeCharaAttackAttriPool_routine(CharaStrongDamageSmallAttri,特攻耐性)
		def_DecodeCharaAttackAttriPool_routine(CharaStrongAttackBigAttri,強元気耐性)
		def_DecodeCharaAttackAttriPool_routine(CharaStrongAttackSmallAttri,元気耐性)
		def_DecodeCharaAttackAttriPool_routine(CharaStrongDefenceBigAttri,強頑強耐性)
		def_DecodeCharaAttackAttriPool_routine(CharaStrongDefenceSmallAttri,頑強耐性)
		def_DecodeCharaAttackAttriPool_routine(CharaWeekDamageBigAttri,強特攻弱点)
		def_DecodeCharaAttackAttriPool_routine(CharaWeekDamageSmallAttri,特攻弱点)
		def_DecodeCharaAttackAttriPool_routine(CharaWeekAttackBigAttri,強脱力弱点)
		def_DecodeCharaAttackAttriPool_routine(CharaWeekAttackSmallAttri,脱力弱点)
		def_DecodeCharaAttackAttriPool_routine(CharaWeekDefenceBigAttri,強軟弱弱点)
		def_DecodeCharaAttackAttriPool_routine(CharaWeekDefenceSmallAttri,軟弱弱点)
	}

	
	return true;
}

const tstring& cDatabaseOfGame::charattrI2S(キャラ属性::キャラ属性リスト attr)
{
	map<キャラ属性::キャラ属性リスト,tstring>::iterator itr;
	itr = charaattrInt2Strlist.find(attr);
	if(charaattrInt2Strlist.end() == itr)
	{
		OnAssert(_T(__FILE__),__LINE__,false,setStyle((int)attr).c_str(),_T(" charattrI2S キャラ属性ID不正エラー"));
		return charaattrInt2Strlist[キャラ属性::無効エラー];
	}
	return itr->second;
}
const キャラ属性::キャラ属性リスト cDatabaseOfGame::charattrS2I(const tstring& attr)
{
	キャラ属性::キャラ属性リスト damy = キャラ属性::無効エラー;
	map<tstring,キャラ属性::キャラ属性リスト>::iterator itr;
	itr = charaattrStr2Intlist.find(attr);
	if(charaattrStr2Intlist.end() == itr)
	{
		OnAssert(_T(__FILE__),__LINE__,false,attr.c_str(),_T(" charattrS2I 文字列が未定義です"));
		return damy;
	}
	return itr->second;
}

const tstring& cDatabaseOfGame::dropattrI2S(落ち物属性::落ち物属性 attr)
{
	map<落ち物属性::落ち物属性,tstring>::iterator itr;
	itr = dropattrInt2Strlist.find(attr);
	if(dropattrInt2Strlist.end() == itr)
	{
		OnAssert(_T(__FILE__),__LINE__,false,setStyle((int)attr).c_str(),_T(" dropattrI2S 落ち物属性ID不正エラー"));
		return dropattrInt2Strlist[落ち物属性::無効エラー];
	}
	return itr->second;
}
const 落ち物属性::落ち物属性 cDatabaseOfGame::dropattrS2I(const tstring& attr)
{
	落ち物属性::落ち物属性 damy = 落ち物属性::無効エラー;
	map<tstring,落ち物属性::落ち物属性>::iterator itr;
	itr = dropattrStr2Intlist.find(attr);
	if(dropattrStr2Intlist.end() == itr)
	{
		OnAssert(_T(__FILE__),__LINE__,false,attr.c_str(),_T(" dropattrS2I 文字列が未定義です"));
		return damy;
	}
	return itr->second;
}

const tstring& cDatabaseOfGame::attackattrI2S(攻撃属性::攻撃属性 attr)
{
	map<攻撃属性::攻撃属性,tstring>::iterator itr;
	itr = attackattrInt2Strlist.find(attr);
	if(attackattrInt2Strlist.end() == itr)
	{
		OnAssert(_T(__FILE__),__LINE__,false,setStyle((int)attr).c_str(),_T(" attackattrI2S 攻撃属性ID不正エラー"));
		return attackattrInt2Strlist[攻撃属性::無効エラー];
	}
	return itr->second;
}
const 攻撃属性::攻撃属性 cDatabaseOfGame::attackattrS2I(const tstring& attr)
{
	攻撃属性::攻撃属性 damy = 攻撃属性::無効エラー;
	map<tstring,攻撃属性::攻撃属性>::iterator itr;
	itr = attackattrStr2Intlist.find(attr);
	if(attackattrStr2Intlist.end() == itr)
	{
		OnAssert(_T(__FILE__),__LINE__,false,attr.c_str(),_T(" attackattrS2I 文字列が未定義です"));
		return damy;
	}
	return itr->second;
}


void cDatabaseOfGame::CharaAttri(int ID, set<キャラ属性::キャラ属性リスト>& CharaAttriSet)
{
	map<int,set<キャラ属性::キャラ属性リスト>>::iterator itr;
	itr = CharaImportAttriPool.find(ID);
	if(CharaImportAttriPool.end() == itr)
	{
		OnAssert(_T(__FILE__),__LINE__,false,setStyle(ID).c_str(),_T(" CharaAttri ID不正エラー"));
		return;
	}
	CharaAttriSet = itr->second;
	return;
}

#define def_CHARAATTACKATTRINAME_Routine(CHARAATTACKATTRINAME) \
void cDatabaseOfGame::CHARAATTACKATTRINAME(int ID, set<攻撃属性::攻撃属性>& AttackAttriSet)\
{\
	map<int,set<攻撃属性::攻撃属性>>::iterator itr;\
	itr = CharaImport_##CHARAATTACKATTRINAME##Pool.find(ID);\
	if( CharaImport_##CHARAATTACKATTRINAME##Pool.end() == itr)\
	{\
		OnAssert(_T(__FILE__),__LINE__,false,setStyle(ID).c_str(),_T(" ") _T(#CHARAATTACKATTRINAME) _T(" ID不正エラー"));\
		return;\
	}\
	AttackAttriSet = itr->second;\
	return;\
}

def_CHARAATTACKATTRINAME_Routine(CharaStrongDamageBigAttri)
def_CHARAATTACKATTRINAME_Routine(CharaStrongDamageSmallAttri)
def_CHARAATTACKATTRINAME_Routine(CharaStrongAttackBigAttri)
def_CHARAATTACKATTRINAME_Routine(CharaStrongAttackSmallAttri)
def_CHARAATTACKATTRINAME_Routine(CharaStrongDefenceBigAttri)
def_CHARAATTACKATTRINAME_Routine(CharaStrongDefenceSmallAttri)
def_CHARAATTACKATTRINAME_Routine(CharaWeekDamageBigAttri)
def_CHARAATTACKATTRINAME_Routine(CharaWeekDamageSmallAttri)
def_CHARAATTACKATTRINAME_Routine(CharaWeekAttackBigAttri)
def_CHARAATTACKATTRINAME_Routine(CharaWeekAttackSmallAttri)
def_CHARAATTACKATTRINAME_Routine(CharaWeekDefenceBigAttri)
def_CHARAATTACKATTRINAME_Routine(CharaWeekDefenceSmallAttri)


pcScriptRLayer cDatabaseOfGame::CharaImportData(int ID)
{
	map<int,pcScriptRLayer>::iterator itr;
	itr = CharaImportDataPool.find(ID);
	if(CharaImportDataPool.end() == itr) return NULLOFcScriptRLayer;
	return itr->second;
}
pcScriptRLayer cDatabaseOfGame::CharaImportData(int ID, const tstring& dataname)
{
	pcScriptRLayer psr;
	psr = CharaImportData(ID);
	if(psr == NULL) return NULLOFcScriptRLayer;
	psr = psr->pmember(dataname);
	if(psr == NULL) return NULLOFcScriptRLayer;
	return psr;
}

pcScriptRLayer cDatabaseOfGame::CharaImportData(int ID, const tstring& dataname, const tstring& dataname2)
{
	pcScriptRLayer psr;
	psr = CharaImportData(ID,dataname);
	if(psr == NULL) return NULLOFcScriptRLayer;
	psr = psr->pmember(dataname2);
	if(psr == NULL) return NULLOFcScriptRLayer;
	return psr;
}

pcScriptRLayer cDatabaseOfGame::CharaImportData(const tstring& dataname)
{
	pcScriptRLayer psr = CharaImportDataParentLayer_;
	psr = psr->pmember(dataname);
	return psr;
}
pcScriptRLayer cDatabaseOfGame::CharaImportData(const tstring& dataname, const tstring& dataname2)
{
	pcScriptRLayer psr = CharaImportData(dataname);
	if(psr == NULL) return psr;
	psr = psr->pmember(dataname2);
	return psr;
}


pcScriptRLayer cDatabaseOfGame::CharaImportLanguage(int ID)
{
	map<int,pcScriptRLayer>::iterator itr;
	itr = CharaImportLanguagePool.find(ID);
	if(CharaImportLanguagePool.end() == itr) return NULLOFcScriptRLayer;
	return itr->second;
}
pcScriptRLayer cDatabaseOfGame::CharaImportLanguage(int ID, const tstring& dataname)
{
	pcScriptRLayer psr;
	psr = CharaImportLanguage(ID);
	if(psr == NULL) return NULLOFcScriptRLayer;
	psr = psr->pmember(dataname);
	if(psr == NULL) return NULLOFcScriptRLayer;
	return psr;
}

pcScriptRLayer cDatabaseOfGame::CharaImportLanguage(int ID, const tstring& dataname, const tstring& dataname2)
{
	pcScriptRLayer psr;
	psr = CharaImportLanguage(ID,dataname);
	if(psr == NULL) return NULLOFcScriptRLayer;
	psr = psr->pmember(dataname2);
	if(psr == NULL) return NULLOFcScriptRLayer;
	return psr;
}

double cDatabaseOfGame::CharaImportData_Value(int ID, const tstring& dataname, double defaultval, int index)
{
	return def_decode_value(CharaImportData(ID, dataname), defaultval, index);
}
double cDatabaseOfGame::CharaImportData_Value(int ID, const tstring& dataname, const tstring& dataname2, double defaultval, int index)
{
	return def_decode_value(CharaImportData(ID, dataname, dataname2), defaultval, index);
}
double cDatabaseOfGame::CharaImportData_Value(const tstring& dataname, double defaultval, int index)
{
	return def_decode_value(CharaImportData(dataname), defaultval, index);
}
double cDatabaseOfGame::CharaImportData_Value(const tstring& dataname, const tstring& dataname2, double defaultval, int index)
{
	return def_decode_value(CharaImportData(dataname, dataname2), defaultval, index);
}

int cDatabaseOfGame::CharaImportData_ValueSize(int ID, const tstring& dataname)
{
	return def_decode_value_size(CharaImportData(ID, dataname));
}
int cDatabaseOfGame::CharaImportData_ValueSize(int ID, const tstring& dataname, const tstring& dataname2)
{
	return def_decode_value_size(CharaImportData(ID, dataname, dataname2));
}

void cDatabaseOfGame::CharaImportData_MapIntToInt(int ID, const tstring& dataname, map<int,int>& mapii)
{
	mapii.clear();
	pcScriptRLayer datalayer = CharaImportData(ID, dataname);
	vector<vector<int>> vvi;

	if(datalayer != NULL)
	{
		split( datalayer->script(), _T("|"), vvi, dataname);

		int i;
		int size = vvi.size();
		for(i=0;i<size;i++)
		{
			if(vvi[i].size() != 2)
			{
				OnAssert(dataname.c_str(),i,false,_T(" 書式にエラーが発生しました。スキップします。There is an error of form. Skip it."));
				continue;
			}
			mapii.insert(pair<int,int>(vvi[i][0],vvi[i][1]));
		}
	}

	if(mapii.empty())
	{
		mapii.insert(pair<int,int>(0,1));
	}

	return;
}

//-------------------------------------
//識別系共有システム
//-------------------------------------
#define ArraySizeOf(array)     (sizeof(array) / sizeof(array[0]))

#define DEF_OF_INSERT_UNKNOWN_NAMEPOOL(classtype,classtypeBIG) \
	pcrl = SR.data().pmember(_T("UnknownName_") _T(#classtype));\
if(pcrl != NULL)\
{\
	DropUnknownNamePool.insert(pair<int, vector<tstring>>(\
	CDROPING_c##classtype##_BASE_ID_NUM ,pcrl->script()));\
}\
if(DropUnknownNamePool[CDROPING_c##classtype##_BASE_ID_NUM].size() < classtypeBIG##_NUM)\
{\
	int shortsize = classtypeBIG##_NUM - DropUnknownNamePool[CDROPING_c##classtype##_BASE_ID_NUM].size();\
	for(i=0;i<shortsize;i++)\
	{\
		DropUnknownNamePool[CDROPING_c##classtype##_BASE_ID_NUM].push_back(_T("UnknownNameOf") _T(#classtype)+setStyle(i).conclete_tstr());\
	}\
}
/*
for(i=0; i < ArraySizeOf(classtype##_UnknownNamePool) ; i++)\
{\
	vstr.push_back( classtype##_UnknownNamePool[i] );\
}\
DropUnknownNamePool.insert(pair<int, vector<tstring>>(\
	CDROPING_##classtype##_BASE_ID_NUM , vstr ));\
vstr.clear();
*/

#define DEF_OF_INIT_IDTO_UNKNOWNNAME(classtype) \
for(i=0; i < DropUnknownNamePool[CDROPING_##classtype##_BASE_ID_NUM].size(); i++)\
{\
	DropIDtoUnknownName.insert(pair<int, tstring>(\
		CDROPING_##classtype##_BASE_ID_NUM + i, DropUnknownNamePool[CDROPING_##classtype##_BASE_ID_NUM][i] ));\
}

int cDatabaseOfGame::InitDropUnknownNamePool()
{
	int i;
	//vector<tstring> vstr;

	int error = 0;
	cScriptReader SR(LANGUAGE_IMPORTDATADIR() + _T("Unknown.id"));
	error |= SR.load();
	SR.outputerrormassage(tstring(_T("ImportUnknownNamePool_log.txt")));

	pcScriptRLayer pcrl;


	DEF_OF_INSERT_UNKNOWN_NAMEPOOL(Drink,DRINK)
	DEF_OF_INSERT_UNKNOWN_NAMEPOOL(Box,BOX)
	DEF_OF_INSERT_UNKNOWN_NAMEPOOL(Book,BOOK)
	DEF_OF_INSERT_UNKNOWN_NAMEPOOL(Spell,SPELL)
	DEF_OF_INSERT_UNKNOWN_NAMEPOOL(Bullet,BULLET)

	DEF_OF_INIT_IDTO_UNKNOWNNAME(cDrink)
	DEF_OF_INIT_IDTO_UNKNOWNNAME(cBox)
	DEF_OF_INIT_IDTO_UNKNOWNNAME(cBook)
	DEF_OF_INIT_IDTO_UNKNOWNNAME(cSpell)
	DEF_OF_INIT_IDTO_UNKNOWNNAME(cBullet)

	return true;
}


void unknownname_pointer_shaffle(vector<tstring*> &vpstr)
{
	int size = vpstr.size();
	int i;
	for(i=0;i<size;i++)
	{
		int a = random()*size;
		tstring* temp = vpstr[i];
		vpstr[i] = vpstr[a];
		vpstr[a] = temp;
	}
}


#define DEF_OF_POINTER_UNKNOWNNAME_INTO_VPSTR_AND_SHAFFLE(classtype) \
vpstr.clear();\
for(i=0; i < DropUnknownNamePool[CDROPING_##classtype##_BASE_ID_NUM].size(); i++)\
{\
	vpstr.push_back( &(DropUnknownNamePool[CDROPING_##classtype##_BASE_ID_NUM][i]) );\
}\
unknownname_pointer_shaffle(vpstr);\
for(i=0; i < vpstr.size(); i++)\
{\
	DropIDtoUnknownName.insert(pair<int, tstring>(\
		CDROPING_##classtype##_BASE_ID_NUM + i, *vpstr[i] ));\
	DropIDtoUnknownNamePriority_.insert(pair<int, double>(\
		CDROPING_##classtype##_BASE_ID_NUM + i, random() ));\
}

void cDatabaseOfGame::未識別名シャッフル()
{
	int i;
	vector<tstring*> vpstr;

	DropIDtoUnknownName.clear();
	DropIDtoUnknownNamePriority_.clear();

	DEF_OF_POINTER_UNKNOWNNAME_INTO_VPSTR_AND_SHAFFLE(cDrink)
	DEF_OF_POINTER_UNKNOWNNAME_INTO_VPSTR_AND_SHAFFLE(cBox)
	DEF_OF_POINTER_UNKNOWNNAME_INTO_VPSTR_AND_SHAFFLE(cBook)
	DEF_OF_POINTER_UNKNOWNNAME_INTO_VPSTR_AND_SHAFFLE(cSpell)
	DEF_OF_POINTER_UNKNOWNNAME_INTO_VPSTR_AND_SHAFFLE(cBullet)
}

void cDatabaseOfGame::大局識別部分キャンセル(const int drop_base_id)
{
	int i;
	vector<tstring*> vpstr;
	for(i=0; i < DropUnknownNamePool[drop_base_id].size(); i++)
	{
		vpstr.push_back( &(DropUnknownNamePool[drop_base_id][i]) );
	}
	unknownname_pointer_shaffle(vpstr);
	for(i=0; i < vpstr.size(); i++)
	{
		int ID = drop_base_id + i;
		DropIDtoUnknownName.erase(ID);
		DropIDtoUnknownName.insert(pair<int, tstring>(
			ID, *vpstr[i] ));

		DropIDtoUnknownNamePriority_.erase(ID);
		DropIDtoUnknownNamePriority_.insert(pair<int, double>(
			ID, random() ));
		
		DropIDtoAppreciated.erase(ID);
		DropIDtoAppreciated.insert(pair<int, bool>(
			ID, false));

		DropIDtoMEMO.erase(ID);
	}
}
void cDatabaseOfGame::大局識別Drinkキャンセル()
{
	大局識別部分キャンセル(CDROPING_cDrink_BASE_ID_NUM);
}
void cDatabaseOfGame::大局識別Boxキャンセル()
{
	大局識別部分キャンセル(CDROPING_cBox_BASE_ID_NUM);
}
void cDatabaseOfGame::大局識別Bookキャンセル()
{
	大局識別部分キャンセル(CDROPING_cBook_BASE_ID_NUM);
}
void cDatabaseOfGame::大局識別Spellキャンセル()
{
	大局識別部分キャンセル(CDROPING_cSpell_BASE_ID_NUM);
}
void cDatabaseOfGame::大局識別Bulletキャンセル()
{
	大局識別部分キャンセル(CDROPING_cBullet_BASE_ID_NUM);
}

void cDatabaseOfGame::大局識別全キャンセル()
{
	DropIDtoAppreciated.clear();

	map<int,tstring>::iterator itr = DropIDtoUnknownName.begin();
	for(;itr != DropIDtoUnknownName.end(); itr++)
	{
		if(GetSampleDroping(itr->first) && GetSampleDroping(itr->first)->デフォルト大局識別済み()){
			DropIDtoAppreciated.insert(pair<int, bool>(itr->first, true));
		}
		else{
			DropIDtoAppreciated.insert(pair<int, bool>(itr->first, false));
		}
	}
}

void cDatabaseOfGame::識別初期化()
{
	未識別名シャッフル();
	大局識別全キャンセル();
	アイテムメモ全削除();
}
void cDatabaseOfGame::ランダム大局識別百分率(double pos, vector<double> posv)
{
	map<int,bool>::iterator itr = DropIDtoAppreciated.begin();
	for(;itr != DropIDtoAppreciated.end(); itr++)
	{
		if(GetSampleDroping(itr->first) && GetSampleDroping(itr->first)->デフォルト大局識別済み()) {
			itr->second = true;
			continue;
		}
		int index = itr->first/1000 - 1;
		if(posv.size() > index)
		{
			itr->second = (posv[index] > random());
		}
		else
		{
			itr->second = (pos > random());
		}
	}
}

//メモ

pcMemoString cDatabaseOfGame::アイテムメモ(int ID)
{
	map<int,pcMemoString>::iterator itr = DropIDtoMEMO.find(ID);
	if(itr != DropIDtoMEMO.end())
		return itr->second;
	else
		return NULLOFpcMemoString;
};
void cDatabaseOfGame::アイテムメモ追加(int ID, pcMemoString pmemo)
{
	DropIDtoMEMO[ID] = pmemo;
}
void cDatabaseOfGame::アイテムメモ全削除()
{
	DropIDtoMEMO.clear();
}

int cDatabaseOfGame::InitDropItemMemoPool()
{

	strSample_.assign(MEMOSTRING_CHARKIND,_T("____"));


	//設定ロード
	pcScriptRLayer pcsr = g_systemSR.pdata()->pmember((tstring)_T("StringWindowMemoData"));

	if(pcsr != NULL)
	{
		pcScriptRLayer pcsrletter = pcsr->pmember((tstring)_T("MemoLetter"));
		if(pcsrletter != NULL)
		{
			strSample_ = pcsrletter->script();
			strSample_.resize(MEMOSTRING_CHARKIND,_T("____"));
		
		}
		else OnAssert(_T(__FILE__),__LINE__,false,_T("StringWindowの初期化データ「MemoLetter」が見つかりません。設定を確認してください。<error> [MemoLetter] is not found."));
		

		pcsrletter = pcsr->pmember((tstring)_T("NullLetter"));
		if(pcsrletter != NULL)
		{

			/*
			int res = 0;
			TCHAR* e;
			res = _tcstol(pcsrletter->topscript().c_str(),&e,10);
			OnAssert(_T(__FILE__),__LINE__, (pcsrletter->topscript().length() != 0) ,_T("StringWindowの初期化データ「NullLetter」に 空欄が設定されています。<error> [NullLetter] is a blank space."));
			OnAssert(_T(__FILE__),__LINE__, (_tcslen(e) == 0) && ( !(res == 0 && _tcslen(e) == 0) || (pcsrletter->topscript() == _T("0")))  ,_T("StringWindowの初期化データ「NullLetter」に 解釈不能な数値 が設定されています。<error> [NullLetter] cannot decode."));
	
			NullSampleIndex_ = res;
			*/
			NullSampleIndex_ = tstrtoi(pcsrletter->topscript(), tstring(_T("StringWindowの初期化データ「NullLetter」")));
		}
		else OnAssert(_T(__FILE__),__LINE__,false,_T("StringWindowの初期化データ「NullLetter」が見つかりません。設定を確認してください。<error> [NullLetter] is not found."));

	}
	else
	{
		OnAssert(_T(__FILE__),__LINE__,false,_T("StringWindowの初期化データが見つかりません。設定を確認してください。<error> Data of [StringWindow] Not found."));
	}


	return true;
}

void cDatabaseOfGame::GenMemoLetter(const vector<int> data, tstring& memo)
{
	int i;
	memo.clear();

	for(i=0;i<data.size();i++)
	{
		memo += GenMemoLetter(data[i]);
	}
}
tstring& cDatabaseOfGame::GenMemoLetter(int data)
{
	int i;
	//memo.clear();


	if(data >= 0 && data < MEMOSTRING_CHARKIND)
	{
		return strSample_[data];
	}
	else
	{
		return strSample_[NullSampleIndex_];
	}

}

void cDatabaseOfGame::savememoDic(std::vector<SByte>& data)
{
	vector<vector<SByte>> memodata;

	memodata.resize(memoDic_.size());

	int i;
	map<tstring, pcMemoString>::iterator itr = memoDic_.begin();
	const map<tstring, pcMemoString>::iterator itrend = memoDic_.end();
	for(i=0;itr!=itrend;itr++,i++)
	{
		itr->second->save(memodata[i]);
	}


	cDataConverter::VecVecC2VecC(memodata,data);
}
void cDatabaseOfGame::loadmemoDic(const std::vector<SByte>& data)
{
	vector<vector<SByte>> memodata;

	cDataConverter::VecC2VecVecC(data,memodata);

	int i;
	int size = memodata.size();
	map<tstring, pcMemoString>::iterator itr = memoDic_.begin();
	const map<tstring, pcMemoString>::iterator itrend = memoDic_.end();
	for(i=0;i<size;i++)
	{
		pcMemoString pmemo = pcMemoString(new cMemoString);
		pmemo->load(memodata[i]);
		memoDic_.insert(pair<tstring, pcMemoString>(pmemo->str(),pmemo));
	}
}