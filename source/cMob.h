#pragma once
#include "ccharacter.h"
#include "ceaiActive.h"

#define DEF_MOB_PALNUM 11


class cMob;
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
typedef boost::shared_ptr<cMob> pcMob;
typedef boost::weak_ptr<cMob> wpcMob;

class cMob :
	public cCharacter
{
public:
	cMob(void);
public:
	virtual ~cMob(void);

	virtual void InitSub(cRenderDevice *pDev);//子クラスで使うかも

	virtual void settingInit();
	virtual void naturalSpawnInit();

	virtual void 配置処理();
	virtual bool 雑魚属性(){return true;};

	virtual void CutIn(タイミング timing, cValiableField& valiable);
	virtual unsigned int ShadowColor();
	virtual bool edgedrawswitch(){return 1;};
	virtual tstring FullNameBase();
	//virtual tstring FullNameSubBase();
	virtual tstring ShortNameBase();
	//virtual tstring ShortNameSubBase();
	//コンディション特殊能力
	virtual void conditionprocess();
protected:
	virtual double baseAttackPower();
	virtual double baseDefencePower();
public:

	virtual int attaP();
	virtual int deffP();
public:
	virtual int SetMHP();
	virtual int GetMHP();
	virtual int HaveEXP();
	virtual int PerOfspecialAttack();
	virtual int UseSPOfspecialAttack();
	virtual int appearFloor(int power);

	virtual double ValiableConstant1();
	virtual double ValiableConstant2();
	virtual double ValiableConstant3();
	virtual double ValiableConstant4();

	virtual int addEXP(int e);
	virtual int LvUP();
	virtual int LvDown();

	virtual int upMHP();

	virtual bool 死亡ドロップアイテムなし();
	virtual int 死亡ドロップアイテムID();
	virtual bool 水上歩行();

	virtual int デフォルト速度移動();
	virtual int デフォルト速度攻撃();

	virtual tstring LVStr();//レベル表現

	
	virtual int 特殊攻撃_アニメ(cValiableField& valiable);

	//---------------------------
	//こっからAI用
	//---------------------------
	//virtual inline int Get_kindofAI_ID(){return AI_KIND_ID_DEFAULT;};
	virtual pcEnemyAI Get_kindofAI(){return pcEnemyAI(new ceaiActive);};

	//---------------------------
	//ここまでAI用
	//---------------------------
private:
	//double pal_coef[DEF_MOB_PALNUM][3];
	double pal_clsv[DEF_MOB_PALNUM][6];

	int calcuratePal(int pal, int Class);
public:
	double 初期配置深い居眠り確率係数();
	double 初期配置浅い居眠り確率係数();
	double 初期配置覚醒確率係数();
	double デフォルト初期配置深い居眠り確率係数();
	double デフォルト初期配置浅い居眠り確率係数();
	double デフォルト初期配置覚醒確率係数();
private:


	virtual void パッシブ能力sub(タイミング timing, cValiableField& valiable);
	
	/*
	//係数
	double exp_coef[3];
	double att_coef[3];
	double def_coef[3];
	double lv_coef[3];
	double mhp_coef[3];
	double sp_coef[3];

	//クラスごとの値
	double exp_clsv[5];
	double att_clsv[5];
	double def_clsv[5];
	double lv_clsv[5];
	double mhp_clsv[5];
	double sp_clsv[5];
	*/
public:
//基準出現階数
	//virtual int appearFloor(int power){return 1;};

	virtual int AI特殊攻撃選択pre(cValiableField& valiable);
	virtual int AIセミパッシブ特殊攻撃選択(cValiableField& valiable);//SP消費なし優先度高
	virtual int 特殊攻撃(cValiableField& valiable);
	virtual double AI特殊攻撃選択確率(){return PerOfspecialAttack()/100.0;};
	virtual int セミパッシブ特殊攻撃効果(cValiableField& valiable);//SP消費なし優先度高
	virtual bool 特殊攻撃可能();
};
