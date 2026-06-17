#pragma once
#include "cAnimation.h"


#include <vector>
#include <list>
#include <tstring_ph.h>

#include <luabind/object.hpp>

using namespace std;



class caFunction :
	public cAnimation
{
public:
	caFunction(void);
public:
	virtual ~caFunction(void);


	virtual int process(cRenderDevice *pDev);
	virtual int Init();

	//virtual bool isOnScreen();

	int wait;


	//à¯êîÇÇ∆ÇÁÇ»Ç¢ä÷êî
	void (*pFunc)();

protected:
	int count;
};
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<caFunction> pcaFunction;


class caPAnimeAdd :
	public cAnimation
{
public:
	caPAnimeAdd(void);
	caPAnimeAdd(bool critical);
public:
	virtual ~caPAnimeAdd(void);


	virtual int process(cRenderDevice *pDev);
	virtual int Init();

	//virtual bool isOnScreen();

	int wait;

	pcAnimation paddanime;


	//à¯êîÇÇ∆ÇÁÇ»Ç¢ä÷êî
	void (*pFunc)();

protected:
	bool _critical;
	int count;
};
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<caPAnimeAdd> pcaPAnimeAdd;


class caLuaFunction :
	public cAnimation
{
public:
	caLuaFunction(luabind::object& function, int wait);
	caLuaFunction(luabind::object& function, luabind::object& val1, int wait);
	caLuaFunction(luabind::object& function, luabind::object& val1, luabind::object& val2, int wait);
	caLuaFunction(luabind::object& function, luabind::object& val1, luabind::object& val2, luabind::object& val3, int wait);
	caLuaFunction(luabind::object& function, luabind::object& val1, luabind::object& val2, luabind::object& val3, luabind::object& val4, int wait);
	caLuaFunction(luabind::object& function, luabind::object& val1, luabind::object& val2, luabind::object& val3, luabind::object& val4,
		luabind::object& val5, int wait);
	caLuaFunction(luabind::object& function, luabind::object& val1, luabind::object& val2, luabind::object& val3, luabind::object& val4,
		luabind::object& val5, luabind::object& val6, int wait);
	caLuaFunction(luabind::object& function, luabind::object& val1, luabind::object& val2, luabind::object& val3, luabind::object& val4,
		luabind::object& val5, luabind::object& val6, luabind::object& val7, int wait);
	caLuaFunction(luabind::object& function, luabind::object& val1, luabind::object& val2, luabind::object& val3, luabind::object& val4,
		luabind::object& val5, luabind::object& val6, luabind::object& val7, luabind::object& val8, int wait);

private:
	void baseInitation(luabind::object& function, int wait);
public:
	virtual ~caLuaFunction(void);


	virtual int process(cRenderDevice *pDev);
	virtual int Init();

	//virtual bool isOnScreen();


protected:
	int count_;

	int wait_;

	//à¯êîÇÇ∆ÇÁÇ»Ç¢ä÷êî
	luabind::object function_;

	vector<luabind::object> valiables_;
};
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<caLuaFunction> pcaLuaFunction;
