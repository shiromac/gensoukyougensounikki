#pragma once
#include "cAnimation.h"
#include "cLandform.h"
#include "cCharacter.h"

//Attack‚ÆDefense

#include <vector>
#include <list>
#include <tstring_ph.h>


using namespace std;

#define ANIME_ATTACK_STEP 16

class caAttack :
	public cAnimation
{
public:
	caAttack(void);
public:
	virtual ~caAttack(void);


	virtual int process(IDirect3DDevice9 *pDev);
	virtual int Draw(IDirect3DDevice9 *pDev);
	virtual int Init(pcAnimation me);

	virtual bool isOnScreen();

	tstring effectfilename;
	IDirect3DTexture9 *p_Tex;
	int vi;
	int SE_playID;
	int SE_playcount;

	pcCharacter  pAttacker;
	pcLandform  pLand;
	int  attackaspect;
	int  attackdistance;

	double count;
	double speed;

protected:
	int placeX_;
	int placeY_;
};
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<caAttack> pcaAttack;



class caDefense :
	public cAnimation
{
public:
	caDefense(void);
public:
	virtual ~caDefense(void);


	virtual int process(IDirect3DDevice9 *pDev);
	virtual int Draw(IDirect3DDevice9 *pDev);

	virtual bool isOnScreen();

	tstring effectfilename;
	IDirect3DTexture9 *p_Tex;
	int vi;
	int SE_playID;
	int SE_playcount;

	pcCharacter  pDefenser;


	double count;
	double speed;


};
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<caDefense> pcaDefense;
