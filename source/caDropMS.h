#pragma once

#include "cAnimation.h"
#include "cLandform.h"
#include "cGameObject.h"

#include <vector>
#include <list>
#include <tstring_ph.h>


using namespace std;



#define ROUTE_HIGH (1)
#define ROUTE_LOW (0)

#define DRAWTYPE_NORMAL (0)
#define DRAWTYPE_ATTACKDRAW (1)
#define THROW_HIMANAGE (2)
#define THROW_HIMANAGESTRAIGHT (4)

#define THROW_BASE_HEIGHT (0.5)
#define THROW_PEAK_HEIGHT (THROW_BASE_HEIGHT + 1.0)


class caThrow;
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<caThrow> pcaThrow;

//ƒAƒCƒeƒ€“Š‚°
class caThrow :
	public cAnimation
{
public:
	caThrow(void);
public:
	virtual ~caThrow(void);


	virtual int process(IDirect3DDevice9 *pDev);
	virtual int Draw(IDirect3DDevice9 *pDev);
	

	virtual int Init(pcAnimation me);

protected:
	virtual int preprocess();
	double entiredistance;
	double progress;

	//‘Ò”ð
	bool t_shouldDraw;
public:

	virtual bool isOnScreen();


	c4DVector start;
	c4DVector end;

	int route;

	int flag;

	int Displayin;

	int drawtype;

	double speed;

	pcGameObject pThrowed;

	bool cameraTracing;
};

