#include "stdafx.h"

#include "caDamage.h"

#define ANIME_DAMAGE_STEP 10 
#define ANIME_RECOVER_STEP 0 

caDamage::caDamage()
{
	End = false;
	count = 0;
	speed = 1;
}

caDamage::~caDamage()
{
}

int caDamage::Init(pcAnimation me)
{
	cAnimation::Init(me);


	Baseopaque = pDamager->opaque;

	pcaECustom caec = pcaECustom(new caECustom);
	caec->Init(caec);
	caec->place.set(pDamager->placeX, pDamager->placeY, 0, 0);

	c4DVector center;
	pcAnimationChipManager_chip pcac;

	int i;

	caEchip_Num caec_num;
	

	//テクスチャセット
	caec_num.num_.Init(sg_pDungeonSystem->pDevice_D3D);
	if(effective > 1.01)
	{
		double size = min(1.5,effective);
		caec_num.num_.Fontindex = 0;
		caec_num.num_.FontHeightSize = 32*size;
		caec_num.num_.FontWidthSize = 16*size;

		cCoordinate coo(pDamager->placeX, pDamager->placeY);
		sg_pDungeonSystem->AnimationManager().
			Anime_PlaySE(_T("mob32_2.wav"),coo);//レジスト
	}
	else if(effective < 0.99)
	{
		double size = max(0.5,effective);
		caec_num.num_.Fontindex = 1;
		caec_num.num_.FontHeightSize = 32*size;
		caec_num.num_.FontWidthSize = 16;

		cCoordinate coo(pDamager->placeX, pDamager->placeY);
		sg_pDungeonSystem->AnimationManager().
			Anime_PlaySE(_T("mob32.wav"),coo);//レジスト
	}
	else
	{
		caec_num.num_.Fontindex = 1;
		caec_num.num_.FontHeightSize = 32;
		caec_num.num_.FontWidthSize = 16;
	}
	//描画基本位置
	caec_num.AbsolutePlaceFlag = 0;
	caec_num.place.set(pDamager->placeX, pDamager->placeY, 0, 0);
	//基本サイズ

	caec_num.num_.arrangeX = cNumField::ARRANGEX_CENTER;
	caec_num.num_.arrangeY = cNumField::ARRANGEY_CENTER;
	//加算
	//caec_num.num_.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;
	caec_num.num_.Value_ = Damage;
	//色
	caec_num.ColorBase.inputD3Dcolor(0xFFFF0000);
	caec_num.Alpha.set(0,2,2,0);
	//場所
	c4DVector place(0,(-pDamager->GetDrawHeadtall_dotY()*2 + 4),0,0);
	caec_num.Centerbegin = place;
	place.y -= caec_num.num_.FontHeightSize;
	caec_num.Centerend = place;
	caec_num.Movepass.set(0,1,2,1);
	
	//表示時間
	caec_num.life = 30;
	caec_num.hidingTime = 0;	
	caec->addAnimechip(pcac = pcAnimationChipManager_chip(new caEchip_Num(caec_num)));
	
	sg_pDungeonSystem->AnimationManager().AddAnime_parallel(boost::static_pointer_cast<cAnimation>(caec));


	return true;
}


int caDamage::process(IDirect3DDevice9 *pDev)
{

	count += speed;
	



	if( !End &&  count < ANIME_DAMAGE_STEP)
	{
		pDamager->visibleemotion.insert(EMOTION_ASERI);

		pDamager->opaque = Baseopaque*(((int)(count/speed)/2) % 2);
	
		
	}
	else
	{
		pDamager->visibleemotion.erase(EMOTION_ASERI);
		pDamager->opaque = Baseopaque;

		End = true;
	}
	
	return true;
}

bool caDamage::isOnScreen()
{

	if(	sg_pDungeonSystem->Map().isOnDisplay(
		pDamager->placeX,pDamager->placeY) )
	{
		return true;
	}
	return false;
}

//-------------------------------------------------------------
//caRecover
//-------------------------------------------------------------
caRecover::caRecover()
{
	End = false;
	count = 0;
	speed = 1;
}

caRecover::~caRecover()
{
}

int caRecover::Init(pcAnimation me)
{
	cAnimation::Init(me);


	pcaECustom caec = pcaECustom(new caECustom);
	caec->Init(caec);
	caec->place.set(pRecoverer->placeX, pRecoverer->placeY, 0, 0);

	c4DVector center;
	pcAnimationChipManager_chip pcac;

	int i;

	caEchip_Num caec_num;
	

	//テクスチャセット
	caec_num.num_.Init(sg_pDungeonSystem->pDevice_D3D);
	caec_num.num_.Fontindex = 2;
	//描画基本位置
	caec_num.AbsolutePlaceFlag = 0;
	caec_num.place.set(pRecoverer->placeX,pRecoverer->placeY,0,0);

	//基本サイズ
	caec_num.num_.FontHeightSize = 32;
	caec_num.num_.FontWidthSize = 16;
	caec_num.num_.arrangeX = cNumField::ARRANGEX_CENTER;
	caec_num.num_.arrangeY = cNumField::ARRANGEY_CENTER;
	//加算
	//caec_num.num_.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;
	caec_num.num_.Value_ = Recover;
	//色
	caec_num.ColorBase.inputD3Dcolor(0xFF00FF00);
	caec_num.Alpha.set(0,2,2,0);
	//場所
	c4DVector place(0,-pRecoverer->GetDrawHeadtall_dotY() - caec_num.num_.FontHeightSize + 4,0,0);
	caec_num.Centerbegin = place;
	place.y -= caec_num.num_.FontHeightSize;
	caec_num.Centerend = place;
	caec_num.Movepass.set(0,1,2,1);
	
	//表示時間
	caec_num.life = 30;
	caec_num.hidingTime = 0;	
	caec->addAnimechip(pcac = pcAnimationChipManager_chip(new caEchip_Num(caec_num)));
	
	sg_pDungeonSystem->AnimationManager().AddAnime_parallel(boost::static_pointer_cast<cAnimation>(caec));


	return true;
}


int caRecover::process(IDirect3DDevice9 *pDev)
{

	count += speed;
	



	if( !End &&  count < ANIME_RECOVER_STEP)
	{

	}
	else
	{

		End = true;
	}
	
	return true;
}

bool caRecover::isOnScreen()
{

	if(	sg_pDungeonSystem->Map().isOnDisplay(
		pRecoverer->placeX,pRecoverer->placeY) )
	{
		return true;
	}
	return false;
}
