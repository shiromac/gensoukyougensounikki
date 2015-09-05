#pragma once

#include "cEquipment.h"


#include <vector>
#include <list>
#include <tstring_ph.h>


using namespace std;

#include <boost/preprocessor.hpp>








#define DEF_class_cEquipment_ID(identify) \
class cEquipment_ID_##identify## :\
	public cEquipment\
{\
public:\
	cEquipment_ID_##identify##(void){};\
public:\
	virtual ~cEquipment_ID_##identify##(void){};\
	virtual inline int ID(){return CDROPING_cEquipment_BASE_ID_NUM + identify;};\
public:\




DEF_class_cEquipment_ID(0)
/*

	virtual StyleString shortExplanationBase(){return (tstring)
_T("侍が持つ刀。日本刀。\n")+
_T("数打物なのでたいした攻撃力はない。\n")
;};
	*/



};

DEF_class_cEquipment_ID(1)
/*

	virtual StyleString shortExplanationBase(){return (tstring)
_T("西洋物の長さが一尺以下の剣。\n")+
_T("刃が丈夫なので打ち払いに適している。\n")

;};
	*/


};

DEF_class_cEquipment_ID(2)
/*

	virtual StyleString shortExplanationBase(){return (tstring)
_T("死神が持つ鎌。\n")+
_T("観光用なのでたいした攻撃力はない。\n")
;};
	*/


};

//ドリルはごろも
DEF_class_cEquipment_ID(3)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
	//-----------------------------------
	//データパック_ここから
	//-----------------------------------
	virtual inline int & 穴掘り_count(){return dpack.integer[6];};//6番
	virtual inline int & 穴掘り_max(){return dpack.integer[7];};//7番
	//6~8まで具体種
	//-----------------------------------
	//データパック_ここまで
	//-----------------------------------
	virtual void DataBeginOptimize(int difficulty);
};

//びっくり傘
DEF_class_cEquipment_ID(4)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
	virtual double 発動率();
	virtual double 効果ターン();
};

//白楼剣
DEF_class_cEquipment_ID(5)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
	virtual double 発動率();
};

//楼観剣
DEF_class_cEquipment_ID(6)
};


//三日月戟
DEF_class_cEquipment_ID(7)
};

//大鎌
DEF_class_cEquipment_ID(8)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};

//妖扇
DEF_class_cEquipment_ID(9)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};

//ヤタノ鏡
DEF_class_cEquipment_ID(10)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//ヤサカニノ勾玉
DEF_class_cEquipment_ID(11)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//霧雨の剣
DEF_class_cEquipment_ID(12)
};
//アマノムラクモノ剣
DEF_class_cEquipment_ID(13)
};
//イナバの杵
DEF_class_cEquipment_ID(14)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//ｽﾋﾟｱ･ｻﾞ･ｸﾞﾝｸﾞﾆﾙ
DEF_class_cEquipment_ID(15)
	//virtual void 能力(const タイミング timing, cValiableField& valiable);
};

//スノートライデント
DEF_class_cEquipment_ID(16)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};

//漆黒の大カミソリ
DEF_class_cEquipment_ID(17)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};

//鬼切
DEF_class_cEquipment_ID(18)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//神切
DEF_class_cEquipment_ID(19)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//人切
DEF_class_cEquipment_ID(20)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//妖切
DEF_class_cEquipment_ID(21)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//妖精切
DEF_class_cEquipment_ID(22)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//青竜刀
DEF_class_cEquipment_ID(23)
	//virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//磁器刀
DEF_class_cEquipment_ID(24)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//犬走刀
DEF_class_cEquipment_ID(25)
	//virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//天狗の盾
DEF_class_cEquipment_ID(26)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//天狗団扇
DEF_class_cEquipment_ID(27)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//のびーるアーム
DEF_class_cEquipment_ID(28)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//卒塔婆
DEF_class_cEquipment_ID(29)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//レーヴァテイン
DEF_class_cEquipment_ID(30)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//悔悟の棒
DEF_class_cEquipment_ID(31)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//浄玻璃
DEF_class_cEquipment_ID(32)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//お払い棒
DEF_class_cEquipment_ID(33)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//魔女箒
DEF_class_cEquipment_ID(34)
	//virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//フラワーパラソル
DEF_class_cEquipment_ID(35)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//八雲卍傘
DEF_class_cEquipment_ID(36)
	//virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//鎖分銅
DEF_class_cEquipment_ID(37)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//巫女針
DEF_class_cEquipment_ID(38)
	//virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//ミニ八卦炉
DEF_class_cEquipment_ID(39)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};

//バトン
DEF_class_cEquipment_ID(40)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};

//警策
DEF_class_cEquipment_ID(41)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};

//壊れたお守り
DEF_class_cEquipment_ID(42)
	//virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//分厚い本
DEF_class_cEquipment_ID(43)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//猫車
DEF_class_cEquipment_ID(44)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//宝塔
DEF_class_cEquipment_ID(45)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//イカリ
DEF_class_cEquipment_ID(46)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//大盃
DEF_class_cEquipment_ID(47)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//大巻物
DEF_class_cEquipment_ID(48)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//制御棒
DEF_class_cEquipment_ID(49)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//氷柱
DEF_class_cEquipment_ID(50)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//道路標識
DEF_class_cEquipment_ID(51)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//要石
DEF_class_cEquipment_ID(52)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};
//緋想の剣 
DEF_class_cEquipment_ID(53)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};

//御幣
DEF_class_cEquipment_ID(54)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};

//必殺の武器
DEF_class_cEquipment_ID(55)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};

//金の延べ棒
DEF_class_cEquipment_ID(56)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};

//マンドラゴラの鉢
DEF_class_cEquipment_ID(57)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};

//蛍光刀
DEF_class_cEquipment_ID(58)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};

//柄杓
DEF_class_cEquipment_ID(59)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};

//エレキテルの羽衣
DEF_class_cEquipment_ID(60)
	virtual void 能力(const タイミング timing, cValiableField& valiable);
};

//チャクラム
DEF_class_cEquipment_ID(61)
	//virtual void 能力(const タイミング timing, cValiableField& valiable);
};

#define EQUIPMENT_NUM 62