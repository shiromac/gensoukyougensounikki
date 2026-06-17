#pragma once
#include "cAnimation.h"
#include "cLandform.h"
#include "cGameObject.h"

#include <vector>
#include <list>
#include <tstring_ph.h>


using namespace std;

#define ANIME_MOVE_STEP 16 

class caMove :
	public cAnimation
{
public:
	caMove(void);
public:
	virtual ~caMove(void);


	virtual int process(cRenderDevice *pDev);

	virtual bool isOnScreen();

	pcGameObject pGobj;

	double count;
	double speed;

	vector<pcLandform> checkpoint;
	vector<double> Sight;
	vector<int> Aspect;
};
