#include "stdafx.h"

#include "caEspellS.h"

#include "caEchipS.h"


#include "cGameWindow.h"
#include "cCharacter.h"




#define DEF_FONT_SIZE 16



#define DEF_ANIME_PHASE1 (30)
#define DEF_ANIME_PHASE2 (60)
#define DEF_ANIME_PHASE3 (90)
#define DEF_ANIME_PHASE4 (180)
#define DEF_ANIME_PHASE5 (195)
//-------------------------------------------------
//
//-------------------------------------------------
caEspell::caEspell(int x,int y,int z,int layer)
{

	End = false;
	preprocessed = 0;
	LayerLevel = layer;

	color1 = 0xFF5090FF;
	color2 = 0xFF5090FF;


	m_count = 0;
}

caEspell::~caEspell(void)
{
}


int caEspell::Init()
{
	return true;
}

int caEspell::preprocess(IDirect3DDevice9 *pDev)
{

	SpellName_Echip.dropshadow = true;
	SpellName_Echip.Text = SpellName;
	//SpellName_Echip.Font = ;
	SpellName_Echip.fontsize = DEF_FONT_SIZE;
	SpellName_Echip.letterXnum = CAESPELL_BACKNUM;
	SpellName_Echip.letterYnum = 1;
	SpellName_Echip.Init();

	SpellName_Echip.life = 1000;
	SpellName_Echip.hidingTime = 0;


	int i;
	for(i=0;i<CAESPELL_BACKNUM;i++)
	{
		backdif[i].set(10-rand()*20/RAND_MAX,10-rand()*20/RAND_MAX,10-rand()*20/RAND_MAX,10-rand()*20/RAND_MAX);
	}


	backDO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
							pDev,_T("effect\\basic\\ball_antismoke.png")),SIZE_BASICTEX,SIZE_BASICTEX);
	backDO.m_TexRange.setLTRB(0,0,1,1);
	underDO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
							pDev,_T("effect\\basic\\ball_simple.png")),SIZE_BASICTEX,SIZE_BASICTEX);
	underDO.m_TexRange.setLTRB(0,0,1,1);

	return true;
}

int caEspell::process(IDirect3DDevice9 *pDev)
{

	if(preprocessed==0)
	{//初回

		preprocess(pDev);
		preprocessed = 1;
	}

	
	if(m_count<DEF_ANIME_PHASE1)
	{
		leftbottom.set(POSITION_BEGIN_X,POSITION_BEGIN_Y,0,0);
	
		SpellName_Echip.DO.m_color.ARGB(m_count*255/DEF_ANIME_PHASE1,255,255,255);
		SpellName_Echip.DO.ScaleX = 1 + 2*(1 - (double)m_count/DEF_ANIME_PHASE1);
		SpellName_Echip.DO.ScaleY = 1 + 2*(1 - (double)m_count/DEF_ANIME_PHASE1);
	
	
		backDO.m_color.inputD3Dcolor(color1);
		backDO.m_color.alpha = m_count*255/DEF_ANIME_PHASE1;

		underDO.m_color.inputD3Dcolor(color2);
		underDO.m_color.alpha = m_count*255/DEF_ANIME_PHASE1;
	}
	else if(m_count<DEF_ANIME_PHASE2)
	{
		SpellName_Echip.DO.m_color.ARGB(255,255,255,255);
		SpellName_Echip.DO.ScaleX = 1;
		SpellName_Echip.DO.ScaleY = 1;

		backDO.m_color.inputD3Dcolor(color1);
		underDO.m_color.inputD3Dcolor(color2);
	}
	else if(m_count<DEF_ANIME_PHASE3)
	{
		leftbottom.y = leftbottom.y*0.8 + POSITION_END_Y*0.2;
	}
	else if(m_count<DEF_ANIME_PHASE4)
	{
		leftbottom.set(POSITION_BEGIN_X,POSITION_END_Y,0,0);
	}
	else if(m_count<DEF_ANIME_PHASE5)
	{
		leftbottom.x = leftbottom.x*0.9 + (-SpellName_Echip.Width)*0.1;
	}
	else
	{//オワリ
		End = true;
	}
	
	
	m_count++;
	
	return true;
}

int caEspell::Draw(IDirect3DDevice9 *pDev)
{

	//字
	SpellName_Echip.DO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;
	SpellName_Echip.DO.Width = SpellName_Echip.letterXnum * SpellName_Echip.fontsize;
	SpellName_Echip.DO.Height = SpellName_Echip.letterYnum * SpellName_Echip.fontsize;
	SpellName_Echip.DO.setLeft(leftbottom.x + (SpellName_Echip.DO.ScaleX-1)*SpellName_Echip.DO.Width/3);
	SpellName_Echip.DO.setBottom(leftbottom.y);

	//バック
	int i;

	backDO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;

	for(i=0;i<CAESPELL_BACKNUM && i<SpellName_Echip.Text.length()/2 ;i++)
	{
		backDO.Width = 32 + backdif[i].z;
		backDO.Height = 32 + backdif[i].z;
		backDO.CenterX = leftbottom.x + DEF_FONT_SIZE*i+ DEF_FONT_SIZE/2 + backdif[i].x*backdif[i].t/10;
		backDO.CenterY = SpellName_Echip.DO.CenterY + backdif[i].y*backdif[i].t/10;
		backDO.Draw(pDev);
	}

	//アンダーライン
	underDO.m_color.inputD3Dcolor(color2);
	underDO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;
	underDO.Width = (SpellName_Echip.letterXnum + 10) * SpellName_Echip.fontsize;
	underDO.Height = 8;
	underDO.CenterX = leftbottom.x + SpellName_Echip.Text.length()*SpellName_Echip.fontsize/4;
	underDO.setBottom(leftbottom.y+SpellName_Echip.fontsize/2);
	underDO.ScaleX = 1;
	underDO.ScaleY = 1;
	underDO.Draw(pDev);

	//underDO.m_color.inputD3Dcolor(0xFFFFFFFF);
	underDO.ScaleX = 0.6;
	underDO.ScaleY = 0.6;
	underDO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;
	underDO.Draw(pDev);


	//SpellName_Echip.Draw(pDev);
	DrawAddShadowDO_mini(SpellName_Echip.DO,pDev);


	return true;
}


bool caEspell::isOnScreen()
{


	return true;

}



//-------------------------------------------------
//ボススペル
//-------------------------------------------------
caBossEspell::caBossEspell(pcCharacter boss, tstring spellName, unsigned long lcolor1, unsigned long lcolor2)
{
	boss_ = wpcCharacter(boss);

	End = false;
	preprocessed = 0;

	color1 = lcolor1;
	color2 = lcolor2;


	m_count = 0;
	spellTurn_chip.Value_ = 0;
	hp_tinkle = 0;

	
	turn_ = 0;
	restSpellNum_ = 0;
	restHPper_ = 0;

	caEspell::SpellName = spellName;

	spellTurn_chip.Init(sg_pDungeonSystem->pDevice_D3D);
	spellTurn_chip.arrangeX = cNumField::ARRANGEX_RIGHT;
	spellTurn_chip.FontHeightSize = 32;
	spellTurn_chip.FontWidthSize = 16;
	spellTurn_chip.color.ARGB(255,255,255,255);
}

caBossEspell::~caBossEspell(void)
{
}


int caBossEspell::Init()
{
	return true;
}

int caBossEspell::preprocess(IDirect3DDevice9 *pDev)
{

	SpellName_Echip.dropshadow = true;
	SpellName_Echip.Text = SpellName;
	//SpellName_Echip.Font = ;
	SpellName_Echip.fontsize = DEF_FONT_SIZE;
	SpellName_Echip.letterXnum = CAESPELL_BACKNUM;
	SpellName_Echip.letterYnum = 1;
	SpellName_Echip.Init();

	SpellName_Echip.life = 1000;
	SpellName_Echip.hidingTime = 0;


	int i;
	for(i=0;i<CAESPELL_BACKNUM;i++)
	{
		backdif[i].set(10-rand()*20/RAND_MAX,10-rand()*20/RAND_MAX,10-rand()*20/RAND_MAX,10-rand()*20/RAND_MAX);
	}


	backDO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
							pDev,_T("effect\\basic\\ball_antismoke.png")),SIZE_BASICTEX,SIZE_BASICTEX);
	backDO.m_TexRange.setLTRB(0,0,1,1);
	underDO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
							pDev,_T("effect\\basic\\ball_simple.png")),SIZE_BASICTEX,SIZE_BASICTEX);
	underDO.m_TexRange.setLTRB(0,0,1,1);

	spellDO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
							pDev,_T("interface\\SpellStar.png")));
	spellDO.m_TexRange.setLTRB(0,0,1,1);

	
	hpDO.m_color.ARGB(255,255,40,30);
	hpDO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
							pDev,_T("interface\\bartex.png")));
	hpDO.m_TexRange.setLTRB(0.5,0,0.5,1);
	return true;
}

int caBossEspell::process(IDirect3DDevice9 *pDev)
{

	if(preprocessed==0)
	{//初回

		preprocess(pDev);
		preprocessed = 1;
	}

	
	if(m_count<DEF_ANIME_PHASE1)
	{
		rightbottom.set(POSITION_BEGIN_X,POSITION_BEGIN_Y,0,0);
	
		SpellName_Echip.DO.m_color.ARGB(m_count*255/DEF_ANIME_PHASE1,255,255,255);
		SpellName_Echip.DO.ScaleX = 1 + 2*(1 - (double)m_count/DEF_ANIME_PHASE1);
		SpellName_Echip.DO.ScaleY = 1 + 2*(1 - (double)m_count/DEF_ANIME_PHASE1);
	
	
		backDO.m_color.inputD3Dcolor(color1);
		backDO.m_color.alpha = m_count*255/DEF_ANIME_PHASE1;

		underDO.m_color.inputD3Dcolor(color2);
		underDO.m_color.alpha = m_count*255/DEF_ANIME_PHASE1;

		
		spellDO.m_color.ARGB(m_count*255/DEF_ANIME_PHASE1,255,255,255);
	}
	else if(m_count<DEF_ANIME_PHASE2)
	{
		SpellName_Echip.DO.m_color.ARGB(255,255,255,255);
		SpellName_Echip.DO.ScaleX = 1;
		SpellName_Echip.DO.ScaleY = 1;

		backDO.m_color.inputD3Dcolor(color1);
		underDO.m_color.inputD3Dcolor(color2);
	}
	else if(m_count<DEF_ANIME_PHASE3)
	{
		rightbottom.y = rightbottom.y*0.8 + POSITION_END_Y*0.2;
	}
	else if(m_count<DEF_ANIME_PHASE4)
	{
		rightbottom.set(POSITION_BEGIN_X,POSITION_END_Y,0,0);
	}
	else if(m_count<DEF_ANIME_PHASE5)
	{
		//rightbottom.x = rightbottom.x*0.9 + (-SpellName_Echip.Width)*0.1;
	}
	else
	{//オワリ

		hp_tinkle += 0.05;
		hpDO.m_color.ARGB(255-pow(sin(hp_tinkle),2)*100,255,40 + pow(sin(hp_tinkle),2)*100,30 + pow(cos(hp_tinkle),2)*100);
		if(boss() == NULL)
		{
			//自分からは終わらない
			End = true;
		}
	}


	spellTurn_chip.Value_ = turn_;
	
	
	m_count++;
	
	return true;
}

int caBossEspell::Draw(IDirect3DDevice9 *pDev)
{

	//字
	SpellName_Echip.DO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;
	SpellName_Echip.DO.Width = SpellName_Echip.letterXnum * SpellName_Echip.fontsize;
	SpellName_Echip.DO.Height = SpellName_Echip.letterYnum * SpellName_Echip.fontsize;
	SpellName_Echip.DO.setLeft(rightbottom.x - BOXSIZE - SpellName_Echip.Text.length()*SpellName_Echip.fontsize/2);
	SpellName_Echip.DO.setBottom(rightbottom.y);

	//バック
	int i;

	backDO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;


	for(i=0;i<CAESPELL_BACKNUM && i<SpellName_Echip.Text.length()/2 ;i++)
	{
		backDO.Width = 32 + backdif[i].z;
		backDO.Height = 32 + backdif[i].z;
		backDO.CenterX = rightbottom.x - BOXSIZE - SpellName_Echip.Text.length()*SpellName_Echip.fontsize/2 + DEF_FONT_SIZE*i+ DEF_FONT_SIZE/2 + backdif[i].x*backdif[i].t/10;
		backDO.CenterY = SpellName_Echip.DO.CenterY + backdif[i].y*backdif[i].t/10;
		backDO.Draw(pDev);
	}

	//アンダーライン
	underDO.m_color.inputD3Dcolor(color2);
	underDO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;
	underDO.Width = (SpellName_Echip.letterXnum + 10) * SpellName_Echip.fontsize;
	underDO.Height = 8;
	underDO.CenterX = rightbottom.x - SpellName_Echip.Text.length()*SpellName_Echip.fontsize/4;
	underDO.setBottom(rightbottom.y+SpellName_Echip.fontsize/2);
	underDO.ScaleX = 1;
	underDO.ScaleY = 1;
	underDO.Draw(pDev);

	//underDO.m_color.inputD3Dcolor(0xFFFFFFFF);
	underDO.ScaleX = 0.6;
	underDO.ScaleY = 0.6;
	underDO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;
	underDO.Draw(pDev);


	//前
	hpDO.Width = BOXSIZE*8 *restHPper_;
	hpDO.Height = 12;
	hpDO.setRight(rightbottom.x);
	hpDO.setBottom(rightbottom.y+3);

	hpDO.Draw(pDev);


	for(i=0;i<restSpellNum_ ;i++)
	{
		spellDO.setRight(rightbottom.x - i*16);
		spellDO.setTop(rightbottom.y);
		spellDO.Width = 16;
		spellDO.Height = 16;
		spellDO.Draw(pDev);
	}

	if(spellTurn_chip.Value_ > 0)
	{
		spellTurn_chip.position = rightbottom;
		spellTurn_chip.Draw(pDev);
	}



	//SpellName_Echip.Draw(pDev);
	DrawAddShadowDO_mini(SpellName_Echip.DO,pDev);




	return true;
}
void caBossEspell::setTurn(int turn, bool isBig){
	turn_ = turn;
	if(isBig) {
		spellTurn_chip.FontHeightSize = 32*1.5;
		spellTurn_chip.FontWidthSize = 16*1.5;
		sg_pDungeonSystem->AnimationManager().Anime_PlaySE(_T("countdown.wav"),sg_pDungeonSystem->pPlayerChara()->足元地形()->place);
	}
	else {
		spellTurn_chip.FontHeightSize = 32;
		spellTurn_chip.FontWidthSize = 16;
	}
}

