#include "cOtherChara.h"
#include "Mob1.h"

#include "gameMainSystem/filemanage/cScriptRLayer.h"
#include "gameMainSystem/filemanage/cScriptReader.h"

#include "cDungeonSystem.h"
#include "GameIdiom.h"

#include "gameMainSystem/cenvironment.h"
cOtherChara::cOtherChara(void)
{
	EXP = 0;
	LV = 1;

	MaxStomach = 100;

	SetMHP();
}

cOtherChara::~cOtherChara(void)
{
}


bool cOtherChara::すれ違い許可(pcCharacter pchara)
{
	return (!sg_pDungeonSystem->キャラクター敵対判定(me(),pchara)
		&& g_pPlayerInput()->dash().on);
}

int cOtherChara::canTalk()
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

int cOtherChara::TalkEvent()
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
	if(size == 0) {
		GameIdiom::キャラの方を向く(me(), sg_pDungeonSystem->pPlayerChara());
	}
	return result;
}

//----------------------------------------------------------------------------------

cOtherChara_Event::cOtherChara_Event(int ID)
{
	setID(ID);
}
cOtherChara_Event::~cOtherChara_Event()
{

}
tstring cOtherChara_Event::surfaceName()
{
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.CharaImportData(simulationID(),
		(tstring)_T("surfaceName"));
	if(psr == NULL) return _T("No surfaceName");
	return psr->topscript();
}

	//唯一名
tstring cOtherChara_Event::uniqueName()
{
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.CharaImportData(simulationID(),
		(tstring)_T("surfaceName"));
	if(psr == NULL) return _T("No surfaceName");
	return psr->topscript() + (tstring)_T("_Event");
}

int cOtherChara_Event::GetTex_aspect_type()
{
	pcScriptRLayer psr;
	pcCharacter pchara = sg_pDungeonSystem->DataBase.GetSampleCharacter(simulationID());
	if(pchara == NULL) return CHARACTER_TEXASPECT_SINPLE;
	return pchara->GetTex_aspect_type();
}

void cOtherChara_Event::setID(int ID)
{
	_realID = ID;
	_simulationID = ID - CCHARACTER_cOtherChara_Event_BASE_ID_NUM + CCHARACTER_cMob_BASE_ID_NUM;
}