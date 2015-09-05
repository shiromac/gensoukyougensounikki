#pragma once


class cEvent;
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
typedef boost::shared_ptr<cEvent> pcEvent;
typedef boost::weak_ptr<cEvent> wpcEvent;


//-----------------------------------------------------
//設計上は仮想クラスだがluaの都合で具体クラスにする
//-----------------------------------------------------

#include <set>

class cValiableField;
enum タイミング;

class cEvent
{
protected:
	cEvent(const int priority);
	virtual void Init(pcEvent self);
	//クラスごとに独自のコンストラクタを定義してください
public:
	virtual ~cEvent(void);

	//コンストラクタ
	template <typename Type>
	static pcEvent pNewEvent(const int priority = PRIORITY_MIDDLE)
	{
		pcEvent pevent = pcEvent(new Type(priority));
		pevent->Init(pevent);
		return pevent;
	};
public:
	
	enum PRIORITY{
		PRIORITY_FIRST	= 100000,
		PRIORITY_HIGH	= 10000,
		PRIORITY_MIDDLE = 0,
		PRIORITY_LOW	= -10000,
		PRIORITY_LAST	= -100000,
	};
protected:
	wpcEvent self_weakpointer;
	int priority_;
public:
	pcEvent me(){return self_weakpointer.lock();};
	int priority(){return priority_;};

public:

	virtual void InitSub();

	virtual void endEvent();
	virtual void startEvent();

	virtual void Draw();
	virtual void DrawUnderObject();
	virtual void Process();
	virtual void minimapDraw(const cCoordinate& leftTop, const int boxSize, const int alpha);

	//統一！！
	virtual void CutIn(タイミング timing, cValiableField& valiable);

	//表示を実値と同期させる
	virtual void visibleReset();
};





#define NULLEvent pcEvent((cEvent*)NULL)