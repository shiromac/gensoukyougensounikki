#pragma once

#include "cBoss.h"


#include <vector>
#include <list>
#include <tstring_ph.h>


using namespace std;

#include <boost/preprocessor.hpp>

#define CCHARACTER_cBoss_BASE_ID_NUM 5000

#define DEF_class_cBoss_ID(identify) \
class cBoss_ID_##identify## :\
	public cBoss\
{\
public:\
	cBoss_ID_##identify##(void){};\
public:\
	virtual ~cBoss_ID_##identify##(void){};\
	virtual inline int ID(){return CCHARACTER_cBoss_BASE_ID_NUM + identify;};\

#define DEF_class_cBoss_ID_based(identify,based) \
class cBoss_ID_##identify## :\
	public cBoss_ID_##based##\
{\
public:\
	cBoss_ID_##identify##(void){};\
public:\
	virtual ~cBoss_ID_##identify##(void){};\
	virtual inline int ID(){return CCHARACTER_cBoss_BASE_ID_NUM + identify;};\


//レミリア
DEF_class_cBoss_ID(0)
	void giveUpEvent();
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 24;};

	//virtual bool loadSpell(int spellIndex);
	virtual pcEnemyAI Get_kindofAI();
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
	
};

//レミリア用フラン
DEF_class_cBoss_ID(1)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 24;};

	virtual pcEnemyAI Get_kindofAI();
	virtual bool isNoSpell(){return true;};

	//スペルデリゲート
	//virtual pcLandform AIお気に入り地形(cValiableField& valiable);
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	//virtual int 特殊攻撃_アニメ(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
	
	


};

//ボスてゐ
DEF_class_cBoss_ID(2)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 22;};

	virtual pcEnemyAI Get_kindofAI();


};

//ボスカナコ
DEF_class_cBoss_ID(3)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 28;};

	virtual pcEnemyAI Get_kindofAI();
};

//ボスヨウム
DEF_class_cBoss_ID(4)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 24;};

	virtual pcEnemyAI Get_kindofAI();
};

//ボスハンヨウム
DEF_class_cBoss_ID(5)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 24;};

	virtual pcEnemyAI Get_kindofAI();
};

//ボス旧アリス
DEF_class_cBoss_ID(6)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 22;};

	virtual pcEnemyAI Get_kindofAI();
};

//レミリア
DEF_class_cBoss_ID_based(7,0)
};
//レミリア用フラン
DEF_class_cBoss_ID_based(8,1)
};
//ボスてゐ
DEF_class_cBoss_ID_based(9,2)
};
//ボスカナコ
DEF_class_cBoss_ID_based(10,3)
};
//ボスヨウム
DEF_class_cBoss_ID_based(11,4)
};
//ボスハンヨウム
DEF_class_cBoss_ID_based(12,5)
};
//ボス旧アリス
DEF_class_cBoss_ID_based(13,6)
};



#define BOSS_NUM 14