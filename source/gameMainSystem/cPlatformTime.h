#pragma once

#ifdef __EMSCRIPTEN__

#include <emscripten/emscripten.h>

typedef unsigned long DWORD;

inline DWORD cPlatformGetMilliseconds()
{
	return static_cast<DWORD>(emscripten_get_now());
}

inline void cPlatformSleepMilliseconds(DWORD milliseconds)
{
	(void)milliseconds;
}

inline void cPlatformBeginTimerPeriod(DWORD milliseconds)
{
	(void)milliseconds;
}

inline void cPlatformEndTimerPeriod(DWORD milliseconds)
{
	(void)milliseconds;
}

#else

#include <windows.h>
#include <mmsystem.h>

inline DWORD cPlatformGetMilliseconds()
{
	return timeGetTime();
}

inline void cPlatformSleepMilliseconds(DWORD milliseconds)
{
	Sleep(milliseconds);
}

inline void cPlatformBeginTimerPeriod(DWORD milliseconds)
{
	timeBeginPeriod(milliseconds);
}

inline void cPlatformEndTimerPeriod(DWORD milliseconds)
{
	timeEndPeriod(milliseconds);
}

#endif
