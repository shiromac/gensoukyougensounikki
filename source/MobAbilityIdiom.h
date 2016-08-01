#pragma once

#include <set>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "timingEnum.h"
#include "utility/valiableField/cValiableField.h"

class cCharacter;
typedef boost::shared_ptr<cCharacter> pcCharacter;
typedef boost::weak_ptr<cCharacter> wpcCharacter;
class cDroping;
typedef boost::shared_ptr<cDroping> pcDroping;
typedef boost::weak_ptr<cDroping> wpcDroping;
class cLandform;
typedef boost::shared_ptr<cLandform> pcLandform;


//-------------------------------------------------
//敵能力イデオム
//-------------------------------------------------
namespace MobAbilityIdiom
{
	class CutInFunctionObject {
		public:
		virtual bool operator()(pcCharacter pchara, タイミング timing, cValiableField& valiable) const = 0;
	};

#define DEF_MobAbilityIdiom_CutInFunction_Class(name) class name : public CutInFunctionObject {\
		public:\
		virtual ~name(){};\
		virtual bool operator()(pcCharacter pchara, タイミング timing, cValiableField& valiable) const;
// def end

#define DEF_MobAbilityIdiom_CutInFunction_Class_Value(name, value) class name : public CutInFunctionObject {\
		public:\
		virtual ~name(){};\
		virtual bool operator()(pcCharacter pchara, タイミング timing, cValiableField& valiable) const;\
		private:\
			name(){};\
		public:\
			name(double value_):value(value_){};\
		double value;
// def end

#define DEF_MobAbilityIdiom_CutInFunction_Class_Value2(name, valueX, valueY) class name : public CutInFunctionObject {\
		public:\
		virtual ~name(){};\
		virtual bool operator()(pcCharacter pchara, タイミング timing, cValiableField& valiable) const;\
		private:\
			name(){};\
		public:\
			name(double valueX_, double valueY_):valueX(valueX_),valueY(valueY_){};\
		double valueX;\
		double valueY;
// def end

	DEF_MobAbilityIdiom_CutInFunction_Class(投擲物反射CutIn)};


	DEF_MobAbilityIdiom_CutInFunction_Class(拾得物修正状態識別CutIn)};
	DEF_MobAbilityIdiom_CutInFunction_Class_Value(フロア開始時所持アイテムランダム識別CutIn, count)};


	DEF_MobAbilityIdiom_CutInFunction_Class_Value(投擲物魔法ダメージ化CutIn, damegeValue)};
	DEF_MobAbilityIdiom_CutInFunction_Class_Value(魔法ダメージ化CutIn, damegeValue)};
	DEF_MobAbilityIdiom_CutInFunction_Class_Value(投擲物ダメージ化CutIn, damegeValue)};


	
	DEF_MobAbilityIdiom_CutInFunction_Class_Value(遠距離ダメージ軽減CutIn, reduceDamegePercent)};


	DEF_MobAbilityIdiom_CutInFunction_Class(呪い無効化CutIn)};


	DEF_MobAbilityIdiom_CutInFunction_Class(攻撃時自分ノックバックCutIn)};
	

	DEF_MobAbilityIdiom_CutInFunction_Class_Value2(攻撃時眠り付与CutIn, percent, turn)};

};