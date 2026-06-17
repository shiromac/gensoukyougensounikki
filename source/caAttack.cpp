#include "stdafx.h"

#include "caAttack.h"


#define ATTACKDEFAULTTEXSIZE 256
#define ATTACKDEFAULTTEXPOWER 2
#define ATTACKDEFAULTTEXVERSEP (4.0)
#define ATTACKDEFAULTTEXHORSEP (4.0)

#define ATTACKDEFAULT_T_FUMIKIRI (0.5)
#define ATTACKDEFAULT_V_FUMIKIRI (0.4)
#define ATTACKDEFAULT_T_KAMAE (0.5)
#define ATTACKDEFAULT_V_KAMAE (0.1)



caAttack::caAttack()
{
	End = false;
	count = 0;
	speed = 1;
	p_Tex = NULL;
	SE_playID = -1;
	SE_playcount = false;
}

caAttack::~caAttack()
{
}


int caAttack::process(cRenderDevice *pDev)
{
	//---------------------------------------
	//	checkpointに始点（中間点）終点の順にいれてから処理を行う
	//---------------------------------------
	count += speed;


	int aspect = attackaspect;
	cCoordinate aspectcoo;
	aspectcoo.SetAspect(aspect);
	
	if( count < ANIME_ATTACK_STEP)
	{

		if(count < ANIME_ATTACK_STEP*ATTACKDEFAULT_T_FUMIKIRI)
		{
			if(count < ANIME_ATTACK_STEP*ATTACKDEFAULT_T_KAMAE)
			{
				pAttacker->visibleplace.set(
					placeX_ + aspectcoo.x*ATTACKDEFAULT_V_KAMAE,
					placeY_ + aspectcoo.y*ATTACKDEFAULT_V_KAMAE,0,0
					);
			}
			else
			{

				pAttacker->visibleplace.set(
					placeX_ + aspectcoo.x*ATTACKDEFAULT_V_FUMIKIRI,
					placeY_ + aspectcoo.y*ATTACKDEFAULT_V_FUMIKIRI,0,0
					);
			}
		}
		else
		{
			if(SE_playcount == false)
			{
				g_GameEnv.m_SoundManager.playSoundEffect(SE_playID);
				SE_playcount = true;
			}
			pAttacker->anime_yawing = -45;
			pAttacker->visibleplace.set(
				placeX_ + aspectcoo.x*ATTACKDEFAULT_V_FUMIKIRI,
				placeY_ + aspectcoo.y*ATTACKDEFAULT_V_FUMIKIRI,0,0
				);

		}
	}
	else
	{
		pAttacker->anime_yawing = 0;
		pAttacker->visibleplace.set(
					placeX_,
					placeY_,0,0);


		End = true;
	
	}
	return true;
}
int caAttack::Init(pcAnimation me)
{
	cAnimation::Init(me);
	placeX_ = pAttacker->placeX;
	placeY_ = pAttacker->placeY;
	return true;
}
int caAttack::Draw(cRenderDevice *pDev)
{

	cDrawingObject DO;

	int aspect = pAttacker->aspect;
	cCoordinate aspectcoo;
	aspectcoo.SetAspect(aspect);

	if(p_Tex == NULL)
	{
		p_Tex = g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev,effectfilename.c_str());
	}

	if( count < ANIME_ATTACK_STEP)
	{

		if(count < ANIME_ATTACK_STEP*ATTACKDEFAULT_T_FUMIKIRI)
		{

		}
		else
		{
			int step = ATTACKDEFAULTTEXHORSEP*(count-ATTACKDEFAULT_T_FUMIKIRI*ANIME_ATTACK_STEP)/(ANIME_ATTACK_STEP*(1-ATTACKDEFAULT_T_FUMIKIRI));

			//エフェクト描画
			DO.setTexture(p_Tex,ATTACKDEFAULTTEXSIZE,ATTACKDEFAULTTEXSIZE);	
			
			/*
			DO.CenterX = aspectcoo.x*MAPTEXBOXSIZE*MAPTEXPOWER +
				MAPDRAWCENTERX + MAPTEXBOXSIZE*MAPTEXPOWER*(pAttacker->placeX - pAttacker->mapForcus.x);
			DO.CenterY = -MAPTEXBOXSIZE*MAPTEXPOWER/4+
				aspectcoo.y*MAPTEXBOXSIZE*MAPTEXPOWER +
				MAPDRAWCENTERY + MAPTEXBOXSIZE*MAPTEXPOWER*(pAttacker->placeY - pAttacker->mapForcus.y);
			*/

			DO.CenterX = 
				MAPDRAWCENTERX + MAPTEXBOXSIZE*MAPTEXPOWER*(pLand->placeX - pAttacker->mapForcus.x);
			DO.CenterY = -MAPTEXBOXSIZE*MAPTEXPOWER/4+
				MAPDRAWCENTERY + MAPTEXBOXSIZE*MAPTEXPOWER*(pLand->placeY - pAttacker->mapForcus.y);
			
			DO.Width = 
				ATTACKDEFAULTTEXSIZE*ATTACKDEFAULTTEXPOWER/ATTACKDEFAULTTEXHORSEP;
			DO.Height =
				ATTACKDEFAULTTEXSIZE*ATTACKDEFAULTTEXPOWER/ATTACKDEFAULTTEXVERSEP;

			DO.m_TexRange.setLTRB((step)/ATTACKDEFAULTTEXHORSEP, (vi)/ATTACKDEFAULTTEXVERSEP,
								(step+1)/ATTACKDEFAULTTEXHORSEP, (vi+1)/ATTACKDEFAULTTEXVERSEP);

			DO.Rotation = 360.0*pAttacker->aspect/8;
			DO.Draw(pDev);
		}
	}

	return true;
}

bool caAttack::isOnScreen()
{


	if(	sg_pDungeonSystem->キャラクター目視可(pAttacker) )
	{
		return true;
	}


	return false;
}


//------------------------------------------------
//
//

caDefense::caDefense()
{
	End = false;
	count = 0;
	speed = 1;
	p_Tex = NULL;
	SE_playID = -1;
	SE_playcount = false;
}

caDefense::~caDefense()
{
}


int caDefense::process(cRenderDevice *pDev)
{
	//---------------------------------------
	//	checkpointに始点（中間点）終点の順にいれてから処理を行う
	//---------------------------------------
	count += speed;

	if( count < ANIME_ATTACK_STEP)
	{

		if(count < ANIME_ATTACK_STEP*ATTACKDEFAULT_T_FUMIKIRI)
		{

		}
		else
		{
			if(SE_playcount == false)
			{
				g_GameEnv.m_SoundManager.playSoundEffect(SE_playID);
				SE_playcount = true;
			}
		}
	}
	else
	{

		End = true;
	
	}
	return true;
}

int caDefense::Draw(cRenderDevice *pDev)
{

	cDrawingObject DO;


	if(p_Tex == NULL)
	{
		p_Tex = g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev,effectfilename.c_str());
	}

	if( count < ANIME_ATTACK_STEP)
	{

		if(count < ANIME_ATTACK_STEP*ATTACKDEFAULT_T_FUMIKIRI)
		{

		}
		else
		{
			int step = ATTACKDEFAULTTEXHORSEP*(count-ATTACKDEFAULT_T_FUMIKIRI*ANIME_ATTACK_STEP)/(ANIME_ATTACK_STEP*(1-ATTACKDEFAULT_T_FUMIKIRI));

			DO.setTexture(p_Tex,ATTACKDEFAULTTEXSIZE,ATTACKDEFAULTTEXSIZE);	
			DO.CenterX = 
				MAPDRAWCENTERX + MAPTEXBOXSIZE*MAPTEXPOWER*(pDefenser->placeX - pDefenser->mapForcus.x);
			DO.CenterY = -MAPTEXBOXSIZE*MAPTEXPOWER/4+
				MAPDRAWCENTERY + MAPTEXBOXSIZE*MAPTEXPOWER*(pDefenser->placeY - pDefenser->mapForcus.y);
	
			DO.Width = 
				ATTACKDEFAULTTEXSIZE*ATTACKDEFAULTTEXPOWER/ATTACKDEFAULTTEXHORSEP;
			DO.Height =
				ATTACKDEFAULTTEXSIZE*ATTACKDEFAULTTEXPOWER/ATTACKDEFAULTTEXVERSEP;

			DO.m_TexRange.setLTRB((step)/ATTACKDEFAULTTEXHORSEP, (vi)/ATTACKDEFAULTTEXVERSEP,
								(step+1)/ATTACKDEFAULTTEXHORSEP, (vi+1)/ATTACKDEFAULTTEXVERSEP);

			DO.Rotation = 360.0*pDefenser->aspect/8;
			DO.Draw(pDev);
		}
	}

	return true;
}

bool caDefense::isOnScreen()
{


	if(	sg_pDungeonSystem->キャラクター目視可(pDefenser) )
	{
		return true;
	}


	return false;
}