#pragma once


class cEvent;
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
typedef boost::shared_ptr<cEvent> pcEvent;
typedef boost::weak_ptr<cEvent> wpcEvent;
#include "cColor.h"

class cEventFadeInOut;
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cEventFadeInOut> pcEventFadeInOut;

//-----------------------------------------------------
//設計上は仮想クラスだがluaの都合で具体クラスにする
//-----------------------------------------------------

#include <set>

class cEventManager
{
public:
	cEventManager();
	//クラスごとに独自のコンストラクタを定義してください
public:
	virtual ~cEventManager(void);
private:
	std::vector<pcEvent> eventList_;
	void sortEvent();
	bool shouldChangeFlag_;
public:

	void addEvent(pcEvent pevent);
	void eraseEvent(pcEvent pevent);

	const std::vector<pcEvent>& eventList();
	void Draw();
	void DrawUnderObject();
	void Process();
	void minimapDraw(const cCoordinate& leftTop, const int boxSize, const int alpha);

	//表示を実値と同期させる
	void visibleReset();

	//grobalEvent

	void fadeIn(const cColor& fadeColor, const int duringFrame);
	void fadeOut(const int duringFrame);
private:
	pcEventFadeInOut _fade;
};





#define NULLEvent pcEvent((cEvent*)NULL)