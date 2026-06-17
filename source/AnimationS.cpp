#include "stdafx.h"

#include "cAnimationManager.h"
#include "caFunction.h"
#include "caFade.h"
//アニメーションマネージャーの各アニメクラス化処理関係


int cAnimationManager::EasyAnime_Attackcharacter(pcCharacter pchara,
									const tstring &effectFileName,
									int vi,
									double speed,
									const tstring &seFileName)
{
	pcaAttack caat = pcaAttack(new caAttack);



	caat->SE_playID = g_GameEnv.m_SoundManager.getSoundEffectFromFile(seFileName.c_str());

	
	caat->LayerLevel = LAYERLEVEL_ATTACK;
	caat->effectfilename = effectFileName;
	caat->vi = vi;
	caat->speed = speed;
	caat->pAttacker = pchara;
	caat->pLand = pchara->GetAttackLand(caat->attackaspect,caat->attackdistance);


	AddAnime(boost::static_pointer_cast<cAnimation>(caat));

	return true;
}

int cAnimationManager::EasyAnime_Defensecharacter(pcCharacter pchara,
									const tstring &effectFileName,
									int vi,
									double speed,
									const tstring &seFileName)
{
	pcaDefense cadf = pcaDefense(new caDefense);



	cadf->SE_playID = g_GameEnv.m_SoundManager.getSoundEffectFromFile(seFileName.c_str());

	
	cadf->LayerLevel = LAYERLEVEL_DEFENSE;
	cadf->effectfilename = effectFileName;
	cadf->vi = vi;
	cadf->speed = speed;
	cadf->pDefenser = pchara;

	AddAnime(boost::static_pointer_cast<cAnimation>(cadf));

	return true;
}

int cAnimationManager::Anime_Damagecharacter(pcCharacter pchara, int damage, double effective)
{
	pcaDamage cada = pcaDamage(new caDamage);
	cada->pDamager = pchara;
	cada->Damage = damage;
	cada->effective = effective;

	if(pchara->Forse == CHARACTER_FORSE_FRIEND)
	{
		cada->speed = 0.6;
	}
	else if(pchara->Forse == CHARACTER_FORSE_OTHER)
	{
		cada->speed = 0.8;
	}
	else if(pchara->Forse == CHARACTER_FORSE_ENEMY)
	{
		cada->speed = 1;
	}


	AddAnime(boost::static_pointer_cast<cAnimation>(cada));
	return true;
}
int cAnimationManager::Anime_Recovercharacter(pcCharacter pchara, int recover)
{
	pcaRecover cada = pcaRecover(new caRecover);
	cada->pRecoverer = pchara;
	cada->Recover = recover;

	if(pchara->Forse == CHARACTER_FORSE_FRIEND)
	{
		cada->speed = 1;
	}
	else if(pchara->Forse == CHARACTER_FORSE_OTHER)
	{
		cada->speed = 1;
	}
	else if(pchara->Forse == CHARACTER_FORSE_ENEMY)
	{
		cada->speed = 1;
	}


	AddAnime(boost::static_pointer_cast<cAnimation>(cada));
	return true;
}

int cAnimationManager::Anime_Retirecharacter(pcCharacter pchara)
{
	pcaRetire care = pcaRetire(new caRetire);
	care->pRetirer = pchara;

	if(pchara->Forse == CHARACTER_FORSE_FRIEND)
	{
		care->speed = 0.3;
	}
	else if(pchara->Forse == CHARACTER_FORSE_OTHER)
	{
		care->speed = 0.6;
	}
	else if(pchara->Forse == CHARACTER_FORSE_ENEMY)
	{
		care->speed = 1;
	}


	AddAnime(boost::static_pointer_cast<cAnimation>(care));
	return true;
}

/*
int cAnimationManager::Anime_DisplayChange(SharedValiable_likeDouble<double>::Pointer pointer, double value)
{
	pcaDisplayChange cadi = pcaDisplayChange(new caDisplayChange);
	cadi->Set(pointer,value);

	AddAnime(boost::static_pointer_cast<cAnimation>(cadi));
	return true;
}
int cAnimationManager::Anime_DisplayChange(SharedValiable_likeInt<int>::Pointer pointer, int value)
{
	pcaDisplayChange cadi = pcaDisplayChange(new caDisplayChange);
	cadi->Set(pointer,value);

	AddAnime(boost::static_pointer_cast<cAnimation>(cadi));
	return true;
}
int cAnimationManager::Anime_DisplayChange(SharedObject<set<int>>::Pointer pointer, set<int> value)
{
	pcaDisplayChange cadi = pcaDisplayChange(new caDisplayChange);
	cadi->Set(pointer,value);

	AddAnime(boost::static_pointer_cast<cAnimation>(cadi));
	return true;
}
int cAnimationManager::Anime_DisplayChange(SharedValiable_likeVector<c4DVector>::Pointer pointer, c4DVector value)
{
	pcaDisplayChange cadi = pcaDisplayChange(new caDisplayChange);
	cadi->Set(pointer,value);

	AddAnime(boost::static_pointer_cast<cAnimation>(cadi));
	return true;
}
*/
int cAnimationManager::Anime_DisplayChange(SharedValiable_likeDouble<double>::Pointer pointer, double value)
{
	pcaDisplayChange cadi = pcaDisplayChange(new caDisplayChange);
	cadi->Set(pointer,value);

	AddAnime(boost::static_pointer_cast<cAnimation>(cadi));
	return true;
}
int cAnimationManager::Anime_DisplayChange(SharedValiable_likeInt<int>::Pointer pointer, int value)
{
	pcaDisplayChange cadi = pcaDisplayChange(new caDisplayChange);
	cadi->Set(pointer,value);

	AddAnime(boost::static_pointer_cast<cAnimation>(cadi));
	return true;
}
int cAnimationManager::Anime_DisplayChange(SafePointer<set<int>> pointer, set<int> value)
{
	pcaDisplayChange cadi = pcaDisplayChange(new caDisplayChange);
	cadi->Set(pointer,value);

	AddAnime(boost::static_pointer_cast<cAnimation>(cadi));
	return true;
}
int cAnimationManager::Anime_DisplayChange(SafePointer<c4DVector> pointer, c4DVector value)
{
	pcaDisplayChange cadi = pcaDisplayChange(new caDisplayChange);
	cadi->Set(pointer,value);

	AddAnime(boost::static_pointer_cast<cAnimation>(cadi));
	return true;
}
int cAnimationManager::Anime_DisplayChange(SafePointer<cMoney> pointer, cMoney value)
{
	pcaDisplayChange cadi = pcaDisplayChange(new caDisplayChange);
	cadi->Set(pointer,value);

	AddAnime(boost::static_pointer_cast<cAnimation>(cadi));
	return true;
}

void NonFunction(){;};
int cAnimationManager::Anime_Wait(const int placeX, const int placeY, int time)
{
	if(sg_pDungeonSystem->Map().isOnDisplay(placeX,placeY))
	{
		return Anime_Function(NonFunction,time);
	}
	return true;
}
int cAnimationManager::Anime_Wait_parallelCritical(const int placeX, const int placeY, int time)
{
	if(sg_pDungeonSystem->Map().isOnDisplay(placeX,placeY))
	{
		pcAnimation_Wait anime = pcAnimation_Wait(new cAnimation_Wait);
		anime->wait = time;
		return AddAnime_parallelCritical(anime, 0);
	}
	return true;
}

int cAnimationManager::Anime_Message(StyleString m)
{
	pcaMessage cama = pcaMessage(new caMessage);
	cama->Message = m;

	AddAnime(boost::static_pointer_cast<cAnimation>(cama));
	return true;
}
int cAnimationManager::Anime_MessageClear()
{
	pcaMessageClear camacl = pcaMessageClear(new caMessageClear);
	
	AddAnime(boost::static_pointer_cast<cAnimation>(camacl));
	return true;
}
int cAnimationManager::Anime_MessageWait(int flag_anykey)
{
	pcaMessageWait cama = pcaMessageWait(new caMessageWait);
	AddAnime(boost::static_pointer_cast<cAnimation>(cama));
	cama->anykey = flag_anykey;
	return true;
}
int cAnimationManager::Anime_primaryInterfaceWait()
{
	pcaPrimaryInterfaceWait cama = pcaPrimaryInterfaceWait(new caPrimaryInterfaceWait);
	AddAnime(boost::static_pointer_cast<cAnimation>(cama));
	return true;
}
int cAnimationManager::Anime_Throw(pcGameObject pGobj, const c4DVector& start, const c4DVector& end,int route, int drawtype, double speed)
{
	return Anime_Throw_private( pGobj, start, end, route, drawtype, speed, false);
}
int cAnimationManager::Anime_Throw_pararell(pcGameObject pGobj, const c4DVector& start, const c4DVector& end,int route, int drawtype, double speed)
{
	return Anime_Throw_private( pGobj, start, end, route, drawtype, speed, true);
}
int cAnimationManager::Anime_Throw_private(pcGameObject pGobj, const c4DVector& start, const c4DVector& end,int route, int drawtype, double speed, bool pararell)
{
	pcaThrow catr = pcaThrow(new caThrow);
	catr->start = start;
	catr->end = end;
	catr->route = route;
	catr->drawtype = drawtype;
	catr->pThrowed = pGobj;
	catr->speed = speed;

	if(sg_pDungeonSystem->pPlayerChara() == pGobj) {
		catr->cameraTracing = true;
	}
	if(pararell)
	{
		AddAnime_parallel(boost::static_pointer_cast<cAnimation>(catr));
	}
	else
	{
		AddAnime(boost::static_pointer_cast<cAnimation>(catr));
	}
	return true;
}

int cAnimationManager::Anime_PlaySE(tstring path, cCoordinate place)
{
	int ID;
	if((ID = g_GameEnv.m_SoundManager.getSoundEffectFromFile(path.c_str())) == -1)
		return false;
	pcaSoundPlay caso = pcaSoundPlay(new caSoundPlay);
	caso->play_ID = ID;
	caso->placeX = place.x;
	caso->placeY = place.y;

	AddAnime(boost::static_pointer_cast<cAnimation>(caso));
	
	return true;
}
int cAnimationManager::Anime_PlaySE(int play_ID, cCoordinate place)
{
	pcaSoundPlay caso = pcaSoundPlay(new caSoundPlay);
	caso->play_ID = play_ID;
	caso->placeX = place.x;
	caso->placeY = place.y;

	AddAnime(boost::static_pointer_cast<cAnimation>(caso));
	return true;
}
int cAnimationManager::Anime_PlayBGM(const tstring& intro, const tstring& roop)
{
	pcaSoundPlay caso = pcaSoundPlay(new caSoundPlay);
	caso->intro = intro;
	caso->roop = roop;

	AddAnime(boost::static_pointer_cast<cAnimation>(caso));
	return true;
}
int cAnimationManager::Anime_StopBGM()
{
	pcaSoundPlay caso = pcaSoundPlay(new caSoundPlay);
	caso->intro = _T("");
	caso->roop = _T("");

	AddAnime(boost::static_pointer_cast<cAnimation>(caso));
	return true;
}
int cAnimationManager::Anime_Function(void (*pFunc)(), int wait)
{
	pcaFunction cafu = pcaFunction(new caFunction);
	cafu->wait = wait;
	cafu->pFunc = pFunc;

	AddAnime(boost::static_pointer_cast<cAnimation>(cafu));
	return true;
}
int cAnimationManager::AddAnime_GotoDungeon(const tstring& DungeonID, const map<tstring,int>& privateFlags, const tstring& savefileName)
{
	pcaFadeGotoDungeon fadeGotoDungeon = pcaFadeGotoDungeon(new caFadeGotoDungeon);
	fadeGotoDungeon->DungeonID = DungeonID;
	fadeGotoDungeon->privateFlags = privateFlags;
	fadeGotoDungeon->savefileName = savefileName;

	AddAnime(boost::static_pointer_cast<cAnimation>(fadeGotoDungeon));
	return true;
}
int cAnimationManager::AddAnime_GotoDungeon(const tstring& DungeonID) {
	map<tstring,int> privateFlags;
	tstring savefileName(DungeonID + _T("_temp"));
	return AddAnime_GotoDungeon(DungeonID,privateFlags,savefileName);
}
int cAnimationManager::AddAnime_ContinueDungeon(pcSaveQuest continueQuest) {
	pcaFadeContinueDungeon fadeContinueDungeon = pcaFadeContinueDungeon(new caFadeContinueDungeon);
	fadeContinueDungeon->continueQuest = continueQuest;

	AddAnime(boost::static_pointer_cast<cAnimation>(fadeContinueDungeon));
	return true;
}