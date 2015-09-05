#include "stdafx.h"

#include "cMassageWindow.h"


#define MASSAGE_SPEED 3

#define MASSAGE_MEMORY_POWER 4

#define MASSAGE_VISIBLETIME 150

#define TEX_SIZE_CURSOL 16

cMassageWindow::cMassageWindow(void)
{
	preRendar = false;

	Waiting = false;

	rolldiff = -1;
	VisibleCount = 0;
}

cMassageWindow::~cMassageWindow(void)
{
	massagelogs.clear();
	rollmassages.clear();
	reservemassages.clear();
}


void cMassageWindow::Init(IDirect3DDevice9 *pDev, int letterXnum, int letterYnum)
{
	Width = letterXnum * FONTSIZE + EDGEWIDTH*2;
	Height = letterYnum * FONTSIZE + EDGEWIDTH*2;

	m_pTexture_Window = g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev, WINDOWTEXPATH);



	int iws = letterXnum * FONTSIZE;
	int ihs = letterYnum * FONTSIZE *MASSAGE_MEMORY_POWER;//＊です
	int tw = iws;
	int th = ihs;
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


	m_SpriteText.Init(pDev, 1, tw, th);
	m_SpriteText.SetCursolPos(0,0);

	forcusTop = m_SpriteText.GetY();

	textmaxWidth = iws;
	lineNum = letterYnum;

	pTex_cursol = g_GameEnv.m_Resourse->getTextureFromFile(pDev,TEXT("interface\\cursor.png"));

}

int cMassageWindow::addMassage(StyleString massage)
{
	StyleString * m = new StyleString;
	*m = massage;
	psstring pm = psstring(m);
	massagelogs.push_back(pm);//ログ

	reservemassages.push_back(pm);//予約

	VisibleCount = MASSAGE_VISIBLETIME;
	return true;
}

int cMassageWindow::process(IDirect3DDevice9 *pDev)
{

	if(m_SpriteText.GetY() - forcusTop > (lineNum)*FONTSIZE)
	{
		forcusTop += MASSAGE_SPEED;
		if(m_SpriteText.GetY() - forcusTop < (lineNum)*FONTSIZE)
		{
			forcusTop = m_SpriteText.GetY() - (lineNum)*FONTSIZE;
		}
	}
	else
	{
		VisibleCount--;

		if(VisibleCount < 0)
		{
			VisibleCount = 0;
		}
		if(VisibleCount == 0)
		{
			textclear(pDev);
		}
	}

	return true;
}

int cMassageWindow::Draw(IDirect3DDevice9 *pDev)
{
	if(!VisibleCount) return false;

	WindowDraw(pDev);
	TextDraw(pDev);

	if(Waiting)
	{
	
		cDrawingObject DO;
		DO.setTexture(pTex_cursol,TEX_SIZE_CURSOL,TEX_SIZE_CURSOL);

		DO.m_TexRange.setLTRB(1,0,0,1);
		DO.Width = DO.Height = TEX_SIZE_CURSOL;

		DO.CenterX = sg_pDungeonSystem->Massage.CenterX;
		DO.CenterY = sg_pDungeonSystem->Massage.Bottom()-TEX_SIZE_CURSOL/2;

		DO.Rotation = -90;

		DO.Draw(pDev);
	}

	return true;
}

int cMassageWindow::RenderText(IDirect3DDevice9 *pDev)
{



	if(m_SpriteText.GetY() - forcusTop > (lineNum)*FONTSIZE)
	{
		return false;
	}


	m_SpriteText.BeginText( TEXT("ＭＳ ゴシック"), FONTSIZE );
	{
		m_SpriteText.SetAntiAlias(1);
		m_SpriteText.SetColor( D3DCOLOR_ARGB(255,255,255,255) );
		m_SpriteText.SetReturnWidth(textmaxWidth - m_SpriteText.GetX());
		
		if(reservemassages.size())
		{
			int prey = m_SpriteText.GetY();
			m_SpriteText.DrawText(*reservemassages[0]);
			if(m_SpriteText.GetY() >= (MASSAGE_MEMORY_POWER-1)*lineNum*FONTSIZE)
			{
				if(rolldiff == -1)
				{
					rolldiff = prey;
				}
				rollmassages.push_back(reservemassages[0]);//ロール用
			}
			reservemassages.pop_front();
		}
	}
	m_SpriteText.EndText();



	if(lineNum*FONTSIZE*MASSAGE_MEMORY_POWER < m_SpriteText.GetY())//テクスチャの外
	{
		NewRoll(pDev);
	}

	return true;
}
int cMassageWindow::TextDraw(IDirect3DDevice9 *pDev)
{
	


	if(reservemassages.size())
	{
		RenderText(pDev);
	}



	TextDrawSub(pDev);

	return true;
}
int cMassageWindow::TextDrawSub(IDirect3DDevice9 *pDev)
{
	int l = Left();
	int t = Top();
	int r = l + Width;
	int b = t + Height;

	double ew = EDGEWIDTH;


	int iws = Width - EDGEWIDTH*2;
	int ihs = Height - EDGEWIDTH*2;
	int tw = iws;
	int th = ihs;
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


	m_DO.setTexture(m_SpriteText.GetTex(), tw, th);
	

	m_DO.m_color = color;
	m_DO.setLTRB(			l + ew,		t + ew,		r - ew,		b - ew);
	m_DO.m_TexRange.setLTRB(0,		forcusTop/(double)th/MASSAGE_MEMORY_POWER,		iws / (double)tw ,	forcusTop/(double)th/MASSAGE_MEMORY_POWER + (ihs / (double)th)/MASSAGE_MEMORY_POWER);
	m_DO.Draw(pDev);
	
	
	//デバック用
	
	m_DO.setLTRB(			l + ew + 500,		t + ew,		r - ew + 500,		b - ew);
	m_DO.m_TexRange.setLTRB(0,	0,		iws / (double)tw ,	(ihs / (double)th));
	m_DO.Draw(pDev);
	
	
	return true;

}


int cMassageWindow::NewRoll(IDirect3DDevice9 *pDev)
{

	m_SpriteText.CleatText();
	m_SpriteText.SetCursolPos(0,0);
	forcusTop = forcusTop - rolldiff;

	int i;
	m_SpriteText.BeginText( TEXT("ＭＳ ゴシック"), FONTSIZE );
	{
		m_SpriteText.SetAntiAlias(1);
		m_SpriteText.SetColor( D3DCOLOR_ARGB(255,255,255,255) );
		m_SpriteText.SetReturnWidth(textmaxWidth - m_SpriteText.GetX());
		
		for(i=0;i<rollmassages.size();i++)
		{
			m_SpriteText.DrawText(*rollmassages[i]);
		}
	}
	m_SpriteText.EndText();

	rolldiff = -1;
	rollmassages.clear();
	return true;
}


int cMassageWindow::textclear(IDirect3DDevice9 *pDev)
{
	m_SpriteText.CleatText();
	m_SpriteText.SetCursolPos(0,0);
	forcusTop = m_SpriteText.GetY();
	rollmassages.clear();
	reservemassages.clear();
	rolldiff = -1;
	return true;
}

int cMassageWindow::Wait()
{
	return Waiting = true;
}
int cMassageWindow::unWait()
{
	return Waiting = false;
}