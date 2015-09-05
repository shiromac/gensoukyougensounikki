#pragma once

#include "cPlayerChara.h"


#include <vector>
#include <list>
#include <tstring_ph.h>


using namespace std;

#include <boost/preprocessor.hpp>

#define CCHARACTER_cPlayerChara_BASE_ID_NUM 1000





#define DEF_class_cPlayerChara_ID(identify) \
class cPlayerChara_ID_##identify## :\
	public cPlayerChara\
{\
public:\
	cPlayerChara_ID_##identify##(void){};\
public:\
	virtual ~cPlayerChara_ID_##identify##(void){};\
	virtual inline int ID(){return CCHARACTER_cPlayerChara_BASE_ID_NUM+identify;};





DEF_class_cPlayerChara_ID(0)

/*
	virtual inline tstring FullNameBase()
		{return _T("霧雨 魔理沙");};
	virtual inline tstring FullNameSubBase()
		{return _T("Marisa Kirisame");};
	virtual inline tstring ShortNameBase()
		{return _T("魔理沙");};
	virtual inline tstring ShortNameSubBase()
		{return _T("Marisa");};
	virtual inline tstring usefulName()
		{return _T("マリサ");};
	virtual inline StyleString titleName()
		{return _T("普通の魔法使い");};
*/
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};

/*
	virtual void SetCharaAttribute(){
		cPlayerChara::SetCharaAttribute();
		CHARAATTRI.insert(キャラ属性::人間);
		CHARAATTRI.insert(キャラ属性::魔法使い);
		CHARAATTRI.insert(キャラ属性::泥棒);
	};
	*/
};


DEF_class_cPlayerChara_ID(1)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};

	virtual inline int GetDrawHeadtall_dotY(){return 28;};
};

DEF_class_cPlayerChara_ID(2)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};

	virtual inline int GetDrawHeadtall_dotY(){return 30;};
};

DEF_class_cPlayerChara_ID(3)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};

	virtual inline int GetDrawHeadtall_dotY(){return 24;};
};

DEF_class_cPlayerChara_ID(4)
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_FULLASPECT;};

	virtual inline int GetDrawHeadtall_dotY(){return 26;};
};

#define PLAYERCHARA_NUM 5
