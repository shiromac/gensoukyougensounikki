#pragma once
#include "cAnimation.h"
#include "cLandform.h"
#include "cCharacter.h"

#include <vector>
#include <list>
#include <tstring_ph.h>


using namespace std;

#define ANIME_RETIRE_STEP (12) 



class caRetire :
	public cAnimation
{
public:
	caRetire(void);
public:
	virtual ~caRetire(void);


	virtual int process(IDirect3DDevice9 *pDev);
	virtual int Draw(IDirect3DDevice9 *pDev);
	virtual int Init(pcAnimation me);



	virtual bool isOnScreen();
	int vi;

	pcCharacter pRetirer;

	double Baseopaque;

	double count;
	double speed;



protected:
	cDrawingObject DO_;
	double randres;
};
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<caRetire> pcaRetire;

