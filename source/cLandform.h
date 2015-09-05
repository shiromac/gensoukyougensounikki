#pragma once
#include "cgameobject.h"
#include "cDrawingObject.h"

#include "utility\valiableField\cValiableField.h"


class cLandform;
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cLandform> pcLandform;
#include <boost/weak_ptr.hpp>
typedef boost::weak_ptr<cLandform> wpcLandform;

#include "timingEnum.h"
#include "cDroping.h"
#include "cCharacter.h"

#define ASPECT_UP 0
#define ASPECT_UPRIGHT 1
#define ASPECT_RIGHT 2
#define ASPECT_DOWNRIGHT 3
#define ASPECT_DOWN 4
#define ASPECT_DOWNLEFT 5
#define ASPECT_LEFT 6
#define ASPECT_UPLEFT 7

#define MAPKIND_WALL 0
#define MAPKIND_WALL2 1
#define MAPKIND_WALLEGDE 2
#define MAPKIND_FLOOR 3
#define MAPKIND_FLOOR2 4
#define MAPKIND_WATER 5
#define MAPKIND_FRAGILEWALL 6
#define MAPKIND_ABYSS 7

#define LANDFORM_FLAG_DARK 0
#define LANDFORM_FLAG_DANGER 1

#define LANDFORM_DECO_LAYERNUM 5

typedef int dChipTexPos;

//座標クラス
class cCoordinate
{
public:
	cCoordinate(void);
	cCoordinate(int ix, int iy);
public:
	virtual ~cCoordinate(void);

	int x;
	int y;

	void Set(int x_int,int y_int);

	//方向を得る。
	int GetAspect();

	//方向を与える。周囲８マスを得る
	void SetAspect(int aspect);

	//距離を得る(到達するために必要なターン)
	int dif();

	//-180 ~ 180 右回り正
	double calcuDegree(const cCoordinate& rother);

	void operator +=(const cCoordinate& rother);
	void operator -=(const cCoordinate& rother);
	cCoordinate& operator =(const cCoordinate& rother);
	bool operator ==(const cCoordinate& rother);
	cCoordinate operator +(const cCoordinate& rother);
	cCoordinate operator -(const cCoordinate& rother);
	cCoordinate operator *(const int rother);
	void operator *=(const int rother);
	cCoordinate operator /(const int rother);
};

inline int aspect_AngleAspect(int aspectA,int aspectB)
{
	int angle = aspectA - aspectB;
	angle = (-(angle/8)*8 + angle+8)%8;
	if(angle > 4) angle = 8 - angle;
	return angle;
}
inline int safeAspect(int aspect)
{
	return (-(aspect/8)*8 + aspect+8)%8;
}

//地形親クラス
class cLandform :
	public cGameObject
{
public:
	cLandform(void);
	cLandform(int x,int y);
public:
	virtual ~cLandform(void);


	//部屋番号（通路は-1）
	int RoomIndex;

	bool isRoom(){return (RoomIndex != -1);};

	//各フラグ
	void setDarkflag(bool flag){Darkflag_ = flag;};
	void setDangerflag(bool flag){Dangerflag_ = flag;};
	int getDarkflag(){return Darkflag_;};
	int getDangerflag(){return Dangerflag_;};

	//種別(見た目)
	const int kind_surface_visible();
	const int kind_surface();

	//飾り
	void process_decoration();
	const dChipTexPos getDecorationchip(int decorationlayer);
	const int getDecorationID(int decorationlayer);
	const int getDecorationDrawMode(int decorationlayer);
	int getDecorationLocation();//占有状況を得る

	void SetDecoration(dChipTexPos pos, int decorationID, int decorationlayer, int drawmode, int decorationanimeframe_min = 15, int decorationanimeframe_max = -1);
	
	void DeleteDecorationConditionID(int decorationID);
	const set<int>& getDecorationConditionID();

	int getDecorationMatchingFlag(){return kindFlag_;};//高速化のための関数
protected:
	int kind;
	SharedValiable_likeInt<int> visiblekind;

	//各フラグ
	UByte Darkflag_;
	UByte Dangerflag_;

	class cDecochip
	{
	public:
		cDecochip(void);
		virtual ~cDecochip(void);

		vector<dChipTexPos> decorationchip_;
		int decorationID_;
		int decorationanimecount_;
		int decorationanimenext_;
		int decorationdrawmode_;
		int decorationanimefase_;
		vector<pair<int,int>> decorationanimeframe_;

		virtual void process();
		virtual dChipTexPos getDecorationchip();
	} Deco_[LANDFORM_DECO_LAYERNUM];
	
	set<int> decorationConditionID_;//このマスを条件に使用している飾りのＩＤ
	
	virtual cDecochip& getdecochip(int decorationlayer);
	
	int kindFlag_;
public:
	//実際の属性を得る
	virtual int getAttribute();
	//種別に対する属性設定
	virtual int setAttribute(int kind);
	SharedValiable_likeInt<int>::Pointer private_visiblekind_pointer(){return &visiblekind;};

	//見た目のリセット
	virtual int visibleReset();

	//つるはしなどで破壊可、不可
	int breakable();

	//乾き可
	int driable();

	//通り抜け可、不可
	int through(int waterwalk,int wallwalk, int airwalk);

	//アイテム配置可、不可
	int canputdrop();

	//ななめ通り抜け可、不可
	int diagonthrough(int waterwalk,int wallwalk, int airwalk);

	//壁属性
	int iswall();

	//水属性
	int iswater();

	//乗り込み可、不可
	int caninto(int waterwalk,int wallwalk, int airwalk);

	//マッピング済み
	int mapped;
	//minimap描画済み
	//int minimapDrawed;

	//上に乗ってる物
	pcDroping pOnDrop;
	pcCharacter pOnChar;

	pcDroping mountedDroping(){return pOnDrop;};
	pcCharacter mountedCharacter(){return pOnChar;};

	//座標
	cCoordinate place;

	//隣接マスの種別
	int visibleneighbor(int aspect);
	int neighbor(int aspect);



	virtual bool 設置可(){return true;};


	virtual void CutIn(タイミング timing, cValiableField& valiable);
	
protected:
	void Constractsub(void);

};




//nullの地形
#define NULLLAND pcLandform((cLandform*)NULL)