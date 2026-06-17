#pragma once
#include "cAnimation.h"


#include <vector>
#include <list>
#include <tstring_ph.h>


using namespace std;


class caSoundPlay :
	public cAnimation
{
public:
	caSoundPlay(void);
public:
	virtual ~caSoundPlay(void);


	virtual int process(cRenderDevice *pDev);

	
	//î≠âŒÇµÇ‚Ç∑Ç¢Ç©ÅiWeekInvocationÇ≈î≠âŒÇ∑ÇÈÇ©Åj
	virtual int EasyInvocation(){return false;};//î≠âŒÇµÇ…Ç≠Ç¢


	virtual bool isOnScreen();

	int placeX;
	int placeY;

	int play_ID;
	
	double loudness;


	tstring intro;
	tstring roop;
};
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<caSoundPlay> pcaSoundPlay;
