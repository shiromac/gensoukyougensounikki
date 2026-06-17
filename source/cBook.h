#pragma once
#include "cItem.h"



//--------------------------------------
//値段
//--------------------------------------

#define PRICE_BOOK_SP 999999
#define PRICE_BOOK_SSS 300
#define PRICE_BOOK_SS 200
#define PRICE_BOOK_S 150
#define PRICE_BOOK_A 100
#define PRICE_BOOK_B 50
#define PRICE_BOOK_C 30
#define PRICE_BOOK_D 20
#define PRICE_BOOK_E 10
#define PRICE_BOOK_F 5
#define PRICE_BOOK_G 5
#define PRICE_BOOK_H 5

class cBook;
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
typedef boost::shared_ptr<cBook> pcBook;
typedef boost::weak_ptr<cBook> wpcBook;

class cBook :
	public cItem
{
public:
	cBook(void);
public:
	virtual ~cBook(void);


	virtual void Init(cRenderDevice *pDev,pcDroping self);


	//virtual cDiscreteProbability stateBeginDP(int difficulty);
	//virtual cDiscreteProbability qualityBeginDP(int difficulty);

	virtual void longExplanationExtraBase(std::vector<StyleString> &explanations);



	virtual void aspectedAttackDraw(cRenderDevice *pDev);
	virtual inline int AttackIconFileSize(){return 256;};
	virtual inline tstring AttackIconFileName(){return _T("item\\effectdrop.png");};
	virtual inline int AttackIconFileIndexX(){return 1;};
	virtual int GetAttackIcon(cDrawingObject& DO);
	cRenderTexture* m_pTexture_AttackIcon;

	virtual StyleString QualityCaption();
	virtual StyleString ExtraHatString();

	virtual int 使用難度(){return 魔法難度();};
	virtual int 魔法難度();

	//効果量(係数)
	virtual inline double 効果量係数(){return 1;};//クオリティーにかかわらず固定

	virtual double 読み時神力切れ確率();
	virtual double デフォルト読み時神力切れ確率();
	virtual double 水時濡れ確率();

	virtual int 修正値最大値(){return 99;};

	virtual bool stategoodRunoutprocess();
	//-----------------------------------
	//データパック_ここから
	//-----------------------------------

	public://使用回数
	virtual inline int & usecountminus(){return dpack.integer[4];};//4番


	//0~3までdrop
	//4~5まで種類
	virtual inline UByte& cannotread(){return dpack_char_index(5,0);};//5,0番
	//6~7まで具体種
	//-----------------------------------
	//データパック_ここまで
	//-----------------------------------

	//--------------------------------------------
	//割り込みイベント
	//--------------------------------------------
	virtual void CutIn(タイミング timing, cValiableField& valiable);


	//---------------------------------
	//識別
	virtual void allAppreciate(){
		cDroping::allAppreciate();
		大局Appreciate();
	};

		
	//virtual int 修正値識別済み();
	virtual StyleString OptionCaption();


	virtual StyleString 識別未識別混合道具名();
	

	//--------------------------------------------------------
	//実行群
protected:

	


public:
	virtual inline tstring IconFileName(){return _T("item\\本.png");};
	virtual inline int IconFileIndexX(){return 0;};
	virtual inline int IconFileIndexY(){return 0;};

	virtual int GetmenuCaption(vector<tstring> &CaptionList);



	//コマンドを解決する;
	//virtual int FireCommand(cRenderDevice *pDev, tstring verb, vector<pcDroping> &ObjectList);


	//--------------------------------------------------------
	//コマンド群
public:

	
	
	//virtual int 読む(cRenderDevice *pDev);
	//virtual int 読み要請(pcCharacter pchara);

	//破壊される時true;
	virtual int 読む_効果_通常(pcCharacter pchara);
	virtual int 読む_効果_呪い(pcCharacter pchara);
	virtual int 読む_効果_祝福(pcCharacter pchara);
	virtual int 読む_メッセージ_通常(pcCharacter pchara);
	virtual int 読む_メッセージ_呪い(pcCharacter pchara);
	virtual int 読む_メッセージ_祝福(pcCharacter pchara);

	virtual int 読むメッセージ定型(pcCharacter pchara);

	virtual inline int 加算攻撃力(){return 1;};
	virtual inline double 乗算攻撃力(){return 1;};
	virtual multiset<攻撃属性::攻撃属性> 攻撃属性();

	virtual void 属性追加()
	{
		cItem::属性追加();
		属性.insert(落ち物属性::読み物);
		
	};

	
	virtual int 衝突(pcCharacter pchara);
	virtual int 衝突時効果(pcCharacter pchara);
	virtual int 衝突時_メッセージ(pcCharacter pchara);
	
	virtual int 異常状態解除();
	
	virtual int 効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect, int flag = 0);
	virtual inline int 効果対象_キャラ(){return true;};
	virtual inline int 効果対象_アイテム(){return true;};
	virtual inline int 効果対象_地形(){return true;};
	virtual inline int 反射回数(){return 1;};

	//エフェクト
	virtual int 溜めエフェクト(pcCharacter pchara);
	virtual int 反射エフェクト(c4DVector place, int aspect);
	virtual int 弾切れエフェクト(pcCharacter pchara);


	virtual unsigned long imagecolorConcrete();
	virtual unsigned long imagecolor();
};
