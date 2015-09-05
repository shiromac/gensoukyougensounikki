#pragma once
#include "ccontrol.h"
#include "cDrawingObject.h"



class cMiniMap :
	public cControl
{
public:
	cMiniMap(void);
public:
	virtual ~cMiniMap(void);



	IDirect3DTexture9* m_pTexture_miniMapchip;
	IDirect3DTexture9* m_pTexture_miniMapchip_land;


	void Init(IDirect3DDevice9 *pDev);

	//地形データ参照
	inline pcLandform Land(const int x,const int y);
	inline pcLandform Land(const cCoordinate& cood);


	//ミニマップを描く
	virtual int DrawMiniMap(IDirect3DDevice9 *pDev);
	

	
	//ミニマップを一度消して書き直すフラグをたてる。
	virtual int ResetMiniMap();
	
	//ミニマップを書き直すフラグをたてる。
	virtual int RefreshMiniMap();
	

	//ミニマップを消す
	virtual int EraseMiniMap(int FocusX, int FocusY);


public:
	//ミニマップ表示する
	virtual void ViewOn();

	//ミニマップ表示消す
	virtual void ViewOff();
protected:
	bool ViewFlag;


protected:

	//補助関数
	virtual int MiniMapDrawSub(IDirect3DDevice9 *pDev,int x,int y,int ui,int vi);
	

	//ミニマップを描きなおす
	virtual int ReDrawMiniMap(IDirect3DDevice9 *pDev);
	
	virtual cCoordinate miniMapLeftTop();
	virtual int miniMapObjectAlpha();
	
	
	virtual int DrawMiniMap_Land(IDirect3DDevice9 *pDev);
	virtual int DrawMiniMap_Land_Sub(IDirect3DDevice9 *pDev,cCoordinate lefttop,
		const int x,const int y,
		const int baseXi, const int baseYi,
		const int first_land, const int second_land,
		const double versep, const double horsep,
		int aspect = 1);
	virtual int DrawMiniMap_Object(IDirect3DDevice9 *pDev);


	//描いたかどうか
	inline int& MiniMapdrawed(int x,int y);
	inline int& MiniMapdrawed(const cCoordinate& cood);

	inline int MiniMapdrawed_edge(const int x,const int y,int aspect);
	inline int MiniMapdrawed_edge(const cCoordinate& cood,int aspect);


public:
	//マッピングされたかどうか
	inline int& MiniMapMapped_store(int x, int y);
	inline int& MiniMapMapped_store(const cCoordinate& cood);
	
	inline int MiniMapMapped_store_edge(const int x,const int y,int aspect);
	inline int MiniMapMapped_store_edge(const cCoordinate& cood,int aspect);

	void MiniMapMapped_list_refresh();
	
	const static int MAPPED_SEE = 1;
	const static int MAPPED_JUSTCHECK = 2;
protected:
	bool RefreshMiniMapFlag;
	bool ClearMiniMapFlag;

	cDrawingObject m_DO;

	cDrawingObject chip_DO;

	cRectObj MiniMapRange;//ミニマップのマッピングされた範囲

	vector<int> MiniMapdrawed_list;//ミニマップの描画されたリスト
	
	vector<int> MiniMapMapped_list_store;//ミニマップのマッピングされたリストのストア


	//マッピング歩いた
	virtual void private_mapping_just(const int x,const int y);
	//マッピング
	virtual void private_mapping(const int x,const int y);

public:


protected:
	//全オブジェクトリストストア
	virtual void StoreAllObjectdraw_list();

	virtual void PushBackMiniMapObjectdraw(const int x,const int y,const int ui,const int vi);
	virtual void GetMiniMapObjectdraw(const int index,int &x,int &y,int &ui,int &vi);

	vector<unsigned long> MiniMapObjectdraw_list;//ミニマップのオブジェクトのストア


	//マップ済み且未ドローストア
	virtual void PushBackMiniMapMappedAndNoDraw(const unsigned int x,const unsigned int y);
	virtual void GetMiniMapMappedAndNoDraw(const int index,int &x,int &y);

	vector<unsigned long> MiniMapMappedAndNoDraw_list;//ミニマップのオブジェクトのストア




public:




	//指定位置を通過済みマッピングする
	virtual int mapping_just(const int x, const int y);

	//指定位置（と周囲）をマッピングする
	virtual int mapping(const int x, const int y);
	//全マッピングする
	virtual int Allmapping();
	//指定位置のみマッピングする
	virtual int pointmapping(const int x, const int y);

	//ミニマップ表示範囲拡張
	virtual int minimapextend(const int x, const int y);
};

