#pragma once
#include "cControl.h"
#include "cDrawingObject.h"
#include "cDrawingObject_belt.h"
#include "cDrawingObject_ring.h"

#include <list>
using namespace std;

class cAnimation;
#include <boost/pointer_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "utility/ValiableField/SharedValiable.h"
typedef boost::shared_ptr<cAnimation> pcAnimation;
typedef boost::weak_ptr<cAnimation> wpcAnimation;
class cAnimation :
	public cControl
{
public:
	cAnimation(void);
public:
	virtual ~cAnimation(void);


	virtual int process(cRenderDevice *pDev){return true;};
	virtual int Draw(cRenderDevice *pDev){return true;};
	virtual int Init(pcAnimation me){wme_ = wpcAnimation(me);return true;};

	//使用するか微妙
	int LayerLevel;

	//発火しやすいか（WeekInvocationで発火するか）
	virtual int EasyInvocation(){return true;};
protected:
	//仕様変更。常に判定する
	virtual bool isOnScreen(){return true;};
public:
	//仕様変更。常に判定する
	virtual bool isOnScreenNow(){return isOnScreen();};
public:
	//終了フラグ
	SharedValiable_likeInt<bool> End;

protected:
	wpcAnimation wme_;
public:
	pcAnimation me(){return wme_.lock();};
};

class cAnimation_Wait :
	public cAnimation
{
public:
	cAnimation_Wait(void){wait = 0;};
public:
	virtual ~cAnimation_Wait(void){};
	
	int wait;

	virtual int process(cRenderDevice *pDev){
		if(wait-- <= 0) {
			End = true;
		}
		return true;
	};
};

typedef boost::shared_ptr<cAnimation_Wait> pcAnimation_Wait;

#include <boost/pointer_cast.hpp>
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cAnimation> pcAnimation;
//boost::static_pointer_cast<cAnimation>()を使ってアップキャスト
class cAnimationChipManager_chip;
#include <boost/shared_ptr.hpp>


typedef boost::shared_ptr<cAnimationChipManager_chip> pcAnimationChipManager_chip;
typedef boost::weak_ptr<cAnimationChipManager_chip> wpcAnimationchip;

class cAnimationChipManager;

typedef boost::weak_ptr<cAnimationChipManager> wpcAnimationChipManager;
typedef boost::shared_ptr<cAnimationChipManager> pcAnimationChipManager;
class cAnimationChipManager_chip;
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cAnimationChipManager_chip> pcAnimationChipManager_chip;

/************************************
cAnimation <|- cAnimationChipManager <>- pcAnimationChipManager_chip
 cAnimationChipManager <|- cAnimationChipManager_chip


*************************************/
//-----------------------------------------------------
class cAnimationChipManager :
	public cAnimation
{
public:
	cAnimationChipManager(void);
public:
	virtual ~cAnimationChipManager(void);

	virtual int chipprocess(cRenderDevice *pDev);
	virtual int chipDraw(cRenderDevice *pDev);

	bool chipEmpty();

	virtual int addAnimechip(pcAnimationChipManager_chip animechip);
	virtual int clearAnimechip();

protected:
	list<pcAnimationChipManager_chip> chiplist;
	wpcAnimationChipManager parentManager_;
public:

	virtual bool isOnScreen(){return true;};

	c4DVector place;
	c4DVector firstmapforcus;

	virtual c4DVector Baseplace(){return place;};
	virtual c4DVector nowplace(){return Baseplace();};
	
	virtual c4DVector absoluteRevise();//絶対座標補正

	virtual int Init(pcAnimation me);
};

//-----------------------------------------------------
class cAnimationChipManager_chip :
	public cAnimationChipManager
{
public:
	cAnimationChipManager_chip(void);
public:
	virtual ~cAnimationChipManager_chip(void);


	virtual int process(cRenderDevice *pDev);
	//virtual int Draw(cRenderDevice *pDev) = 0;

	virtual int Init(pcAnimation me);



	//潜伏フレーム
	int hidingTime;
	int FrameCount;

	virtual void DrowingObjectFunc(){};

	//コピーする
	//virtual void copy(const cAnimationChipManager_chip & chip);

protected:
	pcAnimationChipManager_chip nextchip_;
public:

	virtual int addNextchip(pcAnimationChipManager_chip animechip);

	virtual pcAnimationChipManager_chip nextchip(){return nextchip_;};
	

	int AbsolutePlaceFlag;
};

