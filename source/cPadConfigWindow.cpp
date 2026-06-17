#include "stdafx.h"

#include "cPadConfigWindow.h"

#include "Gameobjects.h"

#define PAGETEXPATH _T("skin\\page.png")
//#define PAGETEXSIZE 32

#define PAGECOUNTNUM 70

cPadConfigWindow::cPadConfigWindow(void)
{
	padsetmode_ = 0;
	padsetmodewait_ = 0;
}

cPadConfigWindow::~cPadConfigWindow(void)
{

}
void cPadConfigWindow::Init(cRenderDevice *pDev)
{
	int letterXnum = 20;
	int letterYnum = 10;

	setCommand();

	settextW_.Text() = g_Lang(_T("ボタン設定説明"));
	settextW_.Init(pDev,12,4);
	settextW_.CenterX = (SCREEN_X/2);
	settextW_.CenterY = (SCREEN_Y/2)-100;


	cSelectWindow::Init(pDev,letterXnum,letterYnum);
}
void cPadConfigWindow::setCommand()
{
	int i;
	StyleString caption,sstr;

	commandList.clear();
	for(i=0;i<8;i++)
	{
		caption = g_Lang(_T("ボタン"));
		caption.addstring(g_GameEnv.m_Input.patInputManager->padconfigI2B[i] + 1);
		
		sstr = _T("ボタン");
		sstr.addstring(i + 1);
		sstr.addstring(_T("Caption"));
		
		commandList.push_back(pcCommand(new cCommandNull(caption + _T("   ") + g_Lang(sstr.c_str())) ));

	}

	commandList.push_back(pcCommand(new cCommandNull(g_Lang(_T("初期化")))));
	commandList.push_back(pcCommand(new cCommandNull(g_Lang(_T("パッドコンフィグ終了")))));

	ReRendarText();
}

void cPadConfigWindow::Init(cRenderDevice *pDev, int letterXnum, int letterYnum)
{
	Init(pDev);
}
StyleString cPadConfigWindow::shortExplanationString(cRenderDevice *pDev)
{
	if(cursolIndex >= 8) return _T("");
	StyleString sstr;
	sstr = _T("ボタン");
	sstr.addstring(cursolIndex + 1);
	sstr.addstring(_T("Explanation"));
	return g_Lang(sstr.c_str());
}

int cPadConfigWindow::Draw(cRenderDevice *pDev)
{
	cSelectWindow::Draw(pDev);

	if(padsetmode_)
	{
		settextW_.Draw(pDev);
	}

	return true;
}

int cPadConfigWindow::process(cRenderDevice *pDev)
{
	int i;

	if(padsetmode_)
	{
	

		if(padsetmodewait_ == 1)
		{
			for(i=0;i<MAX_BUTTONS;i++)
			{
				if(g_GameEnv.m_Input.getrawPadInput(0,i))
				{
					break;
				}
			}
			if(i == MAX_BUTTONS) padsetmodewait_ = 0;
		}
		else
		{
			for(i=0;i<MAX_BUTTONS;i++)
			{
				if(g_GameEnv.m_Input.getrawPadInput(0,i))
				{
					int preB = g_GameEnv.m_Input.patInputManager->padconfigI2B[cursolIndex];
					g_GameEnv.m_Input.patInputManager->padconfigI2B[cursolIndex] = i;
					int k;
					for(k=0;k<MAX_BUTTONS;k++)
					{
						if(cursolIndex != k && g_GameEnv.m_Input.patInputManager->padconfigI2B[k] == i) g_GameEnv.m_Input.patInputManager->padconfigI2B[k] = preB;
					}
					setCommand();
					padsetmode_ = 0;
					padsetmodewait_ = 1;
					break;
				}
			}
		}

		
		if(g_pPlayerInput()->cancel().justOn)
		{

			playsound_cancel();
	
			padsetmode_ = 0;
			padsetmodewait_ = 1;
			return true;
		}
	}
	else
	{
		if(padsetmodewait_ == 1)
		{
			for(i=0;i<MAX_BUTTONS;i++)
			{
				if(g_GameEnv.m_Input.getrawPadInput(0,i))
				{
					break;
				}
			}
			if(i == MAX_BUTTONS) padsetmodewait_ = 0;
		}
		else
		{
			cSelectWindow::process(pDev);

			if(g_pPlayerInput()->decision().justOn)
			{
				if(cursolIndex == 8)
				{
					for(i=0;i<MAX_BUTTONS;i++)
					{
						g_GameEnv.m_Input.patInputManager->padconfigI2B[i] = i;
					}
					setCommand();
				}
				else if(cursolIndex == 9)
				{
					playsound_cancel();
					closeLayer = true;
				}
				else
				{
					padsetmode_ = 1;
					padsetmodewait_ = 1;
				}
			}

			if(g_pPlayerInput()->cancel().justOn)
			{

				playsound_cancel();
		
				closeLayer = true;
				
			}
		}
	}
	return true;
}


//--------------------------------------------------------------------------------------



cParamaterConfigWindow::cParamaterConfigWindow(void)
{
	max_ = 100;
	min_ = 0;
	DidDismissWindow = NULL;
	DidChangeValue = NULL;
}

cParamaterConfigWindow::~cParamaterConfigWindow(void)
{

}
void cParamaterConfigWindow::Init(cRenderDevice *pDev, int *valuepointer, int min, int max)
{
	int letterXnum = 20;
	int letterYnum = 3;

	first_ = *valuepointer;
	output_ = valuepointer;

	max_ = max;
	min_ = min;

	cGameWindow::Init(pDev,letterXnum,letterYnum);
}

void cParamaterConfigWindow::Init(cRenderDevice *pDev, int letterXnum, int letterYnum)
{
	//Init(pDev);
}

int cParamaterConfigWindow::Draw(cRenderDevice *pDev)
{
	cGameWindow::Draw(pDev);

	return true;
}
int cParamaterConfigWindow::TextDraw(cRenderDevice *pDev)
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

		m_SpriteText.SetCursolPos(0, linetop(2));
		m_SpriteText.DrawText(setStyle(min_,_T("%3d")));
		
		m_SpriteText.SetCursolPos((letterXlength-2)*GWFONTSIZE, linetop(2));
		m_SpriteText.DrawText(setStyle(max_,_T("%3d")));

		m_SpriteText.SetCursolPos((letterXlength-2)*GWFONTSIZE/2, linetop(2));
		m_SpriteText.DrawText(setStyle(*output_,_T("%3d"),0xFF00FFFF));

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

int cParamaterConfigWindow::pageDraw(cRenderDevice *pDev)
{

	cDrawingObject DO;
	DO.setTexture(m_pTexture_Page,PAGETEXSIZE,PAGETEXSIZE);
	DO.m_color = color;

	DO.m_TexRange.setLTRB(1/2.0,0/2.0
							,2/2.0,1/2.0);

	int dif = m_pageAnimeCount*6/PAGECOUNTNUM;
	int drawX = Left() + EDGEWIDTH;
	int drawY = CenterY - PAGETEXSIZE/4;


	//右矢印描画
	DO.setLTRB(drawX-dif,drawY,drawX+PAGETEXSIZE/2-dif,drawY+PAGETEXSIZE/2);

	DO.Draw(pDev);
	

	DO.m_TexRange.setLTRB(0/2.0,0/2.0
							,1/2.0,1/2.0);

	dif = -m_pageAnimeCount*6/PAGECOUNTNUM;
	drawX = Right() - EDGEWIDTH - PAGETEXSIZE/2;
	drawY = CenterY - PAGETEXSIZE/4;


	//左矢印描画
	DO.setLTRB(drawX-dif,drawY,drawX+PAGETEXSIZE/2-dif,drawY+PAGETEXSIZE/2);

	DO.Draw(pDev);


	
	DO.setTexture(m_pTexture_Cursor);
	DO.m_TexRange.setLTRB(0,0
							,1,1);

	drawX = (*output_ - min_)*(letterXlength*GWFONTSIZE)/(max_ - min_) + Left() + CURSORSIZE/2;
	drawY = CenterY;
	DO.Height = CURSORSIZE;
	DO.Width = CURSORSIZE;
	DO.CenterX = drawX;
	DO.CenterY = drawY;

	DO.Draw(pDev);

	return true;
}
int cParamaterConfigWindow::process(cRenderDevice *pDev)
{
	int i;

	if((g_pPlayerInput()->migi().justOn) || (g_pPlayerInput()->migi().on && g_pPlayerInput()->migi().onCount > 15 && !(g_pPlayerInput()->migi().onCount%5)))
	{
		*output_ += g_pPlayerInput()->migi().onCount/5 +1;
		preRendar = false;
	}
	if((g_pPlayerInput()->hidari().justOn) || (g_pPlayerInput()->hidari().on && g_pPlayerInput()->hidari().onCount > 15 && !(g_pPlayerInput()->hidari().onCount%5)))
	{
		*output_ -= g_pPlayerInput()->hidari().onCount/5 +1;
		preRendar = false;
	}

	if(*output_ < min_) *output_ = min_;//注意
	if(*output_ > max_) *output_ = max_;

	if( g_pPlayerInput()->migi().justOn || (g_pPlayerInput()->migi().on && g_pPlayerInput()->migi().onCount > 15 && !(g_pPlayerInput()->migi().onCount%5))
		|| g_pPlayerInput()->hidari().justOn || (g_pPlayerInput()->hidari().on && g_pPlayerInput()->hidari().onCount > 15 && !(g_pPlayerInput()->hidari().onCount%5)))
	{
		if(DidChangeValue)
		{
			DidChangeValue();
		}
		playsound_move();
	}

	if(g_pPlayerInput()->cancel().justOn)
	{
		playsound_cancel();
		if(DidDismissWindow)
		{
			DidDismissWindow();
		}
		closeLayer = true;	
	}


	cGameWindow::process(pDev);
	return true;
}
