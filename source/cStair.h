#pragma once
#include "cdroping.h"


#define CDROPING_cStair_BASE_ID_NUM 10000

#define STAIR_ASPECT_DOWN 0
#define STAIR_ASPECT_UP 1

#define DEF_class_cStair_ID(identify) \
class cStair_ID_##identify## :\
	public cStair\
{\
public:\
	cStair_ID_##identify##(void){};\
public:\
	virtual ~cStair_ID_##identify##(void){};\
	virtual inline int ID(){return CDROPING_cStair_BASE_ID_NUM + identify;};\
public:\



class cStair :
	public cDroping
{
public:
	cStair(void);
public:
	virtual ~cStair(void);
	
	virtual void settingInit();

	virtual StyleString FullName();
	virtual StyleString FullNameSub();
	virtual StyleString ShortName();
	virtual StyleString ShortNameSub();

	//virtual tstring FullNameBase(){return _T("階段");};
	//virtual tstring ShortNameBase(){return _T("階段");};
	virtual int GetmenuCaption(vector<tstring> &CaptionList);

	//virtual StyleString shortExplanation();
	//virtual StyleString longExplanation();
	virtual bool 被拾得可(){return false;};//取得不可
	virtual bool 被破壊可(){return false;};//破壊不可

	//自動プロセス群
	virtual int 乗る();
	
	virtual bool 大切(){return true;};

	//virtual int 昇降する(IDirect3DDevice9 *pDev);

	virtual inline tstring IconFileName(){return _T("item\\階段セット.png");};
	virtual inline int IconFileIndexX(){return aspect;};
	virtual inline int IconFileIndexY(){return 0;};
	int aspect;

	virtual void CutIn(タイミング timing, cValiableField& valiable);

	virtual inline int ID(){return CDROPING_cStair_BASE_ID_NUM;};
	
	virtual void setattribute()
	{
		属性.insert(落ち物属性::階段);
	};
};


DEF_class_cStair_ID(0)
};

#define STAIR_NUM 1