#pragma once

#include <set>
#include <map>
#include <vector>
#include <tstring_ph.h>
#include <LuaUtility/LuaStringUtility.h>
#include "StyleString.h"

class cCharacter;
class cDroping;
class cLandform;
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cCharacter> pcCharacter;
typedef boost::shared_ptr<cDroping> pcDroping;
typedef boost::shared_ptr<cLandform> pcLandform;
#define NULLLAND pcLandform((cLandform*)NULL)

enum 変数インデックス
{
	変数_攻撃力,
	変数_攻撃力ボーナス_倍率,
	変数_攻撃力ボーナス_定数,
	変数_武器力基礎値ボーナス_倍率,
	変数_武器力基礎値ボーナス_定数,
	変数_武器力ボーナス_倍率,
	変数_武器力ボーナス_定数,
	変数_防御力,
	変数_防御力ボーナス_倍率,
	変数_防御力ボーナス_定数,
	変数_防具力基礎値ボーナス_倍率,
	変数_防具力基礎値ボーナス_定数,
	変数_防具力ボーナス_倍率,
	変数_防具力ボーナス_定数,
	変数_耐性ボーナス_倍率％,
	変数_防御効果ありフラグ,
	変数_ダメージボーナス_定数,
	変数_ダメージキャップ,
	変数_ダメージ,
	変数_定数ダメージフラグ,
	変数_割合ダメージフラグ,
	変数_割合ダメージ,
	変数_回復力ボーナス_倍率,
	変数_汎用ボーナス_倍率,
	変数_汎用ボーナス_定数,
	変数_命中力,
	変数_直接攻撃フラグ,
	変数_回避力,
	変数_強度,
	変数_ターン,
	変数_方向,
	変数_貫通,
	変数_距離,
	変数_属性,
	変数_属性倍率,
	変数_挿入コマンド,
	変数_挿入コマンド位置,
	変数_死亡回避,
	変数_フェイズ,
	変数_異常状態,

	変数_攻撃者,
	変数_防御者,
	変数_移動者,
	変数_対象者,
	変数_攻撃場所,
	変数_移動場所,
	変数_対象落ち物,

	変数_汎用ブール,
	変数_汎用実数,
	変数_汎用実数2,
	変数_汎用不変定数,//変えないでほしい場合
	変数_汎用不変定数2,
};


template <typename TYPE>
class cValiableFieldChip
{
public:
	cValiableFieldChip(void)
	{
		
	};
public:
	virtual ~cValiableFieldChip(void)
	{
		clear();
	};


	virtual void clear()
	{
		stringmap_.clear();
		intmap_.clear();
	};

	//cValiableField& operator =(const cValiableField& rother);//=演算子

	//変数を定義
	TYPE& dim(tstring& name)
	{
		return dim(name.c_str());
	};
	TYPE& dim(const TCHAR* name)
	{
		TYPE data;
		stringmap_.insert( pair<tstring, TYPE>( name, data ) );
		return stringmap_[name];
	};
	TYPE& dim(const pLuaString name)
	{
		TYPE data;
		tstring name_tstr(luaString2tstring(name));
		stringmap_.insert( pair<tstring, TYPE>( luaString2tstring(name), data ) );
		return stringmap_[name_tstr];
	};
	TYPE& dim(変数インデックス name)
	{
		TYPE data;
		intmap_.insert( pair<変数インデックス, TYPE>( name, data ) );
		return intmap_[name];
	};

	//変数を定義
	TYPE& dim(tstring& name, TYPE& data)
	{
		return dim(name.c_str(), data);
	};
	TYPE& dim(const TCHAR* name, TYPE& data)
	{
		stringmap_.insert( pair<tstring, TYPE>( name, data ) );
		return stringmap_[name];
	};
	TYPE& dim(const pLuaString name, TYPE& data)
	{
		stringmap_.insert( pair<tstring, TYPE>( luaString2tstring(name), data ) );
		return stringmap_[name];
	};
	TYPE& dim(変数インデックス name, TYPE& data)
	{
		intmap_.insert( pair<変数インデックス, TYPE>( name, data ) );
		return intmap_[name];
	};

	//[]演算子。参照を返す。
	TYPE& operator [](tstring& name)
	{
		return val(name.c_str());
	};
	TYPE& operator [](const TCHAR* name)
	{
		return val(name);
	};
	TYPE& operator [](変数インデックス name)
	{
		return val(name);
	};

	//参照を返す。
	TYPE& val(const TCHAR* name)
	{
		std::map<tstring, TYPE>::iterator itr;
		itr = stringmap_.find(name);
		if(itr == stringmap_.end())
		{//見つからない（エラー）
			MessageBox(NULL,_T("不明な変数が参照されました"),name,MB_OK);
			return dumy;
		}
		return itr->second;
	};
	//参照を返す。
	TYPE& val(変数インデックス name)
	{
		std::map<変数インデックス, TYPE>::iterator itr;
		itr = intmap_.find(name);
		if(itr == intmap_.end())
		{//見つからない（エラー）
			MessageBox(NULL,_T("不明な変数が参照されました"),setStyle((int)name).c_str(),MB_OK);
			
			return dumy;
		}
		return itr->second;
	};
	//参照を返す。
	TYPE& val(const pLuaString name)
	{
		std::map<tstring, TYPE>::iterator itr;
		tstring name_tstr(luaString2tstring(name));
		itr = stringmap_.find(name_tstr);
		if(itr == stringmap_.end())
		{//見つからない（エラー）
			MessageBox(NULL,_T("不明な変数が参照されました"),setStyle(name_tstr).c_str(),MB_OK);
			
			return dumy;
		}
		return itr->second;
	};


	//存在するかどうかを返す。
	bool exist(tstring& name)
	{
		return exist(name.c_str());
	};
	bool exist(const TCHAR* name)
	{
		std::map<tstring, TYPE>::iterator itr;
		itr = stringmap_.find(name);
		return (itr != stringmap_.end());
	};
	bool exist(const pLuaString name)
	{
		std::map<tstring, TYPE>::iterator itr;
		itr = stringmap_.find(luaString2tstring(name));
		return (itr != stringmap_.end());
	};

	bool exist(変数インデックス name)
	{
		std::map<変数インデックス, TYPE>::iterator itr;
		itr = intmap_.find(name);
		return (itr != intmap_.end());
	};

	std::map<tstring, TYPE>& ValiableStringMap() {return stringmap_;};
	std::map<変数インデックス, TYPE>& ValiableIntMap() {return intmap_;};
private:
	std::map<tstring, TYPE> stringmap_;
	std::map<変数インデックス, TYPE> intmap_;
	TYPE dumy;
};

class cValiableField
{
public:
	cValiableField(void);
public:
	virtual ~cValiableField(void);

	virtual void clear()
	{
		doubles.clear();
		charas.clear();
		drops.clear();
		lands.clear();
		strsets.clear();
		intsets.clear();
		strvectors.clear();
		intvectors.clear();
	};
	/*
	//double系

	//cValiableField& operator =(const cValiableField& rother);//=演算子

	//変数を定義
	void dim(tstring& name);
	void dim(const TCHAR* name);

	//[]演算子。参照を返す。
	double& operator [](tstring& name);
	double& operator [](const TCHAR* name);
	//参照を返す。
	double& val(const TCHAR* name);
	
	//存在するかどうかを返す。
	bool exist(tstring& name);
	bool exist(const TCHAR* name);
	*/
private:
	/*
	std::map<tstring, double> doubles_;
	double dumyDouble;
	*/
public:
	cValiableFieldChip<double> doubles;
	cValiableFieldChip<tstring> tstrings;
	cValiableFieldChip<StyleString> styleStrings;
	
public:
	cValiableFieldChip<pcCharacter> charas;
	cValiableFieldChip<pcDroping> drops;
	cValiableFieldChip<pcLandform> lands;

	//set系
public:
	//変数を定義
	/*
	void set_dim(tstring& naem);
	void set_dim(const TCHAR* name);

	//[]演算子。参照を返す。
	double& operator [](tstring& name);
	double& operator [](const TCHAR* name);
	//参照を返す。
	double& val(const TCHAR* name);
	
	//存在するかどうかを返す。
	bool exist(tstring& name);
	bool exist(const TCHAR* name);
	*/
private:
	//std::map<tstring, set<tstring>> doubles_;
	//double dumyDouble;
public:
	cValiableFieldChip<std::set<tstring>> strsets;
	cValiableFieldChip<std::multiset<int>> intsets;

	cValiableFieldChip<std::vector<tstring>> strvectors;
	cValiableFieldChip<std::vector<int>> intvectors;
};


