#pragma once

#include "cBook.h"


#include <vector>
#include <list>
#include <tstring_ph.h>


using namespace std;

#include <boost/preprocessor.hpp>



/*
const tstring cBook_UnknownNamePool[] = {
	_T("青行灯の絵の本"),
	_T("小豆洗いの絵の本"),
	_T("アマノジャクの絵の本"),
	_T("一反木綿の絵の本"),
	_T("一本だたらの絵の本"),
	_T("犬神の絵の本"),
	_T("飯綱の絵の本"),
	_T("海小僧の絵の本"),
	_T("うわばみの絵の本"),
	_T("閻魔大王の絵の本"),
	_T("大蟹の絵の本"),
	_T("大百足の絵の本"),
	_T("大緋鯉の絵の本"),
	_T("大蛸の絵の本"),
	_T("女化稲荷の絵の本"),
	_T("朧車の絵の本"),
	_T("骸骨の絵の本"),
	_T("がしゃどくろの絵の本"),
	_T("河童の絵の本"),
	_T("蟹坊主の絵の本"),
	_T("鎌鼬の絵の本"),
	_T("蝦蟇仙人の絵の本"),
	_T("雷小僧の絵の本"),
	_T("烏天狗の絵の本"),
	_T("唐傘お化けの絵の本"),
	_T("キジムナーの絵の本"),
	_T("狐火の絵の本"),
	_T("九尾の狐の絵の本"),
	_T("清姫の絵の本"),
	_T("麒麟の絵の本"),
	_T("口裂け女の絵の本"),
	_T("首切れ地蔵の絵の本"),
	_T("蜘蛛女の絵の本"),
	_T("下駄化けの絵の本"),
	_T("玄武の絵の本"),
	_T("後鬼の絵の本"),
	_T("小獅子の絵の本"),
	_T("コロポックルの絵の本"),
	_T("児泣き爺の絵の本"),
	_T("木ノ葉天狗の絵の本"),
	_T("犀の絵の本"),
	_T("座敷童子の絵の本"),
	_T("猿神の絵の本"),
	_T("山精の絵の本"),
	_T("獅子の絵の本"),
	_T("地震鯰の絵の本"),
	_T("蛇骨婆の絵の本"),
	_T("酒呑童子の絵の本"),
	_T("しょうけらの絵の本"),
	_T("猩々の絵の本"),
	_T("絡新婦の絵の本"),
	_T("不知火の絵の本"),
	_T("白容裔の絵の本"),
	_T("水虎の絵の本"),
	_T("朱雀の絵の本"),
	_T("砂掛け婆の絵の本"),
	_T("青竜の絵の本"),
	_T("殺生石の絵の本"),
	_T("銭神の絵の本"),
	_T("前鬼の絵の本"),
	_T("の絵の本"),
	_T("だいだらぼっちの絵の本"),
	_T("滝霊王の絵の本"),
	_T("狸和尚の絵の本"),
	_T("狸娘の絵の本"),
	_T("提灯お化けの絵の本"),
	_T("氷柱女房の絵の本"),
	_T("鶴女房の絵の本"),
	_T("土蜘蛛の絵の本"),
	_T("土の子の絵の本"),
	_T("釣瓶下ろしの絵の本"),
	_T("天狐の絵の本"),
	_T("百々目鬼の絵の本"),
	_T("天井嘗の絵の本"),
	_T("鍋坊主の絵の本"),
	_T("なまはげの絵の本"),
	_T("人魚の絵の本"),
	_T("人面樹の絵の本"),
	_T("鵺の絵の本"),
	_T("ぬらりひょんの絵の本"),
	_T("塗壁の絵の本"),
	_T("猫又の絵の本"),
	_T("猫娘の絵の本"),
	_T("野槌の絵の本"),
	_T("獏の絵の本"),
	_T("白沢の絵の本"),
	_T("化け蝙蝠の絵の本"),
	_T("橋姫の絵の本"),
	_T("蛤女房の絵の本"),
	_T("鼻高天狗の絵の本"),
	_T("般若の絵の本"),
	_T("ハンザキの絵の本"),
	_T("一つ目小僧の絵の本"),
	_T("一目連の絵の本"),
	_T("火鼠の絵の本"),
	_T("白狐の絵の本"),
	_T("白虎の絵の本"),
	_T("飛龍の絵の本"),
	_T("風神の絵の本"),
	_T("文福茶釜の絵の本"),
	_T("牡丹灯篭の絵の本"),
	_T("舞首の絵の本"),
	_T("目一つ坊の絵の本"),
	_T("八咫烏の絵の本"),
	_T("八面王の絵の本"),
	_T("山犬様の絵の本"),
	_T("八岐大蛇の絵の本"),
	_T("雪女の絵の本"),
	_T("夜雀の絵の本"),
	_T("雷獣の絵の本"),
	_T("雷神の絵の本"),
	_T("雷龍の絵の本"),
	_T("竜神の絵の本"),
	_T("ろくろ首の絵の本"),
	_T("笑い女の絵の本"),


	_T("バアルの絵の本"),
	_T("アガレスの絵の本"),
	_T("ウァサゴの絵の本"),
	_T("ガミジンの絵の本"),
	_T("マルバスの絵の本"),
	_T("ウァレフォルの絵の本"),
	_T("アモンの絵の本"),
	_T("バルバトスの絵の本"),
	_T("パイモンの絵の本"),
	_T("ブエルの絵の本"),
	_T("グシオンの絵の本"),
	_T("シトリーの絵の本"),
	_T("ベレトの絵の本"),
	_T("レラジェの絵の本"),
	_T("エリゴスの絵の本"),
	_T("ゼパルの絵の本"),
	_T("ボティスの絵の本"),
	_T("バティンの絵の本"),
	_T("サレオスの絵の本"),
	_T("プルソンの絵の本"),
	_T("モラクスの絵の本"),
	_T("イポスの絵の本"),
	_T("アイムの絵の本"),
	_T("ナベリウスの絵の本"),
	_T("カッシモラルの絵の本"),
	_T("ブネの絵の本"),
	_T("ロノウェの絵の本"),
	_T("ベリトの絵の本"),
	_T("アスタロトの絵の本"),
	_T("フォルネウスの絵の本"),
	_T("フォラスの絵の本"),
	_T("アスモデウスの絵の本"),
	_T("ガープの絵の本"),
	_T("フルフルの絵の本"),
	_T("マルコシアスの絵の本"),
	_T("ストラスの絵の本"),
	_T("フェニックスの絵の本"),
	_T("ハルファスの絵の本"),
	_T("マルファスの絵の本"),
	_T("ラウムの絵の本"),
	_T("フォカロルの絵の本"),
	_T("ウェパルの絵の本"),
	_T("サブナックの絵の本"),
	_T("シャックスの絵の本"),
	_T("ウィネの絵の本"),
	_T("ビフロンスの絵の本"),
	_T("ヴアルの絵の本"),
	_T("ハーゲンティの絵の本"),
	_T("プロケルの絵の本"),
	_T("フルカスの絵の本"),
	_T("バラムの絵の本"),
	_T("アロケルの絵の本"),
	_T("カイムの絵の本"),
	_T("ムルムルの絵の本"),
	_T("オロバスの絵の本"),
	_T("グレモリーの絵の本"),
	_T("オセの絵の本"),
	_T("アミーの絵の本"),
	_T("オリアスの絵の本"),
	_T("ウァプラの絵の本"),
	_T("ザガムの絵の本"),
	_T("ウァラクの絵の本"),
	_T("アンドラスの絵の本"),
	_T("フラウロスの絵の本"),
	_T("アンドレアルの絵の本"),
	_T("キマリスの絵の本"),
	_T("アムドゥシアスの絵の本"),
	_T("ベリアルの絵の本"),
	_T("デカラビアの絵の本"),
	_T("セーレの絵の本"),
	_T("ダンタリオンの絵の本"),
	_T("アンドロマリウスの絵の本")

};
*/




#define DEF_class_cBook_ID(identify) \
class cBook_ID_##identify## :\
	public cBook\
{\
public:\
	cBook_ID_##identify##(void){};\
public:\
	virtual ~cBook_ID_##identify##(void){};\
	virtual inline int ID(){return CDROPING_cBook_BASE_ID_NUM + identify;};\
public:\




DEF_class_cBook_ID(0)
/*
	virtual tstring FullNameBase()
		{return _T("メランコリーブック");};
	virtual tstring ShortNameBase()
		{return _T("メランコリーブック");};
	virtual tstring usefulName()
		{return _T("メランコリーブック");};

	virtual int 値段基礎値(){return PRICE_BOOK_D;};
	
	//レア度(出現確率基礎値)
	virtual double レア度(){return RARITY_D;};

	virtual StyleString shortExplanationBase(){return (tstring)
_T("")
;};
///　　　５　　　　10　　　　15　　　　20|
	virtual StyleString longExplanationBase(){return (tstring)
_T("")
;};/* ** 

	virtual inline unsigned long imagecolorConcrete()
		{return 0xFFAA44FF;};
*/
	virtual int 効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect, int flag = 0);
	virtual inline int 効果対象_キャラ(){return true;};
	virtual inline int 効果対象_アイテム(){return false;};
	virtual inline int 効果対象_地形(){return false;};
};

DEF_class_cBook_ID(1)
/*
	virtual tstring FullNameBase()
		{return _T("健脚の魔導書");};
	virtual tstring ShortNameBase()
		{return _T("健脚の魔導書");};
	virtual tstring usefulName()
		{return _T("健脚の魔導書");};

	virtual int 値段基礎値(){return PRICE_BOOK_D;};
	
	//レア度(出現確率基礎値)
	virtual double レア度(){return RARITY_D;};

	virtual StyleString shortExplanationBase(){return (tstring)
_T("")
;};
///　　　５　　　　10　　　　15　　　　20|
	virtual StyleString longExplanationBase(){return (tstring)
_T("")
;};

	virtual inline unsigned long imagecolorConcrete()
		{return 0xFFEEFFFF;};
*/
	virtual int 効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect, int flag = 0);
	virtual inline int 効果対象_キャラ(){return true;};
	virtual inline int 効果対象_アイテム(){return false;};
	virtual inline int 効果対象_地形(){return false;};
};

DEF_class_cBook_ID(2)
//千里眼の魔導書
	virtual int 効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect, int flag = 0);
	virtual inline int 効果対象_キャラ(){return true;};
	virtual inline int 効果対象_アイテム(){return false;};
	virtual inline int 効果対象_地形(){return false;};
	virtual int 反射回数();
};

DEF_class_cBook_ID(3)
//トンネル
	virtual int 効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect, int flag = 0);
	virtual inline int 効果対象_キャラ(){return true;};
	virtual inline int 効果対象_アイテム(){return false;};
	virtual inline int 効果対象_地形(){return false;};

};
DEF_class_cBook_ID(4)
//火柱
	virtual int 効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect, int flag = 0);
	virtual inline int 効果対象_キャラ(){return true;};
	virtual inline int 効果対象_アイテム(){return false;};
	virtual inline int 効果対象_地形(){return false;};

};
DEF_class_cBook_ID(5)
//水脈
	virtual int 効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect, int flag = 0);
	virtual inline int 効果対象_キャラ(){return true;};
	virtual inline int 効果対象_アイテム(){return false;};
	virtual inline int 効果対象_地形(){return false;};

};
DEF_class_cBook_ID(6)
//奇跡
	virtual int 効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect, int flag = 0);
	virtual inline int 効果対象_キャラ(){return true;};
	virtual inline int 効果対象_アイテム(){return false;};
	virtual inline int 効果対象_地形(){return false;};

};
DEF_class_cBook_ID(7)
//回復
	virtual int 効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect, int flag = 0);
	virtual inline int 効果対象_キャラ(){return true;};
	virtual inline int 効果対象_アイテム(){return false;};
	virtual inline int 効果対象_地形(){return false;};

};
DEF_class_cBook_ID(8)
//幸せ
	virtual int 効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect, int flag = 0);
	virtual inline int 効果対象_キャラ(){return true;};
	virtual inline int 効果対象_アイテム(){return false;};
	virtual inline int 効果対象_地形(){return false;};

};
DEF_class_cBook_ID(9)
//不幸
	virtual int 効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect, int flag = 0);
	virtual inline int 効果対象_キャラ(){return true;};
	virtual inline int 効果対象_アイテム(){return false;};
	virtual inline int 効果対象_地形(){return false;};

};
DEF_class_cBook_ID(10)
//吹き飛ばし
	virtual int 効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect, int flag = 0);
	virtual inline int 効果対象_キャラ(){return true;};
	virtual inline int 効果対象_アイテム(){return true;};
	virtual inline int 効果対象_地形(){return false;};

};
DEF_class_cBook_ID(11)
//ひきよせ
	virtual int 効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect, int flag = 0);
	virtual inline int 効果対象_キャラ(){return true;};
	virtual inline int 効果対象_アイテム(){return true;};
	virtual inline int 効果対象_地形(){return false;};

};
DEF_class_cBook_ID(12)
//場所替え
	virtual int 効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect, int flag = 0);
	virtual inline int 効果対象_キャラ(){return true;};
	virtual inline int 効果対象_アイテム(){return false;};
	virtual inline int 効果対象_地形(){return false;};

};
DEF_class_cBook_ID(13)
//金縛り
	virtual int 効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect, int flag = 0);
	virtual inline int 効果対象_キャラ(){return true;};
	virtual inline int 効果対象_アイテム(){return false;};
	virtual inline int 効果対象_地形(){return false;};

};
DEF_class_cBook_ID(14)
//泥酔
	virtual int 効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect, int flag = 0);
	virtual inline int 効果対象_キャラ(){return true;};
	virtual inline int 効果対象_アイテム(){return false;};
	virtual inline int 効果対象_地形(){return false;};

};
DEF_class_cBook_ID(15)
//封印
	virtual int 効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect, int flag = 0);
	virtual inline int 効果対象_キャラ(){return true;};
	virtual inline int 効果対象_アイテム(){return false;};
	virtual inline int 効果対象_地形(){return false;};

};
DEF_class_cBook_ID(16)
//空振り
	virtual int 効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect, int flag = 0);
	virtual inline int 効果対象_キャラ(){return true;};
	virtual inline int 効果対象_アイテム(){return false;};
	virtual inline int 効果対象_地形(){return false;};

};
DEF_class_cBook_ID(17)
//貧乏
	virtual int 効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect, int flag = 0);
	virtual inline int 効果対象_キャラ(){return true;};
	virtual inline int 効果対象_アイテム(){return false;};
	virtual inline int 効果対象_地形(){return false;};

};
DEF_class_cBook_ID(18)
//パチュリーの魔導書（敵専用）
	virtual int 効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect, int flag = 0);
	virtual void AppreciationBeginOptimize(int difficulty);
	//-----------------------------------
	//データパック_ここから
	//-----------------------------------
	virtual inline int & power(){return dpack.integer[6];};//6番
	//0~3までdrop
	//4~5まで種類
	//6~8まで具体種
	//9~11までメモ
	//-----------------------------------
	//データパック_ここまで
	//-----------------------------------
	virtual inline int 効果対象_キャラ(){return true;};
	virtual inline int 効果対象_アイテム(){return false;};
	virtual inline int 効果対象_地形(){return false;};

};
DEF_class_cBook_ID(19)
//アヤの風の魔導書（敵専用）
	virtual int 効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect, int flag = 0);
	virtual void AppreciationBeginOptimize(int difficulty);
	//-----------------------------------
	//データパック_ここから
	//-----------------------------------
	virtual inline int & damagepower(){return dpack.integer[6];};//6番
	virtual inline int & distancepower(){return dpack.integer[7];};//7番
	//0~3までdrop
	//4~5まで種類
	//6~8まで具体種
	//9~11までメモ
	//-----------------------------------
	//データパック_ここまで
	//-----------------------------------
	virtual inline int 効果対象_キャラ(){return true;};
	virtual inline int 効果対象_アイテム(){return true;};
	virtual inline int 効果対象_地形(){return false;};
};
#define BOOK_NUM 20