#pragma once

#include "cOtherChara.h"


#include <vector>
#include <list>
#include <tstring_ph.h>


using namespace std;

#include <boost/preprocessor.hpp>

#define CCHARACTER_cOtherChara_BASE_ID_NUM 4000





#define DEF_class_cOtherChara_ID(identify) \
class cOtherChara_ID_##identify## :\
	public cOtherChara\
{\
public:\
	cOtherChara_ID_##identify##(void){};\
public:\
	virtual ~cOtherChara_ID_##identify##(void){};\
	virtual inline int ID(){return CCHARACTER_cOtherChara_BASE_ID_NUM+identify;};



DEF_class_cOtherChara_ID(0)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 22;};
	virtual int canTalk(){return true;};
	virtual int TalkEvent();
};

//ヒナ
DEF_class_cOtherChara_ID(1)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_TURN;};
	virtual inline int GetDrawHeadtall_dotY(){return 28;};
	virtual int canTalk(){return true;};
	virtual int TalkEvent();
};

//マリサ
DEF_class_cOtherChara_ID(2)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 26;};
	virtual int canTalk(){return true;};
	virtual void settingInit();
	virtual inline int MaxholdNum(){return 1000;};
	virtual void CutIn(タイミング timing, cValiableField& valiable);//封印されない能力
	virtual int TalkEvent();
};

//サクヤ(イベント用)
DEF_class_cOtherChara_ID(3)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 28;};

};
//アヤ(イベント用)
DEF_class_cOtherChara_ID(4)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 28;};

};

//ニトリ
DEF_class_cOtherChara_ID(5)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 24;};
	virtual int canTalk(){return true;};
	virtual int TalkEvent();
	static void ShopUsedSave();
	static bool isCanUseShop();
	static int 強化資金();
	static int 合成資金();
	static int 改造資金(pcDroping pdrop);
	static int 御霊削除資金();
	static tstring lastUseTimeLavelKey(){return _T("NitoriShop:lastUseTimeLavelKey");};
	static tstring lastUseTimeExpKey(){return _T("NitoriShop:lastUseTimeExpKey");};
};

//兎(イベント用)
DEF_class_cOtherChara_ID(6)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 24;};

};
//えーりん(イベント用)
DEF_class_cOtherChara_ID(7)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 28;};

};
//あきゅう
DEF_class_cOtherChara_ID(8)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 24;};
	//virtual int canTalk(){return true;};
	//virtual int TalkEvent();
};

//テイ交代用
DEF_class_cOtherChara_ID(9)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 24;};
	//virtual int canTalk(){return true;};
	//virtual int TalkEvent();
};
//めーりん交代用
DEF_class_cOtherChara_ID(10)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 24;};
	//virtual int canTalk(){return true;};
	//virtual int TalkEvent();
};
//アリス(イベント用)
DEF_class_cOtherChara_ID(11)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 26;};

	virtual pcEnemyAI Get_kindofAI(){return pcEnemyAI(new ceaiActive);};
};
//人形(イベント用)
DEF_class_cOtherChara_ID(12)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 22;};

	virtual pcEnemyAI Get_kindofAI(){return pcEnemyAI(new ceaiActive);};
};
//旧アリス
DEF_class_cOtherChara_ID(13)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};
	virtual inline int GetDrawHeadtall_dotY(){return 22;};

};
#define OTHERCHARA_NUM 14


DEF_COMMANDCLASS(_NitoriFactory_reinforce, g_Lang(_T("装備品を強化する")))
	cCommand_NitoriFactory_reinforce(int reinforce_fee, StyleString s):reinforce_fee_(reinforce_fee){caption = s;};
	int reinforce_fee_;
	virtual StyleString shortExplanationText(){
		cMoney money(reinforce_fee_);
		std::map<tstring, StyleString > val;
		val[_T("Money")] = money.caption();
		return g_Lang(_T("装備品を強化するshortExplanationText"),val);
	};
	//virtual StyleString shortExplanationText();
};
DEF_COMMANDCLASS(_NitoriFactory_combine, g_Lang(_T("アイテムを合成する")))
	cCommand_NitoriFactory_combine(int combine_fee, StyleString s):combine_fee_(combine_fee){caption = s;};
	int combine_fee_;
	virtual StyleString shortExplanationText(){
		cMoney money(combine_fee_);
		std::map<tstring, StyleString > val;
		val[_T("Money")] = money.caption();
		return g_Lang(_T("アイテムを合成するshortExplanationText"),val);};
	//virtual StyleString shortExplanationText();
};
