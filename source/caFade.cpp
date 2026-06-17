#include "stdafx.h"

#include "caFade.h"

#include "cFloor.h"

#include "cGameWindow.h"

caFade::caFade()
{
	End = false;
	count = 0;
	speed = 1;
}

caFade::~caFade()
{
}


int caFade::process(cRenderDevice *pDev)
{
	//---------------------------------------
	//	checkpointに始点（中間点）終点の順にいれてから処理を行う
	//---------------------------------------
	count += speed;

	if(inorout == ANIME_FADE_OUT)
	{
		if( count < ANIME_FADE_STEP_OUT)
		{


		}
		else
		{

			End = true;
		
		}
	}
	else
	{
		if( count < ANIME_FADE_STEP_OUT || count < ANIME_FADE_STEP_IN)
		{


		}
		else
		{

			End = true;
		
		}
	}
	return true;
}
int caFade::Init(int OutorIn)
{
	inorout = OutorIn;

	if(inorout == ANIME_FADE_IN)
	{
		m_SpriteText.Init(sg_pDungeonSystem->pDevice_D3D,1,512,128);
		m_SpriteText.SetReturnFontMargin(512);
		m_SpriteText.SetCursolPos(0,0);

		//文字
		m_SpriteText.CleatText();
		m_SpriteText.SetWeight(GWFONTWEIGHT);
		m_SpriteText.BeginText( g_fontstyle().c_str(), 32 );
		m_SpriteText.SetAntiAlias(1);
		m_SpriteText.SetReturnFontMargin(512);
		m_SpriteText.SetColor( D3DCOLOR_ARGB(255,255,255,255) );
		m_SpriteText.SetReturnWidth(512);
	

		StyleString sstring = sg_pDungeonSystem->pDungeon()->DungeonName_;
		int TextWidth = m_SpriteText.GetTextWidth(sstring.c_str());
		if(TextWidth > 500)
		{
			m_SpriteText.FitTextWidth(sstring,500);
			TextWidth = 500;
		}
		m_SpriteText.SetCursolPos((512-TextWidth)/2, 0);

		m_SpriteText.DrawText(sstring);
		
		if(!sg_pDungeonSystem->拠点フラグ())
		{
			//階層表示
			StyleString sstr = g_Lang(_T("第"));
			sstr += setStyle(sg_pDungeonSystem->FloorLevel(),_T("%2d"));
			sstr += g_Lang(_T("階層"));
			int TextWidth = m_SpriteText.GetTextWidth(sstr.c_str());
			
			m_SpriteText.SetCursolPos((512-TextWidth)/2, 32+10);

			m_SpriteText.DrawText(sstr);
		}
		
		m_SpriteText.EndText();

	}
	return true;
}
int caFade::Draw(cRenderDevice *pDev)
{

	cDrawingObject DO;

	DO.setLTRB(0,0,800,600);

	if(inorout == ANIME_FADE_OUT)
	{
		DO.m_color.ARGB(255,0,0,0);
		DO.m_color.alpha = 255 - 255*(ANIME_FADE_STEP_OUT-count*1.05+1)/ANIME_FADE_STEP_OUT;

		DO.Draw(pDev);
	}
	else
	{
		DO.m_color.ARGB(255,0,0,0);
		DO.m_color.alpha = 255*(ANIME_FADE_STEP_OUT-count)/ANIME_FADE_STEP_OUT;

		DO.Draw(pDev);

		DO.m_color.ARGB(255,255,255,255);
		DO.m_color.alpha = 512*(ANIME_FADE_STEP_IN-count)/ANIME_FADE_STEP_IN;

		DO.setTexture(m_SpriteText.GetTex());
		DO.m_TexRange.setLTRB(0,0,1,1);
		DO.Width = 512;
		DO.Height = 128;
		DO.CenterX = MAPDRAWCENTERX;
		DO.CenterY = MAPDRAWCENTERY-100;

		DrawAddShadowDO_mini(DO,pDev);
	}


	return true;
}

bool caFade::isOnScreen()
{

	return true;

}
//-------------------------------------------------------------------
caFadeGotoDungeon::caFadeGotoDungeon()
{
	End = false;
	count = 0;
	speed = 1;
	mode_ = 0;
}

caFadeGotoDungeon::~caFadeGotoDungeon()
{
}

int caFadeGotoDungeon::process(cRenderDevice *pDev)
{
	count += speed;

	if(mode_)
	{//1フレーム遅らせる
		End = true;
	}

	if( count < ANIME_FADE_STEP_OUT)
	{

	}
	else
	{
		sg_pDungeonSystem->GotoDungeon(DungeonID, privateFlags, savefileName);
		mode_ = 1;
	}

	return true;
}
int caFadeGotoDungeon::Draw(cRenderDevice *pDev)
{

	cDrawingObject DO;

	DO.setLTRB(0,0,800,600);

	DO.m_color.ARGB(255,0,0,0);
	DO.m_color.alpha = 255 - 255*(ANIME_FADE_STEP_OUT-count*1.05+1)/ANIME_FADE_STEP_OUT;

	DO.Draw(pDev);

	return true;
}

bool caFadeGotoDungeon::isOnScreen()
{
	return true;
}

//-------------------------------------------------------------------
caFadeContinueDungeon::caFadeContinueDungeon()
{
	End = false;
	count = 0;
	speed = 1;
	mode_ = 0;
}

caFadeContinueDungeon::~caFadeContinueDungeon()
{
}

int caFadeContinueDungeon::process(cRenderDevice *pDev)
{
	count += speed;

	if(mode_)
	{//1フレーム遅らせる
		End = true;
	}

	if( count < ANIME_FADE_STEP_OUT)
	{

	}
	else
	{
		sg_pDungeonSystem->ContinueDungeon(continueQuest);
		mode_ = 1;
	}

	return true;
}
int caFadeContinueDungeon::Draw(cRenderDevice *pDev)
{

	cDrawingObject DO;

	DO.setLTRB(0,0,800,600);

	DO.m_color.ARGB(255,0,0,0);
	DO.m_color.alpha = 255 - 255*(ANIME_FADE_STEP_OUT-count*1.05+1)/ANIME_FADE_STEP_OUT;

	DO.Draw(pDev);

	return true;
}

bool caFadeContinueDungeon::isOnScreen()
{
	return true;
}