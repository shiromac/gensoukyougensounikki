#pragma once

//長方形オブジェクト
class cRectObj
{
public:
	cRectObj(void);
public:
	virtual ~cRectObj(void);

	virtual void Events();

	double CenterX;
	double CenterY;
	double Width;
	double Height;

	//形を変えずにそろえる。
	double setLeft(double Left);
	double setRight(double Right);
	double setTop(double Top);
	double setBottom(double Bottom);



	//当然形が変わります
	void setLTRB(double Left, double Top, double Right, double Bottom);
		
	//形を変えてそろえる。
	double formLeft(double L);
	double formRight(double R);
	double formTop(double T);
	double formBottom(double B);


	inline double Left()
	{
		return CenterX - Width/2;
	}
	inline double Right()
	{
		return CenterX + Width/2;
	}
	inline double Top()
	{
		return CenterY - Height/2;
	}
	inline double Bottom()
	{
		return CenterY + Height/2;
	}

	cRectObj operator =(const cRectObj& rother);//=演算子
};
