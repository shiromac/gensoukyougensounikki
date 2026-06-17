
#include "cLuaFunctionCommand.h"


#include "cDungeonSystem.h"


#include "utility/LuaUtility/LuaEnvironment.h"
/*
cLuaFunctionCommand::cLuaFunctionCommand(StyleString cap, pLuaEnvironment luaEnv, const tstring functionName, StyleString shortExplanation)
:luaEnv_(luaEnv), functionName_(functionName), shortExplanation_(shortExplanation)
{
	caption = cap;
	shortExplanation_ = shortExplanation;
}
cLuaFunctionCommand::cLuaFunctionCommand(StyleString cap, pLuaEnvironment luaEnv, const pLuaString functionName, StyleString shortExplanation)
	:luaEnv_(luaEnv), functionName_(functionName), shortExplanation_(shortExplanation)
{
	caption = cap;
	shortExplanation_ = shortExplanation;
}
*/
cLuaFunctionCommand::cLuaFunctionCommand(StyleString cap, ::luabind::object& function, StyleString shortExplanation):function_(function)
{
	caption = cap;
	shortExplanation_ = shortExplanation;
}

cLuaFunctionCommand::cLuaFunctionCommand(const pLuaString cap, ::luabind::object& function, const pLuaString shortExplanation):function_(function)
{
	LuaStringTransformer capStr(cap), shortExplanationStr(shortExplanation);

	caption = g_Lang(capStr.t_str().c_str());
	shortExplanation_ = g_Lang(shortExplanationStr.t_str().c_str());
}


cLuaFunctionCommand::~cLuaFunctionCommand(void)
{
	//menuをだしたまま終了するとfunction_のhandleで二重開放が起こる
}

int cLuaFunctionCommand::Action(cRenderDevice *pDev)
{
	if(::luabind::type(function_) == LUA_TFUNCTION);
	{
		try
		{
			::luabind::call_function<void>(function_, freeValiable);
		}
		catch(luabind::error const& e)
		{
			return false;
		}
	}
	return true;
}