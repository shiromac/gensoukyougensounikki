#include "stdafx.h"

#include "caEchipS.h"



caEchip_Basic::caEchip_Basic(void)
{
	End = false;
	FrameCount = 0;
	hidingTime = 0;
	life = 0;

	ScaleX.set(1,1,1,1);
	ScaleY.set(1,1,1,1);
	Rotation.set(0,0,1,1);
	Rotation2.set(0,0,1,1);
	Movepass.set(0,1,1,1);

	ColorBase.ARGB(255,255,255,255);
	Alpha.set(1,1,1,1);
	DO.m_color.alpha = 0;
}

caEchip_Basic::~caEchip_Basic(void)
{
}

/*
int caEchip_Basic::Init()
{
	return true;
}
*/

void caEchip_Basic::DrowingObjectFunc()
{
	double d = (double)FrameCount/life;

	DO.ScaleX = ((1-d)*ScaleX.x+d*ScaleX.y)*((1-d)*ScaleX.z+d*ScaleX.t);
	DO.ScaleY = ((1-d)*ScaleY.x+d*ScaleY.y)*((1-d)*ScaleY.z+d*ScaleY.t);
	DO.Rotation = ((1-d)*Rotation.x+d*Rotation.y)*((1-d)*Rotation.z+d*Rotation.t);
	DO.Rotation2 = ((1-d)*Rotation2.x+d*Rotation2.y)*((1-d)*Rotation2.z+d*Rotation2.t);
		
	ColorBase.alpha = 255*((1-d)*Alpha.x+d*Alpha.y)*((1-d)*Alpha.z+d*Alpha.t);
	DO.m_color = ColorBase;

	double d2 = ((1-d)*Movepass.x+d*Movepass.y)*((1-d)*Movepass.z+d*Movepass.t);
	DO.CenterX = (1-d2)*Centerbegin.x + d2*Centerend.x;
	DO.CenterY = (1-d2)*Centerbegin.y + d2*Centerend.y -( (1-d2)*Centerbegin.z + d2*Centerend.z)*0.75;

	if(AbsolutePlaceFlag)
	{
		c4DVector p = absoluteRevise();

		DO.CenterX += p.x;
		DO.CenterY += p.y;
	}
	else
	{
		c4DVector p = Baseplace();
		p = sg_pDungeonSystem->Map().visibleplace(p);

		DO.CenterX += p.x;
		DO.CenterY += p.y;
	}

	if(!texRange.empty())
	{
		c4DVector p;
		texRange.getValue(FrameCount,p);
		DO.m_TexRange.setLTRB(p.x,p.y,p.z,p.t);
	}
	if(FrameCount >= life)
	{
		End = true;
	}
}
int caEchip_Basic::Draw(cRenderDevice *pDev)
{
	if(hidingTime <= 0)
	{
		DO.Draw(pDev);	
	}

	return true;
}

//--------------------------------------------------
//caEchip_Num
//--------------------------------------------------

caEchip_Num::caEchip_Num(void)
{
	End = false;
	FrameCount = 0;
	hidingTime = 0;
	life = 0;

	ScaleX.set(1,1,1,1);
	ScaleY.set(1,1,1,1);
	Rotation.set(0,0,1,1);
	Rotation2.set(0,0,1,1);
	Movepass.set(0,1,1,1);

	ColorBase.ARGB(255,255,255,255);
	Alpha.set(1,1,1,1);
	DO.m_color.alpha = 0;
}

caEchip_Num::~caEchip_Num(void)
{
}

/*
int caEchip_Basic::Init()
{
	return true;
}
*/

void caEchip_Num::DrowingObjectFunc()
{
	double d = (double)FrameCount/life;

	//未実装
	//double ScaleX = ((1-d)*ScaleX.x+d*ScaleX.y)*((1-d)*ScaleX.z+d*ScaleX.t);
	//double ScaleY = ((1-d)*ScaleY.x+d*ScaleY.y)*((1-d)*ScaleY.z+d*ScaleY.t);
	//DO.Rotation = ((1-d)*Rotation.x+d*Rotation.y)*((1-d)*Rotation.z+d*Rotation.t);
	//DO.Rotation2 = ((1-d)*Rotation2.x+d*Rotation2.y)*((1-d)*Rotation2.z+d*Rotation2.t);

	num_.color = ColorBase;	
	num_.color.alpha = 255*((1-d)*Alpha.x+d*Alpha.y)*((1-d)*Alpha.z+d*Alpha.t);


	double d2 = ((1-d)*Movepass.x+d*Movepass.y)*((1-d)*Movepass.z+d*Movepass.t);
	num_.position.x = (1-d2)*Centerbegin.x + d2*Centerend.x;
	num_.position.y = (1-d2)*Centerbegin.y + d2*Centerend.y -( (1-d2)*Centerbegin.z + d2*Centerend.z)*0.75;

	if(AbsolutePlaceFlag)
	{
		c4DVector p = absoluteRevise();

		num_.position += p;
	}
	else
	{
		c4DVector p = Baseplace();
		p = sg_pDungeonSystem->Map().visibleplace(p);

		num_.position += p;
	}
	
	if(FrameCount >= life)
	{
		End = true;
	}
}
int caEchip_Num::Draw(cRenderDevice *pDev)
{
	if(hidingTime <= 0)
	{
		num_.Draw(pDev);	
	}

	return true;
}

//--------------------------------------------------
//caEchip_Text
//--------------------------------------------------


caEchip_Text::caEchip_Text(void)
{
	Text = _T("");
	Font = g_fontstyle().c_str();
	fontsize = 16;

	letterXnum = 0;
	letterYnum = 0;

	dropshadow = true;
}

caEchip_Text::~caEchip_Text(void)
{

}
int caEchip_Text::Init()
{

	setTextTexture();

	return true;
}
int caEchip_Text::Draw(cRenderDevice *pDev)
{

	if(dropshadow)
	{
		cDrawingObject DO2 = DO;

		DO2.m_color.ARGB(196,0,0,0);
		DO2.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;
		DO2.CenterX += 1;
		DO2.CenterY += 1;

		DO2.Draw(pDev);

		DO2.CenterX += 1;
		DO2.CenterY += 1;

		DO2.Draw(pDev);
	}

	caEchip_Basic::Draw(pDev);

	return true;
}

void caEchip_Text::setTextTexture()
{
	if(letterXnum == 0)
	{
		letterXnum = Text.length();
	}
	if(letterYnum == 0)
	{
		letterYnum = 1;
	}

	Width = letterXnum * GWFONTSIZE;
	Height = letterYnum * GWFONTSIZE;



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


	stext.Init(sg_pDungeonSystem->pDevice_D3D, 1, tw, th);



	stext.CleatText();
	stext.SetWeight(GWFONTWEIGHT);
	stext.BeginText( g_fontstyle().c_str(), GWFONTSIZE );
	stext.SetAntiAlias(1);
	stext.SetColor( D3DCOLOR_ARGB(255,255,255,255) );
	stext.SetReturnWidth(Width);
	stext.SetCursolPos(1, 0);
	stext.DrawText(Text);
	stext.EndText();

	DO.setTexture(stext.GetTex(),tw,th);
	DO.m_TexRange.setLTRB(0,0,1,1);
}

//-------------------------------------------------
//ベルト
//-------------------------------------------------

caEchip_Belt::caEchip_Belt(void)
{
	End = false;
	FrameCount = 0;
	hidingTime = 0;
	life = 0;


	Startp.set(0,0,1,1);
	Endp.set(0,0,1,1);

	ColorBase.ARGB(255,255,255,255);
	Alpha.set(1,1,1,1);
}

caEchip_Belt::~caEchip_Belt(void)
{
}

/*
int caEchip_Basic::Init()
{
	return true;
}
*/

void caEchip_Belt::DrowingObjectFunc()
{
	double d = (double)FrameCount/life;

	DOb.start = ((1-d)*Startp.x+d*Startp.y)*((1-d)*Startp.z+d*Startp.t);
	DOb.end = ((1-d)*Endp.x+d*Endp.y)*((1-d)*Endp.z+d*Endp.t);
	
	ColorBase.alpha = 255*((1-d)*Alpha.x+d*Alpha.y)*((1-d)*Alpha.z+d*Alpha.t);
	DOb.m_color = ColorBase;

	DOb.rightStartpoint() = CenterRight;
	DOb.leftStartpoint() = CenterLeft;
	
	if(AbsolutePlaceFlag)
	{
		c4DVector p = absoluteRevise();

		DOb.rightStartpoint() += p;
		DOb.leftStartpoint() += p;
	}
	else
	{
		c4DVector p = Baseplace();
		p = sg_pDungeonSystem->Map().visibleplace(p);

		DOb.rightStartpoint() += p;
		DOb.leftStartpoint() += p;
	}

	if(!texRange.empty())
	{
		c4DVector p;
		texRange.getValue(FrameCount,p);
		DOb.m_TexRange.setLTRB(p.x,p.y,p.z,p.t);
	}

	if(FrameCount >= life)
	{
		End = true;
	}
}
int caEchip_Belt::Draw(cRenderDevice *pDev)
{
	if(hidingTime <= 0)
	{
		DOb.Draw(pDev);	
	}

	return true;
}


//-------------------------------------------------
//バウンド
//-------------------------------------------------


caEchip_Bound::caEchip_Bound(void)
{
	End = false;
	FrameCount = 0;
	hidingTime = 0;
	life = 0;
	gravity = 1;//重力加速度(コマ/F)
	floorHeight = 0;//床の高さ
	restitution = 0.8;//反発係数
	boundlimit = 8;

	ScaleX.set(1,1,1,1);
	ScaleY.set(1,1,1,1);
	Rotation.set(0,0,1,1);
	Rotation2.set(0,0,1,1);

	ColorBase.ARGB(255,255,255,255);
	Alpha.set(1,1,1,1);
	DO.m_color.alpha = 0;

	FragFirstDOF = true;
}

caEchip_Bound::~caEchip_Bound(void)
{
}

/*
int caEchip_Basic::Init()
{
	return true;
}
*/

void caEchip_Bound::DrowingObjectFunc()
{

	if(FragFirstDOF == true)
	{
		NowCenter = Centerbegin;
		FragFirstDOF = false;
	}


	double d = (double)FrameCount/life;

	DO.ScaleX = ((1-d)*ScaleX.x+d*ScaleX.y)*((1-d)*ScaleX.z+d*ScaleX.t);
	DO.ScaleY = ((1-d)*ScaleY.x+d*ScaleY.y)*((1-d)*ScaleY.z+d*ScaleY.t);
	DO.Rotation = ((1-d)*Rotation.x+d*Rotation.y)*((1-d)*Rotation.z+d*Rotation.t);
	DO.Rotation2 = ((1-d)*Rotation2.x+d*Rotation2.y)*((1-d)*Rotation2.z+d*Rotation2.t);
		
	ColorBase.alpha = 255*((1-d)*Alpha.x+d*Alpha.y)*((1-d)*Alpha.z+d*Alpha.t);
	DO.m_color = ColorBase;

	NowCenter += moveVector;

	moveVector.z += -gravity;
	
	DO.CenterX = NowCenter.x;
	DO.CenterY = NowCenter.y - NowCenter.z;
	
	if(AbsolutePlaceFlag)
	{
		c4DVector p = absoluteRevise();

		DO.CenterX += p.x;
		DO.CenterY += p.y;
	}
	else
	{
		c4DVector p = Baseplace();
		p = sg_pDungeonSystem->Map().visibleplace(p);

		DO.CenterX += p.x;
		DO.CenterY += p.y;
	}

	if(floorHeight > NowCenter.z && moveVector.z <= 0)
	{//バウンド
		boundlimit--;
		if(boundlimit <= 0) FrameCount = life;
		moveVector.z *= -restitution;
	}

	if(!texRange.empty())
	{
		c4DVector p;
		texRange.getValue(FrameCount,p);
		DO.m_TexRange.setLTRB(p.x,p.y,p.z,p.t);
	}

	if(FrameCount >= life)
	{
		End = true;
	}
}
int caEchip_Bound::Draw(cRenderDevice *pDev)
{
	if(hidingTime <= 0)
	{
		DO.Draw(pDev);	
	}

	return true;
}


