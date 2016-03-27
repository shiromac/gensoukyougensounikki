#include "cBoss.h"
#include "cDungeonSystem.h"
#include "EffectFunctions.h"
#include "gameMainSystem/filemanage/cScriptRLayer.h"
#include "Event1.h"

#include "gameMainSystem/cEnvironment.h"
cBoss::cBoss(void)
{
	spellIndex_ = -1;

}


cBoss::~cBoss(void)
{

}
void cBoss::makeLuaEnv()
{
	
	pDungeonStoryScript_LuaEnvironment plua(new DungeonStoryScript_LuaEnvironment(tString2luastring(setStyle(ID()).conclete_tstr())));
	luaEnv_ = wpDungeonStoryScript_LuaEnvironment(plua);
	sg_pDungeonSystem->insertLuaEnv(plua);
}

void cBoss::deleteLuaEnv()
{
	//中断すると呼び出されなくてメモリリークする。要改善
	if(luaEnv())
	{
		sg_pDungeonSystem->insertLuaEnv(luaEnv());
	}
}

void cBoss::InitSub(IDirect3DDevice9 *pDev)
{

}

void cBoss::settingInit()
{
	cValiableField valiable;
	valiable.charas.dim(_T("boss"), me());//循環参照
	makeLuaEnv();

	pcScriptRLayer layer = sg_pDungeonSystem->DataBase.CharaImportData(ID(),(tstring)_T("ボススクリプト"));
	if(layer != NULL)
	{
		//メモリリーク！（循環参照が原因だった）
		
		layer->getLuaScriptKeys(bossScriptKeys_);
		if(!bossScriptKeys_.empty())
		{
			luaEnv()->loadDungeonStoryScript(layer
			,bossScriptKeys_[0],valiable);

		}
		
	}

	onMahoujin();
}

void cBoss::CutIn(タイミング timing, cValiableField& valiable)
{
	cCharacter::CutIn(timing, valiable);
	if(timing == 死の誘い追加直前_タイミング)
	{
		valiable.doubles[変数_汎用ブール] = 0;//効果無効
	}
	else if(timing == 死亡直前_タイミング)
	{
	
		if(!isNoSpell())
		{
			if(valiable.doubles[変数_死亡回避] == 0)
			{//まだ死亡回避していない

				valiable.doubles[変数_死亡回避] = 1;//死亡回避

				int exp = baseEXP();
				if(exp > 0)
				{
					sg_pDungeonSystem->経験値獲得( sg_pDungeonSystem->pPlayerChara(), exp);
				}
				gotoNextSpell();
			}
		}
	}
	else if(timing == 封印追加直前_タイミング)
	{
	
		valiable.doubles[変数_汎用ブール] = 1;//封印無効

	}
	else if(timing == 主人公ターン開始直前_タイミング)
	{
		if(!isNoSpell())
		{
			if(nowSpell() && pspelleffect_)
			{
				pspelleffect_->setTurn(nowSpell()->spellRestTurn(), (nowSpell()->spellRestTurn() < nowSpell()->spellTurnLimitCount()*0.3));
				pspelleffect_->setRestHPper((double)me()->HP/me()->MHP);
			}
			if(nowSpell())
			{
				if(nowSpell()->spellRestTurn() <= 0)
				{
					gotoNextSpell();
				}
			}
		}
		if(nowSpell())
		{
			if(!sg_pDungeonSystem->フロア時間飛ばし状態()) {
				nowSpell()->弾幕展開(valiable);
			}
		}
	}

	if(nowSpell())
	{
		if(!sg_pDungeonSystem->フロア時間飛ばし状態()) {
			nowSpell()->パッシブ能力(timing, valiable);
		}
	}
	パッシブ能力(timing, valiable);
}
void cBoss::gotoNextSpell()
{
	endSpell(spellIndex_);
	spellIndex_ += 1;

	sg_pDungeonSystem->満腹度設定要請(me(), 100, false);
	me()->MHP = baseMHP();
	sg_pDungeonSystem->回復要請(me(), baseMHP(), false);
	sg_pDungeonSystem->精神異常治療要請(me(), false);
	sg_pDungeonSystem->身体異常治療要請(me(), false);
	sg_pDungeonSystem->呪術異常治療要請(me(), false);
	//EffectFunctions::ボスエフェクト集中(me()->placeX, me()->placeY, 0);

	loadSpell(spellIndex_);

	if(givedUp())
	{
		giveUpEvent();
		//deleteLuaEnv();これもバグる^q^
	}
	else
	{
		
		startSpell(spellIndex());
	}
}
bool cBoss::isNoSpell()
{
	return sg_pDungeonSystem->DataBase.CharaImportData_Value(ID(),(tstring)_T("isNoSpell"),0);
}
bool cBoss::givedUp()
{
	return (spellIndex() >= spellNum());
}
void cBoss::startSpell(int index)
{
	if(luaEnv()->luaState() != NULL && !luaEnv()->function(_T("startSpell"),index))
	{
		luaEnv()->outputErrorFile(_T("Error_")+uniqueName()+setStyle(index).conclete_tstr()+_T("_startSpell.txt"));
	}
	if(nowSpell())
	{
		sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("spell.wav"),me()->足元地形()->place);
	
		pspelleffect_ = pcaBossEspell(new caBossEspell(me(),
			nowSpell()->spellName(), nowSpell()->imageColor1(), nowSpell()->imageColor2()));

		pspelleffect_->setRestSpellNum(spellNum()-spellIndex_-1);
		pspelleffect_->setTurn(nowSpell()->spellRestTurn(), false);
		spellEndPointer_ = pspelleffect_->End.pointer();
		sg_pDungeonSystem->AnimationManager().AddAnime_parallel(pspelleffect_);

		nowSpell()->setSpellStartTurn( sg_pDungeonSystem->SumTurnCount() );
		nowSpell()->startSpell();
	}
				
}
void cBoss::endSpell(int index)
{
	sg_pDungeonSystem->強制ターンエンド要請(me());
	if(nowSpell())
	{
		nowSpell()->eraseDanmaku(me());
	}
	if(luaEnv()->luaState() != NULL && !luaEnv()->function(_T("endSpell"),index))
	{
		luaEnv()->outputErrorFile(_T("Error_")+uniqueName()+setStyle(index).conclete_tstr()+_T("_endSpell.txt"));
	}
	if(nowSpell())
	{
		*spellEndPointer_ = true;
		nowSpell()->endSpell();
		if(sg_pDungeonSystem->pPlayerChara()->HP < sg_pDungeonSystem->pPlayerChara()->MHP)
		{
			sg_pDungeonSystem->回復要請(sg_pDungeonSystem->pPlayerChara(),sg_pDungeonSystem->pPlayerChara()->MHP,true);
		}
		sg_pDungeonSystem->精神異常治療要請(sg_pDungeonSystem->pPlayerChara(),false);
		sg_pDungeonSystem->身体異常治療要請(sg_pDungeonSystem->pPlayerChara(),false);
		sg_pDungeonSystem->呪術異常治療要請(sg_pDungeonSystem->pPlayerChara(),false);
		sg_pDungeonSystem->速度異常治療要請(sg_pDungeonSystem->pPlayerChara(),false);
	}
}

void cBoss::giveUpEvent()
{
	if(luaEnv()->luaState() != NULL && !luaEnv()->function(_T("giveUpEvent")))
	{
		luaEnv()->outputErrorFile(_T("Error_")+uniqueName()+_T("_giveUpEvent.txt"));
	}
	deleteLuaEnv();
}
bool cBoss::loadSpell(int spellIndex)
{

	pcScriptRLayer layer = sg_pDungeonSystem->DataBase.CharaImportData(ID(),(tstring)_T("スペルスクリプト"));
	if(layer != NULL)
	{
		layer->getLuaScriptKeys(spellScriptKeys_);
		//メモリリーク！（循環参照が原因だった）
		if (spellScriptKeys_.size() > spellIndex) {

			nowSpell_ = pcBossSpell(new cBossSpell(
				layer,
				spellScriptKeys_[spellIndex],
				me(),
				spellIndex));

			layer->getLuaScriptKeys(bossScriptKeys_);
			if(!bossScriptKeys_.empty())
			{
				cValiableField valiable;
				luaEnv()->loadDungeonStoryScript(layer
				,bossScriptKeys_[0],valiable);

			}
		}
	}
	return false;
}

pcLandform cBoss::AIお気に入り地形(cValiableField& valiable)
{
	if(nowSpell())
	{
		return nowSpell()->AIお気に入り地形(valiable);
	}
	
	pcLandform pland;
	if(luaEnv() && luaEnv()->luaState() != NULL &&
		luaEnv()->function_returnable(_T("bossMoveTargetLandform"),pland))
	{
		if( sg_pDungeonSystem->Map().edgeLand() == pland)
		{
			return NULLLAND;
		}
		else
		{
			return pland;
		}
	}
	return NULLLAND;

}
int cBoss::AI特殊攻撃選択(cValiableField& valiable)
{
	if(nowSpell())
	{
		return nowSpell()->AI特殊攻撃選択(valiable);
	}
	
	int result = 0;
	if(luaEnv() && luaEnv()->luaState() != NULL &&
		luaEnv()->function_returnable(_T("AISkillChoise"),result, boost::ref(valiable)))
	{
		return result;
	}
	return false;
}
int cBoss::特殊攻撃効果(cValiableField& valiable)
{
	if(nowSpell())
	{
		return nowSpell()->特殊攻撃効果(valiable);
	}

	int result = 0;
	if(luaEnv() && luaEnv()->luaState() != NULL &&
		luaEnv()->function_returnable(_T("skillEffect"),result, boost::ref(valiable)))
	{
		return result;
	}
	return false;
}
int cBoss::特殊攻撃_アニメ(cValiableField& valiable)
{
	if(nowSpell())
	{
		return nowSpell()->特殊攻撃_アニメ(valiable);
	}
	return false;
}

void cBoss::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	//no
	
	if(luaEnv() && luaEnv()->luaState() != NULL &&
		luaEnv()->function(_T("passiveSkill"), timing, boost::ref(valiable)))
	{
		return;
	}
}

int cBoss::spellNum()
{
	return sg_pDungeonSystem->DataBase.CharaImportData_Value(ID(),(tstring)_T("スペル数"),3);
}
int cBoss::HaveEXP()
{
	if(isNoSpell()) return sg_pDungeonSystem->DataBase.CharaImportData_Value(ID(),(tstring)_T("経験値"),1);
	return 0;
}
int cBoss::GetMHP()
{
	if(isNoSpell()) return sg_pDungeonSystem->DataBase.CharaImportData_Value(ID(),(tstring)_T("MHP"),1);
	return 1;
}
	
double cBoss::baseAttackPower()
{
	int spellIndex = spellIndex_;
	if(isNoSpell()) spellIndex = 0;
	return sg_pDungeonSystem->DataBase.CharaImportData_Value(ID(),(tstring)_T("攻撃力"),1,spellIndex);
}
double cBoss::baseDefencePower()
{
	int spellIndex = spellIndex_;
	if(isNoSpell()) spellIndex = 0;
	return sg_pDungeonSystem->DataBase.CharaImportData_Value(ID(),(tstring)_T("防御力"),1,spellIndex);
}
double cBoss::baseMHP()
{
	int spellIndex = spellIndex_;
	if(isNoSpell()) spellIndex = 0;
	return sg_pDungeonSystem->DataBase.CharaImportData_Value(ID(),(tstring)_T("MHP"),1,spellIndex);
}
double cBoss::baseLV()
{
	int spellIndex = spellIndex_;
	if(isNoSpell()) spellIndex = 0;
	return sg_pDungeonSystem->DataBase.CharaImportData_Value(ID(),(tstring)_T("出現階"),1,spellIndex);
}
double cBoss::baseEXP()
{
	int spellIndex = spellIndex_;
	if(isNoSpell()) spellIndex = 0;
	return sg_pDungeonSystem->DataBase.CharaImportData_Value(ID(),(tstring)_T("経験値"),0,spellIndex);
}
double cBoss::ValiableConstant1()
{
	int spellIndex = spellIndex_;
	if(isNoSpell()) spellIndex = 0;
	return sg_pDungeonSystem->DataBase.CharaImportData_Value(ID(),(tstring)_T("汎用変数一"),0,spellIndex);
}
double cBoss::ValiableConstant2()
{
	int spellIndex = spellIndex_;
	if(isNoSpell()) spellIndex = 0;
	return sg_pDungeonSystem->DataBase.CharaImportData_Value(ID(),(tstring)_T("汎用変数二"),0,spellIndex);
}
double cBoss::ValiableConstant3()
{
	int spellIndex = spellIndex_;
	if(isNoSpell()) spellIndex = 0;
	return sg_pDungeonSystem->DataBase.CharaImportData_Value(ID(),(tstring)_T("汎用変数三"),0,spellIndex);
}
double cBoss::ValiableConstant4()
{
	int spellIndex = spellIndex_;
	if(isNoSpell()) spellIndex = 0;
	return sg_pDungeonSystem->DataBase.CharaImportData_Value(ID(),(tstring)_T("汎用変数四"),0,spellIndex);
}
//--------------------------------------------------------------------------------------------------------------------------

cBossSpell::cBossSpell(pcScriptRLayer scriptlayer, const tstring& scriptKey, pcCharacter boss, const int spellIndex)
{
	wpBoss_ = wpcCharacter(boss);
	scriptlayer_ = scriptlayer;
	scriptKey_ = scriptKey;
	spellIndex_ = spellIndex;
	spellStartTurn_ = sg_pDungeonSystem->SumTurnCount();
}
cBossSpell::~cBossSpell()
{
	
}

void cBossSpell::makeLuaEnv()
{
	
	pDungeonStoryScript_LuaEnvironment plua(new DungeonStoryScript_LuaEnvironment(tString2luastring((setStyle(boss()->ID()) + _T("spell") + setStyle(spellIndex_)).conclete_tstr())));
	luaEnv_ = wpDungeonStoryScript_LuaEnvironment(plua);
	sg_pDungeonSystem->insertLuaEnv(plua);
}

void cBossSpell::deleteLuaEnv()
{
	//中断すると呼び出されなくてメモリリークする。要改善
	if(luaEnv())
	{
		sg_pDungeonSystem->insertLuaEnv(luaEnv());
	}
}
int cBossSpell::eraseDanmaku(pcCharacter boss)
{
	int count = 0;
	{
		vector<pcCharacter> vpchara = sg_pDungeonSystem->CharaList();
		int i,size = vpchara.size();
		for(i=0;i<size;i++)
		{
			if(vpchara[i]->CharaAttribute().count(キャラ属性::弾幕))
			{
				sg_pDungeonSystem->強制退場要請(vpchara[i]);
				++count;
			}
		}
	}
	{
		vector<pcEvent> vpEvent = sg_pDungeonSystem->EventManager().eventList();
		int i,size = vpEvent.size();
		for(i=0;i<size;i++)
		{
			pcEvent_BossLazer lazer = boost::dynamic_pointer_cast<cEvent_BossLazer>(vpEvent[i]);
			if(lazer)
			{
				lazer->endEvent();
				++count;
			}
		}
	}
	return count;
}	

int cBossSpell::spellTurnCount()
{
	return sg_pDungeonSystem->SumTurnCount() - spellStartTurn_;
}
int cBossSpell::spellRestTurn()
{
	return spellTurnLimitCount() - spellTurnCount();
}
int cBossSpell::spellTurnLimitCount()
{
	return sg_pDungeonSystem->DataBase.CharaImportData_Value(boss()->ID(),(tstring)_T("spellTurnLimitCount"),100,spellIndex_);
}


void cBossSpell::setLuaValiable()
{
	if(luaEnv())
	{
		luaEnv()->setGlobalConstValiable("spellTurnCount",spellTurnCount());
		luaEnv()->setGlobalConstValiable("spellRestTurn",spellRestTurn());
		luaEnv()->setGlobalConstValiable("spellTurnLimitCount",spellTurnLimitCount());
		luaEnv()->setGlobalConstValiable("ValiableConstant1",boss()->ValiableConstant1());
		luaEnv()->setGlobalConstValiable("ValiableConstant2",boss()->ValiableConstant2());
		luaEnv()->setGlobalConstValiable("ValiableConstant3",boss()->ValiableConstant3());
		luaEnv()->setGlobalConstValiable("ValiableConstant4",boss()->ValiableConstant4());
	}
}
void cBossSpell::startSpell()
{
	makeLuaEnv();
	if(luaEnv() && scriptlayer_)
	{
		cValiableField val;
		val.charas.dim(_T("boss")) = boss();
		val.doubles.dim(_T("spellIndex")) = spellIndex_;
		luaEnv()->loadDungeonStoryScript(scriptlayer_, scriptKey_, val);
	}

	setLuaValiable();

	if(luaEnv() && luaEnv()->luaState() != NULL && !luaEnv()->function(_T("startSpell")))
	{
		luaEnv()->outputErrorFile(_T("Error_")+boss()->uniqueName()+_T("Spell")+setStyle(spellIndex_).conclete_tstr()+_T("_startSpell.txt"));
	}
}
void cBossSpell::endSpell()
{
	setLuaValiable();
	if(luaEnv() && luaEnv()->luaState() != NULL && !luaEnv()->function(_T("endSpell")))
	{
		luaEnv()->outputErrorFile(_T("Error_")+boss()->uniqueName()+_T("Spell")+setStyle(spellIndex_).conclete_tstr()+_T("_endSpell.txt"));
	}

	deleteLuaEnv();

}
pcLandform cBossSpell::AIお気に入り地形(cValiableField& valiable)
{
	setLuaValiable();
	pcLandform pland;
	if(luaEnv() && luaEnv()->luaState() != NULL && !luaEnv()->function_returnable(_T("bossMoveTargetLandform"),pland))
	{
		luaEnv()->outputErrorFile(_T("Error_")+boss()->uniqueName()+_T("Spell")+setStyle(spellIndex_).conclete_tstr()+_T("_bossMoveTargetLandform.txt"));
	}

	if( sg_pDungeonSystem->Map().edgeLand() == pland)
	{
		return NULLLAND;
	}
	return pland;
}
int cBossSpell::AI特殊攻撃選択(cValiableField& valiable)
{
	setLuaValiable();
	int result = 0;
	if(luaEnv() && luaEnv()->luaState() != NULL && !luaEnv()->function_returnable(_T("AISkillChoise"),result, boost::ref(valiable)))
	{
		luaEnv()->outputErrorFile(_T("Error_")+boss()->uniqueName()+_T("Spell")+setStyle(spellIndex_).conclete_tstr()+_T("_AISkillChoise.txt"));
	}
	return result;
}
int cBossSpell::特殊攻撃効果(cValiableField& valiable)
{
	setLuaValiable();
	int result = 0;
	if(luaEnv() && luaEnv()->luaState() != NULL && !luaEnv()->function_returnable(_T("skillEffect"),result, boost::ref(valiable)))
	{
		luaEnv()->outputErrorFile(_T("Error_")+boss()->uniqueName()+_T("Spell")+setStyle(spellIndex_).conclete_tstr()+_T("_skillEffect.txt"));
	}
	return result;
}
int cBossSpell::特殊攻撃_アニメ(cValiableField& valiable)
{
	return false;
}
void cBossSpell::パッシブ能力(タイミング timing, cValiableField& valiable)
{
	setLuaValiable();
	if(luaEnv() && luaEnv()->luaState() != NULL && !luaEnv()->function(_T("passiveSkill"),timing,boost::ref(valiable)))
	{
		luaEnv()->outputErrorFile(_T("Error_")+boss()->uniqueName()+_T("Spell")+setStyle(spellIndex_).conclete_tstr()+_T("_passiveSkill.txt"));
	}
}
void cBossSpell::弾幕展開(cValiableField& valiable)
{
	setLuaValiable();
	if(luaEnv() && luaEnv()->luaState() != NULL && !luaEnv()->function(_T("putUpBarrage")))
	{
		luaEnv()->outputErrorFile(_T("Error_")+boss()->uniqueName()+_T("Spell")+setStyle(spellIndex_).conclete_tstr()+_T("_putUpBarrage.txt"));
	}
}

tstring cBossSpell::spellName()
{
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.CharaImportLanguage(boss()->ID(),
		(tstring)_T("SpellName"));
	if(psr == NULL) return _T("No SpellName");
	if(psr->script().size() > spellIndex_)
	{
		return psr->script()[spellIndex_];
	}
	return _T("No SpellName");
}
unsigned long cBossSpell::imageColor1()
{
	return 0xFF888888;
}
unsigned long cBossSpell::imageColor2()
{
	return 0xFF888888;
}