#pragma once
#include "cAnimation.h"
#include "cLandform.h"
#include "cCharacter.h"

#include <vector>
#include <list>
#include <tstring_ph.h>


using namespace std;

#define ANIME_DAMAGE_STEP 10 

class caDamage;
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<caDamage> pcaDamage;

class caDamage :
	public cAnimation
{
public:
	caDamage(void);
public:
	virtual ~caDamage(void);

	virtual int preprocess(IDirect3DDevice9 *pDev);

	virtual int process(IDirect3DDevice9 *pDev);
	virtual int Init(pcAnimation me);

	virtual bool isOnScreen();
	int vi;

	bool preprocessed;

	pcCharacter pDamager;
	int Damage;
	double effective;

	double Baseopaque;

	double count;
	double speed;


};

class caRecover;
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<caRecover> pcaRecover;

class caRecover :
	public cAnimation
{
public:
	caRecover(void);
public:
	virtual ~caRecover(void);


	virtual int process(IDirect3DDevice9 *pDev);
	virtual int Init(pcAnimation me);

	virtual bool isOnScreen();

	pcCharacter pRecoverer;
	int Recover;

	double count;
	double speed;


};

