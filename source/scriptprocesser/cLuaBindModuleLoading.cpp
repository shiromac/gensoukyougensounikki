#include "LuaBindModuleLoading.h"

#include <LuaUtility/LuaEnvironment.h>
#include <LuaUtility/LuaScript.h>
#include <LuaUtility/LuaStringURL.h>
#include <LuaUtility/LuaStringUtility.h>
#include <../utility/StyleString.h>

#include "../cDungeonSystem.h"

#include "DungeonScriptFunction.h"

#include "DropsScript.h"

#include "LuaBindModuleLoading.h"
#include "../EffectFunctions.h"

#include <boost/ref.hpp>

#include "../cObjectChara.h"
#include "../cEvent.h"

#include "../cLuaFunctionCommand.h"
#include "../cEventBackground.h"

namespace LuaBindModule
{

void basicStdModule_Loading(LuaEnvironment& luaEnvironment)
{
	set_Loading<int>(luaEnvironment,"set_int");

	vector_Loading<int>(luaEnvironment,"int");
	vector_Loading<pcCharacter>(luaEnvironment,"cCharacter");
	vector_Loading<pcDroping>(luaEnvironment,"cDroping");
	vector_Loading<pcLandform>(luaEnvironment,"cLandform");
	vector_Loading<tstring>(luaEnvironment,"tstring");
	vector_Loading<StyleString>(luaEnvironment,"StyleString");

	map_Loading<tstring,StyleString>(luaEnvironment,"tstring","StyleString");
	map_Loading<tstring,int>(luaEnvironment,"tstring","int");
	map_Loading<tstring,::luabind::object>(luaEnvironment,"tstring","object");
	
}

void Character_private_Loading(LuaEnvironment& luaEnvironment)
{
	luabind::module(luaEnvironment.luaState())
	[
		luabind::class_<cCharacter, boost::shared_ptr<cCharacter>>("cCharacter")
	];
}
namespace cCharacter_forLua{
	const int placeX_forLua(pcCharacter& pchara){return pchara->placeX - EDGELANDWIDTH;};
	const int placeY_forLua(pcCharacter& pchara){return pchara->placeY - EDGELANDWIDTH;};
	set<int> CharaAttribute_forLua(pcCharacter& pchara)
	{
		set<int> attri;
		set<キャラ属性::キャラ属性リスト> attriBefore = pchara->CharaAttribute();
		set<キャラ属性::キャラ属性リスト>::iterator itr = attriBefore.begin();
		set<キャラ属性::キャラ属性リスト>::iterator itrEnd = attriBefore.end();
		for(;itrEnd != itr; itr++)
		{
			attri.insert((int)*itr);
		}
		return attri;
	};
};
void Character_systemLevel_Loading(LuaEnvironment& luaEnvironment)
{

	luabind::module(luaEnvironment.luaState())
	[
		luabind::class_<cCharacter, boost::shared_ptr<cCharacter>>("cCharacter")
		.def(luabind::constructor<>())
			.def(luabind::constructor<cCharacter&>())	
			//.def("placeX", &cCharacter_forLua::placeX_forLua)
			//.def("placeY", &cCharacter_forLua::placeY_forLua)
			.property("placeX", &cCharacter_forLua::placeX_forLua)
			.property("placeY", &cCharacter_forLua::placeY_forLua)
			.def_readonly("aspect", &cCharacter::aspect)
			.def("fullName", &cCharacter::FullName)
			.def("shortName", &cCharacter::ShortName)
			.def("waterWalk", &cCharacter::水上歩行)
			.def("wallWalk", &cCharacter::壁中歩行)
			.def("airWalk", &cCharacter::空中歩行)
			.def_readonly("forse", &cCharacter::Forse)
			.def_readonly("attackEquipment", &cCharacter::attackequipment)
			.def_readonly("defenseEquipment", &cCharacter::defenseequipment)
			.def_readonly("bulletEquipment", &cCharacter::bulletequipment)
			.def("canTalk", &cCharacter::canTalk)
			.def("maxHoldNum", &cCharacter::MaxholdNum)
			.def_readonly("holdItem", &cCharacter::holdItem)
			.def_readonly("forse", &cCharacter::Forse)
			.def_readonly("HP", &cCharacter::HP)
			.def_readonly("maxHP", &cCharacter::MHP)
			.def_readonly("EXP", &cCharacter::EXP)
			.def_readonly("stomach", &cCharacter::Stomach)
			.def_readonly("maxStomach", &cCharacter::MaxStomach)
			.def_readonly("CLASS", &cCharacter::CLASS)

			.def("attackPower", &cCharacter::GetAttackPower)
			.def("defencePower", &cCharacter::GetDefencePower)
			.def("bulletAttackPower", &cCharacter::GetBulletAttackPower)
			.def("isDead", &cCharacter::死亡)
			.def("ID", &cCharacter::ID)
			.def("characterAttribute", &cCharacter_forLua::CharaAttribute_forLua)

			//.def("isNonNull", &is_non_null<cCharacter>)
			.def("isEqualTo", &forLua::is_equal<cCharacter>)
			.def("setStopStamp", &cCharacter::setStopStamp)
		,
		
		#define CHARACTERLUABINDDEF(FunctionNamePrefix) \
		luabind::def(#FunctionNamePrefix, &forLua::FunctionNamePrefix)
		

		CHARACTERLUABINDDEF(MakeCharacter),
		CHARACTERLUABINDDEF(getSumpleCharacter),
		CHARACTERLUABINDDEF(changePlayerCharacter),
		CHARACTERLUABINDDEF(swapPlayerCharacter),

		CHARACTERLUABINDDEF(CharacterHPRecovery),
		CHARACTERLUABINDDEF(CharacterSetHP),
		CHARACTERLUABINDDEF(CharacterMaxHPExtend),
		CHARACTERLUABINDDEF(CharacterMaxHPReduction),

		CHARACTERLUABINDDEF(CharacterStomachRecovery),
		CHARACTERLUABINDDEF(CharacterStomachHungry),
		CHARACTERLUABINDDEF(CharacterSetStomach),
		CHARACTERLUABINDDEF(CharacterMaxStomachExtend),
		CHARACTERLUABINDDEF(CharacterMaxStomachReduction),

		CHARACTERLUABINDDEF(CharacterSpeedUp),
		CHARACTERLUABINDDEF(CharacterSpeedDown),

		CHARACTERLUABINDDEF(CharacterSleep),
		CHARACTERLUABINDDEF(CharacterDeepSleep),
		CHARACTERLUABINDDEF(CharacterDeepDoze),
		CHARACTERLUABINDDEF(CharacterLightDoze),
		CHARACTERLUABINDDEF(CharacterSurprise),
		CHARACTERLUABINDDEF(CharacterLockedIn),

		CHARACTERLUABINDDEF(CharacterSeal),
		CHARACTERLUABINDDEF(CharacterSwingAir),
		CHARACTERLUABINDDEF(CharacterPoor),
		CHARACTERLUABINDDEF(CharacterCowardice),
		CHARACTERLUABINDDEF(CharacterJealousy),
		CHARACTERLUABINDDEF(CharacterFrenzy),

		CHARACTERLUABINDDEF(CharacterAttackDown),
		CHARACTERLUABINDDEF(CharacterAttackUp),

		CHARACTERLUABINDDEF(CharacterDefenceDown),
		CHARACTERLUABINDDEF(CharacterDefenceUp),

		CHARACTERLUABINDDEF(CharacterDrunk),
		CHARACTERLUABINDDEF(CharacterInIce),
		CHARACTERLUABINDDEF(CharacterMatchless),
		CHARACTERLUABINDDEF(CharacterNightBlind),

		CHARACTERLUABINDDEF(CharacterDeathTouch),
		CHARACTERLUABINDDEF(CharacterSubstitution),
		CHARACTERLUABINDDEF(CharacterMimicry),

		CHARACTERLUABINDDEF(CharacterCureMental),
		CHARACTERLUABINDDEF(CharacterCurePhysical),
		CHARACTERLUABINDDEF(CharacterCureMagic),
		CHARACTERLUABINDDEF(CharacterCureSpeed),

		CHARACTERLUABINDDEF(CharacterIsMentalAberration),
		CHARACTERLUABINDDEF(CharacterIsPhysicalAberration),
		CHARACTERLUABINDDEF(CharacterIsMagicAberration),
		CHARACTERLUABINDDEF(CharacterIsSpeedAberration),

		CHARACTERLUABINDDEF(CharacterGoPast),

		CHARACTERLUABINDDEF(CharacterEyeLotion),

		CHARACTERLUABINDDEF(CharacterForsedDamage),

		CHARACTERLUABINDDEF(CharacterInstantDeath),
		CHARACTERLUABINDDEF(CharacterForseGoAway),

		CHARACTERLUABINDDEF(CharacterGetExperience),
		CHARACTERLUABINDDEF(CharacterLevelUp),
		CHARACTERLUABINDDEF(CharacterLevelDown),

		CHARACTERLUABINDDEF(CharacterForsedFlinch),
		CHARACTERLUABINDDEF(CharacterForsedTurnEnd),

		CHARACTERLUABINDDEF(CharacterNormalAttack),
		CHARACTERLUABINDDEF(CharacterForceEquipmentSwap),

		CHARACTERLUABINDDEF(CharacterTurn),
		CHARACTERLUABINDDEF(CharacterTurnNow),

		CHARACTERLUABINDDEF(CharacterMove),
		CHARACTERLUABINDDEF(CharacterCanMove),

		CHARACTERLUABINDDEF(CharacterVisualRefresh),

		CHARACTERLUABINDDEF(CharacterWarp),
		CHARACTERLUABINDDEF(CharacterCanWarp),

		CHARACTERLUABINDDEF(CharacterBlow),
		CHARACTERLUABINDDEF(CharacterPull),
		CHARACTERLUABINDDEF(CharacterReplace),
		CHARACTERLUABINDDEF(CharacterThrow),
		
		CHARACTERLUABINDDEF(CharacterDisguise),

		CHARACTERLUABINDDEF(PlayerCharacter),
		CHARACTERLUABINDDEF(turnEndPlayerCharacter),

		CHARACTERLUABINDDEF(itemCount),


		luabind::def("isEqualcCharacter", &forLua::is_equal<cCharacter>),

		luabind::class_<キャラ属性::キャラ属性リスト>("CharacterAttribute")
			.enum_("constants")
			[
				luabind::value("human",キャラ属性::人間),
				luabind::value("youkai",キャラ属性::妖怪),
				luabind::value("ghost",キャラ属性::幽霊),
				luabind::value("god",キャラ属性::神),
				luabind::value("oni",キャラ属性::鬼),
				luabind::value("fairy",キャラ属性::妖精),
				luabind::value("tengu",キャラ属性::天狗),
				luabind::value("bird",キャラ属性::鳥),
				luabind::value("beast",キャラ属性::獣),
				luabind::value("kedama",キャラ属性::毛玉),
				luabind::value("barrage",キャラ属性::弾幕),
				luabind::value("boss",キャラ属性::ボス)
				/*
				無効エラー,
				人間,
				妖怪,
				幽霊,
				神,
				鬼,
				妖精,
				天狗,
				閻魔,
				鳥,
				獣,
				一文字,
				虫,
				ザコ,
				毛玉,
				弾幕,

				紅魔館,
				永遠亭,
				八雲一家,

				吸血鬼,
				殺人鬼,
				門番,
				魔法使い,
				病弱,
				カリスマ,

				泥棒,
				ボス,
				*/
			]
		
	];

	luaEnvironment.setGlobalConstValiable("CHARACTER_FORSE_FRIEND",CHARACTER_FORSE_FRIEND);
	luaEnvironment.setGlobalConstValiable("CHARACTER_FORSE_ENEMY",CHARACTER_FORSE_ENEMY);
	luaEnvironment.setGlobalConstValiable("CHARACTER_FORSE_OTHER",CHARACTER_FORSE_OTHER);

	//luaEnvironment.setGlobalConstValiable("NULLcCharacter",NULLCHARA);

	luaEnvironment.setGlobalConstValiable("ASPECT_UP",ASPECT_UP);
	luaEnvironment.setGlobalConstValiable("ASPECT_UPRIGHT",ASPECT_UPRIGHT);
	luaEnvironment.setGlobalConstValiable("ASPECT_RIGHT",ASPECT_RIGHT);
	luaEnvironment.setGlobalConstValiable("ASPECT_DOWNRIGHT",ASPECT_DOWNRIGHT);
	luaEnvironment.setGlobalConstValiable("ASPECT_DOWN",ASPECT_DOWN);
	luaEnvironment.setGlobalConstValiable("ASPECT_DOWNLEFT",ASPECT_DOWNLEFT);
	luaEnvironment.setGlobalConstValiable("ASPECT_LEFT",ASPECT_LEFT);
	luaEnvironment.setGlobalConstValiable("ASPECT_UPLEFT",ASPECT_UPLEFT);

}


void Droping_systemLevel_Loading(LuaEnvironment& luaEnvironment)
{

	luabind::module(luaEnvironment.luaState())
	[

		luabind::class_<cDroping, boost::shared_ptr<cDroping>>("cDroping")
			.def(luabind::constructor<>())	
			.def(luabind::constructor<cDroping&>())	

			.def("fullName", &cDroping::FullName)
			.def("shortName", &cDroping::ShortName)

			.def("shortExplanation", &cDroping::shortExplanation)
			.def("longExplanation", &cDroping::longExplanation)

			.def("isCompleteAppreciated", &cDroping::完全識別済み)
			.def("isStateAppreciated", &cDroping::状態値識別済み)
			.def("isQualityAppreciated", &cDroping::修正値識別済み)
			.def("isNameAppreciated", &cDroping::名称識別済み)

			.def("holder", &cDroping::Holder)
			.def("parent", &cDroping::Parent)
			.def_readonly("includedItem", &cDroping::includedItem)

			.def("mitamaCapacity", &cDroping::スロット容量)
			.def("originalRemeinMitamaCapacity", &cDroping::初期残りスロット)
			.def("remainMitamaCapacity", &cDroping::残りスロット)
			.def("yoroshiro", &cDroping::拠り代)
			.def("isMitama", &cDroping::御霊状態)
			.def("mitamaAbilityFullName", &cDroping::能力名Full)
			.def("mitamaAbilityFullShortenName", &cDroping::能力略名Full)
			.def("mitamaAbilityExplanation", &cDroping::能力説明)
			.def("mitamaAbilityExplanationCaption", &cDroping::能力説明Caption)
			.def("mitamaAbilityConstancy", &cDroping::恒常能力)

			.def("equiperForAttack", &cDroping::装備者_攻撃用)
			.def("equiperForDefense", &cDroping::装備者_防御用)
			.def("isEquipedForAttack", &cDroping::装備されている_攻撃用)
			.def("isEquipedForDefense", &cDroping::装備されている_防御用)

			.def("isDropedOnGround", &cDroping::落ちてる)
			.def("isInStore", &cDroping::倉庫の中)
			.def("isBroken", &cDroping::isBroken)
		
			.def("discover", &cDroping::discover)
			.def("disappear", &cDroping::disappear)
			
			.def("isDropedOnGround", &cDroping::落ちてる)
			.def("isInStore", &cDroping::倉庫の中)
			.def("ID", &cDroping::ID)

			//.def("isNonNull", &is_non_null<cDroping>)
			.def_readonly("attribute", &cDroping::属性)
		,

		#define DROPINGLUABINDDEF(FunctionNamePrefix) \
		luabind::def(#FunctionNamePrefix, &forLua::FunctionNamePrefix)
		

		DROPINGLUABINDDEF(MakeDroping),


		DROPINGLUABINDDEF(DropingDiscriminateComplete),
		DROPINGLUABINDDEF(DropingDiscriminateState),
		DROPINGLUABINDDEF(DropingDiscriminateQuality),
		DROPINGLUABINDDEF(DropingDiscriminateName),
		DROPINGLUABINDDEF(DropingStateMakeGood),
		DROPINGLUABINDDEF(DropingStateMakeBad),
		DROPINGLUABINDDEF(DropingStateForsedMakeGood),
		DROPINGLUABINDDEF(DropingStateForsedMakeNormal),
		DROPINGLUABINDDEF(DropingStateForsedMakeBad),

		DROPINGLUABINDDEF(DropingQualityMakeGood),
		DROPINGLUABINDDEF(DropingQualityMakeBad),
		DROPINGLUABINDDEF(DropingQualityForsedMakeValue),
		DROPINGLUABINDDEF(DropingBreak)

	];
	luaEnvironment.setGlobalConstValiable("DROPING_BREAKFLAG_NORMAL",cDroping::BREAKFLAG_NORMAL);
	luaEnvironment.setGlobalConstValiable("DROPING_BREAKFLAG_LOSS",cDroping::BREAKFLAG_LOSS);

	//luaEnvironment.setGlobalConstValiable("NULLcDroping",NULLDROP);

	luaEnvironment.setGlobalConstValiable("DROPING_ATTRIBUTE_INVALID",落ち物属性::無効エラー);
	luaEnvironment.setGlobalConstValiable("DROPING_ATTRIBUTE_OBJECT",落ち物属性::物体);
	luaEnvironment.setGlobalConstValiable("DROPING_ATTRIBUTE_ITEM",落ち物属性::アイテム);
	luaEnvironment.setGlobalConstValiable("DROPING_ATTRIBUTE_DRINK",落ち物属性::飲み物);
	luaEnvironment.setGlobalConstValiable("DROPING_ATTRIBUTE_FOOD",落ち物属性::食べ物);
	luaEnvironment.setGlobalConstValiable("DROPING_ATTRIBUTE_EQUIPMENT",落ち物属性::装備品);
	luaEnvironment.setGlobalConstValiable("DROPING_ATTRIBUTE_BULLET",落ち物属性::撃ち物);
	luaEnvironment.setGlobalConstValiable("DROPING_ATTRIBUTE_BOOK",落ち物属性::読み物);
	luaEnvironment.setGlobalConstValiable("DROPING_ATTRIBUTE_SPELL",落ち物属性::スペカ);
	luaEnvironment.setGlobalConstValiable("DROPING_ATTRIBUTE_BOX",落ち物属性::入れ物);
	luaEnvironment.setGlobalConstValiable("DROPING_ATTRIBUTE_MONEY",落ち物属性::お金);
	luaEnvironment.setGlobalConstValiable("DROPING_ATTRIBUTE_STAIR",落ち物属性::階段);
	luaEnvironment.setGlobalConstValiable("DROPING_ATTRIBUTE_FIRE",落ち物属性::火柱);
	luaEnvironment.setGlobalConstValiable("DROPING_ATTRIBUTE_FOOD",落ち物属性::食べ物);
	luaEnvironment.setGlobalConstValiable("DROPING_ATTRIBUTE_TRAP",落ち物属性::罠);

	luaEnvironment.setGlobalConstValiable("DROPING_ATTRIBUTE_FRAGILE",落ち物属性::割れ物);
	luaEnvironment.setGlobalConstValiable("DROPING_ATTRIBUTE_RESIST_FIRE",落ち物属性::不燃性);
	luaEnvironment.setGlobalConstValiable("DROPING_ATTRIBUTE_RESIST_FREEZE",落ち物属性::耐冷凍);
	luaEnvironment.setGlobalConstValiable("DROPING_ATTRIBUTE_RESIST_ELECTRO",落ち物属性::耐静電気);
	luaEnvironment.setGlobalConstValiable("DROPING_ATTRIBUTE_RESIST_WET",落ち物属性::耐濡れ);
	luaEnvironment.setGlobalConstValiable("DROPING_ATTRIBUTE_ALCOHOL",落ち物属性::酒);
	luaEnvironment.setGlobalConstValiable("DROPING_ATTRIBUTE_MEDICINE",落ち物属性::薬);
	luaEnvironment.setGlobalConstValiable("DROPING_ATTRIBUTE_REMOVE_FIRE",落ち物属性::火消し);
	luaEnvironment.setGlobalConstValiable("DROPING_ATTRIBUTE_SWORD",落ち物属性::剣);
	
	luaEnvironment.setGlobalConstValiable("EQUIP_PART_ATTACK",EQUIP_PART_ATTACK);
	luaEnvironment.setGlobalConstValiable("EQUIP_PART_DEFENSE",EQUIP_PART_DEFENSE);
}
namespace cLandform_forLua{
	const cCoordinate place_forLua(pcLandform& pland){
		cCoordinate coo(pland->place.x - EDGELANDWIDTH, pland->place.y - EDGELANDWIDTH);
		return coo;
	};
};
void Landform_systemLevel_Loading(LuaEnvironment& luaEnvironment)
{

	luabind::module(luaEnvironment.luaState())
	[

		luabind::class_<cCoordinate>("Coordinate")
			.def(luabind::constructor<cCoordinate&>())
			
			.def(luabind::constructor<int,int>())
			
			.def_readwrite("x", &cCoordinate::x)
			.def_readwrite("y", &cCoordinate::y)

			.def("getAspect", &cCoordinate::GetAspect)
			.def("setAspect", &cCoordinate::SetAspect)
			.def("dif", &cCoordinate::dif)
			.def("calcuDegree", &cCoordinate::calcuDegree)
			
			.def(luabind::self * int())
			.def(luabind::self + cCoordinate())
			.def(luabind::self - cCoordinate())
			.def(luabind::self == cCoordinate())
			
		,
		luabind::class_<cLandform, boost::shared_ptr<cLandform>>("cLandform")
			.def(luabind::constructor<>())
			.def(luabind::constructor<cLandform&>())	

			.def_readonly("RoomIndex", &cLandform::RoomIndex)
			.def_readonly("mountedDroping", &cLandform::pOnDrop)
			.def_readonly("mountedCharacter", &cLandform::pOnChar)
			//.def("place", &cLandform_forLua::place_forLua)
			.property("place", &cLandform_forLua::place_forLua)
		
			.def("attribute", &cLandform::getAttribute)	
			.def("breakable", &cLandform::breakable)
			.def("driable", &cLandform::breakable)
			.def("through", &cLandform::through)
			.def("canputdrop", &cLandform::canputdrop)
			.def("diagonthrough", &cLandform::diagonthrough)

			
			.def("isWall", &cLandform::iswall)
			.def("isWater", &cLandform::iswater)
			
			.def("canInto", &cLandform::caninto)

			.def_readonly("mapped", &cLandform::mapped)
			
			//.def("isNonNull", &is_non_null<cLandform>)
			.def("isEqualTo", &forLua::is_equal<cLandform>)
		,

		#define LANDFORMLUABINDDEF(FunctionNamePrefix) \
		luabind::def(#FunctionNamePrefix, &forLua::FunctionNamePrefix)

		LANDFORMLUABINDDEF(LandformRandom_RoomRandom_NotShop_Safe),
		LANDFORMLUABINDDEF(LandformRandom_RoomRandom_Safe),
		LANDFORMLUABINDDEF(LandformRandom_RoomRandom_OutOfRoom_Safe),
		LANDFORMLUABINDDEF(LandformRandom_RoomRandom_OutOfSight_Safe),
		LANDFORMLUABINDDEF(LandformRandom_Random),
		LANDFORMLUABINDDEF(LandformPlace),
		LANDFORMLUABINDDEF(LandformEdge),
		LANDFORMLUABINDDEF(LandformCharacterPosition),
		LANDFORMLUABINDDEF(LandformNearby),
		LANDFORMLUABINDDEF(isSafeSetCharacterLandform),
		LANDFORMLUABINDDEF(getSafeSetCharacterLandform)
	];

	luaEnvironment.setGlobalConstValiable("LANDFORM_ATTRIBUTE_WALL",MAPKIND_WALL);
	luaEnvironment.setGlobalConstValiable("LANDFORM_ATTRIBUTE_WALL2",MAPKIND_WALL2);
	luaEnvironment.setGlobalConstValiable("LANDFORM_ATTRIBUTE_WALLEGDE",MAPKIND_WALLEGDE);
	luaEnvironment.setGlobalConstValiable("LANDFORM_ATTRIBUTE_FLOOR",MAPKIND_FLOOR);
	luaEnvironment.setGlobalConstValiable("LANDFORM_ATTRIBUTE_FLOOR2",MAPKIND_FLOOR2);
	luaEnvironment.setGlobalConstValiable("LANDFORM_ATTRIBUTE_WATER",MAPKIND_WATER);
	luaEnvironment.setGlobalConstValiable("LANDFORM_ATTRIBUTE_FRAGILEWALL",MAPKIND_FRAGILEWALL);
	luaEnvironment.setGlobalConstValiable("LANDFORM_ATTRIBUTE_ABYSS",MAPKIND_ABYSS);

	luaEnvironment.setGlobalConstValiable("MAPHEIGHT",MAPHEIGHT-EDGELANDWIDTH*2);
	luaEnvironment.setGlobalConstValiable("MAPWIDTH",MAPWIDTH-EDGELANDWIDTH*2);

	luaEnvironment.setGlobalConstValiable("EDGELANDWIDTH",EDGELANDWIDTH);

	//luaEnvironment.setGlobalConstValiable("NULLcLandform",NULLLAND);
}

void Event_systemLevel_Loading(LuaEnvironment& luaEnvironment)
{
	luabind::module(luaEnvironment.luaState())
	[

		luabind::class_<cEvent, boost::shared_ptr<cEvent>>("cEvent")
			
			.def(luabind::constructor<cEvent&>())
			
			.def_readonly("priority", &cEvent::priority)

			.enum_("constants")
			[
				luabind::value("PRIORITY_FIRST", cEvent::PRIORITY_FIRST),
				luabind::value("PRIORITY_HIGH", cEvent::PRIORITY_HIGH),
				luabind::value("PRIORITY_MIDDLE", cEvent::PRIORITY_MIDDLE),
				luabind::value("PRIORITY_LOW", cEvent::PRIORITY_LOW),
				luabind::value("PRIORITY_LAST", cEvent::PRIORITY_LAST)
			]
	];

	luabind::module(luaEnvironment.luaState())
	[

		luabind::class_<cEventBackground, cEvent, boost::shared_ptr<cEvent>>("cEventBackground")
			
			.def("fadeIn", &cEventBackground::fadeIn)
			.def("fadeOut", &cEventBackground::fadeOut)

		,
		luabind::def("EventBackground", &forLua::EventBackground)
	];
}

void StyleString_Loading(LuaEnvironment& luaEnvironment)
{

	luabind::module(luaEnvironment.luaState())
	[
		luabind::class_<tstring>("tstring")
			.def(luabind::constructor<>())
			.def(luabind::constructor<tstring &>())
			.def(luabind::self + tstring())
			.def("length", &tstring::length)
			.def(luabind::self == tstring())
		,
		luabind::def("_T", &luaString2tstring)
		,
		luabind::def("string", &tString2luastring)
		,
		luabind::class_<StyleString>("StyleString")
			.def(luabind::constructor<>())
			.def(luabind::constructor<const StyleString&>())
			.def(luabind::constructor<const tstring &>())
			.def(luabind::constructor<const tstring &, unsigned long, float, float, unsigned>())
			.def("addstring", (void(StyleString::*)(const StyleString&))&StyleString::addstring)
			.def("addstring", (void(StyleString::*)(const tstring&, unsigned long, float, float, unsigned))&StyleString::addstring)
			.def("addstring", (void(StyleString::*)(const double, const tstring&, unsigned long, float, float, unsigned))&StyleString::addstring)
			.def(luabind::self + StyleString())
			.def(luabind::self + tstring())
			.def(luabind::self + double())
			.def(luabind::self == StyleString())
			.def("clear", &StyleString::clear)
			.def("isColorDefault", &StyleString::isColorDefault)
			.def("setSameColor", &StyleString::setSameColor)
			.def("setSameSize", (void(StyleString::*)(double, double))&StyleString::setSameSize)
			.def("tstr", &StyleString::conclete_tstr)
			.def("length", &StyleString::length)
			//.def("setSameFlag", &StyleString::setSameFlag)
		,
		luabind::def("setStyle", (StyleString(*)(const tstring&, unsigned long, float, float, unsigned))&setStyle)
		,
		luabind::def("setStyle", (StyleString(*)(const double, const tstring&, unsigned long, float, float, unsigned))&setStyle)
			
	];

	luaEnvironment.setGlobalConstValiable("StyleString_DEFAULT_COLOR",DEFAULT_COLOR);

}

void TextScript_Loading(LuaEnvironment& luaEnvironment)
{
	luabind::module(luaEnvironment.luaState())
	[
		luabind::def("LoadStringStyle", (int(*)(pcScriptRLayer player, StyleString & sstr))&Drop_StringAddStyle)
		,
		luabind::def("LoadStringStyle", (int(*)(pcScriptRLayer player, StyleString & sstr, std::map<tstring, StyleString > & valiable ))&Drop_StringAddStyle)
	];
	
}

void SystemMassage_Loading(LuaEnvironment& luaEnvironment)
{
	luabind::module(luaEnvironment.luaState())
	[
		#define SYSTEMMESSAGELUABINDDEF(FunctionNamePrefix) \
			luabind::def(#FunctionNamePrefix, &forLua::FunctionNamePrefix)
	
		SYSTEMMESSAGELUABINDDEF(cameraMove),
		SYSTEMMESSAGELUABINDDEF(cameraMoveParallel),
		SYSTEMMESSAGELUABINDDEF(moveWithAnime),
		SYSTEMMESSAGELUABINDDEF(moveWithAnimeToLand),
		SYSTEMMESSAGELUABINDDEF(jumpWithAnime),
		SYSTEMMESSAGELUABINDDEF(jumpWithAnimeToLand),
		SYSTEMMESSAGELUABINDDEF(fadeBlack),
		SYSTEMMESSAGELUABINDDEF(fadeWhite),
		SYSTEMMESSAGELUABINDDEF(fadeWithColor),
		SYSTEMMESSAGELUABINDDEF(fadeOut),
		SYSTEMMESSAGELUABINDDEF(setHidingMiniMap),
		SYSTEMMESSAGELUABINDDEF(setHiddenInterface),
		SYSTEMMESSAGELUABINDDEF(gotoSceneTitle),

		luabind::def("stackFunction", (void(*)(luabind::object))&forLua::stackFunction),
		luabind::def("stackFunction", (void(*)(luabind::object function, luabind::object val1))&forLua::stackFunction),
		luabind::def("stackFunction", (void(*)(luabind::object function, luabind::object val1, luabind::object val2))&forLua::stackFunction),
		luabind::def("stackFunction", (void(*)(luabind::object function, luabind::object val1, luabind::object val2, luabind::object val3))&forLua::stackFunction),
		luabind::def("stackFunction", (void(*)(luabind::object function, luabind::object val1, luabind::object val2, luabind::object val3, luabind::object val4))&forLua::stackFunction),
		luabind::def("stackFunction", (void(*)(luabind::object function, luabind::object val1, luabind::object val2, luabind::object val3, luabind::object val4,
			luabind::object val5))&forLua::stackFunction),
		luabind::def("stackFunction", (void(*)(luabind::object function, luabind::object val1, luabind::object val2, luabind::object val3, luabind::object val4,
			luabind::object val5, luabind::object val6))&forLua::stackFunction),
		luabind::def("stackFunction", (void(*)(luabind::object function, luabind::object val1, luabind::object val2, luabind::object val3, luabind::object val4,
			luabind::object val5, luabind::object val6, luabind::object val7))&forLua::stackFunction),
		luabind::def("stackFunction", (void(*)(luabind::object function, luabind::object val1, luabind::object val2, luabind::object val3, luabind::object val4,
			luabind::object val5, luabind::object val6, luabind::object val7, luabind::object val8))&forLua::stackFunction)
		,

		luabind::def("stackDramaThread", &forLua::stackDramaThread),


		SYSTEMMESSAGELUABINDDEF(goToNextFloor),
		luabind::def("messageWithStringStyle", &forLua::message)
		,
		luabind::def("messageWait", &forLua::messageWait)
		,
		luabind::def("messageWaitAnyKey", &forLua::messageWaitAnyKey)
		,
		luabind::def("utility_storyMessage", &forLua::utility_storyMessage)
		,
		luabind::def("utility_localizedMessage", &forLua::utility_localizedMessage)
		,
		luabind::def("localizeString", &(StyleString(*)(const tstring& ))forLua::localizeString)
		,
		luabind::def("localizeString", &(StyleString(*)(const tstring&, std::map<tstring, StyleString > & ))forLua::localizeString)
		,
		SYSTEMMESSAGELUABINDDEF(isExistStoryMessage),
		SYSTEMMESSAGELUABINDDEF(isExistLocalizedMessage),
		SYSTEMMESSAGELUABINDDEF(storyEventMessageLoop),
		SYSTEMMESSAGELUABINDDEF(storyEventMessageLoopRange),
		luabind::def("storyLayerRoot", &forLua::storyLayerRoot)
		,
		luabind::def("eventNotification", &forLua::eventNotification)
		,
		luabind::def("showSelectDungeonWindow", &forLua::showSelectDungeonWindow)
		,
		luabind::def("isNotGoodTerminateInBase", &forLua::isNotGoodTerminateInBase)
		,
		SYSTEMMESSAGELUABINDDEF(addCharacterStorage),
		SYSTEMMESSAGELUABINDDEF(getCharacterStorage),
		SYSTEMMESSAGELUABINDDEF(addDropingStorage),
		SYSTEMMESSAGELUABINDDEF(getDropingStorage),
		SYSTEMMESSAGELUABINDDEF(addLandformStorage),
		SYSTEMMESSAGELUABINDDEF(getLandformStorage),

		SYSTEMMESSAGELUABINDDEF(setClearResultSkipFlag)
	];
	
}
void EffectScript_Loading(LuaEnvironment& luaEnvironment)
{
	luabind::module(luaEnvironment.luaState())
	[
		SYSTEMMESSAGELUABINDDEF(effect_Wait),
		SYSTEMMESSAGELUABINDDEF(effect_WaitPararellCritical),
		SYSTEMMESSAGELUABINDDEF(effect_Smoke1),
		SYSTEMMESSAGELUABINDDEF(effect_boss_focus),
		SYSTEMMESSAGELUABINDDEF(effect_boss_charge),
		SYSTEMMESSAGELUABINDDEF(effect_boss_emission),
		SYSTEMMESSAGELUABINDDEF(effect_boss_defeat),
		SYSTEMMESSAGELUABINDDEF(effect_powerUp),
		SYSTEMMESSAGELUABINDDEF(effect_powerDown),
		SYSTEMMESSAGELUABINDDEF(effect_lightning),
		SYSTEMMESSAGELUABINDDEF(effect_generalStamp),
		SYSTEMMESSAGELUABINDDEF(effect_generalFog),

		SYSTEMMESSAGELUABINDDEF(charaPos),
		
		luabind::class_<EffectFunctions::SpellEffectPart_class>("SpellEffectPart")
			.enum_("constants")
			[
				luabind::value("main", EffectFunctions::SpellEffectPart_class::SpellEffectPart_Main),
				luabind::value("effected", EffectFunctions::SpellEffectPart_class::SpellEffectPart_Effected),
				luabind::value("notEffected", EffectFunctions::SpellEffectPart_class::SpellEffectPart_NotEffected)
			]
	];
}

void cScriptRLayer_Loading(LuaEnvironment& luaEnvironment)
{
	luabind::module(luaEnvironment.luaState())
	[
		luabind::class_<cScriptRLayer,pcScriptRLayer>("DataLayer")
			.def("member", &cScriptRLayer::pmember)
			.def("findMember", &cScriptRLayer::findpmember)
			.def("parent", &cScriptRLayer::pparent)
			.def("name", &cScriptRLayer::name)
			.def("nameAllPass", &cScriptRLayer::nameAllPass)
			.def("topScript", &cScriptRLayer::topscript)
			.def("script", &cScriptRLayer::script)
			.def("memberKeyList", &cScriptRLayer::memberKeyList)
	];
}

void SystemControl_Loading(LuaEnvironment& luaEnvironment)
{
	luabind::module(luaEnvironment.luaState())
	[
		
		luabind::class_<cCommand,pcCommand>("Command")
			.def(luabind::constructor<>())
			.def(luabind::constructor<cCommand&>())

			.def("action", &cCommand::Action)
			.def("shortExplanationText", &cCommand::shortExplanationText)
			.def("resetCaption", &cCommand::resetCaption)
			.def("canIntoShortCut", &cCommand::canIntoShortCut)
			.def("includeObjectiveDroping", &cCommand::includeObjectiveDroping)

			.def_readwrite("caption", &cCommand::caption)
		,
		luabind::class_<cLuaFunctionCommand,cCommand,pcCommand>("LuaFunctionCommand")
			.def(luabind::constructor<cLuaFunctionCommand&>())
			.def(luabind::constructor<StyleString, ::luabind::object, StyleString>())
			.property("freeValiable", &cLuaFunctionCommand::freeValiable)
		,
		
		luabind::class_<cGameWindow,pcGameWindow>("GameWindow")
			.def(luabind::constructor<>())
			.def(luabind::constructor<cGameWindow&>())
			.def("init", (void(cGameWindow::*)(int,int))&cGameWindow::Init)

			.def_readwrite("centerX", &cGameWindow::CenterX)
			.def_readwrite("centerY", &cGameWindow::CenterY)
			.def_readwrite("width", &cGameWindow::Width)
			.def_readwrite("height", &cGameWindow::Height)
			.def("setLeft", &cGameWindow::setLeft)
			.def("setRight", &cGameWindow::setRight)
			.def("setTop", &cGameWindow::setTop)
			.def("setBottom", &cGameWindow::setBottom)
			.def("formLeft", &cGameWindow::formLeft)
			.def("formRight", &cGameWindow::formRight)
			.def("formTop", &cGameWindow::formTop)
			.def("formBottom", &cGameWindow::formBottom)
			.def("left", &cGameWindow::Left)
			.def("right", &cGameWindow::Right)
			.def("top", &cGameWindow::Top)
			.def("bottom", &cGameWindow::Bottom)

			.def_readonly("letterXlength", &cGameWindow::letterXlength)
			.def_readonly("letterYlength", &cGameWindow::letterYlength)
			.def("merginHeight", &cGameWindow::merginheight)
			.def("sideEdgeWith", &cGameWindow::sideEdgeWith)
			.def("lineHeight", &cGameWindow::lineheight)
			.def("lineTop", &cGameWindow::linetop)
			.def_readwrite("color", &cGameWindow::color)
			.property("text", (StyleString&(cGameWindow::*)())&cGameWindow::Text)
			.property("text", (StyleString&(cGameWindow::*)(int))&cGameWindow::Text)
			.property("vText", &cGameWindow::vText)
			.property("pageIndexMax", &cGameWindow::pageIndexMaxF)
			.def_readwrite("pageIndex", &cGameWindow::pageIndex)
			.def("FitTextWidthToWindow", &cGameWindow::FitTextWidthToWindow)
			.def("showWindow", &forLua::pcGameWindow_showWindow)
		,
		luabind::class_<cSelectWindow,cGameWindow,pcGameWindow>("SelectWindow")
			.def(luabind::constructor<>())
			.def(luabind::constructor<cSelectWindow&>())

			.def("addCommand", &cSelectWindow::addCommand)
			.def("commandSize", &cSelectWindow::commandSize)
		,
		luabind::def("menuDismissAllWindows", &forLua::menuDismissAllWindows)
		,
		luabind::def("menuDismissWindowLayer", &forLua::menuDismissWindowLayer)
		,
		luabind::def("menuRerenderAllWindows", &forLua::menuRerenderAllWindows)
		,
		luabind::def("menuDefaultPositionLeft", &forLua::menuDefaultPositionLeft)
		,
		luabind::def("menuDefaultPositionTop", &forLua::menuDefaultPositionTop)
		,
		luabind::def("menuDefaultPositionRight", &forLua::menuDefaultPositionRight)
		,
		luabind::def("menuDefaultPositionBottom", &forLua::menuDefaultPositionBottom)

	];
	luaEnvironment.setGlobalConstValiable("SCREEN_SIZE_X",SCREEN_X);
	luaEnvironment.setGlobalConstValiable("SCREEN_SIZE_Y",SCREEN_Y);
}

void SystemValiable_Loading(LuaEnvironment& luaEnvironment)
{
	luaEnvironment.setGlobalConstValiable("TURN_FOREVER",GAME_TURN_GAMEOVER);

	//luaEnvironment.setGlobalConstValiable("savable_freeFlags",boost::ref(freeFlags_forLua()));

	luaEnvironment.setGlobalConstValiable("buttonIndex_ATTACK",0);
	luaEnvironment.setGlobalConstValiable("buttonIndex_DECIDE",0);
	luaEnvironment.setGlobalConstValiable("buttonIndex_DASH",1);
	luaEnvironment.setGlobalConstValiable("buttonIndex_CANCEL",1);
	luaEnvironment.setGlobalConstValiable("buttonIndex_TURN",2);
	luaEnvironment.setGlobalConstValiable("buttonIndex_MENU",3);
	luaEnvironment.setGlobalConstValiable("buttonIndex_DIAGON",4);
	luaEnvironment.setGlobalConstValiable("buttonIndex_ATTACK",5);
	luaEnvironment.setGlobalConstValiable("buttonIndex_MAP",6);
	luaEnvironment.setGlobalConstValiable("buttonIndex_SDASH",7);

	cValiableFieldChip_Loading<double>(luaEnvironment,"double");
	cValiableFieldChip_Loading<tstring>(luaEnvironment,"tstring");
	cValiableFieldChip_Loading<StyleString>(luaEnvironment,"styleString");
	cValiableFieldChip_Loading<pcCharacter>(luaEnvironment,"cCharacter");
	cValiableFieldChip_Loading<pcDroping>(luaEnvironment,"cDroping");
	cValiableFieldChip_Loading<pcLandform>(luaEnvironment,"cLandform");

	cValiableFieldChip_Loading<set<tstring>>(luaEnvironment,"set_tstring");
	cValiableFieldChip_Loading<set<int>>(luaEnvironment,"set_int");
	cValiableFieldChip_Loading<vector<tstring>>(luaEnvironment,"vector_tstring");
	cValiableFieldChip_Loading<vector<int>>(luaEnvironment,"vector_int");

	luabind::module(luaEnvironment.luaState())
	[
		luabind::class_<cAltaMatterPlus>("AltaMatterPlus")
			.def_readonly("onCount", &cAltaMatterPlus::onCount)
			.def_readonly("offCount", &cAltaMatterPlus::offCount)
			.def_readonly("on", &cAltaMatterPlus::on)
			.def_readonly("eve", &cAltaMatterPlus::eve)
			.def_readonly("justOn", &cAltaMatterPlus::justOn)
			.def_readonly("justOff", &cAltaMatterPlus::justOff)
		,
		luabind::class_<cPlayerInput>("PlayerInput")
			.def("attack", &cPlayerInput::attack)
			.def("turn", &cPlayerInput::turn)
			.def("dash", &cPlayerInput::dash)
			.def("menu", &cPlayerInput::menu)
			.def("miniMap", &cPlayerInput::miniMap)
			.def("diagon", &cPlayerInput::diagon)
			.def("shot", &cPlayerInput::shot)
			.def("smartdash", &cPlayerInput::smartdash)

			.def("decision", &cPlayerInput::decision)
			.def("cancel", &cPlayerInput::cancel)
			
			.def_readonly("x", &cPlayerInput::X)
			.def_readonly("y", &cPlayerInput::Y)

			.def("ue", &cPlayerInput::ue)
			.def("shita", &cPlayerInput::shita)
			.def("hidari", &cPlayerInput::hidari)
			.def("migi", &cPlayerInput::migi)

		,
		luabind::class_<cValiableField>("ValiableField")
			.def(luabind::constructor<>())
			.def(luabind::constructor<cValiableField&>())
			.def_readwrite("doubles", &cValiableField::doubles)
			.def_readwrite("tstrings", &cValiableField::tstrings)
			.def_readwrite("styleStrings", &cValiableField::styleStrings)
			.def_readwrite("charas", &cValiableField::charas)
			.def_readwrite("drops", &cValiableField::drops)
			.def_readwrite("lands", &cValiableField::lands)
			.def_readwrite("strsets", &cValiableField::strsets)
			.def_readwrite("intsets", &cValiableField::intsets)
			.def_readwrite("strvectors", &cValiableField::strvectors)
			.def_readwrite("intvectors", &cValiableField::intvectors)
		,
		luabind::class_<タイミング>("Timing")
			.enum_("constants")
			[
				luabind::value("do_attack_after",攻撃直後時_タイミング),
				luabind::value("receive_attack_after",被攻撃直後時_タイミング),
				
				luabind::value("defeat_enemy_after",敵倒し直後時_タイミング),

				luabind::value("droping_crash_after",落ち物衝突直後時_タイミング),

				luabind::value("droping_himanage_drop_after",落ち物ひまなげ落下直後時_タイミング),
				luabind::value("equipment_crash_before",装備品衝突直前時_タイミング),

				luabind::value("receive_shock_strong",被衝撃_強_タイミング),
				luabind::value("receive_shock_week",被衝撃_弱_タイミング),

				luabind::value("floor_begin",フロア開始_タイミング),
				luabind::value("floor_end",フロア終了_タイミング),

				luabind::value("turn_end",ターン終了_タイミング),
				luabind::value("player_turn_begin_before",主人公ターン開始直前_タイミング),
				luabind::value("section_end",セクション終了_タイミング),
				luabind::value("move_phase_end",移動フェイズ終了_タイミング),
				luabind::value("attack_phase_end",攻撃フェイズ終了_タイミング),
				luabind::value("action_end",アクション終了_タイミング),

				luabind::value("player_input_before",インプット直前_タイミング),
				
				luabind::value("trade_after",売買直後_タイミング),

				luabind::value("die_before_lastspell",死亡直前_ラストスペル_タイミング),
				luabind::value("die_before",死亡直前_タイミング),

				luabind::value("player_attack",主人公素振り_タイミング),

				luabind::value("move_before",通常移動直前_タイミング),
				luabind::value("turn_before",方向転換直前_タイミング),
				luabind::value("player_move_after",主人公通常移動直後_タイミング),

				luabind::value("player_room_in",主人公部屋入室_タイミング),
				luabind::value("player_room_out",主人公部屋退室_タイミング),

				luabind::value("player_neighbor_in",主人公隣接_タイミング),

				luabind::value("attack_directly_before",直接攻撃直前_タイミング),

				luabind::value("receive_attack_approach_before",被攻撃接近直前_タイミング),
				luabind::value("receive_attack_approach_after",被攻撃接近直後_タイミング),

				luabind::value("item_use_after",アイテム使用直後_タイミング),

				luabind::value("trap_fire_private",罠発動専用_タイミング),
				luabind::value("book_read_before",読み直前_タイミング),
				luabind::value("equipment_equip_before",装備直前_タイミング),
				luabind::value("spell_declare_before",宣言直前_タイミング),
				luabind::value("spell_skill_before",スキル直前_タイミング),
				luabind::value("spell_equip_after",スペル装備直後_タイミング),
				luabind::value("spell_unequip_before",スペル装備解除直前_タイミング),

				luabind::value("box_have_electricity",箱帯静電気直前_タイミング),
				luabind::value("book_have_wet",本濡れ直前_タイミング),
				luabind::value("drink_have_ice",水筒冷凍直前_タイミング),
				luabind::value("drink_effect_before",飲み効果直前_タイミング),
				luabind::value("drink_crash_effect_before",飲み物衝突効果直前_タイミング),
				luabind::value("food_have_hot_before",食べ物加熱直前_タイミング),

				luabind::value("pickup_after",拾い直後_タイミング),
				luabind::value("break_before",被破壊直前_タイミング),

				luabind::value("receive_damage_after",ダメージ時_タイミング),
				luabind::value("magic_approach_before",魔法接近直前_タイミング),
				luabind::value("throw_attack_approach_before",投擲攻撃接近直前_タイミング),

				luabind::value("food_have_bad_before",食べ物劣化直前_タイミング),
				luabind::value("food_have_bad_after",食べ物劣化_タイミング),

				luabind::value("darkzone_update",暗闇更新_タイミング),

				luabind::value("command_varb_title",コマンド動詞表記_タイミング),
				luabind::value("command_varb_shortExplanationText",コマンド動詞shortExplanationText_タイミング),



				//この行以降はカットイン回数制限がない。
				//その代わり、ここから下のカットインで別のルーチンのカットインイベントを呼んではならない（ただし伝播は除く）。
				//このルールを破った場合無限ループ防止は保障できない。//一応安全対策は行っている。
				//カットイン値挿入イベント区切り,
				//
				luabind::value("equip_crash_disappear_before",装備品衝突消滅直前時_タイミング),
				
				luabind::value("command_insert_childitem",コマンド挿入_子アイテム_タイミング),
				luabind::value("attack_directly_specify_place",直接攻撃位置指定_タイミング),
				luabind::value("throw_pierce_judge",投擲貫通判定時_タイミング),
				luabind::value("himanage_pierce_judge",ひまなげ投擲貫通判定時_タイミング),
				luabind::value("throw_reflection_num",投擲反射回数計算時_タイミング),
				luabind::value("throw_reflection_num_by_droping",投擲反射回数計算時ドロップ専用_タイミング),

				luabind::value("blowoff_judge",吹き飛ばし判定時_タイミング),
				luabind::value("drawup_judge",引き寄せ判定時_タイミング),
				luabind::value("flingaway_judge",投げ飛ばし判定時_タイミング),

				luabind::value("item_hidding_release_after",アイテム隠れ解除直後時_タイミング),

				luabind::value("normalattack_wallthrough_diagon_judge",通常攻撃_壁斜めすり抜け判定_タイミング),
				luabind::value("normalattack_wallthrough_judge",通常攻撃_壁すり抜け判定_タイミング),
				luabind::value("holditem_num",アイテム所持数計算時_タイミング),
				luabind::value("damage_count_attack",ダメージ計算攻撃時_タイミング),
				luabind::value("damage_count_defense",ダメージ計算防御時_タイミング),
				luabind::value("damage_count_attack_low_priority",ダメージ計算攻撃時優先度低_タイミング),

				luabind::value("item_hit_judge",アイテム命中判定時_タイミング),

				luabind::value("item_pickup_judge",アイテム拾得判定時_タイミング),
				
				luabind::value("trap_target_judge",罠対象キャラ判定時_タイミング),

				luabind::value("attackpower_count",攻撃力計算時_タイミング),
				luabind::value("defensepower_count",防御力計算時_タイミング),
				luabind::value("hitpower_count",直接攻撃命中力計算時_タイミング),
				luabind::value("dodgepower_count",直接攻撃回避力計算時_タイミング),
				luabind::value("item_remain_slot",初期残りスロット計算時_タイミング),

				luabind::value("equipment_base_weaponpower_count",装備品武器力基礎値計算時_タイミング),
				luabind::value("equipment_base_protectorpower_count",装備品防具力基礎値計算時_タイミング),

				luabind::value("equipment_weaponpower_count",装備品武器力計算時_タイミング),
				luabind::value("equipment_protectorpower_count",装備品防具力計算時_タイミング),
				luabind::value("shootpower_count",射撃攻撃力計算時_タイミング),

				luabind::value("experience_get_judge",経験値獲得判定時_タイミング),
				luabind::value("experience_get",経験値獲得時_タイミング),

				luabind::value("trap_firepercent",罠発動率計算時_タイミング),
				luabind::value("character_sight_range_count",キャラクター目視範囲計算時_タイミング),
				luabind::value("enemy_wakeup_count",敵起こしやすさ倍率計算時_タイミング),
				luabind::value("item_have_bad",アイテム劣化直前_タイミング),
				luabind::value("item_have_curse",アイテム呪い直前_タイミング),
				luabind::value("equipment_disarm",装備強制はずし直前_タイミング),
				luabind::value("equipment_attack_mitama_enable",能力発動条件満たしている_攻撃用_タイミング),
				luabind::value("equipment_defence_mitama_enable",能力発動条件満たしている_防御用_タイミング),
				luabind::value("equipment_attack_mark_enable",装備マーク攻撃_タイミング),
				luabind::value("equipment_defence_mark_enable",装備マーク防御_タイミング),
				luabind::value("stumble_before",ころび直前_タイミング),
				luabind::value("stole_before",泥棒直前_タイミング),
				
				luabind::value("swap_equipment_before",装備回し直前_タイミング),

				//精神系
				luabind::value("character_have_sleep",眠り追加直前_タイミング),
				luabind::value("character_have_deepsleep",バクスイ追加直前_タイミング),
				luabind::value("character_have_surprise",びっくり追加直前_タイミング),
				luabind::value("character_have_kanasibari",金縛り追加直前_タイミング),
				luabind::value("character_have_seal",封印追加直前_タイミング),
				luabind::value("character_have_swing",空振り追加直前_タイミング),
				luabind::value("character_have_poor",貧乏追加直前_タイミング),
				luabind::value("character_have_confusion",錯乱追加直前_タイミング),
				luabind::value("character_have_cowardice",臆病追加直前_タイミング),
				luabind::value("character_have_jealousy",嫉妬追加直前_タイミング),
				luabind::value("character_have_madness",狂乱追加直前_タイミング),
				luabind::value("character_have_id",無意識追加直前_タイミング),
				//身体系
				luabind::value("character_have_drunk",泥酔追加直前_タイミング),
				luabind::value("character_have_ice",氷付け追加直前_タイミング),

				luabind::value("character_have_attackdown",脱力追加直前_タイミング),
				luabind::value("character_have_attackup",元気追加直前_タイミング),

				luabind::value("character_have_defensedown",軟弱追加直前_タイミング),
				luabind::value("character_have_defenseup",頑強追加直前_タイミング),
				luabind::value("character_have_torime",鳥目追加直前_タイミング),
				//呪術系
				luabind::value("character_have_sasoi",死の誘い追加直前_タイミング),

				luabind::value("character_have_instantdeath",即死要請直前_タイミング),

				luabind::value("landform_receive_move_enable_judge",隣接地形被移動可判定_タイミング),
				luabind::value("landform_receive_attack_enable_judge",隣接地形被攻撃可判定_タイミング),
				luabind::value("attack_enable_judge",攻撃可判定_タイミング),
				luabind::value("attack_piercewall_judge",攻撃壁貫通判定_タイミング),

				luabind::value("healnatural_count",自然回復量計算時_タイミング),
				luabind::value("hungrynatural_count",自然満腹度減少量計算時_タイミング),

				luabind::value("enable_waterwalk_judge",主人公水上歩行可能判定時_タイミング),
				luabind::value("enable_wallwalk_judge",主人公壁中歩行可能判定時_タイミング),

				luabind::value("skip_danger_judge",飛ばし危険判定時_タイミング),

				luabind::value("enemyai_terget_priority_count",被攻撃優先度計算時_タイミング),

				luabind::value("spelldamage_before",スペルダメージ直前_タイミング),

				luabind::value("imorize_before",スペルダメージ直前_タイミング)
			]
		,

		luabind::class_<変数インデックス>("ValiableIndex")
			.enum_("constants")
			[
				luabind::value("attackpower",変数_攻撃力),
				luabind::value("attackpower_bonus_multi",変数_攻撃力ボーナス_倍率),
				luabind::value("attackpower_bonus_add",変数_攻撃力ボーナス_定数),
				luabind::value("weaponpower_base_bonus_muiti",変数_武器力基礎値ボーナス_倍率),
				luabind::value("weaponpower_base_bonus_add",変数_武器力基礎値ボーナス_定数),
				luabind::value("weaponpower_bonus_multi",変数_武器力ボーナス_倍率),
				luabind::value("weaponpower_bonus_add",変数_武器力ボーナス_定数),
				luabind::value("defensepower",変数_防御力),
				luabind::value("defensepower_bonus_multi",変数_防御力ボーナス_倍率),
				luabind::value("defensepower_bonus_add",変数_防御力ボーナス_定数),
				luabind::value("protecterpower_base_bonus_muiti",変数_防具力基礎値ボーナス_倍率),
				luabind::value("protecterpower_base_bonus_add",変数_防具力基礎値ボーナス_定数),
				luabind::value("protecterpower_bonus_muiti",変数_防具力ボーナス_倍率),
				luabind::value("protecterpower_bonus_add",変数_防具力ボーナス_定数),
				luabind::value("stand_bonus_percent",変数_耐性ボーナス_倍率％),
				luabind::value("stand_effect_flag",変数_防御効果ありフラグ),
				luabind::value("damage_bonus_add",変数_ダメージボーナス_定数),
				luabind::value("damage_cap",変数_ダメージキャップ),
				luabind::value("damage",変数_ダメージ),
				luabind::value("constdamage_flag",変数_定数ダメージフラグ),
				luabind::value("ratiodamage_flag",変数_割合ダメージフラグ),
				luabind::value("ratiodamage",変数_割合ダメージ),
				luabind::value("recovery_bonus",変数_回復力ボーナス_倍率),
				luabind::value("generic_bonus_muiti",変数_汎用ボーナス_倍率),
				luabind::value("generic_bonus_add",変数_汎用ボーナス_定数),
				luabind::value("hitpower",変数_命中力),
				luabind::value("attackdirectly_flag",変数_直接攻撃フラグ),
				luabind::value("dodgepower",変数_回避力),
				luabind::value("aspect",変数_方向),
				luabind::value("pierce",変数_貫通),
				luabind::value("distance",変数_距離),
				luabind::value("propaty",変数_属性),
				luabind::value("propatypower",変数_属性倍率),
				luabind::value("insertcommand",変数_挿入コマンド),
				luabind::value("insertcommand_position",変数_挿入コマンド位置),
				luabind::value("deathavoid",変数_死亡回避),
				luabind::value("phase",変数_フェイズ),
				luabind::value("unusualstate",変数_異常状態),

				luabind::value("attacker",変数_攻撃者),
				luabind::value("defenser",変数_防御者),
				luabind::value("mover",変数_移動者),
				luabind::value("targetchara",変数_対象者),
				luabind::value("attackplace",変数_攻撃場所),
				luabind::value("moveplace",変数_移動場所),
				luabind::value("targetdrop",変数_対象落ち物),

				luabind::value("generic_bool",変数_汎用ブール),
				luabind::value("generic_value",変数_汎用実数),
				luabind::value("generic_value2",変数_汎用実数2),
				luabind::value("generic_const",変数_汎用不変定数),//変えないでほしい場合
				luabind::value("generic_const2",変数_汎用不変定数2)
			]
		,
		luabind::class_<攻撃属性::攻撃属性>("AttackAttribute")
			.enum_("constants")
			[
				luabind::value("explosion",攻撃属性::爆発),
				luabind::value("fire",攻撃属性::火),
				luabind::value("water",攻撃属性::水),
				luabind::value("ice",攻撃属性::冷気),
				luabind::value("soul",攻撃属性::気),
				luabind::value("thunder",攻撃属性::電気),
				luabind::value("poison",攻撃属性::毒),
				luabind::value("sureHit",攻撃属性::必中),
				luabind::value("instantDeath",攻撃属性::敵即死),
				luabind::value("itemBreak",攻撃属性::アイテム破壊)
				/*
				無効エラー = 0,
				アイテム破壊,
				必中,
				敵即死,
				//攻撃属性
				爆発 = 100,
				爆 = 100,
				衝 = 100,
				火 = 101,
				水 = 102,
				冷気 = 103,
				冷 = 103,
				気 = 104,
				電気 = 105,
				雷 = 105,
				//システム属性
				毒 = 200,
				貫通,
				魔法,
				落ち物,
				跳ね返し無効,
				ATTACKATRI_NUM = 19,
				*/
			]
		,
		#define SYSTEMLUABINDDEF(FunctionNamePrefix) \
		luabind::def(#FunctionNamePrefix, &forLua::FunctionNamePrefix)

		SYSTEMLUABINDDEF(controllerInput),

		SYSTEMLUABINDDEF(buttonIndex),
		SYSTEMLUABINDDEF(totalTurnCount),
		SYSTEMLUABINDDEF(floorLevel),
		SYSTEMLUABINDDEF(storyDataLayer),
		SYSTEMLUABINDDEF(localizeDataLayer),
		SYSTEMLUABINDDEF(savable_freeFlags),
		SYSTEMLUABINDDEF(savable_localFlags),
		SYSTEMLUABINDDEF(const_privateFlags),
		SYSTEMLUABINDDEF(const_globalFlags),
		SYSTEMLUABINDDEF(savable_globalFlags),
			
		SYSTEMLUABINDDEF(isUnRelease),

		SYSTEMLUABINDDEF(appearDungeon),
		SYSTEMLUABINDDEF(isAppearedDungeon),
		SYSTEMLUABINDDEF(isDungeonCleared),
		SYSTEMLUABINDDEF(gotoTempDungeon),

		SYSTEMLUABINDDEF(storyEventEnable),

		SYSTEMLUABINDDEF(getMapValue),
		SYSTEMLUABINDDEF(setMapValue),

		SYSTEMLUABINDDEF(isStockingMassage),
		SYSTEMLUABINDDEF(clearMassage),

		luabind::def("isStockingMessage", &forLua::isStockingMassage),
		luabind::def("clearMessage", &forLua::clearMassage),

		SYSTEMLUABINDDEF(alternativeLand),

		SYSTEMLUABINDDEF(canDisCoverCharacter),
		SYSTEMLUABINDDEF(canDisCoverDroping),
		SYSTEMLUABINDDEF(canDisCoverLandform),
		SYSTEMLUABINDDEF(canLandformSeeLandform),

		SYSTEMLUABINDDEF(makeListDisCoverCharacter),
		SYSTEMLUABINDDEF(makeListDisCoverDroping),
		SYSTEMLUABINDDEF(makeListDisCoverExit),

		SYSTEMLUABINDDEF(isWallSeparated),
		SYSTEMLUABINDDEF(landformInFrontOfCharacter),
		SYSTEMLUABINDDEF(landformAroundOfCharacter),
		
		SYSTEMLUABINDDEF(culcuLandformWallReflection),
		SYSTEMLUABINDDEF(culcuLandformWallBending),

		SYSTEMLUABINDDEF(landformSearchObstacleWithRange),
		SYSTEMLUABINDDEF(landformSearchObstacleGetPathWithRange),

		SYSTEMLUABINDDEF(landformSearchObstacle),

		SYSTEMLUABINDDEF(isCharacterCanNeighberAttack),

		SYSTEMLUABINDDEF(landformSearchDropDroping),
		SYSTEMLUABINDDEF(landformSearchDropCharacter),

		SYSTEMLUABINDDEF(isEnemyCharacter),

		SYSTEMLUABINDDEF(mappingFromLandform),
		SYSTEMLUABINDDEF(mappingAllLandform),
		SYSTEMLUABINDDEF(disMappingFromLandform),

		SYSTEMLUABINDDEF(isFloorLight),
		SYSTEMLUABINDDEF(addFloorLight),
		SYSTEMLUABINDDEF(eraseFloorLight),

		SYSTEMLUABINDDEF(isFloorItemCantThrow),
		SYSTEMLUABINDDEF(addFloorItemCantThrow),
		SYSTEMLUABINDDEF(eraseFloorItemCantThrow),

		SYSTEMLUABINDDEF(isFloorTimeStop),
		SYSTEMLUABINDDEF(addFloorTimeStop),
		SYSTEMLUABINDDEF(eraseFloorTimeStop),

		SYSTEMLUABINDDEF(isFloorTimeSkip),
		SYSTEMLUABINDDEF(addFloorTimeSkip),
		SYSTEMLUABINDDEF(eraseFloorTimeSkip),

		SYSTEMLUABINDDEF(isOctasDirection),
		SYSTEMLUABINDDEF(mappingDroping),

		SYSTEMLUABINDDEF(mapTransformArrangement),
		SYSTEMLUABINDDEF(mapTransformWallDig),
		SYSTEMLUABINDDEF(mapTransformWaterMake),
		SYSTEMLUABINDDEF(mapTransformWaterCover),
		SYSTEMLUABINDDEF(mapTransformRoomBreak),
		SYSTEMLUABINDDEF(mapTransformMakeBigRoom),
		SYSTEMLUABINDDEF(mapTransformForseLandChange),

		SYSTEMLUABINDDEF(PlayBGM),
		SYSTEMLUABINDDEF(PlayNormalBGM),
		SYSTEMLUABINDDEF(StopBGM),

		SYSTEMLUABINDDEF(allCharacterList),
		SYSTEMLUABINDDEF(allDropingList),

		SYSTEMLUABINDDEF(spawnFloorNativeItem),
		SYSTEMLUABINDDEF(spawnFloorNativeTrap),
		SYSTEMLUABINDDEF(spawnFloorNativeEnemy),
		
		SYSTEMLUABINDDEF(getIDOfFloorNativeItem),

		SYSTEMLUABINDDEF(walletMoney),
		SYSTEMLUABINDDEF(yakuPoint),
		SYSTEMLUABINDDEF(globalPlayerLavel),
		SYSTEMLUABINDDEF(globalPlayerExp),

		SYSTEMLUABINDDEF(turnCharacterForCharacter),
		
		SYSTEMLUABINDDEF(isValidDirectAttackToEnemy),
		SYSTEMLUABINDDEF(isValidDirectAttackToFriend),
		SYSTEMLUABINDDEF(isValidShootToCharactor),
		SYSTEMLUABINDDEF(distanceOfBetweenCharacters),
		SYSTEMLUABINDDEF(aspectOfBetweenCharacters),
		SYSTEMLUABINDDEF(marginFromShootLineOfBetweenCharacters),

		SYSTEMLUABINDDEF(distanceBetweenCharacter),

		SYSTEMLUABINDDEF(searchOneCharacterOnFloor),
		SYSTEMLUABINDDEF(searchCharactersOnFloor),

		SYSTEMLUABINDDEF(attackHappen)

	];
	luaEnvironment.setGlobalConstValiable("YES",1);
	luaEnvironment.setGlobalConstValiable("NO",0);
}

void DebugFunction_Loading(LuaEnvironment& luaEnvironment)
{
	luabind::module(luaEnvironment.luaState())
	[
		luabind::def("outputDebugString", &forLua::outputDebugString)
		,
		luabind::def("outputDebugValue", &forLua::outputDebugValue)
	];
}

void bossFunction_Loading(LuaEnvironment& luaEnvironment)
{
	luabind::module(luaEnvironment.luaState())
	[
		luabind::class_<弾幕::弾幕色>("BulletColor")
			.enum_("constants")
			[
				luabind::value("blue",弾幕::アオ),
				luabind::value("magenta",弾幕::マゼンタ),
				luabind::value("red",弾幕::アカ),
				luabind::value("orenge",弾幕::ダイダイ),
				luabind::value("orange",弾幕::ダイダイ),
				luabind::value("yellow",弾幕::キイロ),
				luabind::value("green",弾幕::ミドリ),
				luabind::value("cyan",弾幕::シアン),
				luabind::value("black",弾幕::クロ)
			]
		,
		luabind::class_<弾幕::弾幕Type>("bulletType")
			.enum_("constants")
			[
				luabind::value("straight",弾幕::直進),
				luabind::value("homing",弾幕::追尾)
			]
			,
			luabind::def("bulletSummon", &forLua::bulletSummon)
			,
			luabind::def("lazerSummon", &forLua::lazerSummon)
			,
			luabind::def("startBossSpell", &forLua::startBossSpell)
			,
			luabind::def("eraseDanmaku", &forLua::eraseDanmaku)
			,
			luabind::def("eraseFloorDropings", &forLua::eraseFloorDropings)
			,
			luabind::def("eraseEnemys", &forLua::eraseEnemys)
			,
			luabind::def("MakeDropingAndPick", &forLua::MakeDropingAndPick)
			,
			luabind::def("AIshortCut_NomoveAttack", &forLua::AIshortCut_NomoveAttack)
		,
		luabind::class_<int>("int")
			.def(luabind::constructor<int>())
	];
}


void ItemExplanationScript_Loading(LuaEnvironment& luaEnvironment)
{
	map_Loading<tstring,int>(luaEnvironment,"tstring","int");

	luabind::module(luaEnvironment.luaState())
	[
		#define SYSTEMLUABINDDEF(FunctionNamePrefix) \
		luabind::def(#FunctionNamePrefix, &forLua::FunctionNamePrefix)

		SYSTEMLUABINDDEF(controllerInput),
			
		SYSTEMLUABINDDEF(isUnRelease),

		SYSTEMLUABINDDEF(isAppearedDungeon),
		SYSTEMLUABINDDEF(isDungeonCleared),
		
		SYSTEMLUABINDDEF(savable_globalFlags),

		SYSTEMLUABINDDEF(getMapValue)

	];
	luaEnvironment.setGlobalConstValiable("YES",1);
	luaEnvironment.setGlobalConstValiable("NO",0);
}


}//namespace