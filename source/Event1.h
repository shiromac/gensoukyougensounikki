#pragma once

#include "cEvent.h"
#include "cDrawingObject_Rod.h"
#include "cColor.h"

class cEvent_OnryouSpawn;
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cEvent_OnryouSpawn> pcEvent_OnryouSpawn;
class cEvent_OnryouSpawn : public cEvent
{
protected:
	cEvent_OnryouSpawn(const int priority):cEvent(priority){}

public:
	virtual ~cEvent_OnryouSpawn(){};

	static pcEvent pNewEvent(const int priority, int x, int y, int classnum)
	{
		pcEvent_OnryouSpawn pevent = pcEvent_OnryouSpawn(new cEvent_OnryouSpawn(priority));
		pevent->Init(pevent);
		pevent->x_ = x;
		pevent->y_ = y;
		pevent->classnum_ = classnum;
		return boost::static_pointer_cast<cEvent>(pevent);
	};
	virtual void CutIn(タイミング timing, cValiableField& valiable);
	virtual void InitSub();

private:
	int count_;

	int x_;
	int y_;
	int classnum_;
};

class cEvent_ItemHidding;
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cEvent_ItemHidding> pcEvent_ItemHidding;
class cEvent_ItemHidding : public cEvent
{
protected:
	cEvent_ItemHidding(const int priority):cEvent(priority){}

public:
	virtual ~cEvent_ItemHidding(){};

	static pcEvent pNewEvent(const int priority, pcCharacter hideChara, pcDroping hideItem)
	{
		pcEvent_ItemHidding pevent = pcEvent_ItemHidding(new cEvent_ItemHidding(priority));
		pevent->Init(pevent);
		pevent->hideItem_ = hideItem;
		pevent->hideChara_ = hideChara;
		return boost::static_pointer_cast<cEvent>(pevent);
	};
	virtual void CutIn(タイミング timing, cValiableField& valiable);
	virtual void InitSub();

private:

	pcDroping hideItem_;
	pcCharacter hideChara_;
};

class cEvent_ItemSpowning;
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cEvent_ItemSpowning> pcEvent_ItemSpowning;
class cEvent_ItemSpowning : public cEvent
{
protected:
	cEvent_ItemSpowning(const int priority):cEvent(priority){}

public:
	virtual ~cEvent_ItemSpowning(){};

	static pcEvent pNewEvent(const int priority, pcLandform dropLand, pcDroping dropItem)
	{
		pcEvent_ItemSpowning pevent = pcEvent_ItemSpowning(new cEvent_ItemSpowning(priority));
		pevent->Init(pevent);
		pevent->dropItem_ = dropItem;
		pevent->dropLand_ = dropLand;
		return boost::static_pointer_cast<cEvent>(pevent);
	};
	virtual void CutIn(タイミング timing, cValiableField& valiable);
	virtual void InitSub();

private:

	pcDroping dropItem_;
	pcLandform dropLand_;
};

class cEvent_BossLazer;
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cEvent_BossLazer> pcEvent_BossLazer;
class cEvent_BossLazer : public cEvent
{
protected:
	cEvent_BossLazer(const int priority):cEvent(priority){}

public:
	virtual ~cEvent_BossLazer(){};

	static pcEvent pNewEvent(const int priority, pcLandform land, int aspect, int length, int power, int Forse, cColor color, int readyTurn, int attackTurn, bool isAttackFriend, bool isStopAtWall)
	{
		pcEvent_BossLazer pevent = pcEvent_BossLazer(new cEvent_BossLazer(priority));
		pevent->land_ = land;
		pevent->power_ = power;
		pevent->Forse_ = Forse;
		pevent->color_ = color;
		pevent->readyTurn_ = readyTurn;
		pevent->attackTurn_ = attackTurn;
		pevent->aspect_ = aspect;
		pevent->length_ = length;
		pevent->isAttackFriend_ = isAttackFriend;
		pevent->isStopAtWall_ = isStopAtWall;
		pevent->Init(pevent);
		return boost::static_pointer_cast<cEvent>(pevent);
	};
	virtual void CutIn(タイミング timing, cValiableField& valiable);
	virtual void TurnProcess();
	virtual void DrawProcess();
	virtual void Fire();
	virtual void InFire(pcLandform land);
	virtual void InitSub();
	virtual void Draw();
	virtual void minimapDraw(const cCoordinate& leftTop, const int boxSize, const int alpha);

	virtual void visibleReset();
	//SummonLazer(LandForm, power, Forse, color, readyTurn, attackTurn, aspect, length, isAttackFriend, isStopAtWall);

private:
	pcLandform land_;
	int power_;
	int Forse_;
	cColor color_;
	int readyTurn_;
	int attackTurn_;
	int aspect_;
	int length_;
	int isAttackFriend_;
	int isStopAtWall_;


	int countOfReadyTurn_;
	int countOfAttackTurn_;
	int culcuratedLength_;
	int displayCount_;
	enum State {
		stateFirst = 0,
		stateReady,
		stateFire,
		stateEnd,
	} state_;

	
	bool isJustFire_;

	enum DrawState {
		drawStateFirst = 0,
		drawStateFirstToReady,
		drawStateReady,
		drawStateReadyToFire,
		drawStateFire,
		drawStateFireToEnd,
		drawStateEnd,
	} drawState_;


	virtual void resetLength();

	
	virtual void resetVisibleLength();
	int visibleLength_;
	double visibleWidth_;
	double visibleAlpha_;
	
	double visibleAdvanceLightRadian_;
	cDrawingObject_Rod rodEx_;
	cDrawingObject_Rod rodIn_;
	cDrawingObject_Rod rodAdvance_;

	cDrawingObject_Rod rodMiniMap_;
};

