#include "stdafx.h"

#include "cStringWindow.h"
#include "cSelectWindow.h"

#include "scriptprocesser/DropsScript.h"
#include "csTitle.h"

#include "Language.h"
#include "Language.h"
#include "utility/debug.h"
#include "gameMainSystem/filemanage/cScriptRLayer.h"

#define CURSORSIZE 32

#define PAGETEXPATH _T("skin\\page.png")
//#define PAGETEXSIZE 32

#define MULTISELECTTEXPATH _T("interface\\multiselect.png")

#define PAGECOUNTNUM 70

#define FORM_STRWINDOWTOP (136)


//コマンド
class cCoverCommand :
	public cCommand
{
public:
	cCoverCommand(void){};
	cCoverCommand(pcMemoString pmemo, cMemoString * target = NULL)
	{
		caption = pmemo->str();
		pmemo_ = pmemo;
		targetStr = target;
	};
public:
	virtual ~cCoverCommand(void){};

	//StyleString caption;

	cMemoString* targetStr;

protected:
	pcMemoString pmemo_;

public:
	virtual int Action(cRenderDevice *pDev)
	{
		*targetStr = *pmemo_;
	return true;
	};

	virtual StyleString shortExplanationText(){return _T("");};
};

class cCoverCommand_NULL :
	public cCommand
{
public:
	cCoverCommand_NULL(void)
	{
	};
	cCoverCommand_NULL(cMemoString * target)
	{
		caption = target->str();
	};
public:
	virtual ~cCoverCommand_NULL(void){};

	//StyleString caption;

public:
	virtual int Action(cRenderDevice *pDev)
	{
		return true;
	};

	virtual StyleString shortExplanationText(){return _T("");};
};




cStringWindow::cStringWindow(void)
{

	cursolIndexXMax = 10;
	cursolIndexX = 0;
	cursolIndexYMax = 9;
	cursolIndexY = 0;

	cursolforcus = 0;
	StrSet = 0;

	modeCover_ = FALSE;

	targetStr = NULLOFpcMemoString;
	targetStr2 = NULL;
}

cStringWindow::~cStringWindow(void)
{
	commandList.clear();
}

void cStringWindow::Init(cRenderDevice *pDev, int letterXnum, int letterYnum)
{
	InitBasic(pDev,letterXnum,letterYnum);

}
void cStringWindow::Init(cRenderDevice *pDev)
{


	int letterXnum = 10;
	int letterYnum = 9;

	InitBasic(pDev,((GWFONTSIZE + CURSORSIZE)*(letterXnum+1)+GWFONTSIZE*3)/GWFONTSIZE + 1,letterYnum);

	


	Width = (GWFONTSIZE + CURSORSIZE)*(letterXnum+1)+GWFONTSIZE*3 + sideEdgeWith()*2;


	m_pTexture_Page = g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev, PAGETEXPATH);

	m_pTexture_Multiselect =  g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev, MULTISELECTTEXPATH);


	//cursolIndexMax = min(commandList.size(),letterYnum);
	//cursolIndex = 0;
	cursolIndexXMax = letterXnum;
	cursolIndexX = 0;
	cursolIndexYMax = letterYnum;
	cursolIndexY = 0;

	pageIndexMax = 1;
	pageIndex = 0;


	//サンプルテキスト用
	int iws = STRINGWINDOW_STRNUM * GWFONTSIZE;
	int ihs = 1 * lineheight();
	int tw = iws;
	int th = ihs;
	tw = largerMinPow2(tw);//tw以上の最小の2^n
	th = largerMinPow2(th);//th以上の最小の2^n
	m_SpriteText_SumpleText.Init(pDev, 1, tw, th);


	m_pageText.Init(pDev, 1, EDGEWIDTH*8, EDGEWIDTH);


	if(targetStr != NULL)
	{
		str_ = *targetStr;
		cursolforcus = min(str_.getDataSize(),STRINGWINDOW_STRNUM-1);
	}
	else
	{
		int i;
		for(i = 0;i<STRINGWINDOW_STRNUM;i++)
		{
			str_.setdata(sg_pDungeonSystem->DataBase.NullstrSampleIndex(), i);
		}
		str_.setDataSize(0);
	}
/*
	//項目ウィンドウ
	pSetselectW_ = (pcSelectWindow)(new cSelectWindow);
	//pSetselectW_->Init(pDev,4,6);
	pSetselectW_->InitBasic(pDev,4,6);
	pSetselectW_->Width = 4 * GWFONTSIZE + EDGEWIDTH*2 + CURSORSIZE;
	pSetselectW_->pageIndex=0;

	pSetselectW_->setRight(SCREEN_X);
	pSetselectW_->setTop(FORM_STRWINDOWTOP);
	
*/
	
	//選択文字ウィンドウ
	pStrW_ = (pcGameWindow)(new cGameWindow);
	pStrW_->Init(pDev,STRINGWINDOW_STRNUM,1);
	pStrW_->CenterX = (SCREEN_X/2);
	pStrW_->setBottom(FORM_STRWINDOWTOP);


	//メインウィンドウ
	//setRight(pSetselectW_->Left());
	CenterX = (SCREEN_X/2);
	setTop(FORM_STRWINDOWTOP);


	//strSample_.assign(STRINGWINDOW_CHARKIND,_T("____"));
	strSetList_.assign(STRINGWINDOW_SETNUM,_T("____"));

	//設定ロード
	StringWindowMemoData_ = g_systemSR.pdata()->pmember((tstring)_T("StringWindowMemoData"));

	if(StringWindowMemoData_ != NULL)
	{
		pcScriptRLayer pcsrletter;

		pcsrletter = StringWindowMemoData_->pmember((tstring)_T("LetterListName"));
		if(pcsrletter != NULL)
		{
			strSetList_ = pcsrletter->script();
			strSetList_.resize(STRINGWINDOW_SETNUM,_T("____"));
		
		}
		else OnAssert(_T(__FILE__),__LINE__,false,_T("StringWindowの初期化データ「LetterListName」が見つかりません。設定を確認してください。<error> [LetterListName] Not Found"));

		/*
		pcsrletter= pcsr->pmember((tstring)_T("MemoLetter"));
		if(pcsrletter != NULL)
		{
			strSample_ = pcsrletter->script();
			strSample_.resize(STRINGWINDOW_CHARKIND,_T("____"));
		
		}
		else OnAssert(_T(__FILE__),__LINE__,false,_T("StringWindowの初期化データ「MemoLetter」が見つかりません。設定を確認してください。"));
		

		pcsrletter = pcsr->pmember((tstring)_T("NullLetter"));
		if(pcsrletter != NULL)
		{

			int res = 0;
			TCHAR* e;
			res = _tcstol(pcsrletter->topscript().c_str(),&e,10);
			OnAssert(_T(__FILE__),__LINE__, (pcsrletter->topscript().length() != 0) ,_T("StringWindowの初期化データ「NullLetter」に 空欄が設定されています。"));
			OnAssert(_T(__FILE__),__LINE__, (_tcslen(e) == 0) && ( !(res == 0 && _tcslen(e) == 0) || (pcsrletter->topscript() == _T("0")))  ,_T("StringWindowの初期化データ「NullLetter」に 解釈不能な数値 が設定されています。"));
	
			NullSampleIndex_ = res;
		}
		else OnAssert(_T(__FILE__),__LINE__,false,_T("StringWindowの初期化データ「NullLetter」が見つかりません。設定を確認してください。"));
		*/

		pcsrletter = StringWindowMemoData_->pmember((tstring)_T("ShortCutPair"));
		if(pcsrletter != NULL)
		{
			vector<int> vint;
			int i;
			for(i=0;i<pcsrletter->script().size();i++)
			{
				split(pcsrletter->script()[i],_T("|"),vint, tstring(_T("StringWindowの初期化データ「ShortCutPair」")).c_str());
				if(vint.size() != 2)
				{
					OnAssert(_T(__FILE__),__LINE__,false,_T("StringWindowの初期化データ「ShortCutPair」の書式にエラーが発生しました。<error> Exist errors in [ShortCutPair]"));
					continue;
				}
				ShortCutPair_.insert(pair<int,int>(vint[0],vint[1]));

				/*
				split(pcsrletter->script()[i],_T("|"),vstr);
				if(vstr.size() != 2)
				{
					OnAssert(_T(__FILE__),__LINE__,false,_T("StringWindowの初期化データ「ShortCutPair」の書式にエラーが発生しました。"));
					continue;
				}
				int setBefore, setAfter;
				int res = 0;
				TCHAR* e;
				res = _tcstol(vstr[0].c_str(),&e,10);
				OnAssert(_T(__FILE__),__LINE__, (vstr[0].length() != 0) ,_T("StringWindowの初期化データ「ShortCutPair,0」に 空欄が設定されています。"));
				OnAssert(_T(__FILE__),__LINE__, (_tcslen(e) == 0) && ( !(res == 0 && _tcslen(e) == 0) || (vstr[0] == _T("0")))  ,_T("StringWindowの初期化データ「ShortCutPair,0」に 解釈不能な数値 が設定されています。: "),pcsrletter->script()[i].c_str() );
		
				setBefore = res;
				
				res = 0;
				//TCHAR* e;
				res = _tcstol(vstr[1].c_str(),&e,10);
				OnAssert(_T(__FILE__),__LINE__, (vstr[1].length() != 0) ,_T("StringWindowの初期化データ「ShortCutPair,1」に 空欄が設定されています。"));
				OnAssert(_T(__FILE__),__LINE__, (_tcslen(e) == 0) && ( !(res == 0 && _tcslen(e) == 0) || (vstr[1] == _T("0")))  ,_T("StringWindowの初期化データ「ShortCutPair,1」に 解釈不能な数値 が設定されています。: "),pcsrletter->script()[i].c_str());
				setAfter = res;

				ShortCutPair_.insert(pair<int,int>(setBefore,setAfter));
				*/
			}
			
			strSetList_.resize(STRINGWINDOW_SETNUM,_T("____"));
		
		}
		else OnAssert(_T(__FILE__),__LINE__,false,_T("StringWindowの初期化データ「ShortCutPair」が見つかりません。設定を確認してください。<error> [ShortCutPair] Not Found"));

	}
	else
	{
		OnAssert(_T(__FILE__),__LINE__,false,_T("StringWindowの初期化データが見つかりません。設定を確認してください。<error> Data of [StringWindow] Not Found"));
	}





}


StyleString cStringWindow::shortExplanationString(cRenderDevice *pDev)
{
	if(modeCover_)
	{
		if(StringWindowMemoData_ == NULL) return _T("");
		pcScriptRLayer pcsr = StringWindowMemoData_->pmember((tstring)_T("shortExplanationString_Cover"));
		if(pcsr == NULL)
		{
			OnAssert(_T(__FILE__),__LINE__,false,_T("StringWindowの初期化データ「shortExplanationString_Cover」が見つかりません。設定を確認してください。<error> [shortExplanationString_Cover] Not Found"));
			return _T("");
		}
		StyleString output;
		std::map<tstring, StyleString > val;

		//val.insert( pair<tstring, StyleString >(_T("test"),setStyle(_T("test"),0xFFFF0000) ));
		
		Drop_StringAddStyle(pcsr, output, val);
		return output;//commandList[pageIndex*cursolIndexMax + cursolIndex]->shortExplanationText();

	}
	else
	{
		if(StringWindowMemoData_ == NULL) return _T("");
		pcScriptRLayer pcsr = StringWindowMemoData_->pmember((tstring)_T("shortExplanationString"));
		if(pcsr == NULL)
		{
			OnAssert(_T(__FILE__),__LINE__,false,_T("StringWindowの初期化データ「shortExplanationString」が見つかりません。設定を確認してください。<error> [shortExplanationString] Not Found"));
			return _T("");
		}
		StyleString output;
		std::map<tstring, StyleString > val;

		//val.insert( pair<tstring, StyleString >(_T("test"),setStyle(_T("test"),0xFFFF0000) ));
		
		Drop_StringAddStyle(pcsr, output, val);
		return output;//commandList[pageIndex*cursolIndexMax + cursolIndex]->shortExplanationText();
	}
}


int cStringWindow::Draw(cRenderDevice *pDev)
{
	//pSetselectW_->Draw(pDev);
	pStrW_->Draw(pDev);

	WindowDraw(pDev);
	TextDraw(pDev);
	CursorDraw(pDev);
	pageDraw(pDev);

	if(modeCover_) pcsCoverw_->Draw(pDev);

	return true;
}

int cStringWindow::process(cRenderDevice *pDev)
{

	if(modeCover_)
	{
		
		

		if( g_pPlayerInput()->diagon().on && g_pPlayerInput()->decision().justOn)
		{//候補消し
			cMemoString tempstr;
			tempstr = str_;	
		
			pcsCoverw_->process(pDev);

	
			CoverErase(str_);
			str_ = tempstr;
			commandCover();
		}
		else if(g_pPlayerInput()->cancel().justOn || g_pPlayerInput()->decision().justOn)
		{

			pcsCoverw_->process(pDev);


			modeCover_ = FALSE;
			preRendar = false;

			cursolforcus = min(str_.getDataSize(),STRINGWINDOW_STRNUM-1);
			if( g_pPlayerInput()->decision().justOn)
			{//終わるフォーカス
				cursolIndexX = cursolIndexXMax;
				cursolIndexY = cursolIndexYMax -1 ;
			}
		}
		else
		{
			pcsCoverw_->process(pDev);
		}


	}
	else
	{

		if(g_pPlayerInput()->turn().on)
		{
			if(g_pPlayerInput()->migi().justOn)
			{
				commandForward();
				playsound_move();
			}
			if(g_pPlayerInput()->hidari().justOn)
			{
				commandBack();
				playsound_move();
			}

		}
		else
		{
			if(g_pPlayerInput()->dash().justOn)
			{
				if(cursolforcus == 0)
				{

					if(cursolIndexX == cursolIndexXMax &&
						cursolIndexY == cursolIndexYMax -1)
					{
						commandCancel();
					}
					else
					{
						cursolIndexX = cursolIndexXMax;
						cursolIndexY = cursolIndexYMax -1 ;
					}

					playsound_cancel();
				}
				else
				{
					commandErase();
					playsound_cancel();
				}
			}
			else
			{
				cursolIndexX += (g_pPlayerInput()->migi().justOn) || (g_pPlayerInput()->migi().on && g_pPlayerInput()->migi().onCount > 15 && !(g_pPlayerInput()->migi().onCount%5));
				cursolIndexX -= (g_pPlayerInput()->hidari().justOn) || (g_pPlayerInput()->hidari().on && g_pPlayerInput()->hidari().onCount > 15 && !(g_pPlayerInput()->hidari().onCount%5));

				if(cursolIndexX < 0) cursolIndexX = cursolIndexXMax;//注意
				if(cursolIndexX > cursolIndexXMax) cursolIndexX = 0;

				//上下
				cursolIndexY += (g_pPlayerInput()->shita().justOn) || (g_pPlayerInput()->shita().on && g_pPlayerInput()->shita().onCount > 15 && !(g_pPlayerInput()->shita().onCount%5));
				cursolIndexY -= (g_pPlayerInput()->ue().justOn) || (g_pPlayerInput()->ue().on && g_pPlayerInput()->ue().onCount > 15 && !(g_pPlayerInput()->ue().onCount%5));

				if(cursolIndexY < 0) cursolIndexY = cursolIndexYMax - 1;
				if(cursolIndexY >= cursolIndexYMax) cursolIndexY = 0;

				if(g_pPlayerInput()->shita().justOn || (g_pPlayerInput()->shita().on && g_pPlayerInput()->shita().onCount > 15 && !(g_pPlayerInput()->shita().onCount%5))
					|| g_pPlayerInput()->ue().justOn || (g_pPlayerInput()->ue().on && g_pPlayerInput()->ue().onCount > 15 && !(g_pPlayerInput()->ue().onCount%5))
					|| g_pPlayerInput()->migi().justOn || (g_pPlayerInput()->migi().on && g_pPlayerInput()->migi().onCount > 15 && !(g_pPlayerInput()->migi().onCount%5))
					|| g_pPlayerInput()->hidari().justOn || (g_pPlayerInput()->hidari().on && g_pPlayerInput()->hidari().onCount > 15 && !(g_pPlayerInput()->hidari().onCount%5)))
				{
					playsound_move();
				}


				if(g_pPlayerInput()->decision().justOn && !g_pPlayerInput()->menu().justOn)
				{
					playsound_decide();
					if(cursolIndexX < cursolIndexXMax)
					{
						commandWrite(StrSet*cursolIndexXMax*cursolIndexYMax + cursolIndexXMax*cursolIndexY + cursolIndexX);
					}
					else
					{
						if(cursolIndexY < strSetList_.size()) commandStrset(cursolIndexY);
						else if(cursolIndexY == commandIndexErase()) commandErase();
						else if(cursolIndexY == commandIndexCover()) commandCover();
						else if(cursolIndexY == commandIndexChange()) commandChange();
						else if(cursolIndexY == commandIndexBack()) commandBack();
						else if(cursolIndexY == commandIndexForward()) commandForward();
						else if(cursolIndexY == commandIndexEnd()) commandEnd();
					}
				}
			}

		}


		if(g_pPlayerInput()->shot().justOn)
		{
			commandCover();
			playsound_decide();
		}


		if(g_pPlayerInput()->diagon().justOn)
		{
			commandChange();
			playsound_decide();
		}


		if(g_pPlayerInput()->miniMap().justOn)
		{
			cursolIndexX = cursolIndexXMax;
			cursolIndexY = cursolIndexYMax -1 ;
			playsound_decide();
		}

		if(g_pPlayerInput()->menu().justOn)
		{
			StrSet++;
			if(StrSet >= STRINGWINDOW_SETNUM) StrSet = 0;
			playsound_decide();
			preRendar = false;
		}
	}
	
/*

	//安全性確保
	if(commandList.size() <= pageIndex*cursolIndexMax + cursolIndex)
	{
		pageIndex = pageIndexMax - 1;
		cursolIndex = commandList.size() - pageIndex*cursolIndexMax - 1;
	}


	if(g_pPlayerInput()->decision().justOn)
	{
		commandList[pageIndex*cursolIndexMax + cursolIndex]->Action(pDev);
		playsound_decide();
	}

	m_pageAnimeCount++;
	if(m_pageAnimeCount > PAGECOUNTNUM) m_pageAnimeCount = 0;
*/
	return true;
}

int cStringWindow::CursorDraw(cRenderDevice *pDev)
{
	m_DO.setTexture(m_pTexture_Cursor, CURSORSIZE, CURSORSIZE);
	int l = Left();
	int t = Top();
	double ew = EDGEWIDTH;
	double ew_s = sideEdgeWith();

	//m_DO.m_color.ARGB(255,255,255,255);
	m_DO.Width = m_DO.Height = CURSORSIZE;

	if(g_pPlayerInput()->dash().on)
	{
		l = pStrW_->Left();
		t = pStrW_->Top();
		m_DO.setLeft(l+ew_s+(cursolforcus-1)*(GWFONTSIZE));
		m_DO.CenterY = t + ew + lineheight()/2;
	}
	else
	{
		m_DO.setLeft(l+ew_s+cursolIndexX*(GWFONTSIZE + CURSORSIZE));
		m_DO.CenterY = t + ew + cursolIndexY*lineheight() + lineheight()/2;
	}
	m_DO.m_TexRange.setLTRB(0,		0,		1,		1);
	
	m_DO.m_color = color;
	DrawAddShadowDO_mini(m_DO,pDev);


	return true;
}

/*
int cStringWindow::pageDraw(cRenderDevice *pDev)
{
	if(pageIndexMax != 0)
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

		if(pageIndex != pageIndexMax -1)
		{
			//左矢印描画
			DO.setLTRB(drawX-dif,drawY,drawX+PAGETEXSIZE/2-dif,drawY+PAGETEXSIZE/2);

			DO.Draw(pDev);
		}

		if(pageIndexMax >= 2)
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
				coo.SetAspect(i*2);
				DO.setLTRB(drawX+coo.x,drawY+coo.y,drawX+EDGEWIDTH*8+coo.x,drawY+EDGEWIDTH+coo.y);
				DO.m_color.ARGB(255,255,255,255);
				DO.Draw(pDev);
			}

			DO.setLTRB(drawX,drawY,drawX+EDGEWIDTH*8,drawY+EDGEWIDTH);
			DO.m_color.ARGB(255,0,160,140);
			DO.Draw(pDev);

		}

	}

	return true;
}
*/

int cStringWindow::RerenderDraw(cRenderDevice *pDev)
{
	int i,k;
	TCHAR t[MAXBUFFSIZE];
	cRenderVector2 vec;
		

	m_SpriteText.CleatText();
	m_SpriteText.SetWeight(GWFONTWEIGHT);
	m_SpriteText.BeginText( g_fontstyle().c_str(), GWFONTSIZE );
	m_SpriteText.SetAntiAlias(1);
	m_SpriteText.SetColor( D3DCOLOR_ARGB(255,255,255,255) );
	m_SpriteText.SetReturnWidth(1024);//自動returnしない
	for(i=0;i<cursolIndexXMax;i++)
	{
		for(k=0;k<cursolIndexYMax;k++)
		{
			int index = StrSet *cursolIndexXMax*cursolIndexYMax + k * cursolIndexXMax + i;
			m_SpriteText.SetCursolPos( i*(GWFONTSIZE+CURSORSIZE) ,linetop(k));


			if(index >= MEMOSTRING_CHARKIND) break;

			m_SpriteText.DrawText(sg_pDungeonSystem->DataBase.GenMemoLetter(index));
			
		}

	}

	
	//メニュー

	for(k=0;k<cursolIndexYMax;k++)
	{
		
		m_SpriteText.SetCursolPos( cursolIndexXMax*(GWFONTSIZE+CURSORSIZE) ,linetop(k));

		StyleString str;

		if(k < strSetList_.size()) str = strSetList_[k];
		else if(k == commandIndexErase()) str = StringWindowMemoData_->member((tstring)_T("消す")).topscript();
		else if(k == commandIndexCover()) str = StringWindowMemoData_->member((tstring)_T("補う")).topscript();
		else if(k == commandIndexChange()) str = StringWindowMemoData_->member((tstring)_T("変換")).topscript();
		else if(k == commandIndexBack()) str = StringWindowMemoData_->member((tstring)_T("戻る")).topscript();
		else if(k == commandIndexForward()) str = StringWindowMemoData_->member((tstring)_T("進む")).topscript();
		else if(k == commandIndexEnd()) str = StringWindowMemoData_->member((tstring)_T("終わる")).topscript();


		unsigned int colorstrSet = (0xFFFFEF60);
		unsigned int colorErase = (0xFFFF6F5F);
		unsigned int colorCover = (0xFF6FEF8F);
		unsigned int colorChange = (0xFFDF6FEF);
		unsigned int colorMove = (0xFF6FAFFF);
		unsigned int colorEnd = (0xFFFF9F60);
		if(k < strSetList_.size()) str.colors.back() = colorstrSet;
		else if(k == commandIndexErase()) str.colors.back() = colorErase;
		else if(k == commandIndexCover()) str.colors.back() = colorCover;
		else if(k == commandIndexChange()) str.colors.back() = colorChange;
		else if(k == commandIndexBack()) str.colors.back() = colorMove;
		else if(k == commandIndexForward()) str.colors.back() = colorMove;
		else if(k == commandIndexEnd()) str.colors.back() = colorEnd;

		

		
		int maxlen = 4;
		//文字が長いと縮小
		m_SpriteText.FitTextWidth(str,maxlen*GWFONTSIZE);

		m_SpriteText.DrawText(str);
	}

	

	m_SpriteText.EndText();


	m_pageText.CleatText();
	m_pageText.SetWeight(GWFONTWEIGHT);
	m_pageText.BeginText( g_fontstyle().c_str(), EDGEWIDTH);
	m_pageText.SetAntiAlias(1);
	m_pageText.SetCursolPos(0,0);
	m_pageText.SetColor( D3DCOLOR_ARGB(255,255,255,255) );
	m_pageText.SetReturnWidth(1024);//自動returnしない
	_stprintf(t ,_T("%d_/_%d"),pageIndex+1,pageIndexMax);
	m_pageText.DrawText(t);

	m_pageText.EndText();


	//サンプルテキスト
	m_SpriteText_SumpleText.CleatText();
	m_SpriteText_SumpleText.SetWeight(GWFONTWEIGHT);
	m_SpriteText_SumpleText.BeginText( g_fontstyle().c_str(), GWFONTSIZE );
	m_SpriteText_SumpleText.SetAntiAlias(1);
	m_SpriteText_SumpleText.SetColor( D3DCOLOR_ARGB(255,255,255,255) );
	m_SpriteText_SumpleText.SetReturnWidth(1024);//自動returnしない
	for(i=0;i<STRINGWINDOW_STRNUM;i++)
	{
	
		m_SpriteText_SumpleText.SetCursolPos( i*(GWFONTSIZE) + GWFONTSIZE/10 ,linetop(0));


		m_SpriteText_SumpleText.DrawText(sg_pDungeonSystem->DataBase.GenMemoLetter(str_.getdata(i)));

	}
	m_SpriteText_SumpleText.EndText();

	return true;
}

void cStringWindow::commandWrite(int strindex)
{
	if(strindex < 0 || strindex >= MEMOSTRING_CHARKIND) strindex = sg_pDungeonSystem->DataBase.NullstrSampleIndex();
	str_.setdata(strindex,cursolforcus);
	commandForward();
	preRendar = false;
}
void cStringWindow::commandStrset(int index)
{
	StrSet = index;
	preRendar = false;

}
void cStringWindow::commandErase()//消す
{
	int didback = false;
	if(str_.getdata(cursolforcus) == sg_pDungeonSystem->DataBase.NullstrSampleIndex())
	{
		commandBack();
		didback = true;
	}

	str_.setdata(sg_pDungeonSystem->DataBase.NullstrSampleIndex(),cursolforcus);

	
	//if(!didback) commandBack();

	preRendar = false;
}
void cStringWindow::commandCover()//補う
{

	pcsCoverw_ = (pcSelectWindow)(new cSelectWindow);

	
	map<tstring,pcMemoString>::iterator itr = sg_pDungeonSystem->DataBase.memoDic_.upper_bound(str_.str());
	for(;itr !=  sg_pDungeonSystem->DataBase.memoDic_.end();itr++)
	{
		if((*itr).second->str().find(str_.str()) != 0) break;
		pcsCoverw_->commandList.push_back( pcCommand(new cCoverCommand((*itr).second, &str_) ) );
	
	}

	if(pcsCoverw_->commandList.empty())
	{
		pcsCoverw_->commandList.push_back( pcCommand(new cCoverCommand_NULL( &str_) ) );
	}
	
	pcsCoverw_->Init(sg_pDungeonSystem->pDevice_D3D, STRINGWINDOW_STRNUM+1, min(10,pcsCoverw_->commandList.size()));

	pcsCoverw_->setLeft(128);
	pcsCoverw_->setTop(68);
	

	modeCover_ = TRUE;
	preRendar = false;
}
void cStringWindow::commandChange()//変換
{
	int precursolforcus = cursolforcus;
	if(str_.getdata(cursolforcus) == sg_pDungeonSystem->DataBase.NullstrSampleIndex())
	{
		commandBack();
	}
	map<int,int>::iterator itr = ShortCutPair_.find(str_.getdata(cursolforcus));
	
	if(ShortCutPair_.end() != itr)
	{
		str_.setdata(itr->second,cursolforcus);
	}

	//if(didback) commandForward();
	cursolforcus = precursolforcus;

	preRendar = false;
}
void cStringWindow::commandBack()//戻る
{
	if(cursolforcus > 0)
	{
		cursolforcus--;
	}
}
void cStringWindow::commandForward()//進む
{
	if(cursolforcus < STRINGWINDOW_STRNUM-1)
	{
		cursolforcus++;
	}
}
void cStringWindow::commandEnd()//終わる
{
	//if(str_.getDataEnable())
	//{
		if(targetStr != NULL) *targetStr = str_;
		if(targetStr2 != NULL) *targetStr2 = str_.str();

		//sg_pDungeonSystem->DataBase.memoDic_.insert(pcMemoString(new cMemoString(str_)));
	
		if(str_.getDataEnable()) CoverAdd(str_);
	//}
	closeLayer = true;
}

void cStringWindow::commandCancel()//変えずに終わる
{
	closeLayer = true;
}

void cStringWindow::CoverAdd(cMemoString & memostr)//候補加える
{
	sg_pDungeonSystem->DataBase.memoDic_[memostr.str()] = pcMemoString(new cMemoString(memostr));
}

void cStringWindow::CoverErase(cMemoString & memostr)//候補消す
{
	map<tstring, pcMemoString>::iterator itr;
	itr = sg_pDungeonSystem->DataBase.memoDic_.find(memostr.str());
	if(itr == sg_pDungeonSystem->DataBase.memoDic_.end() ) return;
	sg_pDungeonSystem->DataBase.memoDic_.erase(itr);
}


int cStringWindow::TextDraw(cRenderDevice *pDev)
{
	int i;

	if(preRendar == false)
	{

		RerenderDraw(pDev);

		preRendar = true;
	}

	int l = Left()+CURSORSIZE;
	int t = Top();
	int r = Right();
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
	m_DO.setLTRB(			l + ew_s,		t + ew,		r - ew_s,		b - ew);
	m_DO.m_TexRange.setLTRB(0,		0,		iws / (double)tw ,	ihs / (double)th);

	
	m_DO.m_color = color;
	DrawAddShadowDO(m_DO,pDev);



	//上文字のカーソル
	m_DO.setTexture(m_pTexture_Multiselect);
	l = pStrW_->Left();
	t = pStrW_->Top();


	//m_DO.m_color.ARGB(255,255,255,255);
	m_DO.Width = GWFONTSIZE;
	m_DO.Height = lineheight();
	m_DO.setLeft(l+ew_s+cursolforcus*(GWFONTSIZE));
	m_DO.CenterY = t + ew + lineheight()/2;

	m_DO.m_TexRange.setLTRB(0,		0,		1,		1);
	m_DO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;
	m_DO.m_color.ARGB(64,255,255,255);
	m_DO.Draw(pDev);
	m_DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;
	m_DO.m_color.ARGB(196,255,255,255);
	m_DO.Draw(pDev);

	m_DO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;

	//-----------------------------------
	//サンプルテキスト
	//-----------------------------------
	l = pStrW_->Left();
	t = pStrW_->Top();
	r = l + pStrW_->Width;
	b = t + pStrW_->Height;

	iws = pStrW_->Width - ew_s*2;
	ihs = pStrW_->Height - ew*2;
	tw = iws;
	th = ihs;

	tw = largerMinPow2(tw);//tw以上の最小の2^n
	th = largerMinPow2(th);//th以上の最小の2^n
	m_DO.setTexture(m_SpriteText_SumpleText.GetTex(), tw, th);
	

	//m_DO.m_color.ARGB(255,255,255,255);
	m_DO.setLTRB(			l + ew_s,		t + ew,		r - ew_s,		b - ew);
	m_DO.m_TexRange.setLTRB(0,		0,		iws / (double)tw ,	ihs / (double)th);

	
	//影ドロー
	m_DO.m_color = color;
	DrawAddShadowDO(m_DO,pDev);
	return true;
}



//-----------------------------------------------------------
//cMemoString
//-----------------------------------------------------------
cMemoString::cMemoString(void)
{
	strMadefrag_ = FALSE;
	dataSizeCulfrag_ = FALSE;

	int i;
	for(i=0;i<STRINGWINDOW_STRNUM;i++)
	{
		pdata_[i] = &(escapedata_[i]);
		*pdata_[i] = sg_pDungeonSystem->DataBase.NullstrSampleIndex();
	}
	pDatasize_ = &escapeDatasize_;
	*pDatasize_ = 0;
}
cMemoString::cMemoString(cMemoString &rother)
{

	int i;
	for(i=0;i<STRINGWINDOW_STRNUM;i++)
	{
		pdata_[i] = &(escapedata_[i]);
		*pdata_[i] = rother.getdata(i);
	}
	pDatasize_ = &escapeDatasize_;
	*pDatasize_ = rother.getDataSize();
	
	*this = rother;
}
cMemoString::~cMemoString(void)
{
	
}


const tstring& cMemoString::str()
{
	if(strMadefrag_ == FALSE)
	{//生成しなおし
		if(getDataEnable() == FALSE)
		{//無効
			str_ = _T("");
		}
		else
		{//有効
			vector<int> vdata;
			int i;
			for(i=0;i<getDataSize();i++)
			{
				vdata.push_back(getdata(i));
			}
			sg_pDungeonSystem->DataBase.GenMemoLetter(vdata,str_);
		}
		
		strMadefrag_ = TRUE;
	}

	return str_;
}

void cMemoString::refresh()
{
	strMadefrag_ = FALSE;
	dataSizeCulfrag_ = FALSE;
}

bool cMemoString::getDataEnable()
{
	return (getDataSize());
}
UByte cMemoString::getDataSize()
{
	if(dataSizeCulfrag_ == FALSE)
	{//生成しなおし

		int i;
		for(i=STRINGWINDOW_STRNUM-1; i>=0 ;i--)
		{
			if(sg_pDungeonSystem->DataBase.NullstrSampleIndex() != getdata(i)) break;
		}
		i++;
		*pDatasize_ = i;
		dataSizeCulfrag_ = TRUE;
	}

	return *pDatasize_;
}
UByte cMemoString::getdata(int index)
{
	return *(pdata_[index]);
}
void cMemoString::setpdata(UByte *pointer,int index)
{
	pdata_[index] = pointer;
	strMadefrag_ = FALSE;//文字列生成しなおし
	dataSizeCulfrag_ = FALSE;//文字数計算しなおし
}
void cMemoString::setpDataSize(UByte *pointer)
{
	pDatasize_ = pointer;
	strMadefrag_ = FALSE;//文字列生成しなおし
	dataSizeCulfrag_ = FALSE;//文字数計算しなおし
}

void cMemoString::setdata(UByte data,int index)
{
	*(pdata_[index]) = data;
	strMadefrag_ = FALSE;//文字列生成しなおし
	dataSizeCulfrag_ = FALSE;//文字数計算しなおし
}
void cMemoString::setDataSize(UByte size)
{
	*pDatasize_ = size;
	strMadefrag_ = FALSE;//文字列生成しなおし
	dataSizeCulfrag_ = TRUE;//文字数計算しなくていい;
}

cMemoString& cMemoString::operator=(cMemoString& rother)
{
	int i;
	for(i=0;i<STRINGWINDOW_STRNUM;i++)
	{
		pdata_[i] = &(escapedata_[i]);
		*pdata_[i] = *(rother.pdata_[i]);
	}
	pDatasize_ = &escapeDatasize_;
	*pDatasize_ = rother.getDataSize();

	str_ = rother.str();
	strMadefrag_ = TRUE;
	dataSizeCulfrag_ = TRUE;

	return *this;
}

void cMemoString::save(std::vector<SByte>& data)
{
	data.clear();
	int i;
	for(i=0;i<STRINGWINDOW_STRNUM;i++)
	{
		data.push_back(static_cast<SByte>(escapedata_[i]));
	}
	data.push_back(static_cast<SByte>(escapeDatasize_));

}
void cMemoString::load(const std::vector<SByte>& data)
{
	int i;
	if(data.size() < STRINGWINDOW_STRNUM+1) return;
	for(i=0;i<STRINGWINDOW_STRNUM;i++)
	{
		(escapedata_[i]) = static_cast<UByte>(data[i]);
	}
	escapeDatasize_ = static_cast<UByte>(data[i]);

	//refresh();
}