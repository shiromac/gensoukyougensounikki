#include "stdafx.h"
#include "cEventBackground.h"

cEventBackground::cEventBackground(const int priority):cEvent(priority)
{
	_nowAlpha = 0;
}
cEventBackground::~cEventBackground()
{
	
}
void cEventBackground::DrawUnderObject()
{
	_cd.m_color.ARGB(_nowAlpha,255,255,255);
	_cd.m_color.formalize();

	c4DVector pv = sg_pDungeonSystem->Map().visibleplace( c4DVector(_position.x, _position.y, _position.z, 0));
	_cd.CenterX = pv.x;
	_cd.CenterY = pv.y;

	_cd.Draw(sg_pDungeonSystem->pDevice_D3D);
	
}

void cEventBackground::Draw()
{

}
void cEventBackground::Process()
{
	
	if(_fadingInOnt) {
		double ratio = (double)(_fadeCount+1)/ (double)(_durationTime+1);
		_nowAlpha = (1-ratio)*_startAlpha + (ratio)*_destinationAlpha;
		_fadeCount++;
		if(_fadeCount > _durationTime) {
			_fadingInOnt = false;
			_nowAlpha = _destinationAlpha;
		}
	}
	
}

void cEventBackground::fadeIn(const int duringFrame)
{
	
	_cd.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(sg_pDungeonSystem->pDevice_D3D, _fileName.c_str()));

	//_cd.Width = _perticleSize.x;
	//_cd.Height = _perticleSize.y;
	_cd.ScaleX = _power;
	_cd.ScaleY = _power;

	_cd.Width = _cd.getTexSizeX();
	_cd.Height = _cd.getTexSizeY();

	_cd.ScaleX = _power;
	_cd.ScaleY = _power;
	_cd.Rotation = 0;
	_cd.Rotation2 = 0;

	_cd.colorblendmode = cDrawableObject::COLOR_BLEND_MULTIPLE;
	_cd.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;
	
	_cd.m_TexRange.setLTRB(0, 0, 1, 1);

	
	_startAlpha = _nowAlpha;
	_durationTime = duringFrame;
	if(_durationTime < 0)
	{
		_durationTime = 0;
	}
	_fadeCount = 0;
	_fadingInOnt = true;
	_destinationAlpha = 255;
	
}
void cEventBackground::fadeOut(const int duringFrame)
{
	_startAlpha = _nowAlpha;
	_durationTime = duringFrame;
	if(_durationTime < 0)
	{
		_durationTime = 0;
	}
	_fadeCount = 0;
	_fadingInOnt = true;
	_destinationAlpha = 0;
}