#include "stdafx.h"

#include "cEquipment.h"

#include "FireCommand.h"

#include "scriptprocesser/AnimationScript.h"

#include "cDungeonSystem.h"

#include "Language.h"
#include "cStringWindow.h"

#include "gameMainSystem/filemanage/cScriptRLayer.h"
#include "scriptprocesser/DropsScript.h"

#include "cDropingFunc.h"

#include "AttackAnimeAttriDEF.h"

#define BREAK_POSSIBILITY_BASE (0.02)
#define RUNOUTGOOD_POSSIBILITY (0.2)


#define COLOR_ATTACKPOINT (0xFFFF5555)
#define COLOR_DEFFENCEPOINT (0xFF5599FF)
#define COLOR_UNAPPREPOINT (0xFFE0E055)


#define EQUIPMENT_BASICVALUESTR (_T("Equipment基本値"))

cEquipment::cEquipment(void)
{
	weak_pEquiper_Attack = wpcCharacter(NULLCHARA);
	weak_pEquiper_Defence = wpcCharacter(NULLCHARA);
}

cEquipment::~cEquipment(void)
{
}

//消去処理の準備
void cEquipment::Release()
{
	unequip();	

}


StyleString cEquipment::能力タイトルCaption()
{
	StyleString sstr;
	map<tstring, StyleString> valiable;
	//valiable[_T("Item")] = FullName();
	valiable[_T("SlotExpression")] = 能力スロット表現();
	valiable[_T("SlotVolume")] = setStyle((int)スロット容量());
	valiable[_T("SlotCapacity")] = setStyle((int)スロットキャパシティ());
	g_Lang(_T("Equip能力タイトルCaption"), sstr, valiable);
	return sstr;
}

void cEquipment::DataBeginOptimize(int difficulty)
{
	quality() = qualityBeginDP(difficulty).get(random());

	if(quality() < 0)
	{//負
		state() = cDroping::STATE_CURSE;
	}
	else
	{
		state() = stateBeginDP(difficulty).get(random());
	}

	untiappreciate();
}

int cEquipment::値段補正値()
{
	int value = 値段基礎値()
		*(1.0 + 0.1*quality())
		*(1.0 + 0.5*overspec())
		*(1.0 + (int)(state() == cDroping::STATE_GOOD))
		*(1.0 - 0.5*(int)(crash()));
	
	int i, size = includedItem.size();
	for(i=0;i<size;i++)
	{
		value += includedItem[i]->値段基礎値()/2;
	}
	return max(0,value);
}
int cEquipment::厄ポイント値段補正値()
{
	int value = 厄ポイント値段基礎値()
		*(1.0 + 0.1*quality())
		*(1.0 + 0.5*overspec())
		*(1.0+(int)(state() == cDroping::STATE_GOOD))
		*(1.0+熟練度()*2.0/熟練度最大());
	
	int i, size = includedItem.size();
	for(i=0;i<size;i++)
	{
		value += includedItem[i]->厄ポイント値段基礎値();
	}
	return max(0,value);
}

double cEquipment::効果時神力切れ確率()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		ID(),(tstring)_T("効果時神力切れ確率"),デフォルト効果時神力切れ確率());
}

tstring cEquipment::IconFileName()
{
	pcScriptRLayer psr = sg_pDungeonSystem->DataBase.DropImportData(
		ID(),(tstring)_T("IconFileName"));
	if(psr == NULL)
	{
		return _T("item\\武器.png");
	}
	return psr->topscript();
}
int cEquipment::IconFileIndexX()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		ID(),(tstring)_T("IconFileIndexX"),0);
}
int cEquipment::IconFileIndexY()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		ID(),(tstring)_T("IconFileIndexY"),0);
}

double cEquipment::デフォルト効果時神力切れ確率()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		(tstring)EQUIPMENT_BASICVALUESTR, (tstring)_T("デフォルト効果時神力切れ確率"),RUNOUTGOOD_POSSIBILITY);
}

StyleString cEquipment::識別未識別混合道具名()
{
	StyleString sstr;
	if(pmemo == NULL || pmemo->str() == _T(""))
		sstr = FullNameBase();
	else
		sstr = pmemo->str();

	int i;
	for(i=0;i<includedItem.size();i++)
	{
		sstr += includedItem[i]->能力略名Full();
	}


	if(crash()) sstr += g_Lang(_T("(破損)"));

	return sstr;
}

StyleString cEquipment::FullName()
{
	if(御霊状態())
	{
		return 能力名Full();
	}
	else
	{
		return cItem::FullName();
	}
}

tstring cEquipment::形容詞()
{


	return (tstring)_T("");
}


int cEquipment::熟練度()
{
	return 熟練度計算(proficiency_count());
}
int cEquipment::熟練度計算(int pfc)
{
	//pfc /= 100;
	pfc = sqrt((double)pfc);
	if(pfc > 熟練度最大()) pfc = 熟練度最大();
	return pfc;
}
void cEquipment::熟練度カウント加算(int pfc)
{
	if(熟練度計算(proficiency_count()) < 熟練度最大())
	{
		proficiency_count() += pfc;		
	}
}
double cEquipment::熟練度成長倍率(const double selfLV, const double oppsLV)
{
	return max(0,min(1,(oppsLV - selfLV + 3)*4/selfLV + 1.0));
}

int cEquipment::熟練度最大()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(ID(), (tstring)_T("熟練度最大"), 999);
}
StyleString cEquipment::熟練度キャプション()
{
	if(熟練度() != 0)
	{
		StyleString sstr;
		map<tstring, StyleString> valiable;
		valiable[_T("Proficiency")] = setStyle(熟練度());
		g_Lang(_T("熟練度キャプション"), sstr, valiable);
		return sstr;
	}
	else
	{
		return (StyleString)_T("");
	}

}
/*
cDiscreteProbability cItem::stateBeginDP(int difficulty)
{
	cDiscreteProbability DP;
	DP.set(7,cDroping::STATE_NORMAL);
	DP.set(1,cDroping::STATE_GOOD);
	return DP;
}
*/
/*
cDiscreteProbability cEquipment::qualityBeginDP(int difficulty)
{
	cDiscreteProbability DP;
	DP.set(3,-1);//3

	DP.set(13,0);//27
	DP.set(9,1);
	DP.set(4,2);
	DP.set(1,3);
	return DP;
}
*/

void cEquipment::DrawStateIconSub(cRenderDevice *pDev,int x,int y)
{
	cDrawingObject DO;

	int upperline = y-8;
	int bottomline = y+8;
	int liftline = x+8;
	int rightline = x+8+16+8;


	DO.Width = 16;
	DO.Height = 16;

	//本体
	DO.m_color.ARGB(255,255,255,255);
	DO.m_color *= m_color;
	DO.setTexture(m_pTexture_StateIcon,64,64);


	if(装備マーク攻撃())
	{//攻撃装備アイコン
		DO.CenterX = rightline;
		DO.CenterY = upperline;

		DO.m_TexRange.setLTRB(0,0.25,0.25,0.5);
		DO.Draw(pDev);
	}
	
	if(装備マーク防御())
	{
		if(装備マーク攻撃())
		{//防御装備アイコン
			DO.CenterY = bottomline;
		}
		else
		{
			DO.CenterY = upperline;
		}
		DO.CenterX = rightline;

		DO.m_TexRange.setLTRB(0.25,0.25,0.5,0.5);
		DO.Draw(pDev);
	}

	if(過修正である())
	{//過修正アイコン
		DO.CenterX = liftline;
		DO.CenterY = upperline;

		DO.m_TexRange.setLTRB(0.0,0.5,0.25,0.75);
		DO.Draw(pDev);
	}



		
}


void cEquipment::攻撃エフェクト(pcCharacter pchara)
{
	cValiableField val;
	double temp;
	val.doubles.dim(tstring(_T("fromPlaceX")),temp = pchara->placeX);
	val.doubles.dim(tstring(_T("fromPlaceY")),temp = pchara->placeY);
	val.charas.dim(tstring(_T("attacker")),pchara);
	int outputaspect = 0, outputdistance = 0;
	pcLandform land = pchara->GetAttackLand(outputaspect, outputdistance);
	val.doubles.dim(tstring(_T("direction")),temp = outputaspect*45);
	val.doubles.dim(tstring(_T("distance")),temp = outputdistance);
	val.doubles.dim(tstring(_T("toPlaceX")),temp = land->placeX);
	val.doubles.dim(tstring(_T("toPlaceY")),temp = land->placeY);

	val.doubles.dim(tstring(_T("isValidAttack")),temp = (double)sg_pDungeonSystem->isCanNeighberAttack(pchara,pchara->aspect,1,0,0));

	bool didAnime = AnimationScript_AddAnimation(
		sg_pDungeonSystem->DataBase.EffectImport_Equipment_Attack(),
		sg_pDungeonSystem->DataBase.EffectImport_Name_Equipment_Attack(ID()),
		val);	

	if(didAnime)
	{
		return;
	}
	else
	{
		sg_pDungeonSystem->AnimationManager().
			EasyAnime_Attackcharacter(pchara,
										ATTACKATTRI_SWING1);
	}
}


void cEquipment::防御エフェクト(pcCharacter pchara)
{
	//仮
	sg_pDungeonSystem->AnimationManager().
		EasyAnime_Defensecharacter(pchara,
									DEFENSEATTRI_SWING1);
}

bool cEquipment::両手装備()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(ID(), (tstring)_T("両手装備"), 0);
}


pcCharacter cEquipment::装備者()
{
	if(Parent())
	{
		pcEquipment pcequ = boost::dynamic_pointer_cast<cEquipment>(Parent());
		if(pcequ == NULL) return NULLCHARA;
		return pcequ->装備者();
	}
	else
	{
		if(weak_pEquiper_Attack.lock() != NULL)
		{
			return weak_pEquiper_Attack.lock();
		}
		if(weak_pEquiper_Defence.lock() != NULL)
		{
			return weak_pEquiper_Defence.lock();
		}
	}

	return NULLCHARA;
}
pcCharacter cEquipment::装備者_攻撃用()
{
	return 装備者();//仕様変更でとりあえず装備してたら返すように
}
pcCharacter cEquipment::装備者_防御用()
{
	return 装備者();//仕様変更でとりあえず装備してたら返すように
}


int cEquipment::GetmenuCaption(vector<tstring> &CaptionList)
{
	if(倉庫の中())
	{
		CaptionList.push_back(_T("説明"));
		if(能力スロット() > 0 || !includedItem.empty())
			CaptionList.push_back(_T("能力"));

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
	

		CaptionList.push_back(_T("投げる"));
	}
	else
	{//持ってる
	
		if(装備されている_攻撃用() || 装備されている_防御用())
		{//装備されてる
			CaptionList.push_back(_T("はずす"));

			if(state() != cDroping::STATE_CURSE )
			{//呪われて無い
				CaptionList.push_back(_T("投げる"));
			
				if(Holder()->足元() == NULL)
				{//足元に何も無い
					CaptionList.push_back(_T("置く"));
				}		
			}

		}
		else
		{//装備されて無い

			if(!crash())
				if(!内包されている())
					CaptionList.push_back(_T("装備"));

			CaptionList.push_back(_T("投げる"));

			if(Holder()->足元() == NULL)
			{//足元に何も無い
				CaptionList.push_back(_T("置く"));
			}
			else
			{//足元に何かある
				if(!内包されている() && Holder()->足元()->属性.count(落ち物属性::アイテム)) CaptionList.push_back(_T("交換"));
				else if(!内包されている() &&
					Holder()->足元()->属性.count(落ち物属性::罠) &&
					!Holder()->足元()->罠用_効果目標か(sg_pDungeonSystem->pPlayerChara())) CaptionList.push_back(_T("交換"));
			}
		}


	
	

	}

	if(熟練度() >= 100)
	{
		CaptionList.push_back(_T("メモ"));
	}

	CaptionList.push_back(_T("説明"));

	if(能力スロット() > 0 || !includedItem.empty())
		CaptionList.push_back(_T("能力"));

	return true;
}
int cEquipment::EnableShortCutCaptions(vector<tstring> &CaptionList){
	GetmenuCaption(CaptionList);
	if(std::count(CaptionList.begin(), CaptionList.end(), _T("装備")))
	{
		CaptionList.push_back(_T("攻撃用に装備"));
		CaptionList.push_back(_T("防御用に装備"));
	}
	return true;
};
//captionのexplainを得る
StyleString cEquipment::GetmenuExplain(tstring &Caption)
{
	if(Caption == _T("攻撃用に装備") || Caption == _T("防御用に装備") || (Caption == _T("装備") && 両手装備()) || Caption == _T("はずす"))
	{
		
		pcCharacter pchara = sg_pDungeonSystem->pPlayerChara();
		
	
		pcDroping afterAttEqu = pchara->attackequipment 
					,afterDefEqu = pchara->defenseequipment;

		if(Caption == _T("攻撃用に装備"))
		{
			if(afterAttEqu == afterDefEqu)
			{//両手装備または両手装備されて無い
				afterAttEqu = NULLDROP;
				afterDefEqu = NULLDROP;
			}
			afterAttEqu = me();
			
		}	
		else if(Caption == _T("防御用に装備"))
		{
			if(afterAttEqu == afterDefEqu)
			{//両手装備または両手装備されて無い
				afterAttEqu = NULLDROP;
				afterDefEqu = NULLDROP;
			}
			afterDefEqu = me();
		}
		else if(Caption == _T("装備"))
		{//両手装備
			afterAttEqu = me();
			afterDefEqu = me();
		}
		else if(Caption == _T("はずす"))
		{
			if(pchara->attackequipment == me())
			{
				afterAttEqu = NULLDROP;
			}
			if(pchara->defenseequipment == me())
			{
				afterDefEqu = NULLDROP;
			}
		}
		
		double before,after,afterExtra;
		unsigned long color;
		StyleString explain,temp;
		bool NoAppreciationFlag;


		//攻撃
		NoAppreciationFlag = false;
		before = pchara->calcuAttackPower(pchara->attackequipment);
		before += pchara->calcuExtraAttackPower(pchara->defenseequipment);

		map<tstring, StyleString> valiable;
		
		if(afterDefEqu != afterAttEqu)
		{
			if(afterDefEqu == NULL || afterDefEqu->修正値識別済み())
			{		
				afterExtra = pchara->calcuExtraAttackPower(afterDefEqu);
			}
			else
			{
				afterExtra = pchara->calcuNoAppreciationExtraAttackPower(afterDefEqu);
				if(afterExtra > 0) NoAppreciationFlag = true;
			}
		}
		else
		{
			afterExtra = 0;
		}

		if(afterAttEqu == NULL || afterAttEqu->修正値識別済み())
		{		
			after = pchara->calcuAttackPower(afterAttEqu);
			after += afterExtra;

			if(after > before)			color = 0xFFFF7777;
			else if(after == before)	color = 0xFFFFFFFF;
			else						color = 0xFF7777FF;
		

		}
		else
		{
			after = pchara->calcuAttackPower((boost::dynamic_pointer_cast<cEquipment>(afterAttEqu))->武器力基礎値());
			after += afterExtra;

			NoAppreciationFlag = true;
		}

		if(NoAppreciationFlag)
		{
			color = 0xFFFFFF66;

			valiable[_T("Estimate")] = setStyle(g_Lang(_T("(予測値)")).conclete_tstr(),color);
			//explain = setStyle(_T("攻撃力  "))+setStyle((int)before,_T("%3d"))+_T(" ⇒ ")+setStyle((int)after,_T("%3d"),color)+ setStyle(_T("(予測値)"),color)+ _T("\n");
	
		}
		else
		{
			valiable[_T("Estimate")] = _T("");
			//explain = setStyle(_T("攻撃力  "))+setStyle((int)before,_T("%3d"))+_T(" ⇒ ")+setStyle((int)after,_T("%3d"),color)+_T("\n");
	
		}

		valiable[_T("Before")] = setStyle((int)before,_T("%3d"));
		valiable[_T("After")] = setStyle((int)after,_T("%3d"),color);


		g_Lang(_T("装備変更攻撃用説明"), explain, valiable);
		

		//防御
		NoAppreciationFlag = false;
		before = pchara->calcuDefencePower(pchara->defenseequipment);
		before += pchara->calcuExtraDefencePower(pchara->attackequipment);
		
		if(afterDefEqu != afterAttEqu)
		{
			if(afterAttEqu == NULL || afterAttEqu->修正値識別済み())
			{		
				afterExtra = pchara->calcuExtraDefencePower(afterAttEqu);
			}
			else
			{
				
				afterExtra = pchara->calcuNoAppreciationExtraDefencePower(afterAttEqu);
				if(afterExtra > 0) NoAppreciationFlag = true;
			}
		}
		else
		{
			afterExtra = 0;
		}

		if(afterDefEqu == NULL || afterDefEqu->修正値識別済み())
		{		
			after = pchara->calcuDefencePower(afterDefEqu);
			after += afterExtra;

			if(after > before)			color = 0xFFFF7777;
			else if(after == before)	color = 0xFFFFFFFF;
			else						color = 0xFF7777FF;
		

		}
		else
		{
			after = pchara->calcuDefencePower((boost::dynamic_pointer_cast<cEquipment>(afterDefEqu))->防具力基礎値());
			after += afterExtra;

			NoAppreciationFlag = true;
		}


		if(NoAppreciationFlag)
		{
			color = 0xFFFFFF66;

			valiable[_T("Estimate")] = setStyle(g_Lang(_T("(予測値)")).conclete_tstr(),color);
			//explain += setStyle(_T("防御力  "))+setStyle((int)before,_T("%3d"))+_T(" ⇒ ")+setStyle((int)after,_T("%3d"),color)+ setStyle(_T("(予測値)"),color)+_T("\n");
		
		}
		else
		{
			valiable[_T("Estimate")] = _T("");
			//explain += setStyle(_T("防御力  "))+setStyle((int)before,_T("%3d"))+_T(" ⇒ ")+setStyle((int)after,_T("%3d"),color)+_T("\n");
			
		}

		valiable[_T("Before")] = setStyle((int)before,_T("%3d"));
		valiable[_T("After")] = setStyle((int)after,_T("%3d"),color);
		g_Lang(_T("装備変更防御用説明"), temp, valiable);
		

		explain += temp;

	

		if(過修正である())
		{
			explain += setStyle(g_Lang(_T("装備過修正警告")).conclete_tstr(),0xFFFF0000);
		}
		
		return explain;
	}
	return _T("");
}

/*
//コマンドを解決する;
int cEquipment::FireCommand(cRenderDevice *pDev, tstring verb, vector<pcDroping> &ObjectList)
{

	if(verb == _T("装備"))
	{
		if(両手装備())
		{
			装備(pDev,EQUIP_PART_ATTACK);//EQUIP_PART_ATTACKは意味なし
		}
		else
		{
			装備選択(pDev);
		}
	}

	if(verb == _T("攻撃用に装備"))
	{
		装備(pDev,EQUIP_PART_ATTACK);
	}
	if(verb == _T("防御用に装備"))
	{
		装備(pDev,EQUIP_PART_DEFENSE);
	}

	if(verb == _T("はずす"))
	{
		はずす(pDev);
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
int cEquipment::装備選択(cRenderDevice *pDev)
{
	vector<tstring> captions;

	//GetmenuCaption(captions);
	captions.push_back(_T("攻撃用に装備"));
	captions.push_back(_T("防御用に装備"));

	if(captions.size() == 0) return false;

	pcControlLayer pccl;
	pcSelectWindow pcsw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));
	

	int i;
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


int cEquipment::装備(cRenderDevice *pDev, int part)
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


int cEquipment::装備要請(pcCharacter pchara, int part)
{
	if(part == EQUIP_PART_ATTACK && !装備可_攻撃用())
	{//装備できない
		return false;
	}
	if(part == EQUIP_PART_DEFENSE && !装備可_防御用())
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
	else
	{//失敗した
		if(pchara == sg_pDungeonSystem->pPlayerChara())
		{
			sg_pDungeonSystem->メッセージ(_T("今の装備がはずせないので装備できない!\n"));
		}
	}

	return result;
}
*/

int cEquipment::装備_効果_通常(pcCharacter pchara, int part)
{
	int result = 1;
	if(両手装備())
	{
		if(pchara->attackequipment == NULLDROP && pchara->defenseequipment == NULLDROP)
		{//装備されて無い
			equip(pchara,EQUIP_PART_ATTACK);//EQUIP_PART_ATTACKは意味なし
		}
		else
		{//装備されてる
			pcEquipment pcEqu;
			pcEqu = boost::dynamic_pointer_cast<cEquipment>(pchara->attackequipment);
			if(pcEqu != NULL)
			{
				result = Equipment装備はずし要請(pchara,pchara->attackequipment);
			}
			else
			{
				//result = false;
			}
			
			if(result == false)
			{//装備はずせなかった
				return false;
			}

			pcEqu = boost::dynamic_pointer_cast<cEquipment>(pchara->defenseequipment);
			if(pcEqu != NULL)
			{
				result = Equipment装備はずし要請(pchara,pchara->defenseequipment);
			}
			else
			{
				//result = false;
			}
			if(result == false)
			{//装備はずせなかった
				return false;
			}

			equip(pchara,EQUIP_PART_ATTACK);//EQUIP_PART_ATTACKは意味なし
		}
	}
	else
	{
		if(part == EQUIP_PART_ATTACK)
		{
			if(pchara->attackequipment == NULLDROP)
			{//装備されて無い
				
			}
			else
			{//装備されてる
				pcEquipment pcEqu;
				pcEqu = boost::dynamic_pointer_cast<cEquipment>(pchara->attackequipment);
				if(pcEqu != NULL)
				{
					result = Equipment装備はずし要請(pchara,pchara->attackequipment);
				}
				else
				{
					//result = false;
				}
				if(result == false)
				{//装備はずせなかった
					return false;
				}
			}
			
			equip(pchara,EQUIP_PART_ATTACK);

		}
		else if(part == EQUIP_PART_DEFENSE)
		{
			if(pchara->defenseequipment == NULLDROP)
			{//装備されて無い
				
			}
			else
			{//装備されてる
				pcEquipment pcEqu;
				pcEqu = boost::dynamic_pointer_cast<cEquipment>(pchara->defenseequipment);
				if(pcEqu != NULL)
				{
					result = Equipment装備はずし要請(pchara,pchara->defenseequipment);
				}
				else
				{
					//result = false;
				}
				if(result == false)
				{//装備はずせなかった
					return false;
				}
			}

			equip(pchara,EQUIP_PART_DEFENSE);

		
		}


	}



	装備メッセージ定型(pchara,part);
	return true;
}
int cEquipment::装備_メッセージ_通常(pcCharacter pchara, int part)
{
	
	return true;
}

int cEquipment::装備_効果_呪い(pcCharacter pchara, int part)
{
	return 装備_効果_通常(pchara,part);
}
int cEquipment::装備_メッセージ_呪い(pcCharacter pchara, int part)
{
	return 装備_メッセージ_通常(pchara,part);
}

int cEquipment::装備_効果_祝福(pcCharacter pchara, int part)
{
	return 装備_効果_通常(pchara,part);
}
int cEquipment::装備_メッセージ_祝福(pcCharacter pchara, int part)
{
	return 装備_メッセージ_通常(pchara,part);
}


int cEquipment::装備メッセージ定型(pcCharacter pchara, int part)
{
	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("equip.wav"),pchara->足元地形()->place);

	if(両手装備())
	{
		map<tstring, StyleString> valiable;
		valiable[_T("Item")] = FullName();
		g_Langメッセージ(_T("Equip両手装備メッセージ"),valiable);
		
		//sg_pDungeonSystem->メッセージ
		//		(FullName()+_T(" を装備した。\n"));
	}
	else
	{
		if(part == EQUIP_PART_ATTACK)
		{
			map<tstring, StyleString> valiable;
			valiable[_T("Item")] = FullName();
			g_Langメッセージ(_T("Equip攻撃装備メッセージ"),valiable);
		
			//sg_pDungeonSystem->メッセージ
			//		(FullName()+_T(" を攻撃用に装備した。\n"));
		}
		else if(part == EQUIP_PART_DEFENSE)
		{
			map<tstring, StyleString> valiable;
			valiable[_T("Item")] = FullName();
			g_Langメッセージ(_T("Equip防御装備メッセージ"),valiable);
		
			//sg_pDungeonSystem->メッセージ
			//		(FullName()+_T(" を防御用に装備した。\n"));	
		}
	}
	return true;
}

/*
int cEquipment::はずす(cRenderDevice *pDev)
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
		sg_pDungeonSystem->ターンエンド();
	}

	return true;
}


int cEquipment::装備はずし要請(pcCharacter pchara)
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

	}

	return result;
}
*/

int cEquipment::はずす_効果_通常(pcCharacter pchara)
{
	unequip();



	return true;
}
int cEquipment::はずす_メッセージ_通常(pcCharacter pchara)
{
	はずすメッセージ定型(pchara);
	return true;
}

int cEquipment::はずす_効果_呪い(pcCharacter pchara)
{
	//効果なし
	return false;
}
int cEquipment::はずす_メッセージ_呪い(pcCharacter pchara)
{
	sg_pDungeonSystem->メッセージ(g_Lang(_T("Equip呪いによるはずし不可メッセージ")));
	return false;
}

int cEquipment::はずす_効果_祝福(pcCharacter pchara)
{
	はずす_効果_通常(pchara);
	return true;
}
int cEquipment::はずす_メッセージ_祝福(pcCharacter pchara)
{
	はずす_メッセージ_通常(pchara);
	return true;
}


int cEquipment::はずすメッセージ定型(pcCharacter pchara)
{
	//sg_pDungeonSystem->AnimationManager().
	//	Anime_PlaySE(_T("drink.wav"),pchara->足元地形()->place);
	
	map<tstring, StyleString> valiable;
			valiable[_T("Item")] = FullName();
			g_Langメッセージ(_T("Equipはずしメッセージ"),valiable);
		
	//sg_pDungeonSystem->メッセージ
	//		(FullName()+_T(" をはずした。\n"));
	return true;
}


int cEquipment::衝突(pcCharacter pchara)
{
	int result = 0;

	衝突時_メッセージ(pchara);


	result = 衝突時効果(pchara);


	if(result)
	{//使用に成功した
		cValiableField val;
		val.doubles.dim(変数_汎用ブール) = 1;
		sg_pDungeonSystem->CutInM().CutIn(me(), 装備品衝突消滅直前時_タイミング, val);
		if(val.doubles.dim(変数_汎用ブール)) sg_pDungeonSystem->落ち物破壊要請(me());
	}
	
	return result;
}

int cEquipment::衝突時効果(pcCharacter pchara)
{
	multiset<攻撃属性::攻撃属性> attr;
	attr.insert(攻撃属性::落ち物);

	cValiableField val;
	val.doubles.dim(変数_攻撃力ボーナス_倍率) = 1.0;
	val.doubles.dim(変数_耐性ボーナス_倍率％) = 0.0;
	val.charas.dim(変数_防御者) = pchara;
	sg_pDungeonSystem->CutInM().CutIn(me(), 装備品衝突直前時_タイミング, val);

	double power = 0;
	if(投擲者())
	{
		power = 投擲者()->GetBulletAttackPower();
	}

	sg_pDungeonSystem->攻撃接近(攻撃作成(
		投擲者(),//攻撃者
		sg_pDungeonSystem->キャラ足元地形(pchara),//地形
		( 武器力最終値()+防具力最終値()+power)* max(0,val.doubles.dim(変数_攻撃力ボーナス_倍率))* max(0,(1.0 - val.doubles.dim(変数_耐性ボーナス_倍率％)/100.0)),//攻撃力
		true,//定数ダメージ
		attr,
		true
		));

	return true;
}

int cEquipment::衝突時_メッセージ(pcCharacter pchara)
{
	return true;
}

StyleString cEquipment::修正値係数評価(double value)
{
	if(value == 0)
	{
		return _T("");
	}

	StyleString Base;

	double absvalue = abs(value);

	double d = absvalue / 0.2;

	if(value < 0)
	{//負
		Base += _T("-");
	}
	else
	{//正

	}

	if(d > 5)
	{
		Base = g_Lang(_T("★ × "))+setStyle((int)d);
	}
	else
	{
		for(;d>1;d-=0.999)
		{
			Base += g_Lang(_T("★"));
		}
		if(d>0.5)
		{
			Base += g_Lang(_T("☆"));
		}
	}



	return Base;
}

bool cEquipment::通常攻撃_壁斜めすり抜け()
{


	int i;

	cValiableField val;

	val.doubles.dim(変数_汎用ブール) = 0;

	sg_pDungeonSystem->CutInM().CutIn(me(),通常攻撃_壁斜めすり抜け判定_タイミング,val);
		
	for(i=0;i<includedItem.size();i++)
	{
		

		sg_pDungeonSystem->CutInM().CutIn(includedItem[i],通常攻撃_壁斜めすり抜け判定_タイミング,val);
		/*
		pcEquipment pcEqu = boost::dynamic_pointer_cast<cEquipment>(includedItem[i]);
		
		if(pcEqu != NULL)
		{
			flag |= pcEqu->通常攻撃_壁斜めすり抜け();
		}
		*/

	}
	

	return (bool)(val.doubles[変数_汎用ブール]);
}
bool cEquipment::通常攻撃_壁すり抜け()
{

	int i;

	cValiableField val;

	val.doubles.dim(変数_汎用ブール) = 0;

	
	for(i=0;i<includedItem.size();i++)
	{
		

		sg_pDungeonSystem->CutInM().CutIn(includedItem[i],通常攻撃_壁すり抜け判定_タイミング,val);
		/*
		pcEquipment pcEqu = boost::dynamic_pointer_cast<cEquipment>(includedItem[i]);
		
		if(pcEqu != NULL)
		{
			flag |= pcEqu->通常攻撃_壁すり抜け();
		}
		*/

	}

	return (bool)(val.doubles[変数_汎用ブール]);
}
//使用しない
/*
void cEquipment::AbilityExplanationBase(std::vector<StyleString> &explanations)
{
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.DropImportLanguage(ID(),
		(tstring)_T("AbilityExplanationBase"));
	if(psr == NULL) return;

	explanations.push_back(_T(""));
	StyleString& Base = explanations.back();

	
	map<tstring,StyleString> valiable;
	StyleString sstr;

	int i, size = 効果量基礎_定義サイズ();
	for(i=0;i<size;i++)
	{
		効果量推定テキスト(i,sstr);
		valiable[(_T("Value")+setStyle(i)).c_str()] = sstr;
	}

	sstr.clear();
	Drop_StringAddStyle(psr->script(),Base,valiable);

	int i;
	int size = includedItem.size();
	for(i=0;i<size;i++)
	{
		pcEquipment pcequ = boost::dynamic_pointer_cast<cEquipment>(includedItem[i]);
		if(pcequ != NULL)
		{
			AbilityExplanationBase(explanations);
		}
	}
	return;
}

StyleString cEquipment::AbilityName()
{
	StyleString Base;
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.DropImportLanguage(ID(),
		(tstring)_T("AbilityName"));
	if(psr == NULL) return _T("");

	Drop_StringAddStyle(psr->script(),Base);

	return Base;
}
*/

//スモールビュー
int cEquipment::Getsmollview(pcGameWindow& smollview)
{



	StyleString sstring;
	map<tstring, StyleString> valiable;

/*
	if(修正値識別済み())
	{
		Base += _T("武具攻撃力  ")+setStyle((int)武器力最終値(),COLOR_ATTACKPOINT) + 
			_T("  |  ")+ _T("武具防御力  ") +setStyle((int)防具力最終値(),COLOR_DEFFENCEPOINT);
	}
	else
	{
		Base += _T("武具攻撃力  ")+setStyle((int)武器力基礎値(),COLOR_UNAPPREPOINT) + 
			_T("  |  ")+ _T("武具防御力  ") +setStyle((int)防具力基礎値(),COLOR_UNAPPREPOINT);
	}
	
	if(両手装備()) Base += _T("  (両手装備)");
	
	Base += _T("\n");
*/
	if(修正値識別済み())
	{
		valiable[_T("ArmPower")] = setStyle((int)武器力最終値(),COLOR_ATTACKPOINT);
		valiable[_T("ShieldPower")] = setStyle((int)防具力最終値(),COLOR_DEFFENCEPOINT);
	}
	else
	{
		valiable[_T("ArmPower")] = setStyle((int)武器力基礎値(),COLOR_UNAPPREPOINT);
		valiable[_T("ShieldPower")] = setStyle((int)防具力基礎値(),COLOR_UNAPPREPOINT);
	}
	if(両手装備())
	{
		valiable[_T("BothHands")] = g_Lang(_T("(両手装備)"));
	}
	else
	{
		valiable[_T("BothHands")] = _T("");
	}
	valiable[_T("Proficiency")] = 熟練度キャプション();
	if(過修正である())
	{
		valiable[_T("AddLimitWarning")] = setStyle(g_Lang(_T("<過修正>")).conclete_tstr(),0xFFFF0000);
	}
	else
	{
		valiable[_T("AddLimitWarning")] = _T("");
	}
	pcGameWindow pcgw;
	smollview = (pcgw = pcGameWindow(new cGameWindow));


	g_Lang(_T("Equipsmollview"), pcgw->Text(), valiable);



	pcgw->Init(sg_pDungeonSystem->pDevice_D3D, 6, 5);
	pcgw->setRight(SCREEN_X);
	pcgw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(3));


	return true;
}

#define DEF_EquipmentAttributeExpresstions(str) \
if(属性.count(落ち物属性::str)){\
explanations.back() += g_Lang(_T(#str) _T("Expresstions"));}


void cEquipment::longExplanationExtraBase(std::vector<StyleString> &explanations)
{

	explanations.push_back(_T(""));
	StyleString& Base = explanations.back();
	

	map<tstring, StyleString> valiable;


	if(修正値識別済み())
	{
		valiable[_T("ArmPower")] = setStyle((int)武器力最終値(),COLOR_ATTACKPOINT);
		valiable[_T("ShieldPower")] = setStyle((int)防具力最終値(),COLOR_DEFFENCEPOINT);
	}
	else
	{
		valiable[_T("ArmPower")] = setStyle((int)武器力基礎値(),COLOR_UNAPPREPOINT);
		valiable[_T("ShieldPower")] = setStyle((int)防具力基礎値(),COLOR_UNAPPREPOINT);
	}

	valiable[_T("ArmPowerBase")] = setStyle((int)武器力基礎値());
	valiable[_T("ShieldPowerBase")] = setStyle((int)防具力基礎値());
	
	valiable[_T("ArmPowerAdd")] = 修正値係数評価(攻撃修正値係数());
	valiable[_T("ShieldPowerAdd")] = 修正値係数評価(防御修正値係数());

	valiable[_T("AddLimit")] = setStyle((int)修正値限界値());


	valiable[_T("Proficiency")] = 熟練度キャプション();

	
	valiable[_T("ShieldPowerAdd")] = 修正値係数評価(防御修正値係数());

	if(過修正である())
	{
		valiable[_T("AddLimitWarning")] = setStyle(g_Lang(_T("<過修正>")).conclete_tstr(),0xFFFF0000);
	}
	else
	{
		valiable[_T("AddLimitWarning")] = _T("");
	}

	
	valiable[_T("Slot")] = (int)スロットキャパシティ();


	g_Lang(_T("Equip能力値修正値情報"), Base, valiable);


/*
	if(修正値識別済み())
	{
		Base += _T("武具攻撃力  ")+setStyle((int)武器力最終値(),COLOR_ATTACKPOINT) + 
			_T("  |  ")+ _T("武具防御力  ") +setStyle((int)防具力最終値(),COLOR_DEFFENCEPOINT);
	}
	else
	{
		Base += _T("武具攻撃力  ")+setStyle((int)武器力基礎値(),COLOR_UNAPPREPOINT) + 
			_T("  |  ")+ _T("武具防御力  ") +setStyle((int)防具力基礎値(),COLOR_UNAPPREPOINT);
	}
	Base += _T("\n");
	
	Base += _T("攻撃基礎値  ")+setStyle((int)武器力基礎値()) + 
		_T("  :  成長  ") +修正値係数評価(攻撃修正値係数()) + _T("\n");
	
	
	Base += _T("防御基礎値  ")+setStyle((int)防具力基礎値()) + 
		_T("  :  成長  ") +修正値係数評価(防御修正値係数()) + _T("\n");
	
	
	Base += _T("修正値限界値  ")+setStyle((int)修正値限界値());
	if(過修正である())
	{
		Base +=setStyle(_T(" <過修正>"),0xFFFF0000);
	}
	Base +=_T("\n");
*/

	explanations.push_back(StyleString(_T("")));

	explanations.back() += g_Lang(_T("EquipmentAttributeExpresstions"));

	DEF_EquipmentAttributeExpresstions(剣)

	cItem::longExplanationExtraBase(explanations);

	//御霊
	int i,size = includedItem.size();
	for(i=0;i<size;i++)
	{
		explanations.push_back(StyleString(_T("")));
		explanations.back() += includedItem[i]->能力説明Caption();
		explanations.back() += _T("\n\n");
		explanations.back() += includedItem[i]->shortExplanation();
	}

}

/*
StyleString cEquipment::longExplanation()
{
	StyleString Base = longExplanationBase();


	if(修正値識別済み())
	{
		Base += _T("武具攻撃力  ")+setStyle((int)武器力最終値(),COLOR_ATTACKPOINT) + 
			_T("  |  ")+ _T("武具防御力  ") +setStyle((int)防具力最終値(),COLOR_DEFFENCEPOINT);
	}
	else
	{
		Base += _T("武具攻撃力  ")+setStyle((int)武器力基礎値(),COLOR_UNAPPREPOINT) + 
			_T("  |  ")+ _T("武具防御力  ") +setStyle((int)防具力基礎値(),COLOR_UNAPPREPOINT);
	}
	Base += _T("\n");
	
	Base += _T("攻撃基礎値  ")+setStyle((int)武器力基礎値()) + 
		_T("  :  成長  ") +修正値係数評価(攻撃修正値係数()) + _T("\n");
	
	
	Base += _T("防御基礎値  ")+setStyle((int)防具力基礎値()) + 
		_T("  :  成長  ") +修正値係数評価(防御修正値係数()) + _T("\n");
	
	
	Base += _T("修正値限界値  ")+setStyle((int)修正値限界値());
	if(過修正である())
	{
	Base +=setStyle(_T(" <過修正>"),0xFFFF0000);
	}
	Base +=_T("\n");


	return Base;
}
*/

StyleString cEquipment::shortExplanation()
{

	if(御霊状態())
	{
		return 能力説明();
	}

	return shortExplanationBase();
}

//強制的に装備
int cEquipment::equip(pcCharacter pchara, int part)
{
	if(両手装備())
	{
		if(pchara->attackequipment == NULLDROP && pchara->defenseequipment == NULLDROP)
		{
			pchara->attackequipment = me();
			pchara->defenseequipment = me();
			weak_pEquiper_Attack = wpcCharacter(pchara);
			weak_pEquiper_Defence = wpcCharacter(pchara);			
		}
		else
		{
			return false;
		}
	}
	else
	{
		if(part == EQUIP_PART_ATTACK)
		{
			if(pchara->attackequipment == NULLDROP)
			{
				pchara->attackequipment = me();
				weak_pEquiper_Attack = wpcCharacter(pchara);
			}
			else
			{//別の装備されてる。
				return false;
			}
		}
		else if(part == EQUIP_PART_DEFENSE)
		{
			if(pchara->defenseequipment == NULLDROP)
			{
				pchara->defenseequipment = me();
				weak_pEquiper_Defence = wpcCharacter(pchara);
			}
			else
			{//別の装備されてる。
				return false;
			}
		}
	}

	if(!完全識別済み())
	{
		sg_pDungeonSystem->識別(me(),false);
	}

	return true;
}
//強制的に装備解除
int cEquipment::unequip()
{

	if(装備されている_攻撃用())
	{

		装備者_攻撃用()->attackequipment = NULLDROP;
		weak_pEquiper_Attack = wpcCharacter(NULLCHARA);

	}
	
	if(装備されている_防御用())
	{

		装備者_防御用()->defenseequipment = NULLDROP;
		weak_pEquiper_Defence = wpcCharacter(NULLCHARA);

	}


	return true;
}

//--------------------------------------------------------
//パラメータ群
/*
double cEquipment::効果量状態倍率()
{
	if(御霊状態())
	{
		return Parent()->効果量状態倍率();
	}
	else
	{
		if(state() == cDroping::STATE_GOOD)
			return sg_pDungeonSystem->DataBase.DropImportData_Value(ID(), (tstring)_T("効果量状態倍率_祝福"), デフォルト効果量状態倍率_祝福());
		else
			return 1.0;
	}
}
double cEquipment::効果量修正値倍率()
{
	if(御霊状態())
	{
		return Parent()->効果量修正値倍率();
	}
	else
		return cDroping::効果量修正値倍率();
}
*/
double cEquipment::デフォルト効果量状態倍率_祝福()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value((tstring)EQUIPMENT_BASICVALUESTR, (tstring)_T("デフォルト効果量状態倍率_祝福"), 2.0);
}

int cEquipment::装備難度()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(ID(), (tstring)_T("装備難度"), 1);
}
double cEquipment::熟練度定数()
{
	return (熟練度()*熟練度攻撃防御補正率())/100.0;
}
double cEquipment::熟練度攻撃防御補正率()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(ID(), (tstring)_T("熟練度攻撃防御補正率"), デフォルト熟練度攻撃防御補正率());
}
double cEquipment::デフォルト熟練度攻撃防御補正率()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value((tstring)EQUIPMENT_BASICVALUESTR, (tstring)_T("デフォルト熟練度攻撃防御補正率"), 3);
}
bool cEquipment::改造可能()
{
#ifdef _UNRELEASE
	return true;
#endif
	return (熟練度() >= 300);
}
void cEquipment::改造する()
{
	overspec()++;
	proficiency_count() = 100*100;//熟練度100になる調整
}
int cEquipment::改造費()
{
	//改造費用＝定数＊基礎値段＊(2^改造回数)/(定数^（(熟練度-300)/100）)
	double gaku = 10.0*値段基礎値()*pow(1.0+(1.0+装備難度())/3.0,overspec())/(pow(1.5,(熟練度()-300.0)/100.0));
	if(gaku > 999999999) gaku = 999999999;
	return gaku;
}
int cEquipment::修正値限界値()
{
	int val = sg_pDungeonSystem->DataBase.DropImportData_Value(ID(), (tstring)_T("修正値限界値"), 10);
	val += overspec()*(val*0.5 + 1);
	return val;
}
double cEquipment::初期初期残りスロット()
{
	double val = sg_pDungeonSystem->DataBase.DropImportData_Value(ID(), (tstring)_T("初期残りスロット"), 5);
	val += overspec()*(val*0.1 + 1);
	return min(val,20);
}

double cEquipment::修正値重み計算式(double base)
{
	double logArgValue = base/10 + 1;
	double logValue = 0.0;
	if(logArgValue > 0)
	{
		logValue = log(base/10 + 1) / log(1.15);
	}
	else
	{
		logValue = 0.0;
	}
	return logValue/10 +0.5;
}

double cEquipment::武器力修正値重み()
{
	double power = 修正値重み計算式(武器力基礎値());

	power *= 攻撃修正値係数();

	return power;
}
double cEquipment::武器力最終値()
{
	double result = 武器力基礎値() + quality()*武器力修正値重み();

	result = result + 熟練度定数();


	cValiableField val;
	val.doubles.dim(変数_武器力ボーナス_倍率) = 1.0;
	val.doubles.dim(変数_武器力ボーナス_定数) = 0.0;
	sg_pDungeonSystem->CutInM().CutIn(装備者_攻撃用(), 装備品武器力計算時_タイミング, val);

	result *= val.doubles.val(変数_武器力ボーナス_倍率);
	result += val.doubles.val(変数_武器力ボーナス_定数);

	if(state()==cDroping::STATE_GOOD)
	{//信仰
		result *= 効果量状態倍率();
	}

	if(result < 0) result = 0;

	return result;
}

double cEquipment::防具力修正値重み()
{
	double power = 修正値重み計算式(防具力基礎値());

	power *= 防御修正値係数();

	return power;
}
double cEquipment::防具力最終値()
{
	double result = 防具力基礎値() + quality()*防具力修正値重み();
	
	result = result + 熟練度定数();

		
	cValiableField val;
	val.doubles.dim(変数_防具力ボーナス_倍率) = 1.0;
	val.doubles.dim(変数_防具力ボーナス_定数) = 0.0;
	sg_pDungeonSystem->CutInM().CutIn(装備者_防御用(), 装備品防具力計算時_タイミング, val);

	result *= val.doubles.val(変数_防具力ボーナス_倍率);
	result += val.doubles.val(変数_防具力ボーナス_定数);

	if(state()==cDroping::STATE_GOOD)
	{//信仰
		result *= 効果量状態倍率();
	}

	if(result < 0) result = 0;

	return result;
}

double cEquipment::武器力基礎値()
{
	double power = sg_pDungeonSystem->DataBase.DropImportData_Value(ID(), (tstring)_T("攻撃基礎値"), 1);

	cValiableField val;
	val.doubles.dim(変数_武器力基礎値ボーナス_倍率) = 1.0;
	val.doubles.dim(変数_武器力基礎値ボーナス_定数) = 0.0;
	sg_pDungeonSystem->CutInM().CutIn(me(), 装備品武器力基礎値計算時_タイミング, val);

	power *= val.doubles.val(変数_武器力基礎値ボーナス_倍率);
	power += val.doubles.val(変数_武器力基礎値ボーナス_定数);

	return power;
}
double cEquipment::攻撃修正値係数()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(ID(), (tstring)_T("攻撃修正値係数"), 0.5);
}
double cEquipment::命中率補正()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(ID(), (tstring)_T("命中率補正"), 0.0);
}

double cEquipment::防具力基礎値()
{
	double power = sg_pDungeonSystem->DataBase.DropImportData_Value(ID(), (tstring)_T("防御基礎値"), 1);

	cValiableField val;
	val.doubles.dim(変数_防具力基礎値ボーナス_倍率) = 1.0;
	val.doubles.dim(変数_防具力基礎値ボーナス_定数) = 0.0;
	sg_pDungeonSystem->CutInM().CutIn(me(), 装備品防具力基礎値計算時_タイミング, val);

	power *= val.doubles.val(変数_防具力基礎値ボーナス_倍率);
	power += val.doubles.val(変数_防具力基礎値ボーナス_定数);

	return power;
}
double cEquipment::防御修正値係数()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(ID(), (tstring)_T("防御修正値係数"), 0.5);
}
double cEquipment::回避率補正()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(ID(), (tstring)_T("回避率補正"), 0.0);
}

int cEquipment::過修正である()
{
	int qua = abs(quality());
	int dis = qua - 修正値限界値();
	if(dis > 0)
	{
		return dis;
	}
	else
	{
		return 0;
	}
}

void cEquipment::CutIn(タイミング timing, cValiableField& valiable)
{
	cItem::CutIn(timing, valiable);

	if(timing == 攻撃直後時_タイミング && valiable.doubles.exist(変数_直接攻撃フラグ))
	{
		if(装備されている_攻撃用())
		{
			if(state() == cDroping::STATE_GOOD)
			{//祝福
				stategoodRunoutprocess();
			}	
			else if(過修正である())
			{
				overQualityprocess();
			}

			double learnpow;
			if(valiable.charas.exist(変数_防御者) && valiable.charas.val(変数_防御者) != NULL)
			{
				learnpow = 熟練度成長倍率(valiable.charas.val(変数_攻撃者)->LV, valiable.charas.val(変数_防御者)->LV);
			}
			else
			{
				learnpow = 熟練度成長倍率(valiable.charas.val(変数_攻撃者)->LV, 0);
			}
			//習熟
			熟練度カウント加算(sg_pDungeonSystem->DataBase.DropImportData_Value((tstring)EQUIPMENT_BASICVALUESTR, (tstring)_T("効果時熟練度カウント上昇数"), 10000)
				*learnpow);
		}

	}
	else if(timing == 被攻撃直後時_タイミング)
	{
		if(装備されている_防御用())
		{
			if(state() == cDroping::STATE_GOOD)
			{//祝福
				stategoodRunoutprocess();
			}	
			else if(過修正である())
			{
				overQualityprocess();
			}

			double learnpow;
			if(valiable.charas.exist(変数_攻撃者) && valiable.charas.val(変数_攻撃者) != NULL)
			{
				learnpow = 熟練度成長倍率(valiable.charas.val(変数_防御者)->LV, valiable.charas.val(変数_攻撃者)->LV);
			}
			else
			{
				learnpow = 熟練度成長倍率(valiable.charas.val(変数_防御者)->LV, 0);
			}
			//習熟
			熟練度カウント加算(sg_pDungeonSystem->DataBase.DropImportData_Value((tstring)EQUIPMENT_BASICVALUESTR, (tstring)_T("効果時熟練度カウント上昇数"), 10000)
				*learnpow);
		}
	}

	能力(timing,valiable);
	int i;
	int size = includedItem.size();
	vector<pcDroping> vpdrop = includedItem;
	for(i=0;i<size;i++)
	{
		//pcEquipment pcequ = boost::dynamic_pointer_cast<cEquipment>(vpdrop[i]);
		if(vpdrop[i] != NULL)
		{
			vpdrop[i]->能力(timing,valiable);
		}
	}
}


//修正値超過時処理
bool cEquipment::overQualityprocess()
{
	int over = 過修正である();

	double pos = pow(1-過修正累進故障率(),over);
	
	if(pos < random())
	{
		//if(sg_pDungeonSystem->落ち物破壊要請(me()))
		{
			sg_pDungeonSystem->AnimationManager().
			Anime_PlaySE(_T("crashbreak.wav"),現在地形()->place);
		
			map<tstring, StyleString> valiable;
			valiable[_T("Item")] = FullName();
			g_Langメッセージ(_T("Equip破壊メッセージ"),valiable);

			//crash() = 1;
			//unequip();
			//quality() = 修正値限界値()*(quality()/abs(quality()));
			quality() -= (quality()/abs(quality()));

			//sg_pDungeonSystem->メッセージ(
			//	_T("装備していた ") + FullName() + _T(" は粉々に砕け散ってしまった！\n"));
			return true;
		}
	}

	return false;
}
double cEquipment::過修正累進故障率()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value((tstring)EQUIPMENT_BASICVALUESTR, (tstring)_T("過修正累進故障率"), BREAK_POSSIBILITY_BASE);
}

//祝福切れる
bool cEquipment::stategoodRunoutprocess()
{

	if(効果時神力切れ確率() > random())
	{
		map<tstring, StyleString> valiable;
		valiable[_T("Item")] = FullName();
		g_Langメッセージ(_T("Equip神力切れメッセージ"),valiable);
		
		//sg_pDungeonSystem->メッセージ(
		//	_T("装備していた ") + FullName() + _T(" の神力がなくなった！\n"));
		state() = cDroping::STATE_NORMAL;	

		return true;
	}

	return false;
}

void cEquipment::被箱入れ準備()
{
	if(装備されている_攻撃用())
	{
		Equipment装備はずし要請(装備者_攻撃用(),me());

	}
	if(装備されている_防御用())
	{
		Equipment装備はずし要請(装備者_防御用(),me());

	}
}
