#pragma once

#include "cEnemyAI.h"

class ceaiRoomKeeper
	:public cEnemyAI
{
public:
	ceaiRoomKeeper(void);
public:
	virtual ~ceaiRoomKeeper(void);

	virtual int Init(pcCharacter self);

	virtual int Move();
	virtual int Attack();

	virtual bool u_–Ú•W“_ì¬(int& out_tgt_x,int& out_tgt_y);
	virtual bool u_–Ú•W–Æœ(pcCharacter penemychara);

	virtual int Request(int phase);

};
