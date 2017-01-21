#pragma once

#include "cMob.h"


#include <vector>
#include <list>
#include <tstring_ph.h>


using namespace std;

#include <boost/preprocessor.hpp>

#define CCHARACTER_cMob_BASE_ID_NUM 2000




#define DEF_class_cMob_ID_parent(identify,parent) \
class cMob_ID_##identify## :\
	public parent\
{\
public:\
	cMob_ID_##identify##(void){};\
public:\
	virtual ~cMob_ID_##identify##(void){};\
	virtual inline int ID(){return CCHARACTER_cMob_BASE_ID_NUM + identify;};

//
#define DEF_class_cMob_ID_parent2(identify,parent1,parent2) \
class cMob_ID_##identify## :\
	public parent1, public parent2\
{\
public:\
	cMob_ID_##identify##(void){};\
public:\
	virtual ~cMob_ID_##identify##(void){};\
	virtual inline int ID(){return CCHARACTER_cMob_BASE_ID_NUM + identify;};

//
#define DEF_class_cMob_ID(identify) DEF_class_cMob_ID_parent(identify,cMob)
/*
#define DEF_class_cMob_ID(identify) \
class cMob_ID_##identify## :\
	public cMob\
{\
public:\
	cMob_ID_##identify##(void){};\
public:\
	virtual ~cMob_ID_##identify##(void){};\
	virtual inline int ID(){return CCHARACTER_cMob_BASE_ID_NUM + identify;};\
	//virtual int attaP();\
	//virtual int deffP();\
public:\
	virtual int SetMHP();\
	virtual int HaveEXP();\
	virtual int PerOfspecialAttack();\
	virtual int appearFloor(int power);\
*/

//ノラケダマ
DEF_class_cMob_ID(0)

	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 12;};

};

//ザコヨウセイ
DEF_class_cMob_ID(1)

	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 22;};

};

//ヨウセイメイド
DEF_class_cMob_ID(2)

	//キャラの方向の型
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 22;};
	virtual pcEnemyAI Get_kindofAI();
	//virtual int 特殊攻撃();
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);

	//virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
	
};

//ダイヨウセイ
DEF_class_cMob_ID(3)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 22;};
	
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);

	virtual int 特殊攻撃_アニメ(cValiableField& valiable){return false;};
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
	
};

//レミリア
DEF_class_cMob_ID(4)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 24;};

	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);

	virtual int 特殊攻撃_アニメ(cValiableField& valiable){return false;};
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
	
};

//フランドール
DEF_class_cMob_ID(5)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 24;};

	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
	
};
//サクヤ
DEF_class_cMob_ID(6)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 30;};
	virtual pcLandform AIお気に入り地形(cValiableField& valiable);
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);

};
//ルーミア
DEF_class_cMob_ID(7)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 27;};
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
};
//ニトリ
DEF_class_cMob_ID(8)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 24;};
	int didGouseiNum;
	virtual int MaxholdNum();
	virtual void 配置処理();
	virtual int AIセミパッシブ特殊攻撃選択(cValiableField& valiable);//SP消費なし優先度高
	virtual int セミパッシブ特殊攻撃効果(cValiableField& valiable);//SP消費なし優先度高
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
};
//エイキ
DEF_class_cMob_ID(9)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 28;};
	
	virtual int AIセミパッシブ特殊攻撃選択(cValiableField& valiable);//SP消費なし優先度高
	virtual int セミパッシブ特殊攻撃効果(cValiableField& valiable);//SP消費なし優先度高
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
	
};
//ヒナ
DEF_class_cMob_ID(10)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_TURN;};
	virtual inline int GetDrawHeadtall_dotY(){return 28;};

	virtual int AIセミパッシブ特殊攻撃選択(cValiableField& valiable);//SP消費なし優先度高
	virtual int セミパッシブ特殊攻撃効果(cValiableField& valiable);//SP消費なし優先度高
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
	
};
//ユユコ
DEF_class_cMob_ID(11)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 28;};
	virtual pcLandform AIお気に入り地形(cValiableField& valiable);
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
	
};
//ヨウム
DEF_class_cMob_ID(12)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 26;};
	
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
};
//チルノ
DEF_class_cMob_ID(13)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 22;};
	
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	//virtual int 特殊攻撃_アニメ(cValiableField& valiable);

	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
	
};
//マリサ
DEF_class_cMob_ID(14)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 26;};
	virtual pcLandform AIお気に入り地形(cValiableField& valiable);
	virtual int AIセミパッシブ特殊攻撃選択(cValiableField& valiable);//SP消費なし優先度高
	virtual int セミパッシブ特殊攻撃効果(cValiableField& valiable);//SP消費なし優先度高
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual int 特殊攻撃_アニメ(cValiableField& valiable){return false;};
	virtual pcEnemyAI Get_kindofAI();
	static double targetPriority(pcCharacter me, pcCharacter object);
	static double targetPriorityHimanageDrop(pcCharacter me, pcDroping object);

};
//パチュリー
DEF_class_cMob_ID(15)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 26;};
	
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual pcEnemyAI Get_kindofAI();
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
	virtual int 特殊攻撃_アニメ(cValiableField& valiable){return false;};
};
//コアクマ
DEF_class_cMob_ID(16)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 26;};
	
	virtual int AIセミパッシブ特殊攻撃選択(cValiableField& valiable);//SP消費なし優先度高
	virtual int セミパッシブ特殊攻撃効果(cValiableField& valiable);//SP消費なし優先度高
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
	
};
//ウドンゲ
DEF_class_cMob_ID(17)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 28;};
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
};
//アヤ
DEF_class_cMob_ID(18)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 32;};
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);

};
//レイム
DEF_class_cMob_ID(19)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 28;};
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
	
};
//リンノスケ
DEF_class_cMob_ID(20)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 32;};
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
	virtual pcEnemyAI Get_kindofAI();
	virtual bool すれ違い許可(pcCharacter pchara);
	virtual int canTalk();
	virtual int TalkEvent();
	virtual inline int MaxholdNum(){return 100;};
};
//黄金毛玉
DEF_class_cMob_ID(21)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 20;};
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
	
};
//サナエ
DEF_class_cMob_ID(22)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 28;};

	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
};
//メイリン
DEF_class_cMob_ID(23)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 30;};
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
	
};
//トキコ
DEF_class_cMob_ID(24)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 24;};
};
//イク
DEF_class_cMob_ID(25)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 28;};
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
	virtual int 特殊攻撃_アニメ(cValiableField& valiable);
};
//シズハ
DEF_class_cMob_ID(26)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 28;};
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
	virtual pcEnemyAI Get_kindofAI();
};
//スイカ
DEF_class_cMob_ID(27)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 24;};
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
	
};
//スワコ
DEF_class_cMob_ID(28)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 24;};
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
	
};
//テンシ
DEF_class_cMob_ID(29)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 28;};
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual int 特殊攻撃_アニメ(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
	
};
//パルスィ
DEF_class_cMob_ID(30)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 28;};
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
	
};
//ミノリコ
DEF_class_cMob_ID(31)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 26;};
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
	virtual int 特殊攻撃_アニメ(cValiableField& valiable);
	
};
//モミジ
DEF_class_cMob_ID(32)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 26;};
	virtual pcLandform AIお気に入り地形(cValiableField& valiable);
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);

};
//ユウギ
DEF_class_cMob_ID(33)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 30;};
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual int 特殊攻撃_アニメ(cValiableField& valiable);
	
};
//キスメ
DEF_class_cMob_ID(34)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 22;};
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	static double 地形評価(pcCharacter me, pcLandform land);
	static void 桂馬地形リスト(pcCharacter me, vector<pcLandform>& landlist); 
};
//オリン
DEF_class_cMob_ID(35)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 28;};
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
	static double 地形評価(pcCharacter me, pcLandform land);
	void ジャンプ地形リスト(pcCharacter me, vector<pcLandform>& landlist);
};
//ヤマメ
DEF_class_cMob_ID(36)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 26;};
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual int 特殊攻撃_アニメ(cValiableField& valiable){return false;};
};
//サトリ
DEF_class_cMob_ID(37)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 24;};
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual int 特殊攻撃_アニメ(cValiableField& valiable){return false;};
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
	int 召喚(int ID, pcCharacter enemy);
};
//コイシ
DEF_class_cMob_ID(38)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 24;};
	virtual pcLandform AIお気に入り地形(cValiableField& valiable);
	virtual void 配置処理();
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
};
//うつほ
DEF_class_cMob_ID(39)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 28;};
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
	virtual int 特殊攻撃_アニメ(cValiableField& valiable){return false;};
};
//けーね
DEF_class_cMob_ID(40)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 28;};
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual int 特殊攻撃_アニメ(cValiableField& valiable){return false;};
};
//アリス
DEF_class_cMob_ID(41)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 26;};	
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual int 特殊攻撃_アニメ(cValiableField& valiable){return false;};
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
	std::vector<pcCharacter> slaveCharacter;
};
//オンリョウ
DEF_class_cMob_ID(42)
	static double targetPriority(pcCharacter me, pcCharacter object);
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 20;};
	virtual pcLandform AIお気に入り地形(cValiableField& valiable);
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
	virtual pcEnemyAI Get_kindofAI();
};
//チェン
DEF_class_cMob_ID(43)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 24;};
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
};
//ラン
DEF_class_cMob_ID(44)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 26;};
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
};
//ユカリ
DEF_class_cMob_ID(45)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 28;};	
	virtual pcLandform AIお気に入り地形(cValiableField& valiable);
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
};
//エイリン
DEF_class_cMob_ID(46)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 28;};
	virtual pcLandform AIお気に入り地形(cValiableField& valiable);
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
	static double targetPriority(pcCharacter me, pcCharacter object);
};
//カグヤ
DEF_class_cMob_ID(47)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 26;};
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);

};
//テイ
DEF_class_cMob_ID(48)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 22;};
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);

};
//モコウ
DEF_class_cMob_ID(49)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 28;};
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
};
//リグル
DEF_class_cMob_ID(50)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 24;};
	//virtual pcLandform AIお気に入り地形(cValiableField& valiable);
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);

};
//ミスティア
DEF_class_cMob_ID(51)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 24;};	
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);

};
class cMob_ID_prismriver;
typedef boost::shared_ptr<cMob_ID_prismriver> pcMob_ID_prismriver;
typedef boost::weak_ptr<cMob_ID_prismriver> wpcMob_ID_prismriver;
class cMob_ID_prismriver :
	public cMob
{
public:\
		   cMob_ID_prismriver(){};
public:\
	virtual ~cMob_ID_prismriver(void){};
	virtual inline int ID() = 0;
	vector<wpcCharacter> friendcharas;
	bool mode_together;
	static void setfriend(pcCharacter friendchara1, pcCharacter friendchara2, pcCharacter friendchara3);
	bool checkFriendLive();
	
	virtual pcLandform AIお気に入り地形(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
};
//メルラン
DEF_class_cMob_ID_parent(52,cMob_ID_prismriver)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 26;};
	virtual void 配置処理();
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
};
//リリカ
DEF_class_cMob_ID_parent(53,cMob_ID_prismriver)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 26;};
	virtual void 配置処理();
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
};
//ルナサ
DEF_class_cMob_ID_parent(54,cMob_ID_prismriver)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 26;};
	virtual void 配置処理();
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
};
//メディスン
DEF_class_cMob_ID(55)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 22;};
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
};
//ユウカ
DEF_class_cMob_ID(56)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 28;};	
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void 配置処理();
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
};
//コマチ
DEF_class_cMob_ID(57)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 28;};
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
};
//ナズーリン
DEF_class_cMob_ID(58)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 26;};
	virtual pcLandform AIお気に入り地形(cValiableField& valiable);
	static double targetPriority(pcCharacter me, pcDroping object);
};
//コガサ
DEF_class_cMob_ID(59)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 26;};
	virtual pcLandform AIお気に入り地形(cValiableField& valiable);
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
};
//イチリン
DEF_class_cMob_ID(60)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 26;};
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
};
//イチリンノミ
DEF_class_cMob_ID(61)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 26;};	
	virtual pcEnemyAI Get_kindofAI();
	pcLandform AIお気に入り地形(cValiableField& valiable);
};
//ウンザン
DEF_class_cMob_ID(62)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 26;};
	virtual pcEnemyAI Get_kindofAI();
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
};
//ムラサ
DEF_class_cMob_ID(63)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 26;};	
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
};
//トラマル
DEF_class_cMob_ID(64)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 26;};
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual int AIセミパッシブ特殊攻撃選択(cValiableField& valiable);//SP消費なし優先度高
	virtual int セミパッシブ特殊攻撃効果(cValiableField& valiable);//SP消費なし優先度高
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
	int 召喚(int ID, pcCharacter enemy);
};
//ヒジリ
DEF_class_cMob_ID(65)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 28;};
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
};
//ヌエ
DEF_class_cMob_ID(66)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 26;};
	virtual pcLandform AIお気に入り地形(cValiableField& valiable);
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
};
//カナコ
DEF_class_cMob_ID(67)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 28;};
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
};
//レティ
DEF_class_cMob_ID(68)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 26;};
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);

};
//リリー
DEF_class_cMob_ID(69)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 24;};	
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
};
//ハタテ
DEF_class_cMob_ID(70)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 26;};
	virtual bool AI移動許可(cValiableField& valiable);
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
};
//ゾンビフェアリー
DEF_class_cMob_ID(71)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 22;};	
};
class cMob_ID_sangessei;
typedef boost::shared_ptr<cMob_ID_sangessei> pcMob_ID_sangessei;
typedef boost::weak_ptr<cMob_ID_sangessei> wpcMob_ID_sangessei;
class cMob_ID_sangessei :
	public cMob
{
public:\
		   cMob_ID_sangessei(){};
public:\
	virtual ~cMob_ID_sangessei(void){};
	virtual inline int ID() = 0;
	vector<wpcCharacter> friendcharas;
	bool mode_together;
	static void setfriend(pcCharacter friendchara1, pcCharacter friendchara2, pcCharacter friendchara3);
	bool checkFriendLive();
	
	virtual pcLandform AIお気に入り地形(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
};
//サニー
DEF_class_cMob_ID_parent(72,cMob_ID_sangessei)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 22;};
	virtual void 配置処理();
};
//ルナ
DEF_class_cMob_ID_parent(73,cMob_ID_sangessei)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 22;};
	virtual void 配置処理();
};
//スター
DEF_class_cMob_ID_parent(74,cMob_ID_sangessei)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 22;};
	virtual void 配置処理();
};
//キモケーネ
DEF_class_cMob_ID(75)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 28;};
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
};
//カセン
DEF_class_cMob_ID(76)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 28;};
	//virtual int AI特殊攻撃選択(cValiableField& valiable);
	//virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
};
//オオワシ
DEF_class_cMob_ID(77)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 28;};
	//virtual int AI特殊攻撃選択(cValiableField& valiable);
	//virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
};
//ライジュウ
DEF_class_cMob_ID(78)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 28;};
	//virtual int AI特殊攻撃選択(cValiableField& valiable);
	//virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
};
//ザコウサギ
DEF_class_cMob_ID(79)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 28;};
	//virtual int AI特殊攻撃選択(cValiableField& valiable);
	//virtual int 特殊攻撃効果(cValiableField& valiable);
};
//ジバクニンギョウ
DEF_class_cMob_ID(80)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 24;};
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual int AIセミパッシブ特殊攻撃選択(cValiableField& valiable);//SP消費なし優先度高
	virtual int セミパッシブ特殊攻撃効果(cValiableField& valiable);//SP消費なし優先度高

};
//ミセニトリ
DEF_class_cMob_ID_parent2(81,cMob,cCommandDelegateObject)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 24;};
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
	virtual pcEnemyAI Get_kindofAI();
	virtual bool すれ違い許可(pcCharacter pchara);
	virtual bool isCanUseShop();
	virtual int canTalk();
	virtual int TalkEvent();
	virtual inline int MaxholdNum(){return 100;};
	virtual void 配置処理();
	virtual int 強化資金();
	virtual int 合成資金();
	int shop_use_count_;

	enum {
		delegateID_reinforce = 0,
		delegateID_combine = 1,
	};
	void didEndCommand(cCommand& caller);
};

#define MOB_NUM 82