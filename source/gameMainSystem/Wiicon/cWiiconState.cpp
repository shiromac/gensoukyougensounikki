#include "../../stdafx.h"

#include "cWiiconState.h"
#include "../cInput.h"
#include "../cEnvironment.h"
#include "../Main.h"

#include "Wiicondefph.h"
//これが定義されているときWiiコンをソリューションに含める。
#ifdef USE_WII_REMOTE_SWITCH

cWiiconState::cWiiconState(void)
{
}

cWiiconState::~cWiiconState(void)
{
}

void cWiiconState::InitWiiconState(CWiimote * pWiicon)
{
	WiiconDev = pWiicon;

	/*
	noGravity_raw_const.set( (WiiconDev)->m_MotionBaseX.G0,
										(WiiconDev)->m_MotionBaseY.G0,
										(WiiconDev)->m_MotionBaseZ.G0,
										0);
										*/
	noGravity_raw_const.set(128,128,128,0);
/*
	oneGravityForEachXYZ_raw_const.set((WiiconDev)->m_MotionBaseX.G1,
										(WiiconDev)->m_MotionBaseY.G1,
										(WiiconDev)->m_MotionBaseZ.G1,
												0);
*/
	oneGravityForEachXYZ_raw_const.set(99,100,99,0);

	center_pointer_raw.set(MAX_IR_POINTER_X/2,
							MAX_IR_POINTER_Y/2,
							0,0);
	rightbottom_pointer_raw.set(MAX_IR_POINTER_X/2 - SCREEN_X/2,
							MAX_IR_POINTER_Y/2 + SCREEN_Y/2,
							0,0);

	ForceFeedOn = false;
	FFBframe_count = 0;

	motion.set(0,0,-1,0);
	motionAnnealLv1 = motionAnnealLv2 = motionAnnealLv3 = motion;

	InitCustum(pWiicon);
}

//子クラスオーバーロード用
void cWiiconState::ResetJudgeCustum(int playerNum)
{}
//子クラスオーバーロード用
void cWiiconState::InitCustum(CWiimote * pWiicon)
{}


//毎回呼ばれて情報更新を各コントローラー別に行う
int cWiiconState::ResetAdvanceInput(int num)
{
	//実行順を変えないように
	//pressA,Bとrawはすでに入っている
	IRpointer_full_exist.process(IRpointer_1_exist.on && IRpointer_2_exist.on);
	IRpointer_half_exist.process((IRpointer_1_exist.on||IRpointer_2_exist.on) && !IRpointer_full_exist.on);



	if (IRpointer_full_exist.justOn) IRpointer_center_exist_flag = true;
	else if (!IRpointer_half_exist.on && !IRpointer_full_exist.on) IRpointer_center_exist_flag = false;

	IRpointer_center_exist.process(IRpointer_center_exist_flag);


	//バイブ処理
	ForceFeedBackReset();


	premotion = motion;//前の値保存
	motion = decodeMotion(motion_raw);
	additionmotion = motion - premotion;

	motionAnnealLv1 = motionAnnealLv1/2 + motion/2;
	motionAnnealLv2 = motionAnnealLv2*9/10 + motion/10;
	motionAnnealLv3 = motionAnnealLv3*99/100 + motion/100;

	makePlumpdownline();

	if(IRpointer_full_exist.on)GetWiiToWorldDegree();

	prepointer = pointer;
	
	
	//ここから補正合戦。
	c4DVector RAI_v = motionToPointer(additionmotion);
	RAI_v.turn(WiiToWorldDegree);
	//if(RAI_v*additionmotion < 0) RAI_v *= -1;
	RAI_v = RAI_v*abs(additionmotion)*128;
	if(abs(RAI_v) > 500) RAI_v = unit(RAI_v)*500;
	objectivepointer = decodePointer(IRpointer_center_raw) + RAI_v;
	//velocityOfpointer += objectivepointer - prepointer/10;
	//if(abs(velocityOfpointer) > 50)velocityOfpointer = unit(velocityOfpointer)*50;
	//if(velocityOfpointer*(objectivepointer - prepointer)<0) velocityOfpointer.set(0,0,0,0);
	//pointer += velocityOfpointer/5;
	//pointer = objectivepointer;
	if(IRpointer_center_exist.justOn) pointer = objectivepointer;
	else pointer = pointer/2 + objectivepointer/2;

	//pointer = decodePointer(IRpointer_center_raw);
	
	
	ResetJudgeCustum(num);
	return 0;
}

c4DVector cWiiconState::decodePointer(c4DVector p_raw)
{

	c4DVector decodePointer_v = p_raw - center_pointer_raw;
	decodePointer_v.x /= (rightbottom_pointer_raw.x - center_pointer_raw.x)/(SCREEN_X/2);
	decodePointer_v.y /= (rightbottom_pointer_raw.y - center_pointer_raw.y)/(SCREEN_Y/2);
	decodePointer_v.z = 0;
	decodePointer_v.t = 0;


	decodePointer_v.turn(WiiToWorldDegree);

	return decodePointer_v;
}

void cWiiconState::GetWiiToWorldDegree()
{
	//static int GWTWDSign1,GWTWDSign2,GWTWDSign1_counter,GWTWDSign2_counter;
	//static c4DVector horison;
	c4DVector WiiHorizon;

	if(IRpointer_full_exist.on)
	{//	更新　IRpointer_full_existのときのみ
		horison = IRpointer_2_raw - IRpointer_1_raw;
		if(abs(horison) == 0) horison.set(0,0,-1,0);
		horison = unit(horison);
		horison.x *= -1;//rawだと左右反転なので修正
	}

	WiiHorizon.set(1,0,0,0);

	//水平線方向判定
	if(IRpointer_full_exist.justOn)
	{
		GWTWDSign2 = (exproduct(horison,motionToPointer(motionAnnealLv3)).z>0)? 1 : -1;
		GWTWDSign2_counter = GWTWDSign2*10;
	}
	else if(IRpointer_full_exist.on)
	{
		GWTWDSign2_counter += (exproduct(horison,motionToPointer(plumpdownline)).z>0)? 1 : -1;
		if(GWTWDSign2_counter*GWTWDSign2 < 0)
		{//カウンターの符号と異なったら反転
			GWTWDSign2 *= -1;
		}
	}


	horison *= GWTWDSign2;

	GWTWDSign1 = (exproduct(WiiHorizon,horison).z>0)? 1 : -1;
	WiiToWorldDegree = -degree(horison,WiiHorizon)*GWTWDSign1;
	
}


c4DVector cWiiconState::decodeMotion(c4DVector m_raw)
{
	c4DVector decodeMotion_v = m_raw - noGravity_raw_const;

	decodeMotion_v.x /= oneGravityForEachXYZ_raw_const.x - noGravity_raw_const.x;
	decodeMotion_v.y /= oneGravityForEachXYZ_raw_const.y - noGravity_raw_const.y;
	decodeMotion_v.z /= oneGravityForEachXYZ_raw_const.z - noGravity_raw_const.z;
	decodeMotion_v.t = 0;

	return decodeMotion_v;
}

void cWiiconState::makePlumpdownline()
{
	if ((abs(additionmotion) < 0.05)&&(abs(motion)<1.1&&abs(motion)>0.9))
	{
		plumpdownline = unit(motionAnnealLv1);
	}
	else if(abs(additionmotion) < 0.1)
	{
		plumpdownline = unit(motionAnnealLv2);
	}
	else if(abs(motionAnnealLv1-motionAnnealLv2) < 0.2)
	{
		plumpdownline = unit(motionAnnealLv3);
	}
	else;//何も更新しない
}

c4DVector cWiiconState::motionToPointer(c4DVector m)
{
	c4DVector motionToPointer_p;
	motionToPointer_p.set(-m.x,-m.z,0,0);
	return motionToPointer_p;
}

c4DVector cWiiconState::realPointer()
{
	c4DVector realPointer_p;
	realPointer_p.set(pointer.x + SCREEN_X/2,
		pointer.y + SCREEN_Y/2,
		0,0);
	return realPointer_p;
}

void cWiiconState::setForceFeedBack(int frame)
{
	if(FFBframe_count <= -5)
		FFBframe_count = frame;
}
void cWiiconState::setForceFeedBackForcibly(int frame)
{
	FFBframe_count = frame;
}

void cWiiconState::ForceFeedBackReset()
{
	if(FFBframe_count > 0)
	{
		if(!ForceFeedOn)
		{
			(WiiconDev)->WiiFeedbackOn();
			ForceFeedOn = true;
		}
		
	}
	else
	{
		if(ForceFeedOn)
		{
			(WiiconDev)->WiiFeedbackOff();
			ForceFeedOn = false;
		}
		
	}

	FFBframe_count--;
}

void cWiiconState::FeedbackOff()
{
	FFBframe_count = 0;
	(WiiconDev)->WiiFeedbackOff();
	ForceFeedOn = false;

}



#else
//---------------------------------------------
//ダミー
//---------------------------------------------

cWiiconState::cWiiconState(void)
{
}

cWiiconState::~cWiiconState(void)
{
}

void cWiiconState::InitWiiconState(CWiimote * pWiicon)
{}

//子クラスオーバーロード用
void cWiiconState::ResetJudgeCustum(int playerNum)
{}
//子クラスオーバーロード用
void cWiiconState::InitCustum(CWiimote * pWiicon)
{}


//毎回呼ばれて情報更新を各コントローラー別に行う
int cWiiconState::ResetAdvanceInput(int num)
{
	return 0;
}

c4DVector cWiiconState::decodePointer(c4DVector p_raw)
{
	c4DVector decodePointer_v;
	return decodePointer_v;
}

void cWiiconState::GetWiiToWorldDegree()
{}

c4DVector cWiiconState::decodeMotion(c4DVector m_raw)
{
	c4DVector decodeMotion_v;
	return decodeMotion_v;
}

void cWiiconState::makePlumpdownline()
{}

c4DVector cWiiconState::motionToPointer(c4DVector m)
{
	c4DVector motionToPointer_p;
	return motionToPointer_p;
}

c4DVector cWiiconState::realPointer()
{
	c4DVector realPointer_p;
	return realPointer_p;
}

void cWiiconState::setForceFeedBack(int frame)
{}
void cWiiconState::setForceFeedBackForcibly(int frame)
{}

void cWiiconState::ForceFeedBackReset()
{}

void cWiiconState::FeedbackOff()
{}


#endif