#pragma once

#include "cAttackinformation.h"
#include <set>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

class cCharacter;
typedef boost::shared_ptr<cCharacter> pcCharacter;
typedef boost::weak_ptr<cCharacter> wpcCharacter;
class cDroping;
typedef boost::shared_ptr<cDroping> pcDroping;
typedef boost::weak_ptr<cDroping> wpcDroping;
class cLandform;
typedef boost::shared_ptr<cLandform> pcLandform;


//-------------------------------------------------
//ƒQ[ƒ€ƒCƒfƒIƒ€
//-------------------------------------------------
namespace GameIdiom
{
	int ‚”ò‚Ñ(pcCharacter pchara, pcLandform fromplace);
	int ‚”ò‚Ñ(pcDroping pdrop, pcLandform fromplace);


	int ã©ƒ`ƒFƒbƒN(pcLandform fromplace);


	int ”š”­(pcLandform fromplace, pcCharacter pchara, double power, int damegeflag, int enemyDestroyFlag, int ItemDestroyFlag, int landDestroyFlag, int selfDamageFlag = 1, int fireAttributeFlag = 0);

	int L”ÍˆÍUŒ‚(pcLandform fromplace, pcCharacter pchara, double power, int damegeflag, std::multiset<UŒ‚‘®«::UŒ‚‘®«>& attri, int selfDamageFlag);
	int ’´L”ÍˆÍUŒ‚(pcLandform fromplace, pcCharacter pchara, double power, int damegeflag, std::multiset<UŒ‚‘®«::UŒ‚‘®«>& attri, int selfDamageFlag);

	int ”R‚¦(pcCharacter pchara, pcLandform pland, int power, int effectlightflag = 0);

	int ‰Î’Œ¢Š«(pcCharacter pchara, pcLandform pland, int turn);//pchara‚Í¢Š«Ò
	int ƒAƒCƒeƒ€Ä¸(pcDroping pdrop);


	//Œy‚¢ƒCƒfƒIƒ€
	int ƒLƒƒƒ‰‚Ì•û‚ğŒü‚­(pcCharacter frompchara, pcCharacter tochara);
	







};