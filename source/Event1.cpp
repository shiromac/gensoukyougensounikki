#include "stdafx.h"
#include "Event1.h"
#include "EffectFunctions.h"

void cEvent_OnryouSpawn::CutIn(タイミング timing, cValiableField& valiable)
{
	if (ターン終了_タイミング == timing)
	{
		if(count_ < 5)
		{
			++count_;
		}
		else
		{
			pcLandform pland = sg_pDungeonSystem->Map().Land(x_,y_);
			if(sg_pDungeonSystem->キャラ配置安全(pland))
			{
				EffectFunctions::煙エフェクト1(x_,y_);
				pcCharacter pchara = sg_pDungeonSystem->キャラクター生成(2042, classnum_, CHARACTER_FORSE_ENEMY, pland);
				me()->endEvent();
			}
		}
	}
}
void cEvent_OnryouSpawn::InitSub()
{
	count_ = 0;
}
//---------------------------------------------------------------------------
void cEvent_ItemHidding::CutIn(タイミング timing, cValiableField& valiable)
{
	if (主人公ターン開始直前_タイミング == timing)
	{
		if(!hideItem_->broken)
		{
			pcLandform pland = NULLLAND;
			if(hideItem_->Holder() != NULL)
			{
				pland = hideItem_->Holder()->足元地形();
			}
			else if(hideItem_->乗りキャラ())
			{
				pland = hideItem_->現在地形();
			}

			if(pland != NULLLAND)
			{
				EffectFunctions::煙エフェクト1(pland->placeX, pland->placeY);
				if(sg_pDungeonSystem->アイテム隠れ解除(hideChara_, pland))
				{
					me()->endEvent();
				}
			}
		}
	}
}
void cEvent_ItemHidding::InitSub()
{
}
//---------------------------------------------------------------------------
void cEvent_ItemSpowning::CutIn(タイミング timing, cValiableField& valiable)
{
	if (セクション終了_タイミング == timing)
	{
		EffectFunctions::煙エフェクト1(dropLand_->placeX, dropLand_->placeY);
		sg_pDungeonSystem->落ち物設置(dropItem_, dropLand_);
		me()->endEvent();
	}
}
void cEvent_ItemSpowning::InitSub()
{
}
//---------------------------------------------------------------------------
void cEvent_BossLazer::CutIn(タイミング timing, cValiableField& valiable)
{
	if (主人公ターン開始直前_タイミング == timing)
	{
		TurnProcess();
	}
}
void cEvent_BossLazer::TurnProcess()
{
	if(state_ == stateFirst)
	{
		state_ = stateReady;
		countOfReadyTurn_ = 0;
	}

	if(state_ == stateReady)
	{
		if(countOfReadyTurn_ < readyTurn_)
		{
			countOfReadyTurn_++;
		}
		else
		{
			state_ = stateFire;
			countOfAttackTurn_ = 0;
		}
	}

	if(state_ == stateFire)
	{
		//攻撃発生
		Fire();

		if(countOfAttackTurn_ < attackTurn_)
		{
			countOfAttackTurn_++;
		}
		else
		{
			state_ = stateEnd;
		}
	}

	if(state_ == stateEnd)
	{

	}

	
}

void cEvent_BossLazer::DrawProcess()
{
	visibleAdvanceLightRadian_ += 0.5;

	switch (drawState_) {
	case drawStateFirst:

		drawState_ = drawStateFirstToReady;
		break;

	case drawStateFirstToReady:
		
		drawState_ = drawStateReady;
		break;

	case drawStateReady:
		if(state_ >= stateFire)
		{
			drawState_ = drawStateReadyToFire;
		}

		{
			double power = 0.2;
			visibleWidth_ = 0.2 * power + visibleWidth_*(1-power);
			visibleAlpha_ = 0.5 * power + visibleAlpha_*(1-power);
		}
		break;

	case drawStateReadyToFire:

		visibleWidth_ = 2.4;
		visibleAlpha_ = 1.0;
		drawState_ = drawStateFire;
		isJustFire_ = false;
		break;

	case drawStateFire:
		if(state_ >= stateEnd)
		{
			drawState_ = drawStateFireToEnd;
		}
		else if(isJustFire_)
		{
			visibleWidth_ = 2.4;
			visibleAlpha_ = 0.8;
			isJustFire_ = false;
		}
		{
			double power = 0.2;
			visibleWidth_ = 1.2 * power + visibleWidth_*(1-power);
			visibleAlpha_ = 0.5 * power + visibleAlpha_*(1-power);
		}
		break;

	case drawStateFireToEnd:

		{
			double power = 0.4;
			visibleWidth_ = 0 * power + visibleWidth_*(1-power);
			visibleAlpha_ = 0 * power + visibleAlpha_*(1-power);
		}
		
		if(visibleWidth_ <= 0.01)
		{
			drawState_ = drawStateEnd;
		}
		break;

	case drawStateEnd:
		endEvent();
		break;
	}

}

void cEvent_BossLazer::Fire()
{
	isJustFire_ = true;
	int i;
	for(i=0;i<=length_;i++)
	{
		InFire(sg_pDungeonSystem->隣接地形(land_, aspect_, i));
	}
}

void cEvent_BossLazer::InFire(pcLandform land)
{
	pcCharacter pchara = land->pOnChar;
	if(pchara == NULLCHARA)
	{
		return;
	}

	if( ! isAttackFriend_ && 
		(Forse_ == pchara->Forse || CHARACTER_FORSE_OTHER == pchara->Forse))
	{
		return;
	}

	multiset<攻撃属性::攻撃属性> 属性;
	sg_pDungeonSystem->攻撃接近(攻撃作成(
		NULLCHARA,//攻撃者
		land,//場所
		power_,//攻撃力
		true,//定数ダメージ
		属性,
		true
		));
}

void cEvent_BossLazer::Draw()
{
	
	DrawProcess();

	int i;
	/*
	c4DVector point(coo.x*(-visibleWidth_/2), coo.y*(-visibleWidth_/2), 0, 0);
	c4DVector point2(point);
	point.turn(-45);
	point2.turn(45);
	point.x += land_->placeX;
	point.y += land_->placeY;
	point2.x += land_->placeX;
	point2.y += land_->placeY;
	beltEx_.rightStartpoint() = sg_pDungeonSystem->Map().visibleplace(point);
	beltEx_.leftStartpoint() = sg_pDungeonSystem->Map().visibleplace(point2);
	

	beltEx_.Draw(sg_pDungeonSystem->pDevice_D3D);
	*/

	resetVisibleLength();
	rodEx_.Draw(sg_pDungeonSystem->pDevice_D3D);
	rodIn_.Draw(sg_pDungeonSystem->pDevice_D3D);
	rodAdvance_.Draw(sg_pDungeonSystem->pDevice_D3D);
}
void cEvent_BossLazer::minimapDraw(const cCoordinate& leftTop, const int boxSize, const int alpha)
{
	cCoordinate coo;
	coo.SetAspect(aspect_);

	cCoordinate point_vis(land_->placeX*boxSize + leftTop.x, land_->placeY*boxSize + leftTop.y);

	rodMiniMap_.halfWidth = visibleWidth_* boxSize /2;
	rodMiniMap_.startX = point_vis.x + boxSize/2;
	rodMiniMap_.startY = point_vis.y + boxSize/2;
	rodMiniMap_.endX = coo.x*culcuratedLength_ * boxSize + rodMiniMap_.startX;
	rodMiniMap_.endY = coo.y*culcuratedLength_ * boxSize + rodMiniMap_.startY;

	rodMiniMap_.m_color.alpha = alpha;
	rodMiniMap_.Draw(sg_pDungeonSystem->pDevice_D3D);

}

void cEvent_BossLazer::InitSub()
{
	
	cRenderTexture* texture = g_GameEnv.m_GlobalResourse->getTextureFromFile(sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_simple.png"));
	rodEx_.setTexture(texture);
	rodEx_.m_color = color_;
	rodEx_.m_TexRange.setLTRB(0,0,1,1);
	rodIn_.setTexture(texture);
	rodIn_.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;
	rodIn_.m_color.ARGB(255,255,255,255);
	rodAdvance_.setTexture(texture);
	rodAdvance_.m_color = color_;
	rodAdvance_.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

	rodMiniMap_.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(sg_pDungeonSystem->pDevice_D3D,_T("map\\miniMapLazer.png")));

	countOfReadyTurn_ = 0;
	countOfAttackTurn_ = 0;

	state_ = stateFirst;
	drawState_ = drawStateFirst;

	if(isStopAtWall_)
	{
		resetLength();
	}
	else
	{
		culcuratedLength_ = length_;
	}
	displayCount_ = 0;
	visibleLength_ = 0;
	visibleWidth_ = 0;
	visibleAlpha_ = 0;
	visibleAdvanceLightRadian_ = 0.0;
	visibleReset();
	
	isJustFire_ = false;

	TurnProcess();
}
void cEvent_BossLazer::visibleReset()
{
	resetVisibleLength();
}

void cEvent_BossLazer::resetVisibleLength()
{
	visibleLength_ = culcuratedLength_;

	double boxsize = MAPTEXBOXSIZE*MAPTEXPOWER;

	cCoordinate coo;
	coo.SetAspect(aspect_);
	int i;
	c4DVector point(land_->placeX, land_->placeY,0,0);
	c4DVector point_vis = sg_pDungeonSystem->Map().visibleplace(point);

	rodEx_.halfWidth = visibleWidth_* boxsize /2;
	rodEx_.startX = point_vis.x;
	rodEx_.startY = point_vis.y - boxsize /3;
	rodEx_.endX = coo.x*culcuratedLength_ * boxsize + rodEx_.startX;
	rodEx_.endY = coo.y*culcuratedLength_ * boxsize + rodEx_.startY;
	rodIn_.halfWidth = visibleWidth_* boxsize /3;
	rodIn_.startX = rodEx_.startX;
	rodIn_.startY = rodEx_.startY;
	rodIn_.endX = rodEx_.endX;
	rodIn_.endY = rodEx_.endY;
	rodAdvance_.halfWidth = visibleWidth_* boxsize /2;
	rodAdvance_.startX = rodEx_.startX;
	rodAdvance_.startY = rodEx_.startY;
	rodAdvance_.endX = rodEx_.endX;
	rodAdvance_.endY = rodEx_.endY;

	rodEx_.m_color.alpha = visibleAlpha_*255;
	rodIn_.m_color.alpha = visibleAlpha_*255;
	rodAdvance_.m_color.alpha = visibleAlpha_*(sin(visibleAdvanceLightRadian_)+1)*64/2;

	/*
	c4DVector point(coo.x*(-visibleWidth_/2) * boxsize, coo.y*(-visibleWidth_/2) * boxsize, 0, 0);
	beltEx_.rightline().addPoint(point);
	for(i=0;i<culcuratedLength_;i++)
	{
		c4DVector point(coo.x*i * boxsize, coo.y*i * boxsize, 0, 0);
		beltEx_.rightline().addPoint(point);
	}
	point.set(coo.x*(culcuratedLength_+visibleWidth_/2) * boxsize, coo.y*(culcuratedLength_+visibleWidth_/2) * boxsize, 0, 0);
	beltEx_.rightline().addPoint(point);
	beltEx_.leftline() = beltEx_.rightline();
	beltEx_.start = 0;
	beltEx_.end = culcuratedLength_+2;
	*/
}

void cEvent_BossLazer::resetLength()
{
	
	pcLandform endLand = sg_pDungeonSystem->射程内障害物検索(land_, aspect_, length_, true, false, false, false);
	culcuratedLength_ = (land_->place - endLand->place).dif();
}
