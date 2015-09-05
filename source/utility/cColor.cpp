#include "cColor.h"
cColor::cColor(int a, int r, int g, int b):
	red(r),
	green(g),
	blue(b),
	alpha(a)
{
	red = r;
	green = g;
	blue = b;
	alpha = a;
}
cColor::cColor(unsigned long d3dcolor)
{
	inputD3Dcolor(d3dcolor);
}
cColor::cColor():
	red(0),
	green(0),
	blue(0),
	alpha(255)
{
}
cColor::~cColor(void)
{
}

cColor cColor::operator *(const cColor& rother)
{
	cColor result_c;
	result_c.alpha = alpha * rother.alpha / 255;
	result_c.red = red * rother.red / 255;
	result_c.green = green * rother.green / 255;
	result_c.blue = blue * rother.blue / 255;
	return result_c;
}
cColor cColor::operator +(const cColor& rother)
{
	return cColor(rother.alpha + alpha, 
		rother.red + red, 
		rother.green + green, 
		rother.blue + blue
		);
}
void cColor::operator +=(const cColor& rother)
{
	alpha += rother.alpha;
	red += rother.red;
	green += rother.green;
	blue += rother.blue;
}

cColor operator*(double power, const cColor& rother)
{
	return cColor(rother.alpha*power, rother.red*power, rother.green*power, rother.blue*power);
}

void cColor::operator *=(const cColor& rother)
{
	alpha = alpha * rother.alpha / 255;
	red = red * rother.red / 255;
	green = green * rother.green / 255;
	blue = blue * rother.blue / 255;
}

bool cColor::operator ==(const cColor& rother)const
{
	bool flag = (*this).red == rother.red;
	flag &= (*this).green == rother.green;
	flag &= (*this).blue == rother.blue;
	flag &= (*this).alpha == rother.alpha;
	return flag;
}

void cColor::formalize()
{
	if(red < 0) red = 0;
	else if(red > 255) red = 255;
	if(green < 0) green = 0;
	else if(green > 255) green = 255;
	if(blue < 0) blue = 0;
	else if(blue > 255) blue = 255;
	if(alpha < 0) alpha = 0;
	else if(alpha > 255) alpha = 255;

	return;
}

//色相、彩度、明度
void cColor::HSV(double h, double s, double v)
{
	h -= (int)(h/360) * 360.0;
	if(s < 0) s = 0;
	else if(s > 1) s = 1;
	if(v < 0) v = 0;
	else if(v > 1) v = 1;
	int h_i = (int)(h/60) % 6;
	double f = h/60 - h_i;
	int p = (int)(v*(1-s)*255);
	int q = (int)(v*(1-f*s)*255);
	int t = (int)(v*(1-(1-f)*s)*255);
	int v_int = (int)(v*255);
	if(h_i == 0)(red = v_int, green = t, blue = p);
	else if(h_i == 1)(red = q, green = v_int, blue = p);
	else if(h_i == 2)(red = p, green = v_int, blue = t);
	else if(h_i == 3)(red = p, green = q, blue = v_int);
	else if(h_i == 4)(red = t, green = p, blue = v_int);
	else if(h_i == 5)(red = v_int, green = p, blue = q);
}

void cColor::ARGB(int a, int r, int g, int b)
{
	alpha = a;
	red = r;
	green = g;
	blue = b;
}

unsigned long cColor::D3Dcolor()
{
	formalize();

	return ((unsigned long)((((alpha)&0xff)<<24)|(((red)&0xff)<<16)|(((green)&0xff)<<8)|((blue)&0xff)));
}

void cColor::inputD3Dcolor(unsigned long d3dcolor)
{
	ARGB( ((d3dcolor)>>24)&0xff ,
		  ((d3dcolor)>>16)&0xff ,
		  ((d3dcolor)>>8 )&0xff ,
		  ((d3dcolor))&0xff
		  );
}