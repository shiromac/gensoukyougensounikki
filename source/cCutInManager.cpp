#include "stdafx.h"
#include "cCutInManager.h"
#include "cEventManager.h"

#include "timingEnum.h"
#include "cEvent.h"

#include <boost/pointer_cast.hpp>

cCutInManager::cCutInManager(void)
{
	count_ = 0;
}

cCutInManager::~cCutInManager(void)
{
}

/*
double cCutInManager::CutInValue(pcCharacter pchara,int timing)
{
	if(pchara == NULL) return 0;
	return pchara->CutInValue(timing);
}

double cCutInManager::CutInValue(pcDroping pdrop,int timing)
{
	if(pdrop == NULL) return 0;
	return pdrop->CutInValue(timing);
}

double cCutInManager::CutInValue(pcLandform pland,int timing)
{
	if(pland == NULL) return 0;
	//return pland->CutInValue(timing);
	return 0;
}

int cCutInManager::CutInEvent(pcCharacter pchara,int timing)
{
	if(pchara == NULL) return 0;
	return pchara->CutInEvent(timing);
}

int cCutInManager::CutInEvent(pcDroping pdrop,int timing)
{
	if(pdrop == NULL) return 0;
	return pdrop->CutInEvent(timing);
}

int cCutInManager::CutInEvent(pcLandform pland,int timing)
{
	if(pland == NULL) return 0;
	//return pland->CutInEvent(timing);
	return 0;
}


void cCutInManager::CutInAttack(pcCharacter pchara,int timing,cAttackinformation & attinfo)
{
	if(pchara == NULL) return;
	pchara->CutInAttack(timing,attinfo);
}
void cCutInManager::CutInAttack(pcDroping pdrop,int timing,cAttackinformation & attinfo)
{
	if(pdrop == NULL) return;
	pdrop->CutInAttack(timing,attinfo);
}
void cCutInManager::CutInAttack(pcLandform pland,int timing,cAttackinformation & attinfo)
{
	if(pland == NULL) return;
	//pland->CutInAttack(timing,attinfo);
}

*/
void cCutInManager::CutInForAllObject(タイミング timing, cValiableField& valiable)
{
	int i,k;
	vector<pcCharacter> vchara = sg_pDungeonSystem->CharaList();
	vector<pcDroping> vdrop;
	vector<pcEvent> vevent;
	for(i=0;i<vchara.size();i++)
	{
		CutIn_pri(vchara[i], timing, valiable);


		if(vchara[i] == NULL) continue;

		vdrop = vchara[i]->holdItem;
		for(k=0;k<vdrop.size();k++)
		{
			if(vdrop[k] == NULL) continue;
			CutIn_pri(vdrop[k],timing,valiable);
		}
	}

	vdrop = sg_pDungeonSystem->DropList();
	for(k=0;k<vdrop.size();k++)
	{
		if(vdrop[k] == NULL) continue;
		CutIn_pri(vdrop[k],timing,valiable);
	}
	vevent = sg_pDungeonSystem->EventManager().eventList();
	for(k=0;k<vevent.size();k++)
	{
		if(vevent[k] == NULL) continue;
		CutIn_pri(vevent[k],timing,valiable);
	}

	for(i=0;i<vchara.size();i++)
	{
		erasememCutIn(vchara[i],timing);
	}
	for(k=0;k<vdrop.size();k++)
	{
		erasememCutIn(vdrop[k],timing);
	}
	for(k=0;k<vevent.size();k++)
	{
		erasememCutIn(vevent[k],timing);
	}
	//CutInForAllLand(timing, valiable);
	CutIn(timing, valiable);

}
void cCutInManager::CutInForAllChara(タイミング timing, cValiableField& valiable)
{
	int i,k;
	vector<pcCharacter> vchara = sg_pDungeonSystem->CharaList();
	vector<pcDroping> vdrop;
	for(i=0;i<vchara.size();i++)
	{

		if(vchara[i] == NULL) continue;

		CutIn_pri(vchara[i], timing, valiable);


		vdrop = vchara[i]->holdItem;
		for(k=0;k<vdrop.size();k++)
		{
			if(vdrop[k] == NULL) continue;
			CutIn_pri(vdrop[k],timing,valiable);
		}
	}

	for(i=0;i<vchara.size();i++)
	{
		erasememCutIn(vchara[i],timing);
	
		vdrop = vchara[i]->holdItem;
		for(k=0;k<vdrop.size();k++)
		{
			erasememCutIn(vdrop[k],timing);
		}
	}
	
}
void cCutInManager::CutInForAllDrop(タイミング timing, cValiableField& valiable)
{
	int i;
	vector<pcDroping> vdrop = sg_pDungeonSystem->DropList();
	for(i=0;i<vdrop.size();i++)
	{
		CutIn_pri(vdrop[i], timing, valiable);
	}

	for(i=0;i<vdrop.size();i++)
	{
		erasememCutIn(vdrop[i],timing);
	}
}

void cCutInManager::CutInForAllEvent(タイミング timing, cValiableField& valiable)
{
	int i;
	vector<pcEvent> vevent = sg_pDungeonSystem->EventManager().eventList();
	for(i=0;i<vevent.size();i++)
	{
		CutIn_pri(vevent[i], timing, valiable);
	}

	for(i=0;i<vevent.size();i++)
	{
		erasememCutIn(vevent[i],timing);
	}
}
/*
void cCutInManager::CutInForAllLand(タイミング timing, cValiableField& valiable)
{

	return;
}
*/
	
void cCutInManager::CutIn(pcCharacter pchara,タイミング timing, cValiableField& valiable)
{
	if(pchara == NULL) return;
	
	CutIn_pri(pchara,timing,valiable);

	int i;
	vector<pcDroping> vdrop = pchara->holdItem;
	for(i=0;i<vdrop.size();i++)
	{
		CutIn_pri(vdrop[i],timing,valiable);
	}

	for(i=0;i<vdrop.size();i++)
	{
		erasememCutIn(vdrop[i],timing);
	}
	erasememCutIn(pchara,timing);
}
void cCutInManager::CutIn(pcDroping pdrop,タイミング timing, cValiableField& valiable)
{
	if(pdrop == NULL) return;

	CutIn_pri(pdrop,timing,valiable);

	erasememCutIn(pdrop,timing);
}
void cCutInManager::CutIn(pcLandform pland,タイミング timing, cValiableField& valiable)
{
	if(pland == NULL) return;
	CutIn_pri(pland,timing,valiable);

	erasememCutIn(pland,timing);
}
void cCutInManager::CutIn(pcEvent pevent,タイミング timing, cValiableField& valiable)
{
	if(pevent == NULL) return;
	CutIn_pri(pevent,timing,valiable);

	erasememCutIn(pevent,timing);
}
void cCutInManager::CutIn_pri(pcCharacter pchara,タイミング timing, cValiableField& valiable)
{

	if(ExistCutIn(pchara,timing)) return;
	if(count_ >= 10000) return;//無限ループ対策
	memCutIn(pchara,timing);

	pchara->CutIn(timing,valiable);


}
void cCutInManager::CutIn_pri(pcDroping pdrop,タイミング timing, cValiableField& valiable)
{

	if(ExistCutIn(pdrop,timing)) return;
	if(count_ >= 10000) return;//無限ループ対策
	memCutIn(pdrop,timing);

	pdrop->CutIn(timing,valiable);
}
void cCutInManager::CutIn_pri(pcLandform pland,タイミング timing, cValiableField& valiable)
{

	if(ExistCutIn(pland,timing)) return;
	if(count_ >= 10000) return;//無限ループ対策
	memCutIn(pland,timing);
	//CutIn(pland->pOnChar,timing,valiable);
	//CutIn(pland->pOnDrop,timing,valiable);

	pland->CutIn(timing,valiable);
}
void cCutInManager::CutIn_pri(pcEvent pevent,タイミング timing, cValiableField& valiable)
{

	if(ExistCutIn(pevent,timing)) return;
	if(count_ >= 10000) return;//無限ループ対策
	memCutIn(pevent,timing);

	pevent->CutIn(timing,valiable);
}
void cCutInManager::CutIn(タイミング timing, cValiableField& valiable)
{
	/*能動的プログラムに変更
	if(timing == アクション終了_タイミング)
	{
		clearMemCutIn();
	}
	*/
}


void cCutInManager::memCutIn(pcDroping pointer, タイミング timing)
{
	++count_;
	droppointerpool_.insert(pair<pcDroping,タイミング>(pointer,timing));
}
void cCutInManager::memCutIn(pcCharacter pointer, タイミング timing)
{
	++count_;
	charapointerpool_.insert(pair<pcCharacter,タイミング>(pointer,timing));
}
void cCutInManager::memCutIn(pcLandform pointer, タイミング timing)
{
	++count_;
	landpointerpool_.insert(pair<pcLandform,タイミング>(pointer,timing));
}
void cCutInManager::memCutIn(pcEvent pointer, タイミング timing)
{
	++count_;
	eventpointerpool_.insert(pair<pcEvent,タイミング>(pointer,timing));
}

void cCutInManager::erasememCutIn(pcDroping pointer, タイミング timing)
{
	--count_;
	droppointerpool_.erase(pair<pcDroping,タイミング>(pointer,timing));
}
void cCutInManager::erasememCutIn(pcCharacter pointer, タイミング timing)
{
	--count_;
	charapointerpool_.erase(pair<pcCharacter,タイミング>(pointer,timing));
}
void cCutInManager::erasememCutIn(pcLandform pointer, タイミング timing)
{
	--count_;
	landpointerpool_.erase(pair<pcLandform,タイミング>(pointer,timing));
}
void cCutInManager::erasememCutIn(pcEvent pointer, タイミング timing)
{
	--count_;
	eventpointerpool_.erase(pair<pcEvent,タイミング>(pointer,timing));
}

bool cCutInManager::ExistCutIn(pcDroping pointer, タイミング timing)
{
	return (droppointerpool_.end() != droppointerpool_.find(pair<pcDroping,タイミング>(pointer,timing)));
}
bool cCutInManager::ExistCutIn(pcCharacter pointer, タイミング timing)
{
	return (charapointerpool_.end() != charapointerpool_.find(pair<pcCharacter,タイミング>(pointer,timing)));
}
bool cCutInManager::ExistCutIn(pcLandform pointer, タイミング timing)
{
	return (landpointerpool_.end() != landpointerpool_.find(pair<pcLandform,タイミング>(pointer,timing)));
}
bool cCutInManager::ExistCutIn(pcEvent pointer, タイミング timing)
{
	return (eventpointerpool_.end() != eventpointerpool_.find(pair<pcEvent,タイミング>(pointer,timing)));
}

void cCutInManager::clearMemCutIn()
{
	count_ = 0;
	droppointerpool_.clear();
	charapointerpool_.clear();
	landpointerpool_.clear();
	eventpointerpool_.clear();
}