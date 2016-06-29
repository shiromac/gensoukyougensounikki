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
	bool 投擲物反射CutIn(pcCharacter pchara, タイミング timing, cValiableField& valiable);




};