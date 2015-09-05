#pragma once
#include "cgamewindow.h"
#include "cCommand.h"
#include "cSaveStore.h"

#include <vector>
using namespace std;

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cCommand> pcCommand;

#define CURSORSIZE 32


class cPadConfigWindow :
	public cSelectWindow
{
public:
	cPadConfigWindow(void);

	virtual void Init(IDirect3DDevice9 *pDev);

protected:
	virtual void Init(IDirect3DDevice9 *pDev, int letterXnum, int letterYnum);
	
	StyleString shortExplanationString(IDirect3DDevice9 *pDev);


protected:
	void setCommand();

	int padsetmode_;
	int padsetmodewait_;

	cGameWindow settextW_;

public:
	virtual ~cPadConfigWindow(void);

	virtual int process(IDirect3DDevice9 *pDev);
	virtual int Draw(IDirect3DDevice9 *pDev);
};

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cPadConfigWindow> pcPadConfigWindow;

class cParamaterConfigWindow :
	public cGameWindow
{
public:
	cParamaterConfigWindow(void);

	virtual void Init(IDirect3DDevice9 *pDev, int *valuepointer, int min, int max);

	typedef void(*CallBackFunction)(void);
	CallBackFunction DidDismissWindow;
	CallBackFunction DidChangeValue;
protected:
	virtual void Init(IDirect3DDevice9 *pDev, int letterXnum, int letterYnum);

	virtual int pageDraw(IDirect3DDevice9 *pDev);
	virtual int TextDraw(IDirect3DDevice9 *pDev);
	

protected:

	int first_;
	int* output_;
	int min_;
	int max_;

public:
	virtual ~cParamaterConfigWindow(void);

	virtual int process(IDirect3DDevice9 *pDev);
	virtual int Draw(IDirect3DDevice9 *pDev);
};

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cParamaterConfigWindow> pcParamaterConfigWindow;
