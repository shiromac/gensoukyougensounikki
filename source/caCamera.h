#pragma once

#include "cAnimation.h"
#include "cDroping.h"


#include <vector>
#include <list>
#include <tstring_ph.h>


using namespace std;


class caCameraMove :
	public cAnimation
{
public:
	/*
	enum CameraMoveMode
	{
		CameraMoveMode_linear,
		CameraMoveMode_equalRatio,
		CameraMoveMode_quadratic,
	};
	*/
	caCameraMove(double x, double y, double speed, double point1, double point2);
public:
	virtual ~caCameraMove(void);

	virtual int process(cRenderDevice *pDev);
	virtual int Draw(cRenderDevice *pDev);
	virtual int Init();

	virtual bool isOnScreen();


protected:
	double x_;
	double y_;
	double preforcusX_;
	double preforcusY_;
	//CameraMoveMode mode_;
	double speed_;

	double count_;
	double time_frame_;


	double b_;
	double c_;

	int preprocess_;

};

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<caCameraMove> pcaCameraMove;