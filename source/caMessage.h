#pragma once
#include "cAnimation.h"
#include "utility/StyleString.h"

#include <vector>
#include <list>
#include <tstring_ph.h>


using namespace std;

class caMessage;
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<caMessage> pcaMessage;

class caMessage :
	public cAnimation
{
public:
	caMessage(void);
public:
	virtual ~caMessage(void);


	virtual int process(cRenderDevice *pDev);

	
	//î≠âŒÇµÇ‚Ç∑Ç¢Ç©ÅiWeekInvocationÇ≈î≠âŒÇ∑ÇÈÇ©Åj
	virtual int EasyInvocation(){return false;};//î≠âŒÇµÇ…Ç≠Ç¢


	virtual bool isOnScreen();

	StyleString Message;
};

