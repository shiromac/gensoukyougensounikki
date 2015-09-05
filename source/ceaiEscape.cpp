#include "stdafx.h"
#include "ceaiEscape.h"

#define MEM_TGTLANDX 0
#define MEM_TGTLANDY 1
#define MEM_MOVEMODE 2




ceaiEscape::ceaiEscape(void)
{

}

ceaiEscape::~ceaiEscape(void)
{

}

int ceaiEscape::Init(pcCharacter self)
{
	cEnemyAI::Init(self);

	mode = MODE_NOTHING;

	return true;
}

int ceaiEscape::Request(int phase)
{
	return SimpleEscapeRequest(phase);
}

int ceaiEscape::Move()
{
	return SimpleEscapeMove();
}

int ceaiEscape::Attack()
{
	return SimpleEscapeAttack();
}