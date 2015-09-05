#pragma once
#include "../utility/cAltaMatterPlus.h"

#define PLAYERINPUT_BUTTONNUM (12)
class cPlayerInput
{
public:
	cPlayerInput(void);
public:
	virtual ~cPlayerInput(void);



	virtual const cAltaMatterPlus& attack(){return buttons[4];};
	virtual const cAltaMatterPlus& turn(){return buttons[5];};
	virtual const cAltaMatterPlus& dash(){return buttons[6];};
	virtual const cAltaMatterPlus& menu(){return buttons[7];};
	virtual const cAltaMatterPlus& miniMap(){return buttons[8];};
	virtual const cAltaMatterPlus& diagon(){return buttons[9];};
	virtual const cAltaMatterPlus& shot(){return buttons[10];};
	virtual const cAltaMatterPlus& smartdash(){return buttons[11];};

	virtual const cAltaMatterPlus decision();
	virtual const cAltaMatterPlus cancel();

	virtual const cAltaMatter anykey();

	int X;
	int Y;

	virtual const cAltaMatterPlus& ue(){return buttons[0];};
	virtual const cAltaMatterPlus& shita(){return buttons[1];};
	virtual const cAltaMatterPlus& hidari(){return buttons[2];};
	virtual const cAltaMatterPlus& migi(){return buttons[3];};


	virtual cAltaMatterPlus& setattack(){return buttons[4];};
	virtual cAltaMatterPlus& setturn(){return buttons[5];};
	virtual cAltaMatterPlus& setdash(){return buttons[6];};
	virtual cAltaMatterPlus& setmenu(){return buttons[7];};
	virtual cAltaMatterPlus& setminiMap(){return buttons[8];};
	virtual cAltaMatterPlus& setdiagon(){return buttons[9];};
	virtual cAltaMatterPlus& setshot(){return buttons[10];};
	virtual cAltaMatterPlus& setsmartdash(){return buttons[11];};

	virtual cAltaMatterPlus& setue(){return buttons[0];};
	virtual cAltaMatterPlus& setshita(){return buttons[1];};
	virtual cAltaMatterPlus& sethidari(){return buttons[2];};
	virtual cAltaMatterPlus& setmigi(){return buttons[3];};



protected:
	cAltaMatterPlus buttons[PLAYERINPUT_BUTTONNUM];
};
