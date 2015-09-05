#pragma once
#include "cItem.h"

#include "cMoney.h"

#include "Language.h"

//--------------------------------------
//値段
//--------------------------------------

#define PRICE_MONEYBAG_SP 999999
#define PRICE_MONEYBAG_SSS 300
#define PRICE_MONEYBAG_SS 200
#define PRICE_MONEYBAG_S 150
#define PRICE_MONEYBAG_A 100
#define PRICE_MONEYBAG_B 50
#define PRICE_MONEYBAG_C 30
#define PRICE_MONEYBAG_D 20
#define PRICE_MONEYBAG_E 10
#define PRICE_MONEYBAG_F 5
#define PRICE_MONEYBAG_G 3
#define PRICE_MONEYBAG_H 1

class cMoneyBag :
	public cItem
{
public:
	cMoneyBag(void);
public:
	virtual ~cMoneyBag(void);

	virtual inline tstring IconFileName(){return _T("item\\お金.png");};
	virtual inline int IconFileIndexX(){return 0;};
	virtual inline int IconFileIndexY(){return 1;};


	virtual void AppreciationBeginOptimize(int difficulty){allAppreciate();};

	//virtual cDiscreteProbability stateBeginDP(int difficulty);
	virtual cDiscreteProbability qualityBeginDP(int difficulty);


	virtual tstring FullNameBase();
	virtual tstring ShortNameBase()
		{return FullNameBase();};

	//消去処理の準備
	virtual void Release();

	virtual void Init(IDirect3DDevice9 *pDev,pcDroping self);


	virtual void DrawStateIconSub(IDirect3DDevice9 *pDev,int x,int y);


	virtual StyleString QualityCaption();


	//効果量修正値倍率
	virtual inline double 効果量修正値倍率(){
		return sg_pDungeonSystem->DataBase.DropImportData_Value(ID(), (tstring)_T("効果量修正値倍率"), 0.05);};

	//-----------------------------------
	//データパック_ここから
	//-----------------------------------

	//額（修正値上書き）
	virtual inline int amount(){return dpack.integer[1];};//1番


	//-----------------------------------
	//データパック_ここまで
	//-----------------------------------

	virtual bool stategoodRunoutprocess();

	virtual int 値段基礎値(){return amount();};
	virtual int 値段補正値(){return 値段基礎値();};
//----------------------------------
//装備系
public:
	virtual bool 装備可(){return true;};

	virtual bool 装備されている(){return (weak_pEquiper.lock() != NULL);};

	virtual pcCharacter 装備者(){return weak_pEquiper.lock();};

	wpcCharacter weak_pEquiper;
//--------------------------------------------------------
//実行群
protected:



public:

	virtual int GetmenuCaption(vector<tstring> &CaptionList);

	virtual bool 売却可(){return false;};

	//コマンドを解決する;
	//virtual int FireCommand(IDirect3DDevice9 *pDev, tstring verb, vector<pcDroping> &ObjectList);


	//--------------------------------------------------------
	//コマンド群
public:

	virtual void 属性追加()
	{
		属性.insert(落ち物属性::お金);
		
	};


	
	virtual int 衝突(pcCharacter pchara);
	virtual int 衝突時効果(pcCharacter pchara);
	virtual int 衝突時_メッセージ(pcCharacter pchara);
	
	virtual int ダメージ効果(pcCharacter pchara, pcLandform pland, int power);

	virtual multiset<攻撃属性::攻撃属性> 攻撃属性();

	//自動プロセス群
	virtual int 被まとめ要請(pcCharacter pchara);
	virtual int 被まとめる(pcCharacter pchara);

	virtual int まとめ要請(pcDroping pdrop);
	virtual int まとめる(pcDroping pdrop);

};
