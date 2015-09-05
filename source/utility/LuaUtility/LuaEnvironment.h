#pragma once


#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
#include "LuaStringURL.h"
#include <stdarg.h>

#include <boost/ref.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
class LuaEnvironment;
typedef boost::shared_ptr<LuaEnvironment> pLuaEnvironment;
typedef boost::weak_ptr<LuaEnvironment> wpLuaEnvironment;

//lua_Stateラッピング
class LuaEnvironment
{
public:
	LuaEnvironment(::std::basic_string<LuaString> name);
public:
	virtual ~LuaEnvironment(void);
protected:
	lua_State* luaState_;
protected:
	//固定モジュール
	virtual void luabind_module() = 0;
public:
	lua_State* luaState(){return luaState_;};//特殊用途用
public:
	::std::basic_string<LuaString> name(){return name_;};
protected:
	::std::basic_string<LuaString> name_;
public:
	//インターフェイス
	void init();
	//簡易実行
	int runLuaScriptWithPath(const tstring& path);

	int runLuaScriptWithBuffer(const pLuaString buffer, const size_t size, const pLuaString name);
	int loadLuaScriptWithBuffer(const pLuaString buffer, const size_t size, const pLuaString name);
	int run();

	int outputErrorFile(const tstring& path);
	
	//定数定義
	template <class T> void setGlobalConstValiable(const pLuaString name,const T& val)
	{
		luabind::object lua_global = luabind::globals(luaState_);
		lua_global[name] = val;
	}
	void setGlobalConstValiable(const pLuaString name,const pLuaString val)
	{
		luabind::object lua_global = luabind::globals(luaState_);
		lua_global[name] = val;
	}
public:
	//関数呼び出し//失敗したら false
	bool function(const tstring& functionName) {
		LuaStringTransformer luastr_functionName(functionName);	
		try
		{
			luabind::call_function< void >( luaState_ , luastr_functionName.lua_str() );
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
	template < class valclass1>
		bool function(const tstring& functionName,
			valclass1 val1) {
		LuaStringTransformer luastr_functionName(functionName);	
		try
		{
			luabind::call_function< void >( luaState_ , luastr_functionName.lua_str() ,
				val1);
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
	template < class valclass1, class valclass2>
		bool function(const tstring& functionName,
			valclass1 val1, valclass2 val2) {
		LuaStringTransformer luastr_functionName(functionName);	
		try
		{
			luabind::call_function< void >( luaState_ , luastr_functionName.lua_str() ,
				val1, val2);
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
	template < class valclass1, class valclass2, class valclass3>
		bool function( const tstring& functionName,
			valclass1 val1, valclass2 val2, valclass3 val3) {
		LuaStringTransformer luastr_functionName(functionName);	
		try
		{
			luabind::call_function< void >( luaState_ , luastr_functionName.lua_str() ,
				val1, val2, val3);
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
	template < class valclass1, class valclass2, class valclass3, class valclass4>
		bool function( const tstring& functionName,
			valclass1 val1, valclass2 val2, valclass3 val3, valclass4 val4) {
		LuaStringTransformer luastr_functionName(functionName);	
		try
		{
			luabind::call_function< void >( luaState_ , luastr_functionName.lua_str() ,
				val1, val2, val3, val4);
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
	template < class valclass1, class valclass2, class valclass3, class valclass4, class valclass5>
		bool function(const tstring& functionName,
			valclass1 val1, valclass2 val2, valclass3 val3, valclass4 val4, valclass5 val5) {
		LuaStringTransformer luastr_functionName(functionName);	
		try
		{
			luabind::call_function< void >( luaState_ , luastr_functionName.lua_str() ,
				val1, val2, val3, val4, val5);
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
	
	//関数呼び出し//失敗したら false
	bool function(const pLuaString functionName) {
		try
		{
			luabind::call_function< void >( luaState_ , functionName );
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
	template < class valclass1>
		bool function(const pLuaString functionName,
			valclass1 val1) {
		try
		{
			luabind::call_function< void >( luaState_ , functionName ,
				val1);
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
	template < class valclass1, class valclass2>
		bool function(const pLuaString functionName,
			valclass1 val1, valclass2 val2) {
		try
		{
			luabind::call_function< void >( luaState_ , functionName ,
				val1, val2);
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
	template < class valclass1, class valclass2, class valclass3>
		bool function( const pLuaString functionName,
			valclass1 val1, valclass2 val2, valclass3 val3) {
		try
		{
			luabind::call_function< void >( luaState_ , functionName ,
				val1, val2, val3);
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
	template < class valclass1, class valclass2, class valclass3, class valclass4>
		bool function( const pLuaString functionName,
			valclass1 val1, valclass2 val2, valclass3 val3, valclass4 val4) {
		try
		{
			luabind::call_function< void >( luaState_ , functionName ,
				val1, val2, val3, val4);
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
	template < class valclass1, class valclass2, class valclass3, class valclass4, class valclass5>
		bool function(const pLuaString functionName,
			valclass1 val1, valclass2 val2, valclass3 val3, valclass4 val4, valclass5 val5) {
		try
		{
			luabind::call_function< void >( luaState_ , functionName ,
				val1, val2, val3, val4, val5);
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
public:
	//関数呼び出し//失敗したら false
	template < class retclass> 
		bool function_returnable(const tstring& functionName, retclass& result) {
		LuaStringTransformer luastr_functionName(functionName);	
		try
		{
			result = luabind::call_function< retclass >( luaState_ , luastr_functionName.lua_str() );
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
	template < class retclass, class valclass1>
		bool function_returnable(const tstring& functionName, retclass& result,
			valclass1 val1) {
		LuaStringTransformer luastr_functionName(functionName);	
		try
		{
			result = luabind::call_function< retclass >( luaState_ , luastr_functionName.lua_str() ,
				val1);
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
	template < class retclass, class valclass1, class valclass2>
		bool function_returnable(const tstring& functionName, retclass& result,
			valclass1 val1, valclass2 val2) {
		LuaStringTransformer luastr_functionName(functionName);	
		try
		{
			result = luabind::call_function< retclass >( luaState_ , luastr_functionName.lua_str() ,
				val1, val2);
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
	template < class retclass, class valclass1, class valclass2, class valclass3>
		bool function_returnable(const tstring& functionName, retclass& result,
			valclass1 val1, valclass2 val2, valclass3 val3) {
		LuaStringTransformer luastr_functionName(functionName);	
		try
		{
			result = luabind::call_function< retclass >( luaState_ , luastr_functionName.lua_str() ,
				val1, val2, val3);
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
	template < class retclass, class valclass1, class valclass2, class valclass3, class valclass4>
		bool function_returnable(const tstring& functionName, retclass& result,
			valclass1 val1, valclass2 val2, valclass3 val3, valclass4 val4) {
		LuaStringTransformer luastr_functionName(functionName);	
		try
		{
			result = luabind::call_function< retclass >( luaState_ , luastr_functionName.lua_str() ,
				val1, val2, val3, val4);
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
	template < class retclass, class valclass1, class valclass2, class valclass3, class valclass4, class valclass5>
		bool function_returnable(const tstring& functionName, retclass& result,
			valclass1 val1, valclass2 val2, valclass3 val3, valclass4 val4, valclass5 val5) {
		LuaStringTransformer luastr_functionName(functionName);	
		try
		{
			result = luabind::call_function< retclass >( luaState_ , luastr_functionName.lua_str() ,
				val1, val2, val3, val4, val5);
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
	
	//関数呼び出し//失敗したら false
	template < class retclass> 
		bool function_returnable(const pLuaString functionName, retclass& result) {
		try
		{
			result = luabind::call_function< retclass >( luaState_ , functionName );
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
	template < class retclass, class valclass1>
		bool function_returnable(const pLuaString functionName, retclass& result,
			valclass1 val1) {
		try
		{
			result = luabind::call_function< retclass >( luaState_ , functionName ,
				val1);
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
	template < class retclass, class valclass1, class valclass2>
		bool function_returnable(const pLuaString functionName, retclass& result,
			valclass1 val1, valclass2 val2) {
		try
		{
			result = luabind::call_function< retclass >( luaState_ , functionName ,
				val1, val2);
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
	template < class retclass, class valclass1, class valclass2, class valclass3>
		bool function_returnable( const pLuaString functionName, retclass& result,
			valclass1 val1, valclass2 val2, valclass3 val3) {
		try
		{
			result = luabind::call_function< retclass >( luaState_ , functionName ,
				val1, val2, val3);
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
	template < class retclass, class valclass1, class valclass2, class valclass3, class valclass4>
		bool function_returnable( const pLuaString functionName, retclass& result,
			valclass1 val1, valclass2 val2, valclass3 val3, valclass4 val4) {
		try
		{
			result = luabind::call_function< retclass >( luaState_ , functionName ,
				val1, val2, val3, val4);
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
	template < class retclass, class valclass1, class valclass2, class valclass3, class valclass4, class valclass5>
		bool function_returnable(const pLuaString functionName, retclass& result,
			valclass1 val1, valclass2 val2, valclass3 val3, valclass4 val4, valclass5 val5) {
		try
		{
			result = luabind::call_function< retclass >( luaState_ , functionName ,
				val1, val2, val3, val4, val5);
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};

		/*
public:
	//関数呼び出し//失敗したら false
	bool coroutine_function(const tstring& functionName) {
		LuaStringTransformer luastr_functionName(functionName);	
		try
		{
			luabind::resume_function< void >( luaState_ , luastr_functionName.lua_str() );
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
	template < class valclass1>
		bool coroutine_function(const tstring& functionName,
			valclass1 val1) {
		LuaStringTransformer luastr_functionName(functionName);	
		try
		{
			luabind::resume_function< void >( luaState_ , luastr_functionName.lua_str() ,
				val1);
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
	template < class valclass1, class valclass2>
		bool coroutine_function(const tstring& functionName,
			valclass1 val1, valclass2 val2) {
		LuaStringTransformer luastr_functionName(functionName);	
		try
		{
			luabind::resume_function< void >( luaState_ , luastr_functionName.lua_str() ,
				val1, val2);
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
	template < class valclass1, class valclass2, class valclass3>
		bool coroutine_function( const tstring& functionName,
			valclass1 val1, valclass2 val2, valclass3 val3) {
		LuaStringTransformer luastr_functionName(functionName);	
		try
		{
			luabind::resume_function< void >( luaState_ , luastr_functionName.lua_str() ,
				val1, val2, val3);
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
	template < class valclass1, class valclass2, class valclass3, class valclass4>
		bool coroutine_function( const tstring& functionName,
			valclass1 val1, valclass2 val2, valclass3 val3, valclass4 val4) {
		LuaStringTransformer luastr_functionName(functionName);	
		try
		{
			luabind::resume_function< void >( luaState_ , luastr_functionName.lua_str() ,
				val1, val2, val3, val4);
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
	template < class valclass1, class valclass2, class valclass3, class valclass4, class valclass5>
		bool coroutine_function(const tstring& functionName,
			valclass1 val1, valclass2 val2, valclass3 val3, valclass4 val4, valclass5 val5) {
		LuaStringTransformer luastr_functionName(functionName);	
		try
		{
			luabind::resume_function< void >( luaState_ , luastr_functionName.lua_str() ,
				val1, val2, val3, val4, val5);
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
	
	//関数呼び出し//失敗したら false
	bool coroutine_function(const pLuaString functionName) {
		try
		{
			luabind::resume_function< void >( luaState_ , functionName );
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
	template < class valclass1>
		bool coroutine_function(const pLuaString functionName,
			valclass1 val1) {
		try
		{
			luabind::resume_function< void >( luaState_ , functionName ,
				val1);
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
	template < class valclass1, class valclass2>
		bool coroutine_function(const pLuaString functionName,
			valclass1 val1, valclass2 val2) {
		try
		{
			luabind::resume_function< void >( luaState_ , functionName ,
				val1, val2);
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
	template < class valclass1, class valclass2, class valclass3>
		bool coroutine_function( const pLuaString functionName,
			valclass1 val1, valclass2 val2, valclass3 val3) {
		try
		{
			luabind::resume_function< void >( luaState_ , functionName ,
				val1, val2, val3);
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
	template < class valclass1, class valclass2, class valclass3, class valclass4>
		bool coroutine_function( const pLuaString functionName,
			valclass1 val1, valclass2 val2, valclass3 val3, valclass4 val4) {
		try
		{
			luabind::resume_function< void >( luaState_ , functionName ,
				val1, val2, val3, val4);
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
	template < class valclass1, class valclass2, class valclass3, class valclass4, class valclass5>
		bool coroutine_function(const pLuaString functionName,
			valclass1 val1, valclass2 val2, valclass3 val3, valclass4 val4, valclass5 val5) {
		try
		{
			luabind::resume_function< void >( luaState_ , functionName ,
				val1, val2, val3, val4, val5);
		}
		catch(luabind::error const& e)
		{
			return false;
		}
		return true;
	};
	*/
};

int lua_error_handler(lua_State* L, const tstring& path);
