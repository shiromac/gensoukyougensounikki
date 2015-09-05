#pragma once

#include <vector>
#include <map>
#include "../utility/StyleString.h"
#include <LuaUtility/LuaStringURL.h>
#include "../utility/valiableField/cValiableField.h"
#include "fileManage_ph.h"

int DungeonMakeScript_ExtraSetting(const pcScriptRLayer player, const tstring& contentName, cValiableField& valiable);

class cLandform;
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cLandform> pcLandform;

