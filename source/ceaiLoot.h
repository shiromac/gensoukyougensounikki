#pragma once

#include "cEnemyAI.h"

class ceaiLoot
	:public cEnemyAI
{
public:
	ceaiLoot(void);
public:
	virtual ~ceaiLoot(void);

	virtual int Init(pcCharacter self);

	virtual int Move();
	virtual int Attack();



	virtual int Request(int phase);

};
