#pragma once
#include "ccontrol.h"

#include "cGameWindow.h"
#include "cSelectWindow.h"

#include <list>
#include <tstring_ph.h>


using namespace std;



class cControlLayer :
	public cControl
{
public:
	cControlLayer(void);
public:
	virtual ~cControlLayer(void);

	vector<pcGameWindow>	WindowList;
	virtual int process(IDirect3DDevice9 *pDev);
	virtual int Draw(IDirect3DDevice9 *pDev);

	virtual int Init(IDirect3DDevice9 *pDev);

	int DrawedAndNotProcessingFlag;
	int InputFreeFlag;//TRUEのとき他のインターフェイスを動かさない//今のところStringWindow専用

	int notCancelToPop;
	int PleasePop;

	pcGameWindow countStringWindow;
	pcGameWindow smollviewWindow;
protected:
	StyleString shortExplanationText;
	virtual int shortExplanationprocess(IDirect3DDevice9 *pDev);
	pcGameWindow shortExplanationWindow;

	int sscounter;

};

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cControlLayer> pcControlLayer;
