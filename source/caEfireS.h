#pragma once

#include "cAnimation.h"
#include "cLandform.h"
#include "cDroping.h"

#include "caEchipS.h"

#include <vector>
#include <list>
#include <tstring_ph.h>


using namespace std;

class caEfire_flame;
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<caEfire_flame> pcaEfire_flame;

//-------------------------------------------------
//ˆêƒ}ƒX”R‚¦
//-------------------------------------------------
class caEfire_flame :
	public cAnimationChipManager
{
public:
	caEfire_flame(int x = 0,int y = 0,int z = 0, int layer = 0,int lightflag = 0);
public:
	virtual ~caEfire_flame(void);


	virtual int process(cRenderDevice *pDev);
	virtual int Draw(cRenderDevice *pDev);
	

	virtual int Init();
protected:
	virtual int preprocess(cRenderDevice *pDev);
	int preprocessed;

public:

	virtual bool isOnScreen();


	c4DVector place;
	int lightflag_;


};

