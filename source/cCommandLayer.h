#pragma once
#include "ccontrol.h"

#include "cGameWindow.h"
#include "cSelectWindow.h"

#include <vector>
using namespace std;

class cCommandLayer :
	public cControl
{
public:
	cCommandLayer(void);
public:
	virtual ~cCommandLayer(void);

	vector<cGameWindow*>	WindowList;

};
