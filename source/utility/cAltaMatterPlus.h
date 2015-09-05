#pragma once
#include "caltamatter.h"

class cAltaMatterPlus :
	public cAltaMatter
{
public:
	cAltaMatterPlus(void);
public:
	virtual ~cAltaMatterPlus(void);

	void process(bool Input);//各値更新//ここに値を入れる


	int onCount;
	int offCount;

};
