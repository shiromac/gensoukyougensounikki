#pragma once

class cAttackinformation;


#include "cLandform.h"
#include "cCharacter.h"

#include "utility\valiableField\cValiableField.h"

#include <deque>
#include <map>
#include <vector>
#include <list>
#include <set>
using namespace std;


//#define ATTACKINFO_FLAGMAX 128
namespace 攻撃属性
{

	enum 攻撃属性
	{
		無効エラー = 0,
		アイテム破壊,
		必中,
		敵即死,
		//攻撃属性
		爆発 = 100,
		爆 = 100,
		衝 = 100,
		火 = 101,
		水 = 102,
		冷気 = 103,
		冷 = 103,
		気 = 104,
		電気 = 105,
		雷 = 105,
		//システム属性
		毒 = 200,
		貫通,
		魔法,
		落ち物,
		跳ね返し無効,
		ATTACKATRI_NUM = 19,
	};
};


class cAttackinformation
{
public:
	cAttackinformation(void);
public:
	virtual ~cAttackinformation(void);

	pcCharacter attacker;
	pcCharacter defenser;

	pcLandform Landplase;
	
	cValiableField valiable;

	//int attackPower;
	//double attackbonus_multi;
	//bool constattack;


	//valiableに設定されるもの
	//変数_定数ダメージフラグ
	//変数_割合ダメージフラグ
	//変数_攻撃力
	//変数_攻撃力ボーナス_倍率;

	//set<int> attribute;

};
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cAttackinformation> pcAttackinformation;

const int 通常ダメージフラグ = 0;
const int 定数ダメージフラグ = 1;
const int 割合ダメージフラグ = 2;
pcAttackinformation 攻撃作成(pcCharacter attacker, pcLandform Landplase, double attackPower, int damageflag, multiset<攻撃属性::攻撃属性> attribute, int surelyhitflag);

//multiset<int>& 属性設定();
multiset<int> 属性設定();
multiset<int> 属性設定(int i1);
multiset<int> 属性設定(int i1,int i2);
multiset<int> 属性設定(int i1,int i2,int i3);
multiset<int> 属性設定(int i1,int i2,int i3,int i4);
multiset<int> 属性設定(int i1,int i2,int i3,int i4,int i5);
multiset<int> 属性設定(int i1,int i2,int i3,int i4,int i5,int i6);
multiset<int> 属性設定(int i1,int i2,int i3,int i4,int i5,int i6,int i7);
multiset<int> 属性設定(int i1,int i2,int i3,int i4,int i5,int i6,int i7,int i8);

multiset<int> 属性設定(multiset<攻撃属性::攻撃属性>& intset);
multiset<攻撃属性::攻撃属性> 属性設定back(multiset<int>& intset);

set<tstring> 属性設定(tstring i1);
set<tstring> 属性設定(tstring i1,tstring i2);
set<tstring> 属性設定(tstring i1,tstring i2,tstring i3);
set<tstring> 属性設定(tstring i1,tstring i2,tstring i3,tstring i4);
set<tstring> 属性設定(tstring i1,tstring i2,tstring i3,tstring i4,tstring i5);
set<tstring> 属性設定(tstring i1,tstring i2,tstring i3,tstring i4,tstring i5,tstring i6);
set<tstring> 属性設定(tstring i1,tstring i2,tstring i3,tstring i4,tstring i5,tstring i6,tstring i7);
set<tstring> 属性設定(tstring i1,tstring i2,tstring i3,tstring i4,tstring i5,tstring i6,tstring i7,tstring i8);
