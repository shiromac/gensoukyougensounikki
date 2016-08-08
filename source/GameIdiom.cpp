#include "stdafx.h"

#include "GameIdiom.h"
#include "EffectFunctions.h"

#include "FindUtility.h"

#include "cCharacter.h"
#include "cDroping.h"
#include "cLandform.h"

using namespace GameIdiom;


int GameIdiom::高飛び(pcCharacter pchara, pcLandform fromplace)
{
	c4DVector start,end;
	
	//とびアニメーション
	start.set(pchara->placeX,pchara->placeY,0,0);
	end.set(pchara->placeX,pchara->placeY,10,0);
	sg_pDungeonSystem->AnimationManager().Anime_Throw(pchara,start,end,ROUTE_HIGH);

	sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(
		&(pchara->visibleplace),
		c4DVector(fromplace->place.x,fromplace->place.y,0,0));


	sg_pDungeonSystem->ワープ要請(pchara,
		sg_pDungeonSystem->存在安全地形(FindUtility::部屋外優先_各部屋等確率_ランダム地形検索_非配置安全(fromplace),pchara));


	return true;
}
int GameIdiom::高飛び(pcDroping pdrop, pcLandform fromplace)
{

	c4DVector start,end;
	//とびアニメーション
	start.set(pdrop->placeX,pdrop->placeY,0,0);
	end.set(pdrop->placeX,pdrop->placeY,10,0);
	sg_pDungeonSystem->AnimationManager().Anime_Throw(pdrop,start,end,ROUTE_HIGH);

	sg_pDungeonSystem->落ち物設置(pdrop,
		FindUtility::部屋外優先_各部屋等確率_ランダム地形検索_配置安全(fromplace));
	return true;
}

int GameIdiom::罠チェック(pcLandform fromplace)
{

	sg_pDungeonSystem->Map().mapping_just(fromplace->placeX, fromplace->placeY);

	if(fromplace->pOnDrop)
	{

		if(fromplace->pOnDrop != NULL && !fromplace->pOnDrop->isDiscover())
		{
			if(sg_pDungeonSystem->アイテム強制マッピング(fromplace->pOnDrop))
			{
				EffectFunctions::煙エフェクト1(fromplace->placeX,fromplace->placeY);
			}
		}
	}
	return true;
}


int GameIdiom::爆発(pcLandform fromplace, pcCharacter pchara, double power, int damegeflag, int enemyDestroyFlag, int ItemDestroyFlag, int landDestroyFlag, int selfDamageFlag, int fireAttributeFlag)
{
	vector<pcLandform> vpland;
	vpland.push_back(fromplace);

	int a;
	for(a=0;a<8;a++)
	{
		vpland.push_back(sg_pDungeonSystem->隣接地形(fromplace, a, 1));
	}

	int i;
	for(i=0;i<vpland.size();i++)
	{
		multiset<攻撃属性::攻撃属性> 属性;
		属性.insert(攻撃属性::爆発);

		if(enemyDestroyFlag)
		{
			属性.insert(攻撃属性::敵即死);
		}
		if(ItemDestroyFlag)
		{
			属性.insert(攻撃属性::アイテム破壊);
		}
		if(landDestroyFlag)
		{
			sg_pDungeonSystem->穴掘り(vpland[i]);
		}
		if(fireAttributeFlag)
		{
			属性.insert(攻撃属性::火);
		}

		pcAttackinformation pattackinfo = pcAttackinformation(new cAttackinformation);
		//pattackinfo->attacker = NULLCHARA;
		//pattackinfo->defenser = NULLCHARA;
		//pattackinfo->Landplase = vpland[i];
		//pattackinfo->valiable[変数_割合ダメージ] = power;
		//pattackinfo->valiable[変数_割合ダメージフラグ] = TRUE;

		if(selfDamageFlag || pchara != vpland[i]->pOnChar)
		{
			sg_pDungeonSystem->攻撃接近(攻撃作成(
				pchara,//Attacker
				vpland[i],//Landplase
				power,//attackPower
				damegeflag,//damageflag
				属性,//attribute
				true
				));
		}
	}

	if(landDestroyFlag)
	{
		sg_pDungeonSystem->地形整理();
	}

	return true;
}

int GameIdiom::広範囲攻撃(pcLandform fromplace, pcCharacter pchara, double power, int damegeflag, multiset<攻撃属性::攻撃属性>& attri, int selfDamageFlag)
{
	vector<pcLandform> vpland;
	vpland.push_back(fromplace);

	int a;
	for(a=0;a<8;a++)
	{
		vpland.push_back(sg_pDungeonSystem->隣接地形(fromplace, a, 1));
	}

	int i;
	for(i=0;i<vpland.size();i++)
	{

		if(selfDamageFlag || pchara != vpland[i]->pOnChar)
		{
			sg_pDungeonSystem->攻撃接近(攻撃作成(
				pchara,//Attacker
				vpland[i],//Landplase
				power,//attackPower
				damegeflag,//damageflag
				attri,//attribute
				true
				));
		}
	}

	return true;
}
int GameIdiom::超広範囲攻撃(pcLandform fromplace, pcCharacter pchara, double power, int damegeflag, std::multiset<攻撃属性::攻撃属性>& attri, int selfDamageFlag)
{
	vector<pcLandform> vpland;
	vpland.push_back(fromplace);

	int a;
	for(a=1;a<25;a++)
	{
		vpland.push_back(sg_pDungeonSystem->代理地形(fromplace, a));
	}

	int i;
	for(i=0;i<vpland.size();i++)
	{

		if(selfDamageFlag || pchara != vpland[i]->pOnChar)
		{
			sg_pDungeonSystem->攻撃接近(攻撃作成(
				pchara,//Attacker
				vpland[i],//Landplase
				power,//attackPower
				damegeflag,//damageflag
				attri,//attribute
				true
				));
		}
	}

	return true;
}


int GameIdiom::燃え(pcCharacter pchara, pcLandform pland, int power, int effectlightflag)
{
	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("flame.wav"),pland->place);
	//ここにエフェクト
	//炎
	pcaEfire_flame effect = pcaEfire_flame(new caEfire_flame(pland->placeX,pland->placeY,0,0,effectlightflag));
	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(effect));


	multiset<攻撃属性::攻撃属性> 属性;
	属性.insert(攻撃属性::火);

	sg_pDungeonSystem->攻撃接近(攻撃作成(
		pchara,//攻撃者
		pland,//場所
		power,//攻撃力
		true,//定数ダメージ
		属性,
		true
		));

	return true;
}

int GameIdiom::火柱召喚(pcCharacter pchara, pcLandform pland, int turn)
{
	
	if(!pland->through(0,0,0))return false;

	int dpack_int[DEF_DROP_DATAPACK_NUM] = {0};
	pcDroping pdrop = pland->pOnDrop;
	//pcharaは召喚者
	if(pdrop != NULL)
	{
		
		map<tstring, StyleString> valiable;
		valiable[_T("Drop")] = pdrop->FullName();
		if(pdrop->属性.count(落ち物属性::不燃性) || pdrop->大切())
		{
			return false;
		}
		else if(pdrop->属性.count(落ち物属性::火柱))
		{
			sg_pDungeonSystem->落ち物破壊要請(pdrop);
		}
		else
		{
			if(!GameIdiom::アイテム焼失(pdrop)) return false;

			g_Langメッセージ(_T("落ちもの焼失メッセージ"),valiable);
		}
	}
	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("flame.wav"),pland->place);
	pdrop = sg_pDungeonSystem->落ち物生成(11000,dpack_int,pland);
	pdrop->quality() = turn;
	pdrop->投擲者_weakpointer = wpcCharacter(pchara);
	pdrop->上に落ちる(pdrop->乗りキャラ());

	return true;
}
int GameIdiom::アイテム焼失(pcDroping pdrop)
{
	cCoordinate coo;
	if(pdrop->Holder() == NULL)
	{//地面
		coo.x = pdrop->placeX;
		coo.y = pdrop->placeY;
	}
	else
	{
		coo.x = pdrop->Holder()->placeX;
		coo.y = pdrop->Holder()->placeY;
	}
	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("flame.wav"),coo);
	pcaEfire_flame effect = pcaEfire_flame(new caEfire_flame(coo.x,coo.y,0,0,1));
	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(effect));

	return sg_pDungeonSystem->落ち物破壊要請(pdrop);
}


//軽いイデオム
int GameIdiom::キャラの方を向く(pcCharacter frompchara, pcCharacter tochara)
{
	if(tochara == NULL || frompchara == NULL) return false;

	cCoordinate nowco(frompchara->placeX, frompchara->placeY);
	cCoordinate youco(tochara->placeX, tochara->placeY);

	return sg_pDungeonSystem->方向転換要請(frompchara,(youco-nowco).GetAspect());
}


bool GameIdiom::悪性異常状態治療要請(pcCharacter pchara, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	pchara->Condition.とても強い刺激();
	pchara->Condition.バクスイ追加(-1);
	pchara->Condition.眠り追加(-1);
	pchara->Condition.びっくり追加(-1);
	pchara->Condition.金縛り追加(-1);
	pchara->Condition.封印追加(-1);
	pchara->Condition.空振り追加(-1);
	pchara->Condition.貧乏追加(-1);
	pchara->Condition.臆病追加(-1);
	//pchara->Condition.嫉妬追加(-1);
	pchara->Condition.狂乱追加(-1);
	pchara->Condition.無意識追加(-1);

	if(pchara->Condition.力度数() < 0) {
		pchara->Condition.脱力初期化();
	}
	if(pchara->Condition.守度数() < 0) {
		pchara->Condition.軟弱初期化();
	}

	pchara->Condition.泥酔追加(-1);
	pchara->Condition.氷付け追加(-1);
	pchara->Condition.鳥目追加(-1);
	pchara->Condition.病気追加(-1);
	//pchara->Condition.健康追加(-1);

	pchara->Condition.死の誘い追加(-1,NULLCHARA);
	pchara->Condition.みがわり追加(-1,NULLCHARA);
	//pchara->Condition.擬態追加(-1);

	if(pchara->Condition.速度度数() < pchara->Condition.デフォルト速度度数())
	{
		sg_pDungeonSystem->速度異常治療要請(pchara, false);
	}

	return true;
}
bool GameIdiom::良性異常状態治療要請(pcCharacter pchara, int Messageflag)
{
	if(pchara == NULL || pchara->死亡()) return false;

	pchara->Condition.嫉妬追加(-1);

	if(pchara->Condition.力度数() > 0) {
		pchara->Condition.脱力初期化();
	}
	if(pchara->Condition.守度数() > 0) {
		pchara->Condition.軟弱初期化();
	}

	pchara->Condition.健康追加(-1);

	pchara->Condition.擬態追加(-1);

	if(pchara->Condition.速度度数() > pchara->Condition.デフォルト速度度数())
	{
		sg_pDungeonSystem->速度異常治療要請(pchara, false);
	}

	return true;
}

bool GameIdiom::全異常状態治療要請(pcCharacter pchara, int Messageflag)
{
	良性異常状態治療要請(pchara, Messageflag);
	悪性異常状態治療要請(pchara, Messageflag);
	return true;
}

bool GameIdiom::悪性異常状態である(pcCharacter pchara)
{
	if(pchara == NULL || pchara->死亡()) return false;

	if(pchara->Condition.眠りで行動不能である()) {return true;}
	if(pchara->Condition.びっくりで行動不能である()) {return true;}
	if(pchara->Condition.金縛りで行動不能である()) {return true;}
	if(pchara->Condition.封印状態()) {return true;}
	if(pchara->Condition.空振り状態()) {return true;}
	if(pchara->Condition.貧乏状態()) {return true;}
	if(pchara->Condition.臆病状態()) {return true;}
	//if(pchara->Condition.嫉妬状態()) return true;
	if(pchara->Condition.狂乱状態()) {return true;}
	if(pchara->Condition.無意識状態()) {return true;}

	if(pchara->Condition.力度数() < 0) {
		return true;
	}
	if(pchara->Condition.守度数() < 0) {
		return true;
	}

	if(pchara->Condition.泥酔状態()) {return true;}
	if(pchara->Condition.氷付け状態()) {return true;}
	if(pchara->Condition.鳥目状態()) {return true;}
	if(pchara->Condition.病気状態()) {return true;}
	//if(pchara->Condition.健康状態()) {return true;}


	if(pchara->Condition.死の誘い状態()) {return true;}
	if(pchara->Condition.みがわり状態()) {return true;}
	//if(pchara->Condition.擬態状態()) {return true;}


	if(pchara->Condition.速度度数() < pchara->Condition.デフォルト速度度数())
	{
		return true;
	}

	return false;
}
bool GameIdiom::良性異常状態である(pcCharacter pchara)
{
	if(pchara == NULL || pchara->死亡()) return false;

	if(pchara->Condition.嫉妬状態()) {return true;}

	if(pchara->Condition.力度数() > 0) {
		return true;
	}
	if(pchara->Condition.守度数() > 0) {
		return true;
	}

	if(pchara->Condition.健康状態()) {return true;}

	if(pchara->Condition.擬態状態()) {return true;}

	if(pchara->Condition.速度度数() > pchara->Condition.デフォルト速度度数())
	{
		return true;
	}

	return false;
}

bool GameIdiom::異常状態である(pcCharacter pchara)
{
	if(悪性異常状態である(pchara)) {return true;}

	if(良性異常状態である(pchara)) {return true;}

	return false;
}