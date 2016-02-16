#pragma once

#include "utility\cRectObj.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "cMap.h"

#include <vector>
#include <list>


enum DungeonCase
{
	RANDOMDIVIDE = 0,//ランダム分割
	SIMPLEGRID,//シンプルグリッド
	BIGRINGANDGRID,//大きな輪外通路
	NETANDANCHOR,//ネットと錘
	HABANDSATELLITE,//ハブと衛星
	BIGONEROOM,//大部屋
	TWOROOM,//2部屋
	FOURROOM,//4部屋
	RANDOMGROW,//ランダム成長
	RANDOMGRIDUNION,//グリッドランダム合体
};

const tstring DungeonCaseStr[] = {
	_T("RANDOMDIVIDE"),
	_T("SIMPLEGRID"),
	_T("BIGRINGANDGRID"),
	_T("NETANDANCHOR"),
	_T("HABANDSATELLITE"),
	_T("BIGONEROOM"),
	_T("TWOROOM"),
	_T("FOURROOM"),
	_T("RANDOMGROW"),
	_T("RANDOMGRIDUNION")
};

using namespace std;

class cDMMap;
typedef boost::shared_ptr<cDMMap> pcDMMap;
typedef boost::weak_ptr<cDMMap> wpcDMMap;

class cDMLand
{
public:
	cDMLand();
	~cDMLand(){};
	c4DVector pos;


	int rectFrag;
	int roomIndex;
	int wallroundIndex;

	int WallofrectIndex;
	int WallofroomIndex;
	int WallofwallroundIndex;
	int WallofWay;

	int WallofWater;

	int Landflag;

	virtual pcLandform ConcreteLand();
	virtual void OptimazeLand();

	virtual void Init();
};
typedef boost::shared_ptr<cDMLand> pcDMLand;

class cDMRect;
typedef boost::shared_ptr<cDMRect> pcDMRect;
typedef boost::weak_ptr<cDMRect> wpcDMRect;


class cDMWallround;
typedef boost::shared_ptr<cDMWallround> pcDMWallround;
typedef boost::weak_ptr<cDMWallround> wpcDMWallround;

class cDMRoom;
typedef boost::shared_ptr<cDMRoom> pcDMRoom;
typedef boost::weak_ptr<cDMRoom> wpcDMRoom;

//第4過程
class cDMWallround
{
public:
	cDMWallround(wpcDMMap wpcdmmap)
	{
		wpmap_ = wpcdmmap;
		int i;
		for(i=0;i<8;i++)
		{
			directionFlag[i] = false;
		}
		linkcheck = false;
	};
	~cDMWallround(){};

	//壁際地形(部屋内でつながっていることが保障される)
	vector<pcDMLand> land_;
	//壁領域
	vector<pcDMLand> wall_;
	//出口候補地形（壁の外）
	vector<pcDMLand> walloutland_;


	//並び替え不可
	//出口地形
	vector<pcDMLand> exit_;
	//出口地形の行き先
	vector<pcDMLand> exitto_;



	//壁際地形番号
	int WallroundIndex;

	//つなぎ
	vector<wpcDMWallround> link_;
	//つなぎ可能性
	vector<wpcDMWallround> linkpos_;


	wpcDMMap wpmap_;
	wpcDMRoom wproom_;

	int directionFlag[8];

	int linkcheck;


	//仕様変更　出口は部屋の外
	pcDMLand introExit(cCoordinate center);

	bool introExit_check(cCoordinate center, pcDMLand land, int aspect_lean);


	//領域を検索して処理する
	virtual void WallroundOptimaze(pcDMLand pland);
};



//第3過程
class cDMRoom :
	public cRectObj
{
public:
	cDMRoom(wpcDMMap wpcdmmap){wpmap_ = wpcdmmap;};
	~cDMRoom(){};

	wpcDMRoom wpSelfroom_;

	//壁沿い領域
	vector<pcDMWallround> wallround_;

	//壁沿い領域
	pcDMWallround direction_wallround_[8];
	virtual pcDMWallround direction_wallround(double x,double y);

	//部屋領域
	vector<pcDMLand> land_;


	//cDMRoom内つながり保障フラグ
	int	NeverDivide;

	//どういう部屋なのか
	int RoomMode;

	//領域番号//ユニーク
	int RectIndex;

	//部屋番号
	int RoomIndex;

	//宴会
	int PartyFlag;

	//店
	int ShopFlag;

	wpcDMMap wpmap_;


	pcDMRect pArea;

	//通常部屋
	virtual void RoomOptimaze_Normal(vector<double>& Val);

	//通路
	virtual void RoomOptimaze_Way(vector<double>& Val);

	//水路二分部屋
	virtual void RoomOptimaze_Div2water(vector<double>& Val);

	//水路四分部屋
	virtual void RoomOptimaze_Div4water(vector<double>& Val);
	
	//水路四分部屋
	virtual void RoomOptimaze_Oval(vector<double>& Val);
	
	//ノイズ円部屋
	virtual void RoomOptimaze_NoizeOval(vector<double>& Val);



	//水路系
	virtual void makeWaterWay_inRoom(pcDMLand landA, pcDMLand landB);


};
typedef boost::shared_ptr<cDMRoom> pcDMRoom;
typedef boost::weak_ptr<cDMRoom> wpcDMRoom;

//第一過程
class cDMRect :
	public cRectObj
{
public:
	cDMRect(wpcDMMap wpcdmmap){wpmap_ = wpcdmmap;};
	~cDMRect(){};



	//用途フラグ
	int Flag_;

	//cDMRect内つながり保障フラグ
	int	NeverDivide;

	wpcDMMap wpmap_;

	wpcDMRoom wproom_;
};



//大元
class cDMMap
{
public:
	cDMMap(){};
	~cDMMap(){};

	vector<pcDMRect> Rects_;
	vector<pcDMRoom> Room_;//通路分岐を含む。


	//絶対につなげたいペア
	vector<pair<pcDMRect,pcDMRect>> mustlink;
	
	//重複リンクをさけたいペア
	vector<pair<pcDMRect,pcDMRect>> nevermultilink;

protected:
	vector<pcDMLand> landmap_;

	pcDMLand landmap_edge;

	//自分自身
	wpcDMMap selfwp_;

	//最終的に作られる部屋の数
	int RoomNum;//Room_.size()ではない！注意！

	vector<pair<pcDMWallround, pcDMWallround>> linkpair_;


public:
	virtual pcDMLand landmap(int x, int y);

	virtual void Init(pcDMMap selfpointer);
	virtual void Reset();
	virtual void ResetSub_Lands();

	//**********************************************************
	//第一過程
	//！保障！部屋が一つ以上作れる
	virtual void MakeRect(int divideMode, vector<double>& divideVal);

	//ディバイドループ
	int MakeRect_RandomDivideRoop_rectcount;
	virtual void MakeRect_RandomDivideRoop(vector<double>& divideVal);
	virtual void MakeRect_RandomDivideRoop_split(pcDMRect prect, vector<double>& divideVal);

	//でっかいリング
	virtual void MakeRect_BigringAndgrid(vector<double>& divideVal);

	//網と錘（井）
	virtual void MakeRect_NetAndAnchor(vector<double>& divideVal);

	//ハブと衛星
	virtual void MakeRect_HabAndSatellite(vector<double>& divideVal);

	//マス
	virtual void MakeRect_SimpleGrid(vector<double>& divideVal);

	//大部屋
	virtual void MakeRect_BigOneRoom(vector<double>& divideVal);

	//二部屋
	virtual void MakeRect_TwoRoom(vector<double>& divideVal);

	//四部屋
	virtual void MakeRect_FourRoom(vector<double>& divideVal);

	//ランダム成長
	virtual void MakeRect_RandomGrow(vector<double>& divideVal);

	//ランダムグリッド統合
	virtual void MakeRect_RandomGridUnion(vector<double>& divideVal);

	//**********************************************************
	//第二過程
	//！保障！部屋が一つ以上ある
	virtual void RectOptimaze(vector<double>& Val);

	//第三過程
	virtual void Makeroom(vector<double>& Val);

	//第四過程
	virtual void RoomOptimaze(vector<double>& Val);
	
	//第五過程
	virtual void Makelink(vector<double>& Val);

	//第六過程
	virtual void MakeWater(vector<double>& Val);

	//第六.5過程
	virtual void MakeAbyss(vector<double>& Val);

	//第七過程
	virtual void MakeParty(vector<double>& Val);

	//第八過程
	virtual void MakeShop(vector<double>& Val);

	//link
	virtual void Makelink_sub(pcDMRoom roomA, pcDMRoom roomB, int multiwaypos);
	virtual void Makelink_sub(pcDMWallround roundA, pcDMWallround roundB, int multiwaypos);
	virtual void Makelink_likecheckroop(pcDMWallround round);
	
	//第五過程
	virtual void makeWay(vector<double>& Val);
	virtual void makeWay_sub(pcDMWallround roundA, pcDMWallround roundB);
	//二角検索
	virtual int makeWay_sub_nikaku(pcDMLand landA, pcDMLand landB, int aspect,int makeWayFlag = true);
	//最短路検索(最終手段)
	virtual void makeWay_sub_least(pcDMWallround roundA, pcDMWallround roundB, int ModeWay = 0);


	virtual void makeWay_sub(pcDMLand landA, pcDMLand landB, int aspect);
	virtual pcDMLand makeWay_sub(pcDMLand land, int aspect, int Num);
	virtual void makeWay_line(pcDMLand land, pcDMLand land2);




	//水路系
	virtual pcDMLand makeWater_sub(pcDMLand land, int aspect, int Num);
	virtual pcDMLand makeWater_sub(pcDMLand land);
	virtual void makeWater_line(pcDMLand land, pcDMLand land2);

	virtual void makeWater_Opt_way(int percent);
	virtual void makeWater_Opt_mach(int percent);
	virtual void makeWater_Opt_max();

	virtual void makeWater_sub_least(pcDMLand landA, pcDMLand landB);
	virtual void makeWater_sub_pound(pcDMLand land, int size);


	//奈落系
	virtual pcDMLand makeAbyss_sub(pcDMLand land, int aspect, int Num);
	virtual pcDMLand makeAbyss_sub(pcDMLand land);
	virtual void makeAbyss_line(pcDMLand land, pcDMLand land2);

	virtual void makeAbyss_Opt_way(int num);
	virtual void makeAbyss_Opt_mach(int percent);

	virtual void makeAbyss_sub_least(pcDMLand landA, pcDMLand landB);
	virtual void makeAbyss_sub_pound(int percent);


	virtual void Output();

	//---------------------------------------------------------
	//固定系
	//---------------------------------------------------------
	virtual void decode_floor(const vector<vector<tstring>>& vvstr);

};

