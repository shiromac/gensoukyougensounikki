
#pragma once

#include "cCharacter.h"
#include "cDroping.h"

#include "cDroping.h"


#include "utility/cDiscreteProbability.h"

#include "gameMainSystem/filemanage/fileManage_ph.h"
#include "controls_ph.h"

#include <vector>
#include <list>
#include <set>
#include <tstring_ph.h>
#include "scriptprocesser/DungeonStoryScript.h"

class cSaveRanking;
typedef boost::shared_ptr<cSaveRanking> pcSaveRanking;
class cMapChipRule;
typedef boost::shared_ptr<cMapChipRule> pcMapChipRule;
class cMapChipAnimeAll;
typedef boost::shared_ptr<cMapChipAnimeAll> pcMapChipAnimeAll;

using namespace std;

const int MEMOSTRING_CHARKIND = 256;
//struct sPalOfChara;


class cDatabaseOfGame
{
public:
	cDatabaseOfGame(void);
public:
	virtual ~cDatabaseOfGame(void);

	virtual int Init(cRenderDevice *pDev);


protected:

	//見本
	map<int,pcCharacter> CharaDataBase;
	map<int,pcDroping> DropDataBase;

	//確保関数リスト
	map<int, pcCharacter (*)() > GetCharaFunc;
	map<int, pcDroping (*)() > GetDropFunc;


	map<tstring, int> CharaFindNametoID;

	map<tstring, int> DropFindNametoID;
public:
	virtual pcCharacter GetNewCharacter(int ID);
	virtual pcCharacter GetNewCharacter(tstring str);
	

	virtual pcDroping GetNewDroping(int ID);
	virtual pcDroping GetNewDroping(tstring str);

	virtual pcDroping NewCopyDroping(pcDroping pdrop);
	virtual pcCharacter NewCopyCharacter(pcCharacter pchara);

	virtual pcCharacter GetSampleCharacter(int ID);
	virtual pcCharacter GetSampleCharacter(tstring str);

	virtual pcDroping GetSampleDroping(int ID);
	virtual pcDroping GetSampleDroping(tstring str);


	virtual int makeAllCharaList(vector<pcCharacter>& list);
	virtual int makeAllDropList(vector<pcDroping>& list);



	//-------------------------------------
	//検索
	//-------------------------------------
	
	virtual int makeDropIDList(vector<int>& IDlist, set<int> condition_and);

	//-------------------------------------
	//外部取り込み情報
	//-------------------------------------
protected:

	virtual int InitImportData();
public:
	virtual int ReImportData();
protected:
	//汎用デコード
	virtual double def_decode_value(pcScriptRLayer psr, double defaultval, int index);
	virtual int def_decode_value_size(pcScriptRLayer psr);
public:
	virtual void def_decode_Dropattri(pcScriptRLayer psr, set<落ち物属性::落ち物属性>& setint);
	virtual void def_decode_Charaattri(pcScriptRLayer psr, set<キャラ属性::キャラ属性リスト>& setint);
	virtual void def_decode_Attackattri(pcScriptRLayer psr, multiset<攻撃属性::攻撃属性>& setint);
protected:
	//**************************************************************
	//アイテム系
protected:
	//--------------------------------------------------
	//データ
	virtual int ImportDropFileData(tstring type);

	//値段およびレア度(参照高速化用)デコード
	virtual int DecodeDropFileData();

	//外部取り込み情報プール
	pcScriptRLayer DropImportDataParentLayer_;
	map<int,pcScriptRLayer> DropImportDataPool;

	//参照高速化用プール
	map<int,int> DropImportPricePool;
	map<int,double> DropImportRarityPool;
	map<int,tstring> DropIDtousefulName_;
public:


	//デコードユーティリティ//Data
	virtual pcScriptRLayer DropImportData(int ID);
	virtual pcScriptRLayer DropImportData(int ID, const tstring& dataname);
	virtual pcScriptRLayer DropImportData(int ID, const tstring& dataname, const tstring& dataname2);
	virtual pcScriptRLayer DropImportData(const tstring& dataname);
	virtual pcScriptRLayer DropImportData(const tstring& dataname, const tstring& dataname2);

	virtual double DropImportData_Value(int ID, const tstring& dataname, double defaultval, int index = 0);
	virtual double DropImportData_Value(int ID, const tstring& dataname, const tstring& dataname2, double defaultval, int index = 0);
	virtual double DropImportData_Value(const tstring& dataname, double defaultval, int index = 0);
	virtual double DropImportData_Value(const tstring& dataname, const tstring& dataname2, double defaultval, int index = 0);

	virtual int DropImportData_ValueSize(int ID, const tstring& dataname);
	virtual int DropImportData_ValueSize(int ID, const tstring& dataname, const tstring& dataname2);
	virtual int DropImportData_ValueSize(const tstring& dataname);
	virtual int DropImportData_ValueSize(const tstring& dataname, const tstring& dataname2);

	//初期値は[0,1]
	virtual void DropImportData_MapIntToInt(int ID, const tstring& dataname, map<int,int>& mapii);



	//参照高速化群


	virtual tstring DropusefulName(int ID);
	virtual int compareDropusefulName(int IDa, int IDb);
	virtual int DropPrice(int ID);
	virtual double DropRarity(int ID);

protected:
	//--------------------------------------------------
	//言語系
	virtual int ImportDropFileLanguage(tstring type);

	map<int,pcScriptRLayer> DropImportLanguagePool;

	pcScriptRLayer DropImportLanguageParentLayer_;

public:

	//デコードユーティリティ//Language
	virtual pcScriptRLayer DropImportLanguage(int ID);
	virtual pcScriptRLayer DropImportLanguage(int ID, const tstring& dataname);
	virtual pcScriptRLayer DropImportLanguage(int ID, const tstring& dataname, const tstring& dataname2);
	virtual pcScriptRLayer DropImportLanguage(const tstring& dataname);
	virtual pcScriptRLayer DropImportLanguage(const tstring& dataname, const tstring& dataname2);


protected:
	//**************************************************************
	//キャラクター系
	//--------------------------------------------------
	//データ
	virtual int ImportCharaFileData(tstring type);

	//デコード
	virtual int DecodeCharaFileData();

	//外部取り込み情報プール
	map<int,pcScriptRLayer> CharaImportDataPool;
	pcScriptRLayer CharaImportDataParentLayer_;

	//map<int,vector<sPalOfChara>> CharaImportPalPool;

	map<int,set<キャラ属性::キャラ属性リスト>> CharaImportAttriPool;

	map<int,set<攻撃属性::攻撃属性>> CharaImport_CharaStrongDamageBigAttriPool;
	map<int,set<攻撃属性::攻撃属性>> CharaImport_CharaStrongDamageSmallAttriPool;
	map<int,set<攻撃属性::攻撃属性>> CharaImport_CharaStrongAttackBigAttriPool;
	map<int,set<攻撃属性::攻撃属性>> CharaImport_CharaStrongAttackSmallAttriPool;
	map<int,set<攻撃属性::攻撃属性>> CharaImport_CharaStrongDefenceBigAttriPool;
	map<int,set<攻撃属性::攻撃属性>> CharaImport_CharaStrongDefenceSmallAttriPool;
	map<int,set<攻撃属性::攻撃属性>> CharaImport_CharaWeekDamageBigAttriPool;
	map<int,set<攻撃属性::攻撃属性>> CharaImport_CharaWeekDamageSmallAttriPool;
	map<int,set<攻撃属性::攻撃属性>> CharaImport_CharaWeekAttackBigAttriPool;
	map<int,set<攻撃属性::攻撃属性>> CharaImport_CharaWeekAttackSmallAttriPool;
	map<int,set<攻撃属性::攻撃属性>> CharaImport_CharaWeekDefenceBigAttriPool;
	map<int,set<攻撃属性::攻撃属性>> CharaImport_CharaWeekDefenceSmallAttriPool;

	map<tstring,キャラ属性::キャラ属性リスト> charaattrStr2Intlist;
	map<キャラ属性::キャラ属性リスト,tstring> charaattrInt2Strlist;
	map<tstring,落ち物属性::落ち物属性> dropattrStr2Intlist;
	map<落ち物属性::落ち物属性,tstring> dropattrInt2Strlist;
	map<tstring,攻撃属性::攻撃属性> attackattrStr2Intlist;
	map<攻撃属性::攻撃属性,tstring> attackattrInt2Strlist;
public:
	const tstring& charattrI2S(キャラ属性::キャラ属性リスト attr);
	const キャラ属性::キャラ属性リスト charattrS2I(const tstring& attr);

	const tstring& dropattrI2S(落ち物属性::落ち物属性 attr);
	const 落ち物属性::落ち物属性 dropattrS2I(const tstring& attr);

	const tstring& attackattrI2S(攻撃属性::攻撃属性 attr);
	const 攻撃属性::攻撃属性 attackattrS2I(const tstring& attr);


	virtual pcScriptRLayer CharaImportData(int ID);
	virtual pcScriptRLayer CharaImportData(int ID, const tstring& dataname);
	virtual pcScriptRLayer CharaImportData(int ID, const tstring& dataname, const tstring& dataname2);
	virtual pcScriptRLayer CharaImportData(const tstring& dataname);
	virtual pcScriptRLayer CharaImportData(const tstring& dataname, const tstring& dataname2);

	virtual double CharaImportData_Value(int ID, const tstring& dataname, double defaultval, int index = 0);
	virtual double CharaImportData_Value(int ID, const tstring& dataname, const tstring& dataname2, double defaultval, int index = 0);
	virtual double CharaImportData_Value(const tstring& dataname, double defaultval, int index = 0);
	virtual double CharaImportData_Value(const tstring& dataname, const tstring& dataname2, double defaultval, int index = 0);

	virtual int CharaImportData_ValueSize(int ID, const tstring& dataname);
	virtual int CharaImportData_ValueSize(int ID, const tstring& dataname, const tstring& dataname2);

	virtual void CharaAttri(int ID, set<キャラ属性::キャラ属性リスト>& CharaAttriSet);
	
	virtual void CharaStrongDamageBigAttri(int ID, set<攻撃属性::攻撃属性>& AttackAttriSet);
	virtual void CharaStrongDamageSmallAttri(int ID, set<攻撃属性::攻撃属性>& AttackAttriSet);
	virtual void CharaStrongAttackBigAttri(int ID, set<攻撃属性::攻撃属性>& AttackAttriSet);
	virtual void CharaStrongAttackSmallAttri(int ID, set<攻撃属性::攻撃属性>& AttackAttriSet);
	virtual void CharaStrongDefenceBigAttri(int ID, set<攻撃属性::攻撃属性>& AttackAttriSet);
	virtual void CharaStrongDefenceSmallAttri(int ID, set<攻撃属性::攻撃属性>& AttackAttriSet);
	virtual void CharaWeekDamageBigAttri(int ID, set<攻撃属性::攻撃属性>& AttackAttriSet);
	virtual void CharaWeekDamageSmallAttri(int ID, set<攻撃属性::攻撃属性>& AttackAttriSet);
	virtual void CharaWeekAttackBigAttri(int ID, set<攻撃属性::攻撃属性>& AttackAttriSet);
	virtual void CharaWeekAttackSmallAttri(int ID, set<攻撃属性::攻撃属性>& AttackAttriSet);
	virtual void CharaWeekDefenceBigAttri(int ID, set<攻撃属性::攻撃属性>& AttackAttriSet);
	virtual void CharaWeekDefenceSmallAttri(int ID, set<攻撃属性::攻撃属性>& AttackAttriSet);
	
	//初期値は[0,1]
	virtual void CharaImportData_MapIntToInt(int ID, const tstring& dataname, map<int,int>& mapii);

protected:
	//--------------------------------------------------
	//拠点系
	virtual int ImportHomeDataBase();
	pcScriptRLayer HomeDataBase_;

	std::vector<DungeonStoryScript_LuaEnvironment> homeEventEnvironments_;
	std::vector<tstring> homeEventKeys_;
public:
	pcScriptRLayer HomeDataBase();

	virtual std::vector<DungeonStoryScript_LuaEnvironment>& homeEventEnvironments(){return homeEventEnvironments_;};
	virtual std::vector<tstring>& homeEventKeys(){return homeEventKeys_;};

protected:
	//--------------------------------------------------
	//ダンジョン系
	virtual int ImportDungeonFile();

	map<tstring,pcScriptRLayer> DungeonImportDataFile_;
	map<tstring,tstring> DungeonImportDataName_;

	map<tstring,pcSaveRanking> DungeonRanking_;

	map<tstring,pDungeonStoryScript_LuaEnvironment> DungeonRegulationScriptEnvironment_;

public:
	virtual tstring DungeonImportDataName(tstring ID);
	virtual pcScriptRLayer DungeonImportDataFile(tstring ID);
	virtual pcSaveRanking DungeonRanking(tstring ID);
	virtual pDungeonStoryScript_LuaEnvironment DungeonRegulationScriptEnvironment(tstring ID);
protected:
	//--------------------------------------------------
	//マップ系
	virtual int ImportMapFile();

	map<tstring,tstring> MapChipFormImportDataMapchipFile_;
	map<tstring,tstring> MapChipFormImportDataMapchipDecoFile_;
	map<tstring,pair<tstring,tstring>> MapChipFormImportDataMusicFile_;

	map<tstring,pcScriptRLayer> MapImportDataFile_;
	
	map<tstring,vector<pcMapChipRule>> MapImportDataChipDecoRule_;

	map<tstring,pcMapChipAnimeAll> MapImportDataChipAnime_;

public:
	virtual pcScriptRLayer MapImportDataFile(tstring ID);
	virtual tstring MapChipFormImportDataMapchipFile(tstring ID);
	virtual tstring MapChipFormImportDataMapchipDecoFile(tstring ID);
	virtual pair<tstring,tstring> MapChipFormImportDataMusicFile(tstring ID);

	virtual vector<pcMapChipRule>& MapImportDataChipDecoRule(tstring ID);
	virtual pcMapChipAnimeAll MapImportDataChipAnimeAll(tstring ID);

protected:
	//--------------------------------------------------
	//言語系
	virtual int ImportCharaFileLanguage(tstring type);

	map<int,pcScriptRLayer> CharaImportLanguagePool;

public:

	//デコードユーティリティ//Language
	virtual pcScriptRLayer CharaImportLanguage(int ID);
	virtual pcScriptRLayer CharaImportLanguage(int ID, const tstring& dataname);
	virtual pcScriptRLayer CharaImportLanguage(int ID, const tstring& dataname, const tstring& dataname2);

protected:
	//--------------------------------------------------
	//エフェクト
	virtual int ImportEffectFile(const tstring& type);

	pcScriptRLayer EffectImportPool_;
	pcScriptRLayer EffectImportPool_Equipment_Attack_;
	pcScriptRLayer EffectImportPool_Equipment_Damage_;
	pcScriptRLayer EffectImportPool_Spell_;
	pcScriptRLayer EffectImportPool_Trap_;
	pcScriptRLayer EffectImportPool_Chara_Attack_;
	pcScriptRLayer EffectImportPool_Chara_Special_;
	pcScriptRLayer EffectImportPool_Other_;
public:

	virtual tstring EffectImport_Name_Equipment_Attack(int ID);
	virtual tstring EffectImport_Name_Equipment_Damage(int ID);
	virtual tstring EffectImport_Name_Spell(int ID);
	virtual tstring EffectImport_Name_Trap(int ID);
	virtual tstring EffectImport_Name_Chara_Attack(int ID);
	virtual tstring EffectImport_Name_Chara_Special(int ID);

	virtual pcScriptRLayer EffectImport_Equipment_Attack();
	virtual pcScriptRLayer EffectImport_Equipment_Damage();
	virtual pcScriptRLayer EffectImport_Spell();
	virtual pcScriptRLayer EffectImport_Trap();
	virtual pcScriptRLayer EffectImport_Chara_Attack();
	virtual pcScriptRLayer EffectImport_Chara_Special();
	virtual pcScriptRLayer EffectImport_Other();
	//-------------------------------------
	//識別系共有システム
	//-------------------------------------
protected:
	virtual int InitDropUnknownNamePool();


	//未識別名プール
	map<int,vector<tstring>> DropUnknownNamePool;

	virtual void 大局識別部分キャンセル(const int drop_base_id);
public:

	virtual void 未識別名シャッフル();
	virtual void 大局識別全キャンセル();
	virtual void 大局識別Drinkキャンセル();
	virtual void 大局識別Boxキャンセル();
	virtual void 大局識別Bookキャンセル();
	virtual void 大局識別Spellキャンセル();
	virtual void 大局識別Bulletキャンセル();
	virtual void ランダム大局識別百分率(double pos, vector<double> posv);

	virtual void 識別初期化();

protected:
	//識別
	map<int,bool> DropIDtoAppreciated;
public:
	virtual inline bool 大局識別済み(int ID)
	{
		map<int,bool>::iterator itr = DropIDtoAppreciated.find(ID);
		if(itr != DropIDtoAppreciated.end())
			return itr->second;
		else
			return true;
	};

	virtual inline void 大局appreciate(int ID)
	{
		DropIDtoAppreciated[ID] = true;
	};
	virtual inline void 大局untiappreciate(int ID)
	{
		DropIDtoAppreciated[ID] = false;
	};

	map<int,bool>& DropIDtoAppreciatedinstance(){return DropIDtoAppreciated;};//セーブ用
protected:
	//未識別名
	map<int,tstring> DropIDtoUnknownName;
	map<int,double> DropIDtoUnknownNamePriority_;
public:
	virtual inline tstring 未識別名(int ID)
	{
		map<int,tstring>::iterator itr = DropIDtoUnknownName.find(ID);
		if(itr != DropIDtoUnknownName.end())
			return itr->second;
		else
			return _T("未識別名無し");
	};

	virtual inline double 未識別名優先度(int ID)
	{
		map<int,double>::iterator itr = DropIDtoUnknownNamePriority_.find(ID);
		if(itr != DropIDtoUnknownNamePriority_.end())
			return itr->second;
		else
			return 0.0;
	}

protected:
	//メモ
	map<int,pcMemoString> DropIDtoMEMO;

	//メモ用文字列
	vector<tstring> strSample_;
	int NullSampleIndex_;
	virtual int InitDropItemMemoPool();


public:
	virtual pcMemoString アイテムメモ(int ID);
	virtual void アイテムメモ追加(int ID, pcMemoString pmemo);
	virtual void アイテムメモ全削除();

	map<tstring, pcMemoString> memoDic_;

	virtual int NullstrSampleIndex(){return NullSampleIndex_;};

	//コードを文字に変換する
	virtual void GenMemoLetter(const vector<int> data,tstring& memo);
	virtual tstring& GenMemoLetter(int data);

	virtual void savememoDic(std::vector<SByte>& data);
	virtual void loadmemoDic(const std::vector<SByte>& data);

	map<int,pcMemoString>& DropIDtoMEMOinstance(){return DropIDtoMEMO;};//セーブ用

};


const double DEF_UNDEFINE_DEFAULT = 0.00999;

/*
struct sPalOfChara
{
	double coefficient[3];
	double classvalue[5];
};
*/