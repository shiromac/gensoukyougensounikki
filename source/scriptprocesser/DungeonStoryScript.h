#pragma once

#include <vector>
#include <map>
#include "../utility/StyleString.h"
#include <LuaUtility/LuaStringURL.h>
#include <LuaUtility/LuaEnvironment.h>
#include "../utility/valiableField/cValiableField.h"
#include "fileManage_ph.h"

class DungeonStoryScript_LuaEnvironment :public LuaEnvironment
{
protected:
	//std::map<tstring, pluaString> 
public:
	friend class std::vector<DungeonStoryScript_LuaEnvironment>;
	DungeonStoryScript_LuaEnvironment();
public:
	DungeonStoryScript_LuaEnvironment(::std::basic_string<LuaString> name);
	virtual ~DungeonStoryScript_LuaEnvironment(void);
public:

	virtual void luabind_module();

	void setValiable(cValiableField& valiable);


	int loadDungeonStoryScript(const pcScriptRLayer player, const tstring& contentName, cValiableField& valiable);


};

class DungeonStoryScript_LuaEnvironment;
typedef boost::shared_ptr<DungeonStoryScript_LuaEnvironment> pDungeonStoryScript_LuaEnvironment;
typedef boost::weak_ptr<DungeonStoryScript_LuaEnvironment> wpDungeonStoryScript_LuaEnvironment;

