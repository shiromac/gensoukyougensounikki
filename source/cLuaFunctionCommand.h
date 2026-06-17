#pragma once
#include "cCommand.h"
#include <boost/shared_ptr.hpp>
#include "utility/LuaUtility/LuaStringUtility.h"

#include <luabind/luabind.hpp>

class LuaEnvironment;
typedef boost::shared_ptr<LuaEnvironment> pLuaEnvironment;

class cLuaFunctionCommand :
	public cCommand
{
public:
	//cLuaFunctionCommand(StyleString cap, pLuaEnvironment luaEnv, const tstring functionName, StyleString shortExplanation);
	//cLuaFunctionCommand(StyleString cap, pLuaEnvironment luaEnv, const pLuaString functionName, StyleString shortExplanation);
	cLuaFunctionCommand(StyleString cap, ::luabind::object& function, StyleString shortExplanation);
	cLuaFunctionCommand(const pLuaString cap, ::luabind::object& function, const pLuaString shortExplanation);

public:
	virtual ~cLuaFunctionCommand(void);

protected:
	//pLuaEnvironment luaEnv_;
	//LuaStringTransformer functionName_;
	StyleString shortExplanation_;
	::luabind::object function_;
public:
	//pLuaEnvironment luaEnv(){ return luaEnv_; };
	//void setLuaEnv(pLuaEnvironment luaEnv){ luaEnv_ = luaEnv; };

	//const pLuaString functionName(){ return functionName_.lua_str(); };

	std::map<tstring, luabind::object> freeValiable;
public:
//overloads
	virtual int Action(cRenderDevice *pDev);

	virtual StyleString shortExplanationText(){return shortExplanation_;};

	virtual void resetCaption(){};
	//ショートカットに入れても良いかどうか
	virtual bool canIntoShortCut(){return false;};
};