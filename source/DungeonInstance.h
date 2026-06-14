#pragma once
#include "ccontrol.h"
#include "cPlayerChara.h"
#include "cItem.h"
#include "cMap.h"



#include "cDrawingObject.h"
#include "cGameWindow.h"
#include "cSelectWindow.h"
#include "cMessageWindow.h"
#include "cListWindow.h"
#include "cControlLayer.h"
#include "cAnimationManager.h"
#include "cEventManager.h"
#include "cDramaManager.h"

#include "utility/cAltaMatterPlus.h"
#include "utility/cDiscreteProbability.h"
#include "utility/cRandomGen.h"

#include "cGameScreenInterface.h"
#include "menuCommand.h"
#include "cVectorOfControlLayer.h"

#include "cDatabaseOfGame.h"
#include "cCutInManager.h"


#include <vector>
#include <list>
#include <set>
#include <tstring_ph.h>


#include "scriptprocesser/DungeonStoryScript.h"

using namespace std;

class cDungeon;
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
typedef boost::shared_ptr<cDungeon> pcDungeon;
typedef boost::weak_ptr<cDungeon> wpcDungeon;
#include <d3d9.h>
#include <d3dx9.h>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <d3d9.h>
#include <d3dx9.h>

//ダンジョン中保存されるデータ類
class cDungeonInstance
{
public:
	cDungeonInstance(void);
public:
	virtual ~cDungeonInstance(void);
	/*上レイヤ
	//画面情報
	cGameScreenInterface GameScreenInterface;
	*/
public:
	//------------------------------------------------------
	//ダンジョンデータおよびフロア
	//------------------------------------------------------

	//ランダムイニットジェネレーター
	unsigned long random_Seed_;
	cRandomGen RandomInitGen_;

	pcDungeon pDungeon_;

	int FloorLevel_;

public:

	void insertLuaEnv(pDungeonStoryScript_LuaEnvironment luaEnv);
	void deleteLuaEnv(pDungeonStoryScript_LuaEnvironment luaEnv);
	const vector<pDungeonStoryScript_LuaEnvironment> s_luaEnvVector();
private:
	vector<pDungeonStoryScript_LuaEnvironment> s_luaEnvVector_;//ルアシステム
//***************************************************
//-----------------------------------------------
//スクリプト関係
//-----------------------------------------------
//***************************************************
public:
	map<std::basic_string<LuaString>,pcCharacter> charactarStorage_;
	map<std::basic_string<LuaString>,pcDroping> dropingStorage_;
	map<std::basic_string<LuaString>,pcLandform> landformStorage_;

public:


	//
	int replaymode;


	pcCharacter pPlayerChara_;
	vector<pcCharacter> pFriendCharas_;


	//メニュー
	//cVectorOfControlLayer menuControlLayerV_;

	vector<int> FreeFlags_;
	map<tstring,int> localFlags_;
	map<tstring,int> privateFlags_;

public:
	int GameOverFlag_;
	int GameClearFlag_;
	int SumTurnCount_;
	int SumEnemyDefeatNum_;

	//時間計測
	int time_SumFrameCount_;
	double time_Sumtime_;
};

#define GAME_PHASE_INPUT 0
#define GAME_PHASE_ENEMY 1

#define GAME_TURN_SPEED_SINGLE 0
#define GAME_TURN_SPEED_DOUBLE 1
#define GAME_TURN_SPEED_TRIPLE 2
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cDungeonInstance> pcDungeonInstance;

//フロア中保存されるデータ類
class cFloorInstance
{
public:
	cFloorInstance(void);
public:
	virtual ~cFloorInstance(void);

	//------------------------------------------------------
	//ダンジョンシステム
	//------------------------------------------------------

	int s_cannotInputWaitCount_;

	cAltaMatterPlus s_AltaInputprocess_;
	cAltaMatterPlus s_AltaofminimapViewprocess_;
	cAltaMatterPlus s_aspectTurnprocess_;
	cAltaMatterPlus s_aspectDiagonprocess_;
	bool s_minimapHiding_;
	int s_aspectTurndiagonsupport_;
	int s_skipValiable_;
public:
	int s_phase_;
	int s_turn_speed_;
	int s_DashButtonTap_;
	int s_DashButtonCancelEffect_;
	int s_AttackDelay_;

	int s_shootAndShortCutswitch_;
	int s_ShortCutdiagonsupport_;
	int s_ShortCutSelectSwitch_;
	int s_ShortCutSelectOffCount_;

	int s_AttackRapidSwitch_;
	int s_autoTurnFlag_;
public:
	int Processmode_;
	int TurnCount_;

	//時間計測
	int time_FrameCount_;
	double time_Starttime_;

	int aspectTurnprocessswitch_;

	SharedValiable_likeInt<int> floorlightswitchvisible_;//明かり状態
	int floorlightswitch_;//明かり状態

	int floornothrowswitch_;//明かり状態
	int floortimestopswitch_;//時間停止状態
	int floortimeskipswitch_;//時間とばし状態

	SharedValiable_likeInt<int> floormoveswitch_;//フロア更新スイッチ


	int floorstealswitch_;//泥棒スイッチ
	double floorshopprofit_;//ショップ出納増減
	int floorshopownermoney_;//ショップ店長の所持金
	map<pcDroping,int> floorshopitemvalueBuy_;//ショップアイテムの総価値
	map<pcDroping,int> floorshopitemvalueSell_;//ショップアイテムの損失
	vector<pcDroping> floorshopitemlist_;
	int floorshoplastroom_;

	int floorNowParty_;

	

	//メッセージ
	cMessageWindow MessageW_;

	//マップ
	cMap Map_;

	//カットイン
	cCutInManager CutInM_;

	//イヴェントマネージャー
	cEventManager EventManager_;

public:

	//ミニマップ用カウンタ
	int count_minimap_;

public:
	bool isInputWaitingflag_;
public:

	bool cannotinput_;


public:
	//暗闇追加リスト
	set<pcLandform> DarkLandList_;
	//危険追加リスト
	set<pcLandform> DangerLandList_;


//***************************************************
//-----------------------------------------------
//ダッシュ関係
//-----------------------------------------------
//***************************************************
public:
	int DashSwitch_;
	int DashEnemyCount_;
	int DashAspect_;
	int DashStopDelay_;

public:
	int forDoubleInput_X_;
	int forDoubleInput_Y_;
	int forDoubleInput_migi_;
	int forDoubleInput_hidari_;
	int forDoubleInput_ue_;
	int forDoubleInput_shita_;
public:
	//int DashCondition_HP;
	cCoordinate DashTGT_;
public:
	int dashflag_;
//-------------------------------------
//描画関係
//-------------------------------------
	cAnimationManager AnimationManager_;

	cDramaManager DramaManager_;
//-------------------------------------
//ターンエンド
//-------------------------------------
public:
	int turnEnd_flag_;
};


#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cFloorInstance> pcFloorInstance;





