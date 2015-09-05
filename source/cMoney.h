#pragma once


#include <stdio.h>
#include <tstring_ph.h>


using namespace std;

class cMoney
{
public:
	cMoney(void);
	cMoney(const int val);
	virtual ~cMoney(void);
	

	int value;

	virtual tstring caption();
	virtual double caption_length();

	virtual bool caption_BorS();
	virtual tstring caption_onlymeasureB();
	virtual tstring caption_onlymeasureS();
	virtual tstring caption_onlynumberB();
	virtual tstring caption_onlynumberS();
};