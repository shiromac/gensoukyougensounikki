


#include <common.h>
#include "LuaStringUtility.h"



errno_t setLuaString(tstring* uniString, const pLuaString lua_text, const int lua_length)
{
	errno_t error = 0;
	size_t return_text_length = 0;


#ifdef UNICODE
	TCHAR* tstr_adress = new TCHAR[lua_length];

	error |= mbstowcs_s(&return_text_length,
						tstr_adress,
						strlen(lua_text)+1,
						lua_text,
						lua_length);
	(*uniString).assign(tstr_adress,return_text_length-1);

	RELEASE_ARRAY(tstr_adress);
#else
	(*uniString).assign(lua_text,strlen(lua_text)+1);
#endif

	return error;
};


//ÉÅÉÇÉäämï€ÇµÇ»Ç¢
errno_t getLuaString(const tstring& uniString, pLuaString* output, const int lua_length)
{
	errno_t error = 0;
	size_t return_text_length = 0;

#ifdef UNICODE
	error |= wcstombs_s(&return_text_length,
						*output,
						uniString.length()+1,
						uniString.c_str(),
						lua_length);
#else
	strcpy(*output,uniString.c_str());
#endif
	return error;
};

//---------------------------------------------------------------------
LuaStringTransformer::LuaStringTransformer(const pLuaString pLuastr)
:luastr_size_(-1),tstring_enable_(false),pluastring_(NULL)
{
	luastr_size_ = strlen(pLuastr)+1;
	pluastring_ = new LuaString[luastr_size_];
	strcpy(pluastring_,pLuastr);
}
LuaStringTransformer::LuaStringTransformer(const tstring tstr)
:luastr_size_(-1),tstring_enable_(false),pluastring_(NULL)
{
	tstring_str_ = tstr;
	tstring_enable_ = true;
}
LuaStringTransformer::~LuaStringTransformer(void)
{
	RELEASE_ARRAY(pluastring_);
}
const pLuaString LuaStringTransformer::lua_str()
{
	if(luastr_size_ < 0)
	{
		luastr_size_ = tstring_str_.size()+1;
		pluastring_ = new LuaString[luastr_size_];
		getLuaString(tstring_str_, &pluastring_, luastr_size_);
	}
	return pluastring_;
}
const tstring& LuaStringTransformer::t_str()
{
	if(tstring_enable_ == false)
	{
		setLuaString(&tstring_str_, pluastring_, luastr_size_);
		tstring_enable_ = true;
	}
	return tstring_str_;
}

tstring luaString2tstring(const pLuaString lua_text)
{
	return LuaStringTransformer(lua_text).t_str();
}
const ::std::basic_string<LuaString> tString2luastring(const tstring& t_text)
{
	return ::std::basic_string<LuaString>(LuaStringTransformer(t_text).lua_str());
}