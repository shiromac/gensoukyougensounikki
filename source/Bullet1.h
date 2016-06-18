#pragma once

#include "cBullet.h"


#include <vector>
#include <list>
#include <tstring_ph.h>


using namespace std;

#include <boost/preprocessor.hpp>








#define DEF_class_cBullet_ID(identify) \
class cBullet_ID_##identify## :\
	public cBullet\
{\
public:\
	cBullet_ID_##identify##(void){};\
public:\
	virtual ~cBullet_ID_##identify##(void){};\
	virtual inline int ID(){return CDROPING_cBullet_BASE_ID_NUM + identify;};\
public:\




DEF_class_cBullet_ID(0)

/*
	virtual tstring FullNameBase()
		{return _T("木の矢");};
	virtual tstring ShortNameBase()
		{return _T("木の矢");};
	virtual tstring usefulName()
		{return _T("木の矢");};

	virtual int 値段基礎値(){return PRICE_BULLET_E;};
	
	//レア度(出現確率基礎値)
	virtual double レア度(){return RARITY_F;};

	virtual StyleString shortExplanationBase(){return (tstring)
_T("")
;};
///　　　５　　　　10　　　　15　　　　20|
	virtual StyleString longExplanationBase(){return (tstring)
_T("")
;};/* ** */
/*
	virtual inline int 加算攻撃力(){return 1;};
	virtual inline double 乗算攻撃力(){return 1.0;};
	*/
	//virtual inline int 攻撃属性(){return 属性設定();};
	virtual int 衝突時効果(pcCharacter pchara);


};

DEF_class_cBullet_ID(1)
/*
	virtual tstring FullNameBase()
		{return _T("えーりんの矢");};
	virtual tstring ShortNameBase()
		{return _T("えーりんの矢");};
	virtual tstring usefulName()
		{return _T("えーりんの矢");};

	virtual int 値段基礎値(){return PRICE_BULLET_E;};
	
	//レア度(出現確率基礎値)
	virtual double レア度(){return RARITY_F;};

	virtual StyleString shortExplanationBase(){return (tstring)
_T("")
;};
///　　　５　　　　10　　　　15　　　　20|
	virtual StyleString longExplanationBase(){return (tstring)
_T("")
;};/* ** */
/*
	virtual inline int 加算攻撃力(){return 1;};
	virtual inline double 乗算攻撃力(){return 1.0;};
	*/
	//virtual inline int 攻撃属性(){return 属性設定();};
	virtual int 衝突時効果(pcCharacter pchara);

};

DEF_class_cBullet_ID(2)
	virtual int 衝突時効果(pcCharacter pchara);
	virtual int 効果(pcLandform pland);
	virtual void CutIn(タイミング timing, cValiableField& valiable);
	//6~8まで具体種
	virtual inline int & explosion_power(){return dpack.integer[6];};//6番
};
DEF_class_cBullet_ID(3)
	virtual int 衝突時効果(pcCharacter pchara);
};

DEF_class_cBullet_ID(4)
	virtual int 衝突時効果(pcCharacter pchara);
};
DEF_class_cBullet_ID(5)
	virtual int 衝突時効果(pcCharacter pchara);
};
DEF_class_cBullet_ID(6)
	virtual int 衝突時効果(pcCharacter pchara);
};
DEF_class_cBullet_ID(7)
	virtual int 衝突時効果(pcCharacter pchara);
};
DEF_class_cBullet_ID(8)
	virtual int 衝突時効果(pcCharacter pchara);
};
DEF_class_cBullet_ID(9)
	virtual int 衝突時効果(pcCharacter pchara);
	virtual void CutIn(タイミング timing, cValiableField& valiable);
};
DEF_class_cBullet_ID(10)
	virtual int 衝突時効果(pcCharacter pchara);
};
DEF_class_cBullet_ID(11)
	virtual int 衝突時効果(pcCharacter pchara);
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};
DEF_class_cBullet_ID(12)
	virtual int 衝突時効果(pcCharacter pchara);
	virtual int 撃つ_効果_通常(pcCharacter pchara);
	virtual void CutIn(タイミング timing, cValiableField& valiable);
};
DEF_class_cBullet_ID(13)
	virtual int 衝突時効果(pcCharacter pchara);
	virtual int 撃つ_効果_通常(pcCharacter pchara);
};
DEF_class_cBullet_ID(14)
	virtual int 衝突時効果(pcCharacter pchara);
	virtual int 効果(pcLandform pland);
	virtual void CutIn(タイミング timing, cValiableField& valiable);
};
DEF_class_cBullet_ID(15)
	virtual int 衝突時効果(pcCharacter pchara);
};
DEF_class_cBullet_ID(16)
	virtual int 衝突時効果(pcCharacter pchara);
};
DEF_class_cBullet_ID(17)
	virtual int 衝突時効果(pcCharacter pchara);
	virtual void CutIn(タイミング timing, cValiableField& valiable);
};
DEF_class_cBullet_ID(18)
	virtual int 衝突時効果(pcCharacter pchara);
	//virtual void CutIn(タイミング timing, cValiableField& valiable);
};
DEF_class_cBullet_ID(19)
	//virtual int 衝突時効果(pcCharacter pchara);
	virtual void CutIn(タイミング timing, cValiableField& valiable);
};
DEF_class_cBullet_ID(20)
	virtual int 衝突時効果(pcCharacter pchara);
};
DEF_class_cBullet_ID(21)
	virtual int 衝突時効果(pcCharacter pchara);
	virtual int 撃つ_効果_通常(pcCharacter pchara);
};
DEF_class_cBullet_ID(22)
	//virtual int 衝突時効果(pcCharacter pchara);
	virtual void CutIn(タイミング timing, cValiableField& valiable);
};
DEF_class_cBullet_ID(23)
	//virtual int 衝突時効果(pcCharacter pchara);
	virtual void CutIn(タイミング timing, cValiableField& valiable);
};
DEF_class_cBullet_ID(24)
	//virtual int 衝突時効果(pcCharacter pchara);
	virtual void CutIn(タイミング timing, cValiableField& valiable);
};
DEF_class_cBullet_ID(25)
	//virtual int 衝突時効果(pcCharacter pchara);
	virtual void CutIn(タイミング timing, cValiableField& valiable);
};
DEF_class_cBullet_ID(26)
	virtual int 衝突時効果(pcCharacter pchara);
	virtual void CutIn(タイミング timing, cValiableField& valiable);
	inline int & attack_power(){return dpack.integer[6];};//6番
	inline int & push_power(){return dpack.integer[7];};//7番
	virtual int 加算攻撃力();
	virtual StyleString QualityCaption(){return setStyle(_T(""));};
};
DEF_class_cBullet_ID(27)
	virtual int 衝突時効果(pcCharacter pchara);
	virtual int 効果(pcLandform pland);
	virtual void CutIn(タイミング timing, cValiableField& valiable);
	//6~8まで具体種
	virtual inline int & explosion_power(){return dpack.integer[6];};//6番
	virtual StyleString QualityCaption(){return setStyle(_T(""));};
};
DEF_class_cBullet_ID(28)
	virtual int 衝突時効果(pcCharacter pchara);
	virtual int 効果(pcLandform pland, int aspect);
	virtual int 撃つ_効果_通常(pcCharacter pchara);
};
DEF_class_cBullet_ID(29)
	virtual int 衝突時効果(pcCharacter pchara);
	virtual int 効果(pcLandform pland, int aspect);
	virtual int 撃つ_効果_通常(pcCharacter pchara);
};
#define BULLET_NUM 30