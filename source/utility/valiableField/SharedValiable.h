#pragma once

//shared_ptrのインスタンス版
//安全なポインタ参照を得られる

#include <boost/shared_ptr.hpp>


template<class T>
class SharedValiable_likeInt
{
public:
	typedef boost::shared_ptr<T> Pointer;

protected:
	Pointer pValiable_;

public:
	// 補助関数
	inline T& valiable()		{return *pValiable_;};
	inline T  valiable() const	{return *pValiable_;};
	inline Pointer pointer()	{return pValiable_;};
public:
	SharedValiable_likeInt()
		:pValiable_(new T)		{valiable() = 0;};
	SharedValiable_likeInt(const T& t)
		:pValiable_(new T)		{valiable() = t;};//非明示的変換
	// コピーコンストラクタ
	SharedValiable_likeInt(const SharedValiable_likeInt& sharedValiable)
		:pValiable_(new T)		{valiable() = sharedValiable.valiable();};

	virtual ~SharedValiable_likeInt()			{};
public:
	// キャスト
	operator T()	const		{return valiable();};
	operator T&()				{return valiable();};

	// 代入
	const T& operator=(const SharedValiable_likeInt& rother)
		{valiable() = rother.valiable(); return valiable();};
	const T& operator=(T& t)	{valiable() = t; return valiable();};

	// アドレス
	      Pointer operator&()		{return pValiable_;};
	const Pointer operator&()const	{return pValiable_;};

	// 単項算術演算
	T operator+() const		{return valiable();};
	T operator-() const		{return -valiable();};
	
	// 単項ビット演算
	T operator~()const		{return ~valiable();};
	
	// 単項論理演算
	bool operator!()const		{return !valiable();};
	
	// 前置インクリメント、前置デクリメント
	const T& operator++()	{++valiable(); return *this;};
	const T& operator--()	{--valiable(); return *this;};
	
	// 後置インクリメント、後置デクリメント
	T operator++(int)		{T w = valiable(); ++valiable(); return w;};
	T operator--(int)		{T w = valiable(); --valiable(); return w;};

	// 算術代入
	const SharedValiable_likeInt& operator+=(const SharedValiable_likeInt& b)	{valiable()+=b.valiable(); return *this;};
	const SharedValiable_likeInt& operator-=(const SharedValiable_likeInt& b)	{valiable()-=b.valiable(); return *this;};
	const SharedValiable_likeInt& operator*=(const SharedValiable_likeInt& b)	{valiable()*=b.valiable(); return *this;};
	const SharedValiable_likeInt& operator/=(const SharedValiable_likeInt& b)	{valiable()/=b.valiable(); return *this;};
	const SharedValiable_likeInt& operator+=(const T& t)	{valiable()+=t; return *this;};
	const SharedValiable_likeInt& operator-=(const T& t)	{valiable()-=t; return *this;};
	const SharedValiable_likeInt& operator*=(const T& t)	{valiable()*=t; return *this;};
	const SharedValiable_likeInt& operator/=(const T& t)	{valiable()/=t; return *this;};
	template<class U> const SharedValiable_likeInt& operator+=(const U& u)	{valiable()+=(T)u; return *this;};
	template<class U> const SharedValiable_likeInt& operator-=(const U& u)	{valiable()-=(T)u; return *this;};
	template<class U> const SharedValiable_likeInt& operator*=(const U& u)	{valiable()*=(T)u; return *this;};
	template<class U> const SharedValiable_likeInt& operator/=(const U& u)	{valiable()/=(T)u; return *this;};


	// 二項算術演算
	friend SharedValiable_likeInt operator+(const SharedValiable_likeInt& a, const SharedValiable_likeInt& b)	{return SharedValiable_likeInt(a.valiable()+b.valiable());};
	friend SharedValiable_likeInt operator-(const SharedValiable_likeInt& a, const SharedValiable_likeInt& b)	{return SharedValiable_likeInt(a.valiable()-b.valiable());};
	friend SharedValiable_likeInt operator*(const SharedValiable_likeInt& a, const SharedValiable_likeInt& b)	{return SharedValiable_likeInt(a.valiable()*b.valiable());};
	friend SharedValiable_likeInt operator/(const SharedValiable_likeInt& a, const SharedValiable_likeInt& b)	{return SharedValiable_likeInt(a.valiable()/b.valiable());};
	friend T operator+(const SharedValiable_likeInt& a, T b)	{return a.valiable()+b;};
	friend T operator-(const SharedValiable_likeInt& a, T b)	{return a.valiable()-b;};
	friend T operator*(const SharedValiable_likeInt& a, T b)	{return a.valiable()*b;};
	friend T operator/(const SharedValiable_likeInt& a, T b)	{return a.valiable()/b;};
	friend T operator+(T a, const SharedValiable_likeInt& b)	{return a+b.valiable();};
	friend T operator-(T a, const SharedValiable_likeInt& b)	{return a-b.valiable();};
	friend T operator*(T a, const SharedValiable_likeInt& b)	{return a*b.valiable();};
	friend T operator/(T a, const SharedValiable_likeInt& b)	{return a/b.valiable();};
	template<class U> friend U operator+(const SharedValiable_likeInt& a, const U& b)	{return (U)a.valiable()+b;};
	template<class U> friend U operator-(const SharedValiable_likeInt& a, const U& b)	{return (U)a.valiable()-b;};
	template<class U> friend U operator*(const SharedValiable_likeInt& a, const U& b)	{return (U)a.valiable()*b;};
	template<class U> friend U operator/(const SharedValiable_likeInt& a, const U& b)	{return (U)a.valiable()/b;};
	template<class U> friend U operator+(const U& a, const SharedValiable_likeInt& b)	{return a+(U)b.valiable();};
	template<class U> friend U operator-(const U& a, const SharedValiable_likeInt& b)	{return a-(U)b.valiable();};
	template<class U> friend U operator*(const U& a, const SharedValiable_likeInt& b)	{return a*(U)b.valiable();};
	template<class U> friend U operator/(const U& a, const SharedValiable_likeInt& b)	{return a/(U)b.valiable();};


	// 比較演算
	friend bool operator==(const SharedValiable_likeInt& a, const SharedValiable_likeInt& b)	{return a.valiable()==b.valiable();};
	friend bool operator!=(const SharedValiable_likeInt& a, const SharedValiable_likeInt& b)	{return a.valiable()!=b.valiable();};
	friend bool operator<=(const SharedValiable_likeInt& a, const SharedValiable_likeInt& b)	{return a.valiable()<=b.valiable();};
	friend bool operator>=(const SharedValiable_likeInt& a, const SharedValiable_likeInt& b)	{return a.valiable()>=b.valiable();};
	friend bool operator<(const SharedValiable_likeInt& a, const SharedValiable_likeInt& b)	{return a.valiable()<b.valiable();};
	friend bool operator>(const SharedValiable_likeInt& a, const SharedValiable_likeInt& b)	{return a.valiable()>b.valiable();};

	friend bool operator==(const SharedValiable_likeInt& a, T b)	{return a.valiable()==b;};
	friend bool operator!=(const SharedValiable_likeInt& a, T b)	{return a.valiable()!=b;};
	friend bool operator<=(const SharedValiable_likeInt& a, T b)	{return a.valiable()<=b;};
	friend bool operator>=(const SharedValiable_likeInt& a, T b)	{return a.valiable()>=b;};
	friend bool operator<(const SharedValiable_likeInt& a, T b)	{return a.valiable()<b;};
	friend bool operator>(const SharedValiable_likeInt& a, T b)	{return a.valiable()>b;};
	friend bool operator==(T a, const SharedValiable_likeInt& b)	{return a==b.valiable();};
	friend bool operator!=(T a, const SharedValiable_likeInt& b)	{return a!=b.valiable();};
	friend bool operator<=(T a, const SharedValiable_likeInt& b)	{return a<=b.valiable();};
	friend bool operator>=(T a, const SharedValiable_likeInt& b)	{return a>=b.valiable();};
	friend bool operator<(T a, const SharedValiable_likeInt& b)	{return a<b.valiable();};
	friend bool operator>(T a, const SharedValiable_likeInt& b)	{return a>b.valiable();};
	template<class U> friend bool operator==(const SharedValiable_likeInt& a, const U& b)	{return (U)a.valiable()==b;};
	template<class U> friend bool operator!=(const SharedValiable_likeInt& a, const U& b)	{return (U)a.valiable()!=b;};
	template<class U> friend bool operator<=(const SharedValiable_likeInt& a, const U& b)	{return (U)a.valiable()<=b;};
	template<class U> friend bool operator>=(const SharedValiable_likeInt& a, const U& b)	{return (U)a.valiable()>=b;};
	template<class U> friend bool operator<(const SharedValiable_likeInt& a, const U& b)	{return (U)a.valiable()<b;};
	template<class U> friend bool operator>(const SharedValiable_likeInt& a, const U& b)	{return (U)a.valiable()>b;};
	template<class U> friend bool operator==(const U& a, const SharedValiable_likeInt& b)	{return a==(U)b.valiable();};
	template<class U> friend bool operator!=(const U& a, const SharedValiable_likeInt& b)	{return a!=(U)b.valiable();};
	template<class U> friend bool operator<=(const U& a, const SharedValiable_likeInt& b)	{return a<=(U)b.valiable();};
	template<class U> friend bool operator>=(const U& a, const SharedValiable_likeInt& b)	{return a>=(U)b.valiable();};
	template<class U> friend bool operator<(const U& a, const SharedValiable_likeInt& b)	{return a<(U)b.valiable();};
	template<class U> friend bool operator>(const U& a, const SharedValiable_likeInt& b)	{return a>(U)b.valiable();};
};

template<class T>
class SharedValiable_likeDouble
{
public:
	typedef boost::shared_ptr<T> Pointer;

protected:
	Pointer pValiable_;

public:
	// 補助関数
	inline T& valiable()		{return *pValiable_;};
	inline T  valiable() const	{return *pValiable_;};
	inline Pointer pointer()	{return pValiable_;};
public:
	SharedValiable_likeDouble()
		:pValiable_(new T)		{valiable() = 0.0;};
	SharedValiable_likeDouble(const T& t)
		:pValiable_(new T)		{valiable() = t;};//非明示的変換
	// コピーコンストラクタ
	SharedValiable_likeDouble(const SharedValiable_likeDouble& sharedValiable)
		:pValiable_(new T)		{valiable() = sharedValiable.valiable();};

	virtual ~SharedValiable_likeDouble()			{};
public:
	// キャスト
	operator T()	const		{return valiable();};
	operator T&()				{return valiable();};

	// 代入
	const T& operator=(const SharedValiable_likeDouble& rother)
		{valiable() = rother.valiable(); return valiable();};
	const T& operator=(T& t)	{valiable() = t; return valiable();};

	// アドレス
	      Pointer operator&()		{return pValiable_;};
	const Pointer operator&()const	{return pValiable_;};

	// 単項算術演算
	T operator+() const		{return valiable();};
	T operator-() const		{return -valiable();};

	// 算術代入
	const SharedValiable_likeDouble& operator+=(const SharedValiable_likeDouble& b)	{valiable()+=b.valiable(); return *this;};
	const SharedValiable_likeDouble& operator-=(const SharedValiable_likeDouble& b)	{valiable()-=b.valiable(); return *this;};
	const SharedValiable_likeDouble& operator*=(const SharedValiable_likeDouble& b)	{valiable()*=b.valiable(); return *this;};
	const SharedValiable_likeDouble& operator/=(const SharedValiable_likeDouble& b)	{valiable()/=b.valiable(); return *this;};
	const SharedValiable_likeDouble& operator+=(const T& t)	{valiable()+=t; return *this;};
	const SharedValiable_likeDouble& operator-=(const T& t)	{valiable()-=t; return *this;};
	const SharedValiable_likeDouble& operator*=(const T& t)	{valiable()*=t; return *this;};
	const SharedValiable_likeDouble& operator/=(const T& t)	{valiable()/=t; return *this;};
	template<class U> const SharedValiable_likeDouble& operator+=(const U& u)	{valiable()+=(T)u; return *this;};
	template<class U> const SharedValiable_likeDouble& operator-=(const U& u)	{valiable()-=(T)u; return *this;};
	template<class U> const SharedValiable_likeDouble& operator*=(const U& u)	{valiable()*=(T)u; return *this;};
	template<class U> const SharedValiable_likeDouble& operator/=(const U& u)	{valiable()/=(T)u; return *this;};

	// 二項算術演算
	friend SharedValiable_likeDouble operator+(const SharedValiable_likeDouble& a, const SharedValiable_likeDouble& b)	{return SharedValiable_likeDouble(a.valiable()+b.valiable());};
	friend SharedValiable_likeDouble operator-(const SharedValiable_likeDouble& a, const SharedValiable_likeDouble& b)	{return SharedValiable_likeDouble(a.valiable()-b.valiable());};
	friend SharedValiable_likeDouble operator*(const SharedValiable_likeDouble& a, const SharedValiable_likeDouble& b)	{return SharedValiable_likeDouble(a.valiable()*b.valiable());};
	friend SharedValiable_likeDouble operator/(const SharedValiable_likeDouble& a, const SharedValiable_likeDouble& b)	{return SharedValiable_likeDouble(a.valiable()/b.valiable());};
	friend T operator+(const SharedValiable_likeDouble& a, T b)	{return a.valiable()+b;};
	friend T operator-(const SharedValiable_likeDouble& a, T b)	{return a.valiable()-b;};
	friend T operator*(const SharedValiable_likeDouble& a, T b)	{return a.valiable()*b;};
	friend T operator/(const SharedValiable_likeDouble& a, T b)	{return a.valiable()/b;};
	friend T operator+(T a, const SharedValiable_likeDouble& b)	{return a+b.valiable();};
	friend T operator-(T a, const SharedValiable_likeDouble& b)	{return a-b.valiable();};
	friend T operator*(T a, const SharedValiable_likeDouble& b)	{return a*b.valiable();};
	friend T operator/(T a, const SharedValiable_likeDouble& b)	{return a/b.valiable();};
	template<class U> friend T operator+(const SharedValiable_likeDouble& a, const U& b)	{return a.valiable()+(T)b;};
	template<class U> friend T operator-(const SharedValiable_likeDouble& a, const U& b)	{return a.valiable()-(T)b;};
	template<class U> friend T operator*(const SharedValiable_likeDouble& a, const U& b)	{return a.valiable()*(T)b;};
	template<class U> friend T operator/(const SharedValiable_likeDouble& a, const U& b)	{return a.valiable()/(T)b;};
	template<class U> friend T operator+(const U& a, const SharedValiable_likeDouble& b)	{return (T)a+b.valiable();};
	template<class U> friend T operator-(const U& a, const SharedValiable_likeDouble& b)	{return (T)a-b.valiable();};
	template<class U> friend T operator*(const U& a, const SharedValiable_likeDouble& b)	{return (T)a*b.valiable();};
	template<class U> friend T operator/(const U& a, const SharedValiable_likeDouble& b)	{return (T)a/b.valiable();};

	// 比較演算
	friend bool operator==(const SharedValiable_likeDouble& a, const SharedValiable_likeDouble& b)	{return a.valiable()==b.valiable();};
	friend bool operator!=(const SharedValiable_likeDouble& a, const SharedValiable_likeDouble& b)	{return a.valiable()!=b.valiable();};
	friend bool operator<=(const SharedValiable_likeDouble& a, const SharedValiable_likeDouble& b)	{return a.valiable()<=b.valiable();};
	friend bool operator>=(const SharedValiable_likeDouble& a, const SharedValiable_likeDouble& b)	{return a.valiable()>=b.valiable();};
	friend bool operator<(const SharedValiable_likeDouble& a, const SharedValiable_likeDouble& b)	{return a.valiable()<b.valiable();};
	friend bool operator>(const SharedValiable_likeDouble& a, const SharedValiable_likeDouble& b)	{return a.valiable()>b.valiable();};

	friend bool operator==(const SharedValiable_likeDouble& a, T b)	{return a.valiable()==b;};
	friend bool operator!=(const SharedValiable_likeDouble& a, T b)	{return a.valiable()!=b;};
	friend bool operator<=(const SharedValiable_likeDouble& a, T b)	{return a.valiable()<=b;};
	friend bool operator>=(const SharedValiable_likeDouble& a, T b)	{return a.valiable()>=b;};
	friend bool operator<(const SharedValiable_likeDouble& a, T b)	{return a.valiable()<b;};
	friend bool operator>(const SharedValiable_likeDouble& a, T b)	{return a.valiable()>b;};
	friend bool operator==(T a, const SharedValiable_likeDouble& b)	{return a==b.valiable();};
	friend bool operator!=(T a, const SharedValiable_likeDouble& b)	{return a!=b.valiable();};
	friend bool operator<=(T a, const SharedValiable_likeDouble& b)	{return a<=b.valiable();};
	friend bool operator>=(T a, const SharedValiable_likeDouble& b)	{return a>=b.valiable();};
	friend bool operator<(T a, const SharedValiable_likeDouble& b)	{return a<b.valiable();};
	friend bool operator>(T a, const SharedValiable_likeDouble& b)	{return a>b.valiable();};
	template<class U> friend bool operator==(const SharedValiable_likeDouble& a, const U& b)	{return a.valiable()==(T)b;};
	template<class U> friend bool operator!=(const SharedValiable_likeDouble& a, const U& b)	{return a.valiable()!=(T)b;};
	template<class U> friend bool operator<=(const SharedValiable_likeDouble& a, const U& b)	{return a.valiable()<=(T)b;};
	template<class U> friend bool operator>=(const SharedValiable_likeDouble& a, const U& b)	{return a.valiable()>=(T)b;};
	template<class U> friend bool operator<(const SharedValiable_likeDouble& a, const U& b)	{return a.valiable()<(T)b;};
	template<class U> friend bool operator>(const SharedValiable_likeDouble& a, const U& b)	{return a.valiable()>(T)b;};
	template<class U> friend bool operator==(const U& a, const SharedValiable_likeDouble& b)	{return (T)a==b.valiable();};
	template<class U> friend bool operator!=(const U& a, const SharedValiable_likeDouble& b)	{return (T)a!=b.valiable();};
	template<class U> friend bool operator<=(const U& a, const SharedValiable_likeDouble& b)	{return (T)a<=b.valiable();};
	template<class U> friend bool operator>=(const U& a, const SharedValiable_likeDouble& b)	{return (T)a>=b.valiable();};
	template<class U> friend bool operator<(const U& a, const SharedValiable_likeDouble& b)	{return (T)a<b.valiable();};
	template<class U> friend bool operator>(const U& a, const SharedValiable_likeDouble& b)	{return (T)a>b.valiable();};

};

template<class T>
class SharedValiable_likeVector
{
public:
	typedef boost::shared_ptr<T> Pointer;

protected:
	Pointer pValiable_;

public:
	// 補助関数
	inline T& valiable()		{return *valiable_;};
	inline T  valiable() const	{return *valiable_;};
	inline Pointer pointer()	{return valiable_;};
public:
	SharedValiable_likeVector()
		:pValiable_(new T)		{};
	SharedValiable_likeVector(const T& t)
		:pValiable_(new T)		{valiable() = t;};//非明示的変換
	// コピーコンストラクタ
	SharedValiable_likeVector(const SharedValiable_likeVector& SharedValiable_likeVector)
		:pValiable_(new T)		{valiable() = SharedValiable_likeVector.valiable();};

	virtual ~SharedValiable_likeVector()			{};
public:
	// キャスト
	operator T()	const		{return valiable();};
	operator T&()				{return valiable();};

	// 代入
	const T& operator=(const SharedValiable_likeVector& rother)
		{valiable() = rother.valiable(); return valiable();};
	const T& operator=(T& t)	{valiable() = t; return valiable();};

	// アドレス
	      Pointer operator&()		{return pValiable_;};
	const Pointer operator&()const	{return pValiable_;};

	// 単項算術演算
	T operator+() const		{return valiable();};
	T operator-() const		{return -valiable();};

	// 算術代入
	const SharedValiable_likeVector& operator+=(const SharedValiable_likeVector& b)	{valiable()+=b.valiable(); return *this;};
	const SharedValiable_likeVector& operator-=(const SharedValiable_likeVector& b)	{valiable()-=b.valiable(); return *this;};
	const SharedValiable_likeVector& operator*=(const SharedValiable_likeVector& b)	{valiable()*=b.valiable(); return *this;};
	const SharedValiable_likeVector& operator/=(const SharedValiable_likeVector& b)	{valiable()/=b.valiable(); return *this;};
	const SharedValiable_likeVector& operator+=(const T& t)	{valiable()+=t; return *this;};
	const SharedValiable_likeVector& operator-=(const T& t)	{valiable()-=t; return *this;};
	const SharedValiable_likeVector& operator*=(const T& t)	{valiable()*=t; return *this;};
	const SharedValiable_likeVector& operator/=(const T& t)	{valiable()/=t; return *this;};
	template<class U> const SharedValiable_likeVector& operator+=(const U& u)	{valiable()+=(T)u; return *this;};
	template<class U> const SharedValiable_likeVector& operator-=(const U& u)	{valiable()-=(T)u; return *this;};
	template<class U> const SharedValiable_likeVector& operator*=(const U& u)	{valiable()*=(T)u; return *this;};
	template<class U> const SharedValiable_likeVector& operator/=(const U& u)	{valiable()/=(T)u; return *this;};

	// 二項算術演算
	friend SharedValiable_likeVector operator+(const SharedValiable_likeVector& a, const SharedValiable_likeVector& b)	{return SharedValiable_likeVector(a.valiable()+b.valiable());};
	friend SharedValiable_likeVector operator-(const SharedValiable_likeVector& a, const SharedValiable_likeVector& b)	{return SharedValiable_likeVector(a.valiable()-b.valiable());};
	friend SharedValiable_likeVector operator*(const SharedValiable_likeVector& a, const SharedValiable_likeVector& b)	{return SharedValiable_likeVector(a.valiable()*b.valiable());};
	friend SharedValiable_likeVector operator/(const SharedValiable_likeVector& a, const SharedValiable_likeVector& b)	{return SharedValiable_likeVector(a.valiable()/b.valiable());};
	friend T operator+(const SharedValiable_likeVector& a, T b)	{return a.valiable()+b;};
	friend T operator-(const SharedValiable_likeVector& a, T b)	{return a.valiable()-b;};
	friend T operator*(const SharedValiable_likeVector& a, T b)	{return a.valiable()*b;};
	friend T operator/(const SharedValiable_likeVector& a, T b)	{return a.valiable()/b;};
	friend T operator+(T a, const SharedValiable_likeVector& b)	{return a+b.valiable();};
	friend T operator-(T a, const SharedValiable_likeVector& b)	{return a-b.valiable();};
	friend T operator*(T a, const SharedValiable_likeVector& b)	{return a*b.valiable();};
	friend T operator/(T a, const SharedValiable_likeVector& b)	{return a/b.valiable();};
	template<class U> friend U operator+(const SharedValiable_likeVector& a, const U& b)	{return a.valiable()+(T)b;};
	template<class U> friend U operator-(const SharedValiable_likeVector& a, const U& b)	{return a.valiable()-(T)b;};
	template<class U> friend U operator*(const SharedValiable_likeVector& a, const U& b)	{return a.valiable()*(T)b;};
	template<class U> friend U operator/(const SharedValiable_likeVector& a, const U& b)	{return a.valiable()/(T)b;};
	template<class U> friend U operator+(const U& a, const SharedValiable_likeVector& b)	{return (T)a+b.valiable();};
	template<class U> friend U operator-(const U& a, const SharedValiable_likeVector& b)	{return (T)a-b.valiable();};
	template<class U> friend U operator*(const U& a, const SharedValiable_likeVector& b)	{return (T)a*b.valiable();};
	template<class U> friend U operator/(const U& a, const SharedValiable_likeVector& b)	{return (T)a/b.valiable();};

	// 比較演算
	friend bool operator==(const SharedValiable_likeVector& a, const SharedValiable_likeVector& b)	{return a.valiable()==b.valiable();};
	friend bool operator!=(const SharedValiable_likeVector& a, const SharedValiable_likeVector& b)	{return a.valiable()!=b.valiable();};
	friend bool operator==(const SharedValiable_likeVector& a, T b)	{return a.valiable()==b;};
	friend bool operator!=(const SharedValiable_likeVector& a, T b)	{return a.valiable()!=b;};
	friend bool operator==(T a, const SharedValiable_likeVector& b)	{return a==b.valiable();};
	friend bool operator!=(T a, const SharedValiable_likeVector& b)	{return a!=b.valiable();};
	template<class U> friend bool operator==(const SharedValiable_likeVector& a, const U& b)	{return a.valiable()==(T)b;};
	template<class U> friend bool operator!=(const SharedValiable_likeVector& a, const U& b)	{return a.valiable()!=(T)b;};
	template<class U> friend bool operator==(const U& a, const SharedValiable_likeVector& b)	{return (T)a==b.valiable();};
	template<class U> friend bool operator!=(const U& a, const SharedValiable_likeVector& b)	{return (T)a!=b.valiable();};

};

template<class T>
class SharedObject
{
public:
	typedef boost::shared_ptr<T> Pointer;

protected:
	Pointer pValiable_;

public:
	// 補助関数
	inline T& valiable()		{return *valiable_;};
	inline T  valiable() const	{return *valiable_;};
	inline Pointer pointer()	{return valiable_;};
public:
	SharedObject()
		:pValiable_(new T)		{};
	SharedObject(const T& t)
		:pValiable_(new T)		{valiable() = valiable;};//非明示的変換
	// コピーコンストラクタ
	SharedObject(const SharedObject& SharedObject)
		:pValiable_(new T)		{valiable() = SharedObject.valiable();};

	virtual ~SharedObject()				{};
public:
	// キャスト
	operator T()	const		{return valiable();};
	operator T&()				{return valiable();};

	// 代入
	const T& operator=(const SharedObject& rother)
		{valiable() = rother.valiable(); return valiable();};
	const T& operator=(T& t)	{valiable() = t; return valiable();};

	// アドレス
	      Pointer operator&()		{return pValiable_;};
	const Pointer operator&()const	{return pValiable_;};

	// 比較演算
	friend bool operator==(const SharedObject& a, const SharedObject& b)	{return a.valiable()==b.valiable();};
	friend bool operator!=(const SharedObject& a, const SharedObject& b)	{return a.valiable()!=b.valiable();};
	friend bool operator==(const SharedObject& a, T b)	{return a.valiable()==b;};
	friend bool operator!=(const SharedObject& a, T b)	{return a.valiable()!=b;};
	friend bool operator==(T a, const SharedObject& b)	{return a==b.valiable();};
	friend bool operator!=(T a, const SharedObject& b)	{return a!=b.valiable();};

};
