#pragma once

#include <fileManage_ph.h>

#include <LuaUtility/LuaStringUtility.h>
#include <StyleString.h>
#include <boost/shared_ptr.hpp>
#include <boost/ref.hpp>
#include <luabind/luabind.hpp>
#include "../GameIdiom.h"
#include "..\utility\valiableField\cValiableField.h"
#include "..\gameMainSystem\cPlayerInput.h"

class cColor;

class cLandform;
typedef boost::shared_ptr<cLandform> pcLandform;

class cCharacter;
typedef boost::shared_ptr<cCharacter> pcCharacter;

class cDroping;
typedef boost::shared_ptr<cDroping> pcDroping;

class cEvent;
typedef boost::shared_ptr<cEvent> pcEvent;
class cEventBackground;
typedef boost::shared_ptr<cEventBackground> pcEventBackground;

#include <vector>

#include <set>

#include <map>

typedef D3DXVECTOR4 TextureRangeRect;

class cCoordinate;

namespace forLua{

//生成
pcCharacter MakeCharacter(int ID,int CLASS,int Forse,pcLandform land);
pcDroping MakeDroping(int ID, pcLandform land);
pcDroping MakeDropingAndPick(int ID, pcCharacter pchara);
pcCharacter getSumpleCharacter(int ID);
pcCharacter changePlayerCharacter(int ID);
bool swapPlayerCharacter(pcCharacter pchara);
//item
#define DROPINGFUNCTION1_DECLARATION_ForLua(FunctionNamePrefix,SystemFunctionName) \
int FunctionNamePrefix(pcDroping pdrop);

#define DROPINGFUNCTION2_DECLARATION_ForLua(FunctionNamePrefix,SystemFunctionName,valiableType) \
int FunctionNamePrefix(pcDroping pdrop, valiableType value1);

DROPINGFUNCTION1_DECLARATION_ForLua(DropingDiscriminateComplete, 識別)
DROPINGFUNCTION1_DECLARATION_ForLua(DropingDiscriminateState, 状態のみ識別)
DROPINGFUNCTION1_DECLARATION_ForLua(DropingDiscriminateQuality, 修正のみ識別)
DROPINGFUNCTION1_DECLARATION_ForLua(DropingDiscriminateName, 名称のみ識別)
DROPINGFUNCTION1_DECLARATION_ForLua(DropingStateMakeGood, アイテム祝福)
DROPINGFUNCTION1_DECLARATION_ForLua(DropingStateMakeBad, アイテム呪い)
DROPINGFUNCTION1_DECLARATION_ForLua(DropingStateForsedMakeGood, アイテム状態強制設定祝福)
DROPINGFUNCTION1_DECLARATION_ForLua(DropingStateForsedMakeNormal, アイテム状態強制設定通常)
DROPINGFUNCTION1_DECLARATION_ForLua(DropingStateForsedMakeBad, アイテム状態強制設定呪い)

DROPINGFUNCTION1_DECLARATION_ForLua(DropingQualityMakeGood, アイテム劣化)
DROPINGFUNCTION1_DECLARATION_ForLua(DropingQualityMakeBad, アイテム強化)
DROPINGFUNCTION2_DECLARATION_ForLua(DropingQualityForsedMakeValue, アイテム品質強制設定, int)

int DropingBreak(pcDroping pdrop);


#define DROPINGLUABINDDEF(FunctionNamePrefix) \
	luabind::def(#FunctionNamePrefix, &FunctionNamePrefix)
		

//character

#define CHARACTERFUNCTION0_DECLARATION_ForLua(FunctionNamePrefix,SystemFunctionName) \
int FunctionNamePrefix(pcCharacter pchara);

#define CHARACTERFUNCTION1_DECLARATION_ForLua(FunctionNamePrefix,SystemFunctionName) \
int FunctionNamePrefix(pcCharacter pchara);

#define CHARACTERFUNCTION2_DECLARATION_ForLua(FunctionNamePrefix,SystemFunctionName,valiableType) \
int FunctionNamePrefix(pcCharacter pchara, valiableType value1);

#define CHARACTERFUNCTION3_DECLARATION_ForLua(FunctionNamePrefix,SystemFunctionName,valiableType1,valiableType2) \
int FunctionNamePrefix(pcCharacter pchara, valiableType1 value1, valiableType2 value2);

CHARACTERFUNCTION2_DECLARATION_ForLua(CharacterHPRecovery, 回復要請, int)
CHARACTERFUNCTION2_DECLARATION_ForLua(CharacterSetHP, HP設定要請, int)
CHARACTERFUNCTION2_DECLARATION_ForLua(CharacterMaxHPExtend, 最大HP拡張要請, int)
CHARACTERFUNCTION2_DECLARATION_ForLua(CharacterMaxHPReduction, 最大HP縮小要請, int)

CHARACTERFUNCTION2_DECLARATION_ForLua(CharacterStomachRecovery, 満腹度回復要請, double)
CHARACTERFUNCTION2_DECLARATION_ForLua(CharacterStomachHungry, 満腹度減少要請, double)
CHARACTERFUNCTION2_DECLARATION_ForLua(CharacterSetStomach, 満腹度設定要請, double)
CHARACTERFUNCTION2_DECLARATION_ForLua(CharacterMaxStomachExtend, 最大満腹度拡張要請, int)
CHARACTERFUNCTION2_DECLARATION_ForLua(CharacterMaxStomachReduction, 最大満腹度縮小要請, int)

CHARACTERFUNCTION2_DECLARATION_ForLua(CharacterSpeedUp, 速度増加要請, int)
CHARACTERFUNCTION2_DECLARATION_ForLua(CharacterSpeedDown, 速度減少要請, int)

CHARACTERFUNCTION2_DECLARATION_ForLua(CharacterSleep, 眠り要請, int)
CHARACTERFUNCTION2_DECLARATION_ForLua(CharacterDeepSleep, バクスイ要請, int)
CHARACTERFUNCTION2_DECLARATION_ForLua(CharacterSurprise, びっくり要請, int)
CHARACTERFUNCTION2_DECLARATION_ForLua(CharacterLockedIn, 金縛り要請, int)

CHARACTERFUNCTION2_DECLARATION_ForLua(CharacterSeal, 封印要請, int)
CHARACTERFUNCTION2_DECLARATION_ForLua(CharacterSwingAir, 空振り要請, int)
CHARACTERFUNCTION2_DECLARATION_ForLua(CharacterPoor, 貧乏要請, int)
CHARACTERFUNCTION2_DECLARATION_ForLua(CharacterCowardice, 臆病要請, int)
CHARACTERFUNCTION2_DECLARATION_ForLua(CharacterJealousy, 嫉妬要請, int)
CHARACTERFUNCTION2_DECLARATION_ForLua(CharacterFrenzy, 狂乱要請, int)

CHARACTERFUNCTION3_DECLARATION_ForLua(CharacterAttackDown, 脱力要請, int, int)
CHARACTERFUNCTION3_DECLARATION_ForLua(CharacterAttackUp, 元気要請, int, int)

CHARACTERFUNCTION3_DECLARATION_ForLua(CharacterDefenceDown, 軟弱要請, int, int)
CHARACTERFUNCTION3_DECLARATION_ForLua(CharacterDefenceUp, 頑強要請, int, int)

CHARACTERFUNCTION2_DECLARATION_ForLua(CharacterDrunk, 泥酔要請, int)
CHARACTERFUNCTION2_DECLARATION_ForLua(CharacterInIce, 氷付け要請, int)
CHARACTERFUNCTION2_DECLARATION_ForLua(CharacterMatchless, 無敵要請, int)
CHARACTERFUNCTION2_DECLARATION_ForLua(CharacterNightBlind, 鳥目要請, int)

CHARACTERFUNCTION3_DECLARATION_ForLua(CharacterDeathTouch, 死の誘い要請, pcCharacter, int)
CHARACTERFUNCTION3_DECLARATION_ForLua(CharacterSubstitution, 身代わり要請, pcCharacter, int)
CHARACTERFUNCTION2_DECLARATION_ForLua(CharacterMimicry, 擬態要請, int)

CHARACTERFUNCTION1_DECLARATION_ForLua(CharacterCureMental, 精神異常治療要請)
CHARACTERFUNCTION1_DECLARATION_ForLua(CharacterCurePhysical, 身体異常治療要請)
CHARACTERFUNCTION1_DECLARATION_ForLua(CharacterCureMagic, 呪術異常治療要請)
CHARACTERFUNCTION1_DECLARATION_ForLua(CharacterCureSpeed, 速度異常治療要請)

CHARACTERFUNCTION1_DECLARATION_ForLua(CharacterIsMentalAberration, 精神異常状態)
CHARACTERFUNCTION1_DECLARATION_ForLua(CharacterIsPhysicalAberration, 身体異常状態)
CHARACTERFUNCTION1_DECLARATION_ForLua(CharacterIsMagicAberration, 呪術異常状態)
CHARACTERFUNCTION1_DECLARATION_ForLua(CharacterIsSpeedAberration, 速度異常状態)

CHARACTERFUNCTION2_DECLARATION_ForLua(CharacterGoPast, やりすごし要請, int)

CHARACTERFUNCTION1_DECLARATION_ForLua(CharacterEyeLotion, 目薬要請)

CHARACTERFUNCTION2_DECLARATION_ForLua(CharacterForsedDamage, 強制ダメージ要請, int)

CHARACTERFUNCTION1_DECLARATION_ForLua(CharacterInstantDeath, 即死要請)
CHARACTERFUNCTION1_DECLARATION_ForLua(CharacterForseGoAway, 強制退場要請)

CHARACTERFUNCTION2_DECLARATION_ForLua(CharacterGetExperience, 経験値獲得, int)
CHARACTERFUNCTION2_DECLARATION_ForLua(CharacterLevelUp, レベルアップ, int)
CHARACTERFUNCTION2_DECLARATION_ForLua(CharacterLevelDown, レベルダウン, int)

CHARACTERFUNCTION0_DECLARATION_ForLua(CharacterForsedFlinch, 強制ひるみ要請)
CHARACTERFUNCTION0_DECLARATION_ForLua(CharacterForsedTurnEnd, 強制ターンエンド要請)

CHARACTERFUNCTION0_DECLARATION_ForLua(CharacterNormalAttack, 直接攻撃要請)
CHARACTERFUNCTION0_DECLARATION_ForLua(CharacterEquipmentSwap, 装備回し要請)
CHARACTERFUNCTION0_DECLARATION_ForLua(CharacterForceEquipmentSwap, 強制装備回し)

int CharacterDeepDoze(pcCharacter pchara);
int CharacterLightDoze(pcCharacter pchara);

int CharacterTurn(pcCharacter pchara, int aspect);
int CharacterTurnNow(pcCharacter pchara, int aspect);

int CharacterMove(pcCharacter pchara, int aspect);
bool CharacterCanMove(pcCharacter pchara, int aspect);

void CharacterVisualRefresh(pcCharacter pchara);

int CharacterWarp(pcCharacter pchara, pcLandform pland);
bool CharacterCanWarp(pcCharacter pchara, pcLandform pland);

int CharacterBlow(pcCharacter subjectchara, pcCharacter pchara, int aspect,int distance,int collisionpower);
int CharacterPull(pcCharacter subjectchara, pcCharacter pchara, int aspect,int distance);
int CharacterReplace(pcCharacter subjectchara, pcCharacter pchara);
int CharacterThrow(pcCharacter subjectchara, pcCharacter pchara, pcLandform toland, int collisionpower, int dropcollition, double speed);
	
int CharacterDisguise(pcCharacter pchara, int charaID);

pcCharacter PlayerCharacter();
void turnEndPlayerCharacter();

int itemCount(vector<pcDroping> items);

#define CHARACTERLUABINDDEF(FunctionNamePrefix) \
	luabind::def(#FunctionNamePrefix, &FunctionNamePrefix)
		
//landform
pcLandform LandformRandom_RoomRandom_NotShop_Safe();
pcLandform LandformRandom_RoomRandom_Safe();
pcLandform LandformRandom_RoomRandom_OutOfRoom_Safe(pcLandform pland);
pcLandform LandformRandom_RoomRandom_OutOfSight_Safe(pcLandform pland);
pcLandform LandformRandom_Random();
pcLandform LandformPlace(int x, int y);
pcLandform LandformEdge();
pcLandform LandformCharacterPosition(pcCharacter pchara);
pcLandform LandformNearby(pcLandform outLand, int aspect, int distance);

bool isSafeSetCharacterLandform(pcLandform target);
pcLandform getSafeSetCharacterLandform(pcLandform target,pcCharacter pchara);

#define LANDFORMLUABINDDEF(FunctionNamePrefix) \
	luabind::def(#FunctionNamePrefix, &FunctionNamePrefix)


//イベント通知
bool eventNotification(const pLuaString eventName, const pLuaString notificatedObject, const pLuaString argument);

//オブジェクト登録
bool addCharacterStorage(const pLuaString name, pcCharacter chara);
pcCharacter getCharacterStorage(const pLuaString characterName);
bool addDropingStorage(const pLuaString name, pcDroping drop);
pcDroping getDropingStorage(const pLuaString dropingName);
bool addLandformStorage(const pLuaString name, pcLandform land);
pcLandform getLandformStorage(const pLuaString landformName);


//メッセージ

//virtual int メッセージ(StyleString Message);
int message(StyleString& Message);
//virtual int メッセージ決定待ち();
int messageWait();
int messageWaitAnyKey();
pcScriptRLayer storyLayerRoot();
bool utility_storyMessage(const tstring& name, std::map<tstring, StyleString > & valiable);
bool utility_localizedMessage(const tstring& name, std::map<tstring, StyleString > & valiable);
StyleString localizeString(const tstring& name, std::map<tstring, StyleString > & valiable);
StyleString localizeString(const tstring& name);
bool isExistStoryMessage(const tstring& name);
bool isExistLocalizedMessage(const tstring& name);
void storyEventMessageLoop(const tstring& keyPrefix, std::map<tstring, StyleString> & StringValiable, luabind::object intervalFunction);
void storyEventMessageLoopRange(const tstring& keyPrefix, int firstIndex, int lastIndex, std::map<tstring, StyleString> & StringValiable, luabind::object intervalFunction);

//フロアをあがる
void goToNextFloor();

//カメラ移動
void cameraMove(double x, double y, double speed, double point1, double point2);
void cameraMoveParallel(double x, double y, double speed, double point1, double point2);
//エフェクト
void effect_Wait(const double placeX,const double placeY, const int waitFrame);
void effect_WaitPararellCritical(const double placeX,const double placeY, const int waitFrame);
void effect_Smoke1(const double placeX,const double placeY);
void effect_boss_focus(const double placeX,const double placeY);
void effect_boss_charge(const double placeX,const double placeY, cColor& imagecolor1,  cColor& imagecolor2);
void effect_boss_emission(const double placeX,const double placeY, cColor& imagecolor1,  cColor& imagecolor2);
void effect_boss_defeat(const double placeX,const double placeY);
void effect_powerUp(const double placeX, const double placeY, double color_Hue);
void effect_powerDown(const double placeX, const double placeY, double color_Hue);
void effect_lightning(const double placeX, const double placeY);
void effect_generalStamp(const double placeX, const double placeY, const tstring& fileName, TextureRangeRect& texRenge, cColor& burnColor, const double size, const int duration, const int delay);
void effect_generalFog(const double placeX, const double placeY, const tstring& fileName, TextureRangeRect& texRenge, cColor& burnColor, const double size, const double generateRadius, const double generateRatio, const int duration, const int delay);
void effect_generalShoot(const double placeX, const double placeY, const tstring& fileName, TextureRangeRect& texRenge, cColor& burnColor, const double size, const int duration, const int delay);
double charaPos(const double effectPos);

//移動アニメ
bool moveWithAnimeToLand(pcCharacter pchara, pcLandform toLand, bool neverTurn, double speed, bool omission);
bool moveWithAnime(pcCharacter pchara, int aspect, int distance, bool neverTurn, double speed, bool omission);
bool jumpWithAnimeToLand(pcCharacter pchara, pcLandform toLand, double height, bool neverTurn, double speed, bool omission);
bool jumpWithAnime(pcCharacter pchara, int aspect, int distance, double height, bool neverTurn, double speed, bool omission);
//フェードインアウト
void fadeBlack(int duringFrame);
void fadeWhite(int duringFrame);
void fadeWithColor(cColor color, int duringFrame);
void fadeOut(int duringFrame);
//背景
pcEventBackground EventBackground(const int priority, const tstring& fileName, int placeX, int placeY, int power);
//ミニマップをけす
void setHidingMiniMap(bool hidingMiniMap);
void setHiddenInterface(bool hidden);

//検索
pcCharacter searchOneCharacterOnFloor(int ID);
vector<pcCharacter> searchCharactersOnFloor(int ID);

//ラムダ関数を実行
void stackFunction(luabind::object function);
void stackFunction(luabind::object function, luabind::object val1);
void stackFunction(luabind::object function, luabind::object val1, luabind::object val2);
void stackFunction(luabind::object function, luabind::object val1, luabind::object val2, luabind::object val3);
void stackFunction(luabind::object function, luabind::object val1, luabind::object val2, luabind::object val3, luabind::object val4);
void stackFunction(luabind::object function, luabind::object val1, luabind::object val2, luabind::object val3, luabind::object val4,
	luabind::object val5);
void stackFunction(luabind::object function, luabind::object val1, luabind::object val2, luabind::object val3, luabind::object val4,
	luabind::object val5, luabind::object val6);
void stackFunction(luabind::object function, luabind::object val1, luabind::object val2, luabind::object val3, luabind::object val4,
	luabind::object val5, luabind::object val6, luabind::object val7);
void stackFunction(luabind::object function, luabind::object val1, luabind::object val2, luabind::object val3, luabind::object val4,
	luabind::object val5, luabind::object val6, luabind::object val7, luabind::object val8);

//デバッグ
void outputDebugString(const char* message);
void outputDebugValue(const double value);

//グローバル変数
int buttonIndex(int index);
int totalTurnCount();
int floorLevel();
pcScriptRLayer storyDataLayer();
pcScriptRLayer localizeDataLayer();

bool isNotGoodTerminateInBase();

cPlayerInput* controllerInput();

double random_independent();//独立ランダム

std::vector<int>& savable_freeFlags();
std::map<tstring,int>& savable_localFlags();
const std::map<tstring,int>& const_privateFlags();
const std::map<tstring,int>& const_globalFlags();
std::map<tstring,int>& savable_globalFlags();

bool isDungeonCleared(tstring& dungeonID);
void gotoTempDungeon(tstring& dungeonID);

const bool storyEventEnable();//ストーリー

const int getMapValue(map<tstring,int>& mapObject, tstring& key, int defaultValue);
void setMapValue(map<tstring,int>& mapObject, tstring& key, int setValue);

//ダンジョン出現
void appearDungeon(tstring& dungeonID);
bool isAppearedDungeon(tstring& dungeonID);
//
//アンリリースモード
bool isUnRelease();
//
void setClearResultSkipFlag(bool flag);

bool isStockingMassage();
bool clearMassage();

pcLandform alternativeLand(pcLandform main, int orderNum);
bool canDisCoverCharacter(pcCharacter frompchara, pcCharacter topchara);
bool canDisCoverDroping(pcCharacter frompchara, pcDroping pdrop);
bool canDisCoverLandform(pcCharacter frompchara, pcLandform toLand);
bool canLandformSeeLandform(pcLandform fromLand, pcLandform toLand);

bool makeListDisCoverCharacter(pcCharacter frompchara, vector<pcCharacter>& outputlist);
bool makeListDisCoverDroping(pcCharacter frompchara, vector<pcDroping>& outputlist);
bool makeListDisCoverExit(pcCharacter frompchara, vector<pcLandform>& outputlist);

bool isWallSeparated(pcLandform start, pcLandform end);
pcLandform landformInFrontOfCharacter(pcCharacter pchara, int distance);
pcLandform landformAroundOfCharacter(pcCharacter pchara, int plusaspect, int distance);

bool culcuLandformWallReflection(pcLandform &outLand, int &outaspect, pcLandform comeLand, int comeaspect);
bool culcuLandformWallBending(pcLandform &outLand, int &outaspect, pcLandform comeLand, int comeaspect);

pcLandform landformSearchObstacleWithRange(pcLandform start, int aspect, int range, bool stopWall, bool stopChara, bool stopDrop, bool stopNonthrough);
pcLandform landformSearchObstacleGetPathWithRange(vector<pcLandform>& landpass ,pcLandform start, int aspect, int range, bool stopWall, bool stopChara, bool stopDrop, bool stopNonthrough);

pcLandform landformSearchObstacle(pcLandform start, int aspect, bool stopWall, bool stopChara, bool stopDrop);
bool isCharacterCanNeighberAttack(pcCharacter pchara, int aspect, int distance, int diagonthrough, int wallthrough);

pcLandform landformSearchDropDroping(pcLandform start, pcDroping pdrop);
pcLandform landformSearchDropCharacter(pcLandform start, pcCharacter pchara);

bool isEnemyCharacter(pcCharacter pcharaA, pcCharacter pcharaB);

void mappingFromLandform(pcLandform pland);
void mappingAllLandform();
void disMappingFromLandform();

bool isFloorLight();
void addFloorLight();
void eraseFloorLight();

bool isFloorItemCantThrow();
void addFloorItemCantThrow(int turn);
void eraseFloorItemCantThrow();

bool isFloorTimeStop();
void addFloorTimeStop(int turn);
void eraseFloorTimeStop();

bool isFloorTimeSkip();
void addFloorTimeSkip(int turn);
void eraseFloorTimeSkip();

void isOctasDirection(cCoordinate& from, cCoordinate& to);

void mappingDroping(pcDroping pdrop);

void mapTransformArrangement();
void mapTransformWallDig(pcLandform pland);
void mapTransformWaterMake(pcLandform pland);
void mapTransformWaterCover(pcLandform pland);
void mapTransformRoomBreak(pcLandform pland, int range);
void mapTransformMakeBigRoom();

void mapTransformForseLandChange(pcLandform pland, int mapKind);//安全保障なし

const vector<pcCharacter> allCharacterList();
const vector<pcDroping> allDropingList();

void showSelectDungeonWindow(const vector<tstring>& Dungeons);

//アイテム出現
pcDroping spawnFloorNativeItem(pcLandform pland);
pcDroping spawnFloorNativeTrap(pcLandform pland);

//特殊アイテム生成
int getIDOfFloorNativeItem(double rarity,
							double Equipment,
							double Bullet,
							double Spell,
							double Food,
							double Drink,
							double Book,
							double Box,
							double MoneyBag,
							double Other);

//敵出現
pcCharacter spawnFloorNativeEnemy(pcLandform pland, int forse);

//GameIdeom
bool turnCharacterForCharacter(pcCharacter me, pcCharacter you);

//FindUtility
bool isValidDirectAttackToEnemy(pcCharacter me, pcCharacter enemy);
bool isValidDirectAttackToFriend(pcCharacter me, pcCharacter chara);
bool isValidShootToCharactor(pcCharacter me, pcCharacter enemy, const int range);
int distanceOfBetweenCharacters(pcCharacter me, pcCharacter chara);
int aspectOfBetweenCharacters(pcCharacter me, pcCharacter chara);
int marginFromShootLineOfBetweenCharacters(pcCharacter me, pcCharacter chara);


//財布のお金
int walletMoney();
//厄ポイント
int yakuPoint();
//プレイヤーレベル
int globalPlayerLavel();
int globalPlayerExp();

//AIショートカット
bool AIshortCut_NomoveAttack(pcCharacter boss, cValiableField& valiable);

int distanceBetweenCharacter(pcCharacter me, pcCharacter you);

void pcGameWindow_showWindow(pcGameWindow& show);
void menuDismissAllWindows();
void menuDismissWindowLayer();
void menuRerenderAllWindows();
int menuDefaultPositionLeft(int level);
int menuDefaultPositionTop(int level);
int menuDefaultPositionRight(int level);
int menuDefaultPositionBottom(int level);
bool stackDramaThread(luabind::object thread);

int eraseDanmaku(pcCharacter boss);
int eraseFloorDropings();
int eraseEnemys();

void gotoSceneTitle();
/*
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
*/

void PlayBGM(const tstring& intro, const tstring& main);
void PlayNormalBGM();
void StopBGM();

pcCharacter bulletSummon(pcLandform land,
		int HP,
		int Forse,
		int speed,
		int type,
		int color,
		int aspect);

pcEvent lazerSummon(
		pcLandform land,
		int aspect,
		int length,
		int power,
		int Forse,
		cColor color,
		int readyTurn,
		int attackTurn,
		bool isAttackFriend,
		bool isStopAtWall);

void startBossSpell(pcCharacter pchara);

void attackHappen(pcCharacter pchara, pcLandform pland, double power, int constFlag, vector<int> attackAttributes, bool surelyHitFlag);
//std
/*
#include <vector>
namespace std_vector
{

	template<class T> T& 
	at(::std::vector<T>& vec, unsigned int pos){
		return vec.at(pos);
	};
	template<class T> T&
	back(::std::vector<T>& vec){
		return vec.back();
	};
	template<class T> ::std::_Vector_iterator<T,allocator<T>>
	begin(::std::vector<T>& vec){
		return vec.begin();
	};
	template<class T> ::std::_Vector_iterator<T,allocator<T>>
	end(::std::vector<T>& vec){
		return vec.end();
	};
	template<class T> ::std::_Vector_iterator<T,allocator<T>>
	erase1(::std::vector<T>& vec, ::std::_Vector_iterator<T,allocator<T>> pos){
		return vec.erase(pos);
	};
	template<class T> ::std::_Vector_iterator<T,allocator<T>>
	erase2(::std::vector<T>& vec, ::std::_Vector_iterator<T,allocator<T>> start, ::std::_Vector_iterator<T,allocator<T>> end){
		return vec.erase(start, end);
	};
	template<class T> T&
	front(::std::vector<T>& vec){
		return vec.front();
	};
	template<class T> ::std::_Vector_iterator<T,allocator<T>>
	insert1(::std::vector<T>& vec, ::std::_Vector_iterator<T,allocator<T>> pos, const T& val){
		return vec.insert(pos, val);
	};
	template<class T> void
	insert2(::std::vector<T>& vec, ::std::_Vector_iterator<T,allocator<T>> pos, unsigned int num, const T& val){
		return vec.insert(pos, num, val);
	};
	template<class T> void
	insert3(::std::vector<T>& vec, ::std::_Vector_iterator<T,allocator<T>> pos, ::std::_Vector_iterator<T,allocator<T>> start, ::std::_Vector_iterator<T,allocator<T>> end){
		return vec.insert(pos, start, end);
	};
	
	//template<class T> ::std::_Vector_iterator<T,allocator<T>>
	//rbegin(::std::vector<T>& vec){
	//	return vec.rbegin();
	//};
	//template<class T> ::std::_Vector_iterator<T,allocator<T>>
	//rend(::std::vector<T>& vec){
	//	return vec.rend();
	//};
	
	template<class T> void
	resize1(::std::vector<T>& vec, ::std::_Vector_iterator<T,allocator<T>> pos, unsigned int size){
		return vec.resize(size);
	};
	template<class T> void
	resize2(::std::vector<T>& vec, ::std::_Vector_iterator<T,allocator<T>> pos, unsigned int size, const T& val){
		return vec.resize(size, val);
	};

	//iterator
	template<class T> ::std::_Vector_iterator<T,allocator<T>>&
	iterator_increment(::std::_Vector_iterator<T,allocator<T>>& itr)
	{
		return ++itr;
	};
	template<class T> ::std::_Vector_iterator<T,allocator<T>>&
	iterator_decrement(::std::_Vector_iterator<T,allocator<T>>& itr)
	{
		return --itr;
	};
	template<class T> T&
	iterator_dereference(::std::_Vector_iterator<T,allocator<T>>& itr)
	{
		return *itr;
	};
};
*/

template<class T>
class std_vector
{
public:
	typedef typename ::std::vector<T> typename LuaVector;
	typedef typename ::std::vector<T>::iterator typename LuaVector_iterator;

	static void assign(LuaVector& vec, int count, const T val){
		vec.assign(count, val);
	};
	static T at(LuaVector& vec, unsigned int pos){
		return vec.at(pos);
	};
	static void setAt(LuaVector& vec, unsigned int pos, const T value){
		vec.at(pos) = value;
	};
	static T back(LuaVector& vec){
		return vec.back();
	};
	static void setBack(LuaVector& vec, const T value){
		vec.back() = value;
	};
	static LuaVector_iterator begin(LuaVector& vec){
		return vec.begin();
	};
	static LuaVector_iterator end(LuaVector& vec){
		return vec.end();
	};
	static LuaVector_iterator erase1(LuaVector& vec, LuaVector_iterator pos){
		return vec.erase(pos);
	};
	static LuaVector_iterator erase2(LuaVector& vec, LuaVector_iterator start, LuaVector_iterator end){
		return vec.erase(start, end);
	};
	static T front(LuaVector& vec){
		return vec.front();
	};
	static void setFront(LuaVector& vec, const T value){
		vec.front() = value;
	};
	static LuaVector_iterator insert1(LuaVector& vec, LuaVector_iterator pos, const T val){
		return vec.insert(pos, val);
	};
	static LuaVector_iterator insert2(LuaVector& vec, LuaVector_iterator pos, unsigned int num, const T val){
		return vec.insert(pos, num, val);
	};
	static void insert3(LuaVector& vec, LuaVector_iterator pos, LuaVector_iterator start, LuaVector_iterator end){
		vec.insert(pos, start, end);
	};
	static void resize1(LuaVector& vec, unsigned int size){
		vec.resize(size);
	};
	static void resize2(LuaVector& vec, unsigned int size, const T val){
		vec.resize(size, val);
	};
	static bool empty(LuaVector& vec){
		return vec.empty();
	};
	static void clear(LuaVector& vec){
		vec.clear();
	};
	static void pop_back(LuaVector& vec){
		vec.pop_back();
	};
	static void push_back(LuaVector& vec, const T val){
		vec.push_back(val);
	};
	static int size(LuaVector& vec){
		return vec.size();
	};

	//iterator
	static LuaVector_iterator& iterator_increment(LuaVector_iterator itr)
	{
		return ++itr;
	};
	static LuaVector_iterator& iterator_decrement(LuaVector_iterator itr)
	{
		return --itr;
	};
	static T iterator_dereference(LuaVector_iterator itr)
	{
		return *itr;
	};
	static void iterator_setDereference(LuaVector_iterator& itr, const T value)
	{
		*itr = value;
	};
};

template<class T, class U>
class std_map
{
public:
	typedef typename ::std::map<T,U> typename LuaMap;

	typedef typename ::std::map<T,U>::iterator typename LuaMap_iterator;
	
	typedef typename ::std::pair<T,U> typename LuaMap_pair;
	typedef typename ::std::pair<LuaMap_iterator,bool> typename pairOf_LuaMap_pair_bool;


	static LuaMap_iterator begin(LuaMap& m){
		return m.begin();
	};
	static void clear(LuaMap& m){
		return m.clear();
	};
	static int count(LuaMap& m,const T& key){
		return m.count(key);
	};
	static bool empty(LuaMap& m){
		return m.empty();
	};
	static U value(LuaMap& m,const T& key){
		return m[key];
	};
	static void setValue(LuaMap& m,const T& key, const U& value){
		m[key] = value;
	};
	static LuaMap_iterator end(LuaMap& m){
		return m.end();
	};
	static void erase1(LuaMap& m, LuaMap_iterator& itr){
		m.erase(itr);
	};
	static void erase2(LuaMap& m, LuaMap_iterator& itr_first, LuaMap_iterator& itr_last){
		m.erase(itr_first, itr_last);
	};
	static int eraseByKey(LuaMap& m, const T &key ){
		return m.erase(key);
	};
	static LuaMap_iterator find(LuaMap& m,const T& key){
		return m.find(key);
	};
	
	static pairOf_LuaMap_pair_bool insert1(LuaMap& m, LuaMap_pair& pair){
		return m.insert(pair);
	};
	
	static void insert2(LuaMap& m, LuaMap_iterator& itr_start, LuaMap_iterator& itr_end ) {
		return m.insert(itr_start, itr_end);
	};
	
	static LuaMap_iterator lower_bound(LuaMap& m,const T& key){
		return m.lower_bound(key);
	};
	static int size(LuaMap& m){
		return m.size();
	};
	static LuaMap_iterator upper_bound(LuaMap& m,const T& key){
		return m.upper_bound(key);
	};
	/*
	begin() 	マップの先頭を指すイテレータを返す。
clear() 	全ての要素を削除する
count() 	キー値が一致する要素の数を返す
empty() 	マップが空なら真
end() 	マップの末尾を指すイテレータを返す
equal_range() 	特定のキーと一致する先頭・末尾要素を指すイテレータを返す
erase() 	要素を削除する
find() 	要素を検索する
get_allocator() 	マップのアロケータを返す
insert() 	要素を挿入する
key_comp() 	キーを比較する関数を返す
lower_bound() 	キーが特定の値以上である最初の要素を指すイテレータを返す
max_size() 	要素の最大数を返す
rbegin() 	マップの末尾を指すリバースイテレータを返す
rend() 	マップの先頭を指すリバースイテレータを返す
size() 	マップ中の要素数を返す
swap() 	二つのマップを入れ替える
upper_bound() 	キーが特定の値を越える最初の要素を指すイテレータを返す
value_comp() 	値を比較する関数を返す
*/
	
/*
	template<class T, class U> typename ::std::map<T,U>::iterator
	begin(::std::map<T,U>& m){
		return m.begin();
	};
	
	template<class T> ::std::_Vector_iterator<T,allocator<T>>
	end(::std::vector<T>& vec){
		return vec.end();
	};
	template<class T> ::std::_Vector_iterator<T,allocator<T>>
	erase1(::std::vector<T>& vec, ::std::_Vector_iterator<T,allocator<T>> pos){
		return vec.erase(pos);
	};
	template<class T> ::std::_Vector_iterator<T,allocator<T>>
	erase2(::std::vector<T>& vec, ::std::_Vector_iterator<T,allocator<T>> start, ::std::_Vector_iterator<T,allocator<T>> end){
		return vec.erase(start, end);
	};
	template<class T> T&
	front(::std::vector<T>& vec){
		return vec.front();
	};
	template<class T> ::std::_Vector_iterator<T,allocator<T>>
	insert1(::std::vector<T>& vec, ::std::_Vector_iterator<T,allocator<T>> pos, const T& val){
		return vec.insert(pos, val);
	};
	template<class T> void
	insert2(::std::vector<T>& vec, ::std::_Vector_iterator<T,allocator<T>> pos, unsigned int num, const T& val){
		return vec.insert(pos, num, val);
	};
	template<class T> void
	insert3(::std::vector<T>& vec, ::std::_Vector_iterator<T,allocator<T>> pos, ::std::_Vector_iterator<T,allocator<T>> start, ::std::_Vector_iterator<T,allocator<T>> end){
		return vec.insert(pos, start, end);
	};
	template<class T> void
	resize1(::std::vector<T>& vec, ::std::_Vector_iterator<T,allocator<T>> pos, unsigned int size){
		return vec.resize(size);
	};
	template<class T> void
	resize2(::std::vector<T>& vec, ::std::_Vector_iterator<T,allocator<T>> pos, unsigned int size, const T& val){
		return vec.resize(size, val);
	};

	//iterator
	template<class T> ::std::_Vector_iterator<T,allocator<T>>&
	iterator_increment(::std::_Vector_iterator<T,allocator<T>>& itr)
	{
		return ++itr;
	};
	template<class T> ::std::_Vector_iterator<T,allocator<T>>&
	iterator_decrement(::std::_Vector_iterator<T,allocator<T>>& itr)
	{
		return --itr;
	};
	template<class T> T&
	iterator_dereference(::std::_Vector_iterator<T,allocator<T>>& itr)
	{
		return *itr;
	};

	*/

	//iterator
	static LuaMap_iterator&
	iterator_increment(LuaMap_iterator& itr)
	{
		return ++itr;
	};
	static LuaMap_iterator&
	iterator_decrement(LuaMap_iterator& itr)
	{
		return --itr;
	};
	static LuaMap_pair
	iterator_dereference(LuaMap_iterator& itr)
	{
		return *itr;
	};

};


template<class T>
class std_set
{
public:
	typedef typename ::std::set<T> typename LuaSet;
	typedef typename ::std::set<T>::iterator typename LuaSet_iterator;

	typedef typename ::std::pair<LuaSet_iterator,bool> typename pairOf_LuaSet_iterator_bool;


	static LuaSet_iterator begin(LuaSet& s){
		return s.begin();
	};
	static LuaSet_iterator end(LuaSet& s){
		return s.end();
	};
	static LuaSet_iterator erase1(LuaSet& s, LuaSet_iterator pos){
		return s.erase(pos);
	};
	static LuaSet_iterator erase2(LuaSet& s, LuaSet_iterator start, LuaSet_iterator end){
		return s.erase(start, end);
	};
	static int erase3(LuaSet& s, T& key){
		return s.erase(key);
	};
	static LuaSet_iterator insert1(LuaSet& s, LuaSet_iterator pos, const T& val){
		return s.insert(pos, val);
	};
	static void insert2(LuaSet& s, LuaSet_iterator start, LuaSet_iterator end){
		s.insert(start, end);
	};
	static pairOf_LuaSet_iterator_bool insert3(LuaSet& s, const T& val){
		return s.insert(val);
	};
	static LuaSet_iterator find(LuaSet& s, const T& val){
		return s.find(val);
	};
	static int count(LuaSet& s, const T& val){
		return s.count(val);
	};
	static bool empty(LuaSet& s){
		return s.empty();
	};
	static void clear(LuaSet& s){
		s.clear();
	};
	static int size(LuaSet& s){
		return s.size();
	};

	//iterator
	static LuaSet_iterator& iterator_increment(LuaSet_iterator& itr)
	{
		return ++itr;
	};
	static LuaSet_iterator& iterator_decrement(LuaSet_iterator& itr)
	{
		return --itr;
	};
	static T iterator_dereference(LuaSet_iterator& itr)
	{
		return *itr;
	};
	/*
	static void iterator_setDereference(LuaSet_iterator& itr, const T& value)
	{
		*itr = value;
	};
	*/
};

template<class T>
bool is_non_null(boost::shared_ptr<T> const& p)
{
    return (p != boost::shared_ptr<T>((T*)NULL));
}

template<class T>
bool is_equal(boost::shared_ptr<T> const& p1, boost::shared_ptr<T> const& p2)
{
    return p1 == p2;
}


template<class T>
class cValiableFieldChip_function
{
public:
	static void setVal(cValiableFieldChip<T>& chip, const 変数インデックス key, const T& val){
		chip.val(key) = val;
	}
	static T val(cValiableFieldChip<T>& chip, const 変数インデックス key){
		return chip.val(key);
	}
	static void dim(cValiableFieldChip<T>& chip, const 変数インデックス key, const T& val){
		chip.dim(key) = val;
	}

	static void setVal_str(cValiableFieldChip<T>& chip, const pLuaString key, const T& val){
		chip.val(key) = val;
	}
	static T val_str(cValiableFieldChip<T>& chip, const pLuaString key){
		return chip.val(key);
	}
	static void dim_str(cValiableFieldChip<T>& chip, const pLuaString key, const T& val){
		chip.dim(key) = val;
	}
};

}//namespace forLua