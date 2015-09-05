#include "stdafx.h"
#include "ceaiActive.h"

#define MEM_TGTLANDX 0
#define MEM_TGTLANDY 1
#define MEM_MOVEMODE 2




ceaiActive::ceaiActive(void)
{

}

ceaiActive::~ceaiActive(void)
{

}

int ceaiActive::Init(pcCharacter self)
{
	cEnemyAI::Init(self);

	mode = MODE_NOTHING;

	return true;
}

int ceaiActive::Request(int phase)
{
	return SimpleActiveRequest(phase);
}

int ceaiActive::Move()
{
	return SimpleActiveMove();
}

int ceaiActive::Attack()
{
	return SimpleActiveAttack();
}