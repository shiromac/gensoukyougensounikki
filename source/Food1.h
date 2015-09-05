#pragma once

#include "cFood.h"


#include <vector>
#include <list>
#include <tstring_ph.h>


using namespace std;

#include <boost/preprocessor.hpp>






#define DEF_class_cFood(Type) \
class cFood_##Type## :\
	public cFood \
{\
public:\
	cFood_##Type##(void){};\
public:\
	virtual ~cFood_##Type##(void){};\
	virtual inline int ID()=0;\
public:\

//------------------------------
//普通の大きさ
//------------------------------
DEF_class_cFood(Normal)

	//virtual int 値段基礎値(){return PRICE_FOOD_E;};
	
	//レア度(出現確率基礎値)
	//virtual double レア度(){return RARITY_F;};


	virtual int 食べる_効果_通常(pcCharacter pchara);
	//virtual int 飲む_メッセージ_通常(pcCharacter pchara);


	virtual int 食べる_効果_祝福(pcCharacter pchara);
	
	virtual int 効果(pcCharacter pchara, int recover, int extend);
	
	virtual void CutIn(タイミング timing, cValiableField& valiable);
	virtual int 加熱後変化ID() = 0;
};

//------------------------------
//大きい
//------------------------------
DEF_class_cFood(Big)

	//virtual int 値段基礎値(){return PRICE_FOOD_D;};
	
	//レア度(出現確率基礎値)
	//virtual double レア度(){return RARITY_F;};


	virtual int 食べる_効果_通常(pcCharacter pchara);
	//virtual int 飲む_メッセージ_通常(pcCharacter pchara);


	virtual int 食べる_効果_祝福(pcCharacter pchara);
	
	virtual int 効果(pcCharacter pchara, int recover, int extend);
	
	virtual void CutIn(タイミング timing, cValiableField& valiable);

};

//------------------------------
//巨大
//------------------------------
DEF_class_cFood(Large)

	//virtual int 値段基礎値(){return PRICE_FOOD_C;};
	
	//レア度(出現確率基礎値)
	//virtual double レア度(){return RARITY_F;};


	virtual int 食べる_効果_通常(pcCharacter pchara);
	//virtual int 飲む_メッセージ_通常(pcCharacter pchara);


	virtual int 食べる_効果_祝福(pcCharacter pchara);
	
	virtual int 効果(pcCharacter pchara, int recover, int extend);
	
	virtual void CutIn(タイミング timing, cValiableField& valiable);

};

//------------------------------
//くさった
//------------------------------
DEF_class_cFood(Bad)

	//virtual int 値段基礎値(){return PRICE_FOOD_C;};
	
	//レア度(出現確率基礎値)
	//virtual double レア度(){return RARITY_F;};


	virtual int 食べる_効果_通常(pcCharacter pchara);
	//virtual int 飲む_メッセージ_通常(pcCharacter pchara);


	virtual int 食べる_効果_祝福(pcCharacter pchara);
	
	virtual int 効果(pcCharacter pchara, int recover, int extend);


	virtual double 火時性質変化率(){return 1;};
};


//------------------------------
//焼けた
//------------------------------
DEF_class_cFood(Toast)

	//virtual int 値段基礎値(){return PRICE_FOOD_C;};
	
	//レア度(出現確率基礎値)
	//virtual double レア度(){return RARITY_F;};


	virtual int 食べる_効果_通常(pcCharacter pchara);
	//virtual int 飲む_メッセージ_通常(pcCharacter pchara);


	virtual int 食べる_効果_祝福(pcCharacter pchara);
	
	virtual int 効果(pcCharacter pchara, int recover, int extend);

};

//------------------------------
//こげた
//------------------------------
DEF_class_cFood(Burned)

	//virtual int 値段基礎値(){return PRICE_FOOD_C;};
	
	//レア度(出現確率基礎値)
	//virtual double レア度(){return RARITY_F;};


	virtual int 食べる_効果_通常(pcCharacter pchara);
	//virtual int 飲む_メッセージ_通常(pcCharacter pchara);


	virtual int 食べる_効果_祝福(pcCharacter pchara);
	
	virtual int 効果(pcCharacter pchara, int recover);

};

//------------------------------
//黒い
//------------------------------
DEF_class_cFood(Black)

	//virtual int 値段基礎値(){return PRICE_FOOD_C;};
	
	//レア度(出現確率基礎値)
	//virtual double レア度(){return RARITY_F;};


	virtual int 食べる_効果_通常(pcCharacter pchara);
	//virtual int 飲む_メッセージ_通常(pcCharacter pchara);


	virtual int 食べる_効果_祝福(pcCharacter pchara);
	
	virtual int 効果(pcCharacter pchara, int reduse, int damage);
	virtual void 属性追加()
	{
		cFood::属性追加();
		属性.insert(落ち物属性::耐火);
	};
	virtual int 加熱後変化ID(){return ID();};
};

//------------------------------
//普通のイモ
//------------------------------
DEF_class_cFood(Potate)

	//virtual int 値段基礎値(){return PRICE_FOOD_E;};
	
	//レア度(出現確率基礎値)
	//virtual double レア度(){return RARITY_F;};


	virtual int 食べる_効果_通常(pcCharacter pchara);
	//virtual int 飲む_メッセージ_通常(pcCharacter pchara);


	virtual int 食べる_効果_祝福(pcCharacter pchara);
	
	virtual int 効果(pcCharacter pchara, int recover);
	
	virtual void CutIn(タイミング timing, cValiableField& valiable);
	virtual int 加熱後変化ID() = 0;
};

#define DEF_class_cFood_ID(identify,Type) \
class cFood_ID_##identify## :\
	public cFood_##Type## \
{\
public:\
	cFood_ID_##identify##(void){};\
public:\
	virtual ~cFood_ID_##identify##(void){};\
	virtual inline int ID(){return CDROPING_cFood_BASE_ID_NUM + identify;};\
public:\




DEF_class_cFood_ID(0,Normal)

/*
	virtual tstring FullNameBase()
		{return _T("コッペパン");};
	virtual tstring ShortNameBase()
		{return _T("コッペパン");};
	virtual tstring usefulName()
		{return _T("コッペパン");};

	virtual int 値段基礎値(){return PRICE_FOOD_E;};
	
	//レア度(出現確率基礎値)
	virtual double レア度(){return RARITY_F;};

	virtual StyleString shortExplanationBase(){return (tstring)
_T("コッペパン。中国の主食？\n")+setStyle(
_T("食べると満腹度が50回復する。\n"),ITEM_NOTICE_COLOR)+
_T("満腹度が最大の時に食べると最大満腹度が2増える。\n")
;};
///　　　５　　　　10　　　　15　　　　20|
	virtual StyleString longExplanationBase(){return (tstring)
_T("紡錘形で底の平たいパン。\n")+
_T("中国の主食との噂だがはてさて。\n")+
_T("外では甘い果実を煮詰めたものなどをはさんで")
_T("食したりするのが一般的なのだとか。")+
_T("里では西洋食は一般的ではないので里で手に")
_T("入れるのは難しい。\n")
_T("一食分にするには少し大きさが心もとない。\n")
+setStyle(
_T("食べると満腹度が50回復する。\n"),ITEM_NOTICE_COLOR)+
_T("満腹度が最大の時に食べると最大満腹度が2増える。\n")+
_T("\n")
;};/* ** */

	virtual int 加熱後変化ID(){return CDROPING_cFood_BASE_ID_NUM + 4;};
};


DEF_class_cFood_ID(1,Big)
/*
	virtual tstring FullNameBase()
		{return _T("大きなコッペパン");};
	virtual tstring ShortNameBase()
		{return _T("大きなコッペパン");};
	virtual tstring usefulName()
		{return _T("大きなコッペパン");};

	virtual int 値段基礎値(){return PRICE_FOOD_E;};
	
	//レア度(出現確率基礎値)
	virtual double レア度(){return RARITY_F;};

	virtual StyleString shortExplanationBase(){return (tstring)
_T("大きなコッペパン。中国の主食？\n")+setStyle(
_T("食べると満腹度が100回復する。\n"),ITEM_NOTICE_COLOR)+
_T("満腹度が最大の時に食べると最大満腹度が4増える。\n")
;};
///　　　５　　　　10　　　　15　　　　20|
	virtual StyleString longExplanationBase(){return (tstring)
_T("大きな紡錘形で底の平たいパン。\n")+
_T("中国の主食との噂だがはてさて。\n")+
_T("外では甘い果実を煮詰めたものなどをはさんで")
_T("食したりするのが一般的なのだとか。")+
_T("里では西洋食は一般的ではないので里で手に")
_T("入れるのは難しい。")
_T("一食分に丁度良い大きさ。でもちょっと飽きるかも。\n")
+setStyle(
_T("食べると満腹度が100回復する。\n"),ITEM_NOTICE_COLOR)+
_T("満腹度が最大の時に食べると最大満腹度が4増える。\n")
;};/* ** */


	virtual int 加熱後変化ID(){return CDROPING_cFood_BASE_ID_NUM + 7;};
};


DEF_class_cFood_ID(2,Large)
/*
	virtual tstring FullNameBase()
		{return _T("巨大なコッペパン");};
	virtual tstring ShortNameBase()
		{return _T("巨大なコッペパン");};
	virtual tstring usefulName()
		{return _T("巨大なコッペパン");};

	virtual int 値段基礎値(){return PRICE_FOOD_E;};
	
	//レア度(出現確率基礎値)
	virtual double レア度(){return RARITY_F;};

	virtual StyleString shortExplanationBase(){return (tstring)
_T("巨大なコッペパン。中国の主食？\n")+setStyle(
_T("食べると満腹度が200回復する。\n")
_T("さらに最大満腹度が6増える。\n"),ITEM_NOTICE_COLOR)
;};
///　　　５　　　　10　　　　15　　　　20|
	virtual StyleString longExplanationBase(){return (tstring)
_T("巨大な紡錘形で底の平たいパン。\n")+
_T("中国の主食との噂だがはてさて。\n")+
_T("外では甘い果実を煮詰めたものなどをはさんで")
_T("食したりするのが一般的なのだとか。")+
_T("里では西洋食は一般的ではないので里で手に")
_T("入れるのは難しい。\n")
_T("一食分には大きすぎる。これを一気に食べたら")
_T("胃袋が大きくなってしまうかも。\n")
+setStyle(
_T("食べると満腹度が200回復する。\n")
_T("さらに最大満腹度が6増える。\n"),ITEM_NOTICE_COLOR)
;};/* ** */

	virtual int 加熱後変化ID(){return CDROPING_cFood_BASE_ID_NUM + 9;};

};

DEF_class_cFood_ID(3,Bad)
	virtual int 加熱後変化ID(){return CDROPING_cFood_BASE_ID_NUM + 4;};
};

DEF_class_cFood_ID(4,Toast)
	virtual int 加熱後変化ID(){return CDROPING_cFood_BASE_ID_NUM + 5;};
};

DEF_class_cFood_ID(5,Burned)
	virtual int 加熱後変化ID(){return CDROPING_cFood_BASE_ID_NUM + 6;};
};

DEF_class_cFood_ID(6,Black)
};

//大きい
DEF_class_cFood_ID(7,Toast)
	virtual int 加熱後変化ID(){return CDROPING_cFood_BASE_ID_NUM + 8;};
};

DEF_class_cFood_ID(8,Burned)
	virtual int 加熱後変化ID(){return CDROPING_cFood_BASE_ID_NUM + 6;};
};
//巨大な
DEF_class_cFood_ID(9,Toast)
	virtual int 加熱後変化ID(){return CDROPING_cFood_BASE_ID_NUM + 10;};
};

DEF_class_cFood_ID(10,Burned)
	virtual int 加熱後変化ID(){return CDROPING_cFood_BASE_ID_NUM + 6;};
};
DEF_class_cFood_ID(11,Toast)
	virtual int 加熱後変化ID(){return CDROPING_cFood_BASE_ID_NUM + 8;};
};
DEF_class_cFood_ID(12,Potate)
	virtual int 加熱後変化ID(){return CDROPING_cFood_BASE_ID_NUM + 13;};
	virtual inline tstring IconFileName(){return _T("item\\potato.png");};
	virtual inline int IconFileIndexX(){return 0;};
	virtual inline int IconFileIndexY(){return 0;};
};
DEF_class_cFood_ID(13,Potate)
	virtual int 加熱後変化ID(){return CDROPING_cFood_BASE_ID_NUM + 15;};
	virtual inline tstring IconFileName(){return _T("item\\potato.png");};
	virtual inline int IconFileIndexX(){return 0;};
	virtual inline int IconFileIndexY(){return 0;};
};
DEF_class_cFood_ID(14,Bad)
	virtual inline tstring IconFileName(){return _T("item\\potato.png");};
	virtual inline int IconFileIndexX(){return 0;};
	virtual inline int IconFileIndexY(){return 0;};
	virtual int 加熱後変化ID(){return CDROPING_cFood_BASE_ID_NUM + 15;};
};
DEF_class_cFood_ID(15,Black)
	virtual inline tstring IconFileName(){return _T("item\\potato.png");};
	virtual inline int IconFileIndexX(){return 0;};
	virtual inline int IconFileIndexY(){return 0;};
};
#define FOOD_NUM 16