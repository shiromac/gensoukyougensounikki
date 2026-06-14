#pragma once

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
//検索ユーティリティー
//-------------------------------------------------
namespace FindUtility
{

	//すべての部屋の確率が同じ（面積に比例しない）
	//配置安全
	pcLandform 各部屋等確率_ランダム地形検索_配置安全();

	//すべての部屋の確率が同じ（面積に比例しない）
	//配置安全
	pcLandform 各部屋等確率_ランダム地形検索_店回避_配置安全();

	//今の部屋以外が優先されるランダムワープ
	//すべての部屋の確率が同じ（面積に比例しない）
	//配置安全
	pcLandform 部屋外優先_各部屋等確率_ランダム地形検索_配置安全(pcLandform pland);

	//今の部屋以外が優先されるランダムワープ
	//部屋がひとつの場合は視界の外を優先する。
	//すべての部屋の確率が同じ（面積に比例しない）
	//配置安全
	pcLandform 視界外優先_各部屋等確率_ランダム地形検索_配置安全(pcLandform pland);


	//今の部屋以外が優先されるランダムワープ
	//すべての部屋の確率が同じ（面積に比例しない）
	//非配置安全
	pcLandform 部屋外優先_各部屋等確率_ランダム地形検索_非配置安全(pcLandform pland);


	//今の部屋以外が優先されるランダムワープ
	//部屋がひとつの場合は視界の外を優先する。
	//すべての部屋の確率が同じ（面積に比例しない）
	//非配置安全
	pcLandform 視界外優先_各部屋等確率_ランダム地形検索_非配置安全(pcLandform pland);


	//視界 rangeでの敵を索敵する
	vector<pcCharacter> フロア敵索敵_敵リスト(pcCharacter pchara, const int range);

	vector<pcCharacter> 透明でない部屋内と隣接味方リスト(pcCharacter pchara);
	
	vector<pcCharacter> 部屋内と隣接敵リスト(pcCharacter pchara);

	pcCharacter 敵リストから優先敵索敵(const vector<pcCharacter>& vpchara, pcCharacter fromchara);

	bool キャラが敵で直接攻撃可能か(pcCharacter me, pcCharacter enemy);

	bool キャラが味方で直接攻撃可能か(pcCharacter me, pcCharacter chara);

	bool キャラ８方向射線で範囲以内(pcCharacter me, pcCharacter enemy, const int range);


	pcCharacter ターゲットキャラクター検索(const vector<pcCharacter>& charalist, pcCharacter me, double(*priorityFunction)(pcCharacter, pcCharacter));
	pcDroping ターゲット落ち物検索(const vector<pcDroping>& droplist, pcCharacter me, double(*priorityFunction)(pcCharacter, pcDroping));
	pcLandform ターゲット地形検索(const vector<pcLandform>& landlist, pcCharacter me, double(*priorityFunction)(pcCharacter, pcLandform));

	int キャラの距離(pcCharacter me, pcCharacter you);

	vector<pcDroping> 装備品以外の表面手持ちアイテム店売り除く(pcCharacter pchara);

	int キャラの方向(pcCharacter me, pcCharacter you);

	int キャラ八方ズレ数(pcCharacter me, pcCharacter you);

	//地形のみ見る、壁斜め通らない
	bool キャラ中距離攻撃地形射線通り(pcCharacter me, pcCharacter you, int distance);

	//敵味方区別なし、地形のみ見る、壁斜め通らない
	int キャラ中距離攻撃地形攻撃距離(pcCharacter me, int aspect, int distance);


};