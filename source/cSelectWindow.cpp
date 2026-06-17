#include "stdafx.h"

#include "cSelectWindow.h"
#include "cDungeonSystem.h"
#include "cDropingFunc.h"

#include "cSaveStore.h"
#include "cShortCuts.h"

#define PAGETEXPATH _T("skin\\page.png")
//#define PAGETEXSIZE 32

#define PAGECOUNTNUM 40

cSelectWindow::cSelectWindow(void)
{

	preRendar = false;
	Text() = _T("");
	cursolIndex = 0;
	m_pageAnimeCount = 0;
	pageIndexMax = 1;
	shortCutState = 0;
	shortCutSelect_X = 0;
	shortCutSelect_Y = 0;
	cursolEnable = true;
	decisionEnable = true;
	cursolOpacity = 1.0;
	selectMemoryKey_ = _T("");
}

cSelectWindow::~cSelectWindow(void)
{
	commandList.clear();
}

void cSelectWindow::Init(cRenderDevice *pDev, int letterXnum, int letterYnum)
{

	
	InitBasic(pDev,letterXnum,letterYnum);

	Width = letterXnum * GWFONTSIZE + sideEdgeWith()*2 + cursoletcMarginWidth();


	m_pTexture_Page = g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev, PAGETEXPATH);



	cursolIndexMax = min(commandList.size(),letterYnum);
	cursolIndex = 0;

	pageIndexMax = (commandList.size()-1)/cursolIndexMax + 1;
	pageIndex = 0;

/*
	int iws = letterXnum * GWFONTSIZE;
	int ihs = letterYnum * GWFONTSIZE;
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
*/
	m_pageText.Init(pDev, 1, EDGEWIDTH*8, EDGEWIDTH);

	loadMemorizedSelect();
}

void cSelectWindow::SetCursolTolast()
{
	pageIndex = pageIndexMax - 1;
	cursolIndex = commandList.size() - pageIndex*cursolIndexMax - 1;
}

StyleString cSelectWindow::shortExplanationString(cRenderDevice *pDev)
{
	return commandList[pageIndex*cursolIndexMax + cursolIndex]->shortExplanationText();
}


int cSelectWindow::Draw(cRenderDevice *pDev)
{
	
	WindowDraw(pDev);
	TextDraw(pDev);
	CursorDraw(pDev);
	pageDraw(pDev);

	ShortCutDraw(pDev);
	return true;
}

int cSelectWindow::process(cRenderDevice *pDev)
{

	if(shortCut_process(pDev)) return true;
	if(shortCutState) return true;


	//上下
	cursolIndex += (g_pPlayerInput()->shita().justOn) || (g_pPlayerInput()->shita().on && g_pPlayerInput()->shita().onCount > 15 && !(g_pPlayerInput()->shita().onCount%5));
	cursolIndex -= (g_pPlayerInput()->ue().justOn) || (g_pPlayerInput()->ue().on && g_pPlayerInput()->ue().onCount > 15 && !(g_pPlayerInput()->ue().onCount%5));

	if(cursolIndex < 0)
	{
		cursolIndex = cursolIndexMax - 1;
		if(commandList.size() <= pageIndex*cursolIndexMax + cursolIndex)
		{
			cursolIndex = commandList.size() - pageIndex*cursolIndexMax - 1;
		}
	}
	if(cursolIndex >= cursolIndexMax || commandList.size() <= pageIndex*cursolIndexMax + cursolIndex) cursolIndex = 0;


	int tempP = pageIndex;
	pageIndex += (g_pPlayerInput()->migi().justOn);
	pageIndex -= (g_pPlayerInput()->hidari().justOn);


	if(tempP != pageIndex) preRendar = false;

	if(pageIndex < 0) pageIndex = pageIndexMax - 1;
	if(pageIndex >= pageIndexMax) pageIndex = 0;

	//安全性確保
	if(commandList.size() <= pageIndex*cursolIndexMax + cursolIndex)
	{
		pageIndex = pageIndexMax - 1;
		cursolIndex = commandList.size() - pageIndex*cursolIndexMax - 1;
	}

	if(g_pPlayerInput()->shita().justOn || (g_pPlayerInput()->shita().on && g_pPlayerInput()->shita().onCount > 15 && !(g_pPlayerInput()->shita().onCount%5))
			|| g_pPlayerInput()->ue().justOn || (g_pPlayerInput()->ue().on && g_pPlayerInput()->ue().onCount > 15 && !(g_pPlayerInput()->ue().onCount%5))
			)
	{
		playsound_move();
		memorizeSelect();
	}

	if(decisionEnable && g_pPlayerInput()->decision().justOn)
	{
		commandList[pageIndex*cursolIndexMax + cursolIndex]->Action(pDev);
		playsound_decide();
	}

	m_pageAnimeCount++;
	if(m_pageAnimeCount > PAGECOUNTNUM) m_pageAnimeCount = 0;

	return true;
}


//trueを返すとprocessをパスする
int cSelectWindow::shortCut_process(cRenderDevice *pDev)
{
	if(shortCutState == 0)
	{
		//通常
		if(g_pPlayerInput()->shot().justOn)
		{
			if(pforcusedcommand()->canIntoShortCut() 
				&& sg_pDungeonSystem->pSaveData->ShortCutsManager->canAddShortcut(pforcusedcommand()))
			{
				shortCutState = 1;
				shortCutSelect_X = 0;
				shortCutSelect_Y = 0;
				pcDropingFunc p_droping_func = boost::dynamic_pointer_cast<cDropingFunc>(pforcusedcommand());
				
				if(p_droping_func != NULL)
				{
					//アイテムコマンド
					sg_pDungeonSystem->pSaveData->ShortCutsManager->setShortcutTempText(p_droping_func->Subject,pforcusedcommand()->caption);
				}
				else
				{
					//一般コマンド
					sg_pDungeonSystem->pSaveData->ShortCutsManager->setShortcutTempText(NULLDROP,pforcusedcommand()->caption);
				}
				
			}
		}
	}
	else if(shortCutState == 1)
	{

		if(g_pPlayerInput()->migi().justOn)
		{
			shortCutSelect_X++;
			if(shortCutSelect_X > 1)
			{
				shortCutSelect_X = 1;
			}
		}
		if(g_pPlayerInput()->hidari().justOn)
		{
			shortCutSelect_X--;
			if(shortCutSelect_X < -1)
			{
				shortCutSelect_X = -1;
			}
		}
		if(g_pPlayerInput()->ue().justOn)
		{
			shortCutSelect_Y--;
			if(shortCutSelect_Y < -1)
			{
				shortCutSelect_Y = -1;
			}
		}
		if(g_pPlayerInput()->shita().justOn)
		{
			shortCutSelect_Y++;
			if(shortCutSelect_Y > 1)
			{
				shortCutSelect_Y = 1;
			}
		}

		//ショートカット
		//通常
		if(g_pPlayerInput()->cancel().on
			|| g_pPlayerInput()->shot().justOn)
		{
			shortCutState = 0;
			playsound_cancel();
		}
		else if(g_pPlayerInput()->decision().justOn)
		{
			if(pforcusedcommand()->canIntoShortCut() 
				&& sg_pDungeonSystem->pSaveData->ShortCutsManager->canAddShortcut(pforcusedcommand()))
			{
				cCoordinate coo;
				coo.Set(shortCutSelect_X,shortCutSelect_Y);
				if(coo.dif())
				{
					sg_pDungeonSystem->pSaveData->ShortCutsManager->addShortcut((eShortCuts_index)coo.GetAspect(),pforcusedcommand());
					shortCutState = 0;
					playsound_decide();
					return true;
				}
			}

		}
	}

	return false;
}

void cSelectWindow::memorizeSelect()
{
	if(selectMemoryKey_ == _T(""))
	{
		return;
	}

	if(sg_pDungeonSystem->拠点フラグ())
	{
		mapUtility::setMapValue(sg_pDungeonSystem->pSaveData->globalFlags,selectMemoryKey_,cursolIndex);
	}
	else
	{
		mapUtility::setMapValue(sg_pDungeonSystem->pSaveQuest->localFlags,selectMemoryKey_,cursolIndex);
	}
}

void cSelectWindow::loadMemorizedSelect()
{
	if(selectMemoryKey_ == _T(""))
	{
		return;
	}

	int index = 0;
	if(sg_pDungeonSystem->拠点フラグ())
	{
		index = mapUtility::getMapValue(sg_pDungeonSystem->pSaveData->globalFlags,selectMemoryKey_,cursolIndex);
	}
	else
	{
		index = mapUtility::getMapValue(sg_pDungeonSystem->pSaveQuest->localFlags,selectMemoryKey_,cursolIndex);
	}
	if(index >= 0 && index < cursolIndexMax)
	{
		cursolIndex = index;
	}
}

pcCommand cSelectWindow::pforcusedcommand()
{
	if(pageIndex*cursolIndexMax + cursolIndex < commandList.size())
	{
		return commandList[pageIndex*cursolIndexMax + cursolIndex];
	}
	else
	{
		return pcCommand((cCommand*)NULL);
	}
}

void cSelectWindow::setSelectMemoryKey(const tstring& tstr)
{
	if(tstr == _T(""))
	{
		selectMemoryKey_ =  _T("");
	}
	else
	{
		selectMemoryKey_ = _T("SelectWindowMemoryKey:") + tstr;
		loadMemorizedSelect();
	}
}

int cSelectWindow::CursorDraw(cRenderDevice *pDev)
{
	if(enable && cursolEnable)
	{
		m_DO.setTexture(m_pTexture_Cursor, CURSORSIZE, CURSORSIZE);
		int l = Left();
		int t = Top();
		double ew = EDGEWIDTH;

		//m_DO.m_color.ARGB(255,255,255,255);
		m_DO.Width = m_DO.Height = CURSORSIZE;
		m_DO.setLeft(l+ew);
		m_DO.CenterY = t + ew + cursolIndex*lineheight() + lineheight()/2;

		m_DO.m_TexRange.setLTRB(0,		0,		1,		1);
		
		m_DO.m_color = color;
		m_DO.m_color.alpha *= cursolOpacity;
		DrawAddShadowDO_mini(m_DO,pDev);
	}

	return true;
}
/*
int cSelectWindow::pageDraw(cRenderDevice *pDev)
{
	if(pageIndexMaxF() != 0)
	{


		cDrawingObject DO;
		DO.setTexture(m_pTexture_Page,PAGETEXSIZE,PAGETEXSIZE);

		DO.m_TexRange.setLTRB(1/2.0,0/2.0
								,2/2.0,1/2.0);

		int dif = m_pageAnimeCount*6/PAGECOUNTNUM;
		int drawX = Left() + EDGEWIDTH;
		int drawY = Top();

		if(pageIndex != 0)
		{
			//右矢印描画
			DO.setLTRB(drawX-dif,drawY,drawX+PAGETEXSIZE/2-dif,drawY+PAGETEXSIZE/2);

			DO.Draw(pDev);
		}

		DO.m_TexRange.setLTRB(0/2.0,0/2.0
								,1/2.0,1/2.0);

		dif = -m_pageAnimeCount*6/PAGECOUNTNUM;
		drawX = Left() + EDGEWIDTH + PAGETEXSIZE/2 + EDGEWIDTH;
		drawY = Top();

		if(pageIndex != pageIndexMaxF() -1)
		{
			//左矢印描画
			DO.setLTRB(drawX-dif,drawY,drawX+PAGETEXSIZE/2-dif,drawY+PAGETEXSIZE/2);
			DO.m_color = color;
			DO.Draw(pDev);
		}

		if(pageIndexMaxF() >= 2)
		{
			//ページ描画
			DO.setTexture(m_pageText.GetTex(),EDGEWIDTH*8,EDGEWIDTH);
			DO.m_TexRange.setLTRB(0,0,1,1);
			drawX = Left() + EDGEWIDTH + EDGEWIDTH*4;
			drawY = Top();

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
			DO.m_color.ARGB(255,0,160,140);
			DO.Draw(pDev);

		}

	}

	return true;
}*/

int cSelectWindow::RerenderDraw(cRenderDevice *pDev)
{
	int i;
	TCHAR t[MAXBUFFSIZE];
	cRenderVector2 vec;
		

	m_SpriteText.CleatText();

	m_SpriteText.SetAntiAlias(1);
	m_SpriteText.SetColor( D3DCOLOR_ARGB(255,255,255,255) );
	m_SpriteText.SetReturnWidth(1024);//自動returnしない
	m_SpriteText.SetWeight(GWFONTWEIGHT);
	m_SpriteText.BeginText( g_fontstyle().c_str(), GWFONTSIZE );
	for(i=0;i<cursolIndexMax;i++)
	{
		int k = pageIndex*cursolIndexMax + i;
		m_SpriteText.SetCursolPos(0,linetop(i));
		
		if(k >= commandList.size()) break;

		commandList[k]->resetCaption();
		StyleString str = commandList[k]->caption;
		//文字が長いと縮小
		m_SpriteText.FitTextWidth(str,letterXlength*GWFONTSIZE);


		/*
		if(commandList[k]->caption.length() > letterXlength*2)
		{	
			vec.x = (double)letterXlength*2*0.9/commandList[k]->caption.length(); vec.y = 1.0f;
			str.setSameSize(vec);
		}
		*/
		m_SpriteText.DrawText(str);
		m_SpriteText.DrawText(_T("\n"));

	}

	m_SpriteText.EndText();


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

	return true;
}

int cSelectWindow::TextDraw(cRenderDevice *pDev)
{
	int i;

	if(preRendar == false)
	{

		RerenderDraw(pDev);

		preRendar = true;
	}


	TextDrawSub(pDev);

	return true;
}

int cSelectWindow::ShortCutDraw(cRenderDevice *pDev)
{
	if(shortCutState)
	{
		cCoordinate coo;
		coo.Set(shortCutSelect_X,shortCutSelect_Y);

		eShortCuts_index index;
		if(coo.dif() == 0)
		{
			index = eShortCuts_index_NoIndex;
		}
		else
		{
			index = (eShortCuts_index)coo.GetAspect();
		}
		g_pSaveData()->ShortCutsManager->drawWindow(index);
		g_pSaveData()->ShortCutsManager->drawTempWindow(index);
	}
	
	return true;
}

int cSelectWindow::cursoletcMarginWidth()
{
	return CURSORSIZE * enable;
}

int cSelectWindow::TextDrawSub(cRenderDevice *pDev)
{
	int l = Left()+cursoletcMarginWidth();
	int t = Top();
	int r = Right()+cursoletcMarginWidth();
	int b = Bottom();

	double ew = EDGEWIDTH;
	double ew_s = sideEdgeWith();


	int iws = Width - ew_s*2 - CURSORSIZE;
	int ihs = Height - ew*2;
	int tw = letterXlength * GWFONTSIZE;
	int th = letterYlength * lineheight();

	tw = largerMinPow2(tw);//tw以上の最小の2^n
	th = largerMinPow2(th);//th以上の最小の2^n


	m_DO.setTexture(m_SpriteText.GetTex(), tw, th);
	

	//m_DO.m_color.ARGB(255,255,255,255);
	m_DO.setLTRB(			l + ew_s,		t + ew,		r - ew_s - CURSORSIZE,		b - ew);
	m_DO.m_TexRange.setLTRB(0,		0,		iws / (double)tw ,	ihs / (double)th);

	
	m_DO.m_color = color;
	DrawAddShadowDO(m_DO,pDev);

	return true;

}