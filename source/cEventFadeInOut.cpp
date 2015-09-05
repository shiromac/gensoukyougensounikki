#include "stdafx.h"
#include "cEventFadeInOut.h"

cEventFadeInOut::cEventFadeInOut(const int priority):cEvent(priority)
{
	_fadeCount = 0;
	_durationTime = 0;
	_destinationColor.ARGB(0,0,0,0);
	_nowColor.ARGB(0,0,0,0);
	_startColor.ARGB(0,0,0,0);
	_fadingInOnt = false;
}
cEventFadeInOut::~cEventFadeInOut()
{
	
}

void cEventFadeInOut::Draw()
{
	if(_nowColor.alpha > 0) {

		cDrawingObject DO;

		DO.setLTRB(0,0,SCREEN_X,SCREEN_Y);

		DO.m_color = _nowColor;

		DO.Draw(sg_pDungeonSystem->pDevice_D3D);
	}
}
void cEventFadeInOut::Process()
{
	if(_fadingInOnt) {
		double ratio = (double)(_fadeCount+1)/ (double)(_durationTime+1);
		_nowColor = (1-ratio)*_startColor + (ratio)*_destinationColor;
		_fadeCount++;
		if(_fadeCount > _durationTime) {
			_fadingInOnt = false;
			_nowColor = _destinationColor;
		}
	}
}
void cEventFadeInOut::fadeIn(const cColor& fadeColor, const int duringFrame)
{
	_destinationColor = fadeColor;
	if(_nowColor.alpha == 0) {
		_nowColor.ARGB(0,fadeColor.red,fadeColor.green,fadeColor.blue);
	}
	_startColor = _nowColor;
	_durationTime = duringFrame;
	if(_durationTime < 0)
	{
		_durationTime = 0;
	}
	_fadeCount = 0;
	_fadingInOnt = true;
}
void cEventFadeInOut::fadeOut(const int duringFrame)
{
	_destinationColor.ARGB(0,_nowColor.red,_nowColor.green,_nowColor.blue);
	_startColor = _nowColor;
	_durationTime = duringFrame;
	if(_durationTime < 0)
	{
		_durationTime = 0;
	}
	_fadeCount = 0;
	_fadingInOnt = true;
}