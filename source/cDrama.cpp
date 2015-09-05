
#include "cDrama.h"
#include "utility\LuaUtility\LuaEnvironment.h"


#include "debug.h"
#include <tchar.h>
#include <tstring_ph.h>

cDrama::cDrama(luabind::object& dramaThread):dramaThread_(dramaThread)
{
	if(::luabind::type(dramaThread_) != LUA_TTHREAD)
	{
		state_ = state_error;
	}
	else
	{
		state_ = state_ready;
	}
}

cDrama::~cDrama()
{
}


bool cDrama::dramaThread_resume()
{
	/*
	if(lua_type(dramaThread_.interpreter(), -1) != LUA_TTHREAD) {
		OnAssert(_T(__FILE__),__LINE__,false,_T("coroutineのthreadが壊れています。"));
		return true;//バグってるとき強制終了
	}
	*/
	dramaThread_.push(dramaThread_.interpreter());
	lua_State * pThread = lua_tothread(dramaThread_.interpreter(), -1);
	lua_pop(dramaThread_.interpreter(), 1);
	try
	{
		return luabind::resume<bool>(pThread);
	}
	catch(luabind::error const& e)
	{
		lua_error_handler(dramaThread_.interpreter(), (tstring)_T("error_DramaThread.txt"));
		OnAssert(_T(__FILE__),__LINE__,false,_T("dramaThread 内で例外が発生しました 。　error. in DramaThread. ") );
		return true;
	}
	return true;
}

bool cDrama::doDrama()
{
	bool return_value = false;
	if(state_ == state_ready)
	{
		return_value = dramaThread_resume();
		state_ = state_doing;
	}
	else if(state_ == state_doing)
	{
		return_value = dramaThread_resume();
	}
	else
	{
		return_value = true;
	}

	if(return_value == true)
	{
		state_ = state_finished;
	}

	return return_value;
}

bool cDrama::isError()
{
	return (state_ == state_error);
}