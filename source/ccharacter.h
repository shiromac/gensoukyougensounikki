#pragma once

#include "cgameobject.h"
#include "utility/ValiableField/SharedValiable.h"
#include "timingEnum.h"
#include "utility\valiableField\cValiableField.h"


#define AI_MEMORY_NUM 4

#define DEFAULTCHARACTERSIZE 100

#define DEFAULTTEXPOWER 2

#define DEFAULTSTAMPSUMSTEP (60*6)

#define DEFAULTEMOTIONSUMSTEP 40

#define DEFAULTOPTIONSUMSTEP 80

#define CHARACTER_TEXASPECT_SINPLE 0
#define CHARACTER_TEXASPECT_FOURASPECT 1
#define CHARACTER_TEXASPECT_FULLASPECT 2
#define CHARACTER_TEXASPECT_TURN 3

#define CHARACTER_FORSE_FRIEND 0
#define CHARACTER_FORSE_ENEMY 1
#define CHARACTER_FORSE_OTHER 2

class DungeonStoryScript_LuaEnvironment;
typedef boost::shared_ptr<DungeonStoryScript_LuaEnvironment> pDungeonStoryScript_LuaEnvironment;
typedef boost::weak_ptr<DungeonStoryScript_LuaEnvironment> wpDungeonStoryScript_LuaEnvironment;
//----------------------------------
//エモーション
//----------------------------------
#define EMOTION_NULL 0
#define EMOTION_ASERI 1
#define EMOTION_GURUGURU 2
#define EMOTION_UTSU 3
#define EMOTION_PIYO 4
#define EMOTION_ZZZ 5
#define EMOTION_BIKKURI 6
#define EMOTION_BIRIBIRI 7
#define EMOTION_FUUIN 8
#define EMOTION_DOKU 9
#define EMOTION_GENKI 10
#define EMOTION_KARABURI 11
#define EMOTION_SHITTO 12
#define EMOTION_NANJAKU 13
#define EMOTION_GANKYOU 14
#define EMOTION_TIMESKIP 15
#define EMOTION_KAKKO 16
#define EMOTION_BINBOU 17
#define EMOTION_KOORITUKE 18
#define EMOTION_MIGAWARI 19
#define EMOTION_MUTEKI 20
#define EMOTION_GITAI 21
#define EMOTION_HUNGER 22
#define EMOTION_ITEM 23
#define EMOTION_IKARI 24
#define EMOTION_TORIME 25
#define EMOTION_KYOURAN 26
#define EMOTION_OKUBYOU 27
#define EMOTION_PUNPUN 28
#define EMOTION_SEKIMEN 29
#define EMOTION_POWAPOWA 30

#define EMOTION_SUBINFO_STEP_ATHIRD 101
#define EMOTION_SUBINFO_STEP_AHALF 102
#define EMOTION_SUBINFO_STEP_DOUBLE 103
#define EMOTION_SUBINFO_STEP_TRIPLE 104



class cCharacter;
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
typedef boost::shared_ptr<cCharacter> pcCharacter;
typedef boost::weak_ptr<cCharacter> wpcCharacter;




#include "cDroping.h"
#include "cLandform.h"



#include "cAttackinformation.h"

#include "cCondition.h"
#include "cEnemyAI.h"




/*
//攻撃モーション関係

enum ATTACKATTRI
{
	ATTACKATTRI_PUNCH1,
	ATTACKATTRI_PUNCH1_DOUBLESPEED,
	ATTACKATTRI_SWING1,
};
*/


//new関数定義
#define DEF_FUNC_new_cCharacterClass(z, n, data) \
	pcCharacter def_GetInstanceOf##data##_ID_##n##()\
{\
	return pcCharacter(new data##_ID_##n##);\
};



#define SPELL_EQUIP_MAXNUM 8

//-----------------------------------------------------
//設計上は仮想クラスだがluaの都合で具体クラスにする
//-----------------------------------------------------



#include <set>
using namespace std;


namespace キャラ属性
{
	enum キャラ属性リスト
	{
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

		CHARAATRI_NUM,
	};

}

class cCharacter :
	public cGameObject
{
public:
	cCharacter(void);
public:
	virtual ~cCharacter(void);

protected:
	wpcCharacter self_weakpointer;
public:
	pcCharacter me(){return self_weakpointer.lock();};
	



	int anime_stamp_step;//足踏み



	virtual inline int size(){return DEFAULTCHARACTERSIZE;};
	
	virtual void Init(IDirect3DDevice9 *pDev, pcCharacter self);
	virtual void InitSub(IDirect3DDevice9 *pDev){};

	virtual void InitDungeonStart();
	virtual void FinalizeDungeonEnd();

	virtual void settingInit();
	virtual void naturalSpawnInit();

	virtual void Draw(IDirect3DDevice9 *pDev);
	virtual void DrawBody(IDirect3DDevice9 *pDev);
	virtual void DrawShadow(IDirect3DDevice9 *pDev);

	virtual void DrawMahoujin(IDirect3DDevice9 *pDev);

	virtual void onOverDrive();
	virtual bool isOverDrive(){return overdrive_on;};
protected:
	bool overdrive_on;
	virtual void settingInit_overdrive();

public:
	virtual void onMahoujin(double sizePower);

protected:
	virtual void settingInit_mahoujin();
	cDrawingObject mahoujin;
	double mahoujin_count;
	double mahoujin_sizePower;
	bool mahoujin_on;
public:

	virtual unsigned int ShadowColor();
	virtual bool edgedrawswitch(){return 0;};

	virtual void OptionDraw(IDirect3DDevice9 *pDev);
	IDirect3DTexture9 *p_Texoption;
	int anime_option_step;



	cDrawingObject DO;
	//anime用
	double anime_scaleX;
	double anime_scaleY;
	double anime_pitching;
	double anime_yawing;
	c4DVector anime_position;
	bool anime_stopStamp;
	void setStopStamp(bool stopStamp){anime_stopStamp = stopStamp;};


	virtual void EmotionDraw(IDirect3DDevice9 *pDev);

	virtual void Process(IDirect3DDevice9 *pDev);

	//コンディション特殊能力
	virtual void conditionprocess(){};
		
	//--------------------------------------------
	//割り込みイベント
	//--------------------------------------------
	//virtual int CutInEvent(int timing);

	//virtual double CutInValue(int timing);

	//virtual void CutInAttack(int timing, cAttackinformation & attinfo);
	//統一！！
	virtual void CutIn(タイミング timing, cValiableField& valiable);



	//ネーム系
	virtual StyleString FullName(){return setStyle(FullNameBase(),GetColor_charaname());};
	virtual StyleString FullNameSub(){return setStyle(FullNameSubBase(),GetColor_charaname());};
	virtual StyleString ShortName(){return setStyle(ShortNameBase(),GetColor_charaname());};
	virtual StyleString ShortNameSub(){return setStyle(ShortNameSubBase(),GetColor_charaname());};

	virtual tstring FullNameBase();//{return _T("cCharacter");};
	virtual tstring FullNameSubBase();//{return _T("cCharacter");};
	virtual tstring ShortNameBase();//{return _T("cCharacter");};
	virtual tstring ShortNameSubBase();//{return _T("cCharacter");};

	//唯一名
	virtual tstring uniqueName();//{return _T("cCharacter_undefined");};
	
	//見た目
	virtual tstring surfaceName();//{return usefulName();};

	virtual StyleString titleName();//{return _T("cCharacterTitleName");};



	//表示を実値と同期させる
	virtual void visibleReset();




	IDirect3DTexture9 *p_Texshadow;

	set<int> emotion;//感情
	SafePointerObject<set<int>> visibleemotion;//表示感情
	int anime_emotion_step;
	IDirect3DTexture9 *p_Texemotion;
	virtual inline int GetDrawHeadtall_dotY(){return 34;};



	virtual void GetTex_dot(IDirect3DDevice9 *pDev);
	//テクスチャのデフォルトサイズ
protected:
	int GetTex_Size_dotX_;
	int GetTex_Size_dotY_;
public:
	inline int GetTex_Size_dotX(){return GetTex_Size_dotX_;};
	inline int GetTex_Size_dotY(){return GetTex_Size_dotY_;};

	//キャラの方向の型
	virtual inline int GetTex_aspect_type(){return CHARACTER_TEXASPECT_SINPLE;};

	//キャラネーム色
	virtual unsigned long GetColor_charaname();


	//デフォルトからの描画位置のずれ
	virtual inline int GetDrawDifference_dotX(){return 0;};
	virtual inline int GetDrawDifference_dotY(){return 0;};

	//キャラ属性::キャラ属性リスト
protected:
	set<キャラ属性::キャラ属性リスト> CHARAATTRI;
public:
	//virtual void SetCharaAttribute(){;};
	set<キャラ属性::キャラ属性リスト>& CharaAttribute();

	//キャラ属性とは別
	//永続状態異常系などに作用する。
	virtual bool 雑魚属性(){return false;};

	virtual int 被攻撃優先度();

	virtual bool 水上歩行(){return false;};
	virtual bool 壁中歩行(){return false;};
	virtual bool 空中歩行(){return false;};

	virtual int デフォルト速度移動();
	virtual int デフォルト速度攻撃();

	virtual bool 持ち物放棄可(){return true;};

	virtual vector<pcDroping> GetDropItems();

	//装備品
	pcDroping attackequipment;
	pcDroping defenseequipment;
	pcDroping bulletequipment;


	virtual inline int 最大装備スペル数(){return 1;};
	pcDroping spellequipment[SPELL_EQUIP_MAXNUM];


	//持ってるアイテム
	vector<pcDroping> holdItem;
	//持ってるアイテム
	virtual int itemVolumeInInventory();

	virtual vector<int> 死亡ドロップアイテムIDs();
protected:
	virtual int 死亡ドロップアイテムID();
public:

	//持てる最大数
	virtual inline int MaxholdNum(){return 1;};

	virtual int canTalk(){return false;};
	virtual int TalkEvent();
	virtual pcCommand CharaSkillCommond();
//---------------------------
//高速化用一時パラメーター
//---------------------------
	int sightrange;//視界
	int allsight;//視界

//**************************************
//---------------------------
//パラメーター
//---------------------------
//**************************************
	int Forse;//勢力

	int HP;
	int MHP;
	virtual int MaxMHP(){return 999;};//3桁保障

	int LV;
	virtual int MaxLV(){return 100;};//3桁保障
	virtual tstring LVStr();//レベル表現

	int CLASS;
	SharedValiable_likeInt<int> VisibleCLASS;//二重管理

	int EXP;
	virtual int MaxEXP(){return 99999999;};//8桁保障

	double Stomach;
	double MaxStomach;
	double MaxMaxStomach(){return 200.0;};
	
	cCondition Condition;//異常状態

	virtual cCondition::空腹ProcessFlags 空腹ProcessFlag(){return cCondition::空腹ProcessFlag_無し;};

	int TurnEndFlag;
	int TurnEndFlag_half;
	int LastSpelling;
	//---------------------------
	//こっからAI用
	//---------------------------
	//virtual inline int Get_kindofAI_ID(){return AI_KIND_ID_DEFAULT;};
	virtual pcEnemyAI Get_kindofAI(){return pcEnemyAI(new cEnemyAI);};
	virtual int Get_AI();

	pcEnemyAI pAI;

	int AImemory[AI_MEMORY_NUM];
	//---------------------------
	//ここまでAI用
	//---------------------------

	//その他save用
	virtual void othersavedata(vector<SByte> data){;};
	virtual void otherloaddata(const vector<SByte> data){;};

protected:
	virtual double baseAttackPower(){return 0;};
	virtual double baseDefencePower(){return 0;};
public:
	virtual inline int PerOfspecialAttack(){return 0;};

	virtual double calcuAttackPower(pcDroping pcEqu);
	virtual double calcuDefencePower(pcDroping pcEqu);
	virtual double calcuAttackPower(double equPower);
	virtual double calcuDefencePower(double equPower);

	virtual double calcuExtraAttackPower(pcDroping pcEqu);
	virtual double calcuExtraDefencePower(pcDroping pcEqu);
	virtual double calcuNoAppreciationExtraAttackPower(pcDroping pcEqu);
	virtual double calcuNoAppreciationExtraDefencePower(pcDroping pcEqu);


	virtual double GetAttackPower();
	virtual double GetDefencePower();
	virtual double GetBulletAttackPower(){return calcuAttackPower(0.0);};
	virtual pcLandform GetAttackLand(int& outputaspect, int& outputdistance);
	

	//百分率
	virtual double GetHitPowerBase();
	virtual double GetDodgePowerBase();
	virtual double GetItemDodgePowerBase();
	virtual double default_GetHitPowerBase();
	virtual double default_GetDodgePowerBase();
	virtual double default_GetItemDodgePowerBase();

	virtual int isSpecialAttack();

	virtual double HP自然回復割合();
	virtual double HP自然回復開始ターン();
	virtual double HP自然回復最低保障値();
public:
	virtual int SetMHP();
	virtual int GetMHP();
	virtual int upMHP();

	virtual int HaveEXP(){return 0;};
	virtual bool DefeatCountable(){return true;};

	//上がったレベル数を返す
	virtual int addEXP(int e){return 0;};

	virtual int LvUP(){return 0;};
	virtual int LvDown(){return 0;};

//----------------------------------------
//ユーティリティー
//----------------------------------------
	virtual bool 死亡();


	virtual pcDroping 足元();
	virtual pcLandform 足元地形();

	

	virtual bool 拾得可(){return true;};//余白無考慮
	virtual bool 持ち物余白あり(){return (MaxholdNum() > itemVolumeInInventory());};
	//拾おうとしているアイテムが無限に拾えた場合は余白ありと返す
	virtual bool 持ち物余白あり(pcDroping pwillPickDrop);

	virtual bool 持ち物投擲可(){return true;};

	virtual bool 持ち物衝突可(pcDroping pdrop){return true;};

	virtual bool すれ違い許可(pcCharacter pchara);

	//----------------------------------------
	//こっからスクリプト
	//----------------------------------------
	virtual int 特殊攻撃(cValiableField& valiable);
	virtual void AI変数引渡し(cValiableField& valiable){;};
	virtual int AI最大間合い(cValiableField& valiable){return 10;};
	virtual pcLandform AIお気に入り地形(cValiableField& valiable){return NULLLAND;};
	virtual bool AI移動許可(cValiableField& valiable){return true;};
	virtual int AI特殊攻撃選択pre(cValiableField& valiable);
	virtual int AI特殊攻撃選択(cValiableField& valiable);
	virtual double AI特殊攻撃選択確率(){return 1.0;};
	virtual int 特殊攻撃効果(cValiableField& valiable);
	virtual int 特殊攻撃_アニメ(cValiableField& valiable);

	virtual void パッシブ能力(タイミング timing, cValiableField& valiable);
	virtual void パッシブ能力sub(タイミング timing, cValiableField& valiable);
	virtual void パッシブ能力属性弱点耐性(タイミング timing, cValiableField& valiable);
	
	virtual int 通常攻撃();

	virtual int 通常攻撃効果();
	virtual int 通常攻撃_アニメ();

	virtual bool 通常攻撃_壁斜めすり抜け();
	virtual bool 通常攻撃_壁すり抜け();
	
	virtual int 通常防御_アニメ();

	virtual double ValiableConstant1(){return 0;};
	virtual double ValiableConstant2(){return 0;};
	virtual double ValiableConstant3(){return 0;};
	virtual double ValiableConstant4(){return 0;};

	
	static double 強特攻耐性_ダメージ軽減％();
	static double 特攻耐性_ダメージ軽減％();
	static double 強元気耐性_上昇強度();
	static double 強元気耐性_上昇ターン();
	static double 元気耐性_上昇強度();
	static double 元気耐性_上昇ターン();
	static double 強頑強耐性_上昇強度();
	static double 強頑強耐性_上昇ターン();
	static double 頑強耐性_上昇強度();
	static double 頑強耐性_上昇ターン();
	static double 強特攻弱点_ダメージ増加％();
	static double 特攻弱点_ダメージ増加％();
	static double 強脱力弱点_下降強度();
	static double 強脱力弱点_下降ターン();
	static double 脱力弱点_下降強度();
	static double 脱力弱点_下降ターン();
	static double 強軟弱弱点_下降強度();
	static double 強軟弱弱点_下降ターン();
	static double 軟弱弱点_下降強度();
	static double 軟弱弱点_下降ターン();
	static double オーバードライブHP倍率();
	static double オーバードライブ攻撃倍率();
	static double オーバードライブ防御倍率();
	static double オーバードライブ経験値倍率();
	static double オーバードライブドロップ数();
	static double オーバードライブ混酒の箱ドロップ率％();
	static double オーバードライブ固有ドロップ上昇倍率();

	//----------------------------------------
	//luaスクリプト
	//----------------------------------------
	std::vector<wpDungeonStoryScript_LuaEnvironment> luaScript_;
	virtual tstring luaScriptErrorFileName(tstring& functionName, pDungeonStoryScript_LuaEnvironment& luaScriptEnv);
};





#define NULLCHARA pcCharacter((cCharacter*)NULL)