#pragma once
#include "cItem.h"



//--------------------------------------
//値段
//--------------------------------------

#define PRICE_EQUIPMENT_SP 999999
#define PRICE_EQUIPMENT_SSS 3000
#define PRICE_EQUIPMENT_SS 2000
#define PRICE_EQUIPMENT_S 1500
#define PRICE_EQUIPMENT_A 1000
#define PRICE_EQUIPMENT_B 800
#define PRICE_EQUIPMENT_C 600
#define PRICE_EQUIPMENT_D 400
#define PRICE_EQUIPMENT_E 200
#define PRICE_EQUIPMENT_F 100
#define PRICE_EQUIPMENT_G 50
#define PRICE_EQUIPMENT_H 30


class cEquipment;
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
typedef boost::shared_ptr<cEquipment> pcEquipment;
typedef boost::weak_ptr<cEquipment> wpcEquipment;

class cEquipment :
	public cItem
{
public:
	cEquipment(void);
public:
	virtual ~cEquipment(void);

	//消去処理の準備
	virtual void Release();


	//データパックを初期最適化する。アイテムを初期設置するときに一度だけ使う。
	virtual void DataBeginOptimize(int difficulty);
	//virtual cDiscreteProbability stateBeginDP(int difficulty);
	//virtual cDiscreteProbability qualityBeginDP(int difficulty);

	virtual StyleString FullName();

	virtual StyleString 能力タイトルCaption();


	virtual StyleString 識別未識別混合道具名();
	virtual StyleString MemoCaption(){return _T("");};

	virtual double 効果時神力切れ確率();
	virtual double デフォルト効果時神力切れ確率();



	//-----------------------------------
	//データパック_ここから
	//-----------------------------------
	virtual inline int & proficiency_count(){return dpack.integer[4];};//4番
	virtual inline UByte & adjective(){return dpack_char_index(5,0);};//5.0番//形容装備
	virtual inline UByte & crash(){return dpack_char_index(5,1);};//5.1番//クラッシュ
	virtual inline UByte & overspec(){return dpack_char_index(5,2);};//5.2番//改造

	//4~5まで種類
	//6~8まで具体種
	//9~11までメモ
	//-----------------------------------
	//データパック_ここまで
	//-----------------------------------
	virtual inline int 熟練度();//4番
	virtual inline int 熟練度計算(int pfc);
	virtual inline void 熟練度カウント加算(int pfc);
	virtual inline void 熟練度リセット();
	virtual inline double 熟練度成長倍率(const double selfLV, const double oppsLV);
	virtual inline int 熟練度最大();
	virtual inline int 合成時減少済みproficiency();
	virtual StyleString 熟練度キャプション();
public:
	virtual inline tstring IconFileName();
	virtual inline int IconFileIndexX();
	virtual inline int IconFileIndexY();

	virtual void DrawStateIconSub(IDirect3DDevice9 *pDev,int x,int y);

	virtual int GetmenuCaption(vector<tstring> &CaptionList);

	//ショートカットに入れられるcaptionのリストを得る;
	virtual int EnableShortCutCaptions(vector<tstring> &CaptionList);

	//captionのexplainを得る;
	virtual StyleString GetmenuExplain(tstring &Caption);

	//コマンドを解決する;
	//virtual int FireCommand(IDirect3DDevice9 *pDev, tstring verb, vector<pcDroping> &ObjectList);

public:
	//被箱入れ
	virtual void 被箱入れ準備();
	virtual bool 被箱入れ可()
	{
		return !装備されている_攻撃用() && !装備されている_防御用();
	};
	virtual bool 被放棄可()
	{
		return !((装備されている_攻撃用() || 装備されている_防御用()) && (state() == STATE_CURSE));
	}

	//カットイン
	//virtual int CutInEvent(int timing);
	virtual void CutIn(タイミング timing, cValiableField& valiable);



	virtual void 攻撃エフェクト(pcCharacter pchara);
	virtual void 防御エフェクト(pcCharacter pchara);


	virtual bool 両手装備();


	virtual bool 装備可_攻撃用(){return true;};
	virtual bool 装備可_防御用(){return true;};


	virtual pcCharacter 装備者_攻撃用();
	virtual pcCharacter 装備者_防御用();
	virtual pcCharacter 装備者();

	wpcCharacter weak_pEquiper_Attack;
	wpcCharacter weak_pEquiper_Defence;

	virtual bool stategoodRunoutprocess();
	virtual bool overQualityprocess();
	virtual double 過修正累進故障率();

	virtual void 属性追加()
	{
		cItem::属性追加();
		属性.insert(落ち物属性::装備品);
		
	};



	//--------------------------------------------------------
	//実行群
public:
	#define EQUIP_PART_ATTACK (0)
	#define EQUIP_PART_DEFENSE (1)
	#define EQUIP_PART_MAXSIZE (2)
	//強制的に装備
	virtual int equip(pcCharacter pchara, int part);
	//強制的に装備解除
	virtual int unequip();
	//--------------------------------------------------------
	//コマンド群
public:
	//virtual int 装備(IDirect3DDevice9 *pDev, int part);
	//virtual int 装備選択(IDirect3DDevice9 *pDev);
	//virtual int はずす(IDirect3DDevice9 *pDev);
	//virtual int 装備要請(pcCharacter pchara, int part);
	//virtual int 装備はずし要請(pcCharacter pchara);

	//破壊される時true;
	virtual int 装備_効果_通常(pcCharacter pchara, int part);
	virtual int 装備_効果_呪い(pcCharacter pchara, int part);
	virtual int 装備_効果_祝福(pcCharacter pchara, int part);
	virtual int 装備_メッセージ_通常(pcCharacter pchara, int part);
	virtual int 装備_メッセージ_呪い(pcCharacter pchara, int part);
	virtual int 装備_メッセージ_祝福(pcCharacter pchara, int part);

	virtual int 装備メッセージ定型(pcCharacter pchara, int part);

	virtual int はずす_効果_通常(pcCharacter pchara);
	virtual int はずす_効果_呪い(pcCharacter pchara);
	virtual int はずす_効果_祝福(pcCharacter pchara);
	virtual int はずす_メッセージ_通常(pcCharacter pchara);
	virtual int はずす_メッセージ_呪い(pcCharacter pchara);
	virtual int はずす_メッセージ_祝福(pcCharacter pchara);

	virtual int はずすメッセージ定型(pcCharacter pchara);



	
	virtual int 衝突(pcCharacter pchara);
	virtual int 衝突時効果(pcCharacter pchara);
	virtual int 衝突時_メッセージ(pcCharacter pchara);

	//--------------------------------------------------------
	//説明系群
	//virtual StyleString longExplanation();
	virtual StyleString shortExplanation();
	virtual void longExplanationExtraBase(std::vector<StyleString> &explanations);
	//スモールビュー
	virtual int Getsmollview(pcGameWindow& smollview);

		
	//virtual void AbilityExplanationBase(std::vector<StyleString> &explanations);
	//virtual StyleString AbilityName();

	//--------------------------------------------------------
	//形容システム

	virtual tstring 形容詞();


	//--------------------------------------------------------
	//パラメータ群
	virtual int 装備難度();
	virtual int 使用難度(){return 装備難度();};

	virtual int 値段補正値();
	virtual int 単体売値(){
		return 値段補正値()*値段割引()/10;
	}
	virtual int 厄ポイント値段補正値();
	
	//熟練度
	virtual double 熟練度定数(double 装備力);
	virtual double 熟練度攻撃防御補正率();
	virtual double デフォルト熟練度攻撃防御補正率();

	virtual bool 改造可能();
	virtual void 改造する();
	virtual int 改造費();

	virtual double 初期初期残りスロット();


	virtual int 修正値限界値();

	//過修正のとき超過分返す
	virtual int 過修正である();

	virtual StyleString 修正値係数評価(double value);

	virtual bool 通常攻撃_壁斜めすり抜け();
	virtual bool 通常攻撃_壁すり抜け();


	virtual double 効果量修正値倍率(){return 0;};
	//virtual double 効果量状態倍率();
	virtual double デフォルト効果量状態倍率_祝福();

	static double 修正値重み計算式(double base);

	virtual double 武器力基礎値();
	virtual double 武器力修正値重み();
	virtual double 攻撃修正値係数();
	virtual double 命中率補正();

	virtual double 防具力基礎値();
	virtual double 防具力修正値重み();
	virtual double 防御修正値係数();
	virtual double 回避率補正();

	virtual double 武器力最終値();
	virtual double 防具力最終値();


};
