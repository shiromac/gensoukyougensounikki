#include <common.h>
#include "LuaEnvironment.h"

#include "LuaStringUtility.h"


LuaEnvironment::LuaEnvironment(::std::basic_string<LuaString> name):luaState_(NULL), name_(name)
{

}

LuaEnvironment::~LuaEnvironment(void)
{
	if(NULL != luaState_)
	{
		lua_close(luaState_);
	}
}

void LuaEnvironment::init()
{
	/*
	if(NULL != luaState_)
	{
		lua_close(luaState_);
	}
    luaState_ = luaL_newstate();
    luaL_openlibs(luaState_);

	luabind::open(luaState_);
	luabind_module();
	*/
	if(NULL == luaState_)
	{
		luaState_ = luaL_newstate();
		luaL_openlibs(luaState_);
		luabind::open(luaState_);
		luabind_module();
	}


}
int LuaEnvironment::outputErrorFile(const tstring& path)
{
	if(luaState_)
	{
		return lua_error_handler(luaState_,path);
	}
	return false;
}
	
int LuaEnvironment::runLuaScriptWithPath(const tstring& path)
{
	LuaString luastr_path[LuaStringURL::LUASTRINGURL_MAXSIZE];
	getLuaString(path, (pLuaString*)&luastr_path, LuaStringURL::LUASTRINGURL_MAXSIZE);
	int error = luaL_loadfile(luaState_, luastr_path);
	if(error) return error;
	return run();
}
int LuaEnvironment::runLuaScriptWithBuffer(const pLuaString buffer, const size_t size, const pLuaString name)
{
	int error = loadLuaScriptWithBuffer(buffer, size, name);
	if(error) return error;
	return run();
}
int LuaEnvironment::loadLuaScriptWithBuffer(const pLuaString buffer, const size_t size, const pLuaString name)
{
	int error = luaL_loadbuffer(luaState_, buffer, size, name);
	if(error) return error;
	return 0;
}
int LuaEnvironment::run()
{
	return lua_pcall(luaState_,0,0,0);
}



int lua_error_handler(lua_State* L, const tstring& path)
{
	unsigned flag =  std::ios_base::out | std::ios_base::trunc;
	
    lua_Debug d = {};

	std::stringstream msg;
    // スタックからエラーメッセージを取得する

	const char* errString = lua_tostring(L, -1);
    std::string err;
	if(errString) err = errString;
	else return false;

	#ifdef __EMSCRIPTEN__
	std::basic_ofstream<char> ofs(ggn_tchar_to_path(_LOGFOLDER + path).c_str(), flag);
#else
	std::basic_ofstream<char> ofs((_LOGFOLDER + path).c_str(), flag);
#endif
	

    msg << "ERROR: " << err << "\n\nBacktrace:\n";

    for (int stack_depth = 1; lua_getstack(L, stack_depth, &d); ++stack_depth) {

        lua_getinfo(L, "Sln", &d);

        msg << "#" << stack_depth << " ";

        if (d.name)
            msg << "<" << d.namewhat << "> \"" << d.name << "\"";

        else
            msg << "--";

        msg << " (called";

        if (d.currentline > 0)
            msg << " at line " << d.currentline;

        msg << " in ";
        if (d.linedefined > 0)

            msg << "function block between line " << d.linedefined << ".." << d.lastlinedefined << " of ";

        msg << d.short_src;
        msg << ")\n";
    }

    // スタックに積まれているエラーメッセージを、新しい文字列に置換する。
    lua_pop(L, 1);
    lua_pushstring(L, msg.str().c_str());

    ofs << msg.str() << std::endl;

    return 1;
}

