#include "DungeonStoryScript.h"

#include "DungeonScriptFunction.h"

#include "../utility/debug.h"

#include <cstdlib>

#include "cScriptRLayer.h"
	
#include <d3dx9math.h>

#include <LuaUtility/LuaEnvironment.h>
#include <LuaUtility/LuaScript.h>
#include <LuaUtility/LuaStringURL.h>



#include "../cDungeonSystem.h"



#include "LuaBindModuleLoading.h"
DungeonStoryScript_LuaEnvironment::DungeonStoryScript_LuaEnvironment():LuaEnvironment("noname")
{};
DungeonStoryScript_LuaEnvironment::DungeonStoryScript_LuaEnvironment(::std::basic_string<LuaString> name):LuaEnvironment(name)
{};
DungeonStoryScript_LuaEnvironment::~DungeonStoryScript_LuaEnvironment(void)
{};

void DungeonStoryScript_LuaEnvironment::luabind_module()
{
	
	LuaBindModule::basicStdModule_Loading(*this);

	LuaBindModule::Character_systemLevel_Loading(*this);
	LuaBindModule::Droping_systemLevel_Loading(*this);
	LuaBindModule::Landform_systemLevel_Loading(*this);
	LuaBindModule::Event_systemLevel_Loading(*this);

	LuaBindModule::StyleString_Loading(*this);
	LuaBindModule::cScriptRLayer_Loading(*this);

	LuaBindModule::SystemValiable_Loading(*this);

	LuaBindModule::TextScript_Loading(*this);

	LuaBindModule::DebugFunction_Loading(*this);

	LuaBindModule::SystemMassage_Loading(*this);
	LuaBindModule::SystemControl_Loading(*this);

	LuaBindModule::AnimationScript_Loading(*this);
	LuaBindModule::EffectScript_Loading(*this);
	LuaBindModule::bossFunction_Loading(*this);
}
void DungeonStoryScript_LuaEnvironment::setValiable(cValiableField& valiable)
{
	
	std::map<tstring, double>& doublemap = valiable.doubles.ValiableStringMap();
	std::map<tstring, double>::iterator double_map_itr = doublemap.begin();
	for(;double_map_itr != doublemap.end();++double_map_itr)
	{
		LuaStringTransformer luastr_name(double_map_itr->first);
		setGlobalConstValiable<double>(luastr_name.lua_str(), double_map_itr->second);
	}
	std::map<tstring, pcCharacter>& charamap = valiable.charas.ValiableStringMap();
	std::map<tstring, pcCharacter>::iterator chara_map_itr = charamap.begin();
	for(;chara_map_itr != charamap.end();++chara_map_itr)
	{
		LuaStringTransformer luastr_name(chara_map_itr->first);
		setGlobalConstValiable<pcCharacter>(luastr_name.lua_str(), chara_map_itr->second);
	}
	
}
int DungeonStoryScript_LuaEnvironment::loadDungeonStoryScript(const pcScriptRLayer player, const tstring& contentName, cValiableField& valiable)
{
	if(player == NULL)
	{
		return false;
	}
	pLuaScript psrc = player->pcontent(contentName);
	if(psrc == NULL)
	{
		return false;
	}
	pLuaScript pluascript = boost::dynamic_pointer_cast<LuaScript>(psrc);
	
	if(pluascript == NULL)
	{
		return false;
	}

	init();
	setValiable(valiable);

	LuaStringTransformer luastr_contentName(contentName);

	if(runLuaScriptWithBuffer(pluascript->buffer(), pluascript->buffersize(), luastr_contentName.lua_str()))
	{
		tstring filename = _T("Error_LuaScript_DungeonStoryScript");
		vector<tstring> contentNameVector;
		split(contentName,_T("/"),contentNameVector);
		int i, size = contentNameVector.size();
		for(i=0;i<size;i++)
		{
			filename += _T("_");
			filename += contentNameVector[i];
		}
		
		filename += _T(".txt");

		outputErrorFile(filename);
	}

	return true;
}
