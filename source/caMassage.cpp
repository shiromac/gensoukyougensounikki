#include "stdafx.h"

#include "caMassage.h"

caMassage::caMassage()
{
	End = false;
	LayerLevel = LAYERLEVEL_NORMAL;
}

caMassage::~caMassage()
{
}


int caMassage::process(IDirect3DDevice9 *pDev)
{

	sg_pDungeonSystem->Massage.addMassage(massage);

	End = true;
	
	
	return true;
}

bool caMassage::isOnScreen()
{

	return true;

}
