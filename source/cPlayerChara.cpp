#include "stdafx.h"
#include "cPlayerChara.h"
#include "Gameobjects.h"

#include "AttackAnimeAttriDEF.h"

/*
int exttable[] = {0,10,30,60,100,150,
					230,350,500,700,950,
					1200,1450,1750,2100,2500,
					3000,3500,4200,5000,6000,//20
					7000,8000,9500,11000,12500,
					14500,17500,22000,30000,40000,
					50000,65000,80000,100000,120000,
					140000,170000,210000,250000,300000,//40
					350000,400000,450000,500000,550000,
					600000,650000,700000,750000,800000,
					850000,900000,950000,1000000,1050000,
					1100000,1150000,1200000,1250000,1300000,//60
					1350000,1400000,1450000,1500000,1550000,
					1600000,1650000,1700000,1750000,1800000,
					1850000,1900000,1950000,2000000,2050000,
					2100000,2150000,2200000,2250000,2300000,//80
					2350000,2400000,2450000,2500000,2550000,
					2600000,2650000,2700000,2750000,2800000,
					2850000,2900000,2950000,3000000,3050000,
					3100000,3150000,3200000,3300000,2000000000//100
					};


int powertable[] = {0,6,8,10,12,14,
						16,18,20,22,24,
						26,28,30,31,32,
						33,34,35,36,37,//20
						38,39,40,41,42,
						43,44,45,46,47,
						48,49,50,51,52,
						53,54,55,56,57,//40
						58,59,60,61,62,
						63,64,65,66,67,
						68,69,70,71,72,
						73,74,75,76,77,//60
						78,79,80,81,82,
						83,84,85,86,87,
						88,89,90,91,92,
						93,94,95,96,97,//80
						98,99,100,101,102,
						103,104,105,106,107,
						108,109,110,111,112,
						113,114,115,116,117,//100
						};

//40分の
int attackLVrevisetable[]
					= {15,20,23,26,28,30,
						31,32,33,34,35,
						36,37,38,39,40,
						41,42,43,44,45,//20
						46,47,48,49,50,
						51,52,53,54,55,
						56,57,58,59,60,
						61,62,63,64,65,//40
						66,67,68,69,70,
						71,72,73,74,75,
						75,76,76,77,77,
						78,78,79,79,80,//60
						80,81,81,82,82,
						83,83,84,84,85,
						85,86,86,87,87,
						88,88,89,89,90,//80
						90,91,91,92,92,
						93,93,94,94,95,
						95,96,96,97,97,
						98,98,99,99,100,//100
						};

//40分の
int defenceLVrevisetable[]
					= {15,20,23,26,28,30,
						31,32,33,33,34,
						34,35,35,36,36,
						37,37,38,38,39,//20
						39,40,40,40,40,
						40,40,40,40,40,
						40,40,40,40,40,
						40,40,40,40,40,//40
						40,40,40,40,40,
						40,40,40,40,40,
						40,40,40,40,40,
						40,40,40,40,40,//60
						40,40,40,40,40,
						40,40,40,40,40,
						40,40,40,40,40,
						40,40,40,40,40,//80
						40,40,40,40,40,
						40,40,40,40,40,
						40,40,40,40,40,
						40,40,40,40,41//100
						};


int defetable[] = {0,0,0,0,0,0,
					1,1,1,1,1,
					2,2,2,2,2,
					2,3,3,3,3,//20
					3,3,3,3,4,
					4,4,4,4,4,
					4,4,4,4,5,
					5,5,5,5,5,//40
					5,5,5,5,5,
					6,6,6,6,6,
					6,6,6,6,6,
					6,6,7,7,7,//60
					7,7,7,7,7,
					7,7,7,7,7,
					8,8,8,8,8,
					8,8,8,8,8,//80
					8,8,8,8,9,
					9,9,9,9,9,
					9,9,9,9,9,
					9,9,9,9,10//100
						};

int MHPtable[] = {0,15,24,31,38,44,
					50,55,60,65,70,
					75,80,85,90,95,
					100,105,110,115,120,//20
					125,130,135,140,145,
					150,155,160,165,170,
					175,180,185,190,195,
					200,205,210,215,220,//40
					225,230,235,240,245,
					250,255,260,265,270,
					275,280,285,290,295,
					300,305,310,315,320,//60
					325,330,335,340,345,
					350,355,360,365,370,
					375,380,385,390,395,
					400,405,410,415,420,//80
					425,430,435,440,445,
					450,455,460,465,470,
					475,480,485,490,495,
					500,505,510,515,540,//100
						};
*/

//パラメーター類
double cPlayerChara::ExpTable(int lv)
{
	return sg_pDungeonSystem->DataBase.CharaImportData_Value(
		ID(),(tstring)_T("経験値テーブル"), 1,lv);
}

double cPlayerChara::PowerTable(int lv)
{
	return sg_pDungeonSystem->DataBase.CharaImportData_Value(
		ID(),(tstring)_T("攻撃力テーブル"), 1,lv);
}
double cPlayerChara::ArmCoTable(int lv)
{
	return sg_pDungeonSystem->DataBase.CharaImportData_Value(
		ID(),(tstring)_T("武器力係数テーブル"), 1,lv)/100.0;
}
double cPlayerChara::DefenseTable(int lv)
{
	return sg_pDungeonSystem->DataBase.CharaImportData_Value(
		ID(),(tstring)_T("防御力テーブル"), 1,lv);
}
double cPlayerChara::ShieldCoTable(int lv)
{
	return sg_pDungeonSystem->DataBase.CharaImportData_Value(
		ID(),(tstring)_T("防具力係数テーブル"), 1,lv)/100.0;
}
double cPlayerChara::MHPTable(int lv)
{
	return sg_pDungeonSystem->DataBase.CharaImportData_Value(
		ID(),(tstring)_T("最大HPテーブル"), 1,lv);
}
int cPlayerChara::MaxholdNumFromDataBase()
{
	return sg_pDungeonSystem->DataBase.CharaImportData_Value(
		ID(),(tstring)_T("所持アイテム最大数"), 20);
}
int cPlayerChara::最大装備スペル数()
{
	return sg_pDungeonSystem->DataBase.CharaImportData_Value(
		ID(),(tstring)_T("最大装備スペル数"), 2);
}
int cPlayerChara::被攻撃優先度()
{
	return 10;
}
//持てる最大数
int cPlayerChara::MaxholdNum()
{
	cValiableField valiable;
	valiable.doubles.dim(変数_汎用実数) = MaxholdNumFromDataBase();
	sg_pDungeonSystem->CutInM().CutIn(me(), アイテム所持数計算時_タイミング, valiable);
	
	return valiable.doubles.val(変数_汎用実数);
}
cPlayerChara::cPlayerChara(void)
{
	EXP = 0;
	LV = 1;

	MaxStomach = 100;
	int i;
	//for(i=0;i<AI_MEMORY_NUM;i++)
	//{
	//	memory[i] = 0;
	//}
	
	SetMHP();


}

cPlayerChara::~cPlayerChara(void)
{
}
bool cPlayerChara::水上歩行()
{
	cValiableField valiable;
	valiable.doubles.dim(変数_汎用ブール) = 0;
	sg_pDungeonSystem->CutInM().CutIn(me(), 主人公水上歩行可能判定時_タイミング, valiable);
	
	return valiable.doubles.val(変数_汎用ブール);
}
bool cPlayerChara::壁中歩行()
{
	cValiableField valiable;
	valiable.doubles.dim(変数_汎用ブール) = 0;
	sg_pDungeonSystem->CutInM().CutIn(me(), 主人公壁中歩行可能判定時_タイミング, valiable);
	
	return valiable.doubles.val(変数_汎用ブール);
}
int cPlayerChara::通常攻撃()
{
	
	tstring functionName(_T("normalAttack"));
	int i,size = luaScript_.size();
	for(i=0;i<size;i++)
	{
		bool temp_result = false;
		pDungeonStoryScript_LuaEnvironment pluaScriptEnv = luaScript_[i].lock();
		if(pluaScriptEnv && pluaScriptEnv->luaState() != NULL)
		{
			if(pluaScriptEnv->function_returnable(functionName, temp_result))
			{
				if(temp_result)
				{
					return true;
				}
			}
		}
	}
	return 通常攻撃効果();
}
int cPlayerChara::通常攻撃_アニメ()
{
	pcEquipment pEqu;
	if(
		(attackequipment != NULL)//装備してる
		&& (pEqu = boost::dynamic_pointer_cast<cEquipment>(attackequipment) )!= NULL
		&& attackequipment->能力発動条件満たしている_攻撃用())
	{
		pEqu->攻撃エフェクト(me());
	}
	else
	{
		sg_pDungeonSystem->AnimationManager().
			EasyAnime_Attackcharacter(me(),
									ATTACKATTRI_PUNCH1);
	}
	return true;
}
bool cPlayerChara::通常攻撃_壁斜めすり抜け()
{
	pcEquipment pcEqu = boost::dynamic_pointer_cast<cEquipment>(attackequipment);
	if(pcEqu != NULL)
	{
		return pcEqu->通常攻撃_壁斜めすり抜け();
	}
	return false;
}
bool cPlayerChara::通常攻撃_壁すり抜け()
{
	pcEquipment pcEqu = boost::dynamic_pointer_cast<cEquipment>(attackequipment);
	if(pcEqu != NULL)
	{
		return pcEqu->通常攻撃_壁すり抜け();
	}
	return false;
}

int cPlayerChara::通常防御_アニメ()
{
	pcEquipment pEqu;
	if(
		(defenseequipment != NULL)//装備してる
		&& (pEqu = boost::dynamic_pointer_cast<cEquipment>(defenseequipment) )!= NULL
		)
	{
		pEqu->防御エフェクト(me());
	}
	else
	{
		//なし
	}
	return true;
}

//コンディション特殊能力
void cPlayerChara::conditionprocess()
{

}

double cPlayerChara::baseAttackPower()
{
	return calcuAttackPower(attackequipment);
}
double cPlayerChara::baseDefencePower()
{
	return calcuDefencePower(defenseequipment);
}

bool cPlayerChara::luaCalcuEquipPower(double& result, tstring functionName, pcDroping pcEqu)
{
	cValiableField valiable;
	{
		valiable.drops.dim(_T("equipment")) = pcEqu;
		pcEquipment equ = boost::dynamic_pointer_cast<cEquipment>(pcEqu);
		if(equ != NULL)
		{
			valiable.doubles.dim(_T("attack_equipment_power")) = equ->武器力最終値();
			valiable.doubles.dim(_T("attack_equipment_power_base")) = equ->武器力基礎値();
			valiable.doubles.dim(_T("defence_equipment_power")) = equ->防具力最終値();
			valiable.doubles.dim(_T("defence_equipment_power_base")) = equ->防具力基礎値();
		}
		else
		{
			valiable.doubles.dim(_T("attack_equipment_power")) = 0.0;
			valiable.doubles.dim(_T("attack_equipment_power_base")) = 0.0;
			valiable.doubles.dim(_T("defence_equipment_power")) = 0.0;
			valiable.doubles.dim(_T("defence_equipment_power_base")) = 0.0;
		}
		valiable.doubles.dim(_T("player_attack_power")) = PowerTable(LV);
		valiable.doubles.dim(_T("player_attack_equipment_power_magnification")) = ArmCoTable(LV);
		valiable.doubles.dim(_T("player_defence_power")) = DefenseTable(LV);
		valiable.doubles.dim(_T("player_defence_equipment_power_magnification")) = ShieldCoTable(LV);
	}
	int i,size = luaScript_.size();
	for(i=0;i<size;i++)
	{
		double temp_result = 0.0;
		pDungeonStoryScript_LuaEnvironment pluaScriptEnv = luaScript_[i].lock();
		if(pluaScriptEnv && pluaScriptEnv->luaState() != NULL)
		{
			if(pluaScriptEnv->function_returnable(functionName, temp_result, boost::ref(valiable)))
			{
				result = temp_result;
				return true;
			}
		}
	}
	return false;
}

double cPlayerChara::calcuAttackPower(pcDroping pcEqu)
{
	tstring functionName(_T("calcuAttackPower"));
	double temp_result = 0.0;
	if(luaCalcuEquipPower(temp_result, functionName, pcEqu))
	{
		return temp_result;
	}
	/*
	double power = baseattackPower + PowerTable(LV);


	if(equ != NULL)
	{
		power += equ->武器力最終値()*ArmCoTable(LV);
	}

	return power;
	*/

	pcEquipment equ = boost::dynamic_pointer_cast<cEquipment>(pcEqu);

	if(equ != NULL)
	{
		return calcuAttackPower(equ->武器力最終値());
	}
	return calcuAttackPower(0);

}
double cPlayerChara::calcuDefencePower(pcDroping pcEqu)
{
	tstring functionName(_T("calcuExtraDefencePower"));
	double temp_result = 0.0;
	if(luaCalcuEquipPower(temp_result, functionName, pcEqu))
	{
		return temp_result;
	}

	/*
	double power = basediffencePower + DefenseTable(LV);
	
	pcEquipment equ = boost::dynamic_pointer_cast<cEquipment>(pcEqu);

	if(equ != NULL)
	{
		power += equ->防具力最終値()*ShieldCoTable(LV);
	}

	return power;
	*/

	pcEquipment equ = boost::dynamic_pointer_cast<cEquipment>(pcEqu);

	if(equ != NULL)
	{
		return calcuDefencePower(equ->防具力最終値());
	}
	return calcuDefencePower(0);
}

double cPlayerChara::calcuExtraAttackPower(pcDroping pcEqu)
{
	tstring functionName(_T("calcuExtraAttackPower"));
	double temp_result = 0.0;
	if(luaCalcuEquipPower(temp_result, functionName, pcEqu))
	{
		return temp_result;
	}

	pcEquipment equ = boost::dynamic_pointer_cast<cEquipment>(pcEqu);

	if(equ != NULL)
	{
		double power = max(0,equ->武器力最終値() - equ->防具力最終値())/2.0;
		return power*ArmCoTable(LV);
	}
	return 0;

}
double cPlayerChara::calcuExtraDefencePower(pcDroping pcEqu)
{
	tstring functionName(_T("calcuExtraDefencePower"));
	double temp_result = 0.0;
	if(luaCalcuEquipPower(temp_result, functionName, pcEqu))
	{
		return temp_result;
	}

	pcEquipment equ = boost::dynamic_pointer_cast<cEquipment>(pcEqu);

	if(equ != NULL)
	{
		double power = max(0,equ->防具力最終値() - equ->武器力最終値())/2.0;
		return power*ArmCoTable(LV);
	}
	return 0;
}
double cPlayerChara::calcuNoAppreciationExtraAttackPower(pcDroping pcEqu)
{
	tstring functionName(_T("calcuNoAppreciationExtraAttackPower"));
	double temp_result = 0.0;
	if(luaCalcuEquipPower(temp_result, functionName, pcEqu))
	{
		return temp_result;
	}

	pcEquipment equ = boost::dynamic_pointer_cast<cEquipment>(pcEqu);

	if(equ != NULL)
	{
		double power = max(0,equ->武器力基礎値() - equ->防具力基礎値())/2.0;
		return power*ArmCoTable(LV);
	}
	return 0;

}
double cPlayerChara::calcuNoAppreciationExtraDefencePower(pcDroping pcEqu)
{
	tstring functionName(_T("calcuNoAppreciationExtraDefencePower"));
	double temp_result = 0.0;
	if(luaCalcuEquipPower(temp_result, functionName, pcEqu))
	{
		return temp_result;
	}

	pcEquipment equ = boost::dynamic_pointer_cast<cEquipment>(pcEqu);

	if(equ != NULL)
	{
		double power = max(0,equ->防具力基礎値() - equ->武器力基礎値())/2.0;
		return power*ArmCoTable(LV);
	}
	return 0;
}


double cPlayerChara::calcuAttackPower(double equPower)
{
	double power = PowerTable(LV);

	power += equPower*ArmCoTable(LV);

	return power;
}
double cPlayerChara::calcuDefencePower(double equPower)
{
	double power = DefenseTable(LV);
	
	power += equPower*ShieldCoTable(LV);

	return power;
}

//百分率
double cPlayerChara::GetHitPowerBase()
{
	double power = cCharacter::GetHitPowerBase();

	pcEquipment equ = boost::dynamic_pointer_cast<cEquipment>(attackequipment);

	if(equ != NULL)
	{
		power += calcuDefencePower(equ->命中率補正());
	}
	return power;
}
double cPlayerChara::GetDodgePowerBase()
{
	double power = cCharacter::GetDodgePowerBase();

	pcEquipment equ = boost::dynamic_pointer_cast<cEquipment>(attackequipment);

	if(equ != NULL)
	{
		power += calcuDefencePower(equ->回避率補正());
	}
	return power;
}
double cPlayerChara::GetItemDodgePowerBase()
{
	return cCharacter::GetItemDodgePowerBase();
}
int cPlayerChara::upMHP(int downFlag)
{
	int range = MHPTable(LV)-MHPTable(LV-1);
	int i = random_range(range*0.5 ,range*1.5+1);
	if(downFlag) i = -i;
	if(i > 0)
		return sg_pDungeonSystem->最大HP拡張要請(me(), i, 0);
	else
		return sg_pDungeonSystem->最大HP縮小要請(me(), -i, 0);

}
int cPlayerChara::SetMHP()
{
	int i = GetMHP();
	return MHP = i;
}
int cPlayerChara::GetMHP()
{
	return MHPTable(LV);
}

int cPlayerChara::HaveEXP()
{
	return LV*100;
}

int cPlayerChara::addEXP(int e)
{
	EXP += e;
	if(EXP > MaxEXP())
	{
		EXP = MaxEXP();
	}

	int uplv = 0;
	if(ExpTable(LV) <= EXP &&  LV <= MaxLV())
	{
		for(;;)
		{
			//LvUP();
			uplv++;
			if(ExpTable(LV+uplv) > EXP || LV+uplv >= MaxLV()) break;
		}
		//return LV;
	}
	return uplv;
}

int cPlayerChara::LvUP()
{
	if(MaxLV() <= LV) return false;
	LV++;
	if(EXP < ExpTable(LV-1)) EXP = ExpTable(LV-1);
	upMHP();
	return true;
}
int cPlayerChara::LvDown()
{
	if(LV <= 1) return false;
	LV--;
	EXP = ExpTable(LV)-1;
	upMHP(1);
	return true;
}
/*define at _T("cCondition.h")
#define STOMACH_MSG_FULL 0
#define STOMACH_MSG_AFEW 1
#define STOMACH_MSG_FEW 2
#define STOMACH_MSG_ENPTY1 3
#define STOMACH_MSG_ENPTY2 4
#define STOMACH_MSG_ENPTY3 5
*/
StyleString cPlayerChara::uniqueMessage(tstring situation, int sub)
{
	if(situation == _T("空腹"))
	{
		if(sub == STOMACH_MSG_AFEW)
		{
			return g_Lang(_T("空腹メッセージ_少ない"));
			//return _T("お腹が すいてきた。\n");
		}
		if(sub == STOMACH_MSG_FEW)
		{
			return g_Lang(_T("空腹メッセージ_かなり少ない"));
			//return _T("かなりお腹が 減ってきた……。\n");
		}
		if(sub == STOMACH_MSG_ENPTY1)
		{
			return g_Lang(_T("空腹メッセージ_空っぽ１"));
			//return _T("もうだめだ！ 空腹で 倒れそうだ！\n");
		}
		if(sub == STOMACH_MSG_ENPTY2)
		{
			return g_Lang(_T("空腹メッセージ_空っぽ２"));
			//return _T("早く……、何か口にしないと……。\n");
		}
		if(sub == STOMACH_MSG_ENPTY3)
		{
			return g_Lang(_T("空腹メッセージ_空っぽ３"));
			//return setStyle(_T("飢え死に"),0xFFFF0000)+_T(" してしまう！\n");
		}
	}

	return _T("");
}

void cPlayerChara::CutIn(タイミング timing, cValiableField& valiable)
{
	cCharacter::CutIn(timing,valiable);
	

	if(被攻撃接近直後_タイミング == timing)
	{
		sg_pDungeonSystem->s_autoTurnFlag() = true;
	}
	
}

int cPlayerChara::canTalk()
{
	bool result = false;
	tstring functionName(_T("canTalk"));
	int i,size = luaScript_.size();
	for(i=0;i<size;i++)
	{
		bool temp_result = false;
		pDungeonStoryScript_LuaEnvironment pluaScriptEnv = luaScript_[i].lock();
		if(pluaScriptEnv && pluaScriptEnv->luaState() != NULL &&
			!pluaScriptEnv->function_returnable(functionName, temp_result))
		{
			pluaScriptEnv->outputErrorFile(luaScriptErrorFileName(functionName, pluaScriptEnv));
		}
		result |= temp_result;
	}
	return result;
}

int cPlayerChara::TalkEvent()
{
	bool result = true;
	tstring functionName(_T("talkEvent"));
	int i,size = luaScript_.size();
	for(i=0;i<size;i++)
	{
		pDungeonStoryScript_LuaEnvironment pluaScriptEnv = luaScript_[i].lock();
		if(pluaScriptEnv && pluaScriptEnv->luaState() != NULL &&
			!pluaScriptEnv->function(functionName))
		{
			pluaScriptEnv->outputErrorFile(luaScriptErrorFileName(functionName, pluaScriptEnv));
		}
	}
	return result;
}

pcCommand cPlayerChara::CharaSkillCommond()
{
	bool result = true;
	tstring functionName(_T("charaSkillCommond"));
	int i,size = luaScript_.size();
	for(i=0;i<size;i++)
	{
		pcCommand command;
		pDungeonStoryScript_LuaEnvironment pluaScriptEnv = luaScript_[i].lock();
		if(pluaScriptEnv && pluaScriptEnv->luaState() != NULL)
		{
			if(pluaScriptEnv->function_returnable(functionName, command))
			{
				return command;
			}
		}
	}
	return  pcCommand((cCommand*)NULL);
}