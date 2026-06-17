#pragma once
#include "cCharacter.h"

#include "scriptprocesser/DungeonStoryScript.h"

#include "utility/ValiableField/SharedValiable.h"

#include <set>
class cBossSpell
{
public:
	cBossSpell(pcScriptRLayer scriptlayer,
		const tstring& scriptKey,
		pcCharacter boss,
		const int spellIndex);
public:
	virtual ~cBossSpell(void);

	virtual int spellTurnCount();
	virtual int spellRestTurn();

	virtual void setLuaValiable();

	virtual tstring spellName();
	virtual unsigned long imageColor1();
	virtual unsigned long imageColor2();
	
	map<tstring, int> flags;

	virtual int spellTurnLimitCount();
	virtual pcCharacter boss(){return wpBoss_.lock();};
public:
	virtual void startSpell();
	virtual void endSpell();
	virtual pcLandform AIお気に入り地形(cValiableField& valiable);
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual int 特殊攻撃_アニメ(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
	virtual void 弾幕展開(cValiableField& valiable);
public:
	//取り除いた弾幕の数を返す
	static int eraseDanmaku(pcCharacter boss);
public:
	void setSpellStartTurn(int spellStartTurn){spellStartTurn_ = spellStartTurn;};

private:
	wpcCharacter wpBoss_;
	int spellStartTurn_;
	//DungeonStoryScript_LuaEnvironment luaEnv_;
	pcScriptRLayer scriptlayer_;
	tstring scriptKey_;
	int spellIndex_;
private:
	void makeLuaEnv();
	void deleteLuaEnv();
	wpDungeonStoryScript_LuaEnvironment luaEnv_;
	pDungeonStoryScript_LuaEnvironment luaEnv(){return luaEnv_.lock();};

};
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cBossSpell> pcBossSpell;
class caBossEspell;
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<caBossEspell> pcaBossEspell;

class cBoss :
	public cCharacter
{
public:
	cBoss(void);

public:
	virtual ~cBoss(void);

	virtual void InitSub(cRenderDevice *pDev);//子クラスで使うかも

	virtual void settingInit();

	
	virtual bool 雑魚属性(){return false;};

	virtual void CutIn(タイミング timing, cValiableField& valiable);

	virtual bool loadSpell(int spellIndex);

	virtual bool isNoSpell();
	virtual int spellNum();
	virtual bool givedUp();


	virtual void giveUpEvent();

	virtual void startSpell(int index);
	virtual void endSpell(int index);

	virtual int spellIndex(){return spellIndex_;};
	virtual pcBossSpell nowSpell(){return nowSpell_;};
protected:
	virtual void DrawShadow(cRenderDevice *pDev);
	cDrawingObject mouhoujin;
	double mahoujin_count;
public:
	//スペルデリゲート
	virtual pcLandform AIお気に入り地形(cValiableField& valiable);
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual int 特殊攻撃_アニメ(cValiableField& valiable);
	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
	
public:
	virtual int HaveEXP();
	virtual int GetMHP();
	double baseAttackPower();
	double baseDefencePower();
	double baseMHP();
	double baseLV();
	double baseEXP();
	virtual double ValiableConstant1();
	virtual double ValiableConstant2();
	virtual double ValiableConstant3();
	virtual double ValiableConstant4();
protected:
	virtual void gotoNextSpell();
	map<tstring, int> flags;

protected:

	SharedValiable_likeInt<bool>::Pointer spellEndPointer_;
	pcaBossEspell pspelleffect_;

	int spellIndex_;

	pcBossSpell nowSpell_;

	void makeLuaEnv();
	void deleteLuaEnv();
	wpDungeonStoryScript_LuaEnvironment luaEnv_;
	pDungeonStoryScript_LuaEnvironment luaEnv(){return luaEnv_.lock();};

	std::vector<tstring> bossScriptKeys_;
	std::vector<tstring> spellScriptKeys_;
};

