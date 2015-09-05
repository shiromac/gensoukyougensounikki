#include "stdafx.h"

#include "caCamera.h"

caCameraMove::caCameraMove(double x, double y, double speed, double point1, double point2)
{
	x_ = EDGELANDWIDTH + x;
	y_ = EDGELANDWIDTH + y;
	//mode_ = mode;
	speed_ = speed;
	b_ = point1;
	c_ = point2;

	preprocess_ = 0;
}

caCameraMove::~caCameraMove(void)
{
}


int caCameraMove::Init()
{

	preprocess_ = 0;
	return true;
}
int caCameraMove::process(IDirect3DDevice9 *pDev)
{
	if(!preprocess_)
	{
		preforcusX_ = sg_pDungeonSystem->Map().visibleForcusX();
		preforcusY_ = sg_pDungeonSystem->Map().visibleForcusY();

		preprocess_ = 1;
		count_ = 0;
		time_frame_ = sqrt(pow(preforcusX_ - x_, 2) + pow(preforcusY_ - y_, 2));
		time_frame_ = sqrt(time_frame_*2);
	}

	count_ += speed_ / 16;

	if( !End && count_ < time_frame_ )
	{
		double d = (double)count_/time_frame_;

		double x_3 = x_*b_ + preforcusX_*(1-b_);
		double x_2 = x_*c_ + preforcusX_*(1-c_);
		double y_3 = y_*b_ + preforcusY_*(1-b_);
		double y_2 = y_*c_ + preforcusY_*(1-c_);

		double x = pow(d,3)*x_ + 3*pow(d,2)*(1-d)*x_2 + 3*pow(1-d,2)*(d)*x_3 + pow(1-d,3)*preforcusX_;
		double y = pow(d,3)*y_ + 3*pow(d,2)*(1-d)*y_2 + 3*pow(1-d,2)*(d)*y_3 + pow(1-d,3)*preforcusY_;
		sg_pDungeonSystem->Map().setVisibleForcusX(x);
		sg_pDungeonSystem->Map().setVisibleForcusY(y);
	}
	else
	{
		sg_pDungeonSystem->Map().setVisibleForcusX(x_);
		sg_pDungeonSystem->Map().setVisibleForcusY(y_);


		End = true;
	}


	
	return true;
}
int caCameraMove::Draw(IDirect3DDevice9 *pDev)
{
	return true;
}
bool caCameraMove::isOnScreen()
{

	return true;
}