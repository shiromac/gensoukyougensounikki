#pragma once
#include "cItem.h"



//--------------------------------------
//値段
//--------------------------------------

#define PRICE_BOX_SP 999999
#define PRICE_BOX_SSS 300
#define PRICE_BOX_SS 200
#define PRICE_BOX_S 150
#define PRICE_BOX_A 100
#define PRICE_BOX_B 50
#define PRICE_BOX_C 30
#define PRICE_BOX_D 20
#define PRICE_BOX_E 10
#define PRICE_BOX_F 5
#define PRICE_BOX_G 3
#define PRICE_BOX_H 1


class cBox;
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
typedef boost::shared_ptr<cBox> pcBox;
typedef boost::weak_ptr<cBox> wpcBox;

class cBox :
	public cItem
{
public:
	cBox(void);
public:
	virtual ~cBox(void);

	//スモールビュー
	virtual int Getsmollview(pcGameWindow& smollview);

	
	//virtual cDiscreteProbability stateBeginDP(int difficulty);
	//virtual cDiscreteProbability qualityBeginDP(int difficulty);



public:
	virtual inline tstring IconFileName(){return _T("item\\箱.png");};
	virtual inline int IconFileIndexX(){return 0;};
	virtual inline int IconFileIndexY(){return 0;};

	//--------------------------------------------
	//割り込みイベント
	//--------------------------------------------
	//virtual double CutInValue(int timing);
	//virtual int CutInEvent(int timing);
	virtual void CutIn(タイミング timing, cValiableField& valiable);



	//--------------------------------------------
	//割り込みイベント先
	//--------------------------------------------
	virtual int BreakOut();

	//--------------------------------------------
	//
	//--------------------------------------------
	
	virtual StyleString QualityCaption();

	virtual int GetmenuCaption(vector<tstring> &CaptionList);
	virtual StyleString GetmenuExplain(tstring &Caption);

	virtual StyleString ExtraHatString();

	//Fireできるときtrue;
	//virtual int isCanFireCommand( vector<pcDroping> &ObjectList, tstring verb);

	//オブジェクト(目的語)リストを得る;
	//案内を返す
	//virtual StyleString GetmenuFuncObjectsList(vector<pcDroping> &ObjectList, bool &MultiObj,int objectsize, tstring verb);


	//コマンドを解決する;
	//virtual int FireCommand(IDirect3DDevice9 *pDev, tstring verb, vector<pcDroping> &ObjectList);
	
	
	virtual double 弱衝撃時破壊率();
	virtual double 電気時帯静電気率();

	virtual int 修正値最大値(){return 10;};

	virtual int 消費時幻想度加算量();

public:
	//被箱入れ不可
	virtual bool 被箱入れ可(){return false;};
	virtual bool 箱入れ可(pcDroping pdrop){return true;};
	//-----------------------------------
	//データパック_ここから
	//-----------------------------------

	//容量（修正値上書き）
	//quality()
	virtual inline int& capacity(){return dpack.integer[1];};//1番
	//4~5まで種類
	virtual inline UByte& cannotopen(){return dpack_char_index(4,0);};//4,0番
	//-----------------------------------
	//データパック_ここまで
	//-----------------------------------
	//virtual int 値段補正値();
	virtual bool 内包アイテムは実体がない(){return false;};

	virtual inline int 残容量(){return capacity() - includedItem.size();};
	virtual inline bool 出し可(){return false;};

	virtual inline bool 内容物使用可(){return false;};
	virtual bool 内包ショートカット可(){return 内容物使用可();};
	virtual bool 内包落ち物対象可(){return 内容物使用可() && includedItem.size() > 0 && !cannotopen();};
	virtual int 内包落ち物サイズ(){return capacity();}
	virtual vector<pcDroping> 内包落ち物対象リスト(){return includedItem;};

	virtual inline bool 開け用(){return false;};

	//効果量(係数)
	virtual inline double 効果量係数(){return 1;};//クオリティーにかかわらず固定

	//---------------------------------
	//識別
	virtual void allAppreciate(){
		cDroping::allAppreciate();
		大局Appreciate();
	};

	virtual int 修正値識別済み(){return true;};

	virtual StyleString 識別未識別混合道具名();
	
	//アイテムのサイズ
	virtual int size(){return 1 + includedItem.size();};
public:
	//--------------------------------------------------------
	//実行群
	



private:
	/*
	//強制的に入れる
	virtual int putInto(pcDroping pdrop);


	//強制的に出す
	virtual int putOut(pcDroping pdrop);

	//強制的に中身を全部ばら撒く
	virtual int AlldropOut();
	*/

public:
	//--------------------------------------------------------
	//コマンド群
	//virtual int 入れる(IDirect3DDevice9 *pDev, pcDroping pdrop);
	//virtual int 入れ要請(pcDroping pdrop);

	//破壊される時true;
	virtual int 入れる_効果_通常(pcDroping pdrop);
	virtual int 入れる_効果_呪い(pcDroping pdrop);
	virtual int 入れる_効果_祝福(pcDroping pdrop);
	virtual int 入れる_メッセージ_通常(pcDroping pdrop);
	virtual int 入れる_メッセージ_呪い(pcDroping pdrop);
	virtual int 入れる_メッセージ_祝福(pcDroping pdrop);

	virtual int 入れるメッセージ定型(pcDroping pdrop);


	//virtual int 見る(IDirect3DDevice9 *pDev);
	//virtual int 見る要請();

	//破壊される時true;
	virtual int 見る_効果_通常();
	virtual int 見る_効果_呪い();
	virtual int 見る_効果_祝福();
	virtual int 見る_メッセージ_通常();
	virtual int 見る_メッセージ_呪い();
	virtual int 見る_メッセージ_祝福();

	virtual int 見るメッセージ定型();


	//virtual int 出す(IDirect3DDevice9 *pDev, pcDroping pdrop);
	//virtual int 出し要請(pcDroping pdrop, pcCharacter receiver);

	//破壊される時true;
	virtual int 出す_効果_通常(pcDroping pdrop, pcCharacter receiver);
	virtual int 出す_効果_呪い(pcDroping pdrop, pcCharacter receiver);
	virtual int 出す_効果_祝福(pcDroping pdrop, pcCharacter receiver);
	virtual int 出す_メッセージ_通常(pcDroping pdrop, pcCharacter receiver);
	virtual int 出す_メッセージ_呪い(pcDroping pdrop, pcCharacter receiver);
	virtual int 出す_メッセージ_祝福(pcDroping pdrop, pcCharacter receiver);

	virtual int 出すメッセージ定型(pcDroping pdrop, pcCharacter receiver);


	//virtual int 入れ替え(IDirect3DDevice9 *pDev, pcDroping pdrop, pcDroping pdrop2);
	//virtual int 入れ替え要請(pcDroping pdrop, pcDroping pdrop2);

	//破壊される時true;
	virtual int 入れ替え_効果_通常(pcDroping pdrop, pcDroping pdrop2);
	virtual int 入れ替え_効果_呪い(pcDroping pdrop, pcDroping pdrop2);
	virtual int 入れ替え_効果_祝福(pcDroping pdrop, pcDroping pdrop2);
	virtual int 入れ替え_メッセージ_通常(pcDroping pdrop, pcDroping pdrop2);
	virtual int 入れ替え_メッセージ_呪い(pcDroping pdrop, pcDroping pdrop2);
	virtual int 入れ替え_メッセージ_祝福(pcDroping pdrop, pcDroping pdrop2);

	virtual int 入れ替えメッセージ定型(pcDroping pdrop, pcDroping pdrop2);

	//破壊される時true;
	virtual int 開ける_効果_通常();
	virtual int 開ける_効果_呪い();
	virtual int 開ける_効果_祝福();
	virtual int 開ける_メッセージ_通常();
	virtual int 開ける_メッセージ_呪い();
	virtual int 開ける_メッセージ_祝福();

	virtual int 開けるメッセージ定型();



	virtual void 属性追加()
	{
		cItem::属性追加();
		属性.insert(落ち物属性::入れ物);
		属性.insert(落ち物属性::割れ物);
	};


	
	virtual int 衝突(pcCharacter pchara);
	virtual int 衝突時効果(pcCharacter pchara);
	virtual int 衝突時_メッセージ(pcCharacter pchara);

	
	virtual int 異常状態解除();
	
};
