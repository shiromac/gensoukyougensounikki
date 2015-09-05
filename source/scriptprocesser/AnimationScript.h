#pragma once

#include <vector>
#include <map>
#include "../utility/StyleString.h"
#include <LuaUtility/LuaStringURL.h>
#include "../utility/valiableField/cValiableField.h"
#include "fileManage_ph.h"
double random_for_effect();

int AnimationScript_AddAnimation(const pcScriptRLayer player, const tstring& contentName, cValiableField& valiable);

