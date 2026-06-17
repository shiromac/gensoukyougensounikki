#include "stdafx.h"

#include "cGameWindow.h"





#define PAGECOUNTNUM 40

cGameWindow::cGameWindow(void)
{
	vText.push_back(_T(""));

	m_pageAnimeCount = 0;

	preRendar = false;
	Text() = _T("");
	color.ARGB(255,255,255,255);

	letterXlength = 1;
	letterYlength = 1;

	enable = true;
	closeLayer = FALSE;
	pageIndex = 0;


}

cGameWindow::~cGameWindow(void)
{
}

void cGameWindow::InitBasic(cRenderDevice *pDev, int letterXnum, int letterYnum)
{
	letterXlength = letterXnum;
	letterYlength = letterYnum;

	Width = letterXnum * GWFONTSIZE + sideEdgeWith()*2;
	Height = letterYnum * lineheight() + EDGEWIDTH*2;

	m_pTexture_Window = g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev, WINDOWTEXPATH);
	m_pTexture_Cursor = g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev, CURSORTEXPATH);
	m_pTexture_Page =  g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev, PAGETEXPATH);


	
	soundIndex_decide = g_GameEnv.m_SoundManager.getSoundEffectFromFile(SOUND_SELECTDECIDE);
	soundIndex_cancel = g_GameEnv.m_SoundManager.getSoundEffectFromFile(SOUND_SELECTCANCEL);
	soundIndex_move = g_GameEnv.m_SoundManager.getSoundEffectFromFile(SOUND_SELECTMOVE);



	int iws = letterXnum * GWFONTSIZE;
	int ihs = letterYnum * lineheight();
	int tw = iws;
	int th = ihs;


	tw = largerMinPow2(tw);//tw以上の最小の2^n
	th = largerMinPow2(th);//th以上の最小の2^n

	/*
	int i;
	for(i=0;i<64;i++)
	{
		if(!((tw-1) >> i)) break;
	}
	tw = 1<<i;//tw以上の最小の2^n
	for(i=0;i<64;i++)
	{
		if(!((th-1) >> i)) break;
	}
	th = 1<<i;//th以上の最小の2^n
	*/


	m_SpriteText.Init(pDev, 1, tw, th);
	m_SpriteText.SetReturnFontMargin(merginheight());
	m_SpriteText.SetCursolPos(0,0);
	m_SpriteText.SetWeight(GWFONTWEIGHT);
	m_SpriteText.BeginText( g_fontstyle().c_str(), GWFONTSIZE );
	m_SpriteText.EndText();
	m_SpriteText.SetAntiAlias(1);
	m_SpriteText.SetReturnFontMargin(merginheight());
	m_SpriteText.SetColor( D3DCOLOR_ARGB(255,255,255,255) );
	m_SpriteText.SetRot(0.0);
	m_SpriteText.SetSize(cRenderVector2(1,1));

	
	m_pTexture_Page = g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev, PAGETEXPATH);

	//pageIndexMax = (commandList.size()-1)/cursolIndexMax + 1;
	pageIndex = 0;

	m_pageText.Init(pDev, 1, EDGEWIDTH*8, EDGEWIDTH);

}


int cGameWindow::pageDraw(cRenderDevice *pDev)
{
	if(pageIndexMaxF() != 0)
	{


		cDrawingObject DO;
		DO.setTexture(m_pTexture_Page,PAGETEXSIZE,PAGETEXSIZE);
		DO.m_color = color;

		DO.m_TexRange.setLTRB(1/2.0,0/2.0
								,2/2.0,1/2.0);

		int dif = m_pageAnimeCount*10/PAGECOUNTNUM;
		int drawX = CenterX - 50 -PAGETEXSIZE/4;
		int drawY = Bottom() - PAGETEXSIZE/2;

		if(pageIndex > 0)
		{
			//右矢印描画
			DO.setLTRB(drawX-dif,drawY,drawX+PAGETEXSIZE/2-dif,drawY+PAGETEXSIZE/2);

			DO.Draw(pDev);
		}

		DO.m_TexRange.setLTRB(0/2.0,0/2.0
								,1/2.0,1/2.0);

		dif = -m_pageAnimeCount*10/PAGECOUNTNUM;
		drawX = CenterX + 50 -PAGETEXSIZE/4;
		drawY = Bottom() - PAGETEXSIZE/2;

		if(pageIndex < pageIndexMaxF() -1)
		{
			//左矢印描画
			DO.setLTRB(drawX-dif,drawY,drawX+PAGETEXSIZE/2-dif,drawY+PAGETEXSIZE/2);
	
			DO.Draw(pDev);
		}

		if(pageIndexMaxF() >= 2)
		{
			//ページ描画
			DO.setTexture(m_pageText.GetTex(),EDGEWIDTH*8,EDGEWIDTH);
			DO.m_TexRange.setLTRB(0,0,1,1);
			drawX = CenterX -PAGETEXSIZE/4 - EDGEWIDTH/2;
			drawY = Bottom() - PAGETEXSIZE/2;

			int i;
			
			
			for(i=0;i<4;i++)
			{
				cCoordinate coo;
				coo.SetAspect(i*2+1);
				DO.setLTRB(drawX+coo.x,drawY+coo.y,drawX+EDGEWIDTH*8+coo.x,drawY+EDGEWIDTH+coo.y);
				DO.m_color = color;
				DO.Draw(pDev);
			}
			

			for(i=0;i<4;i++)
			{
				cCoordinate coo;
				coo.SetAspect(i*2);
				DO.setLTRB(drawX+coo.x,drawY+coo.y,drawX+EDGEWIDTH*8+coo.x,drawY+EDGEWIDTH+coo.y);
				DO.m_color = color;
				DO.Draw(pDev);
			}

			DO.setLTRB(drawX,drawY,drawX+EDGEWIDTH*8,drawY+EDGEWIDTH);
			DO.m_color.ARGB(255,0,120,110);
			DO.m_color *= color;
			DO.Draw(pDev);

		}

	}

	return true;
}
void cGameWindow::Init(int letterXnum, int letterYnum)
{
	
	Init(sg_pDungeonSystem->pDevice_D3D,letterXnum,letterYnum);

}
void cGameWindow::Init(cRenderDevice *pDev, int letterXnum, int letterYnum)
{
	InitBasic(pDev,letterXnum,letterYnum);
}


int cGameWindow::ReRendarText()
{
	preRendar = false;
	m_SpriteText.SetCursolPos(0,0);
	return true;
}

int cGameWindow::process(cRenderDevice *pDev)
{



	int tempP = pageIndex;
	pageIndex += (g_pPlayerInput()->migi().justOn || g_pPlayerInput()->decision().justOn);
	pageIndex -= (g_pPlayerInput()->hidari().justOn);


	if(tempP != pageIndex)
	{
		playsound_move();
		preRendar = false;
	}
	if(pageIndex < 0) pageIndex = pageIndexMaxF() - 1;
	if(pageIndex >= pageIndexMaxF()) pageIndex = 0;


	

	m_pageAnimeCount++;
	if(m_pageAnimeCount > PAGECOUNTNUM) m_pageAnimeCount = 0;

	return true;
}


int cGameWindow::Draw(cRenderDevice *pDev)
{
	
	WindowDraw(pDev);
	TextDraw(pDev);
	pageDraw(pDev);
	return true;
}

int cGameWindow::TextDraw(cRenderDevice *pDev)
{
	if(preRendar == false)
	{
		m_SpriteText.CleatText();
		m_SpriteText.SetWeight(GWFONTWEIGHT);
		m_SpriteText.BeginText( g_fontstyle().c_str(), GWFONTSIZE );
		m_SpriteText.SetAntiAlias(1);
		m_SpriteText.SetReturnFontMargin(merginheight());
		m_SpriteText.SetColor( D3DCOLOR_ARGB(255,255,255,255) );
		m_SpriteText.SetReturnWidth(Width - EDGEWIDTH*2);
		m_SpriteText.SetCursolPos(0, linetop(0));
		m_SpriteText.DrawText(Text(pageIndex));
		m_SpriteText.EndText();
		preRendar = true;


		TCHAR t[MAXBUFFSIZE];
		//ページインデックス
		m_pageText.CleatText();
		m_pageText.SetWeight(GWFONTWEIGHT);
		m_pageText.BeginText( g_fontstyle().c_str(), EDGEWIDTH);
		m_pageText.SetAntiAlias(1);
		m_pageText.SetCursolPos(0,0);
		m_pageText.SetColor( D3DCOLOR_ARGB(255,255,255,255) );
		m_pageText.SetReturnWidth(1024);//自動returnしない
		_stprintf(t ,_T("%d_/_%d"),pageIndex+1,pageIndexMaxF());
		m_pageText.DrawText(t);

		m_pageText.EndText();
	}

	TextDrawSub(pDev);
	return true;
}

int cGameWindow::TextDrawSub(cRenderDevice *pDev)
{
	int l = Left();
	int t = Top();
	int r = Right();
	int b = Bottom();

	double ew = EDGEWIDTH;
	double ew_s = sideEdgeWith();

	int margin = GWFONTSIZE;//文字がはみ出さないように

	int iws = Width - ew_s*2 + margin;
	int ihs = Height - ew*2;
	int tw = letterXlength * GWFONTSIZE;
	int th = letterYlength * lineheight();
	int i;
	tw = largerMinPow2(tw);//tw以上の最小の2^n
	th = largerMinPow2(th);//th以上の最小の2^n



	m_DO.setTexture(m_SpriteText.GetTex(), tw, th);
	

	m_DO.setLTRB(			l + ew_s,		t + ew,		r - ew_s + margin,		b - ew);
	m_DO.m_TexRange.setLTRB(0,		0,		iws / (double)tw ,	ihs / (double)th);
	
	m_DO.m_color = color;
	DrawAddShadowDO(m_DO,pDev);


	return true;

}


int cGameWindow::WindowDraw(cRenderDevice *pDev)
{

	int l = Left();
	int t = Top();
	int r = l + Width;
	int b = t + Height;

	double ew = EDGEWIDTH*1.5;
	double wts = WINDOWTEXSIZE;

	m_DO.setTexture(m_pTexture_Window, WINDOWTEXSIZE, WINDOWTEXSIZE);
	m_DO.m_color = color;

	m_DO.setLTRB(			l,			t,			l + ew,		t + ew);
	m_DO.m_TexRange.setLTRB(0/wts,		0/wts,		ew/wts,		ew/wts);
	m_DO.Draw(pDev);
	m_DO.setLTRB(			l + ew,		t + ew,		r - ew,		b - ew);
	m_DO.m_TexRange.setLTRB(ew/wts,		ew/wts,		(wts-ew)/wts,	(wts-ew)/wts);
	m_DO.Draw(pDev);
	m_DO.setLTRB(			r - ew,		b - ew,		r ,			b	);
	m_DO.m_TexRange.setLTRB((wts-ew)/wts,	(wts-ew)/wts,	1,	1);
	m_DO.Draw(pDev);

	m_DO.setLTRB(			l + ew,		t,			r - ew,		t + ew);
	m_DO.m_TexRange.setLTRB(ew/wts,		0/wts,		(wts-ew)/wts,		ew/wts);
	m_DO.Draw(pDev);
	m_DO.setLTRB(			l + ew,		b - ew,		r - ew,		b);
	m_DO.m_TexRange.setLTRB(ew/wts,		(wts-ew)/wts,	(wts-ew)/wts,	1);
	m_DO.Draw(pDev);

	m_DO.setLTRB(			l,			t + ew,		l + ew,		b - ew);
	m_DO.m_TexRange.setLTRB(0/wts,		ew/wts,		ew/wts,	(wts-ew)/wts);
	m_DO.Draw(pDev);
	m_DO.setLTRB(			r - ew,		t + ew,		r,		b - ew);
	m_DO.m_TexRange.setLTRB((wts-ew)/wts,	ew/wts,	1,		(wts-ew)/wts);
	m_DO.Draw(pDev);

	m_DO.setLTRB(			l,			b - ew,		l + ew,		b);
	m_DO.m_TexRange.setLTRB(0/wts,		(wts-ew)/wts,	ew/wts,	1);
	m_DO.Draw(pDev);
	m_DO.setLTRB(			r - ew,		t,		r,		t + ew);
	m_DO.m_TexRange.setLTRB((wts-ew)/wts,	0/wts,	1,	ew/wts);
	m_DO.Draw(pDev);
	return true;
}
void DrawAddShadowDO(cDrawingObject& DO, cRenderDevice *pDev)
{
	int i;
	int drawX = DO.CenterX;
	int drawY = DO.CenterY;
	const cColor color = DO.m_color;

	if(sg_pDungeonSystem->EV_charEdgedFlag)
	{

		for(i=0;i<5;i++)
		{
			cCoordinate coo;
			switch(i){
			case 0:
				coo.x = 0;
				coo.y = 0;
					break;
			case 1:
				coo.x = 0;
				coo.y = 1;
					break;
			case 2:
				coo.x = 1;
				coo.y = 0;
					break;
			case 3:
				coo.x = -1;
				coo.y = 1;
					break;
			case 4:
				coo.x = 1;
				coo.y = -1;
					break;
			}
			coo.x += 2;
			coo.y += 2;
			//影ドロー
			DO.m_color.ARGB(32*color.alpha/255,0,0,0);
			DO.CenterX = drawX+coo.x;
			DO.CenterY = drawY+coo.y;
			DO.Draw(pDev);
		}
		

		
		//エッジドロー
		for(i=0;i<16;i++)
		{
			cCoordinate coo;
			switch(i){
			case 0:
				coo.x = 0;
				coo.y = -2;
					break;
			case 1:
				coo.x = 1;
				coo.y = -2;
					break;
			case 2:
				coo.x = 2;
				coo.y = -1;
					break;
			case 3:
				coo.x = 2;
				coo.y = 0;
					break;
			case 4:
				coo.x = 2;
				coo.y = 1;
					break;
			case 5:
				coo.x = 1;
				coo.y = 2;
					break;
			case 6:
				coo.x = 0;
				coo.y = 2;
					break;
			case 7:
				coo.x = -1;
				coo.y = 2;
					break;
			case 8:
				coo.x = -2;
				coo.y = 1;
					break;
			case 9:
				coo.x = -2;
				coo.y = 0;
					break;
			case 10:
				coo.x = -2;
				coo.y = -1;
					break;
			case 11:
				coo.x = -1;
				coo.y = -2;
					break;
			case 12:
				coo.x = -1;
				coo.y = -1;
					break;
			case 13:
				coo.x = 1;
				coo.y = 1;
					break;
			case 14:
				coo.x = 1;
				coo.y = -1;
					break;
			case 15:
				coo.x = -1;
				coo.y = 1;
					break;
			}
			//coo.x += 1;
			//coo.y += 1;
			DO.CenterX = drawX+coo.x;
			DO.CenterY = drawY+coo.y;
			DO.m_color.ARGB(64*color.alpha/255,32,32,32);
			DO.m_color *= color;
			DO.Draw(pDev);
		}
		
		
		//エッジドロー2
		for(i=0;i<4;i++)
		{
			cCoordinate coo;
			coo.SetAspect(i*2);
			DO.CenterX = drawX+coo.x;
			DO.CenterY = drawY+coo.y;
			DO.m_color.ARGB(255*color.alpha/255,32,32,32);
			DO.m_color *= color;
			DO.Draw(pDev);
		}
		
		



		/*
		//エッジドロー
		for(i=0;i<8;i++)
		{
			cCoordinate coo;
			coo.SetAspect(i);
			DO.CenterX = drawX+coo.x;
			DO.CenterY = drawY+coo.y;
			DO.m_color.ARGB(255,32,32,32);
			DO.m_color *= color;
			DO.Draw(pDev);
		}
		*/
	}
	else
	{
		//影ドロー
		DO.m_color.ARGB(160*color.alpha/255,0,0,0);
		DO.CenterX += 2;
		DO.CenterY += 2;
		DO.Draw(pDev);
		DO.CenterX -= 2;
		DO.CenterY -= 2;
	}


	DO.m_color = color;
	DO.CenterX = drawX;
	DO.CenterY = drawY;
	DO.Draw(pDev);
	
	
}
void DrawAddShadowDO_mini(cDrawingObject& DO, cRenderDevice *pDev)
{
	int i;
	int drawX = DO.CenterX;
	int drawY = DO.CenterY;
	const cColor color = DO.m_color;

	if(sg_pDungeonSystem->EV_charEdgedFlag)
	{

		for(i=0;i<5;i++)
		{
			cCoordinate coo;
			switch(i){
			case 0:
				coo.x = 0;
				coo.y = 0;
					break;
			case 1:
				coo.x = 0;
				coo.y = 1;
					break;
			case 2:
				coo.x = 1;
				coo.y = 0;
					break;
			case 3:
				coo.x = -1;
				coo.y = 1;
					break;
			case 4:
				coo.x = 1;
				coo.y = -1;
					break;
			}
			coo.x += 1;
			coo.y += 1;
			//影ドロー
			DO.m_color.ARGB(32*color.alpha/255,0,0,0);
			DO.CenterX = drawX+coo.x;
			DO.CenterY = drawY+coo.y;
			DO.Draw(pDev);
		}
		

		
		//エッジドロー
		for(i=0;i<16;i++)
		{
			cCoordinate coo;
			coo.SetAspect(i);
			

			DO.CenterX = drawX+coo.x;
			DO.CenterY = drawY+coo.y;
			DO.m_color.ARGB(64*color.alpha/255,32,32,32);
			DO.m_color *= color;
			DO.Draw(pDev);
		}
		

	}
	else
	{
		//影ドロー
		DO.m_color.ARGB(160*color.alpha/255,0,0,0);
		DO.CenterX += 2;
		DO.CenterY += 2;
		DO.Draw(pDev);
		DO.CenterX -= 2;
		DO.CenterY -= 2;
	}


	DO.m_color = color;
	DO.CenterX = drawX;
	DO.CenterY = drawY;
	DO.Draw(pDev);
	
	
}

void cGameWindow::playsound_decide()
{
	g_GameEnv.m_SoundManager.playSoundEffect(soundIndex_decide);
}
void cGameWindow::playsound_cancel()
{
	g_GameEnv.m_SoundManager.playSoundEffect(soundIndex_cancel);
}
void cGameWindow::playsound_move()
{
	g_GameEnv.m_SoundManager.playSoundEffect(soundIndex_move);
}

void cGameWindow::FitTextWidthToWindow(StyleString& sstr)
{
	m_SpriteText.BeginText( g_fontstyle().c_str(), GWFONTSIZE);
	m_SpriteText.FitTextWidth(sstr,letterXlength*GWFONTSIZE-2);
	m_SpriteText.EndText();
}