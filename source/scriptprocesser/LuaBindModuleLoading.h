#pragma once
#include <lua.hpp>
#include <luabind/luabind.hpp>
//#include <luabind/return_reference_to_policy.hpp>
#include "..\cEquipment.h"
#include "DungeonScriptFunction.h"
#include "..\utility\valiableField\cValiableField.h"
class LuaEnvironment;
namespace LuaBindModule
{
void basicStdModule_Loading(LuaEnvironment& luaEnvironment);

void Character_systemLevel_Loading(LuaEnvironment& luaEnvironment);
void Character_private_Loading(LuaEnvironment& luaEnvironment);

void Droping_systemLevel_Loading(LuaEnvironment& luaEnvironment);
void Landform_systemLevel_Loading(LuaEnvironment& luaEnvironment);
void Event_systemLevel_Loading(LuaEnvironment& luaEnvironment);

void StyleString_Loading(LuaEnvironment& luaEnvironment);

void cScriptRLayer_Loading(LuaEnvironment& luaEnvironment);

void SystemValiable_Loading(LuaEnvironment& luaEnvironment);

//テキストスクリプト
void TextScript_Loading(LuaEnvironment& luaEnvironment);
//メッセージ
void SystemMassage_Loading(LuaEnvironment& luaEnvironment);
//ウィンドウインターフェイス
void SystemControl_Loading(LuaEnvironment& luaEnvironment);

void DebugFunction_Loading(LuaEnvironment& luaEnvironment);

void bossFunction_Loading(LuaEnvironment& luaEnvironment);

void AnimationScript_Loading(LuaEnvironment& luaEnvironment);

void EffectScript_Loading(LuaEnvironment& luaEnvironment);

//テキスト用パブリックな関数のみ
void ItemExplanationScript_Loading(LuaEnvironment& luaEnvironment);

template<typename T> void vector_Loading(LuaEnvironment& luaEnvironment, const char* templateName)
{
	string templateNameStr("vector_");
	templateNameStr += templateName;
	string templateNameIteratorStr(templateNameStr);
	templateNameIteratorStr += "_iterator";

	//string templateRefStr("ref_");
	//templateRefStr += templateName;

	luabind::module(luaEnvironment.luaState())
	[
	
		luabind::class_<forLua::std_vector<T>::LuaVector>(templateNameStr.c_str())
			.def(luabind::constructor<>())
			.def(luabind::constructor<forLua::std_vector<T>::LuaVector>())
			.def(luabind::constructor<forLua::std_vector<T>::LuaVector::size_type>())
			.def(luabind::constructor<forLua::std_vector<T>::LuaVector::size_type, T>())
			.def("assign", &forLua::std_vector<T>::assign)
			.def("at", &forLua::std_vector<T>::at)
			.def("setAt", &forLua::std_vector<T>::setAt)
			.def("back", &forLua::std_vector<T>::back)
			.def("setBack", &forLua::std_vector<T>::setBack)
			.def("front", &forLua::std_vector<T>::front)
			.def("setFront", &forLua::std_vector<T>::setFront)
			.def("begin", &forLua::std_vector<T>::begin)
			//.def("capacity", &std::vector<T>::capacity)
			.def("clear", &forLua::std_vector<T>::clear)
			.def("empty", &forLua::std_vector<T>::empty)
			.def("end", &forLua::std_vector<T>::end)
			.def("erase", &forLua::std_vector<T>::erase1)
			.def("erase", &forLua::std_vector<T>::erase2)
			.def("insert", &forLua::std_vector<T>::insert1)
			//.def("insert", &forLua::std_vector<T>::insert2)
			.def("insert", &forLua::std_vector<T>::insert3)
			//.def("max_size", &forLua::std_vector<T>::max_size)
			.def("pop_back", &forLua::std_vector<T>::pop_back)
			.def("push_back", &forLua::std_vector<T>::push_back)
			//.def("rbegin", &forLua::std_vector::rbegin<T>)
			//.def("rend", &forLua::std_vector::rend<T>)
			//.def("reserve", &forLua::std_vector<T>::reserve)
			.def("resize", &forLua::std_vector<T>::resize1)
			.def("resize", &forLua::std_vector<T>::resize2)
			.def("size", &forLua::std_vector<T>::size)
			//.def("swap", &forLua::std_vector<T>::swap)
			.def(luabind::self == forLua::std_vector<T>::LuaVector())
			,

		luabind::class_<forLua::std_vector<T>::LuaVector_iterator>(templateNameIteratorStr.c_str())
			.def(luabind::constructor<>())
			.def(luabind::constructor<forLua::std_vector<T>::LuaVector_iterator>())
			.def("increment", &forLua::std_vector<T>::iterator_increment)
			.def("decrement", &forLua::std_vector<T>::iterator_decrement)
			.def("dereference", &forLua::std_vector<T>::iterator_dereference)
			.def("setDereference", &forLua::std_vector<T>::iterator_setDereference)
			.def(luabind::self == forLua::std_vector<T>::LuaVector_iterator())
	];
}

template<typename T, typename U> void map_Loading(LuaEnvironment& luaEnvironment, const char* templateName_Key, const char* templateName_Value)
{
	string LuaMap_iterator_name("map_");
	LuaMap_iterator_name += templateName_Key;
	LuaMap_iterator_name += "_";
	LuaMap_iterator_name += templateName_Value;
	LuaMap_iterator_name += "_iterator";

	string LuaMap_pair_name("pair_");
	LuaMap_pair_name += templateName_Key;
	LuaMap_pair_name += "_";
	LuaMap_pair_name += templateName_Value;

	string pairOf_LuaMap_pair_bool_name("pair_pair_");
	pairOf_LuaMap_pair_bool_name += templateName_Key;
	pairOf_LuaMap_pair_bool_name += "_";
	pairOf_LuaMap_pair_bool_name += templateName_Value;
	pairOf_LuaMap_pair_bool_name += "_bool";

	string LuaMap_name("map_");
	LuaMap_name += templateName_Key;
	LuaMap_name += "_";
	LuaMap_name += templateName_Value;

	luabind::module(luaEnvironment.luaState())
	[

		luabind::class_<forLua::std_map<T,U>::LuaMap_iterator>(LuaMap_iterator_name.c_str())
			.def(luabind::constructor<>())
			.def("increment", &forLua::std_map<T,U>::iterator_increment)
			.def("decrement", &forLua::std_map<T,U>::iterator_decrement)
			.def("dereference", &forLua::std_map<T,U>::iterator_dereference)
			.def(luabind::self == forLua::std_map<T,U>::LuaMap_iterator())
			,
		luabind::class_<forLua::std_map<T,U>::LuaMap_pair>(LuaMap_pair_name.c_str())
			.def(luabind::constructor<>())
			.def(luabind::constructor<T,U>())
			.def_readwrite("first", &forLua::std_map<T,U>::LuaMap_pair::first)
			.def_readwrite("second", &forLua::std_map<T,U>::LuaMap_pair::second)
			.def(luabind::self == forLua::std_map<T,U>::LuaMap_pair())
			,
			
		luabind::class_<forLua::std_map<T,U>::pairOf_LuaMap_pair_bool>(pairOf_LuaMap_pair_bool_name.c_str())
			.def(luabind::constructor<>())
			.def(luabind::constructor<forLua::std_map<T,U>::LuaMap_iterator,bool>())
			.def_readwrite("first", &forLua::std_map<T,U>::pairOf_LuaMap_pair_bool::first)
			.def_readwrite("second", &forLua::std_map<T,U>::pairOf_LuaMap_pair_bool::second)
			.def(luabind::self == forLua::std_map<T,U>::pairOf_LuaMap_pair_bool())
			,
			
		luabind::class_<forLua::std_map<T,U>::LuaMap>(LuaMap_name.c_str())
			.def(luabind::constructor<>())
			.def(luabind::constructor<forLua::std_map<T,U>::LuaMap>())
			.def("begin", &forLua::std_map<T,U>::begin)
			.def("clear", &forLua::std_map<T,U>::clear)
			.def("count", &forLua::std_map<T,U>::count)
			.def("empty", &forLua::std_map<T,U>::empty)
			.def("end", &forLua::std_map<T,U>::end)
			.def("value", &forLua::std_map<T,U>::value)
			.def("setValue", &forLua::std_map<T,U>::setValue)
			.def("erase", &forLua::std_map<T,U>::erase1)
			.def("erase", &forLua::std_map<T,U>::erase2)
			.def("find", &forLua::std_map<T,U>::find)
			.def("insert", &forLua::std_map<T,U>::insert1)
			.def("insert", &forLua::std_map<T,U>::insert2)
			.def("lower_bound", &forLua::std_map<T,U>::lower_bound)
			.def("size", &forLua::std_map<T,U>::size)
			.def("upper_bound", &forLua::std_map<T,U>::upper_bound)
			.def(luabind::self == forLua::std_map<T,U>::LuaMap())
	];
}

template<typename T> void set_Loading(LuaEnvironment& luaEnvironment, const char* templateName_Key)
{
	string Luaset_iterator_name("set_");
	Luaset_iterator_name += templateName_Key;
	Luaset_iterator_name += "_iterator";


	string pairOf_Luaset_iterator_bool_name("pair_");
	pairOf_Luaset_iterator_bool_name += Luaset_iterator_name;
	pairOf_Luaset_iterator_bool_name += "_";
	pairOf_Luaset_iterator_bool_name += templateName_Key;
	pairOf_Luaset_iterator_bool_name += "_bool";

	string Luaset_name("set_");
	Luaset_name += templateName_Key;

	luabind::module(luaEnvironment.luaState())
	[

		luabind::class_<forLua::std_set<T>::LuaSet_iterator>(Luaset_iterator_name.c_str())
			.def(luabind::constructor<>())
			.def("increment", &forLua::std_set<T>::iterator_increment)
			.def("decrement", &forLua::std_set<T>::iterator_decrement)
			.def("dereference", &forLua::std_set<T>::iterator_dereference)
			//.def("setDereference", &forLua::std_set<T>::iterator_setDereference)
			.def(luabind::self == forLua::std_set<T>::LuaSet_iterator())
			,

		luabind::class_<forLua::std_set<T>::pairOf_LuaSet_iterator_bool>(pairOf_Luaset_iterator_bool_name.c_str())
			.def(luabind::constructor<>())
			.def(luabind::constructor<forLua::std_set<T>::LuaSet_iterator,bool>())
			.def_readwrite("first", &forLua::std_set<T>::pairOf_LuaSet_iterator_bool::first)
			.def_readwrite("second", &forLua::std_set<T>::pairOf_LuaSet_iterator_bool::second)
			.def(luabind::self == forLua::std_set<T>::pairOf_LuaSet_iterator_bool())
			,
			
		luabind::class_<forLua::std_set<T>::LuaSet>(Luaset_name.c_str())
			.def(luabind::constructor<>())
			.def(luabind::constructor<forLua::std_set<T>::LuaSet>())
			.def("begin", &forLua::std_set<T>::begin)
			.def("clear", &forLua::std_set<T>::clear)
			.def("count", &forLua::std_set<T>::count)
			.def("empty", &forLua::std_set<T>::empty)
			.def("end", &forLua::std_set<T>::end)
			.def("erase", &forLua::std_set<T>::erase2)
			.def("erase", &forLua::std_set<T>::erase3)
			.def("find", &forLua::std_set<T>::find)
			.def("insert", &forLua::std_set<T>::insert2)
			.def("insert", &forLua::std_set<T>::insert3)
			.def("size", &forLua::std_set<T>::size)
			.def(luabind::self == forLua::std_set<T>::LuaSet())
	];
}

template<typename T> void cValiableFieldChip_Loading(LuaEnvironment& luaEnvironment, const char* templateName_Key)
{
	string Lua_ValiableFieldChip_name("ValiableFieldChip_");
	Lua_ValiableFieldChip_name += templateName_Key;


	luabind::module(luaEnvironment.luaState())
	[

		luabind::class_<cValiableFieldChip<T>>(Lua_ValiableFieldChip_name.c_str())
			.def(luabind::constructor<>())
			.def(luabind::constructor<cValiableFieldChip<T>&>())	
			.def("clear", &cValiableFieldChip<T>::clear)
			.def("exist", (bool(cValiableFieldChip<T>::*)(変数インデックス))&cValiableFieldChip<T>::exist)
			.def("setVal", &forLua::cValiableFieldChip_function<T>::setVal)
			.def("val", &forLua::cValiableFieldChip_function<T>::val)
			.def("dim", &forLua::cValiableFieldChip_function<T>::dim)

			.def("exist_str", (bool(cValiableFieldChip<T>::*)(const pLuaString))&cValiableFieldChip<T>::exist)
			.def("setVal_str", &forLua::cValiableFieldChip_function<T>::setVal_str)
			.def("val_str", &forLua::cValiableFieldChip_function<T>::val_str)
			.def("dim_str", &forLua::cValiableFieldChip_function<T>::dim_str)
	];
}

};//namespace