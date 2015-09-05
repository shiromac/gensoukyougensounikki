#pragma once

#include <queue>

class cDrama;
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

typedef boost::shared_ptr<cDrama> pcDrama;
typedef boost::weak_ptr<cDrama> wpcDrama;

class cDramaManager
{
public:
	cDramaManager();
public:
	virtual ~cDramaManager(void);

	bool stackDrama(pcDrama& pDrama);

	void doDrama();
	bool emptyDrama();
private:

	std::queue<pcDrama> dramaQueue_;
};