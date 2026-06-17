#include "cShortCuts.h"
#include "cDropingFunc.h"
#include "menuCommand.h"
#include "cGameWindow.h"
#include "cLandform.h"
#include "cDungeonSystem.h"
#include "cBox.h"
#include "gameMainSystem\main.h"
/*
ショートカットの種類は二種類に大別できる
	アイテム系コマンドショートカット
	＞cDropingFunc　登録＆実行可能なのは持ち物のみ
	システムコマンドショートカット
	＞cCommandのサブクラス　登録可能なのは基本メニューのみ。
		基本メニューかどうかを判断するのはpcommandの仕事とする
	
*/

cShortCutsManager::cShortCut::cShortCut(pcCommand pcommand, bool autoFunction)
{
	autoFunction_ = autoFunction;
	init(pcommand);
}
cShortCutsManager::cShortCut::cShortCut(pcDroping pdroping, bool autoFunction)
{
	autoFunction_ = autoFunction;
	init(pdroping);
}
cShortCutsManager::cShortCut::~cShortCut()
{

}
void cShortCutsManager::cShortCut::init(pcCommand pcommand)
{
	pcDropingFunc p_droping_func = boost::dynamic_pointer_cast<cDropingFunc>(pcommand);
	if(p_droping_func != NULL)
	{
		//アイテムコマンド
		pDropingFunc_ = p_droping_func;
	}
	else
	{
		//一般コマンド
		pcommand_ = pcommand;
	}
}
void cShortCutsManager::cShortCut::init(pcDroping pdroping)
{
	
	//一般コマンド
	pDroping_ = pdroping;

}
bool cShortCutsManager::cShortCut::canAddShortcut(pcCommand pcommand)
{
	if(pcommand->canIntoShortCut())
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool cShortCutsManager::cShortCut::canAddShortcut(pcDroping pdroping)
{
	return (pdroping != NULL && pdroping->Holder() == sg_pDungeonSystem->pPlayerChara());
}
		//ショートカット実行可能
bool cShortCutsManager::cShortCut::canShortCut()
{
	if(lostShortCut()) return false;

	if(pcommand_ != NULL)
	{
		//一般コマンド
		if(sg_pDungeonSystem->メニュー表示可能())
		{
			return true;
		}
	}
	else if(pDropingFunc_ != NULL)
	{
		//アイテムコマンド
		if(sg_pDungeonSystem->メニュー表示可能())
		{
			pcDroping psubject = pDropingFunc_->Subject;
			//そのアイテムはもう無い
			if(psubject == NULL || psubject->broken) return false;


			std::vector<tstring> CaptionList;
			psubject->EnableShortCutCaptions(CaptionList);
			if(!std::count(CaptionList.begin(), CaptionList.end(), pDropingFunc_->Verb))
			{
				//メニューに無いよ
				return false;
			}

			return canParentShortCut(psubject);
		}
	}
	else if(pDroping_ != NULL)
	{
		//アイテム
		if(sg_pDungeonSystem->メニュー表示可能())
		{
			pcDroping psubject = pDroping_;
			//そのアイテムはもう無い
			if(psubject == NULL || psubject->broken) return false;


			return canParentShortCut(psubject);
		}
		
	}
	return false;
}
bool cShortCutsManager::cShortCut::canParentShortCut(pcDroping pparent)
{
	if(pparent->Holder() != sg_pDungeonSystem->pPlayerChara())
	{//そのアイテムを持っているのは主人公ではない
		return false;
	}
			
	for(;;)
	{
		pparent = pparent->Parent();
		if(pparent == NULL)
		{
			//直持ち
			return true;
		}
		if(pparent->broken)
		{//親壊れてる
			return false;
		}

		if(pparent->内包ショートカット可())
		{
			continue;
		}
		else
		{
			return false;
		}
	}
	return false;
}
//ショートカット実行
bool cShortCutsManager::cShortCut::doShortCut()
{
	if(canShortCut())
	{
		autoFunction_ = false;
		if(pcommand_ != NULL)
		{
			pcommand_->Action(sg_pDungeonSystem->pDevice_D3D);
			return true;
		}
		else if(pDropingFunc_ != NULL)
		{
			pDropingFunc_->Action(sg_pDungeonSystem->pDevice_D3D);
			return true;
		}
		else if(pDroping_ != NULL)
		{
			pDroping_->Action(sg_pDungeonSystem->pDevice_D3D);
			return true;
		}
	
	}
	return false;
}
bool cShortCutsManager::cShortCut::lostShortCut()
{
	if(pcommand_ != NULL)
	{
		return true;
	}
	else if(pDropingFunc_ != NULL)
	{
		pcDroping psubject = pDropingFunc_->Subject;
		//そのアイテムはもう無い
		if(psubject == NULL || psubject->broken) return true;
	}
	else if(pDroping_ != NULL)
	{
		//そのアイテムはもう無い
		if(pDroping_ == NULL || pDroping_->broken) return true;
	}
	return false;
}
//サブジェクト取得
pcDroping cShortCutsManager::cShortCut::Subject()
{
	if(pcommand_ != NULL)
	{
		return NULLDROP;
	}
	else if(pDropingFunc_ != NULL)
	{
		return pDropingFunc_->Subject;
	}
	else if(pDroping_ != NULL)
	{
		return pDroping_;
	}
	return NULLDROP;
}
//動詞取得
tstring cShortCutsManager::cShortCut::DisplayVerb()
{
	if(pcommand_ != NULL)
	{
		return pcommand_->caption.conclete_tstr();
	}
	else if(pDropingFunc_ != NULL)
	{
		return pDropingFunc_->DisplayVerb();
	}
	else if(pDroping_ != NULL)
	{
		return _T("");
	}
	return _T("");
}
//説明取得
StyleString cShortCutsManager::cShortCut::ShortExplanation()
{
	if(pcommand_ != NULL)
	{
		return pcommand_->shortExplanationText();
	}
	else if(pDropingFunc_ != NULL)
	{
		return pDropingFunc_->shortExplanation;
	}
	else if(pDroping_ != NULL)
	{
		return pDroping_->shortExplanation();
	}
	return _T("");
}
//------------------------------------------------------------------------------------------------------------------
//cShortCutsManager
cShortCutsManager::cShortCutsManager(void)
{
	inittedFlag_ = 0;
}

cShortCutsManager::~cShortCutsManager(void)
{

}

void cShortCutsManager::pri_init()
{
	cCoordinate coo;

	int i;
	for(i=0;i<eShortCuts_index_Num;i++)
	{
		coo.SetAspect(i);
		if( i == eShortCuts_index_Neutral )
		{
			coo.Set(0,0);
		}
		//Info
		shortcutWindow_[i] = pcGameWindow(new cGameWindow);
		shortcutWindow_[i]->Init(sg_pDungeonSystem->pDevice_D3D, 8, 4);
		shortcutWindow_[i]->CenterX = SCREEN_X / 2 + coo.x * shortcutWindow_[i]->Width;
		shortcutWindow_[i]->CenterY = SCREEN_Y / 2 - 64 + coo.y * shortcutWindow_[i]->Height;

	}
	shortcutTempWindow_ = pcGameWindow(new cGameWindow);
	shortcutTempWindow_->Init(sg_pDungeonSystem->pDevice_D3D, 8, 4);
	shortcutTempWindow_->CenterX = SCREEN_X / 2 + coo.x * shortcutWindow_[i]->Width;
	shortcutTempWindow_->CenterY = SCREEN_Y / 2 + coo.y * shortcutWindow_[i]->Height;


	pri_setShortcutText(shortcutWindow_[eShortCuts_index_Neutral], NULLDROP, g_Lang(_T("ショートカット")), false);

	inittedFlag_ = 1;
}

void cShortCutsManager::addShortcut(eShortCuts_index index, pcCommand pcommand, bool autoFunction)
{
	if(canAddShortcut(pcommand))
	{

		shortcut_[index] = pcShortCut(new cShortCut(pcommand, autoFunction));


		resetShortcutText(index);
	}
}
void cShortCutsManager::addShortcut(eShortCuts_index index, pcDroping pdroping, bool autoFunction)
{
	if(canAddShortcut(pdroping))
	{

		shortcut_[index] = pcShortCut(new cShortCut(pdroping, autoFunction));


		resetShortcutText(index);
	}
}
void cShortCutsManager::eraseShortcut(eShortCuts_index index) {
	shortcut_[index] = pcShortCut((cShortCut*) NULL);
}
void cShortCutsManager::doShortcut(eShortCuts_index index)
{
	
	if(shortcut_[index] != NULL && shortcut_[index]->canShortCut())
	{
		shortcut_[index]->doShortCut();
	}
}
bool cShortCutsManager::isShortcutAutomatic(eShortCuts_index index)
{
	if(shortcut_[index] != NULL)
	{
		return shortcut_[index]->autoFunction();
	}
	return false;
}
void cShortCutsManager::resetShortcutText(eShortCuts_index index)
{
	if(!inittedFlag_) pri_init();
	if( shortcut_[index] == NULL || shortcut_[index]->lostShortCut())
	{
		shortcut_[index] = pcShortCut((cShortCut*)NULL);
		shortcutWindow_[index]->Text().clear();
		return;
	}
	StyleString str;
	pcDroping pdrop = shortcut_[index]->Subject();

	pri_setShortcutText(shortcutWindow_[index], pdrop, setStyle(shortcut_[index]->DisplayVerb()), shortcut_[index]->autoFunction());
}

void cShortCutsManager::setShortcutTempText(pcDroping pdrop, const StyleString& Verb)
{
	if(!inittedFlag_) pri_init();
	pri_setShortcutText(shortcutTempWindow_, pdrop, Verb, false);
	shortcutTempDrop_ = pdrop;
}

void cShortCutsManager::pri_setShortcutText(pcGameWindow pwindow, pcDroping pdrop, const StyleString& Verb, bool autoFunction)
{
	StyleString str;

	pwindow->Text().clear();
	pwindow->Text().addstring(_T("\n"));//改行
	if(pdrop != NULL)
	{
		//名前
		
		str = pdrop->FullName();
		
		pwindow->FitTextWidthToWindow(str);
	}
	else
	{
		str = g_Lang(_T("メニュー"));
		pwindow->FitTextWidthToWindow(str);
	}
	pwindow->Text().addstring(str);

	pwindow->Text().addstring(_T("\n"));//改行

	str = Verb;

	if(str.conclete_tstr() != _T(""))
	{
		//名前
		pwindow->FitTextWidthToWindow(str);
	}
	else
	{
		str = g_Lang(_T("メニュー"));
		pwindow->FitTextWidthToWindow(str);
	}
	pwindow->Text().addstring(str);
	
	if(autoFunction) {
		pwindow->Text().addstring(_T("\n"));//改行
		str = g_Lang(_T("自動shortCutTag"));
		pwindow->FitTextWidthToWindow(str);
		pwindow->Text().addstring(str);
	}
	
	pwindow->ReRendarText();
}
	
	
void cShortCutsManager::resetAllShortcutText()
{
	int i;
	if(!inittedFlag_) pri_init();
	for(i=0;i<eShortCuts_index_Num;i++)
	{
		resetShortcutText((eShortCuts_index)i);
	}
}

void cShortCutsManager::refreshAutomaticShortCut(vector<pcDroping>& itemList)
{
	//削除
	int index;
	for(index=0;index<eShortCuts_index_Num;index++)
	{
		if(shortcut_[index] == NULL || shortcut_[index]->lostShortCut()) {
			eraseShortcut((eShortCuts_index)index);
		}
	}



	int i,size = itemList.size();
	for(i=0;i<size;i++) {

		pcDroping subject = itemList[i];

		tryAddShortcutForSource(subject);

	}


}
void cShortCutsManager::tryAddShortcutForSource(pcDroping source)
{
	//現在の登録確認
	set<int> itemID;
	int index;
	for(index=0;index<eShortCuts_index_Num;index++)
	{
		if(shortcut_[index] != NULL) {
			pcDroping subject = shortcut_[index]->Subject();
			if( subject ) {

				itemID.insert(subject->ID());
			}
		}
	}

	if(itemID.count(source->ID()) == 0) {
		pcShortCut shortCut = makeAutoShortCut(source);
		if(shortCut) {
			addShortcutToEmpty(shortCut);
		}
		pcBox pBox = boost::dynamic_pointer_cast<cBox>(source);
		if(pBox && pBox->内容物使用可()) {
			int i,size = pBox->includedItem.size();
			for(i=0;i<size;i++) {
				pcDroping subject = pBox->includedItem[i];
				tryAddShortcutForSource(subject);
			}
		}
	}
}
void cShortCutsManager::addShortcutToEmpty(pcShortCut shortcut)
{
	eShortCuts_index priority[] = {
		eShortCuts_index_Up,
		eShortCuts_index_Down,
		eShortCuts_index_Left,
		eShortCuts_index_Right,
		eShortCuts_index_UpRight,
		eShortCuts_index_UpLeft,
		eShortCuts_index_DownRight,
		eShortCuts_index_DownLeft,
	};

	int i, size = sizeof(priority)/sizeof(eShortCuts_index);
	for (i=0; i<size ;i++) {
		if(shortcut_[priority[i]] == NULL) {
			shortcut_[priority[i]] = shortcut;
			resetShortcutText(priority[i]);
			return;
		}
	}
}

cShortCutsManager::pcShortCut cShortCutsManager::makeAutoShortCut(pcDroping source)
{
	vector<tstring> CaptionList;
	source->EnableShortCutCaptions(CaptionList);

	pcShortCut shortCut;
	shortCut = makeAutoShortCutWithVerb(source, CaptionList, (tstring)_T("撃つ"));
	return shortCut;
}

cShortCutsManager::pcShortCut cShortCutsManager::makeAutoShortCutWithVerb(pcDroping source, const vector<tstring>& CaptionList, const tstring& verb)
{
	if(std::count(CaptionList.begin(), CaptionList.end(), verb))
	{
		cDropingFunc *drfn;
		pcCommand command = pcCommand(drfn = new cDropingFunc(verb));
		drfn->Subject = source;
		drfn->Verb = verb;
		drfn->shortExplanation = source->GetmenuExplain(drfn->Verb);
		pcShortCut shortCut = pcShortCut(new cShortCut(command, true));
		if(shortCut->canShortCut()) {
			return shortCut;
		}
	}
	return pcShortCut( (cShortCut*)(NULL));
}

void cShortCutsManager::drawWindow(eShortCuts_index forcus)
{
	int i;
	if(!inittedFlag_) pri_init();
	if(eShortCuts_index_Num > forcus)
	{
		
		for(i=0;i<eShortCuts_index_Num;i++)
		{

			if( i == forcus )
			{
				shortcutWindow_[i]->color.ARGB(255,255,255,255);
			}
			else
			{
				shortcutWindow_[i]->color.ARGB(128,200,200,200);
			}

			if( (shortcut_[i] == NULL || !shortcut_[i]->canShortCut()) 
				&& i != eShortCuts_index_Neutral)
			{
				cColor color(255,128,128,128);
				shortcutWindow_[i]->color *= color;
			}
		}
	}
	else
	{
		for(i=0;i<eShortCuts_index_Num;i++)
		{

			shortcutWindow_[i]->color.ARGB(255,255,255,255);
			
			if( (shortcut_[i] == NULL || !shortcut_[i]->canShortCut())
				&& i != eShortCuts_index_Neutral )
			{
				cColor color(255,128,128,128);
				shortcutWindow_[i]->color *= color;
			}
		}
	}

	for(i=0;i<eShortCuts_index_Num;i++)
	{
		shortcutWindow_[i]->Draw(sg_pDungeonSystem->pDevice_D3D);
		pcDroping pdrop;
		if(shortcut_[i] != NULL)
		{
			pdrop = shortcut_[i]->Subject();
		}

		if(pdrop != NULL)
		{
			//アイコン
			pdrop->DrawIcon(sg_pDungeonSystem->pDevice_D3D,shortcutWindow_[i]->CenterX,
				shortcutWindow_[i]->Top() + shortcutWindow_[i]->sideEdgeWith() + shortcutWindow_[i]->linetop(0));
			pdrop->DrawStateIcon(sg_pDungeonSystem->pDevice_D3D,shortcutWindow_[i]->CenterX,
				shortcutWindow_[i]->Top() + shortcutWindow_[i]->sideEdgeWith() + shortcutWindow_[i]->linetop(0));

		}
		
	}

}
void cShortCutsManager::drawTempWindow(eShortCuts_index forcus)
{
	if(!inittedFlag_) pri_init();
	if(forcus == eShortCuts_index_NoIndex) {
		forcus = eShortCuts_index_Neutral;
	}
	shortcutTempWindow_->CenterX = shortcutWindow_[forcus]->CenterX + 16;
	shortcutTempWindow_->CenterY = shortcutWindow_[forcus]->CenterY + 16;

	shortcutTempWindow_->Draw(sg_pDungeonSystem->pDevice_D3D);

	if(shortcutTempDrop_ != NULL)
	{
		//アイコン
		shortcutTempDrop_->DrawIcon(sg_pDungeonSystem->pDevice_D3D,shortcutTempWindow_->CenterX,
			shortcutTempWindow_->Top() + shortcutTempWindow_->sideEdgeWith() + shortcutTempWindow_->linetop(0));
		shortcutTempDrop_->DrawStateIcon(sg_pDungeonSystem->pDevice_D3D,shortcutTempWindow_->CenterX,
			shortcutTempWindow_->Top() + shortcutTempWindow_->sideEdgeWith() + shortcutTempWindow_->linetop(0));

	}
}
bool cShortCutsManager::canAddShortcut(pcCommand pcommand)
{
	return cShortCut::canAddShortcut(pcommand);
}
bool cShortCutsManager::canAddShortcut(pcDroping pdroping)
{
	return cShortCut::canAddShortcut(pdroping);
}
StyleString cShortCutsManager::ShortExplanation(eShortCuts_index forcus)
{
	if(!inittedFlag_) pri_init();

	if(shortcut_[forcus] != NULL)
	{
		return shortcut_[forcus]->ShortExplanation();
	}
	else
	{
		return _T("");
	}
}


 // namespace