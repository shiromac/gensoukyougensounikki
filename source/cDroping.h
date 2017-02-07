#pragma once

#include "utility/SpriteText.h"
#include "cgameobject.h"
#include "utility\cColor.h"
#include "utility\valiableField\cValiableField.h"
#include "timingEnum.h"

#include "controls_ph.h"
#include <tstring_ph.h>

#include <vector>
#include <set>
using namespace std;

#define DEF_DROP_DATAPACK_NUM 12
#define DEF_DROP_DATAPACK_UCHAR_NUM 48

//-----------------------------------------
//カラー定義
//-----------------------------------------
#define ITEM_NORMAL_COLOR (0xFFEEEEEE)

#define ITEM_CAPTION_COLOR (0xFFBBFFFF)
#define ITEM_CURSE_COLOR (0xFFAA77FF)
#define ITEM_GOOD_COLOR (0xFFCCFF00)

#define ITEM_NONAPP_COLOR (0xFFFFF733)
#define ITEM_HALFAPP_COLOR (0xFF88EF77)
#define ITEM_QUARAPP_COLOR (0xFFBBFFDD)

#define ITEM_NOTICE_COLOR (0xFFFFCC30)
#define ITEM_NOTICE_COLOR2 (0xFF44DDFF)
#define ITEM_NOTICE_COLOR3 (0xFFFF55DD)
#define ITEM_NOTICE_COLOR4 (0xFF66FF44)

#define ITEM_MITAMA_COLOR (0xFFFFCC88)
//-----------------------------------------
//レア度定義
//-----------------------------------------
#define RARITY_SP (0.0)//イベント専用
#define RARITY_SSS (1.0)
#define RARITY_SS (2.0)
#define RARITY_S (4.0)
#define RARITY_A (8.0)
#define RARITY_B (16.0)
#define RARITY_C (32.0)
#define RARITY_D (64.0)
#define RARITY_E (100.0)//デフォ
#define RARITY_F (200.0)
#define RARITY_G (300.0)
#define RARITY_H (400.0)

#include <boost/pointer_cast.hpp>

class cDroping;
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
typedef boost::shared_ptr<cDroping> pcDroping;
typedef boost::weak_ptr<cDroping> wpcDroping;

typedef boost::shared_ptr<vector<pcDroping>> pvdroping;


#include "cCharacter.h"
#include "cLandform.h"
#include "cGameWindow.h"

#include "cAttackinformation.h"


//new関数定義
#define DEF_FUNC_new_cDropingClass(z, n, data) \
	pcDroping def_GetInstanceOf##data##_ID_##n##()\
{\
	pcDroping pdrop = pcDroping(new data##_ID_##n##);\
	return pdrop;\
};


namespace 落ち物属性
{

	enum 落ち物属性
	{
		無効エラー,
		//種別
		物体,
		アイテム,
		飲み物,
		食べ物,
		装備品,
		撃ち物,
		読み物,
		スペカ,
		入れ物,
		お金,
		階段,
		火柱,

		罠,

		//属性
		割れ物,//割れる
		//引火性,//火がつく
		不燃性,//火に強い
		耐冷凍,//
		耐静電気,//
		耐濡れ,//
		耐火,//

		酒,
		薬,
		火消し,

		剣,

		DROPATRI_NUM,
	};
};


class cDroping :
	public cGameObject
{
public:
	cDroping(void);
public:
	virtual ~cDroping(void);

	virtual void Init(IDirect3DDevice9 *pDev,pcDroping self);

	virtual void settingInit();
protected:
	wpcDroping self_weakpointer;
	SharedValiable_likeDouble<int> extraMaterialColor_alpha;
	SharedValiable_likeDouble<int> extraMaterialColor_red;
	SharedValiable_likeDouble<int> extraMaterialColor_green;
	SharedValiable_likeDouble<int> extraMaterialColor_blue;
public:
	pcDroping me(){return self_weakpointer.lock();};

	cColor m_color;


	//データパックを初期最適化する。アイテムを初期設置するときに一度だけ使う。
	virtual void DataBeginOptimize(int difficulty){};




	//消去処理の準備
	virtual void Release(){};


	virtual void Draw(IDirect3DDevice9 *pDev);
	virtual void DrawIcon(IDirect3DDevice9 *pDev,int x,int y);
	virtual void DrawStateIcon(IDirect3DDevice9 *pDev,int x,int y);
	virtual void DrawStateIconSub(IDirect3DDevice9 *pDev,int x,int y){};

	virtual const cColor extraMaterialColor();
	virtual void setExtraMaterialColor(int alpha, int red, int green, int blue);

	//-----------------------------------------
	//特殊ドロー系

	//方向つき攻撃ドロー（主に矢と魔法）
	virtual void aspectedAttackDraw(IDirect3DDevice9 *pDev){Draw(pDev);};




	virtual int GetIcon(cDrawingObject& DO);
	virtual int GetIconForIcon(cDrawingObject& DO);


	virtual int RerenderText(SpriteText & sprite,int MaxLetterLength);


	//表示を実値と同期させる
	virtual void visibleReset();
	virtual bool is_onland_DrawFlag(){return (Holder() == NULL && Parent() == NULL && !broken && 落ちてる());};
	//---------------------------------
	//識別
protected:
	virtual void 大局Appreciate();
	virtual void 大局untiappreciate();

public:
	virtual void allAppreciate();
	virtual void Appreciate_State(){appreciation_state() = true;};
	virtual void Appreciate_Quality(){appreciation_quality() = true;};
	virtual void Appreciate_Name(){大局Appreciate();};

	virtual void untiappreciate();

	virtual int 完全識別済み();


	virtual int 個別識別済み(){return appreciation_indivi();};
	virtual int 大局識別済み();

	virtual int 状態値識別済み();
	virtual int 修正値識別済み();
	virtual int 名称識別済み(){return appreciation_indivi() || 大局識別済み();};

	virtual int デフォルト大局識別済み();




	//マップ上に入った
	virtual int OnMapping();

	
	//見つけたか
	virtual bool isDiscover();
	//見つけた
	virtual int discover();
	//見失う
	virtual int disappear();

protected:
	int pri_discover;

public:



	virtual bool isShouldDraw();


	virtual StyleString FullName(){return _T("droping");};
	virtual StyleString FullNameSub(){return _T("droping");};
	virtual StyleString ShortName(){return _T("droping");};
	virtual StyleString ShortNameSub(){return _T("droping");};
	virtual tstring usefulName();

	virtual tstring NoDataString();
	
	virtual tstring FullNameBase();
	virtual tstring ShortNameBase();

	virtual StyleString shortExplanation();
	virtual StyleString longExplanation();
	virtual void longExplanationExtra(std::vector<StyleString> &explanations){longExplanationExtraBase(explanations);};

	virtual StyleString shortExplanationBase();
	virtual StyleString longExplanationBase();
	virtual void longExplanationExtraBase(std::vector<StyleString> &explanations);

	virtual StyleString ListFullName(){return FullName();};

	//スモールビュー
	virtual int Getsmollview(pcGameWindow& smollview){return false;};
	virtual StyleString ExtraHatString(){return _T("");};

	//--------------------------------------------
	//割り込みイベント
	//--------------------------------------------
	//virtual double CutInValue(int timing){return 0;};
	//virtual int CutInEvent(int timing){return 0;};
	//virtual void CutInAttack(int timing, cAttackinformation & attinfo){};
	virtual void CutIn(タイミング timing, cValiableField& valiable);



	//-----------------------------------
	//データパック_ここから
	//-----------------------------------
	union{
		int integer[DEF_DROP_DATAPACK_NUM];
		UByte uchar[DEF_DROP_DATAPACK_UCHAR_NUM];
	} dpack;
	inline UByte& dpack_char_index(int nth_int,int nth_char)
		{return dpack.uchar[nth_int*4 + nth_char];};
	
	public://呪い祝福
	virtual inline UByte & state(){return dpack_char_index(0,0);};//0.0番
	static const int STATE_NORMAL = 0;
	static const int STATE_CURSE = 1;
	static const int STATE_GOOD = 2;
	public://修正値
	virtual inline int & quality(){return dpack.integer[1];};//1番
	
	static const int SHOPFLAG_NONE = 0;
	static const int SHOPFLAG_SHOP = 1;
	static const int SHOPFLAG_YAKU = 2;
	static const int SHOPFLAG_CANNOTYAKU = 3;
	static const int SHOPFLAG_SHOULDYAKU = 4;

	static const int SHOPDISCOUNTFLAG_NONE = 0;
	static const int SHOPDISCOUNTFLAG_FREE = 1;
	static const int SHOPDISCOUNTFLAG_HALF = 2;

	virtual inline UByte & shopitemflag(){return dpack_char_index(0,1);};//0,1番
	virtual inline UByte & shopDiscountflag(){return dpack_char_index(0,2);};//0,2番

	protected://識別
	virtual inline UByte & appreciation_indivi(){return dpack_char_index(2,0);};//2,0番
	virtual inline UByte & appreciation_state(){return dpack_char_index(2,1);};//2,1番
	virtual inline UByte & appreciation_quality(){return dpack_char_index(2,2);};//2,2番


	//0~3までdrop
	//4~5まで種類
	//6~8まで具体種
	//9~11までメモ
	public:
	pcMemoString pmemo;
	//-----------------------------------
	//データパック_ここまで
	//-----------------------------------
protected:
	//-----------------------------------
	//一時保存データ
	//-----------------------------------
	int 踏みフラグ_;

public:
	//倉庫内でのアイテムのサイズ
	virtual int size(){return 1;};
	
	//インベントリ内でのアイテムのサイズ
	virtual int sizeInInventory(){return 1;};
	//アイテムがいっぱいでも拾えるときtrue
	virtual bool noVolumeInInventory(){return (sizeInInventory() == 0);};

	virtual int 修正値最大値(){return 9999;};
	virtual void 修正値丸め();

	//効果量最終出力
	virtual int 効果量(int index);


	//効果量(係数)
	virtual double 効果量係数();//効果量状態倍率を除く
	virtual double 効果量修正値倍率();
	virtual double 効果量状態倍率();
	virtual double 効果量基礎(int index);
	virtual int 効果量基礎_定義サイズ();
	virtual void 効果量推定テキスト(int index, StyleString& output);
	virtual void 能力効果量推定テキスト(int index, StyleString& output){効果量推定テキスト(index,output);};
	virtual double デフォルト効果量状態倍率_祝福(){return 3.0;};

	//修正値出現分布
	virtual void 修正値出現分布(map<int,int>& output);
	virtual void 修正値出現分布テキスト_デバグ用(StyleString& output);

	//状態値出現分布
	virtual void 状態値出現分布(map<int,int>& output);
	virtual void 状態値出現分布テキスト_デバグ用(StyleString& output);

	virtual int 値段基礎値();
	virtual int 値段補正値();//コレが物の値段

	virtual int 単体買値(int homeshopflag);//コレが割引込みの物の値段
	virtual int 内包アイテム込み買値(int homeshopflag);//コレが内包込みの物の値段
	virtual int 単体売値();
	virtual int 内包アイテム込み売値();

	virtual int 単体購入支払い値();//コレが買い物した時の値段
	virtual int 内包アイテム込み購入支払い値();//コレが内包込みの買い物した時の値段
	virtual int 単体売却受け取り値();
	virtual int 内包アイテム込み売却受け取り値();

	virtual void 幻想度加算();
	virtual int 消費時幻想度加算量();


	virtual double 値段割引();
	//中身の値段を別ルーチンで足す必要がある時false
	//単体買値 == 内包アイテム込み買値のときtrue
	virtual bool 内包アイテムは実体がない(){return true;};

	virtual int 厄ポイント値段基礎値();
	virtual int 厄ポイント値段補正値();
	virtual int 厄ポイント単体買値();
	virtual int 厄ポイント単体売値();
	virtual double 厄ポイント割引();

	virtual int 使用難度(){return 1;};


	//レア度(出現確率基礎値)
	virtual double レア度();

	//落ち物属性
	set<int> 属性;
protected:
	virtual void setattribute(){};
	virtual void setattribute_pri();
public:
	//命中力最終出力
	virtual double 命中力();
	//命中力基礎
	virtual double 命中力基礎();
	//命中力最終出力
	virtual bool 命中力必中(){return false;};
	
	static const int BREAKFLAG_NORMAL = 1;
	static const int BREAKFLAG_LOSS = 2;
	//破棄フラグ
	int broken;
	const bool isBroken(){return (broken != 0);};

	//-----------------------------------
	//投げ関係
	//-----------------------------------
	//跳ね返し無効フラグ
	int 跳ね返し無効フラグ;
	virtual pcCharacter 投擲者(){return 投擲者_weakpointer.lock();};
	wpcCharacter 投擲者_weakpointer;


	//ホールダー(誰が持っているか。根っこ)
	wpcCharacter Holder_weakpointer;
	pcCharacter Holder(){
		if(Parent()) return Parent()->Holder();
		else return Holder_weakpointer.lock();
	};


	//パレント(どこに入っているか。直上)
	wpcDroping Parent_weakpointer;	
	pcDroping Parent(){return Parent_weakpointer.lock();};
	virtual bool 内包されている(){return Parent()!=NULL;};


	//内包アイテム
	vector<pcDroping> includedItem;

public:
	virtual bool 被拾得可(){return false;};
	virtual bool 被放棄可(){return true;};
	virtual bool 被投擲可(){return true;};
	virtual bool 被設置可(){return true;};

	virtual bool 被破壊可(){return true;};


	virtual bool 売却可(){return false;};

	virtual void 被箱入れ準備(){return;};
	virtual bool 被箱入れ可(){return false;};

	virtual bool 内包ショートカット可(){return false;};
	virtual bool 内包落ち物対象可(){return false;};
	virtual vector<pcDroping> 内包落ち物対象リスト(){return vector<pcDroping>();};
	virtual int 内包落ち物サイズ(){return 0;};//box用

	//階段や壊れない高飛びわななど、踏めなくなるとクリアにかかわるものはtrue;
	virtual bool 大切(){return false;};



	//--------------------------------------------------------
	//メニュー関係
public:
	//メニューで選択時に最初に呼ばれる
	virtual int Action(IDirect3DDevice9 *pDev);


	//Fireできるときtrue;
	//virtual int isCanFireCommand( vector<pcDroping> &ObjectList, tstring verb){return true;};

	//オブジェクト(目的語)リストを得る;
	//案内を返す
	//virtual StyleString GetmenuFuncObjectsList(vector<pcDroping> &ObjectList, bool &MultiObj,int objectsize, tstring verb){return _T("");};
	//virtual void GetmenuFuncObjectsList(vector<vector<pcDroping>> &ObjectListlist, vector<StyleString>& listExplain, bool &MultiObj, int objectsize, tstring verb){return;};

	//コマンドを解決する;
	//virtual int FireCommand(IDirect3DDevice9 *pDev, tstring verb, vector<pcDroping> &ObjectList);


	//***********************************************************
	//--------------------------------------------------------
	//コマンド群
	//--------------------------------------------------------
	//virtual int 説明(IDirect3DDevice9 *pDev);
	//virtual int 置く(IDirect3DDevice9 *pDev);
	//virtual int 拾う(IDirect3DDevice9 *pDev);
	//virtual int 投げる(IDirect3DDevice9 *pDev);
	//virtual int 交換(IDirect3DDevice9 *pDev);


	//自動プロセス群
	virtual int 乗る_記録();
	virtual int 乗る();
	virtual int 上に落ちる(pcDroping pdrop);//pdropに何か起こったらtrue;
	virtual int 上に落ちる(pcCharacter pchara);//charaに何か起こったらtrue;
	virtual int 上を通過(pcDroping pdrop);//pdropに何か起こったらtrue;
	virtual int 上を通過(pcCharacter pchara);//charaに何か起こったらtrue;

	virtual int 衝突(pcCharacter pchara);
	virtual int 衝突時効果(pcCharacter pchara){return false;};
	virtual bool 投擲貫通();
	virtual int 再設置(){return false;};

	//成功すればtrue
	virtual int まとめ要請(pcDroping pdrop){return false;};
	virtual int まとめる(pcDroping pdrop){return false;};

	virtual int 被まとめ要請(pcCharacter pchara){return false;};
	virtual int 被まとめる(pcCharacter pchara){return false;};


	virtual int 被呪い要請();
	virtual int 被祝福要請();

	
	virtual int 異常状態解除();
	virtual int 異常状態解除_内容込み();

	virtual bool 罠用_効果目標か(pcCharacter pchara){return false;};

public:	
	//メニューのcaptionを得る;
	virtual int GetmenuCaption(vector<tstring> &CaptionList){return false;};
	//ショートカットに入れられるcaptionのリストを得る;
	virtual int EnableShortCutCaptions(vector<tstring> &CaptionList){
		GetmenuCaption(CaptionList);
		return true;
	};

	//captionのexplainを得る;
	virtual StyleString GetmenuExplain(tstring &Caption){return _T("");};


	//メニューをセットする;
	virtual int SetMenu(IDirect3DDevice9 *pDev);




protected:
	virtual inline int IconFileSize(){return 128;};
	virtual inline tstring IconFileName(){return _T("");};
	virtual inline int IconFileIndexX(){return 0;};
	virtual inline int IconFileIndexY(){return 0;};

	virtual inline int IconForIconFileIndexX(){return IconFileIndexX();};
	virtual inline int IconForIconFileIndexY(){return IconFileIndexY();};

	IDirect3DTexture9* m_pTexture_Icon;
	IDirect3DTexture9* m_pTexture_StateIcon;
public:

	//-----------------------------------
	//御霊合成関係
	//-----------------------------------
	virtual double 能力スロット();
	virtual double 初期残りスロット();
	virtual double 初期初期残りスロット();
	virtual double 残りスロット();
	virtual double スロットキャパシティ(){return 初期残りスロット() + 能力スロット();};
	virtual double スロット容量(){return スロットキャパシティ() - 残りスロット();};
	virtual bool 御霊状態();
	virtual pcDroping 拠り代();
	virtual bool 特殊能力あり();
	virtual bool 恒常能力();

	virtual void 能力(const タイミング timing, cValiableField& valiable);
	virtual tstring 能力名基礎();
	virtual tstring 能力略名基礎();
	virtual StyleString 能力名Full();//・・・の御霊
	virtual StyleString 能力略名Full();//[]
	virtual StyleString 能力説明();
	virtual StyleString 能力説明Caption();
	virtual StyleString 能力スロット表現();

	virtual pcCharacter 装備者_攻撃用();
	virtual pcCharacter 装備者_防御用();
	virtual pcCharacter 装備者();
	virtual bool 装備されている_攻撃用();
	virtual bool 装備されている_防御用();
	virtual bool 能力発動条件満たしている_攻撃用();
	virtual bool 能力発動条件満たしている_防御用();
	virtual bool 装備マーク攻撃();
	virtual bool 装備マーク防御();
	//---------------------------------------
	//ユーティリティ
	//---------------------------------------
	virtual pcCharacter 乗りキャラ();
	virtual pcLandform 現在地形();

	virtual bool 落ちてる();
	virtual bool 倉庫の中();
};





#define NULLDROP pcDroping((cDroping*)NULL)

class cDropingNothing :
	public cDroping
{
public:
	cDropingNothing(void){name_ = _T("なにもない");};
	cDropingNothing(const tstring& tstr){name_ = tstr;};
public:
	virtual ~cDropingNothing(void){};


	virtual int ID(){return 0;};

	virtual StyleString FullName();
	virtual StyleString FullNameSub(){return _T("Nothing");};
	virtual StyleString ShortName();
	virtual StyleString ShortNameSub(){return _T("Nothing");};
	virtual tstring usefulName(){return _T("Nothing");};
	
	virtual tstring FullNameBase(){return name_;};
	virtual tstring ShortNameBase(){return name_;};

	virtual StyleString shortExplanation(){return _T("");};
	virtual StyleString longExplanation(){return _T("");};

	virtual void DrawIcon(IDirect3DDevice9 *pDev,int x,int y){};
private:
	tstring name_;
};



//Databaseに移動
//double def_decode_value(int ID, double defaultval, const TCHAR* valname, int index = 0);
//int def_decode_value_size(int ID, const TCHAR* valname);