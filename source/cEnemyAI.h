#pragma once

#include "cCharacter.h"

#include "cLandform.h"

#include <vector>
#include <list>
#include <tstring_ph.h>

//AI関係
#define AI_KIND_ID_DEFAULT 0


#include <map>
using namespace std;

enum re_Request
{
	AI_REQUEST_MOVE,
	AI_REQUEST_ATTACK,
	AI_REQUEST_CANNOTACTION,
};
#define MODE_NOTHING -1
#define MODE_AVOID 3
#define MODE_ESCAPE 2
#define MODE_ATTACK 1
#define MODE_WALK 0
/*
enum enemy_phase
{
	ENEMY_PHASE_SINGLEACTION,
	ENEMY_PHASE_DOUBLEACTION,
	ENEMY_PHASE_TRIPLEACTION,
};
*/
class cEnemyAI
{
public:
	cEnemyAI(void);
public:
	virtual ~cEnemyAI(void);

	virtual int Init(pcCharacter self);

	int Move_Base();//return falseでAttackに変更

	int Attack_Base();

	virtual int Move();//return falseでAttackに変更

	virtual int Attack();

	//敵対
	virtual bool isOppose(pcCharacter pchara);

	virtual void addEnemy(pcCharacter pchara);
protected:
	wpcCharacter me_wpt;

	int SimpleActiveMove();
	int SimpleActiveAttack();
	int SimpleActiveRequest(int phase);

	int SimpleEscapeMove();
	int SimpleEscapeAttack();
	int SimpleEscapeRequest(int phase);

	vector<wpcCharacter> enemychara_;
public:
	pcCharacter me();


	virtual int Request(int phase);
	int Request_Base(int phase);
	
	int flagSpecialAttack;
	double valueSpecialAttack;
	int mode;

	int tgtLandX;
	int tgtLandY;

	//こっからAIユーティリティー関数
	virtual bool u_敵隣接判定();
	virtual bool u_敵隣接判定(pcCharacter penemychara);
	virtual bool u_敵直接攻撃(pcCharacter penemychara);
	virtual bool u_索敵();
	virtual bool u_敵サーチ_キャラ優先度優先(int& out_tgt_x,int& out_tgt_y);
	virtual bool u_敵サーチ_距離優先(int& out_tgt_x,int& out_tgt_y);
	virtual bool u_目標点作成(int& out_tgt_x,int& out_tgt_y);
	virtual bool u_目標点作成_最近接出口(int& out_tgt_x,int& out_tgt_y);
	virtual bool u_目標点に移動(int tgt_x,int tgt_y);
	virtual bool u_目標点から間合いを取る(int tgt_x,int tgt_y);//失敗でfalse
	virtual bool u_目標点の斜線確保(int tgt_x,int tgt_y);//失敗でfalse
	virtual bool u_目標点に移動迂回モード(int tgt_x,int tgt_y);
	virtual bool u_目標点に移動敵回避モード(int tgt_x,int tgt_y);
	virtual bool u_隣接攻撃通用判定(int aspect);
	virtual bool u_隣接移動通用判定(int aspect);
	virtual int u_目標点との距離(int tgt_x,int tgt_y);
	virtual bool u_目標免除(pcCharacter penemychara);


	virtual int u_攻撃優先度_昇順(pcCharacter penemychara);
};

class ceaiNoMotion
	:public cEnemyAI
{
public:
	ceaiNoMotion(void){};
public:
	virtual ~ceaiNoMotion(void){};

	virtual int Move(){return true;};
	virtual int Attack(){return true;};

	virtual int Request(int phase){return AI_REQUEST_MOVE;};

};


#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cEnemyAI> pcEnemyAI;



//AI用です
//pcEnemyAI g_GetAI(int id);
