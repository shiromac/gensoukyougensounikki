#pragma once
#include "cAnimation.h"
#include "utility/StyleString.h"

#include <vector>
#include <list>
#include <string>
using namespace std;



class caMassage :
	public cAnimation
{
public:
	caMassage(void);
public:
	virtual ~caMassage(void);


	virtual int process(IDirect3DDevice9 *pDev);

	virtual bool isOnScreen();

	StyleString massage;
};

