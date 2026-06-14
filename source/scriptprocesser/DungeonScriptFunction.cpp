#include <common.h>

#include "../cDungeonSystem.h"
#include "../FindUtility.h"
#include "../gameMainSystem/filemanage/cScriptRLayer.h"
#include "DungeonScriptFunction.h"
#include "../gameMainSystem/cEnvironment.h"
#include "DungeonScriptFunction.h"
#include "../cFloor.h"
#include <LuaUtility/LuaStringUtility.h>
#include "../cSaveStore.h"
#include "../caCamera.h"
#include "../caFunction.h"
#include "../EffectFunctions.h"
#include "../cObjectChara.h"
#include "../../cBoss.h"
#include "../cDrama.h"
#include "../utility/debug.h"
#include "../AnimeFunctions.h"
#include "../cEventBackground.h"
#include "../csTitle.h"
namespace forLua{


//生成
pcCharacter MakeCharacter(int ID,int CLASS,int Forse,pcLandform land) {
	return sg_pDungeonSystem->キャラクター生成(ID,CLASS,Forse,land);
}
pcCharacter getSumpleCharacter(int ID)
{
	return sg_pDungeonSystem->DataBase.GetSampleCharacter(ID);
}
pcCharacter changePlayerCharacter(int ID)
{
	return sg_pDungeonSystem->主人公交代(ID);
}
bool swapPlayerCharacter(pcCharacter pchara)
{
	return sg_pDungeonSystem->主人公交代(pchara);
}
pcDroping MakeDroping(int ID, pcLandform land)
{
	return sg_pDungeonSystem->落ち物生成(ID,NULL,land,true);
}
pcDroping MakeDropingAndPick(int ID, pcCharacter pchara)
{
	pcDroping pdrop = sg_pDungeonSystem->落ち物生成_設置なし(ID);
	pdrop->state() = cDroping::STATE_NORMAL;
	pdrop->quality() = 0;

	sg_pDungeonSystem->遠隔拾得要請_持ち物余白無考慮(pchara, pdrop);
	return pdrop;
}

#define DROPINGFUNCTION1_ForLua(FunctionNamePrefix,SystemFunctionName) \
int FunctionNamePrefix(pcDroping pdrop) {\
	return sg_pDungeonSystem->SystemFunctionName(pdrop, false);\
}

#define DROPINGFUNCTION2_ForLua(FunctionNamePrefix,SystemFunctionName,valiableType) \
int FunctionNamePrefix(pcDroping pdrop, valiableType value1) {\
	return sg_pDungeonSystem->SystemFunctionName(pdrop, value1, false);\
}

DROPINGFUNCTION1_ForLua(DropingDiscriminateComplete, 識別)
DROPINGFUNCTION1_ForLua(DropingDiscriminateState, 状態のみ識別)
DROPINGFUNCTION1_ForLua(DropingDiscriminateQuality, 修正のみ識別)
DROPINGFUNCTION1_ForLua(DropingDiscriminateName, 名称のみ識別)
DROPINGFUNCTION1_ForLua(DropingStateMakeGood, アイテム祝福)
DROPINGFUNCTION1_ForLua(DropingStateMakeBad, アイテム呪い)
DROPINGFUNCTION1_ForLua(DropingStateForsedMakeGood, アイテム状態強制設定祝福)
DROPINGFUNCTION1_ForLua(DropingStateForsedMakeNormal, アイテム状態強制設定通常)
DROPINGFUNCTION1_ForLua(DropingStateForsedMakeBad, アイテム状態強制設定呪い)

DROPINGFUNCTION1_ForLua(DropingQualityMakeGood, アイテム劣化)
DROPINGFUNCTION1_ForLua(DropingQualityMakeBad, アイテム強化)
DROPINGFUNCTION2_ForLua(DropingQualityForsedMakeValue, アイテム品質強制設定, int)

int DropingBreak(pcDroping pdrop)
{
	return sg_pDungeonSystem->落ち物破壊要請(pdrop);
}

#define DROPINGLUABINDDEF(FunctionNamePrefix) \
	luabind::def(#FunctionNamePrefix, &FunctionNamePrefix)
		



#define CHARACTERFUNCTION0_ForLua(FunctionNamePrefix,SystemFunctionName) \
int FunctionNamePrefix(pcCharacter pchara) {\
	return sg_pDungeonSystem->SystemFunctionName(pchara);\
}

#define CHARACTERFUNCTION1_ForLua(FunctionNamePrefix,SystemFunctionName) \
int FunctionNamePrefix(pcCharacter pchara) {\
	return sg_pDungeonSystem->SystemFunctionName(pchara, false);\
}

#define CHARACTERFUNCTION2_ForLua(FunctionNamePrefix,SystemFunctionName,valiableType) \
int FunctionNamePrefix(pcCharacter pchara, valiableType value1) {\
	return sg_pDungeonSystem->SystemFunctionName(pchara, value1, false);\
}

#define CHARACTERFUNCTION3_ForLua(FunctionNamePrefix,SystemFunctionName,valiableType1,valiableType2) \
int FunctionNamePrefix(pcCharacter pchara, valiableType1 value1, valiableType2 value2) {\
	return sg_pDungeonSystem->SystemFunctionName(pchara, value1, value2, false);\
}

CHARACTERFUNCTION2_ForLua(CharacterHPRecovery, 回復要請, int)
CHARACTERFUNCTION2_ForLua(CharacterSetHP, HP設定要請, int)
CHARACTERFUNCTION2_ForLua(CharacterMaxHPExtend, 最大HP拡張要請, int)
CHARACTERFUNCTION2_ForLua(CharacterMaxHPReduction, 最大HP縮小要請, int)

CHARACTERFUNCTION2_ForLua(CharacterStomachRecovery, 満腹度回復要請, double)
CHARACTERFUNCTION2_ForLua(CharacterStomachHungry, 満腹度減少要請, double)
CHARACTERFUNCTION2_ForLua(CharacterSetStomach, 満腹度設定要請, double)
CHARACTERFUNCTION2_ForLua(CharacterMaxStomachExtend, 最大満腹度拡張要請, int)
CHARACTERFUNCTION2_ForLua(CharacterMaxStomachReduction, 最大満腹度縮小要請, int)

CHARACTERFUNCTION2_ForLua(CharacterSpeedUp, 速度増加要請, int)
CHARACTERFUNCTION2_ForLua(CharacterSpeedDown, 速度減少要請, int)

CHARACTERFUNCTION2_ForLua(CharacterSleep, 眠り要請, int)
CHARACTERFUNCTION2_ForLua(CharacterDeepSleep, バクスイ要請, int)
CHARACTERFUNCTION2_ForLua(CharacterSurprise, びっくり要請, int)
CHARACTERFUNCTION2_ForLua(CharacterLockedIn, 金縛り要請, int)

CHARACTERFUNCTION2_ForLua(CharacterSeal, 封印要請, int)
CHARACTERFUNCTION2_ForLua(CharacterSwingAir, 空振り要請, int)
CHARACTERFUNCTION2_ForLua(CharacterPoor, 貧乏要請, int)
CHARACTERFUNCTION2_ForLua(CharacterCowardice, 臆病要請, int)
CHARACTERFUNCTION2_ForLua(CharacterJealousy, 嫉妬要請, int)
CHARACTERFUNCTION2_ForLua(CharacterFrenzy, 狂乱要請, int)

CHARACTERFUNCTION3_ForLua(CharacterAttackDown, 脱力要請, int, int)
CHARACTERFUNCTION3_ForLua(CharacterAttackUp, 元気要請, int, int)

CHARACTERFUNCTION3_ForLua(CharacterDefenceDown, 軟弱要請, int, int)
CHARACTERFUNCTION3_ForLua(CharacterDefenceUp, 頑強要請, int, int)

CHARACTERFUNCTION2_ForLua(CharacterDrunk, 泥酔要請, int)
CHARACTERFUNCTION2_ForLua(CharacterInIce, 氷付け要請, int)
CHARACTERFUNCTION2_ForLua(CharacterMatchless, 無敵要請, int)
CHARACTERFUNCTION2_ForLua(CharacterNightBlind, 鳥目要請, int)

CHARACTERFUNCTION3_ForLua(CharacterDeathTouch, 死の誘い要請, pcCharacter, int)
CHARACTERFUNCTION3_ForLua(CharacterSubstitution, 身代わり要請, pcCharacter, int)
CHARACTERFUNCTION2_ForLua(CharacterMimicry, 擬態要請, int)

CHARACTERFUNCTION1_ForLua(CharacterCureMental, 精神異常治療要請)
CHARACTERFUNCTION1_ForLua(CharacterCurePhysical, 身体異常治療要請)
CHARACTERFUNCTION1_ForLua(CharacterCureMagic, 呪術異常治療要請)
CHARACTERFUNCTION1_ForLua(CharacterCureSpeed, 速度異常治療要請)

CHARACTERFUNCTION0_ForLua(CharacterIsMentalAberration, 精神異常状態)
CHARACTERFUNCTION0_ForLua(CharacterIsPhysicalAberration, 身体異常状態)
CHARACTERFUNCTION0_ForLua(CharacterIsMagicAberration, 呪術異常状態)
CHARACTERFUNCTION0_ForLua(CharacterIsSpeedAberration, 速度異常状態)

CHARACTERFUNCTION2_ForLua(CharacterGoPast, やりすごし要請, int)

CHARACTERFUNCTION1_ForLua(CharacterEyeLotion, 目薬要請)

CHARACTERFUNCTION2_ForLua(CharacterForsedDamage, 強制ダメージ要請, int)

CHARACTERFUNCTION1_ForLua(CharacterInstantDeath, 即死要請)
CHARACTERFUNCTION1_ForLua(CharacterForseGoAway, 強制退場要請)

CHARACTERFUNCTION2_ForLua(CharacterGetExperience, 経験値獲得, int)
CHARACTERFUNCTION2_ForLua(CharacterLevelUp, レベルアップ, int)
CHARACTERFUNCTION2_ForLua(CharacterLevelDown, レベルダウン, int)

CHARACTERFUNCTION0_ForLua(CharacterForsedFlinch, 強制ひるみ要請)
CHARACTERFUNCTION0_ForLua(CharacterForsedTurnEnd, 強制ターンエンド要請)

CHARACTERFUNCTION0_ForLua(CharacterNormalAttack, 直接攻撃要請)
CHARACTERFUNCTION1_ForLua(CharacterForceEquipmentSwap, 強制装備回し)

int CharacterDeepDoze(pcCharacter pchara) {
	return pchara->Condition.深い居眠り追加();
}
int CharacterLightDoze(pcCharacter pchara) {
	return pchara->Condition.浅い居眠り追加();
}
int CharacterTurn(pcCharacter pchara, int aspect){
	return sg_pDungeonSystem->方向転換要請(pchara, aspect);
}
int CharacterTurnNow(pcCharacter pchara, int aspect){
	return sg_pDungeonSystem->方向転換要請_主人公専用(pchara, aspect);
}

int CharacterMove(pcCharacter pchara, int aspect){
	return sg_pDungeonSystem->移動要請(pchara, aspect);
}
bool CharacterCanMove(pcCharacter pchara, int aspect){
	return sg_pDungeonSystem->隣接地形移動可(pchara, aspect);
}

void CharacterVisualRefresh(pcCharacter pchara)
{
	/*
	sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(
		&(pchara->visibleplace),
		c4DVector(pchara->placeX, pchara->placeY, 0, 0));
	sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(
		&(pchara->visibleaspect),
		pchara->aspect);
	sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(
		&(pchara->opaque),
		(double)sg_pDungeonSystem->キャラクター目視可(pchara));
	sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(
		&(pchara->VisibleCLASS),
		pchara->CLASS);
	sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(
		&(pchara->visibleemotion),
		pchara->emotion);
		*/
	pchara->visibleplace = c4DVector(pchara->placeX, pchara->placeY, 0, 0);
	pchara->visibleaspect = pchara->aspect;
	pchara->opaque = (double)sg_pDungeonSystem->キャラクター目視可(pchara);
	pchara->VisibleCLASS = pchara->CLASS;
	pchara->visibleemotion = pchara->emotion;
}

int CharacterWarp(pcCharacter pchara, pcLandform pland){
	return sg_pDungeonSystem->ワープ要請(pchara, pland);
}
bool CharacterCanWarp(pcCharacter pchara, pcLandform pland){
	return sg_pDungeonSystem->ワープ地形移動可(pchara, pland);
}

int CharacterBlow(pcCharacter subjectchara, pcCharacter pchara, int aspect,int distance,int collisionpower){
	return sg_pDungeonSystem->吹き飛ばし要請(subjectchara, pchara, aspect, distance, collisionpower);
}
int CharacterPull(pcCharacter subjectchara, pcCharacter pchara, int aspect,int distance){
	return sg_pDungeonSystem->引き寄せ要請(subjectchara, pchara, aspect, distance);
}
int CharacterReplace(pcCharacter subjectchara, pcCharacter pchara){
	return sg_pDungeonSystem->場所替え要請(subjectchara, pchara);
}
int CharacterThrow(pcCharacter subjectchara, pcCharacter pchara, pcLandform toland, int collisionpower, int dropcollition, double speed)
{
	return sg_pDungeonSystem->投げ飛ばし要請( subjectchara, pchara, toland, collisionpower, dropcollition, speed);
}

int CharacterDisguise(pcCharacter pchara, int charaID)
{
	pchara->Condition.anotherSurfaceID = charaID;
	return true;
}



pcCharacter PlayerCharacter() {
	return sg_pDungeonSystem->pPlayerChara();
}
void turnEndPlayerCharacter() {
	sg_pDungeonSystem->ターンエンド();
}

int itemCount(vector<pcDroping> items) {
	return sg_pDungeonSystem->ItemSize(items);
}


#define CHARACTERLUABINDDEF(FunctionNamePrefix) \
	luabind::def(#FunctionNamePrefix, &FunctionNamePrefix)Can
		

pcLandform LandformRandom_RoomRandom_NotShop_Safe() {
	return FindUtility::各部屋等確率_ランダム地形検索_店回避_配置安全();
}
pcLandform LandformRandom_RoomRandom_Safe() {
	return FindUtility::各部屋等確率_ランダム地形検索_配置安全();
}
pcLandform LandformRandom_RoomRandom_OutOfRoom_Safe(pcLandform pland) {
	return FindUtility::部屋外優先_各部屋等確率_ランダム地形検索_配置安全(pland);
}
pcLandform LandformRandom_RoomRandom_OutOfSight_Safe(pcLandform pland) {
	return FindUtility::視界外優先_各部屋等確率_ランダム地形検索_配置安全(pland);
}
pcLandform LandformRandom_Random() {
	return sg_pDungeonSystem->RandomPlace();
}
pcLandform LandformPlace(int x, int y) {
	return sg_pDungeonSystem->Map().Land(EDGELANDWIDTH+x,EDGELANDWIDTH+y);
}
pcLandform LandformEdge() {
	return sg_pDungeonSystem->Map().edgeLand();
}
pcLandform LandformCharacterPosition(pcCharacter pchara) {
	return pchara->足元地形();
}
pcLandform LandformNearby(pcLandform outLand, int aspect, int distance){
	return sg_pDungeonSystem->隣接地形(outLand, aspect, distance);
}

bool isSafeSetCharacterLandform(pcLandform target)
{
	return sg_pDungeonSystem->キャラ配置安全(target);
}
pcLandform getSafeSetCharacterLandform(pcLandform target,pcCharacter pchara)
{
	return sg_pDungeonSystem->存在安全地形(target, pchara);
}

pcLandform getSafeSetCharacterLandform(pcLandform target,pcCharacter pchara);

//グローバル変数
int totalTurnCount() {
	return sg_pDungeonSystem->SumTurnCount();
}

int floorLevel() {
	return sg_pDungeonSystem->FloorLevel();
}

std::vector<int>& savable_freeFlags() {
	return sg_pDungeonSystem->FreeFlags();
}
std::map<tstring,int>& savable_localFlags() {
	return sg_pDungeonSystem->localFlags();
}
const std::map<tstring,int>& const_privateFlags() {
	return sg_pDungeonSystem->privateFlags();
}
const std::map<tstring,int>& const_globalFlags() {
	return sg_pDungeonSystem->pSaveData->globalFlags;
}
std::map<tstring,int>& savable_globalFlags() {
	return sg_pDungeonSystem->pSaveData->globalFlags;
}
bool isDungeonCleared(tstring& dungeonID)
{
	return (sg_pDungeonSystem->pSaveData->globalFlags_ClearedFlag(dungeonID,FALSE) != FALSE);
}
void gotoTempDungeon(tstring& dungeonID)
{
	sg_pDungeonSystem->GotoDungeon(dungeonID);
}
void setClearResultSkipFlag(bool flag)
{
	sg_pDungeonSystem->pDungeon()->setClearResultSkipFlag((int)flag);
}
bool isUnRelease()
{
#ifdef _UNRELEASE	
	return true;
#else
	return false;
#endif
}


const bool storyEventEnable()//ストーリーオン
{
	return (mapUtility::getMapValue(sg_pDungeonSystem->privateFlags(), cSaveQuest::privateFlagKey_StoryEventKey(), TRUE) == TRUE)
		&& sg_pDungeonSystem->isRegureCharacter(sg_pDungeonSystem->pDungeon()->DungeonID_, sg_pDungeonSystem->pPlayerChara());
}

const int getMapValue(map<tstring,int>& mapObject, tstring& key, int defaultValue)
{
	return mapUtility::getMapValue(mapObject, key, defaultValue);
}
void setMapValue(map<tstring,int>& mapObject, tstring& key, int setValue)
{
	mapUtility::setMapValue(mapObject, key, setValue);
}

void appearDungeon(tstring& dungeonID)
{	
	sg_pDungeonSystem->pSaveData->setGlobalFlags_AppearedFlag(dungeonID, TRUE);
}
bool isAppearedDungeon(tstring& dungeonID)
{	
	return sg_pDungeonSystem->pSaveData->globalFlags_AppearedFlag(dungeonID, FALSE);
}

pcScriptRLayer storyDataLayer() {
	return g_systemSR.pdata()->pmember((tstring)_T("Story"));
}
pcScriptRLayer localizeDataLayer() {
	return g_systemSR.pdata()->pmember((tstring)_T("Language"));
}
bool isNotGoodTerminateInBase() {
	return sg_pDungeonSystem->isNotGoodTerminateFlagInBase();
}

cPlayerInput* controllerInput()
{
	return g_pPlayerInput();
}

double random_independent()//独立ランダム
{
	return ((double)rand()) / ((double)RAND_MAX+1);
}


//イベント通知
bool eventNotification(const pLuaString eventName, const pLuaString notificatedObject, const pLuaString argument)
{
	return sg_pDungeonSystem->eventNotification(eventName, notificatedObject, argument);
}

//オブジェクト登録
bool addCharacterStorage(const pLuaString name, pcCharacter chara)
{
	pair<map<std::basic_string<LuaString>,pcCharacter>::iterator, bool> result = sg_pDungeonSystem->charactarStorage().insert(pair<std::basic_string<LuaString>,pcCharacter>(std::basic_string<LuaString>(name),chara));
	return result.second;
}
pcCharacter getCharacterStorage(const pLuaString characterName)
{
	map<std::basic_string<LuaString>,pcCharacter>::iterator itr = sg_pDungeonSystem->charactarStorage().find(std::basic_string<LuaString>(characterName));
	if(itr == sg_pDungeonSystem->charactarStorage().end())
	{
		return NULLCHARA;
	}
	return itr->second;
}
bool addDropingStorage(const pLuaString name, pcDroping drop)
{
	pair<map<std::basic_string<LuaString>,pcDroping>::iterator, bool> result = sg_pDungeonSystem->dropingStorage().insert(pair<std::basic_string<LuaString>,pcDroping>(std::basic_string<LuaString>(name),drop));
	return result.second;
}
pcDroping getDropingStorage(const pLuaString dropingName)
{
	map<std::basic_string<LuaString>,pcDroping>::iterator itr = sg_pDungeonSystem->dropingStorage().find(std::basic_string<LuaString>(dropingName));
	if(itr == sg_pDungeonSystem->dropingStorage().end())
	{
		return NULLDROP;
	}
	return itr->second;
}
bool addLandformStorage(const pLuaString name, pcLandform land)
{
	pair<map<std::basic_string<LuaString>,pcLandform>::iterator, bool> result = sg_pDungeonSystem->landformStorage().insert(pair<std::basic_string<LuaString>,pcLandform>(std::basic_string<LuaString>(name),land));
	return result.second;
}
pcLandform getLandformStorage(const pLuaString landformName)
{
	map<std::basic_string<LuaString>,pcLandform>::iterator itr = sg_pDungeonSystem->landformStorage().find(std::basic_string<LuaString>(landformName));
	if(itr == sg_pDungeonSystem->landformStorage().end())
	{
		return NULLLAND;
	}
	return itr->second;
}


//メッセージ

//virtual int メッセージ(StyleString Message);
int message(StyleString& Message)
{
	return sg_pDungeonSystem->メッセージ(Message);
}
//virtual int メッセージ決定待ち();
int messageWait()
{
	return sg_pDungeonSystem->メッセージ決定待ち();
}
int messageWaitAnyKey()
{
	return sg_pDungeonSystem->メッセージ何かボタン入力待ち();
}


bool utility_storyMessage(const tstring& name, std::map<tstring, StyleString > & valiable) {
	return g_Storyメッセージ(name.c_str(), valiable);
}
bool utility_localizedMessage(const tstring& name, std::map<tstring, StyleString > & valiable) {
	return g_Langメッセージ(name.c_str(), valiable);
}
bool isExistStoryMessage(const tstring& name)
{
	pcScriptRLayer player = storyDataLayer()->pmember(name);
	return (player != NULL);
}
bool isExistLocalizedMessage(const tstring& name)
{
	pcScriptRLayer player = localizeDataLayer()->pmember(name);
	return (player != NULL);
}
void storyEventMessageLoop(const tstring& keyPrefix, std::map<tstring, StyleString> & StringValiable, luabind::object intervalFunction) {
	
    int index = 1;
	TCHAR keySuffix[32];
	
	while(1) {
		_stprintf(keySuffix,_T("%d"),index);
        tstring massageKey = keyPrefix + keySuffix;
		if (!isExistStoryMessage(massageKey)) {
            break;
		}
		utility_storyMessage(massageKey,StringValiable);
		messageWait();
		if(luabind::type(intervalFunction) == LUA_TFUNCTION)
		{
			try
			{
				luabind::call_function<void>(intervalFunction);
			}
			catch(luabind::error const& e)
			{
				lua_error_handler(intervalFunction.interpreter(), (tstring)_T("error_caLuaFunction.txt"));
				OnAssert(_T(__FILE__),__LINE__,false,_T("storyEventMessageLoop Function 内で例外が発生しました 。　error. in storyEventMessageLoopFunction."));
			}
		}
        index = index + 1;
	}
}
void storyEventMessageLoopRange(const tstring& keyPrefix, int firstIndex, int lastIndex, std::map<tstring, StyleString> & StringValiable, luabind::object intervalFunction)
{
    int index = firstIndex;
	TCHAR keySuffix[32];
	
	while(1) {
		_stprintf(keySuffix,_T("%d"),index);
        tstring massageKey = keyPrefix + keySuffix;
		if (lastIndex < index || !isExistStoryMessage(massageKey)) {
            break;
		}
		utility_storyMessage(massageKey,StringValiable);
		messageWait();
		if(luabind::type(intervalFunction) == LUA_TFUNCTION)
		{
			try
			{
				luabind::call_function<void>(intervalFunction);
			}
			catch(luabind::error const& e)
			{
				lua_error_handler(intervalFunction.interpreter(), (tstring)_T("error_caLuaFunction.txt"));
				OnAssert(_T(__FILE__),__LINE__,false,_T("storyEventMessageLoopRange Function 内で例外が発生しました 。　error. in storyEventMessageLoopFunction."));
			}
		}
        index = index + 1;
	}
}

StyleString localizeString(const tstring& name, std::map<tstring, StyleString > & valiable)
{
	return g_Lang(name.c_str(),valiable);
}
StyleString localizeString(const tstring& name)
{
	std::map<tstring, StyleString > valiable;
	return g_Lang(name.c_str(),valiable);
}

void goToNextFloor()
{
	sg_pDungeonSystem->メニューを閉じる();
	sg_pDungeonSystem->フロア更新要請();
}

void cameraMove(double x, double y, double speed, double point1, double point2)
{
	pcaCameraMove panime(new caCameraMove(x,y,speed,point1,point2));
	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(panime));

}
void cameraMoveParallel(double x, double y, double speed, double point1, double point2)
{
	pcaCameraMove panime(new caCameraMove(x,y,speed,point1,point2));
	sg_pDungeonSystem->AnimationManager().AddAnime_parallel(boost::static_pointer_cast<cAnimation>(panime));
}
//エフェクト
void effect_Wait(const double placeX,const double placeY, const int waitFrame)
{
	sg_pDungeonSystem->AnimationManager().Anime_Wait(placeX + EDGELANDWIDTH, placeY + EDGELANDWIDTH, waitFrame);
}
void effect_WaitPararellCritical(const double placeX,const double placeY, const int waitFrame)
{
	sg_pDungeonSystem->AnimationManager().Anime_Wait_parallelCritical(placeX + EDGELANDWIDTH, placeY + EDGELANDWIDTH, waitFrame);
}
void effect_Smoke1(const double placeX,const double placeY)
{
	EffectFunctions::煙エフェクト1(placeX + EDGELANDWIDTH, placeY + EDGELANDWIDTH);
}
void effect_boss_focus(const double placeX,const double placeY)
{
	EffectFunctions::ボスエフェクト集中(placeX + EDGELANDWIDTH, placeY + EDGELANDWIDTH, 0);
}
void effect_boss_charge(const double placeX,const double placeY, cColor& imagecolor1,  cColor& imagecolor2)
{
	EffectFunctions::ボスエフェクトため(placeX + EDGELANDWIDTH, placeY + EDGELANDWIDTH, imagecolor1, imagecolor2);
}
void effect_boss_emission(const double placeX,const double placeY, cColor& imagecolor1,  cColor& imagecolor2)
{
	EffectFunctions::ボスエフェクト発散(placeX + EDGELANDWIDTH, placeY + EDGELANDWIDTH, imagecolor1, imagecolor2);
}
void effect_boss_defeat(const double placeX,const double placeY)
{
	EffectFunctions::ボスエフェクト撃破(placeX + EDGELANDWIDTH, placeY + EDGELANDWIDTH, 0.5);
}
void effect_powerUp(const double placeX, const double placeY, double color_Hue)
{
	EffectFunctions::パワーアップエフェクト(placeX + EDGELANDWIDTH, placeY + EDGELANDWIDTH, color_Hue);
}
void effect_powerDown(const double placeX, const double placeY, double color_Hue)
{
	EffectFunctions::パワーダウンエフェクト(placeX + EDGELANDWIDTH, placeY + EDGELANDWIDTH, color_Hue);
}
void effect_lightning(const double placeX, const double placeY)
{
	EffectFunctions::落雷エフェクト(placeX + EDGELANDWIDTH, placeY + EDGELANDWIDTH);
}
void effect_generalStamp(const double placeX, const double placeY, const tstring& fileName, TextureRangeRect& texRenge, cColor& burnColor, const double size, const int duration, const int delay)
{
	LuaStringTransformer luaStringTransfer(fileName);
	EffectFunctions::スタンプ(placeX + EDGELANDWIDTH, placeY + EDGELANDWIDTH, luaStringTransfer.t_str(), texRenge, burnColor, size, duration, delay);
}
void effect_generalFog(const double placeX, const double placeY, const tstring& fileName, TextureRangeRect& texRenge, cColor& burnColor, const double size, const double generateRadius, const double generateRatio, const int duration, const int delay)
{
	LuaStringTransformer luaStringTransfer(fileName);
	EffectFunctions::霧(placeX + EDGELANDWIDTH, placeY + EDGELANDWIDTH, luaStringTransfer.t_str(), texRenge, burnColor, size, generateRadius, generateRatio, duration, delay);
}
double charaPos(const double effectPos)
{
	return effectPos - EDGELANDWIDTH;
}
bool moveWithAnimeToLand(pcCharacter pchara, pcLandform toLand, bool neverTurn, double speed, bool omission)
{
	return AnimeFunctions::moveWithAnimeToLand(pchara, toLand, neverTurn, speed, omission);
}
bool moveWithAnime(pcCharacter pchara, int aspect, int distance, bool neverTurn, double speed, bool omission)
{
	return AnimeFunctions::moveWithAnime(pchara, aspect, distance, neverTurn, speed, omission);
}
bool jumpWithAnimeToLand(pcCharacter pchara, pcLandform toLand, double height, bool neverTurn, double speed, bool omission)
{
	return AnimeFunctions::jumpWithAnimeToLand(pchara, toLand, height, neverTurn, speed, omission);
}
bool jumpWithAnime(pcCharacter pchara, int aspect, int distance, double height, bool neverTurn, double speed, bool omission)
{
	return AnimeFunctions::jumpWithAnime(pchara, aspect, distance, height, neverTurn, speed, omission);
}
void fadeBlack(int duringFrame)
{
	sg_pDungeonSystem->EventManager().fadeIn(cColor(255,0,0,0), duringFrame);
}
void fadeWhite(int duringFrame)
{
	sg_pDungeonSystem->EventManager().fadeIn(cColor(255,255,255,255), duringFrame);
}
void fadeWithColor(cColor color, int duringFrame)
{
	sg_pDungeonSystem->EventManager().fadeIn(color, duringFrame);
}
void fadeOut(int duringFrame)
{
	sg_pDungeonSystem->EventManager().fadeOut(duringFrame);
}
pcEventBackground EventBackground(const int priority, const tstring& fileName, int placeX, int placeY, int power)
{
	PositionPerGrid position(placeX + EDGELANDWIDTH, placeY + EDGELANDWIDTH, 0);
	pcEventBackground pEventBackground = cEventBackground::pNewEvent(priority, fileName, position, power);
	sg_pDungeonSystem->EventManager().addEvent(pEventBackground);
	return pEventBackground;
}
void setHidingMiniMap(bool hidingMiniMap)
{
	sg_pDungeonSystem->s_minimapHiding() = hidingMiniMap;
}
void setHiddenInterface(bool hidden)
{
	sg_pDungeonSystem->setHiddenInterface(hidden);
}
//検索
pcCharacter searchOneCharacterOnFloor(int ID)
{
	vector<pcCharacter>& searchChara = searchCharactersOnFloor(ID);
	if(searchChara.empty())
	{
		return NULLCHARA;
	}
	return searchChara[0];
}
vector<pcCharacter> searchCharactersOnFloor(int ID)
{
	vector<pcCharacter> vchara;
	const vector<pcCharacter>& floorChara = allCharacterList();
	int i, size = floorChara.size();
	for(i = 0; i < size; i++)
	{
		if(floorChara[i]->ID() == ID)
		{
			vchara.push_back(floorChara[i]);
		}
	}
	return vchara;
}

void stackFunction(luabind::object function)
{
	pcaLuaFunction panime(new caLuaFunction(function,0));
	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(panime));
}
void stackFunction(luabind::object function, luabind::object val1)
{
	pcaLuaFunction panime(new caLuaFunction(function,val1,0));
	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(panime));
}
void stackFunction(luabind::object function, luabind::object val1, luabind::object val2)
{
	pcaLuaFunction panime(new caLuaFunction(function,val1,val2,0));
	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(panime));
}
void stackFunction(luabind::object function, luabind::object val1, luabind::object val2, luabind::object val3)
{
	pcaLuaFunction panime(new caLuaFunction(function,val1,val2,val3,0));
	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(panime));
}
void stackFunction(luabind::object function, luabind::object val1, luabind::object val2, luabind::object val3, luabind::object val4)
{
	pcaLuaFunction panime(new caLuaFunction(function,val1,val2,val3,val4,0));
	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(panime));
}
void stackFunction(luabind::object function, luabind::object val1, luabind::object val2, luabind::object val3, luabind::object val4,
	luabind::object val5)
{
	pcaLuaFunction panime(new caLuaFunction(function,val1,val2,val3,val4,val5,0));
	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(panime));
}
void stackFunction(luabind::object function, luabind::object val1, luabind::object val2, luabind::object val3, luabind::object val4,
	luabind::object val5, luabind::object val6)
{
	pcaLuaFunction panime(new caLuaFunction(function,val1,val2,val3,val4,val5,val6,0));
	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(panime));
}
void stackFunction(luabind::object function, luabind::object val1, luabind::object val2, luabind::object val3, luabind::object val4,
	luabind::object val5, luabind::object val6, luabind::object val7)
{
	pcaLuaFunction panime(new caLuaFunction(function,val1,val2,val3,val4,val5,val6,val7,0));
	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(panime));
}
void stackFunction(luabind::object function, luabind::object val1, luabind::object val2, luabind::object val3, luabind::object val4,
	luabind::object val5, luabind::object val6, luabind::object val7, luabind::object val8)
{
	pcaLuaFunction panime(new caLuaFunction(function,val1,val2,val3,val4,val5,val6,val7,val8,0));
	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(panime));
}


void outputDebugString(const char* message)
{
	LuaStringTransformer luaStringTransfer(message);
	OutputDebugString(luaStringTransfer.t_str().c_str());
}
void outputDebugValue(const double value)
{
	StyleString valueString;
	valueString.addstring(value);
	OutputDebugString(valueString.c_str());
}
//----------------------------------------------------------
pcScriptRLayer storyLayerRoot()
{
	return g_systemSR.pdata()->pmember((tstring)_T("Story"));
}
//
int buttonIndex(int index)
{
	return 1+g_GameEnv.m_Input.patInputManager->padconfigI2B[index];
}
bool isStockingMassage()
{
	return sg_pDungeonSystem->MessageW().isStockingMassage();
}
bool clearMassage()
{
	return sg_pDungeonSystem->メッセージ消去();
}
pcLandform alternativeLand(pcLandform main, int orderNum)
{
	return sg_pDungeonSystem->代理地形(main, orderNum);
}
bool canDisCoverCharacter(pcCharacter frompchara, pcCharacter topchara)
{
	return sg_pDungeonSystem->キャラクター目視可(frompchara, topchara);
}
bool canDisCoverDroping(pcCharacter frompchara, pcDroping pdrop)
{
	return sg_pDungeonSystem->落ち物目視可(frompchara, pdrop);
}
bool canDisCoverLandform(pcCharacter frompchara, pcLandform toLand)
{
	return sg_pDungeonSystem->地形目視可(frompchara, toLand);
}
bool canLandformSeeLandform(pcLandform fromLand, pcLandform toLand)
{
	return sg_pDungeonSystem->地形目視可(fromLand, toLand);
}
bool makeListDisCoverCharacter(pcCharacter frompchara, vector<pcCharacter>& outputlist)
{
	return sg_pDungeonSystem->キャラクター目視可範囲キャラリスト(frompchara, outputlist);
}
bool makeListDisCoverDroping(pcCharacter frompchara, vector<pcDroping>& outputlist)
{
	return sg_pDungeonSystem->キャラクター目視可範囲落ち物リスト(frompchara, outputlist);
}
bool makeListDisCoverExit(pcCharacter frompchara, vector<pcLandform>& outputlist)
{
	return sg_pDungeonSystem->地形目視可範囲出口リスト(frompchara, outputlist);
}
bool isWallSeparated(pcLandform start, pcLandform end)
{
	return sg_pDungeonSystem->壁はさみ判定(start, end);
}
pcLandform landformInFrontOfCharacter(pcCharacter pchara, int distance)
{
	return sg_pDungeonSystem->キャラ前地形(pchara, distance);
}
pcLandform landformAroundOfCharacter(pcCharacter pchara, int plusaspect, int distance)
{
	return sg_pDungeonSystem->キャラ視点方角地形(pchara, plusaspect, distance);
}
bool culcuLandformWallReflection(pcLandform &outLand, int &outaspect, pcLandform comeLand, int comeaspect)
{
	return sg_pDungeonSystem->壁反射判定且検索(outLand, outaspect, comeLand, comeaspect);
}
bool culcuLandformWallBending(pcLandform &outLand, int &outaspect, pcLandform comeLand, int comeaspect)
{
	return sg_pDungeonSystem->壁曲がり判定且検索(outLand, outaspect, comeLand, comeaspect);
}

pcLandform landformSearchObstacleWithRange(pcLandform start, int aspect, int range, bool stopWall, bool stopChara, bool stopDrop, bool stopNonthrough)
{
	return sg_pDungeonSystem->射程内障害物検索(start, aspect, range, stopWall, stopChara, stopDrop, stopNonthrough);
}
pcLandform landformSearchObstacleGetPathWithRange(vector<pcLandform>& landpass ,pcLandform start, int aspect, int range, bool stopWall, bool stopChara, bool stopDrop, bool stopNonthrough)
{
	return sg_pDungeonSystem->射程内障害物検索( landpass , start, aspect, range, stopWall, stopChara, stopDrop, stopNonthrough);
}
pcLandform landformSearchObstacle(pcLandform start, int aspect, bool stopWall, bool stopChara, bool stopDrop)
{
	return sg_pDungeonSystem->障害物検索( start, aspect, stopWall, stopChara, stopDrop);
}
bool isCharacterCanNeighberAttack(pcCharacter pchara, int aspect, int distance, int diagonthrough, int wallthrough)
{
	return sg_pDungeonSystem->isCanNeighberAttack(pchara,aspect,distance,diagonthrough,wallthrough);
}

pcLandform landformSearchDropDroping(pcLandform start, pcDroping pdrop)
{
	return sg_pDungeonSystem->落下可能地点検索( start, pdrop);
}
pcLandform landformSearchDropCharacter(pcLandform start, pcCharacter pchara)
{
	return sg_pDungeonSystem->落下可能地点検索( start, pchara);
}
bool isEnemyCharacter(pcCharacter pcharaA, pcCharacter pcharaB)
{
	return sg_pDungeonSystem->キャラクター敵対判定( pcharaA, pcharaB);
}

void mappingFromLandform(pcLandform pland)
{
	sg_pDungeonSystem->ある地点からマッピング(pland);
}
void mappingAllLandform()
{
	sg_pDungeonSystem->全マッピング();
}
void disMappingFromLandform()
{
	sg_pDungeonSystem->全マッピング解除();
}

bool isFloorLight()
{
	return sg_pDungeonSystem->フロア明かり状態();
}
void addFloorLight()
{
	sg_pDungeonSystem->フロア明かり追加();
}
void eraseFloorLight()
{
	sg_pDungeonSystem->フロア明かり削除();
}

bool isFloorItemCantThrow()
{
	return sg_pDungeonSystem->フロア飛ばず状態();
}
void addFloorItemCantThrow(int turn)
{
	sg_pDungeonSystem->フロア飛ばず追加(turn);
}
void eraseFloorItemCantThrow()
{
	sg_pDungeonSystem->フロア飛ばず削除();
}

bool isFloorTimeStop()
{
	return sg_pDungeonSystem->フロア時間停止状態();
}
void addFloorTimeStop(int turn)
{
	sg_pDungeonSystem->フロア時間停止追加(turn);
}
void eraseFloorTimeStop()
{
	sg_pDungeonSystem->フロア時間停止削除();
}

bool isFloorTimeSkip()
{
	return sg_pDungeonSystem->フロア時間飛ばし状態();
}
void addFloorTimeSkip(int turn)
{
	sg_pDungeonSystem->フロア時間飛ばし追加(turn);
}
void eraseFloorTimeSkip()
{
	sg_pDungeonSystem->フロア時間飛ばし削除();
}

void isOctasDirection(cCoordinate& from, cCoordinate& to)
{
	sg_pDungeonSystem->八方判定(from, to);
}

void mappingDroping(pcDroping pdrop)
{
	sg_pDungeonSystem->アイテムマッピング(pdrop);
}

void mapTransformArrangement()
{
	sg_pDungeonSystem->地形整理();
}
void mapTransformWallDig(pcLandform pland)
{
	sg_pDungeonSystem->穴掘り(pland, 0);
}
void mapTransformWaterMake(pcLandform pland)
{
	sg_pDungeonSystem->水掘り(pland, 0);
}
void mapTransformWaterCover(pcLandform pland)
{
	sg_pDungeonSystem->水埋め(pland, 0);
}
void mapTransformRoomBreak(pcLandform pland, int range)
{
	sg_pDungeonSystem->部屋つぶし(pland, range, 0);
}
void mapTransformMakeBigRoom()
{
	sg_pDungeonSystem->大部屋(0);
}
void mapTransformForseLandChange(pcLandform pland, int mapKind)
{
	sg_pDungeonSystem->landchange(pland,mapKind);
}
const vector<pcCharacter> allCharacterList()
{
	return sg_pDungeonSystem->CharaList();
}
const vector<pcDroping> allDropingList()
{
	return sg_pDungeonSystem->DropList();
}

void showSelectDungeonWindow(const vector<tstring>& Dungeons)
{
	return sg_pDungeonSystem->SelectDungeon(Dungeons);
}

//-------------------------
pcDroping spawnFloorNativeItem(pcLandform pland)
{
	int vint[3];
	sg_pDungeonSystem->アイテム自然湧きID(vint);
	return sg_pDungeonSystem->落ち物生成(vint[0],NULL,pland,true);
}
pcDroping spawnFloorNativeTrap(pcLandform pland)
{
	int vint[3];
	sg_pDungeonSystem->罠自然湧きID(vint);
	return sg_pDungeonSystem->落ち物生成(vint[0],NULL,pland,true);
}
int getIDOfFloorNativeItem(double counter_rarity,
							double Equipment,
							double Bullet,
							double Spell,
							double Food,
							double Drink,
							double Book,
							double Box,
							double MoneyBag,
							double Other)
{
	int vint[3];
	sg_pDungeonSystem->罠自然湧きID(vint);
	sg_pDungeonSystem->アイテムレア種別湧きID(vint,
							counter_rarity,
							Equipment,
							Bullet,
							Spell,
							Food,
							Drink,
							Book,
							Box,
							MoneyBag,
							Other
							);
	return vint[0];
}
pcCharacter spawnFloorNativeEnemy(pcLandform pland, int forse)
{
	int vint[3];
	sg_pDungeonSystem->敵自然湧きID(vint);
	return sg_pDungeonSystem->キャラクター生成(vint[0],vint[1],forse,pland);
}
bool turnCharacterForCharacter(pcCharacter me, pcCharacter you)
{
	return GameIdiom::キャラの方を向く(me, you);
}
bool isValidDirectAttackToEnemy(pcCharacter me, pcCharacter enemy)
{
	return FindUtility::キャラが敵で直接攻撃可能か(me, enemy);
}
bool isValidDirectAttackToFriend(pcCharacter me, pcCharacter chara)
{
	return FindUtility::キャラが味方で直接攻撃可能か(me, chara);
}
bool isValidShootToCharactor(pcCharacter me, pcCharacter enemy, const int range)
{
	return FindUtility::キャラ８方向射線で範囲以内(me, enemy, range);
}
int distanceOfBetweenCharacters(pcCharacter me, pcCharacter chara)
{
	return FindUtility::キャラの距離(me, chara);
}
int aspectOfBetweenCharacters(pcCharacter me, pcCharacter chara)
{
	return FindUtility::キャラの方向(me, chara);
}
int marginFromShootLineOfBetweenCharacters(pcCharacter me, pcCharacter chara)
{
	return FindUtility::キャラ八方ズレ数(me, chara);
}
//財布のお金
int walletMoney()
{
	return sg_pDungeonSystem->主人公お金所持量();
}
//厄ポイント
int yakuPoint()
{
	return sg_pDungeonSystem->pSaveData->pSaveStore()->yaku;
}
//プレイヤーレベル
int globalPlayerLavel()
{
	return sg_pDungeonSystem->pSaveData->playerLevel;
}
int globalPlayerExp()
{
	return sg_pDungeonSystem->pSaveData->playerExp;
}
bool AIshortCut_NomoveAttack(pcCharacter boss, cValiableField& valiable)
{
	{//目標点を向く
		pcLandform pland = valiable.lands[変数_攻撃場所];
		cCoordinate coo,mep,aspectcoo;
		int aspect;
		mep.x = boss->placeX;
		mep.y = boss->placeY;
		coo.x = pland->placeX;
		coo.y = pland->placeY;

		aspectcoo = coo-mep;

		if(aspectcoo.dif() != 0)
		{
			aspect = aspectcoo.GetAspect();
			sg_pDungeonSystem->方向転換要請(boss, aspect);
		}
	}

	multimap<int,pcCharacter> sortMap;
	vector<pcCharacter> visibleCharalist;

	sg_pDungeonSystem->キャラクター目視可範囲キャラリスト(boss,visibleCharalist);
	
	int i;
	for(i=0;i<visibleCharalist.size();i++)
	{
		if(boss->pAI->u_敵隣接判定(visibleCharalist[i]))
		{
			sortMap.insert(pair<int,pcCharacter>(boss->pAI->u_攻撃優先度_昇順(visibleCharalist[i]),visibleCharalist[i]));
		}
	}

	multimap<int,pcCharacter>::iterator itr = sortMap.begin();
	if(sortMap.size())
	{
		for(;itr != sortMap.end();itr++)
		{
			boss->pAI->u_敵直接攻撃(itr->second);
			return true;
		}
	}
	return false;
}
int distanceBetweenCharacter(pcCharacter me, pcCharacter you)
{
	return FindUtility::キャラの距離(me, you);
}

void pcGameWindow_showWindow(pcGameWindow& window)
{
	pcControlLayer pccl;

	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));

	pccl->Init(sg_pDungeonSystem->pDevice_D3D);
	pccl->WindowList.push_back(window);
	window->ReRendarText();
}
void menuDismissAllWindows()
{
	sg_pDungeonSystem->メニューを閉じる();
}
void menuDismissWindowLayer()
{
	sg_pDungeonSystem->メニューを一枚閉じる();
}
void menuRerenderAllWindows()
{
	sg_pDungeonSystem->メニューリレンダ();
}

int menuDefaultPositionLeft(int level)
{
	return sg_pDungeonSystem->GameScreenInterface.menuPosLeft(level);
}
int menuDefaultPositionTop(int level)
{
	return sg_pDungeonSystem->GameScreenInterface.menuPosTop(level);
}
int menuDefaultPositionRight(int level)
{
	return sg_pDungeonSystem->GameScreenInterface.menuPosRight(level);
}
int menuDefaultPositionBottom(int level)
{
	return sg_pDungeonSystem->GameScreenInterface.menuPosBottom(level);
}

int eraseDanmaku(pcCharacter boss)
{
	return cBossSpell::eraseDanmaku(boss);
}

int eraseFloorDropings()
{
	int count = 0;
	vector<pcDroping> dropings = sg_pDungeonSystem->DropList();
	int i, size = dropings.size();

	for(i=0; i<size; i++)
	{
		if(dropings[i]->落ちてる()) {
			sg_pDungeonSystem->落ち物破壊要請(dropings[i]);
		}
	}
	return count;
}
int eraseEnemys()
{
	int count = 0;
	vector<pcCharacter> characters = sg_pDungeonSystem->CharaList();
	int i, size = characters.size();

	for(i=0; i<size; i++)
	{
		if(characters[i]->Forse == CHARACTER_FORSE_ENEMY) {
			sg_pDungeonSystem->強制退場要請(characters[i]);
		}
	}
	return count;
}
void gotoSceneTitle() {
	g_GameEnv.m_SceneManage->SceneChange(sg_pDungeonSystem->pDevice_D3D,new csTitle);
}
bool stackDramaThread(luabind::object thread)
{
	pcDrama pdrama(new cDrama(thread));
	if(pdrama->isError())
	{
		return false;
	}
	sg_pDungeonSystem->DramaManager().stackDrama(pdrama);
	return true;
}

//-------------------------

void PlayBGM(const tstring& intro, const tstring& main)
{
	sg_pDungeonSystem->AnimationManager().Anime_PlayBGM( intro, main );
}
void PlayNormalBGM()
{
	sg_pDungeonSystem->PlayBGM();
}
void StopBGM()
{
	sg_pDungeonSystem->AnimationManager().Anime_StopBGM();
}

pcCharacter bulletSummon(pcLandform land,
		int HP,
		int Forse,
		int speed,
		int type,
		int color,
		int aspect)
{
	return ::弾幕::弾幕召喚(land,
		HP,
		Forse,
		speed,
		(弾幕::弾幕Type)type,
		(弾幕::弾幕色)color,
		aspect,
		NULLCHARA);
}

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
		bool isStopAtWall)
{
	return ::弾幕::レーザー召喚(land,
		aspect,
		length,
		power,
		Forse,
		color,
		readyTurn,
		attackTurn,
		isAttackFriend,
		isStopAtWall);
}

void startBossSpell(pcCharacter pchara)
{
	sg_pDungeonSystem->強制ダメージ要請(pchara,1,0,0);
}

void attackHappen(pcCharacter pchara, pcLandform pland, double power, int constFlag, vector<int> attackAttributes, bool surelyHitFlag)
{
	multiset<攻撃属性::攻撃属性> 属性;
	int i, size = attackAttributes.size();
	for(i=0;i<size;i++) {
		属性.insert((攻撃属性::攻撃属性)attackAttributes[i]);
	}
	sg_pDungeonSystem->攻撃接近(攻撃作成(
		pchara,//攻撃者
		pland,//場所
		power,//攻撃力
		constFlag,//定数ダメージ
		属性,
		surelyHitFlag));
}



}//namespace forLua