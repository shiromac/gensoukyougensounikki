#pragma once

#include "cEvent.h"
class cEventFadeInOut;
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cEventFadeInOut> pcEventFadeInOut;
class cEventFadeInOut : public cEvent
{
protected:
	cEventFadeInOut(const int priority);

public:
	virtual ~cEventFadeInOut();

	static pcEventFadeInOut pNewEvent(const int priority)
	{
		pcEventFadeInOut pevent = pcEventFadeInOut(new cEventFadeInOut(priority));
		pevent->Init(pevent);
		return pevent;
	};
	virtual void Draw();
	virtual void Process();

	void fadeIn(const cColor& fadeColor, const int duringFrame);
	void fadeOut(const int duringFrame);
private:
	cColor _destinationColor;
	cColor _nowColor;
	cColor _startColor;
	int _fadeCount;
	int _durationTime;
	bool _fadingInOnt;
};