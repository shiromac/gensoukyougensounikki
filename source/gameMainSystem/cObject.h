#pragma once
#include "../utility/cRectObj.h"

class cObject : public cRectObj
{
public:
	cObject(void);
public:
	virtual ~cObject(void);

	//Ç¢ÇÁÇ»Ç≠Ç»Ç¡ÇΩÇÁfalseÇï‘Ç∑ÅB
	virtual bool process()=0;
};
