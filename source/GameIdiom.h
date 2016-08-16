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
//ゲームイデオム
//-------------------------------------------------
namespace GameIdiom
{
	int 高飛び(pcCharacter pchara, pcLandform fromplace);
	int 高飛び(pcDroping pdrop, pcLandform fromplace);


	int 罠チェック(pcLandform fromplace);


	int 爆発(pcLandform fromplace, pcCharacter pchara, double power, int damegeflag, int enemyDestroyFlag, int ItemDestroyFlag, int landDestroyFlag, int selfDamageFlag = 1, int fireAttributeFlag = 0);

	int 広範囲攻撃(pcLandform fromplace, pcCharacter pchara, double power, int damegeflag, std::multiset<攻撃属性::攻撃属性>& attri, int selfDamageFlag);
	int 超広範囲攻撃(pcLandform fromplace, pcCharacter pchara, double power, int damegeflag, std::multiset<攻撃属性::攻撃属性>& attri, int selfDamageFlag);

	int 燃え(pcCharacter pchara, pcLandform pland, int power, int effectlightflag = 0);

	int 火柱召喚(pcCharacter pchara, pcLandform pland, int turn);//pcharaは召喚者
	int アイテム焼失(pcDroping pdrop);



	//軽いイデオム
	int キャラの方を向く(pcCharacter frompchara, pcCharacter tochara);
	

	bool 悪性異常状態治療要請(pcCharacter pchara, int Messageflag = 1);
	bool 良性異常状態治療要請(pcCharacter pchara, int Messageflag = 1);
	bool 全異常状態治療要請(pcCharacter pchara, int Messageflag = 1);

	bool 呪術悪性異常状態治療要請(pcCharacter pchara, int Messageflag = 1);


	bool 悪性異常状態である(pcCharacter pchara);
	bool 良性異常状態である(pcCharacter pchara);
	bool 異常状態である(pcCharacter pchara);

	bool 呪術悪性異常状態である(pcCharacter pchara);

};