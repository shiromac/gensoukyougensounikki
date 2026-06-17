#include "stdafx.h"

#include "caEfireS.h"

#include "caEchipS.h"



#define FILENAME_FIRETEX _T("effect\\fire_chip.png")
#define SIZE_FIRETEX (256)

//ptは0~7まで
#define TEXRANGE_FIRETEX_炎(pt) abs(4-(pt))/4.0,0/4.0,abs(4-(pt+1))/4.0,1/4.0

#define TEXRANGE_FIRETEX_明かり 1/4.0,3/4.0,2/4.0,4/4.0

#define TEXRANGE_FIRETEX_火炎雲 0/4.0,3/4.0,1/4.0,4/4.0
#define TEXRANGE_FIRETEX_火花 2/4.0,3/4.0,3/4.0,4/4.0

#define BOXSIZE 64
//-------------------------------------------------
//一マス燃え
//-------------------------------------------------
caEfire_flame::caEfire_flame(int x,int y,int z,int layer,int lightflag)
{
	place.set(x,y,z,0);
	End = false;
	preprocessed = 0;
	LayerLevel = layer;
	lightflag_ = lightflag;
}

caEfire_flame::~caEfire_flame(void)
{
}


int caEfire_flame::Init()
{
	return true;
}

int caEfire_flame::preprocess(cRenderDevice *pDev)
{
	int i,num;
	caEchip_Basic cacb_model;

	//テクスチャセット
	cacb_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
							pDev,FILENAME_FIRETEX),SIZE_FIRETEX,SIZE_FIRETEX);

	//基本サイズ
	cacb_model.DO.Width = 80;
	cacb_model.DO.Height = 80;


	//描画基本位置
	//c4DVector center;
	//center = sg_pDungeonSystem->Map().realvisibleplace(place);
	cacb_model.AbsolutePlaceFlag = 0; cacb_model.place.set(place.x,place.y,0,0);
	
	//加算
	cacb_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

	//色
	cacb_model.ColorBase.ARGB(255,255,255,255);
	cacb_model.Alpha.set(0,2,2,0);

	//表示時間
	if(lightflag_) cacb_model.life = 20;
	else cacb_model.life = 60;


	//--------------------------------------------------------------------
	//明かり
	//--------------------------------------------------------------------
	//テクスチャ座標セット
	cacb_model.DO.m_TexRange.setLTRB(TEXRANGE_FIRETEX_明かり);

	//描画位置
	//cacb_model.Centerbegin = center;
	//cacb_model.Centerend = center;

	//加算
	cacb_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

	//色
	cacb_model.ColorBase.ARGB(255,255,128,0);
	cacb_model.Alpha.set(0,2,2,0);

	cacb_model.hidingTime = 0;
	if(lightflag_) cacb_model.life = 20;
	else cacb_model.life = 55;

	
	pcAnimationChipManager_chip pcac = pcAnimationChipManager_chip(new caEchip_Basic(cacb_model));

	
	addAnimechip(pcac);




	//--------------------------------------------------------------------
	//煙
	//--------------------------------------------------------------------
	//テクスチャ座標セット
	cacb_model.DO.m_TexRange.setLTRB(TEXRANGE_FIRETEX_火炎雲);
	
	//描画位置
	//cacb_model.Centerbegin = center;
	//cacb_model.Centerend.set(center.x, center.y - BOXSIZE*0.7,0,0);
	cacb_model.Centerbegin.set(0,0,0,0);
	cacb_model.Centerend.set(0, - BOXSIZE*0.7,0,0);

	//加算
	cacb_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;

	//色
	cacb_model.ColorBase.ARGB(255,60,20,0);
	cacb_model.Alpha.set(0,2,2,0);
	for(i=0;i<5;i++)
	{
		double dx = ((double)rand()/RAND_MAX)*2-1;//[-1,1]くらい
		double dy = ((double)rand()/RAND_MAX)*2-1;
		double dr = ((double)rand()/RAND_MAX)*2-1;
		double dr2 = ((double)rand()/RAND_MAX)*2-1;
		double dl = ((double)rand()/RAND_MAX);//[0,1]くらい
		double dh = ((double)rand()/RAND_MAX);//[0,1]くらい

		cacb_model.Centerbegin.x = dx*BOXSIZE/2;
		cacb_model.Centerbegin.y = dy*BOXSIZE/2;
		
		if(lightflag_)
		{
			cacb_model.life = dh*5;
			cacb_model.life = dl*5+8;
		}
		else
		{
			cacb_model.hidingTime = dh*30;
			cacb_model.life = dl*15+8;
		}
		cacb_model.Rotation.set(dr*360,dr2*360,1,1);

		pcAnimationChipManager_chip pcac = pcAnimationChipManager_chip(new caEchip_Basic(cacb_model));

	
		addAnimechip(pcac);

	}


	//--------------------------------------------------------------------
	//火（煙状）
	//--------------------------------------------------------------------
	//加算
	cacb_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;
	num = 15;
	if(lightflag_)
	{
		num = 3;
	}
	//色
	cacb_model.ColorBase.ARGB(255,255,255,255);
	cacb_model.Alpha.set(0,2,2,0);
	for(i=0;i<num;i++)
	{
		double dx = ((double)rand()/RAND_MAX)*2-1;//[-1,1]くらい
		double dy = ((double)rand()/RAND_MAX)*2-1;
		double dr = ((double)rand()/RAND_MAX)*2-1;
		double dr2 = ((double)rand()/RAND_MAX)*2-1;
		double dl = ((double)rand()/RAND_MAX);//[0,1]くらい
		double dh = ((double)rand()/RAND_MAX);//[0,1]くらい

		cacb_model.Centerbegin.x = + dx*BOXSIZE/2;
		cacb_model.Centerbegin.y = + dy*BOXSIZE/2;
	
		if(lightflag_)
		{
			cacb_model.life = dh*5;
			cacb_model.life = dl*5+10;
		}
		else
		{
			cacb_model.hidingTime = dh*30;
			cacb_model.life = dl*15+10;
		}

		cacb_model.Rotation.set(dr*360,dr2*360,1,1);

		pcAnimationChipManager_chip pcac = pcAnimationChipManager_chip(new caEchip_Basic(cacb_model));

	
		addAnimechip(pcac);

	}

	//--------------------------------------------------------------------
	//火花
	//--------------------------------------------------------------------
	//テクスチャ座標セット
	cacb_model.DO.m_TexRange.setLTRB(TEXRANGE_FIRETEX_火花);

	//加算
	cacb_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

	//色
	cacb_model.ColorBase.ARGB(255,255,255,255);
	cacb_model.Alpha.set(0,2,2,0);
	for(i=0;i<5;i++)
	{
		double dx = ((double)rand()/RAND_MAX)*2-1;//[-1,1]くらい
		double dy = ((double)rand()/RAND_MAX)*2-1;
		double dr = ((double)rand()/RAND_MAX)*2-1;
		double dr2 = ((double)rand()/RAND_MAX)*2-1;
		double dl = ((double)rand()/RAND_MAX);//[0,1]くらい
		double dh = ((double)rand()/RAND_MAX);//[0,1]くらい

		cacb_model.Centerbegin.x = + dx*BOXSIZE/2;
		cacb_model.Centerbegin.y = + dy*BOXSIZE/2;
		
		if(lightflag_)
		{
			cacb_model.life = dh*5;
			cacb_model.life = dl*5+8;
		}
		else
		{
			cacb_model.hidingTime = dh*30;
			cacb_model.life = dl*15+8;
		}

		cacb_model.Rotation.set(dr*360,dr2*360,1,1);

		pcAnimationChipManager_chip pcac = pcAnimationChipManager_chip(new caEchip_Basic(cacb_model));

	
		addAnimechip(pcac);

	}


	//--------------------------------------------------------------------
	//炎
	//--------------------------------------------------------------------
	//テクスチャ座標セット
	cacb_model.DO.m_TexRange.setLTRB(TEXRANGE_FIRETEX_火花);

	//加算
	cacb_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

	//位置
	//cacb_model.Centerbegin = center;
	//cacb_model.Centerend.set(center.x, center.y - BOXSIZE*1,0,0);
	cacb_model.Centerbegin.set(0,0,0,0);
	cacb_model.Centerend.set(0, - BOXSIZE*1,0,0);

	//色
	cacb_model.ColorBase.ARGB(255,255,255,255);
	cacb_model.Alpha.set(0,2,2,0);
	num = 15;
	if(lightflag_)
	{
		num = 3;
	}
	for(i=0;i<num;i++)
	{
		double dx = ((double)rand()/RAND_MAX)*2-1;//[-1,1]くらい
		double dy = ((double)rand()/RAND_MAX)*2-1;
		double dr2 = ((double)rand()/RAND_MAX)*2-1;
		double dl = ((double)rand()/RAND_MAX);//[0,1]くらい
		double dh = ((double)rand()/RAND_MAX);//[0,1]くらい

		int dpt = ((double)rand()/(RAND_MAX+1))*8;//[0,8)くらい
		//テクスチャ座標セット
		cacb_model.DO.m_TexRange.setLTRB(TEXRANGE_FIRETEX_炎(dpt));

		cacb_model.Centerbegin.x = dx*BOXSIZE/2;
		cacb_model.Centerbegin.y = dy*BOXSIZE/2;
		
		if(lightflag_)
		{
			cacb_model.life = dh*5;
			cacb_model.life = dl*5+10;
			cacb_model.Centerend.x = cacb_model.Centerbegin.x;
			cacb_model.Centerend.y = cacb_model.Centerbegin.y - BOXSIZE/2;
		}
		else
		{
			cacb_model.hidingTime = dh*30;
			cacb_model.life = dl*10+20;
		}


		cacb_model.Rotation.set(0,0,1,1);
		if(lightflag_)
		{
			cacb_model.ScaleX.set(1,	0.5,	0.5,	0.5);
			cacb_model.ScaleY.set(0.2,	2,	0.25,	0.5);
		}
		else
		{
			cacb_model.ScaleX.set(1,	0.5,	1,	1);
			cacb_model.ScaleY.set(0.2,	2,	0.5,	1);
		}
		pcAnimationChipManager_chip pcac = pcAnimationChipManager_chip(new caEchip_Basic(cacb_model));

	
		addAnimechip(pcac);

	}
	
	
	return true;
}

int caEfire_flame::process(cRenderDevice *pDev)
{

	if(preprocessed==0)
	{//初回

		preprocess(pDev);
		preprocessed = 1;
	}

	chipprocess(pDev);

	if(chipEmpty())
	{//オワリ
		End = true;
	}
	
	return true;
}

int caEfire_flame::Draw(cRenderDevice *pDev)
{
	chipDraw(pDev);


	return true;
}


bool caEfire_flame::isOnScreen()
{

	if(	sg_pDungeonSystem->Map().isOnDisplay(place.x, place.y, 1) )
	{
		return true;
	}
	

	return false;
}
