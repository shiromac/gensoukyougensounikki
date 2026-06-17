#include "stdafx.h"

#include "caSoundPlay.h"

caSoundPlay::caSoundPlay()
{
	End = false;
	play_ID = -1;
}

caSoundPlay::~caSoundPlay()
{
}


int caSoundPlay::process(cRenderDevice *pDev)
{
	if(play_ID == -1)
	{
		if(intro.length() == 0)
		{
			if(roop.length() == 0)
			{
				g_GameEnv.m_SoundManager.stopBGM();
			}
			else
			{
				g_GameEnv.m_SoundManager.getplayBGMFromFile(roop.c_str());
			}

		}
		else
		{
			g_GameEnv.m_SoundManager.getplayBGMFromFile(intro.c_str(),roop.c_str());
		}
	}
	else
	{
		g_GameEnv.m_SoundManager.playSoundEffect(play_ID);
	}
	End = true;
	
	
	return true;
}

bool caSoundPlay::isOnScreen()
{

	if(	sg_pDungeonSystem->Map().isOnDisplay(
		placeX,placeY) || (play_ID == -1))//Œø‰Ê‰¹‚Å•·‚±‚¦‚é”ÍˆÍ‚Ü‚½‚ÍBGM
	{
		return true;
	}
	return false;

}
