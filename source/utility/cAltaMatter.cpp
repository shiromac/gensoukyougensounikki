#include "cAltaMatter.h"

cAltaMatter::cAltaMatter(void)
{
	on = false;
	eve = false;
	justOn = false;
	justOff = false;
}

cAltaMatter::~cAltaMatter(void)
{
}

void cAltaMatter::process(bool Input)
{
	eve = on;
	on = Input;

	justOn = on&&!eve;
	justOff = !on&&eve;


}