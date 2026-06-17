#include "stdafx.h"

#include "cListWindow.h"
#include "cDropingFunc.h"

#include "cSaveStore.h"
#include "cShortCuts.h"

#define PAGETEXPATH _T("skin\\page.png")
#define MULTISELECTTEXPATH _T("interface\\multiselect.png")
//#define PAGETEXSIZE 32

#define PAGECOUNTNUM 70



#define Message_ICON_WIDTH (32)
#define Message_ICON_HEIGHT (32)

cListWindow::cListWindow(void)
{
	preRendar = false;

	ppDroplist = NULL;

	MultiSelect = false;
}

cListWindow::~cListWindow(void)
{
	pDroplist.clear();
}


void cListWindow::Init(cRenderDevice *pDev, int letterXnum, int letterYnum)
{

	
	InitBasic(pDev,letterXnum,letterYnum);


	letterXlength = letterXnum;
	letterYlength = letterYnum;

	Width = letterXnum * GWFONTSIZE + sideEdgeWith()*2 + cursoletcMarginWidth();


	m_pTexture_Page =  g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev, PAGETEXPATH);
	m_pTexture_Multiselect =  g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev, MULTISELECTTEXPATH);


	cursolIndexMax = min(pDroplist.size(),letterYnum);
	cursolIndex = 0;

	if(cursolIndexMax == 0)
		pageIndexMax = 1;
	else
		pageIndexMax = (pDroplist.size()-1)/cursolIndexMax + 1;
	pageIndex = 0;

/*
	int iws = letterXnum * GWFONTSIZE;
	int ihs = letterYnum * lineheight();
	int tw = iws;
	int th = ihs;

	tw = largerMinPow2(tw);//tw以上の最小の2^n
	th = largerMinPow2(th);//th以上の最小の2^n

	m_SpriteText.Init(pDev, 1, tw, th);
	m_SpriteText.SetCursolPos(0,0);
*/

	m_pageText.Init(pDev, 1, EDGEWIDTH*8, EDGEWIDTH);


	ReRendarText();

	numf.Init(pDev);

	if(enable)
		EventCarsolmove();
}

void cListWindow::SetCursolTolast()
{
	pageIndex = pageIndexMax - 1;
	cursolIndex = pDroplist.size() - pageIndex*cursolIndexMax - 1;
}

int cListWindow::Draw(cRenderDevice *pDev)
{
	cSelectWindow::Draw(pDev);
	MultiSetDraw(pDev);
	return true;
}


int cListWindow::process(cRenderDevice *pDev)
{
	if(shortCut_process(pDev)) return true;
	if(shortCutState) return true;


	//カーソル移動ルーチン
	//上下
	cursolIndex += (g_pPlayerInput()->shita().justOn) || (g_pPlayerInput()->shita().on && g_pPlayerInput()->shita().onCount > 15 && !(g_pPlayerInput()->shita().onCount%5));
	cursolIndex -= (g_pPlayerInput()->ue().justOn) || (g_pPlayerInput()->ue().on && g_pPlayerInput()->ue().onCount > 15 && !(g_pPlayerInput()->ue().onCount%5));


	if(cursolIndex < 0) cursolIndex = cursolIndexMax - 1;
	else if(cursolIndex >= cursolIndexMax || pDroplist.size() <= pageIndex*cursolIndexMax + cursolIndex) cursolIndex = 0;


	int tempP = pageIndex;
	pageIndex += (g_pPlayerInput()->migi().justOn)*(1+9*g_pPlayerInput()->dash().on);
	pageIndex -= (g_pPlayerInput()->hidari().justOn)*(1+9*g_pPlayerInput()->dash().on);


	if(tempP != pageIndex) preRendar = false;

	if(pageIndex < 0) pageIndex = pageIndexMax - 1;
	else if(pageIndex >= pageIndexMax) pageIndex = 0;



	//安全性確保

	if(pDroplist.size() <= pageIndex*cursolIndexMax + cursolIndex)
	{
		if(g_pPlayerInput()->shita().justOn)
		{
			pageIndex = pageIndexMax - 1;
			cursolIndex = 0;
		}
		else
		{
			pageIndex = pageIndexMax - 1;
			cursolIndex = pDroplist.size() - pageIndex*cursolIndexMax - 1;
		}
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
			if(pDroplist[pageIndex*cursolIndexMax + cursolIndex] != NULL)
				pDroplist[pageIndex*cursolIndexMax + cursolIndex]->Action(pDev);
			playsound_decide();
		}
	}

	if(g_pPlayerInput()->diagon().justOn
		&& ppDroplist != NULL)
	{//ソート
		sortDrops(pDev);
		EventCarsolmove();
	}


	processofMultiSet(pDev);


	m_pageAnimeCount++;
	if(m_pageAnimeCount > PAGECOUNTNUM) m_pageAnimeCount = 0;

	return true;
}

int cListWindow::RerenderDraw(cRenderDevice *pDev)
{
	/*
	int i;
	m_SpriteText.CleatText();
	m_SpriteText.BeginText( g_fontstyle().c_str(), GWFONTSIZE );
	m_SpriteText.SetAntiAlias(1);
	m_SpriteText.SetColor( D3DCOLOR_ARGB(255,255,255,255) );
	m_SpriteText.SetReturnWidth(1024);//自動returnしない
	for(i=0;i<cursolIndexMax;i++)
	{
		int k = pageIndex*cursolIndexMax + i;
		if(pDroplist.size() <= k) break;
		m_SpriteText.SetCursolPos(CURSORSIZE + Message_ICON_WIDTH,linetop(i));
		if(pDroplist[k] != NULL)
			pDroplist[k]->RerenderText(m_SpriteText,letterXlength);
	}

	m_SpriteText.EndText();

	TCHAR t[MAXBUFFSIZE];
	//ページインデックス
	m_pageText.CleatText();
	m_pageText.BeginText( g_fontstyle().c_str(), EDGEWIDTH);
	m_pageText.SetAntiAlias(1);
	m_pageText.SetCursolPos(0,0);
	m_pageText.SetColor( D3DCOLOR_ARGB(255,255,255,255) );
	m_pageText.SetReturnWidth(1024);//自動returnしない
	_stprintf(t ,_T("%d_/_%d"),pageIndex+1,pageIndexMax);
	m_pageText.DrawText(t);

	m_pageText.EndText();
	*/

	int i;
	m_SpriteText.CleatText();
	m_SpriteText.SetWeight(GWFONTWEIGHT);
	m_SpriteText.BeginText( g_fontstyle().c_str(), GWFONTSIZE );
	m_SpriteText.SetAntiAlias(1);
	m_SpriteText.SetReturnFontMargin(merginheight());
	m_SpriteText.SetColor( D3DCOLOR_ARGB(255,255,255,255) );
	m_SpriteText.SetReturnWidth(1024);//自動returnしない
	for(i=0;i<ContentNumFrmPgIndexF(pageIndex);i++)
	{
		if(contents(pageIndex,i) == NULL) break;
		m_SpriteText.SetCursolPos(0,linetop(i));
		if(contents(pageIndex, i) != NULL)
			contents(pageIndex, i)->RerenderText(m_SpriteText,letterXlength);
	}

	m_SpriteText.EndText();

	TCHAR t[MAXBUFFSIZE];
	//ページインデックス
	m_pageText.CleatText();
	m_pageText.SetWeight(GWFONTWEIGHT);
	m_pageText.BeginText( g_fontstyle().c_str(), EDGEWIDTH);
	m_pageText.SetAntiAlias(1);
	m_pageText.SetCursolPos(0,0);
	m_pageText.SetColor( D3DCOLOR_ARGB(255,255,255,255) );
	m_pageText.SetReturnWidth(1024);//自動returnしない
	_stprintf(t ,_T("%d_/_%d"),pageIndex+1,pageIndexMax);
	m_pageText.DrawText(t);

	m_pageText.EndText();


	return true;
}


int cListWindow::TextDraw(cRenderDevice *pDev)
{
	

	/*
	int i;
	if(preRendar == false)
	{
		RerenderDraw(pDev);
		preRendar = true;
	}


	for(i=0;i<cursolIndexMax;i++)
	{
		int k = pageIndex*cursolIndexMax + i;
		if(pDroplist.size() <= k) break;
		m_SpriteText.SetCursolPos(CURSORSIZE + Message_ICON_WIDTH,linetop(i));
		pDroplist[k]->DrawIcon(pDev, Left() + EDGEWIDTH + CURSORSIZE + Message_ICON_WIDTH/2, Top() + EDGEWIDTH + lineheight()*(i+0.5));
		pDroplist[k]->DrawStateIcon(pDev, Left() + EDGEWIDTH + CURSORSIZE + Message_ICON_WIDTH/2, Top() + EDGEWIDTH + lineheight()*(i+0.5));
	}

	

	TextDrawSub(pDev);


	return true;
	*/
	

	int i;
	if(preRendar == false)
	{
		RerenderDraw(pDev);
		preRendar = true;
	}


	for(i=0;i<ContentNumFrmPgIndexF(pageIndex);i++)
	{
		if(contents(pageIndex,i) == NULL) break;
		m_SpriteText.SetCursolPos(CURSORSIZE + CURSORSIZE,GWFONTSIZE*i);
		contents(pageIndex,i)->m_color = color;
		contents(pageIndex,i)->DrawIcon(pDev, Left() + (sideEdgeWith()+EDGEWIDTH)/2 + CURSORSIZE*enable + CURSORSIZE/2, Top() + EDGEWIDTH + lineheight()*(i+0.5));
		contents(pageIndex,i)->DrawStateIcon(pDev, Left() + EDGEWIDTH + CURSORSIZE*enable, Top() + EDGEWIDTH + lineheight()*(i+0.5));
	}

	
	
	TextDrawSub(pDev);


	return true;
}

int cListWindow::cursoletcMarginWidth()
{
	return CURSORSIZE * enable + Message_ICON_WIDTH+GWFONTOVERMARGIN;
}

/*
int cListWindow::TextDrawSub(cRenderDevice *pDev)
{
	int l = Left();
	int t = Top();
	int r = Right();
	int b = Bottom();

	double ew = EDGEWIDTH;
	double ew_s = sideEdgeWith();

	int margin = GWFONTSIZE/2;//文字がはみ出さないように

	int iws = Width - ew_s*2 + margin;
	int ihs = Height - ew*2;
	int tw = letterXlength * GWFONTSIZE;
	int th = letterYlength * lineheight();
	int i;
	tw = largerMinPow2(tw);//tw以上の最小の2^n
	th = largerMinPow2(th);//th以上の最小の2^n



	m_DO.setTexture(m_SpriteText.GetTex(), tw, th);
	

	m_DO.setLTRB(			l + ew_s,		t + ew,		r - ew_s + margin,		b - ew);
	m_DO.m_TexRange.setLTRB(0,		0,		iws / (double)tw ,	ihs / (double)th);
	
	m_DO.m_color = color;
	DrawAddShadowDO(m_DO,pDev);


	return true;

}
*/


//落ち物並び替え順
bool itemIDsortFunc(pcDroping a, pcDroping b){
	if(a->ID()/1000 != b->ID()/1000)
	{//大別
		return (a->ID() 
				< b->ID());
	}

	if(a->大局識別済み() != b->大局識別済み())
	{//識別済み優先
			return (a->大局識別済み()
					> b->大局識別済み());
	}


	if(sg_pDungeonSystem->pPlayerChara()->attackequipment == a)
	{//装備最優先
		return true;
	}
	if(sg_pDungeonSystem->pPlayerChara()->attackequipment == b)
	{//装備最優先
		return false;
	}
	if(sg_pDungeonSystem->pPlayerChara()->defenseequipment == a)
	{//装備最優先
		return true;
	}
	if(sg_pDungeonSystem->pPlayerChara()->defenseequipment == b)
	{//装備最優先
		return false;
	}
	if(sg_pDungeonSystem->pPlayerChara()->bulletequipment == a)
	{//装備最優先
		return true;
	}
	if(sg_pDungeonSystem->pPlayerChara()->bulletequipment == b)
	{//装備最優先
		return false;
	}

	if(a->ID() != b->ID())
	{//異種
		if(a->大局識別済み())
		{//a->大局識別済み()のときb->大局識別済み()が保障される
		
			return (sg_pDungeonSystem->DataBase.compareDropusefulName(a->ID() , b->ID()) > 0);
		}
		else
		{
			return (sg_pDungeonSystem->DataBase.未識別名優先度(a->ID()) > sg_pDungeonSystem->DataBase.未識別名優先度(b->ID()));
		}
	}
	else
	{//同種
	
		int aq = a->quality();
		int bq = b->quality();

		if(!a->修正値識別済み()) aq = 0;//未識別;
		if(!b->修正値識別済み()) bq = 0;//未識別;

		return (aq
				> bq);
	
	}

}

pcDroping cListWindow::contents(int page, int index)
{

	//安全性確保
	int k = page*cursolIndexMax + index;
	if(0 > k) return NULLDROP;
	if(pDroplist.size() <= k) return NULLDROP;
	
	return pDroplist[k];
}


pcDroping cListWindow::pforcusedcontents()
{
	return contents(pageIndex, cursolIndex);
}


int cListWindow::sortDrops(cRenderDevice *pDev)
{
	int i;
	if(ppDroplist == NULL) return false;
	sort( pDroplist.begin(), pDroplist.end(), ::itemIDsortFunc  );

	//元
	sort( ppDroplist->begin(), ppDroplist->end(), ::itemIDsortFunc  );

	ReRendarText();

	return true;
}



StyleString cListWindow::shortExplanationString(cRenderDevice *pDev)
{
	pcDroping pdrop = nowcontents();
	//pcDroping pdrop = pDroplist[pageIndex*cursolIndexMax + cursolIndex];
	if(pdrop == NULL)
	{
		return _T("");
	}
	return pdrop->shortExplanation();

}


int cListWindow::EventCarsolmove()
{
	//pcDroping pdrop = pDroplist[pageIndex*cursolIndexMax + cursolIndex];
	pcDroping pdrop = nowcontents();
	if(pdrop == NULL)
	{
		return false;
	}
		
	if(pdrop->Getsmollview(sg_pDungeonSystem->menuControlLayerV().Last()->smollviewWindow))
	{
	
	}
	else
	{
		sg_pDungeonSystem->menuControlLayerV().Last()->smollviewWindow = pcGameWindow((cGameWindow*)NULL);
	}


	return true;
}

//--------------------------------------------------
//複数選択

int cListWindow::processofMultiSet(cRenderDevice *pDev)
{

	if(MultiSelect && g_pPlayerInput()->diagon().justOn)
	{
		
		pcDroping pdrop = nowcontents();
		if(pdrop == NULL) return false;
		//int k = pageIndex*cursolIndexMax + cursolIndex;
		//if(pDroplist.size() <= k) return false;

		//pcDroping pdrop = pDroplist[k];

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


int cListWindow::MultiSetDraw(cRenderDevice *pDev)
{

	/*
	if(MultiSelect)
	{
		int i;

		for(i=0;i<cursolIndexMax;i++)
		{

			//int k = pageIndex*cursolIndexMax + i;
			//if(pDroplist.size() <= k) break;
			//pcDroping pdrop = pDroplist[k];
			pcDroping pdrop = nowcontents();
			if(pdrop == NULL) break;

			int l = Left();
			int t = Top();
			double ew = EDGEWIDTH;
			cDrawingObject DO;
	
			DO.setTexture(m_pTexture_Cursor, CURSORSIZE, CURSORSIZE);
			DO.m_color.ARGB(255,128,255,255);
			DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;
			DO.Width = DO.Height = CURSORSIZE;
			DO.setLeft(l+ew);

			int index;
			if(index = findfromMultiDrop(pdrop))
			{

				//m_DO.m_color.ARGB(255,255,255,255)

				numf.position.set(Left()+EDGEWIDTH, 16 + t + ew + i*GWFONTSIZE + GWFONTSIZE/2,0,0);
				numf.Value_ = index;
				numf.FontHeightSize = 32;
				numf.FontWidthSize = 16;
				numf.color.ARGB(255,255,255,255);

				numf.Draw(pDev);

				DO.CenterY = t + ew + i*GWFONTSIZE + GWFONTSIZE/2;

				DO.m_TexRange.setLTRB(0,		0,		1,		1);
				DO.Draw(pDev);
			}
		}
	}
	*/

	if(MultiSelect)
	{
		int i;

		for(i=0;i<ContentNumFrmPgIndexF(pageIndex);i++)
		{
			pcDroping pdrop = contents(pageIndex,i);
			if(pdrop == NULL) return false;

			int l = Left();
			int t = Top();
			double ew = EDGEWIDTH;
			cDrawingObject DO;
	
	


			int index;
			if(index = findfromMultiDrop(pdrop))
			{

				//m_DO.m_color.ARGB(255,255,255,255)

				//番号
				numf.position.set(Left()+EDGEWIDTH, EDGEWIDTH + t + ew + i*lineheight() + lineheight()/2,0,0);
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
				
				
				DO.CenterY = t + ew + i*lineheight() + lineheight()/2;

				DO.m_TexRange.setLTRB(0,		0,		1,		1);
				DO.Draw(pDev);

				//マスク的な何か
				DO.setTexture(m_pTexture_Multiselect);
				DO.Height = GWFONTSIZE;
				DO.formLeft(Left() + EDGEWIDTH+CURSORSIZE);
				DO.formRight(Right() - EDGEWIDTH);
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

	return true;
}

int cListWindow::findfromMultiDrop(pcDroping pdrop)
{
	int i;

	for(i=0;i<pMultiDropSet.size();i++)
	{
		if(pMultiDropSet[i] == pdrop) return (i+1);
	}
	
	return 0;
}

int cListWindow::FireofMultiSet(cRenderDevice *pDev)
{

	//insertMultiSet();

	if(multiverbs.empty())
	{

		int i;
		for(i=0;i<pMultiDropSet.size();i++)
		{
			
			if(!pMultiDropSet[i]->Action(pDev)) break;
			
		}


		//eraseMultiSet();
	}
	else
	{
		vector<tstring> captions;
		//GetmenuCaption(captions);
		captions = multiverbs;

		if(captions.size() == 0) return false;

		pcControlLayer pccl;
		pcSelectWindow pcsw;
		pcListWindow pclw;



		//新規コントロールレイヤー
		sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
		pccl->Init(pDev);
		
		/*
		//見本
		pccl->WindowList.push_back(pclw = pcListWindow(new cListWindow));
		pclw->pDroplist.push_back(me());
		pclw->Init(pDev, 20, pclw->pDroplist.size());
		pclw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(1));
		pclw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(1));
		pclw->enable = false;
		*/


		pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));
		
		int size = 3;
		int i;
		for(i=0;i<captions.size();i++)
		{
			cDropingFunc *drfn;
			pcsw->commandList.push_back(pcCommand(drfn = new cDropingFunc(captions[i])));
			//drfn->Subject = me();
			drfn->Verb = captions[i];
			drfn->Objects = pMultiDropSet;
			
			size = max(size,captions[i].size());
			//drfn->shortExplanation = GetmenuExplain(drfn->Verb);
		}

		pcsw->Init(pDev, size, pcsw->commandList.size());
		pcsw->setRight(sg_pDungeonSystem->GameScreenInterface.menuPosRight(1));
		pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(3));


	}

	return true;
}

int cListWindow::insertMultiSet()
{
	//int k = pageIndex*cursolIndexMax + cursolIndex;
	//if(pDroplist.size() <= k) return false;
	//pcDroping pdrop = pDroplist[k];

	pcDroping pdrop = nowcontents();
	if(pdrop == NULL) return false;

	pMultiDropSet.push_back(pdrop);
	return true;
}


int cListWindow::eraseMultiSet()
{
	//int k = pageIndex*cursolIndexMax + cursolIndex;
	//if(pDroplist.size() <= k) return false;
	//pcDroping pdrop = pDroplist[k];
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

int cListWindow::multiverbsSelect()
{
	vector<tstring> captions = multiverbs;

	cRenderDevice *pDev = sg_pDungeonSystem->pDevice_D3D;

	/*
	//コマンド挿入
	cValiableField val;
	val.sets.dim(変数_挿入コマンド);
	sg_pDungeonSystem->CutInM().CutIn(Parent(),コマンド挿入_子アイテム_タイミング,val);
	if(!val.sets[変数_挿入コマンド].empty())
	{//挿入コマンドあり
		set<tstring>::iterator itr;
		itr = val.sets[変数_挿入コマンド].begin();
		for(;itr!=val.sets[変数_挿入コマンド].end();itr++)
		{
			captions.push_back(*itr);
		}
	}
	*/

	if(captions.size() == 0) return false;

	pcControlLayer pccl;
	pcSelectWindow pcsw;
	pcListWindow pclw;



	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);
	
	/*
	//見本
	pccl->WindowList.push_back(pclw = pcListWindow(new cListWindow));
	pclw->pDroplist.push_back(me());
	pclw->Init(pDev, 20, pclw->pDroplist.size());
	pclw->setLeft(40);
	pclw->setTop(32);
	pclw->enable = false;
*/

	pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));
	

	int i;
	for(i=0;i<captions.size();i++)
	{
		cDropingFunc *drfn;
		pcsw->commandList.push_back(pcCommand(drfn = new cDropingFunc(captions[i])));
		
		//drfn->Subject = ;
		drfn->Objects = pMultiDropSet;
		drfn->Verb = captions[i];

		//drfn->shortExplanation = GetmenuExplain(drfn->Verb);
	}

	pcsw->Init(pDev, 3, pcsw->commandList.size());
	pcsw->setRight(sg_pDungeonSystem->GameScreenInterface.menuPosRight(1));
	pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(1));





	return true;

}


int cListWindow::shortCut_process(cRenderDevice *pDev)
{
	if(shortCutState == 0)
	{
		//通常
		if(g_pPlayerInput()->shot().justOn)
		{
			if(
				 sg_pDungeonSystem->pSaveData->ShortCutsManager->canAddShortcut(pforcusedcontents()))
			{
				shortCutState = 1;
				shortCutSelect_X = 0;
				shortCutSelect_Y = 0;
				sg_pDungeonSystem->pSaveData->ShortCutsManager->setShortcutTempText(pforcusedcontents(),setStyle(_T("")));
			}
		}
	}
	else if(shortCutState == 1)
	{

		if(g_pPlayerInput()->migi().justOn)
		{
			shortCutSelect_X++;
			if(shortCutSelect_X > 1)
			{
				shortCutSelect_X = 1;
			}
		}
		if(g_pPlayerInput()->hidari().justOn)
		{
			shortCutSelect_X--;
			if(shortCutSelect_X < -1)
			{
				shortCutSelect_X = -1;
			}
		}
		if(g_pPlayerInput()->ue().justOn)
		{
			shortCutSelect_Y--;
			if(shortCutSelect_Y < -1)
			{
				shortCutSelect_Y = -1;
			}
		}
		if(g_pPlayerInput()->shita().justOn)
		{
			shortCutSelect_Y++;
			if(shortCutSelect_Y > 1)
			{
				shortCutSelect_Y = 1;
			}
		}

		//ショートカット
		//通常
		if(
			 g_pPlayerInput()->cancel().on
			|| g_pPlayerInput()->shot().justOn)
		{
			shortCutState = 0;
			playsound_cancel();
		}
		else if(g_pPlayerInput()->decision().justOn)
		{
			if(
				 sg_pDungeonSystem->pSaveData->ShortCutsManager->canAddShortcut(pforcusedcontents()))
			{
				cCoordinate coo;
				coo.Set(shortCutSelect_X,shortCutSelect_Y);
				if(coo.dif())
				{
					sg_pDungeonSystem->pSaveData->ShortCutsManager->addShortcut((eShortCuts_index)coo.GetAspect(),pforcusedcontents());
					shortCutState = 0;
					playsound_decide();
					return true;
				}
			}

		}
	}

	return false;
}