#pragma once
#include "cDrawingObject.h"
#include <d3d9.h>

#include "utility/cCurveTrack.h"

class cDrawingObject_belt :
	public cDrawableObject
{
public:
	cDrawingObject_belt(void);
public:
	virtual ~cDrawingObject_belt(void);

	virtual bool process(){return true;};//いらない関数ｗ



	//精度（ポリゴン数/2）
	int Accuracy;

	double start;
	double end;

protected:
	c4DVector startingpoints[2];
	cCurveTrack Tracks[2];
	
public:
	virtual c4DVector& rightStartpoint(){return startingpoints[1];};
	virtual c4DVector& leftStartpoint(){return startingpoints[0];};
	virtual cCurveTrack& rightline(){return Tracks[1];};
	virtual cCurveTrack& leftline(){return Tracks[0];};



	virtual int Draw(IDirect3DDevice9 *pDev);
};
