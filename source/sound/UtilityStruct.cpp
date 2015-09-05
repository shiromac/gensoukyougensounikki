#include "stdafx.h"
#include "UtilityStruct.h"
#include <math.h>
#include <stdio.h>

#define PI    (3.141592654f)

//---------------------------------------------------------
// Vec2i
//---------------------------------------------------------
Vec2i::Vec2i(const int x, const int y)
{
	this->x = x;
	this->y = y;
}
Vec2i Vec2i::operator + (const Vec2i& arg) const {
	Vec2i value;
	value.x = this->x + arg.x;
	value.y = this->y + arg.y;
	return value;
}
Vec2i Vec2i::operator - (const Vec2i& arg) const {
	Vec2i value;
	value.x = this->x - arg.x;
	value.y = this->y - arg.y;
	return value;
}
Vec2i Vec2i::operator * (const Vec2i& arg) const {
	Vec2i value;
	value.x = this->x * arg.x;
	value.y = this->y * arg.y;
	return value;
}
Vec2i Vec2i::operator / (const Vec2i& arg) const {
	Vec2i value(0, 0);
	if(arg.x != 0)
		value.x = this->x / arg.x;
	if(arg.y != 0)
		value.y = this->y / arg.y;
	return value;
}
Vec2i Vec2i::operator + (const int& arg) const {
	Vec2i value;
	value.x = this->x + arg;
	value.y = this->y + arg;
	return value;
}
Vec2i Vec2i::operator - (const int& arg) const {
	Vec2i value;
	value.x = this->x - arg;
	value.y = this->y - arg;
	return value;
}
Vec2i Vec2i::operator * (const int& arg) const {
	Vec2i value;
	value.x = this->x * arg;
	value.y = this->y * arg;
	return value;
}
Vec2i Vec2i::operator / (const int& arg) const {
	Vec2i value(0, 0);
	if(arg != 0)
	{
		value.x = this->x / arg;
		value.y = this->y / arg;
	}
	return value;
}
void Vec2i::operator +=(const Vec2i &arg) {
	*this = *this + arg;
}
void Vec2i::operator -=(const Vec2i &arg) {
	*this = *this - arg;
}
void Vec2i::operator *=(const Vec2i &arg) {
	*this = *this * arg;
}
void Vec2i::operator /=(const Vec2i &arg) {
	*this = *this / arg;
}
void Vec2i::operator +=(const int &arg) {
	*this = *this + arg;
}
void Vec2i::operator -=(const int &arg) {
	*this = *this - arg;
}
void Vec2i::operator *=(const int &arg) {
	*this = *this * arg;
}
void Vec2i::operator /=(const int &arg) {
	*this = *this / arg;
}

//---------------------------------------------------------
// Vec2f
//---------------------------------------------------------
Vec2f::Vec2f(const float x, const float y)
{
	this->x = x;
	this->y = y;
}
Vec2f Vec2f::operator + (const Vec2f& arg) const {
	Vec2f value;
	value.x = this->x + arg.x;
	value.y = this->y + arg.y;
	return value;
}
Vec2f Vec2f::operator - (const Vec2f& arg) const {
	Vec2f value;
	value.x = this->x - arg.x;
	value.y = this->y - arg.y;
	return value;
}
Vec2f Vec2f::operator * (const Vec2f& arg) const {
	Vec2f value;
	value.x = this->x * arg.x;
	value.y = this->y * arg.y;
	return value;
}
Vec2f Vec2f::operator / (const Vec2f& arg) const {
	Vec2f value(0, 0);
	if(arg.x != 0)
		value.x = this->x / arg.x;
	if(arg.y != 0)
		value.y = this->y / arg.y;
	return value;
}
Vec2f Vec2f::operator + (const float& arg) const {
	Vec2f value;
	value.x = this->x + arg;
	value.y = this->y + arg;
	return value;
}
Vec2f Vec2f::operator - (const float& arg) const {
	Vec2f value;
	value.x = this->x - arg;
	value.y = this->y - arg;
	return value;
}
Vec2f Vec2f::operator * (const float& arg) const {
	Vec2f value;
	value.x = this->x * arg;
	value.y = this->y * arg;
	return value;
}
Vec2f Vec2f::operator / (const float& arg) const {
	Vec2f value(0, 0);
	if(arg != 0)
	{
		value.x = this->x / arg;
		value.y = this->y / arg;
	}
	return value;
}
Vec2f Vec2f::operator - () const {
	Vec2f value;
	value.x = -this->x;
	value.y = -this->y;
	return value;
}
void Vec2f::operator +=(const Vec2f &arg) {
	*this = *this + arg;
}
void Vec2f::operator -=(const Vec2f &arg) {
	*this = *this - arg;
}
void Vec2f::operator *=(const Vec2f &arg) {
	*this = *this * arg;
}
void Vec2f::operator /=(const Vec2f &arg) {
	*this = *this / arg;
}
void Vec2f::operator +=(const float &arg) {
	*this = *this + arg;
}
void Vec2f::operator -=(const float &arg) {
	*this = *this - arg;
}
void Vec2f::operator *=(const float &arg) {
	*this = *this * arg;
}
void Vec2f::operator /=(const float &arg) {
	*this = *this / arg;
}

//---------------------------------------------------------
// Vec3i
//---------------------------------------------------------
Vec3i::Vec3i(const int x, const int y, const int z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
Vec3i::Vec3i(const Vec2i xy, const int z)
{
	this->x = xy.x;
	this->y = xy.y;
	this->z = z;
}
Vec3i Vec3i::operator + (const Vec3i& arg) const {
	Vec3i value;
	value.x = this->x + arg.x;
	value.y = this->y + arg.y;
	value.z = this->z + arg.z;
	return value;
}
Vec3i Vec3i::operator - (const Vec3i& arg) const {
	Vec3i value;
	value.x = this->x - arg.x;
	value.y = this->y - arg.y;
	value.z = this->z - arg.z;
	return value;
}
Vec3i Vec3i::operator * (const Vec3i& arg) const {
	Vec3i value;
	value.x = this->x * arg.x;
	value.y = this->y * arg.y;
	value.z = this->z * arg.z;
	return value;
}
Vec3i Vec3i::operator / (const Vec3i& arg) const {
	Vec3i value(0, 0, 0);
	if(arg.x != 0)
		value.x = this->x / arg.x;
	if(arg.y != 0)
		value.y = this->y / arg.y;
	if(arg.z != 0)
		value.z = this->z / arg.z;
	return value;
}
Vec3i Vec3i::operator + (const int& arg) const {
	Vec3i value;
	value.x = this->x + arg;
	value.y = this->y + arg;
	value.z = this->z + arg;
	return value;
}
Vec3i Vec3i::operator - (const int& arg) const {
	Vec3i value;
	value.x = this->x - arg;
	value.y = this->y - arg;
	value.z = this->z - arg;
	return value;
}
Vec3i Vec3i::operator * (const int& arg) const {
	Vec3i value;
	value.x = this->x * arg;
	value.y = this->y * arg;
	value.z = this->z * arg;
	return value;
}
Vec3i Vec3i::operator / (const int& arg) const {
	Vec3i value(0, 0, 0);
	if(arg != 0)
	{
		value.x = this->x / arg;
		value.y = this->y / arg;
		value.z = this->z / arg;
	}
	return value;
}
void Vec3i::operator +=(const Vec3i &arg) {
	*this = *this + arg;
}
void Vec3i::operator -=(const Vec3i &arg) {
	*this = *this - arg;
}
void Vec3i::operator *=(const Vec3i &arg) {
	*this = *this * arg;
}
void Vec3i::operator /=(const Vec3i &arg) {
	*this = *this / arg;
}
void Vec3i::operator +=(const int &arg) {
	*this = *this + arg;
}
void Vec3i::operator -=(const int &arg) {
	*this = *this - arg;
}
void Vec3i::operator *=(const int &arg) {
	*this = *this * arg;
}
void Vec3i::operator /=(const int &arg) {
	*this = *this / arg;
}

//---------------------------------------------------------
// Vec3f
//---------------------------------------------------------
Vec3f::Vec3f(const float x, const float y, const float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
Vec3f::Vec3f(const Vec2f xy, const float z)
{
	this->x = xy.x;
	this->y = xy.y;
	this->z = z;
}
Vec3f::Vec3f(const float length, const Vec2f degree)
{
	SetDegree(length, degree);
}
Vec3f Vec3f::operator + (const Vec3f& arg) const {
	Vec3f value;
	value.x = this->x + arg.x;
	value.y = this->y + arg.y;
	value.z = this->z + arg.z;
	return value;
}
Vec3f Vec3f::operator - (const Vec3f& arg) const {
	Vec3f value;
	value.x = this->x - arg.x;
	value.y = this->y - arg.y;
	value.z = this->z - arg.z;
	return value;
}
Vec3f Vec3f::operator * (const Vec3f& arg) const {
	Vec3f value;
	value.x = this->x * arg.x;
	value.y = this->y * arg.y;
	value.z = this->z * arg.z;
	return value;
}
Vec3f Vec3f::operator / (const Vec3f& arg) const {
	Vec3f value(0, 0, 0);
	if(arg.x != 0)
		value.x = this->x / arg.x;
	if(arg.y != 0)
		value.y = this->y / arg.y;
	if(arg.z != 0)
		value.z = this->z / arg.z;
	return value;
}
Vec3f Vec3f::operator + (const float& arg) const {
	Vec3f value;
	value.x = this->x + arg;
	value.y = this->y + arg;
	value.z = this->z + arg;
	return value;
}
Vec3f Vec3f::operator - (const float& arg) const {
	Vec3f value;
	value.x = this->x - arg;
	value.y = this->y - arg;
	value.z = this->z - arg;
	return value;
}
Vec3f Vec3f::operator * (const float& arg) const {
	Vec3f value;
	value.x = this->x * arg;
	value.y = this->y * arg;
	value.z = this->z * arg;
	return value;
}
Vec3f Vec3f::operator / (const float& arg) const {
	Vec3f value(0, 0, 0);
	if(arg != 0)
	{
		value.x = this->x / arg;
		value.y = this->y / arg;
		value.z = this->z / arg;
	}
	return value;
}
Vec3f Vec3f::operator - () const {
	Vec3f value;
	value.x = -this->x;
	value.y = -this->y;
	value.z = -this->z;
	return value;
}
void Vec3f::operator +=(const Vec3f &arg) {
	*this = *this + arg;
}
void Vec3f::operator -=(const Vec3f &arg) {
	*this = *this - arg;
}
void Vec3f::operator *=(const Vec3f &arg) {
	*this = *this * arg;
}
void Vec3f::operator /=(const Vec3f &arg) {
	*this = *this / arg;
}
void Vec3f::operator +=(const float &arg) {
	*this = *this + arg;
}
void Vec3f::operator -=(const float &arg) {
	*this = *this - arg;
}
void Vec3f::operator *=(const float &arg) {
	*this = *this * arg;
}
void Vec3f::operator /=(const float &arg) {
	*this = *this / arg;
}
bool Vec3f::operator ==(const Vec3f& arg) const {
	bool xflag = this->x == arg.x;
	bool yflag = this->y == arg.y;
	bool zflag = this->z == arg.z;
	return xflag && yflag && zflag;
}
bool Vec3f::operator !=(const Vec3f& arg) const {
	return !(*this == arg);
}
Vec2f Vec3f::Getxy() const
{
	return Vec2f(this->x, this->y);
}
void Vec3f::SetDegree(float length, Vec2f degree)
{
	float theta = degree.x * (PI / 180);
	float omega = degree.y * (PI / 180);
	float lengthxz = length * cos(omega);
	this->x = lengthxz * sin(theta);
	this->y = length * sin(omega);
	this->z = lengthxz * cos(theta);
}
void Vec3f::GetDegree(float& length, Vec2f& degree)
{
	float theta = atan2(this->x, this->z);
	float lengthxz = this->z / cos(theta);
	float omega = atan2(this->y, lengthxz);
	length = lengthxz / cos(omega);
	degree.x = theta * (180 / PI);
	degree.y = omega * (180 / PI);
}
void Vec3f::PlusDegree(float pluslength, Vec2f plusdegree)
{
	float length;
	Vec2f degree;
	GetDegree(length, degree);
	SetDegree(length + pluslength, degree + plusdegree);
}

//---------------------------------------------------------
// Vec4i
//---------------------------------------------------------
Vec4i::Vec4i(const int x, const int y, const int z, const int w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}
Vec4i Vec4i::operator + (const Vec4i& arg) const {
	Vec4i value;
	value.x = this->x + arg.x;
	value.y = this->y + arg.y;
	value.z = this->z + arg.z;
	value.w = this->w + arg.w;
	return value;
}
Vec4i Vec4i::operator - (const Vec4i& arg) const {
	Vec4i value;
	value.x = this->x - arg.x;
	value.y = this->y - arg.y;
	value.z = this->z - arg.z;
	value.w = this->w - arg.w;
	return value;
}
Vec4i Vec4i::operator * (const Vec4i& arg) const {
	Vec4i value;
	value.x = this->x * arg.x;
	value.y = this->y * arg.y;
	value.z = this->z * arg.z;
	value.w = this->w * arg.w;
	return value;
}
Vec4i Vec4i::operator / (const Vec4i& arg) const {
	Vec4i value(0, 0, 0, 0);
	if(arg.x != 0)
		value.x = this->x / arg.x;
	if(arg.y != 0)
		value.y = this->y / arg.y;
	if(arg.z != 0)
		value.z = this->z / arg.z;
	if(arg.w != 0)
		value.w = this->w / arg.w;
	return value;
}
Vec4i Vec4i::operator + (const int& arg) const {
	Vec4i value;
	value.x = this->x + arg;
	value.y = this->y + arg;
	value.z = this->z + arg;
	value.w = this->w + arg;
	return value;
}
Vec4i Vec4i::operator - (const int& arg) const {
	Vec4i value;
	value.x = this->x - arg;
	value.y = this->y - arg;
	value.z = this->z - arg;
	value.w = this->w - arg;
	return value;
}
Vec4i Vec4i::operator * (const int& arg) const {
	Vec4i value;
	value.x = this->x * arg;
	value.y = this->y * arg;
	value.z = this->z * arg;
	value.w = this->w * arg;
	return value;
}
Vec4i Vec4i::operator / (const int& arg) const {
	Vec4i value(0, 0, 0, 0);
	if(arg != 0)
	{
		value.x = this->x / arg;
		value.y = this->y / arg;
		value.z = this->z / arg;
		value.w = this->w / arg;
	}
	return value;
}
void Vec4i::operator +=(const Vec4i &arg) {
	*this = *this + arg;
}
void Vec4i::operator -=(const Vec4i &arg) {
	*this = *this - arg;
}
void Vec4i::operator *=(const Vec4i &arg) {
	*this = *this * arg;
}
void Vec4i::operator /=(const Vec4i &arg) {
	*this = *this / arg;
}
void Vec4i::operator +=(const int &arg) {
	*this = *this + arg;
}
void Vec4i::operator -=(const int &arg) {
	*this = *this - arg;
}
void Vec4i::operator *=(const int &arg) {
	*this = *this * arg;
}
void Vec4i::operator /=(const int &arg) {
	*this = *this / arg;
}


//---------------------------------------------------------
// Vec4f
//---------------------------------------------------------
Vec4f::Vec4f(const float x, const float y, const float z, const float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}
Vec4f Vec4f::operator + (const Vec4f& arg) const {
	Vec4f value;
	value.x = this->x + arg.x;
	value.y = this->y + arg.y;
	value.z = this->z + arg.z;
	value.w = this->w + arg.w;
	return value;
}
Vec4f Vec4f::operator - (const Vec4f& arg) const {
	Vec4f value;
	value.x = this->x - arg.x;
	value.y = this->y - arg.y;
	value.z = this->z - arg.z;
	value.w = this->w - arg.w;
	return value;
}
Vec4f Vec4f::operator * (const Vec4f& arg) const {
	Vec4f value;
	value.x = this->x * arg.x;
	value.y = this->y * arg.y;
	value.z = this->z * arg.z;
	value.w = this->w * arg.w;
	return value;
}
Vec4f Vec4f::operator / (const Vec4f& arg) const {
	Vec4f value(0, 0, 0, 0);
	if(arg.x != 0)
		value.x = this->x / arg.x;
	if(arg.y != 0)
		value.y = this->y / arg.y;
	if(arg.z != 0)
		value.z = this->z / arg.z;
	if(arg.w != 0)
		value.w = this->w / arg.w;
	return value;
}
Vec4f Vec4f::operator + (const float& arg) const {
	Vec4f value;
	value.x = this->x + arg;
	value.y = this->y + arg;
	value.z = this->z + arg;
	value.w = this->w + arg;
	return value;
}
Vec4f Vec4f::operator - (const float& arg) const {
	Vec4f value;
	value.x = this->x - arg;
	value.y = this->y - arg;
	value.z = this->z - arg;
	value.w = this->w - arg;
	return value;
}
Vec4f Vec4f::operator * (const float& arg) const {
	Vec4f value;
	value.x = this->x * arg;
	value.y = this->y * arg;
	value.z = this->z * arg;
	value.w = this->w * arg;
	return value;
}
Vec4f Vec4f::operator / (const float& arg) const {
	Vec4f value(0, 0, 0, 0);
	if(arg != 0)
	{
		value.x = this->x / arg;
		value.y = this->y / arg;
		value.z = this->z / arg;
		value.w = this->w / arg;
	}
	return value;
}
void Vec4f::operator +=(const Vec4f &arg) {
	*this = *this + arg;
}
void Vec4f::operator -=(const Vec4f &arg) {
	*this = *this - arg;
}
void Vec4f::operator *=(const Vec4f &arg) {
	*this = *this * arg;
}
void Vec4f::operator /=(const Vec4f &arg) {
	*this = *this / arg;
}
void Vec4f::operator +=(const float &arg) {
	*this = *this + arg;
}
void Vec4f::operator -=(const float &arg) {
	*this = *this - arg;
}
void Vec4f::operator *=(const float &arg) {
	*this = *this * arg;
}
void Vec4f::operator /=(const float &arg) {
	*this = *this / arg;
}
bool Vec4f::operator ==(const Vec4f& arg) const {
	bool xflag = this->x == arg.x;
	bool yflag = this->y == arg.y;
	bool zflag = this->z == arg.z;
	bool wflag = this->w == arg.w;
	return xflag && yflag && zflag && wflag;
}
bool Vec4f::operator !=(const Vec4f& arg) const {
	return !(*this == arg);
}

//---------------------------------------------------------
// Box
//---------------------------------------------------------
Box::Box(const Vec3f pos, const Vec3f size)
{
	this->pos = pos;
	this->size = size;
}