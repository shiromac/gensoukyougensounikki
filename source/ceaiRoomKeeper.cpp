#include "stdafx.h"
#include "ceaiRoomKeeper.h"
#include "FindUtility.h"

#define MEM_TGTLANDX 0
#define MEM_TGTLANDY 1
#define MEM_MOVEMODE 2




ceaiRoomKeeper::ceaiRoomKeeper(void)
{

}

ceaiRoomKeeper::~ceaiRoomKeeper(void)
{

}

int ceaiRoomKeeper::Init(pcCharacter self)
{
	cEnemyAI::Init(self);

	mode = MODE_NOTHING;

	return true;
}

int ceaiRoomKeeper::Request(int phase)
{
	return SimpleActiveRequest(phase);
}

int ceaiRoomKeeper::Move()
{
	pcLandform pland = sg_pDungeonSystem->Map().Land(tgtLandX,tgtLandY);
	if(pland && pland->RoomIndex == -1) {
		return false;
	}
	return SimpleActiveMove();
}

int ceaiRoomKeeper::Attack()
{
	return SimpleActiveAttack();
}
bool ceaiRoomKeeper::u_目標点作成(int& out_tgt_x,int& out_tgt_y)
{
	pcLandform pland = FindUtility::同部屋_ランダム地形検索(me()->足元地形());
	if(pland){
		tgtLandX = pland->placeX;
		tgtLandY = pland->placeY;
	}
	return true;
}
bool ceaiRoomKeeper::u_目標免除(pcCharacter penemychara)
{
	return (penemychara->足元地形()->RoomIndex == -1);
}

//
ceaiRoomKeeperNoMove::ceaiRoomKeeperNoMove(void)
{

}

ceaiRoomKeeperNoMove::~ceaiRoomKeeperNoMove(void)
{

}
