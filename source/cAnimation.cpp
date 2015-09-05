#include "stdafx.h"

#include "cAnimation.h"

cAnimation::cAnimation()
{
	End = false;
	LayerLevel = 0;
}

cAnimation::~cAnimation()
{
}



//------------------------------------------
//アニメチップ
//------------------------------------------
cAnimationChipManager_chip::cAnimationChipManager_chip()
{
	End = false;
	FrameCount = 0;
	hidingTime = 0;
	AbsolutePlaceFlag = 0;
}

cAnimationChipManager_chip::~cAnimationChipManager_chip()
{
}

int cAnimationChipManager_chip::process(IDirect3DDevice9 *pDev)
{
	if(hidingTime <= 0)
	{
		DrowingObjectFunc();
		FrameCount++;
	}
	else
	{
		hidingTime--;
	}
	return true;
}
int cAnimationChipManager_chip::Init(pcAnimation me)
{
	cAnimationChipManager::Init(me);
	
	if(nextchip() != NULL)
	{
		nextchip()->Init(nextchip());
		nextchip()->parentManager_ = parentManager_;
	}
	return true;
}
/*
void cAnimationChipManager_chip::copy(const cAnimationChipManager_chip & chip)
{
	DO = chip.DO;
	FrameCount = chip.FrameCount;
	hidingTime = chip.hidingTime;
}
*/


int cAnimationChipManager_chip::addNextchip(pcAnimationChipManager_chip animechip)
{
	nextchip_ = animechip;
	nextchip_->parentManager_ = parentManager_;
	if(me() != NULL)
	{//自分がいにっとされてる
		nextchip()->Init(nextchip());
	}
	return true;
}


//------------------------------------------
//アニメチップマネージャー
//------------------------------------------
cAnimationChipManager::cAnimationChipManager()
{
	End = false;
}

cAnimationChipManager::~cAnimationChipManager()
{
}
int cAnimationChipManager::chipprocess(IDirect3DDevice9 *pDev)
{
	list<pcAnimationChipManager_chip>::iterator itr = chiplist.begin();

	for(;itr != chiplist.end();)
	{
		(*itr)->process(pDev);
		if((*itr)->End)
		{
			if((*itr)->nextchip() != NULL) addAnimechip((*itr)->nextchip());
			itr = chiplist.erase(itr);
			continue;
		}
			
		itr++;
	}

	return true;
}

int cAnimationChipManager::chipDraw(IDirect3DDevice9 *pDev)
{
	list<pcAnimationChipManager_chip>::iterator itr = chiplist.begin();

	for(;itr != chiplist.end();itr++)
	{
		(*itr)->Draw(pDev);
	}

	return true;
}

int cAnimationChipManager::addAnimechip(pcAnimationChipManager_chip animechip)
{
	
	chiplist.push_back(animechip);
	pcAnimationChipManager me_acm = boost::dynamic_pointer_cast<cAnimationChipManager>(me());
	if(me_acm != NULL)
	{//自分がinitされてる
		animechip->Init(animechip);
		animechip->parentManager_ = wpcAnimationChipManager(me_acm);
	}
	
	return true;
}
int cAnimationChipManager::Init(pcAnimation me)
{
	wme_ = wpcAnimation(me);
	pcAnimationChipManager me_acm = boost::dynamic_pointer_cast<cAnimationChipManager>(me);
	
	list<pcAnimationChipManager_chip>::iterator itr = chiplist.begin();
	for(;itr != chiplist.end(); itr++)
	{
		(*itr)->Init(*itr);
		(*itr)->parentManager_ = wpcAnimationChipManager(me_acm);

	}

	firstmapforcus.x = sg_pDungeonSystem->Map().ForcusX();
	firstmapforcus.y = sg_pDungeonSystem->Map().ForcusY();
	return true;
}
int cAnimationChipManager::clearAnimechip()
{
	chiplist.clear();
	return true;
}
bool cAnimationChipManager::chipEmpty()
{
	return chiplist.empty();
}
c4DVector cAnimationChipManager::absoluteRevise()
{

	c4DVector p,temp = firstmapforcus;
	p += sg_pDungeonSystem->Map().visibleplace(temp);
	temp.x = sg_pDungeonSystem->Map().visibleForcusX();
	temp.y = sg_pDungeonSystem->Map().visibleForcusY();

	p -= sg_pDungeonSystem->Map().visibleplace(temp);
	return p;
}
