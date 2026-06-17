#include "stdafx.h"

#include "caDisplayChange.h"

caDisplayChange::caDisplayChange()
{
	End = false;
}

caDisplayChange::~caDisplayChange()
{
}


int caDisplayChange::process(cRenderDevice *pDev)
{

	if(targetInt)
	{
		*targetInt = changeInt;
	}

	if(targetDouble)
	{
		*targetDouble = changeDouble;
	}

	if(targetSet.lock())
	{
		*static_cast<set<int>*>(targetSet.lock()) = changeSet;
	}

	if(targetC4DVector.lock())
	{
		*static_cast<c4DVector*>(targetC4DVector.lock()) = changeC4DVector;
	}

	if(targetCMoney.lock())
	{
		*static_cast<cMoney*>(targetCMoney.lock()) = changeCMoney;
	}

	End = true;
	
	
	return true;
}

bool caDisplayChange::isOnScreen()
{

	return true;

}

bool caDisplayChange::Set(SharedValiable_likeDouble<double>::Pointer pointer, double value)
{
	targetDouble = pointer;
	changeDouble = value;
	return true;
}

bool caDisplayChange::Set(SharedValiable_likeInt<int>::Pointer pointer, int value)
{
	targetInt = pointer;
	changeInt = value;
	return true;
}

bool caDisplayChange::Set(SafePointer<set<int>> pointer, set<int> value)
{
	targetSet = pointer;
	changeSet = value;
	return true;
}

bool caDisplayChange::Set(SafePointer<c4DVector> pointer, c4DVector value)
{
	targetC4DVector = pointer;
	changeC4DVector = value;
	return true;
}

bool caDisplayChange::Set(SafePointer<cMoney> pointer, cMoney value)
{
	targetCMoney = pointer;
	changeCMoney = value;
	return true;
}