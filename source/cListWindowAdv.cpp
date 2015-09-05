#include "stdafx.h"

#include "cListWindowAdv.h"
#include "cCommandDroping.h"
//#define CURSORSIZE 16

#define PAGETEXPATH _T("skin\\page.png")
#define MULTISELECTTEXPATH _T("interface\\multiselect.png")
//#define PAGETEXSIZE 32

#define PAGECOUNTNUM 70



#define Message_ICON_WIDTH (32)
#define Message_ICON_HEIGHT (32)


cListWindowAdv::cListWindowAdv(void)
{
	preRendar = false;

	ppDroplist = NULL;

	MultiSelect = false;

	subWindowMode = 0;
}

cListWindowAdv::~cListWindowAdv(void)
{
	pDroplist.clear();
}


void cListWindowAdv::Init(IDirect3DDevice9 *pDev, int letterXnum, int letterYnum)
{
	
	InitBasic(pDev,letterXnum,letterYnum);



	cursolIndexMax = letterYnum;

	//ラベルwindow
	pwLabel = pcGameWindow(new cGameWindow);
	pwLabel->Init(pDev,letterXnum,1);


	Width = letterXnum * GWFONTSIZE + sideEdgeWith()*2 + cursoletcMarginWidth();


	m_pTexture_Page =  g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev, PAGETEXPATH);
	m_pTexture_Multiselect =  g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev, MULTISELECTTEXPATH);



	//cursolIndexMaxのかわりにContentNumlist[pageIndex]をつかう
	//cursolIndexMax = min(pDroplist.size(),letterYnum);
	//cursolIndex = 0;

	pvpDropFrmPgIndex.clear();
	ContentNumFrmPgIndex.clear();
	LabelFrmPgIndex.clear();
	pvpDropFrmPgIndex.clear();
	pvpDropFrmPgIndex_forSort.clear();
	pvpDroplistFirstIndex.clear();

	pageIndexMax = 0;
	int i;
	for(i=0;i<vpDroplist.size();i++)
	{
		int k;
		for(k=0;;)
		{
			int rest = vpDroplist[i].size() - k; 
			pvpDropFrmPgIndex.push_back(&(vpDroplist[i]));
			
			if(pvpDroplist_forSort.size() > i)
			{
				pvpDropFrmPgIndex_forSort.push_back(pvpDroplist_forSort[i]);
			}


			pvpDroplistFirstIndex.push_back(k);

			//Label
			if((vpDroplist[i].size()-1)/letterYnum == 0)
			{
				if(pvpDroplistName.size() > i)
				{
					LabelFrmPgIndex.push_back(pvpDroplistName[i]);
				}
			}
			else
			{
				StyleString sstr;
				sstr.addstring(_T(" ("));
				sstr.addstring((int)((k/letterYnum+1)));
				sstr.addstring(_T("/")); 
				sstr.addstring((int)((vpDroplist[i].size()-1)/letterYnum)+1);
				sstr.addstring(_T(")"));
				sstr.setSameColor(0xFF80C0D0);
				if(pvpDroplistName.size() > i)
				{
					LabelFrmPgIndex.push_back(pvpDroplistName[i] + sstr);
				}
			}

			if(rest>letterYnum)
			{//残りある
				ContentNumFrmPgIndex.push_back(letterYnum);
				k += letterYnum;
			}
			else
			{//残りない
				ContentNumFrmPgIndex.push_back(rest);
				break;
			}
		}

		if(vpDroplist[i].size())
			pageIndexMax += ((vpDroplist[i].size()-1)/letterYnum+1);
		
	}

	if(pageIndexMax == 0) pageIndexMax = 1;
	pageIndex = 0;

	//サブ目的オブジェクト

	for(i=0;i<vpDroplist.size();i++)
	{
		int k, size = vpDroplist[i].size();
		for(k=0;k<size;k++)
		{
			pcDroping pdrop = vpDroplist[i][k];
			if(pdrop->内包落ち物対象可())
			{
				vpsubDropmap.insert(pair<pcDroping, vector<pcDroping>>(pdrop, pdrop->内包落ち物対象リスト()));
				/*
				pcCommandDroping pcommandDroping = boost::dynamic_pointer_cast<cCommandDroping>(pdrop);
				if(pcommandDroping != NULL && pcommandDroping->pdropingfunc->LastObject() && pcommandDroping->pdropingfunc->LastObject()->includedItem.size() > 0)
				{
					vector<pcDroping> vsubpDroping;
					pcDroping pobject = pcommandDroping->pdropingfunc->LastObject();
					int i,size = pobject->includedItem.size();
					for(i=0;i<size;i++)
					{
						
						//新規
						pcDropingFunc pcdf = pcDropingFunc(new cDropingFunc);
						pcdf->Subject = pcommandDroping->pdropingfunc->Subject;
						pcdf->Verb = pcommandDroping->pdropingfunc->Verb;
						pcdf->Objects = pcommandDroping->pdropingfunc->Objects;
						pcdf->Objects.pop_back();
						pcdf->Objects.push_back(pobject->includedItem[i]);
						//パック
						vsubpDroping.push_back(DropingFuncToDroping(pcdf));
					}

					vpsubDropmap.insert(pair<pcDroping, vector<pcDroping>>(pdrop, vsubpDroping));
					
				}
				*/
			}
		}
	}

	/*
	int iws = letterXnum * GWFONTSIZE;
	int ihs = letterYnum * GWFONTSIZE;
	int tw = iws;
	int th = ihs;
	//int i;
	for(i=0;i<64;i++)
	{
		if(!((tw-1) >> i)) break;
	}
	tw = 1<<i;//tw以上の最小の2^n
	for(i=0;i<64;i++)
	{
		if(!((th-1) >> i)) break;
	}
	th = 1<<i;//th以上の最小の2^n


	m_SpriteText.Init(pDev, 1, tw, th);
	m_SpriteText.SetCursolPos(0,0);
*/

	m_pageText.Init(pDev, 1, EDGEWIDTH*8, EDGEWIDTH);


	ReRendarText();

	numf.Init(pDev);

	if(enable)
	{
		EventCarsolmove();
	}
}


pcDroping cListWindowAdv::contents(int page, int index)
{

	/*//安全性確保
	if(page >= pvpDropFrmPgIndex.size()) page = pvpDropFrmPgIndex.size() -1;
	if(page == -1) return NULLDROP;

	if(index >= (ContentNumFrmPgIndex)[page]) index = ContentNumFrmPgIndex[page] -1;
	if(index == -1) return NULLDROP;
	*/
	if(page >= pvpDropFrmPgIndex.size()) return NULLDROP;
	if(page <= -1) return NULLDROP;

	if(index >= (ContentNumFrmPgIndex)[page]) return NULLDROP;
	if(index <= -1) return NULLDROP;
	return (pvpDropFrmPgIndex)[page]->at(index+pvpDroplistFirstIndex[page]);
}
pcDroping cListWindowAdv::nowcontents()
{
	if(subWindowMode && psubWindow)
	{
		return psubWindow->nowcontents();
	}
	else
	{
		return cListWindow::nowcontents();
	}
}
void cListWindowAdv::SetCursolTolast()
{
	pageIndex = pageIndexMax - 1;
	cursolIndex = ContentNumFrmPgIndex[pageIndex] - 1;
}

int cListWindowAdv::Draw(IDirect3DDevice9 *pDev)
{
	WindowDraw(pDev);
	TextDraw(pDev);
	CursorDraw(pDev);
	pageDraw(pDev);

	pwLabel->color = color;
	pwLabel->Draw(pDev);

	if(psubWindow != NULL)
	{
		psubWindow->color = color;
		psubWindow->cListWindow::Draw(pDev);
	}

	MultiSetDraw(pDev);

	ShortCutDraw(pDev);
	return true;
}


int cListWindowAdv::process(IDirect3DDevice9 *pDev)
{

	if(subWindowMode)
	{
		return subWindowProcess(pDev);
	}

	if(psubWindow)
	{
		if(g_pPlayerInput()->migi().justOn)
		{
			playsound_move();
			subWindowMode = 1;
			psubWindow->cursolEnable = true;
			cursolOpacity = 0.5;
			return true;
		}
	}

	if(shortCut_process(pDev)) return true;
	if(shortCutState) return true;

	//カーソル移動ルーチン
	//上下
	cursolIndex += (g_pPlayerInput()->shita().justOn) || (g_pPlayerInput()->shita().on && g_pPlayerInput()->shita().onCount > 15 && !(g_pPlayerInput()->shita().onCount%5));
	cursolIndex -= (g_pPlayerInput()->ue().justOn) || (g_pPlayerInput()->ue().on && g_pPlayerInput()->ue().onCount > 15 && !(g_pPlayerInput()->ue().onCount%5));


	if(cursolIndex < 0) cursolIndex = ContentNumFrmPgIndex[pageIndex] - 1;
	else if(cursolIndex >= ContentNumFrmPgIndex[pageIndex]) cursolIndex = 0;


	int tempP = pageIndex;
	pageIndex += (g_pPlayerInput()->migi().justOn)*(1+9*g_pPlayerInput()->dash().on);
	pageIndex -= (g_pPlayerInput()->hidari().justOn)*(1+9*g_pPlayerInput()->dash().on);


	if(tempP != pageIndex) preRendar = false;

	if(pageIndex < 0) pageIndex = pageIndexMax - 1;
	else if(pageIndex >= pageIndexMax) pageIndex = 0;
	
	//ページ移動による丸め込み
	else if(cursolIndex >= ContentNumFrmPgIndex[pageIndex]) cursolIndex = ContentNumFrmPgIndex[pageIndex] - 1;



	//安全性確保
	if( contents(pageIndex,cursolIndex) == NULLDROP)
	{
		pageIndex = pageIndexMax - 1;
		cursolIndex = ContentNumFrmPgIndex[pageIndex] - 1;
	}



	if(g_pPlayerInput()->shita().justOn || (g_pPlayerInput()->shita().on && g_pPlayerInput()->shita().onCount > 15 && !(g_pPlayerInput()->shita().onCount%5))
			|| g_pPlayerInput()->ue().justOn || (g_pPlayerInput()->ue().on && g_pPlayerInput()->ue().onCount > 15 && !(g_pPlayerInput()->ue().onCount%5))
			|| g_pPlayerInput()->hidari().justOn
			|| g_pPlayerInput()->migi().justOn)
	{
		playsound_move();
	}


	if(preCarsolIndex != (pageIndex<<16) + cursolIndex)
	{//カーソル動いた
		EventCarsolmove();
	}
	preCarsolIndex = (pageIndex<<16) + cursolIndex;

	//決定ボタン
	if(decisionEnable && g_pPlayerInput()->decision().justOn)
	{
		if(MultiSelect && !pMultiDropSet.empty())
		{
			FireofMultiSet(pDev);
			playsound_decide();
		}
		else
		{
			if(nowcontents() != NULL)
				nowcontents()->Action(pDev);
			playsound_decide();
		}
	}

	if(g_pPlayerInput()->diagon().justOn 
		&& pvpDropFrmPgIndex_forSort.size() > pageIndex)
	{//ソート
		sortDrops(pDev);
		togetherDrops(pDev);
		EventCarsolmove();
	}


	processofMultiSet(pDev);


	m_pageAnimeCount++;
	if(m_pageAnimeCount > PAGECOUNTNUM) m_pageAnimeCount = 0;

	return true;
}

int cListWindowAdv::subWindowProcess(IDirect3DDevice9 *pDev)
{
	if(subWindowMode)
	{
		
		if(psubWindow->shortCut_process(pDev)) return true;
		if(psubWindow->shortCutState) return true;

		psubWindow->process(pDev);

		if(g_pPlayerInput()->migi().justOn)
		{
			pageIndex += 1;
			preRendar = false;
			playsound_move();
			subWindowMode = 0;
			psubWindow->cursolEnable = false;
			cursolOpacity = 1.0;
			EventCarsolmove();
		}
		else if(g_pPlayerInput()->hidari().justOn)
		{
			playsound_move();
			subWindowMode = 0;
			psubWindow->cursolEnable = false;
			cursolOpacity = 1.0;
			EventCarsolmove();
		}


		if(pageIndex < 0) pageIndex = pageIndexMax - 1;
		else if(pageIndex >= pageIndexMax) pageIndex = 0;
		
		//ページ移動による丸め込み
		else if(cursolIndex >= ContentNumFrmPgIndex[pageIndex]) cursolIndex = ContentNumFrmPgIndex[pageIndex] - 1;



		//安全性確保
		if( contents(pageIndex,cursolIndex) == NULLDROP)
		{
			pageIndex = pageIndexMax - 1;
			cursolIndex = ContentNumFrmPgIndex[pageIndex] - 1;
		}


		if(preCarsolIndex != (pageIndex<<16) + cursolIndex)
		{//カーソル動いた
			EventCarsolmove();
		}
		preCarsolIndex = (pageIndex<<16) + cursolIndex;


		//決定ボタン
		if(decisionEnable && g_pPlayerInput()->decision().justOn)
		{
			if(MultiSelect && !pMultiDropSet.empty())
			{
				FireofMultiSet(pDev);
				playsound_decide();
			}
			else
			{
				if(nowcontents() != NULL)
					nowcontents()->Action(pDev);
				playsound_decide();
			}
		}
		
		processofMultiSet(pDev);

		return true;
	}
	else
	{
		subWindowMode = 0;
	}
	return false;
}
int cListWindowAdv::RerenderDraw(IDirect3DDevice9 *pDev)
{
	cListWindow::RerenderDraw(pDev);

	//ラベルwindow
	if(LabelFrmPgIndex.size() > pageIndex)
		pwLabel->Text() = LabelFrmPgIndex[pageIndex];
	pwLabel->ReRendarText();

	//ラベル
	pwLabel->setBottom(Top());
	pwLabel->setLeft(Left());

	return true;
}

/*
int cListWindowAdv::TextDraw(IDirect3DDevice9 *pDev)
{
	


	int i;
	if(preRendar == false)
	{
		RerenderDraw(pDev);
		preRendar = true;
	}


	for(i=0;i<ContentNumFrmPgIndex[pageIndex];i++)
	{
		if(contents(pageIndex,i) == NULL) break;
		m_SpriteText.SetCursolPos(CURSORSIZE + CURSORSIZE,GWFONTSIZE*i);
		contents(pageIndex,i)->DrawIcon(pDev, Left() + EDGEWIDTH + CURSORSIZE + CURSORSIZE/2, Top() + EDGEWIDTH + lineheight()*(i+0.5));
		contents(pageIndex,i)->DrawStateIcon(pDev, Left() + EDGEWIDTH + CURSORSIZE + CURSORSIZE/2, Top() + EDGEWIDTH + lineheight()*(i+0.5));
	}

	
	
	TextDrawSub(pDev);


	return true;
}
*/

/*
int cListWindow::TextDrawSub(IDirect3DDevice9 *pDev)
{
	int l = Left();
	int t = Top();
	int r = l + Width;
	int b = t + Height;

	double ew = EDGEWIDTH;


	int iws = Width - EDGEWIDTH*2;
	int ihs = Height - EDGEWIDTH*2;
	int tw = iws;
	int th = ihs;
	int i;
	for(i=0;i<64;i++)
	{
		if(!((tw-1) >> i)) break;
	}
	tw = 1<<i;//tw以上の最小の2^n
	for(i=0;i<64;i++)
	{
		if(!((th-1) >> i)) break;
	}
	th = 1<<i;//th以上の最小の2^n


	m_DO.setTexture(m_SpriteText.GetTex(), tw, th);

	m_DO.setLTRB(			l + ew,		t + ew,		r - ew,		b - ew);
	m_DO.m_TexRange.setLTRB(0,		0,		iws / (double)tw ,	ihs / (double)th);

	//影ドロー
	m_DO.m_color.ARGB(128,0,0,0);
	m_DO.CenterX += 2;
	m_DO.CenterY += 2;
	m_DO.Draw(pDev);


	m_DO.m_color = color;
	m_DO.CenterX -= 2;
	m_DO.CenterY -= 2;
	m_DO.Draw(pDev);

	return true;

}

*/

  
int cListWindowAdv::sortDrops(IDirect3DDevice9 *pDev)
{
	int i;
	if(pvpDropFrmPgIndex_forSort[pageIndex] == NULL) return false;

	sort( pvpDropFrmPgIndex_forSort[pageIndex]->begin(), pvpDropFrmPgIndex_forSort[pageIndex]->end(), ::itemIDsortFunc  );
	sort( pvpDropFrmPgIndex[pageIndex]->begin(), pvpDropFrmPgIndex[pageIndex]->end(), ::itemIDsortFunc  );

	cCoordinate coo(sg_pDungeonSystem->pPlayerChara()->placeX,sg_pDungeonSystem->pPlayerChara()->placeY);
	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("itemseiri.wav"),coo);

	ReRendarText();

	return true;
}

int cListWindowAdv::togetherDrops(IDirect3DDevice9 *pDev)
{
	
	if(pvpDropFrmPgIndex_forSort[pageIndex] == NULL) return false;
	vector<pcDroping> drops = *pvpDropFrmPgIndex[pageIndex];
	vector<pcDroping>* pointer_drops = pvpDropFrmPgIndex[pageIndex];
	vector<pcDroping>* pointer_source_drops = pvpDropFrmPgIndex_forSort[pageIndex];

	if(drops.empty()) {
		return true;
	}
	pcCharacter pchara = drops[0]->Holder();
	if(pchara == NULL) {
		return true;
	}

	int i,size = drops.size();
	for(i=0; i<size; i++)
	{
		sg_pDungeonSystem->落ち物まとめ要請(drops[i], pchara);
	}

	*pointer_drops = *pointer_source_drops;

	Init(sg_pDungeonSystem->pDevice_D3D,letterXlength,letterYlength);

	
	return true;
}


StyleString cListWindowAdv::shortExplanationString(IDirect3DDevice9 *pDev)
{
	pcDroping pdrop = nowcontents();
	if(pdrop == NULL)
	{
		return _T("");
	}
	return pdrop->shortExplanation();

}


int cListWindowAdv::EventCarsolmove()
{
	pcDroping pdrop = nowcontents();
	if(pdrop == NULL)
	{
		return false;
	}
		

	sg_pDungeonSystem->menuControlLayerV().Last()->smollviewWindow = pcGameWindow((cGameWindow*)NULL);

/*
	if(pdrop->内包落ち物対象可())
	{
		pcCommandDroping pcommandDroping = boost::dynamic_pointer_cast<cCommandDroping>(pdrop);
		if(pcommandDroping != NULL && pcommandDroping->pdropingfunc->LastObject() && pcommandDroping->pdropingfunc->LastObject()->includedItem.size() > 0)
		{
			pcDroping pobject = pcommandDroping->pdropingfunc->LastObject();
			psubWindow = pcListWindow(new cListWindow);

			
			int i,size = pobject->includedItem.size();
			for(i=0;i<size;i++)
			{
				
				//新規
				pcDropingFunc pcdf = pcDropingFunc(new cDropingFunc);
				pcdf->Subject = pcommandDroping->pdropingfunc->Subject;
				pcdf->Verb = pcommandDroping->pdropingfunc->Verb;
				pcdf->Objects = pcommandDroping->pdropingfunc->Objects;
				pcdf->Objects.pop_back();
				pcdf->Objects.push_back(pobject->includedItem[i]);
				//パック
				psubWindow->pDroplist.push_back(DropingFuncToDroping(pcdf));
			}

			psubWindow->enable = false;
			psubWindow->Init(sg_pDungeonSystem->pDevice_D3D, 12, pobject->内包落ち物サイズ());
			psubWindow->setRight(SCREEN_X);
			psubWindow->setTop(Top());
		}
		else
		{
			psubWindow = pcListWindow((cListWindow*)NULL);

			pdrop->Getsmollview(sg_pDungeonSystem->menuControlLayerV().Last()->smollviewWindow);
		}
	
		//smollwindow消去
	}
	else
	{
		psubWindow = pcListWindow((cListWindow*)NULL);

		pdrop->Getsmollview(sg_pDungeonSystem->menuControlLayerV().Last()->smollviewWindow);
	}
	*/
	int subflag = 1;
	pcCommandDroping pcommandDroping = boost::dynamic_pointer_cast<cCommandDroping>(pdrop);
	if(pcommandDroping && pcommandDroping->pdropingfunc->Subject == pcommandDroping->pdropingfunc->LastObject())
	{
		subflag = 0;
	}
	map<pcDroping,vector<pcDroping>>::iterator itr = vpsubDropmap.find(pdrop);
	if(itr != vpsubDropmap.end() && subflag)
	{
		psubWindow = pcListWindow(new cListWindow);

		psubWindow->pDroplist = itr->second;

		psubWindow->cursolEnable = false;
		psubWindow->decisionEnable = false;
		psubWindow->enable = false;//InitでEventCarsolmoveしないようにするための小細工
		psubWindow->Init(sg_pDungeonSystem->pDevice_D3D, 12, pdrop->内包落ち物サイズ());
		psubWindow->setRight(SCREEN_X);
		psubWindow->setTop(Top());
		psubWindow->enable = true;
	}
	else
	{
		psubWindow = pcListWindow((cListWindow*)NULL);

		pdrop->Getsmollview(sg_pDungeonSystem->menuControlLayerV().Last()->smollviewWindow);
	}
	return true;
}


//--------------------------------------------------
//複数選択
/*
int cListWindowAdv::processofMultiSet(IDirect3DDevice9 *pDev)
{

	if(MultiSelect && g_pPlayerInput()->diagon().justOn)
	{
		pcDroping pdrop = nowcontents();
		if(pdrop == NULL) return false;


		if(findfromMultiDrop(pdrop))
		{
			eraseMultiSet();
		}
		else
		{
			insertMultiSet();
		}
		playsound_decide();
	}

	return true;
}
*/

int cListWindowAdv::MultiSetDraw(IDirect3DDevice9 *pDev)
{

	cListWindow::MultiSetDraw(pDev);

	if(MultiSelect)
	{

		if(psubWindow)
		{

			int i;
			for(i=0;i<psubWindow->ContentNumFrmPgIndexF(psubWindow->pageIndex);i++)
			{
				pcDroping pdrop = psubWindow->contents(psubWindow->pageIndex,i);
				if(pdrop == NULL) return false;

				int l = psubWindow->Left();
				int t = psubWindow->Top();
				double ew = EDGEWIDTH;
				cDrawingObject DO;
		
		


				int index;
				if(index = findfromMultiDrop(pdrop))
				{

					//m_DO.m_color.ARGB(255,255,255,255)

					//番号
					numf.position.set(psubWindow->Left()+EDGEWIDTH, EDGEWIDTH + t + ew + i*psubWindow->lineheight() + psubWindow->lineheight()/2,0,0);
					numf.Value_ = index;
					numf.FontHeightSize = 32;
					numf.FontWidthSize = 16;
					numf.color.ARGB(255,255,255,255);

					numf.Draw(pDev);

					//カーソル
					DO.setTexture(m_pTexture_Cursor, CURSORSIZE, CURSORSIZE);
					DO.m_color.ARGB(255,128,255,255);
					DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;
					DO.Width = DO.Height = CURSORSIZE;
					DO.setLeft(l+ew);	
					
					
					DO.CenterY = t + ew + i*psubWindow->lineheight() + psubWindow->lineheight()/2;

					DO.m_TexRange.setLTRB(0,		0,		1,		1);
					DO.Draw(pDev);

					//マスク的な何か
					DO.setTexture(m_pTexture_Multiselect);
					DO.Height = GWFONTSIZE;
					DO.formLeft(psubWindow->Left() + EDGEWIDTH+CURSORSIZE);
					DO.formRight(psubWindow->Right() - EDGEWIDTH);
					//DO.setLeft(l+ew);
					DO.m_TexRange.setLTRB(0,		0,		1,		1);
					DO.m_color.ARGB(64,255,255,255);
					DO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;
					DO.Draw(pDev);
					DO.m_color.ARGB(196,255,255,255);
					DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;
					DO.Draw(pDev);
				}
			}

		}
	}

	return true;
}
/*
int cListWindowAdv::FireofMultiSet(IDirect3DDevice9 *pDev)
{

	//insertMultiSet();


	int i;
	for(i=0;i<pMultiDropSet.size();i++)
	{
		
		pMultiDropSet[i]->Action(pDev);
		
	}

	eraseMultiSet();

	return true;
}

int cListWindowAdv::insertMultiSet()
{
	pcDroping pdrop = nowcontents();
	if(pdrop == NULL) return false;

	pMultiDropSet.push_back(pdrop);
	return true;
}


int cListWindowAdv::eraseMultiSet()
{
	pcDroping pdrop = nowcontents();
	if(pdrop == NULL) return false;

	vector<pcDroping>::iterator itr = pMultiDropSet.begin();
	for(;itr!=pMultiDropSet.end();itr++)
	{
		if(*itr == pdrop)
		{
			pMultiDropSet.erase(itr);
			return true;
		}
	}

	return true;
}
*/