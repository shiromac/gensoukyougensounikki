#pragma once
#include "ccharacter.h"
#include "ceaiDanmaku.h"

class cObjectChara;
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
typedef boost::shared_ptr<cObjectChara> pcObjectChara;
typedef boost::weak_ptr<cObjectChara> wpcObjectChara;

class cObjectChara :
	public cCharacter
{
public:
	cObjectChara(void);
public:
	virtual ~cObjectChara(void);

	virtual int HaveEXP(){return 0;};

	//上がったレベル数を返す
	virtual int addEXP(int e){return 0;};
	virtual void OptionDraw(IDirect3DDevice9 *pDev){};


	virtual int LvUP(){return 0;};
	virtual int LvDown(){return 0;};
};


#define CCHARACTER_cObjectChara_BASE_ID_NUM 3000

#define DEF_class_cObjectChara_ID(identify) \
class cObjectChara_ID_##identify## :\
	public cObjectChara\
{\
public:\
	cObjectChara_ID_##identify##(void){};\
public:\
	virtual ~cObjectChara_ID_##identify##(void){};\
	virtual inline int ID(){return CCHARACTER_cObjectChara_BASE_ID_NUM + identify;};\


DEF_class_cObjectChara_ID(0)
	//キャラの方向の型
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};

	virtual pcEnemyAI Get_kindofAI(){return pcEnemyAI(new ceaiNoMotion);};

};

DEF_class_cObjectChara_ID(1)
	//キャラの方向の型
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_SINPLE;};
	
	virtual void CutIn(タイミング timing, cValiableField& valiable);
	virtual pcEnemyAI Get_kindofAI(){return pcEnemyAI(new ceaiNoMotion);};

};

class cEvent;
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cEvent> pcEvent;

namespace 弾幕{
	enum 弾幕色
	{
		アオ = 0,
		マゼンタ,
		アカ,
		ダイダイ,
		キイロ,
		ミドリ,
		シアン,
		クロ,
		弾幕色_NUM,
	};
	enum 弾幕Type
	{
		直進,
		追尾,
	};
	pcCharacter 弾幕召喚(pcLandform land,
		int HP,
		int Forse,
		int speed,
		弾幕Type type,
		弾幕色 color,
		int aspect,
		pcCharacter targetenemy);

	pcEvent レーザー召喚(
		pcLandform land,
		int aspect,
		int length,
		int power,
		int Forse,
		cColor color,
		int readyTurn,
		int attackTurn,
		bool isAttackFriend,
		bool isStopAtWall
		);
};


class cObjectChara_Damaku:
	public cObjectChara
{
public:
	cObjectChara_Damaku(){};
public:
	virtual ~cObjectChara_Damaku(){};
	virtual inline int ID()=0;
	virtual void CutIn(タイミング timing, cValiableField& valiable);
	
	virtual bool 水上歩行(){return true;};
	virtual bool 空中歩行(){return true;};
	virtual bool DefeatCountable(){return false;};

	virtual bool 雑魚属性(){return true;};
	virtual int 通常攻撃_アニメ();

};

#define DEF_class_cObjectChara_ID_Danmaku(identify) \
class cObjectChara_ID_##identify## :\
	public cObjectChara_Damaku\
{\
public:\
	cObjectChara_ID_##identify##(void){};\
public:\
	virtual ~cObjectChara_ID_##identify##(void){};\
	virtual inline int ID(){return CCHARACTER_cObjectChara_BASE_ID_NUM + identify;};\

DEF_class_cObjectChara_ID_Danmaku(2)
	//キャラの方向の型
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};

	virtual pcEnemyAI Get_kindofAI(){return pcEnemyAI(new ceaiDanmaku);};

};
DEF_class_cObjectChara_ID_Danmaku(3)
	//キャラの方向の型
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};

	virtual pcEnemyAI Get_kindofAI(){return pcEnemyAI(new ceaiDanmaku);};

};
DEF_class_cObjectChara_ID_Danmaku(4)
	//キャラの方向の型
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};

	virtual pcEnemyAI Get_kindofAI(){return pcEnemyAI(new ceaiDanmaku);};

};
DEF_class_cObjectChara_ID_Danmaku(5)
	//キャラの方向の型
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};

	virtual pcEnemyAI Get_kindofAI(){return pcEnemyAI(new ceaiDanmaku);};

};
DEF_class_cObjectChara_ID_Danmaku(6)
	//キャラの方向の型
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};

	virtual pcEnemyAI Get_kindofAI(){return pcEnemyAI(new ceaiDanmaku);};

};
DEF_class_cObjectChara_ID_Danmaku(7)
	//キャラの方向の型
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};

	virtual pcEnemyAI Get_kindofAI(){return pcEnemyAI(new ceaiDanmaku);};

};
DEF_class_cObjectChara_ID_Danmaku(8)
	//キャラの方向の型
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};

	virtual pcEnemyAI Get_kindofAI(){return pcEnemyAI(new ceaiDanmaku);};

};
DEF_class_cObjectChara_ID_Danmaku(9)
	//キャラの方向の型
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};

	virtual pcEnemyAI Get_kindofAI(){return pcEnemyAI(new ceaiDanmaku);};

};
DEF_class_cObjectChara_ID_Danmaku(10)
	//キャラの方向の型
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};

	virtual pcEnemyAI Get_kindofAI(){return pcEnemyAI(new ceaiDanmakuTuibi);};

};
DEF_class_cObjectChara_ID_Danmaku(11)
	//キャラの方向の型
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};

	virtual pcEnemyAI Get_kindofAI(){return pcEnemyAI(new ceaiDanmakuTuibi);};

};
DEF_class_cObjectChara_ID_Danmaku(12)
	//キャラの方向の型
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};

	virtual pcEnemyAI Get_kindofAI(){return pcEnemyAI(new ceaiDanmakuTuibi);};

};
DEF_class_cObjectChara_ID_Danmaku(13)
	//キャラの方向の型
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};

	virtual pcEnemyAI Get_kindofAI(){return pcEnemyAI(new ceaiDanmakuTuibi);};

};
DEF_class_cObjectChara_ID_Danmaku(14)
	//キャラの方向の型
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};

	virtual pcEnemyAI Get_kindofAI(){return pcEnemyAI(new ceaiDanmakuTuibi);};

};
DEF_class_cObjectChara_ID_Danmaku(15)
	//キャラの方向の型
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};

	virtual pcEnemyAI Get_kindofAI(){return pcEnemyAI(new ceaiDanmakuTuibi);};

};
DEF_class_cObjectChara_ID_Danmaku(16)
	//キャラの方向の型
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};

	virtual pcEnemyAI Get_kindofAI(){return pcEnemyAI(new ceaiDanmakuTuibi);};

};
DEF_class_cObjectChara_ID_Danmaku(17)
	//キャラの方向の型
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};

	virtual pcEnemyAI Get_kindofAI(){return pcEnemyAI(new ceaiDanmakuTuibi);};

};
#define OBJECTCHARA_NUM 18