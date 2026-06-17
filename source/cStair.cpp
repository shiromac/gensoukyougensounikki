#include "cStair.h"

#include "cDungeonSystem.h"

#include "cFloor.h"


#define ITEM_CAPTION_MAX_LENGTH (40.0)

cStair::cStair(void)
{
	aspect = STAIR_ASPECT_UP;
}

cStair::~cStair(void)
{
}
void cStair::settingInit()
{
	cDroping::settingInit();
	if(sg_pDungeonSystem->pDungeon()->dungeonDirection() == cDungeon::DungeonDirection_Up) {
		aspect = STAIR_ASPECT_UP;
	}
	else if(sg_pDungeonSystem->pDungeon()->dungeonDirection() == cDungeon::DungeonDirection_Down){
		aspect = STAIR_ASPECT_DOWN;
	}
	else {
		aspect = STAIR_ASPECT_UP;
	}
}

StyleString cStair::FullName()
{
	return FullNameBase();
}

StyleString cStair::FullNameSub()
{
	return setStyle(_T("Stairs"));
}

StyleString cStair::ShortName()
{
	return ShortNameBase();
}

StyleString cStair::ShortNameSub()
{
	return setStyle(_T("Stairs"));
}

/*
StyleString cStair::shortExplanation()
{
	return _T("");
}

StyleString cStair::longExplanation()
{
	return _T("");
}
*/


int cStair::GetmenuCaption(vector<tstring> &CaptionList)
{
	if(aspect == STAIR_ASPECT_DOWN)
	{//はじめ
		CaptionList.push_back(_T("下りる"));
	}
	else if(aspect == STAIR_ASPECT_UP)
	{//はじめ
		CaptionList.push_back(_T("上る"));
	}


	if(!sg_pDungeonSystem->拠点フラグ() && sg_pDungeonSystem->pDungeon()->MaxFloor() != sg_pDungeonSystem->FloorLevel()) CaptionList.push_back(_T("一旦中断する"));
	

	CaptionList.push_back(_T("やめる"));


	return true;
}

int cStair::乗る()
{
	if(乗りキャラ() == sg_pDungeonSystem->pPlayerChara())
	{
		sg_pDungeonSystem->AnimationManager().
			Anime_PlaySE(SOUND_SELECTDECIDE,現在地形()->place);
	
		Action(sg_pDungeonSystem->pDevice_D3D);
	}
	return true;
}

/*
int cStair::昇降する(cRenderDevice *pDev)
{
	sg_pDungeonSystem->フロア更新要請();
	return true;
}
*/

void cStair::CutIn(タイミング timing, cValiableField& valiable)
{

	if(timing == インプット直前_タイミング)
	{
		if(踏みフラグ_)
		{
			踏みフラグ_ = 0;
			乗る();
		}
	
	}
}

