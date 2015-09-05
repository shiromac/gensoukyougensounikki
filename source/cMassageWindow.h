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

	virtual void Init(IDirect3DDevice9 *pDev, int letterXnum, int letterYnum);
protected:
	virtual int TextDraw(IDirect3DDevice9 *pDev);
	virtual int NewRoll(IDirect3DDevice9 *pDev);
	virtual int RenderText(IDirect3DDevice9 *pDev);
	virtual int TextDrawSub(IDirect3DDevice9 *pDev);

	deque<psstring> massagelogs;
	deque<psstring> rollmassages;
	int rolldiff;
	deque<psstring> reservemassages;

	int textmaxWidth;
	int lineNum;

	int forcusTop;

	int Waiting;//‘Ò‚¿ƒtƒ‰ƒO
	IDirect3DTexture9* pTex_cursol;

public:
	virtual int Wait();
	virtual int unWait();

	int VisibleCount;


	virtual ~cMassageWindow(void);


	virtual int process(IDirect3DDevice9 *pDev);
	virtual int Draw(IDirect3DDevice9 *pDev);

	virtual int addMassage(StyleString massage);

	virtual int textclear(IDirect3DDevice9 *pDev);
};
