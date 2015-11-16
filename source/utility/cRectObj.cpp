#include "cRectObj.h"

cRectObj::cRectObj(void)
{
	CenterX = 0;
	CenterY = 0;
	Width = 0;
	Height = 0;
}

cRectObj::cRectObj(double Left, double Top, double Right, double Bottom)
{
	setLTRB(Left, Top, Right, Bottom);
}


cRectObj::~cRectObj(void)
{
}

void cRectObj::Events()
{
}

double cRectObj::setLeft(double Left)
{
	CenterX = Left + Width/2;
	return Left;
}
double cRectObj::setRight(double Right)
{
	CenterX = Right - Width/2;
	return Right;
}
double cRectObj::setTop(double Top)
{
	CenterY = Top + Height/2;
	return Top;
}
double cRectObj::setBottom(double Bottom)
{
	CenterY = Bottom - Height/2;
	return Bottom;
}

void cRectObj::setLTRB(double Left, double Top, double Right, double Bottom)
{
	CenterX = (Left + Right)/2;
	CenterY = (Top + Bottom)/2;
	Width = Right - Left;
	Height = Bottom - Top;
}

double cRectObj::formLeft(double L)
{
	double R = Right();
	CenterX = (R + L)/2;
	Width = R - L;
	return L;
}
double cRectObj::formRight(double R)
{
	double L = Left();
	CenterX = (R + L)/2;
	Width = R - L;
	return R;
}
double cRectObj::formTop(double T)
{
	double B = Bottom();
	CenterY = (T + B)/2;
	Height = B - T;
	return T;
}
double cRectObj::formBottom(double B)
{
	double T = Top();
	CenterY = (T + B)/2;
	Height = B - T;
	return B;
}




cRectObj cRectObj::operator =(const cRectObj& rother)//=‰‰ŽZŽq
{
	CenterX = rother.CenterX;
	CenterY = rother.CenterY;
	Width = rother.Width;
	Height = rother.Height;
	return *this;
}