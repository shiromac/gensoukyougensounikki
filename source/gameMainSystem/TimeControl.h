#ifndef __TIMECONTROL_H__
#define __TIMECONTROL_H__



#include "cPlatformTime.h"

//=======================================================
//フレームレートを制御するクラスです。
//======================================================
class TimeControl
{
public:
	TimeControl(DWORD dwFps = 60, bool bFrameSkip = true);
	virtual ~TimeControl();
private:
	DWORD m_dwLastFlipped;
	DWORD m_dwLastMinitues;
	DWORD m_dwFrameRate;
	DWORD m_dwSkipRate;
	float m_fFrameTime;
	int m_maxSkip;
	DWORD m_SkipCount;
	bool m_bDrawFlag;
	bool m_bFrameSkip;
		
	bool m_bInit;
	//=======================================================
	//現在の１秒単位のレートの表示
	//======================================================
	DWORD m_dwFrameCount;
	DWORD m_dwSkipCount;
public:
	void Measure();
	void SetFPS(DWORD time);
	void SetmaxSkip(DWORD maxskip);
	void OnFrameSkip(bool bFrameSkip);
	bool GetDrawFlag();
	void TimeRegular();
	DWORD GetSkipRate();
	DWORD GetFrameRate();
};




#endif