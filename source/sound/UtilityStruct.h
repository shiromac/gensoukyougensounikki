//---------------------------------------------------------
// 便利構造体
//---------------------------------------------------------
#pragma once

// ベクトル関連
struct Vec2i {
	int x, y;
	Vec2i() {}
	Vec2i(const int x, const int y);
	Vec2i operator + (const Vec2i& arg) const;
	Vec2i operator - (const Vec2i& arg) const;
	Vec2i operator * (const Vec2i& arg) const;
	Vec2i operator / (const Vec2i& arg) const;
	Vec2i operator + (const int& arg) const;
	Vec2i operator - (const int& arg) const;
	Vec2i operator * (const int& arg) const;
	Vec2i operator / (const int& arg) const;
	void operator += (const Vec2i& arg);
	void operator -= (const Vec2i& arg);
	void operator *= (const Vec2i& arg);
	void operator /= (const Vec2i& arg);
	void operator += (const int& arg);
	void operator -= (const int& arg);
	void operator *= (const int& arg);
	void operator /= (const int& arg);
};

struct Vec2f {
	float x, y;
	Vec2f() {}
	Vec2f(const float x, const float y);
	Vec2f operator + (const Vec2f& arg) const;
	Vec2f operator - (const Vec2f& arg) const;
	Vec2f operator * (const Vec2f& arg) const;
	Vec2f operator / (const Vec2f& arg) const;
	Vec2f operator + (const float& arg) const;
	Vec2f operator - (const float& arg) const;
	Vec2f operator * (const float& arg) const;
	Vec2f operator / (const float& arg) const;
	Vec2f operator - () const;
	void operator += (const Vec2f& arg);
	void operator -= (const Vec2f& arg);
	void operator *= (const Vec2f& arg);
	void operator /= (const Vec2f& arg);
	void operator += (const float& arg);
	void operator -= (const float& arg);
	void operator *= (const float& arg);
	void operator /= (const float& arg);
};

struct Vec3i {
	int x, y, z;
	Vec3i() {}
	Vec3i(const int x, const int y, const int z);
	Vec3i(const Vec2i xy, const int z);
	Vec3i operator + (const Vec3i& arg) const;
	Vec3i operator - (const Vec3i& arg) const;
	Vec3i operator * (const Vec3i& arg) const;
	Vec3i operator / (const Vec3i& arg) const;
	Vec3i operator + (const int& arg) const;
	Vec3i operator - (const int& arg) const;
	Vec3i operator * (const int& arg) const;
	Vec3i operator / (const int& arg) const;
	void operator += (const Vec3i& arg);
	void operator -= (const Vec3i& arg);
	void operator *= (const Vec3i& arg);
	void operator /= (const Vec3i& arg);
	void operator += (const int& arg);
	void operator -= (const int& arg);
	void operator *= (const int& arg);
	void operator /= (const int& arg);
};

struct Vec3f {
	float x, y, z;
	Vec3f() {}
	Vec3f(const float x, const float y, const float z);
	Vec3f(const Vec2f xy, const float z);
	Vec3f(const float length, const Vec2f degree);
	Vec3f operator + (const Vec3f& arg) const;
	Vec3f operator - (const Vec3f& arg) const;
	Vec3f operator * (const Vec3f& arg) const;
	Vec3f operator / (const Vec3f& arg) const;
	Vec3f operator + (const float& arg) const;
	Vec3f operator - (const float& arg) const;
	Vec3f operator * (const float& arg) const;
	Vec3f operator / (const float& arg) const;
	Vec3f operator - () const;
	void operator += (const Vec3f& arg);
	void operator -= (const Vec3f& arg);
	void operator *= (const Vec3f& arg);
	void operator /= (const Vec3f& arg);
	void operator += (const float& arg);
	void operator -= (const float& arg);
	void operator *= (const float& arg);
	void operator /= (const float& arg);
	// 比較
	bool operator == (const Vec3f& arg) const;
	bool operator != (const Vec3f& arg) const;
	// 便利関数
	Vec2f Getxy() const;
	void SetDegree(float length, Vec2f degree);
	void GetDegree(float& length, Vec2f& degree);
	void PlusDegree(float pluslength, Vec2f plusdegree);
};

struct Vec4i {
	int x, y, z, w;
	Vec4i() {}
	Vec4i(const int x, const int y, const int z, const int w);
	Vec4i operator + (const Vec4i& arg) const;
	Vec4i operator - (const Vec4i& arg) const;
	Vec4i operator * (const Vec4i& arg) const;
	Vec4i operator / (const Vec4i& arg) const;
	Vec4i operator + (const int& arg) const;
	Vec4i operator - (const int& arg) const;
	Vec4i operator * (const int& arg) const;
	Vec4i operator / (const int& arg) const;
	void operator += (const Vec4i& arg);
	void operator -= (const Vec4i& arg);
	void operator *= (const Vec4i& arg);
	void operator /= (const Vec4i& arg);
	void operator += (const int& arg);
	void operator -= (const int& arg);
	void operator *= (const int& arg);
	void operator /= (const int& arg);
};

struct Vec4f {
	float x, y, z, w;
	Vec4f() {}
	Vec4f(const float x, const float y, const float z, const float w);
	Vec4f operator + (const Vec4f& arg) const;
	Vec4f operator - (const Vec4f& arg) const;
	Vec4f operator * (const Vec4f& arg) const;
	Vec4f operator / (const Vec4f& arg) const;
	Vec4f operator + (const float& arg) const;
	Vec4f operator - (const float& arg) const;
	Vec4f operator * (const float& arg) const;
	Vec4f operator / (const float& arg) const;
	void operator += (const Vec4f& arg);
	void operator -= (const Vec4f& arg);
	void operator *= (const Vec4f& arg);
	void operator /= (const Vec4f& arg);
	void operator += (const float& arg);
	void operator -= (const float& arg);
	void operator *= (const float& arg);
	void operator /= (const float& arg);
	// 比較
	bool operator == (const Vec4f& arg) const;
	bool operator != (const Vec4f& arg) const;
};

// 物体関連
struct Box {
	Vec3f pos;
	Vec3f size;
	Box() {}
	Box(const Vec3f pos, const Vec3f size);
};