#include "cCommand.h"
#include "cDungeonSystem.h"
cCommand::cCommand(StyleString s)
{
	caption = s;
}
cCommand::cCommand(void)
{
	caption = _T("");
}


cCommand::~cCommand(void)
{
}

int cCommandCancel::Action(cRenderDevice *pDev)
{
	sg_pDungeonSystem->メニューを一枚閉じる();
	sg_pDungeonSystem->メニューリレンダ();
	return true;
}

