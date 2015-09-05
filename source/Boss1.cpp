#include "Boss1.h"
//レミリア
#include "cObjectChara.h"
#include "ceaiActive.h"
#include "ceaiEscape.h"
#include "GameIdiom.h"
#include "cDungeonSystem.h"
#include "FindUtility.h"
#include "EffectFunctions.h"
#include "gameMainSystem/filemanage/cScriptRLayer.h"
class cBossSpell_remilia1: public cBossSpell
{
public:
	cBossSpell_remilia1(pcScriptRLayer scriptlayer,
		const tstring& scriptKey,
		pcCharacter boss,
		const int spellIndex)
		:cBossSpell(scriptlayer,
		scriptKey,
		boss,
		spellIndex)
	{

	};
public:
	virtual ~cBossSpell_remilia1(void){};
	unsigned long imageColor1()
	{
		return 0xFFDD0022;
	};
	unsigned long imageColor2()
	{
		return 0xFFFFBBBB;
	};
public:
	virtual void startSpell()	
	{
		int vint[3];

		int i;
		for(i=0; i<boss()->ValiableConstant4() ;i++)
		{
			sg_pDungeonSystem->アイテム自然湧きID(vint);
			pcLandform pland = FindUtility::各部屋等確率_ランダム地形検索_店回避_配置安全();
			sg_pDungeonSystem->落ち物生成(vint[0],NULL,pland,true);
			EffectFunctions::煙エフェクト1(pland->placeX,pland->placeY);
		}

	};
	virtual void endSpell()
	{};
	virtual pcLandform AIお気に入り地形(cValiableField& valiable){return NULLLAND;};
	virtual int AI特殊攻撃選択(cValiableField& valiable)
	{
		flags[_T("Danmaku")] = 0;
		flags[_T("kakutou")] = 0;
		flags[_T("NoMove")] = 0;
		flags[_T("Hakkoukakutou")] = 0;
		int limitCount = spellTurnLimitCount();
		int restTurn = spellRestTurn();
		if(limitCount*0.3 > restTurn)
		{
			//発狂
			if(spellTurnCount() % 8 == 3)
			{
				flags[_T("kakutou")] = 1;
				return true;
			}
			else if(spellTurnCount() % 8 == 7)
			{
				flags[_T("Danmaku")] = 1;
				return true;
			}
			else if(spellTurnCount() % 8 == 6
				|| spellTurnCount() % 8 == 5
				|| spellTurnCount() % 8 == 4)
			{
				flags[_T("Hakkoukakutou")] = 1;
				return true;
			}
		}
		else
		{
			if(spellTurnCount() % 10 == 4)
			{
				flags[_T("kakutou")] = 1;
				return true;
			}
			else if(spellTurnCount() % 10 == 9)
			{
				flags[_T("Danmaku")] = 1;
				return true;
			}
			else if(spellTurnCount() % 10 == 8
				|| spellTurnCount() % 10 == 7
				|| spellTurnCount() % 10 == 6)
			{
				flags[_T("NoMove")] = 1;
				return true;
			}
		}
		return false;
	};
	virtual int 特殊攻撃効果(cValiableField& valiable)
	{
		if(flags[_T("Danmaku")] == 1)
		{
			flags[_T("Danmaku")] = 0;

			EffectFunctions::ボスエフェクト集中(boss()->placeX, boss()->placeY, 0);
			int i,k;
			for(i=0;i<32;i++)
			{
				for(k=0;k<8;k++)
				{
					double x = pow(i/32.0 -1, 2);
					c4DVector v(i*boss()->ValiableConstant3() + x*boss()->ValiableConstant3()*2,0,0,0);
					v.turn(k*45+i*8);
					v.x += boss()->placeX + 0.5;
					v.y += boss()->placeY + 0.5;
					pcLandform pland = sg_pDungeonSystem->Map().Land(v.x,v.y);
					if(sg_pDungeonSystem->キャラ配置安全(pland))
					{
						EffectFunctions::煙エフェクト1(pland->placeX,pland->placeY);

						弾幕::弾幕Type type = 弾幕::直進;
						弾幕::弾幕色 color = 弾幕::アカ;
						if(i%4 == 0)
						{
							color = 弾幕::マゼンタ;
							type = 弾幕::追尾;
						}
						弾幕::弾幕召喚(pland,//場所
										boss()->ValiableConstant2(),//HP
										CHARACTER_FORSE_ENEMY,//Forse
										0,//speed
										type,//弾幕Type
										color,//弾幕色
										boss()->aspect+k + i/2,//aspect
										NULLCHARA//targetenemy
										);
						
					}
				}
			}
			return true;
		}
		if(flags[_T("kakutou")] == 1)
		{
			flags[_T("kakutou")] = 0;
			pcLandform pland = valiable.lands[変数_攻撃場所];
			cCoordinate coo;
			if(pland)
			{
				coo.x = boss()->placeX - pland->placeX;
				coo.y = boss()->placeY - pland->placeY;
				
				EffectFunctions::ボスエフェクト集中(boss()->placeX, boss()->placeY, 0);
			
				if(coo.dif() >= 20)
				{
					sg_pDungeonSystem->投げ飛ばし要請(boss(),boss(),
						pland,
						boss()->ValiableConstant1(), true, 2.0);
				}
				else if(coo.dif() <= 1)
				{
					int a;
					for(a=0;a<8;a++)
					{
						pcLandform pland = sg_pDungeonSystem->キャラ視点方角地形(boss(),a,1);
						if(pland->pOnChar != NULL)
						{
							pcCharacter pcharafuki = pland->pOnChar;
							sg_pDungeonSystem->投げ飛ばし要請(boss(),pcharafuki,
								sg_pDungeonSystem->隣接地形(pland, boss()->aspect + a, 10),
								 boss()->ValiableConstant1(), true, 2.0);
								 
						}
					}
				}
				else
				{
					int i;
					for(i=0;i<3;i++)
					{
						pcLandform pland = sg_pDungeonSystem->キャラ視点方角地形(boss(),i*2 -2,1);
						if(sg_pDungeonSystem->キャラ配置安全(pland))
						{
							弾幕::弾幕Type type = 弾幕::直進;
							弾幕::弾幕色 color = 弾幕::アカ;
							弾幕::弾幕召喚(pland,//場所
										boss()->ValiableConstant2(),//HP
										CHARACTER_FORSE_ENEMY,//Forse
										1,//speed
										type,//弾幕Type
										color,//弾幕色
										boss()->aspect,//aspect
										NULLCHARA//targetenemy
										);
						}
					}
				}
			}
			return true;
		}
		if(flags[_T("NoMove")] == 1)
		{
			flags[_T("NoMove")] = 0;

			{//目標点を向く
				pcLandform pland = valiable.lands[変数_攻撃場所];
				cCoordinate coo,mep,aspectcoo;
				int aspect;
				mep.x = boss()->placeX;
				mep.y = boss()->placeY;
				coo.x = pland->placeX;
				coo.y = pland->placeY;

				aspectcoo = coo-mep;

				if(aspectcoo.dif() != 0)
				{
					aspect = aspectcoo.GetAspect();
					sg_pDungeonSystem->方向転換要請(boss(), aspect);
				}
			}

			multimap<int,pcCharacter> sortMap;
			vector<pcCharacter> visibleCharalist;

			sg_pDungeonSystem->キャラクター目視可範囲キャラリスト(boss(),visibleCharalist);
			
			int i;
			for(i=0;i<visibleCharalist.size();i++)
			{
				if(boss()->pAI->u_敵隣接判定(visibleCharalist[i]))
				{
					sortMap.insert(pair<int,pcCharacter>(boss()->pAI->u_攻撃優先度_昇順(visibleCharalist[i]),visibleCharalist[i]));
				}
			}

			multimap<int,pcCharacter>::iterator itr = sortMap.begin();
			if(sortMap.size())
			{
				for(;itr != sortMap.end();itr++)
				{
					boss()->pAI->u_敵直接攻撃(itr->second);
					return true;
				}
			}
			return true;
		}
		if(flags[_T("Hakkoukakutou")] == 1)
		{
			flags[_T("Hakkoukakutou")] = 0;

			int i;
			for(i=0;i<3;i++)
			{
				pcLandform pland = sg_pDungeonSystem->キャラ視点方角地形(boss(),i*2 -2,1);
				if(sg_pDungeonSystem->キャラ配置安全(pland))
				{
					弾幕::弾幕Type type = 弾幕::追尾;
					弾幕::弾幕色 color = 弾幕::マゼンタ;
					弾幕::弾幕召喚(pland,//場所
								boss()->ValiableConstant2(),//HP
								CHARACTER_FORSE_ENEMY,//Forse
								1,//speed
								type,//弾幕Type
								color,//弾幕色
								boss()->aspect,//aspect
								NULLCHARA//targetenemy
								);
				}
			}
			return true;
		}
		return false;
	};
	virtual int 特殊攻撃_アニメ(cValiableField& valiable){return false;};
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable){};
	
};


class cBossSpell_remilia2: public cBossSpell
{
public:
	cBossSpell_remilia2(pcScriptRLayer scriptlayer,
		const tstring& scriptKey,
		pcCharacter boss,
		const int spellIndex)
		:cBossSpell(scriptlayer,
		scriptKey,
		boss,
		spellIndex)
	{

	};
public:
	virtual ~cBossSpell_remilia2(void){};
	unsigned long imageColor1()
	{
		return 0xFFDD0022;
	};
	unsigned long imageColor2()
	{
		return 0xFFFFEE00;
	};
public:
	
	virtual void startSpell()	
	{
		int vint[3];

		int i;
		for(i=0; i<boss()->ValiableConstant4() ;i++)
		{
			sg_pDungeonSystem->アイテム自然湧きID(vint);
			pcLandform pland = FindUtility::各部屋等確率_ランダム地形検索_店回避_配置安全();
			sg_pDungeonSystem->落ち物生成(vint[0],NULL,pland,true);
			EffectFunctions::煙エフェクト1(pland->placeX,pland->placeY);
		}

		flags[_T("Level")] = 0;
		flags[_T("Aspect")] = ASPECT_DOWN;
		/*
		pcLandform pland = sg_pDungeonSystem->Map().Land(MAPWIDTH/2,MAPHEIGHT/2);
		
		EffectFunctions::煙エフェクト1(boss()->足元地形()->placeX, boss()->足元地形()->placeY);
		sg_pDungeonSystem->ワープ要請(boss(),pland);
		EffectFunctions::煙エフェクト1(pland->placeX,pland->placeY);

		for(i=0;i<4;i++)
		{
			sg_pDungeonSystem->キャラクター生成(5001, 0, CHARACTER_FORSE_ENEMY, 
				sg_pDungeonSystem->隣接地形(pland, ASPECT_UP+i*2, 1));
		}
		*/
	};
	virtual void endSpell()
	{};
	virtual pcLandform AIお気に入り地形(cValiableField& valiable){return NULLLAND;};
	virtual int AI特殊攻撃選択(cValiableField& valiable)
	{
		flags[_T("Danmaku")] = 0;
		flags[_T("kakutou")] = 0;
		flags[_T("NoMove")] = 0;
		flags[_T("Hakkoukakutou")] = 0;
		int limitCount = spellTurnLimitCount();
		int restTurn = spellRestTurn();
		if(limitCount*0.3 > restTurn)
		{
			//発狂
			if(spellTurnCount() % 6 == 5 || spellTurnCount() % 6 == 2)
			{
				flags[_T("Danmaku")] = 1;
				return true;
			}
			if(spellTurnCount() % 6 == 4 || spellTurnCount() % 6 == 1)
			{
				flags[_T("Hakkoukakutou")] = 1;
				return true;
			}
			else
			{
				flags[_T("NoMove")] = 1;
				return true;
			}
		}
		else
		{

			if(spellTurnCount() % 6 == 5)
			{
				flags[_T("Danmaku")] = 1;
				return true;
			}
			if(spellTurnCount() % 6 == 4)
			{
				flags[_T("kakutou")] = 1;
				return true;
			}
			else
			{
				flags[_T("NoMove")] = 1;
				return true;
			}
	
		}
		return false;
	};
	virtual int 特殊攻撃効果(cValiableField& valiable)
	{
		if(flags[_T("Danmaku")] == 1)
		{
			flags[_T("Danmaku")] = 0;

			EffectFunctions::ボスエフェクト集中(boss()->placeX, boss()->placeY, 0);
			int i,k;
			for(i=0;i<8;i++)
			{

				pcLandform pland = sg_pDungeonSystem->Map().Land(boss()->placeX, boss()->placeY);
				pland = sg_pDungeonSystem->隣接地形(pland, i, 1);
				if(sg_pDungeonSystem->キャラ配置安全(pland))
				{
					EffectFunctions::煙エフェクト1(pland->placeX,pland->placeY);

					弾幕::弾幕Type type = 弾幕::追尾;
					弾幕::弾幕色 color = 弾幕::マゼンタ;
					弾幕::弾幕召喚(pland,//場所
									boss()->ValiableConstant2(),//HP
									CHARACTER_FORSE_ENEMY,//Forse
									0,//speed
									type,//弾幕Type
									color,//弾幕色
									i,//aspect
									NULLCHARA//targetenemy
									);
					
				}
				
			}
			return true;
		}
		if(flags[_T("kakutou")] == 1)
		{
			flags[_T("kakutou")] = 0;
			pcLandform pland = valiable.lands[変数_攻撃場所];
			cCoordinate coo;
			if(pland)
			{
				coo.x = boss()->placeX - pland->placeX;
				coo.y = boss()->placeY - pland->placeY;
				
				EffectFunctions::ボスエフェクト集中(boss()->placeX, boss()->placeY, 0);
			

				if(coo.dif() <= 1)
				{
					int a;
					for(a=0;a<8;a++)
					{
						pcLandform pland = sg_pDungeonSystem->キャラ視点方角地形(boss(),a,1);
						if(pland->pOnChar != NULL)
						{
							pcCharacter pcharafuki = pland->pOnChar;
							sg_pDungeonSystem->投げ飛ばし要請(boss(),pcharafuki,
								sg_pDungeonSystem->隣接地形(pland, boss()->aspect + a, 10),
								 boss()->ValiableConstant1(), true, 2.0);
								 
						}
					}
				}
				else
				{
					int i;
					for(i=0;i<3;i++)
					{
						pcLandform pland = sg_pDungeonSystem->キャラ視点方角地形(boss(),i*2 -2,1);
						if(sg_pDungeonSystem->キャラ配置安全(pland))
						{
							弾幕::弾幕Type type = 弾幕::直進;
							弾幕::弾幕色 color = 弾幕::アカ;
							弾幕::弾幕召喚(pland,//場所
										boss()->ValiableConstant2(),//HP
										CHARACTER_FORSE_ENEMY,//Forse
										0,//speed
										type,//弾幕Type
										color,//弾幕色
										boss()->aspect,//aspect
										NULLCHARA//targetenemy
										);
						}
					}
				}
			}
			return true;
		}
		if(flags[_T("NoMove")] == 1)
		{
			flags[_T("NoMove")] = 0;

			{//目標点を向く
				pcLandform pland = valiable.lands[変数_攻撃場所];
				cCoordinate coo,mep,aspectcoo;
				int aspect;
				mep.x = boss()->placeX;
				mep.y = boss()->placeY;
				coo.x = pland->placeX;
				coo.y = pland->placeY;

				aspectcoo = coo-mep;

				if(aspectcoo.dif() != 0)
				{
					aspect = aspectcoo.GetAspect();
					sg_pDungeonSystem->方向転換要請(boss(), aspect);
				}
			}

			multimap<int,pcCharacter> sortMap;
			vector<pcCharacter> visibleCharalist;

			sg_pDungeonSystem->キャラクター目視可範囲キャラリスト(boss(),visibleCharalist);
			
			int i;
			for(i=0;i<visibleCharalist.size();i++)
			{
				if(boss()->pAI->u_敵隣接判定(visibleCharalist[i]))
				{
					sortMap.insert(pair<int,pcCharacter>(boss()->pAI->u_攻撃優先度_昇順(visibleCharalist[i]),visibleCharalist[i]));
				}
			}

			multimap<int,pcCharacter>::iterator itr = sortMap.begin();
			if(sortMap.size())
			{
				for(;itr != sortMap.end();itr++)
				{
					boss()->pAI->u_敵直接攻撃(itr->second);
					return true;
				}
			}
			return true;
		}
		if(flags[_T("Hakkoukakutou")] == 1)
		{
			flags[_T("Hakkoukakutou")] = 0;

			int i;
			for(i=0;i<3;i++)
			{
				pcLandform pland = sg_pDungeonSystem->キャラ視点方角地形(boss(),i*2 -2,1);
				if(sg_pDungeonSystem->キャラ配置安全(pland))
				{
					弾幕::弾幕Type type = 弾幕::追尾;
					弾幕::弾幕色 color = 弾幕::マゼンタ;
					弾幕::弾幕召喚(pland,//場所
								boss()->ValiableConstant2(),//HP
								CHARACTER_FORSE_ENEMY,//Forse
								1,//speed
								type,//弾幕Type
								color,//弾幕色
								boss()->aspect,//aspect
								NULLCHARA//targetenemy
								);
				}
			}
			return true;
		}
		return false;
	};
	virtual int 特殊攻撃_アニメ(cValiableField& valiable){return false;};
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable)
	{
		if(timing == ターン終了_タイミング)
		{
			if(spellTurnCount() % 8 == 4)
			{
				int i,k;
				for(i=0;i<MAPHEIGHT;i++)
				{
		
					int aspect = ASPECT_LEFT + (i%2)*4;
					pcLandform pland = sg_pDungeonSystem->射程内障害物検索(
						sg_pDungeonSystem->Map().Land(MAPWIDTH/2,i),
						aspect+4,
						MAPWIDTH);
					pland = sg_pDungeonSystem->隣接地形(pland,aspect,1);

					if(sg_pDungeonSystem->キャラ配置安全(pland))
					{
						EffectFunctions::煙エフェクト1(pland->placeX,pland->placeY);

						弾幕::弾幕Type type = 弾幕::直進;
						弾幕::弾幕色 color = 弾幕::ミドリ;

						弾幕::弾幕召喚(pland,//場所
										boss()->ValiableConstant2(),//HP
										CHARACTER_FORSE_ENEMY,//Forse
										0,//speed
										type,//弾幕Type
										color,//弾幕色
										aspect,//aspect
										NULLCHARA//targetenemy
										);
					}
				}
				for(i=0;i<MAPWIDTH;i++)
				{
		
					int aspect = ASPECT_DOWN + (i%2)*4;
					pcLandform pland = sg_pDungeonSystem->射程内障害物検索(
						sg_pDungeonSystem->Map().Land(i,MAPHEIGHT/2),
						aspect+4,
						MAPHEIGHT);
					pland = sg_pDungeonSystem->隣接地形(pland,aspect,1);

					if(sg_pDungeonSystem->キャラ配置安全(pland))
					{
						EffectFunctions::煙エフェクト1(pland->placeX,pland->placeY);

						弾幕::弾幕Type type = 弾幕::直進;
						弾幕::弾幕色 color = 弾幕::ミドリ;

						弾幕::弾幕召喚(pland,//場所
										boss()->ValiableConstant2(),//HP
										CHARACTER_FORSE_ENEMY,//Forse
										0,//speed
										type,//弾幕Type
										color,//弾幕色
										aspect,//aspect
										NULLCHARA//targetenemy
										);
					}
				}
			}
		}
	
	};
	
};


class cBossSpell_remilia3: public cBossSpell
{
public:
	cBossSpell_remilia3(pcScriptRLayer scriptlayer,
		const tstring& scriptKey,
		pcCharacter boss,
		const int spellIndex)
		:cBossSpell(scriptlayer,
		scriptKey,
		boss,
		spellIndex)
	{

	};
public:
	virtual ~cBossSpell_remilia3(void){};
	unsigned long imageColor1()
	{
		return 0xFFDD0022;
	};
	unsigned long imageColor2()
	{
		return 0xFF880033;
	};
public:
	virtual void startSpell()	
	{
		int vint[3];

		int i;
		for(i=0; i<boss()->ValiableConstant4() ;i++)
		{
			sg_pDungeonSystem->アイテム自然湧きID(vint);
			pcLandform pland = FindUtility::各部屋等確率_ランダム地形検索_店回避_配置安全();
			sg_pDungeonSystem->落ち物生成(vint[0],NULL,pland,true);
			EffectFunctions::煙エフェクト1(pland->placeX,pland->placeY);
		}

		flags[_T("Level")] = 0;
		flags[_T("Aspect")] = ASPECT_DOWN;

		pcLandform pland = sg_pDungeonSystem->射程内障害物検索(
			sg_pDungeonSystem->Map().Land(MAPWIDTH/2,MAPHEIGHT/2),
			ASPECT_UP,
			MAPHEIGHT);
		pland = sg_pDungeonSystem->隣接地形(pland,ASPECT_DOWN,1);

		
		EffectFunctions::煙エフェクト1(boss()->足元地形()->placeX,boss()->足元地形()->placeY);
		sg_pDungeonSystem->ワープ要請(boss(),pland);
		EffectFunctions::煙エフェクト1(pland->placeX,pland->placeY);

	};
	virtual void endSpell()
	{};
	virtual pcLandform AIお気に入り地形(cValiableField& valiable){return NULLLAND;};
	virtual int AI特殊攻撃選択(cValiableField& valiable)
	{
		flags[_T("Danmaku")] = 0;
		flags[_T("kakutou")] = 0;
		flags[_T("NoMove")] = 0;
		flags[_T("Hakkoukakutou")] = 0;
		int limitCount = spellTurnLimitCount();
		int restTurn = spellRestTurn();
		if(limitCount*0.3 > restTurn)
		{
			//発狂
			if(spellTurnCount() % 6 == 5 || spellTurnCount() % 6 == 2)
			{
				flags[_T("Danmaku")] = 1;
				return true;
			}
			if(spellTurnCount() % 6 == 4 || spellTurnCount() % 6 == 1)
			{
				flags[_T("Hakkoukakutou")] = 1;
				return true;
			}
			else
			{
				flags[_T("NoMove")] = 1;
				return true;
			}
		}
		else
		{

			if(spellTurnCount() % 6 == 5)
			{
				flags[_T("Danmaku")] = 1;
				return true;
			}
			if(spellTurnCount() % 6 == 4)
			{
				flags[_T("kakutou")] = 1;
				return true;
			}
			else
			{
				flags[_T("NoMove")] = 1;
				return true;
			}
	
		}
		return false;
	};
	virtual int 特殊攻撃効果(cValiableField& valiable)
	{
		if(flags[_T("Danmaku")] == 1)
		{
			flags[_T("Danmaku")] = 0;

			EffectFunctions::ボスエフェクト集中(boss()->placeX, boss()->placeY, 0);
			int i,k;
			for(i=ASPECT_RIGHT;i<8;i+=4)
			{

				pcLandform pland = sg_pDungeonSystem->Map().Land(boss()->placeX, boss()->placeY);
				pland = sg_pDungeonSystem->隣接地形(pland, i, 1);
				if(sg_pDungeonSystem->キャラ配置安全(pland))
				{
					EffectFunctions::煙エフェクト1(pland->placeX,pland->placeY);

					弾幕::弾幕Type type = 弾幕::追尾;
					弾幕::弾幕色 color = 弾幕::マゼンタ;
					弾幕::弾幕召喚(pland,//場所
									boss()->ValiableConstant2(),//HP
									CHARACTER_FORSE_ENEMY,//Forse
									0,//speed
									type,//弾幕Type
									color,//弾幕色
									i,//aspect
									NULLCHARA//targetenemy
									);
					
				}
				
			}
			for(i=0;i<MAPWIDTH;i++)
			{
				if((spellTurnCount()/10) %2)
				{
					if((i+1)%2)
					{
						continue;
					}
				}
				else
				{
					if(i%2)
					{
						continue;
					}
				}
				int aspect = flags[_T("Aspect")];
				pcLandform pland = sg_pDungeonSystem->射程内障害物検索(
					sg_pDungeonSystem->Map().Land(i,MAPHEIGHT/2),
					aspect+4,
					MAPHEIGHT);
				pland = sg_pDungeonSystem->隣接地形(pland,aspect,2);

				if(sg_pDungeonSystem->キャラ配置安全(pland))
				{
					EffectFunctions::煙エフェクト1(pland->placeX,pland->placeY);

					弾幕::弾幕Type type = 弾幕::直進;
					弾幕::弾幕色 color = 弾幕::ダイダイ;
					int speed = 0;
					if(flags[_T("Level")] == 0)
					{
						speed = 0;
					}
					else if(flags[_T("Level")] == 1)
					{
						speed = 1;
					}
					else if(flags[_T("Level")] == 2)
					{
						speed = 2;
					}
					弾幕::弾幕召喚(pland,//場所
									boss()->ValiableConstant2(),//HP
									CHARACTER_FORSE_ENEMY,//Forse
									speed,//speed
									type,//弾幕Type
									color,//弾幕色
									aspect,//aspect
									NULLCHARA//targetenemy
									);
				}
			}
			return true;

		}
		if(flags[_T("kakutou")] == 1)
		{
			flags[_T("kakutou")] = 0;
			pcLandform pland = valiable.lands[変数_攻撃場所];
			cCoordinate coo;
			if(pland)
			{
				coo.x = boss()->placeX - pland->placeX;
				coo.y = boss()->placeY - pland->placeY;


				if(coo.dif() <= 1)
				{
				
					EffectFunctions::ボスエフェクト集中(boss()->placeX, boss()->placeY, 0);
			
					int a;
					for(a=0;a<8;a++)
					{
						pcLandform pland = sg_pDungeonSystem->キャラ視点方角地形(boss(),a,1);
						if(pland->pOnChar != NULL)
						{
							pcCharacter pcharafuki = pland->pOnChar;
							sg_pDungeonSystem->投げ飛ばし要請(boss(),pcharafuki,
								sg_pDungeonSystem->隣接地形(pland, boss()->aspect + a, 10),
								 boss()->ValiableConstant1(), true, 2.0);
								 
						}
					}
				}

			}
			return true;
		}
		if(flags[_T("NoMove")] == 1)
		{
			flags[_T("NoMove")] = 0;

			{//目標点を向く
				pcLandform pland = valiable.lands[変数_攻撃場所];
				cCoordinate coo,mep,aspectcoo;
				int aspect;
				mep.x = boss()->placeX;
				mep.y = boss()->placeY;
				coo.x = pland->placeX;
				coo.y = pland->placeY;

				aspectcoo = coo-mep;

				if(aspectcoo.dif() != 0)
				{
					aspect = aspectcoo.GetAspect();
					sg_pDungeonSystem->方向転換要請(boss(), aspect);
				}
			}

			multimap<int,pcCharacter> sortMap;
			vector<pcCharacter> visibleCharalist;

			sg_pDungeonSystem->キャラクター目視可範囲キャラリスト(boss(),visibleCharalist);
			
			int i;
			for(i=0;i<visibleCharalist.size();i++)
			{
				if(boss()->pAI->u_敵隣接判定(visibleCharalist[i]))
				{
					sortMap.insert(pair<int,pcCharacter>(boss()->pAI->u_攻撃優先度_昇順(visibleCharalist[i]),visibleCharalist[i]));
				}
			}

			multimap<int,pcCharacter>::iterator itr = sortMap.begin();
			if(sortMap.size())
			{
				for(;itr != sortMap.end();itr++)
				{
					boss()->pAI->u_敵直接攻撃(itr->second);
					return true;
				}
			}
			return true;
		}
		if(flags[_T("Hakkoukakutou")] == 1)
		{
			flags[_T("Hakkoukakutou")] = 0;

			int i;
			for(i=0;i<3;i++)
			{
				pcLandform pland = sg_pDungeonSystem->キャラ視点方角地形(boss(),i*2 -2,1);
				if(sg_pDungeonSystem->キャラ配置安全(pland))
				{
					弾幕::弾幕Type type = 弾幕::追尾;
					弾幕::弾幕色 color = 弾幕::マゼンタ;
					弾幕::弾幕召喚(pland,//場所
								boss()->ValiableConstant2(),//HP
								CHARACTER_FORSE_ENEMY,//Forse
								1,//speed
								type,//弾幕Type
								color,//弾幕色
								boss()->aspect,//aspect
								NULLCHARA//targetenemy
								);
				}
			}
			return true;
		}
		return false;
	};
	virtual int 特殊攻撃_アニメ(cValiableField& valiable){return false;};
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable)
	{
		if(timing == ターン終了_タイミング)
		{
			{
				int hplevel = 0;
				double hpper = (double)boss()->HP / (double)boss()->MHP;
				hplevel = (1 - hpper) * 3;
				if(hplevel != flags[_T("Level")])
				{
					//フェイズチェンジ
					flags[_T("Level")] = hplevel;
					flags[_T("Aspect")] = (flags[_T("Aspect")] + 4) % 8;
					eraseDanmaku(boss());
					pcLandform pland = sg_pDungeonSystem->射程内障害物検索(
						boss()->足元地形(),
						flags[_T("Aspect")]+4,
						MAPHEIGHT);
					pland = sg_pDungeonSystem->隣接地形(pland,flags[_T("Aspect")],1);

					EffectFunctions::煙エフェクト1(boss()->足元地形()->placeX,boss()->足元地形()->placeY);
					sg_pDungeonSystem->ワープ要請(boss(),pland);
					EffectFunctions::煙エフェクト1(pland->placeX,pland->placeY);

				}
			}
			if(spellTurnCount() % 2 == 1)
			{
				int i,k;
				for(i=0;i<MAPWIDTH;i++)
				{
					if((spellTurnCount()/10) %2)
					{
						if(i%2)
						{
							continue;
						}
					}
					else
					{
						if((i+1)%2)
						{
							continue;
						}
					}
					int aspect = flags[_T("Aspect")];
					pcLandform pland = sg_pDungeonSystem->射程内障害物検索(
						sg_pDungeonSystem->Map().Land(i,MAPHEIGHT/2),
						aspect+4,
						MAPHEIGHT);
					pland = sg_pDungeonSystem->隣接地形(pland,aspect,2);

					if(sg_pDungeonSystem->キャラ配置安全(pland))
					{
						EffectFunctions::煙エフェクト1(pland->placeX,pland->placeY);

						弾幕::弾幕Type type = 弾幕::直進;
						弾幕::弾幕色 color = 弾幕::アカ;
						int speed = 0;
						if(flags[_T("Level")] == 0)
						{
							speed = 0;
						}
						else if(flags[_T("Level")] == 1)
						{
							speed = 1;
						}
						else if(flags[_T("Level")] == 2)
						{
							speed = 2;
						}
						弾幕::弾幕召喚(pland,//場所
										boss()->ValiableConstant2(),//HP
										CHARACTER_FORSE_ENEMY,//Forse
										speed,//speed
										type,//弾幕Type
										color,//弾幕色
										aspect,//aspect
										NULLCHARA//targetenemy
										);
					}
				}
			}
		}
	
	};
	
};


void cBoss_ID_0::giveUpEvent()
{
	
	EffectFunctions::ボスエフェクト撃破(me()->placeX,me()->placeY,0);
	cBoss::giveUpEvent();
	//sg_pDungeonSystem->強制退場要請( me(),false,false);
	//sg_pDungeonSystem->メニューを閉じる();
	//sg_pDungeonSystem->フロア更新要請();
}

pcEnemyAI cBoss_ID_0::Get_kindofAI()
{return pcEnemyAI(new ceaiActive);};
/*
bool cBoss_ID_0::loadSpell(int spellIndex)
{
	
	if(spellIndex == 0)
	{
		nowSpell_ = pcBossSpell(new cBossSpell_remilia1(
			NULLOFcScriptRLayer,
			tstring(_T("key")),
			me(),
			spellIndex));
	}
	else if(spellIndex == 1)
	{
		nowSpell_ = pcBossSpell(new cBossSpell_remilia2(
			NULLOFcScriptRLayer,
			tstring(_T("key")),
			me(),
			spellIndex));
	}
	else if(spellIndex == 2)
	{
		nowSpell_ = pcBossSpell(new cBossSpell_remilia3(
			NULLOFcScriptRLayer,
			tstring(_T("key")),
			me(),
			spellIndex));
	}
	return true;
}*/
void cBoss_ID_0::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	if(timing == ターン終了_タイミング)
	{
		map<tstring, int>::iterator itr = flags.find(_T("Init"));
		if(itr == flags.end())
		{
			flags[_T("Init")] = 1;
			sg_pDungeonSystem->強制ダメージ要請(me(),1,0,0);
		}
	}
}

//------------------------------------------------------------
//フラン
pcEnemyAI cBoss_ID_1::Get_kindofAI()
{return pcEnemyAI(new ceaiActive);};

int cBoss_ID_1::AI特殊攻撃選択(cValiableField& valiable)
{
	flags[_T("Danmaku")] = 0;
	cCoordinate coo(me()->placeX - sg_pDungeonSystem->pPlayerChara()->placeX, me()->placeY - sg_pDungeonSystem->pPlayerChara()->placeY);
	if(coo.dif() > 4)
	{
		flags[_T("Count")]++;
		if(flags[_T("Count")] > 4)
		{
			flags[_T("Count")] = 0;
			flags[_T("Danmaku")] = 1;
		}
		return true;
	}
	return false;
};
int cBoss_ID_1::特殊攻撃効果(cValiableField& valiable)
{
	if(flags[_T("Danmaku")] == 1)
	{
		flags[_T("Danmaku")] = 0;

		int i;
		for(i=0;i<8;i++)
		{

			pcLandform pland = sg_pDungeonSystem->Map().Land(me()->placeX, me()->placeY);
			pland = sg_pDungeonSystem->隣接地形(pland, i, 1);
			if(sg_pDungeonSystem->キャラ配置安全(pland))
			{
				EffectFunctions::煙エフェクト1(pland->placeX,pland->placeY);

				弾幕::弾幕Type type = 弾幕::追尾;
				弾幕::弾幕色 color = 弾幕::キイロ;
				弾幕::弾幕召喚(pland,//場所
								ValiableConstant2(),//HP
								CHARACTER_FORSE_ENEMY,//Forse
								0,//speed
								type,//弾幕Type
								color,//弾幕色
								i,//aspect
								NULLCHARA//targetenemy
								);
				
			}
			
		}
		
		return true;

	}
	return false;
};
void cBoss_ID_1::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	if(timing == ターン終了_タイミング)
	{
		map<tstring, int>::iterator itr = flags.find(_T("Init"));
		if(itr == flags.end())
		{
			flags[_T("Init")] = 1;
			flags[_T("Count")] = 0;
			//GameIdiom::高飛び(me(), sg_pDungeonSystem->RandomPlace());
		}
	}
}

//ボスてゐ
pcEnemyAI cBoss_ID_2::Get_kindofAI()
{return pcEnemyAI(new ceaiEscape);};


//ボス神奈子
pcEnemyAI cBoss_ID_3::Get_kindofAI()
{return pcEnemyAI(new ceaiActive);};

//ボスヨウム
pcEnemyAI cBoss_ID_4::Get_kindofAI()
{return pcEnemyAI(new ceaiActive);};

//ボスハンヨウム
pcEnemyAI cBoss_ID_5::Get_kindofAI()
{return pcEnemyAI(new ceaiActive);};

//ボス旧アリス
pcEnemyAI cBoss_ID_6::Get_kindofAI()
{return pcEnemyAI(new ceaiActive);};