#pragma once


#include "LuaStringUtility.h"

#include <boost/tokenizer.hpp>


typedef boost::char_separator<TCHAR> tchar_separator;
typedef boost::tokenizer<tchar_separator, tstring::const_iterator, tstring> tTokenizer;

//----------------------------------------------------------
//strURL class
class LuaStringURL
{
public:
	LuaStringURL(const pLuaString lua_text);

	~LuaStringURL();
private:


    pLuaString URL_;
public:

	static const int LUASTRINGURL_MAXSIZE = (256);

	static const int maxStringSize() {return LUASTRINGURL_MAXSIZE;};

	void setURL(const pLuaString lua_text);

	tTokenizer getLocation(tstring* transrated_pass_str) const;
	/*
	void showURL() {
		if(URL_)
		{
			std::cout << URL_ << std::endl;
		}
    }
	*/
};

