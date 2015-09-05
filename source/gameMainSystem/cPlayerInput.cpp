#include "cPlayerInput.h"


cPlayerInput::cPlayerInput(void)
{
}

cPlayerInput::~cPlayerInput(void)
{
}


const cAltaMatterPlus cPlayerInput::cancel()
{
	cAltaMatterPlus cancelbutton;
	/*
	cancelbutton.eve = dash().eve || turn().eve;
	cancelbutton.on = dash().on || turn().on;
	cancelbutton.offCount = (dash().onCount> turn().onCount)?dash().onCount: turn().onCount;
	cancelbutton.onCount = (dash().onCount> turn().onCount)?dash().onCount: turn().onCount;
	cancelbutton.justOn = dash().justOn || turn().justOn;
	cancelbutton.justOff = dash().justOff || turn().justOff;
	*/
	cancelbutton = dash();
	return cancelbutton;
}
const cAltaMatterPlus cPlayerInput::decision()
{
	cAltaMatterPlus decisionbutton;
	decisionbutton = attack();
/*
	decisionbutton.eve = attack().eve || menu().eve;
	decisionbutton.on = attack().on || menu().on;
	decisionbutton.offCount = (attack().onCount> menu().onCount)?attack().onCount: menu().onCount;
	decisionbutton.onCount = (attack().onCount> menu().onCount)?attack().onCount: menu().onCount;
	decisionbutton.justOn = attack().justOn || menu().justOn;
	decisionbutton.justOff = attack().justOff || menu().justOff;
*/
	return decisionbutton;
}

const cAltaMatter cPlayerInput::anykey()
{
	cAltaMatter pressanykey;
	pressanykey.eve = false;
	pressanykey.on = false;
	pressanykey.justOn = false;
	pressanykey.justOff = false;
	int i;
	for(i=0;i<PLAYERINPUT_BUTTONNUM;i++)
	{
		pressanykey.eve |= buttons[i].eve;
		pressanykey.on |= buttons[i].on;

		pressanykey.justOn |= buttons[i].justOn;
		pressanykey.justOff |= buttons[i].justOff;
	}

	return pressanykey;
}