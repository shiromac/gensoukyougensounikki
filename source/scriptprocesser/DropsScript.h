#pragma once

#include <vector>
#include <map>
#include "../utility/StyleString.h"
#include <LuaUtility/LuaStringURL.h>

#include "fileManage_ph.h"
int Drop_StringAddStyle(pcScriptRLayer player, StyleString & sstr);

int Drop_StringAddStyle(pcScriptRLayer player, StyleString & sstr, std::map<tstring, StyleString > & valiable );

