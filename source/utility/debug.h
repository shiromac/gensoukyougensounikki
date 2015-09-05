
#pragma once

#include <windows.h>

void OnAssert(const TCHAR*  fileName, int lineNumber, bool exception, const TCHAR*  msg = NULL);
void OnAssert(const TCHAR*  fileName, int lineNumber, bool exception, const TCHAR*  msg , const TCHAR*  msg2);