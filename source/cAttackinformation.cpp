#include "cAttackinformation.h"


cAttackinformation::cAttackinformation(void)
{
	valiable.doubles.dim(変数_攻撃力) = 1.0;
	valiable.doubles.dim(変数_攻撃力ボーナス_倍率) = 1.0;
	valiable.doubles.dim(変数_定数ダメージフラグ) = false;

	valiable.intsets.dim(変数_属性);
}

cAttackinformation::~cAttackinformation(void)
{

}

pcAttackinformation 攻撃作成(pcCharacter attacker, pcLandform Landplase, double attackPower, int damageflag, multiset<攻撃属性::攻撃属性> attribute, int surelyhitflag)
{
	pcAttackinformation patinfo = pcAttackinformation(new cAttackinformation);

	patinfo->attacker = attacker;
	patinfo->defenser = NULLCHARA;
	patinfo->Landplase = Landplase;


	if(damageflag == 定数ダメージフラグ || damageflag == 通常ダメージフラグ)
	{
		patinfo->valiable.doubles.dim(変数_攻撃力);
		patinfo->valiable.doubles.dim(変数_定数ダメージフラグ);
		patinfo->valiable.doubles.dim(変数_割合ダメージフラグ);
		patinfo->valiable.doubles[変数_攻撃力] = attackPower;
		patinfo->valiable.doubles[変数_定数ダメージフラグ] = (damageflag == 定数ダメージフラグ);
		patinfo->valiable.doubles[変数_割合ダメージフラグ] = FALSE;
	}
	else if(damageflag == 割合ダメージフラグ)
	{
		patinfo->valiable.doubles.dim(変数_割合ダメージ);
		patinfo->valiable.doubles.dim(変数_定数ダメージフラグ);
		patinfo->valiable.doubles.dim(変数_割合ダメージフラグ);
		patinfo->valiable.doubles[変数_割合ダメージ] = attackPower;
		patinfo->valiable.doubles[変数_割合ダメージフラグ] = TRUE;
		patinfo->valiable.doubles[変数_定数ダメージフラグ] = FALSE;
	}

	patinfo->valiable.intsets[変数_属性] = 属性設定(attribute);
	if(surelyhitflag)
	{//必中フラグ
		patinfo->valiable.intsets[変数_属性].insert(攻撃属性::必中);
	}


	return patinfo;
}
/*
multiset<int>& 属性設定()
{
	multiset<int> attr;
	return attr;
}
*/
multiset<int> 属性設定(int i1)
{
	multiset<int> attr;
	attr.insert(i1);
	return attr;
}
multiset<int> 属性設定(int i1,int i2)
{
	multiset<int> attr;
	attr.insert(i1);
	attr.insert(i2);
	return attr;
}
multiset<int> 属性設定(int i1,int i2,int i3)
{
	multiset<int> attr;
	attr.insert(i1);
	attr.insert(i2);
	attr.insert(i3);
	return attr;
}
multiset<int> 属性設定(int i1,int i2,int i3,int i4)
{
	multiset<int> attr;
	attr.insert(i1);
	attr.insert(i2);
	attr.insert(i3);
	attr.insert(i4);
	return attr;
}
multiset<int> 属性設定(int i1,int i2,int i3,int i4,int i5)
{
	multiset<int> attr;
	attr.insert(i1);
	attr.insert(i2);
	attr.insert(i3);
	attr.insert(i4);
	attr.insert(i5);
	return attr;
}
multiset<int> 属性設定(int i1,int i2,int i3,int i4,int i5,int i6)
{
	multiset<int> attr;
	attr.insert(i1);
	attr.insert(i2);
	attr.insert(i3);
	attr.insert(i4);
	attr.insert(i5);
	attr.insert(i6);
	return attr;
}
multiset<int> 属性設定(int i1,int i2,int i3,int i4,int i5,int i6,int i7)
{
	multiset<int> attr;
	attr.insert(i1);
	attr.insert(i2);
	attr.insert(i3);
	attr.insert(i4);
	attr.insert(i5);
	attr.insert(i6);
	attr.insert(i7);
	return attr;
}
multiset<int> 属性設定(int i1,int i2,int i3,int i4,int i5,int i6,int i7,int i8)
{
	multiset<int> attr;
	attr.insert(i1);
	attr.insert(i2);
	attr.insert(i3);
	attr.insert(i4);
	attr.insert(i5);
	attr.insert(i6);
	attr.insert(i7);
	attr.insert(i8);
	return attr;
}

multiset<int> 属性設定(multiset<攻撃属性::攻撃属性>& intset)
{
	multiset<int> attr;
	multiset<攻撃属性::攻撃属性>::iterator itr = intset.begin();
	for(;itr!=intset.end();itr++)
	{
		attr.insert(*itr);
	}
	return attr;
}
multiset<攻撃属性::攻撃属性> 属性設定back(multiset<int>& intset)
{
	multiset<攻撃属性::攻撃属性> attr;
	multiset<int>::iterator itr = intset.begin();
	for(;itr!=intset.end();itr++)
	{
		attr.insert((攻撃属性::攻撃属性)*itr);
	}
	return attr;
}

multiset<int> 属性設定()
{
	multiset<int> attr;
	return attr;
}
set<tstring> 属性設定(tstring i1)
{
	set<tstring> attr;
	attr.insert(i1);
	return attr;
}
set<tstring> 属性設定(tstring i1,tstring i2)
{
	set<tstring> attr;
	attr.insert(i1);
	attr.insert(i2);
	return attr;
}
set<tstring> 属性設定(tstring i1,tstring i2,tstring i3)
{
	set<tstring> attr;
	attr.insert(i1);
	attr.insert(i2);
	attr.insert(i3);
	return attr;
}
set<tstring> 属性設定(tstring i1,tstring i2,tstring i3,tstring i4)
{
	set<tstring> attr;
	attr.insert(i1);
	attr.insert(i2);
	attr.insert(i3);
	attr.insert(i4);
	return attr;
}
set<tstring> 属性設定(tstring i1,tstring i2,tstring i3,tstring i4,tstring i5)
{
	set<tstring> attr;
	attr.insert(i1);
	attr.insert(i2);
	attr.insert(i3);
	attr.insert(i4);
	attr.insert(i5);
	return attr;
}
set<tstring> 属性設定(tstring i1,tstring i2,tstring i3,tstring i4,tstring i5,tstring i6)
{
	set<tstring> attr;
	attr.insert(i1);
	attr.insert(i2);
	attr.insert(i3);
	attr.insert(i4);
	attr.insert(i5);
	attr.insert(i6);
	return attr;
}
set<tstring> 属性設定(tstring i1,tstring i2,tstring i3,tstring i4,tstring i5,tstring i6,tstring i7)
{
	set<tstring> attr;
	attr.insert(i1);
	attr.insert(i2);
	attr.insert(i3);
	attr.insert(i4);
	attr.insert(i5);
	attr.insert(i6);
	attr.insert(i7);
	return attr;
}
set<tstring> 属性設定(tstring i1,tstring i2,tstring i3,tstring i4,tstring i5,tstring i6,tstring i7,tstring i8)
{
	set<tstring> attr;
	attr.insert(i1);
	attr.insert(i2);
	attr.insert(i3);
	attr.insert(i4);
	attr.insert(i5);
	attr.insert(i6);
	attr.insert(i7);
	attr.insert(i8);
	return attr;
}
