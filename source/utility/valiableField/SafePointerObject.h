#pragma once


#include "SharedValiable.h"

template<class T> class SafePointerObject;

template<class T>
class SafePointer
{
private:
	SafePointerObject<T>* PointerOfSafePointerObject_;
	SharedValiable_likeInt<int>::Pointer PointerOfSafeFlag_;
public:
	SafePointer()
		:PointerOfSafePointerObject_(NULL){};
	SafePointer(SafePointerObject<T>* pspo, SharedValiable_likeInt<int>::Pointer posf)
		:PointerOfSafePointerObject_(pspo),PointerOfSafeFlag_(posf){};
	virtual ~SafePointer(){};

	SafePointerObject<T>* lock();
};

template<class T>
class SafePointerObject : public T
{
public:
	enum {
		SafeFlag_Enabled = 0,
		SafeFlag_NotEnabled,
	};
private:
	SharedValiable_likeInt<int> SafeFlag_;
public:
	SafePointerObject(const T& t)
	{
		SafeFlag_ = SafeFlag_Enabled;
		*static_cast<T*>(this) = t;
	}

	SafePointerObject()
	{
		SafeFlag_ = SafeFlag_Enabled;
	};
	virtual ~SafePointerObject()
	{
		SafeFlag_ = SafeFlag_NotEnabled;
	}
	// アドレス
	      SafePointer<T> operator&()		{return SafePointer<T>(this,&SafeFlag_);};
	const SafePointer<T> operator&() const	{return SafePointer<T>(this,&SafeFlag_);};

	// キャスト
	operator T()	const		{return *static_cast<T*>(this);};
	operator T&()				{return *static_cast<T*>(this);};

	// 代入
	const T& operator=(const SafePointerObject& rother)
		{*static_cast<T*>(this) = rother; return *static_cast<T*>(this);};
	const T& operator=(T& t)
		{*static_cast<T*>(this) = t; return *static_cast<T*>(this);};

};

template<class T> SafePointerObject<T>* SafePointer<T>::lock()
{
	if(PointerOfSafeFlag_ == NULL || *PointerOfSafeFlag_ == SafePointerObject<T>::SafeFlag_NotEnabled)
	{
		PointerOfSafePointerObject_ = NULL;
		return NULL;
	}
	else
	{
		return PointerOfSafePointerObject_;
	}
};
	