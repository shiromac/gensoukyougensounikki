#include "cMob.h"

#include "gameMainSystem/filemanage/cScriptRLayer.h"
#include "gameMainSystem/filemanage/cScriptReader.h"

#include "cDungeonSystem.h"
#include "EffectFunctions.h"
#include "scriptprocesser/AnimationScript.h"
#include "cFloor.h"

inline double pow(int a,int b)
{
	return pow((double)a,(double)b);
}


cMob::cMob(void)
{
	EXP = 0;
	LV = 1;
	MHP = 0;
	HP = 0;
	CLASS = 0;

	int i,k;
	/*
	for(i=0;i<DEF_MOB_PALNUM;i++)
	{
		for(k=0;k<3;k++)
		{
			pal_coef[i][k] = 0;
		}
	}
	*/
	for(i=0;i<DEF_MOB_PALNUM;i++)
	{
		for(k=0;k<5;k++)
		{
			pal_clsv[i][k] = 0;
		}
	}

	SetMHP();
}

cMob::~cMob(void)
{
}

tstring cMob::LVStr()
{
	return g_Lang(_T("レベル表現_敵")).conclete_tstr();
}

enum 
{
	DEFPAL_出現階 = 0,
	DEFPAL_MHP,
	DEFPAL_経験値,
	DEFPAL_攻撃力,
	DEFPAL_防御力,
	DEFPAL_特殊使用率,
	DEFPAL_特殊消費SP,
	DEFPAL_汎用定数1,
	DEFPAL_汎用定数2,
	DEFPAL_汎用定数3,
	DEFPAL_汎用定数4
};
int indexpalOfChara[] = 
{
	DEFPAL_出現階,
	DEFPAL_MHP,
	DEFPAL_経験値,
	DEFPAL_攻撃力,
	DEFPAL_防御力,
	DEFPAL_特殊使用率,
	DEFPAL_特殊消費SP,
	DEFPAL_汎用定数1,
	DEFPAL_汎用定数2,
	DEFPAL_汎用定数3,
	DEFPAL_汎用定数4
};

tstring StrpalOfChara[] = 
{
	_T("出現階"),
	_T("MHP"),
	_T("経験値"),
	_T("攻撃力"),
	_T("防御力"),
	_T("特殊使用率"),
	_T("特殊消費SP"),
	_T("汎用定数一"),
	_T("汎用定数二"),
	_T("汎用定数三"),
	_T("汎用定数四"),
};

/*
tstring Strclass[] = 
{
	_T("CLASS_0"),
	_T("CLASS_1"),
	_T("CLASS_2"),
	_T("CLASS_3"),
	_T("CLASS_4")
};
*/
unsigned int cMob::ShadowColor()
{
	if(VisibleCLASS == 0)
	{
		return 0xFF999999;
	}
	else if(VisibleCLASS == 1)
	{
		return 0xFF0088FF;
	}
	else if(VisibleCLASS == 2)
	{
		return 0xFF00FF00;
	}
	else if(VisibleCLASS == 3)
	{
		return 0xFFFF9900;
	}
	else if(VisibleCLASS == 4)
	{
		return 0xFFFF0077;
	}
	else if(VisibleCLASS >= 5)
	{
		cColor c;
		c.HSV(360*anime_option_step/DEFAULTOPTIONSUMSTEP,1,0.9);
		return c.D3Dcolor();
	}

	return 0xFF000000;
}
int cMob::calcuratePal(int pal, int Class)
{
	double value = 0;
	
	int EXCLASS = 5;

	if(Class < EXCLASS+1)
	{
		value = pal_clsv[pal][Class];
	}
	else
	{
		value = pal_clsv[pal][EXCLASS] + (pal_clsv[pal][EXCLASS] - pal_clsv[pal][EXCLASS-1])*(Class-EXCLASS);
	}

	return value;

	/*
	if()
	double value = 0;
	int i;
	for(i=0;i<3;i++)
	{
		value += pal_coef[pal][i]*pow(Lv,i);
	}
	return value;
	*/
	/*
	double value = 0;
	int i;
	for(i=0;i<3;i++)
	{
		value += pal_coef[pal][i]*pow(Lv,i);
	}
	return value;
	*/
}

void cMob::InitSub(IDirect3DDevice9 *pDev)
{
	cCharacter::InitSub(pDev);

	int i,k;

	pcScriptRLayer pslayer;

	
	for(k=0;k<DEF_MOB_PALNUM;k++)
	{
		for(i=0;i<6;i++)
		{			
			pal_clsv[k][i] = sg_pDungeonSystem->DataBase.CharaImportData_Value(ID(), StrpalOfChara[k], DEF_UNDEFINE_DEFAULT, i);
		}
	}

	LV = appearFloor(CLASS);

/*
	tstring coefstr[] = {_T("定数"),_T("一次定数"),_T("二次定数")};
	
	for(k=0;k<6;k++)
	{
		for(i=0;i<3;i++)
		{
			pslayer = sg_pDungeonSystem->DataBase.CharaImportData(
				ID(),
				StrpalOfChara[k],
				coefstr[i]);
			if(pslayer != NULL)
			{
				pal_coef[k][i] = _tstof(pslayer->topscript().c_str());
			}
			else
			{
				//if(k != DEFPAL_出現階) //困った
			}
			
			
		}
	}
*/
	/*
	for(k=0;k<DEF_MOB_PALNUM;k++)//パラメータ
	{
		for(i=0;i<6;i++)//クラス
		{
			pslayer = sg_pDungeonSystem->DataBase.CharaImportData(
				ID(),
				StrpalOfChara[k],
				Strclass[i]);
			if(pslayer != NULL)
			{
				pal_clsv[k][i] = _tstof(pslayer->topscript().c_str());
			}
			else
			{
				pal_clsv[k][i] = calcuratePal(k, pal_clsv[DEFPAL_出現階][i]);
			}
			
			
		}
	}
*/

}


void cMob::settingInit()
{
	cCharacter::settingInit();


/*
	cDiscreteProbability CD;
	CD.set(初期配置深い居眠り確率係数(),0);
	CD.set(初期配置浅い居眠り確率係数(),1);
	CD.set(初期配置覚醒確率係数(),2);

	int res = CD.get(random());
	if(res == 0)
	{
		Condition.深い居眠り追加();
	}
	else if(res == 1)
	{
		Condition.浅い居眠り追加();
	}
	else if(res == 2)
	{
		
	}
*/
	VisibleCLASS = CLASS;

	配置処理();
}

void cMob::naturalSpawnInit()
{
	
	cCharacter::naturalSpawnInit();

	cDiscreteProbability CD;
	CD.set(初期配置深い居眠り確率係数(),0);
	CD.set(初期配置浅い居眠り確率係数(),1);
	CD.set(初期配置覚醒確率係数(),2);
	
	int res = CD.get(random());
	if(res == 0)
	{
		Condition.深い居眠り追加();
	}
	else if(res == 1)
	{
		Condition.浅い居眠り追加();
	}
	else if(res == 2)
	{
		
	}

}

void cMob::配置処理()
{
	return;
}


//コンディション特殊能力
void cMob::conditionprocess()
{
	if(!holdItem.empty())
	{
		emotion.insert(EMOTION_ITEM);
	}
	else
	{
		emotion.erase(EMOTION_ITEM);
	}
}

tstring cMob::FullNameBase()
{
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.CharaImportLanguage(ID(),
		(tstring)_T("CLASSName"));
	if(psr == NULL) return _T("No FullNameBase");
	int cl = CLASS;
	if(cl >= psr->script().size())
	{	
		cl = psr->script().size()-1;
		return psr->script()[cl] + setStyle(CLASS-cl+1).conclete_tstr();
	}
	if(psr->script().empty()) return _T("No FullNameBase"); 
	return psr->script()[cl];
}

tstring cMob::ShortNameBase()
{
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.CharaImportLanguage(ID(),
		(tstring)_T("CLASSName"));
	if(psr == NULL) return _T("No ShortNameBase");
	int cl = CLASS;
	if(cl >= psr->script().size())
	{	
		cl = psr->script().size()-1;
		return psr->script()[cl] + setStyle(CLASS-cl+1).conclete_tstr();
	}
	if(psr->script().empty()) return _T("No ShortNameBase"); 
	return psr->script()[cl];

}


double cMob::baseAttackPower()
{
	return attaP();
}
double cMob::baseDefencePower()
{
	return deffP();
}

int cMob::attaP()
{
	return calcuratePal(DEFPAL_攻撃力, CLASS);
}
int cMob::deffP()
{
	return calcuratePal(DEFPAL_防御力, CLASS);
}
int cMob::HaveEXP()
{
	return calcuratePal(DEFPAL_経験値, CLASS);
}
int cMob::GetMHP()
{
	return calcuratePal(DEFPAL_MHP, CLASS);
}
int cMob::PerOfspecialAttack()
{
	return calcuratePal(DEFPAL_特殊使用率, CLASS);
}
int cMob::UseSPOfspecialAttack()
{
	return calcuratePal(DEFPAL_特殊消費SP, CLASS);
}
double cMob::ValiableConstant1()
{
	return calcuratePal(DEFPAL_汎用定数1, CLASS);
}
double cMob::ValiableConstant2()
{
	return calcuratePal(DEFPAL_汎用定数2, CLASS);
}
double cMob::ValiableConstant3()
{
	return calcuratePal(DEFPAL_汎用定数3, CLASS);
}
double cMob::ValiableConstant4()
{
	return calcuratePal(DEFPAL_汎用定数4, CLASS);
}

int cMob::appearFloor(int power)
{
	return calcuratePal(DEFPAL_出現階, power);
}

int cMob::SetMHP()
{
	return MHP = GetMHP();
}

int cMob::addEXP(int e)
{
	EXP += e;
	if(EXP > MaxEXP())
	{
		EXP = MaxEXP();
	}
	if(EXP >= HaveEXP()/10)
	{
		//LvUP();
		return 1;
	}
	return 0;
}
int cMob::upMHP()
{
	int mhp = MHP;
	int def = GetMHP() - mhp;
	if(def > 0)
		return sg_pDungeonSystem->最大HP拡張要請(me(), def, 0);
	else
		return sg_pDungeonSystem->最大HP縮小要請(me(), -def, 0);
}

int cMob::LvUP()
{


	EXP = 0;
	CLASS++;
	upMHP();
	LV = appearFloor(CLASS);


	switch(デフォルト速度移動())
	{
	case -2:
		Condition.デフォルト速度設定(超鈍足); break;
	case -1:
		Condition.デフォルト速度設定(鈍足); break;
	case 0:
		Condition.デフォルト速度設定(等速); break;
	case 1:
		Condition.デフォルト速度設定(二倍速); break;
	case 2:
		Condition.デフォルト速度設定(三倍速); break;
	}

	
	return true;
}
int cMob::LvDown()
{
	if(CLASS > 0)
	{
		EXP = 0;
		CLASS--;
		upMHP();
		LV = appearFloor(CLASS);

		switch(デフォルト速度移動())
		{
		case -2:
			Condition.デフォルト速度設定(超鈍足); break;
		case -1:
			Condition.デフォルト速度設定(鈍足); break;
		case 0:
			Condition.デフォルト速度設定(等速); break;
		case 1:
			Condition.デフォルト速度設定(二倍速); break;
		case 2:
			Condition.デフォルト速度設定(三倍速); break;
		}

		return true;
	}

	return false;
}
int cMob::デフォルト速度移動()
{
	return sg_pDungeonSystem->DataBase.CharaImportData_Value(ID(),(tstring)_T("デフォルト速度移動"),0,min(5,CLASS));
}
int cMob::デフォルト速度攻撃()
{
	return sg_pDungeonSystem->DataBase.CharaImportData_Value(ID(),(tstring)_T("デフォルト速度攻撃"),0,min(5,CLASS));
}

double cMob::初期配置深い居眠り確率係数()
{
	return sg_pDungeonSystem->DataBase.CharaImportData_Value(ID(),(tstring)_T("初期配置深い居眠り確率係数"),デフォルト初期配置深い居眠り確率係数());
}
double cMob::初期配置浅い居眠り確率係数()
{
	return sg_pDungeonSystem->DataBase.CharaImportData_Value(ID(),(tstring)_T("初期配置浅い居眠り確率係数"),デフォルト初期配置浅い居眠り確率係数());
}
double cMob::初期配置覚醒確率係数()
{
	return sg_pDungeonSystem->DataBase.CharaImportData_Value(ID(),(tstring)_T("初期配置覚醒確率係数"),デフォルト初期配置覚醒確率係数());
}
double cMob::デフォルト初期配置深い居眠り確率係数()
{
	return sg_pDungeonSystem->DataBase.CharaImportData_Value((tstring)_T("Enemy基本値"),(tstring)_T("デフォルト初期配置深い居眠り確率係数"),100);
}
double cMob::デフォルト初期配置浅い居眠り確率係数()
{
	return sg_pDungeonSystem->DataBase.CharaImportData_Value((tstring)_T("Enemy基本値"),(tstring)_T("デフォルト初期配置浅い居眠り確率係数"),100);
}
double cMob::デフォルト初期配置覚醒確率係数()
{
	return sg_pDungeonSystem->DataBase.CharaImportData_Value((tstring)_T("Enemy基本値"),(tstring)_T("デフォルト初期配置覚醒確率係数"),100);
}



void cMob::CutIn(タイミング timing, cValiableField& valiable)
{
	cCharacter::CutIn(timing, valiable);

	if(timing == 主人公部屋入室_タイミング)
	{
		Condition.弱い刺激(valiable.doubles[変数_汎用実数]);
	}
	else if(timing == 主人公隣接_タイミング)
	{
		Condition.強い刺激(valiable.doubles[変数_汎用実数]);
	}
}
bool cMob::死亡ドロップアイテムなし()
{
	return 0;
}
int cMob::死亡ドロップアイテムID()
{
	if(死亡ドロップアイテムなし()) return 0;
	
	map<int,int> output;
	cDiscreteProbability DP;

	tstring datanamestr = _T("死亡ドロップアイテムID");
	sg_pDungeonSystem->DataBase.CharaImportData_MapIntToInt(ID(), datanamestr, output);


	map<int,int>::iterator itr = output.begin();
	for(;itr!=output.end();itr++)
	{
		DP.set(itr->second,itr->first);
	}
	
	int id = DP.get(random());

	//フロアの装備ドロップがなかった場合、装備ドロップをキャンセルする。
	if(floor(id / 1000.0)*1000 == CDROPING_cEquipment_BASE_ID_NUM) {
		double Equipment;
		double Bullet;
		double Spell;
		double Food;
		double Drink;
		double Book;
		double Box;
		double MoneyBag;
		double Other;
		sg_pDungeonSystem->pFloor()->dropitemDP().getItemGroupPos(&Equipment,
																&Bullet,
																&Spell,
																&Food,
																&Drink,
																&Book,
																&Box,
																&MoneyBag,
																&Other);
		if(Equipment == 0.0) {
			return 0;
		}
	}

	if(id == -1)
	{
		int outputID[3] = {0};
		sg_pDungeonSystem->アイテム自然湧きID(outputID);
		id = outputID[0];
	}
	return id;
}
bool cMob::水上歩行()
{
	return sg_pDungeonSystem->DataBase.CharaImportData_Value(ID(),(tstring)_T("水上歩行"),0);
}
int cMob::特殊攻撃(cValiableField& valiable)
{
	//速度
	if(sg_pDungeonSystem->s_turn_speed() < min(デフォルト速度移動(),Condition.速度度数()) - min(デフォルト速度攻撃(),Condition.速度度数()))
	{//s_turn_speed() = 0~2 ０が通常
		return false;//不許可
	}

	if(Condition.封印状態()) return false;
	if(valiable.doubles[変数_汎用実数])//セミパッシブフラグ
		return セミパッシブ特殊攻撃効果(valiable);
	
	if( Stomach < UseSPOfspecialAttack())
	{
		return false;
	}

	EffectFunctions::特殊能力発揮エフェクト(placeX,placeY,1.7);
	if(特殊攻撃効果(valiable))
	{
		sg_pDungeonSystem->満腹度減少要請(me(),UseSPOfspecialAttack(),false);
		//特殊攻撃_アニメ(valiable);
		return true;
	}
	return false;
}
int cMob::AIセミパッシブ特殊攻撃選択(cValiableField& valiable)
{
	return false;
}
int cMob::セミパッシブ特殊攻撃効果(cValiableField& valiable)
{//SP消費なし優先度高
	return false;
}
int cMob::AI特殊攻撃選択pre(cValiableField& valiable)
{
	if(Condition.封印状態())
	{
		return false;
	}

	if(AIセミパッシブ特殊攻撃選択(valiable)) return valiable.doubles[変数_汎用実数] = true;//セミパッシブフラグ

	if(!特殊攻撃可能())
	{
		return false;
	}

	if( !(Condition.嫉妬状態() || Condition.無意識状態() || AI特殊攻撃選択確率() > random()))
	{
		return false;
	}
	return AI特殊攻撃選択(valiable);
}
bool cMob::特殊攻撃可能()
{
	if(Condition.封印状態()) return false;

	if(  (Stomach < UseSPOfspecialAttack()))
	{
		return false;
	}
	return true;
}

void cMob::パッシブ能力sub(タイミング timing, cValiableField& valiable)
{
	return;
}

int cMob::特殊攻撃_アニメ(cValiableField& valiable)
{
	double temp;
	valiable.doubles.dim(tstring(_T("fromPlaceX")),temp = me()->placeX);
	valiable.doubles.dim(tstring(_T("fromPlaceY")),temp = me()->placeY);
	valiable.charas.dim(tstring(_T("attacker")),me());
	int outputaspect = 0, outputdistance = 0;
	//pcLandform land = pchara->GetAttackLand(outputaspect, outputdistance);
	valiable.doubles.dim(tstring(_T("direction")),temp = me()->aspect*45);
	//valiable.doubles.dim(tstring(_T("distance")),temp = outputdistance);
	//valiable.doubles.dim(tstring(_T("toPlaceX")),temp = land->placeX);
	//valiable.doubles.dim(tstring(_T("toPlaceY")),temp = land->placeY);

	//val.doubles.dim(tstring(_T("isValidAttack")),temp = (double)sg_pDungeonSystem->isCanNeighberAttack(pchara,pchara->aspect,1,0,0));

	bool didAnime = AnimationScript_AddAnimation(
		sg_pDungeonSystem->DataBase.EffectImport_Chara_Special(),
		sg_pDungeonSystem->DataBase.EffectImport_Name_Chara_Special(ID()),
		valiable);	

	if(didAnime)
	{
		return true;
	}
	else
	{
		return cCharacter::特殊攻撃_アニメ(valiable);
	}
}