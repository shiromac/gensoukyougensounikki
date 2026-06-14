#pragma once



#include <vector>
#include <list>
#include <tstring_ph.h>

#include "utility/cRandomGen.h"
#include "utility/cDiscreteProbability.h"

#include "cDropingDistribution.h"

#include <cScriptRLayer.h>
#include <cScriptReader.h>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "scriptprocesser/DungeonStoryScript.h"

class cFloor;

typedef boost::shared_ptr<cFloor> pcFloor;
typedef boost::weak_ptr<cFloor> wpcFloor;

enum floorMapGenForm
{
	FLOORMAPGENFORM_RANDOM = 0,
	FLOORMAPGENFORM_CONST,
	FLOORMAPGENFORM_SHUFFLE,
};


class cParty
{
public:
	virtual int& pos(){return pos_;};
protected:
	int pos_;
public:
	virtual cDiscreteProbability enemyDP();
	virtual cDropingDistribution dropitemDP();
	virtual cDiscreteProbability trapDP();
	int enemyDPexist;
	int dropitemDPexist;
	int trapDPexist;
public:
	cDiscreteProbability enemyDP_;
	cDropingDistribution dropitemDP_;
	cDiscreteProbability trapDP_;
public:
	//敵初期配置
	virtual int enemyfirstsetnum(const int landnum, int floornum);

	double& enemyfirstsetnums(int index){return enemyfirstsetnums_[index];};
private:
	double enemyfirstsetnums_[8];

public:
	//罠初期配置
	virtual int trapfirstsetnum(const int landnum, int floornum);

	double& trapfirstsetnums(int index){return trapfirstsetnums_[index];};
private:
	double trapfirstsetnums_[8];

public:
	//アイテム初期配置数
	virtual int itemfirstsetnum(const int landnum, int floornum);

	double& itemfirstsetnums(int index){return itemfirstsetnums_[index];};
private:
	double itemfirstsetnums_[8];
};
typedef boost::shared_ptr<cParty> pcParty;

class cFloor
{
public:
	cFloor(void);
public:
	virtual ~cFloor(void)
	{
		enemyDP_.clear();
		dropitemDP_.clear();
		shopitemDP_.clear();
		trapDP_.clear();
		dungeonDP_.clear();
		dungeonFormVal_.clear();
		dungeonRandomVal_.clear();
	};

	unsigned long floorrandomSeed;

	int floorindex;
public:
	virtual int floorMapGenForm(){return floorMapGenForm_;};
protected:
	int floorMapGenForm_;
public:
	const tstring& mapchipsetname(){return mapchipsetname_;};
protected:
	tstring mapchipsetname_;

public:
	virtual const cDiscreteProbability& enemyDP(){return enemyDP_;};
	virtual const cDropingDistribution& dropitemDP(){return dropitemDP_;};
	virtual const cDropingDistribution& shopitemDP(){return shopitemDP_;};
	virtual const cDiscreteProbability& trapDP(){return trapDP_;};


	virtual const cDiscreteProbability& dungeonDP(){return dungeonDP_;};
	virtual map<tstring,vector<double>>& dungeonFormVal(){return dungeonFormVal_;};
	virtual map<int,vector<double>>& dungeonRandomVal(){return dungeonRandomVal_;};
	

protected:
	cDiscreteProbability enemyDP_;
	cDropingDistribution dropitemDP_;
	cDropingDistribution shopitemDP_;
	cDiscreteProbability trapDP_;

	cDiscreteProbability dungeonDP_;
	map<tstring,vector<double>> dungeonFormVal_;
	map<int,vector<double>> dungeonRandomVal_;

private:
	virtual void decodeProbabilityItem(cDiscreteProbability& dp, const cDiscreteProbability& modeldp, pcScriptRLayer pfloordata, const tstring& name);
	virtual void decodeProbabilityChara(cDiscreteProbability& dp, const cDiscreteProbability& modeldp, pcScriptRLayer pfloordata, const tstring& name);

	virtual void decodeProbabilityGroup(cDropingDistribution& dp, const cDropingDistribution& modeldp, pcScriptRLayer pfloordata, const tstring& name);

	virtual void decodetstring(tstring& tv, const tstring& modelv, pcScriptRLayer pfloordata, const tstring& name, const int index);

	virtual void decodeInt(int& tv, const int modelv, pcScriptRLayer pfloordata, const tstring& name, const int index);
	virtual void decodeInt(int& tv, const int modelv, pcScriptRLayer pfloordata, const tstring& parentname, const tstring& name, const int index);
	virtual void decodeDouble(double& tv, const double modelv, pcScriptRLayer pfloordata, const tstring& parentname, const tstring& name, const int index);
	virtual void decodeDouble(double& tv, const double modelv, pcScriptRLayer pfloordata, const tstring& name, const int index);
	
	virtual void decodeIntMatrix(vector<vector<int>>& tvvi, const vector<vector<int>>& modelvvi, pcScriptRLayer pfloordata, const tstring& name);
	virtual void decodeIntMatrix(vector<vector<int>>& tvvi, const vector<vector<int>>& modelvvi, pcScriptRLayer pfloordata, const tstring& parentname, const tstring& name);

	virtual void decodeParty(pcParty& tv, pcScriptRLayer floordata);
	

	virtual void decodeDungeon(pcFloor pmodelfloor, pcScriptRLayer floordata);

	virtual void decodeDungeonRandom(pcFloor pmodelfloor, pcScriptRLayer floordata);
	virtual void decodeDungeonConst(pcFloor pmodelfloor, pcScriptRLayer floordata);
	virtual void decodeDungeonShuffle(pcFloor pmodelfloor, pcScriptRLayer floordata);
	virtual void decodeDungeonForm(pcFloor pmodelfloor, pcScriptRLayer floordata);

public:

	virtual int decodeFloor(pcFloor pmodelfloor, pcScriptRLayer floordata);
public:
	//敵自然湧き（毎ターン聞く）
	//0で自然湧きしない
	virtual int enemyAppear();

	const int appearenemycount(){return appearenemycount_;};
	const int nextappearenemy(){return nextappearenemy_;};
	const int Maxappearenemy(){return Maxappearenemy_;};
	const int Minappearenemy(){return Minappearenemy_;};
	const int Maxenemynum(){return Maxenemynum_;};
private:
	int appearenemycount_;
	int nextappearenemy_;
	int Maxappearenemy_;
	int Minappearenemy_;
	int Maxenemynum_;
public:
	//敵初期配置
	virtual int enemyfirstsetnum(const int roomnum, const int landnum);

	const double MinenemyfirstsetnumPerFloor(){return enemyfirstsetnums_[0];};
	const double MaxenemyfirstsetnumPerFloor(){return enemyfirstsetnums_[1];};
	const double MinenemyfirstsetnumPerRoom(){return enemyfirstsetnums_[2];};
	const double MaxenemyfirstsetnumPerRoom(){return enemyfirstsetnums_[3];};
	const double MinenemyfirstsetnumPerLand(){return enemyfirstsetnums_[4];};
	const double MaxenemyfirstsetnumPerLand(){return enemyfirstsetnums_[5];};
	const double MinenemyfirstsetnumPerFloorIndex(){return enemyfirstsetnums_[6];};
	const double MaxenemyfirstsetnumPerFloorIndex(){return enemyfirstsetnums_[7];};
	const double enemyfirstsetnums(int index){return enemyfirstsetnums_[index];};
private:
	double enemyfirstsetnums_[8];

public:
	//罠初期配置
	virtual int trapfirstsetnum(const int roomnum, const int landnum);

	const double MintrapfirstsetnumPerFloor(){return trapfirstsetnums_[0];};
	const double MaxtrapfirstsetnumPerFloor(){return trapfirstsetnums_[1];};
	const double MintrapfirstsetnumPerRoom(){return trapfirstsetnums_[2];};
	const double MaxtrapfirstsetnumPerRoom(){return trapfirstsetnums_[3];};
	const double MintrapfirstsetnumPerLand(){return trapfirstsetnums_[4];};
	const double MaxtrapfirstsetnumPerLand(){return trapfirstsetnums_[5];};
	const double MintrapfirstsetnumPerFloorIndex(){return trapfirstsetnums_[6];};
	const double MaxtrapfirstsetnumPerFloorIndex(){return trapfirstsetnums_[7];};
	const double trapfirstsetnums(int index){return trapfirstsetnums_[index];};
private:
	double trapfirstsetnums_[8];

public:
	//アイテム初期配置数
	virtual int itemfirstsetnum(const int roomnum, const int landnum);

	const double MinitemfirstsetnumPerFloor(){return itemfirstsetnums_[0];};
	const double MaxitemfirstsetnumPerFloor(){return itemfirstsetnums_[1];};
	const double MinitemfirstsetnumPerRoom(){return itemfirstsetnums_[2];};
	const double MaxitemfirstsetnumPerRoom(){return itemfirstsetnums_[3];};
	const double MinitemfirstsetnumPerLand(){return itemfirstsetnums_[4];};
	const double MaxitemfirstsetnumPerLand(){return itemfirstsetnums_[5];};
	const double MinitemfirstsetnumPerFloorIndex(){return itemfirstsetnums_[6];};
	const double MaxitemfirstsetnumPerFloorIndex(){return itemfirstsetnums_[7];};
	const double itemfirstsetnums(int index){return itemfirstsetnums_[index];};
private:
	double itemfirstsetnums_[8];

public:
	//アイテム初期配置
	const vector<vector<int>>& itemfirstsets(){return itemfirstsets_;};
private:
	vector<vector<int>> itemfirstsets_;

public:
	//敵初期配置
	const vector<vector<int>>& charafirstsets(){return charafirstsets_;};
private:
	vector<vector<int>> charafirstsets_;

public:
	//主人公初期配置
	const vector<vector<int>>& friendfirstsets(){return friendfirstsets_;};
private:
	vector<vector<int>> friendfirstsets_;

public:
	//マップ
	const vector<vector<tstring>>& floormap(){return floormap_;};
private:
	vector<vector<tstring>> floormap_;

public:
	//シャッフルマップ
	map<tstring,vector<vector<tstring>>>& sufflefloormap(){return sufflefloormap_;};
	map<tstring,double>& sufflefloorpossiblity(){return sufflefloorpossiblity_;};

private:
	map<tstring,vector<vector<tstring>>> sufflefloormap_;
	map<tstring,double> sufflefloorpossiblity_;

public:
	//店資金
	const double ShopFirstFund(){return shopfunddefines_[0];};
	const double ShopConstProfit(){return shopfunddefines_[1];};
	const double ShopPerProfit(){return shopfunddefines_[2];};
	const double shopfunddefines(int index){return shopfunddefines_[index];};
private:
	double shopfunddefines_[3];

	//宴会
public:
	virtual pcParty getParty();

	virtual vector<pcParty> vpParty(){return vpParty_;};
	virtual cDiscreteProbability cPartyDP(){return cPartyDP_;};
private:
	vector<pcParty> vpParty_;
	cDiscreteProbability cPartyDP_;

	//スクリプト
public:
	virtual const std::vector<tstring>& extraLuaScriptKeys(){return extraLuaScriptKeys_;};
	virtual pcScriptRLayer extraLuaScript(){return extraLuaScript_;};
private:
	std::vector<tstring> extraLuaScriptKeys_;
	pcScriptRLayer extraLuaScript_;
};


class cDungeon
{
public:
	cDungeon(void){appreciationpos_ = 1;};
public:
	virtual ~cDungeon(void){};


	virtual void decode(pcScriptRLayer data);
	virtual int MaxFloor();

	virtual int culcuscore();
	static int culcuscore(pcSaveQuest pQuest);
	static int culcuscore(const pcCharacter pPlayer,
						 const int floorlevel,
						 const int sumTurnCount,
						 const int sumEnemyDefeatNum,
						 const int FirstConditionOfMoney,
						 const pcCharacter pFirstConditionOfPlayer,
						 const bool appreciationSupport);

	virtual pcFloor pfloor(int floorindex);

	StyleString DungeonName_;
	tstring DungeonID_;
public:
	const double& appreciationpos(){return appreciationpos_;};
	const vector<double>& appreciationposkind(){return appreciationposkind_;};
	const bool allAppreciationOfBullet();
	const bool allAppreciationOfSpell();
	const bool allAppreciationOfDrink();
	const bool allAppreciationOfBook();
	const bool allAppreciationOfBox();
protected:
	double appreciationpos_;
	vector<double> appreciationposkind_;
private:
	vector<pcFloor> vpfloor_;


	pcScriptRLayer data_;
public:
	const int& homeFlag(){return homeFlag_;};
protected:
	int homeFlag_;

public:
	const double& playerExpPower(){return playerExpPower_;};
protected:
	int playerExpPower_;

public:
	const int& clearResultSkipFlag(){return clearResultSkipFlag_;};
	void setClearResultSkipFlag(int flag){clearResultSkipFlag_ = flag;};
protected:
	int clearResultSkipFlag_;

public:
	enum DungeonDirection{
		DungeonDirection_Up = 0,
		DungeonDirection_Down = 1,
	};
	const DungeonDirection& dungeonDirection(){return dungeonDirection_;};
protected:
	DungeonDirection dungeonDirection_;

public:
	const vector<vector<int>>& firstItems(){return firstItems_;};
protected:
	vector<vector<int>> firstItems_;

public:
	//ランダムジェネレーター
	virtual cRandomGen& randomgen(){return randomgen_;};

private:
	cRandomGen randomgen_;


public:
	//スクリプト
	virtual const std::vector<tstring>& storyScriptKeys(){return storyScriptKeys_;};
	
	virtual std::vector<DungeonStoryScript_LuaEnvironment>& storyScriptEnvironments(){return storyScriptEnvironments_;};

	virtual pcScriptRLayer storyScript(){return storyScript_;};
private:
	std::vector<tstring> storyScriptKeys_;
	pcScriptRLayer storyScript_;
	std::vector<DungeonStoryScript_LuaEnvironment> storyScriptEnvironments_;

public:
	//レギュレーションスクリプト
	//virtual pDungeonStoryScript_LuaEnvironment regulationScriptEnvironment(){return regulationScriptEnvironment_;};
private:
	//pDungeonStoryScript_LuaEnvironment regulationScriptEnvironment_;
};

typedef boost::shared_ptr<cDungeon> pcDungeon;
typedef boost::weak_ptr<cDungeon> wpcDungeon;
