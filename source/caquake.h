#pragma once

#include "cAnimation.h"
#include "cLandform.h"
#include "cDroping.h"


#include <vector>
#include <list>
#include <tstring_ph.h>


using namespace std;


class caQuake :
	public cAnimation
{
public:
	caQuake();
public:
	virtual ~caQuake(void);

	virtual int process(IDirect3DDevice9 *pDev);
	virtual int Draw(IDirect3DDevice9 *pDev);
	virtual int Init();

	virtual bool isOnScreen();

	int time_frame;
	int amplitude;

	int preprocess;

	double ForcusX;
	double ForcusY;
	int count;

	double countX;
	double countY;
};

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<caQuake> pcaQuake;