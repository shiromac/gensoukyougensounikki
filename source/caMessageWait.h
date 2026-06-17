#pragma once
#include "cAnimation.h"

#include <vector>
#include <list>
#include <tstring_ph.h>


using namespace std;

class caMessageWait;
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<caMessageWait> pcaMessageWait;

class caMessageWait :
	public cAnimation
{
public:
	caMessageWait(void);
public:
	virtual ~caMessageWait(void);

	//cRenderTexture* pTex;

	virtual int process(cRenderDevice *pDev);
	virtual int Draw(cRenderDevice *pDev);

	virtual bool isOnScreen();

	int anykey;
private:
	int preprocessed;

	int preendflag;


};

class caMessageClear;
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<caMessageClear> pcaMessageClear;

class caMessageClear :
	public cAnimation
{
public:
	caMessageClear(void);
public:
	virtual ~caMessageClear(void);

	int preprocessed;

	//cRenderTexture* pTex;

	virtual int process(cRenderDevice *pDev);
	virtual int Draw(cRenderDevice *pDev);

	virtual bool isOnScreen();


};


class caPrimaryInterfaceWait;
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<caPrimaryInterfaceWait> pcaPrimaryInterfaceWait;

class caPrimaryInterfaceWait :
	public cAnimation
{
public:
	caPrimaryInterfaceWait(void);
public:
	virtual ~caPrimaryInterfaceWait(void);

	//cRenderTexture* pTex;

	virtual int process(cRenderDevice *pDev);
	virtual int Draw(cRenderDevice *pDev);

	virtual bool isOnScreen();

private:
	int preprocessed;


};
