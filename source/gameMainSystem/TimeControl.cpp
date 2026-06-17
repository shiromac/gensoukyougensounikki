#include "TimeControl.h"
#include <cstring>
#ifndef __EMSCRIPTEN__
#pragma comment(lib, "winmm.lib")
#endif

//======================================================
//コンストラクタ
//======================================================
TimeControl::TimeControl(DWORD dwFps, bool bFrameSkip)
{
	memset(this,0,sizeof(TimeControl));
	m_bInit = 1;
	m_bFrameSkip = bFrameSkip;
	SetFPS(dwFps);
	cPlatformBeginTimerPeriod(1);
	SetmaxSkip(dwFps);
	m_SkipCount = 0;
}


//======================================================
//デストラクタ
//======================================================
TimeControl::~TimeControl()
{
	cPlatformEndTimerPeriod(1);
}

//======================================================
//ここで時間制御をします。
//======================================================
void TimeControl::TimeRegular()
{
#ifdef __EMSCRIPTEN__
	m_dwFrameCount++;
	if(m_bInit == 1)
	{
		m_dwLastMinitues = cPlatformGetMilliseconds();
		m_bInit = 0;
	}
	m_bDrawFlag = 1;

	DWORD now = cPlatformGetMilliseconds();
	if(now - m_dwLastMinitues >= 1000)
	{
		m_dwLastMinitues = now;
		m_dwFrameRate = m_dwFrameCount;
		m_dwFrameCount = 0;
		m_dwSkipRate = 0;
		m_dwSkipCount = 0;
	}
	return;
#endif
	m_dwFrameCount++;

	
	if(m_bInit == 1)
	{
		m_dwLastMinitues = cPlatformGetMilliseconds();
		m_bInit = 0;
		m_bDrawFlag = 1;
		return;
	}

	if(m_bFrameSkip == 1 && cPlatformGetMilliseconds()  > (DWORD)((m_dwFrameCount + 1) * m_fFrameTime + m_dwLastMinitues)
		&& (m_maxSkip > m_SkipCount))
	{
		m_bDrawFlag = 0;
		m_dwSkipCount++;
		m_SkipCount++;
	}
	else
	{
		//===========================================================
		//項の移動をしたことで無駄な演算を減らしました。
		//条件			cPlatformGetMilliseconds() - m_dwLastMinitues <= (DWORD)((m_dwFrameCount + 1) * m_fFrameTime)
		//項の移動		cPlatformGetMilliseconds()  <= (DWORD)((m_dwFrameCount + 1) * m_fFrameTime) +  m_dwLastMinitues
		//変数置き換え	cPlatformGetMilliseconds()　<= dwTime
		//===========================================================
		DWORD dwTime = (DWORD)(m_dwFrameCount * m_fFrameTime + m_dwLastMinitues);
		while(cPlatformGetMilliseconds() <= dwTime)
		{
			cPlatformSleepMilliseconds(1);
		}
		m_bDrawFlag = 1;
		m_SkipCount = 0;
	}

	
	if(cPlatformGetMilliseconds() - m_dwLastMinitues >= 1000)
	{
		m_dwLastMinitues = cPlatformGetMilliseconds();
		m_dwFrameRate = m_dwFrameCount;
		m_dwFrameCount = 0;
		m_dwSkipRate = m_dwSkipCount;
		m_dwSkipCount = 0;
	}
}

//==================================================
//最大フレームレートを測定します。
//==================================================
void TimeControl::Measure()
{
	m_dwFrameCount++;
	m_bDrawFlag = 1;
	if(cPlatformGetMilliseconds() - m_dwLastMinitues >= 1000)
	{
		m_dwLastMinitues = cPlatformGetMilliseconds();
		m_dwFrameRate = m_dwFrameCount;
		m_dwFrameCount = 0;
		m_dwSkipRate = m_dwSkipCount;
		m_dwSkipCount = 0;
	}	
}

//======================================================
//フレームスキップをするかしないかを設定します。
//======================================================
void TimeControl::OnFrameSkip(bool bFrameSkip)
{
	m_bFrameSkip = bFrameSkip;
}

//======================================================
//ここでFPSを設定する。
//======================================================
void TimeControl::SetFPS(DWORD fps)
{
	m_fFrameTime = 1000.0f / fps;
}

void TimeControl::SetmaxSkip(DWORD maxskip)
{
	m_maxSkip = maxskip;
}


//======================================================
//ここでフレームレートを取得します。
//======================================================
DWORD TimeControl::GetFrameRate()
{
	return m_dwFrameRate;
}

//======================================================
//ここでスキップレートを取得します。
//======================================================
DWORD TimeControl::GetSkipRate()
{
	return m_dwSkipRate;
}

//======================================================
//描画するかしないかを判定
//======================================================
bool TimeControl::GetDrawFlag()
{
	return m_bDrawFlag;
}