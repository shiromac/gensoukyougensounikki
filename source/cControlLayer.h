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
	virtual int process(cRenderDevice *pDev);
	virtual int Draw(cRenderDevice *pDev);

	virtual int Init(cRenderDevice *pDev);

	int DrawedAndNotProcessingFlag;
	int InputFreeFlag;//TRUEのとき他のインターフェイスを動かさない//今のところStringWindow専用

	int notCancelToPop;
	int PleasePop;

	pcGameWindow countStringWindow;
	pcGameWindow smollviewWindow;
protected:
	StyleString shortExplanationText;
	virtual int shortExplanationprocess(cRenderDevice *pDev);
	pcGameWindow shortExplanationWindow;

	int sscounter;

};

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cControlLayer> pcControlLayer;
