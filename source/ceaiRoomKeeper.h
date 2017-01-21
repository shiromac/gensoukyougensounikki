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

	virtual bool u_目標点作成(int& out_tgt_x,int& out_tgt_y);
	virtual bool u_目標免除(pcCharacter penemychara);

	virtual int Request(int phase);

};

class ceaiRoomKeeperNoMove
	:public ceaiRoomKeeper
{
public:
	ceaiRoomKeeperNoMove(void);
public:
	virtual ~ceaiRoomKeeperNoMove(void);

	virtual bool u_目標点作成(int& out_tgt_x,int& out_tgt_y){return true;};

};
