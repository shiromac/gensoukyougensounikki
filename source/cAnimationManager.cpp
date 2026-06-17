#include "stdafx.h"

#include "caFunction.h"
#include "cAnimationManager.h"
#include <boost/pointer_cast.hpp>


cAnimationManager::cAnimationManager(void)
{
	Enpty = true;
}

cAnimationManager::~cAnimationManager(void)
{

	int i;
	for(i=0;i<MovePool.size();i++)
	{
		MovePool[i]->clear();
	}

	MovePool.clear();
}

int cAnimationManager::Init(cRenderDevice *pDev)
{
	int i;

	MovePool.clear();


	for(i=MovePool.size();i<AM_MOVEPOOLHASHNUM;i++)
	{
		MovePool.push_back(plistps_Move(new list<ps_Move>));
	}

	return true;
}

//trueを返した時プロセス無し
int cAnimationManager::process(cRenderDevice *pDev)
{

	if(!MoveAnimeprocess(pDev)) return false;

	if(!AttackAnimeprocess(pDev)) return false;

	if(ParallelCriticalAnimeList.size() > 0) return false;
	return true;
}
void cAnimationManager::process_parallel(cRenderDevice *pDev)
{
	//リアルタイムアニメ
	{
		list<pcAnimation>::iterator itr = ParallelAnimeList.begin();
		for(;itr != ParallelAnimeList.end();)
		{
			(*itr)->process(pDev);
			if((*itr)->End)
			{
				itr = ParallelAnimeList.erase(itr);
				continue;
			}
			itr++;
		}
	}

	{
		list<pcAnimation>::iterator itr = ParallelCriticalAnimeList.begin();
		for(;itr != ParallelCriticalAnimeList.end();)
		{
			(*itr)->process(pDev);
			if((*itr)->End)
			{
				itr = ParallelCriticalAnimeList.erase(itr);
				continue;
			}
			itr++;
		}
	}
}


//trueを返した時プロセス無し
int cAnimationManager::MoveAnimeprocess(cRenderDevice *pDev)
{
	int i,endcount = 0;
	int processcount = 0;

	for(;;)
	{
		endcount = 0;
		processcount = 0;

		for(i=0;i<MoveAnimeList.size();i++)
		{
			if( MoveAnimeList[i]->isOnScreenNow())
			{
				processcount++;
			}
			MoveAnimeList[i]->process(pDev);
			if(MoveAnimeList[i]->End) endcount++;
			

		}

		if(processcount != 0 || endcount >= MoveAnimeList.size())
		{//見える移動があった || 全部終わり
			break;
		}

	}
	
	if(MoveAnimeList.size())
	{
		sg_pDungeonSystem->Map().setVisibleForcusX(sg_pDungeonSystem->pPlayerChara()->visibleplace.x);
		sg_pDungeonSystem->Map().setVisibleForcusY(sg_pDungeonSystem->pPlayerChara()->visibleplace.y);
	}
	else
	{
		/*
		sg_pDungeonSystem->Map().visibleForcusX() = sg_pDungeonSystem->Map().visibleForcusX()*2 + sg_pDungeonSystem->pPlayerChara()->visibleplace.x;
		sg_pDungeonSystem->Map().visibleForcusY() = sg_pDungeonSystem->Map().visibleForcusY()*2 + sg_pDungeonSystem->pPlayerChara()->visibleplace.y;
		sg_pDungeonSystem->Map().visibleForcusX() /= 3;
		sg_pDungeonSystem->Map().visibleForcusY() /= 3;
		*/
	}

	if(endcount >= MoveAnimeList.size() && endcount)
	{//全部終了
		//sg_pDungeonSystem->refreshCharaDropView();
		MoveAnimeList.clear();
	}

	if(MoveAnimeList.size() == 0) return true;
	else return false;

}

int cAnimationManager::AttackAnimeprocess(cRenderDevice *pDev)
{

	int Layer[LAYERLEVEL_LAYERMAXNUM] = {0};
	
	while(1)
	{
	
		int i = 0;
		while(1)
		{
			//空っぽ
			if(AttackAnimeList.empty()) return true;
			
			//もう次ない
			if(i >= AttackAnimeList.size()) break;




			
			if(!AttackAnimeList[i]->End)
			{//終わってない
				for(;;)
				{
					AttackAnimeList[i]->process(pDev);
					if( AttackAnimeList[i]->isOnScreenNow() || AttackAnimeList[i]->End) break;
				}

				if(!(AttackAnimeList[i]->End))
				{//やっぱり終わってない
				
					if(AttackAnimeList[i]->LayerLevel < LAYERLEVEL_LAYERMAXNUM)
						Layer[AttackAnimeList[i]->LayerLevel] = true;


					if(i+1 < AttackAnimeList.size())
					{//つづきある
						if(AttackAnimeList[i+1]->LayerLevel < LAYERLEVEL_LAYERMAXNUM)
						{
							if(Layer[AttackAnimeList[i+1]->LayerLevel])
							{//次のがレイヤ同じ
								break;
							}
						}
					}
				}

			}


			i++;
		}


		int nextflag = 0;
		while(1)
		{
			if(AttackAnimeList.size() == 0) return true;

			if(AttackAnimeList[0]->End)
			{
				if(AttackAnimeList[0]->LayerLevel < LAYERLEVEL_LAYERMAXNUM)
					Layer[AttackAnimeList[0]->LayerLevel] = false;
				AttackAnimeList.pop_front();
				nextflag = 1;
			}
			else
			{
				break;
			}
		}

		if(!nextflag)
		{
			break;
		}
		else
		{
			continue;
		}
	}

	//ダッシュ強制終了
	sg_pDungeonSystem->DashSwitch() = 0;

	return false;
}


int cAnimationManager::StrongInvocation(cRenderDevice *pDev)
{

	int i,j,k;

	for(i=0;i<AM_MOVEPOOLHASHNUM;i++)
	{
		if(MovePool[i]->size()) break;
	}
	if(i == AM_MOVEPOOLHASHNUM)
	{//そもそも動くキャラがいない
		Enpty = true;
		return true;
	}

	//動かないキャラに対し透明度変更の処理を施すために、
	//仮の移動アニメーション（あし踏み）を追加する。
	//（あし踏み）を追加するキャラクターは透明度が変更されるキャラのみ
	for(i=0;i<sg_pDungeonSystem->visibleCharaList().size();i++)
	{//i番目のキャラに対し
		pcCharacter pchara = sg_pDungeonSystem->visibleCharaList()[i];		
		j = g_Hash((long)(pchara.get()),AM_MOVEPOOLHASHNUM);
		list<ps_Move>::iterator itr = MovePool[j]->begin();	

		while(itr != MovePool[j]->end())
		{
			if((*itr)->pchara == pchara)
			{//検索ヒット
				break;
			}
			itr++;
		}
		if(itr == MovePool[j]->end())
		{//検索ヒットしなかった
			if(
				(pchara->opaque > 0.5) ==//これは前の待機時に表示されていたかどうかが入る
				!sg_pDungeonSystem->キャラクター目視可(pchara) && !pchara->死亡())
			{//透明度が変更されるキャラ
				AddMove(pchara,
					sg_pDungeonSystem->Map().Land(pchara->placeX,pchara->placeY),
					sg_pDungeonSystem->Map().Land(pchara->placeX,pchara->placeY),
					pchara->aspect);
			}
		}

	}

	//アイテムに対し透明度変更の処理を施すために、
	//仮の移動アニメーションを追加する。
	//（あし踏み）を追加するアイテムは透明度が変更されるキャラのみ
	for(i=0;i<sg_pDungeonSystem->visibleDropList().size();i++)
	{//i番目のアイテムに対し
		pcDroping pdrop = sg_pDungeonSystem->visibleDropList()[i];		

		if(
			(pdrop->opaque > 0.5) ==//これは前の待機時に表示されていたかどうかが入る
			!pdrop->isDiscover())
		{//透明度が変更されるアイテム
			caMove * pcam = new caMove;
			pcAnimation pca = pcAnimation(pcam);
			

			pcam->pGobj = pdrop;
			pcam->checkpoint.push_back(pdrop->現在地形());
			pcam->checkpoint.push_back(pdrop->現在地形());
			pcam->Sight.push_back((pdrop->opaque > 0.5));//これは前の待機時に表示されていたかどうかが入る
			pcam->Sight.push_back((pdrop->isDiscover()));//これは表示されるべきかどうかが入る

			pcam->Aspect.push_back(5);

			pcam->speed = 1;

			//------------------------------
			//ダッシュ用のスピードUP
			if(sg_pDungeonSystem->DashSwitch())
			{
				pcam->speed *= sg_pDungeonSystem->DashSpeed();//8;
			}


			
			//if(pca->isOnScreen())
			{//スクリーン上にあるときだけ
				MoveAnimeList.push_back(pca);
			}
			
		}


	}


	//moveの整理
	for(i=0;i<AM_MOVEPOOLHASHNUM;i++)
	{




		while(MovePool[i]->size() != 0)
		{
			list<ps_Move>::iterator itr = MovePool[i]->begin();
			vector<ps_Move> pmoveV;
			pcCharacter pchara = (*itr)->pchara;//このキャラについてまとめる

			pmoveV.clear();

			while(itr != MovePool[i]->end())
			{
				if((*itr)->pchara == pchara)
				{//移動リストに追加。削除
					pmoveV.push_back(*itr);
					itr = MovePool[i]->erase(itr);
				}
				else
				{//次
					itr++;
				}
			}
			for(j=0;j<pmoveV.size();j++)
			{//まとめた移動リストを処理
				caMove * pcam = new caMove;
				pcAnimation pca = pcAnimation(pcam);
				

				pcam->pGobj = pchara;
				pcam->checkpoint.push_back(pmoveV[0]->start);
				pcam->Sight.push_back((pchara->opaque > 0.5));//これは前の待機時に表示されていたかどうかが入る

				pcam->speed = pmoveV.size();

				//------------------------------
				//ダッシュ用のスピードUP
				if(sg_pDungeonSystem->DashSwitch())
				{
					pcam->speed *= sg_pDungeonSystem->DashSpeed();//8;
				}


				for(k=0;k<pmoveV.size();k++)
				{
					pcam->checkpoint.push_back(pmoveV[k]->end);
					pcam->Sight.push_back(pmoveV[k]->sight/* && !pchara->死亡()*/);
					pcam->Aspect.push_back(pmoveV[k]->aspect);
				}

				
				//if(pca->isOnScreen())
				{//スクリーン上にあるときだけ
					MoveAnimeList.push_back(pca);
				}
				
			}
			
		}
		MovePool[i]->clear();
	}



	Enpty = true;
	return true;
}
int cAnimationManager::WeekInvocation(cRenderDevice *pDev)
{

	int i;
	int EIfrag = false;
	for(i=0;i<AttackAnimeList.size();i++)
	{
		EIfrag |= AttackAnimeList[i]->EasyInvocation();
		if(EIfrag)
		{
			break;
		}
	}
	
	if(EIfrag)
	{
		StrongInvocation(pDev);
		return true;
	}


	return false;
}

int cAnimationManager::Draw(cRenderDevice *pDev)
{


	AttackAnimeDraw(pDev);


	return true;
}
void cAnimationManager::Draw_parallel(cRenderDevice *pDev)
{
	{
		list<pcAnimation>::iterator itr = ParallelAnimeList.begin();
		for(;itr != ParallelAnimeList.end();itr++)
		{
			(*itr)->Draw(pDev);
		}
	}

	{
		list<pcAnimation>::iterator itr = ParallelCriticalAnimeList.begin();
		for(;itr != ParallelCriticalAnimeList.end();itr++)
		{
			(*itr)->Draw(pDev);
		}
	}
}

int cAnimationManager::AttackAnimeDraw(cRenderDevice *pDev)
{
	int Layer[LAYERLEVEL_LAYERMAXNUM] = {0};

	int i = 0;
	while(1)
	{
		//空っぽ
		if(AttackAnimeList.size() == 0) return true;
		
		//もう次ない
		if(i >= AttackAnimeList.size()) break;

	



		if(!AttackAnimeList[i]->End)
		{//終わってない
			AttackAnimeList[i]->Draw(pDev);
			

			if(AttackAnimeList[i]->LayerLevel < LAYERLEVEL_LAYERMAXNUM)		
				Layer[AttackAnimeList[i]->LayerLevel] = true;


			if(i+1 < AttackAnimeList.size())
			{//つづきある
				if(AttackAnimeList[i+1]->LayerLevel < LAYERLEVEL_LAYERMAXNUM)
				{
					if(Layer[AttackAnimeList[i+1]->LayerLevel])
					{//次のがレイヤ同じ
						break;
					}
				}
			}

		}


		i++;
	}



	return true;
}



int cAnimationManager::AddMove(pcCharacter pchara, pcLandform start, pcLandform end,int aspect)
{
	ps_Move psm = ps_Move(new s_Move);
	psm->pchara = pchara;
	psm->start = start;
	psm->end = end;
	psm->sight = sg_pDungeonSystem->キャラクター目視可(pchara);//endのほう
	if(aspect == -1) psm->aspect = (end->place-start->place).GetAspect();
	else psm->aspect = aspect;
	MovePool[g_Hash((long)(pchara.get()),AM_MOVEPOOLHASHNUM)]->push_back(psm);
	
	Enpty = false;
	return true;
}

int cAnimationManager::AddAnime_realtime(pcAnimation pcanime)
{
	pcanime->Init(pcanime);
	//if(pcanime->isOnScreen())
	{
		ParallelAnimeList.push_back(pcanime);
		return true;
	}
	return false;
}
int cAnimationManager::AddAnime_realtimeCritical(pcAnimation pcanime)
{
	pcanime->Init(pcanime);
	//if(pcanime->isOnScreen())
	{
		ParallelCriticalAnimeList.push_back(pcanime);
		return true;
	}
	return false;
}
int cAnimationManager::AddAnime_parallel(pcAnimation pcanime, int wait)
{
	pcanime->Init(pcanime);
	pcaPAnimeAdd pcaa = pcaPAnimeAdd(new caPAnimeAdd);

	//if(pcaa->isOnScreen())
	{
		pcaa->Init();
		pcaa->wait = wait;
		pcaa->paddanime = pcanime;
		AttackAnimeList.push_back(boost::dynamic_pointer_cast<cAnimation>(pcaa));
		return true;
	}
	return false;
}
int cAnimationManager::AddAnime_parallelCritical(pcAnimation pcanime, int wait)
{
	pcanime->Init(pcanime);
	pcaPAnimeAdd pcaa = pcaPAnimeAdd(new caPAnimeAdd(true));

	//if(pcaa->isOnScreen())
	{
		pcaa->Init();
		pcaa->wait = wait;
		pcaa->paddanime = pcanime;
		AttackAnimeList.push_back(boost::dynamic_pointer_cast<cAnimation>(pcaa));
		return true;
	}
	return false;
}

int cAnimationManager::AddAnime(pcAnimation pcanime)
{
	//pcAnimation pcanime = pcAnimation(pca);

	//if(pcanime->isOnScreen())
	{
		pcanime->Init(pcanime);
		AttackAnimeList.push_back(pcanime);
	}

	Enpty = false;
	return true;
}

int g_Hash(unsigned long int resource, int maxsize)
{
	double h = 0;
	unsigned long int r = resource*1103515245+12345;
	r = r & 2147483647;
	h = (double)r / 2147483648;

	return h * maxsize;
}


/*
void cAnimationManager::GetNormalAttackAttribute(int Input_ID,
		tstring &effectFileName, int &vi, double &speed, tstring &seFileName)
{
	switch(Input_ID)
	{
	case ATTACKATTRI_PUNCH1:
		effectFileName = _T("effect\\attack_hit.png");
		vi = 0;
		speed = 1;
		seFileName = _T("attack_punch1.wav");
		break;
	case ATTACKATTRI_PUNCH1_DOUBLESPEED:
		effectFileName = _T("effect\\attack_hit.png");
		vi = 0;
		speed = 2;
		seFileName = _T("attack_punch1.wav");
		break;
	case ATTACKATTRI_SWING1:
		effectFileName = _T("effect\\attack_swing.png");
		vi = 0;
		speed = 1;
		seFileName = _T("attack_swing1.wav");
		break;
	default:
		effectFileName = _T("effect\\attack_hit.png");
		vi = 0;
		speed = 1;
		seFileName = _T("attack_punch1.wav");
	}
}
*/