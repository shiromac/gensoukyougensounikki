#pragma once

#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>

class cDrama;
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

typedef boost::shared_ptr<cDrama> pcDrama;
typedef boost::weak_ptr<cDrama> wpcDrama;

class cDrama
{
public:
	cDrama(luabind::object& dramaThread);
public:
	virtual ~cDrama(void);

	bool doDrama();//true‚Ì‚Æ‚«I—¹

	bool isError();
private:
	enum state{
		state_error = -1,
		state_ready = 0,
		state_doing,
		state_finished,
	};
	state state_; 
	luabind::object dramaThread_;

	bool dramaThread_resume();
};