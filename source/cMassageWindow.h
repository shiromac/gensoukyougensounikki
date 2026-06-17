#pragma once
#include "cgamewindow.h"

#include "utility/StyleString.h"

#include <deque>
#include <string>
using namespace std;

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<StyleString> psstring;

class cMassageWindow :
	public cGameWindow
{
public:
	cMassageWindow(void);

	virtual void Init(cRenderDevice *pDev, int letterXnum, int letterYnum);
protected:
	virtual int TextDraw(cRenderDevice *pDev);
	virtual int NewRoll(cRenderDevice *pDev);
	virtual int RenderText(cRenderDevice *pDev);
	virtual int TextDrawSub(cRenderDevice *pDev);

	deque<psstring> massagelogs;
	deque<psstring> rollmassages;
	int rolldiff;
	deque<psstring> reservemassages;

	int textmaxWidth;
	int lineNum;

	int forcusTop;

	int Waiting;//‘Ò‚¿ƒtƒ‰ƒO
	cRenderTexture* pTex_cursol;

public:
	virtual int Wait();
	virtual int unWait();

	int VisibleCount;


	virtual ~cMassageWindow(void);


	virtual int process(cRenderDevice *pDev);
	virtual int Draw(cRenderDevice *pDev);

	virtual int addMassage(StyleString massage);

	virtual int textclear(cRenderDevice *pDev);
};
