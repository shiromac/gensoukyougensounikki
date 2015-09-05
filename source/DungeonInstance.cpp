#include "stdafx.h"

#include "DungeonInstance.h"
//#include "utility/mt19937ar.h"
#include "utility/cAltaMatterPlus.h"
#include "Gameobjects.h"

#include "FireCommand.h"
#include "FindUtility.h"

#include "csHomeFirst.h"

#include "cDropingDistribution.h"

#include "GameIdiom.h"
#include "cFloor.h"

#include "utility/debug.h"

#include "ceaiActive.h"

#include <boost/pointer_cast.hpp>

cDungeonInstance::cDungeonInstance()
{

	GameOverFlag_ = false;
	GameClearFlag_ = false;

	SumTurnCount_ = 0;

	time_SumFrameCount_ = 0;
	time_Sumtime_ = 0;
	SumEnemyDefeatNum_ = 0;
}
cDungeonInstance::~cDungeonInstance()
{

}
void cDungeonInstance::insertLuaEnv(pDungeonStoryScript_LuaEnvironment luaEnv)
{
	vector<pDungeonStoryScript_LuaEnvironment>::iterator itr = std::find(s_luaEnvVector_.begin(), s_luaEnvVector_.end(), luaEnv);
	if( itr == s_luaEnvVector_.end() )
	{
		s_luaEnvVector_.push_back(luaEnv);
	}
}
void cDungeonInstance::deleteLuaEnv(pDungeonStoryScript_LuaEnvironment luaEnv)
{
	vector<pDungeonStoryScript_LuaEnvironment>::iterator itr = std::find(s_luaEnvVector_.begin(), s_luaEnvVector_.end(), luaEnv);
	if( itr != s_luaEnvVector_.end() )
	{
		s_luaEnvVector_.erase(itr);
	}
}
const vector<pDungeonStoryScript_LuaEnvironment> cDungeonInstance::s_luaEnvVector()
{
	return s_luaEnvVector_;
}

cFloorInstance::cFloorInstance()
{
	s_cannotInputWaitCount_ = 0;
	s_phase_ = GAME_PHASE_INPUT;
	s_turn_speed_ = GAME_TURN_SPEED_SINGLE;
	s_DashButtonTap_ = 0;
	s_DashButtonCancelEffect_ = 0;
	s_AttackDelay_ = 0;
	s_AttackRapidSwitch_ = 0;
	s_skipValiable_ = 0;
	s_shootAndShortCutswitch_ = 0;
	s_ShortCutdiagonsupport_ = 0;
	s_ShortCutSelectSwitch_ = 0;
	s_ShortCutSelectOffCount_ = 0;
	s_minimapHiding_ = false;

	floorlightswitchvisible_ = 0;
	floorlightswitch_ = 0;
	floornothrowswitch_ = 0;
	floortimestopswitch_ = 0;
	floortimeskipswitch_ = 0;

	floormoveswitch_ = 0;
	floorstealswitch_ = 0;
	floorshopownermoney_ = 0;
	floorshopprofit_ = 0.0;
	floorshoplastroom_ = -1;

	floorNowParty_ = 0;

	s_aspectTurndiagonsupport_ = 0;

	time_FrameCount_ = 0;
	time_Starttime_ = 0;

	
	TurnCount_ = 0;

	count_minimap_ = 0;
	cannotinput_ = false;
	aspectTurnprocessswitch_ = false;
	DashSwitch_ = 0;
	DashStopDelay_ = 0;
	DashEnemyCount_ = 0;

	Processmode_ = 0;
}

cFloorInstance::~cFloorInstance()
{

}

