#pragma once
#include "ccontrol.h"
#include "utility/SpriteText.h"

#include <tstring_ph.h>
#include <boost/any.hpp>
#include <map>

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

	virtual int Action(IDirect3DDevice9 *pDev){return true;}// = 0;//luabindのために具体化

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

	virtual int Action(IDirect3DDevice9 *pDev){
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

	virtual int Action(IDirect3DDevice9 *pDev);

};

class cCommandDelegated;
typedef int(*cCommandDelegate)(cCommandDelegated& command);
class cCommandDelegated :
	public cCommand
{
public:
	cCommandDelegated(void){};
	cCommandDelegated(StyleString s):cCommand(s){};
	cCommandDelegated(StyleString s, cCommandDelegate delegate):cCommand(s){setDelegate(delegate);};
public:
	virtual ~cCommandDelegated(void){};

	virtual int Action(IDirect3DDevice9 *pDev);

	void setDelegate(const cCommandDelegate& delegate);

	std::map<tstring, boost::any> info;
private:
	cCommandDelegate _delegate;
};
