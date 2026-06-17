#pragma once
#include "cgameobject.h"
#include "utility/SpriteText.h"

#include "cDroping.h"
#include "cDropingFunc.h"

#include <vector>
#include <set>
using namespace std;


class cCommandDroping :
	public cDroping
{
public:
	cCommandDroping(void);
	cCommandDroping(pcDropingFunc pdfunc);
public:
	virtual ~cCommandDroping(void);


	//消去処理の準備
	virtual void Release(){};

	virtual StyleString shortExplanation();
	virtual StyleString longExplanation();


	virtual void DrawIcon(cRenderDevice *pDev,int x,int y);
	virtual void DrawStateIcon(cRenderDevice *pDev,int x,int y);
	virtual void DrawStateIconSub(cRenderDevice *pDev,int x,int y);

	virtual int RerenderText(SpriteText & sprite,int MaxLetterLength);
	virtual int Getsmollview(pcGameWindow& smollview);
	
	//メニューで選択時に最初に呼ばれる
	virtual int Action(cRenderDevice *pDev);

	virtual bool 内包落ち物対象可();
	virtual int 内包落ち物サイズ();
	virtual vector<pcDroping> 内包落ち物対象リスト();

	pcDropingFunc pdropingfunc;

};
#include <boost/shared_ptr.hpp>
class cCommandDroping;
typedef boost::shared_ptr<cCommandDroping> pcCommandDroping;


pcDroping DropingFuncToDroping(pcDropingFunc pdfunc);


class cObjectiveDroping :
	public cDroping
{
public:
	cObjectiveDroping(pcDroping surface, pcCommand action);
public:
	virtual ~cObjectiveDroping(void);


	//消去処理の準備
	virtual void Release(){};

	virtual StyleString shortExplanation();
	virtual StyleString longExplanation();


	virtual void DrawIcon(cRenderDevice *pDev,int x,int y);
	virtual void DrawStateIcon(cRenderDevice *pDev,int x,int y);
	virtual void DrawStateIconSub(cRenderDevice *pDev,int x,int y);

	virtual int RerenderText(SpriteText & sprite,int MaxLetterLength);
	virtual int Getsmollview(pcGameWindow& smollview);
	
	//メニューで選択時に最初に呼ばれる
	virtual int Action(cRenderDevice *pDev);

	virtual bool 内包落ち物対象可();
	virtual int 内包落ち物サイズ();
	virtual vector<pcDroping> 内包落ち物対象リスト();

private:
	pcDroping surface_;
	pcCommand action_;

};
#include <boost/shared_ptr.hpp>
class cObjectiveDroping;
typedef boost::shared_ptr<cObjectiveDroping> pcObjectiveDroping;
