
#include "stdafx.h"
#include "cGameScreenInterface.h"


#define INTERFACEFONTSIZE 32


//#define MAXBUFFSIZE 256
#define TEXSIZEWIDTH 1024
#define TEXSIZEHEIGHT 64

#define STILE_TOP 4

#define STILE_FLOOR_X 20
#define STILE_FLOOR_D 8

#define STILE_MONEY_X (SCREEN_X-20)
#define STILE_MONEY_D 8

#define STILE_HP_X 256
#define STILE_HP_D 8

#define STILE_HP_BAR_EDGE 2
#define STILE_HP_BAR_MEASURE 2
#define STILE_HP_BAR_HEIGHT 20

#define STILE_ST_X 256
#define STILE_ST_D 8


#define STILE_ST_BAR_EDGE 2
#define STILE_ST_BAR_MEASURE 2
#define STILE_ST_BAR_HEIGHT 4

#define STILE_LV_X 464
#define STILE_LV_D 10

#define D3DCOLOR_CONSTCOLOR D3DCOLOR_ARGB(255,200,255,240)
#define D3DCOLOR_LETTERCOLOR D3DCOLOR_ARGB(255,255,255,255)

cGameScreenInterface::cGameScreenInterface(void)
{
	money.value = 0;
	FloorLevel = 0;
	(disphero_HP = 5);
	(disphero_MHP = 5);
	(disphero_LV = 0);
	(disphero_Stomach = 100);	
	(disphero_MaxStomach = 100);
}

cGameScreenInterface::~cGameScreenInterface(void)
{
}

int cGameScreenInterface::Init(IDirect3DDevice9 *pDev,pcCharacter phero)
{
	text.Init(pDev,1,TEXSIZEWIDTH,TEXSIZEHEIGHT);
	
	hero = phero;

	heroCopy = pcCharacter(new cPlayerChara);

	Resetdisp(pDev);
	RefreshDraw(pDev);


	HPGage.edge_width = STILE_HP_BAR_EDGE;
	HPGage.edgeColor.ARGB(200,230,230,230);
	HPGage.forwardColor.ARGB(200,0,230,0);
	HPGage.backColor.ARGB(200,180,0,0);
	HPGage.drawmode = 1;


	STGage.edge_width = STILE_ST_BAR_EDGE;
	STGage.edgeColor.ARGB(200,230,230,230);
	STGage.forwardColor.ARGB(200,230,170,0);
	STGage.backColor.ARGB(200,100,0,200);
	STGage.drawmode = 1;

	return true;
}

int cGameScreenInterface::process(IDirect3DDevice9 *pDev)
{

	return true;
}

int cGameScreenInterface::RefreshDraw(IDirect3DDevice9 *pDev)
{
	cDrawingObject DO;
	

	//TCHAR t[MAXBUFFSIZE];

	text.CleatText();
	text.SetWeight(GWFONTWEIGHT);
	text.BeginText( g_fontstyle().c_str(), INTERFACEFONTSIZE );
	{
		text.SetAntiAlias(1);

		text.SetReturnWidth(1024);//Ž©“®return‚µ‚È‚¢
		text.SetRot(0.0);


		text.SetColor( D3DCOLOR_CONSTCOLOR );
		//ŠK‘w•\Ž¦
		//_stprintf(t,);
		text.SetCursolPos(STILE_FLOOR_X,STILE_TOP);
		StyleString sstr = g_Lang(_T("‘æ"));
		sstr.setSameColor(D3DCOLOR_CONSTCOLOR);
		text.DrawText(sstr);

		//_stprintf(t,);
		text.SetCursolPos(STILE_FLOOR_X + INTERFACEFONTSIZE*2 + STILE_FLOOR_D*2,STILE_TOP);
		sstr = g_Lang(_T("ŠK‘w"));
		sstr.setSameColor(D3DCOLOR_CONSTCOLOR);
		text.DrawText(sstr);
		
		text.SetColor( D3DCOLOR_LETTERCOLOR );
		//_stprintf(t,_T("%2d"),dispFloorLevel);
		text.SetCursolPos(STILE_FLOOR_X + INTERFACEFONTSIZE*1 + STILE_FLOOR_D*1,STILE_TOP);
		text.DrawText(setStyle(dispFloorLevel,_T("%2d")).c_str());



		//‚¨‹à•\Ž¦
		if(dispmoney.caption_BorS())
		{//‰~
			text.SetColor( D3DCOLOR_LETTERCOLOR );
			//_tcscpy(t, dispmoney.caption_onlynumberB().c_str());
			//text.SetCursolPos(STILE_MONEY_X - dispmoney.caption_length()*INTERFACEFONTSIZE,STILE_TOP);
			text.SetCursolPos(STILE_MONEY_X - text.GetTextWidth(dispmoney.caption_onlynumberB().c_str()) - INTERFACEFONTSIZE*3.75, STILE_TOP);
			text.DrawText(dispmoney.caption_onlynumberB().c_str());

			text.SetColor( D3DCOLOR_CONSTCOLOR );
			//_tcscpy(t, dispmoney.caption_onlymeasureB().c_str());
			//text.SetCursolPos(STILE_MONEY_X - INTERFACEFONTSIZE*3 - STILE_MONEY_D*2 ,STILE_TOP);
			text.SetCursolPos(STILE_MONEY_X - text.GetTextWidth(dispmoney.caption_onlymeasureB().c_str()) - INTERFACEFONTSIZE*2.5, STILE_TOP);
			text.DrawText(dispmoney.caption_onlymeasureB().c_str());
		}

		text.SetColor( D3DCOLOR_LETTERCOLOR );
		//_tcscpy(t, dispmoney.caption_onlynumberS().c_str());
		text.SetCursolPos(STILE_MONEY_X - INTERFACEFONTSIZE*2 - STILE_MONEY_D*1 ,STILE_TOP);
		text.DrawText(dispmoney.caption_onlynumberS().c_str());

		text.SetColor( D3DCOLOR_CONSTCOLOR );
		//_tcscpy(t, dispmoney.caption_onlymeasureS().c_str());
		text.SetCursolPos(STILE_MONEY_X - INTERFACEFONTSIZE*1 ,STILE_TOP);
		text.DrawText(dispmoney.caption_onlymeasureS().c_str());

		//HP
		text.SetColor( D3DCOLOR_CONSTCOLOR );
		//_stprintf(t,);
		text.SetCursolPos(STILE_HP_X,STILE_TOP);
		text.DrawText(_T("HP"));

		text.SetColor( D3DCOLOR_LETTERCOLOR );
		//_stprintf(t,_T("%d/%d"),disphero_HP,disphero_MHP);
		text.SetCursolPos(STILE_HP_X + INTERFACEFONTSIZE*1 + STILE_HP_D,STILE_TOP);
		text.DrawText((setStyle(disphero_HP)+_T("/")+setStyle(disphero_MHP)).c_str());

		//LV
		text.SetColor( D3DCOLOR_CONSTCOLOR );
		//_stprintf(t,_T("Lv."));
		text.SetCursolPos(STILE_LV_X,STILE_TOP);
		text.DrawText(_T("Lv."));

		text.SetColor( D3DCOLOR_LETTERCOLOR );
		//_stprintf(t,_T("%d"),disphero_LV);
		text.SetCursolPos(STILE_LV_X + INTERFACEFONTSIZE*1 + STILE_LV_D,STILE_TOP);
		text.DrawText(setStyle(disphero_LV));
	}
	text.EndText();


	return true;
}
int cGameScreenInterface::Resetdisp(IDirect3DDevice9 *pDev)
{
	if(!hero) return false;
	(disphero_HP = hero->HP);
	(disphero_MHP = hero->MHP);
	(disphero_LV = hero->LV);
	(disphero_Stomach = hero->Stomach);	
	(disphero_MaxStomach = hero->MaxStomach);
	(disphero_emotion = hero->emotion);

	(dispmoney.value = money.value);
	(dispFloorLevel = FloorLevel);
	return true;
}
	
int cGameScreenInterface::isHeroPchange(IDirect3DDevice9 *pDev)
{
	int flag = false;
	flag |= (heroCopy->HP != disphero_HP);
	flag |= (heroCopy->MHP != disphero_MHP);
	flag |= (heroCopy->LV != disphero_LV);
	flag |= (heroCopy->Stomach != disphero_Stomach);	
	flag |= (heroCopy->MaxStomach != disphero_MaxStomach);
	flag |= (heroCopy->emotion != disphero_emotion);

	flag |= (moneyCopy.value != dispmoney.value);
	flag |= (FloorLevelCopy != dispFloorLevel);


	(heroCopy->HP = disphero_HP);
	(heroCopy->MHP = disphero_MHP);
	(heroCopy->LV = disphero_LV);
	(heroCopy->Stomach = disphero_Stomach);	
	(heroCopy->MaxStomach = disphero_MaxStomach);
	(heroCopy->emotion = disphero_emotion);

	(moneyCopy.value = dispmoney.value);
	(FloorLevelCopy = dispFloorLevel);


	return flag;
}
int cGameScreenInterface::menuPosTop(int level)
{

	switch(level)
	{
		case 1: return 32*2;
		case 2: return 32*3;
		case 3: return 32*4;
		case 4: return 32*5;
		case 5: return 32*6;
	}
	return 0;
}
int cGameScreenInterface::menuPosLeft(int level)
{
	switch(level)
	{
		case 1: return 32*0 + 56;
		case 2: return 32*1 + 56;
		case 3: return 32*2 + 56;
		case 4: return 32*3 + 56;
		case 5: return 32*4 + 56;
	}
	return 0;
}
int cGameScreenInterface::menuPosRight(int level)
{
	switch(level)
	{
		case 1: return -32*1 + SCREEN_X;
		case 2: return -32*2 + SCREEN_X;
		case 3: return -32*3 + SCREEN_X;
		case 4: return -32*4 + SCREEN_X;
		case 5: return -32*5 + SCREEN_X;
	}
	return SCREEN_X;
}
int cGameScreenInterface::menuPosBottom(int level)
{
	switch(level)
	{
		case 1: return SCREEN_Y;
		case 2: return -32*1 + SCREEN_Y;
		case 3: return -32*2 + SCREEN_Y;
		case 4: return -32*3 + SCREEN_Y;
		case 5: return -32*4 + SCREEN_Y;
	}
	return SCREEN_Y;
}

int cGameScreenInterface::Draw(IDirect3DDevice9 *pDev)
{
	cDrawingObject DO;
	

	
	if(isHeroPchange(pDev))
	{
		RefreshDraw(pDev);
	}

	DO.setTexture(text.GetTex(),TEXSIZEWIDTH,TEXSIZEHEIGHT);
	DO.m_TexRange.setLTRB(0,0,1,1);


	DO.setLTRB(2,2,TEXSIZEWIDTH+2,TEXSIZEHEIGHT+2);	
	DO.m_color.ARGB(192,0,0,0);
	DO.Draw(pDev);

	DO.m_color.ARGB(255,255,255,255);
	DO.setLTRB(0,0,TEXSIZEWIDTH,TEXSIZEHEIGHT);	

	DO.Draw(pDev);


	//HP
	HPGage.Value = disphero_HP;
	HPGage.MaxValue = disphero_MHP;



	HPGage.Width = HPGage.MaxValue*STILE_HP_BAR_MEASURE;
	HPGage.Height = STILE_HP_BAR_HEIGHT;
	
	HPGage.setTop(STILE_TOP+INTERFACEFONTSIZE);
	HPGage.setLeft(STILE_HP_X);
	if(HPGage.Right() > SCREEN_X-16) HPGage.formRight(SCREEN_X-16);

	HPGage.ShadowDraw(pDev,0,0,128);
	HPGage.Draw(pDev);

	/*
	DO.setTexture(NULL,0,0);
	DO.m_color.ARGB(255,230,230,230);
	DO.setLTRB(STILE_HP_X,STILE_TOP+INTERFACEFONTSIZE,
		STILE_HP_X + STILE_HP_BAR_EDGE*2 + MHP*STILE_HP_BAR_MEASURE,STILE_TOP+INTERFACEFONTSIZE +STILE_HP_BAR_HEIGHT);
	DO.Draw(pDev);


	DO.setTexture(NULL,0,0);
	DO.m_color.ARGB(255,0,230,0);
	DO.setLTRB(STILE_HP_X+STILE_HP_BAR_EDGE,STILE_TOP+INTERFACEFONTSIZE+STILE_HP_BAR_EDGE,
		STILE_HP_X+STILE_HP_BAR_EDGE + HP*STILE_HP_BAR_MEASURE,STILE_TOP+INTERFACEFONTSIZE +STILE_HP_BAR_HEIGHT-STILE_HP_BAR_EDGE);
	DO.Draw(pDev);

	DO.m_color.ARGB(255,230,0,0);
	DO.setLTRB(STILE_HP_X+STILE_HP_BAR_EDGE + (HP)*STILE_HP_BAR_MEASURE,STILE_TOP+INTERFACEFONTSIZE+STILE_HP_BAR_EDGE,
		STILE_HP_X+STILE_HP_BAR_EDGE + MHP*STILE_HP_BAR_MEASURE,STILE_TOP+INTERFACEFONTSIZE +STILE_HP_BAR_HEIGHT-STILE_HP_BAR_EDGE);
	DO.Draw(pDev);
	*/


	//–ž• “x
	STGage.Value = disphero_Stomach;
	STGage.MaxValue = disphero_MaxStomach;



	STGage.Width = STGage.MaxValue*STILE_HP_BAR_MEASURE;
	STGage.Height = STILE_ST_BAR_HEIGHT;
	
	STGage.setTop(STILE_TOP+INTERFACEFONTSIZE +STILE_HP_BAR_HEIGHT + STILE_ST_BAR_EDGE);
	STGage.setLeft(STILE_ST_X);
	if(STGage.Right() > SCREEN_X-16) STGage.formRight(SCREEN_X-16);
	
	STGage.ShadowDraw(pDev,0,0,128);
	STGage.Draw(pDev);

	/*
	int MST = disphero_MaxStomach;
	DO.setTexture(NULL,0,0);
	DO.m_color.ARGB(255,230,230,230);
	DO.setLTRB(STILE_ST_X, STILE_ST_BAR_EDGE+INTERFACEFONTSIZE +STILE_HP_BAR_HEIGHT-STILE_HP_BAR_EDGE,
		STILE_ST_X + STILE_ST_BAR_EDGE*2 + MST*STILE_ST_BAR_MEASURE, STILE_TOP+INTERFACEFONTSIZE +STILE_ST_BAR_HEIGHT +STILE_HP_BAR_HEIGHT-STILE_HP_BAR_EDGE);
	DO.Draw(pDev);

	int ST = disphero_Stomach + 0.99;
	DO.setTexture(NULL,0,0);
	DO.m_color.ARGB(255,230,170,0);
	DO.setLTRB(STILE_ST_X+STILE_ST_BAR_EDGE, STILE_TOP+INTERFACEFONTSIZE+STILE_ST_BAR_EDGE +STILE_HP_BAR_HEIGHT-STILE_HP_BAR_EDGE,
		STILE_ST_X+STILE_ST_BAR_EDGE + ST*STILE_ST_BAR_MEASURE, STILE_TOP+INTERFACEFONTSIZE +STILE_ST_BAR_HEIGHT-STILE_ST_BAR_EDGE +STILE_HP_BAR_HEIGHT-STILE_HP_BAR_EDGE);
	DO.Draw(pDev);

	DO.m_color.ARGB(255,100,0,200);
	DO.setLTRB(STILE_ST_X+STILE_ST_BAR_EDGE + (ST)*STILE_ST_BAR_MEASURE, STILE_TOP+INTERFACEFONTSIZE+STILE_ST_BAR_EDGE +STILE_HP_BAR_HEIGHT-STILE_HP_BAR_EDGE,
		STILE_ST_X+STILE_ST_BAR_EDGE + MST*STILE_ST_BAR_MEASURE, STILE_TOP+INTERFACEFONTSIZE +STILE_ST_BAR_HEIGHT-STILE_ST_BAR_EDGE +STILE_HP_BAR_HEIGHT-STILE_HP_BAR_EDGE);
	DO.Draw(pDev);
	*/
	

	return true;
}
