#pragma once

#include <string>

namespace ATLcompati{


class CStringW
{
public:
	CStringW(const char* str);
//	CStringW(const wchar_t* wstr);
	~CStringW();
private:
	std::wstring str_;
public:
	std::wstring& GetString();
};


};//namespace