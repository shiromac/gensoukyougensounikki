#pragma once

#include "cMoneyBag.h"


#include <vector>
#include <list>
#include <tstring_ph.h>


using namespace std;

#include <boost/preprocessor.hpp>








#define DEF_class_cMoneyBag_ID(identify) \
class cMoneyBag_ID_##identify## :\
	public cMoneyBag\
{\
public:\
	cMoneyBag_ID_##identify##(void){};\
public:\
	virtual ~cMoneyBag_ID_##identify##(void){};\
	virtual inline int ID(){return CDROPING_cMoneyBag_BASE_ID_NUM + identify;};\
public:\




DEF_class_cMoneyBag_ID(0)

/*
	virtual tstring FullNameBase()
		{return _T("お金");};
	virtual tstring ShortNameBase()
		{return _T("お金");};
*/

	

	//レア度(出現確率基礎値)
	//virtual double レア度(){return RARITY_F;};

	/*
	virtual StyleString shortExplanationBase(){return (tstring)
_T("")
;};
///　　　５　　　　10　　　　15　　　　20|
	virtual StyleString longExplanationBase(){return (tstring)
_T("")

;};
*/
};

DEF_class_cMoneyBag_ID(1)
};
DEF_class_cMoneyBag_ID(2)
};
DEF_class_cMoneyBag_ID(3)
};
DEF_class_cMoneyBag_ID(4)
};
DEF_class_cMoneyBag_ID(5)
};
DEF_class_cMoneyBag_ID(6)
};
DEF_class_cMoneyBag_ID(7)
};
DEF_class_cMoneyBag_ID(8)
};
DEF_class_cMoneyBag_ID(9)
};
DEF_class_cMoneyBag_ID(10)
};
DEF_class_cMoneyBag_ID(11)
};
DEF_class_cMoneyBag_ID(12)
};
DEF_class_cMoneyBag_ID(13)
};
DEF_class_cMoneyBag_ID(14)
};
DEF_class_cMoneyBag_ID(15)
};
DEF_class_cMoneyBag_ID(16)
};
DEF_class_cMoneyBag_ID(17)
};
DEF_class_cMoneyBag_ID(18)
};
DEF_class_cMoneyBag_ID(19)
};
#define MONEYBAG_NUM 20