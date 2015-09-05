#pragma once

#include "cBox.h"


#include <vector>
#include <list>
#include <tstring_ph.h>


using namespace std;

#include <boost/preprocessor.hpp>



/*
const tstring cBox_UnknownNamePool[] = {
	_T("大きめの箱"),
	_T("小さめの箱"),
	_T("豪華な箱"),
	_T("質素な箱"),
	_T("丸い箱"),
	_T("細長い箱"),
	_T("ひし形の箱"),
	_T("くびれた箱"),
	_T("円柱の箱"),
	_T("平たい箱"),//10
	_T("厚い箱"),
	_T("へこんだ箱"),
	_T("とがった箱"),
	_T("星型の箱"),
	_T("やわらかい箱"),
	_T("かたい箱"),
	_T("傷のついた箱"),
	_T("きれいな箱"),
	_T("三日月形の箱"),
	_T("黄金比の箱"),//20
	_T("角錐形の箱"),
	_T("円錐形の箱"),
	_T("もっさりした箱"),
	_T("ふかふかの箱"),
	_T("つるつるの箱"),
	_T("しっとりした箱"),
	_T("左右対称の箱"),
	_T("三角の箱"),
	_T("五角形の箱"),
	_T("六角形の箱"),//30
	_T("四角い箱")
};
*/


#define DEF_class_cBox_ID(identify) \
class cBox_ID_##identify## :\
	public cBox\
{\
public:\
	cBox_ID_##identify##(void){};\
public:\
	virtual ~cBox_ID_##identify##(void){};\
	virtual inline int ID(){return CDROPING_cBox_BASE_ID_NUM + identify;};\
public:\




DEF_class_cBox_ID(0)
/*
	virtual tstring FullNameBase()
		{return _T("空箱");};
	virtual tstring ShortNameBase()
		{return _T("空箱");};
	virtual tstring usefulName()
		{return _T("空箱");};



	virtual int 値段基礎値(){return PRICE_BOX_E;};
	
	//レア度(出現確率基礎値)
	virtual double レア度(){return RARITY_F;};

	virtual StyleString shortExplanationBase(){return (tstring)
_T("空の箱。何が入っていたのかはご想像にお任せします。\n")+setStyle(
_T("さまざまなアイテムを出し入れできる。入れたままの使用も可能\n"),ITEM_NOTICE_COLOR)+
_T("ただし、箱の中に箱は入れられない。\n")
;};
///　　　５　　　　10　　　　15　　　　20|
	virtual StyleString longExplanationBase(){return (tstring)
_T("空の箱。何が入っていたのかはご想像にお任せします。\n")+
_T("\n")+
_T("衝撃を与えると壊れてしまう。\n")+
_T("\n")+setStyle((tstring)
_T("さまざまなアイテムを入れられる。")+
_T("また、入れたアイテムは自由に出すことが出来る。\n")+
_T("さらにアイテムを箱に入れたまま使用できる。\n"),ITEM_NOTICE_COLOR)+
_T("ただし、箱の中に箱は入れられない。\n")+
_T("\n")+
_T("\n")
;};/* ** */

	//virtual int 見る(IDirect3DDevice9 *pDev);
	//virtual int 見る要請();
	virtual inline bool 内容物使用可(){return true;};
	//出せる
	virtual inline bool 出し可(){return true;};
};

DEF_class_cBox_ID(1)
/*
	virtual tstring FullNameBase()
		{return _T("ありがたい箱");};
	virtual tstring ShortNameBase()
		{return _T("ありがたい箱");};
	virtual tstring usefulName()
		{return _T("ありがたい箱");};



	virtual int 値段基礎値(){return PRICE_BOX_E;};
	
	//レア度(出現確率基礎値)
	virtual double レア度(){return RARITY_F;};

	virtual StyleString shortExplanationBase(){return (tstring)
_T("とにもかくにもありがたい箱。\n")+setStyle(
_T("入れたアイテムが信仰状態になる。\n"),ITEM_NOTICE_COLOR)+
_T("ただし、箱の中に箱は入れられない。\n")
;};
///　　　５　　　　10　　　　15　　　　20|
	virtual StyleString longExplanationBase(){return (tstring)
_T("とにかくありがたーい箱\n")+
_T("思わず拝みたくなってしまう。\n")+
_T("\n")+
_T("衝撃を与えると壊れてしまう。\n")+
_T("\n")+setStyle(
_T("入れたアイテムが信仰状態になる。\n"),ITEM_NOTICE_COLOR)+
_T("ただし、箱の中に箱は入れられない。\n")+
_T("\n")+
_T("\n")+
_T("\n")+
_T("\n")
;};/* ** */

virtual int 入れる_効果_通常(pcDroping pdrop);

};

DEF_class_cBox_ID(2)
	virtual inline bool 内容物使用可(){return true;};
	virtual bool 箱入れ可(pcDroping pdrop);
	//出せる
	virtual inline bool 出し可(){return true;};
};
DEF_class_cBox_ID(3)
	virtual inline bool 内容物使用可(){return true;};
	virtual bool 箱入れ可(pcDroping pdrop);
	//出せる
	virtual inline bool 出し可(){return true;};
};
DEF_class_cBox_ID(4)
	virtual inline bool 内容物使用可(){return true;};
	virtual bool 箱入れ可(pcDroping pdrop);
	//出せる
	virtual inline bool 出し可(){return true;};
};
DEF_class_cBox_ID(5)
	virtual inline bool 内容物使用可(){return true;};
	virtual bool 箱入れ可(pcDroping pdrop);
	//出せる
	virtual inline bool 出し可(){return true;};
};
DEF_class_cBox_ID(6)
	virtual inline bool 内容物使用可(){return true;};
	virtual bool 箱入れ可(pcDroping pdrop);
	//出せる
	virtual inline bool 出し可(){return true;};
};
DEF_class_cBox_ID(7)
	virtual inline bool 内容物使用可(){return true;};
	virtual bool 箱入れ可(pcDroping pdrop);
	//出せる
	virtual inline bool 出し可(){return true;};
};
DEF_class_cBox_ID(8)
	virtual bool 被箱入れ可(){return true;};
	virtual bool 箱入れ可(pcDroping pdrop);
	//出せる
	virtual inline bool 出し可(){return true;};
};
DEF_class_cBox_ID(9)
	virtual inline bool 内容物使用可(){return true;};
	virtual bool 箱入れ可(pcDroping pdrop);
	//出せる
	virtual inline bool 出し可(){return true;};
};
DEF_class_cBox_ID(10)
	virtual bool 箱入れ可(pcDroping pdrop);
	virtual int 入れる_効果_通常(pcDroping pdrop);
	virtual int 衝突時効果(pcCharacter pchara);
};
DEF_class_cBox_ID(11)
	virtual int 入れる_効果_通常(pcDroping pdrop);
	virtual bool 箱入れ可(pcDroping pdrop);
};

DEF_class_cBox_ID(12)
	virtual int 入れる_効果_通常(pcDroping pdrop);
};
DEF_class_cBox_ID(13)
	virtual int 入れる_効果_通常(pcDroping pdrop);
};
DEF_class_cBox_ID(14)
	virtual int 入れる_効果_通常(pcDroping pdrop);
};

DEF_class_cBox_ID(15)
	virtual int 開ける_効果_通常();
	virtual inline bool 開け用(){return true;};
};
DEF_class_cBox_ID(16)
	virtual int 開ける_効果_通常();
	virtual inline bool 開け用(){return true;};
	virtual int BreakOut();
	virtual int 効果(pcLandform pland);
};
DEF_class_cBox_ID(17)
	virtual void DataBeginOptimize(int difficulty);
	virtual void AppreciationBeginOptimize(int difficulty);
	virtual int 開ける_メッセージ_通常();
	virtual int 開ける_メッセージ_呪い();
	virtual int 開ける_効果_通常();
	virtual int 開ける_効果_呪い();
	virtual int BreakOut(){return false;};
	virtual inline bool 開け用(){return true;};
	virtual bool 被拾得可(){return false;};
	virtual bool 内包アイテムは実体がない(){return true;};//中のアイテムは値段がタダ扱い
	virtual void CutIn(タイミング timing, cValiableField& valiable);
};
DEF_class_cBox_ID(18)
	virtual int 開ける_効果_通常();
	virtual inline bool 開け用(){return true;};
	virtual int BreakOut();
	virtual int 効果(pcLandform pland);
};
DEF_class_cBox_ID(19)
	virtual int 開ける_効果_通常();
	virtual inline bool 開け用(){return true;};
	virtual int BreakOut();
	virtual int 効果(pcLandform pland);
};
DEF_class_cBox_ID(20)
	virtual int 開ける_効果_通常();
	virtual inline bool 開け用(){return true;};
	virtual int 効果(pcLandform pland);
	virtual void CutIn(タイミング timing, cValiableField& valiable);
};
DEF_class_cBox_ID(21)
	virtual int 入れる_効果_通常(pcDroping pdrop);
	virtual double 各種効果量対応(const int index, const int difficulty);
};
DEF_class_cBox_ID(22)
	virtual int 入れる_効果_通常(pcDroping pdrop);
	virtual double 各種効果量対応(const int index, const int difficulty);
};
DEF_class_cBox_ID(23)
	virtual inline bool 開け用(){return true;};
	virtual int 開ける_効果_通常();
	virtual int 衝突時効果(pcCharacter pchara);
};
DEF_class_cBox_ID(24)
	virtual void DataBeginOptimize(int difficulty);
	virtual void AppreciationBeginOptimize(int difficulty);

	virtual bool 箱入れ可(pcDroping pdrop);
	virtual bool 被拾得可(){return false;};
	virtual inline bool 内容物使用可(){return true;};
	//出せる
	virtual inline bool 出し可(){return true;};
	//-----------------------------------
	//データパック_ここから
	//-----------------------------------

	//容量（修正値上書き）
	//quality()
	virtual inline UByte& material(int index){return dpack_char_index(6,index);};//6番
	virtual inline UByte& used(){return dpack_char_index(8,0);};//8,0番
	//0~3までdrop
	//4~5まで種類
	//6~8まで具体種
	//9~11までメモ
	//-----------------------------------
	//データパック_ここまで
	//-----------------------------------
	virtual int GetmenuCaption(vector<tstring> &CaptionList);
	virtual int 開ける_効果_通常();
	virtual int 開ける_メッセージ_通常(){return true;};
	virtual void SumMaterial(vector<int>& vmaterial);
	
	virtual int Getsmollview(pcGameWindow& smollview);
};
typedef boost::shared_ptr<cBox_ID_24> pcBox_ID_24;
void getMaterialText(const vector<int>& vmaterial, StyleString& output);
void getMaterialTextShort(const vector<int>& vmaterial, StyleString& output);

DEF_class_cBox_ID(25)
	virtual int 入れる_効果_通常(pcDroping pdrop);
};
DEF_class_cBox_ID(26)
	virtual int 入れる_効果_通常(pcDroping pdrop);
};
DEF_class_cBox_ID(27)
	virtual int 入れる_効果_通常(pcDroping pdrop);
	virtual bool 箱入れ可(pcDroping pdrop);
};
DEF_class_cBox_ID(28)
	virtual int 入れる_効果_通常(pcDroping pdrop);
	virtual bool 箱入れ可(pcDroping pdrop);
};
#define BOX_NUM 29