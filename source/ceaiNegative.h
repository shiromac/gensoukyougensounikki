#pragma once

#include "cEnemyAI.h"

class ceaiNegative
	:public cEnemyAI
{
public:
	ceaiNegative(void);
public:
	virtual ~ceaiNegative(void);

	virtual int Init(pcCharacter self);

	virtual int Move();
	virtual int Attack();



	virtual int Request(int phase);

};
