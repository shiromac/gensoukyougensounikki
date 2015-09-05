#pragma once
#include "cgamewindow.h"
#include "cCommand.h"

#include <vector>
using namespace std;

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cCommand> pcCommand;

#define CURSORSIZE 32


class cSelectWindow :
	public cGameWindow
{
public:
	cSelectWindow(void);

	virtual void Init(IDirect3DDevice9 *pDev, int letterXnum, int letterYnum);

public:
	virtual void SetCursolTolast();


	int cursolEnable;//カーソルを表示するかどうか
	double cursolOpacity;
	int decisionEnable;//決定ボタンが働くかどうか
	int shortCutState;
protected:
	virtual int TextDraw(IDirect3DDevice9 *pDev);
	virtual int CursorDraw(IDirect3DDevice9 *pDev);
	//virtual int pageDraw(IDirect3DDevice9 *pDev);

	virtual int TextDrawSub(IDirect3DDevice9 *pDev);

	virtual int RerenderDraw(IDirect3DDevice9 *pDev);

	virtual int ShortCutDraw(IDirect3DDevice9 *pDev);

	//SpriteText m_pageText;
	//int m_pageAnimeCount;
	
	//IDirect3DTexture9* m_pTexture_Page;

	int cursolIndexMax;
	int cursolIndex;

	virtual int pageIndexMaxF(){return pageIndexMax;};
	int pageIndexMax;
	//int pageIndex;




	virtual int cursoletcMarginWidth();


	int shortCutSelect_X;
	int shortCutSelect_Y;
	virtual int shortCut_process(IDirect3DDevice9 *pDev);

	tstring selectMemoryKey_;
	virtual void memorizeSelect();
	virtual void loadMemorizedSelect();
public:
	virtual ~cSelectWindow(void);

	vector<pcCommand> commandList;
	virtual void addCommand(pcCommand pcommand){commandList.push_back(pcommand);};
	virtual int commandSize(){return commandList.size();};
	virtual int process(IDirect3DDevice9 *pDev);
	virtual int Draw(IDirect3DDevice9 *pDev);

	virtual StyleString shortExplanationString(IDirect3DDevice9 *pDev);

	virtual pcCommand pforcusedcommand();
	virtual void setSelectMemoryKey(tstring& tstr);//選択欄を保存するためのキーをセットする（保存する設定をオン）
};

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cSelectWindow> pcSelectWindow;