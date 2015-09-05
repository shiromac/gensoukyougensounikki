#pragma once

#include "cAnimation.h"
#include "cLandform.h"
#include "cDroping.h"

#include "utility/SpriteText.h"
#include "utility/cNumField.h"

#include <vector>
#include <list>
#include <map>
#include <tstring_ph.h>


//今後以下のクラスは使用しない
using namespace std;

//汎用テクスチャアニメクラス
template <class T> 
class cAnimeValue
{
public:
	cAnimeValue(void)
	:maxSum_(0)
	{
		
	};

	virtual ~cAnimeValue(void)
	{
	
	};

public:
	virtual bool empty()
	{
		if(key_.empty()) return true;//空
		else return false;
	};
	virtual void clear()
	{
		key_.clear();
		keytype_.clear();
		maxSum_ = 0.0;
	};
	virtual void getValue(const double value, T& output)
	{
		if(key_.empty()) return;//空
		double realvalue = value - floor(value/maxSum_)*maxSum_;
		map<double, T>::iterator itr;
		itr = key_.lower_bound(realvalue);
		if(itr != key_.begin()) itr--;
		getValue_pri(itr->first,realvalue,keytype_[itr->first],output);
	};
	virtual void setValue(const double range,const T& imput,const int keytype = KEYTYPE_CONST)
	{
		key_.insert(pair<double,T>(maxSum_,imput));
		keytype_.insert(pair<double,int>(maxSum_,keytype));
		maxSum_ += range;
	};
	const static int KEYTYPE_CONST = 0;
	const static int KEYTYPE_LINEAR = 1;
protected:

	virtual void getValue_pri(double keyitr_key, double value, const int keyType, T& output)
	{
		map<double, T>::iterator keyitr = key_.find(keyitr_key);
		double before ,band, per;
		before = keyitr->first;
		map<double, T>::iterator keyitrAfter;
		if((++keyitr) == key_.end())
		{//ラスト
			keyitrAfter = key_.begin();
			band = maxSum_ - before;
		}
		else
		{
			keyitrAfter = keyitr;
			band = keyitrAfter->first - before;
		}
		keyitr--;
		per = (value - before)/band;

		switch(keyType)
		{
		case KEYTYPE_CONST:
			output = keyitr->second;
			break;
		case KEYTYPE_LINEAR:
			output = (1-per)*keyitr->second + (per)*keyitrAfter->second;
			break;
		}
	};
	std::map<double,T> key_;
	std::map<double,int> keytype_;
	double maxSum_;
};

//-------------------------------------------------
//基礎
//-------------------------------------------------
class caEchip_Basic :
	public cAnimationChipManager_chip
{
public:
	caEchip_Basic(void);
public:
	virtual ~caEchip_Basic(void);

	virtual void DrowingObjectFunc();

	virtual int Draw(IDirect3DDevice9 *pDev);

	cDrawingObject DO;

	int life;

	//-------------------------------------
	//[x,y]*[z,t]で計算する。
	//-------------------------------------
	c4DVector ScaleX;
	c4DVector ScaleY;
	c4DVector Rotation;
	c4DVector Rotation2;

	cColor ColorBase;
	c4DVector Alpha;

	c4DVector Centerbegin;
	c4DVector Centerend;
	c4DVector Movepass;

	cAnimeValue<c4DVector> texRange;

	//virtual void copy(const caEchip_Basic & chip);
};
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<caEchip_Basic> pcaEchip_Basic;
//-------------------------------------------------
//数字
//-------------------------------------------------
class caEchip_Num :
	public caEchip_Basic
{
public:
	caEchip_Num(void);
public:
	virtual ~caEchip_Num(void);

	virtual void DrowingObjectFunc();

	virtual int Draw(IDirect3DDevice9 *pDev);

	cNumField num_;

};
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<caEchip_Num> pcaEchip_Num;
//-------------------------------------------------
//_T
//-------------------------------------------------
class caEchip_Text :
	public caEchip_Basic
{
public:
	caEchip_Text(void);
public:
	virtual ~caEchip_Text(void);

	virtual int Draw(IDirect3DDevice9 *pDev);

	virtual int Init();


	tstring Text;
	tstring Font;
	int fontsize;
	int letterXnum;
	int letterYnum;

	int dropshadow;

protected:
	SpriteText stext;
	virtual void setTextTexture();
public:

};

//-------------------------------------------------
//ベルト
//-------------------------------------------------
class caEchip_Belt :
	public cAnimationChipManager_chip
{
public:
	caEchip_Belt(void);
public:
	virtual ~caEchip_Belt(void);

	//virtual int Init();

	virtual void DrowingObjectFunc();

	virtual int Draw(IDirect3DDevice9 *pDev);

	cDrawingObject_belt DOb;

	int life;

	//-------------------------------------
	//[x,y]*[z,t]で計算する。
	//-------------------------------------
	c4DVector Startp;
	c4DVector Endp;

	cColor ColorBase;
	c4DVector Alpha;

	c4DVector CenterLeft;
	c4DVector CenterRight;

	cAnimeValue<c4DVector> texRange;
	//virtual void copy(const caEchip_Basic & chip);
};


//-------------------------------------------------
//バウンド
//-------------------------------------------------
class caEchip_Bound :
	public cAnimationChipManager_chip
{
public:
	caEchip_Bound(void);
public:
	virtual ~caEchip_Bound(void);

	//virtual int Init();

	virtual void DrowingObjectFunc();

	virtual int Draw(IDirect3DDevice9 *pDev);

	cDrawingObject DO;

	int life;

	//-------------------------------------
	//[x,y]*[z,t]で計算する。
	//-------------------------------------
	c4DVector ScaleX;
	c4DVector ScaleY;
	c4DVector Rotation;
	c4DVector Rotation2;

	cColor ColorBase;
	c4DVector Alpha;

	c4DVector Centerbegin;
	c4DVector NowCenter;
	c4DVector moveVector;

	cAnimeValue<c4DVector> texRange;

	double gravity;//重力加速度(コマ/F)
	double floorHeight;//床の高さ
	double restitution;//反発係数
	int boundlimit;

	int FragFirstDOF;
	//virtual void copy(const caEchip_Basic & chip);
};
