#include "../stdafx.h"

#include "debug.h"

#include <assert.h>
#ifndef __EMSCRIPTEN__
#include <windows.h>
#endif

#include <tstring_ph.h>



using namespace std;

void OnAssert(const TCHAR*  fileName, int lineNumber, bool exception, const TCHAR*  msg)
{
	#ifdef _UNRELEASE

	if(!exception)
	{
		TCHAR c[16];
		tstring str = fileName;
		str += _T(" : line ");
		_stprintf(c,_T("%d"),lineNumber);
 		str += c;
		str += _T(" : ");
		if(msg != NULL)str += msg;
#ifndef __EMSCRIPTEN__
		MessageBox(NULL,str.c_str(),_T("åxçê"),MB_OK);
#endif
		//assert(!(str.c_str()));
	}
	#endif
}

void OnAssert(const TCHAR*  fileName, int lineNumber, bool exception, const TCHAR*  msg , const TCHAR*  msg2)
{

	#ifdef _UNRELEASE

	if(!exception)
	{
		OnAssert(fileName,lineNumber,exception,((tstring)msg+msg2).c_str());
	}
	#endif
}