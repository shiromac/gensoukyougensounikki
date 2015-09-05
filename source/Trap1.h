#pragma once

#include "cTrap.h"


#include <vector>
#include <list>
#include <tstring_ph.h>


using namespace std;

#include <boost/preprocessor.hpp>

#define CDROPING_cTrap_BASE_ID_NUM 9000



#define DEF_class_cTrap_ID(identify) \
class cTrap_ID_##identify## :\
	public cTrap\
{\
public:\
	cTrap_ID_##identify##(void){};\
public:\
	virtual ~cTrap_ID_##identify##(void){};\
	virtual inline int ID(){return CDROPING_cTrap_BASE_ID_NUM + identify;};\
public:\


#define DEF_class_cTrap_ID_child(identify,parent) \
class cTrap_ID_##identify## :\
	public cTrap_ID_##parent##\
{\
public:\
	cTrap_ID_##identify##(void){};\
public:\
	virtual ~cTrap_ID_##identify##(void){};\
	virtual inline int ID(){return CDROPING_cTrap_BASE_ID_NUM + identify;};\
public:\



DEF_class_cTrap_ID(0)


/*


	virtual tstring FullNameBase()
		{return _T("天狗風のワナ");};
	virtual tstring ShortNameBase()
		{return _T("天狗風のワナ");};
	virtual tstring usefulName()
		{return _T("テングカゼノワナ");};
*/
	virtual inline tstring IconFileName(){return _T("item\\罠１.png");};
	virtual inline int IconFileIndexX(){return 0;};
	virtual inline int IconFileIndexY(){return 0;};

	/*
	//発動率(%)
	virtual double 発動率(){return 80;};

	//故障率(%)
	virtual double 故障率(){return 20;};

	//レア度(出現確率基礎値)
	virtual double レア度(){return RARITY_F;};
	*/
/*
	virtual StyleString shortExplanationBase(){return (tstring)_T("")

;};
///　　　５　　　　10　　　　15　　　　20|
	virtual StyleString longExplanationBase(){return (tstring)_T("")

;};
*/

	virtual int 効果();
	virtual int 効果(pcCharacter pchara);
	virtual int 効果(pcDroping pdrop);//pdropに何か起こったらtrue;

};


class cTrap_ID_1 :
	public cTrap_ID_0
{
public:
	cTrap_ID_1(void){};
public:
	virtual ~cTrap_ID_1(void){};
	virtual inline int ID(){return CDROPING_cTrap_BASE_ID_NUM + 1;};
public:
/*
	virtual tstring FullNameBase()
		{return _T("天狗風のワナ[丈夫]");};
	virtual tstring ShortNameBase()
		{return _T("天狗風のワナ[丈夫]");};
	virtual tstring usefulName()
		{return _T("テングカゼノワナ[丈夫]");};
*/	
	virtual bool 被破壊可(){return false;};


	//データパックを初期最適化する。アイテムを初期設置するときに一度だけ使う。
	virtual void DataBeginOptimize(int difficulty);

	virtual bool 大切(){return true;};
	virtual inline tstring IconFileName(){return _T("item\\罠１.png");};
	virtual inline int IconFileIndexX(){return 1;};
	virtual inline int IconFileIndexY(){return 0;};
/*
	//発動率(%)
	virtual double 発動率(){return 80;};

	//故障率(%)
	virtual double 故障率(){return 0;};

	//レア度(出現確率基礎値)
	virtual double レア度(){return RARITY_SP;};
*/
};

DEF_class_cTrap_ID(2)



	virtual inline tstring IconFileName(){return _T("item\\罠１.png");};
	virtual inline int IconFileIndexX(){return 2;};
	virtual inline int IconFileIndexY(){return 0;};

	virtual int 効果();
	virtual int 効果(pcCharacter pchara);
	virtual int 効果(pcDroping pdrop);//pdropに何か起こったらtrue;
	virtual void CutIn(タイミング timing, cValiableField& valiable);

};

DEF_class_cTrap_ID_child(3,2)


	virtual inline tstring IconFileName(){return _T("item\\罠１.png");};
	virtual inline int IconFileIndexX(){return 3;};
	virtual inline int IconFileIndexY(){return 0;};

};

DEF_class_cTrap_ID(4)



	virtual inline tstring IconFileName(){return _T("item\\罠１.png");};
	virtual inline int IconFileIndexX(){return 0;};
	virtual inline int IconFileIndexY(){return 1;};

	virtual int 効果();
	virtual int 効果(pcCharacter pchara);
	virtual int 効果(pcDroping pdrop);//pdropに何か起こったらtrue;
};

DEF_class_cTrap_ID(5)



	virtual inline tstring IconFileName(){return _T("item\\罠１.png");};
	virtual inline int IconFileIndexX(){return 1;};
	virtual inline int IconFileIndexY(){return 1;};

	virtual int 効果();
	virtual int 効果(pcCharacter pchara);
	virtual int 効果(pcDroping pdrop);//pdropに何か起こったらtrue;

	virtual int 効果(int aspect);
};

DEF_class_cTrap_ID(6)



	virtual inline tstring IconFileName(){return _T("item\\罠１.png");};
	virtual inline int IconFileIndexX(){return 2;};
	virtual inline int IconFileIndexY(){return 1;};

	virtual int 効果();
	virtual int 効果(pcCharacter pchara);
	virtual int 効果(pcDroping pdrop);//pdropに何か起こったらtrue;

		virtual int 効果(int aspect);
};
DEF_class_cTrap_ID(7)

	virtual inline tstring IconFileName(){return _T("item\\罠２.png");};
	virtual inline int IconFileIndexX(){return 0;};
	virtual inline int IconFileIndexY(){return 0;};

	virtual int 効果();
	virtual int 効果(pcCharacter pchara);
	virtual int 効果(pcDroping pdrop);//pdropに何か起こったらtrue;

		virtual int 効果(int aspect);
};
DEF_class_cTrap_ID(8)

	virtual inline tstring IconFileName(){return _T("item\\罠２.png");};
	virtual inline int IconFileIndexX(){return 2;};
	virtual inline int IconFileIndexY(){return 1;};

	virtual int 効果();
	virtual int 効果(pcCharacter pchara);
	virtual int 効果(pcDroping pdrop);//pdropに何か起こったらtrue;

	virtual int 効果(int aspect);
};
DEF_class_cTrap_ID(9)

	virtual inline tstring IconFileName(){return _T("item\\罠１.png");};
	virtual inline int IconFileIndexX(){return 3;};
	virtual inline int IconFileIndexY(){return 1;};

	virtual int 効果();
	virtual int 効果(pcCharacter pchara);
	virtual int 効果(pcDroping pdrop);//pdropに何か起こったらtrue;

};
DEF_class_cTrap_ID(10)

	virtual inline tstring IconFileName(){return _T("item\\罠１.png");};
	virtual inline int IconFileIndexX(){return 0;};
	virtual inline int IconFileIndexY(){return 2;};

	virtual int 効果();
	virtual int 効果(pcCharacter pchara);
	virtual int 効果(pcDroping pdrop);//pdropに何か起こったらtrue;

};
DEF_class_cTrap_ID(11)

	virtual inline tstring IconFileName(){return _T("item\\罠１.png");};
	virtual inline int IconFileIndexX(){return 1;};
	virtual inline int IconFileIndexY(){return 2;};

	virtual int 効果();
	virtual int 効果_pri(pcDroping pdrop);
	virtual int 効果(pcCharacter pchara);
	virtual int 効果(pcDroping pdrop);//pdropに何か起こったらtrue;

};
DEF_class_cTrap_ID(12)

	virtual inline tstring IconFileName(){return _T("item\\罠１.png");};
	virtual inline int IconFileIndexX(){return 2;};
	virtual inline int IconFileIndexY(){return 2;};

	virtual int 効果();
	virtual int 効果(pcCharacter pchara);
	virtual int 効果(pcDroping pdrop);//pdropに何か起こったらtrue;

};
DEF_class_cTrap_ID(13)

	virtual inline tstring IconFileName(){return _T("item\\罠１.png");};
	virtual inline int IconFileIndexX(){return 3;};
	virtual inline int IconFileIndexY(){return 2;};

	virtual int 効果();
	virtual int 効果(pcCharacter pchara);
	virtual int 効果(pcDroping pdrop);//pdropに何か起こったらtrue;

};
DEF_class_cTrap_ID(14)

	virtual inline tstring IconFileName(){return _T("item\\罠１.png");};
	virtual inline int IconFileIndexX(){return 0;};
	virtual inline int IconFileIndexY(){return 3;};

	virtual int 効果();
	virtual int 効果_pri(pcDroping pdrop);
	virtual int 効果(pcCharacter pchara);
	virtual int 効果(pcDroping pdrop);//pdropに何か起こったらtrue;

};
DEF_class_cTrap_ID(15)

	virtual inline tstring IconFileName(){return _T("item\\罠１.png");};
	virtual inline int IconFileIndexX(){return 1;};
	virtual inline int IconFileIndexY(){return 3;};

	virtual int 効果();
	virtual int 効果(pcCharacter pchara);
	virtual int 効果(pcDroping pdrop);//pdropに何か起こったらtrue;

};
DEF_class_cTrap_ID(16)

	virtual inline tstring IconFileName(){return _T("item\\罠１.png");};
	virtual inline int IconFileIndexX(){return 2;};
	virtual inline int IconFileIndexY(){return 3;};

	virtual int 効果();
	virtual int 効果(pcCharacter pchara);
	virtual int 効果(pcDroping pdrop);//pdropに何か起こったらtrue;

};
DEF_class_cTrap_ID(17)

	virtual inline tstring IconFileName(){return _T("item\\罠１.png");};
	virtual inline int IconFileIndexX(){return 3;};
	virtual inline int IconFileIndexY(){return 3;};

	virtual int 効果();
	virtual int 効果(pcCharacter pchara);
	virtual int 効果(pcDroping pdrop);//pdropに何か起こったらtrue;

};
DEF_class_cTrap_ID(18)

	virtual inline tstring IconFileName(){return _T("item\\罠２.png");};
	virtual inline int IconFileIndexX(){return 1;};
	virtual inline int IconFileIndexY(){return 0;};

	virtual int 効果();
	virtual int 効果(pcCharacter pchara);
	virtual int 効果(pcDroping pdrop);//pdropに何か起こったらtrue;

};
DEF_class_cTrap_ID(19)

	virtual inline tstring IconFileName(){return _T("item\\罠２.png");};
	virtual inline int IconFileIndexX(){return 2;};
	virtual inline int IconFileIndexY(){return 0;};

	virtual int 効果();
	virtual int 効果(pcCharacter pchara);
	virtual int 効果(pcDroping pdrop);//pdropに何か起こったらtrue;

};
DEF_class_cTrap_ID(20)

	virtual inline tstring IconFileName(){return _T("item\\罠２.png");};
	virtual inline int IconFileIndexX(){return 3;};
	virtual inline int IconFileIndexY(){return 0;};

	virtual int 効果();
	virtual int 効果(pcCharacter pchara);
	virtual int 効果(pcDroping pdrop);//pdropに何か起こったらtrue;

};
DEF_class_cTrap_ID(21)

	virtual inline tstring IconFileName(){return _T("item\\罠２.png");};
	virtual inline int IconFileIndexX(){return 0;};
	virtual inline int IconFileIndexY(){return 1;};

	virtual int 効果();
	virtual int 効果(pcCharacter pchara);
	virtual int 効果(pcDroping pdrop);//pdropに何か起こったらtrue;

};
DEF_class_cTrap_ID(22)

	virtual inline tstring IconFileName(){return _T("item\\罠２.png");};
	virtual inline int IconFileIndexX(){return 1;};
	virtual inline int IconFileIndexY(){return 1;};

	virtual int 効果();
	virtual int 効果(pcCharacter pchara);
	virtual int 効果(pcDroping pdrop);//pdropに何か起こったらtrue;

};
DEF_class_cTrap_ID(23)

	virtual inline tstring IconFileName(){return _T("item\\罠２.png");};
	virtual inline int IconFileIndexX(){return 3;};
	virtual inline int IconFileIndexY(){return 1;};

	virtual int 効果();
	virtual int 効果(pcCharacter pchara);
	virtual int 効果(pcDroping pdrop);//pdropに何か起こったらtrue;

};
DEF_class_cTrap_ID(24)

	virtual inline tstring IconFileName(){return _T("item\\罠２.png");};
	virtual inline int IconFileIndexX(){return 0;};
	virtual inline int IconFileIndexY(){return 2;};

	virtual int 効果();
	virtual int 効果(pcCharacter pchara);
	virtual int 効果(pcDroping pdrop);//pdropに何か起こったらtrue;

};
DEF_class_cTrap_ID(25)

	virtual inline tstring IconFileName(){return _T("item\\罠２.png");};
	virtual inline int IconFileIndexX(){return 1;};
	virtual inline int IconFileIndexY(){return 2;};

	virtual int 効果();
	virtual int 効果(pcCharacter pchara);
	virtual int 効果(pcDroping pdrop);//pdropに何か起こったらtrue;

};
DEF_class_cTrap_ID(26)

	virtual inline tstring IconFileName(){return _T("item\\罠２.png");};
	virtual inline int IconFileIndexX(){return 2;};
	virtual inline int IconFileIndexY(){return 2;};

	virtual int 効果();
	virtual int 効果(pcCharacter pchara);
	virtual int 効果(pcDroping pdrop);//pdropに何か起こったらtrue;

};
DEF_class_cTrap_ID(27)

	virtual inline tstring IconFileName(){return _T("item\\罠２.png");};
	virtual inline int IconFileIndexX(){return 3;};
	virtual inline int IconFileIndexY(){return 2;};

	virtual int 効果();
	virtual int 効果(pcCharacter pchara);
	virtual int 効果(pcDroping pdrop);//pdropに何か起こったらtrue;

};
DEF_class_cTrap_ID(28)

	virtual inline tstring IconFileName(){return _T("item\\罠２.png");};
	virtual inline int IconFileIndexX(){return 0;};
	virtual inline int IconFileIndexY(){return 3;};

	virtual int 効果();
	virtual int 効果(pcCharacter pchara);
	virtual int 効果(pcDroping pdrop);//pdropに何か起こったらtrue;

};
DEF_class_cTrap_ID(29)

	virtual inline tstring IconFileName(){return _T("item\\罠２.png");};
	virtual inline int IconFileIndexX(){return 2;};
	virtual inline int IconFileIndexY(){return 3;};

	virtual int 効果();
	virtual int 効果(pcCharacter pchara);
	virtual int 効果(pcDroping pdrop);//pdropに何か起こったらtrue;

};
DEF_class_cTrap_ID(30)

	virtual inline tstring IconFileName(){return _T("item\\罠２.png");};
	virtual inline int IconFileIndexX(){return 1;};
	virtual inline int IconFileIndexY(){return 3;};

	virtual int 効果();
	virtual int 効果(pcCharacter pchara);
	virtual int 効果(pcDroping pdrop);//pdropに何か起こったらtrue;

	virtual void CutIn(タイミング timing, cValiableField& valiable);

	virtual inline int & sw(){return dpack.integer[6];};//6番
};
DEF_class_cTrap_ID(31)

	virtual inline tstring IconFileName(){return _T("item\\罠３.png");};
	virtual inline int IconFileIndexX(){return 0;};
	virtual inline int IconFileIndexY(){return 0;};

	virtual int 効果();
	virtual int 効果(pcCharacter pchara);
	virtual int 効果(pcDroping pdrop);//pdropに何か起こったらtrue;
	virtual bool 被拾得可(){return false;};
};
#define TRAP_NUM 32