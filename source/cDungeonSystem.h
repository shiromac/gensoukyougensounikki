#pragma once
#include "ccontrol.h"
#include "cPlayerChara.h"
#include "cItem.h"
#include "cMap.h"

#include "cDramaManager.h"


#include "cDrawingObject.h"
#include "cGameWindow.h"
#include "cSelectWindow.h"
#include "cMessageWindow.h"
#include "cListWindow.h"
#include "cControlLayer.h"
#include "cAnimationManager.h"

#include "utility/cAltaMatterPlus.h"
#include "utility/cDiscreteProbability.h"
#include "utility/cRandomGen.h"

#include "cGameScreenInterface.h"
#include "menuCommand.h"
#include "cVectorOfControlLayer.h"

#include "cDatabaseOfGame.h"
#include "cCutInManager.h"


#include "DungeonInstance.h"

#include "utility/ValiableField/SafePointerObject.h"

#include <vector>
#include <list>
#include <set>
#include <tstring_ph.h>


using namespace std;


//セーブデータクラス
class cSaveData;
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cSaveData> pcSaveData;
class cSaveQuest;
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cSaveQuest> pcSaveQuest;

//class DungeonStoryScript_LuaEnvironment;
class DungeonStoryScript_LuaEnvironment;
typedef boost::shared_ptr<DungeonStoryScript_LuaEnvironment> pDungeonStoryScript_LuaEnvironment;
typedef boost::weak_ptr<DungeonStoryScript_LuaEnvironment> wpDungeonStoryScript_LuaEnvironment;



//------------------------------------
//カラー定義
//------------------------------------
#define MASSAGE_IFCHANGE (2)

//------------------------------------
//カラー定義
//------------------------------------
#define DENGER_COLOR (0xFFEE0000)

//------------------------------------
//ターン定義
//------------------------------------
#define GAME_TURN_CAUTION 1000
#define GAME_TURN_ALERT 1300
#define GAME_TURN_GAMEOVER 1500

class cDungeonSystem :
	public cControl
{
public:
	cDungeonSystem(void);
public:
	virtual ~cDungeonSystem(void);

public:
	//はじめに一度だけ呼ばれるシステム初期化関数
	virtual int InitSystem(IDirect3DDevice9 *pDev);
private:
	//ダンジョンデータ初期化関数
	virtual int InitDungeon(IDirect3DDevice9 *pDev);
	//フロアデータ初期化関数
	virtual int InitFloor(IDirect3DDevice9 *pDev);
	virtual int StartFloor(IDirect3DDevice9 *pDev);

	//ダンジョン終了関数
	virtual int FinalizeDungeon(IDirect3DDevice9 *pDev);
public:
	//デバイスリセット
	virtual int resetedDevice(IDirect3DDevice9 *pDev);

	//------------------------------------------------------
	//環境変数
	//------------------------------------------------------

	int EV_charEdgedFlag;//文字縁取りモード

	int EV_mapThickness;//ミニマップの濃さ(0~10)
public:

	cDatabaseOfGame DataBase;

	//セーブデータ
	pcSaveData pSaveData;

	//クエストセーブデータ
	pcSaveQuest pSaveQuest;

	IDirect3DDevice9 *pDevice_D3D;//デバイスが通ってないところの救済用（泣）
private:

	//ランダム関数
public:

	IDirect3DTexture9* pFrameTexture;
	IDirect3DTexture9* pArrowsTexture;



	//画面情報
	cGameScreenInterface GameScreenInterface;
private:
	cVectorOfControlLayer menuControlLayerV_;
	cVectorOfControlLayer primaryMenuControlLayerV_;//アニメと平行ではしり、ばあいによってはアニメより優先できる
//------------------------------------------------------
//ダンジョンデータおよびフロア
//------------------------------------------------------

	pcDungeonInstance pDungeonInstance_;
	pcFloorInstance pFloorInstance_;

	//pcDungeon pDungeon_;
	//int FloorLevel_;

	virtual int resetfloor();
	virtual int cleanfloor();
public:
	inline unsigned long& random_Seed(){return pDungeonInstance_->random_Seed_;};

	inline pcDungeon& pDungeon(){return pDungeonInstance_->pDungeon_;};

	inline vector<int>& FreeFlags(){return pDungeonInstance_->FreeFlags_;};
	inline map<tstring,int>& localFlags(){return pDungeonInstance_->localFlags_;};
	inline map<tstring,int>& privateFlags(){return pDungeonInstance_->privateFlags_;};

	inline int& FloorLevel(){return pDungeonInstance_->FloorLevel_;};

	virtual pcFloor pFloor();
	inline cRandomGen& RandomInitGen(){return pDungeonInstance_->RandomInitGen_;};

	//ダンジョンデータをセットする
	virtual void setDungeonData(const tstring& DungeonID); 


	//主人公
	inline pcCharacter& pPlayerChara(){return pDungeonInstance_->pPlayerChara_;};
	inline vector<pcCharacter>& pFriendCharas(){return pDungeonInstance_->pFriendCharas_;};


	//メニュー
	inline cVectorOfControlLayer& menuControlLayerV(){return menuControlLayerV_;;};

	//キャラクターからプッシュされたルア環境
	void insertLuaEnv(pDungeonStoryScript_LuaEnvironment luaEnv){pDungeonInstance_->insertLuaEnv(luaEnv);};
	void deleteLuaEnv(pDungeonStoryScript_LuaEnvironment luaEnv){pDungeonInstance_->deleteLuaEnv(luaEnv);};

	virtual const int 拠点フラグ();
	virtual const int 独立モード();

	const static int GAMECLEAR_CLEARFLAG = 1<<0;
	const static int GAMECLEAR_PULLOUTFLAG = 1<<1;

protected:
	inline int& GameOverFlag(){return pDungeonInstance_->GameOverFlag_;};
	inline cDungeonInstance::GameEndFlag & GameRestartFlag(){return pDungeonInstance_->GameRestartFlag_;};
	inline int& GameClearFlag(){return pDungeonInstance_->GameClearFlag_;};

	//時間計測
	inline int& time_SumFrameCount(){return pDungeonInstance_->time_SumFrameCount_;};
	inline double& time_Sumtime(){return pDungeonInstance_->time_Sumtime_;};
public:
	inline int& SumTurnCount(){return pDungeonInstance_->SumTurnCount_;};
	inline int& SumEnemyDefeatNum(){return pDungeonInstance_->SumEnemyDefeatNum_;};

	virtual double Floortime();
	virtual double Sumtime(){return Floortime()+time_Sumtime();};

	//------------------------------------------------------
	//ダンジョンシステム
	//------------------------------------------------------

	virtual int process(IDirect3DDevice9 *pDev);
	virtual int Inputprocess(IDirect3DDevice9 *pDev);
	//一ターンに一度実行される
	virtual int Turnprocess(IDirect3DDevice9 *pDev);
	virtual int menuCommandprocess(IDirect3DDevice9 *pDev);
	virtual int minimapViewprocess(IDirect3DDevice9 *pDev);
	virtual int aspectTurnprocess(IDirect3DDevice9 *pDev);
	virtual int aspectDiagonprocess(IDirect3DDevice9 *pDev);
	virtual int shootAndShortCutprocess(IDirect3DDevice9 *pDev);

	//イベント通知システム
	bool eventNotification(const pLuaString& eventName, const pLuaString& notificatedObject, const pLuaString& argument);
	map<std::basic_string<LuaString>,pcCharacter>& charactarStorage(){return pDungeonInstance_->charactarStorage_;};
	map<std::basic_string<LuaString>,pcDroping>& dropingStorage(){return pDungeonInstance_->dropingStorage_;};
	map<std::basic_string<LuaString>,pcLandform>& landformStorage(){return pDungeonInstance_->landformStorage_;};
	
	bool isNotGoodTerminateFlagInBase();

	inline int& s_cannotInputWaitCount(){return pFloorInstance_->s_cannotInputWaitCount_;};
	inline cAltaMatterPlus& s_AltaInputprocess(){return pFloorInstance_->s_AltaInputprocess_;};
	inline cAltaMatterPlus& s_AltaofminimapViewprocess(){return pFloorInstance_->s_AltaofminimapViewprocess_;};
	inline cAltaMatterPlus& s_aspectTurnprocess(){return pFloorInstance_->s_aspectTurnprocess_;};
	inline bool& s_minimapHiding(){return pFloorInstance_->s_minimapHiding_;};
	inline int& s_skipValiable(){return pFloorInstance_->s_skipValiable_;};
	
	inline int& s_aspectTurndiagonsupport(){return pFloorInstance_->s_aspectTurndiagonsupport_;};
	
	inline cAltaMatterPlus& s_aspectDiagonprocess(){return pFloorInstance_->s_aspectDiagonprocess_;};

	
	inline int& s_autoTurnFlag(){return pFloorInstance_->s_autoTurnFlag_;};

	inline int& s_phase(){return pFloorInstance_->s_phase_;};
	inline int& s_turn_speed(){return pFloorInstance_->s_turn_speed_;};
	inline int& s_DashButtonTap(){return pFloorInstance_->s_DashButtonTap_;};
	inline int& s_DashButtonCancelEffect(){return pFloorInstance_->s_DashButtonCancelEffect_;};
	
	inline int& s_AttackDelay(){return pFloorInstance_->s_AttackDelay_;};
	inline int& s_AttackRapidSwitch(){return pFloorInstance_->s_AttackRapidSwitch_;};
	inline int& s_shootAndShortCutswitch(){return pFloorInstance_->s_shootAndShortCutswitch_;};
	inline int& s_ShortCutdiagonsupport(){return pFloorInstance_->s_ShortCutdiagonsupport_;};
	inline int& s_ShortCutSelectSwitch(){return pFloorInstance_->s_ShortCutSelectSwitch_;};
	inline int& s_ShortCutSelectOffCount(){return pFloorInstance_->s_ShortCutSelectOffCount_;};
	
private:
	inline int& Processmode(){return pFloorInstance_->Processmode_;};
	inline int& aspectTurnprocessswitch(){return pFloorInstance_->aspectTurnprocessswitch_;};
	
	inline int& TurnCount(){return pFloorInstance_->TurnCount_;};

	inline int& time_FrameCount(){return pFloorInstance_->time_FrameCount_;};
	inline double& time_Starttime(){return pFloorInstance_->time_Starttime_;};

	inline SharedValiable_likeInt<int>& floorlightswitchvisible(){return pFloorInstance_->floorlightswitchvisible_;};
	inline int& floorlightswitch(){return pFloorInstance_->floorlightswitch_;};

	inline int& floornothrowswitch(){return pFloorInstance_->floornothrowswitch_;};
	inline int& floortimestopswitch(){return pFloorInstance_->floortimestopswitch_;};
	inline int& floortimeskipswitch(){return pFloorInstance_->floortimeskipswitch_;};

	inline SharedValiable_likeInt<int>& floormoveswitch(){return pFloorInstance_->floormoveswitch_;};

	inline int& floorstealswitch(){return pFloorInstance_->floorstealswitch_;};
	inline double& floorshopprofit(){return pFloorInstance_->floorshopprofit_;};
	inline int& floorshopownermoney(){return pFloorInstance_->floorshopownermoney_;};
	inline map<pcDroping,int>& floorshopitemvalueBuy(){return pFloorInstance_->floorshopitemvalueBuy_;};
	inline map<pcDroping,int>& floorshopitemvalueSell(){return pFloorInstance_->floorshopitemvalueSell_;};

	inline int& floorshoplastroom(){return pFloorInstance_->floorshoplastroom_;};
	
	inline int& floorNowParty(){return pFloorInstance_->floorNowParty_;};

	inline vector<pcDroping>& floorshopitemlist(){return pFloorInstance_->floorshopitemlist_;};

	inline set<pcLandform>& DarkLandList(){return pFloorInstance_->DarkLandList_;};
	inline set<pcLandform>& DangerLandList(){return pFloorInstance_->DangerLandList_;};

public:


	//ミニマップを書き直す。
	virtual int Refreshminimap(IDirect3DDevice9 *pDev);
	//ミニマップを消して書き直す。
	virtual int Resetminimap(IDirect3DDevice9 *pDev);

	virtual int minimapDrawDelegate(IDirect3DDevice9 *pDev, const cCoordinate& leftTop, const int boxSize, const int alpha);



	virtual int Draw(IDirect3DDevice9 *pDev);
	virtual int LandDraw(IDirect3DDevice9 *pDev);
	virtual int MiniMapDraw(IDirect3DDevice9 *pDev);
	virtual int CharacterDraw(IDirect3DDevice9 *pDev);
	virtual int DropingDraw(IDirect3DDevice9 *pDev);
	virtual int FrameDraw(IDirect3DDevice9 *pDev);
	virtual int refreshCharaDropView();
	virtual int EventDraw();
	virtual int EventDrawUnderObject();


	inline cMap& Map(){return pFloorInstance_->Map_;};



	//インプットの直前に一度初期化する
	virtual int resetTurnInformation(IDirect3DDevice9 *pDev);

	//整理して使われなくなったオブジェクトを消す。
	virtual int arrangementObject(IDirect3DDevice9 *pDev);


	virtual int PlayBGM();


	virtual void setHiddenInterface(bool hidden){hiddenInterface_ = hidden;};
protected:
	bool hiddenInterface_;
public:
	//-----------------------------------------------
	//ゲームシステム関係
	//-----------------------------------------------
	virtual int gameTurnprocess(IDirect3DDevice9 *pDev);


	inline int& DashSwitch(){return pFloorInstance_->DashSwitch_;};
	inline int& DashStopDelay(){return pFloorInstance_->DashStopDelay_;};
	double DashSpeed();
	inline int& DashEnemyCount(){return pFloorInstance_->DashEnemyCount_;};
	inline int& DashAspect(){return pFloorInstance_->DashAspect_;};

protected:
	inline int& forDoubleInput_X(){return pFloorInstance_->forDoubleInput_X_;};
	inline int& forDoubleInput_Y(){return pFloorInstance_->forDoubleInput_Y_;};
	inline int& forDoubleInput_migi(){return pFloorInstance_->forDoubleInput_migi_;};
	inline int& forDoubleInput_hidari(){return pFloorInstance_->forDoubleInput_hidari_;};
	inline int& forDoubleInput_ue(){return pFloorInstance_->forDoubleInput_ue_;};
	inline int& forDoubleInput_shita(){return pFloorInstance_->forDoubleInput_shita_;};
public:
	//int DashCondition_HP;
	inline cCoordinate& DashTGT(){return pFloorInstance_->DashTGT_;};
	inline int& dashflag(){return pFloorInstance_->dashflag_;};


	inline int& count_minimap(){return pFloorInstance_->count_minimap_;};


	//入力待ち状態かどうか
	virtual bool isInputWaiting();
protected:
	inline bool& isInputWaitingflag(){return pFloorInstance_->isInputWaitingflag_;};
public:

	inline bool& cannotinput(){return pFloorInstance_->cannotinput_;};

	//存在するすべてのキャラのリスト
	inline vector<pcCharacter>& CharaList(){return pFloorInstance_->AnimationManager_.CharaList_;};
	inline vector<pcCharacter>& visibleCharaList(){return pFloorInstance_->AnimationManager_.visibleCharaList_;};

	//ただし退避リストを除く
	inline vector<pcCharacter>& stockCharaList(){return pFloorInstance_->AnimationManager_.stockCharaList_;};
public:
	virtual int pushbackCharaList(pcCharacter pchara);//必ずこれを使う
	//存在するすべてのアイテムのリスト
	inline vector<pcDroping>& DropList(){return pFloorInstance_->AnimationManager_.DropList_;};
	inline vector<pcDroping>& visibleDropList(){return pFloorInstance_->AnimationManager_.visibleDropList_;};
public:
	virtual int pushbackDropList(pcDroping pdrop);//必ずこれを使う

public:
	inline cEventManager& EventManager(){return pFloorInstance_->EventManager_;};

public:

	inline cMessageWindow& MessageW(){return pFloorInstance_->MessageW_;};
	inline cVectorOfControlLayer& primaryMenuControlLayerV(){return primaryMenuControlLayerV_;};


	inline cCutInManager& CutInM(){return pFloorInstance_->CutInM_;};



	//描画関係

	inline cAnimationManager& AnimationManager(){return pFloorInstance_->AnimationManager_;};
	
	inline cDramaManager& DramaManager(){return pFloorInstance_->DramaManager_;};
	


	//-------------------------------------
	//ターンエンド
	//-------------------------------------
	virtual void ターンエンド();
protected:
	inline int& turnEnd_flag(){return pFloorInstance_->turnEnd_flag_;};

protected:
	virtual void MakeResultWindow(int clear);
public:

	virtual void KnockOutHero();
	virtual void GameOver();
	virtual void GameOverAndContinue();
	virtual void GameOverAndContinueAndSuspend();
	virtual void GameOverAndRestart();

	virtual int ダンジョン引き上げ要請();


	virtual void GameEndSavetyPrepareing();

	virtual void GameBaseEnd();
	virtual void GameBaseStart();

	virtual void GameReturnTitle();
protected:
	virtual void GameClear();
	virtual void GamePullout();
	virtual void GameOverpreprocess();
	virtual void GameOverprocess();
//***************************************************
//-----------------------------------------------
//ダンジョン選択関係
//-----------------------------------------------
//***************************************************
protected:
	tstring nextDungeon;
	virtual void resetDungeon(const tstring DungeonID);

public:
	virtual bool isReservedNextDungeon(){return nextDungeon != _T("");};

	virtual void SelectDungeon(const vector<tstring>& Dungeons);

	virtual void GotoDungeon(const tstring& DungeonID, const int saveFileNum, const map<tstring,int>& privateFlags, const tstring& savefileName);
	virtual void GotoDungeon(const tstring& DungeonID);
	
	virtual void ContinueDungeon(pcSaveQuest continueQuest);

	virtual void Dungeonprocess(const tstring& DungeonsID);	

	virtual bool isPlayerCharacterAccordingStartRegulation(const tstring& DungeonsID, StyleString& error);
	virtual bool isRegureCharacter(const tstring& DungeonsID, pcCharacter chara);
	virtual StyleString StartRegulationMassage(const tstring& DungeonsID);
//***************************************************
//-----------------------------------------------
//ユーティリティ関係
//-----------------------------------------------
//***************************************************
public:
	virtual pcLandform RandomPlace();


	virtual int メッセージ(StyleString& Message);

	virtual int メッセージ消去();

	virtual int メッセージ決定待ち();
	virtual int メッセージ何かボタン入力待ち();

	virtual void メニューを開く();
	virtual void メニューを一枚閉じる();
	virtual void メニューを閉じる();
	virtual void メニューリフレッシュ();
	virtual void メニューリレンダ();

	virtual bool メニュー表示可能();

public:
	virtual pcLandform 代理地形(pcLandform main, int orderNum){return alternativeLand(main,orderNum);};
	
protected:
	//代理地形
	virtual pcLandform alternativeLand(pcLandform main, int orderNum);
	

public:

	//-------------------------
	//目視関係
	virtual int キャラクター目視可(pcCharacter penemychara);//主人公から
	virtual int 落ち物目視可(pcDroping pdrop);//主人公から
	virtual int キャラクター目視可(pcCharacter frompchara, pcCharacter topchara);
	virtual int 落ち物目視可(pcCharacter frompchara, pcDroping pdrop);

	//キャラクターの目視範囲を無視して判定
	virtual int キャラクター目視可(pcCharacter frompchara, pcCharacter topchara, const int range);
	

	virtual int 地形目視可(pcCharacter frompchara, pcLandform toLand);

	virtual int 地形目視可(pcLandform fromLand, pcLandform toLand);

	virtual bool キャラが同室または出口または隣接か(pcCharacter me, pcCharacter enemy);

	//-------------------------
	//リスト作成
	virtual int キャラクター部屋キャラリスト(pcCharacter frompchara, vector<pcCharacter>& outputlist);
	virtual int キャラクター目視可範囲キャラリスト(pcCharacter frompchara, vector<pcCharacter>& outputlist);
	virtual int キャラクター目視可範囲落ち物リスト(pcCharacter frompchara, vector<pcDroping>& outputlist);

	virtual int 地形目視可範囲出口リスト(pcCharacter frompchara, vector<pcLandform>& outputlist);

	virtual int キャラクター隣接地形リスト(pcCharacter frompchara, vector<pcLandform>& outputlist);



	//-------------------------
	//地形関係
	virtual pcLandform 配置安全地形(pcLandform target,pcCharacter pchara, int firstsetting = 0);
	virtual pcLandform 配置安全地形(pcLandform target,pcDroping pdrop, int firstsetting = 0);
	
	virtual bool キャラ配置安全(pcLandform target);
	virtual bool キャラ配置安全(pcLandform target,pcCharacter pchara);
	virtual bool 落ち物配置安全(pcLandform target);

	virtual pcLandform 存在安全地形(pcLandform target,pcCharacter pchara);
	virtual pcLandform 存在安全地形(pcLandform target,pcDroping pdrop);

	//3マス以上はなれたところに関して関与しない。
	virtual bool 壁はさみ判定(pcLandform start, pcLandform end);

	virtual pcLandform キャラ足元地形(pcCharacter pchara);
	virtual pcLandform キャラ前地形(pcCharacter pchara,int distance);
	virtual pcLandform キャラ視点方角地形(pcCharacter pchara, int plusaspect, int distance);

	virtual pcLandform 隣接地形(pcLandform outLand, int aspect, int distance);


	//-------------------------
	//検索関係
	//FindUtility.hも参照
	bool 壁反射判定且検索(pcLandform &outLand, int &outaspect, pcLandform comeLand, int comeaspect);
	bool 壁曲がり判定且検索(pcLandform &outLand, int &outaspect, pcLandform comeLand, int comeaspect);
	
	//射程内の障害物を検索する。障害物がなかった場合最大射程を返す。
	virtual pcLandform 射程内障害物検索(pcLandform start, int aspect, int range, bool stopWall = 1, bool stopChara = 0, bool stopDrop = 0, bool stopNonthrough = 0);
	//軌跡を足して返す//スタートは含まない
	virtual pcLandform 射程内障害物検索(vector<pcLandform>& landpass ,pcLandform start, int aspect, int range, bool stopWall = 1, bool stopChara = 0, bool stopDrop = 0, bool stopNonthrough = 0);

	virtual pcLandform 障害物検索(pcLandform start, int aspect, bool stopWall = 1, bool stopChara = 0, bool stopDrop = 0);


	virtual pcLandform 落下可能地点検索(pcLandform start, pcDroping pdrop);
	virtual pcLandform 落下可能地点検索(pcLandform start, pcCharacter pchara);

	//-------------------------
	//相互関係関係
	virtual bool キャラクター敵対判定(pcCharacter pcharaA,pcCharacter pcharaB);

	
	virtual bool ボス戦中判定();

	//-------------------------
	//そのた

	//キャラのポインタからラッピングずみを検索
	virtual pcCharacter システム用キャラ検索(pcCharacter pchara);

	virtual int 敵の数();//敵の数


	virtual tstring 漢数字(int digit, int flag = 0);

//***************************************************
//-----------------------------------------------
//ダッシュ関係
//-----------------------------------------------
//***************************************************
public:
	virtual int Dashprocess(IDirect3DDevice9 *pDev);
	virtual int DashRiskCulcurate(int& output, int nextaspect);
	virtual int DashAspectCulcurate();

//***************************************************
//-----------------------------------------------
//マッピング関係
//-----------------------------------------------
//***************************************************
public:


	virtual bool 現在地からマッピング(pcCharacter pchara);
	virtual bool ある地点からマッピング(pcLandform pland);
	virtual bool 全マッピング();
	virtual bool 全マッピング解除();

	virtual const int フロア明かり状態();
	virtual const int フロア明かり状態ビジブル();
	virtual void フロア明かり追加();
	virtual void フロア明かり削除();

	virtual const int フロア飛ばず状態();
	virtual void フロア飛ばず追加(int turn);
	virtual void フロア飛ばず削除();

	virtual const int フロア時間停止状態();
	virtual void フロア時間停止追加(int turn);
	virtual void フロア時間停止削除();

	virtual const int フロア時間飛ばし状態();
	virtual void フロア時間飛ばし追加(int turn);
	virtual void フロア時間飛ばし削除();

	virtual bool 八方判定(c4DVector from, c4DVector to);
	virtual bool 八方判定(cCoordinate from, cCoordinate to);

	
	virtual bool アイテムマッピング(pcDroping pdrop);
	virtual bool アイテム強制マッピング(pcDroping pdrop);


	virtual bool 地形整理();
	virtual bool 穴掘り(pcLandform pland, int messageflag = 0);
	virtual bool 水掘り(pcLandform pland, int messageflag = 0);
	virtual bool 水埋め(pcLandform pland, int messageflag = 0);
	virtual bool 部屋つぶし(pcLandform pland, int range, int messageflag = 0);
	virtual bool 大部屋(int messageflag = 1);


	virtual bool 暗闇更新();
	virtual bool 暗闇付与(pcLandform pland, int range);
//protected:
	virtual bool landchange(pcLandform pland, int kind);

//***************************************************
//-----------------------------------------------
//攻撃関係
//-----------------------------------------------
//***************************************************
public:

	virtual int 直接攻撃要請(pcCharacter pchara);
	virtual int 直接攻撃要請_強制混乱なし(pcCharacter pchara);

	virtual int 直接攻撃可(pcCharacter pchara);

	virtual int 投擲攻撃接近(pcCharacter pchara, pcDroping pdrop, int passthroughflag = 0);

	virtual int 落ち物接近(pcLandform pland, pcDroping pdrop);

	virtual int 魔法接近(pcCharacter pchara, pcDroping pdrop);

	virtual int 攻撃接近(pcAttackinformation pattackinfo);

	virtual pcAttackinformation 直接攻撃攻撃情報取得(pcCharacter pchara);

	//地形しか見ない
	virtual bool isCanNeighberAttack(pcCharacter pchara, int aspect, int distance, int diagonthrough, int wallthrough);

	//攻撃の情報を得る
	virtual int GetNormalAttackinformation(pcCharacter pchara,pcAttackinformation &pcattackinfo);

protected:

	//攻撃する
	virtual int Attackcharacter(pcCharacter pchara);

	//攻撃があたったかどうか
	virtual int isAttackHit(pcAttackinformation pattackinfo);

	//攻撃があたった
	virtual int AttackHit(pcAttackinformation pattackinfo);

	//攻撃が外れた
	virtual int AttackMiss(pcAttackinformation pattackinfo);

	//ダメージ量計算
	virtual int CalculateDamege(pcAttackinformation pattackinfo);

	//アイテム投擲があたったかどうか
	virtual int isThrowHit(double hitpower, double dodgepower);


//***************************************************
//-----------------------------------------------
//移動関係
//-----------------------------------------------
//***************************************************
public:

	virtual int 方向転換要請(pcCharacter pchara, int aspect);
	
	virtual int 方向転換要請_主人公専用(pcCharacter pchara, int aspect);

	virtual int 泥棒確認移動可_主人公専用(pcCharacter pchara, int moveaspect);

	virtual int 移動要請(pcCharacter pchara, int moveaspect, int badyaspect = -1);

	virtual int すれ違い要請(pcCharacter pchara, int moveaspect, int badyaspect = -1);

	//地形とキャラ両方見る
	virtual bool 隣接地形移動可(pcCharacter pchara, int aspect);
	//地形のみ見る
	virtual bool 隣接地形移動可地形のみ(pcCharacter pchara, int aspect);

	//地形とキャラ両方見る
	virtual bool ワープ地形移動可(pcCharacter pchara, pcLandform pland);


	//キャラクター判定込みで乗り入れ可
	virtual bool isCanNeighberInto(pcCharacter pchara, int aspect);
	//キャラクター判定込みで乗り入れ可
	virtual bool isCanNeighberInto(pcLandform pland, int aspect, int waterwalking, int wallwalking, int airwalking);
	//キャラクター判定なしで乗り入れ可
	virtual bool isCanNeighberThrough(pcCharacter pchara, int aspect);
	//キャラクター判定なしで乗り入れ可
	virtual bool isCanNeighberThrough(pcLandform pland, int aspect, int waterwalking, int wallwalking, int airwalking);

	virtual int ワープ要請(pcCharacter pchara, pcLandform pland);

	virtual int 吹き飛ばし要請(pcCharacter subjectchara, pcCharacter pchara, int aspect,int distance,int collisionpower);
	virtual int 引き寄せ要請(pcCharacter subjectchara, pcCharacter pchara, int aspect,int distance);
	virtual int 場所替え要請(pcCharacter subjectchara, pcCharacter pchara);
	
	virtual int 投げ飛ばし要請(pcCharacter subjectchara, pcCharacter pchara, pcLandform toland, int collisionpower, int dropcollition, double speed = 1.0);
	
	virtual int フロア更新要請();
	
	//敵サーチ振り向き
	virtual int 自動振り向き(int& notfirstflag);

	virtual int キャラ落下(pcCharacter pchara, pcLandform pland, int dropcollition = 1);//dropcollitionは落ち物反応性

	virtual int 分身要請(pcCharacter pchara);

	virtual int アイテム隠れ(pcCharacter pchara, pcDroping pdrop);
	virtual int アイテム隠れ解除(pcCharacter pchara, pcLandform pland);

	//射程内の障害物を検索する。障害物がなかった場合最大射程を返す。
	virtual pcLandform キャラ飛ばし障害物検索(pcLandform start, int aspect, int range, bool Wallthrough = 0);

protected:
	//キャラを除去する。
	virtual void removecharacter(pcCharacter pchara);

	//強制的に動かす
	virtual void movecharacter(pcCharacter pchara, int moveaspect, int badyaspect);

	//強制的に動かす
	virtual void passmovecharacter(pcCharacter pchara, pcCharacter pchara2, int moveaspect, int badyaspect);

	//ワープ的に動く
	virtual void warpcharacter(pcCharacter pchara, pcLandform pland);
	
	//強制的にキャラを吹き飛ばし
	virtual void ThrowCharacter(pcCharacter subjectchara, pcCharacter pchara, int aspect,int distance,int collisionpower,int nearstop = 0);
	
	//強制的にキャラを投げ飛ばし
	virtual void JumpCharacter(pcCharacter subjectchara, pcCharacter pchara, pcLandform toland, int collisionpower, int dropcollition, double speed);
	
	//強制的にキャラを場所替え
	virtual void SwapCharacter(pcCharacter subjectchara, pcCharacter pchara);
	
	//キャラクターを地面リンクから開放する//いた場所を返す。
	virtual pcLandform unlinkcharacter(pcCharacter pchara);
	//キャラクターを地面リンクする//いた場所を返す。
	virtual void linkcharacter(pcCharacter pchara, pcLandform pland);

	//次のフロアに移動
	virtual void movefloor();

	//フロアのキャラを除去する。
	virtual void allremovefloorcharacter();

	//キャラクターを地面リンクから開放して退避リストに入れる
	virtual void stockInCharacter(pcCharacter pchara);
	//キャラクターを退避リストからもとに戻す。plandに衝突なし落下判定
	virtual bool stockOntCharacter(pcCharacter pchara, pcLandform pland);


//***************************************************
//-----------------------------------------------
//アイテム状態変化関係
//-----------------------------------------------
//***************************************************
public:
	//-------------------------
	//識別関係

	//識別補助システム
	virtual int 動的識別(pcDroping pdrop, int massage = 1);

	virtual int 識別(pcDroping pdrop, int massage = 1);
	virtual int 状態のみ識別(pcDroping pdrop, int massage = 1);
	virtual int 修正のみ識別(pcDroping pdrop, int massage = 1);
	virtual int 名称のみ識別(pcDroping pdrop, int massage = 1);

	virtual int カテゴリ未識別化(pcDroping pdrop, int massage = 1);

	virtual int アイテム祝福(pcDroping pdrop, int massage = 1);
	virtual int アイテム呪い(pcDroping pdrop, int massage = 1);
	//強制設定
	virtual int アイテム状態強制設定祝福(pcDroping pdrop, int massage = 1);
	virtual int アイテム状態強制設定通常(pcDroping pdrop, int massage = 1);
	virtual int アイテム状態強制設定呪い(pcDroping pdrop, int massage = 1);

	//フラグ全ツケ
	static int アイテム店フラグ(pcDroping pdrop, int flag);
	static int アイテム割引フラグ(pcDroping pdrop, int flag);

	virtual int アイテム劣化(pcDroping pdrop, int massage = 1);
	virtual int アイテム強化(pcDroping pdrop, int massage = 1);
	//設定
	virtual int アイテム品質強制設定(pcDroping pdrop, int quality, int massage = 1);

	virtual int 装備強制はずし(pcDroping pdrop, int massage = 1);
	virtual int 強制装備(pcDroping pdrop, int part, int massage = 1);
	virtual int 装備回し要請(pcCharacter pchara, int massage = 1);
	virtual int 強制装備回し(pcCharacter pchara, int massage = 1);
//***************************************************
//-----------------------------------------------
//拾得関係
//-----------------------------------------------
//***************************************************

public:
	virtual int 罠自然湧きID(int outputID[3], int flag = 0);
	virtual int アイテム自然湧きID(int outputID[3], int flag = 0);
	virtual int アイテム店湧きID(int outputID[3], int flag = 0);
	
	virtual int アイテムレア湧きID(int outputID[3], double Rarity, int flag = 0);//Rarity=(0~1)0==Rare

	virtual int アイテムレア種別湧きID(int outputID[3], double Rarity,
							double Equipment,
							double Bullet,
							double Spell,
							double Food,
							double Drink,
							double Book,
							double Box,
							double MoneyBag,
							double Otherint,
							int flag = 0);//Rarity=(0~1)0==Rare

	virtual int 敵自然湧きID(int outputID[3], int flag = 0);
	
	virtual int 敵グループ湧き(pcLandform land);
	//-------------------------
	//初期設置関係

	virtual int 初期設置(pcCharacter go, pcLandform land, int firstsetting = 0);
	virtual int 初期設置(pcDroping go, pcLandform land, int flexible = 1, int firstsetting = 0);

	//階段などの重要オブジェクトを配置する
	virtual int 初期設置_大切(pcDroping go, pcLandform land);

public:
	virtual int アイテム合成(vector<pcDroping> vpdrop, int mitamaFlag = 1, int isyuFrag = 1, int cocktailFlag = 1);

public:
	virtual int 落ち物落下(pcDroping pdrop, pcLandform pland);

	virtual int 落ち物ワープ(pcDroping pdrop, pcLandform pland);

	//上に落ちるが発動しない//単体で使用するとバグる
	virtual int 落ち物設置(pcDroping pdrop, pcLandform pland, bool anime_pararell = false);

	virtual int 落ち物落し物(pcDroping pdrop, pcLandform pland);

	virtual int 落ち物変化(pcDroping frompdrop, pcDroping topdrop);

	virtual int 落ち物すり替え(pcDroping frompdrop, pcDroping topdrop);

	virtual int 落ち物破壊要請(pcDroping pdrop, int lossFlag = 0);

	virtual int ころび要請(pcCharacter pchara, int DropItemNum, int mustFlag = 0, int breakFlag = 1);

	virtual int 落ち物非破壊非設置放棄要請(pcDroping pdrop);

	virtual int 足元拾得要請(pcCharacter pchara);

	virtual int 遠隔拾得要請(pcCharacter pchara, pcDroping pdrop);
	virtual int 遠隔拾得要請_持ち物余白無考慮(pcCharacter pchara, pcDroping pdrop);

	virtual int 泥棒要請(pcCharacter psubjectchara, pcDroping pdrop, int massageFlag = 1, int mustflag = 0);
	
	virtual int アイテム弾き飛ばし要請(pcCharacter psubjectchara, pcDroping pdrop, int distanse, int massageFlag = 1);
	
	virtual int 落ち物まとめ要請(pcDroping pdrop, pcCharacter pchara);

	virtual int 主人公お金拾得要請(pcDroping pdrop);
	virtual int 主人公お金増減(int value);
	virtual int 主人公お金所持量();
	virtual int 主人公お金所持量最大();

	virtual bool 落ち物拾得可(pcDroping pdrop, pcCharacter pchara);
	virtual bool 落ち物拾得可_持ち物余白無考慮(pcDroping pdrop, pcCharacter pchara);


	virtual int 無条件投擲要請(pcCharacter subjectchara, pcLandform pland, pcDroping pdrop, int aspect, int option = DRAWTYPE_NORMAL, int throwrange = -1);
	virtual int 投擲要請(pcCharacter pchara, pcDroping pdrop, int option = DRAWTYPE_NORMAL, int throwrange = -1);
	virtual int 吹き飛ばし要請(pcCharacter subjectchara, pcDroping pdrop, int aspect,int distance);
	virtual int 引き寄せ要請(pcCharacter subjectchara, pcDroping pdrop, int aspect,int distance);
	
	virtual int 魔法発射要請(pcCharacter pchara, pcDroping pdrop);
	

	virtual int 足元設置要請(pcCharacter pchara, pcDroping pdrop);

	virtual bool 落ち物放棄可(pcCharacter pchara, pcDroping pdrop);

	virtual bool 落ち物投擲可(pcCharacter pchara, pcDroping pdrop);

	virtual bool 落ち物設置可(pcDroping pdrop, pcLandform pland);
	virtual bool 落ち物設置可_アイテム判定無し(pcDroping pdrop, pcLandform pland);

	virtual int 足元交換要請(pcCharacter pchara, pcDroping pdrop);

	//アイテムの総サイズを返す
	virtual int ItemSize(vector<pcDroping> vpdrop);

	//条件を吟味しない
	virtual int 落ち物入れ要請(pcDroping box, pcDroping pdrop);
	virtual int 落ち物入れ子展開要請(pcDroping pdrop);
	virtual int 落ち物出し要請(pcDroping pdrop, pcCharacter receiver);
	virtual int 落ち物出し要請(pcDroping pdrop, pcDroping receiver);
	virtual int 落ち物ばら撒き要請(pcDroping box);
	virtual int 落ち物入れ替え要請(pcDroping pdrop, pcDroping pdrop2);

	virtual int 倉庫入れ要請(vector<pcDroping>& store, const int storecapacity, pcDroping pdrop);
	virtual int 倉庫出し要請(vector<pcDroping>& store, pcDroping pdrop);

	virtual int アイテム倉庫行き要請(pcDroping pdrop);

	static int 厄システム挿入要請(pcSaveQuest pQuest, pcDroping pdrop);
	static void 厄システム挿入要請(pcSaveQuest pQuest, vector<pcDroping>& vpdrop);
	static int 厄ポイント冒険評価(int gameClearFlag, int homeFlag, pcSaveQuest pQuest, pcCharacter pPlayer);
	virtual int 厄システム整理();//保存する直前に呼べ！

	virtual int 入れ子復元(pcDroping box, pcDroping pdrop)
	{return putInto_simple(box,pdrop);};
	virtual int 入れ子復元(pcCharacter pchara, pcDroping pdrop)
	{return haveDroping(pchara,pdrop);};
protected:
	
	//強制的に持ち物を投げる	
	virtual void ThrowDroping(pcCharacter subjectchara, pcLandform fromland, pcDroping pdrop, int aspect ,int distance, int passthrough, int option = DRAWTYPE_NORMAL, int nearstop = 0);//nearstop寸止め
	//強制的に持ち物を投げる	
	virtual void himanageThrowDroping(pcCharacter subjectchara, pcLandform fromland, pcDroping pdrop, int aspect ,int distance, int passthrough, int option = DRAWTYPE_NORMAL, int nearstop = 0);//nearstop寸止め

	//強制的に魔法を発射する
	virtual int MagicShooting(pcCharacter pchara, pcDroping pdrop);
	

	//強制的に足元から持ち物を拾う
	virtual int pickupDropingfromStep(pcCharacter pchara);

	//強制的に持ち物を拾う
	virtual int pickupDroping(pcCharacter pchara, pcDroping pdrop);

	//強制的に足元と持ち物を交換する
	virtual int exchangeDropingfromStep(pcCharacter pchara, pcDroping pdrop);

	
	//強制的に持ち物を持つ
	//release済みの持ち物限定
	virtual int haveDroping(pcCharacter pchara, pcDroping pdrop);
	
	//ドロップリストに追加する
	//持ち込みアイテム、階をまたぐアイテムにつかう
	virtual int insertDroplist(vector<pcDroping> pdropvec, int appreciating = 0);

	//強制的に落ち物を床に設置する。
	virtual int setDropingOnLand(pcDroping pdrop, pcLandform pland);

	//強制的に落ち物を地形から引き離す
	virtual pcDroping releaseDroping(pcLandform pland);
	//強制的に落ち物を所持から引き離す
	virtual pcDroping releaseDroping(pcDroping pdrop);


	//フロアの落ち物を除去する。
	virtual void allremovefloorDroping();




	//落ち物を除去する。
	virtual void removedroping(pcDroping pdrop, int lossFlag);
	//落ち物を除去する。
	virtual void removedropingnoanime(pcDroping pdrop, int lossFlag);

	//----------------------------------------
	//箱系
	//----------------------------------------
	//強制的に入れる
	virtual int putInto(pcDroping box, pcDroping pdrop);
	//強制的に入れる
	virtual int putInto_simple(pcDroping box, pcDroping pdrop);

	//強制的に出す
	virtual int putOut(pcDroping pdrop, pcCharacter receiver);
	//強制的に出す
	virtual int putOut(pcDroping pdrop, pcDroping receiver);

	//強制的に中身を全部ばら撒く
	virtual int AlldropOut(pcDroping box);

	//強制的に入れ替えする
	virtual int changeDrop(pcDroping pdrop, pcDroping pdrop2);

	//----------------------------------------
	//倉庫系
	//----------------------------------------
	//強制的に入れる
	virtual int storeInto(vector<pcDroping>& store, pcDroping pdrop);
	//強制的に出す
	virtual int storeOut(vector<pcDroping>& store, pcDroping pdrop);//主人公に持たせる
	virtual int storeOut(vector<pcDroping>& store, pcDroping pdrop, pcLandform pland);

//アニメクラス専用関数
public:
	//キャラを消す。(不要)
	//virtual void キャラ消去_アニメ用(pcCharacter pchara);

//***************************************************
//-----------------------------------------------
//生成関係
//-----------------------------------------------
//***************************************************
public:
	//-------------------------
	//キャラ生成関係
	virtual pcCharacter キャラクター生成(tstring name,int CLASS,int Forse,pcLandform land = NULLLAND);
	virtual pcCharacter キャラクター生成(int ID,int CLASS,int Forse,pcLandform land = NULLLAND);

	virtual pcCharacter 主人公交代(int ID);
	virtual bool 主人公交代(pcCharacter pchara);

	//居眠り付き
	virtual pcCharacter キャラクター生成_自然湧き(int ID,int CLASS,int Forse,pcLandform land = NULLLAND);

	//-------------------------
	//落ち物生成関係
	virtual pcDroping 落ち物生成(tstring name,const int dpack_int[] = NULL,pcLandform land = NULLLAND, int firstsetting = 0);
	virtual pcDroping 落ち物生成(int ID,const int dpack_int[] = NULL,pcLandform land = NULLLAND, int firstsetting = 0);

	virtual pcDroping 落ち物生成_設置なし(tstring name,const int dpack_int[] = NULL, int firstsetting = 0);
	virtual pcDroping 落ち物生成_設置なし(int ID,const int dpack_int[] = NULL, int firstsetting = 0);

	//-------------------------
	virtual void 宴会生成(int roomindex, int partyFlag = 1);
	virtual void 宴会発覚(int roomindex);
	
	virtual int 店入出判定(int roomindex);
	virtual pcCharacter 店長(int roomindex);
	virtual int 店入る(int roomindex);
	virtual int 店出る(int roomindex);
	virtual int 店退出確認();

	virtual int 店ツケ可能金額();
	virtual int 店アイテム購入清算(pcCharacter pShopowner);
	virtual int 店アイテム売却清算(pcCharacter pShopowner);	

	virtual void 店生成(int roomindex, int shopFlag = 1);
	virtual void 店アイテム追加(pcDroping pdrop, pcCharacter pShopowner);
	virtual int 店レベル(double fund);
	virtual int 店主持ち金();
	virtual int 店アイテム数(int level);


protected:
	virtual int 店購入総価値差額再計算(pcCharacter pShopowner);
	virtual int 店売却総価値差額再計算(pcCharacter pShopowner);
	
	virtual int 店購入価値再計算(pcDroping pdrop, pcCharacter pShopowner);
	virtual int 店売却価値再計算(pcDroping pdrop, pcCharacter pShopowner);
	virtual int 店購入価値差額再計算(pcDroping pdrop, pcCharacter pShopowner);
	virtual int 店売却価値差額再計算(pcDroping pdrop, pcCharacter pShopowner);
	
	virtual int 店売却アイテムリスト(vector<pcDroping>& output, pcCharacter pShopowner);
	virtual int 店購入アイテムリスト(vector<pcDroping>& output, pcCharacter pShopowner);

	virtual int 店アイテム価値再計算(pcDroping pdrop, pcCharacter pShopowner);
	

	virtual int 店アイテム購入(pcDroping pdrop, pcCharacter pShopowner);
	virtual int 店アイテム売却(pcDroping pdrop, pcCharacter pShopowner);
	virtual int 店金支払い(int value);
	virtual int 店金受け取り(int value);
public:
	virtual int 店後払い支払い();
	virtual int 店請求金額(pcCharacter pShopowner);
	virtual int 店支払い金額(pcCharacter pShopowner);
	virtual void 泥棒発覚();
	virtual bool 泥棒状態();
//***************************************************
//-----------------------------------------------
//状態変化関係
//-----------------------------------------------
//***************************************************

	//カラー
	#define DAMAGE_COLOR (0xFFFF3300)
	#define RECOVER_COLOR (0xFF00FF66)
	#define EXP_COLOR (0xFF0099FF)
	#define LV_COLOR (0xFFFF6600)
	#define HUNGER_COLOR (0xFFFF9900)

public:

	virtual int 回復要請(pcCharacter pchara, int recovery, int Messageflag = 1);
	virtual int HP設定要請(pcCharacter pchara, int afterHP, int Messageflag = 1);
	virtual int 最大HP拡張要請(pcCharacter pchara, int extention, int Messageflag = 1);
	virtual int 最大HP縮小要請(pcCharacter pchara, int reduction, int Messageflag = 1);

	virtual int 満腹度回復要請(pcCharacter pchara, double recovery, int Messageflag = 1);
	virtual int 満腹度減少要請(pcCharacter pchara, double hunger, int Messageflag = 1);
	virtual int 満腹度設定要請(pcCharacter pchara, double afterSP, int Messageflag = 1);
	virtual int 最大満腹度拡張要請(pcCharacter pchara, int extention, int Messageflag = 1);
	virtual int 最大満腹度縮小要請(pcCharacter pchara, int reduction, int Messageflag = 1);


	virtual int 速度増加要請(pcCharacter pchara, int turn, int Messageflag = 1);
	virtual int 速度減少要請(pcCharacter pchara, int turn, int Messageflag = 1);

	//精神系
	virtual int 眠り要請(pcCharacter pchara, int turn, int Messageflag = 1);
	virtual int バクスイ要請(pcCharacter pchara, int turn, int Messageflag = 1);
	virtual int びっくり要請(pcCharacter pchara, int turn, int Messageflag = 1);
	virtual int 金縛り要請(pcCharacter pchara, int turn, int Messageflag = 1);

	virtual int 封印要請(pcCharacter pchara, int turn, int Messageflag = 1);
	virtual int 空振り要請(pcCharacter pchara, int turn, int Messageflag = 1);
	virtual int 貧乏要請(pcCharacter pchara, int turn, int Messageflag = 1);
	virtual int 錯乱要請(pcCharacter pchara, int Messageflag = 1);
	virtual int 臆病要請(pcCharacter pchara, int turn, int Messageflag = 1);
	virtual int 嫉妬要請(pcCharacter pchara, int turn, int Messageflag = 1);
	virtual int 狂乱要請(pcCharacter pchara, int turn, int Messageflag = 1);
	virtual int 無意識要請(pcCharacter pchara, int turn, int Messageflag = 1);

	//身体系
	virtual int 脱力要請(pcCharacter pchara, int power, int turn, int Messageflag = 1);
	virtual int 元気要請(pcCharacter pchara, int power, int turn, int Messageflag = 1);
	
	virtual int 軟弱要請(pcCharacter pchara, int power, int turn, int Messageflag = 1);
	virtual int 頑強要請(pcCharacter pchara, int power, int turn, int Messageflag = 1);

	virtual int 泥酔要請(pcCharacter pchara, int turn, int Messageflag = 1);
	virtual int 氷付け要請(pcCharacter pchara, int turn, int Messageflag = 1);
	virtual int 無敵要請(pcCharacter pchara, int turn, int Messageflag = 1);
	virtual int 鳥目要請(pcCharacter pchara, int turn, int Messageflag = 1);

	//呪術系
	virtual int 死の誘い要請(pcCharacter pchara, pcCharacter subject, int turn, int Messageflag = 1);
	virtual int 身代わり要請(pcCharacter pchara, pcCharacter subject, int turn, int Messageflag = 1);
	virtual int 変身要請(pcCharacter pchara, pcCharacter subject, int turn, int Messageflag = 1);
	virtual int 擬態要請(pcCharacter pchara, int turn, int Messageflag = 1);

	virtual int 精神異常治療要請(pcCharacter pchara, int Messageflag = 1);
	virtual int 身体異常治療要請(pcCharacter pchara, int Messageflag = 1);
	virtual int 呪術異常治療要請(pcCharacter pchara, int Messageflag = 1);
	virtual int 速度異常治療要請(pcCharacter pchara, int Messageflag = 1);

	virtual bool 精神異常状態(pcCharacter pchara);
	virtual bool 身体異常状態(pcCharacter pchara);
	virtual bool 呪術異常状態(pcCharacter pchara);
	virtual bool 速度異常状態(pcCharacter pchara);

	virtual int やりすごし要請(pcCharacter pchara, int turn, int Messageflag = 0);

	virtual int 目薬要請(pcCharacter pchara, int Messageflag = 1);

	virtual int 強制ダメージ要請(pcCharacter pchara, int damage, int Messageflag = 1, int animationflag = 1);
	
	virtual int 即死要請(pcCharacter pchara, int Messageflag = 1, int animationflag = 1);

	virtual int 強制退場要請(pcCharacter pchara, int Messageflag = 1, int animationflag = 1);

	virtual int 経験値獲得(pcCharacter pchara, int point, int Messageflag = 1);
	virtual int レベルアップ(pcCharacter pchara, int uplv = 1, int Messageflag = 0);
	virtual int レベルダウン(pcCharacter pchara, int downlv = 1, int Messageflag = 0);

	virtual int 強制ひるみ要請(pcCharacter pchara);//強制的に(実質)等速以下にする。
	
	virtual int 強制ターンエンド要請(pcCharacter pchara);//強制的に主人公ターン間で待機させる。
	virtual int 全キャラ強制ターンエンド要請();//強制的に主人公ターン間で待機させる。
protected:
	//経験値を得る
	virtual int EXTcharge(pcAttackinformation pattackinfo);
	//経験値を得る
	virtual int EXTcharge(pcCharacter pchara, int point, int Messageflag = 1);
	//レベルアップ。
	virtual int levelupCharacter(pcCharacter pchara, int uplv = 1);
	//レベルアップ。
	virtual int leveldownCharacter(pcCharacter pchara, int uplv = 1);

	//--------------------------
	//ダメージ関係

	//ダメージ処理
	virtual int DamageCharacter(pcCharacter pchara, int damage, int animationflag = 1, double effective = 1.0);

	//死亡処理
	virtual int DieCharacter(pcCharacter pchara, int Messageflag = 1);

	//満腹度減少処理
	virtual int hungerCharacter(pcCharacter pchara, double hunger);

	//--------------------------
	//回復関係

	//回復処理
	virtual int RecoverCharacter(pcCharacter pchara, int recovery, int animationflag = 1);
	//最大HP拡張処理
	virtual int HPextentionCharacter(pcCharacter pchara, int extention);
	//最大HP縮小処理
	virtual int HPreductionCharacter(pcCharacter pchara, int reduction);

	//満腹度回復処理
	virtual int StmRecoverCharacter(pcCharacter pchara, int recovery);
	//最大満腹度拡張処理
	virtual int StmextentionCharacter(pcCharacter pchara, int extention);

};


#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cDungeonSystem> pcDungeonSystem;


extern pcDungeonSystem sg_pDungeonSystem;



//ランダム関数系

double random();

//[0,max)
int random_int(int max);


//Range[min,max)
double random_range(double min,double max);