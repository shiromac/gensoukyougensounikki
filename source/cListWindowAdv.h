#pragma once
#include "clistwindow.h"

#include "utility/StyleString.h"

#include <deque>
#include <set>
#include <tstring_ph.h>


using namespace std;

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<StyleString> psstring;


#define LISTWINDOW_MAXINDEX 10

class cListWindowAdv :
	public cListWindow
{
public:
	cListWindowAdv(void);
	~cListWindowAdv(void);


	virtual void Init(cRenderDevice *pDev, int letterXnum, int letterYnum);

public:
	virtual void SetCursolTolast();

protected:
	//virtual int TextDraw(cRenderDevice *pDev);
	virtual int Draw(cRenderDevice *pDev);
	

	//virtual int TextDrawSub(cRenderDevice *pDev);

	virtual int RerenderDraw(cRenderDevice *pDev);


	//SpriteText ListText[LISTWINDOW_MAXINDEX];
	//cDrawingObject ListIcon[LISTWINDOW_MAXINDEX];


	virtual int EventCarsolmove();
protected:
	//int preCarsolIndex;

public:


	virtual int process(cRenderDevice *pDev);
	

	//追加メンバ
public:
	vector<vector<pcDroping>*> pvpDroplist_forSort;
	vector<vector<pcDroping>> vpDroplist;
	vector<tstring> pvpDroplistName;
protected:
	vector<vector<pcDroping>*> pvpDropFrmPgIndex;
	vector<vector<pcDroping>*> pvpDropFrmPgIndex_forSort;
	vector<int> pvpDroplistFirstIndex;
	vector<int> ContentNumFrmPgIndex;
	virtual int ContentNumFrmPgIndexF(int page){return ContentNumFrmPgIndex[page];};
	vector<StyleString> LabelFrmPgIndex;

	virtual pcDroping contents(int page, int index);
	virtual pcDroping nowcontents();//現在選択しているコンテンツ
	

	pcGameWindow pwLabel;

public:
	//vector<pcDroping> pDroplist;
	//vector<pcDroping>*ppDroplist;

	virtual int sortDrops(cRenderDevice *pDev);
	
	virtual int togetherDrops(cRenderDevice *pDev);
	
	
	virtual StyleString shortExplanationString(cRenderDevice *pDev);
/*
public:
	//--------------------------------------------------
	//複数選択
	int MultiSelect;
	set<pcDroping> pMultiDropSet;
*/
	/*
	virtual int processofMultiSet(cRenderDevice *pDev);
	virtual int MultiSetDraw(cRenderDevice *pDev);


	virtual int FireofMultiSet(cRenderDevice *pDev);
	virtual int insertMultiSet();
	virtual int eraseMultiSet();
*/
	virtual int MultiSetDraw(cRenderDevice *pDev);
	
public:
	pcListWindow psubWindow;
	map<pcDroping,vector<pcDroping>> vpsubDropmap;
	virtual int subWindowProcess(cRenderDevice *pDev);
	int subWindowMode;
};
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cListWindowAdv> pcListWindowAdv;

bool itemIDsortFunc(pcDroping a, pcDroping b);