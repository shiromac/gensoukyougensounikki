#pragma once
#include "cItem.h"



//--------------------------------------
//値段
//--------------------------------------

#define PRICE_BULLET_SP 999999
#define PRICE_BULLET_SSS 300
#define PRICE_BULLET_SS 200
#define PRICE_BULLET_S 150
#define PRICE_BULLET_A 100
#define PRICE_BULLET_B 50
#define PRICE_BULLET_C 30
#define PRICE_BULLET_D 20
#define PRICE_BULLET_E 10
#define PRICE_BULLET_F 5
#define PRICE_BULLET_G 3
#define PRICE_BULLET_H 1


class cBullet;
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
typedef boost::shared_ptr<cBullet> pcBullet;
typedef boost::weak_ptr<cBullet> wpcBullet;

class cBullet :
	public cItem
{
public:
	cBullet(void);
public:
	virtual ~cBullet(void);

	//消去処理の準備
	virtual void Release();

	virtual void Init(IDirect3DDevice9 *pDev,pcDroping self);


	virtual void DrawStateIconSub(IDirect3DDevice9 *pDev,int x,int y);

	virtual StyleString 識別未識別混合道具名();
	//---------------------------------
	//識別
	virtual void allAppreciate(){
		cDroping::allAppreciate();
		大局Appreciate();
	};

	virtual void longExplanationExtraBase(std::vector<StyleString> &explanations);
	virtual void CutIn(タイミング timing, cValiableField& valiable);

	//virtual cDiscreteProbability stateBeginDP(int difficulty);
	//virtual cDiscreteProbability qualityBeginDP(int difficulty);

	virtual void AppreciationBeginOptimize(int difficulty)
	{
		cItem::AppreciationBeginOptimize(difficulty);
		Appreciate_State();
		Appreciate_Quality();
	};

	virtual int 値段補正値();


	virtual void aspectedAttackDraw(IDirect3DDevice9 *pDev);
	virtual inline int AttackIconFileSize(){return 256;};
	virtual inline tstring AttackIconFileName(){return _T("item\\effectdrop.png");};
	virtual inline int AttackIconFileIndexX(){return 0;};
	virtual int GetAttackIcon(cDrawingObject& DO);
	IDirect3DTexture9* m_pTexture_AttackIcon;

	virtual StyleString QualityCaption();

	virtual double 撃ち時神力切れ確率();
	virtual double デフォルト撃ち時神力切れ確率();

	//効果量修正値倍率
	virtual double 効果量修正値倍率();
	virtual double デフォルト効果量修正値倍率();
	virtual void 効果量推定テキスト(int index, StyleString& output);
	virtual double 架空攻撃力_定数();
	virtual double 架空攻撃力_フロア係数();
	virtual double 架空攻撃力_ダメージキャップ();

	virtual double デフォルト効果量状態倍率_祝福();
	//-----------------------------------
	//データパック_ここから
	//-----------------------------------

	//数量（修正値上書き）
	//quality()+1
	virtual inline int quantity(){return dpack.integer[1]+1;};//1番
	
	//4~5まで種類
	virtual inline UByte& bombSet(){return dpack_char_index(4,0);};//4,1番
	virtual inline UByte& bombed(){return dpack_char_index(4,1);};//4,1番

	//-----------------------------------
	//データパック_ここまで
	//-----------------------------------

	virtual bool stategoodRunoutprocess();


//----------------------------------
//装備系
public:
	virtual bool 装備可(){return true;};

	virtual bool 装備されている(){return (weak_pEquiper.lock() != NULL);};

	virtual pcCharacter 装備者(){return weak_pEquiper.lock();};

	wpcCharacter weak_pEquiper;
//--------------------------------------------------------
//実行群
public:
	//強制的に装備
	virtual int equip(pcCharacter pchara);
	//強制的に装備解除
	virtual int unequip();


public:
	virtual inline tstring IconFileName(){return _T("item\\アイテムテンプレート１.png");};
	virtual inline int IconFileIndexX(){return 0 + (撃ち返し爆弾() && bombSet());};
	virtual inline int IconFileIndexY(){return 3 + -2*(撃ち返し爆弾() && bombSet());};

	virtual int GetmenuCaption(vector<tstring> &CaptionList);



	//コマンドを解決する;
	//virtual int FireCommand(IDirect3DDevice9 *pDev, tstring verb, vector<pcDroping> &ObjectList);
	
	//--------------------------------------------------------
	//コマンド群
public:
	//virtual int 装備(IDirect3DDevice9 *pDev);
	//virtual int はずす(IDirect3DDevice9 *pDev);
	//virtual int 装備要請(pcCharacter pchara);
	//virtual int 装備はずし要請(pcCharacter pchara);

	//破壊される時true;
	virtual int 装備_効果_通常(pcCharacter pchara);
	virtual int 装備_効果_呪い(pcCharacter pchara);
	virtual int 装備_効果_祝福(pcCharacter pchara);
	virtual int 装備_メッセージ_通常(pcCharacter pchara);
	virtual int 装備_メッセージ_呪い(pcCharacter pchara);
	virtual int 装備_メッセージ_祝福(pcCharacter pchara);

	virtual int 装備メッセージ定型(pcCharacter pchara);

	virtual int はずす_効果_通常(pcCharacter pchara);
	virtual int はずす_効果_呪い(pcCharacter pchara);
	virtual int はずす_効果_祝福(pcCharacter pchara);
	virtual int はずす_メッセージ_通常(pcCharacter pchara);
	virtual int はずす_メッセージ_呪い(pcCharacter pchara);
	virtual int はずす_メッセージ_祝福(pcCharacter pchara);

	virtual int はずすメッセージ定型(pcCharacter pchara);


	
	
	//virtual int 撃つ(IDirect3DDevice9 *pDev);

	//virtual int 撃ち要請(pcCharacter pchara);

	//破壊される時true;
	virtual int 撃つ_効果_通常(pcCharacter pchara);
	virtual int 撃つ_効果_呪い(pcCharacter pchara);
	virtual int 撃つ_効果_祝福(pcCharacter pchara);
	virtual int 撃つ_メッセージ_通常(pcCharacter pchara);
	virtual int 撃つ_メッセージ_呪い(pcCharacter pchara);
	virtual int 撃つ_メッセージ_祝福(pcCharacter pchara);

	virtual int 撃つメッセージ定型(pcCharacter pchara);

	virtual int 本数消費();

	//攻撃力
	virtual inline int 加算攻撃力();
	virtual inline double 乗算攻撃力();
	virtual inline int 射程距離();
	virtual inline int ひま投げ();
	virtual multiset<攻撃属性::攻撃属性> 攻撃属性();
	virtual bool 命中力必中(){return 攻撃属性().count(攻撃属性::必中);};
	virtual int 攻撃属性_定数ダメージ();

	//攻撃属性とは違う
	virtual void 属性追加()
	{
		cItem::属性追加();
		属性.insert(落ち物属性::撃ち物);
		
	};
	virtual bool 撃ち返し爆弾();
	virtual bool 爆弾爆発();

	virtual bool 被拾得可(){return !(撃ち返し爆弾() && bombSet());};

	virtual int 上を通過(pcDroping pdrop);
	virtual int 上を通過(pcCharacter pchara);
	virtual int 上に落ちる(pcDroping pdrop);
	virtual int 上に落ちる(pcCharacter pchara);
	virtual int 乗る();


	virtual int 威力計算();
	virtual int 衝突(pcCharacter pchara);
	virtual int 衝突時効果(pcCharacter pchara);
	virtual int 衝突時効果land(pcLandform pland);
	virtual int 衝突時_メッセージ(pcCharacter pchara);
	
	virtual int ダメージ効果(pcCharacter pchara, pcLandform pland, int power);

	//自動プロセス群
	virtual int まとめ要請(pcDroping pdrop);
	virtual int まとめる(pcDroping pdrop);

	virtual int まとめ最大数();
	virtual void 能力効果量推定テキスト(int index, StyleString& output);
	

	virtual double 能力スロット();
	virtual double 能力効果量倍率();

	//エフェクト
	virtual int 溜めエフェクト(pcCharacter pchara);
};
