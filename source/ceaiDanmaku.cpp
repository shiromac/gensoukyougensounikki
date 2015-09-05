#include "stdafx.h"
#include "ceaiDanmaku.h"



ceaiDanmaku::ceaiDanmaku(void)
{

}

ceaiDanmaku::~ceaiDanmaku(void)
{

}

int ceaiDanmaku::Init(pcCharacter self)
{
	cEnemyAI::Init(self);

	mode = MODE_NOTHING;

	me()->AImemory[3] = 0;
	return true;
}

int ceaiDanmaku::Request(int phase)
{

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

	pcCharacter pchara = sg_pDungeonSystem->キャラ前地形(me(),1)->pOnChar;
	if(pchara)
	{
		if((sg_pDungeonSystem->キャラクター敵対判定(me(),pchara)) &&
			u_隣接攻撃通用判定(me()->aspect))
		{//敵
			return AI_REQUEST_ATTACK;
		}
	}


	return AI_REQUEST_MOVE;
}

int ceaiDanmaku::Move()
{
	bool flag = false;
	if(sg_pDungeonSystem->隣接地形移動可(me(), me()->aspect))
	{
		flag = sg_pDungeonSystem->移動要請(me(),me()->aspect);
	}
	if(flag)
	{
		me()->AImemory[3] = 0;
	}
	else
	{
		me()->AImemory[3]++;
	}
	return flag;
}

int ceaiDanmaku::Attack()
{
	pcCharacter pchara = sg_pDungeonSystem->キャラ前地形(me(),1)->pOnChar;
	
	if((sg_pDungeonSystem->キャラクター敵対判定(me(),pchara)) &&
		u_隣接攻撃通用判定(me()->aspect))
	{//敵
		return u_敵直接攻撃(pchara);
	}
	return false;
}

//--------------------------------------------------------------


ceaiDanmakuTuibi::ceaiDanmakuTuibi(void)
{

}

ceaiDanmakuTuibi::~ceaiDanmakuTuibi(void)
{

}

int ceaiDanmakuTuibi::Init(pcCharacter self)
{
	cEnemyAI::Init(self);

	mode = MODE_NOTHING;

	me()->AImemory[3] = 0;
	return true;
}

int ceaiDanmakuTuibi::Request(int phase)
{

	if(mode == MODE_NOTHING)
	{
		u_目標点作成(tgtLandX,tgtLandY);
		mode = MODE_ATTACK;
	}
	if(mode == MODE_WALK)//徘徊モード
	{
		//何もしない
	}
	else if(mode == MODE_ATTACK)//追尾モード
	{
		multimap<int,pcCharacter> sortMap;
		int i;
		vector<pcCharacter> visCharaList;

		int maxForcevalue = 0;
		sg_pDungeonSystem->キャラクター目視可範囲キャラリスト(me(),visCharaList);
		
		if(visCharaList.empty())
		{
			mode = MODE_WALK;
		}

		for(i=0;i<visCharaList.size();i++)
		{//敵キャラリスト作成
			if(sg_pDungeonSystem->キャラクター敵対判定(me(),visCharaList[i]))
			{
				cCoordinate coo;
				coo.x = visCharaList[i]->placeX - me()->placeX;
				coo.y = visCharaList[i]->placeY - me()->placeY;

				if( aspect_AngleAspect( me()->aspect, coo.GetAspect()) < 2)
				{//正面方向にいる

					if(maxForcevalue < visCharaList[i]->被攻撃優先度())
					{//優先度高い
						maxForcevalue = visCharaList[i]->被攻撃優先度();
						sortMap.clear();
					}
					else if(maxForcevalue > visCharaList[i]->被攻撃優先度())
					{//優先度低い
						continue;
					}

					sortMap.insert(pair<int,pcCharacter>(coo.dif(),visCharaList[i]));
				}
			}
		}

		multimap<int,pcCharacter>::iterator itr = sortMap.begin();
		if(sortMap.size())
		{//最も近い敵をサーチする。
			for(;itr != sortMap.end();itr++)
			{
				tgtLandX = itr->second->placeX;
				tgtLandY = itr->second->placeY;
			}
		}
		else
		{//敵を見失う
			mode = MODE_WALK; 
		}

	}
	else
	{
		u_目標点作成(tgtLandX,tgtLandY);
		mode = MODE_WALK;
	}


	pcCharacter pchara = sg_pDungeonSystem->キャラ前地形(me(),1)->pOnChar;
	if(pchara)
	{
		if((sg_pDungeonSystem->キャラクター敵対判定(me(),pchara)) &&
			u_隣接攻撃通用判定(me()->aspect))
		{//敵
			return AI_REQUEST_ATTACK;
		}
	}


	return AI_REQUEST_MOVE;
}

int ceaiDanmakuTuibi::Move()
{
	bool flag = false;
	flag = sg_pDungeonSystem->移動要請(me(),me()->aspect);
	if(flag)
	{
		me()->AImemory[3] = 0;
	}
	else
	{
		me()->AImemory[3]++;
	}
	if(flag && mode == MODE_ATTACK)
	{
		cCoordinate coo,mep,aspectcoo;
		int aspect;
		mep.x = me()->placeX;
		mep.y = me()->placeY;
		coo.x = tgtLandX;
		coo.y = tgtLandY;

		aspectcoo = coo-mep;

		if(aspectcoo.dif() != 0)
		{
			aspect = aspectcoo.GetAspect();
			if(aspect_AngleAspect(aspect,me()->aspect) < 2)
			{
				if(sg_pDungeonSystem->隣接地形移動可(me(), aspect))
				{
					sg_pDungeonSystem->方向転換要請(me(), aspect);
				}
			}
		}
		else
		{
			mode = MODE_WALK;
		}
	}
	return flag;
}

int ceaiDanmakuTuibi::Attack()
{
	pcCharacter pchara = sg_pDungeonSystem->キャラ前地形(me(),1)->pOnChar;
	
	if((sg_pDungeonSystem->キャラクター敵対判定(me(),pchara)) &&
		u_隣接攻撃通用判定(me()->aspect))
	{//敵
		return u_敵直接攻撃(pchara);
	}
	return false;
}