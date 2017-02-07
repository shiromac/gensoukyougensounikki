#pragma once
#include "cItem.h"



//--------------------------------------
//値段
//--------------------------------------

#define PRICE_FOOD_SP 999999
#define PRICE_FOOD_SSS 300
#define PRICE_FOOD_SS 200
#define PRICE_FOOD_S 150
#define PRICE_FOOD_A 100
#define PRICE_FOOD_B 50
#define PRICE_FOOD_C 30
#define PRICE_FOOD_D 20
#define PRICE_FOOD_E 10
#define PRICE_FOOD_F 5
#define PRICE_FOOD_G 3
#define PRICE_FOOD_H 1

class cFood;
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
typedef boost::shared_ptr<cFood> pcFood;
typedef boost::weak_ptr<cFood> wpcFood;

class cFood :
	public cItem
{
public:
	cFood(void);
public:
	virtual ~cFood(void);



public:
	virtual inline tstring IconFileName(){return _T("item\\アイテムテンプレート１.png");};
	virtual inline int IconFileIndexX(){return 0;};
	virtual inline int IconFileIndexY(){return 0;};

	virtual int GetmenuCaption(vector<tstring> &CaptionList);


	//コマンドを解決する;
	//virtual int FireCommand(IDirect3DDevice9 *pDev, tstring verb, vector<pcDroping> &ObjectList);


	virtual void AppreciationBeginOptimize(int difficulty){allAppreciate();};
	
	virtual double デフォルト効果量状態倍率_祝福();
	virtual int 修正値最大値(){return 4;};
	//--------------------------------------------------------
	//コマンド群
	//virtual int 食べる(IDirect3DDevice9 *pDev);
	//virtual int 食べ要請(pcCharacter pchara);

	//破壊される時true;
	virtual int 食べる_効果_通常(pcCharacter pchara);
	virtual int 食べる_効果_呪い(pcCharacter pchara);
	virtual int 食べる_効果_祝福(pcCharacter pchara);
	virtual int 食べる_メッセージ_通常(pcCharacter pchara);
	virtual int 食べる_メッセージ_呪い(pcCharacter pchara);
	virtual int 食べる_メッセージ_祝福(pcCharacter pchara);

	virtual int 食べるメッセージ定型(pcCharacter pchara);

	virtual void 属性追加()
	{
		cItem::属性追加();
		属性.insert(落ち物属性::食べ物);
		
	};


	
	virtual int 衝突(pcCharacter pchara);
	virtual int 衝突時効果(pcCharacter pchara);
	virtual int 衝突時_メッセージ(pcCharacter pchara);
	
	virtual void CutIn(タイミング timing, cValiableField& valiable);

	virtual double 火時性質変化率();
	virtual int 加熱後変化ID() = 0;
	virtual int 消費時幻想度加算量();
};
