#include "cCurveTrack.h"

#include <math.h>

cCurveTrack::cCurveTrack(void)
{

	pointN = 0;
	Loop = true;
	stable = false;
}

cCurveTrack::~cCurveTrack(void)
{
	TrackPoint.clear();

}

void cCurveTrack::stabilizeTrack()
{
	if(stable) return;//すでに固定されていたら適応しない
	stable = true;

	if(Loop)
	{
		//寄り点を作成（0と最後の中間）
		TrackPoint.push_back(TrackPoint[2*pointN]/2 + TrackPoint[0]/2);
	}
	else
	{
		//寄り点を作成（最後の点）
		TrackPoint.push_back(TrackPoint[2*pointN]);
	}

}

bool cCurveTrack::addPoint(c4DVector point)
{
	if(stable) return false;//固定されていたら適応しない
	c4DVector v;
	if(pointN == 0)
	{
		TrackPoint.push_back(-point);//反対の点
	}
	TrackPoint.push_back(TrackPoint[2*pointN]/2 + point/2);//中間点
	TrackPoint.push_back(point);//2*pointNに寄り点
	pointN++;
	return true;
}

bool cCurveTrack::addPoint(c4DVector point1,c4DVector point2)
{
	addPoint(point1);
	return addPoint(point2);
}
bool cCurveTrack::addPoint(c4DVector point1,c4DVector point2,c4DVector point3)
{
	addPoint(point1);
	addPoint(point2);
	return addPoint(point3);
}
bool cCurveTrack::addPoint(c4DVector point1,c4DVector point2,c4DVector point3,c4DVector point4)
{
	addPoint(point1);
	addPoint(point2);
	addPoint(point3);
	return addPoint(point4);
}

bool cCurveTrack::setLoop(bool flag)
{
	if(stable) return false;//固定されていたら適応しない
	Loop = flag;
	return true;
}


c4DVector cCurveTrack::get(const double rother)
{
	c4DVector v;
	if(pointN == 0) return v;//空だったら(0,0,0,0)を返す
		
	unsigned int index1=0,index2=0,index3=0;

	double t = rother;
	if (t < 0) t = 0;//tは0または正の値

	int i = (int)t;//整数部
	double a = t - i;//比、tの小数部が入る。

	if(stable)
	{//安定化されてる
		//-------------------------------------
		//高速（推奨）
		//-------------------------------------
		if(Loop)
		{
			i = (i % (pointN+1));//適正な値に変換
			//t = i + a;

		}
		else
		{
			if(t > pointN)
			{
				t = pointN;
				a = 0;
			}
	
			//iの値をtについての適正な値に変換
			//i = (int)t;
		}
		if(i < 0) i = 0;


		//高速化のための小細工
		int temp_a = (i+1)*2;
		int temp_b = (pointN+1)*2;

		index1 = (temp_a - 1);//modする必要なしのはず
		index2 = (temp_a) % (temp_b);
		index3 = (temp_a + 1) % (temp_b);

		//高速化のための小細工
		double temp_c = (1-a);

		if((pointN+1)*2 <= index1 || index1 < 0)
		{
			index1 = 0;
		}

		v = ((temp_c)*(temp_c))*TrackPoint[index1] + (2*(temp_c)*a)*TrackPoint[index2] + (a*a)*TrackPoint[index3];

	}
	else
	{//安定化されてない！
		//-------------------------------------
		//遅いので通常は使わないこと！
		//-------------------------------------
		if(Loop)
		{
			i = (i % (pointN+1));//適正な値に変換
			//t = i + a;
			//一時的に仮の寄り点を作成（0と最後の中間）
			TrackPoint.push_back(TrackPoint[2*pointN]/2 + TrackPoint[0]/2);
		}
		else
		{
			if(t > pointN)
			{
				t = pointN;
				a = 0;
			}
			//一時的に仮の寄り点を作成（最後の点）
			TrackPoint.push_back(TrackPoint[2*pointN]);

			//iの値をtについての適正な値に変換
			//i = (int)floor(t);
		}
		if(i < 0) i = 0;

		//高速化のための小細工
		int temp_a = (i+1)*2;
		int temp_b = (pointN+1)*2;

		index1 = (temp_a - 1);//modする必要なしのはず
		index2 = (temp_a) % (temp_b);
		index3 = (temp_a + 1) % (temp_b);

		v = ((1-a)*(1-a))*TrackPoint[index1] + (2*(1-a)*a)*TrackPoint[index2] + (a*a)*TrackPoint[index3];

		//一時的な寄り点を削除
		TrackPoint.pop_back();

	}
	return v;

}



c4DVector cCurveTrack::getSpeed(const double rother)
{
	c4DVector v;
	if(pointN == 0) return v;//空だったら(0,0,0,0)を返す
		
	int index1,index2,index3;

	double t = rother;
	if (t < 0) t = 0;//tは0または正の値

	int i = (int)t;//整数部
	double a = t - i;//比、tの小数部が入る。

	if(stable)
	{//安定化されてる
		//-------------------------------------
		//高速（推奨）
		//-------------------------------------
		if(Loop)
		{
			i = (i % (pointN+1));//適正な値に変換
			t = i + a;

		}
		else
		{
			if(t > pointN)
			{
				t = pointN;
				a = 0;
			}
	
			//iの値をtについての適正な値に変換
			i = (int)t;
		}
		if(i < 0) i = 0;

		//高速化のための小細工
		int temp_a = (i+1)*2;
		int temp_b = (pointN+1)*2;

		index1 = (temp_a - 1);//modする必要なしのはず
		index2 = (temp_a) % (temp_b);
		index3 = (temp_a + 1) % (temp_b);


		v = 2*( (a-1)*TrackPoint[index1] + (1-2*a)*TrackPoint[index2] + (a)*TrackPoint[index3] );
	}
	else
	{//安定化されてない！
		//-------------------------------------
		//遅いので通常は使わないこと！
		//-------------------------------------
		if(Loop)
		{
			i = (i % (pointN+1));//適正な値に変換
			t = i + a;
			//一時的に仮の寄り点を作成（0と最後の中間）
			TrackPoint.push_back(TrackPoint[2*pointN]/2 + TrackPoint[0]/2);
		}
		else
		{
			if(t > pointN)
			{
				t = pointN;
				a = 0;
			}
			//一時的に仮の寄り点を作成（最後の点）
			TrackPoint.push_back(TrackPoint[2*pointN]);

			//iの値をtについての適正な値に変換
			i = (int)t;
		}


		//高速化のための小細工
		int temp_a = (i+1)*2;
		int temp_b = (pointN+1)*2;

		index1 = (temp_a - 1);//modする必要なしのはず
		index2 = (temp_a) % (temp_b);
		index3 = (temp_a + 1) % (temp_b);

		v = 2*( (a-1)*TrackPoint[index1] + (1-2*a)*TrackPoint[index2] + (a)*TrackPoint[index3] );

		//一時的な寄り点を削除
		TrackPoint.pop_back();

	}
	return v;

}


void cCurveTrack::deleteCurveTrack()
{
	TrackPoint.clear();
	pointN = 0;
	stable = false;
}


int cCurveTrack::pointNum()
{
	return pointN;
}