#include "stdafx.h"

#include "cNumSetWindow.h"

#include "Gameobjects.h"

#define PAGETEXPATH _T("skin\\page.png")
//#define PAGETEXSIZE 32

#define PAGECOUNTNUM 70


cNumSetWindow::cNumSetWindow(void)
{
	max_ = 100;
	min_ = 0;
}

cNumSetWindow::~cNumSetWindow(void)
{

}
void cNumSetWindow::Init(cRenderDevice *pDev,
						 unsigned long int *valuepointer,
						 unsigned long int min,
						 unsigned long int max,
						 int basenum)
{

	first_ = *valuepointer;
	output_ = valuepointer;

	max_ = max;
	min_ = min;
	basenum_ = basenum;

	keta_.resize(getMaxketa(),0);
	ketaCaption_.resize(getMaxketa());

	setKeta(first_ , keta_);

	int letterXnum = getMaxketa()+2;
	int letterYnum = 5;

	cGameWindow::Init(pDev,letterXnum,letterYnum);
}

void cNumSetWindow::Init(cRenderDevice *pDev, int letterXnum, int letterYnum)
{
	//Init(pDev);
}

int cNumSetWindow::Draw(cRenderDevice *pDev)
{
	cGameWindow::Draw(pDev);

	return true;
}
int cNumSetWindow::TextDraw(cRenderDevice *pDev)
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
		m_SpriteText.DrawText(Text());

		int i,size;

		size = ketaCaption_.size();
		for(i=0;i<size;i++)
		{
			m_SpriteText.SetCursolPos((letterXlength-i-1)*GWFONTSIZE, linetop(3));
			m_SpriteText.DrawText(ketaCaption_[i]);		
		}

		size = ketaCaption_.size();
		for(i=0;i<size;i++)
		{
			m_SpriteText.SetCursolPos((letterXlength-i-1)*GWFONTSIZE, linetop(2));
			m_SpriteText.DrawText(setStyle(keta_[i],_T("%X")));		
		}

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

int cNumSetWindow::pageDraw(cRenderDevice *pDev)
{

	cDrawingObject DO;
	DO.setTexture(m_pTexture_Page,PAGETEXSIZE,PAGETEXSIZE);
	DO.m_color = color;

	DO.m_TexRange.setLTRB(1/2.0,0/2.0
							,2/2.0,1/2.0);

	int dif = m_pageAnimeCount*6/PAGECOUNTNUM;
	int drawX = Left() + EDGEWIDTH + (letterXlength-cursolX_-1)*GWFONTSIZE + GWFONTSIZE/4;
	int drawY = CenterY - PAGETEXSIZE/4 + 24 +dif;


	//右矢印描画
	DO.setLTRB(drawX,drawY,drawX+PAGETEXSIZE/2,drawY+PAGETEXSIZE/2);
	DO.Rotation = -90;
	DO.Draw(pDev);
	

	DO.m_TexRange.setLTRB(0/2.0,0/2.0
							,1/2.0,1/2.0);

	dif = -m_pageAnimeCount*6/PAGECOUNTNUM;
	drawY = CenterY - PAGETEXSIZE/4 -24 +dif;


	//左矢印描画
	DO.setLTRB(drawX,drawY,drawX+PAGETEXSIZE/2,drawY+PAGETEXSIZE/2);

	DO.Draw(pDev);


	return true;
}
int cNumSetWindow::process(cRenderDevice *pDev)
{
	int i;

	if((g_pPlayerInput()->migi().justOn) || (g_pPlayerInput()->migi().on && g_pPlayerInput()->migi().onCount > 15 && !(g_pPlayerInput()->migi().onCount%5)))
	{
		cursolX_--;
	}
	if((g_pPlayerInput()->hidari().justOn) || (g_pPlayerInput()->hidari().on && g_pPlayerInput()->hidari().onCount > 15 && !(g_pPlayerInput()->hidari().onCount%5)))
	{
		cursolX_++;
	}
	int maxketa = getMaxketa();
	if(cursolX_ < 0) cursolX_ = 0;
	if(cursolX_ >= maxketa) cursolX_ = maxketa-1;

	if(*output_ < min_) *output_ = min_;//注意
	if(*output_ > max_) *output_ = max_;

	if((g_pPlayerInput()->ue().justOn) || (g_pPlayerInput()->ue().on && g_pPlayerInput()->ue().onCount > 15 && !(g_pPlayerInput()->ue().onCount%5)))
	{
		keta_[cursolX_]++;
	}
	if((g_pPlayerInput()->shita().justOn) || (g_pPlayerInput()->shita().on && g_pPlayerInput()->shita().onCount > 15 && !(g_pPlayerInput()->shita().onCount%5)))
	{
		keta_[cursolX_]--;
	}

	if(keta_[cursolX_] < 0) keta_[cursolX_] = 0;
	if(keta_[cursolX_] >= basenum_) keta_[cursolX_] = basenum_-1;

	*output_ = getKeta(keta_);

	if(*output_ < min_) *output_ = min_;//注意
	if(*output_ > max_) *output_ = max_;

	setKeta(*output_ , keta_);


	if( g_pPlayerInput()->migi().justOn || (g_pPlayerInput()->migi().on && g_pPlayerInput()->migi().onCount > 15 && !(g_pPlayerInput()->migi().onCount%5))
		|| g_pPlayerInput()->hidari().justOn || (g_pPlayerInput()->hidari().on && g_pPlayerInput()->hidari().onCount > 15 && !(g_pPlayerInput()->hidari().onCount%5))
		|| (g_pPlayerInput()->ue().justOn) || (g_pPlayerInput()->ue().on && g_pPlayerInput()->ue().onCount > 15 && !(g_pPlayerInput()->ue().onCount%5))
		|| (g_pPlayerInput()->shita().justOn) || (g_pPlayerInput()->shita().on && g_pPlayerInput()->shita().onCount > 15 && !(g_pPlayerInput()->shita().onCount%5))
		)
	{
		playsound_move();
		ReRendarText();
	}

	if(g_pPlayerInput()->cancel().justOn)
	{
		playsound_cancel();
		
		closeLayer = true;	
	}

	if(g_pPlayerInput()->decision().justOn)
	{
		playsound_decide();
		pcommand_->Action(pDev);
		closeLayer = true;	
	}

	cGameWindow::process(pDev);
	return true;
}
void cNumSetWindow::setKeta(const unsigned long int val, vector<int> output)
{
	unsigned long int value = val;
	int i,size = output.size();
	for(i=0;i<size;i++)
	{
		keta_[i] = value % basenum_;
		value /= basenum_;
	}
}
unsigned long int cNumSetWindow::getKeta(const vector<int> keta)
{
	unsigned long int value = 0;
	int i,size = keta.size();
	for(i=0;i<size;i++)
	{
		value *= basenum_;
		value += keta[size-1-i];
	}
	return value;
}
void cNumSetWindow::setKetaCaption(const tstring& cap, int keta)
{
	int maxketa = getMaxketa();
	if(keta < 0) keta = 0;
	if(keta >= maxketa) keta = maxketa-1;
	ketaCaption_[keta] = cap;
}
int cNumSetWindow::getMaxketa()
{
	return MAXKETA_BINARY*log(2.0)/log((double)basenum_) + 1;
}