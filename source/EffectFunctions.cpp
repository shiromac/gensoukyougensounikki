
#include "stdafx.h"

#include "EffectFunctions.h"
#include "cLandform.h"

#include "cAnimation.h"
#include "caquake.h"

#include "cDungeonSystem.h"
#include "caEchipS.h"
#include "caECustom.h"
#include "AnimeChips.h"
#include "cColor.h"
#include "scriptprocesser/AnimationScript.h"
#include <d3dx9math.h>
using namespace EffectFunctions;

void EffectFunctions::上吹き飛ばし風エフェクト(const double placeX,const double placeY, int shortflag)
{
	int life = 20;
	int linenum = 3;
	double height = 412/64.0;
	int length = 30;
	int i,numth;

	if(shortflag)
	{
		life = 20;
		linenum = 6;
	}
	else
	{
		life = 30;
		linenum = 24;
	}
	pAnimeChip_Hab hab = AnimeChip_Hab::new_shared_ptr();
	pAnimeChip_Sound sound = AnimeChip_Sound::new_shared_ptr(_T("spell.wav"), PositionPerGrid(placeX,placeY,0), 1.0);

	hab->addAnimeChip(boost::static_pointer_cast<AnimeChip>(sound));


	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(hab));

	for(numth=0;numth<linenum;numth++)
	{
		pAnimeChip_Belt chip = AnimeChip_Belt::new_shared_ptr(life);
		hab->addAnimeChip(boost::static_pointer_cast<AnimeChip>(chip));

		chip->setTexture(_T("effect\\basic\\ball_antismoke.png"));
		chip->color.addCascade(cColor(0,255,255,255),cColor(255*2,255,255,255),cColor(0,255,255,255),life);
	
		if(shortflag)
		{
			//表示時間
			chip->setDelay(10*rand()/RAND_MAX);
		}
		else
		{
			//表示時間
			chip->setDelay(40*rand()/RAND_MAX);
		}

		double s = 1.0*rand()/RAND_MAX+1;		
		
		chip->position.addCascade(PositionPerGrid(placeX,placeY,height),life);
		//ライン定義
		chip->leftEdge.resize(length-1);
		chip->rightEdge.resize(length-1);
		chip->setLeftEdgeStartingPositionalPoint(PositionPerGrid(0,0,0));
		chip->setRightEdgeStartingPositionalPoint(PositionPerGrid(0,0,0.5));
		
		for(i=1;i<length;i++)
		{
			c4DVector v;
			v.set(10/64.0,0,0,0);
			v*=(1.5)*i*i/length*i/length +4;
			v.turn(-30*i*s +numth*360/8);

			v.z -= height*(i)/length;
			v.y *= 0.75;

			chip->leftEdge[i-1].addCascade(PositionPerGrid(v.x,v.y,v.z),life);
			chip->rightEdge[i-1].addCascade(PositionPerGrid(v.x,v.y,v.z),life);
		}

		//ドラウイングレンジ
		chip->drawRange.addCascade(BeltRange(length-4,length-1),BeltRange(0,3),life);
		chip->textureRange.addCascade(TextureRangeRect(0,0,1,1),life);
	
		if(linenum/2 < numth)
		{
			chip->drawMode.addCascade(cDrawableObject::DRAW_MODE_ADDITION,life);
		}
		else
		{
			chip->drawMode.addCascade(cDrawableObject::DRAW_MODE_NORMAL,life);
		}
	}

	if(shortflag)
	{
		life = 20;
		linenum = 3;
	}
	else
	{
		life = 30;
		linenum = 8;
	}

	
	for(numth=0;numth<linenum;numth++)
	{
		pAnimeChip_Perticle chip = AnimeChip_Perticle::new_shared_ptr(life);
		hab->addAnimeChip(boost::static_pointer_cast<AnimeChip>(chip));
		
		if(shortflag)
		{
			//表示時間
			chip->setDelay(10*rand()/RAND_MAX);
		}
		else
		{
			//表示時間
			chip->setDelay(40*rand()/RAND_MAX);
		}

		double upheight = rand()*1.5/RAND_MAX;
		chip->position.addCascade(PositionPerGrid(placeX,placeY-upheight,0),
			PositionPerGrid(placeX,placeY-upheight-1,0),
			life);
		chip->drawMode.addCascade(cDrawableObject::DRAW_MODE_ADDITION,life);
		chip->setTexture(_T("effect\\basic\\ring_nagi2.png"));
		chip->scaleX.addCascade(1,	0.2,	life);
		chip->scaleY.addCascade(0.75,	0.2*0.75,life);
		chip->rotationBefore.addCascade(0,360*4,life);
		chip->setPerticleSize(D3DXVECTOR2(512,512));
		chip->color.addCascade(cColor(0,0x77,0x88,0x88),cColor(512,0x77,0x88,0x88),cColor(0,0x77,0x88,0x88),life);
		sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(chip));
	}
	
	return;

/*
	//---------------------------------------
	pcaECustom caec;
	c4DVector center;
	pcAnimationChipManager_chip pcac;

	int i;
	caEchip_Belt caec_b_model;

	caEchip_Basic caec_model;


	//------------------------------------------------
	//ぐるぐる
	//------------------------------------------------
	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->place.set(placeX,placeY,0,0);
	
	//レイヤー
	//caec->LayerLevel = LAYERLEVEL_EXTRAEFFECT1;
	


	//テクスチャセット
	caec_b_model.DOb.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
		sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_antismoke.png")),SIZE_BASICTEX,SIZE_BASICTEX);

	//テクスチャ座標セット
	caec_b_model.DOb.m_TexRange.setLTRB(0,0,1,1);


	//描画基本位置
	center.set(placeX,placeY,0,0);
	//center = sg_pDungeonSystem->Map().realvisibleplace(center);



	//基本サイズ
	//caec_model.DO.Width = 128;
	//caec_model.DO.Height = 128;




	
	//加算
	caec_b_model.DOb.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;

	//色
	caec_b_model.ColorBase.inputD3Dcolor(0xFF778888);
	caec_b_model.Alpha.set(0,2,2,0);


	
	//表示時間
	caec_b_model.life = 60;
	caec_b_model.hidingTime = 0;


	int height = 412;
	//描画位置
	caec_b_model.CenterLeft = center;
	caec_b_model.CenterRight = center;
	caec_b_model.CenterLeft.z += height+10;
	caec_b_model.CenterRight.z += 40 + height+10;
	

	int length = 30;
	int numth;
	int linenum = 0;

	if(shortflag)
	{
		length = 20;
		linenum = 3;
	}
	else
	{
		length = 30;
		linenum = 12;
	}

	for(numth=0;numth<linenum;numth++)
	{


		if(shortflag)
		{
			//表示時間
			caec_b_model.life = 20;
			caec_b_model.hidingTime = 10*rand()/RAND_MAX;
		}
		else
		{
			//表示時間
			caec_b_model.life = 30;
			caec_b_model.hidingTime = 40*rand()/RAND_MAX;
		}

		double s = 1.0*rand()/RAND_MAX+1;		
		
		//ライン定義
		caec_b_model.DOb.rightline().deleteCurveTrack();
		caec_b_model.DOb.leftline().deleteCurveTrack();
		for(i=1;i<length;i++)
		{
			c4DVector v;
			v.set(10,0,0,0);
			v*=(1.5)*i*i/length*i/length +4;
			v.turn(-30*i*s +numth*360/8);

			v.z -= height*(i)/length;
			v.y *= 0.75;

			caec_b_model.DOb.rightline().addPoint(v);
			caec_b_model.DOb.leftline().addPoint(v);
		}
		caec_b_model.DOb.rightline().stabilizeTrack();
		caec_b_model.DOb.leftline().stabilizeTrack();


		//ドラウイングレンジ
		caec_b_model.Startp.set(length-3,0,1,1);
		caec_b_model.Endp.set  (length  ,3,1,1);



		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Belt(caec_b_model));
		caec->addAnimechip(pcac);
	}

	//加算
	caec_b_model.DOb.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;
	//レイヤー
	//caec->LayerLevel = LAYERLEVEL_EXTRAEFFECT2;
	

	if(shortflag)
	{
		linenum = 3;
	}
	else
	{
		linenum = 12;
	}

	for(numth=0;numth<linenum;numth++)
	{



		if(shortflag)
		{
			//表示時間
			caec_b_model.life = 20;
			caec_b_model.hidingTime = 10*rand()/RAND_MAX;
		}
		else
		{
			//表示時間
			caec_b_model.life = 30;
			caec_b_model.hidingTime = 40*rand()/RAND_MAX;
		}

		double s = 1.0*rand()/RAND_MAX+1;		
		
		//ライン定義
		caec_b_model.DOb.rightline().deleteCurveTrack();
		caec_b_model.DOb.leftline().deleteCurveTrack();
		for(i=1;i<length;i++)
		{
			c4DVector v;
			v.set(10,0,0,0);
			v*=(1.5)*i*i/length*i/length +4;
			v.turn(-30*i*s +numth*360/8);

			v.z -= height*(i)/length;
			v.y *= 0.75;

			caec_b_model.DOb.rightline().addPoint(v);
			caec_b_model.DOb.leftline().addPoint(v);
		}
		caec_b_model.DOb.rightline().stabilizeTrack();
		caec_b_model.DOb.leftline().stabilizeTrack();


		//ドラウイングレンジ
		caec_b_model.Startp.set(length-3,0,1,1);
		caec_b_model.Endp.set  (length  ,3,1,1);



		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Belt(caec_b_model));
		caec->addAnimechip(pcac);
	}

	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));
	
	//------------------------------------------------
	//ぼわっ
	//------------------------------------------------
	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->place.set(placeX,placeY,0,0);
	
	//レイヤー
	caec->LayerLevel = LAYERLEVEL_EXTRAEFFECT3;
	


	//テクスチャセット
	caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
		sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ring_nagi2.png")),SIZE_BASICTEX,SIZE_BASICTEX);

	//テクスチャ座標セット
	caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


	//描画基本位置
	center.set(placeX, placeY, 0, 0);
	//center = sg_pDungeonSystem->Map().realvisibleplace(center);



	//基本サイズ
	caec_model.DO.Width = 512;
	caec_model.DO.Height = 512;


	//シェイプオプション
	caec_model.ScaleX.set(1,1,1,0.2);
	caec_model.ScaleY.set(0.75,0.75,1,0.2);
	caec_model.Rotation.set(rand()*360/RAND_MAX, rand()*360/RAND_MAX + 360*4,1,1.5);
	//caec_model.Rotation2

	
	//加算
	caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

	//色
	caec_model.ColorBase.inputD3Dcolor(0xFF778888);
	caec_model.Alpha.set(0,2,2,0);


	if(shortflag)
	{
		linenum = 3;
	}
	else
	{
		linenum = 8;
	}

	for(i=0;i<linenum;i++)
	{

		//描画位置
		c4DVector v(4,0,0,0);
		v.turn(360*rand()/RAND_MAX);

		caec_model.Centerbegin = center+v;
		caec_model.Centerbegin.y -= rand()*92/RAND_MAX;

		caec_model.Centerend = caec_model.Centerbegin;
		caec_model.Centerend.y -= 64;



		if(shortflag)
		{
			//表示時間
			caec_model.life = 20;
			caec_model.hidingTime = 10*rand()/RAND_MAX;
		}
		else
		{
			//表示時間
			caec_model.life = 30;
			caec_model.hidingTime = 40*rand()/RAND_MAX;
		}


		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
		caec->addAnimechip(pcac);
	}
	
	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));
*/
}

void EffectFunctions::煙エフェクト1(const double placeX,const double placeY)
{

	pcaECustom caec;
	c4DVector center;
	pcAnimationChipManager_chip pcac;

	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->place.set(placeX,placeY,0,0);
	
	//レイヤー２
	caec->LayerLevel = LAYERLEVEL_EXTRAEFFECT2;
	
	caEchip_Basic caec_model;

	caec_model.AbsolutePlaceFlag = false;
	caec_model.place.set(placeX,placeY,0,0);

	//テクスチャセット
	caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
		sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_smoke.png")),SIZE_BASICTEX,SIZE_BASICTEX);

	//テクスチャ座標セット
	caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


	//描画基本位置
	center.set(placeX,placeY,0,0);
	//center = sg_pDungeonSystem->Map().realvisibleplace(center);



	//基本サイズ
	caec_model.DO.Width = 128;
	caec_model.DO.Height = 128;


	//描画位置
	/*
	caec_model.Centerbegin = center;
	caec_model.Centerend = center;
	*/
	caec_model.Centerend.y -= 32;
	
	
	//シェイプオプション
	caec_model.ScaleX.set(0,2,2,0);
	caec_model.ScaleY.set(0,2,2,0);
	//caec_model.Rotation.set(0,0,0,0);
	double d = rand()*360/RAND_MAX;
	caec_model.Rotation2.set(d,d+(rand()*60-30)/RAND_MAX,1,1);



	
	//加算
	caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;

	//色
	caec_model.ColorBase.inputD3Dcolor(0xFFFFFFFF);
	caec_model.Alpha.set(0,2,1,0);


	
	//表示時間
	caec_model.life = 20;
	caec_model.hidingTime = 0;	

	//チップに追加
	pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
	caec->addAnimechip(pcac);


	caec_model.hidingTime = 4;
	caec_model.Rotation2.set(rand()*360/RAND_MAX,rand()*360/RAND_MAX,1,0.5);
	caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

	//チップに追加
	pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
	caec->addAnimechip(pcac);


	caec_model.hidingTime = 8;
	caec_model.Rotation2.set(rand()*360/RAND_MAX,rand()*360/RAND_MAX,1,0.5);
	caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;

	//チップに追加
	pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
	caec->addAnimechip(pcac);

	//sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));
	sg_pDungeonSystem->AnimationManager().AddAnime_parallel(boost::static_pointer_cast<cAnimation>(caec));
	//sg_pDungeonSystem->AnimationManager().Anime_Wait(placeX,placeY,10);

}
void EffectFunctions::爆発エフェクト_中(const double placeX,const double placeY, double color_H)
{
	unsigned int color,darkcolor;
	cColor c;
	c.HSV(color_H,1.0,1.0);
	color = c.D3Dcolor();
	c.HSV(color_H,1.0,0.4);
	darkcolor = c.D3Dcolor();

	cCoordinate coo(placeX,placeY);
	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("bom.wav"),coo);

	pcaECustom caec;
	c4DVector center;
	pcAnimationChipManager_chip pcac;

	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->place.set(placeX,placeY,0,0);
	
	//レイヤー２
	caec->LayerLevel = LAYERLEVEL_NORMAL;
	
	caEchip_Basic caec_model;

	caec_model.AbsolutePlaceFlag = false; caec_model.place.set(placeX,placeY,0,0);
	{//影
		//テクスチャセット
		caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_simple.png")),SIZE_BASICTEX,SIZE_BASICTEX);

		//テクスチャ座標セット
		caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


		//描画基本位置
		center.set(placeX,placeY,0,0);
		//center = sg_pDungeonSystem->Map().realvisibleplace(center);



		//基本サイズ
		caec_model.DO.Width = (64*3+8)*3;
		caec_model.DO.Height = (64*3+8)*3;


		//描画位置

		caec_model.Centerbegin.set(0,0,0,0);
		caec_model.Centerend.set(0,0,0,0);
		
		caec_model.Centerend.z -= 16;
		
		
		//シェイプオプション
		caec_model.ScaleX.set(1,1,1,1);
		caec_model.ScaleY.set(1,1,1,1);
		//caec_model.Rotation.set(0,0,0,0);
		double d = rand()*360/RAND_MAX;
		caec_model.Rotation2.set(d,d+(rand()*60-30)/RAND_MAX,1,1);



		
		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;

		//色
		caec_model.ColorBase.inputD3Dcolor(darkcolor);
		caec_model.Alpha.set(0,2,1,0);


		
		//表示時間
		caec_model.life = 20;
		caec_model.hidingTime = 0;	

		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
		caec->addAnimechip(pcac);
	}
	{//光１
		//テクスチャセット
		caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_radio.png")),SIZE_BASICTEX,SIZE_BASICTEX);

		//テクスチャ座標セット
		caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


		//描画基本位置
		center.set(placeX,placeY,0,0);
		//center = sg_pDungeonSystem->Map().realvisibleplace(center);



		//基本サイズ
		caec_model.DO.Width = (64*3+8)*3;
		caec_model.DO.Height = (64*3+8)*3;


		//描画位置
		caec_model.Centerbegin.set(0,0,0,0);
		caec_model.Centerend.set(0,0,0,0);

		caec_model.Centerend.y -= 16;
		
		
		//シェイプオプション
		caec_model.ScaleX.set(0,1,0,1);
		caec_model.ScaleY.set(0,1,0,1);
		//caec_model.Rotation.set(0,0,0,0);
		double d = rand()*360/RAND_MAX;
		caec_model.Rotation2.set(d,d+(rand()*60-30)/RAND_MAX,1,1);



		
		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

		//色
		caec_model.ColorBase.inputD3Dcolor(color);
		caec_model.Alpha.set(0,2,2,0);


		
		//表示時間
		caec_model.life = 5;
		caec_model.hidingTime = 0;	

		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
		caec->addAnimechip(pcac);
	}
	{//衝撃波
		//テクスチャセット
		caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ring_smoke.png")),SIZE_BASICTEX,SIZE_BASICTEX);

		//テクスチャ座標セット
		caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


		//描画基本位置
		center.set(placeX,placeY,0,0);
		//center = sg_pDungeonSystem->Map().realvisibleplace(center);



		//基本サイズ
		caec_model.DO.Width = (64*3+8)*2;
		caec_model.DO.Height = (64*3)*2;


		//描画位置
		caec_model.Centerbegin.set(0,0,0,0);
		caec_model.Centerend.set(0,0,0,0);
		caec_model.Centerend.y -= 16;
		
		
		//シェイプオプション
		caec_model.ScaleX.set(0,1,2,1);
		caec_model.ScaleY.set(0,1,2,1);
		//caec_model.Rotation.set(0,0,0,0);
		double d = rand()*360/RAND_MAX;
		caec_model.Rotation2.set(d,d+(rand()*60-30)/RAND_MAX,1,1);



		
		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;

		//色
		caec_model.ColorBase.inputD3Dcolor(0xFFFFFFFF);
		caec_model.Alpha.set(0,2,1,0);


		
		//表示時間
		caec_model.life = 15;
		caec_model.hidingTime = 5;	

		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
		caec->addAnimechip(pcac);
	}

	{//光2

		//テクスチャセット
		caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_simple.png")),SIZE_BASICTEX,SIZE_BASICTEX);

		//テクスチャ座標セット
		caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


		//描画基本位置
		center.set(placeX,placeY,0,0);
		//center = sg_pDungeonSystem->Map().realvisibleplace(center);



		//基本サイズ
		caec_model.DO.Width = (64*3+8)*1.4;
		caec_model.DO.Height = (64*3+8)*1.4;


		//描画位置
		caec_model.Centerbegin.set(0,0,0,0);
		caec_model.Centerend.set(0,0,0,0);
		caec_model.Centerend.y -= 16;
		
		
		//シェイプオプション
		caec_model.ScaleX.set(0.5,1,0.5,1);
		caec_model.ScaleY.set(0.5,1,0.5,1);
		//caec_model.Rotation.set(0,0,0,0);
		double d = rand()*360/RAND_MAX;
		caec_model.Rotation2.set(d,d+(rand()*60-30)/RAND_MAX,1,1);



		
		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

		//色
		caec_model.ColorBase.inputD3Dcolor(color);
		caec_model.Alpha.set(1,2,2,0);


		
		//表示時間
		caec_model.life = 20;
		caec_model.hidingTime = 0;	

		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
		caec->addAnimechip(pcac);

		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
		caec->addAnimechip(pcac);
	}
	{//光2

		//テクスチャセット
		caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ring_nagi.png")),SIZE_BASICTEX,SIZE_BASICTEX);

		//テクスチャ座標セット
		caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


		//描画基本位置
		center.set(placeX,placeY,0,0);
		//center = sg_pDungeonSystem->Map().realvisibleplace(center);



		//基本サイズ
		caec_model.DO.Width = (64*3+8)*1.1;
		caec_model.DO.Height = (64*3+8)*1.1;


		//描画位置
		caec_model.Centerbegin.set(0,0,0,0);
		caec_model.Centerend.set(0,0,0,0);
		caec_model.Centerend.y -= 16;
		
		
		//シェイプオプション
		caec_model.ScaleX.set(1,1,1,1);
		caec_model.ScaleY.set(1,1,1,1);
		//caec_model.Rotation.set(0,0,0,0);
		double d = rand()*360/RAND_MAX;
		caec_model.Rotation2.set(d,d+2*360,1,1);



		
		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

		//色
		caec_model.ColorBase.inputD3Dcolor(color);
		caec_model.Alpha.set(1,2,2,0);


		
		//表示時間
		caec_model.life = 20;
		caec_model.hidingTime = 0;	

		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
		caec->addAnimechip(pcac);

	}
	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));

}

void EffectFunctions::夢想封印(const double fromplaceX,const double fromplaceY, const double placeX,const double placeY, double color_H)
{
	unsigned int color,darkcolor;
	cColor c;
	c.HSV(color_H,1.0,1.0);
	color = c.D3Dcolor();
	c.HSV(color_H,1.0,0.4);
	darkcolor = c.D3Dcolor();

	cCoordinate coo(placeX,placeY);
	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("bom.wav"),coo);

	pcaECustom caec;
	c4DVector center,fromcenter;
	pcAnimationChipManager_chip pcac;

	caec = pcaECustom(new caECustom);

	//描画位置通知
	cCoordinate fromcoodif(fromplaceX - sg_pDungeonSystem->pPlayerChara()->placeX, fromplaceY - sg_pDungeonSystem->pPlayerChara()->placeY);
	cCoordinate tocoodif(placeX - sg_pDungeonSystem->pPlayerChara()->placeX, placeY - sg_pDungeonSystem->pPlayerChara()->placeY);
	if(fromcoodif.dif() < tocoodif.dif())
	{
		caec->place.set(fromplaceX, fromplaceY,0,0);
	}
	else
	{
		caec->place.set(placeX, placeY,0,0);
	}

	//レイヤー２
	caec->LayerLevel = LAYERLEVEL_NORMAL;
	
	caEchip_Basic caec_model;

	caec_model.AbsolutePlaceFlag = true;
	{//影
		//テクスチャセット
		caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_simple.png")),SIZE_BASICTEX,SIZE_BASICTEX);

		//テクスチャ座標セット
		caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


		//描画基本位置
		center.set(placeX,placeY,0,0);
		//center = sg_pDungeonSystem->Map().realvisibleplace(center);



		//基本サイズ
		caec_model.DO.Width = (64)*3;
		caec_model.DO.Height = (64)*3;

		caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeX,placeY,0,0);

		//描画位置
		//caec_model.Centerbegin = center;
		//caec_model.Centerend = center;

		caec_model.Centerend.y -= 16;
		
		
		//シェイプオプション
		caec_model.ScaleX.set(1,1,1,1);
		caec_model.ScaleY.set(1,1,1,1);
		//caec_model.Rotation.set(0,0,0,0);
		double d = rand()*360/RAND_MAX;
		caec_model.Rotation2.set(d,d+(rand()*60-30)/RAND_MAX,1,1);



		
		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;

		//色
		caec_model.ColorBase.inputD3Dcolor(darkcolor);
		caec_model.Alpha.set(0,2,1,0);


		
		//表示時間
		caec_model.life = 20;
		caec_model.hidingTime = 10;	

		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
		caec->addAnimechip(pcac);
	}
	{//光１
		//テクスチャセット
		caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_smoke.png")),SIZE_BASICTEX,SIZE_BASICTEX);

		//テクスチャ座標セット
		caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


		//描画基本位置
		center.set(placeX,placeY,0,0);
		//center = sg_pDungeonSystem->Map().realvisibleplace(center);
		fromcenter.set(fromplaceX,fromplaceY,0,0);
		fromcenter = sg_pDungeonSystem->Map().realvisibleplace(fromcenter);


		//基本サイズ
		caec_model.DO.Width = (64);
		caec_model.DO.Height = (64);


		caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeX,placeY,0,0);

		//描画位置
		caec_model.Centerbegin = fromcenter;
		//caec_model.Centerend = center;

		caec_model.Centerend.y -= 16;
		
		
		//シェイプオプション
		caec_model.ScaleX.set(1,1,1,1);
		caec_model.ScaleY.set(1,1,1,1);
		//caec_model.Rotation.set(0,0,0,0);
		double d = rand()*360/RAND_MAX;
		caec_model.Rotation2.set(d,d+360*2,1,1);



		
		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

		//色
		caec_model.ColorBase.inputD3Dcolor(color);
		caec_model.Alpha.set(1,1,1,1);


		
		//表示時間
		caec_model.life = 20;
		caec_model.hidingTime = 0;	

		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
		caec->addAnimechip(pcac);

		//テクスチャセット
		caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ring_wind.png")),SIZE_BASICTEX,SIZE_BASICTEX);

	
		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
		caec->addAnimechip(pcac);
	}
	{//衝撃波
		//テクスチャセット
		caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ring_nagi2.png")),SIZE_BASICTEX,SIZE_BASICTEX);

		//テクスチャ座標セット
		caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


		//描画基本位置
		center.set(placeX,placeY,0,0);
		//center = sg_pDungeonSystem->Map().realvisibleplace(center);



		//基本サイズ
		caec_model.DO.Width = (64*3+8)*2;
		caec_model.DO.Height = (64*3)*2;


		//描画位置
		caec_model.Centerbegin = center;
		caec_model.Centerend = center;

		caec_model.Centerend.y -= 16;
		
		
		//シェイプオプション
		caec_model.ScaleX.set(0,1,2,1);
		caec_model.ScaleY.set(0,1,2,1);
		//caec_model.Rotation.set(0,0,0,0);
		double d = rand()*360/RAND_MAX;
		caec_model.Rotation2.set(d,d+720,1,1);



		
		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

		//色
		caec_model.ColorBase.inputD3Dcolor(color);
		caec_model.Alpha.set(0,2,1,0);


		
		//表示時間
		caec_model.life = 10;
		caec_model.hidingTime = 20;	

		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
		caec->addAnimechip(pcac);

		 d = rand()*360/RAND_MAX;
		caec_model.Rotation2.set(d,d+720,1,1);

		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
		caec->addAnimechip(pcac);

		 d = rand()*360/RAND_MAX;
		caec_model.Rotation2.set(d,d+720,1,1);

		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
		caec->addAnimechip(pcac);
	}

	{//光2

		//テクスチャセット
		caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_radio.png")),SIZE_BASICTEX,SIZE_BASICTEX);

		//テクスチャ座標セット
		caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


		//描画基本位置
		center.set(placeX,placeY,0,0);
		//center = sg_pDungeonSystem->Map().realvisibleplace(center);



		//基本サイズ
		caec_model.DO.Width = (64*3+8)*1.4;
		caec_model.DO.Height = (64*3+8)*1.4;


		//描画位置
		caec_model.Centerbegin = center;
		caec_model.Centerend = center;

		caec_model.Centerend.y -= 16;
		
		
		//シェイプオプション
		caec_model.ScaleX.set(0.5,1,0.5,1);
		caec_model.ScaleY.set(0.5,1,0.5,1);
		//caec_model.Rotation.set(0,0,0,0);
		double d = rand()*360/RAND_MAX;
		caec_model.Rotation2.set(d,d+(rand()*60-30)/RAND_MAX,1,1);



		
		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

		//色
		caec_model.ColorBase.inputD3Dcolor(color);
		caec_model.Alpha.set(1,2,2,0);


		
		//表示時間
		caec_model.life = 10;
		caec_model.hidingTime = 20;	

		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
		caec->addAnimechip(pcac);

		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
		caec->addAnimechip(pcac);
	}
	
	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));

}

void EffectFunctions::地震(int time_frame, int amplitude)
{

	pcaQuake quake;
	quake = pcaQuake(new caQuake);

	//レイヤー２
	quake->LayerLevel = LAYERLEVEL_NORMAL;

	quake->time_frame = time_frame;
	quake->amplitude = amplitude;
	quake->Init();

	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(quake));

}

void EffectFunctions::三歩必殺(const double placeX,const double placeY)
{
	{
		cCoordinate coo(placeX,placeY);
		sg_pDungeonSystem->AnimationManager().
			Anime_PlaySE(_T("bom.wav"),coo);
		unsigned int color,darkcolor;
		cColor c;
		c.HSV(250,1.0,1.0);
		color = c.D3Dcolor();
		c.HSV(250,1.0,0.4);
		darkcolor = c.D3Dcolor();


		pcaECustom caec;
		c4DVector center;
		pcAnimationChipManager_chip pcac;

		caec = pcaECustom(new caECustom);


		//描画位置通知
		caec->place.set(placeX, placeY,0,0);
		

		//レイヤー２
		caec->LayerLevel = LAYERLEVEL_NORMAL;
		
		caEchip_Basic caec_model;

		caec_model.AbsolutePlaceFlag = true;
		{//影
			//テクスチャセット
			caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
				sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_simple.png")),SIZE_BASICTEX,SIZE_BASICTEX);

			//テクスチャ座標セット
			caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


			//描画基本位置
			center.set(placeX,placeY,0,0);
			//center = sg_pDungeonSystem->Map().realvisibleplace(center);


			caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeX,placeY,0,0);


			//基本サイズ
			caec_model.DO.Width = (64)*2;
			caec_model.DO.Height = (64)*2;


			//描画位置
			//caec_model.Centerbegin = center;
			//caec_model.Centerend = center;

			caec_model.Centerend.y -= 16;
			
			
			//シェイプオプション
			caec_model.ScaleX.set(1,1,1,1);
			caec_model.ScaleY.set(1,1,1,1);
			//caec_model.Rotation.set(0,0,0,0);
			double d = rand()*360/RAND_MAX;
			caec_model.Rotation2.set(d,d+(rand()*60-30)/RAND_MAX,1,1);



			
			//加算
			caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;

			//色
			caec_model.ColorBase.inputD3Dcolor(darkcolor);
			caec_model.Alpha.set(0,2,1,0);


			
			//表示時間
			caec_model.life = 15;
			caec_model.hidingTime = 0;	

			//チップに追加
			pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
			caec->addAnimechip(pcac);
		}
		{//光１
			//テクスチャセット
			caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
				sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_smoke.png")),SIZE_BASICTEX,SIZE_BASICTEX);

			//テクスチャ座標セット
			caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


			//描画基本位置
			center.set(placeX,placeY,0,0);
			//center = sg_pDungeonSystem->Map().realvisibleplace(center);


			//基本サイズ
			caec_model.DO.Width = (64);
			caec_model.DO.Height = (64);

			caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeX,placeY,0,0);

			//描画位置
			//caec_model.Centerbegin = center;
			//caec_model.Centerend = center;

			caec_model.Centerend.y -= 16;
			
			
			//シェイプオプション
			caec_model.ScaleX.set(1,1,1,1);
			caec_model.ScaleY.set(1,1,1,1);
			//caec_model.Rotation.set(0,0,0,0);
			double d = rand()*360/RAND_MAX;
			caec_model.Rotation2.set(d,d+360*2,1,1);



			
			//加算
			caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

			//色
			caec_model.ColorBase.inputD3Dcolor(color);
			caec_model.Alpha.set(1,1,1,1);


			
			//表示時間
			caec_model.life = 15;
			caec_model.hidingTime = 0;	

			//チップに追加
			pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
			caec->addAnimechip(pcac);


		}
		{//衝撃波
			//テクスチャセット
			caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
				sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ring_out.png")),SIZE_BASICTEX,SIZE_BASICTEX);

			//テクスチャ座標セット
			caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


			//描画基本位置
			center.set(placeX,placeY,0,0);
			//center = sg_pDungeonSystem->Map().realvisibleplace(center);

			caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeX,placeY,0,0);


			//基本サイズ
			caec_model.DO.Width = (64+8)*2;
			caec_model.DO.Height = (64)*2;


			//描画位置
			//caec_model.Centerbegin = center;
			//caec_model.Centerend = center;

			caec_model.Centerend.y -= 16;
			
			
			//シェイプオプション
			caec_model.ScaleX.set(0,1,1,1);
			caec_model.ScaleY.set(0,1,1,1);
			//caec_model.Rotation.set(0,0,0,0);
			double d = rand()*360/RAND_MAX;
			caec_model.Rotation2.set(d,d+720,1,1);



			
			//加算
			caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

			//色
			caec_model.ColorBase.inputD3Dcolor(color);
			caec_model.Alpha.set(0,2,1,0);


			
			//表示時間
			caec_model.life = 15;
			caec_model.hidingTime = 0;	

			//チップに追加
			pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
			caec->addAnimechip(pcac);
		}

		{//光2

			//テクスチャセット
			caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
				sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_radio.png")),SIZE_BASICTEX,SIZE_BASICTEX);

			//テクスチャ座標セット
			caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


			//描画基本位置
			center.set(placeX,placeY,0,0);
			//center = sg_pDungeonSystem->Map().realvisibleplace(center);

			caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeX,placeY,0,0);


			//基本サイズ
			caec_model.DO.Width = (64+8);
			caec_model.DO.Height = (64+8);


			//描画位置
			//caec_model.Centerbegin = center;
			//caec_model.Centerend = center;

			caec_model.Centerend.y -= 16;
			
			
			//シェイプオプション
			caec_model.ScaleX.set(0.5,1,0.5,1);
			caec_model.ScaleY.set(0.5,1,0.5,1);
			//caec_model.Rotation.set(0,0,0,0);
			double d = rand()*360/RAND_MAX;
			caec_model.Rotation2.set(d,d+(rand()*60-30)/RAND_MAX,1,1);



			
			//加算
			caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

			//色
			caec_model.ColorBase.inputD3Dcolor(color);
			caec_model.Alpha.set(1,2,2,0);


			
			//表示時間
			caec_model.life = 15;
			caec_model.hidingTime = 0;	

			//チップに追加
			pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
			caec->addAnimechip(pcac);

		}
		
		sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));
	}

	{//にはつめ
		cCoordinate coo(placeX,placeY);
		sg_pDungeonSystem->AnimationManager().
			Anime_PlaySE(_T("bom.wav"),coo);
		unsigned int color,darkcolor;
		cColor c;
		c.HSV(220,1.0,1.0);
		color = c.D3Dcolor();
		c.HSV(220,1.0,0.4);
		darkcolor = c.D3Dcolor();


		pcaECustom caec;
		c4DVector center;
		pcAnimationChipManager_chip pcac;

		int placeXnear, placeYnear;

		caec = pcaECustom(new caECustom);


		//描画位置通知
		caec->place.set(placeX, placeY,0,0);
		

		//レイヤー２
		caec->LayerLevel = LAYERLEVEL_NORMAL;
		
		caEchip_Basic caec_model;

		int i;
		for(i=0;i<8;i++)
		{
			cCoordinate coo,Mplace;
			switch(i+1)
			{
			case 1:
				coo.Set(1,0);break;
			case 2:
				coo.Set(-1,0);break;
			case 3:
				coo.Set(0,-1);break;
			case 4:
				coo.Set(0,1);break;
			case 5:
				coo.Set(1,-1);break;
			case 6:
				coo.Set(-1,-1);break;
			case 7:
				coo.Set(1,1);break;
			case 8:
				coo.Set(-1,1);break;
			case 9:
				coo.Set(2,0);break;
			case 10:
				coo.Set(-2,0);break;
			case 11:
				coo.Set(0,-2);break;
			case 12:
				coo.Set(0,2);break;
			case 13:
				coo.Set(2,-1);break;
			case 14:
				coo.Set(-2,-1);break;
			case 15:
				coo.Set(2,1);break;
			case 16:
				coo.Set(-2,1);break;
			case 17:
				coo.Set(1,-2);break;
			case 18:
				coo.Set(-1,-2);break;
			case 19:
				coo.Set(1,2);break;
			case 20:
				coo.Set(-1,2);break;
			case 21:
				coo.Set(2,-2);break;
			case 22:
				coo.Set(-2,-2);break;
			case 23:
				coo.Set(2,2);break;
			case 24:
				coo.Set(-2,2);break;
			default:
				coo.Set(0,0);break;
			}

			placeXnear = coo.x + placeX;
			placeYnear = coo.y + placeY;
			{//影
				//テクスチャセット
				caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
					sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_simple.png")),SIZE_BASICTEX,SIZE_BASICTEX);

				//テクスチャ座標セット
				caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


				//描画基本位置
				center.set(placeXnear,placeYnear,0,0);
				//center = sg_pDungeonSystem->Map().realvisibleplace(center);



				//基本サイズ
				caec_model.DO.Width = (64)*2;
				caec_model.DO.Height = (64)*2;
				caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeXnear,placeYnear,0,0);


				//描画位置
				//caec_model.Centerbegin = center;
				//caec_model.Centerend = center;

				caec_model.Centerend.y = -16;
				
				
				//シェイプオプション
				caec_model.ScaleX.set(1,1,1,1);
				caec_model.ScaleY.set(1,1,1,1);
				//caec_model.Rotation.set(0,0,0,0);
				double d = rand()*360/RAND_MAX;
				caec_model.Rotation2.set(d,d+(rand()*60-30)/RAND_MAX,1,1);



				
				//加算
				caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;

				//色
				caec_model.ColorBase.inputD3Dcolor(darkcolor);
				caec_model.Alpha.set(0,2,1,0);


				
				//表示時間
				caec_model.life = 15;
				caec_model.hidingTime = 0;	

				//チップに追加
				pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
				caec->addAnimechip(pcac);
			}
			{//光１
				//テクスチャセット
				caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
					sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_smoke.png")),SIZE_BASICTEX,SIZE_BASICTEX);

				//テクスチャ座標セット
				caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


				//描画基本位置
				center.set(placeXnear,placeYnear,0,0);
				//center = sg_pDungeonSystem->Map().realvisibleplace(center);


				//基本サイズ
				caec_model.DO.Width = (64);
				caec_model.DO.Height = (64);

				caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeXnear,placeYnear,0,0);


				//描画位置
				//caec_model.Centerbegin = center;
				//caec_model.Centerend = center;

				caec_model.Centerend.y = -16;
				
				
				//シェイプオプション
				caec_model.ScaleX.set(1,1,1,1);
				caec_model.ScaleY.set(1,1,1,1);
				//caec_model.Rotation.set(0,0,0,0);
				double d = rand()*360/RAND_MAX;
				caec_model.Rotation2.set(d,d+360*2,1,1);



				
				//加算
				caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

				//色
				caec_model.ColorBase.inputD3Dcolor(color);
				caec_model.Alpha.set(1,1,1,1);


				
				//表示時間
				caec_model.life = 15;
				caec_model.hidingTime = 0;	

				//チップに追加
				pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
				caec->addAnimechip(pcac);


			}
			{//衝撃波
				//テクスチャセット
				caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
					sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ring_out.png")),SIZE_BASICTEX,SIZE_BASICTEX);

				//テクスチャ座標セット
				caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


				//描画基本位置
				center.set(placeXnear,placeYnear,0,0);
				//center = sg_pDungeonSystem->Map().realvisibleplace(center);


				//基本サイズ
				caec_model.DO.Width = (64+8)*2;
				caec_model.DO.Height = (64)*2;

				caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeXnear,placeYnear,0,0);


				//描画位置
				//caec_model.Centerbegin = center;
				//caec_model.Centerend = center;

				caec_model.Centerend.y = -16;
				
				
				//シェイプオプション
				caec_model.ScaleX.set(0,1,1,1);
				caec_model.ScaleY.set(0,1,1,1);
				//caec_model.Rotation.set(0,0,0,0);
				double d = rand()*360/RAND_MAX;
				caec_model.Rotation2.set(d,d+720,1,1);



				
				//加算
				caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

				//色
				caec_model.ColorBase.inputD3Dcolor(color);
				caec_model.Alpha.set(0,2,1,0);


				
				//表示時間
				caec_model.life = 15;
				caec_model.hidingTime = 0;	

				//チップに追加
				pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
				caec->addAnimechip(pcac);
			}

			{//光2

				//テクスチャセット
				caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
					sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_radio.png")),SIZE_BASICTEX,SIZE_BASICTEX);

				//テクスチャ座標セット
				caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


				//描画基本位置
				center.set(placeXnear,placeYnear,0,0);
				//center = sg_pDungeonSystem->Map().realvisibleplace(center);


				//基本サイズ
				caec_model.DO.Width = (64+8);
				caec_model.DO.Height = (64+8);
			
				caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeXnear,placeYnear,0,0);


				//描画位置
				//caec_model.Centerbegin = center;
				//caec_model.Centerend = center;

				caec_model.Centerend.y = -16;
				
				
				//シェイプオプション
				caec_model.ScaleX.set(0.5,1,0.5,1);
				caec_model.ScaleY.set(0.5,1,0.5,1);
				//caec_model.Rotation.set(0,0,0,0);
				double d = rand()*360/RAND_MAX;
				caec_model.Rotation2.set(d,d+(rand()*60-30)/RAND_MAX,1,1);



				
				//加算
				caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

				//色
				caec_model.ColorBase.inputD3Dcolor(color);
				caec_model.Alpha.set(1,2,2,0);


				
				//表示時間
				caec_model.life = 15;
				caec_model.hidingTime = 0;	

				//チップに追加
				pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
				caec->addAnimechip(pcac);

			}
		}
		sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));
	}
	{//さんはつめ
		cCoordinate coo(placeX,placeY);
		sg_pDungeonSystem->AnimationManager().
			Anime_PlaySE(_T("bom.wav"),coo);
		unsigned int color,darkcolor;
		cColor c;
		c.HSV(340,1.0,1.0);
		color = c.D3Dcolor();
		c.HSV(340,1.0,0.4);
		darkcolor = c.D3Dcolor();


		pcaECustom caec;
		c4DVector center;
		pcAnimationChipManager_chip pcac;

		int placeXnear, placeYnear;

		caec = pcaECustom(new caECustom);


		//描画位置通知
		caec->place.set(placeX, placeY,0,0);
		

		//レイヤー２
		caec->LayerLevel = LAYERLEVEL_NORMAL;
		
		caEchip_Basic caec_model;

		int i;
		for(i=8;i<25;i++)
		{
			cCoordinate coo,Mplace;
			switch(i+1)
			{
			case 1:
				coo.Set(1,0);break;
			case 2:
				coo.Set(-1,0);break;
			case 3:
				coo.Set(0,-1);break;
			case 4:
				coo.Set(0,1);break;
			case 5:
				coo.Set(1,-1);break;
			case 6:
				coo.Set(-1,-1);break;
			case 7:
				coo.Set(1,1);break;
			case 8:
				coo.Set(-1,1);break;
			case 9:
				coo.Set(2,0);break;
			case 10:
				coo.Set(-2,0);break;
			case 11:
				coo.Set(0,-2);break;
			case 12:
				coo.Set(0,2);break;
			case 13:
				coo.Set(2,-1);break;
			case 14:
				coo.Set(-2,-1);break;
			case 15:
				coo.Set(2,1);break;
			case 16:
				coo.Set(-2,1);break;
			case 17:
				coo.Set(1,-2);break;
			case 18:
				coo.Set(-1,-2);break;
			case 19:
				coo.Set(1,2);break;
			case 20:
				coo.Set(-1,2);break;
			case 21:
				coo.Set(2,-2);break;
			case 22:
				coo.Set(-2,-2);break;
			case 23:
				coo.Set(2,2);break;
			case 24:
				coo.Set(-2,2);break;
			default:
				coo.Set(0,0);break;
			}

			placeXnear = coo.x + placeX;
			placeYnear = coo.y + placeY;
			{//影
				//テクスチャセット
				caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
					sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_simple.png")),SIZE_BASICTEX,SIZE_BASICTEX);

				//テクスチャ座標セット
				caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


				//描画基本位置
				center.set(placeXnear,placeYnear,0,0);
				//center = sg_pDungeonSystem->Map().realvisibleplace(center);


				//基本サイズ
				caec_model.DO.Width = (64)*2;
				caec_model.DO.Height = (64)*2;


				caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeXnear,placeYnear,0,0);


				//描画位置
				//caec_model.Centerbegin = center;
				//caec_model.Centerend = center;

				caec_model.Centerend.y = -16;
				
				
				//シェイプオプション
				caec_model.ScaleX.set(1,1,1,1);
				caec_model.ScaleY.set(1,1,1,1);
				//caec_model.Rotation.set(0,0,0,0);
				double d = rand()*360/RAND_MAX;
				caec_model.Rotation2.set(d,d+(rand()*60-30)/RAND_MAX,1,1);



				
				//加算
				caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;

				//色
				caec_model.ColorBase.inputD3Dcolor(darkcolor);
				caec_model.Alpha.set(0,2,1,0);


				
				//表示時間
				caec_model.life = 15;
				caec_model.hidingTime = 0;	

				//チップに追加
				pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
				caec->addAnimechip(pcac);
			}
			{//光１
				//テクスチャセット
				caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
					sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_smoke.png")),SIZE_BASICTEX,SIZE_BASICTEX);

				//テクスチャ座標セット
				caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


				//描画基本位置
				center.set(placeXnear,placeYnear,0,0);
				//center = sg_pDungeonSystem->Map().realvisibleplace(center);

				//基本サイズ
				caec_model.DO.Width = (64);
				caec_model.DO.Height = (64);

				caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeXnear,placeYnear,0,0);


				//描画位置
				//caec_model.Centerbegin = center;
				//caec_model.Centerend = center;

				caec_model.Centerend.y = -16;
				
				
				//シェイプオプション
				caec_model.ScaleX.set(1,1,1,1);
				caec_model.ScaleY.set(1,1,1,1);
				//caec_model.Rotation.set(0,0,0,0);
				double d = rand()*360/RAND_MAX;
				caec_model.Rotation2.set(d,d+360*2,1,1);



				
				//加算
				caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

				//色
				caec_model.ColorBase.inputD3Dcolor(color);
				caec_model.Alpha.set(1,1,1,1);


				
				//表示時間
				caec_model.life = 15;
				caec_model.hidingTime = 0;	

				//チップに追加
				pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
				caec->addAnimechip(pcac);


			}
			{//衝撃波
				//テクスチャセット
				caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
					sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ring_out.png")),SIZE_BASICTEX,SIZE_BASICTEX);

				//テクスチャ座標セット
				caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


				//描画基本位置
				center.set(placeXnear,placeYnear,0,0);
				//center = sg_pDungeonSystem->Map().realvisibleplace(center);



				//基本サイズ
				caec_model.DO.Width = (64+8)*2;
				caec_model.DO.Height = (64)*2;
			
				caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeXnear,placeYnear,0,0);


				//描画位置
				//caec_model.Centerbegin = center;
				//caec_model.Centerend = center;

				caec_model.Centerend.y = -16;
				
				
				//シェイプオプション
				caec_model.ScaleX.set(0,1,1,1);
				caec_model.ScaleY.set(0,1,1,1);
				//caec_model.Rotation.set(0,0,0,0);
				double d = rand()*360/RAND_MAX;
				caec_model.Rotation2.set(d,d+720,1,1);



				
				//加算
				caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

				//色
				caec_model.ColorBase.inputD3Dcolor(color);
				caec_model.Alpha.set(0,2,1,0);


				
				//表示時間
				caec_model.life = 15;
				caec_model.hidingTime = 0;	

				//チップに追加
				pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
				caec->addAnimechip(pcac);
			}

			{//光2

				//テクスチャセット
				caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
					sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_radio.png")),SIZE_BASICTEX,SIZE_BASICTEX);

				//テクスチャ座標セット
				caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


				//描画基本位置
				center.set(placeXnear,placeYnear,0,0);
				//center = sg_pDungeonSystem->Map().realvisibleplace(center);


				//基本サイズ
				caec_model.DO.Width = (64+8);
				caec_model.DO.Height = (64+8);


				caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeXnear,placeYnear,0,0);


				//描画位置
				//caec_model.Centerbegin = center;
				//caec_model.Centerend = center;

				caec_model.Centerend.y = -16;
				
				
				//シェイプオプション
				caec_model.ScaleX.set(0.5,1,0.5,1);
				caec_model.ScaleY.set(0.5,1,0.5,1);
				//caec_model.Rotation.set(0,0,0,0);
				double d = rand()*360/RAND_MAX;
				caec_model.Rotation2.set(d,d+(rand()*60-30)/RAND_MAX,1,1);



				
				//加算
				caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

				//色
				caec_model.ColorBase.inputD3Dcolor(color);
				caec_model.Alpha.set(1,2,2,0);


				
				//表示時間
				caec_model.life = 15;
				caec_model.hidingTime = 0;	

				//チップに追加
				pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
				caec->addAnimechip(pcac);

			}
		}
		sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));
	}
}

void EffectFunctions::特殊能力発揮エフェクト(const double placeX,const double placeY,const double placeZ)
{
	unsigned int color,darkcolor;
	color = 0xFFFFFFFF;
	darkcolor = 0xFF000000;

	pcaECustom caec;
	c4DVector center;
	pcAnimationChipManager_chip pcac;

	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->Init(caec);
	caec->place.set(placeX,placeY,0,0);
	

	cCoordinate coo(placeX,placeY);
	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("specialattack.wav"),coo);

	//レイヤー２
	caec->LayerLevel = LAYERLEVEL_NORMAL;
	
	caEchip_Basic caec_model;

	caec_model.AbsolutePlaceFlag = true;
	{//影
		//テクスチャセット
		caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_simple.png")),SIZE_BASICTEX,SIZE_BASICTEX);

		//テクスチャ座標セット
		caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


		//描画基本位置
		center.set(0,-placeZ*64/2,0,0);

		//基本サイズ
		caec_model.DO.Width = (64);
		caec_model.DO.Height = (64);

		caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeX,placeY,0,0);

		//描画位置
		caec_model.Centerbegin = center;
		caec_model.Centerend = center;
		
		
		//シェイプオプション
		caec_model.ScaleX.set(1,1,1,1);
		caec_model.ScaleY.set(1,1,1,1);
		//caec_model.Rotation.set(0,0,0,0);
		double d = rand()*360/RAND_MAX;
		caec_model.Rotation2.set(d,d+(rand()*60-30)/RAND_MAX,1,1);



		
		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_SUBSTRACTION;

		//色
		caec_model.ColorBase.inputD3Dcolor(color);
		caec_model.Alpha.set(0,0.5,0.5,0);


		
		//表示時間
		caec_model.life = 7;
		caec_model.hidingTime = 0;	

		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
		caec->addAnimechip(pcac);
	}
	{//光１
		//テクスチャセット
		caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ring_out.png")),SIZE_BASICTEX,SIZE_BASICTEX);

		//テクスチャ座標セット
		caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


		//描画基本位置
		center.set(0,-placeZ*64/2,0,0);


		//基本サイズ
		caec_model.DO.Width = (64);
		caec_model.DO.Height = (64);

		caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeX,placeY,0,0);
		//描画位置
		caec_model.Centerbegin = center;
		caec_model.Centerend = center;

		
		//シェイプオプション
		caec_model.ScaleX.set(2,1,0,1);
		caec_model.ScaleY.set(2,1,0,1);
		//caec_model.Rotation.set(0,0,0,0);
		double d = rand()*360/RAND_MAX;
		caec_model.Rotation2.set(d,d+(rand()*60-30)/RAND_MAX,1,1);



		
		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

		//色
		caec_model.ColorBase.inputD3Dcolor(color);
		caec_model.Alpha.set(0,1.5,1.5,0);


		
		//表示時間
		caec_model.life = 7;
		caec_model.hidingTime = 0;	

		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
		caec->addAnimechip(pcac);
	}
	{//光２
		//テクスチャセット
		caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_simple.png")),SIZE_BASICTEX,SIZE_BASICTEX);

		//テクスチャ座標セット
		caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


		//描画基本位置//実座標
		center.set(0,-placeZ*64/2,0,0);

		//基本サイズ
		caec_model.DO.Width = (64);
		caec_model.DO.Height = (64);

		caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeX,placeY,0,0);

		//描画位置
		caec_model.Centerbegin = center;
		caec_model.Centerend = center;
		
		
		//シェイプオプション
		caec_model.ScaleX.set(0,2,2,0);
		caec_model.ScaleY.set(0,2,2,0);
		//caec_model.Rotation.set(0,0,0,0);
		double d = rand()*360/RAND_MAX;
		caec_model.Rotation2.set(d,d+(rand()*60-30)/RAND_MAX,1,1);



		
		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

		//色
		caec_model.ColorBase.inputD3Dcolor(color);
		caec_model.Alpha.set(0,1.5,1.5,0);


		
		//表示時間
		caec_model.life = 7;
		caec_model.hidingTime = 0;	

		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
		caec->addAnimechip(pcac);
	}

	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));

}

void EffectFunctions::ボスエフェクト集中(const double placeX,const double placeY,const double placeZ)
{
	pAnimeChip_Hab hab(new AnimeChip_Hab(PositionPerGrid(placeX,placeY,placeZ)));
	int life = 60;
	
	int i;
	for(i=0;i<200;i++)
	{
		double x = 1-pow(random_for_effect(),2);
		pAnimeChip_Perticle particle(new AnimeChip_Perticle("effect\\basic\\ball_simple.png",PerticleSize(64,64),life*x));
		particle->color.addCascade(cColor(0,255,255,255),cColor(255,255,255,255),cColor(0,255,255,255),life*x);
		
		
		PositionPerGrid pos(x*20,0,0);
		
		D3DXMATRIX mat;
		D3DXMatrixRotationZ(&mat, random_for_effect()*2*3.14159);
		D3DXVec3TransformCoord(&pos,&pos,&mat);

		particle->position.addCascade(pos,pos,PositionPerGrid(0,0,0),life*x);
		particle->colorBlendMode.setDefault(cDrawableObject::DRAW_MODE_ADDITION);
		hab->addAnimeChip(particle);
	}
	sg_pDungeonSystem->AnimationManager().AddAnime(hab);
}

void EffectFunctions::ボスエフェクトため(const double placeX,const double placeY, cColor& imagecolor1, cColor& imagecolor2)
{
	cValiableField val;
	double temp;
	val.doubles.dim(tstring(_T("PlaceX")),temp = placeX);
	val.doubles.dim(tstring(_T("PlaceY")),temp = placeY);
	val.doubles.dim(tstring(_T("imageColor1")),temp = imagecolor1.D3Dcolor());
	val.doubles.dim(tstring(_T("imageColor2")),temp = imagecolor2.D3Dcolor());


	bool didAnime = AnimationScript_AddAnimation(
		sg_pDungeonSystem->DataBase.EffectImport_Other(),
		_T("EffectBossTame.lua"),
		val);	

}
void EffectFunctions::ボスエフェクト発散(const double placeX,const double placeY, cColor& imagecolor1, cColor& imagecolor2)
{
	cValiableField val;
	double temp;
	val.doubles.dim(tstring(_T("PlaceX")),temp = placeX);
	val.doubles.dim(tstring(_T("PlaceY")),temp = placeY);
	val.doubles.dim(tstring(_T("imageColor1")),temp = imagecolor1.D3Dcolor());
	val.doubles.dim(tstring(_T("imageColor2")),temp = imagecolor2.D3Dcolor());


	bool didAnime = AnimationScript_AddAnimation(
		sg_pDungeonSystem->DataBase.EffectImport_Other(),
		_T("EffectBossHassan.lua"),
		val);	
}

void EffectFunctions::ボスエフェクト撃破(const double placeX,const double placeY,const double placeZ)
{
	pAnimeChip_Hab hab(new AnimeChip_Hab(PositionPerGrid(placeX,placeY,placeZ)));
	int life = 180;
	int life_chip = 30;
	
	int i;
	for(i=0;i<1000;i++)
	{
		double x = 1-pow(random_for_effect(),2);
		pAnimeChip_Perticle particle(new AnimeChip_Perticle("effect\\basic\\ball_simple.png",PerticleSize(64,64),life_chip));
		particle->color.addCascade(cColor(0,255,255,255),cColor(512,255,255,255),cColor(0,255,255,255),life_chip);
		particle->setDelay(life*x);
		
		PositionPerGrid pos(x*20,0,0);
		
		D3DXMATRIX mat;
		D3DXMatrixRotationZ(&mat, random_for_effect()*2*3.14159);
		D3DXVec3TransformCoord(&pos,&pos,&mat);

		particle->position.addCascade(PositionPerGrid(0,0,0),pos,pos,life_chip);
		particle->colorBlendMode.setDefault(cDrawableObject::DRAW_MODE_ADDITION);
		hab->addAnimeChip(particle);
	}
	for(i=0;i<100;i++)
	{
		double x = 1-pow(random_for_effect(),2);
		pAnimeChip_Perticle particle(new AnimeChip_Perticle("effect\\basic\\ball_simple.png",PerticleSize(64,64),life_chip));
		particle->color.addCascade(cColor(0,255,255,255),cColor(512,255,255,255),cColor(0,255,255,255),life_chip);
		
		PositionPerGrid pos(x*20,0,0);
		particle->setDelay(life);
		D3DXMATRIX mat;
		D3DXMatrixRotationZ(&mat, random_for_effect()*2*3.14159);
		D3DXVec3TransformCoord(&pos,&pos,&mat);

		particle->position.addCascade(PositionPerGrid(0,0,0),pos,pos,life_chip);
		particle->colorBlendMode.setDefault(cDrawableObject::DRAW_MODE_ADDITION);
		hab->addAnimeChip(particle);
	}
	for(i=0;i<20;i++)
	{
		pAnimeChip_Sound sound(new AnimeChip_Sound((tstring)_T("hit14.wav"),PositionPerGrid(placeX,placeY,0),1.0));
		sound->setDelay(life*i/20);
		hab->addAnimeChip(sound);
	}
	pAnimeChip_Sound sound(new AnimeChip_Sound((tstring)_T("bom35.wav"),PositionPerGrid(placeX,placeY,0),1.0));
	sound->setDelay(life);
	hab->addAnimeChip(sound);

	sg_pDungeonSystem->AnimationManager().AddAnime(hab);
}

void EffectFunctions::落雷エフェクト(const double placeX,const double placeY)
{
	unsigned int color,darkcolor;
	color = 0xFFFFFFFF;
	darkcolor = 0xFF000000;

	pcaECustom caec;
	c4DVector center;
	pcAnimationChipManager_chip pcac;

	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->Init(caec);
	caec->place.set(placeX,placeY,0,0);
	
	//レイヤー２
	caec->LayerLevel = LAYERLEVEL_NORMAL;
	
	caEchip_Basic caec_model;

	caec_model.AbsolutePlaceFlag = true;
	int texindex = rand()*4/(RAND_MAX+1);

	cCoordinate coo(placeX,placeY);
	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("kaminari.wav"),coo);

	{//カミナリ

		//テクスチャセット
		caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\elec.png")));

		//テクスチャ座標セット
		caec_model.DO.m_TexRange.setLTRB(texindex/8.0, 0, (texindex+1)/8.0, 1);
		caec_model.texRange.clear();
		//c4DVector tex;
		//tex.set(0/4.0, 0, 1/4.0, 1);
		//caec_model.texRange.setValue(3,tex);
		//tex.set(1/4.0, 0, 2/4.0, 1);
		//caec_model.texRange.setValue(3,tex);

		//基本サイズ
		caec_model.DO.Width = (64);
		caec_model.DO.Height = (512);

		caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeX,placeY,0,0);

		//描画位置
		caec_model.Centerbegin.set(0,-256 +32,0,0);
		caec_model.Centerend.set(0,-256 +32,0,0);

		//ムーブパス
		//caec_model.Movepass.set(0,1,0.5,1);
		
		//シェイプオプション
		caec_model.ScaleX.set(1,1,1,1);
		caec_model.ScaleY.set(1,1,1,1);
		//caec_model.Rotation.set(0,0,0,0);
		//double d = rand()*360/RAND_MAX;
		//caec_model.Rotation2.set(d,d+(rand()*60-30)/RAND_MAX,1,1);



		
		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

		//色
		caec_model.ColorBase.inputD3Dcolor(color);
		caec_model.Alpha.set(1,0,1,1);


		
		//表示時間
		caec_model.life = 20;
		caec_model.hidingTime = 0;	

		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
		caec->addAnimechip(pcac);
	}
	{//光１
		//テクスチャセット
		caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ring_out.png")),SIZE_BASICTEX,SIZE_BASICTEX);

		//テクスチャ座標セット
		caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


		//描画基本位置
		center.set(0,0,0,0);


		//基本サイズ
		caec_model.DO.Width = (64);
		caec_model.DO.Height = (64);

		caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeX,placeY,0,0);
		//描画位置
		caec_model.Centerbegin = center;
		caec_model.Centerend = center;

		
		//シェイプオプション
		caec_model.ScaleX.set(2,1,0,1);
		caec_model.ScaleY.set(2,1,0,1);
		//caec_model.Rotation.set(0,0,0,0);
		double d = rand()*360/RAND_MAX;
		caec_model.Rotation2.set(d,d+(rand()*60-30)/RAND_MAX,1,1);



		
		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

		//色
		caec_model.ColorBase.inputD3Dcolor(color);
		caec_model.Alpha.set(0,2,2,0);


		
		//表示時間
		caec_model.life = 6;
		caec_model.hidingTime = 0;	

		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
		caec->addAnimechip(pcac);
	}
	sg_pDungeonSystem->AnimationManager().AddAnime_parallel(boost::static_pointer_cast<cAnimation>(caec));

}
void EffectFunctions::ダメージ火エフェクト(const double placeX,const double placeY)
{
	unsigned int color,darkcolor;
	color = 0xFFFF5500;
	darkcolor = 0xFF000000;
	double d,d2;
	
	pcaECustom caec;
	c4DVector center;
	pcAnimationChipManager_chip pcac;

	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->Init(caec);
	caec->place.set(placeX,placeY,0,0);
	
	//レイヤー２
	caec->LayerLevel = LAYERLEVEL_NORMAL;
	
	caEchip_Basic caec_model;

	caec_model.AbsolutePlaceFlag = true;
	int texindex = rand()*4/(RAND_MAX+1);

	IDirect3DTexture9* ptex[4];
	ptex[0] = g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\fire\\firetex1mini.png"));
	ptex[1] = g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\fire\\firetex2mini.png"));
	ptex[2] = g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\fire\\firetex3mini.png"));
	ptex[3] = g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\fire\\firetex4mini.png"));


	{//火



		//テクスチャ座標セット
		caec_model.DO.m_TexRange.setLTRB(texindex/8.0, 0, (texindex+1)/8.0, 1);
		caec_model.texRange.clear();
		c4DVector tex;
		int i,k;
		for(i=0;i<4;i++)
		{
			for(k=0;k<4;k++)
			{
				tex.set(k/4.0, i/4.0, (k+1)/4.0, (i+1)/4.0);
				caec_model.texRange.setValue(1,tex);
			}
		}


		//基本サイズ
		caec_model.DO.Width = (48);
		caec_model.DO.Height = (48);

		caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeX,placeY,0,0);


		//ムーブパス
		//caec_model.Movepass.set(0,1,0.5,1);
		
		//シェイプオプション
		caec_model.ScaleX.set(0.5,1.5,1.5,0.5);
		caec_model.ScaleY.set(0.5,1.5,1.5,0.5);
		//caec_model.Rotation.set(0,0,0,0);
		//double d = rand()*360/RAND_MAX;
		//caec_model.Rotation2.set(d,d+(rand()*60-30)/RAND_MAX,1,1);



		
		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;

		//色
		caec_model.ColorBase.inputD3Dcolor(color);
		caec_model.Alpha.set(1,1,1,1);


		
		//表示時間
		caec_model.life = 16;
		caec_model.hidingTime = 0;

		for(i=0;i<6;i++)
		{
			caec_model.hidingTime = i*2;

			
			d2 = rand()*64/RAND_MAX;
			d = rand()*360/RAND_MAX;
			//描画位置
			caec_model.Centerbegin.set(16,0,d2,0);
			caec_model.Centerbegin.turn(d);
			caec_model.Centerend = caec_model.Centerbegin;
			caec_model.Centerend.z += 16;
			
			//テクスチャセット
			texindex = rand()*4/(RAND_MAX+1);
			caec_model.DO.setTexture(ptex[texindex]);

			//チップに追加
			pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
			caec->addAnimechip(pcac);
		}
		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

		for(i=0;i<6;i++)
		{
			caec_model.hidingTime = i*2;

			
			d2 = rand()*64/RAND_MAX;
			d = rand()*360/RAND_MAX;
			//描画位置
			caec_model.Centerbegin.set(16,0,d2,0);
			caec_model.Centerbegin.turn(d);
			caec_model.Centerend = caec_model.Centerbegin;
			caec_model.Centerend.z += 16;
			
			//テクスチャセット
			texindex = rand()*4/(RAND_MAX+1);
			caec_model.DO.setTexture(ptex[texindex]);

			//チップに追加
			pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
			caec->addAnimechip(pcac);
		}
	}
	
	sg_pDungeonSystem->AnimationManager().AddAnime_parallel(boost::static_pointer_cast<cAnimation>(caec));

}
void EffectFunctions::ダメージ水エフェクト(const double placeX,const double placeY)
{
	unsigned int color,darkcolor;
	color = 0xFF88FFFF;
	darkcolor = 0xFF000000;
	double d,d2;

	pcaECustom caec;
	c4DVector center;
	pcAnimationChipManager_chip pcac;

	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->Init(caec);
	caec->place.set(placeX,placeY,0,0);
	
	//レイヤー２
	caec->LayerLevel = LAYERLEVEL_NORMAL;
	
	caEchip_Basic caec_model;

	caec_model.AbsolutePlaceFlag = true;
	{//水
		//テクスチャセット
		caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\water.png")));

		//テクスチャ座標セット
		caec_model.DO.m_TexRange.setLTRB(0/4.0, 0/4.0, (1)/4.0, (1)/4.0);



		//基本サイズ
		caec_model.DO.Width = (24);
		caec_model.DO.Height = (24);

		caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeX,placeY,0,0);


		//ムーブパス
		caec_model.Movepass.set(0,1,0.5,1);
		
		//シェイプオプション
		caec_model.ScaleX.set(0.5,1,1.5,1);
		caec_model.ScaleY.set(0.5,1,1.5,1);
		//caec_model.Rotation.set(0,0,0,0);
		//double d = rand()*360/RAND_MAX;
		//caec_model.Rotation2.set(d,d+(rand()*60-30)/RAND_MAX,1,1);



		
		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;

		//色
		caec_model.ColorBase.inputD3Dcolor(color);
		caec_model.Alpha.set(0,2,2,0);


		
		//表示時間
		caec_model.life = 16;
		caec_model.hidingTime = 0;

		int i;
		for(i=0;i<6;i++)
		{
			caec_model.hidingTime = i*2;

			
			d2 = rand()*80/RAND_MAX;
			d = rand()*360/RAND_MAX;
			//描画位置
			caec_model.Centerbegin.set(16,0,d2,0);
			caec_model.Centerbegin.turn(d);
			caec_model.Centerend = caec_model.Centerbegin;
			caec_model.Centerend.z -= 16;

			//チップに追加
			pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
			caec->addAnimechip(pcac);
		}
	}
	
	sg_pDungeonSystem->AnimationManager().AddAnime_parallel(boost::static_pointer_cast<cAnimation>(caec));

}
void EffectFunctions::ダメージ電気エフェクト(const double placeX,const double placeY)
{
	unsigned int color,darkcolor;
	color = 0xFFFFFFFF;
	darkcolor = 0xFF000000;
	double d,d2;

	pcaECustom caec;
	c4DVector center;
	pcAnimationChipManager_chip pcac;

	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->Init(caec);
	caec->place.set(placeX,placeY,0,0);
	
	//レイヤー２
	caec->LayerLevel = LAYERLEVEL_NORMAL;
	
	caEchip_Basic caec_model;

	caec_model.AbsolutePlaceFlag = true;
	int texindex;

	{//カミナリ
		//テクスチャセット
		caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\elec.png")));



		//基本サイズ
		caec_model.DO.Width = (64);
		caec_model.DO.Height = (8);

		caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeX,placeY,0,0);


		//ムーブパス
		caec_model.Movepass.set(0,1,0.5,1);
		
		//シェイプオプション
		caec_model.ScaleX.set(1,1,1,1);
		caec_model.ScaleY.set(2,2,1,1);
		//caec_model.Rotation.set(0,0,0,0);


		
		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

		//色
		caec_model.ColorBase.inputD3Dcolor(color);
		caec_model.Alpha.set(1,0,1,1);


		
		//表示時間
		caec_model.life = 16;
		caec_model.hidingTime = 0;

		int i;
		for(i=0;i<6;i++)
		{
			caec_model.hidingTime = i*2;
			
			//シェイプオプション
			d = rand()*360/RAND_MAX;
			caec_model.Rotation2.set(d,d,1,1);


			//テクスチャ座標セット
			texindex = rand()*3/(RAND_MAX+1);
			caec_model.DO.m_TexRange.setLTRB(texindex/8.0, 0/4.0, (1+texindex)/8.0, (4)/4.0);
			caec_model.texRange.clear();

			
			d2 = rand()*64/RAND_MAX;
			d = rand()*360/RAND_MAX;
			//描画位置
			caec_model.Centerbegin.set(16,0,d2,0);
			caec_model.Centerbegin.turn(d);
			caec_model.Centerend = caec_model.Centerbegin;


			//チップに追加
			pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
			caec->addAnimechip(pcac);
		}
	}
	
	sg_pDungeonSystem->AnimationManager().AddAnime_parallel(boost::static_pointer_cast<cAnimation>(caec));

}
void EffectFunctions::ダメージ冷気エフェクト(const double placeX,const double placeY)
{
	unsigned int color,darkcolor;
	color = 0xFF99FFFF;
	darkcolor = 0xFF000000;
	int i;
	double d,d2;

	pcaECustom caec;
	c4DVector center;
	pcAnimationChipManager_chip pcac;

	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->Init(caec);
	caec->place.set(placeX,placeY,0,0);
	
	//レイヤー２
	caec->LayerLevel = LAYERLEVEL_NORMAL;
	
	caEchip_Basic caec_model;

	caec_model.AbsolutePlaceFlag = true;
	int texindex;

	{//雪

		//テクスチャセット
		caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\ice.png")));

		//テクスチャ座標セット
		caec_model.DO.m_TexRange.setLTRB(0/4.0, 0/4.0, (1)/4.0, (1)/4.0);
		caec_model.texRange.clear();


		//基本サイズ
		caec_model.DO.Width = (24);
		caec_model.DO.Height = (24);

		caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeX,placeY,0,0);



		//ムーブパス
		//caec_model.Movepass.set(0,1,0.5,1);
		
		//シェイプオプション
		caec_model.ScaleX.set(1,1,1,1);
		caec_model.ScaleY.set(1,1,1,1);

		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

		//色
		caec_model.ColorBase.inputD3Dcolor(color);
		caec_model.Alpha.set(0,2,2,0);
		
		//表示時間
		caec_model.life = 16;

		for(i=0;i<6;i++)
		{
			caec_model.hidingTime = i*2;
			
			//テクスチャ座標セット
			texindex = rand()*3/(RAND_MAX+1);
			caec_model.DO.m_TexRange.setLTRB(texindex/4.0, 0/4.0, (1+texindex)/4.0, (1)/4.0);
			caec_model.texRange.clear();


			
			d2 = rand()*80/RAND_MAX;
			d = rand()*360/RAND_MAX;
			//描画位置
			caec_model.Centerbegin.set(16,0,d2,0);
			caec_model.Centerbegin.turn(d);
			caec_model.Centerend = caec_model.Centerbegin;
			caec_model.Centerend.z -= 16;
			caec_model.Centerend.y += rand()*8/(RAND_MAX+1) -4;
			
			d = rand()*360/RAND_MAX;
			caec_model.Rotation2.set(d,d+(rand()*60-30)/RAND_MAX,1,1);


			//チップに追加
			pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
			caec->addAnimechip(pcac);
		}
	}
	
	sg_pDungeonSystem->AnimationManager().AddAnime_parallel(boost::static_pointer_cast<cAnimation>(caec));
	
}
void EffectFunctions::ダメージ爆発エフェクト(const double placeX,const double placeY)
{

	unsigned int color,darkcolor;
	color = 0xFFFFFFFF;
	darkcolor = 0xFF000000;
	int i;
	double d,d2;

	pcaECustom caec;
	c4DVector center;
	pcAnimationChipManager_chip pcac;

	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->Init(caec);
	caec->place.set(placeX,placeY,0,0);
	
	//レイヤー２
	caec->LayerLevel = LAYERLEVEL_NORMAL;
	
	caEchip_Basic caec_model;

	caec_model.AbsolutePlaceFlag = true;
	int texindex;

	{//もわ

		//テクスチャセット
		caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ring_out.png")));

		//テクスチャ座標セット
		caec_model.DO.m_TexRange.setLTRB(0,0,1,1);
		caec_model.texRange.clear();


		//基本サイズ
		caec_model.DO.Width = (32);
		caec_model.DO.Height = (32);

		caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeX,placeY,0,0);



		//ムーブパス
		//caec_model.Movepass.set(0,1,0.5,1);
		
		//シェイプオプション
		caec_model.ScaleX.set(0,1,1,1);
		caec_model.ScaleY.set(0,1,1,1);

		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

		//色
		caec_model.ColorBase.inputD3Dcolor(color);
		caec_model.Alpha.set(0,2,2,0);

		//表示時間
		caec_model.life = 8;

		for(i=0;i<6;i++)
		{
			caec_model.hidingTime = i*2;
			

			
			d2 = rand()*64/RAND_MAX;
			d = rand()*360/RAND_MAX;
			//描画位置
			caec_model.Centerbegin.set(16,0,d2,0);
			caec_model.Centerbegin.turn(d);
			caec_model.Centerend = caec_model.Centerbegin;



			//チップに追加
			pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
			caec->addAnimechip(pcac);
		}
	}
	
	sg_pDungeonSystem->AnimationManager().AddAnime_parallel(boost::static_pointer_cast<cAnimation>(caec));
	
}
void EffectFunctions::ダメージ気エフェクト(const double placeX,const double placeY)
{
	cColor color_c;
	unsigned int color,darkcolor;
	color = 0xFFFFFFFF;
	darkcolor = 0xFF000000;
	int i;
	double d,d2;

	pcaECustom caec;
	c4DVector center;
	pcAnimationChipManager_chip pcac;

	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->Init(caec);
	caec->place.set(placeX,placeY,0,0);
	
	//レイヤー２
	caec->LayerLevel = LAYERLEVEL_NORMAL;
	
	caEchip_Basic caec_model;

	caec_model.AbsolutePlaceFlag = true;
	int texindex;

	{//もわ

		//テクスチャセット
		caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_antismoke.png")));

		//テクスチャ座標セット
		caec_model.DO.m_TexRange.setLTRB(0,0,1,1);
		caec_model.texRange.clear();


		//基本サイズ
		caec_model.DO.Width = (24);
		caec_model.DO.Height = (24);

		caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeX,placeY,0,0);



		//ムーブパス
		//caec_model.Movepass.set(0,1,0.5,1);
		

		//シェイプオプション
		caec_model.ScaleX.set(1,1,1,1);
		caec_model.ScaleY.set(1,1,1,1);
		//caec_model.Rotation.set(0,0,0,0);



		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

		//色
		caec_model.ColorBase.inputD3Dcolor(color);
		caec_model.Alpha.set(0,2,2,0);

		//表示時間
		caec_model.life = 16;

		for(i=0;i<6;i++)
		{
			caec_model.hidingTime = i*2;
			
			
			d2 = rand()*64/RAND_MAX;
			d = rand()*360/RAND_MAX;
			//描画位置
			caec_model.Centerbegin.set(16,0,d2,0);
			caec_model.Centerbegin.turn(d);
			caec_model.Centerend = caec_model.Centerbegin;


			d = rand()*360/RAND_MAX;
			color_c.HSV(d,0.9,0.9);
			//色
			caec_model.ColorBase.inputD3Dcolor(color_c.D3Dcolor());
			
			//シェイプオプション
			//caec_model.Rotation.set(0,0,0,0);
			double d = rand()*360/RAND_MAX;
			caec_model.Rotation2.set(d,d,1,1);

			//チップに追加
			pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
			caec->addAnimechip(pcac);
		}
	}
	
	sg_pDungeonSystem->AnimationManager().AddAnime_parallel(boost::static_pointer_cast<cAnimation>(caec));
	
}


void EffectFunctions::つるべ落としエフェクト(const double placeX,const double placeY)
{
	unsigned int color,darkcolor;
	color = 0xFFFFFFFF;
	darkcolor = 0xFF000000;

	pcaECustom caec;
	c4DVector center;
	pcAnimationChipManager_chip pcac;

	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->Init(caec);
	caec->place.set(placeX,placeY,0,0);
	
	//レイヤー２
	caec->LayerLevel = LAYERLEVEL_NORMAL;
	
	caEchip_Basic caec_model;

	caec_model.AbsolutePlaceFlag = true;
	int texindex = rand()*4/(RAND_MAX+1);

	{//きすめ
		if(texindex == 3)
		{
			//テクスチャセット
			caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
				sg_pDungeonSystem->pDevice_D3D,_T("effect\\つるべおとし・きすめ.png")));

			//テクスチャ座標セット
			c4DVector tex;
			tex.set(0/4.0, 0, 1/4.0, 1);
			caec_model.texRange.setValue(3,tex);
			tex.set(1/4.0, 0, 2/4.0, 1);
			caec_model.texRange.setValue(3,tex);
		}
		else
		{
			//テクスチャセット
			caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
				sg_pDungeonSystem->pDevice_D3D,_T("effect\\つるべおとし・そのた.png")));

			//テクスチャ座標セット
			c4DVector tex;
			tex.set(texindex/4.0, 0, (texindex+1)/4.0, 1);
			caec_model.texRange.setValue(3,tex);
		}
		//基本サイズ
		caec_model.DO.Width = (64);
		caec_model.DO.Height = (64);

		caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeX,placeY,0,0);

		//描画位置
		caec_model.Centerbegin.set(0,-300,0,0);
		caec_model.Centerend.set(0,-64,0,0);

		//ムーブパス
		caec_model.Movepass.set(0,1,0.5,1);
		
		//シェイプオプション
		caec_model.ScaleX.set(1,1,1,1);
		caec_model.ScaleY.set(1,1,1,1);
		//caec_model.Rotation.set(0,0,0,0);
		//double d = rand()*360/RAND_MAX;
		//caec_model.Rotation2.set(d,d+(rand()*60-30)/RAND_MAX,1,1);



		
		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;

		//色
		caec_model.ColorBase.inputD3Dcolor(color);
		caec_model.Alpha.set(0,1,1,1);


		
		//表示時間
		caec_model.life = 30;
		caec_model.hidingTime = 0;	

		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
		caec->addAnimechip(pcac);
	}
	{//きすめおわり
		if(texindex == 3)
		{
			//テクスチャセット
			caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
				sg_pDungeonSystem->pDevice_D3D,_T("effect\\つるべおとし・きすめ.png")));

			//テクスチャ座標セット
			caec_model.DO.m_TexRange.setLTRB(2/4.0, 0, 3/4.0, 1);
			caec_model.texRange.clear();
		}
		else
		{
			//テクスチャセット
			caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
				sg_pDungeonSystem->pDevice_D3D,_T("effect\\つるべおとし・そのた.png")));

			//テクスチャ座標セット
			caec_model.DO.m_TexRange.setLTRB(texindex/4.0, 0, (texindex+1)/4.0, 1);
			caec_model.texRange.clear();
		}
		//基本サイズ
		caec_model.DO.Width = (64);
		caec_model.DO.Height = (64);

		caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeX,placeY,0,0);

		//描画位置
		caec_model.Centerbegin.set(0,-64,0,0);
		caec_model.Centerend.set(0,-64,0,0);

		//ムーブパス
		caec_model.Movepass.set(0,1,0.5,1);
		
		//シェイプオプション
		caec_model.ScaleX.set(1,1,1,1);
		caec_model.ScaleY.set(1,1,1,1);
		//caec_model.Rotation.set(0,0,0,0);
		//double d = rand()*360/RAND_MAX;
		//caec_model.Rotation2.set(d,d+(rand()*60-30)/RAND_MAX,1,1);


		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;

		//色
		caec_model.ColorBase.inputD3Dcolor(color);
		caec_model.Alpha.set(1,0,1,1);


		
		//表示時間
		caec_model.life = 5;
		caec_model.hidingTime = 30;	

		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
		caec->addAnimechip(pcac);
	}
	{//☆
		caEchip_Bound caec_model_bound;
		//テクスチャセット
		caec_model_bound.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\star.png")));

		//テクスチャ座標セット
		caec_model_bound.DO.m_TexRange.setLTRB(0, 0, 1, 1);
		caec_model_bound.texRange.clear();

		//基本サイズ
		caec_model_bound.DO.Width = (16);
		caec_model_bound.DO.Height = (16);

		caec_model_bound.AbsolutePlaceFlag = 0;

		//開始位置
		caec_model_bound.Centerbegin.set(0,0,64,0);
		
		//シェイプオプション
		//caec_model_bound.ScaleX.set(1,1,1,1);
		//caec_model_bound.ScaleY.set(1,1,1,1);
		//caec_model_bound.Rotation.set(0,0,0,0);
		//double d = rand()*360/RAND_MAX;
		//caec_model_bound.Rotation2.set(d,d+(rand()*60-30)/RAND_MAX,1,1);


		//加算
		caec_model_bound.DO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;


		//色
		caec_model_bound.ColorBase.inputD3Dcolor(0xFFFFDD00);
		caec_model_bound.Alpha.set(0,2,2,0);


		
		//表示時間
		caec_model_bound.life = 5;
		caec_model_bound.hidingTime = 30;	

		int i;
		for(i=0;i<8;i++)
		{
			//方向
			caec_model_bound.moveVector.set(8,0,4,0);
			double d = rand()*360/RAND_MAX;
			caec_model_bound.moveVector.turn(d);

			//チップに追加
			pcac = pcAnimationChipManager_chip(new caEchip_Bound(caec_model_bound));
			caec->addAnimechip(pcac);
		}
	}
	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));

}

void EffectFunctions::氷付けエフェクト(const double placeX,const double placeY)
{
	unsigned int color,darkcolor;
	color = 0xFF77EEFF;
	darkcolor = 0xFF000000;

	pcaECustom caec;
	c4DVector center;
	pcAnimationChipManager_chip pcac;

	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->Init(caec);
	caec->place.set(placeX,placeY,0,0);
	
	//レイヤー２
	caec->LayerLevel = LAYERLEVEL_NORMAL;
	
	caEchip_Basic caec_model;

	caec_model.AbsolutePlaceFlag = true;
	cCoordinate coo(placeX,placeY);
	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("freeze.wav"),coo);

	int texindex = rand()*3/(RAND_MAX+1);

	{//氷

		
		//テクスチャセット
		caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\ice.png")));

		//テクスチャ座標セット
		c4DVector tex;
		tex.set(texindex/4.0, 1/4.0, (texindex+1)/4.0, 2/4.0);
		caec_model.texRange.clear();
		caec_model.texRange.setValue(3,tex);
	
		//基本サイズ
		caec_model.DO.Width = (64);
		caec_model.DO.Height = (64);

		caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeX,placeY,0,0);

		//描画位置
		caec_model.Centerbegin.set(0,-0,0,0);
		caec_model.Centerend.set(0,-0,0,0);

		//ムーブパス
		//caec_model.Movepass.set(0,1,0.5,1);
		
		//シェイプオプション
		caec_model.ScaleX.set(1,1,1,1);
		caec_model.ScaleY.set(1,1,1,1);
		//caec_model.Rotation.set(0,0,0,0);
		double d = rand()*360/RAND_MAX;
		caec_model.Rotation2.set(d,d,1,1);



		
		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;

		//色
		caec_model.ColorBase.inputD3Dcolor(color);
		caec_model.Alpha.set(1,0,1,1);


		
		//表示時間
		caec_model.life = 30;
		caec_model.hidingTime = 0;	

		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
		caec->addAnimechip(pcac);
		
		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
		caec->addAnimechip(pcac);
	}
	{//☆
		caEchip_Bound caec_model_bound;
		//テクスチャセット
		caec_model_bound.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\ice.png")));



		//基本サイズ
		caec_model_bound.DO.Width = (32);
		caec_model_bound.DO.Height = (32);

		caec_model_bound.AbsolutePlaceFlag = 0; caec_model_bound.place.set(placeX,placeY,0,0);

		//開始位置
		caec_model_bound.Centerbegin.set(0,0,0,0);
		
		//シェイプオプション
		caec_model_bound.ScaleX.set(1,0.2,1,1);
		caec_model_bound.ScaleY.set(1,0.2,1,1);
		caec_model_bound.Rotation.set(0,0,0,0);


		//加算
		caec_model_bound.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;


		//色
		caec_model_bound.ColorBase.inputD3Dcolor(color);
		caec_model_bound.Alpha.set(0,2,2,0);


		
		//表示時間
		caec_model_bound.life = 30;
		caec_model_bound.hidingTime = 0;	

		int i;
		for(i=0;i<8;i++)
		{
			texindex = rand()*3/(RAND_MAX+1);
			//テクスチャ座標セット
			caec_model_bound.DO.m_TexRange.setLTRB(texindex/4.0, 1/4.0, (texindex+1)/4.0, 2/4.0);
	
			double d;
			d = rand()*360/RAND_MAX;
			caec_model_bound.Rotation2.set(d,d+(rand()*60-30)/RAND_MAX,1,1);

			//方向
			caec_model_bound.moveVector.set(8,0,8,0);
			d = rand()*360/RAND_MAX;
			caec_model_bound.moveVector.turn(d);

			//チップに追加
			pcac = pcAnimationChipManager_chip(new caEchip_Bound(caec_model_bound));
			caec->addAnimechip(pcac);
		}
	}
	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));

}
void EffectFunctions::氷付けエフェクト_短(const double placeX,const double placeY)
{
	unsigned int color,darkcolor;
	color = 0xFF77EEFF;
	darkcolor = 0xFF000000;

	pcaECustom caec;
	c4DVector center;
	pcAnimationChipManager_chip pcac;

	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->Init(caec);
	caec->place.set(placeX,placeY,0,0);
	
	//レイヤー２
	caec->LayerLevel = LAYERLEVEL_NORMAL;
	
	caEchip_Basic caec_model;

	caec_model.AbsolutePlaceFlag = true;
	int texindex = rand()*3/(RAND_MAX+1);

	{//氷

		
		//テクスチャセット
		caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\ice.png")));

		//テクスチャ座標セット
		c4DVector tex;
		tex.set(texindex/4.0, 1/4.0, (texindex+1)/4.0, 2/4.0);
		caec_model.texRange.clear();
		caec_model.texRange.setValue(3,tex);
	
		//基本サイズ
		caec_model.DO.Width = (64);
		caec_model.DO.Height = (64);

		caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeX,placeY,0,0);

		//描画位置
		caec_model.Centerbegin.set(0,-0,0,0);
		caec_model.Centerend.set(0,-0,0,0);

		//ムーブパス
		//caec_model.Movepass.set(0,1,0.5,1);
		
		//シェイプオプション
		caec_model.ScaleX.set(1,1,1,1);
		caec_model.ScaleY.set(1,1,1,1);
		//caec_model.Rotation.set(0,0,0,0);
		double d = rand()*360/RAND_MAX;
		caec_model.Rotation2.set(d,d,1,1);



		
		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;

		//色
		caec_model.ColorBase.inputD3Dcolor(color);
		caec_model.Alpha.set(1,0,1,1);


		
		//表示時間
		caec_model.life = 15;
		caec_model.hidingTime = 0;	

		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
		caec->addAnimechip(pcac);
		
		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
		caec->addAnimechip(pcac);
	}
	{//☆
		caEchip_Bound caec_model_bound;
		//テクスチャセット
		caec_model_bound.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\ice.png")));



		//基本サイズ
		caec_model_bound.DO.Width = (32);
		caec_model_bound.DO.Height = (32);

		caec_model_bound.AbsolutePlaceFlag = 0; caec_model_bound.place.set(placeX,placeY,0,0);


		//開始位置
		caec_model_bound.Centerbegin.set(0,0,0,0);
		
		//シェイプオプション
		caec_model_bound.ScaleX.set(1,0.2,1,1);
		caec_model_bound.ScaleY.set(1,0.2,1,1);
		caec_model_bound.Rotation.set(0,0,0,0);


		//加算
		caec_model_bound.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;


		//色
		caec_model_bound.ColorBase.inputD3Dcolor(color);
		caec_model_bound.Alpha.set(0,2,2,0);


		
		//表示時間
		caec_model_bound.life = 15;
		caec_model_bound.hidingTime = 0;	

		int i;
		for(i=0;i<4;i++)
		{
			texindex = rand()*3/(RAND_MAX+1);
			//テクスチャ座標セット
			caec_model_bound.DO.m_TexRange.setLTRB(texindex/4.0, 1/4.0, (texindex+1)/4.0, 2/4.0);
	
			double d;
			d = rand()*360/RAND_MAX;
			caec_model_bound.Rotation2.set(d,d+(rand()*60-30)/RAND_MAX,1,1);

			//方向
			caec_model_bound.moveVector.set(8,0,8,0);
			d = rand()*360/RAND_MAX;
			caec_model_bound.moveVector.turn(d);

			//チップに追加
			pcac = pcAnimationChipManager_chip(new caEchip_Bound(caec_model_bound));
			caec->addAnimechip(pcac);
		}
	}
	sg_pDungeonSystem->AnimationManager().AddAnime_parallel(boost::static_pointer_cast<cAnimation>(caec));
	sg_pDungeonSystem->AnimationManager().Anime_Wait(placeX,placeY,10);
}
void EffectFunctions::パワーアップエフェクト(const double placeX,const double placeY, double color_H)
{
	cColor color_c;
	color_c.HSV(color_H,0.9,0.9);
	unsigned int color,darkcolor;
	color = color_c.D3Dcolor();
	darkcolor = 0xFF000000;
	int i;

	pcaECustom caec;
	c4DVector center;
	pcAnimationChipManager_chip pcac;

	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->Init(caec);
	caec->place.set(placeX,placeY,0,0);
	
	//レイヤー２
	caec->LayerLevel = LAYERLEVEL_NORMAL;
	
	caEchip_Basic caec_model;

	caec_model.AbsolutePlaceFlag = true;
	cCoordinate coo(placeX,placeY);
	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("powerup.wav"),coo);

	{//わっか

		//テクスチャセット
		caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ring_out.png")));

		//テクスチャ座標セット
		caec_model.DO.m_TexRange.setLTRB(0, 0, 1, 1);
		caec_model.texRange.clear();
		//c4DVector tex;
		//tex.set(0/4.0, 0, 1/4.0, 1);
		//caec_model.texRange.setValue(3,tex);
		//tex.set(1/4.0, 0, 2/4.0, 1);
		//caec_model.texRange.setValue(3,tex);

		//基本サイズ
		caec_model.DO.Width = (64);
		caec_model.DO.Height = (64)-16;

		caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeX,placeY,0,0);

		//描画位置
		caec_model.Centerbegin.set(0,0,0,0);
		caec_model.Centerend.set(0,0,128,0);

		//ムーブパス
		//caec_model.Movepass.set(0,1,0.5,1);
		
		//シェイプオプション
		caec_model.ScaleX.set(1,1,1,1);
		caec_model.ScaleY.set(1,1,1,1);
		//caec_model.Rotation.set(0,0,0,0);
		//double d = rand()*360/RAND_MAX;
		//caec_model.Rotation2.set(d,d+(rand()*60-30)/RAND_MAX,1,1);


		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

		//色
		caec_model.ColorBase.inputD3Dcolor(color);
		caec_model.Alpha.set(1,0,1,1);


		for(i=0;i<3;i++)
		{
			//表示時間
			caec_model.life = 10;
			caec_model.hidingTime = 0+i*5;	

			//チップに追加
			pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
			caec->addAnimechip(pcac);
		}
	}
	{//放射

		//テクスチャセット
		caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ring_radio.png")));

		//テクスチャ座標セット
		caec_model.DO.m_TexRange.setLTRB(0, 0, 1, 1);
		caec_model.texRange.clear();
		//c4DVector tex;
		//tex.set(0/4.0, 0, 1/4.0, 1);
		//caec_model.texRange.setValue(3,tex);
		//tex.set(1/4.0, 0, 2/4.0, 1);
		//caec_model.texRange.setValue(3,tex);

		//基本サイズ
		caec_model.DO.Width = (64);
		caec_model.DO.Height = (64);

		caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeX,placeY,0,0);

		//描画位置
		caec_model.Centerbegin.set(0,0,0,0);
		caec_model.Centerend.set(0,0,0,0);

		//ムーブパス
		//caec_model.Movepass.set(0,1,0.5,1);
		
		//シェイプオプション
		caec_model.ScaleX.set(0,3,1,1);
		caec_model.ScaleY.set(0,3,0.75,0.75);
		double d = rand()*360/RAND_MAX;
		caec_model.Rotation.set(d,d,1,1);


		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

		//色
		caec_model.ColorBase.inputD3Dcolor(color);
		caec_model.Alpha.set(0,2,2,0);


		for(i=0;i<3;i++)
		{
			//表示時間
			caec_model.life = 10;
			caec_model.hidingTime = 0+i*5;	

			//チップに追加
			pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
			caec->addAnimechip(pcac);
		}
	}
	{//点々

		//テクスチャセット
		caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_simple.png")));

		//テクスチャ座標セット
		caec_model.DO.m_TexRange.setLTRB(0, 0, 1, 1);
		caec_model.texRange.clear();
		//c4DVector tex;
		//tex.set(0/4.0, 0, 1/4.0, 1);
		//caec_model.texRange.setValue(3,tex);
		//tex.set(1/4.0, 0, 2/4.0, 1);
		//caec_model.texRange.setValue(3,tex);

		//基本サイズ
		caec_model.DO.Width = (4);
		caec_model.DO.Height = (16);

		caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeX,placeY,0,0);

		//描画位置
		caec_model.Centerbegin.set(0,0,0,0);
		caec_model.Centerend.set(0,0,128,0);

		//ムーブパス
		caec_model.Movepass.set(0,1,0.5,1);
		
		//シェイプオプション
		caec_model.ScaleX.set(1,1,1,1);
		caec_model.ScaleY.set(1,1,1,1);
		caec_model.Rotation.set(0,0,0,0);
		//double d = rand()*360/RAND_MAX;
		caec_model.Rotation2.set(0,0,1,1);


		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

		//色
		caec_model.ColorBase.inputD3Dcolor(color);
		caec_model.Alpha.set(0,2,2,0);


		for(i=0;i<10;i++)
		{
			//表示時間
			caec_model.life = 10;
			caec_model.hidingTime = 0+i*2;	
			double d;
			d = rand()*360/RAND_MAX;
			//描画位置
			caec_model.Centerbegin.set(32,0,0,0);
			caec_model.Centerend.set(32,0,128,0);
			caec_model.Centerbegin.turn(d);
			caec_model.Centerend.turn(d);

			//チップに追加
			pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
			caec->addAnimechip(pcac);
		}
		
	}
	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));
	
}
void EffectFunctions::パワーダウンエフェクト(const double placeX,const double placeY, double color_H)
{
	cColor color_c;
	color_c.HSV(color_H+180,0.9,0.9);
	unsigned int color,darkcolor;
	color = color_c.D3Dcolor();
	darkcolor = 0xFF000000;
	int i;

	pcaECustom caec;
	c4DVector center;
	pcAnimationChipManager_chip pcac;

	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->Init(caec);
	caec->place.set(placeX,placeY,0,0);
	
	//レイヤー２
	caec->LayerLevel = LAYERLEVEL_NORMAL;
	
	caEchip_Basic caec_model;

	caec_model.AbsolutePlaceFlag = true;
	cCoordinate coo(placeX,placeY);
	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("powerdown.wav"),coo);

	{//わっか

		//テクスチャセット
		caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ring_out.png")));

		//テクスチャ座標セット
		caec_model.DO.m_TexRange.setLTRB(0, 0, 1, 1);
		caec_model.texRange.clear();
		//c4DVector tex;
		//tex.set(0/4.0, 0, 1/4.0, 1);
		//caec_model.texRange.setValue(3,tex);
		//tex.set(1/4.0, 0, 2/4.0, 1);
		//caec_model.texRange.setValue(3,tex);

		//基本サイズ
		caec_model.DO.Width = (64);
		caec_model.DO.Height = (64)-16;

		caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeX,placeY,0,0);

		//描画位置
		caec_model.Centerbegin.set(0,0,128,0);
		caec_model.Centerend.set(0,0,0,0);

		//ムーブパス
		//caec_model.Movepass.set(0,1,0.5,1);
		
		//シェイプオプション
		caec_model.ScaleX.set(1,1,1,1);
		caec_model.ScaleY.set(1,1,1,1);
		//caec_model.Rotation.set(0,0,0,0);
		//double d = rand()*360/RAND_MAX;
		//caec_model.Rotation2.set(d,d+(rand()*60-30)/RAND_MAX,1,1);


		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_SUBSTRACTION;

		//色
		caec_model.ColorBase.inputD3Dcolor(color);
		caec_model.Alpha.set(0,2,2,0);


		for(i=0;i<3;i++)
		{
			//表示時間
			caec_model.life = 10;
			caec_model.hidingTime = 0+i*5;	

			//チップに追加
			pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
			caec->addAnimechip(pcac);
		}
	}
	{//放射

		//テクスチャセット
		caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ring_in.png")));

		//テクスチャ座標セット
		caec_model.DO.m_TexRange.setLTRB(0, 0, 1, 1);
		caec_model.texRange.clear();
		//c4DVector tex;
		//tex.set(0/4.0, 0, 1/4.0, 1);
		//caec_model.texRange.setValue(3,tex);
		//tex.set(1/4.0, 0, 2/4.0, 1);
		//caec_model.texRange.setValue(3,tex);

		//基本サイズ
		caec_model.DO.Width = (64);
		caec_model.DO.Height = (64);

		caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeX,placeY,0,0);

		//描画位置
		caec_model.Centerbegin.set(0,0,0,0);
		caec_model.Centerend.set(0,0,0,0);

		//ムーブパス
		//caec_model.Movepass.set(0,1,0.5,1);
		
		//シェイプオプション
		caec_model.ScaleX.set(3,0,1,1);
		caec_model.ScaleY.set(3,0,0.75,0.75);
		double d = rand()*360/RAND_MAX;
		caec_model.Rotation.set(d,d,1,1);


		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_SUBSTRACTION;

		//色
		caec_model.ColorBase.inputD3Dcolor(color);
		caec_model.Alpha.set(0,2,2,0);


		for(i=0;i<3;i++)
		{
			//表示時間
			caec_model.life = 10;
			caec_model.hidingTime = 0+i*5;	

			//チップに追加
			pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
			caec->addAnimechip(pcac);
		}
	}
	{//点々

		//テクスチャセット
		caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_simple.png")));

		//テクスチャ座標セット
		caec_model.DO.m_TexRange.setLTRB(0, 0, 1, 1);
		caec_model.texRange.clear();
		//c4DVector tex;
		//tex.set(0/4.0, 0, 1/4.0, 1);
		//caec_model.texRange.setValue(3,tex);
		//tex.set(1/4.0, 0, 2/4.0, 1);
		//caec_model.texRange.setValue(3,tex);

		//基本サイズ
		caec_model.DO.Width = (4);
		caec_model.DO.Height = (16);

		caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeX,placeY,0,0);

		//描画位置
		caec_model.Centerbegin.set(0,0,0,0);
		caec_model.Centerend.set(0,0,128,0);

		//ムーブパス
		caec_model.Movepass.set(0,1,0.5,1);
		
		//シェイプオプション
		caec_model.ScaleX.set(1,1,1,1);
		caec_model.ScaleY.set(1,1,1,1);
		caec_model.Rotation.set(0,0,0,0);
		//double d = rand()*360/RAND_MAX;
		caec_model.Rotation2.set(0,0,1,1);


		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_SUBSTRACTION;

		//色
		caec_model.ColorBase.inputD3Dcolor(color);
		caec_model.Alpha.set(0,2,2,0);


		for(i=0;i<10;i++)
		{
			//表示時間
			caec_model.life = 10;
			caec_model.hidingTime = 0+i*2;	
			double d;
			d = rand()*360/RAND_MAX;
			//描画位置
			caec_model.Centerbegin.set(32,0,128,0);
			caec_model.Centerend.set(32,0,0,0);
			caec_model.Centerbegin.turn(d);
			caec_model.Centerend.turn(d);

			//チップに追加
			pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
			caec->addAnimechip(pcac);
		}
		
	}
	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));
	
}


void EffectFunctions::Mob29エフェクト(const double placeX,const double placeY)
{
	unsigned int color,darkcolor;
	color = 0xFFFFFFFF;
	darkcolor = 0xFF000000;
	int i;

	pcaECustom caec;
	c4DVector center;
	pcAnimationChipManager_chip pcac;

	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->Init(caec);
	caec->place.set(placeX,placeY,0,0);
	
	//レイヤー２
	caec->LayerLevel = LAYERLEVEL_NORMAL;
	
	caEchip_Basic caec_model;

	caec_model.AbsolutePlaceFlag = true;
	cCoordinate coo(placeX,placeY);
	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("mob29.wav"),coo);

	{//カミナリ

		//テクスチャセット
		caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ring_out.png")));

		//テクスチャ座標セット
		caec_model.DO.m_TexRange.setLTRB(0, 0, 1, 1);
		caec_model.texRange.clear();
		//c4DVector tex;
		//tex.set(0/4.0, 0, 1/4.0, 1);
		//caec_model.texRange.setValue(3,tex);
		//tex.set(1/4.0, 0, 2/4.0, 1);
		//caec_model.texRange.setValue(3,tex);

		//基本サイズ
		caec_model.DO.Width = (64*3)+32;
		caec_model.DO.Height = (64*3)+32;

		caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeX,placeY,0,0);

		//描画位置
		caec_model.Centerbegin.set(0,0,0,0);
		caec_model.Centerend.set(0,0,0,0);

		//ムーブパス
		//caec_model.Movepass.set(0,1,0.5,1);
		
		//シェイプオプション
		caec_model.ScaleX.set(0,1,1,1);
		caec_model.ScaleY.set(0,1,1,1);
		//caec_model.Rotation.set(0,0,0,0);
		//double d = rand()*360/RAND_MAX;
		//caec_model.Rotation2.set(d,d+(rand()*60-30)/RAND_MAX,1,1);


		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

		//色
		caec_model.ColorBase.inputD3Dcolor(color);
		caec_model.Alpha.set(1,0,1,1);


		for(i=0;i<3;i++)
		{
			//表示時間
			caec_model.life = 6;
			caec_model.hidingTime = 0+i*3;	

			//チップに追加
			pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
			caec->addAnimechip(pcac);
		}
	}
	{//いわ？
		caEchip_Bound caec_model_bound;
		//テクスチャセット
		caec_model_bound.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\star.png")));

		//テクスチャ座標セット
		caec_model_bound.DO.m_TexRange.setLTRB(0, 0, 1, 1);
		caec_model_bound.texRange.clear();

		//基本サイズ
		caec_model_bound.DO.Width = (32);
		caec_model_bound.DO.Height = (32);

		caec_model_bound.AbsolutePlaceFlag = 0;

		//開始位置
		caec_model_bound.Centerbegin.set(0,0,64,0);
		


		//加算
		caec_model_bound.DO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;


		//色
		caec_model_bound.ColorBase.inputD3Dcolor(0xFF775544);
		caec_model_bound.Alpha.set(0,2,2,0);


		
		//表示時間
		caec_model_bound.life = 12;
		caec_model_bound.hidingTime = 0;	

		int i;
		for(i=0;i<8;i++)
		{
			//シェイプオプション
			//caec_model_bound.ScaleX.set(1,1,1,1);
			//caec_model_bound.ScaleY.set(1,1,1,1);
			//caec_model_bound.Rotation.set(0,0,0,0);
			double d = rand()*360/RAND_MAX;
			caec_model_bound.Rotation2.set(d,d+(rand()*60-30)/RAND_MAX,1,1);


			//方向
			caec_model_bound.moveVector.set(12,0,4,0);
			d = rand()*360/RAND_MAX;
			caec_model_bound.moveVector.turn(d);

			//チップに追加
			pcac = pcAnimationChipManager_chip(new caEchip_Bound(caec_model_bound));
			caec->addAnimechip(pcac);
		}
	}
	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));
	
}


void EffectFunctions::Mob31エフェクト(const double placeX,const double placeY)
{
	unsigned int color,darkcolor;
	color = 0xFF9944DD;
	darkcolor = 0xFF000000;
	int i;

	pcaECustom caec;
	c4DVector center;
	pcAnimationChipManager_chip pcac;

	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->Init(caec);
	caec->place.set(placeX,placeY,0,0);
	
	//レイヤー２
	caec->LayerLevel = LAYERLEVEL_NORMAL;
	
	caEchip_Basic caec_model;

	caec_model.AbsolutePlaceFlag = true;
	cCoordinate coo(placeX,placeY);
	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("mob31.wav"),coo);

	{//もわ

		//テクスチャセット
		caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_smoke.png")));

		//テクスチャ座標セット
		caec_model.DO.m_TexRange.setLTRB(0, 0, 1, 1);
		caec_model.texRange.clear();
		//c4DVector tex;
		//tex.set(0/4.0, 0, 1/4.0, 1);
		//caec_model.texRange.setValue(3,tex);
		//tex.set(1/4.0, 0, 2/4.0, 1);
		//caec_model.texRange.setValue(3,tex);

		//基本サイズ
		caec_model.DO.Width = (64);
		caec_model.DO.Height = (64);

		caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(placeX,placeY,0,0);



		//ムーブパス
		//caec_model.Movepass.set(0,1,0.5,1);
		
		//シェイプオプション
		caec_model.ScaleX.set(0,2,2,0);
		caec_model.ScaleY.set(0,2,2,0);

		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;

		//色
		caec_model.ColorBase.inputD3Dcolor(color);
		caec_model.Alpha.set(0,2,2,0);


		for(i=0;i<8;i++)
		{
			//描画位置
			caec_model.Centerbegin.set(rand()*60/RAND_MAX-30, rand()*60/RAND_MAX-30,0,0);
			caec_model.Centerend.set(rand()*60/RAND_MAX-30, -64+rand()*60/RAND_MAX-30,0,0);

			//回転
			//caec_model.Rotation.set(0,0,0,0);
			double d = rand()*360/RAND_MAX;
			caec_model.Rotation2.set(d,d+(rand()*60-30)/RAND_MAX,1,1);

			//表示時間
			caec_model.life = 30;
			caec_model.hidingTime = 0+i*3;	

			//チップに追加
			pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
			caec->addAnimechip(pcac);
		}
	}
	
	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));
	
}

void EffectFunctions::弾幕体当たり(const double placeX,const double placeY, pcCharacter pchara, const int aspect)
{

	cValiableField val;
	double temp;
	val.doubles.dim(tstring(_T("PlaceX")),temp = pchara->placeX);
	val.doubles.dim(tstring(_T("PlaceY")),temp = pchara->placeY);
	cCoordinate coo;
	coo.SetAspect(aspect);
	val.doubles.dim(tstring(_T("toPlaceX")),temp = pchara->placeX + coo.x);
	val.doubles.dim(tstring(_T("toPlaceY")),temp = pchara->placeY + coo.y);
	val.doubles.dim(tstring(_T("direction")),temp = pchara->aspect*45);
	val.doubles.dim(tstring(_T("aspect")),temp = (double)aspect);
	val.charas.dim(tstring(_T("player")),pchara);


	bool didAnime = AnimationScript_AddAnimation(
		sg_pDungeonSystem->DataBase.EffectImport_Other(),
		_T("EffectDanmakuAttack.lua"),
		val);	

	/*
	pcCharacter pchara = sg_pDungeonSystem->DataBase.GetSampleCharacter(charaID);
	if(pchara == NULL) return;

	pcaECustom caec;
	pcAnimationChipManager_chip pcac;
	caec = pcaECustom(new caECustom);
	//レイヤー２
	caec->LayerLevel = LAYERLEVEL_NORMAL;

	//描画位置通知
	caec->Init(caec);
	caec->place.set(placeX,placeY,0,0);
	caEchip_Basic caec_model;

	TCHAR path[128] = _T("");
	_stprintf(path,_T("chara\\dot\\%sdot.png"),pchara->surfaceName().c_str());
	IDirect3DTexture9* m_pTexture = g_GameEnv.m_GlobalResourse->getTextureFromFile(sg_pDungeonSystem->pDevice_D3D,path);

	//テクスチャセット
	caec_model.DO.setTexture(m_pTexture);

	//テクスチャ座標セット
	caec_model.DO.m_TexRange.setLTRB(0, aspect/8.0, 0.25, (aspect+1)/8.0);
	caec_model.texRange.clear();

	//基本サイズ
	caec_model.DO.Width = (128);
	caec_model.DO.Height = (128);

	//描画位置
	cCoordinate aspectCoo;
	aspectCoo.SetAspect(aspect);
	caec_model.Centerend.set(aspectCoo.x*64,aspectCoo.y*64,40,0);
	caec_model.Centerbegin.set(0,0,40,0);
	caec_model.AbsolutePlaceFlag = 0;
	caec_model.place.set(placeX,placeY,0,0);
	
	//シェイプオプション
	caec_model.ScaleX.set(1,1,1,1);
	caec_model.ScaleY.set(1,1,1,1);

	//加算
	caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;

	//色
	caec_model.ColorBase.inputD3Dcolor(0xFFFFFFFF);
	caec_model.Alpha.set(0,2,2,0);

	//表示時間
	caec_model.life = 16;
	caec_model.hidingTime = 0;	

	//チップに追加
	pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
	caec->addAnimechip(pcac);

	
	sg_pDungeonSystem->AnimationManager().AddAnime_parallel(boost::static_pointer_cast<cAnimation>(caec));
	*/
}

void EffectFunctions::スタンプ(const double placeX, const double placeY, tstring fileName, const TextureRangeRect texRenge, cColor burnColor, const double size, const int duration, const int delay)
{
	cValiableField val;
	double temp;
	val.doubles.dim(tstring(_T("placeX")),temp = placeX);
	val.doubles.dim(tstring(_T("placeY")),temp = placeY);

	val.tstrings.dim(tstring(_T("fileName")),fileName);

	val.doubles.dim(tstring(_T("burnColor_D3Dcolor")),temp = burnColor.D3Dcolor());

	val.doubles.dim(tstring(_T("texRengeX1")),temp = texRenge.x);
	val.doubles.dim(tstring(_T("texRengeY1")),temp = texRenge.y);
	val.doubles.dim(tstring(_T("texRengeX2")),temp = texRenge.z);
	val.doubles.dim(tstring(_T("texRengeY2")),temp = texRenge.w);

	val.doubles.dim(tstring(_T("size")),temp = size);

	val.doubles.dim(tstring(_T("duration")),temp = duration);

	val.doubles.dim(tstring(_T("delay")),temp = delay);

	bool didAnime = AnimationScript_AddAnimation(
		sg_pDungeonSystem->DataBase.EffectImport_Other(),
		_T("EffectGeneralStamp.lua"),
		val);	
}

void EffectFunctions::霧(const double placeX, const double placeY, tstring fileName, const TextureRangeRect texRenge, cColor burnColor, const double size, const double generateRadius, const double generateRatio, const int duration, const int delay)
{
	cValiableField val;
	double temp;
	val.doubles.dim(tstring(_T("placeX")),temp = placeX);
	val.doubles.dim(tstring(_T("placeY")),temp = placeY);

	val.tstrings.dim(tstring(_T("fileName")),fileName);

	val.doubles.dim(tstring(_T("burnColor_D3Dcolor")),temp = burnColor.D3Dcolor());

	val.doubles.dim(tstring(_T("texRengeX1")),temp = texRenge.x);
	val.doubles.dim(tstring(_T("texRengeY1")),temp = texRenge.y);
	val.doubles.dim(tstring(_T("texRengeX2")),temp = texRenge.z);
	val.doubles.dim(tstring(_T("texRengeY2")),temp = texRenge.w);

	val.doubles.dim(tstring(_T("size")),temp = size);

	val.doubles.dim(tstring(_T("duration")),temp = duration);
	
	val.doubles.dim(tstring(_T("generateRadius")),temp = generateRadius);
	val.doubles.dim(tstring(_T("generateRatio")),temp = generateRatio);

	val.doubles.dim(tstring(_T("delay")),temp = delay);

	bool didAnime = AnimationScript_AddAnimation(
		sg_pDungeonSystem->DataBase.EffectImport_Other(),
		_T("EffectGeneralFog.lua"),
		val);	
}


void EffectFunctions::スペルエフェクト(const double placeX, const double placeY, const int ID, const SpellEffectPart_class::SpellEffectPart part)
{
	cValiableField val;
	double temp;
	val.doubles.dim(tstring(_T("placeX")),temp = placeX);
	val.doubles.dim(tstring(_T("placeY")),temp = placeY);

	val.doubles.dim(tstring(_T("part")),temp = part);


	bool didAnime = AnimationScript_AddAnimation(
		sg_pDungeonSystem->DataBase.EffectImport_Spell(),
		sg_pDungeonSystem->DataBase.EffectImport_Name_Spell(ID),
		val);	
}

