
#ifndef INC_VECTOR_HEADER_so301fkgmr83s_INCLUDED
#define INC_VECTOR_HEADER_so301fkgmr83s_INCLUDED

#define V_PI (3.141592653589793) //円周率

enum dimension_No {DIM_X=0,DIM_Y=1,DIM_Z=2,DIM_T=3};

class c4DVector 
{
	public:


	c4DVector();//コンストラクタ
	c4DVector(const double valueX,const double valueY,const double valueZ,const double valueT);//コンストラクタ
	~c4DVector();//デストラクタ

	
	double x;//x軸座標
	double y;//y軸座標
	double z;//z軸座標
	double t;//t軸座標

	c4DVector& operator =(const c4DVector& rother);//=演算子
	void operator +=(const c4DVector& rother);//+=演算子
	void operator -=(const c4DVector& rother);//-=演算子
	void operator *=(const double& rother);//*=演算子
	void operator /=(const double& rother);///=演算子
	bool operator ==(const c4DVector& rother);//==演算子
	bool operator !=(const c4DVector& rother);//!=演算子
	c4DVector operator +(const c4DVector& rother);//+演算子
	c4DVector operator -(const c4DVector& rother);//-演算子
	c4DVector operator *(const double& rother);//*演算子(定数掛け算　ベクトル＊定数)
	c4DVector operator /(const double& rother);///演算子(定数割り算)
	c4DVector operator -();//単項-演算子(マイナス)
	//c4DVector operator ~();//単項~演算子(デバッグ用)
	double operator *(const c4DVector& rother);//*演算子(内積)
	double*operator ()(const dimension_No& rother);//()演算子(要素それぞれDIM_X:x,DIM_Y:y,DIM_Z:z,DIM_T:tのアドレスを返す)
	//（それ以外の数字はヌルをかえす）(あまりお勧めしません）

	void turn(const double degree,const dimension_No first_d,const dimension_No second_d);//(_d:次元 要素DIM_X:x,DIM_Y:y,DIM_Z:z,DIM_T:t)
	void turn(const double degree);
	//ベクトルをxy平面上で(degree)度回転させる。（度数法）
	//例えばベクトルvをxy平面上で30度回転させたいときは
	//v.turn(30);
	//と書く。
	//また、
	//ベクトルを(first_d)(second_d)平面上で(degree)度回転させることもできる。（度数法）
	//例えばベクトルvをyt平面上で30度回転させたいときは
	//v.turn(30,DIM_Y,DIM_T);
	//と書く。
	void turn90(void);//90度回転（高速）

	void set(const double valueX,const double valueY,const double valueZ,const double valueT);
	//void set(const double valueX,const double valueY);
	//ベクトルの値をセットする
	//例えばベクトルvにx=1,y=-5を入れたいときは
	//v.set(1,-5);
	//と書く。(z,tの値は保存される)
	//また、
	//ベクトルの値を４つ一気にセットすることもできる
	//例えばベクトルvにx=1,y=-5,z=3,t=2を入れたいときは
	//v.set(1,-5,3,2);
	//と書く。
};
c4DVector operator*(const double rother, c4DVector v);//*演算子(定数掛け算　定数＊ベクトル)

double abs(const c4DVector v);//ベクトルvの絶対値を取得

c4DVector unit(const c4DVector v);//ベクトルvに平行な単位ベクトルを取得

double degree(c4DVector v,const dimension_No first_d,const dimension_No second_d);//(_d:次元 要素DIM_X:x,DIM_Y:y,DIM_Z:z,DIM_T:t)
double degree(const c4DVector v);
//xy平面上でベクトルvの角度を測る。（度数法値域０～３６０）（零ベクトルのときは－３６０を返す）
//例えばベクトルvのxy平面上での角度を得たいときは
//(角度) = degree(v)
//と書く。
//また、
//(first_d)(second_d)平面上でベクトルvをの角度を測ることもできる。（度数法値域０～３６０）（零ベクトルのときは－３６０を返す）
//例えばベクトルvのtz平面上での角度を得たいときは
//(角度) = degree(v,DIM_T,DIM_Z)
//と書く。

double degree(const c4DVector v1,const c4DVector v2);
//ベクトルv1とベクトルv2の間の角度を測る。（度数法値域０～１８０）（片方または両方が零ベクトルのときは－３６０を返す）
//例えばベクトルv1とベクトルv2の角度を得たいときは
//(角度) = degree(v1,v2)
//と書く。

c4DVector exproduct( c4DVector& v1, c4DVector& v2,const dimension_No first_d,const dimension_No second_d,const dimension_No third_d);//(_d:次元 要素DIM_X:x,DIM_Y:y,DIM_Z:z,DIM_T:t)
c4DVector exproduct( c4DVector& v1, c4DVector& v2);
//xyz空間上でベクトルv1とベクトルv2の外積をとる。
//例えばxyz空間上でベクトルv1とベクトルv2の外積を得たいときは
//(外積) = exproduct(v1,v2)
//と書く。
//また、
//(first_d)(second_d)(third_d)空間上でベクトルv1とベクトルv2の外積をとることもできる。
//例えばyzt空間上でベクトルv1とベクトルv2の外積を得たいときは
//(外積) = exproduct(v1,v2,DIM_Y,DIM_Z,DIM_T)
//と書く。



inline double*c4DVector::operator ()(const dimension_No& rother)//説明は上に
{
double*result_da = 0;
if(rother==0)result_da=&x;
else if(rother==1)result_da=&y;
else if(rother==2)result_da=&z;
else if(rother==3)result_da=&t;
//else puts(_T("bad dimension No. error"));
return result_da;
}

//交差判定//平面
bool cross_Judge_Flat(c4DVector& a_begin, c4DVector& a_end, c4DVector& b_begin,c4DVector& b_end);

#endif // #ifndef INC_VECTOR_HEADER_so301fkgmr83s_INCLUDED