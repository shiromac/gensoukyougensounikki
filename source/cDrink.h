#pragma once
#include "cItem.h"



//--------------------------------------
//値段
//--------------------------------------

#define PRICE_DRINK_SP 999999
#define PRICE_DRINK_SSS 300
#define PRICE_DRINK_SS 200
#define PRICE_DRINK_S 150
#define PRICE_DRINK_A 100
#define PRICE_DRINK_B 50
#define PRICE_DRINK_C 30
#define PRICE_DRINK_D 20
#define PRICE_DRINK_E 10
#define PRICE_DRINK_F 5
#define PRICE_DRINK_G 3
#define PRICE_DRINK_H 1

class cDrink;
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
typedef boost::shared_ptr<cDrink> pcDrink;
typedef boost::weak_ptr<cDrink> wpcDrink;

class cDrink :
	public cItem
{
public:
	cDrink(void);
public:
	virtual ~cDrink(void);



public:
	virtual inline tstring IconFileName(){return _T("item\\ひょうたん.png");};
	virtual inline int IconFileIndexX(){return 0;};
	virtual inline int IconFileIndexY(){return 0;};

	virtual int GetmenuCaption(vector<tstring> &CaptionList);
	virtual StyleString ExtraHatString();

	virtual void AppreciationBeginOptimize(int difficulty);

	virtual void longExplanationExtraBase(std::vector<StyleString> &explanations);

	//コマンドを解決する;
	//virtual int FireCommand(IDirect3DDevice9 *pDev, tstring verb, vector<pcDroping> &ObjectList);

	//---------------------------------
	//識別
	virtual void allAppreciate(){
		cDroping::allAppreciate();
		大局Appreciate();
	};

	virtual StyleString 識別未識別混合道具名();
	virtual int 修正値最大値(){return 4;};
	virtual double 冷気時冷凍確率();
	virtual int 消費時幻想度加算量();
	//--------------------------------------------
	//割り込みイベント
	//--------------------------------------------
	virtual void CutIn(タイミング timing, cValiableField& valiable);

	//--------------------------------------------------------
	//コマンド群
	//virtual int 飲む(IDirect3DDevice9 *pDev);
	//virtual int 飲み要請(pcCharacter pchara);

	//破壊される時true;
	virtual int 飲む_効果_通常(pcCharacter pchara);
	virtual int 飲む_効果_呪い(pcCharacter pchara);
	virtual int 飲む_効果_祝福(pcCharacter pchara);
	virtual int 飲む_メッセージ_通常(pcCharacter pchara);
	virtual int 飲む_メッセージ_呪い(pcCharacter pchara);
	virtual int 飲む_メッセージ_祝福(pcCharacter pchara);

	virtual int 飲むメッセージ定型(pcCharacter pchara);

	//破壊される時true;
	virtual int 汲む_効果_通常(pcCharacter pchara, pcDroping& pdrop);
	virtual int 汲む_効果_呪い(pcCharacter pchara, pcDroping& pdrop);
	virtual int 汲む_効果_祝福(pcCharacter pchara, pcDroping& pdrop);
	virtual int 汲む_メッセージ_通常(pcCharacter pchara, pcDroping pdrop);
	virtual int 汲む_メッセージ_呪い(pcCharacter pchara, pcDroping pdrop);
	virtual int 汲む_メッセージ_祝福(pcCharacter pchara, pcDroping pdrop);

	virtual int 汲むメッセージ定型(pcCharacter pchara, pcDroping pdrop);

	virtual int カクテル_効果(pcCharacter pchara, vector<pcDroping> ObjectList);

	//cCocktailWindowDelegate
	virtual void didFinishCocktail(const vector<pcDroping>& materialList, const vector<pcDroping>& resultCocktails);
	virtual void didCancelCocktail();

	virtual void 属性追加()
	{
		cItem::属性追加();
		属性.insert(落ち物属性::飲み物);
		
	};

	//-----------------------------------
	//データパック_ここから
	//-----------------------------------
	virtual inline UByte & reuse(){return dpack_char_index(4,0);};//4.0番
	virtual inline UByte& cannotdrink(){return dpack_char_index(4,1);};//5,0番
	
	//0~3までdrop
	//4~5まで種類
	//6~8まで具体種
	//9~11までメモ
	//-----------------------------------
	//データパック_ここまで
	//-----------------------------------
	static const int MATERIALNUM = 4;
	virtual int マテリアル(int index);

	virtual int カクテルレシピから削除();

	virtual int 衝突(pcCharacter pchara);

	virtual double 能力スロット();
	virtual void 能力効果量推定テキスト(int index, StyleString& output);
	virtual double 効果量修正値倍率();

	virtual int 衝突時ばら撒き();
	virtual int ばら撒き効果(pcCharacter pchara);
	virtual int 衝突時効果(pcCharacter pchara);
	virtual int 衝突時_メッセージ(pcCharacter pchara);
	
	virtual int 異常状態解除();

	virtual double デフォルト効果量状態倍率_祝福();
	
};
