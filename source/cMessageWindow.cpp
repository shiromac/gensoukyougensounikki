#include "stdafx.h"

#include "cMessageWindow.h"


#define Message_SPEED 4

#define Message_MEMORY_POWER 4

#define Message_VISIBLETIME 360

#define TEX_SIZE_CURSOL 32

cMessageWindow::cMessageWindow(void)
{
	preRendar = false;

	Waiting = false;

	rolldiff = -1;
	VisibleCount = 0;
}

cMessageWindow::~cMessageWindow(void)
{
	Messagelogs.clear();
	rollMessages.clear();
	reserveMessages.clear();
}

#define WAITTEXPATH _T("skin\\wait.png")
void cMessageWindow::Init(IDirect3DDevice9 *pDev, int letterXnum, int letterYnum)
{
	Width = letterXnum * GWFONTSIZE + sideEdgeWith()*2;
	Height = letterYnum * lineheight() + EDGEWIDTH*2;

	m_pTexture_Window = g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev, WINDOWTEXPATH);
	m_pTexture_Wait = g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev, WAITTEXPATH);
	


	int iws = letterXnum * GWFONTSIZE;
	int ihs = letterYnum * lineheight() *Message_MEMORY_POWER;//＊です
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
	m_SpriteText.SetCursolPos(0,linetop(0));
	m_SpriteText.SetReturnFontMargin(merginheight());

	forcusTop = m_SpriteText.GetY();

	textmaxWidth = iws;
	lineNum = letterYnum;

	pTex_cursol = g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev,_T("interface\\cursor.png"));

}

int cMessageWindow::addMessage(StyleString Message)
{
	if(Message.isFlagContent(STYLES_FLAG_MESSAGE_CLEAR))
	{//クリアフラグ
		textclear(sg_pDungeonSystem->pDevice_D3D);
	}

	StyleString * m = new StyleString;
	*m = Message;
	psstring pm = psstring(m);
	Messagelogs.push_back(pm);//ログ

	reserveMessages.push_back(pm);//予約


	VisibleCount = Message_VISIBLETIME;
	return true;
}

int cMessageWindow::process(IDirect3DDevice9 *pDev)
{

	if(m_SpriteText.GetY() - forcusTop > (lineNum)*lineheight())
	{
		forcusTop += massageSpeed();
		if(m_SpriteText.GetY() - forcusTop < (lineNum)*lineheight())
		{
			forcusTop = m_SpriteText.GetY() - (lineNum)*lineheight();
		}
	}
	else
	{
		if(!Waiting) VisibleCount--;

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

int cMessageWindow::Draw(IDirect3DDevice9 *pDev)
{
	if(!VisibleCount) return false;

	WindowDraw(pDev);
	TextDraw(pDev);

	if(Waiting)
	{
		//waiting cursol
		static double difference = 0;

		double speed = 0.2;
		difference += speed;
		if(difference > speed * 30)
		{
			difference = - speed * 15;
		}
		cDrawingObject DO;
		DO.setTexture(m_pTexture_Wait,TEX_SIZE_CURSOL,TEX_SIZE_CURSOL);

		DO.m_TexRange.setLTRB(0,0,1,1);
		DO.Width = DO.Height = TEX_SIZE_CURSOL;

		DO.CenterX = sg_pDungeonSystem->MessageW().CenterX;
		DO.CenterY = sg_pDungeonSystem->MessageW().Bottom()-TEX_SIZE_CURSOL/2 + difference;

		DO.Rotation = 0;

		DO.Draw(pDev);
	}

	return true;
}

int cMessageWindow::RenderText(IDirect3DDevice9 *pDev)
{

	if(m_SpriteText.GetY() - forcusTop <= (lineNum)*lineheight())
	{

		m_SpriteText.SetWeight(GWFONTWEIGHT);
		m_SpriteText.BeginText( g_fontstyle().c_str(), GWFONTSIZE );
		{
			m_SpriteText.SetAntiAlias(1);
			m_SpriteText.SetColor( D3DCOLOR_ARGB(255,255,255,255) );
			m_SpriteText.SetReturnWidth(textmaxWidth - m_SpriteText.GetX());
			
			if(reserveMessages.size())
			{
				int prey = m_SpriteText.GetY();
				m_SpriteText.SetCursolPos(0,prey);
				m_SpriteText.DrawText(*reserveMessages[0]);
				if(m_SpriteText.GetY() >= ((Message_MEMORY_POWER-1)*lineNum)*lineheight())
				{
					if(rolldiff == -1)
					{
						rolldiff = prey;
					}
					rollMessages.push_back(reserveMessages[0]);//ロール用
				}
				reserveMessages.pop_front();
			}
		}
		m_SpriteText.EndText();
	}


	if(lineheight()*((Message_MEMORY_POWER)*lineNum) < forcusTop + (lineNum)*lineheight())//テクスチャの外
	{
		NewRoll(pDev);
	}

	return true;
}
int cMessageWindow::TextDraw(IDirect3DDevice9 *pDev)
{
	


	if(reserveMessages.size())
	{
		RenderText(pDev);
	}



	TextDrawSub(pDev);

	return true;
}
int cMessageWindow::TextDrawSub(IDirect3DDevice9 *pDev)
{
	int l = Left();
	int t = Top();
	int r = Right();
	int b = Bottom();

	double ew = EDGEWIDTH;
	double ew_s = sideEdgeWith();

	int iws = Width - ew_s*2;
	int ihs = Height - ew*2;
	int tw = iws;
	int th = ihs;

	tw = largerMinPow2(tw);//tw以上の最小の2^n
	th = largerMinPow2(th);//th以上の最小の2^n


	m_DO.setTexture(m_SpriteText.GetTex(), tw, th);
	

	m_DO.m_color = color;
	m_DO.setLTRB(			l + ew_s,		t + ew,		r - ew_s,		b - ew);
	m_DO.m_TexRange.setLTRB(
		0,
		(forcusTop -linetop(0)-1)/(double)th/Message_MEMORY_POWER,
		iws / (double)tw ,
		(forcusTop -linetop(0)-1)/(double)th/Message_MEMORY_POWER + (ihs / (double)th)/Message_MEMORY_POWER);
	
	
	
	m_DO.m_color = color;
	DrawAddShadowDO(m_DO,pDev);


	//デバック用
	/*
	m_DO.setLTRB(			l + ew + 500,		t + ew,		r - ew + 500,		b - ew);
	m_DO.m_TexRange.setLTRB(0,	0,		iws / (double)tw ,	(ihs / (double)th));
	m_DO.Draw(pDev);
	*/
	
	return true;

}


int cMessageWindow::NewRoll(IDirect3DDevice9 *pDev)
{
	if(rolldiff == -1)
	{
		return false;
	}
	m_SpriteText.CleatText();
	m_SpriteText.SetCursolPos(0,linetop(0));
	forcusTop = forcusTop - rolldiff;

	int i;

	m_SpriteText.SetWeight(GWFONTWEIGHT);
	m_SpriteText.BeginText( g_fontstyle().c_str(), GWFONTSIZE );
	{
		m_SpriteText.SetAntiAlias(1);
		m_SpriteText.SetColor( D3DCOLOR_ARGB(255,255,255,255) );
		m_SpriteText.SetReturnWidth(textmaxWidth - m_SpriteText.GetX());
		
		for(i=0;i<rollMessages.size();i++)
		{
			m_SpriteText.DrawText(*rollMessages[i]);
		}
	}
	m_SpriteText.EndText();

	rolldiff = -1;
	rollMessages.clear();
	return true;
}


int cMessageWindow::textclear(IDirect3DDevice9 *pDev)
{
	m_SpriteText.CleatText();
	m_SpriteText.SetCursolPos(0,linetop(0));
	forcusTop = m_SpriteText.GetY();
	rollMessages.clear();
	reserveMessages.clear();
	rolldiff = -1;
	VisibleCount = 0;
	return true;
}

int cMessageWindow::Wait()
{
	return Waiting = true;
}
int cMessageWindow::unWait()
{
	return Waiting = false;
}

double cMessageWindow::massageSpeed()
{
	if(reserveMessages.size() > 4)
	{
		int addspeednum = reserveMessages.size() - 4;
		return Message_SPEED + 0.5*addspeednum;
	}
	else
	{
		return Message_SPEED;
	}
}