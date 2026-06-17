#pragma once

#include "cDroping.h"
#include "gameMainSystem/filemanage/cAutoSaveClass.h"
#include <tstring_ph.h>
#include <common.h>

#include <vector>
using namespace std;

inline tstring g_SaveDataDirectory()
{
#ifdef __EMSCRIPTEN__
	return tstring(_T("/save/"));
#else
	return tstring(_EXELOCATION _T("savedata/"));
#endif
}

inline tstring g_SaveDataPath(const tstring& filename)
{
	return g_SaveDataDirectory() + filename;
}

tstring g_VersionString();

class cShortCutsManager;
typedef boost::shared_ptr<cShortCutsManager> pcShortCutsManager;
typedef boost::weak_ptr<cShortCutsManager> wpcShortCutsManager;

namespace mapUtility
{
	const int getMapValue(map<tstring,int>& mapObject, const tstring& key, int defaultValue);
	void setMapValue(map<tstring,int>& mapObject, const tstring& key, int setValue);
};
//コンフィグデータ
class cSaveConfig
{
public:
	cSaveConfig(void);
	virtual ~cSaveConfig(void);

	virtual void Init(cRenderDevice *pDev, tstring savefile);

	virtual void save();
	
protected:
	cAutoSaveClass saveclass_;

	//保存コンフィグデータ
	vector<SByte>* ppadconfig_;

	vector<int>* pSoundVolume_;

	vector<int>* pGhaphicsLight_;

	vector<int>* pMapThickness_;

	int inited;
};


//クエストクラス
//ダンジョン内で参照するデータ
class cSaveQuest
{
public:
	cSaveQuest(void);
	virtual ~cSaveQuest(void);

	virtual void Init(cRenderDevice *pDev, tstring savefile);

	unsigned long randBase;
	
	//ダンジョン識別
	tstring DungeonID;

	int floor;

	pcCharacter pPlayer;
	vector<pcCharacter> pFriend;


	//プレイ情報
	int money;
	int Sumturn;
	int SumFrame;
	int SumFramerealtime;
	int SumdefeatNum;

	//店システム
	double ShopFund;
	double ShopDebt;

	vector<int> FreeFlags;

	map<tstring,int> localFlags;
	map<tstring,int> privateFlags;

	map<int,pcMemoString> DropIDtoMEMO;
	map<int,bool> DropIDtoAppreciated;

	//厄システム
	vector<pcDroping> hinaPickItem;

	int GoodEndFlags;//正常終了フラグ
	int BadEndNum;//異常終了カウンタ

	int key;//key
	int FirstConditionOfMoney;

	tstring Version_;//Version

	pcCharacter pFirstConditionOfPlayer;

public:
	void setUnEnable();
	int enable();
	pcSaveClass savefile_;
	virtual int save();
	virtual int load();

	virtual StyleString shortExplanationText();

	//privateFlagKeys
	
	static tstring privateFlagKey_AppreciationSupportKey();//識別補助
	static tstring privateFlagKey_StoryEventKey();//ストーリー



#define def_cSaveQuest_privateFlag_setter_getter_routine(MEMBER) \
	const int privateFlag_##MEMBER(int defalutValue){/*getter*/\
		return mapUtility::getMapValue(privateFlags, privateFlagKey_##MEMBER##Key(), defalutValue);\
	};\
	void setPrivateFlag_##MEMBER(int setValue){/*setter*/\
		mapUtility::setMapValue(privateFlags, privateFlagKey_##MEMBER##Key(), setValue);\
	};

def_cSaveQuest_privateFlag_setter_getter_routine(AppreciationSupport)
def_cSaveQuest_privateFlag_setter_getter_routine(StoryEvent)

};
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cSaveQuest> pcSaveQuest;

//ストアクラス
//ダンジョン内で参照しないデータ
//大容量データ
class cSaveStore
{
public:
	cSaveStore(void);
	virtual ~cSaveStore(void);

	virtual void Init(cRenderDevice *pDev, tstring savefile);

	//倉庫
	vector<vector<pcDroping>> storeItem;
	vector<pcMemoString> storeMemo;

	//厄システム
	vector<pcDroping> hinaItem;
	vector<pcDroping> hinaKeepItem;

	//銀行
	double bank;
	//厄ポイント
	double yaku;

	double playerExpStore;//加算待ちplayerExp
	double yakuStore;//加算待ちyaku

	//霧雨魔法店
	vector<pcDroping> marisaShopItem;
	double marisaShopLastSiireLevel;
	double marisaShopLastSiireExp;

	int save(pcSaveClass data) const;
	int load(const pcSaveClass data);
protected:
	//ここから下は独自に保存する
	cAutoSaveClass saveclass_dic_;
	//メモ
	vector<SByte>* memodata_;

};

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cSaveStore> pcSaveStore;


//セーブデータクラス
class cSaveData
{
public:
	cSaveData(void);
	virtual ~cSaveData(void);

	virtual void Init(cRenderDevice *pDev);

	virtual void save();//明示的にセーブ

protected:
	pcSaveStore pSaveStore_;

	cSaveClass playersave_;
public:


	pcSaveStore pSaveStore(){return pSaveStore_;};

	//プレイヤー情報
	tstring playersigns;
	int playerLevel;
	int playerExp;

	//ショートカット
	pcShortCutsManager ShortCutsManager;

	//レベル特典
	double LevelShopDiscount();
	int LevelHinaKeepNum();

	//レベル特典ではなくなった
	int LevelStoreSpaceNum();
	int StoreSpaceNumForExtendNum(int extendNum);
	int StoreSpaceExtendCount();
	int StoreSpaceExtendCountMax();
	int StoreSpaceExtendNeedMoney();
	void StoreSpaceExtend();

	static int NextPlayerLevel(int now_level);
	int AddPlayerLevel(double addExp);

	//拠点データ
	pcCharacter lastpPlayer;
	int lastmoney;
	vector<pcDroping> lastfloorItem;

	map<tstring,int> globalFlags;

	//セーブデータ入れ替え防止セーブキー
	map<tstring,int> savekey;

	//拠点での不正終了フラグ
	int GoodEndFlagInBaseTemp;//こっちは仮フラグ
	int DidGoodEndFlagInBase;//アプリ正常終了していたフラグ（拠点で参照

	//globalFlagsKeys
	static tstring globalFlagsKey_AppreciationSupportKey(const tstring& dungeonID, int num);//識別補助
	static tstring globalFlagsKey_StoryEventKey(const tstring& dungeonID, int num);//ストーリー
	static tstring globalFlagsKey_ClearedFlagKey(const tstring& dungeonID);//クリアしたことがあるか
	static tstring globalFlagsKey_AppearedFlagKey(const tstring& dungeonID);//ダンジョン出現フラグ


#define def_cSaveData_globalFlags_setter_getter_routine_argument_dungeon_num(MEMBER) \
	const int globalFlags_##MEMBER(const tstring& dungeonID, int num, int defalutValue){/*getter*/\
	return mapUtility::getMapValue(globalFlags, globalFlagsKey_##MEMBER##Key(dungeonID, num), defalutValue);\
	};\
	void setGlobalFlags_##MEMBER(const tstring& dungeonID, int num, int setValue){/*setter*/\
		mapUtility::setMapValue(globalFlags, globalFlagsKey_##MEMBER##Key(dungeonID, num), setValue);\
	};
#define def_cSaveData_globalFlags_setter_getter_routine_argument_dungeon(MEMBER) \
	const int globalFlags_##MEMBER(const tstring& dungeonID, int defalutValue){/*getter*/\
		return mapUtility::getMapValue(globalFlags, globalFlagsKey_##MEMBER##Key(dungeonID), defalutValue);\
	};\
	void setGlobalFlags_##MEMBER(const tstring& dungeonID, int setValue){/*setter*/\
		mapUtility::setMapValue(globalFlags, globalFlagsKey_##MEMBER##Key(dungeonID), setValue);\
	};

def_cSaveData_globalFlags_setter_getter_routine_argument_dungeon_num(AppreciationSupport)
def_cSaveData_globalFlags_setter_getter_routine_argument_dungeon_num(StoryEvent)
def_cSaveData_globalFlags_setter_getter_routine_argument_dungeon(ClearedFlag)
def_cSaveData_globalFlags_setter_getter_routine_argument_dungeon(AppearedFlag)

};


#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cSaveData> pcSaveData;

pcSaveData g_pSaveData();


//結果クラス
class cSaveResult
{
public:
	cSaveResult(void);
	virtual ~cSaveResult(void);
	cSaveResult(const cSaveResult& rother);

	virtual void Init(cRenderDevice *pDev);


public:
	vector<StyleString> lastmessage_;
	vector<pcDroping> lastequipItem_;

	StyleString dungeonName_;
	StyleString CharaName_;
	int score_;

	int LastMHP_;
	int LastMSP_;
	int LastLV_;

	int clearflag_;
	int lastfloor_;
	int Sumturn_;
	int SumFrame_;
	int SumFramerealtime_;
	int SumExp_;
	int SumdefeatNum_;
	int money_;

	int BadEndNum_;//異常終了カウンタ

	time_t Date_;
	tstring PlayerSign_;

	map<tstring,int> localFlags_;
	map<tstring,int> privateFlags_;

	tstring Version_;//Version

	pcCharacter pFirstConditionOfPlayer_;
	int FirstConditionOfMoney_;
public:
	int save(pcSaveClass data) const;
	int load(const pcSaveClass data);
};

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cSaveResult> pcSaveResult;

//結果ランキング
class cSaveRanking
{
public:
	cSaveRanking(void);
	virtual ~cSaveRanking(void);

	virtual void Init(cRenderDevice *pDev,const tstring DungeonID);

	int MaxFloor;
	static const int RankingNum = 10;
public:
	vector<cSaveResult> RTARanking;
	vector<cSaveResult> TTARanking;
	vector<cSaveResult> SARanking;
	vector<cSaveResult> FreeResult;

	vector<cSaveResult> FreeStyle_RTARanking;
	vector<cSaveResult> FreeStyle_TTARanking;
	vector<cSaveResult> FreeStyle_SARanking;

	virtual int rankingIn(pcSaveResult result);
	virtual int rankingInInstance(pcSaveResult result, vector<cSaveResult>& RTARankingInstance, vector<cSaveResult>& TTARankingInstance, vector<cSaveResult>& SARankingInstance);
protected:
	pcSaveClass savefile_;
	tstring DungeonID_;
	int save(pcSaveClass data);
	int load(const pcSaveClass data);
};
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cSaveRanking> pcSaveRanking;

int ConvertT2VecC(const StyleString& tmpl, vector<SByte>& data);
int DecodeVecC2T(const vector<SByte>& data, StyleString& tmpl);
inline int BackDecodeVecC2T(StyleString& tmpl, const vector<SByte>& data)
{return DecodeVecC2T(data, tmpl);};

int ConvertT2VecC(const pcDroping& tmpl, vector<SByte>& data);
int DecodeVecC2T(const vector<SByte>& data, pcDroping& tmpl);
inline int BackDecodeVecC2T(pcDroping& tmpl, const vector<SByte>& data)
{return DecodeVecC2T(data, tmpl);};

int ConvertT2VecC(const pcCharacter& tmpl, vector<SByte>& data);
int DecodeVecC2T(const vector<SByte>& data, pcCharacter& tmpl);
inline int BackDecodeVecC2T(pcCharacter& tmpl, const vector<SByte>& data)
{return DecodeVecC2T(data, tmpl);};

int ConvertT2VecC(const cSaveResult& tmpl, vector<SByte>& data);
int DecodeVecC2T(const vector<SByte>& data, cSaveResult& tmpl);
inline int BackDecodeVecC2T(cSaveResult& tmpl, const vector<SByte>& data)
{return DecodeVecC2T(data, tmpl);};

int ConvertT2VecC(const pcMemoString& tmpl, vector<SByte>& data);
int DecodeVecC2T(const vector<SByte>& data, pcMemoString& tmpl);
inline int BackDecodeVecC2T(pcMemoString& tmpl, const vector<SByte>& data)
{return DecodeVecC2T(data, tmpl);};

int ConvertT2VecC(const pcSaveStore& tmpl, vector<SByte>& data);
int DecodeVecC2T(const vector<SByte>& data, pcSaveStore& tmpl);
inline int BackDecodeVecC2T(pcSaveStore& tmpl, const vector<SByte>& data)
{return DecodeVecC2T(data, tmpl);};
