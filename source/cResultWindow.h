#pragma once
#include "cgamewindow.h"
#include "cCommand.h"
#include "cSaveStore.h"

#include <vector>
using namespace std;

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cCommand> pcCommand;

#define CURSORSIZE 32


class cResultWindow :
	public cGameWindow
{
public:
	cResultWindow(void);

	virtual void Init(IDirect3DDevice9 *pDev,pcSaveResult pResult);
	virtual void Init(IDirect3DDevice9 *pDev,cSaveResult& Result);
protected:
	virtual void Init(IDirect3DDevice9 *pDev, int letterXnum, int letterYnum);


protected:


	cSaveResult Result_;


public:
	virtual ~cResultWindow(void);

	virtual int process(IDirect3DDevice9 *pDev);
};

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cResultWindow> pcResultWindow;