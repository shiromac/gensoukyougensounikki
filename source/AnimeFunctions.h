#pragma once

class cCharacter;
class cDroping;
class cLandform;
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cCharacter> pcCharacter;
typedef boost::shared_ptr<cDroping> pcDroping;
typedef boost::shared_ptr<cLandform> pcLandform;
//-------------------------------------------------
//汎用エフェクト
//-------------------------------------------------
namespace AnimeFunctions
{

	bool moveWithAnimeToLand(pcCharacter pchara, pcLandform toLand, bool neverTurn, double speed, bool omission);
	bool moveWithAnime(pcCharacter pchara, int aspect, int distance, bool neverTurn, double speed, bool omission);
	bool jumpWithAnimeToLand(pcCharacter pchara, pcLandform toLand, double height, bool neverTurn, double speed, bool omission);
	bool jumpWithAnime(pcCharacter pchara, int aspect, int distance, double height, bool neverTurn, double speed, bool omission);

};