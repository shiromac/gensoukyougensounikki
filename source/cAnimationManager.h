#pragma once


#include "AnimationS.h"

#include "cAnimation.h"
#include "cLandform.h"
#include "cCharacter.h"

#include "cAttackinformation.h"

#include "utility/ValiableField/SharedValiable.h"
#include "utility/ValiableField/SafePointerObject.h"

#include <deque>
#include <map>
#include <set>
#include <vector>
#include <list>
using namespace std;

struct s_Move
{
	pcCharacter pchara;
	pcLandform start;
	pcLandform end;
	int sight;
	int aspect;
};

#define AM_MOVEPOOLHASHNUM 16

//レイヤーの違うアニメは平行して（パラレルに）実行される。
const int LAYERLEVEL_NORMAL = 0; //通常
const int LAYERLEVEL_EXTRAEFFECT1 = 1;//エフェクト1
const int LAYERLEVEL_EXTRAEFFECT2 = 2;//エフェクト2
const int LAYERLEVEL_EXTRAEFFECT3 = 3;//エフェクト3
const int LAYERLEVEL_EXTRAEFFECT4 = 4;//エフェクト4

const int LAYERLEVEL_EXTRAEFFECT5 = 5;//エフェクト5
const int LAYERLEVEL_EXTRAEFFECT6 = 6;//エフェクト6
const int LAYERLEVEL_EXTRAEFFECT7 = 7;//エフェクト7
const int LAYERLEVEL_EXTRAEFFECT8 = 8;//エフェクト8

const int LAYERLEVEL_SPELL = 9;

const int LAYERLEVEL_ATTACK = 10;
const int LAYERLEVEL_DEFENSE = 11;

const int LAYERLEVEL_LAYERMUSTPARALLEL = 16; //強制パラレル
const int LAYERLEVEL_LAYERMAXNUM = 16;

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<s_Move> ps_Move;
typedef boost::shared_ptr<list<ps_Move>> plistps_Move;
class cSaveQuest;
typedef boost::shared_ptr<cSaveQuest> pcSaveQuest;

class cAnimationManager
{
public:
	cAnimationManager(void);
public:
	virtual ~cAnimationManager(void);


	virtual int Init(cRenderDevice *pDev);
	virtual int clear()
	{
		MovePool.clear();
		MoveAnimeList.clear();
		AttackAnimeList.clear();
		ParallelAnimeList.clear();
		return true;
	};

	virtual int process(cRenderDevice *pDev);
	virtual void process_parallel(cRenderDevice *pDev);

	virtual int MoveAnimeprocess(cRenderDevice *pDev);
	virtual int AttackAnimeprocess(cRenderDevice *pDev);

	virtual int Draw(cRenderDevice *pDev);
	virtual void Draw_parallel(cRenderDevice *pDev);

	virtual int AttackAnimeDraw(cRenderDevice *pDev);


	int AddAnime(pcAnimation pcanime);	
	int AddAnime_realtime(pcAnimation pcanime);
	int AddAnime_realtimeCritical(pcAnimation pcanime);

	//強制発動
	int StrongInvocation(cRenderDevice *pDev);

	//必要なら発動
	int WeekInvocation(cRenderDevice *pDev);

	int Enpty;

	//--------------------------------------
	//外から呼ぶ用
	//--------------------------------------
	int AddMove(pcCharacter pchara, pcLandform start, pcLandform end, int aspect = -1);

	int EasyAnime_Attackcharacter(pcCharacter pchara,
									const tstring &effectFileName,
									int vi,
									double speed,
									const tstring &seFileName);
	int EasyAnime_Defensecharacter(pcCharacter pchara,
									const tstring &effectFileName,
									int vi,
									double speed,
									const tstring &seFileName);

	int Anime_Damagecharacter(pcCharacter pchara, int damage, double effective);
	int Anime_Recovercharacter(pcCharacter pchara, int recover);
	int Anime_Retirecharacter(pcCharacter pchara);
/*
	int Anime_DisplayChange(double* pointer, double value);
	int Anime_DisplayChange(int* pointer, int value);
	int Anime_DisplayChange(set<int>* pointer, set<int> value);
	int Anime_DisplayChange(c4DVector* pointer, c4DVector value);
*/
	int Anime_DisplayChange(SharedValiable_likeDouble<double>::Pointer pointer, double value);
	int Anime_DisplayChange(SharedValiable_likeInt<int>::Pointer pointer, int value);
	int Anime_DisplayChange(SafePointer<set<int>> pointer, set<int> value);
	int Anime_DisplayChange(SafePointer<c4DVector> pointer, c4DVector value);
	int Anime_DisplayChange(SafePointer<cMoney> pointer, cMoney value);

	int Anime_Wait(const int placeX, const int placeY, int time);
	int Anime_Wait_parallelCritical(const int placeX, const int placeY, int time);

	int Anime_Message(StyleString m);
	int Anime_MessageClear();
	int Anime_MessageWait(int flag_anykey = 0);
	int Anime_primaryInterfaceWait();
	int Anime_Throw(pcGameObject pGobj, const c4DVector& start, const c4DVector& end,int route = ROUTE_LOW, int drawtype = DRAWTYPE_NORMAL, double speed = 1.0);
	int Anime_Throw_pararell(pcGameObject pGobj, const c4DVector& start, const c4DVector& end,int route = ROUTE_LOW, int drawtype = DRAWTYPE_NORMAL, double speed = 1.0);
	int Anime_PlaySE(tstring path, cCoordinate place);
	int Anime_PlaySE(int play_ID, cCoordinate place);
	int Anime_PlayBGM(const tstring& intro, const tstring& roop);
	int Anime_StopBGM();

	int AddAnime_parallel(pcAnimation pcanime, int wait = 0);
	int AddAnime_parallelCritical(pcAnimation pcanime, int wait = 0);

	int AddAnime_GotoDungeon(const tstring& DungeonID, const map<tstring,int>& privateFlags, const tstring& savefileName);
	int AddAnime_GotoDungeon(const tstring& DungeonID);
	int AddAnime_ContinueDungeon(pcSaveQuest continueQuest);

	int Anime_Function(void (*pFunc)(), int wait = 0);
protected:
	int Anime_Throw_private(pcGameObject pGobj, const c4DVector& start, const c4DVector& end,int route, int drawtype, double speed, bool pararell);
	
protected:
	vector<plistps_Move> MovePool;

	vector<pcAnimation> MoveAnimeList;

	//Move以外
	deque<pcAnimation> AttackAnimeList;

	list<pcAnimation> ParallelAnimeList;

	list<pcAnimation> ParallelCriticalAnimeList;


	//攻撃の性質情報を得る
	void GetNormalAttackAttribute(int Input_ID,
		const tstring &effectFileName, int &vi, double &speed, const tstring &seFileName);

public:
	//存在するすべてのキャラのリスト
	vector<pcCharacter> CharaList_;
	vector<pcCharacter> visibleCharaList_;
public:

	//存在するすべてのアイテムのリスト
	vector<pcDroping> DropList_;
	vector<pcDroping> visibleDropList_;

public:
	//退避キャラのリスト
	vector<pcCharacter> stockCharaList_;
};

int g_Hash(unsigned long int resource, int maxsize);

