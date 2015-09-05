
//#include <OX/String/Traits.hpp>
#include "CString.h"
#include "../LuaUtility/LuaStringUtility.h"

#include <atlstr.h>
namespace ATLcompati{

CStringW::CStringW(const char* str)
{
	//OX::String::
	ATL::CStringW cstr(str);
	str_.assign(cstr.GetString());
	//setLuaString(&str_, str, strlen(str)+1);
};
/*
CStringW::CStringW(const wchar_t* wstr)
{
	
};
*/
CStringW::~CStringW()
{

};
std::wstring& CStringW::GetString()
{
	return str_;
}

}