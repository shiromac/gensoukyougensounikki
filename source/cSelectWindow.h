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

	virtual void Init(cRenderDevice *pDev, int letterXnum, int letterYnum);

public:
	virtual void SetCursolTolast();


	int cursolEnable;//カーソルを表示するかどうか
	double cursolOpacity;
	int decisionEnable;//決定ボタンが働くかどうか
	int shortCutState;
protected:
	virtual int TextDraw(cRenderDevice *pDev);
	virtual int CursorDraw(cRenderDevice *pDev);
	//virtual int pageDraw(cRenderDevice *pDev);

	virtual int TextDrawSub(cRenderDevice *pDev);

	virtual int RerenderDraw(cRenderDevice *pDev);

	virtual int ShortCutDraw(cRenderDevice *pDev);

	//SpriteText m_pageText;
	//int m_pageAnimeCount;
	
	//cRenderTexture* m_pTexture_Page;

	int cursolIndexMax;
	int cursolIndex;

	virtual int pageIndexMaxF(){return pageIndexMax;};
	int pageIndexMax;
	//int pageIndex;




	virtual int cursoletcMarginWidth();


	int shortCutSelect_X;
	int shortCutSelect_Y;
	virtual int shortCut_process(cRenderDevice *pDev);

	tstring selectMemoryKey_;
	virtual void memorizeSelect();
	virtual void loadMemorizedSelect();
public:
	virtual ~cSelectWindow(void);

	vector<pcCommand> commandList;
	virtual void addCommand(pcCommand pcommand){commandList.push_back(pcommand);};
	virtual int commandSize(){return commandList.size();};
	virtual int process(cRenderDevice *pDev);
	virtual int Draw(cRenderDevice *pDev);

	virtual StyleString shortExplanationString(cRenderDevice *pDev);

	virtual pcCommand pforcusedcommand();
	virtual void setSelectMemoryKey(const tstring& tstr);//選択欄を保存するためのキーをセットする（保存する設定をオン）
};

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cSelectWindow> pcSelectWindow;