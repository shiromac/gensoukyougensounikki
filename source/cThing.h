#pragma once
#include "cdroping.h"
#include "caEchipS.h"
#define CDROPING_cThing_BASE_ID_NUM 11000


#define DEF_class_cThing_ID(identify) \
class cThing_ID_##identify## :\
	public cDroping\
{\
public:\
	cThing_ID_##identify##(void){};\
public:\
	virtual ~cThing_ID_##identify##(void){};\
	virtual inline int ID(){return CDROPING_cThing_BASE_ID_NUM + identify;};\
public:\

//火
DEF_class_cThing_ID(0)
	virtual StyleString FullName(){return FullNameBase();};
	virtual StyleString ShortName(){return ShortNameBase();};
	virtual inline tstring IconFileName(){return _T("item\\アイテムテンプレート１.png");};
	virtual inline int IconFileIndexX(){return 1;};
	virtual inline int IconFileIndexY(){return 0;};
	virtual bool 被拾得可(){return false;};
	virtual bool 売却可(){return false;};

	virtual void DataBeginOptimize(int difficulty);

	virtual double 効果量修正値倍率(){return 0.0;};

	virtual int GetmenuCaption(vector<tstring> &CaptionList);

	virtual void Init(IDirect3DDevice9 *pDev,pcDroping self);
	virtual void Draw(IDirect3DDevice9 *pDev);
	virtual void Process(IDirect3DDevice9 *pDev);
	virtual int 乗る();
	virtual void CutIn(タイミング timing, cValiableField& valiable);
	virtual int 効果(pcCharacter pchara, int power, int mustFlag = 0);

	//表示を実値と同期させる
	virtual void visibleReset();

	virtual int 上に落ちる(pcDroping pdrop);//charaに何か起こったらtrue;
	virtual int 上に落ちる(pcCharacter pchara);//pdropに何か起こったらtrue;
	virtual int 上を通過(pcDroping pdrop);//pdropに何か起こったらtrue;
	virtual int 上を通過(pcCharacter pchara);//charaに何か起こったらtrue;

	vector<pcaEchip_Basic> flooranime;
	vector<pcaEchip_Basic> flameanime;

	virtual int flamelevel();
	//-----------------------------------
	//データパック_ここから
	//-----------------------------------
public:
	virtual inline int & flamepower(){return dpack.integer[1];};//1番quality()上書き

	//4~5まで種類
	virtual inline UByte & burned(){return dpack_char_index(4,0);};//4,0番

	//6~8まで具体種
	//-----------------------------------
	//データパック_ここまで
	//-----------------------------------
	virtual int Maxflamepower();

	virtual void setflamepower(int power);
	virtual void recoverflamepower(int power);
	virtual inline int & visibleflamepower(){return visibleflamepower_;};
protected:
	SharedValiable_likeInt<int> visibleflamepower_;

	virtual void setattribute()
	{
		属性.insert(落ち物属性::火柱);
	};
};

//御柱
DEF_class_cThing_ID(1)
	virtual StyleString FullName(){return FullNameBase();};
	virtual StyleString ShortName(){return ShortNameBase();};
	virtual inline tstring IconFileName(){return _T("item\\階段セット.png");};
	virtual inline int IconFileIndexX(){return 1;};
	virtual inline int IconFileIndexY(){return 0;};
	virtual inline int IconForIconFileIndexX(){return 0;};
	virtual inline int IconForIconFileIndexY(){return 1;};

	virtual bool 被拾得可(){return false;};
	virtual bool 売却可(){return false;};
	virtual double 効果量修正値倍率(){return 0.0;};

	virtual int 乗る();
	virtual int 上を通過(pcCharacter pchara);//charaに何か起こったらtrue;
	virtual void CutIn(タイミング timing, cValiableField& valiable);

	virtual int GetmenuCaption(vector<tstring> &CaptionList);

	virtual void Draw(IDirect3DDevice9 *pDev);
	virtual int GetIcon(cDrawingObject& DO);
};

//出口
DEF_class_cThing_ID(2)
	virtual StyleString FullName(){return FullNameBase();};
	virtual StyleString ShortName(){return ShortNameBase();};
	virtual inline tstring IconFileName(){return _T("item\\出入口.png");};
	virtual inline int IconFileIndexX(){return 0;};
	virtual inline int IconFileIndexY(){return 0;};

	virtual bool 被拾得可(){return false;};
	virtual bool 売却可(){return false;};
	virtual double 効果量修正値倍率(){return 0.0;};

	virtual int 乗る();
	virtual bool 大切(){return true;};


	virtual int GetmenuCaption(vector<tstring> &CaptionList);

	virtual void Draw(IDirect3DDevice9 *pDev);
	virtual int GetIcon(cDrawingObject& DO);
};


#define THING_NUM 3