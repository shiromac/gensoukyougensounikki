#include "stdafx.h"

#include "cResultWindow.h"

#include "Gameobjects.h"

#include <time.h>

#define PAGETEXPATH _T("skin\\page.png")
//#define PAGETEXSIZE 32

#define PAGECOUNTNUM 70

cResultWindow::cResultWindow(void)
{

}

cResultWindow::~cResultWindow(void)
{

}
void cResultWindow::Init(IDirect3DDevice9 *pDev, pcSaveResult pResult)
{
	Init(pDev, *pResult);
}
void cResultWindow::Init(IDirect3DDevice9 *pDev, cSaveResult& Result)
{
	int letterXnum = 24;
	int letterYnum = 12;

	Result_ = Result;
	
	InitBasic(pDev,letterXnum,letterYnum);
	vText.resize(3);


	//if(Result_)
	{
		m_SpriteText.SetAntiAlias(1);
		m_SpriteText.SetColor( D3DCOLOR_ARGB(255,255,255,255) );
		m_SpriteText.SetReturnWidth(1024);//自動returnしない
		m_SpriteText.SetWeight(GWFONTWEIGHT);
		m_SpriteText.BeginText( g_fontstyle().c_str(), GWFONTSIZE );

		int power;
		pcEquipment pcEqu;
		if(Result_.lastequipItem_.size() >= 1)
			pcEqu = boost::dynamic_pointer_cast<cEquipment>(Result_.lastequipItem_[0]);
		power = 0;
		if(pcEqu != NULL) power = pcEqu->武器力最終値();

		map<tstring, StyleString> valiable;
		valiable[_T("ArmP")].addstring(power,_T("%4d"));

		if(pcEqu != NULL)
		{
			valiable[_T("ArmName")].addstring(pcEqu->FullName());
			valiable[_T("ArmMaster")].addstring(pcEqu->熟練度(),_T("%3d"));
		}
		else
		{
			valiable[_T("ArmName")].addstring(_T(""));
			valiable[_T("ArmMaster")].addstring( 0 ,_T("%3d"));
		}
		m_SpriteText.FitTextWidth(valiable[_T("ArmName")],letterXlength*GWFONTSIZE);

		pcEqu = pcEquipment((cEquipment*)NULL);
		if(Result_.lastequipItem_.size() >= 2)
			pcEqu = boost::dynamic_pointer_cast<cEquipment>(Result_.lastequipItem_[1]);
		power = 0;
		if(pcEqu != NULL) power = pcEqu->防具力最終値();
		valiable[_T("ShdP")].addstring(power,_T("%4d"));
		
		if(pcEqu != NULL)
		{
			valiable[_T("ShdName")].addstring(pcEqu->FullName());
			valiable[_T("ShdMaster")].addstring(pcEqu->熟練度(),_T("%3d"));
		}
		else
		{
			valiable[_T("ShdName")].addstring(_T(""));
			valiable[_T("ShdMaster")].addstring( 0 ,_T("%3d"));
		}
		m_SpriteText.FitTextWidth(valiable[_T("ShdName")],letterXlength*GWFONTSIZE);

		valiable[_T("StmPMAX")].addstring(Result_.LastMSP_,_T("%3d"));

		valiable[_T("ExpP")].addstring(Result_.SumExp_,_T("%8d"));

		valiable[_T("DefeatNum")].addstring(Result_.SumdefeatNum_,_T("%5d"));

		valiable[_T("Turn")].addstring(Result_.Sumturn_,_T("%6d"));
		valiable[_T("Score")].addstring(Result_.score_,_T("%9d"));
		valiable[_T("LV")].addstring(Result_.LastLV_,_T("%3d"));
		valiable[_T("MHP")].addstring(Result_.LastMHP_,_T("%3d"));

		double sumtime = Result_.SumFrame_/60;
		valiable[_T("GameTimeSec")].addstring(((int)sumtime)%60,_T("%2d"));
		valiable[_T("GameTimeMin")].addstring((((int)sumtime)/60)%60,_T("%2d"));
		valiable[_T("GameTimeHour")].addstring(((int)sumtime)/60/60,_T("%3d"));
		
		sumtime = Result_.SumFramerealtime_/60;
		valiable[_T("RealTimeSec")].addstring(((int)sumtime)%60,_T("%2d"));
		valiable[_T("RealTimeMin")].addstring((((int)sumtime)/60)%60,_T("%2d"));
		valiable[_T("RealTimeHour")].addstring(((int)sumtime)/60/60,_T("%3d"));

        struct tm *timeObject;
        timeObject = localtime(&Result_.Date_);
		valiable[_T("DateSec")].addstring(timeObject->tm_sec,_T("%02d"));
		valiable[_T("DateMin")].addstring(timeObject->tm_min,_T("%02d"));
		valiable[_T("DateHour")].addstring(timeObject->tm_hour,_T("%2d"));
		valiable[_T("DateDay")].addstring(timeObject->tm_mday,_T("%2d"));
		valiable[_T("DateMon")].addstring(timeObject->tm_mon + 1,_T("%2d"));
		valiable[_T("DateYear")].addstring(timeObject->tm_year + 1900,_T("%4d"));

		cMoney mny;
		mny.value = Result_.money_;
		valiable[_T("Money")].addstring(mny.caption());

		valiable[_T("PlayerSigns")].addstring(Result_.PlayerSign_);

		valiable[_T("DropFrame")].addstring((Result_.SumFramerealtime_*100.0 - Result_.SumFrame_*100.0
			)/Result_.SumFrame_,_T("%3.2f"));

		valiable[_T("FloorNum")].addstring(Result_.lastfloor_,_T("%2d"));
		valiable[_T("Dungeon")].addstring(Result_.dungeonName_);
		valiable[_T("Chara")].addstring(Result_.CharaName_);

		valiable[_T("XXX")].addstring(Result_.BadEndNum_);

		valiable[_T("Ver")].addstring(Result_.Version_);






		StyleString sstr;
		int itemnum = 0;
		int firstmoney = Result_.FirstConditionOfMoney_;
		pcCharacter pchara = Result_.pFirstConditionOfPlayer_;
		if(pchara != NULL)
		{
			itemnum = sg_pDungeonSystem->ItemSize(pchara->holdItem);
		}
		sstr.clear();
		if(itemnum == 0 && firstmoney == 0)
		{
			g_Lang(_T("持ち込み無し文節"),sstr,valiable);
		}
		else
		{
			g_Lang(_T("持ち込み有り文節"),sstr,valiable);
		}
		valiable[_T("FirstItemSentence")].addstring(sstr);

		valiable[_T("FirstItemNum")].addstring(itemnum,_T("%3d"));
		mny.value = firstmoney;
		valiable[_T("FirstMoney")].addstring(mny.caption());

		sstr.clear();
		if(Result_.clearflag_ == cDungeonSystem::GAMECLEAR_CLEARFLAG)
		{
			g_Lang(_T("リザルトクリアメッセージ"),sstr,valiable);
		}
		else if(Result_.clearflag_ == cDungeonSystem::GAMECLEAR_PULLOUTFLAG)
		{
			g_Lang(_T("リザルト引き揚げメッセージ"),sstr,valiable);
		}
		else
		{
			g_Lang(_T("リザルト失敗メッセージ"),sstr,valiable);
		}
		valiable[_T("ClearMassage")].addstring(sstr);


		int i;
		for(i=0;i<10;i++)
		{
			StyleString sstr;
			sstr = _T("LastMassage");
			sstr.addstring(i+1);

			if(i < Result_.lastmessage_.size())
			{
				StyleString sstr2 = Result_.lastmessage_[i].straightString();
				sstr2.addstring(_T("\n"));
				valiable[sstr.c_str()].addstring(sstr2);
				m_SpriteText.FitTextWidth(valiable[sstr.c_str()],letterXlength*GWFONTSIZE);
			}
			else
			{
				valiable[sstr.c_str()].addstring(_T(""));
			}
		}

		g_Lang(_T("リザルトウィンドウテキスト1"),Text(0),valiable);
		g_Lang(_T("リザルトウィンドウテキスト2"),Text(1),valiable);
		g_Lang(_T("リザルトウィンドウテキスト3"),Text(2),valiable);

		m_SpriteText.EndText();
	}
}
void cResultWindow::Init(IDirect3DDevice9 *pDev, int letterXnum, int letterYnum)
{
	//Init(pDev, pcSaveResult((cSaveResult*)NULL));
}

int cResultWindow::process(IDirect3DDevice9 *pDev)
{
	cGameWindow::process(pDev);


	int tempP = pageIndex;
	pageIndex += (g_pPlayerInput()->decision().justOn);
	if(tempP != pageIndex)
	{
		playsound_decide();
		preRendar = false;
	}
	if(pageIndex < 0) pageIndex = pageIndexMaxF() - 1;
	if(pageIndex >= pageIndexMaxF())
	{	
		pageIndex = 0;
		sg_pDungeonSystem->メニューを一枚閉じる();
	}

	return true;
}
