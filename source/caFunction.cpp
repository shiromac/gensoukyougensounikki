#include "stdafx.h"
#include "debug.h"

#include "caFunction.h"
#include <luabind/luabind.hpp>
caFunction::caFunction()
{
	End = false;
	count = 0;
	wait = 0;
	pFunc = NULL;
}
caFunction::caFunction(void (*function)())
{
	End = false;
	count = 0;
	wait = 0;
	pFunc = function;
}

caFunction::~caFunction()
{
}

int caFunction::Init()
{
	return true;
}

int caFunction::process(IDirect3DDevice9 *pDev)
{
	//---------------------------------------
	//	checkpointに始点（中間点）終点の順にいれてから処理を行う
	//---------------------------------------
	count += 1;
	


	if( !End &&  count <= wait)
	{

	}
	else
	{
		pFunc();
		End = true;
	}


	
	return true;
}


caPAnimeAdd::caPAnimeAdd()
{
	_critical = false;
	End = false;
	count = 0;
	wait = 0;
}
caPAnimeAdd::caPAnimeAdd(bool critical)
{
	_critical = critical;
	End = false;
	count = 0;
	wait = 0;
}
caPAnimeAdd::~caPAnimeAdd()
{
}
int caPAnimeAdd::Init()
{
	return true;
}

int caPAnimeAdd::process(IDirect3DDevice9 *pDev)
{
	//---------------------------------------
	//	checkpointに始点（中間点）終点の順にいれてから処理を行う
	//---------------------------------------
	count += 1;
	


	if( !End &&  count <= wait)
	{

	}
	else
	{
		if(_critical) {
			sg_pDungeonSystem->AnimationManager().AddAnime_realtimeCritical(paddanime);
		}
		else {
			sg_pDungeonSystem->AnimationManager().AddAnime_realtime(paddanime);
		}
		End = true;
	}


	
	return true;
}

//--------------------------------------------------------------

caLuaFunction::caLuaFunction(luabind::object& function, int wait)
{
	baseInitation(function, wait);
}
caLuaFunction::caLuaFunction(luabind::object& function, luabind::object& val1, int wait)
{
	baseInitation(function, wait);
	valiables_.push_back(val1);
}
caLuaFunction::caLuaFunction(luabind::object& function, luabind::object& val1, luabind::object& val2, int wait)
{
	baseInitation(function, wait);
	valiables_.push_back(val1);
	valiables_.push_back(val2);
}
caLuaFunction::caLuaFunction(luabind::object& function, luabind::object& val1, luabind::object& val2, luabind::object& val3, int wait)
{
	baseInitation(function, wait);
	valiables_.push_back(val1);
	valiables_.push_back(val2);
	valiables_.push_back(val3);
}
caLuaFunction::caLuaFunction(luabind::object& function, luabind::object& val1, luabind::object& val2, luabind::object& val3, luabind::object& val4, int wait)
{
	baseInitation(function, wait);
	valiables_.push_back(val1);
	valiables_.push_back(val2);
	valiables_.push_back(val3);
	valiables_.push_back(val4);
}
caLuaFunction::caLuaFunction(luabind::object& function, luabind::object& val1, luabind::object& val2, luabind::object& val3, luabind::object& val4,
	luabind::object& val5, int wait)
{
	baseInitation(function, wait);
	valiables_.push_back(val1);
	valiables_.push_back(val2);
	valiables_.push_back(val3);
	valiables_.push_back(val4);
	valiables_.push_back(val5);
}
caLuaFunction::caLuaFunction(luabind::object& function, luabind::object& val1, luabind::object& val2, luabind::object& val3, luabind::object& val4,
	luabind::object& val5, luabind::object& val6, int wait)
{
	baseInitation(function, wait);
	valiables_.push_back(val1);
	valiables_.push_back(val2);
	valiables_.push_back(val3);
	valiables_.push_back(val4);
	valiables_.push_back(val5);
	valiables_.push_back(val6);
}
caLuaFunction::caLuaFunction(luabind::object& function, luabind::object& val1, luabind::object& val2, luabind::object& val3, luabind::object& val4,
	luabind::object& val5, luabind::object& val6, luabind::object& val7, int wait)
{
	baseInitation(function, wait);
	valiables_.push_back(val1);
	valiables_.push_back(val2);
	valiables_.push_back(val3);
	valiables_.push_back(val4);
	valiables_.push_back(val5);
	valiables_.push_back(val6);
	valiables_.push_back(val7);
}
caLuaFunction::caLuaFunction(luabind::object& function, luabind::object& val1, luabind::object& val2, luabind::object& val3, luabind::object& val4,
	luabind::object& val5, luabind::object& val6, luabind::object& val7, luabind::object& val8, int wait)
{
	baseInitation(function, wait);
	valiables_.push_back(val1);
	valiables_.push_back(val2);
	valiables_.push_back(val3);
	valiables_.push_back(val4);
	valiables_.push_back(val5);
	valiables_.push_back(val6);
	valiables_.push_back(val7);
	valiables_.push_back(val8);
}
void caLuaFunction::baseInitation(luabind::object& function, int wait)
{
	End = false;
	count_ = 0;
	wait_ = wait;
	function_ = function;
}
caLuaFunction::~caLuaFunction()
{

}
int caLuaFunction::Init()
{
	return true;
}

int caLuaFunction::process(IDirect3DDevice9 *pDev)
{
	//---------------------------------------
	//	checkpointに始点（中間点）終点の順にいれてから処理を行う
	//---------------------------------------
	count_ += 1;
	

	if( !End &&  count_ <= wait_)
	{

	}
	else
	{

		if(luabind::type(function_) == LUA_TFUNCTION)
		{
			try
			{
				int valiable_num = valiables_.size();
				if(valiable_num == 0)
				{
					luabind::call_function<void>(function_);
				}
				else if(valiable_num == 1)
				{
					luabind::call_function<void>(function_, valiables_[0]);
				}
				else if(valiable_num == 2)
				{
					luabind::call_function<void>(function_, valiables_[0], valiables_[1]);
				}
				else if(valiable_num == 3)
				{
					luabind::call_function<void>(function_, valiables_[0], valiables_[1], valiables_[2]);
				}
				else if(valiable_num == 4)
				{
					luabind::call_function<void>(function_, valiables_[0], valiables_[1], valiables_[2], valiables_[3]);
				}
				else if(valiable_num == 5)
				{
					luabind::call_function<void>(function_, valiables_[0], valiables_[1], valiables_[2], valiables_[3], valiables_[4]);
				}
				else if(valiable_num == 6)
				{
					luabind::call_function<void>(function_, valiables_[0], valiables_[1], valiables_[2], valiables_[3], valiables_[4], valiables_[5]);
				}
				else if(valiable_num == 7)
				{
					luabind::call_function<void>(function_, valiables_[0], valiables_[1], valiables_[2], valiables_[3], valiables_[4], valiables_[5], valiables_[6]);
				}
				else if(valiable_num == 8)
				{
					luabind::call_function<void>(function_, valiables_[0], valiables_[1], valiables_[2], valiables_[3], valiables_[4], valiables_[5], valiables_[6], valiables_[7]);
				}
			}
			catch(luabind::error const& e)
			{
				lua_error_handler(function_.interpreter(), (tstring)_T("error_caLuaFunction.txt"));
				OnAssert(_T(__FILE__),__LINE__,false,_T("caLuaFunction 内で例外が発生しました 。　error. in caLuaFunction."));
			}
			
		}
		else
		{
			OnAssert(_T(__FILE__),__LINE__,false,_T("caLuaFunctionに渡されたLua Objectが関数ではありません。　error. the lua object is not a Function.:"));
		}
		End = true;
	}


	
	return true;
}
