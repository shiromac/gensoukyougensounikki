#pragma once
#include "cItem.h"

#include "MobAbilityIdiom.h"


//--------------------------------------
//値段
//--------------------------------------

#define PRICE_SPELL_SP 999999
#define PRICE_SPELL_SSS 300
#define PRICE_SPELL_SS 200
#define PRICE_SPELL_S 150
#define PRICE_SPELL_A 100
#define PRICE_SPELL_B 50
#define PRICE_SPELL_C 30
#define PRICE_SPELL_D 20
#define PRICE_SPELL_E 10
#define PRICE_SPELL_F 5
#define PRICE_SPELL_G 3
#define PRICE_SPELL_H 1



//--------------------------------------
//装備箇所
//--------------------------------------
#define SPELL_EQUIP_PART_NULL -1


class cSpell;
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
typedef boost::shared_ptr<cSpell> pcSpell;
typedef boost::weak_ptr<cSpell> wpcSpell;

class cSpell :
	public cItem
{
public:
	cSpell(void);
public:
	virtual ~cSpell(void);

	//消去処理の準備
	virtual void Release();

	virtual void AppreciationBeginOptimize(int difficulty);
	//virtual cDiscreteProbability stateBeginDP(int difficulty);
	//virtual cDiscreteProbability qualityBeginDP(int difficulty);

	virtual bool 被放棄可(){return !装備されている();};

	virtual StyleString ListFullName();

	virtual tstring SpellTitle();
	virtual tstring SpellName();

	virtual tstring SpellTitleFullName(){return SpellTitle() +_T("「")+ SpellName()+ _T("」");};

	virtual void longExplanationExtraBase(std::vector<StyleString> &explanations);

public:
	virtual inline tstring IconFileName(){return _T("item\\本.png");};
	virtual inline int IconFileIndexX(){return 1;};
	virtual inline int IconFileIndexY(){return 0;};
	virtual inline int IconForIconFileIndexX(){return IconFileIndexX() + 装備されている();};
	virtual inline int IconForIconFileIndexY(){return IconFileIndexY();};

	virtual void DrawStateIconSub(IDirect3DDevice9 *pDev,int x,int y);


	virtual int GetmenuCaption(vector<tstring> &CaptionList);

	//captionのexplainを得る;
	virtual StyleString GetmenuExplain(tstring &Caption);

	//virtual int isCanFireCommand(vector<pcDroping> &ObjectList, tstring verb);
	virtual int オブジェクト宣言可能(int objectsize){return true;};

	virtual int オブジェクトスキル可能(int objectsize){return true;};


	//StyleString GetmenuFuncObjectsList(vector<pcDroping> &ObjectList, bool &MultiObj,int objectsize, tstring verb);
	virtual void 宣言オブジェクト候補(vector<vector<pcDroping>> &ObjectListlist, vector<StyleString>& listExplain, bool &MultiObj, int objectsize, tstring verb);
	
	virtual void スキルオブジェクト候補(vector<vector<pcDroping>> &ObjectListlist, vector<StyleString>& listExplain, bool &MultiObj, int objectsize, tstring verb);
	
	//オブジェクトが取れなかったときfalseを返す。
	virtual int 宣言オブジェクト自動選択(vector<pcDroping>& ObjectList);


	//コマンドを解決する;
	//virtual int FireCommand(IDirect3DDevice9 *pDev, tstring verb, vector<pcDroping> &ObjectList);


public:
	//被箱入れ
	virtual bool 被箱入れ可(){return !装備されている();};



	//イメージカラー
	virtual unsigned long imagecolar1_real();
	virtual unsigned long imagecolar2_real();
	
	unsigned long imagecolar1();
	unsigned long imagecolar2();


	//カットイン
	//virtual int CutInEvent(int timing);
	virtual void CutIn(タイミング timing, cValiableField& valiable);

	
	//-----------------------------------
	//データパック_ここから
	//-----------------------------------
	//4~5まで種類
protected://耐久度
	virtual inline UByte & damage_criticalFlag(){return dpack_char_index(4,0);};//4,0番
	virtual inline int & damage_count(){return dpack.integer[5];};//5番



	//-----------------------------------
	//データパック_ここまで
	//-----------------------------------
public:

	virtual bool 破損している(){return damage_criticalFlag();};

	virtual int 通常時最悪耐久回数();
	virtual int 亀裂時最悪耐久回数();
	virtual int デフォルト通常時最悪耐久回数();
	virtual int デフォルト亀裂時最悪耐久回数();

	virtual double 通常時破損率();
	virtual double 亀裂時破損率();
	virtual double デフォルト通常時破損率();
	virtual double デフォルト亀裂時破損率();

	virtual double 宣言時神力切れ確率();
	virtual double デフォルト宣言時神力切れ確率();

	virtual double 効果時腕輪ダメージ();

	virtual bool 破損する();
	virtual bool 破損回復(int message);

	virtual int 修正値最大値(){return 4;};
	
	//効果量(係数)
	virtual double 効果量状態倍率(){return 1.0;};

	//装備
	virtual bool 装備可(){return true;};

	virtual bool 装備されている(){return (weak_pEquiper.lock() != NULL);};

	virtual pcCharacter 装備者(){return weak_pEquiper.lock();};
	
	int 装備箇所;
	
	wpcCharacter weak_pEquiper;


	virtual bool stategoodRunoutprocess();
	virtual bool Breakcrashprocess(int power = 1);

	virtual bool LastSpell();
	virtual void LastSpellAfterProcess(pcCharacter equipedChara){return;};//sub class用


	//---------------------------------
	//識別
	virtual void allAppreciate(){
		cDroping::allAppreciate();
		大局Appreciate();
	};



	virtual StyleString 識別未識別混合道具名();

	tstring 装備コマンド名称(int part);


	//--------------------------------------------------------
	//実行群
public:
	//強制的に装備
	virtual int equip(pcCharacter pchara, int part);
	//強制的に装備解除
	virtual int unequip();

public:

	//--------------------------------------------------------
	//コマンド群
	
	//virtual int 装備(IDirect3DDevice9 *pDev, int part);
	//virtual int 装備選択(IDirect3DDevice9 *pDev);
	//virtual int 装備要請(pcCharacter pchara, int part);


	virtual int 装備_効果_通常(pcCharacter pchara, int part);
	virtual int 装備_効果_呪い(pcCharacter pchara, int part);
	virtual int 装備_効果_祝福(pcCharacter pchara, int part);
	virtual int 装備_メッセージ_通常(pcCharacter pchara, int part);
	virtual int 装備_メッセージ_呪い(pcCharacter pchara, int part);
	virtual int 装備_メッセージ_祝福(pcCharacter pchara, int part);

	virtual int 装備メッセージ定型(pcCharacter pchara, int part);


	//virtual int はずす(IDirect3DDevice9 *pDev);
	//virtual int 装備はずし要請(pcCharacter pchara);

	
	virtual int はずす_効果_通常(pcCharacter pchara);
	virtual int はずす_効果_呪い(pcCharacter pchara);
	virtual int はずす_効果_祝福(pcCharacter pchara);
	virtual int はずす_メッセージ_通常(pcCharacter pchara);
	virtual int はずす_メッセージ_呪い(pcCharacter pchara);
	virtual int はずす_メッセージ_祝福(pcCharacter pchara);

	virtual int はずすメッセージ定型(pcCharacter pchara);


	virtual int 破壊_効果_通常(pcCharacter pchara);
	virtual int 破壊_効果_呪い(pcCharacter pchara);
	virtual int 破壊_効果_祝福(pcCharacter pchara);
	virtual int 破壊_メッセージ_通常(pcCharacter pchara);
	virtual int 破壊_メッセージ_呪い(pcCharacter pchara);
	virtual int 破壊_メッセージ_祝福(pcCharacter pchara);

	virtual int 破壊メッセージ定型(pcCharacter pchara);

	
	//virtual int 宣言(IDirect3DDevice9 *pDev ,vector<pcDroping> &ObjectList);
	//virtual int 宣言要請(pcCharacter pchara ,vector<pcDroping> &ObjectList);

	//破壊される時true;
	virtual int 宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);
	virtual int 宣言_効果_呪い(pcCharacter pchara ,vector<pcDroping> &ObjectList);
	virtual int 宣言_効果_祝福(pcCharacter pchara ,vector<pcDroping> &ObjectList);
	virtual int 宣言_メッセージ_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList);
	virtual int 宣言_メッセージ_呪い(pcCharacter pchara ,vector<pcDroping> &ObjectList);
	virtual int 宣言_メッセージ_祝福(pcCharacter pchara ,vector<pcDroping> &ObjectList);

	virtual int 宣言メッセージ定型(pcCharacter pchara ,vector<pcDroping> &ObjectList);

	virtual void 属性追加()
	{
		cItem::属性追加();
		属性.insert(落ち物属性::スペカ);
		
	};

	virtual int スキルあり(pcCharacter pchara);
	virtual int スキル使用(pcCharacter pchara, vector<pcDroping> &ObjectList);
	virtual int スキル効果(pcCharacter pchara, vector<pcDroping> &ObjectList);
	virtual int スキルコスト満腹度(pcCharacter pchara);
	virtual int デフォルトスキルコスト満腹度();
	//NULLCHARA注意
	virtual int スキルスペルダメージ(pcCharacter pchara);
	virtual int デフォルトスキルスペルダメージ();
	virtual int スキル_メッセージ(pcCharacter pchara ,vector<pcDroping> &ObjectList);
	
	
	virtual StyleString スキル説明(pcCharacter pchara);
	
	
	virtual int 衝突(pcCharacter pchara);
	virtual int 衝突時効果(pcCharacter pchara);
	virtual int 衝突時_メッセージ(pcCharacter pchara);
	

	virtual int スペル定型エフェクト_宣言();
	virtual int スペル定型エフェクト_装備(pcCharacter pchara);

private:

	bool CutInMobAbilityIdiom(MobAbilityIdiom::CutInFunction function, pcCharacter pchara, タイミング timing, cValiableField& valiable, bool enabledSpellDamage, bool enabledActiveIdentify, bool guardDuplicateAbility);
};

