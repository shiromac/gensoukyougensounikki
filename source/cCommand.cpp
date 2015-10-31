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

int cCommandCancel::Action(IDirect3DDevice9 *pDev)
{
	sg_pDungeonSystem->メニューを一枚閉じる();
	sg_pDungeonSystem->メニューリレンダ();
	return true;
}

void cCommandDelegated::setDelegate(const cCommandDelegate& delegate){
	_delegate = delegate;
}
int cCommandDelegated::Action(IDirect3DDevice9 *pDev) {
	return _delegate(*this);
}