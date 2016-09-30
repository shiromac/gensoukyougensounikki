#include "stdafx.h"
#include "cDungeonSystem.h"

#include "cEnemyAI.h"

#include "ceaiActive.h"
//AI用です・・
/*
pcEnemyAI g_GetAI(int id)
{
	switch(id)
	{
	case AI_KIND_ID_DEFAULT:
		return pcEnemyAI(new ceaiActive);
	default:
		return pcEnemyAI(new cEnemyAI);
	}
}
*/


cEnemyAI::cEnemyAI(void)
{
	flagSpecialAttack = 0;
	valueSpecialAttack = 0.0;
}

cEnemyAI::~cEnemyAI(void)
{

}

int cEnemyAI::Init(pcCharacter self)
{
	me_wpt = wpcCharacter(self);

	mode = MODE_NOTHING;

	return true;
}

int cEnemyAI::Request(int phase)
{
	return AI_REQUEST_MOVE;
}
int cEnemyAI::Request_Base(int phase)
{
	if(me()->Condition.混乱状態である())
	{
		return AI_REQUEST_MOVE;
	}
	else if(me()->Condition.封印状態() || me()->Condition.狂乱状態())
	{
		return SimpleActiveRequest(phase);
	}
	else if(me()->Condition.臆病状態())
	{
		return SimpleEscapeRequest(phase);
	}
	else
		return Request(phase);
}

pcCharacter cEnemyAI::me()
{
	return me_wpt.lock();
}

//return falseでAttackに変更
int cEnemyAI::Move_Base()
{
	if(me()->Condition.混乱状態である())
	{
		return sg_pDungeonSystem->移動要請(me(),random()*8);
	}
	else if(me()->Condition.封印状態() || me()->Condition.狂乱状態())
	{
		return SimpleActiveMove();
	}
	else if(me()->Condition.臆病状態())
	{
		return SimpleEscapeMove();
	}
	else
	{
		
		cValiableField valiable;
		valiable.lands.dim(変数_移動場所) = sg_pDungeonSystem->Map().Land(tgtLandX,tgtLandY);
		if(!me()->AI移動許可(valiable))
		{
			return false;
		}

		return Move();
	}
}
int cEnemyAI::Move()
{
	return sg_pDungeonSystem->移動要請(me(),random()*8);
}
int cEnemyAI::Attack_Base()
{
	if(me()->Condition.混乱状態である())
	{
		if( (sg_pDungeonSystem->キャラ前地形(me(),1)->pOnChar) && sg_pDungeonSystem->isCanNeighberAttack(me(),me()->aspect,1,me()->通常攻撃_壁斜めすり抜け(),me()->通常攻撃_壁すり抜け()) )
		{
			sg_pDungeonSystem->直接攻撃要請_強制混乱なし(me());
		}
	}
	else if(me()->Condition.封印状態() || me()->Condition.狂乱状態())
	{
		return SimpleActiveAttack();
	}
	else if(me()->Condition.臆病状態())
	{
		return SimpleEscapeAttack();
	}
	else
	{
		return Attack();
	}
	return true;
}
int cEnemyAI::Attack()
{
	if( !(sg_pDungeonSystem->キャラ前地形(me(),1)->iswall()) )
	{
		sg_pDungeonSystem->直接攻撃要請(me());
	}
	return true;
}
bool cEnemyAI::isOppose(pcCharacter pchara)
{
	pcCharacter penemy;
	int i,size = enemychara_.size();
	for(i=0;i<size;i++)
	{
		penemy = enemychara_[i].lock();
		if(penemy == NULL) continue;
		if(penemy == pchara)
		{
			return true;
		}
	}
	return false;
}
void cEnemyAI::addEnemy(pcCharacter pchara)
{
	enemychara_.push_back(wpcCharacter(pchara));
}
int cEnemyAI::SimpleActiveMove()
{
	if(u_敵隣接判定())
	{
		return false;
	}

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
	else
	{
		if(u_目標点に移動(tgtLandX,tgtLandY))
		{//成功
			return true;
		}
	}

	return true;

}
int cEnemyAI::SimpleActiveAttack()
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
int cEnemyAI::SimpleActiveRequest(int phase)
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
			else if(me()->Forse == CHARACTER_FORSE_FRIEND) {
				//主人公についていく
				u_味方主人公サーチ(tgtLandX,tgtLandY);
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
	//if(mode == MODE_ATTACK)//追尾モード
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

	if(u_敵隣接判定())
	{
		return AI_REQUEST_ATTACK;
	}


	return AI_REQUEST_MOVE;
}


int cEnemyAI::SimpleEscapeMove()
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
	else if(mode == MODE_ESCAPE)//逃亡モード
	{
		if(u_目標点に移動敵回避モード(tgtLandX,tgtLandY))
		{//成功
			return true;
		}
		else
		{//失敗
			mode = MODE_ATTACK;
			return false;
		}
	}
	else if(mode == MODE_AVOID)//逃亡モード
	{
		if(u_目標点から間合いを取る(tgtLandX,tgtLandY))
		{//成功
			return true;
		}
		else
		{//失敗
			mode = MODE_ATTACK;
			return false;
		}
		
	}

	return true;
}
int cEnemyAI::SimpleEscapeAttack()
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
int cEnemyAI::SimpleEscapeRequest(int phase)
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

			mode = MODE_ESCAPE;
		}
		else if(u_索敵())
		{//成功
			mode = MODE_ESCAPE;
			if(u_目標点作成_最近接出口(tgtLandX,tgtLandY))
			{
				
			}
			else
			{//出口がない
				if(u_敵サーチ_距離優先(tgtLandX,tgtLandY))
				{
					mode = MODE_AVOID;
				}
			}
		}
	}
	else if(mode == MODE_ATTACK)//追尾モード
	{
		u_目標点作成_最近接出口(tgtLandX,tgtLandY);
		if(u_索敵())
		{//成功
			mode = MODE_AVOID;
		}
		else
		{
			mode = MODE_WALK;
		}
	}
	else if(mode == MODE_ESCAPE)//逃亡モード
	{
		if(tgtLandX == me()->placeX 
		&& tgtLandY == me()->placeY)
		{//目標点に到達
			if(u_目標点作成_最近接出口(tgtLandX,tgtLandY))
			{
				
			}
			else
			{//出口がない
				if(u_敵サーチ_距離優先(tgtLandX,tgtLandY))
				{
					mode = MODE_AVOID;
				}
				else
				{
				
				}
			}
		}
		cValiableField val;
		pcLandform pland;
		if((pland = me()->AIお気に入り地形(val)) != NULL)
		{//お気に入り地形発見
			tgtLandX = pland->placeX;
			tgtLandY = pland->placeY;

			mode = MODE_ESCAPE;
		}
		else if(u_索敵())
		{//成功
			//mode = MODE_ESCAPE;
		}
		else
		{//失敗
			u_目標点作成(tgtLandX,tgtLandY);
			mode = MODE_WALK;
		}
	}
	else if(mode == MODE_AVOID)//逃亡モード
	{
		cValiableField val;
		pcLandform pland;
		if((pland = me()->AIお気に入り地形(val)) != NULL)
		{//お気に入り地形発見
			tgtLandX = pland->placeX;
			tgtLandY = pland->placeY;

			mode = MODE_ESCAPE;
		}
		else if(u_敵サーチ_距離優先(tgtLandX,tgtLandY))
		{
			mode = MODE_AVOID;
		}
		else
		{//失敗
			mode = MODE_WALK;
		}
	}

	//cMap* pmap = &sg_pDungeonSystem->Map();
	//if(mode == MODE_ATTACK)//追尾モード
	{//特殊攻撃選択
		cValiableField val;
		val.doubles.dim(変数_フェイズ) = phase;
		val.lands.dim(変数_攻撃場所) = sg_pDungeonSystem->Map().Land(tgtLandX,tgtLandY);
		val.doubles.dim(変数_汎用実数) = 0.0;
		if( me()->AI特殊攻撃選択pre(val) )
		{
			flagSpecialAttack = 1;
			return AI_REQUEST_ATTACK;
		}
		else
		{
			flagSpecialAttack = 0;
		}
	}

	return AI_REQUEST_MOVE;
}



//少なくともひとりの敵に隣接しているのが見える//攻撃できる位置
bool cEnemyAI::u_敵隣接判定()
{
	cMap* pmap = &sg_pDungeonSystem->Map();

	cCoordinate coo,mylandcoo;

	int i;
	for(i=0;i<8;i++)
	{
		coo.SetAspect(i);
		coo.x += me()->placeX;
		coo.y += me()->placeY;
		mylandcoo.x = me()->placeX;
		mylandcoo.y = me()->placeY;

		if(pmap->Land(coo)->pOnChar && 
			(sg_pDungeonSystem->キャラクター敵対判定(me(),pmap->Land(coo)->pOnChar)) &&
			!pmap->Land(coo)->pOnChar->Condition.透明状態である() &&
			pmap->isOnSight(pmap->Land(mylandcoo),pmap->Land(coo)) &&
			u_隣接攻撃通用判定(i))
		{//敵
			if(pmap->Land(coo)->pOnChar && u_目標免除(pmap->Land(coo)->pOnChar))
			{
				continue;
			}
			return true;
		}
	}

	return false;
}
bool cEnemyAI::u_敵直接攻撃(pcCharacter penemychara)
{
	cCoordinate coo;
	coo.x = penemychara->placeX - me()->placeX;
	coo.y = penemychara->placeY - me()->placeY;
	
	sg_pDungeonSystem->方向転換要請(me(), coo.GetAspect());
	/*
	if(me()->isSpecialAttack())
	{
		me()->特殊攻撃();
	}
	else
	{
		me()->通常攻撃();
	}
	*/
	me()->通常攻撃();
	return true;
}
bool cEnemyAI::u_敵隣接判定(pcCharacter penemychara)
{
	cCoordinate coo;
	coo.x = penemychara->placeX - me()->placeX;
	coo.y = penemychara->placeY - me()->placeY;
	if(coo.dif() == 1
	&& (sg_pDungeonSystem->キャラクター敵対判定(me(),penemychara)))
	{
		return u_隣接攻撃通用判定(coo.GetAspect());
	}
	return false;
}


bool cEnemyAI::u_隣接攻撃通用判定(int aspect)
{
	return sg_pDungeonSystem->isCanNeighberAttack(me(),aspect,1,me()->通常攻撃_壁斜めすり抜け(),me()->通常攻撃_壁すり抜け());
}
bool cEnemyAI::u_隣接移動通用判定(int aspect)
{
	return sg_pDungeonSystem->隣接地形移動可(me(),aspect);
}
int cEnemyAI::u_目標点との距離(int tgt_x,int tgt_y)
{
	cCoordinate coo,mep;

	mep.x = me()->placeX;
	mep.y = me()->placeY;
	coo.x = tgt_x;
	coo.y = tgt_y;

	return (mep-coo).dif();
}

bool cEnemyAI::u_目標点に移動(int tgt_x,int tgt_y)
{
	//cMap* pmap = &sg_pDungeonSystem->Map();
	multimap<int,int> sortMap;
	cCoordinate coo,mep,aspectcoo;
	int aspect;
	mep.x = me()->placeX;
	mep.y = me()->placeY;
	coo.x = tgt_x;
	coo.y = tgt_y;

	aspectcoo.x = ((coo-mep).x)?(coo-mep).x/abs((coo-mep).x):0;
	aspectcoo.y = ((coo-mep).y)?(coo-mep).y/abs((coo-mep).y):0;

	if(aspectcoo.dif() == 0) return false;
	aspect = aspectcoo.GetAspect();
	if(sg_pDungeonSystem->隣接地形移動可(me(),aspect))
	{
		sg_pDungeonSystem->移動要請(me(),aspect);
		return true;
	}
	int i;
	cCoordinate relative;
	for(i=0;i<8;i++)
	{
		aspectcoo.SetAspect(i);
		relative = coo-(mep+aspectcoo);
		int value_minus = 0;
		if(relative.dif() > (coo-mep).dif())continue;
		if(!u_隣接移動通用判定(i))continue;
		value_minus += relative.dif() * 128*128;
		value_minus += (abs(relative.x)+abs(relative.y)) * 128;
		value_minus += aspect_AngleAspect(me()->aspect,i);
		sortMap.insert(pair<int,int>(value_minus,i));
	}

	multimap<int,int>::iterator itr = sortMap.begin();
	if(!sortMap.empty())
	{
		for(;itr != sortMap.end();itr++)
		{
			if(sg_pDungeonSystem->隣接地形移動可(me(),itr->second))
			{
				if(sg_pDungeonSystem->移動要請(me(),itr->second))
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool cEnemyAI::u_目標点から間合いを取る(int tgt_x,int tgt_y)
{
	//cMap* pmap = &sg_pDungeonSystem->Map();
	multimap<int,int> sortMap;
	cCoordinate coo,mep,aspectcoo;
	int aspect;
	mep.x = me()->placeX;
	mep.y = me()->placeY;
	coo.x = tgt_x;
	coo.y = tgt_y;

	aspectcoo.x = ((coo-mep).x)?(coo-mep).x/abs((coo-mep).x):0;
	aspectcoo.y = ((coo-mep).y)?(coo-mep).y/abs((coo-mep).y):0;
	
	int i;
	cCoordinate relative;
	for(i=0;i<8;i++)
	{
		aspectcoo.SetAspect(i);
		relative = coo-(mep+aspectcoo);
		int value_minus = 0;
		if(relative.dif() <= 1 ) continue;
		if(!u_隣接移動通用判定(i))continue;
		value_minus += -relative.dif() * 128*128;
		value_minus += (abs(relative.x)+abs(relative.y)) * 128;
		value_minus += aspect_AngleAspect(me()->aspect,i);
		sortMap.insert(pair<int,int>(value_minus,i));
	}

	multimap<int,int>::iterator itr = sortMap.begin();
	if(!sortMap.empty())
	{
		for(;itr != sortMap.end();itr++)
		{
			if(sg_pDungeonSystem->隣接地形移動可(me(),itr->second))
			{
				sg_pDungeonSystem->移動要請(me(),itr->second);
				return true;
			}
		}
	}
	return false;
}
bool cEnemyAI::u_目標点の斜線確保(int tgt_x,int tgt_y)
{
	//cMap* pmap = &sg_pDungeonSystem->Map();
	multimap<int,int> sortMap;
	cCoordinate coo,mep,aspectcoo;
	int aspect;
	mep.x = me()->placeX;
	mep.y = me()->placeY;
	coo.x = tgt_x;
	coo.y = tgt_y;

	aspectcoo.x = ((coo-mep).x)?(coo-mep).x/abs((coo-mep).x):0;
	aspectcoo.y = ((coo-mep).y)?(coo-mep).y/abs((coo-mep).y):0;
	
	int i;
	cCoordinate relative;
	for(i=0;i<8;i++)
	{
		aspectcoo.SetAspect(i);
		relative = coo-(mep+aspectcoo);
		if( relative.dif() <= 1 ) continue;
		if(!u_隣接移動通用判定(i))continue;
		int value_minus = 0;
		value_minus += relative.dif() * 128;
		value_minus += min(min(abs(relative.x),abs(relative.y)),min(abs(relative.x-relative.y),abs(relative.x+relative.y))) * 128*128;
		value_minus += aspect_AngleAspect(me()->aspect,i);
		sortMap.insert(pair<int,int>(value_minus,i));
	}

	multimap<int,int>::iterator itr = sortMap.begin();
	if(!sortMap.empty())
	{
		for(;itr != sortMap.end();itr++)
		{
			if(sg_pDungeonSystem->隣接地形移動可(me(),itr->second))
			{
				sg_pDungeonSystem->移動要請(me(),itr->second);
				return true;
			}
		}
	}
	return false;
}
bool cEnemyAI::u_目標点に移動迂回モード(int tgt_x,int tgt_y)
{
	//cMap* pmap = &sg_pDungeonSystem->Map();
	multimap<int,int> sortMap;
	cCoordinate coo,mep,aspectcoo;
	int aspect;
	mep.x = me()->placeX;
	mep.y = me()->placeY;
	coo.x = tgt_x;
	coo.y = tgt_y;

	aspectcoo.x = ((coo-mep).x)?(coo-mep).x/abs((coo-mep).x):0;
	aspectcoo.y = ((coo-mep).y)?(coo-mep).y/abs((coo-mep).y):0;

	if(aspectcoo.dif() == 0) return false;
	aspect = aspectcoo.GetAspect();
	if(sg_pDungeonSystem->隣接地形移動可(me(),aspect))
	{
		sg_pDungeonSystem->移動要請(me(),aspect);
		return true;
	}
	
	int i;
	for(i=0;i<8;i++)
	{
		aspectcoo.SetAspect(i);
		int value_minus = 0;
		value_minus += (coo-(mep+aspectcoo)).dif() * 128;
		value_minus += aspect_AngleAspect(me()->aspect,i);
		sortMap.insert(pair<int,int>(value_minus,i));
	}

	multimap<int,int>::iterator itr = sortMap.begin();
	if(sortMap.size())
	{
		for(;itr != sortMap.end();itr++)
		{
			if(sg_pDungeonSystem->隣接地形移動可(me(),itr->second))
			{
				sg_pDungeonSystem->移動要請(me(),itr->second);
				return true;
			}
		}
	}
	return false;

}
bool cEnemyAI::u_目標点に移動敵回避モード(int tgt_x,int tgt_y)
{
	//cMap* pmap = &sg_pDungeonSystem->Map();
	multimap<int,int> sortMap;
	cCoordinate coo,mep,aspectcoo;
	int aspect;
	mep.x = me()->placeX;
	mep.y = me()->placeY;
	coo.x = tgt_x;
	coo.y = tgt_y;

	aspectcoo.x = ((coo-mep).x)?(coo-mep).x/abs((coo-mep).x):0;
	aspectcoo.y = ((coo-mep).y)?(coo-mep).y/abs((coo-mep).y):0;
	
	vector<pcCharacter> visCharaList;
	sg_pDungeonSystem->キャラクター目視可範囲キャラリスト(me(),visCharaList);
	
	int i,k;
	cCoordinate relative,enemycoo;
	for(i=0;i<8;i++)
	{
		aspectcoo.SetAspect(i);
		relative = coo-(mep+aspectcoo);
		int value_minus = 0;
		if(!u_隣接移動通用判定(i)) continue;
		value_minus += relative.dif() * 128;
		for(k=0;k<visCharaList.size();k++)
		{
			if(sg_pDungeonSystem->キャラクター敵対判定(me(),visCharaList[k]))
			{
				enemycoo.Set(visCharaList[k]->placeX,visCharaList[k]->placeY);
				value_minus += 128*128 * ((mep+aspectcoo - enemycoo).dif() <= 1);
			}
		}
		
		value_minus += aspect_AngleAspect(me()->aspect,i);
		sortMap.insert(pair<int,int>(value_minus,i));
	}

	multimap<int,int>::iterator itr = sortMap.begin();
	if(!sortMap.empty())
	{
		for(;itr != sortMap.end();itr++)
		{
			if(sg_pDungeonSystem->隣接地形移動可(me(),itr->second))
			{
				sg_pDungeonSystem->移動要請(me(),itr->second);
				return true;
			}
		}
	}
	return false;
}
bool cEnemyAI::u_索敵()
{
	vector<pcCharacter> visCharaList;

	sg_pDungeonSystem->キャラクター目視可範囲キャラリスト(me(),visCharaList);
	int i;
	for(i=0;i<visCharaList.size();i++)
	{//敵キャラリスト作成
		if(sg_pDungeonSystem->キャラクター敵対判定(me(),visCharaList[i]))
		{
			if(u_目標免除(visCharaList[i]))
			{
				continue;
			}
			return true;
		}
	}
	return false;
}
bool cEnemyAI::u_敵サーチ_キャラ優先度優先(int& out_tgt_x,int& out_tgt_y)
{
	multimap<int,pcCharacter> sortMap;
	int i;
	vector<pcCharacter> visCharaList;

	int maxForcevalue = 0;
	sg_pDungeonSystem->キャラクター目視可範囲キャラリスト(me(),visCharaList);
	
	if(visCharaList.empty()) return false;

	for(i=0;i<visCharaList.size();i++)
	{//敵キャラリスト作成
		if(sg_pDungeonSystem->キャラクター敵対判定(me(),visCharaList[i]))
		{
			if(u_目標免除(visCharaList[i]))
			{
				continue;
			}
			if(maxForcevalue < visCharaList[i]->被攻撃優先度())
			{//優先度高い
				maxForcevalue = visCharaList[i]->被攻撃優先度();
				sortMap.clear();
			}
			else if(maxForcevalue > visCharaList[i]->被攻撃優先度())
			{//優先度低い
				continue;
			}
			cCoordinate coo;
			coo.x = visCharaList[i]->placeX - me()->placeX;
			coo.y = visCharaList[i]->placeY - me()->placeY;
			sortMap.insert(pair<int,pcCharacter>(coo.dif(),visCharaList[i]));
		}
	}

	multimap<int,pcCharacter>::iterator itr = sortMap.begin();
	if(sortMap.size())
	{//最も近い敵をサーチする。
		for(;itr != sortMap.end();itr++)
		{
			out_tgt_x = itr->second->placeX;
			out_tgt_y = itr->second->placeY;
			return true;
		}
	}

	return false;
}

bool cEnemyAI::u_味方主人公サーチ(int& out_tgt_x,int& out_tgt_y)
{
	multimap<int,pcCharacter> sortMap;
	int i;
	vector<pcCharacter> visCharaList;

	int maxForcevalue = 0;
	sg_pDungeonSystem->キャラクター目視可範囲キャラリスト(me(),visCharaList);
	
	if(visCharaList.empty()) return false;

	for(i=0;i<visCharaList.size();i++)
	{
		if(!sg_pDungeonSystem->キャラクター敵対判定(me(),visCharaList[i]))
		{
			if(visCharaList[i] != sg_pDungeonSystem->pPlayerChara())
			{
				continue;
			}
			cCoordinate coo;
			coo.x = visCharaList[i]->placeX - me()->placeX;
			coo.y = visCharaList[i]->placeY - me()->placeY;
			sortMap.insert(pair<int,pcCharacter>(coo.dif(),visCharaList[i]));
		}
	}

	multimap<int,pcCharacter>::iterator itr = sortMap.begin();
	if(sortMap.size())
	{
		for(;itr != sortMap.end();itr++)
		{
			out_tgt_x = itr->second->placeX;
			out_tgt_y = itr->second->placeY;
			return true;
		}
	}

	return false;
}

bool cEnemyAI::u_敵サーチ_距離優先(int& out_tgt_x,int& out_tgt_y)
{
	multimap<int,pcCharacter> sortMap;
	int i;
	vector<pcCharacter> visCharaList;

	int maxForcevalue = -100000000;
	sg_pDungeonSystem->キャラクター目視可範囲キャラリスト(me(),visCharaList);
	
	if(visCharaList.empty()) return false;
	
	for(i=0;i<visCharaList.size();i++)
	{//敵キャラリスト作成
		if(sg_pDungeonSystem->キャラクター敵対判定(me(),visCharaList[i]))
		{
			cCoordinate coo(visCharaList[i]->placeX - me()->placeX, visCharaList[i]->placeY - me()->placeY);
			if(u_目標免除(visCharaList[i]))
			{
				continue;
			}
			if(maxForcevalue < -coo.dif())
			{//優先度高い
				maxForcevalue = -coo.dif();
				sortMap.clear();
			}
			else if(maxForcevalue > -coo.dif())
			{//優先度低い
				continue;
			}
			sortMap.insert(pair<int,pcCharacter>(-visCharaList[i]->被攻撃優先度(), visCharaList[i]));
		}
	}

	multimap<int,pcCharacter>::iterator itr = sortMap.begin();
	if(sortMap.size())
	{//最も近い敵をサーチする。
		for(;itr != sortMap.end();itr++)
		{
			out_tgt_x = itr->second->placeX;
			out_tgt_y = itr->second->placeY;
			return true;
		}
	}

	return false;
}


bool cEnemyAI::u_目標点作成(int& out_tgt_x,int& out_tgt_y)
{
	vector<pcLandform> visibleExitLandlist;
	sg_pDungeonSystem->地形目視可範囲出口リスト(me(), visibleExitLandlist);
	
	if(visibleExitLandlist.size() == 0)
	{
		cCoordinate aspectcoo;
		aspectcoo.SetAspect(random()*8);
		out_tgt_x = me()->placeX + aspectcoo.x;
		out_tgt_y = me()->placeY + aspectcoo.y;
		return false;
	}


	int r = random()*visibleExitLandlist.size();



	pcLandform tgtland = visibleExitLandlist[r];



	out_tgt_x = tgtland->placeX;
	out_tgt_y = tgtland->placeY;

	cCoordinate aspectcoo;
	aspectcoo.SetAspect(me()->aspect);
	if(abs(me()->placeX -aspectcoo.x - out_tgt_x) + abs(me()->placeY -aspectcoo.y- out_tgt_y) <= 1)
	{//それがすぐ後ろ
		if(visibleExitLandlist.size() >= 2)
		{//出口が二つ以上ある
			int r2 = random()*visibleExitLandlist.size()-1;
			if(r2 >= r) r2++;

			pcLandform tgtland2 = visibleExitLandlist[r2];
			out_tgt_x = tgtland2->placeX;
			out_tgt_y = tgtland2->placeY;
		}
	}
	
	return true;
}
bool cEnemyAI::u_目標点作成_最近接出口(int& out_tgt_x,int& out_tgt_y)
{
	vector<pcLandform> visibleExitLandlist;
	sg_pDungeonSystem->地形目視可範囲出口リスト(me(), visibleExitLandlist);
	
	if(visibleExitLandlist.size() == 0)
	{
		return false;
	}

	pcLandform tgtland;
	cCoordinate mep;
	mep.x = me()->placeX;
	mep.y = me()->placeY;

	cCoordinate aspectcoo;
	aspectcoo.SetAspect(me()->aspect);
	int i;
	int mindif = 100000000;
	for(i=0;i<visibleExitLandlist.size();i++)
	{


		if(mindif > (visibleExitLandlist[i]->place - mep).dif()*128 + (visibleExitLandlist[i]->place - mep - aspectcoo).dif())
		{
			tgtland = visibleExitLandlist[i];
			mindif = (visibleExitLandlist[i]->place - mep).dif();
		}
	}


	out_tgt_x = tgtland->placeX;
	out_tgt_y = tgtland->placeY;


	return true;
}

int cEnemyAI::u_攻撃優先度_昇順(pcCharacter penemychara)
{
	int value = 0;

	//value += (boost::dynamic_pointer_cast<cPlayerChara>(penemychara)==NULL) * 256;//主人公優先
	value -= penemychara->被攻撃優先度();

	return value;
}

bool cEnemyAI::u_目標免除(pcCharacter penemychara)
{
	return penemychara->CharaAttribute().count(キャラ属性::弾幕)
		&& !penemychara->Condition.みがわり状態();
}