#pragma once

#include "cDrink.h"


#include <vector>
#include <list>
#include <tstring_ph.h>


using namespace std;

#include <boost/preprocessor.hpp>




/*
const tstring cDrink_UnknownNamePool[] = {
	_T("赤い液体"),
	_T("青い液体"),
	_T("黒っぽい液体"),
	_T("白っぽい液体"),
	_T("黄色い液体"),
	_T("緑色の液体"),
	_T("茶色い液体"),
	_T("紅色の液体"),
	_T("桃色の液体"),
	_T("黄土色の液体"),//10
	_T("金色の液体"),
	_T("銀色の液体"),
	_T("水色の液体"),
	_T("灰色の液体"),
	_T("茜色の液体"),
	_T("藍色の液体"),
	_T("紫色の液体"),
	_T("橙色の液体"),
	_T("山吹色の液体"),
	_T("どす黒い液体"),//20
	_T("黄緑色の液体"),
	_T("朱色の液体"),
	_T("肌色の液体"),
	_T("こげ茶色の液体"),
	_T("赤茶色の液体"),
	_T("虹色の液体"),
	_T("萌黄色の液体"),
	_T("青紫色の液体"),
	_T("赤紫色の液体"),
	_T("空色の液体"),//30
	_T("桜色の液体"),
	_T("薔薇色の液体"),
	_T("青緑色の液体"),
	_T("半透明な液体"),
	_T("透明な液体")
};
*/


#define DEF_class_cDrink_ID(identify) \
class cDrink_ID_##identify## :\
	public cDrink\
{\
public:\
	cDrink_ID_##identify##(void){};\
public:\
	virtual ~cDrink_ID_##identify##(void){};\
	virtual inline int ID(){return CDROPING_cDrink_BASE_ID_NUM + identify;};\
public:\


#define DEF_class_cDrink_ID_child(identify,parent) \
class cDrink_ID_##identify## :\
	public cDrink_ID_##parent##\
{\
public:\
	cDrink_ID_##identify##(void){};\
public:\
	virtual ~cDrink_ID_##identify##(void){};\
	virtual inline int ID(){return CDROPING_cDrink_BASE_ID_NUM + identify;};\
public:\


DEF_class_cDrink_ID(0)
/*
	virtual tstring FullNameBase()
		{return _T("水道水");};
	virtual tstring ShortNameBase()
		{return _T("水道水");};
	virtual tstring usefulName()
		{return _T("水道水");};

	virtual int 値段基礎値(){return PRICE_DRINK_E;};
	
	//レア度(出現確率基礎値)
	virtual double レア度(){return RARITY_F;};

	virtual StyleString shortExplanationBase(){return (tstring)
_T("水道というところを通って来たらしい水。\n")+setStyle(
_T("飲むとHPが回復する。\n"),ITEM_NOTICE_COLOR)+
_T("HPが最大の時に飲むと最大HPが2増える。\n")
;};
///　　　５　　　　10　　　　15　　　　20|
	virtual StyleString longExplanationBase(){return (tstring)
_T("水道というところを通って来たらしい水。\n")+
_T("外の世界の主流な飲み物らしい。希少品。\n")+
_T("わずかにカルキの匂いがする。あんまりおい")+
_T("しいものではない。良薬は口に苦し。\n")+setStyle(
_T("飲むとHPが回復する。\n"),ITEM_NOTICE_COLOR)+
_T("HPが最大の時に飲むと最大HPが2増える。\n")+
_T("\n")+
_T("\n")+
_T("\n")+
setStyle(
_T("良く見るとチルノのサインが入ってる。なぜだろう・・・\n"),ITEM_NORMAL_COLOR,0.5f,0.5f)

;};/* ** */

	virtual int 飲む_効果_通常(pcCharacter pchara);
	//virtual int 飲む_メッセージ_通常(pcCharacter pchara);



	virtual int 効果(pcCharacter pchara, int recover, int extend);
};


DEF_class_cDrink_ID(1)
/*
	virtual tstring FullNameBase()
		{return _T("鬼の酒「火炎櫻」");};
	virtual tstring ShortNameBase()
		{return _T("火炎櫻");};
	virtual tstring usefulName()
		{return _T("火炎桜");};

	virtual int 値段基礎値(){return PRICE_DRINK_B;};
	
	//レア度(出現確率基礎値)
	virtual double レア度(){return RARITY_D;};

	virtual StyleString shortExplanationBase(){return (tstring)
_T("鬼が飲む酒。超辛口。\n")+setStyle(
_T("飲むと火を噴いて目の前の敵を攻撃する。\n"),ITEM_NOTICE_COLOR)+
_T("投げて当てると敵にダメージ。\n")
;};
///　　　５　　　　10　　　　15　　　　20|
	virtual StyleString longExplanationBase(){return (tstring)
_T("鬼が飲む酒。超辛口。\n")+
_T("アルコール濃度が99％を超えるので\n")
_T("容易に火がつく。\n")+
_T("そのまま飲むのは\n")
_T("鬼でも無い限り止めた方がいいだろう。\n")+
_T("\n")+setStyle(
_T("飲むと火を噴いて目の前の敵を攻撃する。\n"),ITEM_NOTICE_COLOR)+
_T("投げて当てると敵が火だるまになる。\n")+
_T("\n")+
_T("お酒は二十歳になってから。\n")
;};
*/

	virtual int 飲む_効果_通常(pcCharacter pchara);
	virtual int 飲む_メッセージ_通常(pcCharacter pchara);

	//virtual int 飲む_効果_祝福(pcCharacter pchara);
	

	virtual int 衝突時効果(pcCharacter pchara);
	//virtual int 衝突時_メッセージ(pcCharacter pchara);
	
	virtual int 効果(pcCharacter pchara, pcLandform pland, int power);
};

DEF_class_cDrink_ID(2)
/*
	virtual tstring FullNameBase()
		{return _T("脱力水");};
	virtual tstring ShortNameBase()
		{return _T("脱力水");};
	virtual tstring usefulName()
		{return _T("脱力水");};

	virtual int 値段基礎値(){return PRICE_DRINK_D;};
	//レア度(出現確率基礎値)
	virtual double レア度(){return RARITY_D;};

	virtual StyleString shortExplanationBase(){return
_T("脱力水。正体はただの炭酸水。\n")
_T("プラシーボ効果で")+setStyle(_T("鬱になる。\n"),ITEM_NOTICE_COLOR)+
_T("鬱状態では２ターンに１回しか行動できない。\n")
;};
///　　　５　　　　10　　　　15　　　　20|
	virtual StyleString longExplanationBase(){return 
_T("有馬温泉の銀泉から湧き出る水と同成分の水。\n")
_T("脱力水の沸く洞穴に鳥や虫が入るとたちまち \n")
_T("死んでしまう。\n")
_T("脱力水の正体は良質の炭酸水。飲用可。\n")
_T("しかしその名前が余りにも脱力々しいので\n")
+setStyle(
_T("飲むと気が重くなり、鬱状態になる。\n"),ITEM_NOTICE_COLOR)+
_T("しばらくすると思い直して回復する。\n")
_T("\n")
_T("\n")
_T("ところで有馬ってどこ？\n")
;};
*/

	virtual int 飲む_効果_通常(pcCharacter pchara);

	virtual int 飲む_効果_祝福(pcCharacter pchara);
	
	virtual int 効果(pcCharacter pchara, int turn);

};


DEF_class_cDrink_ID(3)
/*
	virtual tstring FullNameBase()
		{return _T("天狗水");};
	virtual tstring ShortNameBase()
		{return _T("天狗水");};
	virtual tstring usefulName()
		{return _T("天狗水");};

	virtual int 値段基礎値(){return PRICE_DRINK_D;};
	//レア度(出現確率基礎値)
	virtual double レア度(){return RARITY_D;};

	virtual StyleString shortExplanationBase(){return
_T("妖怪の山の麓に湧き出る水。らしい。\n")
+setStyle(_T("飲むと倍速状態になる\n"),ITEM_NOTICE_COLOR)+
_T("倍速状態では１ターンに２回行動できる。\n")
;};
///　　　５　　　　10　　　　15　　　　20|
	virtual StyleString longExplanationBase(){return 
_T("妖怪の山の麓に湧き出る水らしい。\n")
_T("妖怪の山の麓は天狗の縄張りなので実際に湧き")
_T("出ているところは天狗にしか見られない。\n")
_T("天狗から買い付けることが可能。\n")
_T("しかしこの飲み物、色がついている上に甘味までする。")
_T("中には発泡するものまであるらしい。\n")
_T("これが沸いて出る泉があるというのも怪しい話である。\n")
+setStyle(
_T("飲むと元気が出てきて、倍速状態になる。\n"),ITEM_NOTICE_COLOR)+
_T("しばらくすると元にもどる。\n")
_T("\n")
;};
*/

	virtual int 飲む_効果_通常(pcCharacter pchara);

	virtual int 飲む_効果_祝福(pcCharacter pchara);
	
	virtual int 効果(pcCharacter pchara, int turn);

};

DEF_class_cDrink_ID(4)

	virtual int 飲む_効果_通常(pcCharacter pchara);

	virtual int 効果(pcCharacter pchara, int turn);

};

DEF_class_cDrink_ID_child(5,0)
};
DEF_class_cDrink_ID_child(6,0)
	virtual void 能力(const タイミング timing, cValiableField& valiable);

};

DEF_class_cDrink_ID(7)
	virtual int 飲む_効果_通常(pcCharacter pchara);
	//virtual int 飲む_メッセージ_通常(pcCharacter pchara);
	virtual void 能力(const タイミング timing, cValiableField& valiable);


	virtual int 効果(pcCharacter pchara, double recoverper);
};

//空
DEF_class_cDrink_ID(8)
	virtual int 衝突時効果(pcCharacter pchara);
	virtual int 衝突時ばら撒き(){return false;};
	virtual int GetmenuCaption(vector<tstring> &CaptionList);
	virtual void AppreciationBeginOptimize(int difficulty);
	virtual int 汲む_効果_通常(pcCharacter pchara, pcDroping& pdrop);
	virtual int 飲む_効果_通常(pcCharacter pchara);

};

//生水
DEF_class_cDrink_ID(9)
	virtual void AppreciationBeginOptimize(int difficulty);
	virtual int 飲む_効果_通常(pcCharacter pchara);
	virtual int 効果(pcCharacter pchara, int power);
	virtual int 衝突時効果(pcCharacter pchara);
	virtual void CutIn(タイミング timing, cValiableField& valiable);
	virtual void 加熱();
	virtual void 冷却();
	virtual tstring FullNameBase();
	//-----------------------------------
	//データパック_ここから
	//-----------------------------------
	//6~8まで具体種
	virtual inline int & temperature(){return dpack.integer[6];};//6番
	//-----------------------------------
	//データパック_ここまで
	//-----------------------------------
};

//水
DEF_class_cDrink_ID_child(10,9)
	virtual int 効果(pcCharacter pchara, int power);
	virtual int 飲む_効果_祝福(pcCharacter pchara);
};

//熱湯
DEF_class_cDrink_ID(11)
	virtual void AppreciationBeginOptimize(int difficulty);
	virtual int 飲む_効果_通常(pcCharacter pchara);
	virtual int 効果(pcCharacter pchara, int power);
	virtual int 衝突時効果(pcCharacter pchara);
	virtual void CutIn(タイミング timing, cValiableField& valiable);
	virtual void 加熱();
	virtual void 冷却();
};

//酒精
DEF_class_cDrink_ID(12)
	virtual int 飲む_効果_通常(pcCharacter pchara);
	virtual int 効果(pcCharacter pchara, int turn);
};

//レベルアップする酒（仮名）
DEF_class_cDrink_ID(13)
	virtual int 飲む_効果_通常(pcCharacter pchara);
	virtual int 効果(pcCharacter pchara, int power);
};
//レベルダウンする酒（仮名）
DEF_class_cDrink_ID(14)
	virtual int 飲む_効果_通常(pcCharacter pchara);
	virtual int 効果(pcCharacter pchara, int power);
};

//高飛びする酒（仮名）
DEF_class_cDrink_ID(15)
	virtual int 飲む_効果_通常(pcCharacter pchara);
	virtual int 効果(pcCharacter pchara);
};
//レベルアップする酒強（仮名）
DEF_class_cDrink_ID_child(16,13)

};

//レベルダウンする酒強（仮名）
DEF_class_cDrink_ID_child(17,14)

};
//胃拡張（仮名）
DEF_class_cDrink_ID(18)
	virtual int 飲む_効果_通常(pcCharacter pchara);
	virtual int 効果(pcCharacter pchara, int power);
};
//胃縮小（仮名）
DEF_class_cDrink_ID(19)
	virtual int 飲む_効果_通常(pcCharacter pchara);
	virtual int 効果(pcCharacter pchara, int power);
};
//脱力（仮名）
DEF_class_cDrink_ID(20)
	virtual int 飲む_効果_通常(pcCharacter pchara);
	virtual int 効果(pcCharacter pchara, int power, int turn);
};
//元気（仮名）
DEF_class_cDrink_ID(21)
	virtual int 飲む_効果_通常(pcCharacter pchara);
	virtual int 効果(pcCharacter pchara, int power, int turn);
};
//目薬
DEF_class_cDrink_ID(22)
	virtual int 飲む_効果_通常(pcCharacter pchara);
	virtual int 効果(pcCharacter pchara);
};
//目薬
DEF_class_cDrink_ID(23)
	virtual int 飲む_効果_通常(pcCharacter pchara);
	virtual int 効果(pcCharacter pchara, int power);
};
//貧乏
DEF_class_cDrink_ID(24)
	virtual int 飲む_効果_通常(pcCharacter pchara);
	virtual int 効果(pcCharacter pchara, int power);
};
//異常状態
DEF_class_cDrink_ID(25)
	virtual int 飲む_効果_通常(pcCharacter pchara);
	virtual int 効果(pcCharacter pchara);
};
//液体窒素
DEF_class_cDrink_ID(26)
	virtual int 飲む_効果_通常(pcCharacter pchara);
	virtual int 効果(pcCharacter pchara, int power);
};
//神便鬼毒酒
DEF_class_cDrink_ID(27)
	virtual int 飲む_効果_通常(pcCharacter pchara);
};
//封印酒
DEF_class_cDrink_ID(28)
	virtual int 飲む_効果_通常(pcCharacter pchara);
};
//国士無双の薬
DEF_class_cDrink_ID(29)
	virtual int 飲む_効果_通常(pcCharacter pchara);
};
//嫉妬のカクテル
DEF_class_cDrink_ID(30)
	virtual int 飲む_効果_通常(pcCharacter pchara);
};
//ハイビール
DEF_class_cDrink_ID(31)
	virtual int 飲む_効果_通常(pcCharacter pchara);
};
//エクスビール
DEF_class_cDrink_ID(32)
	virtual int 飲む_効果_通常(pcCharacter pchara);
};
//おしるこ
DEF_class_cDrink_ID(33)
	virtual int 飲む_効果_通常(pcCharacter pchara);
};
//カレーライス
DEF_class_cDrink_ID(34)
	virtual int 飲む_効果_通常(pcCharacter pchara);
};
#define DRINK_NUM 35
/*
DEF_class_cDrink_ID(999)

	virtual tstring FullNameBase()
		{return _T("とても長い名前のテストをするためだけに作られた飲み物。文字数６０くらいまでやってみようかななんて考えてるんだけど。");};
	virtual tstring ShortNameBase()
		{return _T("名前の長い水");};
	virtual tstring usefulName()
		{return _T("名前の長い水");};

	virtual int 値段基礎値(){return PRICE_DRINK_E;};


	virtual StyleString shortExplanationBase(){return _T("\
名前長すぎってレヴェルじゃねーぞ。\n\
\n\
\n\
");};
//　　　５　　　　10　　　　15　　　　20|
	virtual StyleString longExplanationBase(){return _T("\
\n\
\n\
\
\n\
\n\
\n\
\n\
\n\
\n\
\n\
");};


};
*/

