#include "stdafx.h"

#include "Language.h"
#include "menuCommand.h"

#include "cListWindow.h"
#include "cListWindowAdv.h"

#include "cMessageDroping.h"
#include "ConfigCommands.h"

#include "Gameobjects.h"

#include "cFloor.h"

#include "csTitle.h"

#include "scriptprocesser/DropsScript.h"

//----------------------------------------------------
//情報
//----------------------------------------------------
void inforOfHero(StyleString & infor)
{

	pcCharacter pchara = sg_pDungeonSystem->pPlayerChara();
	int power;
	pcEquipment pcEqu;
	pcEqu = boost::dynamic_pointer_cast<cEquipment>(pchara->attackequipment);
	power = 0;
	if(pcEqu != NULL) power = (int)pcEqu->武器力最終値();


	map<tstring, StyleString> valiable;


	valiable[_T("ArmP")].addstring(power,_T("%4d"));
	valiable[_T("AtkP")].addstring((int)pchara->GetAttackPower(),_T("%4d"));
	valiable[_T("StmP")].addstring((int)(pchara->Stomach+0.999),_T("%3d"));
	valiable[_T("StmPMAX")].addstring((int)pchara->MaxStomach,_T("%3d"));
	pcEqu = boost::dynamic_pointer_cast<cEquipment>(pchara->defenseequipment);
	power = 0;
	if(pcEqu != NULL) power = (int)pcEqu->防具力最終値();
	valiable[_T("ShdP")].addstring(power,_T("%4d"));
	valiable[_T("DefP")].addstring((int)pchara->GetDefencePower(),_T("%4d"));
	valiable[_T("ExpP")].addstring(pchara->EXP,_T("%8d"));

	valiable[_T("Turn")].addstring(sg_pDungeonSystem->SumTurnCount(),_T("%6d"));

	double sumtime = sg_pDungeonSystem->Sumtime();
	valiable[_T("TimeSec")].addstring(((int)sumtime)%60,_T("%2d"));
	valiable[_T("TimeMin")].addstring((((int)sumtime)/60)%60,_T("%2d"));
	valiable[_T("TimeHour")].addstring(((int)sumtime)/60/60,_T("%3d"));

	g_Lang(_T("メインメニューサブウィンドウテキスト"), infor, valiable);


/*
	infor.addstring(g_Lang(_T("__武器_")));
	infor.addstring(power,_T("%4d"));

	infor.addstring(_T("    _"));
	infor.addstring(g_Lang(_T("攻撃力_")));
	infor.addstring((int)pchara->GetAttackPower(),_T("%4d"));

	infor.addstring(_T("    _"));
	infor.addstring(g_Lang(_T("満腹度___")));
	infor.addstring();
	infor.addstring(_T("/"));
	infor.addstring((int)pchara->MaxStomach,_T("%3d"));

	infor.addstring(_T("\n"));


	pcEqu = dynamic_cast<cEquipment *>(pchara->defenseequipment.get());
	power = 0;
	if(pcEqu != NULL) power = pcEqu->防具力最終値();
	
	infor.addstring(g_Lang(_T("__防具_")));
	infor.addstring(power,_T("%4d"));

	infor.addstring(_T("    _"));
	infor.addstring(g_Lang(_T("防御力_")));
	infor.addstring((int)pchara->GetDefencePower(),_T("%4d"));
	
	infor.addstring(_T("    _"));
	infor.addstring(g_Lang(_T("経験値_")));
	infor.addstring(pchara->EXP,_T("%8d"));

	infor.addstring(_T("\n"));
	*/

}



//道具
vector<pcDroping> 足元リスト;
vector<pcDroping> エスケープリスト;
int cCommand_menuItem::Action(IDirect3DDevice9 *pDev)
{
	pcControlLayer pccl;
	pcListWindowAdv pcladw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	pccl->WindowList.push_back(pcladw = pcListWindowAdv(new cListWindowAdv));

	pcDroping pdrop;
	if(sg_pDungeonSystem->pPlayerChara()->holdItem.size() != 0)
	{//なにかある
		pcladw->vpDroplist.push_back(sg_pDungeonSystem->pPlayerChara()->holdItem);
		pcladw->pvpDroplist_forSort.push_back(&sg_pDungeonSystem->pPlayerChara()->holdItem);
	}
	else
	{//なにもない
		エスケープリスト.clear();
		エスケープリスト.push_back(pcDroping(new cDropingNothing));
		pcladw->pvpDroplist_forSort.push_back(&エスケープリスト);
		pcladw->vpDroplist.push_back(エスケープリスト);
	}
	pcladw->pvpDroplistName.push_back(g_Lang(_T("道具")).conclete_tstr());

	if((pdrop = sg_pDungeonSystem->キャラ足元地形(sg_pDungeonSystem->pPlayerChara())->pOnDrop) != NULL)
	{//足元になにかある
		足元リスト.clear();
		足元リスト.push_back(pdrop);
		pcladw->pvpDroplist_forSort.push_back(&足元リスト);
		pcladw->vpDroplist.push_back(足元リスト);
	}
	else
	{//足元になにもない
		エスケープリスト.clear();
		エスケープリスト.push_back(pcDroping(new cDropingNothing));
		pcladw->pvpDroplist_forSort.push_back(&エスケープリスト);
		pcladw->vpDroplist.push_back(エスケープリスト);
	}
	pcladw->pvpDroplistName.push_back(g_Lang(_T("足元")).conclete_tstr());
	
	pcladw->Init(pDev, 20, 10);
	pcladw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(2));
	pcladw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(3));
	
	return true;
};
StyleString cCommand_menuItem::shortExplanationText()
{
	StyleString infor;
	inforOfHero(infor);
	return infor;
}

//足元
int cCommand_menuFoot::Action(IDirect3DDevice9 *pDev)
{
	vector<pcDroping> vpdrop;

	pcDroping pdrop;
	if((pdrop = sg_pDungeonSystem->キャラ足元地形(sg_pDungeonSystem->pPlayerChara())->pOnDrop) != NULL)
	{//足元になにかある
		vpdrop.push_back(pdrop);
	}

	if(vpdrop.size() == 1)
	{
		vpdrop[0]->Action(pDev);
	}
	else
	{

		if(vpdrop.empty())
		{
			vpdrop.push_back(pcDroping(new cDropingNothing));
		}

		pcControlLayer pccl;
		pcListWindow pclw;

		//新規コントロールレイヤー
		sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
		pccl->Init(pDev);

		pccl->WindowList.push_back(pclw = pcListWindow(new cListWindow));



		pclw->pDroplist = vpdrop;

		pclw->Init(pDev, 20, pclw->pDroplist.size());
		pclw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(2));
		pclw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(2));
		
	}

	return true;
};
StyleString cCommand_menuFoot::shortExplanationText()
{
	StyleString infor;
	inforOfHero(infor);
	return infor;
}



//状態
int cCommand_menuStates::Action(IDirect3DDevice9 *pDev)
{

	sg_pDungeonSystem->pPlayerChara()->Condition.MakeExplainWindow(pDev);

	return true;
};
StyleString cCommand_menuStates::shortExplanationText()
{
	StyleString infor;
	inforOfHero(infor);
	return infor;
}


//その他
int cCommand_menuOther::Action(IDirect3DDevice9 *pDev)
{
	pcControlLayer pccl;
	pcSelectWindow pcsw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

	pcsw->commandList.push_back(pcCommand(new cCommand_menuOther_MessageHistory ));
	pcsw->commandList.push_back(pcCommand(new cCommandConfigConfig ));

#ifdef _UNRELEASE	

	pcsw->commandList.push_back(pcCommand(new cCommand_menuOther_Summon ));
	pcsw->commandList.push_back(pcCommand(new cCommand_menuOther_Make ));
	pcsw->commandList.push_back(pcCommand(new cCommand_menuOther_Get ));
	pcsw->commandList.push_back(pcCommand(new cCommand_menuOther_String ));
	//pcsw->commandList.push_back(pcCommand(new cCommand_menuOther_ReImport ));
	//pcsw->commandList.push_back(pcCommand(new cCommand_menuOther_ReImportGraphic ));
	//pcsw->commandList.push_back(pcCommand(new cCommand_menuOther_ImportDataCheck ));
#else
	#ifdef _DEBUG
		pcsw->commandList.push_back(pcCommand(new cCommand_menuOther_Summon ));
		pcsw->commandList.push_back(pcCommand(new cCommand_menuOther_Make ));
	#endif
#endif


	if(!sg_pDungeonSystem->拠点フラグ()) pcsw->commandList.push_back(pcCommand(new cCommand_menuOther_RetireChoice ));
	else pcsw->commandList.push_back(pcCommand(new cCommand_menuOther_ReturnTitle ));

	pcsw->Init(pDev, 8, pcsw->commandList.size());
	pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(2));
	pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(2));



	return true;

};
StyleString cCommand_menuOther::shortExplanationText()
{
	StyleString infor;
	inforOfHero(infor);
	return infor;
}

//メッセージ履歴
int cCommand_menuOther_MessageHistory::Action(IDirect3DDevice9 *pDev)
{
	pcControlLayer pccl;
	pcListWindow pclw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	//新規リストウィンドウ
	pccl->WindowList.push_back(pclw = pcListWindow(new cListWindow));

	
	int i;
	for(i=0;i<sg_pDungeonSystem->MessageW().Messagelogs.size();i++)
	{
		pcDroping pdrop;
		pclw->pDroplist.push_back(pdrop = pcDroping(new cMessageDroping(sg_pDungeonSystem->MessageW().Messagelogs[i]) ));
		pdrop->Init(pDev,pdrop);
	}
	if(i == 0)
	{//メッセージ無し
		pcDroping pdrop;
		pclw->pDroplist.push_back(pdrop = pcDroping(new cMessageDroping(psstring(new StyleString(g_Lang(_T("メッセージ履歴なし"))))) ));
		pdrop->Init(pDev,pdrop);
	}

	pclw->Init(pDev, 26, 10);
	pclw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(2));
	pclw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(2));


	pclw->SetCursolTolast();

	return true;
};
StyleString cCommand_menuOther_MessageHistory::shortExplanationText()
{return g_Lang(_T("メッセージ履歴サブ説明"));}

//-------------------------------------------------------------------------------------
//デバッグ
//-------------------------------------------------------------------------------------
//デバッグ召喚
int cCommand_menuOther_Summon::Action(IDirect3DDevice9 *pDev)
{
	pcControlLayer pccl;
	pcSelectWindow pcsw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	//新規セレクトウィンドウ
	pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

	vector<pcCharacter> charalist;
	sg_pDungeonSystem->DataBase.makeAllCharaList(charalist);

	int i;
	for(i=0;i<charalist.size();i++)
	{
		cCommand_menuOther_Summon_Chara* pccm;
		pcsw->commandList.push_back(pcCommand(pccm = new cCommand_menuOther_Summon_Chara(charalist[i]->FullName()) ));
		
		pccm->chara_ID = charalist[i]->ID();
	}
	pcsw->Init(pDev, 20, 10);
	pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(3));
	pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(2));

	return true;
};
StyleString cCommand_menuOther_Summon::shortExplanationText()
{return _T("敵キャラクターを召喚する。\n");}

int cCommand_menuOther_Summon_Chara::Action(IDirect3DDevice9 *pDev)
{
	pcControlLayer pccl;
	pcSelectWindow pcsw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	//新規セレクトウィンドウ
	pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

	int i;
	for(i=0;i<5;i++)
	{
		TCHAR t[16];
		_stprintf(t,_T("CLASS %d "),i);
		cCommand_menuOther_Summon_Chara_Class* pccm;
		pcsw->commandList.push_back(pcCommand(
			pccm = new cCommand_menuOther_Summon_Chara_Class(t + setStyle(_T("ENEMY"),0xFFFF5050))));
		
		pccm->chara_ID = chara_ID;
		pccm->CLASS = i;
		pccm->Forse = CHARACTER_FORSE_ENEMY;
	}
	for(i=0;i<5;i++)
	{
		TCHAR t[16];
		_stprintf(t,_T("CLASS %d "),i);
		cCommand_menuOther_Summon_Chara_Class* pccm;
		pcsw->commandList.push_back(pcCommand(
			pccm = new cCommand_menuOther_Summon_Chara_Class(t + setStyle(_T("FRIEND"),0xFF50FF50))));
		
		pccm->chara_ID = chara_ID;
		pccm->CLASS = i;
		pccm->Forse = CHARACTER_FORSE_FRIEND;
	}
	pcsw->Init(pDev, 20, 5);
	pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(4));
	pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(2));

	return true;
};
StyleString cCommand_menuOther_Summon_Chara::shortExplanationText()
{
	pcCharacter pchara = sg_pDungeonSystem->DataBase.GetSampleCharacter(chara_ID);
	tstring str;
	TCHAR t[16];
	_stprintf(t,_T("ID %d\n"),chara_ID);
	str = (t + pchara->titleName() + _T("\n")+ pchara->FullName() + _T("\n")).conclete_tstr();
	return str;
}

int cCommand_menuOther_Summon_Chara_Class::Action(IDirect3DDevice9 *pDev)
{
	pcCharacter pchara = sg_pDungeonSystem->DataBase.GetSampleCharacter(chara_ID);
	sg_pDungeonSystem->キャラクター生成(chara_ID,CLASS,Forse,
		sg_pDungeonSystem->Map().Land(sg_pDungeonSystem->pPlayerChara()->placeX,sg_pDungeonSystem->pPlayerChara()->placeY));
	sg_pDungeonSystem->resetTurnInformation(pDev);
	return true;
};
StyleString cCommand_menuOther_Summon_Chara_Class::shortExplanationText()
{
	pcCharacter pchara = sg_pDungeonSystem->DataBase.GetSampleCharacter(chara_ID);
	pcMob pmob = boost::dynamic_pointer_cast<cMob>(pchara);


	//pmob->CLASS = CLASS;
	pchara->CLASS = CLASS;
	
	pchara->Init(sg_pDungeonSystem->pDevice_D3D,pchara);
	
	StyleString str;
	TCHAR t[256];
	//MHP
	_stprintf(t,_T("HP %d  :"),pchara->SetMHP());
	str += setStyle(t,0xFF50FF50);

	_stprintf(t,_T("EXP %d\n"),pchara->HaveEXP());
	str += setStyle(t,0xFF5080FF);
	
	_stprintf(t,_T("基礎攻撃力 %d  :"),pchara->GetAttackPower());
	str += setStyle(t,0xFFFF5050);

	_stprintf(t,_T("基礎防御力 %d\n"),pchara->GetDefencePower());
	str += setStyle(t,0xFFFFFF50);

	_stprintf(t,_T("特殊発動率 %d  :"),pchara->PerOfspecialAttack());
	str += setStyle(t,0xFFFF50FF);
	
	if(pmob != NULL)
	{
		_stprintf(t,_T("出現階 %d  :\n"),pmob->appearFloor(CLASS));
		str += setStyle(t,0xFFFFFFFF);
	}

	set<キャラ属性::キャラ属性リスト> tempset = pchara->CharaAttribute();
	set<キャラ属性::キャラ属性リスト>::iterator itr = tempset.begin();
	for(;itr!=tempset.end();itr++)
	{
		str += setStyle(sg_pDungeonSystem->DataBase.charattrI2S(*itr),0xFFFFFFFF);
		str += _T(" ");
	}

	return str;
}

//デバッグ生成
int cCommand_menuOther_Make::Action(IDirect3DDevice9 *pDev)
{
	pcControlLayer pccl;
	pcSelectWindow pcsw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	//新規セレクトウィンドウ
	pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

	//ドロップリスト
	vector<pcDroping> droplist;
	sg_pDungeonSystem->DataBase.makeAllDropList(droplist);

	pcsw->commandList.push_back(pcCommand(new cCommand_menuOther_Make_randomDrop ));
		
	int i;
	for(i=0;i<droplist.size();i++)
	{
		cCommand_menuOther_Make_Drop* pccm;
		pcsw->commandList.push_back(pcCommand(pccm = new cCommand_menuOther_Make_Drop(setStyle(droplist[i]->ID())+_T(": ")+ droplist[i]->FullNameBase()) ));
		
		pccm->Drop_ID = droplist[i]->ID();
	}
	pcsw->Init(pDev, 20,10);
	pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(4));
	pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(2));

	return true;
};
StyleString cCommand_menuOther_Make::shortExplanationText()
{return _T("アイテム類を生成する。\n");}

//ランダムアイテム生成
int cCommand_menuOther_Make_randomDrop::Action(IDirect3DDevice9 *pDev)
{
	
	unsigned int itemID = sg_pDungeonSystem->pDungeon()->pfloor(sg_pDungeonSystem->FloorLevel())->dropitemDP().get(random());
	pcDroping pdrop = sg_pDungeonSystem->落ち物生成(itemID,NULL,
		sg_pDungeonSystem->Map().Land(sg_pDungeonSystem->pPlayerChara()->placeX,sg_pDungeonSystem->pPlayerChara()->placeY));
	
	if(pdrop == NULL) return true;

	sg_pDungeonSystem->アイテム強制マッピング(pdrop);

	sg_pDungeonSystem->resetTurnInformation(pDev);

	return true;
};
StyleString cCommand_menuOther_Make_randomDrop::shortExplanationText()
{
	return _T("今のフロアの設定でランダムアイテムを生成する。\n");
}

//アイテム
int cCommand_menuOther_Make_Drop::Action(IDirect3DDevice9 *pDev)
{

	pcControlLayer pccl;
	pcSelectWindow pcsw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	//新規セレクトウィンドウ
	pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));



	cCommand_menuOther_Make_Drop_State* pccm;
	cCommand_menuOther_Make_Drop_Natural* pcnm;

	pcsw->commandList.push_back(pcCommand(
		pcnm = new cCommand_menuOther_Make_Drop_Natural));
	pcnm->Drop_ID = Drop_ID;

	pcsw->commandList.push_back(pcCommand(
		pccm = new cCommand_menuOther_Make_Drop_State(_T("通常状態")) ));
	pccm->state = cDroping::STATE_NORMAL;
	pccm->Drop_ID = Drop_ID;

	pcsw->commandList.push_back(pcCommand(
		pccm = new cCommand_menuOther_Make_Drop_State(_T("呪い状態")) ));
	pccm->state = cDroping::STATE_CURSE;
	pccm->Drop_ID = Drop_ID;

	pcsw->commandList.push_back(pcCommand(
		pccm = new cCommand_menuOther_Make_Drop_State(_T("信仰状態")) ));
	pccm->state = cDroping::STATE_GOOD;
	pccm->Drop_ID = Drop_ID;



	pcsw->Init(pDev, 8,4);
	pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(4));
	pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(1));

	return true;
};
StyleString cCommand_menuOther_Make_Drop::shortExplanationText()
{
	pcDroping pdrop = sg_pDungeonSystem->DataBase.GetSampleDroping(Drop_ID);
	StyleString str;
	TCHAR t[16];
	_stprintf(t,_T("ID %d : "),Drop_ID);
	str = setStyle(t,0xFFCCCCBB) + pdrop->shortExplanation() + _T("\n");
	return str;
}

int cCommand_menuOther_Make_Drop_Natural::Action(IDirect3DDevice9 *pDev)
{
	pcDroping pdrop = sg_pDungeonSystem->落ち物生成(Drop_ID,NULL,
		sg_pDungeonSystem->Map().Land(sg_pDungeonSystem->pPlayerChara()->placeX,sg_pDungeonSystem->pPlayerChara()->placeY));
	
	if(pdrop == NULL) return true;

	sg_pDungeonSystem->アイテム強制マッピング(pdrop);

	sg_pDungeonSystem->resetTurnInformation(pDev);

	return true;
};
StyleString cCommand_menuOther_Make_Drop_Natural::shortExplanationText()
{
	pcDroping pdrop = sg_pDungeonSystem->DataBase.GetSampleDroping(Drop_ID);
	StyleString str;
	TCHAR t[16];
	_stprintf(t,_T("ID %d : "),Drop_ID);
	str = setStyle(t,0xFFCCCCBB) + pdrop->shortExplanation() + _T("\n");
	str.addstring( _T("設定された確率でアイテムをランダム初期化する。\n"));
	return str;
}

//状態
int cCommand_menuOther_Make_Drop_State::Action(IDirect3DDevice9 *pDev)
{

	pcControlLayer pccl;
	pcSelectWindow pcsw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	//新規セレクトウィンドウ
	pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

	int i;
	for(i=0;i<100;i++)
	{
		cCommand_menuOther_Make_Drop_State_Quality* pccm;
		pcsw->commandList.push_back(pcCommand(
			pccm = new cCommand_menuOther_Make_Drop_State_Quality( _T(" + ") + setStyle(i) ) ));
		
		
		pccm->quality = i;
		pccm->Drop_ID = Drop_ID;
		pccm->state = state;
	}


	for(i=1;i<100;i++)
	{
		cCommand_menuOther_Make_Drop_State_Quality* pccm;
		pcsw->commandList.push_back(pcCommand(
			pccm = new cCommand_menuOther_Make_Drop_State_Quality( _T(" - ") + setStyle(i) ) ));
		
		
		pccm->quality = -i;
		pccm->Drop_ID = Drop_ID;
		pccm->state = state;
	}

	pcsw->Init(pDev, 20,10);
	pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(5));
	pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(2));

	return true;
};
StyleString cCommand_menuOther_Make_Drop_State::shortExplanationText()
{
	return _T("");
}

//修正値
int cCommand_menuOther_Make_Drop_State_Quality::Action(IDirect3DDevice9 *pDev)
{

	pcDroping pdrop = sg_pDungeonSystem->DataBase.GetSampleDroping(Drop_ID);
	pdrop = sg_pDungeonSystem->落ち物生成(Drop_ID,NULL,
		sg_pDungeonSystem->Map().Land(sg_pDungeonSystem->pPlayerChara()->placeX,sg_pDungeonSystem->pPlayerChara()->placeY));
	
	if(pdrop == NULL) return true;

	sg_pDungeonSystem->アイテム強制マッピング(pdrop);
	pdrop->state() = state;
	pdrop->quality() = quality;

	sg_pDungeonSystem->resetTurnInformation(pDev);
	return true;
};
StyleString cCommand_menuOther_Make_Drop_State_Quality::shortExplanationText()
{
	return _T("");
}
//----------------------------------------------------------------------------------------------------------------
int cCommand_menuOther_Get::Action(IDirect3DDevice9 *pDev)
{
	pcControlLayer pccl;
	pcSelectWindow pcsw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	//新規セレクトウィンドウ
	pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

	vector<pcCharacter> charalist;
	sg_pDungeonSystem->DataBase.makeAllCharaList(charalist);


	//pcsw->commandList.push_back(pcCommand(pccm = new cCommand_menuOther_String_AllItemShortExplanation() ));
	pcsw->commandList.push_back(pcCommand(new cCommand_menuOther_Get_EquipUp() ));
	pcsw->commandList.push_back(pcCommand(new cCommand_menuOther_Get_MoneyUp() ));
	pcsw->commandList.push_back(pcCommand(new cCommand_menuOther_Get_Level10Up() ));
	pcsw->commandList.push_back(pcCommand(new cCommand_menuOther_Get_MaxHP100Up() ));
		

	pcsw->Init(pDev, 10, 4);
	pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(3));
	pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(2));

	return true;
};
StyleString cCommand_menuOther_Get::shortExplanationText()
{
	return StyleString(_T(""));
}

int cCommand_menuOther_Get_EquipUp::Action(IDirect3DDevice9 *pDev)
{
	pcEquipment pcEqu = boost::dynamic_pointer_cast<cEquipment>(sg_pDungeonSystem->pPlayerChara()->attackequipment);
	if(pcEqu) {
		pcEqu->熟練度カウント加算(20000);
	}
	pcEqu = boost::dynamic_pointer_cast<cEquipment>(sg_pDungeonSystem->pPlayerChara()->defenseequipment);
	if(pcEqu) {
		pcEqu->熟練度カウント加算(20000);
	}
	return true;
};
StyleString cCommand_menuOther_Get_EquipUp::shortExplanationText()
{
	return StyleString(_T(""));
}

int cCommand_menuOther_Get_MoneyUp::Action(IDirect3DDevice9 *pDev)
{
	sg_pDungeonSystem->主人公お金増減(+100000);

	return true;
};
StyleString cCommand_menuOther_Get_MoneyUp::shortExplanationText()
{
	return StyleString(_T(""));
}

int cCommand_menuOther_Get_Level10Up::Action(IDirect3DDevice9 *pDev)
{
	sg_pDungeonSystem->レベルアップ(sg_pDungeonSystem->pPlayerChara(), 10);

	return true;
};
StyleString cCommand_menuOther_Get_Level10Up::shortExplanationText()
{
	return StyleString(_T(""));
}

int cCommand_menuOther_Get_MaxHP100Up::Action(IDirect3DDevice9 *pDev)
{
	sg_pDungeonSystem->最大HP拡張要請(sg_pDungeonSystem->pPlayerChara(), 100);

	return true;
};
StyleString cCommand_menuOther_Get_MaxHP100Up::shortExplanationText()
{
	return StyleString(_T(""));
}
//----------------------------------------------------------------------------------------------------------------
int cCommand_menuOther_String::Action(IDirect3DDevice9 *pDev)
{
	pcControlLayer pccl;
	pcSelectWindow pcsw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	//新規セレクトウィンドウ
	pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

	vector<pcCharacter> charalist;
	sg_pDungeonSystem->DataBase.makeAllCharaList(charalist);


	//pcsw->commandList.push_back(pcCommand(pccm = new cCommand_menuOther_String_AllItemShortExplanation() ));
	pcsw->commandList.push_back(pcCommand(new cCommand_menuOther_String_AllItemLongExplanation() ));
	pcsw->commandList.push_back(pcCommand(new cCommand_menuOther_String_AllMassage() ));
		

	pcsw->Init(pDev, 10, 2);
	pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(3));
	pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(2));

	return true;
};
StyleString cCommand_menuOther_String::shortExplanationText()
{
	return StyleString(_T(""));
}
//デバッグ生成
int cCommand_menuOther_String_AllItemLongExplanation::Action(IDirect3DDevice9 *pDev)
{
	pcControlLayer pccl;
	pcSelectWindow pcsw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	//新規セレクトウィンドウ
	pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

	//ドロップリスト
	vector<pcDroping> droplist;
	sg_pDungeonSystem->DataBase.makeAllDropList(droplist);

	int i;
	for(i=0;i<droplist.size();i++)
	{
		cCommand_menuOther_String_AllItemLongExplanation_ID* pccm;
		pcsw->commandList.push_back(pcCommand(pccm = new cCommand_menuOther_String_AllItemLongExplanation_ID(setStyle(droplist[i]->ID())+_T(": ")+ droplist[i]->FullNameBase()) ));
		
		pccm->Drop_ID = droplist[i]->ID();
	}
	pcsw->Init(pDev, 20,10);
	pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(4));
	pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(2));

	return true;
};
StyleString cCommand_menuOther_String_AllItemLongExplanation::shortExplanationText()
{
	return StyleString(_T(""));
}
int cCommand_menuOther_String_AllItemLongExplanation_ID::Action(IDirect3DDevice9 *pDev)
{
	pcControlLayer pccl;
	pcGameWindow pcgw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	//新規ゲームウィンドウ
	pccl->WindowList.push_back(pcgw = pcGameWindow(new cGameWindow));
	pcgw->Init(pDev, 24, 14);

	pcDroping pdrop = sg_pDungeonSystem->DataBase.GetSampleDroping(Drop_ID);
	pcgw->Text() = pdrop->longExplanation();
	pdrop->longExplanationExtra(pcgw->vText);

	pcgw->CenterX = SCREEN_X/2;
	pcgw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(1));

	return true;
};
StyleString cCommand_menuOther_String_AllItemLongExplanation_ID::shortExplanationText()
{
	pcDroping pdrop = sg_pDungeonSystem->DataBase.GetSampleDroping(Drop_ID);
	return pdrop->shortExplanation();
}
int cCommand_menuOther_String_AllMassage::Action(IDirect3DDevice9 *pDev)
{
	pcControlLayer pccl;
	pcSelectWindow pcsw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	//新規セレクトウィンドウ
	pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

	std::map<tstring,pcScriptRLayer>::iterator itr, itrend;
	itr = g_systemSR.pdata()->pmember(_T("Story"))->membermap().begin();
	itrend = g_systemSR.pdata()->pmember(_T("Story"))->membermap().end();
	//ドロップリスト
	
	int i;
	for(;itr!=itrend;itr++)
	{
		const tstring& key = itr->first;
		cCommand_menuOther_String_AllMassage_Massage* pccm;
		pcsw->commandList.push_back(pcCommand(pccm = new cCommand_menuOther_String_AllMassage_Massage(key)));
		
		pccm->massage_title = key;
	}
	pcsw->Init(pDev, 20,10);
	pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(4));
	pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(2));

	return true;
};
StyleString cCommand_menuOther_String_AllMassage::shortExplanationText()
{
	return StyleString(_T(""));
}

int cCommand_menuOther_String_AllMassage_Massage::Action(IDirect3DDevice9 *pDev)
{
	/*
	pcControlLayer pccl;
	pcGameWindow pcgw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	//新規ゲームウィンドウ
	pccl->WindowList.push_back(pcgw = pcGameWindow(new cGameWindow));
	pcgw->Init(pDev, 24, 14);

	pcDroping pdrop = sg_pDungeonSystem->DataBase.GetSampleDroping(Drop_ID);
	pcgw->Text() = pdrop->longExplanation();
	pdrop->longExplanationExtra(pcgw->vText);

	pcgw->CenterX = SCREEN_X/2;
	pcgw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(1));

	return true;
	*/
	return false;
};
StyleString cCommand_menuOther_String_AllMassage_Massage::shortExplanationText()
{
	pcScriptRLayer player = g_systemSR.pdata()->pmember(_T("Story"))->pmember(massage_title);
	if(player)
	{
		StyleString sstr;
		Drop_StringAddStyle(player,sstr);
		return sstr;
	}
	return StyleString(_T(""));
}
//----------------------------------------------------------------------------------------------------------------
//外部データ再読み込み
int cCommand_menuOther_ReImport::Action(IDirect3DDevice9 *pDev)
{
	sg_pDungeonSystem->DataBase.ReImportData();
	sg_pDungeonSystem->メニューを閉じる();
	return true;
}
StyleString cCommand_menuOther_ReImport::shortExplanationText()
{
	return _T("外部データを再読み込みします。");
}

//外部データ再読み込み
int cCommand_menuOther_ReImportGraphic::Action(IDirect3DDevice9 *pDev)
{
	sg_pDungeonSystem->メニューを閉じる();
	sg_pDungeonSystem->フロア更新要請();
	return true;
}
StyleString cCommand_menuOther_ReImportGraphic::shortExplanationText()
{
	return _T("フロアをリセットし画像データを再読み込みします。");
}

//外部読み込み形式エラーチェック
int cCommand_menuOther_ImportDataCheck::Action(IDirect3DDevice9 *pDev)
{
	int id;

	#ifdef _UNRELEASE

	sg_pDungeonSystem->DataBase.ReImportData();



	sg_pDungeonSystem->メッセージ((StyleString)_T("spell 通常時最悪耐久回数 チェック \n"));
	sg_pDungeonSystem->メッセージ((StyleString)_T("spell 亀裂時最悪耐久回数 チェック \n"));
	sg_pDungeonSystem->メッセージ((StyleString)_T("spell 通常時破損率 チェック \n"));
	sg_pDungeonSystem->メッセージ((StyleString)_T("spell 亀裂時破損率 チェック \n"));
	for(id = CDROPING_cSpell_BASE_ID_NUM ;id<1000+CDROPING_cSpell_BASE_ID_NUM; id++)
	{//Spell
		pcDroping pdrop = 
			sg_pDungeonSystem->DataBase.GetSampleDroping(id);

		pcSpell pSpell = pcSpell((cSpell*)NULL);
		if( pdrop != NULL ) pSpell = boost::dynamic_pointer_cast<cSpell>(pdrop);
		if( pSpell == NULL ) continue;

		
		pSpell->通常時最悪耐久回数();
		pSpell->亀裂時最悪耐久回数();
		pSpell->通常時破損率();
		pSpell->亀裂時破損率();

	}



	#else
	sg_pDungeonSystem->メッセージ((StyleString)_T("！Debugビルドモードでないと機能しません。 \n"));

	#endif

	sg_pDungeonSystem->メニューを閉じる();
	return true;
}
StyleString cCommand_menuOther_ImportDataCheck::shortExplanationText()
{
	return _T("外部データを再読み込みした後、読み込みデータの形式に不備が無いがチェックします。\n外部データの保存し忘れに注意してください。");
}

int cCommand_menuOther_RetireChoice::Action(IDirect3DDevice9 *pDev)
{
	pcControlLayer pccl;
	pcSelectWindow pcsw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

	pcsw->commandList.push_back(pcCommand(new cCommand_menuOther_RetireChoice_No ));
	pcsw->commandList.push_back(pcCommand(new cCommand_menuOther_RetireChoice_Yes ));

	pcsw->Init(pDev, 8, pcsw->commandList.size());
	pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(3));
	pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(3));
	return true;
}
StyleString cCommand_menuOther_RetireChoice::shortExplanationText()
{
	return g_Lang(_T("あきらめるExplanation"));
}
int cCommand_menuOther_RetireChoice_Yes::Action(IDirect3DDevice9 *pDev)
{
	sg_pDungeonSystem->GameOver();
	sg_pDungeonSystem->メニューを閉じる();
	return true;
}
StyleString cCommand_menuOther_RetireChoice_Yes::shortExplanationText()
{
	return g_Lang(_T("本当にあきらめるExplanation"));
}
int cCommand_menuOther_RetireChoice_No::Action(IDirect3DDevice9 *pDev)
{
	sg_pDungeonSystem->メニューを閉じる();
	return true;
}
StyleString cCommand_menuOther_RetireChoice_No::shortExplanationText()
{
	return g_Lang(_T("あきらめないExplanation"));
}
int cCommand_menuOther_ReturnTitle::Action(IDirect3DDevice9 *pDev)
{	
	g_GameEnv.m_SceneManage->SceneChange(pDev,new csTitle);
	sg_pDungeonSystem->メニューを閉じる();


	return true;
}
//----------------------------------------------------------------------------------------------------------------
namespace CommandContinue{
	int CommandFunc_Continue(cCommandDelegated& command) {

		//新規コントロールレイヤー
		//sg_pDungeonSystem->
		sg_pDungeonSystem->GameOverAndContinue();
		sg_pDungeonSystem->メニューを閉じる();
		return true;
	}
	int CommandFunc_ContinueAndSuspend(cCommandDelegated& command) {

		//新規コントロールレイヤー
		//sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
		sg_pDungeonSystem->GameOverAndContinueAndSuspend();
		sg_pDungeonSystem->メニューを閉じる();
		return true;
	}
	int CommandFunc_Retire(cCommandDelegated& command) {
		pcControlLayer pccl;
		pcSelectWindow pcsw;

		//新規コントロールレイヤー
		sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
		pccl->Init(sg_pDungeonSystem->pDevice_D3D);

		pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

		pcsw->commandList.push_back(pcCommand(new cCommandDelegated(g_Lang(_T("ContinueRetire+Retry")), CommandFunc_Retire_Restart)));
		pcsw->commandList.push_back(pcCommand(new cCommandDelegated(g_Lang(_T("ContinueRetire")), CommandFunc_Retire_Retire)));

		pcsw->Init(sg_pDungeonSystem->pDevice_D3D, 16, pcsw->commandList.size());
		pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosParentOfControlLayer(pccl).Left() + sg_pDungeonSystem->GameScreenInterface.menuPosWidthByLevel());
		pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosParentOfControlLayer(pccl).Top() + sg_pDungeonSystem->GameScreenInterface.menuPosHeightByLevel());
		return true;
	}
	int CommandFunc_Retire_Retire(cCommandDelegated& command) {
		sg_pDungeonSystem->GameOver();
		sg_pDungeonSystem->メニューを閉じる();
		return true;
	}
	int CommandFunc_Retire_Restart(cCommandDelegated& command) {
		sg_pDungeonSystem->GameOverAndRestart();
		sg_pDungeonSystem->メニューを閉じる();
		return true;
	}

}