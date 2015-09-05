
#include "LuaStringURL.h"

#include <common.h>


//--------------------------------------
LuaStringURL::LuaStringURL(const pLuaString lua_text):URL_(NULL)
{
	setURL(lua_text);
};

LuaStringURL::~LuaStringURL()
{
	if(URL_ != NULL)
	{
		RELEASE_ARRAY(URL_);
	}
};

void LuaStringURL::setURL(const pLuaString lua_text)
{
	if(URL_ != NULL)
	{
		RELEASE_ARRAY(URL_);
	}

	URL_ = new char[maxStringSize()];

	strcpy_s(URL_, maxStringSize(), lua_text);
};
//transrated_pass_str‚Ímemory•ÛŠÇ—p‚ð‚©‚Ë‚é
tTokenizer LuaStringURL::getLocation(tstring* transrated_pass_str) const
{
	setLuaString(transrated_pass_str, URL_, maxStringSize());
	if(*--transrated_pass_str->end() == '\0')
	{
		transrated_pass_str->erase(--transrated_pass_str->end());
	}
	tchar_separator sep(_T("/\\"), NULL, boost::drop_empty_tokens);
	return tTokenizer(*transrated_pass_str, sep);
}
