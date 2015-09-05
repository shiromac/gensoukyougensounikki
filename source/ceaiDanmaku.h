#pragma once

#include "cEnemyAI.h"

class ceaiDanmaku
	:public cEnemyAI
{
public:
	ceaiDanmaku(void);
public:
	virtual ~ceaiDanmaku(void);

	virtual int Init(pcCharacter self);

	virtual int Move();
	virtual int Attack();



	virtual int Request(int phase);

};

class ceaiDanmakuTuibi
	:public cEnemyAI
{
public:
	ceaiDanmakuTuibi(void);
public:
	virtual ~ceaiDanmakuTuibi(void);

	virtual int Init(pcCharacter self);

	virtual int Move();
	virtual int Attack();



	virtual int Request(int phase);

};
