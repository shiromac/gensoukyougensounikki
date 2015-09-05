
#include "stdafx.h"
#include "cHomeResultWindow.h"

cHomeResultWindow::cHomeResultWindow()
{
	state_ = kInited;
	counter_ = 0;
}
cHomeResultWindow::~cHomeResultWindow()
{

}

void cHomeResultWindow::Init(IDirect3DDevice9 *pDev,
						const int levelBefore,
						const int experienceBefore,
						const int experienceAddition,
						const ExperienceTabelFunction experienceTableFunction,
						const double yakuBefore,
						const double yakuAddition
		)
{
	experienceBefore_ = experienceBefore;
	experienceAddition_ = experienceAddition;
	experienceTableFunction_ = experienceTableFunction;
	yakuBefore_ = yakuBefore;
	yakuAddition_ = yakuAddition;

	int letterXnum = 24;
	int letterYnum = 12;

	retain_experience_ = experienceAddition;
	retain_yaku_ = yakuAddition;

	now_experience_ = experienceBefore_;
	now_yaku_ = yakuBefore_;

	InitBasic(pDev,letterXnum,letterYnum);
	CenterX = SCREEN_X/2;
	CenterY = SCREEN_Y/2;

	now_level_ = first_level_ = levelBefore;

	expGage.formRight(Right() - EDGEWIDTH);
	expGage.formLeft(Left() + EDGEWIDTH);
	expGage.Height = GWFONTSIZE;
	expGage.setTop(Top()+linetop(5));
	expGage.forwardColor.ARGB(255,20,100,255);
	expGage.backColor.ARGB(64,20,20,20);
	expGage.edge_width = 2;
	expGage.edgeColor.ARGB(128,255,255,255);
}

int cHomeResultWindow::process(IDirect3DDevice9 *pDev)
{
	if(state_ == kInited)
	{
		if(g_pPlayerInput()->decision().justOn || g_pPlayerInput()->cancel().justOn)
		{
			if(g_pPlayerInput()->decision().justOn)
			{
				playsound_decide();
			}
			state_ = kIncreasePoint;
			counter_ = 0;
		}

	}
	else if(state_ == kIncreasePoint)
	{
		counter_ += 1.0/120.0;

		now_experience_ = experienceBefore_ + counter_*(experienceAddition_);
		retain_experience_ = (1-counter_)*(experienceAddition_);
		now_level_ = first_level_;
		for(;;)
		{
			if(now_experience_ >= experienceTableFunction_(now_level_))
			{
				//レベルアップ
				now_experience_ -= experienceTableFunction_(now_level_);
				now_level_ += 1;
				continue;
			}
			break;
		}
		
		now_yaku_ = yakuBefore_ + counter_*(yakuAddition_);
		retain_yaku_ = (1-counter_)*(yakuAddition_);

		if(counter_ >= 1.0 || g_pPlayerInput()->decision().justOn || g_pPlayerInput()->cancel().justOn)
		{
			if(g_pPlayerInput()->decision().justOn)
			{
				playsound_decide();
			}

			retain_experience_ = 0;
			retain_yaku_ = 0;

			//now_experience_ = experienceBefore_ + experienceAddition_;
			now_yaku_ = yakuBefore_ + yakuAddition_;

			state_ = kFinished;
		}
	
	}
	else if(state_ == kFinished)
	{
	
		if(g_pPlayerInput()->decision().justOn || g_pPlayerInput()->cancel().justOn)
		{
			if(g_pPlayerInput()->decision().justOn)
			{
				playsound_decide();
			}

			sg_pDungeonSystem->メニューを一枚閉じる();
		}
	
	}
	else
	{
		assert(!"cHomeResultWindow::process");
	}


	return true;
}

int cHomeResultWindow::Draw(IDirect3DDevice9 *pDev)
{
	
	WindowDraw(pDev);
	Text().clear();
	map<tstring, StyleString> valiable;
	valiable[_T("now_yaku")].addstring( now_yaku_,_T("%6.0f") );
	valiable[_T("retain_yaku")].addstring( retain_yaku_,_T("%6.0f") );
	valiable[_T("now_experience")].addstring( now_experience_,_T("%6d") );
	valiable[_T("retain_experience")].addstring( retain_experience_,_T("%6d") );
	valiable[_T("next_experience")].addstring( experienceTableFunction_(now_level_),_T("%6d") );
	valiable[_T("playerRank")].addstring( now_level_,_T("%3d") );
	g_Lang(_T("HomeResultText"),Text(),valiable);
	
	preRendar = false;



	expGage.MaxValue = experienceTableFunction_(now_level_);
	expGage.Value = now_experience_;

	expGage.Draw(pDev);

	TextDraw(pDev);

	return true;
}