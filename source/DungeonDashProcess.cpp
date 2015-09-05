#include "stdafx.h"

#include "cDungeonSystem.h"
#include "cLandform.h"
#include "utility/debug.h"
#include "cTrap.h"

#define DASHSWITCH_ACTION 1
#define DASHSWITCH_INTELLI 2

vector<vector<int>> g_SDashWeight;
int setSDashWeight(const int x, const int y, const int goal_x, const int goal_y, const int onlyInitFlag, const int fastFlag)
{
	if(g_SDashWeight.empty())
	{//初期化
		vector<int> v_int(MAPHEIGHT,INT_MAX);//十分大きな値が初期値
		g_SDashWeight.assign(MAPWIDTH,v_int);
	}
	deque<c4DVector> que;

	cMap& map = sg_pDungeonSystem->Map();


	int roomindex = sg_pDungeonSystem->pPlayerChara()->足元地形()->RoomIndex;
	if(roomindex == -1)
	{//通路
		return false;
	}
	int i;
	//初期化
	vector<pcLandform> vpland = map.RoomList[roomindex]->allLandformList();
	for(i=0;i<vpland.size();i++)
	{
		g_SDashWeight[vpland[i]->placeX][vpland[i]->placeY] = INT_MAX;
	}
	vpland = map.RoomList[roomindex]->LandformNeighborList;
	for(i=0;i<vpland.size();i++)
	{
		g_SDashWeight[vpland[i]->placeX][vpland[i]->placeY] = INT_MAX;
	}

	if(onlyInitFlag) return false;


	c4DVector v;
	v.set(x, y, 0, 0);
	que.push_back(v);
	g_SDashWeight[v.x][v.y] = v.z;


	int reachGoal = 0;

	pcLandform pland;
	c4DVector newstate;
	cCoordinate coo,dis;
	int a;

	c4DVector state;
	int addcost = 0;
	//int count = 0;

	for(;;)
	{
		if(que.empty()) break;
		if(que.size() > 4000)
		{//無限ループ防止
			OnAssert(_T(__FILE__),__LINE__,false,_T("makeWay_sub_least関数内で異常に大きなキューが発生しました。"));
			break;
		}
		//count++;

		state = que[0];
		que.pop_front();

		if(g_SDashWeight[state.x][state.y] < state.z)
		{//コストもっと小さいのがある。
			continue;
		}
		

		int aspect;
		for(aspect=0;aspect<8;aspect++)
		{

			a = aspect*2 + aspect/4;
			coo.SetAspect(a);


			newstate.set( state.x + coo.x, state.y + coo.y ,0,0);
			pland = map.Land(newstate.x,newstate.y);

			dis.x = newstate.x - goal_x;
			dis.y = newstate.y - goal_y;

			if(fastFlag && dis.dif() >= (dis-coo).dif()) continue;
		
			if(newstate.x < 1 || newstate.x >= MAPWIDTH-1
				|| newstate.y < 1 || newstate.y >= MAPHEIGHT-1)
			{//端っこ
				continue;
			}
			if(pland->RoomIndex == -1)
			{//
				continue;
			}
			if(!sg_pDungeonSystem->isCanNeighberThrough(pland,a+4,
				sg_pDungeonSystem->pPlayerChara()->水上歩行(),sg_pDungeonSystem->pPlayerChara()->壁中歩行(), sg_pDungeonSystem->pPlayerChara()->空中歩行()))//ただし壁の中はダメージがあるので通らない
			{//	通れないよ
				continue;
			}
			if(pland->iswall())
			{//壁だ！
				continue;
			}
			if(goal_x == newstate.x && goal_y == newstate.y)
			{//ゴール
				reachGoal = 1;
				continue;
			}

			newstate.z = state.z;//現在コスト
			
			addcost = 0;

			if(pland->pOnDrop && pland->pOnDrop->isDiscover() && pland->pOnDrop->属性.count(落ち物属性::罠))
			{//	ワナ
				addcost = 10000;
			}
			else if(pland->getAttribute() == MAPKIND_FLOOR &&
				!(pland->mapped & cMiniMap::MAPPED_JUSTCHECK))
			{//	部屋ワナ未チェック
				addcost = 100;
			}
			else
			{//そのた
				addcost = 0;
			}

			if(a%2)
			{
				addcost += 1;//斜め
			}

			addcost += 1000000;

			newstate.z += addcost;

			if(newstate.z < g_SDashWeight[newstate.x][newstate.y])//現在コスト
			{//コストが小さい
				g_SDashWeight[newstate.x][newstate.y] = newstate.z;
				if(!reachGoal)que.push_back(newstate);
			}
			
		}
		
	}


	return reachGoal;

}

//ダッシュしている時trueを返す
int cDungeonSystem::Dashprocess(IDirect3DDevice9 *pDev)
{
	int i;

	int Dashed = false;


	if(DashSwitch())
	{
		do
		{

			//------------------------------
			//ここからダッシュ終了条件
	
			//メッセージがある
			if(MessageW().isStockingMassage())
			{
				DashSwitch() = 0;
				DashStopDelay() = 30;
				break;
			}
			
			
			//敵が増えるor敵に隣接
			/*
			vector<pcCharacter> pcharalist;
			キャラクター目視可範囲キャラリスト(pPlayerChara(),pcharalist);
			int enemycount = 0;
			int enemynearcount = 0;
			UByte Dangerflag = 0;
			for(i=0;i<pcharalist.size();i++)
			{
				if(pcharalist[i]->Forse == CHARACTER_FORSE_ENEMY)
				{
					enemycount++;
				}
			}
			//敵が隣接
			for(i=0;i<pcharalist.size();i++)
			{
				if(pcharalist[i]->Forse == CHARACTER_FORSE_ENEMY)
				{
					if((pcharalist[i]->足元地形()->place - pPlayerChara()->足元地形()->place).dif() <= 1)
					{
						enemynearcount += 1 << 16;
					}
				}
			}
			int x,y;
			for(x=pPlayerChara()->足元地形()->placeX-4;x<=pPlayerChara()->足元地形()->placeX+4;x++)
			{
				for(y=pPlayerChara()->足元地形()->placeY-4;y<=pPlayerChara()->足元地形()->placeY+4;y++)
				{
					if(pPlayerChara()->足元地形()->RoomIndex != -1 && Map().Land(x,y)->RoomIndex != -1 && (Map().Land(x,y)->getDarkflag() || Map().Land(x,y)->getDangerflag() ))
					{
						Dangerflag++;
						break;
					}
				}
				if(Dangerflag) break;
			}
			*/
			
			//一歩進む
			int aspect;
			/*
			cCoordinate coo,mep,aspectcoo;
			mep.x = pPlayerChara()->placeX;
			mep.y = pPlayerChara()->placeY;
			
			coo = DashTGT();
			aspectcoo.x = ((coo-mep).x)?(coo-mep).x/abs((coo-mep).x):0;
			aspectcoo.y = ((coo-mep).y)?(coo-mep).y/abs((coo-mep).y):0;
			
			if(pPlayerChara()->足元地形()->RoomIndex == -1)
			{//通路
				//nasi
			}
			else
			{
				int a;
				if(g_SDashWeight.empty())
				{
					setSDashWeight(mep.x,mep.y,
						mep.x,mep.y,true,true);
				}
				int min = g_SDashWeight[mep.x][mep.y];//今の地形
				int index = aspectcoo.GetAspect();
				cCoordinate nextcoo;
				for(a=index;a<index+8;a++)
				{
					nextcoo.SetAspect(a);
					aspectcoo = nextcoo + mep;
					if(g_SDashWeight[aspectcoo.x][aspectcoo.y] < min)
					{
						min = g_SDashWeight[aspectcoo.x][aspectcoo.y];
						index = a;
					}
				}
				aspectcoo.SetAspect(index);
			}
			aspect = aspectcoo.GetAspect();
			*/

			aspect = DashAspectCulcurate();

			int tempdata;
			DashRiskCulcurate(tempdata,DashAspectCulcurate());

			if((DashEnemyCount() & 0xFFFF) < (tempdata & 0xFFFF)
				|| (DashEnemyCount() & 0x7FFF0000) < (tempdata & 0x7FFF0000))
			{
				DashEnemyCount() = tempdata;
				DashSwitch() = 0;
				DashStopDelay() = 200000000;
				break;
			}
			DashEnemyCount() = tempdata;


			//足元に落ち物がある
			if(pPlayerChara()->足元() != NULL)
			{
				DashSwitch() = 0;
				DashStopDelay() = 30;
				break;
			}

			//ダッシュボタンを離す
			if(DashSwitch() == DASHSWITCH_ACTION
				&& !g_pPlayerInput()->dash().on)
			{
				DashSwitch() = 0;
				break;
			}

			//前方または横に見える落ち物がある
			if(DashSwitch() == DASHSWITCH_ACTION)
			{
				for(i=-2;i<=2;i++)
				{
					if(キャラ視点方角地形(pPlayerChara(),i,1)->pOnDrop != NULL
						&& キャラ視点方角地形(pPlayerChara(),i,1)->pOnDrop->isDiscover()
						)
					{
						DashSwitch() = 0;
						DashStopDelay() = 0;
						break;
					}
				}
			}
			else if(DashSwitch() == DASHSWITCH_INTELLI)
			{
				cCoordinate coo,mep,aspectcoo;
				int aspect;
				mep.x = pPlayerChara()->placeX;
				mep.y = pPlayerChara()->placeY;
				coo = DashTGT();

				aspectcoo.x = ((coo-mep).x)?(coo-mep).x/abs((coo-mep).x):0;
				aspectcoo.y = ((coo-mep).y)?(coo-mep).y/abs((coo-mep).y):0;

				aspect = aspectcoo.GetAspect() - pPlayerChara()->aspect;
				if(キャラ視点方角地形(pPlayerChara(),aspect,1)->pOnDrop != NULL
					&& キャラ視点方角地形(pPlayerChara(),aspect,1)->pOnDrop->isDiscover())
				{
					DashSwitch() = 0;
					break;
				}
		
			}


			//危険な状態
			if(pPlayerChara()->Condition.飛ばし危険())
			{
				DashSwitch() = 0;
				DashStopDelay() = 60;
				break;
			}



			//部屋の入り口
			if(pPlayerChara()->足元地形()->RoomIndex == -1//通路
				&& キャラ前地形(pPlayerChara(),1)->RoomIndex != -1)
			{
				DashSwitch() = 0;
				DashStopDelay() = 0;
				break;
			}

			//部屋の入り口２
			if((pPlayerChara()->足元地形()->RoomIndex != -1)
				&&(
				 (キャラ視点方角地形(pPlayerChara(),4,1)->RoomIndex == -1)
					&&	隣接地形移動可(pPlayerChara(),pPlayerChara()->aspect+4)
				))
			{
				DashSwitch() = 0;
				DashStopDelay() = 30;
				break;
			}


			//部屋の出口
			if((pPlayerChara()->足元地形()->RoomIndex != -1)
				&&(
				 (キャラ視点方角地形(pPlayerChara(),0,1)->RoomIndex == -1)
					&&	隣接地形移動可(pPlayerChara(),pPlayerChara()->aspect)
					&& isCanNeighberInto(pPlayerChara()->足元地形(), pPlayerChara()->aspect, 0, 0, 0)
				|| (キャラ視点方角地形(pPlayerChara(),1,1)->RoomIndex == -1)
					&&	隣接地形移動可(pPlayerChara(),pPlayerChara()->aspect+1)
					&& isCanNeighberInto(pPlayerChara()->足元地形(), pPlayerChara()->aspect+1, 0, 0, 0)
				|| (キャラ視点方角地形(pPlayerChara(),-1,1)->RoomIndex == -1)
					&&	隣接地形移動可(pPlayerChara(),pPlayerChara()->aspect-1)
					&& isCanNeighberInto(pPlayerChara()->足元地形(), pPlayerChara()->aspect-1, 0, 0, 0)
				|| (キャラ視点方角地形(pPlayerChara(),2,1)->RoomIndex == -1)
					&&	隣接地形移動可(pPlayerChara(),pPlayerChara()->aspect+2)
					&& isCanNeighberInto(pPlayerChara()->足元地形(), pPlayerChara()->aspect+2, 0, 0, 0)
				|| (キャラ視点方角地形(pPlayerChara(),-2,1)->RoomIndex == -1)
					&&	隣接地形移動可(pPlayerChara(),pPlayerChara()->aspect-2)
					&& isCanNeighberInto(pPlayerChara()->足元地形(), pPlayerChara()->aspect-2, 0, 0, 0)
				))
			{
				DashSwitch() = 0;
				DashStopDelay() = 0;
				break;
			}

			//通路分岐点系
			if((pPlayerChara()->足元地形()->RoomIndex == -1) &&
				((
					(隣接地形移動可(pPlayerChara(),pPlayerChara()->aspect+2)
					&& isCanNeighberInto(pPlayerChara()->足元地形(), pPlayerChara()->aspect+2, 0, 0, 0)
					)
				&&!(
					隣接地形移動可(pPlayerChara(),pPlayerChara()->aspect+3)
					&& isCanNeighberInto(pPlayerChara()->足元地形(), pPlayerChara()->aspect+3, 0, 0, 0)
					)
				)//右側 
				|| (
					(隣接地形移動可(pPlayerChara(),pPlayerChara()->aspect-2)
					&& isCanNeighberInto(pPlayerChara()->足元地形(), pPlayerChara()->aspect-2, 0, 0, 0)
					)
				&&!(隣接地形移動可(pPlayerChara(),pPlayerChara()->aspect-3)
					&& isCanNeighberInto(pPlayerChara()->足元地形(), pPlayerChara()->aspect-3, 0, 0, 0)
					)
				)//左側
				)
				&&
				(
					(
						隣接地形移動可(pPlayerChara(),pPlayerChara()->aspect)
						&& isCanNeighberInto(pPlayerChara()->足元地形(), pPlayerChara()->aspect, 0, 0, 0)
					)
				|| 
					(隣接地形移動可(pPlayerChara(),pPlayerChara()->aspect-1)
					&& isCanNeighberInto(pPlayerChara()->足元地形(), pPlayerChara()->aspect-1, 0, 0, 0)
					)
				|| (
						隣接地形移動可(pPlayerChara(),pPlayerChara()->aspect+1)
						&& isCanNeighberInto(pPlayerChara()->足元地形(), pPlayerChara()->aspect+1, 0, 0, 0)
					)
				))
			{
				DashSwitch() = 0;
				DashStopDelay() = 30;
				break;
			}


			if(DashSwitch() == DASHSWITCH_INTELLI)
			{//インテリ終了条件
				cCoordinate mep;

				mep.x = pPlayerChara()->placeX;
				mep.y = pPlayerChara()->placeY;
			
				if(mep == DashTGT())
				{
					DashSwitch() = 0;
					break;
				}

				//目標点
				if(!g_SDashWeight.empty() && g_SDashWeight[mep.x][mep.y] == 0)
				{
					DashSwitch() = 0;
					break;
				}
			}
			else
			{
				//方向ボタンが押されていない
				if(!(g_pPlayerInput()->migi().on ||
					g_pPlayerInput()->hidari().on ||
					g_pPlayerInput()->ue().on ||
					g_pPlayerInput()->shita().on))
				{
					DashSwitch() = 0;
					break;
				}

			}

			//何らかのキーを押す
			if(g_pPlayerInput()->anykey().justOn)
			{
				DashSwitch() = 0;
				DashStopDelay() = 30;
				break;
			}

		}while(0);

		if(!DashSwitch())
		{
			return false;
		}
	}

	if(DashSwitch() == 0)
	{//ダッシュ処理系


		/*
		else if(((g_pPlayerInput()->migi().justOn && forDoubleInput_migi() < 16) ||
			(g_pPlayerInput()->hidari().justOn && forDoubleInput_hidari() < 16) ||
			(g_pPlayerInput()->ue().justOn && forDoubleInput_ue() < 16) ||
			(g_pPlayerInput()->shita().justOn && forDoubleInput_shita() < 16) )
			&&
			((g_pPlayerInput()->X == forDoubleInput_X())
			|| (g_pPlayerInput()->Y == forDoubleInput_Y()))
			&&
			(g_pPlayerInput()->turn().on)
			)*/
		if((g_pPlayerInput()->turn().on && g_pPlayerInput()->dash().on || 
			g_pPlayerInput()->smartdash().on)
			&&( g_pPlayerInput()->migi().justOn ||
				g_pPlayerInput()->hidari().justOn ||
				g_pPlayerInput()->ue().justOn ||
				g_pPlayerInput()->shita().justOn) )
		{//インテリジェンスダッシュ
			DashSwitch() = DASHSWITCH_INTELLI;
			
			//方向を定める
			cCoordinate tempcoo;
			tempcoo.x = g_pPlayerInput()->X;
			tempcoo.y = g_pPlayerInput()->Y;
			DashAspect() = tempcoo.GetAspect();

			//---------------------------------
			//目標点作成
			//---------------------------------
			if(pPlayerChara()->足元地形()->RoomIndex == -1)
			{//通路
				//pcLandform pland = 障害物検索(pPlayerChara()->足元地形(), pPlayerChara()->aspect, false);
				pcLandform pland = 障害物検索(pPlayerChara()->足元地形(), DashAspect(), false);
				
				DashTGT() = pland->place;
			
			}
			else
			{//部屋
				//pcLandform pland = 障害物検索(pPlayerChara()->足元地形(), pPlayerChara()->aspect, false);
				pcLandform pland = 障害物検索(pPlayerChara()->足元地形(), DashAspect(), false);

				int distmin = INT_MAX;
				int roomIndex = pPlayerChara()->足元地形()->RoomIndex;
				vector<pcLandform> plandlist = Map().RoomList[roomIndex]->LandformList;
				for(i=0;i<plandlist.size();i++)
				{//目標検索１
					cCoordinate coo;
					coo = plandlist[i]->place - pPlayerChara()->足元地形()->place;
					bool isTrapSafe = false;
					if(plandlist[i])
					{
						pcTrap ptrap = boost::dynamic_pointer_cast<cTrap>(plandlist[i]);
						if(ptrap)
						{
							isTrapSafe = !ptrap->罠用_効果目標か(pPlayerChara());
						}
					}
					if(((coo.GetAspect() == DashAspect())
						&& (distmin > coo.dif() && coo.dif() > 0))
						&&
						(//以下目標条件
							plandlist[i]->pOnDrop != NULL//アイテムがある
							&&( plandlist[i]->pOnDrop->属性.count(落ち物属性::アイテム)
								||plandlist[i]->pOnDrop->属性.count(落ち物属性::階段)//アイテムか階段
								||(isTrapSafe)
								)
							&& plandlist[i]->pOnDrop->isDiscover()//アイテムが見えてる
						))
					{
						pland = plandlist[i];
						distmin = coo.dif();
					}
				}
				plandlist = Map().RoomList[roomIndex]->LandformExitList;
				for(i=0;i<plandlist.size();i++)
				{//目標検索２
					cCoordinate coo;
					coo = plandlist[i]->place - pPlayerChara()->足元地形()->place;
					if((coo.GetAspect() == DashAspect())
						&& (distmin > coo.dif()*2 && coo.dif() > 0))
					{
						pland = plandlist[i];
						distmin = coo.dif()*2;
					}
				}
				
				for(i=0;i<plandlist.size();i++)
				{//目標検索３
					cCoordinate coo;
					coo = plandlist[i]->place - pPlayerChara()->足元地形()->place;
					if(((coo.GetAspect() == safeAspect(DashAspect()+1) ||
						coo.GetAspect() == safeAspect(DashAspect()-1))
						&& (distmin > coo.dif()*8 && coo.dif() > 0))//重み8倍
						)
					{
						pland = plandlist[i];
						distmin = coo.dif()*8;//重み8倍
					}
				}
				plandlist = Map().RoomList[roomIndex]->LandformList;
				for(i=0;i<plandlist.size();i++)
				{//目標検索４
					cCoordinate coo;
					coo = plandlist[i]->place - pPlayerChara()->足元地形()->place;
					if(((coo.GetAspect() == safeAspect(DashAspect()+1) ||
						coo.GetAspect() == safeAspect(DashAspect()-1))
						&& (distmin > coo.dif()*8 && coo.dif() > 0))
						&&
						(//以下目標条件
							plandlist[i]->pOnDrop != NULL//アイテムがある
							&&( plandlist[i]->pOnDrop->属性.count(落ち物属性::アイテム)||plandlist[i]->pOnDrop->属性.count(落ち物属性::階段))//アイテムか階段
							&& plandlist[i]->pOnDrop->isDiscover()//アイテムが見えてる
						))
					{
						pland = plandlist[i];
						distmin = coo.dif()*8;
					}
				}

				DashTGT() = pland->place;
				int onlyInitFlag = (INT_MAX == distmin);
				if(onlyInitFlag)
				{
					setSDashWeight(pland->placeX,pland->placeY,
						pPlayerChara()->placeX,pPlayerChara()->placeY,onlyInitFlag,true);
				}
				else
				{
					if(!setSDashWeight(pland->placeX,pland->placeY,
						pPlayerChara()->placeX,pPlayerChara()->placeY,onlyInitFlag,true))
					{
						setSDashWeight(pland->placeX,pland->placeY,
						pPlayerChara()->placeX,pPlayerChara()->placeY,onlyInitFlag,false);
					}
				}
			}

		}
		else if(g_pPlayerInput()->dash().on)
		{//通常ダッシュ

			if((g_pPlayerInput()->migi().on ||
				g_pPlayerInput()->hidari().on ||
				g_pPlayerInput()->ue().on ||
				g_pPlayerInput()->shita().on) &&
				(g_pPlayerInput()->X || g_pPlayerInput()->Y))
			{
				if((g_pPlayerInput()->migi().justOn ||
					g_pPlayerInput()->hidari().justOn ||
					g_pPlayerInput()->ue().justOn ||
					g_pPlayerInput()->shita().justOn) &&
					(g_pPlayerInput()->X || g_pPlayerInput()->Y))
				{
					DashStopDelay() = 0;
				}
				if(DashStopDelay() > 0)
				{
					DashStopDelay()--;
				}
				if(DashStopDelay() <= 0)
				{
					DashSwitch() = DASHSWITCH_ACTION;
					
					//方向を定める
					cCoordinate coo;
					coo.x = g_pPlayerInput()->X;
					coo.y = g_pPlayerInput()->Y;
					DashAspect() = coo.GetAspect();

					pcCharacter pchara = Map().Land(pPlayerChara()->placeX + coo.x, pPlayerChara()->placeY + coo.y)->pOnChar;
					if(pchara != NULL && すれ違い要請(pPlayerChara(),DashAspect()))
					{
						DashSwitch() = 0;
						return true;
					}
				}
			}
		}
		
		if(g_pPlayerInput()->migi().justOn) forDoubleInput_migi() = 0;
		if(g_pPlayerInput()->hidari().justOn) forDoubleInput_hidari() = 0;
		if(g_pPlayerInput()->ue().justOn) forDoubleInput_ue() = 0;
		if(g_pPlayerInput()->shita().justOn) forDoubleInput_shita() = 0;
		forDoubleInput_migi()++;
		forDoubleInput_hidari()++;
		forDoubleInput_ue()++;
		forDoubleInput_shita()++;


		if(g_pPlayerInput()->X)
		{
			forDoubleInput_X() = g_pPlayerInput()->X;
		}
		if(g_pPlayerInput()->Y)
		{
			forDoubleInput_Y() = g_pPlayerInput()->Y;
		}


		if(DashAspect())
		{
			//敵の数を数えましょう
			/*
			vector<pcCharacter> pcharalist;
			キャラクター目視可範囲キャラリスト(pPlayerChara(),pcharalist);
			int enemycount = 0;
			int enemynearcount = 0;
			UByte Dangerflag = 0;
			for(i=0;i<pcharalist.size();i++)
			{
				if(pcharalist[i]->Forse == CHARACTER_FORSE_ENEMY)
				{
					enemycount++;
				}
			}
			//敵が隣接
			for(i=0;i<pcharalist.size();i++)
			{
				if(pcharalist[i]->Forse == CHARACTER_FORSE_ENEMY)
				{
					if((pcharalist[i]->足元地形()->place - pPlayerChara()->足元地形()->place).dif() <= 1)
					{
						enemynearcount += 1 << 16;
					}
				}
			}
			int x,y;
			for(x=pPlayerChara()->足元地形()->placeX-4;x<=pPlayerChara()->足元地形()->placeX+4;x++)
			{
				for(y=pPlayerChara()->足元地形()->placeY-4;y<=pPlayerChara()->足元地形()->placeY+4;y++)
				{
					if(pPlayerChara()->足元地形()->RoomIndex != -1 && Map().Land(x,y)->RoomIndex != -1 && (Map().Land(x,y)->getDarkflag() || Map().Land(x,y)->getDangerflag() ))
					{
						Dangerflag++;
						break;
					}
				}
				if(Dangerflag) break;
			}


			DashEnemyCount() = Dangerflag + enemycount + enemynearcount;
			*/
			DashRiskCulcurate(DashEnemyCount(),DashAspect());
		}
	}




	if(DashSwitch() == DASHSWITCH_ACTION)
	{//解決
		//進むとあぷない
		/*
		vector<pcCharacter> pcharalist;
		キャラクター目視可範囲キャラリスト(pPlayerChara(),pcharalist);
		//敵が隣接
		cCoordinate coo;
		coo.SetAspect(DashAspect());
		for(i=0;i<pcharalist.size();i++)
		{
			if(pcharalist[i]->Forse == CHARACTER_FORSE_ENEMY && !pcharalist[i]->Condition.行動不能である())
			{
				if((pcharalist[i]->足元地形()->place - pPlayerChara()->足元地形()->place - coo).dif() <= 1)
				{
					DashSwitch() = 0;
					break;
				}
			}
		}
		*/

		//方向転換を許す
		int aspect = DashAspect();
		cCoordinate aspectcoo(g_pPlayerInput()->X, g_pPlayerInput()->Y);
		if(aspectcoo.dif() != 0)
		{
			aspect = aspectcoo.GetAspect();
		}

		if(DashSwitch())
		{
			//一歩進む
			if(isCanNeighberInto(pPlayerChara()->足元地形(), aspect, 0, 0, 0))
			{
				if(泥棒確認移動可_主人公専用(pPlayerChara(),aspect) && 移動要請(pPlayerChara(),aspect))
				{
					現在地からマッピング(sg_pDungeonSystem->pPlayerChara());
					Dashed = true;
				}
				else
				{
					DashSwitch() = 0;
				}
			}
			else
			{
				DashSwitch() = 0;
			}
		}
	}
	else if(DashSwitch() == DASHSWITCH_INTELLI)
	{//インテリ解決
		//一歩進む
		int aspect;
		/*
		cCoordinate coo,mep,aspectcoo;
		
		mep.x = pPlayerChara()->placeX;
		mep.y = pPlayerChara()->placeY;
		
		coo = DashTGT();
		aspectcoo.x = ((coo-mep).x)?(coo-mep).x/abs((coo-mep).x):0;
		aspectcoo.y = ((coo-mep).y)?(coo-mep).y/abs((coo-mep).y):0;
		
		if(pPlayerChara()->足元地形()->RoomIndex == -1)
		{//通路
			//nasi
		}
		else
		{
			int a;
			if(g_SDashWeight.empty())
			{
				setSDashWeight(mep.x,mep.y,
					mep.x,mep.y,true,true);
			}
			int min = g_SDashWeight[mep.x][mep.y];//今の地形
			int index = aspectcoo.GetAspect();
			cCoordinate nextcoo;
			for(a=index;a<index+8;a++)
			{
				nextcoo.SetAspect(a);
				aspectcoo = nextcoo + mep;
				if(g_SDashWeight[aspectcoo.x][aspectcoo.y] < min)
				{
					min = g_SDashWeight[aspectcoo.x][aspectcoo.y];
					index = a;
				}
			}
			aspectcoo.SetAspect(index);
		}
		aspect = aspectcoo.GetAspect();
		*/
		aspect = DashAspectCulcurate();

		/*
		//進むとあぷない
		vector<pcCharacter> pcharalist;
		キャラクター目視可範囲キャラリスト(pPlayerChara(),pcharalist);
		//敵が隣接
		coo.SetAspect(DashAspect());
		for(i=0;i<pcharalist.size();i++)
		{
			if(pcharalist[i]->Forse == CHARACTER_FORSE_ENEMY && !pcharalist[i]->Condition.行動不能である())
			{
				if((pcharalist[i]->足元地形()->place - pPlayerChara()->足元地形()->place - aspectcoo).dif() <= 1)
				{
					DashSwitch() = 0;
					break;
				}
			}
		}
		*/
		if(DashSwitch())
		{
			pPlayerChara()->anime_stamp_step += DEFAULTSTAMPSUMSTEP/8;
			do
			{

				if(泥棒確認移動可_主人公専用(pPlayerChara(),aspect))
				{
					if(隣接地形移動可(pPlayerChara(),aspect) && isCanNeighberInto(pPlayerChara()->足元地形(), aspect, 0, 0, 0))
					{
						if(移動要請(pPlayerChara(),aspect))
						{
							現在地からマッピング(pPlayerChara());
							Dashed = true;
						}
						else
						{
							DashSwitch() = 0;
						}
						break;
					}

					int a = 1;
					if( aspect_AngleAspect(DashAspect() ,aspect+1) > aspect_AngleAspect(DashAspect() ,aspect-1) )
					{
						a = -1;
					}

					
					if(隣接地形移動可(pPlayerChara(),aspect+a) && isCanNeighberInto(pPlayerChara()->足元地形(), aspect+a, 0, 0, 0))
					{
						if(移動要請(pPlayerChara(),aspect+a))
						{
							現在地からマッピング(pPlayerChara());
							Dashed = true;
						}
						else
						{
							DashSwitch() = 0;
						}
						break;
					}

					if(隣接地形移動可(pPlayerChara(),aspect-a) && isCanNeighberInto(pPlayerChara()->足元地形(), aspect-a, 0, 0, 0))
					{
						if(移動要請(pPlayerChara(),aspect-a))
						{
							現在地からマッピング(pPlayerChara());
							Dashed = true;
						}
						else
						{
							DashSwitch() = 0;
						}
						break;
					}
				}
				else
				{
					break;
				}


				//インテリ再発条件
				int through = 0;
				for(i=-3;i<=3;i++)
				{
					through += (隣接地形移動可(pPlayerChara(),pPlayerChara()->aspect+i) && isCanNeighberInto(pPlayerChara()->足元地形(), aspect+i, 0, 0, 0));
				}

				if(through == 1)
				{//一本道
					for(i=-3;i<=3;i++)
					{
						if(隣接地形移動可(pPlayerChara(),pPlayerChara()->aspect+i) && isCanNeighberInto(pPlayerChara()->足元地形(), aspect+i, 0, 0, 0))
						{
							DashTGT() = 障害物検索(pPlayerChara()->足元地形(), safeAspect(pPlayerChara()->aspect + i), false)->place;
							DashAspect() = pPlayerChara()->aspect+i;
							移動要請(pPlayerChara(),pPlayerChara()->aspect + i);
							現在地からマッピング(pPlayerChara());
							Dashed = true;
							break;
						}
					}
					if(i<=3) break;
				}

				

				DashSwitch() = 0;

			}while(0);

		}
	}

	if(DashSwitch())
	{//ダッシュ処理



		//------------------------------
		//ここからダッシュ終了条件
	
		do
		{



		}while(0);

	}





	if(Dashed)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int cDungeonSystem::DashAspectCulcurate()
{

	if(DashSwitch() == DASHSWITCH_ACTION)
	{//解決
		return DashAspect();
	}
	else if(DashSwitch() == DASHSWITCH_INTELLI)
	{//インテリ解決
		//一歩進む
		cCoordinate coo,mep,aspectcoo;
		mep.x = pPlayerChara()->placeX;
		mep.y = pPlayerChara()->placeY;
		
		coo = DashTGT();
		aspectcoo.x = ((coo-mep).x)?(coo-mep).x/abs((coo-mep).x):0;
		aspectcoo.y = ((coo-mep).y)?(coo-mep).y/abs((coo-mep).y):0;
		
		if(pPlayerChara()->足元地形()->RoomIndex == -1)
		{//通路
			//nasi
		}
		else
		{
			int a;
			if(g_SDashWeight.empty())
			{
				setSDashWeight(mep.x,mep.y,
					mep.x,mep.y,true,true);
			}
			int min = g_SDashWeight[mep.x][mep.y];//今の地形
			int index = aspectcoo.GetAspect();
			cCoordinate nextcoo;
			for(a=index;a<index+8;a++)
			{
				nextcoo.SetAspect(a);
				aspectcoo = nextcoo + mep;
				if(g_SDashWeight[aspectcoo.x][aspectcoo.y] < min)
				{
					min = g_SDashWeight[aspectcoo.x][aspectcoo.y];
					index = a;
				}
			}
			aspectcoo.SetAspect(index);
		}
		return aspectcoo.GetAspect();
	}

	return DashAspect();
}

int cDungeonSystem::DashRiskCulcurate(int& output, int nextaspect)
{
	int i;

	//敵が増えるor敵に隣接
	vector<pcCharacter> pcharalist;
	キャラクター目視可範囲キャラリスト(pPlayerChara(),pcharalist);
	UByte enemycount = 0;
	UByte enemynearcount = 0;
	UByte Dangerflag = 0;
	UByte MaxHp = 0;
	
	for(i=0;i<pcharalist.size();i++)
	{
		if(pcharalist[i]->Forse == CHARACTER_FORSE_ENEMY)
		{
			enemycount++;
		}
	}
	//敵が隣接
	for(i=0;i<pcharalist.size();i++)
	{
		if(pcharalist[i]->Forse == CHARACTER_FORSE_ENEMY)
		{
			if((pcharalist[i]->足元地形()->place - pPlayerChara()->足元地形()->place).dif() <= 1)
			{
				enemynearcount += 8;
			}
		}
	}

	int x,y;
	for(x=pPlayerChara()->足元地形()->placeX-4;x<=pPlayerChara()->足元地形()->placeX+4;x++)
	{
		for(y=pPlayerChara()->足元地形()->placeY-4;y<=pPlayerChara()->足元地形()->placeY+4;y++)
		{
			if(pPlayerChara()->足元地形()->RoomIndex != -1 && Map().Land(x,y)->RoomIndex != -1 && (Map().Land(x,y)->getDarkflag() || Map().Land(x,y)->getDangerflag() ))
			{
				Dangerflag++;
				break;
			}
		}
		if(Dangerflag) break;
	}

	if(pPlayerChara()->HP == pPlayerChara()->MHP)
	{
		MaxHp = 1;
	}

	//進むとあぷない
	pcharalist.clear();
	キャラクター目視可範囲キャラリスト(pPlayerChara(),pcharalist);
	//敵が隣接
	cCoordinate coo;
	coo.SetAspect(nextaspect);
	for(i=0;i<pcharalist.size();i++)
	{
		if(pcharalist[i]->Forse == CHARACTER_FORSE_ENEMY && !pcharalist[i]->Condition.行動不能である())
		{
			if((pcharalist[i]->足元地形()->place - pPlayerChara()->足元地形()->place - coo).dif() <= 1)
			{
				enemynearcount += 1;
			}
		}
	}


	output = Dangerflag+enemycount+ (((int)enemynearcount)<<8) + (((int)MaxHp)<<16);


	return output;
}

double cDungeonSystem::DashSpeed()
{
	if(DashSwitch() == DASHSWITCH_ACTION)
	{//解決
		return 4;
	}
	else if(DashSwitch() == DASHSWITCH_INTELLI)
	{//インテリ解決
		return 8;
	}

	return 1;
}