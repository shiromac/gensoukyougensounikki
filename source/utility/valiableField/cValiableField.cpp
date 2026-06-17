#include "cValiableField.h"
#ifndef __EMSCRIPTEN__
#include <windows.h>
#endif

using namespace std;

cValiableField::cValiableField(void)
{

}

cValiableField::~cValiableField(void)
{

}

/*
double& cValiableField::operator [](tstring& name)//[]演算子
{
	return doubles_.val(name.c_str());
}
double& cValiableField::operator [](const TCHAR* name)//[]演算子
{
	return doubles_.val(name);
}
double& cValiableField::val(const TCHAR* name)
{
	return doubles_.val(name);
}
*/
/*
cValiableField& cValiableField::operator =(const cValiableField& rother)//=演算子
{

	return *this;
}
*/
/*
void cValiableField::dim(tstring& name)
{
	doubles_.dim(name.c_str());
};
void cValiableField::dim(const TCHAR* name)
{
	doubles_.dim(name);
}

//存在するかどうかを返す。
bool cValiableField::exist(tstring& name)
{
	return doubles_.exist(name.c_str());
}
bool cValiableField::exist(const TCHAR* name)
{
	return doubles_.exist(name);
}
*/
/*
template <typename TYPE>
cValiableFieldChip<TYPE>::cValiableFieldChip<TYPE>(void)
{

}

template <typename TYPE>
cValiableFieldChip<TYPE>::~cValiableFieldChip<TYPE>(void)
{
	doubles_.clear();

}

template <typename TYPE>
TYPE& cValiableFieldChip<TYPE>::operator [](tstring& name)//[]演算子
{
	return val(name.c_str());
}

template <typename TYPE>
TYPE& cValiableFieldChip<TYPE>::operator [](const TCHAR* name)//[]演算子
{
	return val(name);
}

template <typename TYPE>
TYPE& cValiableFieldChip<TYPE>::val(const TCHAR* name)
{
	std::map<tstring, TYPE>::iterator itr;
	itr = doubles_.find(name);
	if(itr == doubles_.end())
	{//見つからない（エラー）
		MessageBox(NULL,_T("不明な変数が参照されました"),name,MB_OK);
		return dumy;
	}
	return itr->second;
}
*/
/*
cValiableFieldChip<TYPE>& cValiableFieldChip<TYPE>::operator =(const cValiableFieldChip<TYPE>& rother)//=演算子
{

	return *this;
}
*/
/*
template <typename TYPE>
TYPE& cValiableFieldChip<TYPE>::dim(tstring& name)
{
	return dim(name.c_str());
}
template <typename TYPE>
TYPE& cValiableFieldChip<TYPE>::dim(const TCHAR* name)
{
	TYPE data;
	doubles_.insert( pair<tstring, TYPE>( name, data ) );
	return doubles_[name];
}

//存在するかどうかを返す。
template <typename TYPE>
bool cValiableFieldChip<TYPE>::exist(tstring& name)
{
	return exist(name.c_str());
}

template <typename TYPE>
bool cValiableFieldChip<TYPE>::exist(const TCHAR* name)
{
	std::map<tstring, TYPE>::iterator itr;
	itr = doubles_.find(name);
	return (itr != doubles_.end());
}
*/

