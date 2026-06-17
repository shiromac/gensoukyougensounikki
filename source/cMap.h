#pragma once
#include "ccontrol.h"
#include "cRoom.h"
#include "cLandform.h"
#include "cDrawingObject.h"

#include "cMinimap.h"

#include <vector>
#include <list>
#include <tstring_ph.h>


using namespace std;

class cFloor;
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
typedef boost::shared_ptr<cFloor> pcFloor;
typedef boost::weak_ptr<cFloor> wpcFloor;

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cRectObj> pcRectObj;

#define MAPDRAWCENTERX 366
#define MAPDRAWCENTERY 310

#define MAPTEXBOXSIZE 32
#define MAPTEXPOWER 2

#define MAPWIDTH (50)
#define MAPHEIGHT (50)

#define EDGELANDWIDTH (5)

class cMapChipObject;
typedef boost::shared_ptr<cMapChipObject> pcMapChipObject;
class cMapChipAnimeAll;
typedef boost::shared_ptr<cMapChipAnimeAll> pcMapChipAnimeAll;

//----------------------------------------------------
//ここから迷路作成アルゴリズム用
//----------------------------------------------------


struct s_rect {
  boolean done_split_v;
  boolean done_split_h;
  cRectObj rect;
  pcRectObj room;
};
typedef boost::shared_ptr<s_rect> ps_rect;

struct s_couple {
  int v_or_h;
  ps_rect rect0, rect1; 
};
typedef boost::shared_ptr<s_couple> ps_couple;
//----------------------------------------------------
//ここまで迷路作成アルゴリズム用
//----------------------------------------------------
class cMap :
	public cControl
{
public:
	cMap(void);
public:
	virtual ~cMap(void);

	//画面上にある
	virtual bool isOnDisplay(double x, double y, int margin = 0);
	
	//同じ部屋か
	virtual bool isOnSameRoom(pcLandform la, pcLandform lb);

	//同じ部屋もしくは出口または隣接
	virtual bool isOnSameRoomOrExit(pcLandform from, pcLandform to);

	//可視範囲か
	virtual bool isOnSight(pcLandform from, pcLandform to, int sightrange = 1, int torime = 0);

	//画面上での位置
	virtual c4DVector visibleplace(c4DVector place);
	//画面上主人公の移動を考慮した実際の位置
	virtual c4DVector realvisibleplace(c4DVector place);

	virtual void setChipset(const tstring& chipsetID);

	virtual void DecorationAll(const tstring& chipsetID);
	virtual void ReDecoration(const tstring& chipsetID, vector<pcLandform> changeLand);

	cRenderTexture* m_pTexture_Dark;

	static const int DECORATIONLAYER_NULL = -1;
	static const int DECORATIONLAYER_LAND1 = 0;
	static const int DECORATIONLAYER_LAND2 = 1;
	static const int DECORATIONLAYER_LAND3 = 2;
	static const int DECORATIONLAYER_SKY = 3;
	static const int DECORATIONLAYER_SKY2 = 4;

	vector<pcLandform> changedLand;

	pcLandform edgeLand(){return EdgeLand;};
protected:
	//地形関連データ
	vector<pcLandform> LandListOfSqr;	
	vector<pcLandform> landrandam;	
	vector<double> skyDecoAlpha_;
	virtual double& skyDecoAlpha(int x,int y);

	//定義域以外の地形
	pcLandform EdgeLand;


	cRenderTexture* m_pTexture_Mapchip;
	cRenderTexture* m_pTexture_Decochip;
	//cRenderTexture* m_pTexture_miniMapchip;
	//飾り関連データ
	vector<pcMapChipObject> DecorationObject;	
	vector<int> DecorationNum_;

	pcMapChipAnimeAll mapChipAnimeAll;

	cMiniMap Minimap;
public:

	//ミニマップ表示する
	virtual void MinimapViewOn(){Minimap.ViewOn();};

	//ミニマップ表示消す
	virtual void MinimapViewOff(){Minimap.ViewOff();};


	//初期化
	virtual void Init(cRenderDevice *pDev);

	virtual void process();

	//再初期化
	virtual void Renew(cRenderDevice *pDev);

	//部屋リスト
	vector<pcRoom> RoomList;
protected:
	double visibleForcusX_;
	double visibleForcusY_;

	//エフェクト用
	double additinal_visibleForcusX_;
	double additinal_visibleForcusY_;
public:
	//画面の位置
	double& additinal_visibleForcusX();
	double& additinal_visibleForcusY();

	const double visibleForcusX();
	const double visibleForcusY();
	
	void setVisibleForcusX(double x);
	void setVisibleForcusY(double y);

	double ForcusX();
	double ForcusY();

	//描く
	virtual int Draw(cRenderDevice *pDev);
	//描く
	virtual int DrawDecoLand(cRenderDevice *pDev);
	//描く
	virtual int DrawGridLand(cRenderDevice *pDev,const double alpha, const double forcus_alpha);
	virtual int DrawGridDrop(cRenderDevice *pDev,const double alpha);
	virtual int DrawGridChara(cRenderDevice *pDev,const double alpha);
	//描く
	virtual int DrawDecoSky(cRenderDevice *pDev);

	//暗闇描く
	virtual int DrawDark(cRenderDevice *pDev, pcCharacter  hero);
	//暗闇描く補助関数
	virtual int DrawDarkSub(cRenderDevice *pDev, pcCharacter  hero, pcLandform heroplace, double opaque);


	//--------------------------------------------
	//ミニマップ系
	//--------------------------------------------
	//ミニマップを描く
	virtual int DrawMiniMap(cRenderDevice *pDev);
	
	//ミニマップを一度消して書き直すフラグをたてる。
	virtual int ResetMiniMap();
	
	//ミニマップを書き直すフラグをたてる。
	virtual int RefreshMiniMap();
	
	//ミニマップを消す
	virtual int EraseMiniMap(int FocusX, int FocusY);

	//--------------------------------------------
	//マッピング系(ミニマップ系に含む)
	//--------------------------------------------
	//指定位置を通過済みマッピングする
	virtual int mapping_just(const int x, const int y);

	//指定位置（と周囲）をマッピングする
	virtual int mapping(const int x, const int y);
	//全マッピングする
	virtual int Allmapping();
	//指定位置のみマッピングする
	virtual int pointmapping(const int x, const int y);

	//マップの表示をリセット（主人公の周りだけ）
	virtual void MapvisibleReset();

	//マップの表示をリセット
	virtual void AllMapvisibleReset();
public:







protected:
	//チップを書く
	virtual int Drawchip(cRenderDevice *pDev, const int x,const int y);
	virtual int DrawchipSub(cRenderDevice *pDev,cCoordinate lefttop,
		const int x,const int y,
		const int baseXi,const int baseYi,
		const int first_land,const int second_land,
		const double versep,const double horsep);
	cDrawingObject m_DO;
	virtual int DrawDeco(cRenderDevice *pDev, const int x, const int y, const int decoLayer);

public:

	//地形データ参照
	inline pcLandform Land(const int x,const int y);
	inline pcLandform Land(const cCoordinate& cood);

	//隣接地形データ再設定
	virtual int resetLandNeighbor(const int x,const int y);
	virtual int resetAllLandNeighbor();


//----------------------------------------------------
//ここから迷路作成アルゴリズム用
//----------------------------------------------------
	//新しい迷路作成
	virtual int makeNewDungeon(pcFloor pfloor);


protected:
	//区画
	vector<ps_rect> MD_rectList;
	//部屋
	vector<pcRectObj> MD_roomList;
	//通路
	vector<pcRectObj> MD_lineList;
	//ペア(奇数と偶数のペア)
	vector<ps_couple> MD_pairList;

	//水路
	vector<pcRectObj> MD_waterlineList;

	//新規区画
	virtual ps_rect MD_rect_add(int lx, int ly, int hx, int hy);

	//新規部屋領域
	virtual pcRectObj MD_room_add(int lx, int ly, int hx, int hy);

	//新規ペア
	virtual ps_couple MD_couple_add(int v_or_h, ps_rect rect0, ps_rect rect1);

	//区画区切り
	virtual void MD_rect_split(ps_rect rect_parent);

	//部屋作成
	virtual void MD_room_make();

	//水路作成
	virtual void MD_water_make();

	//ペア追加
	virtual void MD_couple_more();

	//通路作成
	virtual void MD_pass_make();

	virtual void MD_pass_link(cCoordinate start,cCoordinate end, vector<pcRectObj>* linelist = NULL);

	//データの最適化
	virtual void MD_data_optimize();

};

inline pcLandform cMap::Land(const int x,const int y)
{
	pcLandform pcland;
	if(x<0||x>=MAPWIDTH||y<0||y>=MAPHEIGHT)
		pcland = EdgeLand;
	else
		pcland = LandListOfSqr[y*MAPWIDTH + x];
	return pcland;
};
inline pcLandform cMap::Land(const cCoordinate& cood)
{
	pcLandform pcland;
	if(cood.x<0||cood.x>=MAPWIDTH||cood.y<0||cood.y>=MAPHEIGHT)
		pcland = EdgeLand;
	else
		pcland = LandListOfSqr[cood.y*MAPWIDTH + cood.x];
	return pcland;
};


