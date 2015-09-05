#include "stdafx.h"

#include "cDungeonSystem.h"
#include "cEquipment.h"
#include "cDrink.h"
#include "cBox.h"
#include "cBook.h"
#include "cSpell.h"
#include "cBullet.h"
#include "cSaveStore.h"
#include "cFloor.h"

int cDungeonSystem::動的識別(pcDroping pdrop, int massage)
{

	pcCharacter pthrowChara = pdrop->投擲者();
	if(pthrowChara){
		if(pthrowChara->Forse != CHARACTER_FORSE_FRIEND) {
			return false;
		}
	}
	if(!pSaveQuest->privateFlags[cSaveQuest::privateFlagKey_AppreciationSupportKey()])
	{
		return false;
	}


	pcItem pitem = boost::dynamic_pointer_cast<cItem>(pdrop);

	if(pitem == NULL) return false;


	StyleString prename = pitem->FullName();

	if(pitem->大局識別済み())
	{


	}
	else
	{
		
		名称のみ識別(pdrop, 0);
		
		if(massage)
		{
			map<tstring, StyleString> valiable;

			valiable[_T("Unknown")] = prename;
			valiable[_T("Item")] = pitem->FullName();
		
			g_Langメッセージ(_T("動的識別アイテムメッセージ"),valiable);
			
		}
	}

	return true;
	
}


int cDungeonSystem::識別(pcDroping pdrop, int massage)
{
	pcItem pitem = boost::dynamic_pointer_cast<cItem>(pdrop);

	if(pitem == NULL) return false;


	StyleString prename = pitem->FullName();

	if(pitem->完全識別済み())
	{
		if(massage == true)
		{
			
			map<tstring, StyleString> valiable;

			valiable[_T("Item")] = pitem->FullName();
		
			g_Langメッセージ(_T("識別アイテム識別済みメッセージ"),valiable);
			
			//メッセージ(_T("これは")+ pitem->FullName()+_T(" にまちがいない。 ってけーねが言ってた。\n"));
		}
	
	}
	else
	{
		pitem->allAppreciate();
		
		if(massage == true
			|| (massage == MASSAGE_IFCHANGE && prename != pitem->FullName()))
		{
			map<tstring, StyleString> valiable;

			valiable[_T("Unknown")] = prename;
			valiable[_T("Item")] = pitem->FullName();
		
			g_Langメッセージ(_T("識別アイテムメッセージ"),valiable);
			
			//メッセージ(prename +_T(" は") +pitem->FullName()+_T(" だった!\n"));
		}
	}

	return true;
}
int cDungeonSystem::状態のみ識別(pcDroping pdrop, int massage)
{
	pcItem pitem = boost::dynamic_pointer_cast<cItem>(pdrop);

	if(pitem == NULL) return false;


	StyleString prename = pitem->FullName();

	if(pitem->状態値識別済み())
	{
		if(massage)
		{
			map<tstring, StyleString> valiable;

			valiable[_T("Item")] = pitem->FullName();
		
			g_Langメッセージ(_T("識別アイテム識別済みメッセージ"),valiable);
			
			//メッセージ(_T("これは")+ pitem->FullName()+_T(" にまちがいない。 ってけーねが言ってた。\n"));
		}
	
	}
	else
	{
		pitem->Appreciate_State();
		
		if(massage)
		{
			map<tstring, StyleString> valiable;

			valiable[_T("Unknown")] = prename;
			valiable[_T("Item")] = pitem->FullName();
		
			g_Langメッセージ(_T("識別アイテムメッセージ"),valiable);
			
			//メッセージ(prename +_T(" は") +pitem->FullName()+_T(" だった!\n"));
		}
	}

	return true;
}
int cDungeonSystem::修正のみ識別(pcDroping pdrop, int massage)
{
	pcItem pitem = boost::dynamic_pointer_cast<cItem>(pdrop);

	if(pitem == NULL) return false;


	StyleString prename = pitem->FullName();

	if(pitem->修正値識別済み())
	{
		if(massage)
		{
			map<tstring, StyleString> valiable;

			valiable[_T("Item")] = pitem->FullName();
		
			g_Langメッセージ(_T("識別アイテム識別済みメッセージ"),valiable);
			
			//メッセージ(_T("これは")+ pitem->FullName()+_T(" にまちがいない。 ってけーねが言ってた。\n"));
		}
	
	}
	else
	{
		pitem->Appreciate_Quality();
		
		if(massage)
		{
			map<tstring, StyleString> valiable;

			valiable[_T("Unknown")] = prename;
			valiable[_T("Item")] = pitem->FullName();
		
			g_Langメッセージ(_T("識別アイテムメッセージ"),valiable);
			
			//メッセージ(prename +_T(" は") +pitem->FullName()+_T(" だった!\n"));
		}
	}

	return true;
}
int cDungeonSystem::名称のみ識別(pcDroping pdrop, int massage)
{
	pcItem pitem = boost::dynamic_pointer_cast<cItem>(pdrop);

	if(pitem == NULL) return false;


	StyleString prename = pitem->FullName();

	if(pitem->名称識別済み())
	{
		if(massage)
		{
			map<tstring, StyleString> valiable;

			valiable[_T("Item")] = pitem->FullName();
		
			g_Langメッセージ(_T("識別アイテム識別済みメッセージ"),valiable);
			
			//メッセージ(_T("これは")+ pitem->FullName()+_T(" にまちがいない。 ってけーねが言ってた。\n"));
		}
	
	}
	else
	{
		pitem->Appreciate_Name();
		
		if(massage)
		{
			map<tstring, StyleString> valiable;

			valiable[_T("Unknown")] = prename;
			valiable[_T("Item")] = pitem->FullName();
		
			g_Langメッセージ(_T("識別アイテムメッセージ"),valiable);
			
			//メッセージ(prename +_T(" は") +pitem->FullName()+_T(" だった!\n"));
		}
	}

	return true;
}

int cDungeonSystem::カテゴリ未識別化(pcDroping pdrop, int massage)
{
	if(boost::dynamic_pointer_cast<cDrink>(pdrop))
	{
		if(!pDungeon()->allAppreciationOfDrink())
		{
			DataBase.大局識別Drinkキャンセル();
			return true;
		}
	}
	if(boost::dynamic_pointer_cast<cBox>(pdrop))
	{
		if(!pDungeon()->allAppreciationOfBox())
		{
			DataBase.大局識別Boxキャンセル();
			return true;
		}
	}
	if(boost::dynamic_pointer_cast<cBook>(pdrop))
	{
		if(!pDungeon()->allAppreciationOfBook())
		{
			DataBase.大局識別Bookキャンセル();
			return true;
		}
	}
	if(boost::dynamic_pointer_cast<cSpell>(pdrop))
	{
		if(!pDungeon()->allAppreciationOfSpell())
		{
			DataBase.大局識別Spellキャンセル();
			return true;
		}
	}
	if(boost::dynamic_pointer_cast<cBullet>(pdrop))
	{
		if(!pDungeon()->allAppreciationOfBullet())
		{
			DataBase.大局識別Bulletキャンセル();
			return true;
		}
	}
	return false;
}


int cDungeonSystem::アイテム祝福(pcDroping pdrop, int massage)
{
	if(pdrop == NULL) return false;

	pdrop->被祝福要請();
	状態のみ識別(pdrop,false);

	if(massage)
	{
		if(pdrop->state() == cDroping::STATE_GOOD)
		{
			map<tstring, StyleString> valiable;
			valiable[_T("Drop")] = pdrop->FullName();
		
			g_Langメッセージ(_T("アイテム信仰メッセージ"),valiable);
			
			//メッセージ(pdrop->FullName()+_T(" は神力がついた！\n"));
		}
		else if(pdrop->state() == cDroping::STATE_NORMAL)
		{
			map<tstring, StyleString> valiable;
			valiable[_T("Drop")] = pdrop->FullName();
		
			g_Langメッセージ(_T("アイテム呪い解除メッセージ"),valiable);
			
			//メッセージ(pdrop->FullName()+_T(" の呪いが解けた！\n"));
		}
	}
	return true;
}
int cDungeonSystem::アイテム呪い(pcDroping pdrop, int massage)
{
	if(pdrop == NULL) return false;


	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
	valf.drops.dim(変数_対象落ち物) = pdrop;
	CutInM().CutIn(pdrop,アイテム呪い直前_タイミング,valf);
	CutInM().CutIn(pdrop->Holder(),アイテム呪い直前_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール))
	{
		pdrop->被呪い要請();
		状態のみ識別(pdrop,false);

		if(massage)
		{
			if(pdrop->state() == cDroping::STATE_CURSE)
			{
				map<tstring, StyleString> valiable;
				valiable[_T("Drop")] = pdrop->FullName();
			
				g_Langメッセージ(_T("アイテム呪いメッセージ"),valiable);
				
				//メッセージ(pdrop->FullName()+_T(" は呪われた！\n"));
			}
			else if(pdrop->state() == cDroping::STATE_NORMAL)
			{
				map<tstring, StyleString> valiable;
				valiable[_T("Drop")] = pdrop->FullName();
			
				g_Langメッセージ(_T("アイテム信仰解除メッセージ"),valiable);
				
				//メッセージ(pdrop->FullName()+_T(" の神力がなくなった！\n"));
			}
		}
		return true;
	}
	return false;
}
	//設定メッセージは未実装
int cDungeonSystem::アイテム状態強制設定祝福(pcDroping pdrop, int massage)
{
	pdrop->state() = cDroping::STATE_GOOD;
	return true;
}
int cDungeonSystem::アイテム状態強制設定通常(pcDroping pdrop, int massage)
{
	pdrop->state() = cDroping::STATE_NORMAL;
	return true;
}
int cDungeonSystem::アイテム状態強制設定呪い(pcDroping pdrop, int massage)
{
	pdrop->state() = cDroping::STATE_CURSE;
	return true;
}
	//設定
int cDungeonSystem::アイテム品質強制設定(pcDroping pdrop, int quality, int massage)
{
	pdrop->quality() = quality;
	return true;
}


int cDungeonSystem::アイテム店フラグ(pcDroping pdrop, int flag)
{
	if(pdrop == NULL) return false;

	pdrop->shopitemflag() = flag;
	
	if(!pdrop->内包アイテムは実体がない())
	{
		int i;
		for(i=0;i<pdrop->includedItem.size();i++)
		{
			if(pdrop->includedItem[i]->売却可())
			{
				アイテム店フラグ(pdrop->includedItem[i], flag);
			}
		}
	}
	return flag;
}

int cDungeonSystem::アイテム割引フラグ(pcDroping pdrop, int flag)
{
	if(pdrop == NULL) return false;

	pdrop->shopDiscountflag() = flag;
	
	if(!pdrop->内包アイテムは実体がない())
	{
		int i;
		for(i=0;i<pdrop->includedItem.size();i++)
		{
			アイテム割引フラグ(pdrop->includedItem[i], flag);
		}
	}
	return flag;
}

int cDungeonSystem::アイテム劣化(pcDroping pdrop, int massage)
{
	if(pdrop == NULL) return false;


	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
	valf.drops.dim(変数_対象落ち物) = pdrop;
	CutInM().CutIn(pdrop,アイテム劣化直前_タイミング,valf);
	CutInM().CutIn(pdrop->Holder(),アイテム劣化直前_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール))
	{

		if(massage)
		{
			map<tstring, StyleString> valiable;
			valiable[_T("Drop")] = pdrop->FullName();
		
			g_Langメッセージ(_T("アイテム劣化メッセージ"),valiable);
		}

		pdrop->quality()--;

		pdrop->修正値丸め();

		return true;
	}
	return false;
}
int cDungeonSystem::アイテム強化(pcDroping pdrop, int massage)
{
	if(pdrop == NULL) return false;

	pdrop->quality()++;

	pdrop->修正値丸め();

	if(massage)
	{
		map<tstring, StyleString> valiable;
		valiable[_T("Drop")] = pdrop->FullName();
	
		g_Langメッセージ(_T("アイテム強化メッセージ"),valiable);
	}
	return true;
}
int cDungeonSystem::装備強制はずし(pcDroping pdrop, int massage)
{
	if(pdrop == NULL) return false;
	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
	valf.drops.dim(変数_対象落ち物) = pdrop;
	CutInM().CutIn(pdrop,装備強制はずし直前_タイミング,valf);
	CutInM().CutIn(pdrop->Holder(),装備強制はずし直前_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール))
	{

		pcEquipment pEqu = boost::dynamic_pointer_cast<cEquipment>(pdrop);
		pcSpell pSpe = boost::dynamic_pointer_cast<cSpell>(pdrop);

		if(pEqu != NULL)
		{
			if(pEqu->装備されている_攻撃用() || pEqu->装備されている_防御用())
			{
				pEqu->unequip();
			}
			else return false;
		}
		else if(pSpe != NULL)
		{
			if(pSpe->装備されている())
			{
				pSpe->unequip();
			}
			else return false;
		
		}
		else
		{
			return false;
		}

		if(massage)
		{
			map<tstring, StyleString> valiable;
			valiable[_T("Drop")] = pdrop->FullName();
		
			g_Langメッセージ(_T("装備強制はずしメッセージ"),valiable);
		}
		return true;
	}
	return false;
}
int cDungeonSystem::強制装備(pcDroping pdrop, int part, int massage)
{
	pcEquipment pEqu = boost::dynamic_pointer_cast<cEquipment>(pdrop);
	pcSpell pSpe = boost::dynamic_pointer_cast<cSpell>(pdrop);

	if(pEqu != NULL)
	{
		if(pEqu->Holder() == NULL) return false;

		if(pEqu->装備されている_攻撃用() || pEqu->装備されている_防御用())
		{
			pEqu->unequip();
		}
		
		pEqu->equip(pEqu->Holder(),part);

	}
	else if(pSpe != NULL)
	{
		if(pSpe->Holder() == NULL) return false;

		if(pSpe->装備されている())
		{
			pSpe->unequip();
		}
		
		pSpe->equip(pEqu->Holder(),part);

	}
	else
	{
		return false;
	}

	if(massage)
	{
		map<tstring, StyleString> valiable;
		valiable[_T("Drop")] = pdrop->FullName();
	
		g_Langメッセージ(_T("強制装備メッセージ"),valiable);
	}
	return true;
}
int cDungeonSystem::装備回し要請(pcCharacter pchara, int massage)
{
	if(pchara == NULL) return false;

	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
	CutInM().CutIn(pchara,装備回し直前_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール))
	{
		return 強制装備回し(pchara, massage);
	}
	return false;
}
int cDungeonSystem::強制装備回し(pcCharacter pchara, int massage)
{
	if(pchara == NULL) return false;

	int res = 0;

	pcDroping pdrop1 = pchara->attackequipment;
	pcDroping pdrop2 = pchara->defenseequipment;

	res |= sg_pDungeonSystem->装備強制はずし(pchara->attackequipment,0);
	res |= sg_pDungeonSystem->装備強制はずし(pchara->defenseequipment,0);
	
	if(pdrop1) res |= sg_pDungeonSystem->強制装備(pdrop1,EQUIP_PART_DEFENSE,0);
	if(pdrop2) res |= sg_pDungeonSystem->強制装備(pdrop2,EQUIP_PART_ATTACK,0);

	if(massage)
	{
		map<tstring, StyleString> valiable;
		valiable[_T("Chara")] = pchara->FullName();
	
		g_Langメッセージ(_T("強制装備回しメッセージ"),valiable);
	}
	return res;
}