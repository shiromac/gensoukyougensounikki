#pragma once
#include "cselectwindow.h"

#include "utility/StyleString.h"
#include "utility/cNumField.h"

#include <deque>
#include <set>
#include <tstring_ph.h>


using namespace std;

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<StyleString> psstring;


class cListWindow;
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cListWindow> pcListWindow;

#define LISTWINDOW_MAXINDEX 10

class cListWindow :
	public cSelectWindow
{
public:
	cListWindow(void);

	virtual void Init(IDirect3DDevice9 *pDev, int letterXnum, int letterYnum);

public:
	virtual void SetCursolTolast();

	virtual int Draw(IDirect3DDevice9 *pDev);
protected:
	virtual int TextDraw(IDirect3DDevice9 *pDev);



	//virtual int TextDrawSub(IDirect3DDevice9 *pDev);

	virtual int RerenderDraw(IDirect3DDevice9 *pDev);


	//SpriteText ListText[LISTWINDOW_MAXINDEX];
	//cDrawingObject ListIcon[LISTWINDOW_MAXINDEX];


	virtual int EventCarsolmove();
protected:
	int preCarsolIndex;

	
	virtual pcDroping pforcusedcontents();


	virtual int cursoletcMarginWidth();
public:
	virtual ~cListWindow(void);
	virtual int process(IDirect3DDevice9 *pDev);
	
	vector<pcDroping> pDroplist;
	vector<pcDroping>*ppDroplist;

	virtual int sortDrops(IDirect3DDevice9 *pDev);
	
	virtual pcDroping contents(int page, int index);

	virtual pcDroping nowcontents()//現在選択しているコンテンツ
		{return contents(pageIndex, cursolIndex);};
	
	virtual int ContentNumFrmPgIndexF(int page)
	{
		return min(cursolIndexMax, pDroplist.size() - page*cursolIndexMax);
	};

	virtual StyleString shortExplanationString(IDirect3DDevice9 *pDev);

protected:
	cNumField numf;//数字描画用
	//pdropが含まれてる場合番号を返す 0 で含まれない
	virtual int findfromMultiDrop(pcDroping pdrop);
	
	//マルチセレクト動詞選択肢表示
	virtual int multiverbsSelect();

	IDirect3DTexture9* m_pTexture_Multiselect;
public:
	//--------------------------------------------------
	//複数選択
	int MultiSelect;
	vector<pcDroping> pMultiDropSet;


	virtual int processofMultiSet(IDirect3DDevice9 *pDev);
	virtual int MultiSetDraw(IDirect3DDevice9 *pDev);


	virtual int FireofMultiSet(IDirect3DDevice9 *pDev);
	virtual int insertMultiSet();
	virtual int eraseMultiSet();

	//複数選択時特殊動詞
	vector<tstring> multiverbs;


public:
	int shortCut_process(IDirect3DDevice9 *pDev);


};
