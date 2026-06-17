#include "DungeonMakeScript.h"




#include "../utility/debug.h"

#include <cstdlib>

#include "cScriptRLayer.h"
	

#include <LuaUtility/LuaEnvironment.h>
#include <LuaUtility/LuaScript.h>
#include <LuaUtility/LuaStringURL.h>

#include "../cDungeonSystem.h"
class DungeonMakeScript_LuaEnvironment;

#include "LuaBindModuleLoading.h"



class DungeonMakeScript_LuaEnvironment :public LuaEnvironment
{
protected:
	//std::map<tstring, pluaString> 
public:
	DungeonMakeScript_LuaEnvironment(::std::basic_string<LuaString> name);
	virtual ~DungeonMakeScript_LuaEnvironment(void);
public:

	virtual void luabind_module();

	void setValiable(cValiableField& valiable);

};
DungeonMakeScript_LuaEnvironment::DungeonMakeScript_LuaEnvironment(::std::basic_string<LuaString> name):LuaEnvironment(name)
{};
DungeonMakeScript_LuaEnvironment::~DungeonMakeScript_LuaEnvironment(void)
{};

void DungeonMakeScript_LuaEnvironment::luabind_module()
{

	LuaBindModule::Character_systemLevel_Loading(*this);
	LuaBindModule::Droping_systemLevel_Loading(*this);
	LuaBindModule::Landform_systemLevel_Loading(*this);
	LuaBindModule::SystemValiable_Loading(*this);

}
void DungeonMakeScript_LuaEnvironment::setValiable(cValiableField& valiable)
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



int DungeonMakeScript_ExtraSetting(const pcScriptRLayer player, const tstring& contentName, cValiableField& valiable)
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

	tstring name(_T("DungeonMakeScript_"));
	name += contentName;
	static DungeonMakeScript_LuaEnvironment luaEnv(tString2luastring(name));
	luaEnv.init();
	luaEnv.setValiable(valiable);

	LuaStringTransformer luastr_contentName(contentName);

	if(luaEnv.runLuaScriptWithBuffer(pluascript->buffer(), pluascript->buffersize(), luastr_contentName.lua_str()))
	{
		tstring filename = _T("Error_LuaScript_DungeonMakeScript_");
		filename += contentName;
		filename += _T(".txt");

		luaEnv.outputErrorFile(filename);
	}

	return true;
}

