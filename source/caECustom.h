#pragma once

#include "cAnimation.h"
#include "cLandform.h"
#include "cDroping.h"

#include "caEchipS.h"

#include <vector>
#include <list>
#include <tstring_ph.h>


using namespace std;

#define SIZE_BASICTEX (256)

class caECustom;
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<caECustom> pcaECustom;

//-------------------------------------------------
//ÉJÉXÉ^ÉÄ
//-------------------------------------------------
class caECustom :
	public cAnimationChipManager
{
public:
	caECustom(void);
	caECustom(int x,int y,int z);
public:
	virtual ~caECustom(void);


	virtual int process(cRenderDevice *pDev);
	virtual int Draw(cRenderDevice *pDev);
	
protected:
	virtual int preprocess(cRenderDevice *pDev);
	int preprocessed;

	cRenderTexture*pTex;
public:

	virtual bool isOnScreen();

};


