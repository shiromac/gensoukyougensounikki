#include "stdafx.h"
#include "ceaiNegative.h"

#define MEM_TGTLANDX 0
#define MEM_TGTLANDY 1
#define MEM_MOVEMODE 2

#define MODE_NOTHING -1
#define MODE_ATTACK 1
#define MODE_WALK 0


ceaiNegative::ceaiNegative(void)
{

}

ceaiNegative::~ceaiNegative(void)
{

}

int ceaiNegative::Init(pcCharacter self)
{
	cEnemyAI::Init(self);

	mode = MODE_NOTHING;

	return true;
}

int ceaiNegative::Request(int phase)
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
		
		if(u_敵サーチ_距離優先(tgtLandX,tgtLandY))
		{//成功
			mode = MODE_ATTACK;
		}
		else if(me()->Forse == CHARACTER_FORSE_FRIEND) {
			//主人公についていく
			u_味方主人公サーチ(tgtLandX,tgtLandY);
		}
	}
	else if(mode == MODE_ATTACK)//追尾モード
	{
		if(tgtLandX == me()->placeX 
		&& tgtLandY == me()->placeY)
		{//目標点に到達
			u_目標点作成(tgtLandX,tgtLandY);
		}

		if(u_敵サーチ_距離優先(tgtLandX,tgtLandY))
		{//成功
			mode = MODE_ATTACK;
		}
		else
		{//失敗
			//u_目標点作成(tgtLandX,tgtLandY);
			mode = MODE_WALK;
		}
	}
	else
	{
		u_目標点作成(tgtLandX,tgtLandY);
		mode = MODE_WALK;
	}

	//cMap* pmap = &sg_pDungeonSystem->Map();
	if(mode == MODE_ATTACK)//追尾モード
	{//特殊攻撃選択
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

int ceaiNegative::Move()
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


		cValiableField val;
		if(u_目標点との距離(tgtLandX,tgtLandY) <= 1)
		{
			if(u_目標点から間合いを取る(tgtLandX,tgtLandY))
			{//成功
				return true;
			}
			else
			{//失敗
				return false;
			}
		
		}
		else if(u_目標点との距離(tgtLandX,tgtLandY) > me()->AI最大間合い(val))
		{//間合いより遠い
			
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
		else
		{
			if(u_目標点の斜線確保(tgtLandX,tgtLandY))
			{//成功
				return true;
			}
			else
			{//失敗
				return false;
			}
		
		}
	
	}


	return true;
}

int ceaiNegative::Attack()
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

	multimap<int,pcCharacter> sortMap;
	vector<pcCharacter> visibleCharalist;

	sg_pDungeonSystem->キャラクター目視可範囲キャラリスト(me(),visibleCharalist);
	
	int i;
	for(i=0;i<visibleCharalist.size();i++)
	{
		if(u_敵隣接判定(visibleCharalist[i]))
		{
			sortMap.insert(pair<int,pcCharacter>(u_攻撃優先度_昇順(visibleCharalist[i]),visibleCharalist[i]));
		}
	}

	multimap<int,pcCharacter>::iterator itr = sortMap.begin();
	if(sortMap.size())
	{
		for(;itr != sortMap.end();itr++)
		{
			u_敵直接攻撃(itr->second);
			return true;
		}
	}
	return false;



}