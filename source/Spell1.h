#pragma once

#include "cSpell.h"


#include <vector>
#include <list>
#include <tstring_ph.h>


using namespace std;

#include <boost/preprocessor.hpp>



/*
const tstring cSpell_UnknownNamePool[] = {
	_T("クラシックな図の符"),
	_T("モダンな図の符"),
	_T("精密な模様の符"),
	_T("幾何学模様の符"),
	_T("シックな図の符"),
	_T("華やかな符"),
	_T("優雅な図の符"),
	_T("幻想的な図の符"),
	_T("妖艶な図の符"),
	_T("荘厳な図の符"),//10
	_T("静謐な図の符"),
	_T("凄絶な図の符"),
	_T("エレガントな符"),
	_T("端麗な図の符"),
	_T("綺麗な符"),
	_T("スマートな図の符"),
	//_T("匂い立つような符"),
	_T("艶やかな符"),
	_T("絶景の図の符"),
	_T("言語に絶する符"),//20
	_T("秀麗な図の符"),
	_T("神々しい符"),
	//_T("澄み切った符"),
	_T("鮮やかな符"),
	_T("きらびやかな符"),
	_T("雅やかな符"),
	_T("霊妙な図の符"),
	_T("奥ゆかしい図の符"),
	_T("うっとりする図の符"),
	_T("ロマンチックな図の符"),//30
	_T("立派な符"),
	_T("瀟洒な符"),
	_T("美麗な図の符"),
	_T("抒情的な図の符"),
	_T("印象的な図の符"),
	_T("感動的な図の符"),
	_T("格調高い符"),
	_T("力強い図の符"),
	_T("見事な符"),
	_T("圧倒される図の符"),//40
	_T("鬼気せまる図の符"),
	_T("迫力のある符"),
	_T("妖しい符"),
	_T("爽やかな図の符"),
	_T("雄雄しい図の符"),
	_T("堂々とした図の符"),
	_T("おどろおどろしい符"),
	_T("憮然とした符"),
	_T("唐草模様の符"),//50
	_T("市松模様の符"),
	_T("水玉模様の符"),
	_T("縞模様の符"),
	_T("矢絣模様の符"),
	_T("麻の葉模様の符"),
	_T("千鳥格子模様の符"),
	_T("控えめな模様の符"),
	_T("物々しい図の符"),
	//_T("アンティークな符"),
	_T("胡散くさい符"),//60
	_T("厳めしい符"),
	_T("不穏な雰囲気の符"),
	_T("ただならぬ雰囲気の符"),
	_T("物騒な雰囲気の符"),
	_T("危ない雰囲気の符"),
	_T("険呑な雰囲気の符"),
	_T("緊迫した雰囲気の符"),
	_T("大仰な雰囲気の符"),
	_T("厳粛な雰囲気の符"),
	_T("快い雰囲気の符"),//70
	_T("愉快な図の符"),
	_T("面白い図の符"),
	_T("晴れやかな図の符"),
	_T("爽快な雰囲気の符"),
	_T("すっきりした図の符"),
	_T("シンプルな図の符"),
	_T("興味深い図の符"),
	_T("楽しい図の符"),
	_T("風流な図の符"),
	_T("お洒落な符"),//80
	_T("賑やかな図の符"),
	_T("優美な図の符"),
	_T("幽玄な図の符")
};
*/



#define DEF_class_cSpell_ID(identify) \
class cSpell_ID_##identify## :\
	public cSpell\
{\
public:\
	cSpell_ID_##identify##(void){};\
public:\
	virtual ~cSpell_ID_##identify##(void){};\
	virtual inline int ID(){return CDROPING_cSpell_BASE_ID_NUM + identify;};\
public:\




DEF_class_cSpell_ID(0)

/*
	virtual StyleString SpellTitle()
		{return _T("識別");};
	virtual StyleString SpellName()
		{return _T("未知のアイテムの名称と用途がわかる程度の能力");};


	virtual tstring FullNameBase()
		{return _T("覚識「森近霖之助」");};
	virtual tstring ShortNameBase()
		{return _T("森近霖之助");};
	virtual tstring usefulName()
		{return _T("コーリンの符");};

	//イメージカラー
	virtual unsigned long imagecolar1_real()
	{return 0xFF60A0B0;};
	virtual unsigned long imagecolar2_real()
	{return 0xFF505060;};


	virtual int 値段基礎値(){return PRICE_SPELL_E;};
	
	//レア度(出現確率基礎値)
	virtual double レア度(){return RARITY_F;};

	virtual StyleString shortExplanationBase(){return (tstring)
_T("所持者不明の森近霖之助の人格を体現した符\n") +setStyle(
_T("宣言するとアイテムを一つ識別できる\n"),ITEM_NOTICE_COLOR) +setStyle(
_T("装備すると"),ITEM_NOTICE_COLOR2)

;};
///　　　５　　　　10　　　　15　　　　20|
	virtual StyleString longExplanationBase(){return (tstring)
_T("覚識「森近霖之助」\n") + 
_T("所持者不明の森近霖之助の人格を体現した符") + 
_T("")

;};
*/

	virtual int オブジェクト宣言可能(int objectsize)
		{return (objectsize >= 1);};//オブジェクト要求数 1


	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);
	//virtual int 宣言_メッセージ_通常(pcCharacter pchara);


	virtual int 宣言_効果_祝福(pcCharacter pchara ,vector<pcDroping> &ObjectList);
	
	int 効果(pcCharacter pchara ,vector<pcDroping> &ObjectList);
	virtual void CutIn(タイミング timing, cValiableField& valiable);
	
	virtual int オブジェクトスキル可能(int objectsize)
		{return (objectsize >= 1);};//オブジェクト要求数 1

	virtual int スキルあり(pcCharacter pchara){return true;};
	
	virtual int スキル効果(pcCharacter pchara, vector<pcDroping> &ObjectList);
	
};

DEF_class_cSpell_ID(1)

/*
	virtual StyleString SpellTitle()
		{return _T("灯符");};
	virtual StyleString SpellName()
		{return _T("ファイヤフライフェノメノン");};


	virtual tstring FullNameBase()
		{return _T("操蟲「リグル・ナイトバグ」");};
	virtual tstring ShortNameBase()
		{return _T("リグル・ナイトバグ");};
	virtual tstring usefulName()
		{return _T("リグル・ナイトバグの符");};

	//イメージカラー
	virtual unsigned long imagecolar1_real()
	{return 0xFFDDFF55;};
	virtual unsigned long imagecolar2_real()
	{return 0xFF22AA55;};


	virtual int 値段基礎値(){return PRICE_SPELL_D;};
	
	//レア度(出現確率基礎値)
	virtual double レア度(){return RARITY_E;};

	virtual StyleString shortExplanationBase(){return (tstring)
_T("所持者不明のリグル・Nの人格を体現した符\n") +setStyle(
_T("宣言するとアイテムを一つ識別できる\n"),ITEM_NOTICE_COLOR) +setStyle(
_T("装備すると"),ITEM_NOTICE_COLOR2)

;};
///　　　５　　　　10　　　　15　　　　20|
	virtual StyleString longExplanationBase(){return (tstring)
_T("操蟲「リグル・ナイトバグ」\n") + 
_T("所持者不明のリグル・ナイトバグの人格を体現した符") + 
_T("")

;};/* ** */

	/*
	virtual int オブジェクト宣言可能(int objectsize)
		{return (objectsize >= 1);};//オブジェクト要求数 1
	*/

	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);
	//virtual int 宣言_メッセージ_通常(pcCharacter pchara);


	virtual int 宣言_効果_祝福(pcCharacter pchara ,vector<pcDroping> &ObjectList);
	
	int 効果(pcCharacter pchara ,vector<pcDroping> &ObjectList);
	virtual void CutIn(タイミング timing, cValiableField& valiable);
};

DEF_class_cSpell_ID(2)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);
	virtual int 宣言_効果_祝福(pcCharacter pchara ,vector<pcDroping> &ObjectList){return 宣言_効果_通常(pchara,ObjectList);};
	
	virtual void CutIn(タイミング timing, cValiableField& valiable);
	int 効果();
};
DEF_class_cSpell_ID(3)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);

	virtual void CutIn(タイミング timing, cValiableField& valiable);
	int 効果(pcCharacter pchara);
};
DEF_class_cSpell_ID(4)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);

	virtual void CutIn(タイミング timing, cValiableField& valiable);
	int 効果(pcCharacter pchara, int aspect);
};
DEF_class_cSpell_ID(5)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);
	virtual int 宣言_効果_呪い(pcCharacter pchara ,vector<pcDroping> &ObjectList){return 宣言_効果_通常(pchara, ObjectList);};
	virtual int 宣言_メッセージ_呪い(pcCharacter pchara ,vector<pcDroping> &ObjectList){return 宣言_メッセージ_通常(pchara, ObjectList);};
	virtual void CutIn(タイミング timing, cValiableField& valiable);
	int 効果(pcCharacter pchara);
};
DEF_class_cSpell_ID(6)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);

	virtual void CutIn(タイミング timing, cValiableField& valiable);
	int 効果(pcCharacter pchara, int aspect);
};
DEF_class_cSpell_ID(7)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);

	virtual void CutIn(タイミング timing, cValiableField& valiable);
	int 効果(pcCharacter pchara);

	virtual int スキルあり(pcCharacter pchara){return true;};
	
	virtual int スキル効果(pcCharacter pchara, vector<pcDroping> &ObjectList);
	
};
DEF_class_cSpell_ID(8)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);

	virtual void CutIn(タイミング timing, cValiableField& valiable);
	int 効果(pcCharacter pchara);
};
DEF_class_cSpell_ID(9)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);

	virtual void CutIn(タイミング timing, cValiableField& valiable);
	int 効果(pcCharacter pchara);
};
DEF_class_cSpell_ID(10)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);

	virtual void CutIn(タイミング timing, cValiableField& valiable);
	int 効果(pcCharacter pchara);
};
DEF_class_cSpell_ID(11)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);

	virtual void CutIn(タイミング timing, cValiableField& valiable);
	int 効果(pcCharacter pchara ,vector<pcDroping> &ObjectList);
	virtual int オブジェクト宣言可能(int objectsize)
		{return (objectsize >= 1);};//オブジェクト要求数 1
};
DEF_class_cSpell_ID(12)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);

	virtual void CutIn(タイミング timing, cValiableField& valiable);
	int 効果(pcCharacter pchara);
};
DEF_class_cSpell_ID(13)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);

	virtual void CutIn(タイミング timing, cValiableField& valiable);
	int 効果(pcCharacter pchara);
};
DEF_class_cSpell_ID(14)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);

	virtual void CutIn(タイミング timing, cValiableField& valiable);
	int 効果(pcCharacter pchara);
};
DEF_class_cSpell_ID(15)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);

	virtual void CutIn(タイミング timing, cValiableField& valiable);
	int 効果(pcCharacter pchara);
};
DEF_class_cSpell_ID(16)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);
	virtual int オブジェクト宣言可能(int objectsize)
		{return (objectsize >= 1);};//オブジェクト要求数 1

	virtual void CutIn(タイミング timing, cValiableField& valiable);
	int 効果(pcCharacter pchara ,vector<pcDroping> &ObjectList);
};
DEF_class_cSpell_ID(17)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);

	virtual void CutIn(タイミング timing, cValiableField& valiable);
	int 効果(pcCharacter pchara );
};
DEF_class_cSpell_ID(18)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);

	virtual void CutIn(タイミング timing, cValiableField& valiable);
	int 効果(pcCharacter pchara );
};
DEF_class_cSpell_ID(19)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);

	virtual void CutIn(タイミング timing, cValiableField& valiable);
	int 効果(pcCharacter pchara );
};
DEF_class_cSpell_ID(20)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);

	virtual void CutIn(タイミング timing, cValiableField& valiable);
	int 効果(pcCharacter pchara );
};
DEF_class_cSpell_ID(21)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);

	virtual void CutIn(タイミング timing, cValiableField& valiable);
	int 効果(pcCharacter pchara );
};
DEF_class_cSpell_ID(22)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);

	virtual void CutIn(タイミング timing, cValiableField& valiable);
	int 効果(pcCharacter pchara );
};
DEF_class_cSpell_ID(23)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);

	virtual void CutIn(タイミング timing, cValiableField& valiable);
	int 効果(pcCharacter pchara );
};
DEF_class_cSpell_ID(24)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);

	virtual void CutIn(タイミング timing, cValiableField& valiable);
	int 効果(pcCharacter pchara );
};
DEF_class_cSpell_ID(25)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);

	virtual void CutIn(タイミング timing, cValiableField& valiable);
	int 効果(pcCharacter pchara );
};
DEF_class_cSpell_ID(26)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);

	virtual void CutIn(タイミング timing, cValiableField& valiable);
	int 効果(pcCharacter pchara );
};
DEF_class_cSpell_ID(27)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);

	virtual void CutIn(タイミング timing, cValiableField& valiable);
	int 効果(pcCharacter pchara );
};
DEF_class_cSpell_ID(28)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);

	virtual void CutIn(タイミング timing, cValiableField& valiable);
	int 効果(pcCharacter pchara );
	//6~8まで具体種
	virtual int& tempmem(){return dpack.integer[6];};
};
DEF_class_cSpell_ID(29)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);
	int 効果(pcCharacter pchara );
	virtual int GetmenuCaption(vector<tstring> &CaptionList);

};
DEF_class_cSpell_ID(30)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);
	int 効果(pcCharacter pchara );
	virtual void CutIn(タイミング timing, cValiableField& valiable);
	
};


DEF_class_cSpell_ID(31)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);
	int 効果(pcCharacter pchara );
	virtual void CutIn(タイミング timing, cValiableField& valiable);
};
DEF_class_cSpell_ID(32)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);
	int 効果(pcCharacter pchara );
	virtual void CutIn(タイミング timing, cValiableField& valiable);
};
DEF_class_cSpell_ID(33)};
DEF_class_cSpell_ID(34)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);
	int 効果(pcCharacter pchara );
	virtual void CutIn(タイミング timing, cValiableField& valiable);
};
DEF_class_cSpell_ID(35)};
DEF_class_cSpell_ID(36)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);
	int 効果(pcCharacter pchara );
	virtual void CutIn(タイミング timing, cValiableField& valiable);
};
DEF_class_cSpell_ID(37)};
DEF_class_cSpell_ID(38)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);
	int 効果(pcDroping pdrop );
	virtual void CutIn(タイミング timing, cValiableField& valiable);
	virtual int オブジェクト宣言可能(int objectsize)
		{return (objectsize >= 1);};//オブジェクト要求数 1
};
DEF_class_cSpell_ID(39)};
DEF_class_cSpell_ID(40)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);
	int 効果(pcDroping pdrop);
	virtual void CutIn(タイミング timing, cValiableField& valiable);
	virtual int オブジェクト宣言可能(int objectsize)
		{return (objectsize >= 1);};//オブジェクト要求数 1
};
DEF_class_cSpell_ID(41)};
DEF_class_cSpell_ID(42)};
DEF_class_cSpell_ID(43)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);
	int 効果(pcCharacter pchara );
	virtual void CutIn(タイミング timing, cValiableField& valiable);
};
DEF_class_cSpell_ID(44)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);
	int 効果(pcCharacter pchara );
	virtual void CutIn(タイミング timing, cValiableField& valiable);
};
DEF_class_cSpell_ID(45)};
DEF_class_cSpell_ID(46)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);
	int 効果(pcCharacter pchara );
	virtual void CutIn(タイミング timing, cValiableField& valiable);
};
DEF_class_cSpell_ID(47)};
DEF_class_cSpell_ID(48)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);
	int 効果(pcCharacter pchara );
	virtual void CutIn(タイミング timing, cValiableField& valiable);
};
DEF_class_cSpell_ID(49)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);
	int 効果(pcCharacter pchara );
	virtual void CutIn(タイミング timing, cValiableField& valiable);
};
DEF_class_cSpell_ID(50)};
DEF_class_cSpell_ID(51)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);
	int 効果(pcCharacter pchara );
	virtual void CutIn(タイミング timing, cValiableField& valiable);
};
DEF_class_cSpell_ID(52)};
DEF_class_cSpell_ID(53)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);
	int 効果(pcCharacter pchara );
	virtual void CutIn(タイミング timing, cValiableField& valiable);
};
DEF_class_cSpell_ID(54)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);
	int 効果(pcCharacter pchara );
	virtual void CutIn(タイミング timing, cValiableField& valiable);
	virtual void LastSpellAfterProcess(pcCharacter equipedChara);
};
DEF_class_cSpell_ID(55)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);
	int 効果(pcCharacter pchara );
	virtual inline int & up_maxhp(){return dpack.integer[6];};//6番
	virtual void CutIn(タイミング timing, cValiableField& valiable);
};
DEF_class_cSpell_ID(56)
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);
	int 効果(pcCharacter pchara );
	virtual void CutIn(タイミング timing, cValiableField& valiable);
};
/*
DEF_class_cSpell_ID(57)
DEF_class_cSpell_ID(58)
DEF_class_cSpell_ID(59)
DEF_class_cSpell_ID(60)
DEF_class_cSpell_ID(61)
DEF_class_cSpell_ID(62)
DEF_class_cSpell_ID(63)
DEF_class_cSpell_ID(64)
DEF_class_cSpell_ID(65)
DEF_class_cSpell_ID(66)
DEF_class_cSpell_ID(67)
DEF_class_cSpell_ID(68)
DEF_class_cSpell_ID(69)
*/
#define SPELL_NUM 57