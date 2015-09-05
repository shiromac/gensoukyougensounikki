#include "stdafx.h"
#include "cEvent.h"

#include "utility/valiableField/cValiableField.h"
#include "timingEnum.h"
#include "cEventManager.h"

cEvent::cEvent(const int priority): priority_(priority)
{
	
}

cEvent::~cEvent(void)
{
}

void cEvent::Init(pcEvent self)
{
	self_weakpointer = wpcEvent(self);

	InitSub();//子クラスで使うかも
}

void cEvent::InitSub()
{

}

void cEvent::Draw()
{

}

void cEvent::DrawUnderObject()
{

}

void cEvent::Process()
{

}

void cEvent::minimapDraw(const cCoordinate& leftTop, const int boxSize, const int alpha)
{

}

void cEvent::visibleReset()
{

}

//統一！！
void cEvent::CutIn(タイミング timing, cValiableField& valiable)
{

}

void cEvent::endEvent()
{
	sg_pDungeonSystem->EventManager().eraseEvent(me());
}
void cEvent::startEvent()
{
	sg_pDungeonSystem->EventManager().addEvent(me());
}