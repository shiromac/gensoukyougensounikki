#pragma once
#include "cdroping.h"

#include "utility/cDiscreteProbability.h"

#define TRAP_COLOR (0xFFFFAAFF)

class cTrap;
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
typedef boost::shared_ptr<cTrap> pcTrap;
typedef boost::weak_ptr<cTrap> wpcTrap;

class cTrap :
	public cDroping
{
public:
	cTrap(void);
public:
	virtual ~cTrap(void);


	//レア度(出現確率基礎値)
	//virtual double レア度();

	//発動率(%)
	virtual double 発動率();
	virtual double 発動率_発見済み();
	//故障率(%)
	virtual double 故障率();

	virtual int 消費時幻想度加算量();


	//データパックを初期最適化する。アイテムを初期設置するときに一度だけ使う。
	virtual void DataBeginOptimize(int difficulty);

	virtual void AppreciationBeginOptimize(int difficulty);

	virtual void visibleReset();

	virtual cDiscreteProbability stateBeginDP(int difficulty);
	virtual cDiscreteProbability qualityBeginDP(int difficulty);

	virtual StyleString QualityCaption();

	virtual StyleString FullName();
	virtual StyleString FullNameSub();
	virtual StyleString ShortName();
	virtual StyleString ShortNameSub();


	virtual StyleString 識別未識別混合道具名(){return FullNameBase();};

	//マップ上に入った
	virtual int OnMapping();


	virtual bool 被箱入れ可(){return false;};



	//virtual StyleString shortExplanationBase() = 0;
	//virtual StyleString longExplanationBase() = 0;

	virtual bool 被拾得可();
	virtual bool 売却可(){return false;};

	virtual void setattribute()
	{
		属性.insert(落ち物属性::物体);	
		属性.insert(落ち物属性::罠);
		属性.insert(落ち物属性::不燃性);
		属性追加();
	};
	virtual void 属性追加(){};

	//-----------------------------------
	//データパック_ここから
	//-----------------------------------

public://踏んだ数
	virtual inline int & stepcount(){return dpack.integer[4];};//4番
	virtual inline UCHAR & setflag(){return dpack_char_index(5,0);};//5,0番
	//4~5まで種類
	//6~8まで具体種
	//-----------------------------------
	//データパック_ここまで
	//-----------------------------------

	//一時記憶
	int Fired;


	
	virtual int GetmenuCaption(vector<tstring> &CaptionList);

	//コマンドを解決する;
	//virtual int FireCommand(IDirect3DDevice9 *pDev, tstring verb, vector<pcDroping> &ObjectList);

	//--------------------------------------------------------
	//コマンド群
	//virtual int 踏む(IDirect3DDevice9 *pDev);
	//virtual int 踏み要請();

	virtual void CutIn(タイミング timing, cValiableField& valiable);


	virtual bool 罠用_効果目標か(pcCharacter pchara);
	//一時記憶
	map<wpcCharacter, int> 罠用_効果目標か_一時記憶_;


	//自動プロセス群
	virtual int 乗る();
	virtual int 上に落ちる(pcDroping pdrop);//pdropに何か起こったらtrue;
	virtual int 上に落ちる(pcCharacter pchara);//charaに何か起こったらtrue;

	virtual int 発動要請();
	virtual int 発動要請(pcDroping pdrop);
	virtual int 発動要請(pcCharacter pchara);

	virtual int 衝突(pcCharacter pchara);
	virtual int 再設置();

protected:
	virtual int 発動(pcCharacter pchara);
	virtual int 発動(pcDroping pdrop);//pdropに何か起こったらtrue;
	bool isDiscoveredWhen乗るBefore;
public:
	virtual int 効果();
	virtual int 効果(pcCharacter pchara);
	virtual int 効果(pcDroping pdrop);//pdropに何か起こったらtrue;

	virtual int 故障();


	virtual void 踏みエフェクト();
};

