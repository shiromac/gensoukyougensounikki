#pragma once
#include "ccharacter.h"



class cPlayerChara :
	public cCharacter
{
public:
	cPlayerChara(void);
public:
	virtual ~cPlayerChara(void);

protected:
	virtual double baseAttackPower();
	virtual double baseDefencePower();
public:

	virtual double calcuAttackPower(pcDroping pcEqu);
	virtual double calcuDefencePower(pcDroping pcEqu);
	virtual double calcuAttackPower(double equPower);
	virtual double calcuDefencePower(double equPower);

	virtual double calcuExtraAttackPower(pcDroping pcEqu);
	virtual double calcuExtraDefencePower(pcDroping pcEqu);
	virtual double calcuNoAppreciationExtraAttackPower(pcDroping pcEqu);
	virtual double calcuNoAppreciationExtraDefencePower(pcDroping pcEqu);

	virtual bool luaCalcuEquipPower(double& result, tstring functionName, pcDroping pcEqu);

	virtual int canTalk();

	virtual int TalkEvent();

	virtual pcCommand CharaSkillCommond();

	//百分率
	virtual double GetHitPowerBase();
	virtual double GetDodgePowerBase();
	virtual double GetItemDodgePowerBase();

	//パラメーター類
	virtual double ExpTable(int lv);
	virtual double PowerTable(int lv);
	virtual double ArmCoTable(int lv);
	virtual double DefenseTable(int lv);
	virtual double ShieldCoTable(int lv);
	virtual double MHPTable(int lv);

	virtual int MaxholdNumFromDataBase();

	virtual StyleString uniqueMessage(tstring situation, int sub);

	
	virtual pcEnemyAI Get_kindofAI(){return pcEnemyAI(new ceaiNoMotion);};

	//コンディション特殊能力
	virtual void conditionprocess();

	virtual cCondition::空腹ProcessFlags 空腹ProcessFlag(){return cCondition::空腹ProcessFlag_空腹回復;};


	//持てる最大数
	virtual inline int MaxholdNum();

	virtual inline int 最大装備スペル数();
	
	virtual int 被攻撃優先度();
	virtual bool 水上歩行();
	virtual bool 壁中歩行();
public:
	virtual int upMHP(int downFlag = 0);
	virtual int SetMHP();
	virtual int GetMHP();
	virtual int HaveEXP();

	
	virtual int addEXP(int e);
	virtual int LvUP();
	virtual int LvDown();

	virtual void CutIn(タイミング timing, cValiableField& valiable);

	virtual int 通常攻撃();
	virtual int 通常攻撃_アニメ();
	virtual bool 通常攻撃_壁斜めすり抜け();
	virtual bool 通常攻撃_壁すり抜け();

	virtual int 通常防御_アニメ();

};

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cPlayerChara> pcPlayerChara;
