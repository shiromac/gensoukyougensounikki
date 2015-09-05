#pragma once

#include "cEvent.h"
#include "AnimeChips.h"
class cEventBackground;
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cEventBackground> pcEventBackground;
class cEventBackground : public cEvent
{
protected:
	cEventBackground(const int priority);

public:
	virtual ~cEventBackground();

	static pcEventBackground pNewEvent(const int priority, const tstring& fileName, PositionPerGrid position, int power)
	{
		pcEventBackground pevent = pcEventBackground(new cEventBackground(priority));
		pevent->Init(pevent);
		pevent->_position = position;
		pevent->_fileName = fileName;
		pevent->_power = power;
		return pevent;
	};
	virtual void Draw();
	virtual void DrawUnderObject();
	virtual void Process();

	void fadeIn(const int duringFrame);
	void fadeOut(const int duringFrame);
private:
	cDrawingObject _cd;
	PositionPerGrid _position;
	tstring _fileName;
	int _power;
	//D3DXVECTOR2 _perticleSize;
	int _fadeCount;
	int _durationTime;
	bool _fadingInOnt;
	double _destinationAlpha;
	double _nowAlpha;
	double _startAlpha;
};