#include "stdafx.h"

#include "caRetire.h"

caRetire::caRetire()
{
	End = false;
	count = 0;
	speed = 1;
}

caRetire::~caRetire()
{
}

int caRetire::Init(pcAnimation me)
{
	cAnimation::Init(me);

	Baseopaque = pRetirer->opaque;

	//テクスチャセット
	DO_.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
		sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ring_out.png")),256,256);
	//テクスチャ座標セット
	DO_.m_TexRange.setLTRB(0,0,1,1);


	//基本サイズ
	DO_.Width = 176;
	DO_.Height = 176;

	randres = (double)rand()/RAND_MAX;



	return true;
}

int caRetire::process(cRenderDevice *pDev)
{
	//---------------------------------------
	//	checkpointに始点（中間点）終点の順にいれてから処理を行う
	//---------------------------------------
	count += speed;
	


	if( !End &&  count < ANIME_RETIRE_STEP)
	{
		pRetirer->visibleemotion.insert(EMOTION_ASERI);

		pRetirer->opaque = Baseopaque*(((int)(count/speed)/2) % 2);
	}
	else
	{
		pRetirer->visibleemotion.erase(EMOTION_ASERI);
		pRetirer->opaque = 0;
		//sg_pDungeonSystem->キャラ消去_アニメ用(pRetirer);（不要）
		End = true;
	}


	
	return true;
}
int caRetire::Draw(cRenderDevice *pDev)
{
	c4DVector center;
	center.set(pRetirer->placeX ,pRetirer->placeY ,0,0);
	center = sg_pDungeonSystem->Map().realvisibleplace(center);

	DO_.CenterX = center.x;
	DO_.CenterY = center.y - center.z/2 - pRetirer->GetDrawHeadtall_dotY()/2;


	double s = speed*0.6;
	double d = (double)count/s/ANIME_RETIRE_STEP - (1-s)/s;
	d = max(0,d);
	d = d*d;

	DO_.m_color.HSV(randres*360,1,0.8);
	DO_.m_color.alpha = 100;

	DO_.ScaleX = d*1.1;
	DO_.ScaleY = d*1.1;
	DO_.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;
	DO_.Draw(pDev);

	DO_.Rotation2 = randres*360;
	DO_.ScaleX = d*1.1*1.2;
	DO_.ScaleY = d*1.1*0.4;
	DO_.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;
	DO_.Draw(pDev);

	DO_.m_color.HSV(randres*360+30,0.1,0.9);
	DO_.m_color.alpha = 100;

	DO_.ScaleX = d*1.2;
	DO_.ScaleY = d*0.4;
	DO_.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;
	DO_.Draw(pDev);

	DO_.ScaleX = d;
	DO_.ScaleY = d;
	DO_.Rotation2 = 0;
	DO_.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;
	DO_.Draw(pDev);

	return true;
}
bool caRetire::isOnScreen()
{
	

	if(	sg_pDungeonSystem->Map().isOnDisplay(
		pRetirer->placeX,pRetirer->placeY)
		&& sg_pDungeonSystem->Map().isOnSight(
		pRetirer->足元地形(),sg_pDungeonSystem->pPlayerChara()->足元地形())
		)
	{

		return true;
	}
	
	//sg_pDungeonSystem->キャラ消去_アニメ用(pRetirer);（不要）
	pRetirer->opaque = 0;

	return false;
}