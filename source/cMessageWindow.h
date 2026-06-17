#pragma once
#include "cgamewindow.h"

#include "utility/StyleString.h"

#include <deque>
#include <tstring_ph.h>


using namespace std;

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<StyleString> psstring;

class cMessageWindow :
	public cGameWindow
{
public:
	cMessageWindow(void);

	virtual void Init(cRenderDevice *pDev, int letterXnum, int letterYnum);
protected:
	virtual int TextDraw(cRenderDevice *pDev);
	virtual int NewRoll(cRenderDevice *pDev);
	virtual int RenderText(cRenderDevice *pDev);
	virtual int TextDrawSub(cRenderDevice *pDev);
public:
	deque<psstring> Messagelogs;
protected:

	cRenderTexture* m_pTexture_Wait;

	deque<psstring> rollMessages;
	int rolldiff;
	deque<psstring> reserveMessages;

	int textmaxWidth;
	int lineNum;

	int forcusTop;

	int Waiting;//待ちフラグ
	cRenderTexture* pTex_cursol;

	virtual double massageSpeed();
public:
	virtual int Wait();
	virtual int unWait();


	//まだ流してないメッセージがある
	virtual int isStockingMassage(){return reserveMessages.size();};

	int VisibleCount;


	virtual ~cMessageWindow(void);


	virtual int process(cRenderDevice *pDev);
	virtual int Draw(cRenderDevice *pDev);

	virtual int addMessage(StyleString Message);

	virtual int textclear(cRenderDevice *pDev);
};
