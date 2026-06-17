
#include "4Dvector.h"
#include <math.h>


c4DVector::c4DVector():x(0),y(0),z(0),t(0)
{
}
c4DVector::c4DVector(const double valueX,const double valueY,const double valueZ,const double valueT)
:x(valueX),y(valueY),z(valueZ),t(valueT)
{
}

c4DVector::~c4DVector()
{
}


c4DVector& c4DVector::operator =(const c4DVector& rother)
{
	x = rother.x;
	y = rother.y;
	z = rother.z;
	t = rother.t;
	return *this;
}
void c4DVector::operator +=(const c4DVector& rother)
{
	x += rother.x;
	y += rother.y;
	z += rother.z;
	t += rother.t;
	return;
}
void c4DVector::operator -=(const c4DVector& rother)
{
	x -= rother.x;
	y -= rother.y;
	z -= rother.z;
	t -= rother.t;
	return;
}
void c4DVector::operator *=(const double& rother)
{
	x *= rother;
	y *= rother;
	z *= rother;
	t *= rother;
	return;
}
void c4DVector::operator /=(const double& rother)
{
	x /= rother;
	y /= rother;
	z /= rother;
	t /= rother;
	return;
}
bool c4DVector::operator ==(const c4DVector& rother)
{
	return (x == rother.x)&&(y == rother.y)&&(z == rother.z)&&(t == rother.t);
}
bool c4DVector::operator !=(const c4DVector& rother)
{
	return (x != rother.x)||(y != rother.y)||(z != rother.z)||(t != rother.t);
}
c4DVector c4DVector::operator +(const c4DVector& rother)
{
	c4DVector result_v;
	result_v.x = x + rother.x;
	result_v.y = y + rother.y;
	result_v.z = z + rother.z;
	result_v.t = t + rother.t;
	return result_v;
}
c4DVector c4DVector::operator -(const c4DVector& rother)
{
	c4DVector result_v;
	result_v.x = x - rother.x;
	result_v.y = y - rother.y;
	result_v.z = z - rother.z;
	result_v.t = t - rother.t;
	return result_v;
}
c4DVector c4DVector::operator *(const double& rother)
{
	c4DVector result_v;
	result_v.x = x * rother;
	result_v.y = y * rother;
	result_v.z = z * rother;
	result_v.t = t * rother;
	return result_v;
}
c4DVector c4DVector::operator /(const double& rother)
{
	c4DVector result_v;
	result_v.x = x / rother;
	result_v.y = y / rother;
	result_v.z = z / rother;
	result_v.t = t / rother;
	return result_v;
}
c4DVector c4DVector::operator -()
{
	c4DVector result_v;
	result_v.x = -x;
	result_v.y = -y;
	result_v.z = -z;
	result_v.t = -t;
	return result_v;
}
//c4DVector c4DVector::operator ~()
//{
//	_tprintf(_T("%ld:(%f,%f,%f,%f)\n"),this,x,y,z,t);
//	return *this;
//}
double c4DVector::operator *(const c4DVector& rother)
{
double result_d;
result_d = x*rother.x + y*rother.y + z*rother.z + t*rother.t;
return result_d;
}

void c4DVector::turn(const double degree,const dimension_No first_d,const dimension_No second_d)
{
    double rad = degree * V_PI / 180;
	c4DVector v = *this;
	double s = sin(rad),c = cos(rad);
	double*p,*q;
	p = (*this)(first_d);
	q = (*this)(second_d);
	*v(first_d) = (*p) * c - (*q) * s;
	*v(second_d) = (*p) * s + (*q) * c;

	*this = v;
}
void c4DVector::turn(const double degree)
{
    double rad = degree * V_PI / 180;
	c4DVector v = *this;
	double s = sin(rad),c = cos(rad);

	v.x = x * c - y * s;
	v.y = x * s + y * c;

	*this = v;
}

void c4DVector::turn90(void)
{
	c4DVector v;
	v.x = -y;
	v.y = x;
	*this = v;
}

void c4DVector::set(const double valueX,const double valueY,const double valueZ,const double valueT)
{
	x = valueX;
	y = valueY;
	z = valueZ;
	t = valueT;
	return;
}
/*
void c4DVector::set(const double valueX,const double valueY)
{
	x = valueX;
	y = valueY;
	return;
}
*/
double abs(const c4DVector v)
{
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z + v.t*v.t);
}

c4DVector unit(const c4DVector v)
{
	c4DVector result_v;
	double a = abs(v);
	result_v.x = v.x / a;
	result_v.y = v.y / a;
	result_v.z = v.z / a;
	result_v.t = v.t / a;
	return result_v;
}
double degree(c4DVector v,const dimension_No first_d,const dimension_No second_d)
{
	double result_d=0,r_cos;
	c4DVector p,q;
	*p(first_d) = 1;
	*q(first_d) = *v(first_d);
	*q(second_d) = *v(second_d);
	if(abs(q) != 0)
	{
		r_cos = ((p)*(unit(q)));
		result_d = 180*acos(r_cos)/V_PI;
		if(*q(second_d)<0)result_d = 360 - result_d;
	}
	else
	{
		result_d = -360;
	}
	return result_d;
}
double degree(const c4DVector v)
{
	double result_d=0,r_cos;
	c4DVector p,q;
	p.x = 1;
	q.x = v.x;
	q.y = v.y;
	if(abs(q) != 0)
	{
		r_cos = ((p)*(unit(q)));
		result_d = 180*acos(r_cos)/V_PI;
		if(q.y<0)result_d = 360 - result_d;
	}
	else
	{
		result_d = -360;
	}
	return result_d;
}
double degree(const c4DVector v1,const c4DVector v2)
{
	double result_d=0,r_cos;

	if(abs(v1) != 0 && abs(v2) != 0)
	{
		r_cos = unit(v1)*unit(v2);
		result_d = 180*acos(r_cos)/V_PI;
	}
	else
	{
		result_d = -360;
	}
	return result_d;
}


c4DVector exproduct(c4DVector v1, c4DVector v2,const dimension_No first_d,const dimension_No second_d,const dimension_No third_d)
{
	c4DVector result_v;
	*result_v(first_d) = (*v1(second_d))*(*v2(third_d)) - (*v1(third_d))*(*v2(second_d));
	*result_v(second_d) = (*v1(third_d))*(*v2(first_d)) - (*v1(first_d))*(*v2(third_d));
	*result_v(third_d) = (*v1(first_d))*(*v2(second_d)) - (*v1(second_d))*(*v2(first_d));
	return result_v;
}
c4DVector exproduct( c4DVector v1, c4DVector v2)
{
	c4DVector result_v;
	result_v.x = v1.y*v2.z - v1.z*v2.y;
	result_v.y = v1.z*v2.x - v1.x*v2.z;
	result_v.z = v1.x*v2.y - v1.y*v2.x;
	return result_v;
}

c4DVector operator*(const double rother, c4DVector v)
{
	c4DVector result_v;
	result_v.x = v.x * rother;
	result_v.y = v.y * rother;
	result_v.z = v.z * rother;
	result_v.t = v.t * rother;
	return result_v;
}

bool cross_Judge_Flat(c4DVector& a_begin, c4DVector& a_end, c4DVector& b_begin, c4DVector& b_end)
{
	c4DVector a = a_end - a_begin;
	c4DVector b = b_end - b_begin;
	c4DVector b_be2a_be = a_begin - b_begin;

	return ((exproduct( a, -b_be2a_be).z * exproduct( a, b_end - a_begin).z < 0)
	&& (exproduct( b, b_be2a_be).z * exproduct(b, a_end - b_begin).z < 0));
}
