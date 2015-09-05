#pragma once

#include "4DVector.h"

#include <vector>
using namespace std;


//-------------------------------------------------------------
//(0,0,0,0)から始まる曲線を作成する。
//初期点((0,0,0,0))が０
//寄り点１から２の中点が１
//寄り点２から３の中点が２　である。
//（注：寄り点上は通らない）
//+++++++++++++++++++++++++++++++++++++++++++
//  stabilizeTrack();を忘れないこと！！！
//(stabilizeTrackするとdeleteCurveTrackするまでトラックの変更が出来なくなる)
//+++++++++++++++++++++++++++++++++++++++++++
//-------------------------------------------------------------
class cCurveTrack
{
public:
	cCurveTrack(void);
public:
	virtual ~cCurveTrack(void);

	//寄り点を加える
	bool addPoint(c4DVector point);
	bool addPoint(c4DVector point1,c4DVector point2);
	bool addPoint(c4DVector point1,c4DVector point2,c4DVector point3);
	bool addPoint(c4DVector point1,c4DVector point2,c4DVector point3,c4DVector point4);

	//ループするかのフラグ
	bool setLoop(bool flag);

	//------------------------------------------
	//トラックを固定する。高速化に必須！
	//------------------------------------------
	void stabilizeTrack();

	//ここに得たいタイミングの値を入れる
	//0または正の値
	c4DVector get(const double rother);

	//ここに得たいタイミングの値を入れる
	//0または正の値
	c4DVector getSpeed(const double rother);

	//値を全て破棄し初期化
	void deleteCurveTrack();

	int pointNum();//入れたポイントの数
private:
	bool Loop;//ループするかのフラグ
	bool stable;//固定フラグ
	int pointN;//入れたポイントの数
	vector<c4DVector> TrackPoint;
};
