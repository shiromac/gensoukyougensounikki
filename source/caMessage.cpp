#include "stdafx.h"

#include "caMessage.h"

caMessage::caMessage()
{
	End = false;
	LayerLevel = LAYERLEVEL_NORMAL;
}

caMessage::~caMessage()
{
}


int caMessage::process(cRenderDevice *pDev)
{

	sg_pDungeonSystem->MessageW().addMessage(Message);

	End = true;
	
	
	return true;
}

bool caMessage::isOnScreen()
{

	return true;

}
