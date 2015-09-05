#pragma once
#include <lua.hpp>
#include <luabind/luabind.hpp>
#include "LuaStringURL.h"
#include <cScriptRLayer.h>
#include <fileManage_ph.h>
#include <vector>

class LuaScript;
typedef boost::shared_ptr<LuaScript> pLuaScript;
typedef boost::weak_ptr<LuaScript> wpLuaScript;


class LuaScript
{
public:
	LuaScript(void);
public:
	virtual ~LuaScript(void);
protected:
	std::vector<SByte> buffer_;
	bool isFlexible_;
	tstring filename_;
	FileTime lastWriteTime_;

public:
	virtual int savedata(std::vector<SByte>& data);
	virtual int loaddata(const std::vector<SByte>& data);

	virtual void setFlexible(const int flexible);
public:
	//インターフェイス
	const pLuaString buffer();
	const size_t buffersize();

	int loadFile(const tstring& filename);
};

int ConvertT2VecC(const pLuaScript& tmpl, std::vector<SByte>& data);
int DecodeVecC2T(const std::vector<SByte>& data, pLuaScript& pLuaScript);
