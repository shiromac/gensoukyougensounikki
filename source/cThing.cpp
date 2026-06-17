#include "stdafx.h"
#include "cThing.h"

#include "GameIdiom.h"
void cThing_ID_0::DataBeginOptimize(int difficulty)
{
	cDroping::DataBeginOptimize(difficulty);

	discover();//常に見える
}
void cThing_ID_0::Init(cRenderDevice *pDev,pcDroping self)
{
	cDroping::Init(pDev, self);

	caEchip_Basic cacb_model;
	//テクスチャセット
	cacb_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
							pDev,_T("effect\\fire_chip.png")));

	//基本サイズ
	cacb_model.DO.Width = 40;
	cacb_model.DO.Height = 40;

	//加算
	cacb_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;

	//色
	cacb_model.ColorBase.ARGB(255,128,32,0);
	cacb_model.Alpha.set(0,2,2,0);

	//テクスチャ座標セット
	cacb_model.DO.m_TexRange.setLTRB(0/4.0,3/4.0,1/4.0,4/4.0);

	//色
	cacb_model.DO.m_color.ARGB(255,255,128,0);
	cacb_model.Alpha.set(0,2,2,0);


	//表示時間
	cacb_model.life = 20;
	cacb_model.hidingTime = 0;

	cacb_model.End = true;

	int i;
	for(i=0;i<6;i++)
	{
		flooranime.push_back(pcaEchip_Basic(new caEchip_Basic(cacb_model)));
	}
	cacb_model.ColorBase.ARGB(255,255,255,255);
	cacb_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;
	for(i=0;i<4;i++)
	{
		flooranime.push_back(pcaEchip_Basic(new caEchip_Basic(cacb_model)));
	}

	cacb_model.DO.Width = 28;
	cacb_model.DO.Height = 28;
	for(i=0;i<10;i++)
	{
		flameanime.push_back(pcaEchip_Basic(new caEchip_Basic(cacb_model)));
	}


	for(i=0;i<flooranime.size();i++)
	{
		
	}
	for(i=0;i<flameanime.size();i++)
	{
		
	}
}
void cThing_ID_0::Draw(cRenderDevice *pDev)
{
	if(!pri_onland_DrawFlag) return;

	cDrawingObject DO;
	DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
							pDev,_T("effect\\basic\\ball_simple.png")));
	DO.m_TexRange.setLTRB(0,0,1,1);
	DO.m_color.ARGB(255,128,32,0);
	DO.m_color.alpha = opaque*DO.m_color.alpha;
	if(flamelevel() == 2)
	{
		DO.Width = MAPTEXBOXSIZE*MAPTEXPOWER * 1.2;
		DO.Height = MAPTEXBOXSIZE*MAPTEXPOWER * 1.2;
	}
	else if(flamelevel() == 1)
	{
		DO.Width = MAPTEXBOXSIZE*MAPTEXPOWER * 0.8;
		DO.Height = MAPTEXBOXSIZE*MAPTEXPOWER * 0.8;
		DO.m_color.alpha *= 0.7;
	}
	else
	{
		DO.Width = MAPTEXBOXSIZE*MAPTEXPOWER * 0.4;
		DO.Height = MAPTEXBOXSIZE*MAPTEXPOWER * 0.4;
		DO.m_color.alpha *= 0.6;
	}


	DO.CenterX =  
		MAPDRAWCENTERX + MAPTEXBOXSIZE*MAPTEXPOWER*(visibleplace.x - mapForcus.x)
		+2;
	
	DO.CenterY =  
		MAPDRAWCENTERY + MAPTEXBOXSIZE*MAPTEXPOWER*(visibleplace.y - mapForcus.y)
		+2;

	DO.Draw(pDev);

	int i;
	for(i=0;i<flooranime.size();i++)
	{
		//描画基本位置
		
		c4DVector center;
		center.x = placeX;
		center.y = placeY;
		//center = sg_pDungeonSystem->Map().visibleplace(center);
		center.x += flooranime[i]->CenterX;
		center.y += flooranime[i]->CenterY;
		//描画位置
		//flooranime[i]->Centerbegin = center;
		//flooranime[i]->Centerend = center;
		//flooranime[i]->Centerend.y -= MAPTEXBOXSIZE*MAPTEXPOWER/8;
		
		flooranime[i]->place = center;
		flooranime[i]->AbsolutePlaceFlag = 0;
		flooranime[i]->Centerend.y = -MAPTEXBOXSIZE*MAPTEXPOWER/4;
		
		flooranime[i]->DO.m_color.alpha = opaque*DO.m_color.alpha;
		
		flooranime[i]->Draw(pDev);
	}

}
void cThing_ID_0::Process(cRenderDevice *pDev)
{
	int i;
	
	for(i=0;i<flooranime.size();i++)
	{
		flooranime[i]->process(pDev);
		flooranime[i]->DrowingObjectFunc();
		if(flooranime[i]->End && opaque > 0.5 )
		{
			flooranime[i]->End = false;
			//表示時間
			flooranime[i]->hidingTime = rand()*4/RAND_MAX;
			flooranime[i]->FrameCount = 0;

			double d = rand()*360/RAND_MAX;
			flooranime[i]->Rotation.set(d,d+rand()*60/RAND_MAX-30,1,1);
			d = (double)rand()/RAND_MAX;
			flooranime[i]->ScaleX.set(0.5,1,1+d,1+d);
			flooranime[i]->ScaleY.set(0.5,1,1+d,1+d);
			if(flamelevel() == 2)
			{
				//描画位置
				flooranime[i]->CenterX = (rand()/(double)RAND_MAX - 0.5)*0.9;
				flooranime[i]->CenterY = (rand()/(double)RAND_MAX - 0.5)*0.9;
			}
			else if(flamelevel() == 1)
			{
				//描画位置
				flooranime[i]->CenterX = (rand()/(double)RAND_MAX - 0.5)*0.6;
				flooranime[i]->CenterY = (rand()/(double)RAND_MAX - 0.5)*0.6;
				flooranime[i]->ScaleX.z *= 0.75;
				flooranime[i]->ScaleX.t *= 0.75;
				flooranime[i]->ScaleY.z *= 0.75;
				flooranime[i]->ScaleY.t *= 0.75;
			}
			else
			{
				//描画位置
				flooranime[i]->CenterX = (rand()/(double)RAND_MAX - 0.5)*0.3;
				flooranime[i]->CenterY = (rand()/(double)RAND_MAX - 0.5)*0.3;
				flooranime[i]->ScaleX.z *= 0.75;
				flooranime[i]->ScaleX.t *= 0.75;
				flooranime[i]->ScaleY.z *= 0.75;
				flooranime[i]->ScaleY.t *= 0.75;
			}

		}
	}
	for(i=0;i<flameanime.size();i++)
	{


		if(flamelevel() != 0)
		{
			if(flameanime[i]->End && opaque > 0.5 && pri_onland_DrawFlag)
			{
				flameanime[i]->End = false;
				//表示時間
				flameanime[i]->hidingTime = 0;
				flameanime[i]->FrameCount = 0;
				double d;
				int index = rand()*24/(RAND_MAX+1);
				//テクスチャ座標セット
				if(index<8)
				{
					flameanime[i]->Rotation.set(0,0,1,1);
					d = (double)rand()/RAND_MAX;
					flameanime[i]->ScaleX.set(1,	0.7,	1+d,	1+d);
					flameanime[i]->ScaleY.set(1.5,	1.75,	0.5+d/4,	1+d/2);
					flameanime[i]->life = 15+ rand()*5/(RAND_MAX+1);
					flameanime[i]->Alpha.set(0,1.5,1.5,0);
					flameanime[i]->DO.m_TexRange.setLTRB(abs(4-(index))/4.0,0/4.0,abs(4-(index+1))/4.0,1/4.0);
				}
				else
				{
					d = rand()*360/RAND_MAX;
					flameanime[i]->Rotation.set(d,d+rand()*60/RAND_MAX-30,1,1);
					d = (double)rand()/RAND_MAX;
					flameanime[i]->Alpha.set(0,2,2,0);
					flameanime[i]->ScaleX.set(0.7,1,(1+d)*1.5,(1+d)*1.5);
					flameanime[i]->ScaleY.set(0.7,1,(1+d)*1.5,(1+d)*1.5);
					flameanime[i]->life = 15+ rand()*5/(RAND_MAX+1);
					flameanime[i]->DO.m_TexRange.setLTRB(0/4.0,3/4.0,1/4.0,4/4.0);
				}
				if(flamelevel() == 2)
				{
					//描画位置
					flameanime[i]->CenterX = (rand()/(double)RAND_MAX - 0.5)*0.9;
					flameanime[i]->CenterY = (rand()/(double)RAND_MAX - 0.5)*0.9;
				}
				else if(flamelevel() == 1)
				{
					//描画位置
					flameanime[i]->CenterX = (rand()/(double)RAND_MAX - 0.5)*0.5;
					flameanime[i]->CenterY = (rand()/(double)RAND_MAX - 0.5)*0.5;
					flameanime[i]->ScaleX.z *= 0.6;
					flameanime[i]->ScaleX.t *= 0.6;
					flameanime[i]->ScaleY.z *= 0.6;
					flameanime[i]->ScaleY.t *= 0.6;
				}
				else
				{

				}
				sg_pDungeonSystem->AnimationManager().AddAnime_realtime(boost::static_pointer_cast<cAnimation>(flameanime[i]));
			}
		}


		//描画基本位置
		c4DVector center;
		center.x = placeX;
		center.y = placeY;
		//center = sg_pDungeonSystem->Map().visibleplace(center);
		center.x += flameanime[i]->CenterX;
		center.y += flameanime[i]->CenterY;
		//描画位置
		flameanime[i]->place = center;
		flameanime[i]->AbsolutePlaceFlag = 0;
		//flameanime[i]->Centerbegin = center;
		//flameanime[i]->Centerend = center;
		//flameanime[i]->Centerend.y -= MAPTEXBOXSIZE*MAPTEXPOWER/4;
		flameanime[i]->Centerend.y = -MAPTEXBOXSIZE*MAPTEXPOWER/4;
	}
}
int cThing_ID_0::乗る()
{		
	pcCharacter pchara = 乗りキャラ();
	if(pchara != NULL)
	{
		効果(pchara, 効果量(0), true);	
	}
	
	return false;
}
int cThing_ID_0::効果(pcCharacter pchara, int power, int mustFlag)
{
	if(burned() && !mustFlag) return false;
	if(!GameIdiom::燃え(投擲者(), pchara->足元地形(), power,1)) return false;
	if(pchara->死亡()){recoverflamepower(効果量(2));}
	burned() = true;
	sg_pDungeonSystem->強制ひるみ要請(pchara);
	return true;
}
void cThing_ID_0::CutIn(タイミング timing, cValiableField& valiable)
{
	cDroping::CutIn(timing, valiable);
	

	if(timing == セクション終了_タイミング)
	{
		pcCharacter pchara = 乗りキャラ();
		if(pchara != NULL)
		{
			if(
				(
				(pchara->Condition.二倍速以上である() && sg_pDungeonSystem->s_turn_speed() == GAME_TURN_SPEED_DOUBLE)
				|| (pchara->Condition.三倍速である() && sg_pDungeonSystem->s_turn_speed() == GAME_TURN_SPEED_TRIPLE)
				|| sg_pDungeonSystem->s_turn_speed() == GAME_TURN_SPEED_SINGLE
				)
				&& !pchara->TurnEndFlag
				)
			{

				効果(pchara, 効果量(0));
			}
		}
		burned() = false;
	}
	else if(timing == ターン終了_タイミング)
	{
		setflamepower(flamepower()-1);
		if(flamepower() <= 0)
		{
			sg_pDungeonSystem->落ち物破壊要請(me());
		}
	}
	else if(timing == 被攻撃接近直後_タイミング)
	{
		if(valiable.intsets[変数_属性].count(攻撃属性::水))
		{
			setflamepower(0);
		}
	}
}
void cThing_ID_0::setflamepower(int power)
{
	flamepower() = power;
	sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(&visibleflamepower_,power);
}
void cThing_ID_0::recoverflamepower(int power)
{
	int p = flamepower() + power;
	if(Maxflamepower() < p) p = Maxflamepower();
	map<tstring, StyleString> valiable;
	valiable[_T("Drop")] = FullName();
	g_Langメッセージ(_T("火柱回復メッセージ"),valiable);
	setflamepower(p);
}
int cThing_ID_0::Maxflamepower()
{
	return 効果量(1);
}
int cThing_ID_0::GetmenuCaption(vector<tstring> &CaptionList)
{
	CaptionList.push_back(_T("説明"));
	return true;
}
int cThing_ID_0::flamelevel()
{
	if(visibleflamepower() >= 6) return 2;
	else if(visibleflamepower() >= 3) return 1;
	else return 0;
}
//表示を実値と同期させる
void cThing_ID_0::visibleReset()
{
	cDroping::visibleReset();
	visibleflamepower_ = flamepower();
}
int cThing_ID_0::上に落ちる(pcDroping pdrop)//pdropを破壊したいときtrue;
{
	if(!GameIdiom::アイテム焼失(pdrop)) return false;
	map<tstring, StyleString> valiable;
	valiable[_T("Drop")] = pdrop->FullName();
	g_Langメッセージ(_T("落ちもの焼失メッセージ"),valiable);
	if(pdrop->属性.count(落ち物属性::火消し)) setflamepower(0);
	else recoverflamepower(効果量(2));
	return true;
}
int cThing_ID_0::上に落ちる(pcCharacter pchara)//pdropに何か起こったらtrue;
{
	if(pchara != NULL)
	{
		効果(pchara, 効果量(0));
		return pchara->死亡();
	}
	return false;
}
int cThing_ID_0::上を通過(pcDroping pdrop)//pdropに何か起こったらtrue;
{
	if(!GameIdiom::アイテム焼失(pdrop)) return false;
	map<tstring, StyleString> valiable;
	valiable[_T("Drop")] = pdrop->FullName();
	g_Langメッセージ(_T("落ちもの焼失メッセージ"),valiable);
	if(pdrop->属性.count(落ち物属性::火消し)) setflamepower(0);
	else recoverflamepower(効果量(2));
	return true;
}
int cThing_ID_0::上を通過(pcCharacter pchara)//pdropに何か起こったらtrue;
{
	if(pchara != NULL)
	{
		効果(pchara, 効果量(0));	
		return pchara->死亡();
	}
	return false;
}
//----------------------------------------------
//御柱
//----------------------------------------------
int cThing_ID_1::GetmenuCaption(vector<tstring> &CaptionList)
{
	CaptionList.push_back(_T("説明"));
	return true;
}

void cThing_ID_1::Draw(cRenderDevice *pDev)
{
	cDrawingObject DO;
	GetIcon(DO);



	DO.Width = MAPTEXBOXSIZE*MAPTEXPOWER*2;
	DO.Height = MAPTEXBOXSIZE*MAPTEXPOWER*2;

	DO.CenterX =  
		MAPDRAWCENTERX + MAPTEXBOXSIZE*MAPTEXPOWER*(visibleplace.x - mapForcus.x)
		+2;
	
	DO.CenterY =  
		MAPDRAWCENTERY + MAPTEXBOXSIZE*MAPTEXPOWER*(visibleplace.y - mapForcus.y)
		+2;
	
	//影
	DO.m_color.ARGB(128,0,0,0);
	DO.m_color.alpha = opaque*DO.m_color.alpha;
		
	DO.Draw(pDev);

	
	DO.CenterX =  
		MAPDRAWCENTERX + MAPTEXBOXSIZE*MAPTEXPOWER*(visibleplace.x - mapForcus.x);
	
	DO.CenterY =  
		MAPDRAWCENTERY + MAPTEXBOXSIZE*MAPTEXPOWER*(visibleplace.y -visibleplace.z*0.5 - mapForcus.y);
	
	//本体
	DO.m_color.ARGB(255,255,255,255);
	DO.m_color.alpha = opaque*DO.m_color.alpha;
		
	DO.Draw(pDev);
}
int cThing_ID_1::GetIcon(cDrawingObject& DO)
{
	
	DO.setTexture(m_pTexture_Icon,IconFileSize(),IconFileSize());
	DO.m_TexRange.setLTRB(IconFileIndexX()/2.0, IconFileIndexY()/2.0,
					(IconFileIndexX()+1)/2.0, (IconFileIndexY()+1)/2.0);


	return true;
}
int cThing_ID_1::乗る()
{		
	
	
	return false;
}
int cThing_ID_1::上を通過(pcCharacter pchara)//pdropに何か起こったらtrue;
{
	if(pchara != NULL && sg_pDungeonSystem->キャラクター敵対判定(sg_pDungeonSystem->pPlayerChara(), pchara))
	{
		
		multiset<攻撃属性::攻撃属性> temp;
		sg_pDungeonSystem->攻撃接近(攻撃作成(
			NULLCHARA,//攻撃者
			pchara->足元地形(),//場所
			効果量(0),//攻撃力
			1,//定数ダメージ
			temp,
			true));//必中

		return pchara->死亡();
	}
	return false;
}
void cThing_ID_1::CutIn(タイミング timing, cValiableField& valiable)
{
	cDroping::CutIn(timing, valiable);
	

	if(timing == 隣接地形被移動可判定_タイミング)
	{
		if(sg_pDungeonSystem->キャラクター敵対判定(sg_pDungeonSystem->pPlayerChara(), valiable.charas.val(変数_移動者)))
		{
			valiable.doubles.dim(変数_汎用ブール) = 0;//不許可
		}
	}
	else if(timing == 隣接地形被攻撃可判定_タイミング)
	{
		if(sg_pDungeonSystem->キャラクター敵対判定(sg_pDungeonSystem->pPlayerChara(), valiable.charas.val(変数_攻撃者)))
		{
			valiable.doubles.dim(変数_汎用ブール) = 0;//不許可
		}
	}


}


//----------------------------------------------
//出口
//----------------------------------------------
int cThing_ID_2::GetmenuCaption(vector<tstring> &CaptionList)
{
	CaptionList.push_back(_T("説明"));
	return true;
}

void cThing_ID_2::Draw(cRenderDevice *pDev)
{
	cDrawingObject DO;
	GetIcon(DO);


	DO.Width = MAPTEXBOXSIZE*MAPTEXPOWER;
	DO.Height = MAPTEXBOXSIZE*MAPTEXPOWER*2;

	DO.CenterX =  
		MAPDRAWCENTERX + MAPTEXBOXSIZE*MAPTEXPOWER*(visibleplace.x - mapForcus.x);
	
	DO.CenterY = -MAPTEXBOXSIZE*MAPTEXPOWER/2 +
		MAPDRAWCENTERY + MAPTEXBOXSIZE*MAPTEXPOWER*(visibleplace.y -visibleplace.z*0.5 - mapForcus.y);
	
	//本体
	DO.m_color.ARGB(255,255,255,255);
	DO.m_color.alpha = opaque*DO.m_color.alpha;
	DO.AddingDraw = cDrawingObject::DRAW_MODE_ADDITION;
	DO.Draw(pDev);
}
int cThing_ID_2::GetIcon(cDrawingObject& DO)
{
	
	DO.setTexture(m_pTexture_Icon,IconFileSize(),IconFileSize());
	DO.m_TexRange.setLTRB(IconFileIndexX()/4.0, IconFileIndexY()/2.0,
					(IconFileIndexX()+1)/4.0, (IconFileIndexY()+1)/2.0);


	return true;
}
int cThing_ID_2::乗る()
{		
	
	
	return false;
}
