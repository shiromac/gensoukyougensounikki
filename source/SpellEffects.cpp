
#include "stdafx.h"

#include "SpellEffects.h"

#include "cAnimation.h"

#include "cDungeonSystem.h"
#include "caEchipS.h"
#include "caECustom.h"
#include "scriptprocesser/AnimationScript.h"

using namespace SpellEffects;

//こーりんの符
void SpellEffects::EffectcSpell_ID_0(const double placeX,const double placeY)
{
	pcaECustom caec;
	//c4DVector center;
	pcAnimationChipManager_chip pcac;

	int i;
	caEchip_Belt caec_b_model;

	caEchip_Basic caec_model;


	
	//------------------------------------------------
	//螺旋降り線
	//------------------------------------------------
	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->place.set(placeX,placeY,0,0);
	
	//レイヤー
	//caec->LayerLevel = LAYERLEVEL_EXTRAEFFECT1;
	


	//テクスチャセット
	caec_b_model.DOb.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
		sg_pDungeonSystem->pDevice_D3D,_T("effect\\belts.png")),SIZE_BASICTEX,SIZE_BASICTEX);

	//テクスチャ座標セット
	caec_b_model.DOb.m_TexRange.setLTRB(1,	8/16.0,
										0,	9/16.0);


	//描画基本位置
	//center.set(placeX,placeY,0,0);
	//center = sg_pDungeonSystem->Map().realvisibleplace(center);
	caec_b_model.AbsolutePlaceFlag = false;
	caec_b_model.place.set(placeX,placeY,0,0);




	//基本サイズ
	//caec_model.DO.Width = 128;
	//caec_model.DO.Height = 128;




	
	//加算
	caec_b_model.DOb.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

	//色
	caec_b_model.ColorBase.inputD3Dcolor(0xFFFFFFFF);
	caec_b_model.Alpha.set(0,2,2,0);


	
	//表示時間
	caec_b_model.life = 60;
	caec_b_model.hidingTime = 0;

	//逆順につき最大点
	int height = 1024;

	//描画位置原点
//	caec_b_model.CenterLeft = center;
//	caec_b_model.CenterRight = center;
	caec_b_model.CenterLeft.z = 32;
	caec_b_model.CenterRight.z = 32;
	

	int length = 30;
	int numth;

	for(numth=0;numth<16;numth++)
	{



		//表示時間
		caec_b_model.life = 30;
		caec_b_model.hidingTime = (90*(rand() + rand())/2)/RAND_MAX;

		//回転量基本値
		double s = 6.0*rand()/RAND_MAX-3;		
		
		//ライン定義
		caec_b_model.DOb.rightline().deleteCurveTrack();
		caec_b_model.DOb.leftline().deleteCurveTrack();
		for(i=1;i<length;i++)
		{
			c4DVector vl,vr;
			vl.set(30,0,0,0);
			vl *=(0.5)*i/length +1;

			//上に
			vl.z += height*(i)/length;


			vr = vl;

			//回転
			vl.turn(-10*i*s +numth*360/8);
			vr.turn(-10*i*s +numth*360/8+30);



			//俯瞰を出す
			vl.y *= 0.75;
			vr.y *= 0.75;

			caec_b_model.DOb.rightline().addPoint(vr);
			caec_b_model.DOb.leftline().addPoint(vl);
		}
		caec_b_model.DOb.rightline().stabilizeTrack();
		caec_b_model.DOb.leftline().stabilizeTrack();


		//ドラウイングレンジ//逆順
		caec_b_model.Startp.set(length-20,0 ,1,0.4);
		caec_b_model.Endp.set  (length   ,20,1,0.4);



		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Belt(caec_b_model));
		caec->addAnimechip(pcac);
	}

	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));


	//------------------------------------------------
	//上に出す光
	//------------------------------------------------
	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->place.set(placeX,placeY,0,0);
	
	//レイヤー
	caec->LayerLevel = LAYERLEVEL_EXTRAEFFECT1;
	


	//テクスチャセット
	caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
		sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_simple.png")),SIZE_BASICTEX,SIZE_BASICTEX);

	//テクスチャ座標セット
	caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


	//描画基本位置
	//center.set(placeX, placeY, 0, 0);
	//center = sg_pDungeonSystem->Map().realvisibleplace(center);
	//描画位置
	caec_model.AbsolutePlaceFlag = false;
	caec_model.place.set(placeX,placeY,0,0);
	//caec_model.Centerbegin = center;
	caec_model.Centerbegin.y = -256;
	caec_model.Centerend.y = -256;

	//caec_model.Centerend = caec_model.Centerbegin;


	//基本サイズ
	caec_model.DO.Width = 32;
	caec_model.DO.Height = 512;


	//シェイプオプション
	caec_model.ScaleX.set(1,1,1,1);
	caec_model.ScaleY.set(1,1,1,1);
	//caec_model.Rotation.set(rand()*360/RAND_MAX, rand()*360/RAND_MAX + 360*4,1,1.5);
	//caec_model.Rotation2

	
	//加算
	caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

	//色
	caec_model.ColorBase.inputD3Dcolor(0xFFFFFFFF);
	caec_model.Alpha.set(0,1,2,0);


	//表示時間
	caec_model.life = 100;
	caec_model.hidingTime = 0;	


	//チップに追加
	pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
	caec->addAnimechip(pcac);


	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));


	//------------------------------------------------
	//ピカッ
	//------------------------------------------------
	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->place.set(placeX,placeY,0,0);
	
	//レイヤー
	caec->LayerLevel = LAYERLEVEL_EXTRAEFFECT1;
	


	//テクスチャセット
	caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
		sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_simple.png")),SIZE_BASICTEX,SIZE_BASICTEX);

	//テクスチャ座標セット
	caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


	//描画基本位置
	//center.set(placeX, placeY, 0, 0);
	//center = sg_pDungeonSystem->Map().realvisibleplace(center);
	caec_model.AbsolutePlaceFlag = false;
	caec_model.place.set(placeX,placeY,0,0);
	//描画位置
	caec_model.Centerbegin.y = -32;
	caec_model.Centerend.y = -32;

	//基本サイズ
	caec_model.DO.Width = 128;
	caec_model.DO.Height = 128;


	//シェイプオプション
	caec_model.ScaleX.set(1,0,1,0);
	caec_model.ScaleY.set(1,2,1,2);
	//caec_model.Rotation.set(rand()*360/RAND_MAX, rand()*360/RAND_MAX + 360*4,1,1.5);
	//caec_model.Rotation2

	
	//加算
	caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

	//色
	caec_model.ColorBase.inputD3Dcolor(0xFFFFFFFF);
	caec_model.Alpha.set(0,2,2,0);





	//表示時間
	caec_model.life = 6;
	caec_model.hidingTime = 0;	


	//チップに追加
	pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
	caec->addAnimechip(pcac);


	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));


	
	//------------------------------------------------
	//ピカッ
	//------------------------------------------------
	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->place.set(placeX,placeY,0,0);
	
	//レイヤー
	caec->LayerLevel = LAYERLEVEL_EXTRAEFFECT1;
	


	//テクスチャセット
	caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
		sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_simple.png")),SIZE_BASICTEX,SIZE_BASICTEX);

	//テクスチャ座標セット
	caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


	//描画基本位置
	//center.set(placeX, placeY, 0, 0);
	//center = sg_pDungeonSystem->Map().realvisibleplace(center);
	caec_model.AbsolutePlaceFlag = false;
	caec_model.place.set(placeX,placeY,0,0);
	//描画位置
	caec_model.Centerbegin.y = -32;
	caec_model.Centerend.y = -32;

	//基本サイズ
	caec_model.DO.Width = 128;
	caec_model.DO.Height = 128;


	//シェイプオプション
	caec_model.ScaleX.set(1,6,1,2);
	caec_model.ScaleY.set(1,0,1,0);
	//caec_model.Rotation.set(rand()*360/RAND_MAX, rand()*360/RAND_MAX + 360*4,1,1.5);
	//caec_model.Rotation2

	
	//加算
	caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

	//色
	caec_model.ColorBase.inputD3Dcolor(0xFFFFFFFF);
	caec_model.Alpha.set(0,2,2,0);






	//表示時間
	caec_model.life = 6;
	caec_model.hidingTime = 0;	


	//チップに追加
	pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
	caec->addAnimechip(pcac);


	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));


	

}

void SpellEffects::EffectcSpell_ID_1(const double placeX,const double placeY)
{
	pcaECustom caec;
	//c4DVector center;
	pcAnimationChipManager_chip pcac;

	int i;

	sg_pDungeonSystem->AnimationManager().AddAnime_parallel(pcAnimation(new caESpell_ID_1(200)));


	caEchip_Basic caec_model_egg, caec_model_light;
	
	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->place.set(placeX,placeY,0,0);

	//------------------------------------------------
	//ベースブラック
	//------------------------------------------------
		//テクスチャセット
	caec_model_light.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
		sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_simple.png")),SIZE_BASICTEX,SIZE_BASICTEX);
	//テクスチャ座標セット
	caec_model_light.DO.m_TexRange.setLTRB(0,0,1,1);
	//描画基本位置
	//center.set(placeX, placeY, 0, 0);
	//center = sg_pDungeonSystem->Map().realvisibleplace(center);
	//基本サイズ
	caec_model_light.DO.Width = 1024;
	caec_model_light.DO.Height = 1024;
	//シェイプオプション
	caec_model_light.ScaleX.set(1,1,1,1);
	caec_model_light.ScaleY.set(1,1,1,1);
	caec_model_light.Rotation.set(rand()*360/RAND_MAX, rand()*360/RAND_MAX + 360*4,1,1.5);
	//caec_model_light.Rotation2
	//加算
	caec_model_light.DO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;
	//色
	caec_model_light.ColorBase.inputD3Dcolor(0xFF000000);
	caec_model_light.Alpha.set(0,1,1,0);
	//場所
	//caec_model_light.Centerend = caec_model_light.Centerbegin = center;
	caec_model_light.AbsolutePlaceFlag = false;
	caec_model_light.place.set(placeX,placeY,0,0);
	//表示時間
	caec_model_light.life = 120;
	caec_model_light.hidingTime = 0;	
	caec->addAnimechip(pcAnimationChipManager_chip(new caEchip_Basic(caec_model_light)));
	//------------------------------------------------
	//上に出す光
	//------------------------------------------------
		//テクスチャセット
	caec_model_light.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
		sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_simple.png")),SIZE_BASICTEX,SIZE_BASICTEX);
	//テクスチャ座標セット
	caec_model_light.DO.m_TexRange.setLTRB(0,0,1,1);
	//描画基本位置
	//center.set(placeX, placeY, 0, 0);
	//center = sg_pDungeonSystem->Map().realvisibleplace(center);
	caec_model_light.AbsolutePlaceFlag = false;
	caec_model_light.place.set(placeX,placeY,0,0);
	//基本サイズ
	caec_model_light.DO.Width = 512;
	caec_model_light.DO.Height = 512;
	//シェイプオプション
	caec_model_light.ScaleX.set(0.2,2,2,0);
	caec_model_light.ScaleY.set(0.2,2,2,0);
	caec_model_light.Rotation.set(rand()*360/RAND_MAX, rand()*360/RAND_MAX + 360*4,1,1.5);
	//caec_model_light.Rotation2
	//加算
	caec_model_light.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;
	//色
	caec_model_light.ColorBase.inputD3Dcolor(0xFFCCFF88);
	caec_model_light.Alpha.set(0,1,1,0);


	//表示時間
	caec_model_light.life = 40;
	caec_model_light.hidingTime = 0;	
	//------------------------------------------------
	//卵
	//------------------------------------------------

	//テクスチャセット
	caec_model_egg.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
		sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_simple.png")),SIZE_BASICTEX,SIZE_BASICTEX);
	//テクスチャ座標セット
	caec_model_egg.DO.m_TexRange.setLTRB(0,	0,
									1,	1);
	//描画基本位置
	//center.set(placeX,placeY,0,0);
	//center = sg_pDungeonSystem->Map().realvisibleplace(center);
	caec_model_egg.AbsolutePlaceFlag = false;
	caec_model_egg.place.set(placeX,placeY,0,0);
	//基本サイズ
	caec_model_egg.DO.Width = 24;
	caec_model_egg.DO.Height = 24;
	//加算
	caec_model_egg.DO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;
	//色
	caec_model_egg.ColorBase.inputD3Dcolor(0xFF66AA44);
	caec_model_egg.Alpha.set(0,.8,.8,0);
	//軌跡
	caec_model_egg.Movepass.set(0,1,2,1);
	//表示時間
	caec_model_egg.life = 80;
	caec_model_egg.hidingTime = 0;
	//逆順につき最大点
	int num = 8*6;
	//描画位置原点
	for(i=0;i<num;i++)
	{
		caec_model_egg.hidingTime = (i%8)*40/8;

		c4DVector v;
		v.set(200,0,0,0);
		v.turn(i*360/num);
		caec_model_egg.AbsolutePlaceFlag = false;
		caec_model_egg.place.set(placeX,placeY,0,0);
		caec_model_egg.Centerend = v;

		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model_egg));
		caec->addAnimechip(pcac);


	}
	//基本サイズ
	caec_model_egg.DO.Width = 8;
	caec_model_egg.DO.Height = 8;
	//加算
	caec_model_egg.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;
	//色
	caec_model_egg.ColorBase.inputD3Dcolor(0xFFCCFFAA);
	caec_model_egg.Alpha.set(0,2,2,0);
	//描画位置原点
	for(i=0;i<num;i++)
	{
		caec_model_egg.hidingTime = (i%8)*40/8;

		c4DVector v;
		v.set(200,0,0,0);
		v.turn(i*360/num);
		
		caec_model_egg.AbsolutePlaceFlag = false;
		caec_model_egg.place.set(placeX,placeY,0,0);
		caec_model_egg.Centerend = v;

		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model_egg));
		caec->addAnimechip(pcac);


		//描画位置
		caec_model_light.Centerbegin = caec_model_egg.Centerend;
		caec_model_light.Centerend = caec_model_light.Centerbegin;
		pcac->addNextchip(pcAnimationChipManager_chip(new caEchip_Basic(caec_model_light)));
	}
	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));

	/*
	//------------------------------------------------
	//上に出す光
	//------------------------------------------------
	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->place.set(placeX,placeY,0,0);
	
	//レイヤー
	caec->LayerLevel = LAYERLEVEL_EXTRAEFFECT1;
	


	//テクスチャセット
	caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
		sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_simple.png")),SIZE_BASICTEX,SIZE_BASICTEX);

	//テクスチャ座標セット
	caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


	//描画基本位置
	center.set(placeX, placeY, 0, 0);
	//center = sg_pDungeonSystem->Map().realvisibleplace(center);



	//基本サイズ
	caec_model.DO.Width = 1024;
	caec_model.DO.Height = 1024;


	//シェイプオプション
	caec_model.ScaleX.set(0.5,1.5,1.5,0.5);
	caec_model.ScaleY.set(0.5,1.5,1.5,0.5);
	caec_model.Rotation.set(rand()*360/RAND_MAX, rand()*360/RAND_MAX + 360*4,1,1.5);
	//caec_model.Rotation2

	
	//加算
	caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;

	//色
	caec_model.ColorBase.inputD3Dcolor(0xFF000000);
	caec_model.Alpha.set(0,1,1,0);



	//描画位置

	caec_model.Centerbegin = center;

	caec_model.Centerend = caec_model.Centerbegin;

	//表示時間
	caec_model.life = 150;
	caec_model.hidingTime = 30;	
	//チップに追加
	pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
	caec->addAnimechip(pcac);

	//加算
	caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

	//表示時間
	caec_model.life = 120;
	caec_model.hidingTime = 60;	

	//色
	caec_model.ColorBase.inputD3Dcolor(0xFFAABB88);
	caec_model.Alpha.set(0,2,2,0);

	//チップに追加
	pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
	caec->addAnimechip(pcac);
	//3重

	caec_model.Rotation.set(rand()*360/RAND_MAX, rand()*360/RAND_MAX + 360*4,1,1.5);

	pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
	caec->addAnimechip(pcac);

	caec_model.Rotation.set(rand()*360/RAND_MAX, rand()*360/RAND_MAX + 360*4,1,1.5);

	pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
	caec->addAnimechip(pcac);

	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));
	*/
	/*
	pcaECustom caec;
	c4DVector center;
	pcAnimationChipManager_chip pcac;

	int i;


	caEchip_Basic caec_model;


	
	//------------------------------------------------
	//卵
	//------------------------------------------------
	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->place.set(placeX,placeY,0,0);
	
	//レイヤー
	//caec->LayerLevel = LAYERLEVEL_EXTRAEFFECT2;
	


	//テクスチャセット
	caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
		sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_simple.png")),SIZE_BASICTEX,SIZE_BASICTEX);

	//テクスチャ座標セット
	caec_model.DO.m_TexRange.setLTRB(0,	0,
									1,	1);


	//描画基本位置
	center.set(placeX,placeY,0,0);
	//center = sg_pDungeonSystem->Map().realvisibleplace(center);



	//基本サイズ
	caec_model.DO.Width = 8;
	caec_model.DO.Height = 8;



	//加算
	caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

	//色
	caec_model.ColorBase.inputD3Dcolor(0xFFFFFFCC);
	caec_model.Alpha.set(0,2,2,0);

	caec_model.Movepass.set(0,1,2,1);


	
	//表示時間
	caec_model.life = 60;
	caec_model.hidingTime = 0;

	//逆順につき最大点
	int num = 8*6;

	//描画位置原点
	for(i=0;i<num;i++)
	{
		caec_model.hidingTime = (i%8)*30/8;

		c4DVector v;
		v.set(200,0,0,0);
		v.turn(i*360/num);
		caec_model.Centerbegin = center;

		caec_model.Centerend = caec_model.Centerbegin +v;

		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
		caec->addAnimechip(pcac);
		//二重
		pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
		caec->addAnimechip(pcac);
	}
	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));

	//------------------------------------------------
	//上に出す光
	//------------------------------------------------
	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->place.set(placeX,placeY,0,0);
	
	//レイヤー
	caec->LayerLevel = LAYERLEVEL_EXTRAEFFECT1;
	


	//テクスチャセット
	caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
		sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_simple.png")),SIZE_BASICTEX,SIZE_BASICTEX);

	//テクスチャ座標セット
	caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


	//描画基本位置
	center.set(placeX, placeY, 0, 0);
	//center = sg_pDungeonSystem->Map().realvisibleplace(center);



	//基本サイズ
	caec_model.DO.Width = 1024;
	caec_model.DO.Height = 1024;


	//シェイプオプション
	caec_model.ScaleX.set(0.5,1.5,1.5,0.5);
	caec_model.ScaleY.set(0.5,1.5,1.5,0.5);
	caec_model.Rotation.set(rand()*360/RAND_MAX, rand()*360/RAND_MAX + 360*4,1,1.5);
	//caec_model.Rotation2

	
	//加算
	caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;

	//色
	caec_model.ColorBase.inputD3Dcolor(0xFF000000);
	caec_model.Alpha.set(0,1,1,0);



	//描画位置

	caec_model.Centerbegin = center;

	caec_model.Centerend = caec_model.Centerbegin;

	//表示時間
	caec_model.life = 150;
	caec_model.hidingTime = 30;	
	//チップに追加
	pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
	caec->addAnimechip(pcac);

	//加算
	caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

	//表示時間
	caec_model.life = 120;
	caec_model.hidingTime = 60;	

	//色
	caec_model.ColorBase.inputD3Dcolor(0xFFAABB88);
	caec_model.Alpha.set(0,2,2,0);

	//チップに追加
	pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
	caec->addAnimechip(pcac);
	//3重

	caec_model.Rotation.set(rand()*360/RAND_MAX, rand()*360/RAND_MAX + 360*4,1,1.5);

	pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
	caec->addAnimechip(pcac);

	caec_model.Rotation.set(rand()*360/RAND_MAX, rand()*360/RAND_MAX + 360*4,1,1.5);

	pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
	caec->addAnimechip(pcac);

	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));
	*/

}

caESpell_ID_1::caESpell_ID_1(int num)
{

	End = false;
	preprocessed = 0;
	num_ = num;
}
caESpell_ID_1::~caESpell_ID_1(void)
{
}
int caESpell_ID_1::Init()
{
	return true;
}
int caESpell_ID_1::preprocess(IDirect3DDevice9 *pDev)
{

	fireflys.resize(num_);

	int i;
	int size = fireflys.size();

	for(i=0;i<size;i++)
	{
		fireflys[i].flag = 0;
		fireflys[i].position.set((double)rand()*MAPWIDTH/RAND_MAX,(double)rand()*MAPHEIGHT/RAND_MAX,0.5,0);
		fireflys[i].angle = (double)rand()*360/RAND_MAX;
		fireflys[i].acangle = 0;
		fireflys[i].lightpower = 200;
		fireflys[i].cicle = 1;
		fireflys[i].life = fireflys[i].lifecount = (double)rand()*60*30/RAND_MAX + 60*30;
	}
	life = 60*60;
	
	return true;
}

int caESpell_ID_1::process(IDirect3DDevice9 *pDev)
{
	if(preprocessed==0)
	{//初回
		preprocess(pDev);
		preprocessed = 1;
	}


	int i;
	int size = fireflys.size();

	for(i=0;i<size;i++)
	{
		if(fireflys[i].flag == 0)
		{//move
			fireflys[i].acangle += (double)rand()*0.5/RAND_MAX-0.25;
			if(fireflys[i].acangle > 2) fireflys[i].acangle = 2;
			if(fireflys[i].acangle < -2) fireflys[i].acangle = -2;
			fireflys[i].angle += fireflys[i].acangle;

			c4DVector v(0.015,0,0,0);
			v.turn(fireflys[i].angle);
			fireflys[i].position += v;



			if((double)rand()/RAND_MAX < 0.003)
			{
				fireflys[i].flag = 1;
				fireflys[i].flag2 = (sg_pDungeonSystem->Map().Land(fireflys[i].position.x,fireflys[i].position.y)->pOnChar != NULL);
			}

		}
		else
		{
		
			if((double)rand()/RAND_MAX < 0.002 ||
				fireflys[i].flag2 != (sg_pDungeonSystem->Map().Land(fireflys[i].position.x,fireflys[i].position.y)->pOnChar != NULL)
				)
			{
				fireflys[i].flag = 0;
				fireflys[i].flag2 = (sg_pDungeonSystem->Map().Land(fireflys[i].position.x,fireflys[i].position.y)->pOnChar != NULL);
			}
		}

		fireflys[i].cicle += (double)rand()*0.2/RAND_MAX-0.1;
		if(fireflys[i].cicle < 1) fireflys[i].cicle = 1;
		if(fireflys[i].cicle > 2) fireflys[i].cicle = 2;
		
		fireflys[i].lightpower += fireflys[i].cicle;

		fireflys[i].lifecount--;
	}


	if(life <= 0)
	{//オワリ
		End = true;
	}
	life--;
	return true;
}

int caESpell_ID_1::Draw(IDirect3DDevice9 *pDev)
{
	int i;
	int size = fireflys.size();

	cDrawingObject DO;
	c4DVector center;
	//テクスチャセット
	DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
		sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_simple.png")),SIZE_BASICTEX,SIZE_BASICTEX);
	//テクスチャ座標セット
	DO.m_TexRange.setLTRB(0,0,1,1);
	for(i=0;i<size;i++)
	{
		if(fireflys[i].lifecount < 0) continue;
		double d = ((double)fireflys[i].lifecount)/fireflys[i].life;
		d = ((1-d)*2+d*1)*(d*1);


		d *= (sin(fireflys[i].lightpower/60)+0.5)/1.5;

		//描画基本位置
		center = fireflys[i].position;
		if(!sg_pDungeonSystem->Map().isOnDisplay(center.x, center.y, 1)) continue;

		center = sg_pDungeonSystem->Map().visibleplace(center);
		
		DO.CenterX = center.x;
		DO.CenterY = center.y;


		DO.Width = DO.Height = 32;
		DO.m_color.ARGB(128*d,64,96,32);
		DO.Draw(pDev);
		DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

		DO.m_color.ARGB(96*d,128,168,32);
		DO.Width = DO.Height = 32;
		DO.Draw(pDev);

		DO.m_color.ARGB(128*d,128,168,64);
		DO.Width = DO.Height = 8;
		DO.Draw(pDev);

	}
	return true;
}
//マリサの符
void SpellEffects::EffectcSpell_ID_4(const double placeX,const double placeY, const int aspect)
{
	pcaECustom caec;
	//c4DVector center;
	pcAnimationChipManager_chip pcac;
	cCoordinate coo;

	int i;
	caEchip_Belt caec_b_model;

	caEchip_Basic caec_model;

	
	//------------------------------------------------
	//螺旋効果線
	//------------------------------------------------
	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->place.set(placeX,placeY,0,0);
	
	//レイヤー
	caec->LayerLevel = LAYERLEVEL_NORMAL;
	


	//テクスチャセット
	caec_b_model.DOb.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
		sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_simple.png")),SIZE_BASICTEX,SIZE_BASICTEX);

	//テクスチャ座標セット
	caec_b_model.DOb.m_TexRange.setLTRB(1,	1,
										0,	0);


	//描画基本位置
	//center.set(placeX,placeY,0,0);
	//center = sg_pDungeonSystem->Map().realvisibleplace(center);
	caec_b_model.AbsolutePlaceFlag = false;
	caec_b_model.place.set(placeX,placeY,0,0);
	//描画位置原点
	caec_b_model.CenterLeft.z = 32;
	caec_b_model.CenterRight.z = 32;

	//基本サイズ
	//caec_model.DO.Width = 128;
	//caec_model.DO.Height = 128;

	
	//加算
	caec_b_model.DOb.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

	//色
	caec_b_model.ColorBase.inputD3Dcolor(0xFFFFFFFF);
	caec_b_model.Alpha.set(0.5,2,2,0);


	
	//表示時間
	caec_b_model.life = 60;
	caec_b_model.hidingTime = 0;

	//最大点
	int height = 64*10;


	

	int length = 30;
	int numth;

	unsigned int color;
	cColor c;


	for(numth=0;numth<256;numth++)
	{
		c.HSV(rand()*360/RAND_MAX,1.0,1.0);
		color = c.D3Dcolor();
		caec_b_model.ColorBase.inputD3Dcolor(color);

		//表示時間
		caec_b_model.life = 30;
		caec_b_model.hidingTime = (90*(rand() + rand())/2)/RAND_MAX;

		//回転量基本値
		double s = 4.0*rand()/RAND_MAX-2;		
		
		//ライン定義
		caec_b_model.DOb.rightline().deleteCurveTrack();
		caec_b_model.DOb.leftline().deleteCurveTrack();
		for(i=1;i<length;i++)
		{
			c4DVector vl,vr;
			vl.set(90.0*(1.0-(double)(i-length)*(i-length)*(i-length)*(i-length)/length/length/length/length),0,0,0);
			vl *=(0.5)*i/length +1;

			
			vl.y += height*(i)/length*(i)/length;


			vr = vl;

			//回転
			vl.turn(-10*i*s +numth*360/8,DIM_X,DIM_Z);
			vr.turn(-10*i*s +numth*360/8+80,DIM_X,DIM_Z);

			vl.turn(aspect*45+180);
			vr.turn(aspect*45+180);




			caec_b_model.DOb.rightline().addPoint(vr);
			caec_b_model.DOb.leftline().addPoint(vl);
		}
		caec_b_model.DOb.rightline().stabilizeTrack();
		caec_b_model.DOb.leftline().stabilizeTrack();


		//ドラウイングレンジ
		caec_b_model.Startp.set(0		,length-5 ,1,1);
		caec_b_model.Endp.set  (5   ,length ,1,1);



		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Belt(caec_b_model));
		caec->addAnimechip(pcac);
	}


	//------------------------------------------------
	//上に出す光
	//------------------------------------------------


	//テクスチャセット
	caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
		sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_simple.png")),SIZE_BASICTEX,SIZE_BASICTEX);

	//テクスチャ座標セット
	caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


	//描画基本位置
	//center.set(placeX, placeY, 0, 0);
	//center = sg_pDungeonSystem->Map().realvisibleplace(center);
	caec_model.AbsolutePlaceFlag = false;
	caec_model.place.set(placeX,placeY,0,0);
	//描画位置
	c4DVector v(caec_model.DO.Height/2 - 50,0,0,0);
	v.turn(45*aspect - 90);
	caec_model.Centerbegin = v;
	caec_model.Centerbegin.y -= 16;
	caec_model.Centerend = caec_model.Centerbegin;

	//基本サイズ
	caec_model.DO.Width = 64;
	caec_model.DO.Height = 64*10;


	//シェイプオプション
	caec_model.ScaleX.set(1,1,1,1);
	caec_model.ScaleY.set(1,1,1,1);
	//caec_model.Rotation.set(rand()*360/RAND_MAX, rand()*360/RAND_MAX + 360*4,1,1.5);
	caec_model.Rotation2.set(aspect*45,aspect*45,1,1);

	
	//加算
	caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

	//色
	caec_model.ColorBase.inputD3Dcolor(0xFFFFFFFF);
	caec_model.Alpha.set(0,1,2,0);



	//表示時間
	caec_model.life = 100;
	caec_model.hidingTime = 0;	


	//チップに追加
	pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
	caec->addAnimechip(pcac);




	//-----------------------------------------------
	//魔方陣
	//-----------------------------------------------

	//テクスチャセット
	caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
		sg_pDungeonSystem->pDevice_D3D,_T("effect\\magiccircle.png")),512,512);

	//テクスチャ座標セット
	caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


	//描画基本位置
	coo.SetAspect(aspect);
	//center.set(placeX ,placeY ,0,0);
	//center = sg_pDungeonSystem->Map().realvisibleplace(center);
	caec_model.AbsolutePlaceFlag = false;
	caec_model.place.set(placeX,placeY,0,0);



	//基本サイズ
	caec_model.DO.Width = 512;
	caec_model.DO.Height = 512;
	
	
	//シェイプオプション
	caec_model.ScaleX.set(1,1,1,1);
	caec_model.ScaleY.set(0.75,0.75,1,1);
	caec_model.Rotation.set(100,180,1,1);
	caec_model.Rotation2.set(aspect*45,aspect*45,1,1);



	//表示時間
	caec_model.life = 100;
	caec_model.hidingTime = 0;	
	
	//加算
	caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

	//色
	caec_model.ColorBase.inputD3Dcolor(0xFFFFFF88);
	caec_model.Alpha.set(0,2,1,0);


	
	//チップに追加
	pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
	caec->addAnimechip(pcac);



	//-----------------------------------------------
	//衝撃
	//-----------------------------------------------

	//テクスチャセット
	caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
		sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ring_radio.png")),SIZE_BASICTEX,SIZE_BASICTEX);

	
	//テクスチャ座標セット
	caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


	//描画基本位置
	coo.SetAspect(aspect);
	//center.set(placeX +coo.x*0.2,placeY +coo.y*0.2,0.25,0);
	//center = sg_pDungeonSystem->Map().realvisibleplace(center);
	caec_model.AbsolutePlaceFlag = false;
	caec_model.place.set(placeX +coo.x*0.2,placeY +coo.y*0.2,0.25,0);

	caec_model.Centerend.x = -coo.x*128;
	caec_model.Centerend.y = -coo.y*128;

	//基本サイズ
	caec_model.DO.Width = 256;
	caec_model.DO.Height = 256;

	
	//シェイプオプション
	caec_model.ScaleX.set(0.2,1,1,2);
	caec_model.ScaleY.set(0.1,1,0.5,1);
	double d = rand()/(double)RAND_MAX;
	caec_model.Rotation.set(d*360,d*360,1,1);
	caec_model.Rotation2.set(aspect*45,aspect*45,1,1);

	
	//加算
	caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

	//色
	caec_model.ColorBase.inputD3Dcolor(0xFFFFFF88);
	caec_model.Alpha.set(0,1,2,0);


	for(i=0;i<10;i++)
	{
		//表示時間
		caec_model.life = 10;
		caec_model.hidingTime = i*8+8+10;	
		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
		caec->addAnimechip(pcac);
	}

	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));



}
//ヨウムの符
void SpellEffects::EffectcSpell_ID_6(const double placeX,const double placeY, const int aspect)
{
	pcaECustom caec;
	c4DVector center;
	pcAnimationChipManager_chip pcac;
	cCoordinate coo;
	coo.SetAspect(aspect);

	int i;
	caEchip_Belt caec_b_model;

	caEchip_Basic caec_model;


	//------------------------------------------------
	//前に出す光
	//------------------------------------------------
	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->place.set(placeX,placeY,0,0);
	
	//レイヤー
	caec->LayerLevel = LAYERLEVEL_NORMAL;
	


	//テクスチャセット
	caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
		sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_simple.png")),SIZE_BASICTEX,SIZE_BASICTEX);

	//テクスチャ座標セット
	caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


	//描画基本位置
	//center.set(placeX, placeY, 0, 0);
	//center = sg_pDungeonSystem->Map().realvisibleplace(center);
	caec_model.AbsolutePlaceFlag = false;
	caec_model.place.set(placeX,placeY,0,0);


	//基本サイズ
	caec_model.DO.Width = 32;
	caec_model.DO.Height = 32;


	//シェイプオプション
	caec_model.ScaleX.set(1,1,1,1);
	caec_model.ScaleY.set(1,2,1,8);
	//caec_model.Rotation.set(rand()*360/RAND_MAX, rand()*360/RAND_MAX + 360*4,1,1.5);
	caec_model.Rotation2.set(aspect*45,aspect*45,1,1);

	
	//加算
	caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

	//色
	caec_model.ColorBase.inputD3Dcolor(0xFF88EEFF);
	caec_model.Alpha.set(0,2,2,0);



	//描画位置

	caec_model.Centerbegin.x = -coo.x*1000;
	caec_model.Centerbegin.y = -coo.y*1000;
	caec_model.Centerbegin.z = 0;

	caec_model.Centerend.x = coo.x*1000;
	caec_model.Centerend.y = coo.y*1000;
	caec_model.Centerend.z = 0;

	//表示時間
	caec_model.life = 20;
	caec_model.hidingTime = 10;	


	//チップに追加
	pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
	caec->addAnimechip(pcac);


	for(i=0;i<6;i++)
	{
		//------------------------------------------------
		//ズバ
		//------------------------------------------------


		//テクスチャセット
		caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
			sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ring_nagi2.png")),SIZE_BASICTEX,SIZE_BASICTEX);

		//テクスチャ座標セット
		caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


		//描画基本位置
		//center.set(placeX, placeY, 0, 0);
		//center = sg_pDungeonSystem->Map().realvisibleplace(center);
		caec_model.AbsolutePlaceFlag = false;
		caec_model.place.set(placeX,placeY,0,0);


		//基本サイズ
		caec_model.DO.Width = 64;
		caec_model.DO.Height = 64;


		//シェイプオプション
		caec_model.ScaleX.set(0,1,1,1);
		caec_model.ScaleY.set(0,1,5,5);
		double d = (double)rand()/RAND_MAX;
		caec_model.Rotation.set(d*360, d*360+150,1,1);
		d = (double)rand()/RAND_MAX;
		caec_model.Rotation2.set(d*360,d*360,1,1);

		
		//加算
		caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

		//色
		caec_model.ColorBase.inputD3Dcolor(0xFF88EEFF);
		caec_model.Alpha.set(0,2,2,0);



		//描画位置

		caec_model.Centerbegin.x = coo.x*64*i;
		caec_model.Centerbegin.y = coo.y*64*i;

		caec_model.Centerend = caec_model.Centerbegin;



		//表示時間
		caec_model.life = 10;
		caec_model.hidingTime = +10+i*3;	


		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
		caec->addAnimechip(pcac);


	}

	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));


}


//カグヤの符
void SpellEffects::EffectcSpell_ID_22(const double placeX,const double placeY, const int time)
{
	cValiableField val;
	double temp;
	val.doubles.dim(tstring(_T("PlaceX")),temp = placeX);
	val.doubles.dim(tstring(_T("PlaceY")),temp = placeY);
	val.doubles.dim(tstring(_T("time")),temp = time);

	bool didAnime = AnimationScript_AddAnimation(
		sg_pDungeonSystem->DataBase.EffectImport_Spell(),
		_T("EffectSpell3022.lua"),
		val);	

}
