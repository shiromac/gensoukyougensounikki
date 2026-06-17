#pragma once
#include "ccontrol.h"
#include "utility/SpriteText.h"

#include <tstring_ph.h>


using namespace std;
class cDroping;
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cDroping> pcDroping;

class cCommand :
	public cControl
{
public:
	cCommand(void);
	cCommand(StyleString s);
public:
	virtual ~cCommand(void);

	StyleString caption;

	virtual int Action(cRenderDevice *pDev){return true;}// = 0;//luabindのために具体化

	virtual StyleString shortExplanationText(){return _T("");};

	virtual void resetCaption(){};
	//ショートカットに入れても良いかどうか
	virtual bool canIntoShortCut(){return false;};

	//内包落ち物対象リスト();
	virtual pcDroping includeObjectiveDroping(pcDroping objectpdrop){return pcDroping((cDroping*)NULL);};
};

class cCommandNull :
	public cCommand
{
public:
	cCommandNull(void){};
	cCommandNull(StyleString s){caption = s;};
public:
	virtual ~cCommandNull(void){};

	virtual int Action(cRenderDevice *pDev){
	return true;};

};

class cCommandCancel :
	public cCommand
{
public:
	cCommandCancel(void){};
	cCommandCancel(StyleString s){caption = s;};
public:
	virtual ~cCommandCancel(void){};

	virtual int Action(cRenderDevice *pDev);

};

