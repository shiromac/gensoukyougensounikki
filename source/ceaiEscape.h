#pragma once

#include "cEnemyAI.h"

class ceaiEscape
	:public cEnemyAI
{
public:
	ceaiEscape(void);
public:
	virtual ~ceaiEscape(void);

	virtual int Init(pcCharacter self);

	virtual int Move();
	virtual int Attack();



	virtual int Request(int phase);

};
