#pragma once

//tstringíËã`
#include <tstring_ph.h>

#define pLuaString char*
#define LuaString char

//


errno_t setLuaString(tstring* uniString, const pLuaString lua_text, const int lua_length);

//ÉÅÉÇÉäämï€ÇµÇ»Ç¢
errno_t getLuaString(const tstring& uniString, pLuaString* output, const int lua_length);

class LuaStringTransformer
{
public:
	LuaStringTransformer(const pLuaString pLuastr);
	LuaStringTransformer(const tstring tstr);
	~LuaStringTransformer(void);
private:
	pLuaString pluastring_;
	int luastr_size_;
	tstring tstring_str_;
	bool tstring_enable_;
public:
	const pLuaString lua_str();
	const tstring& t_str();
};

tstring luaString2tstring(const pLuaString lua_text);
const ::std::basic_string<LuaString> tString2luastring(const tstring& t_text);