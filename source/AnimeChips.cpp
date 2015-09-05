#include "AnimeChips.h"
#include <debug.h>
#include "cDrawingObject.h"
#include "cDungeonSystem.h"
#include "gameMainSystem/cEnvironment.h"

const double FRAMEPARSECOND = 60.0;
const double M_PI = 3.14159265358979;

void compatible(c4DVector& vec, const D3DXVECTOR3& d3dvec)
{
	vec.x = d3dvec.x;
	vec.y = d3dvec.y;
	vec.z = d3dvec.z;
	vec.t = 0;
}
void compatible(D3DXVECTOR3& d3dvec, const c4DVector& vec)
{
	d3dvec.x = vec.x;
	d3dvec.y = vec.y;
	d3dvec.z = vec.z;
}
void turnOnHorizon(D3DXVECTOR3& d3dvec , const double degree)
{
    double rad = degree * V_PI / 180;
	D3DXVECTOR3 tmp(d3dvec);
	double s = sin(rad),c = cos(rad);

	d3dvec.x = tmp.x * c - tmp.y * s;
	d3dvec.y = tmp.x * s + tmp.y * c;
}

AnimeChip::AnimeChip():
		count_(0)
{

}
AnimeChip::~AnimeChip()
{

}
PositionPerGrid AnimeChip::worldPosition()
{
	pAnimeChip parent_chip = parent();

	if(parent_chip == NULL)
	{
		return localPosition();
	}

	return parent_chip->worldPosition() + localPosition();
}
bool AnimeChip::isOnScreenStatic(const PositionPerGrid& position, double marginInGrid)
{
	return sg_pDungeonSystem->Map().isOnDisplay(position.x, position.y - position.z/2, (int)marginInGrid+1);
};

//------------------------------------------------------
AnimeChip_Chain::AnimeChip_Chain():
		life_(0),
		delay_(0),
		delayCount_(0),
		worldRotation_(0),
		alwaysOnScreen_(false)
{
}

AnimeChip_Chain::~AnimeChip_Chain()
{
	return;
}
int AnimeChip_Chain::Draw(IDirect3DDevice9 *pDev)
{
	if(selfEnd())
	{
		if(nextchip_ != NULL)
		{
			nextchip_->Draw(pDev);
		}
	}
	else
	{
		if(delay_ <= delayCount_)
		{
			selfDraw(pDev);
		}
	}
	return true;
}
int AnimeChip_Chain::Process()
{
	if(selfEnd())
	{
		if(nextchip_ != NULL)
		{
			nextchip_->Process();
		}
	}
	else
	{
		if(delay_ <= delayCount_)
		{
			++count_;
			selfProcess();
		}
		else
		{
			++delayCount_;
		}

		if(delay_ == delayCount_)
		{
			selfProcessInitialize();
		}
		
		if(count_ == life_)
		{
			selfProcessFinalize();
		}
	}

	if(nextchip_ != NULL)
	{
		if(nextchip_->End && selfEnd())
		{
			End = true;
		}
	}
	else
	{
		if(selfEnd())
		{
			End = true;
		}
	}
	return true;
}
bool AnimeChip_Chain::selfEnd()
{
	return (count_ >= life_);
}
bool AnimeChip_Chain::isOnScreen()
{
	if(alwaysOnScreen_) {
		return true;
	}
	if(isOnScreenJudgeFunction())
	{
		return true;
	}
	if(nextchip_)
	{
		return nextchip_->isOnScreen();
	}
	return false;
}
bool AnimeChip_Chain::isOnScreenNow(){
	if(alwaysOnScreen_) {
		return true;
	}
	if(isOnScreenNowJudgeFunction())
	{
		return true;
	}
	if(nextchip_)
	{
		return nextchip_->isOnScreenNow();
	}
	return false;
}
void AnimeChip_Chain::setAlwaysOnScreen(bool flag) {
	alwaysOnScreen_ = flag;
}
void AnimeChip_Chain::addNextChip(pAnimeChip animechip)
{
	//init(parent)伝播をする必要がある
	nextchip_ = animechip;
	nextchip_->Init(nextchip_);
	if(me() != NULL)
	{//自分がinitされてる
		nextchip_->setParent(me_pAnimeChip());
	}
}
int AnimeChip_Chain::Init(pcAnimation me)
{
	cAnimation::Init(me);
	if(me_pAnimeChip() == NULL)
	{
		OnAssert(_T(__FILE__),__LINE__,false,_T("Initに自分自身以外のポインタが挿入されました。"),_T(""));
		return false;
	}
	//init(parent)伝播をする必要がある
	if(nextchip_ && nextchip_->parent() == NULL)
	{//子がparentを知らない
		nextchip_->setParent(me_pAnimeChip());
	}
	return true;
}

//------------------------------------------------------
AnimeChip_DrawableObject::AnimeChip_DrawableObject():
	scaleX(1.0),scaleY(1.0),
		color(cColor(255,255,255,255)),
		colorBlendMode(cDrawableObject::COLOR_BLEND_MULTIPLE),
		drawMode(cDrawableObject::DRAW_MODE_NORMAL),
		textureRange(TextureRangeRect(0,0,1,1)),
		position(PositionPerGrid(0,0,0))
{
}

AnimeChip_DrawableObject::~AnimeChip_DrawableObject()
{
	return;
}
PositionPerGrid AnimeChip_DrawableObject::localPosition()
{
	PositionPerGrid local = position((double)count_);
	turnOnHorizon(local, worldRotation_);
	return local;
}
void AnimeChip_DrawableObject::setTexture(const pLuaString textureFileName)
{
	LuaStringTransformer luastring(textureFileName);
	setTexture(luastring.t_str());
}
bool AnimeChip_DrawableObject::isOnScreenJudgeFunction()
{
	PositionPerGrid parentPosition(0,0,0);
	if(parent())
	{
		parentPosition = parent()->worldPosition();
	}
	PositionPerGrid beginPosition = position.value(0);
	turnOnHorizon(beginPosition,worldRotation_);
	if(isOnScreenStatic(parentPosition + beginPosition,0))
	{
		return true;
	}
	PositionPerGrid endPosition = position.value(position.sumLength());
	turnOnHorizon(endPosition, worldRotation_);
	return isOnScreenStatic(parentPosition + endPosition,0);
}
bool AnimeChip_DrawableObject::isOnScreenNowJudgeFunction()
{
	return isOnScreenStatic(worldPosition(),0);
}
//------------------------------------------------------
AnimeChip_Hab::AnimeChip_Hab():
	position(PositionPerGrid(0,0,0)),
	alwaysOnScreen_(false)
{

}
AnimeChip_Hab::AnimeChip_Hab(const PositionPerGrid& pos):
	position(pos)
{

}

AnimeChip_Hab::~AnimeChip_Hab()
{
	return;
}
int AnimeChip_Hab::Draw(IDirect3DDevice9 *pDev)
{
	if(End)
	{
		return true;
	}
	std::vector<pAnimeChip>::iterator itrend = chipVector_.end();
	std::vector<pAnimeChip>::iterator itr = chipVector_.begin();
	
	selfDraw(pDev);
	for(;itr != itrend;++itr)
	{
		(*itr)->Draw(pDev);
	}
	return true;
}
int AnimeChip_Hab::Process()
{
	if(End)
	{
		return true;
	}

	selfProcess();
	count_++;

	int tempEndFlag = true;
	std::vector<pAnimeChip>::iterator itrend = chipVector_.end();
	std::vector<pAnimeChip>::iterator itr = chipVector_.begin();
	
	for(;itr != itrend;++itr)
	{
		(*itr)->Process();
		if(!((*itr)->End))
		{
			tempEndFlag = false;
		}
	}
	if(tempEndFlag)
	{
		End = true;
	}

	return true;
}
PositionPerGrid AnimeChip_Hab::localPosition()
{
	return position((double)count_); 
}
int AnimeChip_Hab::Init(pcAnimation me)
{
	cAnimation::Init(me);
	if(me_pAnimeChip() == NULL)
	{
		OnAssert(_T(__FILE__),__LINE__,false,_T("Initに自分自身以外のポインタが挿入されました。"),_T(""));
		return false;
	}
	//init(parent)伝播をする必要がある
	std::vector<pAnimeChip>::iterator itrend = chipVector_.end();
	std::vector<pAnimeChip>::iterator itr = chipVector_.begin();
	
	for(;itr != itrend;++itr)
	{
		if(*itr && (*itr)->parent() == NULL)
		{//子がparentを知らない
			(*itr)->setParent(me_pAnimeChip());
		}
	}

	return true;
}
void AnimeChip_Hab::addAnimeChip(pAnimeChip animechip)
{
	//init(parent)伝播をする必要がある
	chipVector_.push_back(animechip);
	animechip->Init(animechip);
	if(me() != NULL)
	{//自分がinitされてる
		animechip->setParent(me_pAnimeChip());
	}
}
void AnimeChip_Hab::clearAnimechip()
{
	chipVector_.clear();
}
bool AnimeChip_Hab::childEmpty()
{
	return chipVector_.empty();
}
bool AnimeChip_Hab::isOnScreenJudgeFunction()
{
	PositionPerGrid parentPosition(0,0,0);
	if(parent())
	{
		parentPosition = parent()->worldPosition();
	}
	return isOnScreenStatic(parentPosition + position.value(0),0) || isOnScreenStatic(parentPosition + position.value(position.sumLength()),0);
}
bool AnimeChip_Hab::isOnScreenNowJudgeFunction()
{
	return isOnScreenStatic(worldPosition(),0);
}
bool AnimeChip_Hab::isOnScreen()
{
	if(alwaysOnScreen_) {
		return true;
	}
	if(isOnScreenJudgeFunction())
	{
		return true;
	}
	int i,size = chipVector_.size();
	for(i=0;i<size;i++)
	{
		return chipVector_[i]->isOnScreen();
	}
	return false;
}
bool AnimeChip_Hab::isOnScreenNow(){
	if(alwaysOnScreen_) {
		return true;
	}
	if(isOnScreenNowJudgeFunction())
	{
		return true;
	}
	int i,size = chipVector_.size();
	for(i=0;i<size;i++)
	{
		return chipVector_[i]->isOnScreenNow();
	}
	return false;
}
void AnimeChip_Hab::setAlwaysOnScreen(bool flag) {
	alwaysOnScreen_ = flag;
}
//------------------------------------------------------
AnimeChip_Perticle::AnimeChip_Perticle(const pLuaString filename, const PerticleSize& perticleSize, const int life):
		rotationBefore(0.0),
		rotationAfter(0.0),
		perticleSize_(perticleSize)
{
	AnimeChip_DrawableObject::setTexture(filename);
	setLife(life);
}
AnimeChip_Perticle::AnimeChip_Perticle(const int life):
		rotationBefore(0.0),
		rotationAfter(0.0),
		perticleSize_(16,16)
{
	setLife(life);
}
AnimeChip_Perticle::~AnimeChip_Perticle()
{
	return;
}
void AnimeChip_Perticle::selfProcess()
{
	return;
}
void AnimeChip_Perticle::selfDraw(IDirect3DDevice9 *pDev)
{
	D3DXVECTOR3 pos = worldPosition();
	c4DVector pv = sg_pDungeonSystem->Map().visibleplace( c4DVector(pos.x, pos.y, pos.z, 0));
	DO.CenterX = pv.x;
	DO.CenterY = pv.y;
	
	DO.Width = perticleSize_.x;
	DO.Height = perticleSize_.y;

	DO.ScaleX = scaleX(count_);
	DO.ScaleY = scaleY(count_);
	DO.Rotation = rotationBefore(count_);
	DO.Rotation2 = rotationAfter(count_);

	DO.m_color = color(count_);
	DO.m_color.formalize();

	DO.colorblendmode = (cDrawableObject::ColorBlendMode)colorBlendMode(count_);
	DO.AddingDraw = (cDrawableObject::DrawMode)drawMode(count_);
	
	TextureRangeRect range = textureRange(count_);
	DO.m_TexRange.setLTRB(range.x, range.y, range.z, range.w);

	DO.Draw(pDev);
}
void AnimeChip_Perticle::setTexture(const tstring& textureFileName)
{
	DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(sg_pDungeonSystem->pDevice_D3D, textureFileName.c_str()));
}
void AnimeChip_Perticle::setPerticleSize(const D3DXVECTOR2& perticleSize)
{
	perticleSize_ = perticleSize;
}

//------------------------------------------------------
AnimeChip_Num::AnimeChip_Num(const int life):
	fontSize(16.0),
		captionValue(0),
		fontIndex(0),
		fontSeparateDistance(0)
{
	setLife(life);
}

AnimeChip_Num::~AnimeChip_Num()
{
	return;
}
void AnimeChip_Num::selfProcess()
{
	return;
}
void AnimeChip_Num::selfDraw(IDirect3DDevice9 *pDev)
{
	D3DXVECTOR3 pos = worldPosition();
	c4DVector pv = sg_pDungeonSystem->Map().visibleplace( c4DVector(pos.x, pos.y, pos.z, 0));
	
	num_.position = pv;

	double fontsize_temp = fontSize(count_);
	num_.FontWidthSize = static_cast<int>(fontsize_temp * scaleX(count_) / 2);
	num_.FontHeightSize = static_cast<int>(fontsize_temp * scaleY(count_));

	num_.color = color(count_);
	num_.color.formalize();

	num_.colorBlendMode = colorBlendMode(count_);
	num_.drawMode = drawMode(count_);
	
	num_.arrangeX = arrangeX_;
	num_.arrangeY = arrangeY_;

	num_.Fontindex = fontIndex(count_);
	num_.distance = fontSeparateDistance(count_);
	num_.Value_ = captionValue(count_);

	num_.Draw(pDev);
}
/*
void AnimeChip_Num::setTexture(const tstring& textureFileName)
{
	DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(sg_pDungeonSystem->pDevice_D3D, textureFileName.c_str()));
}
*/
void AnimeChip_Num::setArrangeX(cNumField::PositionArrangeX arrangeX)
{
	arrangeX_ = arrangeX;
}
void AnimeChip_Num::setArrangeY(cNumField::PositionArrangeY arrangeY)
{
	arrangeY_ = arrangeY;
}
//------------------------------------------------------
AnimeChip_Belt::AnimeChip_Belt(const int life):
		Accuracy_(16),
		drawRange(BeltRange(0,1)),
		leftEdgeStartingPositionalPoint_(PositionPerGrid(0,0,0)),
		rightEdgeStartingPositionalPoint_(PositionPerGrid(0,0,0)),
		loop_(0)
{
	setLife(life);
}
AnimeChip_Belt::AnimeChip_Belt(const pLuaString filename,
							   const PositionPerGrid& leftStartingPoint,
							   const PositionPerGrid& rightStartingPoint,
							   const int life):
		Accuracy_(16),		
		drawRange(BeltRange(0,1)),
		leftEdgeStartingPositionalPoint_(leftStartingPoint),
		rightEdgeStartingPositionalPoint_(rightStartingPoint),
		loop_(0)
{
	setLife(life);
	AnimeChip_DrawableObject::setTexture(filename);
}
AnimeChip_Belt::~AnimeChip_Belt()
{
	return;
}
void AnimeChip_Belt::selfProcess()
{
	return;
}
void AnimeChip_Belt::selfDraw(IDirect3DDevice9 *pDev)
{
	D3DXVECTOR3 pos = worldPosition();
	c4DVector pv;
	compatible(pv, pos);
	//c4DVector pv = sg_pDungeonSystem->Map().visibleplace( c4DVector(pos.x, pos.y, pos.z, 0));
	
	//DOb.CenterX = pv.x;
	//DOb.CenterY = pv.y;
	
	DOb.m_color = color(count_);
	DOb.m_color.formalize();

	DOb.colorblendmode = (cDrawableObject::ColorBlendMode)colorBlendMode(count_);
	DOb.AddingDraw = (cDrawableObject::DrawMode)drawMode(count_);
	
	DOb.Accuracy = Accuracy_;
	DOb.rightline().deleteCurveTrack();
	DOb.leftline().deleteCurveTrack();

	TextureRangeRect tex_range = textureRange(count_);
	DOb.m_TexRange.setLTRB(tex_range.x, tex_range.y, tex_range.z, tex_range.w);

	int num = min(leftEdge.size(), rightEdge.size());
	int i;
	/*
	c4DVector position_right_base = sg_pDungeonSystem->Map().visibleplace(
		c4DVector(rightEdgeStartingPositionalPoint_.x + pos.x,
		rightEdgeStartingPositionalPoint_.y + pos.y,
		rightEdgeStartingPositionalPoint_.z + pos.z, 0));
	c4DVector position_left_base = sg_pDungeonSystem->Map().visibleplace( 
		c4DVector(leftEdgeStartingPositionalPoint_.x + pos.x,
		leftEdgeStartingPositionalPoint_.y + pos.y,
		leftEdgeStartingPositionalPoint_.z + pos.z, 0));
		*/
	c4DVector position_right_base, position_left_base;

	compatible(position_right_base, rightEdgeStartingPositionalPoint_);
	compatible(position_left_base, leftEdgeStartingPositionalPoint_);
	//position_right_base.turn(worldRotation_);
	//position_left_base.turn(worldRotation_);

	DOb.rightStartpoint() = position_right_base;
	DOb.leftStartpoint() = position_left_base;

	c4DVector position_right;
	c4DVector position_left;
	PositionPerGrid position_Pergrid_right;
	PositionPerGrid position_Pergrid_left;

	for(i=0;i<num;++i)
	{
		position_Pergrid_left = leftEdge[i](count_);
		position_Pergrid_right = rightEdge[i](count_);
		compatible(position_right, position_Pergrid_right);
		compatible(position_left, position_Pergrid_left);
		position_right += position_right_base;
		position_left += position_left_base;
		position_right.turn(worldRotation_);
		position_left.turn(worldRotation_);
		position_left.x *= scaleX(count_);
		position_right.x *= scaleX(count_);
		position_left.y *= scaleY(count_);
		position_right.y *= scaleY(count_);

		position_right += pv;
		position_left += pv;

		position_right = sg_pDungeonSystem->Map().visibleplace(position_right);
		position_left = sg_pDungeonSystem->Map().visibleplace(position_left);

		DOb.rightline().addPoint(position_right);
		DOb.leftline().addPoint(position_left);
	}

	DOb.rightline().setLoop(loop_);
	DOb.leftline().setLoop(loop_);

	if(num > 0)
	{
		DOb.rightline().stabilizeTrack();
		DOb.leftline().stabilizeTrack();
	}

	BeltRange range = drawRange(count_);

	DOb.start = range.x+0.5;
	DOb.end = range.y+0.5;

	DOb.Draw(pDev);
}
void AnimeChip_Belt::setAccuracy(int accuracy)
{
	Accuracy_ = accuracy;
}
void AnimeChip_Belt::setLoop(bool loop)
{
	loop_ = loop;
}
void AnimeChip_Belt::setLeftEdgeStartingPositionalPoint(const PositionPerGrid& position)
{
	leftEdgeStartingPositionalPoint_ = position;
}
void AnimeChip_Belt::setRightEdgeStartingPositionalPoint(const PositionPerGrid& position)
{
	rightEdgeStartingPositionalPoint_ = position;
}
void AnimeChip_Belt::setTexture(const tstring& textureFileName)
{
	DOb.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(sg_pDungeonSystem->pDevice_D3D, textureFileName.c_str()));
}
bool AnimeChip_Belt::isOnScreenJudgeFunction()
{
	c4DVector position_right_base, position_left_base;
	compatible(position_right_base, rightEdgeStartingPositionalPoint_);
	compatible(position_left_base, leftEdgeStartingPositionalPoint_);
	PositionPerGrid position_Pergrid_left;
	PositionPerGrid position_Pergrid_right;
	c4DVector position_right;
	c4DVector position_left;

	if(!leftEdge.empty() && !rightEdge.empty() )
	{
		position_Pergrid_left = leftEdge.front()(leftEdge.front().sumLength());
		position_Pergrid_right = rightEdge.front()(0);
	}
	compatible(position_right, position_Pergrid_right);
	compatible(position_left, position_Pergrid_left);
	position_right += position_right_base;
	position_left += position_left_base;
	position_right.turn(worldRotation_);
	position_left.turn(worldRotation_);
	compatible(position_Pergrid_right, position_right);
	compatible(position_Pergrid_left, position_left);

	PositionPerGrid parentPosition(0,0,0);
	if(parent())
	{
		parentPosition = parent()->worldPosition();
	}

	if(isOnScreenStatic(parentPosition + position_Pergrid_right,0) || isOnScreenStatic(parentPosition + position_Pergrid_left,0));
	{
		return true;
	}

	if(!leftEdge.empty() && !rightEdge.empty() )
	{
		position_Pergrid_left = leftEdge.back()(0);
		position_Pergrid_right = rightEdge.back()(rightEdge.back().sumLength());
	}
	compatible(position_right, position_Pergrid_right);
	compatible(position_left, position_Pergrid_left);
	position_right += position_right_base;
	position_left += position_left_base;
	position_right.turn(worldRotation_);
	position_left.turn(worldRotation_);
	compatible(position_Pergrid_right, position_right);
	compatible(position_Pergrid_left, position_left);

	return isOnScreenStatic(parentPosition + position_Pergrid_right,0) || isOnScreenStatic(parentPosition + position_Pergrid_left,0);
}
bool AnimeChip_Belt::isOnScreenNowJudgeFunction()
{
	c4DVector position_right_base, position_left_base;
	compatible(position_right_base, rightEdgeStartingPositionalPoint_);
	compatible(position_left_base, leftEdgeStartingPositionalPoint_);
	PositionPerGrid position_Pergrid_left;
	PositionPerGrid position_Pergrid_right;
	c4DVector position_right;
	c4DVector position_left;

	if(!leftEdge.empty() && !rightEdge.empty() )
	{
		position_Pergrid_left = leftEdge.front()(count_);
		position_Pergrid_right = rightEdge.front()(count_);
	}
	compatible(position_right, position_Pergrid_right);
	compatible(position_left, position_Pergrid_left);
	position_right += position_right_base;
	position_left += position_left_base;
	position_right.turn(worldRotation_);
	position_left.turn(worldRotation_);
	compatible(position_Pergrid_right, position_right);
	compatible(position_Pergrid_left, position_left);

	PositionPerGrid parentPosition(0,0,0);
	if(parent())
	{
		parentPosition = parent()->worldPosition();
	}

	if(isOnScreenStatic(parentPosition + position_Pergrid_right,0) || isOnScreenStatic(parentPosition + position_Pergrid_left,0));
	{
		return true;
	}

	if(!leftEdge.empty() && !rightEdge.empty() )
	{
		position_Pergrid_left = leftEdge.back()(count_);
		position_Pergrid_right = rightEdge.back()(count_);
	}
	compatible(position_right, position_Pergrid_right);
	compatible(position_left, position_Pergrid_left);
	position_right += position_right_base;
	position_left += position_left_base;
	position_right.turn(worldRotation_);
	position_left.turn(worldRotation_);
	compatible(position_Pergrid_right, position_right);
	compatible(position_Pergrid_left, position_left);

	return isOnScreenStatic(parentPosition + position_Pergrid_right,0) || isOnScreenStatic(parentPosition + position_Pergrid_left,0);

}
//------------------------------------------------------
AnimeChip_Bound::AnimeChip_Bound(const int life):
	AnimeChip_Perticle(life),
	boundLimit_(100),
	gravity(0.05),
	floorHeight(0),
	restitution(0.8),
	boundCount_(0),
	airResistance(0.02),
	extraPosition_(0,0,0),
	speedVector_(0,-1,-1)
{
}
	AnimeChip_Bound::AnimeChip_Bound(const pLuaString filename, const PerticleSize& perticleSize, const PositionPerGrid& startVector, const int life):
	AnimeChip_Perticle(filename,perticleSize,life),
	boundLimit_(100),
	gravity(8),
	floorHeight(0),
	restitution(0.8),
	boundCount_(0),
	airResistance(0.02),
	extraPosition_(0,0,0),
	speedVector_(startVector)
{
}


AnimeChip_Bound::~AnimeChip_Bound()
{
	return;
}
/*
void AnimeChip_Bound::selfDraw(IDirect3DDevice9 *pDev)
{

	return;
}
*/
void AnimeChip_Bound::selfProcess()
{
	extraPosition_ += speedVector_/FRAMEPARSECOND;
	speedVector_.z -= gravity(count_)/FRAMEPARSECOND;
	speedVector_ *= (1-airResistance(count_));
	if(extraPosition_.z < floorHeight(count_) && speedVector_.z < 0)
	{
		speedVector_.z = -speedVector_.z * restitution(count_);
		++boundCount_;
	}
	return;
}
bool AnimeChip_Bound::selfEnd()
{
	return AnimeChip_Perticle::selfEnd() || (boundCount_ >= boundLimit_);
}
PositionPerGrid AnimeChip_Bound::localPosition()
{
	PositionPerGrid local = extraPosition_;
	turnOnHorizon(local, worldRotation_);
	return AnimeChip_Perticle::localPosition() + local;
}
void AnimeChip_Bound::setStartVector(const PositionPerGrid& startVector)
{
	speedVector_ = startVector; 
}
void AnimeChip_Bound::setBoundLimit(const int boundLimit)
{
	boundLimit_ = boundLimit;
}

//------------------------------------------------------
AnimeChip_Sound::AnimeChip_Sound():
	didSoundFrag_(0),
	volume_(0.0),
	position_(PositionPerGrid(0,0,0))
{
}

AnimeChip_Sound::AnimeChip_Sound(const tstring& soundFileName,
								 const PositionPerGrid& position,
								 const double volume):
	didSoundFrag_(0),
	volume_(0.0),
	position_(PositionPerGrid(0,0,0))
{
	setSoundFile(soundFileName);
	setPosition(position);
	setVolume(volume);
}
AnimeChip_Sound::AnimeChip_Sound(const pLuaString soundFileName,
								 const PositionPerGrid& position,
								 const double volume):
	didSoundFrag_(0),
	volume_(0.0),
	position_(PositionPerGrid(0,0,0))
{
	setSoundFile(soundFileName);
	setPosition(position);
	setVolume(volume);
}

AnimeChip_Sound::~AnimeChip_Sound()
{
	return;
}
void AnimeChip_Sound::selfDraw(IDirect3DDevice9 *pDev)
{

	return;
}
void AnimeChip_Sound::selfProcess()
{
	if(didSoundFrag_ == 0)
	{
		pcaSoundPlay chip(new caSoundPlay);

		PositionPerGrid pos = worldPosition();

		int ID;
		if((ID = g_GameEnv.m_SoundManager.getSoundEffectFromFile(soundFileName_.c_str())) == -1)
		{
			OnAssert(_T(__FILE__),__LINE__,false,_T("Soundファイルが見つかりません。"),_T(""));
			didSoundFrag_ = 1;
			return;
		}
		pcaSoundPlay caso = pcaSoundPlay(new caSoundPlay);
		caso->play_ID = ID;
		caso->placeX = pos.x;
		caso->placeY = pos.y;

		sg_pDungeonSystem->AnimationManager().AddAnime_realtime(boost::static_pointer_cast<cAnimation>(caso));

		didSoundFrag_ = 1;
	}
	return;
}
void AnimeChip_Sound::setSoundFile(const tstring& soundFileName)
{
	soundFileName_ = soundFileName;
}
void AnimeChip_Sound::setSoundFile(const pLuaString soundFileName)
{
	LuaStringTransformer luastring(soundFileName);
	soundFileName_ = luastring.t_str();
}
void AnimeChip_Sound::setPosition(const PositionPerGrid& position)
{
	position_ = position;
}
void AnimeChip_Sound::setVolume(double volume)
{
	volume_ = volume;
}
bool AnimeChip_Sound::selfEnd()
{
	return didSoundFrag_;
}
PositionPerGrid AnimeChip_Sound::localPosition()
{
	return position_;
}
bool AnimeChip_Sound::isOnScreenJudgeFunction()
{
	return true;
}
bool AnimeChip_Sound::isOnScreenNowJudgeFunction()
{
	return true;
}
//------------------------------------------------------
AnimeChip_Chara::AnimeChip_Chara(pcCharacter pchara, const int life):pchara_(pchara),
scaleX(1), scaleY(1), pitching(0), yawing(0), position(PositionPerGrid(0,0,0))//, opaque(1.0)
{
	setLife(life);
}

AnimeChip_Chara::~AnimeChip_Chara()
{
	return;
}
void AnimeChip_Chara::selfProcess()
{
	if(pchara_ == NULL) return;
	pchara_->anime_pitching = pitching(count_);
	pchara_->anime_scaleX = scaleX(count_);
	pchara_->anime_scaleY = scaleY(count_);
	pchara_->anime_yawing = yawing(count_);

	//pchara_->opaque = opaque(count_);


	PositionPerGrid pos = position(count_);
	pchara_->anime_position.set(pos.x, pos.y, pos.z, 0);
	pchara_->anime_position.turn(worldRotation_);
	return;
}
void AnimeChip_Chara::selfDraw(IDirect3DDevice9 *pDev)
{
	return;
}
void AnimeChip_Chara::selfProcessFinalize()
{
	if(pchara_ == NULL) return;
	pchara_->anime_yawing = 0;
	pchara_->anime_pitching = 0;
	pchara_->anime_scaleX = 1;
	pchara_->anime_scaleY = 1;

	pchara_->anime_position.set(0, 0, 0, 0);
}
pAnimeChip_Chara AnimeChip_Chara::new_shared_ptr(pcCharacter pchara, const int life)
{
	pAnimeChip_Chara chara = pAnimeChip_Chara(new AnimeChip_Chara(pchara,life));
	return chara;
};
PositionPerGrid AnimeChip_Chara::localPosition()
{
	return position(count_);
}
bool AnimeChip_Chara::isOnScreenJudgeFunction()
{
	PositionPerGrid parentPosition(0,0,0);
	//if(parent())
	{
		parentPosition = PositionPerGrid(pchara_->visibleplace.x, pchara_->visibleplace.y ,0);//parent()->worldPosition();
	}
	PositionPerGrid beginPosition = position.value(0);
	turnOnHorizon(beginPosition,worldRotation_);
	if(isOnScreenStatic(parentPosition + beginPosition,0))
	{
		return true;
	}
	PositionPerGrid endPosition = position.value(position.sumLength());
	turnOnHorizon(endPosition, worldRotation_);
	return isOnScreenStatic(parentPosition + endPosition,0);
}
bool AnimeChip_Chara::isOnScreenNowJudgeFunction()
{
	PositionPerGrid parentPosition(pchara_->visibleplace.x, pchara_->visibleplace.y ,0);
	PositionPerGrid pos = position.value(count_);
	turnOnHorizon(pos, worldRotation_);
	return isOnScreenStatic(parentPosition + pos,0);
}
//------------------------------------------------------
AnimeChip_Quake::AnimeChip_Quake(const int life):
	amplitude(8),
	periodicity(8),
	direction(0),
	random(1),
	QuakedX_(0),
	QuakedY_(0),
	phase_(0),
	quakeDirection_(0),
	quakeDirection_phase_(0)
{
	setLife(life);
}

AnimeChip_Quake::~AnimeChip_Quake()
{
	return;
}
void AnimeChip_Quake::selfProcess()
{
	double amp = amplitude(count_);//振幅
	double prd = periodicity(count_);//周期

	double dir = direction(count_);//ゆれ方向
	double rnd = random(count_);//ゆれ方向の誤差

	phase_ += M_PI*2/prd;

	quakeDirection_phase_ += (rand()/(double)RAND_MAX);
	quakeDirection_ += sin(quakeDirection_phase_)*rnd*360*0.1;//適当補正

	double d = sin(phase_);

	double x, y;
	x = cos((dir + quakeDirection_ + worldRotation_ -90)*M_PI/180)*d*amp / (MAPTEXBOXSIZE * MAPTEXPOWER);
	y = sin((dir + quakeDirection_ + worldRotation_ -90)*M_PI/180)*d*amp / (MAPTEXBOXSIZE * MAPTEXPOWER);

	//キャンセル
	sg_pDungeonSystem->Map().additinal_visibleForcusX() -= QuakedX_;
	sg_pDungeonSystem->Map().additinal_visibleForcusY() -= QuakedY_;

	sg_pDungeonSystem->Map().additinal_visibleForcusX() += x;
	sg_pDungeonSystem->Map().additinal_visibleForcusY() += y;

	QuakedX_ = x;
	QuakedY_ = y;

	return;
}
void AnimeChip_Quake::selfDraw(IDirect3DDevice9 *pDev)
{
	return;
}
void AnimeChip_Quake::selfProcessFinalize()
{
	//キャンセル
	sg_pDungeonSystem->Map().additinal_visibleForcusX() -= QuakedX_;
	sg_pDungeonSystem->Map().additinal_visibleForcusY() -= QuakedY_;
	QuakedX_ = 0;
	QuakedY_ = 0;
}
PositionPerGrid AnimeChip_Quake::localPosition()
{
	return PositionPerGrid(0,0,0);
}