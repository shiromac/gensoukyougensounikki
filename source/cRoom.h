#pragma once
#include "ccontrol.h"
#include "cLandform.h"

#include <vector>
#include <list>
#include <tstring_ph.h>


using namespace std;


class cRoom :
	public cControl
{
public:
	cRoom(void);
public:
	virtual ~cRoom(void);


	int Index;

	int PartyFlag;
	int ShopFlag;
	//地形領域リスト
	vector<pcLandform> LandformList;

	//地形領域リスト水路、浮島などの通常侵入不可領域
	vector<pcLandform> isolatedLandformList;
	
	//地形領域リスト水路、浮島などの通常侵入不可領域を含めたすべての地形
	vector<pcLandform> allLandformList();


	//地形隣接領域リスト//部屋の中でない
	vector<pcLandform> LandformNeighborList;

	//地形出口リスト
	vector<pcLandform> LandformExitList;

	//地形縁リスト//部屋の中
	vector<pcLandform> LandformEdgeList;
	//地形非縁リスト//部屋の中
	vector<pcLandform> LandformNonEdgeList;

	//NeighborListを作成しなおし
	void resetNeighborList();
	//出口を再検索
	//resetNeighborListをやってから();
	void resetExitList();

	//部屋自体が完全に他と切り離されている
	int allisolated;

	//地形リストの整合性を取る
	//地形のインデックスを優先する
	//resetNeighborListとresetExitListは行わない
	int stabilize();

	//部屋の統合
	//resetNeighborListとresetExitListは行わない
	int compose(cRoom& anotherroom);

	//Change
	int ChangeIndex(int toindex);

public:
	pcLandform plastputland;
};

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cRoom> pcRoom;