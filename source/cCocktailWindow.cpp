#include "stdafx.h"

#include "cCocktailWindow.h"
#include "cSelectWindow.h"

#include "scriptprocesser/DropsScript.h"
#include "csTitle.h"
#include "Box1.h"
#include "cDrink.h"

#include "Language.h"
#include "utility/debug.h"
#include "gameMainSystem/filemanage/cScriptRLayer.h"

#define CURSORSIZE 32

#define PAGETEXPATH _T("skin\\page.png")
//#define PAGETEXSIZE 32

#define MULTISELECTTEXPATH _T("interface\\multiselect.png")

#define PAGECOUNTNUM 70

#define FORM_STRWINDOWTOP (136)


#define DEF_COMMANDCLASS(name,deftext) \
class cCommand##name## : public cCommand\
{\
public:\
	cCommand##name##(void){caption = deftext;};\
	cCommand##name##(StyleString s){caption = s;};\
	virtual ~cCommand##name##(void){};\
	virtual int Action(IDirect3DDevice9 *pDev);\

DEF_COMMANDCLASS(_Null,g_Lang(_T("何もない")))
};
int cCommand_Null::Action(IDirect3DDevice9 *pDev)
{
	return true;
}

DEF_COMMANDCLASS(_cocktailRecipe,g_Lang(_T("レシピ")))
	virtual StyleString shortExplanationText();
	int id;
	int quality;

	int* ptgtid;
	int* ptgtquality;
};
int cCommand_cocktailRecipe::Action(IDirect3DDevice9 *pDev)
{
	*ptgtid = id;
	*ptgtquality = quality;

	sg_pDungeonSystem->メニューを一枚閉じる();
	sg_pDungeonSystem->メニューリレンダ();
	return true;
}
StyleString cCommand_cocktailRecipe::shortExplanationText()
{
	pcDroping pdrop = sg_pDungeonSystem->DataBase.GetSampleDroping(id);
	if(pdrop == NULL) return tstring(_T(""));
	return sg_pDungeonSystem->DataBase.GetSampleDroping(id)->shortExplanationBase();
}
typedef boost::shared_ptr<cCommand_cocktailRecipe> pcCommand_cocktailRecipe;


DEF_COMMANDCLASS(_cocktailMenu,g_Lang(_T("menu")))
	virtual StyleString shortExplanationText();
	int id;
	int quality;

	bool unappreciatedCooktailable_;
	vector<int> materials;
};
int cCommand_cocktailMenu::Action(IDirect3DDevice9 *pDev)
{
	if(id == 0)
	{
		pcControlLayer pccl;
		pcSelectWindow pcsw;

		//新規コントロールレイヤー
		sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
		pccl->Init(pDev);

		//新規セレクトウィンドウ
		pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));

		vector<int> idlist,vmaterial;
		set<int> condition;
		condition.insert(落ち物属性::飲み物);
		int i,k,m;
		sg_pDungeonSystem->DataBase.makeDropIDList(idlist,condition);
		pcCommand_cocktailRecipe pccm;
		multimap<double,pcCommand_cocktailRecipe> recipelist;
		for(i=0;i<idlist.size();i++)
		{
			pcDrink pDrk = boost::dynamic_pointer_cast<cDrink>(sg_pDungeonSystem->DataBase.GetSampleDroping(idlist[i]));

			if(pDrk == NULL) continue;
			if(!unappreciatedCooktailable_ && !pDrk->大局識別済み()) {
				continue;
			}
			if(pDrk->カクテルレシピから削除()) {
				continue;
			}

			for(k=0;k<3;k++)
			{
				pDrk->quality() = k;
				pDrk->state() = cDroping::STATE_NORMAL;

				vmaterial.clear();
				vmaterial.resize(cDrink::MATERIALNUM+1,0);

				double yuusenndo = 0;
				int continueflag = 0;

				for(m=0;m<cDrink::MATERIALNUM;m++)
				{
					vmaterial[m] += pDrk->マテリアル(m);
					yuusenndo += -vmaterial[m]*100000 - pow(10.0,m)*vmaterial[m];
				
					if(materials[m] < vmaterial[m])
					{
						continueflag = 1;
						break;
					}
				}
				vmaterial[m] += 1;

				if(continueflag || materials[m] < vmaterial[m])
				{
					continue;
				}



				StyleString sstr;

				getMaterialTextShort(vmaterial,sstr);


				tstring str = _T("");
				if(k)
				{
					str += _T("+");
					str += setStyle(k).conclete_tstr();
				}
				pccm = pcCommand_cocktailRecipe(
					new cCommand_cocktailRecipe(sstr +_T(" ") +  pDrk->FullNameBase() + str) );

				pccm->id = idlist[i];
				pccm->quality = k;
		
				pccm->ptgtid = &id;
				pccm->ptgtquality = &quality;

				recipelist.insert(pair<double,pcCommand_cocktailRecipe>(yuusenndo,pccm));
			}
			
		}

		multimap<double,pcCommand_cocktailRecipe>::iterator itr = recipelist.begin();
		for(;itr!=recipelist.end();itr++)
		{
			pcsw->commandList.push_back(pcCommand(itr->second));
		}

		if(pcsw->commandList.empty())
		{
			pcsw->commandList.push_back(pcCommand( new cCommand_Null(g_Lang(_T("材料が足りません")))));
		}

		pcsw->Init(pDev, 20,10);
		pcsw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(3));
		pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(2));

	}
	return true;
}
StyleString cCommand_cocktailMenu::shortExplanationText()
{
	pcDroping pdrop = sg_pDungeonSystem->DataBase.GetSampleDroping(id);
	if(pdrop == NULL) return tstring(_T(""));
	return sg_pDungeonSystem->DataBase.GetSampleDroping(id)->shortExplanationBase();
}
typedef boost::shared_ptr<cCommand_cocktailMenu> pcCommand_cocktailMenu;


DEF_COMMANDCLASS(_cocktailDecition,g_Lang(_T("作る")))
	virtual StyleString shortExplanationText();
	int id;
};
int cCommand_cocktailDecition::Action(IDirect3DDevice9 *pDev)
{
	//ダミー

	return true;
}
StyleString cCommand_cocktailDecition::shortExplanationText()
{
	return g_Lang(_T("作るshortExplanationText"));
}
typedef boost::shared_ptr<cCommand_cocktailDecition> pcCommand_cocktailDecition;


cCocktailWindow::cCocktailWindow(void)
{

	cursolIndexXMax = 10;
	cursolIndexX = 0;
	cursolIndexYMax = 9;
	cursolIndexY = 0;

	unappreciatedCooktailable_ = true;
}

cCocktailWindow::~cCocktailWindow(void)
{
	commandList.clear();
}

void cCocktailWindow::Init(IDirect3DDevice9 *pDev, int letterXnum, int letterYnum)
{
	InitBasic(pDev,letterXnum,letterYnum);

}
void cCocktailWindow::Init(IDirect3DDevice9 *pDev, pcDroping pbox)
{

	pcBox_ID_24 pcocktailBox = boost::dynamic_pointer_cast<cBox_ID_24>(pbox);

	vector<int> materials;
	pcocktailBox->SumMaterial(materials);
	pbox_ = pbox;

	Init(pDev, pbox->includedItem, materials, true);
}
void cCocktailWindow::Init(IDirect3DDevice9 *pDev, vector<pcDroping> materialList, vector<int> materials, bool unappreciatedCooktailable)
{
	InitBasic(pDev,0,0);
	pFromSelectW_ = pcSelectWindow(new cSelectWindow);
	materials_ = materials;
	materialList_ = materialList;
	materials_.resize(5);
	firstMaterials_ = materials_;
	unappreciatedCooktailable_ = unappreciatedCooktailable;

	int i;
	for(i=0;i<materialList.size();i++)
	{
		pcCommand pccm;
		pFromSelectW_->commandList.push_back(pccm = pcCommand(
			new cCommand_cocktailMenu(materialList[i]->FullName())));


	}
	{
		pcCommand pccm;
		pFromSelectW_->commandList.push_back(pccm = pcCommand(
			new cCommand_cocktailMenu(g_Lang(_T("(副材料)")))));
	}
	pFromSelectW_->enable = false;
	pFromSelectW_->Init(pDev,10,10);
	pFromSelectW_->setLeft(0);
	pFromSelectW_->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(2));


	pToSelectW_ = pcSelectWindow(new cSelectWindow);
	if(pToSelectW_->commandList.empty())
	{
		cCommand_cocktailMenu* pccm;
		pToSelectW_->commandList.push_back(pcCommand(
			pccm = new cCommand_cocktailMenu(g_Lang(_T("レシピを見る")))));
				
		pccm->id = 0;
		pccm->materials = materials_;
		pccm->unappreciatedCooktailable_ = unappreciatedCooktailable_;
	}
	pToSelectW_->Init(pDev,12,10);
	pToSelectW_->setRight(SCREEN_X);
	pToSelectW_->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(2));


	pFromCaptionW_ = pcGameWindow(new cGameWindow);
	pFromCaptionW_->Text() = g_Lang(_T("材料"));
	pFromCaptionW_->Init(pDev,16,1);
	pFromCaptionW_->setBottom(pFromSelectW_->Top());
	pFromCaptionW_->setLeft(pFromSelectW_->Left());

	pToCaptionW_ = pcGameWindow(new cGameWindow);
	pToCaptionW_->Text() = g_Lang(_T("メニュー"));
	pToCaptionW_->Init(pDev,12,1);
	pToCaptionW_->setBottom(pToSelectW_->Top());
	pToCaptionW_->setLeft(pToSelectW_->Left());

	pMaterialW_ = pcGameWindow(new cGameWindow);
	getMaterialText(materials_,pMaterialW_->Text());
	pMaterialW_->Init(pDev,4,5);
	pMaterialW_->setLeft(pFromSelectW_->Right());
	pMaterialW_->setTop(pFromSelectW_->Top());

}


StyleString cCocktailWindow::shortExplanationString(IDirect3DDevice9 *pDev)
{
	return pToSelectW_->shortExplanationString(pDev);
}


int cCocktailWindow::Draw(IDirect3DDevice9 *pDev)
{
	pMaterialW_->Draw(pDev);
	pFromCaptionW_->Draw(pDev);
	pToCaptionW_->Draw(pDev);
	pFromSelectW_->Draw(pDev);
	pToSelectW_->Draw(pDev);

	return true;
}

int cCocktailWindow::process(IDirect3DDevice9 *pDev)
{
		if(g_pPlayerInput()->cancel().justOn)
		{


			playsound_cancel();
			
			pcCommand_cocktailMenu pccm = boost::dynamic_pointer_cast<cCommand_cocktailMenu>(pToSelectW_->pforcusedcommand());
			if(pccm == NULL || pccm->id == 0)
			{//一つ消す
				
				int i;
				for(i=pToSelectW_->commandList.size()-1;i>=0;i--)
				{
					pccm = boost::dynamic_pointer_cast<cCommand_cocktailMenu>(pToSelectW_->commandList[i]);
					if(pccm == NULL || pccm->id == 0)
					{
						continue;
					}
					else
					{
						pToSelectW_->commandList.erase(pToSelectW_->commandList.begin()+i);
						break;
					}
				}
				if(i < 0)
				{
					closeLayer = true;
				}
			}
			else
			{//該当消す
				pToSelectW_->commandList.erase(find(pToSelectW_->commandList.begin(),pToSelectW_->commandList.end(),pccm));
			}

			ReRendarText();
			return true;
		}

		if(g_pPlayerInput()->decision().justOn)
		{
			pcCommand_cocktailDecition pccmd = boost::dynamic_pointer_cast<cCommand_cocktailDecition>(pToSelectW_->pforcusedcommand());
			if(pccmd != NULL)
			{//決定
				sg_pDungeonSystem->AnimationManager().
					Anime_PlaySE(_T("makecocktail.wav"),sg_pDungeonSystem->pPlayerChara()->足元地形()->place);
				
				if(delegateObject_) {
					int i;
					vector<pcDroping> resultCocktails;
					for(i=0;i<pToSelectW_->commandList.size();i++)
					{
						pcCommand_cocktailMenu pccm = boost::dynamic_pointer_cast<cCommand_cocktailMenu>(pToSelectW_->commandList[i]);
						if(pccm != NULL && pccm->id != 0)
						{
							
							pcDroping pdrop = sg_pDungeonSystem->落ち物生成_設置なし(pccm->id);
							
							resultCocktails.push_back(pdrop);

							pcDrink pcDrk = boost::dynamic_pointer_cast<cDrink>(pdrop);
							
							if(pcDrk != NULL)
							{
								pcDrk->quality() = pccm->quality;
								pcDrk->reuse() = 1;
								sg_pDungeonSystem->識別(pcDrk,false);
							}
						}
					}

					delegateObject_->didFinishCocktail(materialList_, resultCocktails);
				}
				else
				{
					pcBox_ID_24 pcocktailBox = boost::dynamic_pointer_cast<cBox_ID_24>(pbox_);
					if(pcocktailBox->state() == cDroping::STATE_GOOD)
					{
						pcocktailBox->state() = cDroping::STATE_NORMAL;
					}
					else
					{
						pcocktailBox->used() = 1;
					}

					int i;
					vector<pcDroping> vpdrop = pcocktailBox->includedItem;
					for(i=0;i<vpdrop.size();i++)
					{
						sg_pDungeonSystem->落ち物破壊要請(vpdrop[i]);
					}
					
					vector<pcDroping> resultCocktails;
					for(i=0;i<pToSelectW_->commandList.size();i++)
					{
						pcCommand_cocktailMenu pccm = boost::dynamic_pointer_cast<cCommand_cocktailMenu>(pToSelectW_->commandList[i]);
						if(pccm != NULL && pccm->id != 0)
						{
							
							pcDroping pdrop = sg_pDungeonSystem->落ち物生成_設置なし(pccm->id);
							
							resultCocktails.push_back(pdrop);

							pcDrink pcDrk = boost::dynamic_pointer_cast<cDrink>(pdrop);
							
							if(pcDrk != NULL)
							{
								pcDrk->quality() = pccm->quality;
								pcDrk->reuse() = 1;
								sg_pDungeonSystem->識別(pcDrk,false);
								sg_pDungeonSystem->落ち物入れ要請(pcocktailBox,pcDrk);
							}
						}
					}

				}

				sg_pDungeonSystem->メニューを閉じる();
				
				return true;
			}
		}
		pToSelectW_->process(pDev);
/*
		if(g_pPlayerInput()->dash().on)
		{
			if(g_pPlayerInput()->migi().justOn)
			{
				commandForward();
				playsound_move();
			}
			if(g_pPlayerInput()->hidari().justOn)
			{
				commandBack();
				playsound_move();
			}

		}
		else
		{

			cursolIndexX += (g_pPlayerInput()->migi().justOn);
			cursolIndexX -= (g_pPlayerInput()->hidari().justOn);

			if(cursolIndexX < 0) cursolIndexX = cursolIndexXMax;//注意
			if(cursolIndexX > cursolIndexXMax) cursolIndexX = 0;

			//上下
			cursolIndexY += (g_pPlayerInput()->shita().justOn);
			cursolIndexY -= (g_pPlayerInput()->ue().justOn);

			if(cursolIndexY < 0) cursolIndexY = cursolIndexYMax - 1;
			if(cursolIndexY >= cursolIndexYMax) cursolIndexY = 0;

			if(g_pPlayerInput()->shita().justOn 
				|| g_pPlayerInput()->ue().justOn
				|| g_pPlayerInput()->migi().justOn
				|| g_pPlayerInput()->hidari().justOn)
			{
				playsound_move();
			}


			if(g_pPlayerInput()->decision().justOn)
			{
				playsound_decide();
				if(cursolIndexX < cursolIndexXMax)
				{
					commandWrite(StrSet*cursolIndexXMax*cursolIndexYMax + cursolIndexXMax*cursolIndexY + cursolIndexX);
				}
				else
				{
					if(cursolIndexY < strSetList_.size()) commandStrset(cursolIndexY);
					else if(cursolIndexY == commandIndexErase()) commandErase();
					else if(cursolIndexY == commandIndexCover()) commandCover();
					else if(cursolIndexY == commandIndexChange()) commandChange();
					else if(cursolIndexY == commandIndexBack()) commandBack();
					else if(cursolIndexY == commandIndexForward()) commandForward();
					else if(cursolIndexY == commandIndexEnd()) commandEnd();
				}
			}


		}


		if(g_pPlayerInput()->shot().justOn)
		{
			commandCover();
			playsound_decide();
		}


		if(g_pPlayerInput()->diagon().justOn)
		{
			commandChange();
			playsound_decide();
		}


		if(g_pPlayerInput()->miniMap().justOn)
		{
			cursolIndexX = cursolIndexXMax;
			cursolIndexY = cursolIndexYMax -1 ;
			playsound_decide();
		}

		if(g_pPlayerInput()->menu().justOn)
		{
			StrSet++;
			if(StrSet >= STRINGWINDOW_SETNUM) StrSet = 0;
			playsound_decide();
			preRendar = false;
		}
	}
*/
	return true;
}

int cCocktailWindow::CursorDraw(IDirect3DDevice9 *pDev)
{
	/*
	m_DO.setTexture(m_pTexture_Cursor, CURSORSIZE, CURSORSIZE);
	int l = Left();
	int t = Top();
	double ew = EDGEWIDTH;
	double ew_s = sideEdgeWith();

	//m_DO.m_color.ARGB(255,255,255,255);
	m_DO.Width = m_DO.Height = CURSORSIZE;

	if(g_pPlayerInput()->dash().on)
	{
		l = pStrW_->Left();
		t = pStrW_->Top();
		m_DO.setLeft(l+ew_s+(cursolforcus-1)*(GWFONTSIZE));
		m_DO.CenterY = t + ew + lineheight()/2;
	}
	else
	{
		m_DO.setLeft(l+ew_s+cursolIndexX*(GWFONTSIZE + CURSORSIZE));
		m_DO.CenterY = t + ew + cursolIndexY*lineheight() + lineheight()/2;
	}
	m_DO.m_TexRange.setLTRB(0,		0,		1,		1);
	
	m_DO.m_color = color;
	DrawAddShadowDO_mini(m_DO,pDev);
*/

	return true;
}

int cCocktailWindow::ReRendarText()
{
	int i,m;

	pcBox_ID_24 pcocktailBox = boost::dynamic_pointer_cast<cBox_ID_24>(pbox_);
	if(pcocktailBox){
		pcocktailBox->SumMaterial(materials_);
	}
	else {
		materials_ = firstMaterials_;
	}

	pcCommand_cocktailMenu pccm;
	for(i=pToSelectW_->commandList.size()-1;i>=0;i--)
	{//レシピを見るを削除
		pccm = boost::dynamic_pointer_cast<cCommand_cocktailMenu>(pToSelectW_->commandList[i]);
		if(pccm == NULL)
		{
			pToSelectW_->commandList.erase(pToSelectW_->commandList.begin()+i);
			continue;
		}
		else
		{
			if(pccm->id == 0)
			{
				pToSelectW_->commandList.erase(pToSelectW_->commandList.begin()+i);
				continue;
			}
		}
	}

	int count = 0;
	for(i=0;i<pToSelectW_->commandList.size();i++)
	{
		pcCommand_cocktailMenu pccm = boost::dynamic_pointer_cast<cCommand_cocktailMenu>(pToSelectW_->commandList[i]);
		if(pccm == NULL) continue;


		pcDrink pDrk = boost::dynamic_pointer_cast<cDrink>(sg_pDungeonSystem->DataBase.GetSampleDroping(pccm->id));

		if(pDrk == NULL) continue;

		pDrk->quality() = pccm->quality;
		pDrk->state() = cDroping::STATE_NORMAL;

		
		for(m=0;m<cDrink::MATERIALNUM;m++)
		{
			materials_[m] -= pDrk->マテリアル(m);
		}
		materials_[m] -= !(pDrk->reuse());
		
		tstring str = _T("");
		if(pccm->quality)
		{
			str += _T("+");
			str += setStyle(pccm->quality).conclete_tstr();
		}
		pccm->caption = pDrk->FullNameBase() + str;

		++count;
	}
	if(count == pToSelectW_->commandList.size())
	{
		pcCommand_cocktailMenu pccm;
		pToSelectW_->commandList.push_back(pccm = pcCommand_cocktailMenu(
			new cCommand_cocktailMenu(g_Lang(_T("レシピを見る")))));
				
		pccm->id = 0;
		pccm->materials = materials_;
		pccm->unappreciatedCooktailable_ = unappreciatedCooktailable_;
	}
	if(pToSelectW_->commandList.size() > 1)
	{
		pToSelectW_->commandList.push_back(pcCommand_cocktailDecition(
			new cCommand_cocktailDecition()));
	}
	pMaterialW_->Text().clear();
	getMaterialText(materials_,pMaterialW_->Text());


	pMaterialW_->ReRendarText();

	pToSelectW_->Init(sg_pDungeonSystem->pDevice_D3D,12,10);
	pToSelectW_->ReRendarText();
	return true;
}
int cCocktailWindow::RerenderDraw(IDirect3DDevice9 *pDev)
{
	//pFromSelectW_->RerenderDraw(pDev);
	//pToSelectW_->RerenderDraw(pDev);

	return true;
}

int cCocktailWindow::TextDraw(IDirect3DDevice9 *pDev)
{
	//pFromSelectW_->TextDraw(pDev);
	//pToSelectW_->TextDraw(pDev);

	return true;
}
