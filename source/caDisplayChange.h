#pragma once
#include "cAnimation.h"
#include "cLandform.h"
#include "cCharacter.h"

#include "cMoney.h"

#include <vector>
#include <list>
#include <set>
#include <tstring_ph.h>

#include "utility/ValiableField/SafePointerObject.h"

using namespace std;



class caDisplayChange :
	public cAnimation
{
public:
	caDisplayChange(void);
public:
	virtual ~caDisplayChange(void);


	virtual int process(IDirect3DDevice9 *pDev);

	virtual bool isOnScreen();

	virtual bool Set(SharedValiable_likeDouble<double>::Pointer pointer, double value);
	virtual bool Set(SharedValiable_likeInt<int>::Pointer pointer, int value);
	virtual bool Set(SafePointer<set<int>> pointer, set<int> value);
	virtual bool Set(SafePointer<c4DVector> pointer, c4DVector value);
	virtual bool Set(SafePointer<cMoney> pointer, cMoney value);


	//î≠âŒÇµÇ‚Ç∑Ç¢Ç©ÅiWeekInvocationÇ≈î≠âŒÇ∑ÇÈÇ©Åj
	virtual int EasyInvocation(){return false;};//î≠âŒÇµÇ…Ç≠Ç¢


	SharedValiable_likeInt<int>::Pointer targetInt;
	SharedValiable_likeDouble<double>::Pointer targetDouble;
	SafePointer<set<int>> targetSet;
	SafePointer<c4DVector> targetC4DVector;
	SafePointer<cMoney> targetCMoney;

	int changeInt;
	double changeDouble;
	set<int> changeSet;
	c4DVector changeC4DVector;
	cMoney changeCMoney;
};

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<caDisplayChange> pcaDisplayChange;