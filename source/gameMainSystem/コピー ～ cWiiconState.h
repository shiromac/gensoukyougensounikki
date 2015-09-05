#pragma once
#include "../utility/4Dvector.h"
#include "../utility/cAltaMatter.h"
#include "../utility/cAltaMatterPlus.h"
#include "Wiicon/cWiimote.h"

class cInput;

class cInputChild
{
public:
	cInputChild(void);
public:
	virtual ~cInputChild(void);

	virtual void InitWiicon(CWiimote * pWiicon);

	int ResetAdvanceInput(int num);

	//子クラス用
	virtual void ResetJudgeCustum(int playerNum);
	virtual void InitCustum(CWiimote * pWiicon);

	c4DVector decodeMotion(c4DVector m_raw);
	c4DVector decodePointer(c4DVector p_raw);


	//ForceFeedBackを設定
	void setForceFeedBack(int frame);
	void setForceFeedBackForcibly(int frame);
	
private:
	CWiimote* WiiconDev;

	//鉛直下向き判定
	void makePlumpdownline();

	//ForceFeedBack関係
	void ForceFeedBackReset();
	bool ForceFeedOn;
	int FFBframe_count;
public:

	//モーションの軸系をポインタの軸系に変換
	c4DVector motionToPointer(c4DVector motion);
private:
	//WiiToWorldDegreeを計算してセット
	void GetWiiToWorldDegree();
public:
	//実際のXY軸でポインターを取得
	c4DVector realPointer();

	int playerNo;

	//この角度分回転すれば世界座標系に重なる。
	double WiiToWorldDegree;

	//bool enable;

	c4DVector pointer;
	c4DVector motion;//単位はG

	c4DVector prepointer;
	c4DVector premotion;

	c4DVector additionpointer;
	c4DVector additionmotion;

	c4DVector plumpdownline;//鉛直方向下向き

	c4DVector objectivepointer;//目標
	//c4DVector velocityOfpointer;//速度

	//ボタン
	cAltaMatterPlus A;
	cAltaMatterPlus B;

	cAltaMatterPlus Plus;
	cAltaMatterPlus Minus;

	cAltaMatterPlus Home;

	cAltaMatterPlus One;
	cAltaMatterPlus Two;

	cAltaMatterPlus Up;
	cAltaMatterPlus Down;
	cAltaMatterPlus Left;
	cAltaMatterPlus Right;



	c4DVector motionAnnealLv1;//ぼかし
	c4DVector motionAnnealLv2;
	c4DVector motionAnnealLv3;

	c4DVector center_pointer_raw;//ポインタ画面中心のときのIR
	c4DVector rightbottom_pointer_raw;//ポインタ画面右下のときのIR


	cAltaMatter IRpointer_1_exist;
	cAltaMatter IRpointer_2_exist;
	cAltaMatter IRpointer_half_exist;
	cAltaMatter IRpointer_full_exist;

	cAltaMatter IRpointer_center_exist;//この値がtrueのときポインタが有効
/*
	bool IRpointer_1_exist;
	bool IRpointer_2_exist;
	bool IRpointer_half_exist;
	bool IRpointer_full_exist;
	bool IRpointer_center_exist;

	bool IRpointer_1_existed;
	bool IRpointer_2_existed;
	bool IRpointer_half_existed;
	bool IRpointer_full_existed;
	bool IRpointer_center_existed;

	bool IRpointer_full_in;
	bool IRpointer_full_out;
	bool IRpointer_half_in;
	bool IRpointer_half_out;
*/
	c4DVector IRpointer_center_raw;//インプットとしては使えない。
	c4DVector IRpointer_1_raw;//インプットとしては使えない。
	c4DVector IRpointer_2_raw;//インプットとしては使えない。
	

	c4DVector motion_raw;//インプットとしては使えない。

	c4DVector noGravity_raw_const;//無重力
	c4DVector oneGravityForEachXYZ_raw_const;//各軸に対する1G


	virtual void FeedbackOff();


private:
	int GWTWDSign1;
	int GWTWDSign2;
	int GWTWDSign1_counter;
	int GWTWDSign2_counter;

	c4DVector horison;

	bool IRpointer_center_exist_flag;


};

#define MAX_IR_POINTER_X 1022
#define MAX_IR_POINTER_Y 760