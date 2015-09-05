#pragma once

#include "cDrink.h"


#include <vector>
#include <list>
#include <string>
using namespace std;

#include <boost/preprocessor.hpp>

#define CDROPING_cDrink_BASE_ID_NUM 5000






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




DEF_class_cDrink_ID(0)

	virtual string FullNameBase()
		{return TEXT("水道水");};
	virtual string ShortNameBase()
		{return TEXT("水道水");};
	virtual string usefulName()
		{return TEXT("水道水");};

	virtual int 値段基礎値(){return PRICE_DRINK_E;};
	
	//レア度(出現確率基礎値)
	virtual double レア度(){return RARITY_F;};

	virtual StyleString shortExplanationBase(){return (string)
TEXT("水道というところを通って来たらしい水。\n")+setStyle(
TEXT("飲むとHPが回復する。\n"),ITEM_NOTICE_COLOR)+
TEXT("HPが最大の時に飲むと最大HPが2増える。\n")
;};
///　　　５　　　　10　　　　15　　　　20|
	virtual StyleString longExplanationBase(){return (string)
TEXT("水道というところを通って来たらしい水。\n")+
TEXT("外の世界の主流な飲み物らしい。希少品。\n")+
TEXT("わずかにカルキの匂いがする。あんまりおい")+
TEXT("しいものではない。良薬は口に苦し。\n")+setStyle(
TEXT("飲むとHPが回復する。\n"),ITEM_NOTICE_COLOR)+
TEXT("HPが最大の時に飲むと最大HPが2増える。\n")+
TEXT("\n")+
TEXT("\n")+
TEXT("良く見るとチルノのサインが入ってる。\n")+
TEXT("なぜだろう・・・\n")
;};/* ** */

	virtual int 飲む_効果_通常(pcCharacter pchara);
	//virtual int 飲む_メッセージ_通常(pcCharacter pchara);


	virtual int 飲む_効果_祝福(pcCharacter pchara);
	
	virtual int 効果(pcCharacter pchara, int recover, int extend);
};


DEF_class_cDrink_ID(1)

	virtual string FullNameBase()
		{return TEXT("鬼の酒「火炎櫻」");};
	virtual string ShortNameBase()
		{return TEXT("火炎櫻");};
	virtual string usefulName()
		{return TEXT("火炎桜");};

	virtual int 値段基礎値(){return PRICE_DRINK_B;};
	
	//レア度(出現確率基礎値)
	virtual double レア度(){return RARITY_D;};

	virtual StyleString shortExplanationBase(){return (string)
TEXT("鬼が飲む酒。超辛口。\n")+setStyle(
TEXT("飲むと火を噴いて目の前の敵を攻撃する。\n"),ITEM_NOTICE_COLOR)+
TEXT("投げて当てると敵にダメージ。\n")
;};
///　　　５　　　　10　　　　15　　　　20|
	virtual StyleString longExplanationBase(){return (string)
TEXT("鬼が飲む酒。超辛口。\n")+
TEXT("アルコール濃度が99％を超えるので\n")
TEXT("容易に火がつく。\n")+
TEXT("そのまま飲むのは\n")
TEXT("鬼でも無い限り止めた方がいいだろう。\n")+
TEXT("\n")+setStyle(
TEXT("飲むと火を噴いて目の前の敵を攻撃する。\n"),ITEM_NOTICE_COLOR)+
TEXT("投げて当てると敵が火だるまになる。\n")+
TEXT("\n")+
TEXT("お酒は二十歳になってから。\n")
;};/* ** */

	virtual int 飲む_効果_通常(pcCharacter pchara);
	virtual int 飲む_メッセージ_通常(pcCharacter pchara);


	virtual int 飲む_効果_祝福(pcCharacter pchara);
	

	virtual int 衝突時効果(pcCharacter pchara);
	virtual int 衝突時_メッセージ(pcCharacter pchara);
	
	virtual int 効果(pcCharacter pchara, pcLandform pland, int power);
};

DEF_class_cDrink_ID(2)

	virtual string FullNameBase()
		{return TEXT("毒水");};
	virtual string ShortNameBase()
		{return TEXT("毒水");};
	virtual string usefulName()
		{return TEXT("毒水");};

	virtual int 値段基礎値(){return PRICE_DRINK_D;};
	//レア度(出現確率基礎値)
	virtual double レア度(){return RARITY_D;};

	virtual StyleString shortExplanationBase(){return
TEXT("毒水。正体はただの炭酸水。\n")
TEXT("プラシーボ効果で")+setStyle(TEXT("鬱になる。\n"),ITEM_NOTICE_COLOR)+
TEXT("鬱状態では２ターンに１回しか行動できない。\n")
;};
///　　　５　　　　10　　　　15　　　　20|
	virtual StyleString longExplanationBase(){return 
TEXT("有馬温泉の銀泉から湧き出る水と同成分の水。\n")
TEXT("毒水の沸く洞穴に鳥や虫が入るとたちまち \n")
TEXT("死んでしまう。\n")
TEXT("毒水の正体は良質の炭酸水。飲用可。\n")
TEXT("しかしその名前が余りにも毒々しいので\n")
+setStyle(
TEXT("飲むと気が重くなり、鬱状態になる。\n"),ITEM_NOTICE_COLOR)+
TEXT("しばらくすると思い直して回復する。\n")
TEXT("\n")
TEXT("\n")
TEXT("ところで有馬ってどこ？\n")
;};/* ** */

	virtual int 飲む_効果_通常(pcCharacter pchara);

	virtual int 飲む_効果_祝福(pcCharacter pchara);
	
	virtual int 効果(pcCharacter pchara, int turn);

};


DEF_class_cDrink_ID(3)

	virtual string FullNameBase()
		{return TEXT("天狗水");};
	virtual string ShortNameBase()
		{return TEXT("天狗水");};
	virtual string usefulName()
		{return TEXT("天狗水");};

	virtual int 値段基礎値(){return PRICE_DRINK_C;};
	//レア度(出現確率基礎値)
	virtual double レア度(){return RARITY_D;};

	virtual StyleString shortExplanationBase(){return
TEXT("妖怪の山の麓に湧き出る水。らしい。\n")
+setStyle(TEXT("飲むと倍速状態になる\n"),ITEM_NOTICE_COLOR)+
TEXT("倍速状態では１ターンに２回行動できる。\n")
;};
///　　　５　　　　10　　　　15　　　　20|
	virtual StyleString longExplanationBase(){return 
TEXT("妖怪の山の麓に湧き出る水らしい。\n")
TEXT("妖怪の山の麓は天狗の縄張りなので実際に湧き")
TEXT("出ているところは天狗にしか見られない。\n")
TEXT("天狗から買い付けることが可能。\n")
TEXT("しかしこの飲み物、色がついている上に甘味までする。")
TEXT("中には発泡するものまであるらしい。\n")
TEXT("これが沸いて出る泉があるというのも怪しい話である。\n")
+setStyle(
TEXT("飲むと元気出てきて、倍速状態になる。\n"),ITEM_NOTICE_COLOR)+
TEXT("しばらくすると元にもどる。\n")
TEXT("\n")
;};/* ** */

	virtual int 飲む_効果_通常(pcCharacter pchara);

	virtual int 飲む_効果_祝福(pcCharacter pchara);
	
	virtual int 効果(pcCharacter pchara, int turn);

};


DEF_class_cDrink_ID(4)

	virtual string FullNameBase()
		{return TEXT("とても長い名前のテストをするためだけに作られた飲み物。文字数６０くらいまでやってみようかななんて考えてるんだけど。");};
	virtual string ShortNameBase()
		{return TEXT("名前の長い水");};
	virtual string usefulName()
		{return TEXT("名前の長い水");};

	virtual int 値段基礎値(){return PRICE_DRINK_E;};


	virtual StyleString shortExplanationBase(){return TEXT("\
名前長すぎってレヴェルじゃねーぞ。\n\
\n\
\n\
");};
//　　　５　　　　10　　　　15　　　　20|
	virtual StyleString longExplanationBase(){return TEXT("\
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
");};/* ** */


};


#define DRINK_NUM 4