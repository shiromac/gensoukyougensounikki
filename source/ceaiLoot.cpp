#include "stdafx.h"
#include "ceaiLoot.h"

#define MEM_TGTLANDX 0
#define MEM_TGTLANDY 1
#define MEM_MOVEMODE 2

#define MODE_NOTHING -1
#define MODE_ATTACK 1
#define MODE_WALK 0


ceaiLoot::ceaiLoot(void)
{

}

ceaiLoot::~ceaiLoot(void)
{

}

int ceaiLoot::Init(pcCharacter self)
{
	cEnemyAI::Init(self);

	mode = MODE_NOTHING;

	return true;
}

int ceaiLoot::Request(int phase)
{
	if(mode == MODE_NOTHING)
	{
		u_目標点作成(tgtLandX,tgtLandY);
		mode = MODE_WALK;
	}
	if(mode == MODE_WALK)//徘徊モード
	{
		if(tgtLandX == me()->placeX 
		&& tgtLandY == me()->placeY)
		{//目標点に到達
			u_目標点作成(tgtLandX,tgtLandY);
		}

		cValiableField val;
		pcLandform pland;
		if((pland = me()->AIお気に入り地形(val)) != NULL)
		{//お気に入り地形発見
			tgtLandX = pland->placeX;
			tgtLandY = pland->placeY;

			mode = MODE_ATTACK;
		}
		else
		{
			if(u_敵サーチ_キャラ優先度優先(tgtLandX,tgtLandY))
			{//成功
				mode = MODE_ATTACK;
			}
		}
	}
	else if(mode == MODE_ATTACK)//追尾モード
	{
		if(tgtLandX == me()->placeX 
		&& tgtLandY == me()->placeY)
		{//目標点に到達
			u_目標点作成(tgtLandX,tgtLandY);
		}
		
		cValiableField val;
		pcLandform pland;
		if((pland = me()->AIお気に入り地形(val)) != NULL)
		{//お気に入り地形発見
			tgtLandX = pland->placeX;
			tgtLandY = pland->placeY;

			mode = MODE_ATTACK;
		}
		else
		{
			if(u_敵サーチ_キャラ優先度優先(tgtLandX,tgtLandY))
			{//成功
				mode = MODE_ATTACK;
			}
			else
			{//失敗
				//u_目標点作成(tgtLandX,tgtLandY);
				mode = MODE_WALK;
			}
		}
	}
	else
	{
		u_目標点作成(tgtLandX,tgtLandY);
		mode = MODE_WALK;
	}

	//cMap* pmap = &sg_pDungeonSystem->Map();
	if(mode == MODE_ATTACK)//追尾モード
	{
		cValiableField val;
		val.doubles.dim(変数_フェイズ) = phase;
		val.lands.dim(変数_攻撃場所) = sg_pDungeonSystem->Map().Land(tgtLandX,tgtLandY);
		val.doubles.dim(変数_汎用実数) = 0.0;
		if( me()->AI特殊攻撃選択pre(val) )
		{
			flagSpecialAttack = 1;
			valueSpecialAttack = val.doubles.val(変数_汎用実数);
			return AI_REQUEST_ATTACK;
		}
		else
		{
			valueSpecialAttack = val.doubles.val(変数_汎用実数);
			flagSpecialAttack = 0;
		}
	}

	return AI_REQUEST_MOVE;
}

int ceaiLoot::Move()
{


	if(mode == MODE_WALK)//徘徊モード
	{

		if(u_目標点に移動(tgtLandX,tgtLandY))
		{//移動成功
			return true;
		}
		else
		{//失敗
			u_目標点作成(tgtLandX,tgtLandY);
			return false;
		}

	}
	else if(mode == MODE_ATTACK)//追尾モード
	{


	
		if(u_目標点に移動(tgtLandX,tgtLandY))
		{//成功
			return true;
		}
		else
		{//失敗
			if(u_目標点に移動迂回モード(tgtLandX,tgtLandY))
			{
				u_目標点作成(tgtLandX,tgtLandY);
				return true;
			}
			else
			{
				u_目標点作成(tgtLandX,tgtLandY);
				return false;
			}
		}
	
	}


	return true;
}

int ceaiLoot::Attack()
{
	if(flagSpecialAttack)
	{
		cValiableField val;
		val.lands.dim(変数_攻撃場所) = sg_pDungeonSystem->Map().Land(tgtLandX,tgtLandY);
		val.doubles.dim(変数_汎用実数) = valueSpecialAttack;
		if(me()->特殊攻撃(val))
		{

			return true;
		}
	}

	pcCharacter pchara = sg_pDungeonSystem->Map().Land(tgtLandX,tgtLandY)->pOnChar;

	if(pchara != NULL)
	{
		if(u_敵隣接判定(pchara))
		{
			u_敵直接攻撃(pchara);
			return true;
		}
	}

	return false;

}