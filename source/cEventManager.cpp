#include "stdafx.h"
#include "cEventManager.h"
#include "cEvent.h"
#include "utility/valiableField/cValiableField.h"
#include "timingEnum.h"

#include "cEventFadeInOut.h"

//—Dæ“x‡
bool prioritySortFunction(pcEvent a, pcEvent b){
    return (a->priority()
		< b->priority());
}

cEventManager::cEventManager(void):shouldChangeFlag_(false)
{
	
}

cEventManager::~cEventManager(void)
{
}

void cEventManager::sortEvent()
{
	if(shouldChangeFlag_)
	{
		std::sort( eventList_.begin(), eventList_.end(), ::prioritySortFunction  );
		shouldChangeFlag_ = false;
	}
}

void cEventManager::addEvent(pcEvent pevent)
{
	eventList_.push_back(pevent);
	shouldChangeFlag_ = true;
}
void cEventManager::eraseEvent(pcEvent pevent)
{
	std::vector<pcEvent>::iterator itr = std::find( eventList_.begin(), eventList_.end(), pevent);
	if(itr != eventList_.end())
	{
		eventList_.erase(itr);
	}
}

const std::vector<pcEvent>& cEventManager::eventList()
{
	sortEvent();
	return eventList_;
}

void cEventManager::Process()
{
	vector<pcEvent> eventList = eventList_;
	std::vector<pcEvent>::iterator itr = eventList.begin();
	for(;itr != eventList.end();itr++)
	{
		(*itr)->Process();
	}
}

void cEventManager::Draw()
{
	vector<pcEvent> eventList = eventList_;
	std::vector<pcEvent>::iterator itr = eventList.begin();
	for(;itr != eventList.end();itr++)
	{
		(*itr)->Draw();
	}
}

void cEventManager::DrawUnderObject()
{
	vector<pcEvent> eventList = eventList_;
	std::vector<pcEvent>::iterator itr = eventList.begin();
	for(;itr != eventList.end();itr++)
	{
		(*itr)->DrawUnderObject();
	}
}

void cEventManager::minimapDraw(const cCoordinate& leftTop, const int boxSize, const int alpha)
{
	vector<pcEvent> eventList = eventList_;
	std::vector<pcEvent>::iterator itr = eventList.begin();
	for(;itr != eventList.end();itr++)
	{
		(*itr)->minimapDraw(leftTop, boxSize, alpha);
	}
}
//•\Ž¦‚ðŽÀ’l‚Æ“¯Šú‚³‚¹‚é
void cEventManager::visibleReset()
{

}

//-------------------------------------------------------------------------------

void cEventManager::fadeIn(const cColor& fadeColor, const int duringFrame)
{
	if(_fade == NULL) {
		_fade = cEventFadeInOut::pNewEvent(cEvent::PRIORITY_FIRST);
		addEvent(_fade);
	}

	_fade->fadeIn(fadeColor, duringFrame);

	
}
void cEventManager::fadeOut(const int duringFrame)
{

	_fade->fadeOut(duringFrame);
}