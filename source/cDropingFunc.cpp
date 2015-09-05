#include "stdafx.h"

#include "Language.h"
#include "cDropingFunc.h"
#include "cCommandDroping.h"
#include "FireCommand.h"

#include "cListWindowAdv.h"

cDropingFunc::cDropingFunc(void)
{
	Subject = NULLDROP;
	Verb = _T("");
	Objects.clear();
}

cDropingFunc::cDropingFunc(tstring str)
{
	Subject = NULLDROP;
	Verb = str;
	Objects.clear();
	caption = DisplayVerb();
}

cDropingFunc::~cDropingFunc(void)
{

}

//コマンド実行時処理
int cDropingFunc::Action(IDirect3DDevice9 *pDev)
{

	//FireできるかItemに聞く
	if(isCanFireCommand(Subject, Objects, Verb))
	{//できる
		return Fire(pDev);
	}
	else
	{//できない
	
		//新規のオブジェクトを得る
		vector<vector<pcDroping>> ObjectList;
		vector<StyleString> infolist;
		bool MultiObj = false;

		GetmenuFuncObjectsList(Subject,ObjectList, infolist, MultiObj, Objects.size(), Verb);
	
		MakeSetObjectMenu(pDev, ObjectList, MultiObj, infolist);
	
		return true;
	}

}

//アイテム使用時処理
int cDropingFunc::Fire(IDirect3DDevice9 *pDev)
{

	//コマンドを解決する;
	//Subject->FireCommand(pDev, Verb, Objects);

	return FireCommand(pDev, Verb, Objects, Subject);


}

tstring cDropingFunc::DisplayVerb()
{
	cValiableField val;
	val.tstrings.dim(_T("Verb")) = Verb;
	sg_pDungeonSystem->CutInM().CutIn(sg_pDungeonSystem->pPlayerChara(),コマンド動詞表記_タイミング,val);
	if(val.tstrings.exist(_T("DisplayVerb")))
	{
		return val.tstrings.val(_T("DisplayVerb"));
	}

	return g_Lang(Verb.c_str()).conclete_tstr();
}
	
StyleString cDropingFunc::shortExplanationText()
{
	cValiableField val;
	val.tstrings.dim(_T("Verb")) = Verb;
	val.drops.dim(_T("Subject")) = Subject;
	sg_pDungeonSystem->CutInM().CutIn(sg_pDungeonSystem->pPlayerChara(),コマンド動詞shortExplanationText_タイミング,val);
	if(val.styleStrings.exist(_T("shortExplanationText")))
	{
		return val.styleStrings.val(_T("shortExplanationText"));
	}

	return shortExplanation;
}


//新規の目的語(オブジェクト)を選択するためのリストを作成する。
void cDropingFunc::MakeSetObjectMenu(IDirect3DDevice9 *pDev, vector<pcDroping> &Objects, bool MultiObj, StyleString info)
{
	int i;

	pcControlLayer pccl;
	pcListWindow pclw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	//Info
	pccl->countStringWindow = pcGameWindow(new cGameWindow);
	pccl->countStringWindow->Text() = info;
	pccl->countStringWindow->Init(pDev, info.length()/2, 1);
	pccl->countStringWindow->setLeft(40);
	pccl->countStringWindow->setTop(0);

	//Object
	pccl->WindowList.push_back(pclw = pcListWindow(new cListWindow));

	pcDroping pdrop;
	if(Objects.size() != 0)
	{//なにかある
		for(i=0;i<Objects.size();i++)
		{
			//新規
			pcDropingFunc pcdf = pcDropingFunc(new cDropingFunc(*this));
			pcdf->Objects.push_back(Objects[i]);

			//パック
			pdrop = DropingFuncToDroping(pcdf);

			pclw->pDroplist.push_back(pdrop);
		}	
	}
	else
	{//なにもない
		pclw->pDroplist.push_back(pcDroping(new cDropingNothing));
	}
	pclw->Init(pDev, 20, min(10,pclw->pDroplist.size()));
	pclw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(2));
	pclw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(3));
	pclw->MultiSelect = MultiObj;

}


void cDropingFunc::MakeSetObjectMenu(IDirect3DDevice9 *pDev, vector<vector<pcDroping>> &Objectslist, bool MultiObj, vector<StyleString> infolist)
{
	pcControlLayer pccl;
	pcListWindowAdv pcladw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	pccl->WindowList.push_back(pcladw = pcListWindowAdv(new cListWindowAdv));

	pcDroping pdrop;

	int i,k;
	//vector<pcDroping> エスケープリスト;

	pcladw->vpDroplist.resize(Objectslist.size());
	for(i=0;i<Objectslist.size();i++)
	{
		if(Objectslist[i].size() != 0)
		{//なにかある
			
			for(k=0;k<Objectslist[i].size();k++)
			{
				//新規
				pcDropingFunc pcdf = pcDropingFunc(new cDropingFunc(*this));
				pcdf->Objects.push_back(Objectslist[i][k]);

				//パック
				pdrop = DropingFuncToDroping(pcdf);

				pcladw->vpDroplist[i].push_back(pdrop);
			}
		}
		else
		{//なにもない
			エスケープリスト.clear();
			エスケープリスト.push_back(pcDroping(new cDropingNothing));
			pcladw->vpDroplist[i] = エスケープリスト;
		}
		pcladw->pvpDroplistName.push_back(infolist[i].conclete_tstr());
	}

	pcladw->Init(pDev, 20, 10);
	pcladw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(2));
	pcladw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(3));
	pcladw->MultiSelect = MultiObj;

}

//最新のオブジェクトを返す
pcDroping cDropingFunc::LastObject()
{
	if(Objects.size())
		return Objects[Objects.size()-1];
	else
		return NULLDROP;
}

bool cDropingFunc::canIntoShortCut()
{

	std::vector<tstring> CaptionList;
	if(Subject)
	{
		Subject->EnableShortCutCaptions(CaptionList);
		if(!std::count(CaptionList.begin(), CaptionList.end(), Verb))
		{
			//メニューに無いよ
			return false;
		}
	}
	else
	{
		return false;
	}
	return true;
}

//--------------------------------------------------------
//cStoreDroping
//--------------------------------------------------------
cStoreDroping::cStoreDroping(vector<pcDroping>& store): store_( store )
{

}
cStoreDroping::~cStoreDroping(void)
{

}

//--------------------------------------------------------
//cShopDroping
//--------------------------------------------------------
cShopDroping::cShopDroping(pcDroping pdrop, vector<tstring>& captions)
: item_( pdrop ), captions_( captions )
{

}
cShopDroping::~cShopDroping(void)
{

}
void cShopDroping::Init(IDirect3DDevice9 *pDev,pcDroping self)
{
	cDroping::Init(pDev,self);
	pmemo = item_->pmemo;
}
int cShopDroping::Action(IDirect3DDevice9 *pDev)
{

	vector<tstring>& captions = captions_;


	if(captions.size() == 0) return false;

	pcControlLayer pccl;
	pcSelectWindow pcsw;
	pcListWindow pclw;



	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);
	
	
	//見本
	pccl->WindowList.push_back(pclw = pcListWindow(new cListWindow));
	pclw->pDroplist.push_back(me());
	pclw->enable = false;
	pclw->Init(pDev, 20, pclw->pDroplist.size());
	pclw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(2));
	pclw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(1));



	pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));
	

	int strsize = 0;
	int i;
	for(i=0;i<captions.size();i++)
	{
		cDropingFunc *drfn;
		pcsw->commandList.push_back(pcCommand(drfn = new cDropingFunc(captions[i])));
		drfn->Subject = me();
		drfn->Verb = captions[i];

		strsize = max(strsize,captions[i].size());


		drfn->shortExplanation = GetmenuExplain(drfn->Verb);
	}

	strsize = max(strsize,3);
	strsize = min(strsize,20);

	pcsw->Init(pDev, strsize, pcsw->commandList.size());
	pcsw->setRight(sg_pDungeonSystem->GameScreenInterface.menuPosRight(1));
	pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(3));





	return true;
}

StyleString cShopDroping::shortExplanation()
{
	return item_->shortExplanation();
}
StyleString cShopDroping::longExplanation()
{
	return item_->longExplanation();
}
StyleString cShopDroping::FullName()
{
	return item_->FullName();
}
StyleString cShopDroping::ShortName()
{
	return item_->ShortName();
}
StyleString cShopDroping::ListFullName()
{
	return ExtraHatString() + item_->ListFullName();
}

StyleString cShopDroping::ExtraHatString()
{

	if(item_->shopitemflag() == SHOPFLAG_NONE)
	{
		cMoney money;
		money.value = item_->内包アイテム込み売却受け取り値();
	

		map<tstring, StyleString> valiable;

		valiable[_T("Value")] = money.caption();

		return g_Lang(_T("売り値段Caption"), valiable);
	}
	else
	{
		return _T("");
		//return item_->ExtraHatString();
	}
}