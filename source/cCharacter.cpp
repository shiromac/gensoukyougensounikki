#include "stdafx.h"
#include "cCharacter.h"
#include "cEquipment.h"
#include "cMap.h"
#include "gameMainSystem/filemanage/cScriptRLayer.h"
#include "scriptprocesser/DropsScript.h"

#define TEXSIZE_SHADOW_X 32
#define TEXSIZE_SHADOW_Y 32

#define TEXSIZE_EMOTION_Y 512
#define TEXSIZE_EMOTION_X 256

#define TEXSIZE_OPTION_Y 64
#define TEXSIZE_OPTION_X 64

#define BOXSIZE 64

#define FILENAME_SHADOWTEX _T("chara\\common\\shadow.png")

#define FILENAME_EMOTIONTEX _T("chara\\common\\emotion.png")


#include "AttackAnimeAttriDEF.h"

//----------------------------------
//カラー
//----------------------------------
#define CHARACTER_NAME_COLOR_FRIEND (0xFF66FF66)
#define CHARACTER_NAME_COLOR_ENEMY (0xFFFF6666)
#define CHARACTER_NAME_COLOR_OTHER (0xFFFFFF66)

cCharacter::cCharacter(void)
{
	EXP = 0;
	MHP = 0;
	HP = 0;

	TurnEndFlag = 0;
	TurnEndFlag_half = 0;

	anime_stamp_step = 0;
	anime_emotion_step = 0;
	anime_option_step = 0;
	anime_stopStamp = false;
	opaque = 1;
	emotion.clear();

	int i;
	for(i=0;i<AI_MEMORY_NUM;i++)
	{
		AImemory[i] = 0;
	}
	
	attackequipment = NULLDROP;
	defenseequipment = NULLDROP;
	bulletequipment = NULLDROP;
	

	LV = 1;
	CLASS = 0;

	SetMHP();

	Stomach = 100;
	MaxStomach = 100;

	sightrange = 1;
	allsight = 0;

	LastSpelling = 0;


	anime_scaleX = 1;
	anime_scaleY = 1;
	anime_pitching = 0;
	anime_position.set(0,0,0,0);
	anime_yawing = 0;
}

cCharacter::~cCharacter(void)
{
	
	int i,size = luaScript_.size();
	if(sg_pDungeonSystem)
	{
		for(i=0;i<size;i++)
		{
			pDungeonStoryScript_LuaEnvironment pluaenv = luaScript_[i].lock();
			if(pluaenv)
			{
				sg_pDungeonSystem->deleteLuaEnv(pluaenv);
			}
		}
	}
	luaScript_.clear();
	
}

void cCharacter::visibleReset()
{
	visibleemotion = emotion;
	VisibleCLASS = CLASS;

	anime_scaleX = 1;
	anime_scaleY = 1;
	anime_pitching = 0;
	anime_position.set(0,0,0,0);
	anime_yawing = 0;
}

void cCharacter::Init(IDirect3DDevice9 *pDev, pcCharacter self)
{
	visibleaspect = aspect = ASPECT_DOWN;

	self_weakpointer = wpcCharacter(self);


	InitSub(pDev);//子クラスで使うかも

	

	GetTex_dot(pDev);

	Get_AI();

	Condition.init(me());

	//SetCharaAttribute();

	//HP = MHP = 0;
	HP = SetMHP();


}

void cCharacter::InitDungeonStart()
{
	pcScriptRLayer scriptlayer = sg_pDungeonSystem->DataBase.CharaImportData(ID(),tstring(_T("Script")));
	if(scriptlayer)
	{
		std::vector<tstring> scriptKeys;
		scriptlayer->getLuaScriptKeys(scriptKeys);
		cValiableField tempValiable;
		tempValiable.charas.dim(_T("me"),me());
		int i,size = scriptKeys.size();
		for(i=0;i<size;i++)
		{
			pDungeonStoryScript_LuaEnvironment plua(new DungeonStoryScript_LuaEnvironment(tString2luastring(( setStyle(ID())+scriptKeys[i] ).conclete_tstr())));
			luaScript_.push_back(wpDungeonStoryScript_LuaEnvironment(plua));
			sg_pDungeonSystem->insertLuaEnv(plua);
			plua->loadDungeonStoryScript(scriptlayer,scriptKeys[i],tempValiable);
		}
	}
}
void cCharacter::FinalizeDungeonEnd()
{
	int i,size = luaScript_.size();
	if(sg_pDungeonSystem)
	{
		for(i=0;i<size;i++)
		{
			sg_pDungeonSystem->deleteLuaEnv(luaScript_[i].lock());
		}
		luaScript_.clear();
	}
}
void cCharacter::settingInit()
{
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

}

void cCharacter::naturalSpawnInit()
{
	return;
}

int cCharacter::upMHP()
{
	return MHP=HP= GetMHP();
}
int cCharacter::SetMHP()
{
	return MHP=HP= GetMHP();
}
int cCharacter::GetMHP()
{
	return 1;
}

double cCharacter::GetAttackPower()
{
	int power;
	power = calcuAttackPower(attackequipment);
	if(defenseequipment != attackequipment)
	{
		power += calcuExtraAttackPower(defenseequipment);
	}

	cValiableField val;
	val.doubles.dim(変数_攻撃力ボーナス_倍率) = 1.0;
	val.doubles.dim(変数_攻撃力ボーナス_定数) = 0.0;
	sg_pDungeonSystem->CutInM().CutIn(me(), 攻撃力計算時_タイミング, val);
	
	power *= val.doubles[変数_攻撃力ボーナス_倍率];
	power += val.doubles[変数_攻撃力ボーナス_定数];
	return power*Condition.力係数();
}

double cCharacter::GetDefencePower()
{
	int power;
	power = calcuDefencePower(defenseequipment);
	if(defenseequipment != attackequipment)
	{
		power += calcuExtraDefencePower(attackequipment);
	}

	cValiableField val;
	val.doubles.dim(変数_防御力ボーナス_倍率) = 1.0;
	val.doubles.dim(変数_防御力ボーナス_定数) = 0.0;
	sg_pDungeonSystem->CutInM().CutIn(me(), 防御力計算時_タイミング, val);
	
	power *= val.doubles[変数_防御力ボーナス_倍率];
	power += val.doubles[変数_防御力ボーナス_定数];
	return power*Condition.守係数();
}

double cCharacter::calcuAttackPower(pcDroping pcEqu)
{
	pcEquipment pEqu = boost::dynamic_pointer_cast<cEquipment>(pcEqu);
	if(pEqu == NULL) return calcuAttackPower(0.0);
	
	return calcuAttackPower(pEqu->武器力最終値());
}
double cCharacter::calcuDefencePower(pcDroping pcEqu)
{
	pcEquipment pEqu = boost::dynamic_pointer_cast<cEquipment>(pcEqu);
	if(pEqu == NULL) return calcuDefencePower(0.0);
	
	return calcuDefencePower(pEqu->防具力最終値());
}
double cCharacter::calcuAttackPower(double equPower)
{
	return baseAttackPower();
}
double cCharacter::calcuDefencePower(double equPower)
{
	return baseDefencePower();
}
double cCharacter::calcuExtraAttackPower(pcDroping pcEqu)
{
	return 0;
}
double cCharacter::calcuExtraDefencePower(pcDroping pcEqu)
{
	return 0;
}
double cCharacter::calcuNoAppreciationExtraAttackPower(pcDroping pcEqu)
{
	return 0;
}
double cCharacter::calcuNoAppreciationExtraDefencePower(pcDroping pcEqu)
{
	return 0;
}



pcLandform cCharacter::GetAttackLand(int& outputaspect, int& outputdistance)
{
	cCoordinate aspectcoo;

	cValiableField valiable;
	valiable.doubles.dim(変数_距離) = 1;
	valiable.doubles.dim(変数_方向) = aspect;

	sg_pDungeonSystem->CutInM().CutIn(attackequipment,直接攻撃位置指定_タイミング,valiable);

	outputaspect = valiable.doubles[変数_方向];
	outputdistance = valiable.doubles[変数_距離];

	aspectcoo.SetAspect(valiable.doubles[変数_方向]);

	return sg_pDungeonSystem->Map().Land(placeX + valiable.doubles[変数_距離]*aspectcoo.x, placeY + valiable.doubles[変数_距離]*aspectcoo.y);
}
vector<pcDroping> cCharacter::GetDropItems()
{
	return holdItem;
}

//百分率
double cCharacter::GetHitPowerBase()
{
	double m = sg_pDungeonSystem->DataBase.CharaImportData_Value(ID(),(tstring)_T("攻撃命中力"),default_GetHitPowerBase());
	cValiableField valiable;
	valiable.doubles.dim(変数_命中力) = m;
	sg_pDungeonSystem->CutInM().CutIn(me(), 直接攻撃命中力計算時_タイミング, valiable);
	
	return valiable.doubles[変数_命中力];
}
double cCharacter::GetDodgePowerBase()
{
	double m = sg_pDungeonSystem->DataBase.CharaImportData_Value(ID(),(tstring)_T("攻撃回避力"),default_GetDodgePowerBase());
	cValiableField valiable;
	valiable.doubles.dim(変数_回避力) = m;
	sg_pDungeonSystem->CutInM().CutIn(me(), 直接攻撃回避力計算時_タイミング, valiable);
	
	return valiable.doubles[変数_回避力];
}
double cCharacter::GetItemDodgePowerBase()
{
	return sg_pDungeonSystem->DataBase.CharaImportData_Value(ID(),(tstring)_T("アイテム回避力"),default_GetItemDodgePowerBase());
}
double cCharacter::default_GetHitPowerBase()
{
	return sg_pDungeonSystem->DataBase.CharaImportData_Value((tstring)_T("Character基本値"),(tstring)_T("デフォルト攻撃命中力"),100.0);
}
double cCharacter::default_GetDodgePowerBase()
{
	return sg_pDungeonSystem->DataBase.CharaImportData_Value((tstring)_T("Character基本値"),(tstring)_T("デフォルト攻撃回避力"),100.0/16.0);
}
double cCharacter::default_GetItemDodgePowerBase()
{
	return sg_pDungeonSystem->DataBase.CharaImportData_Value((tstring)_T("Character基本値"),(tstring)_T("デフォルトアイテム回避力"),100.0/8.0);
}
double cCharacter::HP自然回復割合()
{
	return sg_pDungeonSystem->DataBase.CharaImportData_Value(ID(),(tstring)_T("HP自然回復割合"),0.005);
}
double cCharacter::HP自然回復開始ターン()
{
	return sg_pDungeonSystem->DataBase.CharaImportData_Value(ID(),(tstring)_T("HP自然回復開始ターン"),10);
}
double cCharacter::HP自然回復最低保障値()
{
	return sg_pDungeonSystem->DataBase.CharaImportData_Value(ID(),(tstring)_T("HP自然回復最低保障値"),0.5);
}

int cCharacter::isSpecialAttack()
{
	return (100*random() < PerOfspecialAttack());
}

tstring cCharacter::FullNameBase()
{
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.CharaImportLanguage(ID(),
		(tstring)_T("FullNameBase"));
	if(psr == NULL) return _T("No FullNameBase");
	return psr->topscript();
}
tstring cCharacter::FullNameSubBase()
{
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.CharaImportLanguage(ID(),
		(tstring)_T("FullNameSubBase"));
	if(psr == NULL) return _T("No FullNameSubBase");
	return psr->topscript();
}
tstring cCharacter::ShortNameBase()
{
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.CharaImportLanguage(ID(),
		(tstring)_T("ShortNameBase"));
	if(psr == NULL) return _T("No ShortNameBase");
	return psr->topscript();
}
tstring cCharacter::ShortNameSubBase()
{
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.CharaImportLanguage(ID(),
		(tstring)_T("ShortNameSubBase"));
	if(psr == NULL) return _T("No ShortNameSubBase");
	return psr->topscript();
}

tstring cCharacter::uniqueName()
{
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.CharaImportData(ID(),
		(tstring)_T("uniqueName"));
	if(psr == NULL) return _T("No uniqueName");
	return psr->topscript();
}
tstring cCharacter::surfaceName()
{
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.CharaImportData(ID(),
		(tstring)_T("surfaceName"));
	if(psr == NULL) return _T("No surfaceName");
	return psr->topscript();
}
StyleString cCharacter::titleName()
{
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.CharaImportLanguage(ID(),
		(tstring)_T("titleName"));
	if(psr == NULL) return _T("No titleName");
	
	StyleString sstr;
	Drop_StringAddStyle(psr,sstr);
	return sstr;
}

int cCharacter::被攻撃優先度()
{
	cValiableField valiable;
	valiable.doubles.dim(変数_汎用実数) = 1;
	sg_pDungeonSystem->CutInM().CutIn(me(), 被攻撃優先度計算時_タイミング, valiable);

	if(!雑魚属性())
	{
		valiable.doubles.val(変数_汎用実数) += 1;
	}
	return valiable.doubles.val(変数_汎用実数);
}
int cCharacter::デフォルト速度移動()
{
	return sg_pDungeonSystem->DataBase.CharaImportData_Value(ID(),(tstring)_T("デフォルト速度移動"),0);
}
int cCharacter::デフォルト速度攻撃()
{
	return sg_pDungeonSystem->DataBase.CharaImportData_Value(ID(),(tstring)_T("デフォルト速度攻撃"),デフォルト速度移動());
}
int cCharacter::死亡ドロップアイテムID()
{
	return 0;
}
int cCharacter::TalkEvent()
{
	cCoordinate aspectcoo;

	aspectcoo.x = sg_pDungeonSystem->pPlayerChara()->placeX - placeX;
	aspectcoo.y = sg_pDungeonSystem->pPlayerChara()->placeY - placeY;
	sg_pDungeonSystem->方向転換要請(me(), aspectcoo.GetAspect());
	
	return true;
}
pcCommand cCharacter::CharaSkillCommond()
{
	return pcCommand((cCommand*)NULL);
}
tstring cCharacter::LVStr()
{
	return g_Lang(_T("レベル表現_味方")).conclete_tstr();
}
void cCharacter::DrawShadow(IDirect3DDevice9 *pDev)
{

	
	c4DVector chara_place = visibleplace + anime_position;
	//影
	DO.setTexture(p_Texshadow, TEXSIZE_SHADOW_X, TEXSIZE_SHADOW_Y);

	DO.Width = BOXSIZE;
	DO.Height = BOXSIZE;

	DO.m_color.inputD3Dcolor(ShadowColor());
	DO.m_color.alpha = opaque*DO.m_color.alpha / (1+visibleplace.z);

	DO.CenterX =  
		MAPDRAWCENTERX + MAPTEXBOXSIZE*MAPTEXPOWER*(chara_place.x - mapForcus.x);
	
	DO.CenterY =  
		MAPDRAWCENTERY + MAPTEXBOXSIZE*MAPTEXPOWER*(chara_place.y - mapForcus.y);
	
	DO.m_TexRange.setLTRB(0,0,
						1,1);
	
	DO.Draw(pDev);
}
unsigned int cCharacter::ShadowColor()
{
	return 0xFF000000;
}

void cCharacter::Draw(IDirect3DDevice9 *pDev)
{

	pcCharacter pchara = Condition.anotherSurface();
	if( pchara == NULL )
	{
		pchara = me();
	}
	else
	{

		pchara->Init(pDev,pchara);
		pchara->visibleplace = visibleplace;
		pchara->mapForcus = mapForcus;
		pchara->opaque = opaque;
		pchara->VisibleCLASS = VisibleCLASS;
		pchara->visibleaspect = visibleaspect;
		pchara->anime_scaleX = anime_scaleX;
		pchara->anime_scaleY = anime_scaleY;
		pchara->anime_pitching = anime_pitching;
		pchara->anime_yawing = anime_yawing;
		pchara->anime_position = anime_position;
		pchara->anime_stamp_step = anime_stamp_step;
	}

	DrawShadow(pDev);


	pchara->DrawBody(pDev);

	pchara->OptionDraw(pDev);

	EmotionDraw(pDev);


}
void cCharacter::DrawBody(IDirect3DDevice9 *pDev)
{
	int texaspect = 0;

	int step = 0;

	
	if(anime_stopStamp)
	{
		if(GetTex_aspect_type() == CHARACTER_TEXASPECT_TURN)
		{//雛専用
			texaspect = aspect;
		}
		else
		{
			step = 0;
		}
	}
	else
	{
		if(GetTex_aspect_type() == CHARACTER_TEXASPECT_TURN)
		{//雛専用
			int a = anime_stamp_step*8/DEFAULTSTAMPSUMSTEP + 4;
			if(a > 8) a -= 8;//初期状態を下向き
			texaspect = a;
		}
		else
		{
			step = anime_stamp_step*4/DEFAULTSTAMPSUMSTEP;
		}
	}

	DO.setTexture(m_pTexture);//, GetTex_Size_dotX(), GetTex_Size_dotY());
	//GetTex_Size_dotX_ = DO.getTexSizeX();
	//GetTex_Size_dotY_ = DO.getTexSizeY();

	if(GetTex_aspect_type() == CHARACTER_TEXASPECT_FULLASPECT)
	{
		texaspect = visibleaspect;
	}
	else if(GetTex_aspect_type() == CHARACTER_TEXASPECT_SINPLE)
	{
		texaspect = ASPECT_DOWN;
	}
	else if(GetTex_aspect_type() == CHARACTER_TEXASPECT_FOURASPECT)
	{
		texaspect = (visibleaspect/2)*2;
	}
	else if(GetTex_aspect_type() == CHARACTER_TEXASPECT_TURN)
	{//雛専用
		step = 0;
	}

	//anime
	texaspect += ((anime_yawing + 45/2) * 8 ) / 360 + 8;
	texaspect = safeAspect(texaspect);

	c4DVector chara_place = visibleplace + anime_position;


	DO.m_TexRange.setLTRB(step/4.0	,texaspect/8.0,
						(step+1)/4.0	,(texaspect+1)/8.0);
	DO.Width = GetTex_Size_dotX() * DEFAULTTEXPOWER/4.0;
	DO.Height = GetTex_Size_dotY() * DEFAULTTEXPOWER/8.0;

	DO.CenterX = GetDrawDifference_dotX()*MAPTEXPOWER + 
		MAPDRAWCENTERX + MAPTEXBOXSIZE*MAPTEXPOWER*(chara_place.x - mapForcus.x);
	
	DO.setBottom( GetDrawDifference_dotY()*MAPTEXPOWER + 
		MAPDRAWCENTERY + MAPTEXBOXSIZE*MAPTEXPOWER*(chara_place.y -chara_place.z*0.5 - mapForcus.y)	
		+ MAPTEXBOXSIZE*MAPTEXPOWER/2);

	if(edgedrawswitch())
	{
		DO.colorblendmode = cDrawableObject::COLOR_BLEND_FILL;
		DO.m_color.inputD3Dcolor(ShadowColor());
		DO.m_color.alpha = opaque*255;
		DO.CenterY -= 4;
		DO.Draw(pDev);

		DO.CenterY += 4;

		DO.CenterX += 4;
		DO.Draw(pDev);
		DO.CenterX -= 8;
		DO.Draw(pDev);
		DO.CenterX += 4;
	}


	//anime
	DO.Rotation = anime_pitching;
	DO.ScaleX = anime_scaleX;
	DO.ScaleY = anime_scaleY;


	DO.colorblendmode = cDrawableObject::COLOR_BLEND_MULTIPLE;
	DO.m_color.ARGB(opaque*255,255,255,255);
	DO.Draw(pDev);
}

void cCharacter::OptionDraw(IDirect3DDevice9 *pDev)
{
	DO.setTexture(p_Texoption,TEXSIZE_OPTION_X,TEXSIZE_OPTION_Y);
	int step = anime_option_step*4/DEFAULTOPTIONSUMSTEP;
	cCoordinate coo;
	coo.x = MAPDRAWCENTERX + MAPTEXBOXSIZE*MAPTEXPOWER*(visibleplace.x - mapForcus.x);
	coo.y = MAPDRAWCENTERY + MAPTEXBOXSIZE*MAPTEXPOWER*(visibleplace.y - mapForcus.y) - 4;
	int fl = /*sin((double)anime_option_step/DEFAULTOPTIONSUMSTEP * 6.284 / 2)*2.5*/ -6;
	DO.Width = TEXSIZE_OPTION_X * DEFAULTTEXPOWER/4.0;
	DO.Height = TEXSIZE_OPTION_Y * DEFAULTTEXPOWER/4.0;

	int i;
	if(VisibleCLASS == 1)
	{
		i = 0;
		DO.m_TexRange.setLTRB(step/4.0	,i/4.0,(step+1)/4.0	,(i+1)/4.0);
		//DO.setRight(coo.x+MAPTEXBOXSIZE*MAPTEXPOWER/2);
		DO.CenterX = coo.x+MAPTEXBOXSIZE*MAPTEXPOWER*3/8;
		DO.CenterY = coo.y+fl*MAPTEXPOWER;
		//DO.CenterY = coo.y+fl*MAPTEXPOWER;
		DO.Draw(pDev);
	}
	else if(VisibleCLASS == 2)
	{
		i = 1;
		DO.m_TexRange.setLTRB(step/4.0	,i/4.0,(step+1)/4.0	,(i+1)/4.0);
		DO.CenterX = coo.x+MAPTEXBOXSIZE*MAPTEXPOWER*3/8;
		DO.CenterY = coo.y+fl*MAPTEXPOWER;
		DO.Draw(pDev);
		//i = 0;
		DO.m_TexRange.setLTRB(step/4.0	,i/4.0,(step+1)/4.0	,(i+1)/4.0);
		DO.CenterX = coo.x-MAPTEXBOXSIZE*MAPTEXPOWER*3/8;
		DO.CenterY = coo.y+fl*MAPTEXPOWER;
		DO.Draw(pDev);
	}
	else if(VisibleCLASS == 3)
	{
		i = 2;
		DO.m_TexRange.setLTRB(step/4.0	,i/4.0,(step+1)/4.0	,(i+1)/4.0);
		DO.CenterX = coo.x+MAPTEXBOXSIZE*MAPTEXPOWER*3/8;
		DO.CenterY = coo.y+fl*MAPTEXPOWER;
		DO.Draw(pDev);
		//i = 1;
		DO.m_TexRange.setLTRB(step/4.0	,i/4.0,(step+1)/4.0	,(i+1)/4.0);
		DO.CenterX = coo.x-MAPTEXBOXSIZE*MAPTEXPOWER*3/8;
		DO.CenterY = coo.y+fl*MAPTEXPOWER;
		DO.Draw(pDev);
		//i = 0;
		DO.m_TexRange.setLTRB(step/4.0	,i/4.0,(step+1)/4.0	,(i+1)/4.0);
		DO.CenterX = coo.x;
		DO.CenterY = coo.y+fl*MAPTEXPOWER+MAPTEXBOXSIZE*MAPTEXPOWER/4;
		DO.Draw(pDev);
	}
	else if(VisibleCLASS == 4)
	{
		i = 3;
		DO.m_TexRange.setLTRB(step/4.0	,i/4.0,(step+1)/4.0	,(i+1)/4.0);
		DO.CenterX = coo.x+MAPTEXBOXSIZE*MAPTEXPOWER*3/8;
		DO.CenterY = coo.y+fl*MAPTEXPOWER;
		DO.Draw(pDev);
		//i = 2;
		DO.m_TexRange.setLTRB(step/4.0	,i/4.0,(step+1)/4.0	,(i+1)/4.0);
		DO.CenterX = coo.x-MAPTEXBOXSIZE*MAPTEXPOWER*3/8;
		DO.CenterY = coo.y+fl*MAPTEXPOWER;
		DO.Draw(pDev);
		//i = 1;
		DO.m_TexRange.setLTRB(step/4.0	,i/4.0,(step+1)/4.0	,(i+1)/4.0);
		DO.CenterX = coo.x - MAPTEXBOXSIZE*MAPTEXPOWER/4;
		DO.CenterY = coo.y+fl*MAPTEXPOWER + MAPTEXBOXSIZE*MAPTEXPOWER/4;
		DO.Draw(pDev);
		//i = 0;
		DO.m_TexRange.setLTRB(step/4.0	,i/4.0,(step+1)/4.0	,(i+1)/4.0);
		DO.CenterX = coo.x + MAPTEXBOXSIZE*MAPTEXPOWER/4;
		DO.CenterY = coo.y+fl*MAPTEXPOWER + MAPTEXBOXSIZE*MAPTEXPOWER/4;
		DO.Draw(pDev);
	}
	else if(VisibleCLASS >= 5)
	{
		i = (step+3)%4;
		DO.m_TexRange.setLTRB(step/4.0	,i/4.0,(step+1)/4.0	,(i+1)/4.0);
		DO.CenterX = coo.x+MAPTEXBOXSIZE*MAPTEXPOWER*3/8;
		DO.CenterY = coo.y+fl*MAPTEXPOWER;
		DO.Draw(pDev);
		i = (step+2)%4;
		DO.m_TexRange.setLTRB(step/4.0	,i/4.0,(step+1)/4.0	,(i+1)/4.0);
		DO.CenterX = coo.x-MAPTEXBOXSIZE*MAPTEXPOWER*3/8;
		DO.CenterY = coo.y+fl*MAPTEXPOWER;
		DO.Draw(pDev);
		i = (step+1)%4;
		DO.m_TexRange.setLTRB(step/4.0	,i/4.0,(step+1)/4.0	,(i+1)/4.0);
		DO.CenterX = coo.x - MAPTEXBOXSIZE*MAPTEXPOWER/4;
		DO.CenterY = coo.y+fl*MAPTEXPOWER + MAPTEXBOXSIZE*MAPTEXPOWER/4;
		DO.Draw(pDev);
		i = (step+0)%4;
		DO.m_TexRange.setLTRB(step/4.0	,i/4.0,(step+1)/4.0	,(i+1)/4.0);
		DO.CenterX = coo.x + MAPTEXBOXSIZE*MAPTEXPOWER/4;
		DO.CenterY = coo.y+fl*MAPTEXPOWER + MAPTEXBOXSIZE*MAPTEXPOWER/4;
		DO.Draw(pDev);
	}
}

void cCharacter::Process(IDirect3DDevice9 *pDev)
{
	if(!Condition.行動不能である_visual())
	{
		if(visibleemotion.size() == 0)
		{
			anime_stamp_step += 6;
		}
		else if(visibleemotion.count(EMOTION_SUBINFO_STEP_ATHIRD))
		{
			anime_stamp_step += 2;
		}
		else if(visibleemotion.count(EMOTION_SUBINFO_STEP_AHALF))
		{
			anime_stamp_step += 3;
		}
		else if(visibleemotion.count(EMOTION_SUBINFO_STEP_DOUBLE))
		{
			anime_stamp_step += 12;
		}
		else if(visibleemotion.count(EMOTION_SUBINFO_STEP_TRIPLE))
		{
			anime_stamp_step += 18;
		}
		else
		{
			anime_stamp_step += 6;
		}


		if(anime_stamp_step>=DEFAULTSTAMPSUMSTEP)
		{
			anime_stamp_step = 0;
		}
	}

	anime_emotion_step++;

	if(anime_emotion_step>=DEFAULTEMOTIONSUMSTEP)
	{
		anime_emotion_step = 0;
	}

	anime_option_step++;
	if(anime_option_step>=DEFAULTOPTIONSUMSTEP)
	{
		anime_option_step = 0;
	}
	

}

void cCharacter::GetTex_dot(IDirect3DDevice9 *pDev)
{
	TCHAR path[128] = _T("");
	_stprintf(path,_T("chara\\dot\\%sdot.png"),surfaceName().c_str());
	m_pTexture = g_GameEnv.m_GlobalResourse->getTextureFromFile(
		pDev,path);
	_stprintf(path,_T("chara\\op\\%sop.png"),surfaceName().c_str());
	p_Texoption = g_GameEnv.m_GlobalResourse->getTextureFromFile(
		pDev,path);

	p_Texshadow = g_GameEnv.m_GlobalResourse->getTextureFromFile(
		pDev,FILENAME_SHADOWTEX);
	p_Texemotion = g_GameEnv.m_GlobalResourse->getTextureFromFile(
		pDev,FILENAME_EMOTIONTEX);


	cDrawingObject drawobject;
	drawobject.setTexture(m_pTexture);//, GetTex_Size_dotX(), GetTex_Size_dotY());
	GetTex_Size_dotX_ = drawobject.getTexSizeX();
	GetTex_Size_dotY_ = drawobject.getTexSizeY();
}


//キャラネーム色
unsigned long cCharacter::GetColor_charaname()
{
	if(Forse == CHARACTER_FORSE_ENEMY)
	{
		return CHARACTER_NAME_COLOR_ENEMY;
	}
	else if(Forse == CHARACTER_FORSE_FRIEND)
	{
		return CHARACTER_NAME_COLOR_FRIEND;	
	}
	else
	{
		return CHARACTER_NAME_COLOR_OTHER;
	}
}

/*
//ネーム系

tstring cCharacter::FullNameBase(){
	return sg_pDungeonSystem.DataBase.CharaImportData(ID(),_T("FullNameBase"));
}
tstring cCharacter::FullNameSubBase(){
	return sg_pDungeonSystem.DataBase.CharaImportData(ID(),_T("FullNameSubBase"));
}
tstring cCharacter::ShortNameBase(){
	return sg_pDungeonSystem.DataBase.CharaImportData(ID(),_T("ShortNameBase"));
}
tstring cCharacter::ShortNameSubBase(){
	return sg_pDungeonSystem.DataBase.CharaImportData(ID(),_T("ShortNameSubBase"));
}


//唯一名
tstring cCharacter::uniqueName(){
	return sg_pDungeonSystem.DataBase.CharaImportData(ID(),_T("uniqueName"));
}

tstring cCharacter::usefulName(){
	return sg_pDungeonSystem.DataBase.CharaImportData(ID(),_T("usefulName"));
}

//見た目
tstring cCharacter::surfaceName(){
	return sg_pDungeonSystem.DataBase.CharaImportData(ID(),_T("surfaceName"));
}

*/

void cCharacter::EmotionDraw(IDirect3DDevice9 *pDev)
{
	cDrawingObject DO;
	DO.m_color.alpha = opaque*255;
	int step = anime_emotion_step*4/DEFAULTEMOTIONSUMSTEP;


	
	DO.Width = BOXSIZE;
	DO.Height = BOXSIZE;

	DO.CenterX = GetDrawDifference_dotX()*MAPTEXPOWER + 
		MAPDRAWCENTERX + MAPTEXBOXSIZE*MAPTEXPOWER*(visibleplace.x - mapForcus.x);
	
	DO.setBottom( GetDrawDifference_dotY()*MAPTEXPOWER + 
		-GetDrawHeadtall_dotY()*MAPTEXPOWER +
		MAPDRAWCENTERY + MAPTEXBOXSIZE*MAPTEXPOWER*(visibleplace.y - mapForcus.y)	
		+ MAPTEXBOXSIZE*MAPTEXPOWER/2);

	int ui,vi;

	set<int>::iterator itr = visibleemotion.begin();
	for(;itr != visibleemotion.end();itr++)
	{
		int ei = (*itr)-1;

		if(ei<0 || ei>=32) return;

		ui = ei % 2;
		vi = ei / 2;

		DO.setTexture(p_Texemotion,TEXSIZE_EMOTION_X,TEXSIZE_EMOTION_Y);
		DO.m_TexRange.setLTRB((ui*4+step)/8.0	,vi/16.0,
							(ui*4+step+1)/8.0	,(vi+1)/16.0);
		DO.Draw(pDev);
	}

	Condition.Draw(pDev);
}

set<キャラ属性::キャラ属性リスト>& cCharacter::CharaAttribute()
{
	sg_pDungeonSystem->DataBase.CharaAttri(ID(),CHARAATTRI);
	return CHARAATTRI;
}

int cCharacter::Get_AI()
{
	//pAI = g_GetAI(Get_kindofAI_ID());
	pAI = Get_kindofAI();

	pAI->Init(me());

	return (pAI != NULL);
}

bool cCharacter::死亡()
{
	return (HP <= 0 && !LastSpelling);
}

pcDroping cCharacter::足元()
{
	return sg_pDungeonSystem->キャラ足元地形(me())->pOnDrop;
}

pcLandform cCharacter::足元地形()
{
	return sg_pDungeonSystem->キャラ足元地形(me());
}


int cCharacter::通常攻撃効果()
{
	sg_pDungeonSystem->直接攻撃要請(me());

	return true;
}

int cCharacter::通常攻撃()
{

	return 通常攻撃効果();
}
int cCharacter::通常攻撃_アニメ()
{
	sg_pDungeonSystem->AnimationManager().
		EasyAnime_Attackcharacter(me(),
									ATTACKATTRI_PUNCH1);
	return true;
}
bool cCharacter::通常攻撃_壁斜めすり抜け()
{
	return false;
}
bool cCharacter::通常攻撃_壁すり抜け()
{
	return false;
}

int cCharacter::通常防御_アニメ()
{
	return true;
}

int cCharacter::特殊攻撃効果(cValiableField& valiable)
{
	sg_pDungeonSystem->直接攻撃要請(me());
	return true;
}
int cCharacter::特殊攻撃(cValiableField& valiable)
{
	if(Condition.封印状態())
	{
		return false;
	}

	if(特殊攻撃効果(valiable))
	{
		特殊攻撃_アニメ(valiable);
		return true;
	}
	return false;
}
int cCharacter::AI特殊攻撃選択pre(cValiableField& valiable)
{
	if(!(AI特殊攻撃選択確率() > random()) || Condition.封印状態())
	{
		return false;
	}
	return AI特殊攻撃選択(valiable);
}
int cCharacter::AI特殊攻撃選択(cValiableField& valiable)
{
	return false;
}
int cCharacter::特殊攻撃_アニメ(cValiableField& valiable)
{
	return 通常攻撃_アニメ();
}
void cCharacter::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	return;
}
void cCharacter::パッシブ能力sub(タイミング timing, cValiableField& valiable)
{
	return;
}
bool cCharacter::すれ違い許可(pcCharacter pchara)
{
	return !sg_pDungeonSystem->キャラクター敵対判定(me(),pchara);
}

//割り込みイベント
/*
int cCharacter::CutInEvent(int timing)
{
	if(attackequipment != NULL) attackequipment->CutInEvent(timing);
	if(defenseequipment != NULL) defenseequipment->CutInEvent(timing);

	int i;
	for(i=0;i<SPELL_EQUIP_MAXNUM;i++)
	{
		if(spellequipment[i] != NULL) spellequipment[i]->CutInEvent(timing);
	}

	return 0;
}


double cCharacter::CutInValue(int timing)
{
	double result = 0;

	if(attackequipment != NULL) result += attackequipment->CutInValue(timing);
	if(defenseequipment != NULL) result += defenseequipment->CutInValue(timing);

	int i;
	for(i=0;i<SPELL_EQUIP_MAXNUM;i++)
	{
		if(spellequipment[i] != NULL) result += spellequipment[i]->CutInValue(timing);
	}

	return result;
}

void cCharacter::CutInAttack(int timing, cAttackinformation & attinfo)
{

	if(attackequipment != NULL) attackequipment->CutInAttack(timing,attinfo);
	if(defenseequipment != NULL) defenseequipment->CutInAttack(timing,attinfo);

	int i;
	for(i=0;i<SPELL_EQUIP_MAXNUM;i++)
	{
		if(spellequipment[i] != NULL) spellequipment[i]->CutInAttack(timing,attinfo);
	}

	return;
}
*/

void cCharacter::CutIn(タイミング timing, cValiableField& valiable)
{
	double result = 0;


	/*
	if(attackequipment == defenseequipment)
	{//両手
		if(attackequipment != NULL) attackequipment->CutIn(timing,valiable);
	}
	else
	{
		if(attackequipment != NULL) attackequipment->CutIn(timing,valiable);
		if(defenseequipment != NULL) defenseequipment->CutIn(timing,valiable);
	}

	int i;
	for(i=0;i<SPELL_EQUIP_MAXNUM;i++)
	{
		if(spellequipment[i] != NULL) spellequipment[i]->CutIn(timing,valiable);
	}
	*/


	Condition.CutIn(timing,valiable);

	if(timing == フロア開始_タイミング)
	{
		cValiableField valf;
		valf.doubles.dim(変数_汎用実数) = 1;
		valf.doubles.dim(変数_汎用ブール) = 0;
		sg_pDungeonSystem->CutInM().CutIn(me(),キャラクター目視範囲計算時_タイミング,valf);
		sightrange = valf.doubles[変数_汎用実数];
		allsight = valf.doubles[変数_汎用ブール];
	
	}

	if(timing == アクション終了_タイミング)
	{//毎ターン情報更新
		

		cValiableField valf;
		valf.doubles.dim(変数_汎用実数) = 1;
		valf.doubles.dim(変数_汎用ブール) = 0;
		sg_pDungeonSystem->CutInM().CutIn(me(),キャラクター目視範囲計算時_タイミング,valf);
		sightrange = valf.doubles[変数_汎用実数];
		allsight = valf.doubles[変数_汎用ブール];
	
		
	}

	//速度
	else if(timing == 攻撃可判定_タイミング)
	{
		int diff = min(デフォルト速度移動(),Condition.速度度数()) - min(デフォルト速度攻撃(),Condition.速度度数());
		
		if((max(Condition.速度度数(),0) - diff) < sg_pDungeonSystem->s_turn_speed())
		{//s_turn_speed() = 0~2 ０が通常
			valiable.doubles.val(変数_汎用ブール) *= 0;//不許可
		}
	}

	if(!Condition.封印状態())
	{
		パッシブ能力(timing,valiable);
		パッシブ能力sub(timing,valiable);
		パッシブ能力属性弱点耐性(timing,valiable);
	
	}

	int i, size = luaScript_.size();
	tstring functionName(_T("cutIn"));
	for(i=0;i<size;i++)
	{
		pDungeonStoryScript_LuaEnvironment pluaScriptEnv = luaScript_[i].lock();
		if(pluaScriptEnv && pluaScriptEnv->luaState() != NULL)
		{
			if(!pluaScriptEnv->function(functionName, timing, boost::ref(valiable)))
			{
				pluaScriptEnv->outputErrorFile(luaScriptErrorFileName(functionName, pluaScriptEnv));
			}
		}
	}

}


#define def_AttackAttriWeekStrong_routine(NAME,DEVALUTVALUE) \
double cCharacter::NAME()\
{\
	return sg_pDungeonSystem->DataBase.CharaImportData_Value((tstring)_T("Enemy基本値"),(tstring)_T(#NAME),DEVALUTVALUE);\
}

def_AttackAttriWeekStrong_routine(強特攻耐性_ダメージ軽減％,0)
def_AttackAttriWeekStrong_routine(特攻耐性_ダメージ軽減％,0)
def_AttackAttriWeekStrong_routine(強元気耐性_上昇強度,0)
def_AttackAttriWeekStrong_routine(強元気耐性_上昇ターン,0)
def_AttackAttriWeekStrong_routine(元気耐性_上昇強度,0)
def_AttackAttriWeekStrong_routine(元気耐性_上昇ターン,0)
def_AttackAttriWeekStrong_routine(強頑強耐性_上昇強度,0)
def_AttackAttriWeekStrong_routine(強頑強耐性_上昇ターン,0)
def_AttackAttriWeekStrong_routine(頑強耐性_上昇強度,0)
def_AttackAttriWeekStrong_routine(頑強耐性_上昇ターン,0)
def_AttackAttriWeekStrong_routine(強特攻弱点_ダメージ増加％,0)
def_AttackAttriWeekStrong_routine(特攻弱点_ダメージ増加％,0)
def_AttackAttriWeekStrong_routine(強脱力弱点_下降強度,0)
def_AttackAttriWeekStrong_routine(強脱力弱点_下降ターン,0)
def_AttackAttriWeekStrong_routine(脱力弱点_下降強度,0)
def_AttackAttriWeekStrong_routine(脱力弱点_下降ターン,0)
def_AttackAttriWeekStrong_routine(強軟弱弱点_下降強度,0)
def_AttackAttriWeekStrong_routine(強軟弱弱点_下降ターン,0)
def_AttackAttriWeekStrong_routine(軟弱弱点_下降強度,0)
def_AttackAttriWeekStrong_routine(軟弱弱点_下降ターン,0)


bool isDuplicate(set<攻撃属性::攻撃属性>& set1, multiset<int>& set2)
{
	if(set1.empty() || set2.empty())
	{
		return false;
	}

	set<攻撃属性::攻撃属性>::iterator itr1;
	for(itr1 = set1.begin(); itr1 != set1.end() ;itr1++)
	{
		if(set2.count(*itr1))
		{
			return true;
		}
	}
	return false;
}

int duplicateDensity(set<攻撃属性::攻撃属性>& set1, multiset<int>& set2)
{
	if(set1.empty() || set2.empty())
	{
		return 0;
	}

	int deep = 0;
	set<攻撃属性::攻撃属性>::iterator itr1;
	for(itr1 = set1.begin(); itr1 != set1.end() ;itr1++)
	{
		deep += (int)set2.count(*itr1);
	}
	return deep;
}

void cCharacter::パッシブ能力属性弱点耐性(タイミング timing, cValiableField& valiable)
{
	if(timing == ダメージ計算防御時_タイミング)
	{
		
		set<攻撃属性::攻撃属性> attackAttri;
		{
			double defBonus = 0;
			sg_pDungeonSystem->DataBase.CharaStrongDamageBigAttri(ID(),attackAttri);
			if(isDuplicate(attackAttri,valiable.intsets.val(変数_属性)))
			{
				defBonus = max(defBonus,強特攻耐性_ダメージ軽減％());
			}
			sg_pDungeonSystem->DataBase.CharaStrongDamageSmallAttri(ID(),attackAttri);
			if(isDuplicate(attackAttri,valiable.intsets.val(変数_属性)))
			{
				defBonus = max(defBonus,特攻耐性_ダメージ軽減％());
			}
			
			valiable.doubles.val(変数_耐性ボーナス_倍率％) += defBonus;
		}
		{
			double damegeBonus = 0;
			sg_pDungeonSystem->DataBase.CharaWeekDamageBigAttri(ID(),attackAttri);
			if(isDuplicate(attackAttri,valiable.intsets.val(変数_属性)))
			{
				int density = duplicateDensity(attackAttri,valiable.intsets.val(変数_属性));
				damegeBonus += 強特攻弱点_ダメージ増加％()*density;
			}
			sg_pDungeonSystem->DataBase.CharaWeekDamageSmallAttri(ID(),attackAttri);
			if(isDuplicate(attackAttri,valiable.intsets.val(変数_属性)))
			{
				int density = duplicateDensity(attackAttri,valiable.intsets.val(変数_属性));
				damegeBonus += 特攻弱点_ダメージ増加％()*density;
			}
			valiable.doubles.val(変数_耐性ボーナス_倍率％) -= damegeBonus;
		}
		{
			double attackUpBonusPower = 0, attackUpBonusTurn = 0;
			sg_pDungeonSystem->DataBase.CharaStrongAttackBigAttri(ID(),attackAttri);
			if(isDuplicate(attackAttri,valiable.intsets.val(変数_属性)))
			{
				int density = duplicateDensity(attackAttri,valiable.intsets.val(変数_属性));
				attackUpBonusPower += 強元気耐性_上昇強度()*density;
				attackUpBonusTurn = max(attackUpBonusTurn,強元気耐性_上昇ターン());
			}
			sg_pDungeonSystem->DataBase.CharaStrongAttackSmallAttri(ID(),attackAttri);
			if(isDuplicate(attackAttri,valiable.intsets.val(変数_属性)))
			{
				int density = duplicateDensity(attackAttri,valiable.intsets.val(変数_属性));
				attackUpBonusPower += 元気耐性_上昇強度()*density;
				attackUpBonusTurn = max(attackUpBonusTurn,元気耐性_上昇ターン());
			}
			double attackDownBonusPower = 0, attackDownBonusTurn = 0;
			sg_pDungeonSystem->DataBase.CharaWeekAttackBigAttri(ID(),attackAttri);
			if(isDuplicate(attackAttri,valiable.intsets.val(変数_属性)))
			{
				int density = duplicateDensity(attackAttri,valiable.intsets.val(変数_属性));
				attackDownBonusPower += 強脱力弱点_下降強度()*density;
				attackDownBonusTurn = max(attackDownBonusTurn,強脱力弱点_下降ターン());
			}
			sg_pDungeonSystem->DataBase.CharaWeekAttackSmallAttri(ID(),attackAttri);
			if(isDuplicate(attackAttri,valiable.intsets.val(変数_属性)))
			{
				int density = duplicateDensity(attackAttri,valiable.intsets.val(変数_属性));
				attackDownBonusPower += 脱力弱点_下降強度()*density;
				attackDownBonusTurn = max(attackDownBonusTurn,脱力弱点_下降ターン());
			}
			
			double attackBonusPower = attackUpBonusPower + -attackDownBonusPower;
			if(attackBonusPower != 0)
			{
				if(attackBonusPower > 0)
				{
					sg_pDungeonSystem->元気要請(me(),attackBonusPower,attackUpBonusTurn,true);
				}
				else
				{
					sg_pDungeonSystem->脱力要請(me(),-attackBonusPower,attackDownBonusTurn,true);
				}
			}
		}
		{
			double defenceUpBonusPower = 0, defenceUpBonusTurn = 0;
			sg_pDungeonSystem->DataBase.CharaStrongDefenceBigAttri(ID(),attackAttri);
			if(isDuplicate(attackAttri,valiable.intsets.val(変数_属性)))
			{
				int density = duplicateDensity(attackAttri,valiable.intsets.val(変数_属性));
				defenceUpBonusPower += 強頑強耐性_上昇強度()*density;
				defenceUpBonusTurn = max(defenceUpBonusTurn,強頑強耐性_上昇ターン());
			}
			sg_pDungeonSystem->DataBase.CharaStrongDefenceSmallAttri(ID(),attackAttri);
			if(isDuplicate(attackAttri,valiable.intsets.val(変数_属性)))
			{
				int density = duplicateDensity(attackAttri,valiable.intsets.val(変数_属性));
				defenceUpBonusPower += 頑強耐性_上昇強度()*density;
				defenceUpBonusTurn = max(defenceUpBonusTurn,頑強耐性_上昇ターン());
			}
			double defenceDownBonusPower = 0, defenceDownBonusTurn = 0;
			sg_pDungeonSystem->DataBase.CharaWeekDefenceBigAttri(ID(),attackAttri);
			if(isDuplicate(attackAttri,valiable.intsets.val(変数_属性)))
			{
				int density = duplicateDensity(attackAttri,valiable.intsets.val(変数_属性));
				defenceDownBonusPower += 強軟弱弱点_下降強度()*density;
				defenceDownBonusTurn = max(defenceDownBonusTurn,強軟弱弱点_下降ターン());
			}
			sg_pDungeonSystem->DataBase.CharaWeekDefenceSmallAttri(ID(),attackAttri);
			if(isDuplicate(attackAttri,valiable.intsets.val(変数_属性)))
			{
				int density = duplicateDensity(attackAttri,valiable.intsets.val(変数_属性));
				defenceDownBonusPower += 軟弱弱点_下降強度()*density;
				defenceDownBonusTurn = max(defenceDownBonusTurn,軟弱弱点_下降ターン());
			}
			
			double defenceBonusPower = defenceUpBonusPower + -defenceDownBonusPower;
			if(defenceBonusPower != 0)
			{
				if(defenceBonusPower > 0)
				{
					sg_pDungeonSystem->頑強要請(me(),defenceBonusPower,defenceUpBonusTurn,true);
				}
				else
				{
					sg_pDungeonSystem->軟弱要請(me(),-defenceBonusPower,defenceDownBonusTurn,true);
				}
			}
		}
	}
}

tstring cCharacter::luaScriptErrorFileName(tstring& functionName, pDungeonStoryScript_LuaEnvironment& luaScriptEnv)
{
	return _T("Error_") + me()->uniqueName() + _T("_Script_") + luaString2tstring(luaScriptEnv->name().c_str()) + _T("::")  + functionName.c_str() + _T(".txt");
}
