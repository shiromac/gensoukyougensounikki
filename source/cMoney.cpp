#include "stdafx.h"

#include "cMoney.h"

#include "Language.h"

cMoney::cMoney(void)
{
	value = 0;
}

cMoney::cMoney(const int val)
{
	value = val;
}

cMoney::~cMoney(void)
{
	
}

tstring cMoney::caption()
{
	int s = value;
	int y = s/100;
	s -= y*100;

	TCHAR cap[32];
	if(caption_BorS())
	{
		tstring str = _T("%d_")+caption_onlymeasureB()+_T("_%.2d_")+caption_onlymeasureS();
		_stprintf(cap,str.c_str(),y,s);
	}
	else
	{
		tstring str = _T("%d_")+caption_onlymeasureS();
		_stprintf(cap,str.c_str(),s);
	}
	
	return cap;
}

bool cMoney::caption_BorS()
{
	return (value >= 100);
}

double cMoney::caption_length()//‘SŠp‚¢‚­‚Â•ª‚©
{
	int i,v = value;
	double bl = caption_onlymeasureB().length();
	double sl = caption_onlymeasureS().length();
	double l = ( bl + sl ) * 0.5 + 0.75;
	
	for(i=0;v>0;i++) v/=10;
	if(i == 0) i = 1;//i‚ªŒ…”
	if(i<=2) l -= bl + 0.5;
	l += i*0.5;
	
	return l;
}

tstring cMoney::caption_onlymeasureB()
{
	return g_Lang(_T("‰~")).conclete_tstr();
}

tstring cMoney::caption_onlymeasureS()
{
	return g_Lang(_T("‘K")).conclete_tstr();
}

tstring cMoney::caption_onlynumberB()
{
	int s = value;
	int y = s/100;
	s -= y*100;

	TCHAR cap[32];
	if(value >= 100)
		_stprintf(cap,_T("%d"),y);
	else
		_stprintf(cap,_T(""));
	
	return cap;
}

tstring cMoney::caption_onlynumberS()
{
	int s = value;
	int y = s/100;
	s -= y*100;

	TCHAR cap[32];
	if(value >= 100)
		_stprintf(cap,_T("%.2d"),s);
	else
		_stprintf(cap,_T("%2d"),s);
	
	return cap;
}