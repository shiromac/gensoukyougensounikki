#include "stdafx.h"

#include "cSpell.h"

#include "FireCommand.h"

#include "cDungeonSystem.h"

#include "cDropingFunc.h"

#include "EffectFunctions.h"


#include "Language.h"


#include "gameMainSystem/filemanage/cScriptRLayer.h"
#include "scriptprocesser/DropsScript.h"

#include "utility/debug.h"
#include "scriptprocesser/AnimationScript.h"


#define BREAK_POSSIBILITY_BASE_NORMAL (0.01)
#define BREAK_POSSIBILITY_BASE_DAMAGE (0.02)
#define RUNOUTGOOD_POSSIBILITY (0.8)
#define BREAK_LEASTSTAND_NORMAL (5)
#define BREAK_LEASTSTAND_DAMAGE (10)

cSpell::cSpell(void)
{
	装備箇所 = SPELL_EQUIP_PART_NULL;
}

cSpell::~cSpell(void)
{
}

//消去処理の準備
void cSpell::Release()
{
	unequip();	

}

tstring cSpell::SpellTitle()
{
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.DropImportLanguage(ID(),
		(tstring)_T("SpellTitle"));
	OnAssert(_T(__FILE__),__LINE__, (psr != NULL) , FullNameBase().c_str(),_T(" > SpellTitleが定義されてません。<error> [SpellTitle] Not Found"));
	if(psr == NULL)
		return _T("No Title");
	return psr->topscript();
}
tstring cSpell::SpellName()
{
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.DropImportLanguage(ID(),
		(tstring)_T("SpellName"));
	OnAssert(_T(__FILE__),__LINE__, (psr != NULL) , FullNameBase().c_str(),_T(" > SpellNameが定義されてません。<error> [SpellName] Not Found"));
	if(psr == NULL) return _T("No Name");
	return psr->topscript();
}


StyleString cSpell::識別未識別混合道具名()
{
	tstring damege = _T("");
	if(破損している())
	{
		damege = g_Lang(_T("(ひび)")).conclete_tstr();
	}
	if(大局識別済み())
		return FullNameBase()+damege;
	else
	{
		if(NamingCaption() == _T(""))
			return sg_pDungeonSystem->DataBase.未識別名(ID())+damege;
		else
			return g_Lang(_T("符:")) + NamingCaption()+damege;
	}
}

void cSpell::AppreciationBeginOptimize(int difficulty)
{
	cItem::AppreciationBeginOptimize(difficulty);
	appreciation_state() = true;//状態識別済み
	appreciation_quality() = true;//修正値識別済み
}

/*
cDiscreteProbability cSpell::stateBeginDP(int difficulty)
{
	cDiscreteProbability DP;
	DP.set(1,cDroping::STATE_CURSE);
	DP.set(4,cDroping::STATE_NORMAL);
	DP.set(1,cDroping::STATE_GOOD);
	return DP;
}
*/
/*
cDiscreteProbability cSpell::qualityBeginDP(int difficulty)
{
	cDiscreteProbability DP;
	DP.set(1,3);
	DP.set(2,4);
	DP.set(2,5);
	DP.set(1,6);
	return DP;
}
*/


tstring cSpell::装備コマンド名称(int part)
{
	return g_Lang(_T("装備__")).conclete_tstr()+sg_pDungeonSystem->漢数字(part+1)+_T("符");
}




int cSpell::通常時最悪耐久回数()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		ID(),(tstring)_T("通常時最悪耐久回数"),デフォルト通常時最悪耐久回数());
	/*
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.DropImportData(ID(),
		(tstring)_T("通常時最悪耐久回数"));
	if(psr == NULL) return BREAK_LEASTSTAND_NORMAL;

	int res;
	TCHAR* e;
	res = _tcstol(psr->topscript().c_str(),&e,10);
	OnAssert(_T(__FILE__),__LINE__, (psr->topscript().length() != 0) , FullNameBase().c_str(),
		_T(" > 通常時最悪耐久回数 に　空欄　が設定されています。"));
	OnAssert(_T(__FILE__),__LINE__, (_tcslen(e) == 0) && ( !(res == 0 && _tcslen(e) == 0) || (psr->topscript() == _T("0")))  , FullNameBase().c_str(),
		_T(" > 通常時最悪耐久回数 に　解釈不能な数値　が設定されています。"));
	
	return res;
	*/
}


int cSpell::亀裂時最悪耐久回数()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		ID(),(tstring)_T("亀裂時最悪耐久回数"),デフォルト亀裂時最悪耐久回数());
	/*
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.DropImportData(ID(),
		(tstring)_T("亀裂時最悪耐久回数"));
	if(psr == NULL) return BREAK_LEASTSTAND_DAMAGE;


	int res;
	TCHAR* e;
	res = _tcstol(psr->topscript().c_str(),&e,10);
	OnAssert(_T(__FILE__),__LINE__, (psr->topscript().length() != 0) , FullNameBase().c_str(),
		_T(" > 亀裂時最悪耐久回数 に　空欄　が設定されています。"));
	OnAssert(_T(__FILE__),__LINE__, (_tcslen(e) == 0) && ( !(res == 0 && _tcslen(e) == 0) || (psr->topscript() == _T("0")))  , FullNameBase().c_str(),
		_T(" > 亀裂時最悪耐久回数 に　解釈不能な数値　が設定されています。"));
	
	return res;
	*/
}
int cSpell::デフォルト通常時最悪耐久回数()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		(tstring)_T("Spell基本値"),(tstring)_T("デフォルト通常時最悪耐久回数"),BREAK_LEASTSTAND_NORMAL);
}
int cSpell::デフォルト亀裂時最悪耐久回数()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		(tstring)_T("Spell基本値"),(tstring)_T("デフォルト亀裂時最悪耐久回数"),BREAK_LEASTSTAND_DAMAGE);
}
double cSpell::通常時破損率()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		ID(),(tstring)_T("通常時破損率"),デフォルト通常時破損率());
	/*
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.DropImportData(ID(),
		(tstring)_T("通常時破損率"));
	if(psr == NULL) return BREAK_POSSIBILITY_BASE_NORMAL;

	double res;
	TCHAR* e;
	res = _tcstod(psr->topscript().c_str(),&e);
	OnAssert(_T(__FILE__),__LINE__, (psr->topscript().length() != 0) , FullNameBase().c_str(),
		_T(" > 通常時破損率 に　空欄　が設定されています。"));
	OnAssert(_T(__FILE__),__LINE__, (_tcslen(e) == 0) && ( !(res == 0 && _tcslen(e) == 0) || (psr->topscript() == _T("0")))  , FullNameBase().c_str(),
		_T(" > 通常時破損率 に　解釈不能な数値　が設定されています。"));
	
	return res;
	*/
}
double cSpell::亀裂時破損率()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		ID(),(tstring)_T("亀裂時破損率"),デフォルト亀裂時破損率());
	/*
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.DropImportData(ID(),
		(tstring)_T("亀裂時破損率"));
	if(psr == NULL) return BREAK_POSSIBILITY_BASE_DAMAGE;

	double res;
	TCHAR* e;
	res = _tcstod(psr->topscript().c_str(),&e);
	OnAssert(_T(__FILE__),__LINE__, (psr->topscript().length() != 0) , FullNameBase().c_str(),
		_T(" > 亀裂時破損率 に　空欄　が設定されています。"));
	OnAssert(_T(__FILE__),__LINE__, (_tcslen(e) == 0) && ( !(res == 0 && _tcslen(e) == 0) || (psr->topscript() == _T("0")))  , FullNameBase().c_str(),
		_T(" > 亀裂時破損率 に　解釈不能な数値　が設定されています。"));
	
	return res;
	*/
}
double cSpell::デフォルト通常時破損率()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		(tstring)_T("Spell基本値"),(tstring)_T("デフォルト通常時破損率"),BREAK_POSSIBILITY_BASE_NORMAL);
}
double cSpell::デフォルト亀裂時破損率()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		(tstring)_T("Spell基本値"),(tstring)_T("デフォルト亀裂時破損率"),BREAK_POSSIBILITY_BASE_DAMAGE);
}
double cSpell::宣言時神力切れ確率()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		ID(),(tstring)_T("宣言時神力切れ確率"),デフォルト宣言時神力切れ確率());
}
double cSpell::デフォルト宣言時神力切れ確率()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		(tstring)_T("Spell基本値"),(tstring)_T("デフォルト宣言時神力切れ確率"),RUNOUTGOOD_POSSIBILITY);
}
double cSpell::効果時腕輪ダメージ()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		ID(),(tstring)_T("効果時腕輪ダメージ"),0.0);
}
void cSpell::DrawStateIconSub(IDirect3DDevice9 *pDev,int x,int y)
{
	cDrawingObject DO;

	int upperline = y-8;
	int bottomline = y+8;
	int liftline = x+8;
	int rightline = x+8+16;


	DO.Width = 16;
	DO.Height = 16;

	//本体
	DO.m_color.ARGB(255,255,255,255);
	DO.m_color *= m_color;
	DO.setTexture(m_pTexture_StateIcon,64,64);


	if(装備されている())
	{//攻撃装備アイコン
		DO.CenterX = rightline;
		DO.CenterY = upperline;

		DO.m_TexRange.setLTRB(0.75,0.25,1,0.5);
		DO.Draw(pDev);
	}
	
	if(破損している())
	{//亀裂アイコン
		DO.CenterX = liftline;
		DO.CenterY = upperline;

		DO.m_TexRange.setLTRB(0.25,0.5,0.5,0.75);
		DO.Draw(pDev);
	}
	

		
}

StyleString cSpell::ListFullName()
{
	StyleString FullN = cItem::FullName();

	
	if(装備されている())
	{
		unsigned long color = FullN.colors[0];
		FullN = sg_pDungeonSystem->漢数字(装備箇所+1) + g_Lang(_T("符『"))+
			FullN + g_Lang(_T("』"));
		FullN.setSameColor(color);
	}

	return FullN;
}
/*
int cSpell::isCanFireCommand( vector<pcDroping> &ObjectList, tstring verb)
{
	if(verb == _T("宣言"))
	{
		return オブジェクト宣言可能(ObjectList.size());

	}

	return true;
}
*/

/*
StyleString cSpell::GetmenuFuncObjectsList(vector<pcDroping> &ObjectList, bool &MultiObj,int objectsize, tstring verb)
{
	if(verb == _T("宣言"))
	{

		return 宣言オブジェクト候補(ObjectList, MultiObj, objectsize, verb);

	}

	return _T("");
}
*/


void cSpell::宣言オブジェクト候補(vector<vector<pcDroping>> &ObjectListlist, vector<StyleString>& listExplain, bool &MultiObj, int objectsize, tstring verb)
{
	//if(objectsize == 0)
	//{

		ObjectListlist.push_back(sg_pDungeonSystem->pPlayerChara()->holdItem);
		listExplain.push_back(g_Lang(_T("どれに？ (持ち物)")));

		//MultiObj = true;
		if(sg_pDungeonSystem->pPlayerChara()->足元() != NULL)
		{
			vector<pcDroping> 足元リスト;
			足元リスト.push_back(sg_pDungeonSystem->pPlayerChara()->足元());
			ObjectListlist.push_back(足元リスト);
		}

		listExplain.push_back(g_Lang(_T("どれに？ (足元)")));
		return;
	//}
	//return _T("");
}

	
void cSpell::スキルオブジェクト候補(vector<vector<pcDroping>> &ObjectListlist, vector<StyleString>& listExplain, bool &MultiObj, int objectsize, tstring verb)
{
	//if(objectsize == 0)
	//{

		ObjectListlist.push_back(sg_pDungeonSystem->pPlayerChara()->holdItem);
		listExplain.push_back(g_Lang(_T("どれに？ (持ち物)")));

		//MultiObj = true;
		if(sg_pDungeonSystem->pPlayerChara()->足元() != NULL)
		{
			vector<pcDroping> 足元リスト;
			足元リスト.push_back(sg_pDungeonSystem->pPlayerChara()->足元());
			ObjectListlist.push_back(足元リスト);
		}

		listExplain.push_back(g_Lang(_T("どれに？ (足元)")));
		return;
	//}
	//return _T("");
}
	

int cSpell::宣言オブジェクト自動選択(vector<pcDroping>& ObjectList)
{
	ObjectList.clear();
	if(オブジェクト宣言可能(ObjectList.size()))	return true;

	int i,k;
	for(i=0;i<1024;i++)//一応無限ループ防止
	{
		vector<StyleString> listExplain;
		vector<vector<pcDroping>> ObjectListlist;
		bool MultiObj;
		int objectsize = ObjectList.size();
		tstring verb = _T("宣言");//コマンドとしてなので日本語でOK

		宣言オブジェクト候補(ObjectListlist, listExplain, MultiObj, objectsize, verb);

		vector<pcDroping> pdropList;
		for(k=0;k<ObjectListlist.size();k++)
		{
			pdropList.insert(pdropList.end(),ObjectListlist[k].begin(),ObjectListlist[k].end());
		}

		if(pdropList.size() == 0) return false;//オブジェクト候補なし！

		ObjectList.push_back(pdropList[pdropList.size()*random()]);
	
		if(オブジェクト宣言可能(ObjectList.size()))
		{
			break;
		}
	}
	return true;
}



int cSpell::GetmenuCaption(vector<tstring> &CaptionList)
{
	if(倉庫の中())
	{
		CaptionList.push_back(_T("説明"));
		CaptionList.push_back(_T("倉庫から出す"));
		return true;
	}



	
	
	if(Holder() == NULL)
	{//落ちてる

		if(Parent() == NULL)
		{
			if(sg_pDungeonSystem->pPlayerChara()->holdItem.size() >= sg_pDungeonSystem->pPlayerChara()->MaxholdNum())
				CaptionList.push_back(_T("交換"));
			else
				CaptionList.push_back(_T("拾う"));
		}
	
		if(!破損している()) CaptionList.push_back(_T("宣言"));

		CaptionList.push_back(_T("投げる"));
	}
	else
	{//持ってる
	
		if(装備されている())
		{//装備されてる
			if(state() == cDroping::STATE_GOOD || sg_pDungeonSystem->拠点フラグ())
				CaptionList.push_back(_T("はずす"));

			if(スキルあり(装備者()))
			{
				//CaptionList.push_back(_T("スキル"));
			}

			CaptionList.push_back(_T("破壊"));
		}
		else
		{//装備されて無い
			if(!破損している()) CaptionList.push_back(_T("宣言"));

			if(!内包されている())
				CaptionList.push_back(_T("装備"));


			CaptionList.push_back(_T("投げる"));

			if(Holder()->足元() == NULL)
			{//足元に何も無い
				CaptionList.push_back(_T("置く"));
			}
			else if(!装備されている() )
			{//足元に何かある
				if(!内包されている() && Holder()->足元()->属性.count(落ち物属性::アイテム)) CaptionList.push_back(_T("交換"));
				else if(!内包されている() &&
					Holder()->足元()->属性.count(落ち物属性::罠) &&
					!Holder()->足元()->罠用_効果目標か(sg_pDungeonSystem->pPlayerChara())) CaptionList.push_back(_T("交換"));
			}
		}

	}

	if(!大局識別済み()) CaptionList.push_back(_T("名前"));

	CaptionList.push_back(_T("説明"));

	return true;
}


//captionのexplainを得る
StyleString cSpell::GetmenuExplain(tstring &Caption)
{

	int i;
	int part = SPELL_EQUIP_PART_NULL;
	for(i=0;i<SPELL_EQUIP_MAXNUM;i++)
	{
		if(装備コマンド名称(i) == Caption) part = i;
	}
	if(part != SPELL_EQUIP_PART_NULL)
	{
		pcDroping beforeSpl = sg_pDungeonSystem->pPlayerChara()->spellequipment[part];
		pcDroping afterSpl = me();
	
		StyleString explain;
		if(beforeSpl != NULL)
		{
			/*
			int len = beforeSpl->FullName().length();
			for(i=0;i<12*2-len/2;i++)
			{
				explain += _T(" ");
			}
			*/
			explain += beforeSpl->FullName() + _T("\n");
			explain += g_Lang(_T("▽（上書き）"));
			explain += _T("\n");
		}
		else
		{
			explain += g_Lang(_T("<装備なし>"));
			explain += _T("\n");
			explain += g_Lang(_T("▼"));
			explain += _T("\n");
		}

		/*
		int len = afterSpl->FullName().length();
		for(i=0;i<12*2-len/2;i++)
		{
			explain += _T(" ");
		}
		*/
		explain += afterSpl->FullName() + _T("\n");

		return explain;
	}

	if(Caption == _T("スキル"))
	{
		return スキル説明(装備者());
	}

	return _T("");
}

/*
//コマンドを解決する;
int cSpell::FireCommand(IDirect3DDevice9 *pDev, tstring verb, vector<pcDroping> &ObjectList)
{

	if(verb == _T("宣言"))
	{
		宣言(pDev,ObjectList);
	}

	if(verb == _T("装備"))
	{
		装備選択(pDev);
	}

	int i;

	for(i=0;i<SPELL_EQUIP_MAXNUM;i++)
	{
		if(verb == 装備コマンド名称(i))
		{
			装備(pDev,i);
		}
	}

	if(verb == _T("説明")) 説明(pDev);
	if(verb == _T("拾う")) 拾う(pDev);
	if(verb == _T("置く")) 置く(pDev);
	if(verb == _T("投げる")) 投げる(pDev);
	if(verb == _T("交換")) 交換(pDev);

	return true;
}
*/

/*
int cSpell::宣言(IDirect3DDevice9 *pDev ,vector<pcDroping> &ObjectList)
{
	int result = 0;


	if(state() == cDroping::STATE_NORMAL)
	{
		result = 宣言_メッセージ_通常(sg_pDungeonSystem->pPlayerChara(), ObjectList);
	}
	else if(state() == cDroping::STATE_CURSE)
	{
		result = 宣言_メッセージ_呪い(sg_pDungeonSystem->pPlayerChara(), ObjectList);
	}
	else if(state() == cDroping::STATE_GOOD)
	{
		result = 宣言_メッセージ_祝福(sg_pDungeonSystem->pPlayerChara(), ObjectList);
	}

	if(result)
	{//使用に成功した
		sg_pDungeonSystem->メニューを閉じる();
		sg_pDungeonSystem->ターンエンド();
	}


	result = 宣言要請(sg_pDungeonSystem->pPlayerChara(), ObjectList);




	return true;
}
int cSpell::宣言要請(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{

	



	int result = 0;
	if(state() == cDroping::STATE_NORMAL)
	{
		result = 宣言_効果_通常(pchara, ObjectList);
	}
	else if(state() == cDroping::STATE_CURSE)
	{
		result = 宣言_効果_呪い(pchara, ObjectList);
	}
	else if(state() == cDroping::STATE_GOOD)
	{
		result = 宣言_効果_祝福(pchara, ObjectList);
	}

	if(result)
	{//使用に成功した
		if(!名称識別済み())
		{
			sg_pDungeonSystem->識別(me(),true);
		}
		else if(!完全識別済み())
		{
			sg_pDungeonSystem->識別(me(),false);
		}

		if(state() == cDroping::STATE_GOOD)
		{
			stategoodRunoutprocess();
		}
		else
		{
			sg_pDungeonSystem->落ち物破壊要請(me());
		}
	}

	return result;
}
*/

int cSpell::宣言_効果_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	スペル定型エフェクト_宣言();
	sg_pDungeonSystem->メッセージ((StyleString)_T("cSpell 宣言_効果_通常() 効果が設定されていません。\n"));
	return true;
}
int cSpell::宣言_メッセージ_通常(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	return 宣言メッセージ定型(pchara, ObjectList);
}

int cSpell::宣言_効果_呪い(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	//効果なし
	return false;
}
int cSpell::宣言_メッセージ_呪い(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	sg_pDungeonSystem->メッセージ(g_Lang(_T("Spell呪いによる宣言不可メッセージ")));
	sg_pDungeonSystem->状態のみ識別(me(),false);
	return false;
}

int cSpell::宣言_効果_祝福(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	return 宣言_効果_通常(pchara, ObjectList);
}
int cSpell::宣言_メッセージ_祝福(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	return 宣言_メッセージ_通常(pchara, ObjectList);
}


int cSpell::宣言メッセージ定型(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("spell.wav"),pchara->足元地形()->place);
	
	map<tstring, StyleString> valiable;
	valiable[_T("Item")] = FullName();
	g_Langメッセージ(_T("Spell宣言メッセージ"),valiable);
	
	//sg_pDungeonSystem->メッセージ
	//		(FullName()+g_Lang(_T("Spell宣言メッセージ")));
	return true;
}


int cSpell::衝突(pcCharacter pchara)
{
	int result = 0;

	衝突時_メッセージ(pchara);


	result = 衝突時効果(pchara);


	if(result)
	{//使用に成功した
		//sg_pDungeonSystem->落ち物破壊要請(me());
	}
	
	return result;
}

int cSpell::衝突時効果(pcCharacter pchara)
{

	if(sg_pDungeonSystem->遠隔拾得要請(pchara,me()))
	{//拾えた
		return Spell装備要請(pchara,pchara->最大装備スペル数() * random(), me());
	}
	else
	{//拾えない
		return false;
	}
}

int cSpell::衝突時_メッセージ(pcCharacter pchara)
{


	return true;
}
	




/*

int cSpell::装備選択(IDirect3DDevice9 *pDev)
{
	vector<tstring> captions;

	//GetmenuCaption(captions);
	int i;
	for(i=0;i<SPELL_EQUIP_MAXNUM && i< sg_pDungeonSystem->pPlayerChara()->最大装備スペル数();i++)
	{
		captions.push_back(装備コマンド名称(i));
	}
	if(captions.size() == 0) return false;

	pcControlLayer pccl;
	pcSelectWindow pcsw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));
	


	for(i=0;i<captions.size();i++)
	{
		cDropingFunc *drfn;
		pcsw->commandList.push_back(pcCommand(drfn = new cDropingFunc(captions[i])));
		drfn->Subject = me();
		drfn->Verb = captions[i];

		drfn->shortExplanation = GetmenuExplain(drfn->Verb);
	}

	pcsw->Init(pDev, 6, pcsw->commandList.size());
	pcsw->setRight(SCREEN_X-32-16);
	pcsw->setTop(32+16);
	
	return true;
}


int cSpell::装備(IDirect3DDevice9 *pDev, int part)
{
	int result = 0;


	if(state() == cDroping::STATE_NORMAL)
	{
		result = 装備_メッセージ_通常(sg_pDungeonSystem->pPlayerChara(),part);
	}
	else if(state() == cDroping::STATE_CURSE)
	{
		result = 装備_メッセージ_呪い(sg_pDungeonSystem->pPlayerChara(),part);
	}
	else if(state() == cDroping::STATE_GOOD)
	{
		result = 装備_メッセージ_祝福(sg_pDungeonSystem->pPlayerChara(),part);
	}


	result = 装備要請(sg_pDungeonSystem->pPlayerChara(),part);


	if(result)
	{//使用に成功した
		sg_pDungeonSystem->メニューを閉じる();
		sg_pDungeonSystem->ターンエンド();
	}

	return true;
}


int cSpell::装備要請(pcCharacter pchara, int part)
{
	if(装備されている())
	{//装備できない
		return false;
	}


	int result = 0;
	if(state() == cDroping::STATE_NORMAL)
	{
		result = 装備_効果_通常(pchara,part);
	}
	else if(state() == cDroping::STATE_CURSE)
	{
		result = 装備_効果_呪い(pchara,part);
	}
	else if(state() == cDroping::STATE_GOOD)
	{
		result = 装備_効果_祝福(pchara,part);
	}

	if(result)
	{//装備に成功した
		
	}

	return result;
}
*/

int cSpell::装備_効果_通常(pcCharacter pchara, int part)
{
	int result = 1;

	if(pchara->spellequipment[part] == NULL)
	{//装備されて無い
		equip(pchara,part);//EQUIP_PART_ATTACKは意味なし
	}
	else
	{//装備されてる
		pcSpell pcSpl;
		pcSpl = boost::dynamic_pointer_cast<cSpell>(pchara->spellequipment[part]);
		if(pcSpl != NULL)
		{

			pcDroping spl = pchara->spellequipment[part];
			if(spl->state() == cDroping::STATE_CURSE)
			{//呪われてる
				map<tstring, StyleString> valiable;
				valiable[_T("Item")] = spl->FullName();
				g_Langメッセージ(_T("Spell呪いによる破壊不可メッセージ"),valiable);
				//sg_pDungeonSystem->メッセージ(spl->FullName() + _T(" は呪われていて 破壊出来ない！\n"));
				result = false;
			}
			else
			{
				map<tstring, StyleString> valiable;
				valiable[_T("Item")] = spl->FullName();
				g_Langメッセージ(_T("Spell破壊＆メッセージ"),valiable);
				
				//sg_pDungeonSystem->メッセージ(spl->FullName() + _T(" を破壊して、\n"));
				//破壊処理
				sg_pDungeonSystem->落ち物破壊要請(pchara->spellequipment[part]);		
			}

		}
		else
		{//スペルじゃない//ありえない
			//result = false;
		}
		
		if(result == false)
		{//装備はずせなかった
			return false;
		}


		equip(pchara,part);


	}
		
	sg_pDungeonSystem->CutInM().CutIn(me(),スペル装備直後_タイミング);




	装備メッセージ定型(pchara,part);
	return true;
}
int cSpell::装備_メッセージ_通常(pcCharacter pchara, int part)
{

	return true;
}

int cSpell::装備_効果_呪い(pcCharacter pchara, int part)
{
	return 装備_効果_通常(pchara,part);
}
int cSpell::装備_メッセージ_呪い(pcCharacter pchara, int part)
{
	return 装備_メッセージ_通常(pchara,part);
}

int cSpell::装備_効果_祝福(pcCharacter pchara, int part)
{
	return 装備_効果_通常(pchara,part);
}
int cSpell::装備_メッセージ_祝福(pcCharacter pchara, int part)
{
	return 装備_メッセージ_通常(pchara,part);
}


int cSpell::装備メッセージ定型(pcCharacter pchara, int part)
{
	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("equip.wav"),pchara->足元地形()->place);

	スペル定型エフェクト_装備(pchara);

	map<tstring, StyleString> valiable;
	valiable[_T("Item")] = FullName();
	valiable[_T("Num")] = sg_pDungeonSystem->漢数字(part+1);
	g_Langメッセージ(_T("Spell装備メッセージ"),valiable);


	//sg_pDungeonSystem->メッセージ
	//	(FullName()+_T(" を ")+sg_pDungeonSystem->漢数字(part+1)+_T("符 に装備した。\n"));

	return true;
}


//強制的に装備
int cSpell::equip(pcCharacter pchara, int part)
{

	
	pchara->spellequipment[part] = me();
	weak_pEquiper = wpcCharacter(pchara);
	装備箇所 = part;

	return true;
}
//強制的に装備解除
int cSpell::unequip()
{

	if(装備されている())
	{
		sg_pDungeonSystem->CutInM().CutIn(me(),スペル装備解除直前_タイミング);

		装備者()->spellequipment[装備箇所] = NULLDROP;
		weak_pEquiper = wpcCharacter(NULLCHARA);
		装備箇所 = SPELL_EQUIP_PART_NULL;
	}
	

	return true;
}


/*
int cSpell::はずす(IDirect3DDevice9 *pDev)
{
	int result = 0;


	if(state() == cDroping::STATE_NORMAL)
	{
		result = はずす_メッセージ_通常(sg_pDungeonSystem->pPlayerChara());
	}
	else if(state() == cDroping::STATE_CURSE)
	{
		result = はずす_メッセージ_呪い(sg_pDungeonSystem->pPlayerChara());
	}
	else if(state() == cDroping::STATE_GOOD)
	{
		result = はずす_メッセージ_祝福(sg_pDungeonSystem->pPlayerChara());
	}


	result = 装備はずし要請(sg_pDungeonSystem->pPlayerChara());


	if(result)
	{//使用に成功した
		sg_pDungeonSystem->メニューを閉じる();
	}

	return true;
}


int cSpell::装備はずし要請(pcCharacter pchara)
{

	int result = 0;
	if(state() == cDroping::STATE_NORMAL)
	{
		result = はずす_効果_通常(pchara);
	}
	else if(state() == cDroping::STATE_CURSE)
	{
		result = はずす_効果_呪い(pchara);
	}
	else if(state() == cDroping::STATE_GOOD)
	{
		result = はずす_効果_祝福(pchara);
	}

	if(result)
	{//使用に成功した
		sg_pDungeonSystem->メニューを閉じる();
	}

	return result;
}
*/
int cSpell::はずす_効果_通常(pcCharacter pchara)
{
	unequip();

	if(state() == cDroping::STATE_GOOD) stategoodRunoutprocess();

	return true;
}
int cSpell::はずす_メッセージ_通常(pcCharacter pchara)
{
	return はずすメッセージ定型(pchara);
}

int cSpell::はずす_効果_呪い(pcCharacter pchara)
{
	//効果なし
	return false;
}
int cSpell::はずす_メッセージ_呪い(pcCharacter pchara)
{
	return sg_pDungeonSystem->メッセージ(g_Lang(_T("Spellはずす不可メッセージ")));
}

int cSpell::はずす_効果_祝福(pcCharacter pchara)
{
	return はずす_効果_通常(pchara);
}
int cSpell::はずす_メッセージ_祝福(pcCharacter pchara)
{
	return はずす_メッセージ_通常(pchara);
}


int cSpell::はずすメッセージ定型(pcCharacter pchara)
{
	//sg_pDungeonSystem->AnimationManager().
	//	Anime_PlaySE(_T("drink.wav"),pchara->足元地形()->place);
	map<tstring, StyleString> valiable;
	valiable[_T("Item")] = FullName();
	g_Langメッセージ(_T("Spellはずすメッセージ"),valiable);

	//sg_pDungeonSystem->メッセージ
	//		(FullName()+g_Lang(_T("Spellはずすメッセージ")));
	return true;
}


int cSpell::破壊_効果_通常(pcCharacter pchara)
{
	sg_pDungeonSystem->落ち物破壊要請(me());



	return true;
}
int cSpell::破壊_メッセージ_通常(pcCharacter pchara)
{
	return 破壊メッセージ定型(pchara);
}

int cSpell::破壊_効果_呪い(pcCharacter pchara)
{
	//効果なし
	return false;
}
int cSpell::破壊_メッセージ_呪い(pcCharacter pchara)
{
	return sg_pDungeonSystem->メッセージ(g_Lang(_T("Spell破壊不可メッセージ")));
}

int cSpell::破壊_効果_祝福(pcCharacter pchara)
{
	return 破壊_効果_通常(pchara);
}
int cSpell::破壊_メッセージ_祝福(pcCharacter pchara)
{
	return 破壊_メッセージ_通常(pchara);
}


int cSpell::破壊メッセージ定型(pcCharacter pchara)
{
	//sg_pDungeonSystem->AnimationManager().
	//	Anime_PlaySE(_T("drink.wav"),pchara->足元地形()->place);
	map<tstring, StyleString> valiable;
	valiable[_T("Item")] = FullName();
	g_Langメッセージ(_T("Spell破壊メッセージ"),valiable);

	//sg_pDungeonSystem->メッセージ
	//		(FullName()+g_Lang(_T("Spell破壊メッセージ")));
	return true;
}

int cSpell::スキルあり(pcCharacter pchara)
{
	return false;
}

int cSpell::スキル使用(pcCharacter pchara, vector<pcDroping> &ObjectList)
{
	int res = 0;
	res = スキル効果(pchara, ObjectList);
	Breakcrashprocess(スキルスペルダメージ(pchara));
	sg_pDungeonSystem->満腹度減少要請(pchara,スキルコスト満腹度(pchara),0);
	return res;
}
int cSpell::スキル_メッセージ(pcCharacter pchara ,vector<pcDroping> &ObjectList)
{
	//sg_pDungeonSystem->AnimationManager().
	//	Anime_PlaySE(_T("spell.wav"),pchara->足元地形()->place);
	
	map<tstring, StyleString> valiable;
	valiable[_T("Item")] = FullName();
	g_Langメッセージ(_T("Spellスキルメッセージ"),valiable);
	
	//sg_pDungeonSystem->メッセージ
	//		(FullName()+g_Lang(_T("Spell宣言メッセージ")));
	return true;
}
	

int cSpell::スキル効果(pcCharacter pchara, vector<pcDroping> &ObjectList)
{
	return false;
}
StyleString cSpell::スキル説明(pcCharacter pchara)
{
	if(!個別識別済み())
	{
		StyleString Base;
		map<tstring, StyleString> valiable;
		valiable[_T("Item")] = FullName();
		g_Lang(_T("Item未識別shortExplanation"),Base,valiable);
		return Base;
	}

	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.DropImportLanguage(ID(),
		(tstring)_T("SkillExplanationBase"));
	if(psr == NULL) return NoDataString();
	
	map<tstring,StyleString> valiable;
	StyleString sstr;

	int i, size = 効果量基礎_定義サイズ();
	for(i=0;i<size;i++)
	{
		効果量推定テキスト(i,sstr);
		valiable[(_T("Value")+setStyle(i)).c_str()] = sstr;
	}

	valiable[_T("Cost")] = setStyle(スキルコスト満腹度(pchara));

	sstr.clear();
	Drop_StringAddStyle(psr,sstr,valiable);
	return sstr;
}
	
int cSpell::スキルコスト満腹度(pcCharacter pchara)
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		ID(),(tstring)_T("スキルコスト満腹度"),デフォルトスキルコスト満腹度());
}
int cSpell::デフォルトスキルコスト満腹度()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		(tstring)_T("Spell基本値"),(tstring)_T("デフォルトスキルコスト満腹度"),2);
}
	
int cSpell::スキルスペルダメージ(pcCharacter pchara)
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		ID(),(tstring)_T("スキルスペルダメージ"),デフォルトスキルスペルダメージ());
}
int cSpell::デフォルトスキルスペルダメージ()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		(tstring)_T("Spell基本値"),(tstring)_T("デフォルトスキルスペルダメージ"),1);
}


void cSpell::CutIn(タイミング timing, cValiableField& valiable)
{
	cItem::CutIn(timing,valiable);

	if(timing == 攻撃直後時_タイミング)
	{
		/*
		if(装備されている())
		{
			Breakcrashprocess();
		}
		*/
	}
	else if(timing == 被攻撃直後時_タイミング)
	{
		if(装備されている())
		{
			Breakcrashprocess();
		}
	}
	else if(timing == 死亡直前_ラストスペル_タイミング)
	{
		if(装備されている())
		{
			if(valiable.doubles[変数_死亡回避] == 0)
			{//まだ死亡回避していない

				pcCharacter pchara = 装備者();

				if(state() != cDroping::STATE_CURSE)
				{
					map<tstring, StyleString> valiable;
					valiable[_T("Item")] = FullName();
					g_Langメッセージ(_T("SpellLastSpellメッセージ"),valiable);
					if(pchara && pchara->Forse == CHARACTER_FORSE_FRIEND)
					{
						pchara->LastSpelling = 1;
					}
				}
				if(LastSpell())
				{
					if(pchara && pchara->Forse == CHARACTER_FORSE_FRIEND)
					{
						
						valiable.doubles[変数_死亡回避] = 1;//味方なら死亡回避
						sg_pDungeonSystem->回復要請(pchara,1,0);//1回復
						pchara->LastSpelling = 0;
					}
					
				}

				pchara->LastSpelling = 0;

				sg_pDungeonSystem->落ち物破壊要請(me());


			}
		}
	}

}

/*
int cSpell::CutInEvent(int timing)
{
	if(timing == 攻撃直後時_タイミング)
	{
		if(装備されている())
		{
			return Breakcrashprocess();
		}
	}
	else if(timing == 被攻撃直後時_タイミング)
	{
		if(装備されている())
		{
			return Breakcrashprocess();
		}
	}

	return 0;
}
*/

//祝福切れる
bool cSpell::stategoodRunoutprocess()
{

	if(宣言時神力切れ確率() > random())
	{
		map<tstring, StyleString> valiable;
		valiable[_T("Item")] = FullName();
		g_Langメッセージ(_T("Spell神力切れメッセージ"),valiable);

		//sg_pDungeonSystem->メッセージ(
		//	FullName() + g_Lang(_T("Spell神力切れメッセージ")));
		
		state() = cDroping::STATE_NORMAL;	

		return true;
	}

	return false;
}


bool cSpell::Breakcrashprocess(int power)
{
	if(装備者() == NULL)
	{
		return false;
	}

	cValiableField valiable;
	valiable.doubles.dim(変数_汎用ブール) = 1;
	valiable.doubles.dim(変数_ダメージ) = power;
	valiable.drops.dim(変数_対象落ち物) = me();
	sg_pDungeonSystem->CutInM().CutIn(装備者(), スペルダメージ直前_タイミング, valiable);

	if(!valiable.doubles.val(変数_汎用ブール)) return false;


	if(破損している())
	{
		damage_count() += power;

		if(damage_count() <= 亀裂時最悪耐久回数())
		{
			return false;
		}

		power = damage_count() - 亀裂時最悪耐久回数();
	
		double pos = pow(1-min(亀裂時破損率(),1.0),power);

		if(pos < random())
		{
			pcCharacter pchara = 装備者();
			if(state() == cDroping::STATE_GOOD)
			{//祝福
				
				if(sg_pDungeonSystem->pPlayerChara() == pchara)
				{
					map<tstring, StyleString> valiable;
					valiable[_T("Item")] = FullName();
					g_Langメッセージ(_T("Spell自壊回避メッセージ"),valiable);
				}
				//sg_pDungeonSystem->メッセージ(
				//	g_Lang(_T("Spell自壊回避メッセージ_前半")) + FullName() + g_Lang(_T("Spell自壊回避メッセージ_後半")));
				
				damage_count() = 0;
				return stategoodRunoutprocess();
			}	
			else if(sg_pDungeonSystem->落ち物破壊要請(me()))
			{
				sg_pDungeonSystem->AnimationManager().
				Anime_PlaySE(_T("crashbreak.wav"),pchara->足元地形()->place);
			
				if(sg_pDungeonSystem->pPlayerChara() == pchara)
				{
					map<tstring, StyleString> valiable;
					valiable[_T("Item")] = FullName();
					g_Langメッセージ(_T("Spell自壊メッセージ"),valiable);
				}

				//sg_pDungeonSystem->メッセージ(
				//	g_Lang(_T("Spell自壊メッセージ_前半")) + FullName() + g_Lang(_T("Spell自壊メッセージ_後半")));

					
				if(!名称識別済み())
				{
					sg_pDungeonSystem->識別(me(),true);
				}
				else if(!完全識別済み())
				{
					sg_pDungeonSystem->識別(me(),false);
				}

				return true;
			}
		}

	}
	else
	{//通常時
		damage_count() += power;

		if(damage_count() <= 通常時最悪耐久回数())
		{
			return false;
		}

		power = damage_count() - 亀裂時最悪耐久回数();
	
		double pos = pow(1-min(通常時破損率(),1.0),power);

		if(pos < random())
		{
			if(state() == cDroping::STATE_GOOD)
			{//祝福

				if(sg_pDungeonSystem->pPlayerChara() == 装備者())
				{
					map<tstring, StyleString> valiable;
					valiable[_T("Item")] = FullName();
					g_Langメッセージ(_T("Spell自壊回避メッセージ"),valiable);
				}

				return stategoodRunoutprocess();
			}	
			else
			{
				破損する();

				return true;
			}
		}
	
	
	}


	return false;
}

bool cSpell::LastSpell()
{
	pcCharacter equipedChara = 装備者();
	bool result = 宣言強制要請(equipedChara,me());
	LastSpellAfterProcess(equipedChara);
	return result;
}


bool cSpell::破損する()
{
	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("crashbreak.wav"),現在地形()->place);

	if(sg_pDungeonSystem->pPlayerChara() == 装備者())
	{
		map<tstring, StyleString> valiable;
		valiable[_T("Item")] = FullName();
		g_Langメッセージ(_T("Spell破損メッセージ"),valiable);
	}
	//sg_pDungeonSystem->メッセージ(
	//	g_Lang(_T("Spell破損メッセージ_前半")) + FullName() + g_Lang(_T("Spell破損メッセージ_後半")));

	//亀裂
	damage_count() = 0;
	damage_criticalFlag() = TRUE;

	return true;
}
bool cSpell::破損回復(int message)
{

	if(message)
	{
		sg_pDungeonSystem->AnimationManager().
			Anime_PlaySE(_T("recover.wav"),現在地形()->place);

		if(sg_pDungeonSystem->pPlayerChara() == 装備者())
		{
			map<tstring, StyleString> valiable;
			valiable[_T("Item")] = FullName();
			g_Langメッセージ(_T("Spell破損回復メッセージ"),valiable);
		}
	}
	//sg_pDungeonSystem->メッセージ(
	//	g_Lang(_T("Spell破損回復メッセージ_前半")) + FullName() + g_Lang(_T("Spell破損回復メッセージ_後半")));

	//亀裂
	damage_count() = 0;
	damage_criticalFlag() = FALSE;

	return true;
}

//イメージカラー
unsigned long cSpell::imagecolar1_real()
{
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.DropImportData(ID(),
		(tstring)_T("imagecolar1_real"));
	if(psr == NULL) return 0xFFFFFFFF;
	TCHAR** end = NULL;
	return _tcstoul(psr->topscript().c_str(), end, 16);
}
unsigned long cSpell::imagecolar2_real()
{
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.DropImportData(ID(),
		(tstring)_T("imagecolar2_real"));
	if(psr == NULL) return 0xFFFFFFFF;
	TCHAR** end = NULL;
	return _tcstoul(psr->topscript().c_str(), end, 16);
}
	

unsigned long cSpell::imagecolar1()
{
	if(名称識別済み())
	{
		return imagecolar1_real();
	}
	else
	{
		return 0xFF808085;
	}
}
unsigned long cSpell::imagecolar2()
{
	if(名称識別済み())
	{
		return imagecolar2_real();
	}
	else
	{
		return 0xFFB0A8A0;
	}
}

void cSpell::longExplanationExtraBase(std::vector<StyleString> &explanations)
{
	cItem::longExplanationExtraBase(explanations);
	#ifdef _UNRELEASE
	//アンリリースモードのみ
	explanations.push_back(StyleString(_T("")));
	explanations.back() += _T("通常時最悪耐久回数 ");
	explanations.back() += 通常時最悪耐久回数();
	explanations.back() += _T("\n");

	explanations.back() += _T("亀裂時最悪耐久回数 ");
	explanations.back() += 亀裂時最悪耐久回数();
	explanations.back() += _T("\n");

	explanations.back() += _T("通常時破損率 ");
	explanations.back() += 通常時破損率();
	explanations.back() += _T("\n");

	explanations.back() += _T("亀裂時破損率 ");
	explanations.back() += 亀裂時破損率();
	explanations.back() += _T("\n");

	explanations.back() += _T("宣言時神力切れ確率 ");
	explanations.back() += 宣言時神力切れ確率();
	explanations.back() += _T("\n");

	explanations.back() += _T("スキルコスト満腹度 ");
	explanations.back() += スキルコスト満腹度(NULLCHARA);
	explanations.back() += _T("\n");

	explanations.back() += _T("スキルスペルダメージ ");
	explanations.back() += スキルスペルダメージ(NULLCHARA);
	explanations.back() += _T("\n");
	//int i;
	//int size = 効果量基礎_定義サイズ();
	//for(i=0;i<size;i++)
	//{
	//	explanations.back() += _T("効果量基礎");
	//	explanations.back() += i;
	//	explanations.back() += _T(" ");
	//	explanations.back() += 効果量基礎(i);
	//	explanations.back() += _T("\n");
	//}
	#endif
}


int cSpell::スペル定型エフェクト_宣言()
{


	pcaEspell pspelleffect = pcaEspell(new caEspell);

	pspelleffect->color1 = imagecolar1_real();
	pspelleffect->color2 = imagecolar2_real();

	pspelleffect->SpellName = SpellTitleFullName();

	pspelleffect->LayerLevel = LAYERLEVEL_SPELL;

	sg_pDungeonSystem->AnimationManager().AddAnime_parallel(pspelleffect);

	EffectFunctions::ボスエフェクト発散(me()->現在地形()->placeX, me()->現在地形()->placeY, cColor(imagecolar1_real()), cColor(imagecolar2_real()));

	EffectFunctions::スペルエフェクト(me()->現在地形()->placeX, me()->現在地形()->placeY, ID(), EffectFunctions::SpellEffectPart_class::SpellEffectPart_Main);

	return true;
}

int cSpell::スペル定型エフェクト_装備(pcCharacter pchara)
{

	cValiableField val;
	double temp;
	val.doubles.dim(tstring(_T("PlaceX")),temp = pchara->placeX);
	val.doubles.dim(tstring(_T("PlaceY")),temp = pchara->placeY);
	val.doubles.dim(tstring(_T("direction")),temp = pchara->aspect*45);
	val.doubles.dim(tstring(_T("imageColor1")),temp = imagecolar1());
	val.doubles.dim(tstring(_T("imageColor2")),temp = imagecolar2());
	val.charas.dim(tstring(_T("player")),pchara);


	bool didAnime = AnimationScript_AddAnimation(
		sg_pDungeonSystem->DataBase.EffectImport_Spell(),
		_T("EffectSpellEquip.lua"),
		val);	

	if(didAnime) return true;

	pcaECustom caec;
	cCoordinate coo;
	c4DVector center;
	pcAnimationChipManager_chip pcac;

	int i;
	caEchip_Belt caec_b_model;

	caEchip_Basic caec_model;

	//------------------------------------------------
	//ぐるぐる
	//------------------------------------------------
	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->place.set(pchara->placeX,pchara->placeY,0,0);
	
	//レイヤー
	//caec->LayerLevel = LAYERLEVEL_EXTRAEFFECT1;
	


	//テクスチャセット
	caec_b_model.DOb.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
		sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_smoke.png")),SIZE_BASICTEX,SIZE_BASICTEX);

	//テクスチャ座標セット
	caec_b_model.DOb.m_TexRange.setLTRB(0,0,1,1);


	//描画基本位置
	coo.SetAspect(pchara->aspect);
	center.set(pchara->placeX,pchara->placeY,0,0);
	center = sg_pDungeonSystem->Map().realvisibleplace(center);



	//基本サイズ
	//caec_model.DO.Width = 128;
	//caec_model.DO.Height = 128;




	
	//加算
	caec_b_model.DOb.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

	//色
	caec_b_model.ColorBase.inputD3Dcolor(0xFFFFFFFF);
	caec_b_model.Alpha.set(0,1.8,1.8,0);


	
	//表示時間
	caec_b_model.life = 60;
	caec_b_model.hidingTime = 0;


	//描画位置
	caec_b_model.CenterLeft = center;
	caec_b_model.CenterRight = center;
	caec_b_model.CenterLeft.z += 64+10;
	caec_b_model.CenterRight.z += 20 + 64+10;
	

	int length = 30;
	int numth;

	for(numth=0;numth<16;numth++)
	{

		//色
		if(rand() > RAND_MAX/2)
			caec_b_model.ColorBase.inputD3Dcolor(imagecolar1());
		else
			caec_b_model.ColorBase.inputD3Dcolor(imagecolar2());


		//表示時間
		caec_b_model.life = 50;
		caec_b_model.hidingTime = 10+ 30*rand()/RAND_MAX;

		double s = 1.0*rand()/RAND_MAX+1;
		double anglestart = (1.0*rand()/RAND_MAX+1)*360;
		
		//ライン定義
		caec_b_model.DOb.rightline().deleteCurveTrack();
		caec_b_model.DOb.leftline().deleteCurveTrack();
		for(i=1;i<length;i++)
		{
			c4DVector v;
			v.set(10,0,0,0);
			v*=(1.5)*i*i/length*i/length +4;
			v.turn(-30*i*s +numth*360/8 + anglestart);

			v.z -= 64*(i)/length ;
			v.y *= 0.75;

			caec_b_model.DOb.rightline().addPoint(v);
			caec_b_model.DOb.leftline().addPoint(v);
		}
		caec_b_model.DOb.rightline().stabilizeTrack();
		caec_b_model.DOb.leftline().stabilizeTrack();


		//ドラウイングレンジ
		caec_b_model.Startp.set(length-3,0,1,1);
		caec_b_model.Endp.set  (length  ,3,1,1);



		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Belt(caec_b_model));
		caec->addAnimechip(pcac);
	}

	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));
	
	//------------------------------------------------
	//ぼわっ
	//------------------------------------------------
	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->place.set(pchara->placeX,pchara->placeY,0,0);
	
	//レイヤー
	caec->LayerLevel = LAYERLEVEL_EXTRAEFFECT1;
	


	//テクスチャセット
	caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
		sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ring_nagi2.png")),SIZE_BASICTEX,SIZE_BASICTEX);

	//テクスチャ座標セット
	caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


	//描画基本位置
	coo.SetAspect(pchara->aspect);
	center.set(pchara->placeX, pchara->placeY, 0, 0);
	center = sg_pDungeonSystem->Map().realvisibleplace(center);



	//基本サイズ
	caec_model.DO.Width = 128;
	caec_model.DO.Height = 128;



	
	
	//シェイプオプション
	caec_model.ScaleX.set(1,1,1,0.7);
	caec_model.ScaleY.set(0.75,0.75,1,0.7);
	caec_model.Rotation.set(rand()*360/RAND_MAX, rand()*360/RAND_MAX + 360,1,1.5);
	//caec_model.Rotation2



	
	//加算
	caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

	//色
	caec_model.ColorBase.inputD3Dcolor(0xFFFFFFFF);
	caec_model.Alpha.set(0,2,2,0);


	for(i=0;i<8;i++)
	{
		//色
		if(rand() > RAND_MAX/2)
			caec_model.ColorBase.inputD3Dcolor(imagecolar1());
		else
			caec_model.ColorBase.inputD3Dcolor(imagecolar2());

		//描画位置
		c4DVector v(4,0,0,0);
		v.turn(360*rand()/RAND_MAX);

		caec_model.Centerbegin = center+v;
		caec_model.Centerbegin.y -= rand()*48/RAND_MAX;

		caec_model.Centerend = caec_model.Centerbegin;
		caec_model.Centerend.y -= 16;


		//表示時間
		caec_model.life = 20;
		caec_model.hidingTime = 10+ 40 - 10 + 30*rand()/RAND_MAX;	


		//チップに追加
		pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
		caec->addAnimechip(pcac);
	}
	
	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));


	//------------------------------------------------
	//カードくるくる
	//------------------------------------------------
	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->place.set(pchara->placeX,pchara->placeY,0,0);
	
	//レイヤー
	caec->LayerLevel = LAYERLEVEL_EXTRAEFFECT2;
	


	//テクスチャセット
	//caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
	//	sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ring_nagi2.png")),SIZE_BASICTEX,SIZE_BASICTEX);
	//テクスチャセット
	caec_model.DO.setTexture(NULL,SIZE_BASICTEX,SIZE_BASICTEX);


	//テクスチャ座標セット
	caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


	//描画基本位置
	coo.SetAspect(pchara->aspect);
	center.set(pchara->placeX, pchara->placeY, 0.25, 0);
	center = sg_pDungeonSystem->Map().realvisibleplace(center);



	//基本サイズ
	caec_model.DO.Width = 16;
	caec_model.DO.Height = 10;



	//描画位置
	caec_model.Centerbegin = center;
	caec_model.Centerbegin.y -= 64+16;

	caec_model.Centerend = caec_model.Centerbegin;
	caec_model.Centerend.y += 16;



	//表示時間
	caec_model.life = 90;
	caec_model.hidingTime = 0;	

	
	//シェイプオプション
	caec_model.ScaleX.set(1,1,1,1);
	caec_model.ScaleY.set(0.75,0.75,1,1);
	caec_model.Rotation.set(0, 1070,1,1.5);
	//caec_model.Rotation2





	
	//加算
	caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

	//色
	caec_model.ColorBase.inputD3Dcolor(imagecolar1());
	caec_model.Alpha.set(0,2,2,0);


	
	//チップに追加
	pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
	caec->addAnimechip(pcac);


	
	return sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));

}



