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

	virtual void Init(IDirect3DDevice9 *pDev, int letterXnum, int letterYnum);
protected:
	virtual int TextDraw(IDirect3DDevice9 *pDev);
	virtual int NewRoll(IDirect3DDevice9 *pDev);
	virtual int RenderText(IDirect3DDevice9 *pDev);
	virtual int TextDrawSub(IDirect3DDevice9 *pDev);
public:
	deque<psstring> Messagelogs;
protected:

	IDirect3DTexture9* m_pTexture_Wait;

	deque<psstring> rollMessages;
	int rolldiff;
	deque<psstring> reserveMessages;

	int textmaxWidth;
	int lineNum;

	int forcusTop;

	int Waiting;//待ちフラグ
	IDirect3DTexture9* pTex_cursol;

	virtual double massageSpeed();
public:
	virtual int Wait();
	virtual int unWait();


	//まだ流してないメッセージがある
	virtual int isStockingMassage(){return reserveMessages.size();};

	int VisibleCount;


	virtual ~cMessageWindow(void);


	virtual int process(IDirect3DDevice9 *pDev);
	virtual int Draw(IDirect3DDevice9 *pDev);

	virtual int addMessage(StyleString Message);

	virtual int textclear(IDirect3DDevice9 *pDev);
};
