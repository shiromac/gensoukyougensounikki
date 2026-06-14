
#include "cDrink.h"
#include "Box1.h"
#include "cDungeonSystem.h"
#include "cCocktailWindow.h"

#include "Language.h"
#include "cCocktailWindow.h"

cDrink::cDrink(void)
{
}

cDrink::~cDrink(void)
{
}




//
//
StyleString cDrink::識別未識別混合道具名()
{
	StyleString sstr;
	if(reuse())
	{
		if(大局識別済み())
			return FullNameBase() + g_Lang(_T("詰め替え表現"));
		else
		{
			if(NamingCaption() == _T(""))
				return sg_pDungeonSystem->DataBase.未識別名(ID()) + g_Lang(_T("詰め替え表現"));
			else
				return g_Lang(_T("液体:")) + NamingCaption() + g_Lang(_T("詰め替え表現"));
		}
	}
	else
	{
		if(大局識別済み())
			return FullNameBase();
		else
		{
			if(NamingCaption() == _T(""))
				return sg_pDungeonSystem->DataBase.未識別名(ID());
			else
				return g_Lang(_T("液体:")) + NamingCaption();
		}
	}
}

void cDrink::AppreciationBeginOptimize(int difficulty)
{
	cItem::AppreciationBeginOptimize(difficulty);
	appreciation_state() = true;//状態識別済み
}

int cDrink::GetmenuCaption(vector<tstring> &CaptionList)
{
	if(倉庫の中())
	{
		CaptionList.push_back(_T("説明"));
		CaptionList.push_back(_T("倉庫から出す"));
		return true;
	}

	if(Holder() == NULL)
	{//はじめ
		if(Parent() == NULL)
		{
			if(sg_pDungeonSystem->pPlayerChara()->holdItem.size() >= sg_pDungeonSystem->pPlayerChara()->MaxholdNum())
				CaptionList.push_back(_T("交換"));
			else
				CaptionList.push_back(_T("拾う"));
		}
	}

	CaptionList.push_back(_T("飲む"));
	CaptionList.push_back(_T("投げる"));

	if(Holder() == NULL)
	{
		
	}
	else if(Holder()->足元() == NULL)
	{
		CaptionList.push_back(_T("置く"));
	}

	if(Holder() != NULL && Holder()->足元() != NULL && !内包されている())
	{//落ちてる
		if(Holder()->足元()->属性.count(落ち物属性::アイテム)) {
			CaptionList.push_back(_T("交換"));
		}
		else if(Holder()->足元()->属性.count(落ち物属性::罠) &&
			!Holder()->足元()->罠用_効果目標か(sg_pDungeonSystem->pPlayerChara())) {
			CaptionList.push_back(_T("交換"));
		}
	}

	if(!大局識別済み()) CaptionList.push_back(_T("名前"));

	CaptionList.push_back(_T("説明"));

	return true;
}
StyleString cDrink::ExtraHatString()
{
	StyleString ehs = cItem::ExtraHatString();

	if(cannotdrink())
	{
		ehs += g_Lang(_T("中身飲めずCaption"));
	}
	return ehs;
}

/*
//コマンドを解決する;
int cDrink::FireCommand(IDirect3DDevice9 *pDev, tstring verb, vector<pcDroping> &ObjectList)
{

	if(verb == _T("飲む"))
	{
		飲む(pDev);
	}


	if(verb == _T("説明")) 説明(pDev);
	if(verb == _T("拾う")) 拾う(pDev);
	if(verb == _T("置く")) 置く(pDev);
	if(verb == _T("投げる")) 投げる(pDev);
	if(verb == _T("交換")) 交換(pDev);

	return true;
}


int cDrink::飲む(IDirect3DDevice9 *pDev)
{
	int result = 0;


	if(state() == cDroping::STATE_NORMAL)
	{
		result = 飲む_メッセージ_通常(sg_pDungeonSystem->pPlayerChara());
	}
	else if(state() == cDroping::STATE_CURSE)
	{
		result = 飲む_メッセージ_呪い(sg_pDungeonSystem->pPlayerChara());
	}
	else if(state() == cDroping::STATE_GOOD)
	{
		result = 飲む_メッセージ_祝福(sg_pDungeonSystem->pPlayerChara());
	}

	if(result)
	{//使用に成功した
		sg_pDungeonSystem->メニューを閉じる();
		sg_pDungeonSystem->ターンエンド();
	}


	result = 飲み要請(sg_pDungeonSystem->pPlayerChara());


	return true;
}
int cDrink::飲み要請(pcCharacter pchara)
{



	int result = 0;
	if(state() == cDroping::STATE_NORMAL)
	{
		result = 飲む_効果_通常(pchara);
	}
	else if(state() == cDroping::STATE_CURSE)
	{
		result = 飲む_効果_呪い(pchara);
	}
	else if(state() == cDroping::STATE_GOOD)
	{
		result = 飲む_効果_祝福(pchara);
	}

	if(result)
	{//使用に成功した

		if(!名称識別済み())
		{
			sg_pDungeonSystem->識別(me(),true);
		}
		else if(!完全識別済み())
		{
			sg_pDungeonSystem->識別(me(),MASSAGE_IFCHANGE);
		}


		sg_pDungeonSystem->満腹度回復要請(pchara,5,false);
		sg_pDungeonSystem->落ち物破壊要請(me());
	}

	return result;
}

*/

int cDrink::飲む_効果_通常(pcCharacter pchara)
{
	sg_pDungeonSystem->メッセージ((StyleString)_T("cDrink 飲む_効果_通常() 効果が設定されていません。\n"));
	return !cannotdrink();
}
int cDrink::飲む_メッセージ_通常(pcCharacter pchara)
{
	return 飲むメッセージ定型(pchara);
}

int cDrink::飲む_効果_呪い(pcCharacter pchara)
{
	//効果なし
	return false;
}
int cDrink::飲む_メッセージ_呪い(pcCharacter pchara)
{
	sg_pDungeonSystem->メッセージ(g_Lang(_T("Drink呪いによる飲み不可メッセージ")));
	sg_pDungeonSystem->状態のみ識別(me(),false);
	return false;
}

int cDrink::飲む_効果_祝福(pcCharacter pchara)
{
	//飲む_効果_通常(pchara);
	return 飲む_効果_通常(pchara);
}
int cDrink::飲む_メッセージ_祝福(pcCharacter pchara)
{
	return 飲む_メッセージ_通常(pchara);
}


int cDrink::飲むメッセージ定型(pcCharacter pchara)
{
	if(cannotdrink())
	{
		sg_pDungeonSystem->メッセージ(g_Lang(_T("Drink冷凍による読み不可メッセージ")));
		return false;
	}


	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("drink.wav"),pchara->足元地形()->place);
	
	map<tstring, StyleString> valiable;
	valiable[_T("Item")] = FullName();
	g_Langメッセージ(_T("Drink飲みメッセージ"),valiable);

	//sg_pDungeonSystem->メッセージ
	//		(FullName()+_T(" を飲んだ。\n"));
	return true;
}


int cDrink::汲む_効果_通常(pcCharacter pchara, pcDroping& pdrop)
{
	sg_pDungeonSystem->メッセージ((StyleString)_T("cDrink 汲む_効果_通常() 効果が設定されていません。\n"));
	return true;
}
int cDrink::汲む_メッセージ_通常(pcCharacter pchara, pcDroping pdrop)
{
	return 汲むメッセージ定型(pchara,pdrop);
}

int cDrink::汲む_効果_呪い(pcCharacter pchara, pcDroping& pdrop)
{
	//効果なし
	return false;
}
int cDrink::汲む_メッセージ_呪い(pcCharacter pchara, pcDroping pdrop)
{
	sg_pDungeonSystem->メッセージ(g_Lang(_T("Drink呪いによる汲み不可メッセージ")));
	sg_pDungeonSystem->状態のみ識別(me(),false);
	return true;
}

int cDrink::汲む_効果_祝福(pcCharacter pchara, pcDroping& pdrop)
{
	//汲む_効果_通常(pchara);
	return 汲む_効果_通常(pchara,pdrop);
}
int cDrink::汲む_メッセージ_祝福(pcCharacter pchara, pcDroping pdrop)
{
	return 汲む_メッセージ_通常(pchara,pdrop);
}


int cDrink::汲むメッセージ定型(pcCharacter pchara, pcDroping pdrop)
{

	map<tstring, StyleString> valiable;
	valiable[_T("Item")] = pdrop->FullName();
	g_Langメッセージ(_T("Drink汲みメッセージ"),valiable);

	//sg_pDungeonSystem->メッセージ
	//		(FullName()+_T(" を汲んだ。\n"));
	return true;
}
int cDrink::カクテル_効果(pcCharacter pchara, vector<pcDroping> ObjectList)
{
	if(state() == STATE_CURSE) {
		sg_pDungeonSystem->メッセージ(g_Lang(_T("Drink呪いによる飲み不可メッセージ")));
		sg_pDungeonSystem->状態のみ識別(me(),false);
		return false;
	}

	pcControlLayer pccl;
	pcCocktailWindow pccw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(sg_pDungeonSystem->pDevice_D3D);
	pccl->notCancelToPop = true;

	pccl->WindowList.push_back(pccw = pcCocktailWindow(new cCocktailWindow));
	
	vector<int> materials(5);

	pcDrink pcDrk;
	int i,k;
	for(k=0;k<ObjectList.size();k++)
	{
		pcDrk = boost::dynamic_pointer_cast<cDrink>(ObjectList[k]);
		if(pcDrk == NULL) continue;
		if(pcDrk->名称識別済み())
		{
			for(i=0;i<cDrink::MATERIALNUM;i++)
			{
				materials[i] += pcDrk->マテリアル(i);
			}
		}
		else
		{
			for(i=0;i<cDrink::MATERIALNUM;i++)
			{
				materials[i] += 1;
			}
		}
		
		materials[i] += !(pcDrk->reuse());
	}
	
	materials[MATERIALNUM] += !(reuse()) - 1;
	
	if(state() == STATE_GOOD) {
		for(i=0;i<MATERIALNUM;i++)
		{
			materials[i] += 3;
		}
		materials[MATERIALNUM] += 1;
	}

	pccw->Init(sg_pDungeonSystem->pDevice_D3D, ObjectList, materials, false);
	pccw->setDelegate(new cCocktailWindowDelegate<cDrink>(boost::dynamic_pointer_cast<cDrink>(me())));
	return 2;//混酒の箱の例外処理
}
void cDrink::didFinishCocktail(const vector<pcDroping>& materialList, const vector<pcDroping>& resultCocktails)
{
	int i;
	for(i=0;i<resultCocktails.size();i++)
	{
		sg_pDungeonSystem->落ち物落下(resultCocktails[i],me()->現在地形());
	}

	for(i=0;i<materialList.size();i++)
	{
		sg_pDungeonSystem->落ち物破壊要請(materialList[i]);
	}

	sg_pDungeonSystem->落ち物破壊要請(me());
}
void cDrink::didCancelCocktail()
{

}

double cDrink::冷気時冷凍確率()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		(tstring)_T("Drink基本値"),(tstring)_T("冷気時冷凍確率"),0.05);
}
void cDrink::CutIn(タイミング timing, cValiableField& valiable)
{
	cItem::CutIn(timing,valiable);

	if(timing == 被攻撃接近直後_タイミング)
	{
		if(valiable.intsets.val(変数_属性).count(攻撃属性::冷気)
			&& random() < 冷気時冷凍確率() && !属性.count(落ち物属性::耐冷凍)
			&& !cannotdrink())
		{
			cValiableField newvaliable;
			newvaliable.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
			sg_pDungeonSystem->CutInM().CutIn(Holder(),水筒冷凍直前_タイミング,newvaliable);
			if(newvaliable.doubles.val(変数_汎用ブール))
			{
				map<tstring, StyleString> val;
				val[_T("Item")] = FullName();
				g_Langメッセージ(_T("Drink冷気付与メッセージ"),val);
				cannotdrink() = 1;
			}
		}
	}
	else if(timing == フロア開始_タイミング)
	{
		cannotdrink() = 0;
	}
}
int cDrink::異常状態解除()
{
	if (cannotdrink()) {
		map<tstring, StyleString> val;
		val[_T("Item")] = FullName();
		g_Langメッセージ(_T("Drink冷気解消メッセージ"),val);
		cannotdrink() = 0;
		return true;
	}
	return false;
}
int cDrink::衝突(pcCharacter pchara)
{
	int result = 0;


	if(衝突時ばら撒き())
	{
		result = ばら撒き効果(pchara);
	}
	else
	{
		result = 衝突時効果(pchara);
	}

	if(result)
	{//使用に成功した
		sg_pDungeonSystem->落ち物破壊要請(me());
	}
	
	return result;
}

int cDrink::衝突時ばら撒き()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(ID(), (tstring)_T("衝突時ばら撒き"), 1);
}

int cDrink::衝突時効果(pcCharacter pchara)
{


	衝突時_メッセージ(pchara);

	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;
	valf.drops.dim(変数_対象落ち物) = me();
	sg_pDungeonSystem->CutInM().CutIn(pchara,飲み物衝突効果直前_タイミング,valf);
		
	if(state() == cDroping::STATE_NORMAL)
	{
	
		if(valf.doubles.dim(変数_汎用ブール))
			return 飲む_効果_通常(pchara);
		else
			return false;
	}
	else if(state() == cDroping::STATE_GOOD)
	{

		if(valf.doubles.dim(変数_汎用ブール))
			return 飲む_効果_祝福(pchara);
		else
			return false;
	}
	else
	{
		return cDroping::衝突(pchara);
	}
}
int cDrink::ばら撒き効果(pcCharacter pchara)
{

	int i;

	pcLandform pland = pchara->足元地形();
	for(i=0;i<9;i++)
	{
		pcCharacter pchara2;
		pchara2 = sg_pDungeonSystem->代理地形(pland,i)->pOnChar;
		if(pchara2 != NULL && pchara->Forse == pchara2->Forse)
		{

	
				
			衝突時効果(pchara2);
		}
	}

	return true;
}

int cDrink::衝突時_メッセージ(pcCharacter pchara)
{
	if(state() == cDroping::STATE_NORMAL || state() == cDroping::STATE_GOOD)
	{
		if(衝突時ばら撒き())
		{
			map<tstring, StyleString> valiable;
			valiable[_T("Chara")] = pchara->ShortName();
			valiable[_T("Item")] = FullName();
			g_Langメッセージ(_T("Drink衝突時ばら撒きメッセージ"),valiable);

			//sg_pDungeonSystem->メッセージ( pchara->FullName()+_T("は ")+
			//	FullName() + _T(" を頭からかぶった！\n"));

		}
		else
		{
			map<tstring, StyleString> valiable;
			valiable[_T("Item")] = FullName();
			g_Langメッセージ(_T("Drink衝突時メッセージ"),valiable);
			
			//sg_pDungeonSystem->メッセージ((FullName()+_T(" を飲んでしまった。\n")));
		}
	
	}
	else
	{
		return 飲む_メッセージ_呪い(pchara);
	}




	return true;
}
void cDrink::能力効果量推定テキスト(int index, StyleString& output)
{
	double qualityPower = 1.0;
	double statePower = 1.0;
	if(修正値識別済み())
		qualityPower = 効果量係数();//効果量状態倍率を除く
	if(状態値識別済み())
		statePower = 効果量状態倍率();

	if(!修正値識別済み() || !状態値識別済み())
	{
		output.clear();
		output.addstring((int)効果量基礎(index),_T("(%d)"),ITEM_HALFAPP_COLOR);
		return;
	}
	else
	{
		output.clear();
		if(御霊状態()) output.addstring((int)(効果量(index)));
		else output.addstring((int)(効果量(index)/効果量状態倍率()));
		
		return;
	}
}
double cDrink::能力スロット()
{
	int slot = cItem::能力スロット();


	return ceil(slot*効果量係数());
}
double cDrink::効果量修正値倍率()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(ID(), (tstring)_T("効果量修正値倍率"), 0.5);
}
int cDrink::マテリアル(int index)
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(ID(), (tstring)_T("マテリアル"), 0, index)*(max(1,1+quality()))*効果量状態倍率();
}
int cDrink::カクテルレシピから削除()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(ID(), (tstring)_T("カクテルレシピから削除"), 0);
}


#define DEF_DrinkAttributeExpresstions(str) \
if(属性.count(落ち物属性::str)){\
explanations.back() += g_Lang(_T(#str) _T("Expresstions"));}


void cDrink::longExplanationExtraBase(std::vector<StyleString> &explanations)
{
	cItem::longExplanationExtraBase(explanations);

	explanations.push_back(StyleString(_T("")));

	explanations.back() += g_Lang(_T("DrinkAttributeExpresstions"));

	DEF_DrinkAttributeExpresstions(薬)
	DEF_DrinkAttributeExpresstions(酒)
	DEF_DrinkAttributeExpresstions(火消し)

	explanations.push_back(StyleString(_T("")));

	int i;
	vector<int> vmaterial(MATERIALNUM+1,0);

	explanations.back() += g_Lang(_T("DrinkMaterialExpresstions"));

	for(i=0;i<cDrink::MATERIALNUM;i++)
	{
		vmaterial[i] += マテリアル(i);
	}
	vmaterial[i] += !(reuse());
	
	getMaterialText(vmaterial,explanations.back());
}

#define Drink_BASICVALUESTR (_T("Drink基本値"))

double cDrink::デフォルト効果量状態倍率_祝福()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value((tstring)Drink_BASICVALUESTR, (tstring)_T("デフォルト効果量状態倍率_祝福"), 2.0);
}
