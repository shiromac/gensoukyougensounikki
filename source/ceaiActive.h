#pragma once

#include "cEnemyAI.h"

class ceaiActive
	:public cEnemyAI
{
public:
	ceaiActive(void);
public:
	virtual ~ceaiActive(void);

	virtual int Init(pcCharacter self);

	virtual int Move();
	virtual int Attack();



	virtual int Request(int phase);

};
