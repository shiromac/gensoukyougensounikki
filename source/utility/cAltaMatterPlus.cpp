#include "cAltaMatterPlus.h"

cAltaMatterPlus::cAltaMatterPlus(void)
{
	on = false;
	eve = false;
	justOn = false;
	justOff = false;
	onCount = 0;
	offCount = 0;

}

cAltaMatterPlus::~cAltaMatterPlus(void)
{
}

void cAltaMatterPlus::process(bool Input)
{
	eve = on;
	on = Input;

	justOn = on&&!eve;
	justOff = !on&&eve;

	if(justOn) onCount = 0;
	if(on) onCount++;


	if(justOff) offCount = 0;
	if(!on) offCount++;

}