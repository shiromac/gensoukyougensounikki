#include "stdafx.h"

#include "cDroping.h"

#include "cDropingFunc.h"
#include "cEquipment.h"
#include "cStringWindow.h"

#include "gameMainSystem/filemanage/cScriptRLayer.h"
#include "scriptprocesser/DropsScript.h"

#include "cSaveStore.h"

#include "utility/debug.h"

#include "FireCommand.h"


#define DEF_ICON_INDEXSIZE 4.0


cDroping::cDroping(void)
{
	opaque = 1;
	pri_discover = false;
	broken = false;
	m_color.ARGB(255,255,255,255);


	跳ね返し無効フラグ = 0;
	pri_onland_DrawFlag = FALSE;

	int i;
	for(i=0;i<DEF_DROP_DATAPACK_NUM;i++)
	{
		dpack.integer[i] = 0;
	}

	pmemo = pcMemoString(new cMemoString);
	pmemo->setpDataSize(&dpack_char_index(9,3));
	pmemo->setpdata(&dpack_char_index(10,0),0);
	pmemo->setpdata(&dpack_char_index(10,1),1);
	pmemo->setpdata(&dpack_char_index(10,2),2);
	pmemo->setpdata(&dpack_char_index(10,3),3);
	pmemo->setpdata(&dpack_char_index(11,0),4);
	pmemo->setpdata(&dpack_char_index(11,1),5);
	pmemo->setpdata(&dpack_char_index(11,2),6);
	pmemo->setpdata(&dpack_char_index(11,3),7);
	pmemo->setDataSize(0);

	踏みフラグ_ = 0;

	pmemo = pcMemoString(new cMemoString);

	Holder_weakpointer = wpcCharacter(NULLCHARA);
	Parent_weakpointer = wpcDroping(NULLDROP);

	extraMaterialColor_alpha = 255;
	extraMaterialColor_red = 255;
	extraMaterialColor_green = 255;
	extraMaterialColor_blue = 255;
}

cDroping::~cDroping(void)
{
}

int cDroping::OnMapping()
{
	return discover();
}

//見つけた
int cDroping::discover()
{
	return pri_discover = true;
}
//見失う
int cDroping::disappear()
{
	return pri_discover = false;
}
//見つけたか
bool cDroping::isDiscover()
{
	return (pri_discover != false);
}

bool cDroping::isShouldDraw()
{
	return (pri_onland_DrawFlag);
}


StyleString cDroping::shortExplanation()
{
	if(御霊状態())
	{
		return 能力説明();
	}

	StyleString Base;
	
	if(大局識別済み())
	{
		Base = shortExplanationBase();
	}
	else
	{

		map<tstring, StyleString> valiable;
		valiable[_T("Item")] = FullName();
		g_Lang(_T("Item未識別shortExplanation"),Base,valiable);
		
		//Base = _T(" - ") + FullName() + _T(" -");
		//Base += _T("\n");
		//Base += _T("識別されていないので、 どんな効果なのかわからない。\n");
	}
	return Base;
}

StyleString cDroping::longExplanation()
{
	StyleString Base;
	
	if(大局識別済み())
	{
		Base = longExplanationBase();
	}
	else
	{

		map<tstring, StyleString> valiable;
		valiable[_T("Item")] = FullName();
		g_Lang(_T("Item未識別longExplanationBase"),Base,valiable);
		
		//Base = _T(" - ") + FullName() + _T(" -");
		//Base += _T("\n");
		//Base += _T("識別されていないので、 どんな効果なのかわからない。\n");
	}

	return Base;
}


void cDroping::Init(IDirect3DDevice9 *pDev,pcDroping self)
{

	m_pTexture_Icon = g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev, IconFileName().c_str());
	m_pTexture_StateIcon = g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev, _T("item\\itemstateicon.png"));

	self_weakpointer = wpcDroping(self);

	setattribute_pri();
}


void cDroping::settingInit()
{
	pcLandform pland = 現在地形();
	if(pland)
	{
		if(pland->mapped)
		{
			discover();
		}
	}
	opaque = isDiscover();
	visibleplace.set(placeX,placeY,0,0);
	pri_onland_DrawFlag = is_onland_DrawFlag();
}
void cDroping::setattribute_pri()
{
	set<落ち物属性::落ち物属性> temp;
	sg_pDungeonSystem->DataBase.def_decode_Dropattri(sg_pDungeonSystem->DataBase.DropImportData(ID(),(tstring)_T("属性")), temp);
	属性.insert(temp.begin(),temp.end());

	setattribute();
}

void cDroping::Draw(IDirect3DDevice9 *pDev)
{
	cDrawingObject DO;
	GetIcon(DO);



	DO.Width = MAPTEXBOXSIZE*MAPTEXPOWER;
	DO.Height = MAPTEXBOXSIZE*MAPTEXPOWER;

	DO.CenterX =  
		MAPDRAWCENTERX + MAPTEXBOXSIZE*MAPTEXPOWER*(visibleplace.x - mapForcus.x)
		+2;
	
	DO.CenterY =  
		MAPDRAWCENTERY + MAPTEXBOXSIZE*MAPTEXPOWER*(visibleplace.y - mapForcus.y)
		+2;
	
	//影
	DO.m_color.ARGB(128,0,0,0);
	DO.m_color.alpha = opaque*DO.m_color.alpha;
		
	DO.Draw(pDev);

	
	DO.CenterX =  
		MAPDRAWCENTERX + MAPTEXBOXSIZE*MAPTEXPOWER*(visibleplace.x - mapForcus.x);
	
	DO.CenterY =  
		MAPDRAWCENTERY + MAPTEXBOXSIZE*MAPTEXPOWER*(visibleplace.y -visibleplace.z*0.5 - mapForcus.y);
	
	//本体
	DO.m_color.ARGB(255,255,255,255);
	DO.m_color.alpha = opaque*DO.m_color.alpha;

	DO.m_color *= extraMaterialColor();
	DO.Draw(pDev);
}
const cColor cDroping::extraMaterialColor()
{
	return cColor(extraMaterialColor_alpha, extraMaterialColor_red, extraMaterialColor_green, extraMaterialColor_blue);
}

void cDroping::setExtraMaterialColor(int alpha, int red, int green, int blue)
{
	if(sg_pDungeonSystem)
	{
		sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(&(extraMaterialColor_alpha), alpha);
		sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(&(extraMaterialColor_red), red);
		sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(&(extraMaterialColor_green), green);
		sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(&(extraMaterialColor_blue), blue);
	}
	else
	{
		extraMaterialColor_alpha = alpha;
		extraMaterialColor_red = red;
		extraMaterialColor_green = green;
		extraMaterialColor_blue = blue;
	}
}
void cDroping::visibleReset()
{
	if(現在地形()->iswater())
	{
		setExtraMaterialColor(180,170,220,255); 
	}
	else
	{
		setExtraMaterialColor(255,255,255,255); 
	}
}
void cDroping::DrawIcon(IDirect3DDevice9 *pDev,int x,int y)
{
	cDrawingObject DO;


	DO.CenterX = x;
	DO.CenterY = y;

	DO.Width = IconFileSize()/DEF_ICON_INDEXSIZE;
	DO.Height = IconFileSize()/DEF_ICON_INDEXSIZE;



	GetIconForIcon(DO);



	DO.m_color = m_color;
	DrawAddShadowDO_mini(DO,pDev);

}
void cDroping::DrawStateIcon(IDirect3DDevice9 *pDev,int x,int y)
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


	if(state() == cDroping::STATE_CURSE)
	{//呪いアイコン

		if(状態値識別済み())
		{
			DO.CenterX = liftline;
			DO.CenterY = bottomline;

			DO.m_TexRange.setLTRB(0,0,0.25,0.25);
			DO.Draw(pDev);
		}
	}
	else if(state() == cDroping::STATE_GOOD)
	{//信仰アイコン
		DO.CenterX = liftline;
		DO.CenterY = bottomline;

		DO.m_TexRange.setLTRB(0.25,0,0.5,0.25);
		DO.Draw(pDev);
	}






	DrawStateIconSub(pDev,x,y);


		
	
}

int cDroping::GetIcon(cDrawingObject& DO)
{
	
	DO.setTexture(m_pTexture_Icon,IconFileSize(),IconFileSize());
	DO.m_TexRange.setLTRB(IconFileIndexX()/DEF_ICON_INDEXSIZE, IconFileIndexY()/DEF_ICON_INDEXSIZE,
					(IconFileIndexX()+1)/DEF_ICON_INDEXSIZE, (IconFileIndexY()+1)/DEF_ICON_INDEXSIZE);


	return true;
}
int cDroping::GetIconForIcon(cDrawingObject& DO)
{
	
	DO.setTexture(m_pTexture_Icon,IconFileSize(),IconFileSize());
	DO.m_TexRange.setLTRB(IconForIconFileIndexX()/DEF_ICON_INDEXSIZE, IconForIconFileIndexY()/DEF_ICON_INDEXSIZE,
					(IconForIconFileIndexX()+1)/DEF_ICON_INDEXSIZE, (IconForIconFileIndexY()+1)/DEF_ICON_INDEXSIZE);


	return true;
}

int cDroping::RerenderText(SpriteText & sprite,int MaxLetterLength)
{

	StyleString str = ListFullName();



	if(str.sector.size() > 0)
	{//セクションが一つ以上

		if(str.colors[0] == ITEM_CAPTION_COLOR)
		{
			str.setSameColor(0xFFFFFFFF);
		}


		//文字が長いと縮小
		sprite.FitTextWidth(str,MaxLetterLength*GWFONTSIZE);

		/*
		if(str.length() > MaxLetterLength*2)
		{	
			str.sizes[0].x = (double)MaxLetterLength*2*0.9/str.length();
			str.setSameSize(str.sizes[0]);
		}
		*/

	}

	
	sprite.DrawText(str);



	return true;
}

int cDroping::SetMenu(IDirect3DDevice9 *pDev)
{
	vector<tstring> captions;
	GetmenuCaption(captions);

	//コマンド挿入
	cValiableField val;
	val.strvectors.dim(変数_挿入コマンド);
	val.intvectors.dim(変数_挿入コマンド位置);
	sg_pDungeonSystem->CutInM().CutIn(Parent(),コマンド挿入_子アイテム_タイミング,val);
	if(!val.strvectors[変数_挿入コマンド].empty())
	{//挿入コマンドあり
		vector<tstring>::iterator itr;
		int i;
		for(i=0;i<val.strvectors[変数_挿入コマンド].size();i++)
		{

			int index = -1;
			if(i < val.intvectors[変数_挿入コマンド位置].size())
			{
				index = val.intvectors[変数_挿入コマンド位置][i];
			}
			const tstring& comstr = val.strvectors[変数_挿入コマンド][i];
			vector<tstring>::iterator itr = find(captions.begin(),captions.end(),comstr);
			if(itr == captions.end())
			{//重複無し
				if(index < 0 || index >= captions.size()) index = captions.size();
				captions.insert(captions.begin()+index ,comstr);
			}
		}
	}

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

//メニューで選択時に最初に呼ばれる
int cDroping::Action(IDirect3DDevice9 *pDev)
{
	SetMenu(pDev);
	return true;
}


/*
//コマンドを解決する;
int cDroping::FireCommand(IDirect3DDevice9 *pDev, tstring verb, vector<pcDroping> &ObjectList)
{

	if(verb == _T("説明")) 説明(pDev);
	if(verb == _T("拾う")) 拾う(pDev);
	if(verb == _T("置く")) 置く(pDev);
	if(verb == _T("投げる")) 投げる(pDev);
	if(verb == _T("交換")) 交換(pDev);


	return true;
}
*/

/*
int cDroping::説明(IDirect3DDevice9 *pDev)
{
	pcControlLayer pccl;
	pcGameWindow pcgw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	//新規ゲームウィンドウ
	pccl->WindowList.push_back(pcgw = pcGameWindow(new cGameWindow));
	pcgw->Init(pDev, 21, 10);


	pcgw->Text() = longExplanation();

	pcgw->CenterX = SCREEN_X/2;
	pcgw->setTop(80);
	
	return true;
}
*/

/*
int cDroping::置く(IDirect3DDevice9 *pDev)
{

	if(sg_pDungeonSystem->足元設置要請(sg_pDungeonSystem->pPlayerChara(), me()))
	{

		sg_pDungeonSystem->メッセージ(_T("足元に")+me()->FullName()+_T("を置いた。\n"));
		sg_pDungeonSystem->メニューを閉じる();
		sg_pDungeonSystem->ターンエンド();
		return true;
	}

	return false;
	
}
*/

/*
int cDroping::拾う(IDirect3DDevice9 *pDev)
{

	pcDroping pdrop = sg_pDungeonSystem->pPlayerChara()->足元();
	if(sg_pDungeonSystem->足元拾得要請(sg_pDungeonSystem->pPlayerChara()))
	{
		
		sg_pDungeonSystem->AnimationManager().Anime_PlaySE(_T("pickup.wav"),pdrop->現在地形()->place);
		if(!me()->broken)
		{//まとめられて無い
			sg_pDungeonSystem->メッセージ(pdrop->FullName() + _T("を拾った。\n"));
		}
		sg_pDungeonSystem->メニューリフレッシュ();
		return true;
	}
	
	if(!sg_pDungeonSystem->pPlayerChara()->持ち物余白あり())
	{
		sg_pDungeonSystem->メッセージ(_T("持ち物がいっぱいで ") + pdrop->FullName() + _T("は拾えなかった。\n"));
	}
	else
	{
		sg_pDungeonSystem->メッセージ(pdrop->FullName() + _T("は拾えなかった。\n"));
	}
	return false;
	
}
*/

/*
int cDroping::投げる(IDirect3DDevice9 *pDev)
{

	if(sg_pDungeonSystem->投擲要請(sg_pDungeonSystem->pPlayerChara(), me()))
	{
		sg_pDungeonSystem->メニューを閉じる();
		sg_pDungeonSystem->ターンエンド();
		return true;
	}
	return false;
	
}
*/
/*
int cDroping::交換(IDirect3DDevice9 *pDev)
{
	
	pcDroping pdrop = sg_pDungeonSystem->pPlayerChara()->足元();
	if(sg_pDungeonSystem->足元交換要請(sg_pDungeonSystem->pPlayerChara(), me()))
	{
		sg_pDungeonSystem->メッセージ(_T("足元に")+me()->FullName()+_T("を置いて、")
			+pdrop->FullName() + _T("と交換した。\n"));
		sg_pDungeonSystem->メニューを閉じる();
		sg_pDungeonSystem->ターンエンド();
		return true;
	}
	return false;
	
}
*/

int cDroping::乗る_記録()
{
	return 踏みフラグ_ = 1;
}
int cDroping::乗る()
{
	pcCharacter pchara = 乗りキャラ();
	if(pchara != NULL)
	{
		pcPlayerChara ppchara;
		ppchara = boost::dynamic_pointer_cast<cPlayerChara>(pchara);
		if(ppchara != NULL)
		{//プレイヤー

			if(g_pPlayerInput()->cancel().on || g_pPlayerInput()->dash().on
				|| shopitemflag() != SHOPFLAG_NONE)
			{//拾わない
				map<tstring, StyleString> valiable;
				valiable[_T("Item")] = FullName();
				g_Langメッセージ(_T("アイテム乗るメッセージ"),valiable);
			
				//sg_pDungeonSystem->メッセージ(FullName() + _T("の上に乗った。\n"));
			}
			else
			{//拾う
				if(!拾う(sg_pDungeonSystem->pDevice_D3D))
				{//ひろえなかった
					map<tstring, StyleString> valiable;
					valiable[_T("Item")] = FullName();
					g_Langメッセージ(_T("アイテム乗るメッセージ"),valiable);
				}


				/*
				if(sg_pDungeonSystem->足元拾得要請(pchara))
				{
					sg_pDungeonSystem->AnimationManager().Anime_PlaySE(_T("pickup.wav"),pchara->足元地形()->place);
					if(!me()->broken)
					{//まとめられて無い
						sg_pDungeonSystem->メッセージ(FullName() + _T("を拾った。\n"));
					}
				}
				else
				{
					if(!pchara->持ち物余白あり())
					{
						sg_pDungeonSystem->メッセージ(_T("持ち物がいっぱいで 拾えない。\n"));
					}
					sg_pDungeonSystem->メッセージ(FullName() + _T("の上に乗った。\n"));
				}
				*/
				
			}
				
		}
		return true;
	}
	return false;
}
int cDroping::上に落ちる(pcDroping pdrop)
{
	return false;
}
int cDroping::上に落ちる(pcCharacter pchara)
{
	return false;
}
int cDroping::上を通過(pcDroping pdrop)
{
	return false;
}
int cDroping::上を通過(pcCharacter pchara)
{
	return false;
}
int cDroping::衝突(pcCharacter pchara)
{

	multiset<攻撃属性::攻撃属性> 属性;

	if(投擲者() == NULL)
	{
		投擲者_weakpointer = wpcCharacter(NULLCHARA);
	}


	属性.insert(攻撃属性::落ち物);
	if(跳ね返し無効フラグ)属性.insert(攻撃属性::跳ね返し無効);
	sg_pDungeonSystem->攻撃接近(攻撃作成(
		投擲者(),//攻撃者
		pchara->足元地形(),//場所
		1,//攻撃力
		true,//定数ダメージ
		属性,
		true
		));

	return true;
}
bool cDroping::投擲貫通()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		ID(),(tstring)_T("投擲貫通"),0);
}
int cDroping::デフォルト大局識別済み()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		ID(),(tstring)_T("デフォルト大局識別済み"),0);
}

int cDroping::被呪い要請()
{
	if(state() == cDroping::STATE_GOOD)
	{
		state() = cDroping::STATE_NORMAL;
	}
	else
	{
		state() = cDroping::STATE_CURSE;
	}

	return true;
}
int cDroping::被祝福要請()
{
	if(state() == cDroping::STATE_CURSE)
	{
		state() = cDroping::STATE_NORMAL;
	}
	else
	{
		state() = cDroping::STATE_GOOD;
	}

	return true;
}
int cDroping::異常状態解除_内容込み()
{
	int eff = 0;
	vector<pcDroping> vpdrop = includedItem;
	int i,size = vpdrop.size();
	for(i=0;i<size;i++)
	{
		eff |= vpdrop[i]->異常状態解除_内容込み();
	}
	return eff || 異常状態解除();
}
int cDroping::異常状態解除()
{
	return false;
}
pcCharacter cDroping::乗りキャラ()
{
	pcLandform pland = sg_pDungeonSystem->Map().Land(placeX,placeY);

	return pland->pOnChar;
}

pcLandform cDroping::現在地形()
{
	pcLandform pland;
	if(Holder() == NULL)
	{
		if(Parent() != NULL) pland = Parent()->現在地形();
		else pland = sg_pDungeonSystem->Map().Land(placeX,placeY);
	}
	else
		pland = Holder()->足元地形();
	return pland;
}

bool cDroping::落ちてる()
{
	return 現在地形()->pOnDrop == me();
}
bool cDroping::倉庫の中()
{
	return !落ちてる() && (Holder() == NULL && (Parent() == NULL || Parent()->倉庫の中()));
}




//---------------------------------
//識別
void cDroping::大局Appreciate()
{
	sg_pDungeonSystem->DataBase.大局appreciate(ID());
}

void cDroping::大局untiappreciate()
{
	sg_pDungeonSystem->DataBase.大局untiappreciate(ID());
}

void cDroping::allAppreciate()
{
	appreciation_indivi() = true;
	appreciation_state() = true;
	appreciation_quality() = true;
}
void cDroping::untiappreciate()
{
	appreciation_indivi() = false;
	appreciation_state() = false;
	appreciation_quality() = false;
}


int cDroping::完全識別済み()
{
	return (appreciation_indivi()
		&& appreciation_state()
		&& appreciation_quality()
		&& 大局識別済み());
}



int cDroping::大局識別済み()
{
	return (sg_pDungeonSystem->DataBase.大局識別済み(ID()));
}

int cDroping::状態値識別済み()
{
	if(御霊状態())
	{
		return Parent()->状態値識別済み();
	}
	else
	{
		return appreciation_state();
	}
}
int cDroping::修正値識別済み()
{
	if(御霊状態())
	{
		return Parent()->修正値識別済み();
	}
	else
		return appreciation_quality();
}


//外部からのインポート系
tstring cDroping::FullNameBase()
{
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.DropImportLanguage(ID(),
		(tstring)_T("FullNameBase"));
	if(psr == NULL) return NoDataString();
	return psr->topscript();
}
tstring cDroping::ShortNameBase()
{
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.DropImportLanguage(ID(),
		(tstring)_T("ShortNameBase"));
	if(psr == NULL) return NoDataString();
	return psr->topscript();
}

tstring cDroping::usefulName()
{
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.DropImportData(ID(),
		(tstring)_T("usefulName"));
	if(psr == NULL) return NoDataString();
	return psr->topscript();
}

tstring cDroping::NoDataString()
{
	return tstring(_T("No data: ID")) + setStyle(ID()).conclete_tstr();
}

int cDroping::値段基礎値()
{
	return sg_pDungeonSystem->DataBase.DropPrice(ID());
}
int cDroping::値段補正値()
{
	return max(0,値段基礎値()*(1.0 + 0.1*quality())*(1.0+(int)(state() == cDroping::STATE_GOOD)));
}

int cDroping::単体買値(int homeshopflag)
{
	if(homeshopflag)
	{
		return 値段補正値()*sg_pDungeonSystem->pSaveData->LevelShopDiscount();
	}
	else return 値段補正値()*値段割引();
}
int cDroping::内包アイテム込み買値(int homeshopflag)
{
	int value = 単体買値(sg_pDungeonSystem->拠点フラグ());
	if(!内包アイテムは実体がない())
	{
		int i;
		for(i=0;i<includedItem.size();i++)
		{
			value += includedItem[i]->内包アイテム込み買値(homeshopflag);
		}
	}
	return value;
}
int cDroping::単体売値()
{
	return 値段補正値()*値段割引()/2;
}
int cDroping::内包アイテム込み売値()
{
	int value = 単体売値();
	if(!内包アイテムは実体がない())
	{
		int i;
		for(i=0;i<includedItem.size();i++)
		{
			value += includedItem[i]->内包アイテム込み売値();
		}
	}
	return value;
}
int cDroping::単体購入支払い値()
{
	if(shopitemflag() == SHOPFLAG_SHOP)
	{
		return me()->単体買値(sg_pDungeonSystem->拠点フラグ());
	}
	else
	{
		return 0;
	}
}
int cDroping::内包アイテム込み購入支払い値()
{
	int value = 単体購入支払い値();
	if(!内包アイテムは実体がない())
	{
		int i;
		for(i=0;i<includedItem.size();i++)
		{
			value += includedItem[i]->単体購入支払い値();
		}
	}
	return value;
}
int cDroping::単体売却受け取り値()
{
	if(shopitemflag() == SHOPFLAG_SHOP)
	{
		return 0;
	}
	else
	{
		return me()->単体売値();
	}
}
int cDroping::内包アイテム込み売却受け取り値()
{
	int value = 単体売却受け取り値();
	if(!内包アイテムは実体がない())
	{
		int i;
		for(i=0;i<includedItem.size();i++)
		{
			value += includedItem[i]->単体売却受け取り値();
		}
	}
	return value;
}

double cDroping::値段割引()
{
	if(shopDiscountflag() == SHOPDISCOUNTFLAG_FREE)
	{
		return 0.0;
	}
	else if(shopDiscountflag() == SHOPDISCOUNTFLAG_HALF)
	{
		return 0.5;
	}
	else if(shopDiscountflag() == SHOPDISCOUNTFLAG_NONE)
	{
		return 1.0;
	}
	OnAssert(_T(__FILE__),__LINE__,false,_T("割引フラグが不正な値です"));
	return 1.0;
}
int cDroping::厄ポイント単体買値()
{
	return 厄ポイント値段補正値()*sg_pDungeonSystem->pSaveData->LevelShopDiscount()*厄ポイント割引();
}
int cDroping::厄ポイント単体売値()
{
	return 厄ポイント値段補正値()*厄ポイント割引()/2;
}
double cDroping::厄ポイント割引()
{
	if(shopDiscountflag() == SHOPDISCOUNTFLAG_FREE)
	{
		return 0.0;
	}
	else if(shopDiscountflag() == SHOPDISCOUNTFLAG_HALF)
	{
		return 0.5;
	}
	else if(shopDiscountflag() == SHOPDISCOUNTFLAG_NONE)
	{
		return 1.0;
	}
	OnAssert(_T(__FILE__),__LINE__,false,_T("割引フラグが不正な値です"));
	return 1.0;
}
int cDroping::厄ポイント値段基礎値()
{
	return max(1.0,(10.0 - log(レア度())/log(2.0)))*100;
}
int cDroping::厄ポイント値段補正値()
{
	return  max(0,厄ポイント値段基礎値()*(1.0 + 0.1*quality())*(1.0+(int)(state() == cDroping::STATE_GOOD)));
}
	
double cDroping::効果量係数()
{
	return max(0.5,1+quality()*効果量修正値倍率());
}
double cDroping::効果量基礎(int index)
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(ID(), (tstring)_T("効果量基礎"), DEF_UNDEFINE_DEFAULT, index);
}
int cDroping::効果量基礎_定義サイズ()
{
	return sg_pDungeonSystem->DataBase.DropImportData_ValueSize(ID(), (tstring)_T("効果量基礎"));
}
//最終出力
int cDroping::効果量(int index)
{
	return 効果量基礎(index)* 効果量係数()*効果量状態倍率();
}
void cDroping::修正値丸め()
{
	if(abs(quality()) > 修正値最大値())
	{
		quality() = 修正値最大値()*(quality()/abs(quality()));
	}
}
double cDroping::効果量状態倍率()
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
double cDroping::効果量修正値倍率()
{
	
	if(御霊状態())
	{
		return Parent()->効果量修正値倍率();//0
	}
	else
		return sg_pDungeonSystem->DataBase.DropImportData_Value(ID(), (tstring)_T("効果量修正値倍率"), 0.5);
}

void cDroping::効果量推定テキスト(int index, StyleString& output)
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
		output.addstring(効果量(index));
		return;
	}
}

//修正値出現分布
void cDroping::修正値出現分布(map<int,int>& output)
{
	output.clear();
	tstring datanamestr = _T("修正値出現分布");
	sg_pDungeonSystem->DataBase.DropImportData_MapIntToInt(ID(), datanamestr, output);
	return;
}
void cDroping::修正値出現分布テキスト_デバグ用(StyleString& output)
{
	
	map<int,int> mapii;
	修正値出現分布(mapii);

	output.addstring(_T("修正値出現分布 "));
	int i;
	map<int,int>::iterator itr = mapii.begin();
	for(;itr!=mapii.end();itr++)
	{
		output.addstring(_T("["));
		output.addstring(itr->first);
		output.addstring(_T("|"));
		output.addstring(itr->second);
		output.addstring(_T("]"));
		output.addstring(_T(";"));
	}
	output.addstring(_T("\n"));
}
//状態値出現分布
void cDroping::状態値出現分布(map<int,int>& output)
{
	output.clear();
	tstring datanamestr = _T("状態値出現分布");
	sg_pDungeonSystem->DataBase.DropImportData_MapIntToInt(ID(), datanamestr, output);
	return;
}
void cDroping::状態値出現分布テキスト_デバグ用(StyleString& output)
{
	
	map<int,int> mapii;
	状態値出現分布(mapii);

	output.addstring(_T("状態値出現分布 "));
	int i;
	map<int,int>::iterator itr = mapii.begin();
	for(;itr!=mapii.end();itr++)
	{
		output.addstring(_T("["));
		output.addstring(itr->first);
		output.addstring(_T("|"));
		output.addstring(itr->second);
		output.addstring(_T("]"));
		output.addstring(_T(";"));
	}
	output.addstring(_T("\n"));
}

//レア度(出現確率基礎値)
double cDroping::レア度()
{
	return sg_pDungeonSystem->DataBase.DropRarity(ID());
}

double cDroping::命中力()
{
	return 命中力基礎();
}
double cDroping::命中力基礎()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(ID(), (tstring)_T("命中力基礎"), 100.0);
}

StyleString cDroping::shortExplanationBase()
{
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.DropImportLanguage(ID(),
		(tstring)_T("shortExplanationBase"));
	if(psr == NULL) return NoDataString();
	
	map<tstring,StyleString> valiable;
	StyleString sstr;

	int i, size = 効果量基礎_定義サイズ();
	for(i=0;i<size;i++)
	{
		効果量推定テキスト(i,sstr);
		valiable[(_T("Value")+setStyle(i)).c_str()] = sstr;
	}


	sstr.clear();
	Drop_StringAddStyle(psr,sstr,valiable);
	return sstr;
}
///　　　５　　　　10　　　　15　　　　20|
StyleString cDroping::longExplanationBase()
{	
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.DropImportLanguage(ID(),
		(tstring)_T("longExplanationBase"));
	if(psr == NULL) return NoDataString();
	
	map<tstring,StyleString> valiable;
	StyleString sstr;


	int i, size = 効果量基礎_定義サイズ();
	for(i=0;i<size;i++)
	{
		効果量推定テキスト(i,sstr);
		valiable[(_T("Value")+setStyle(i)).c_str()] = sstr;
	}


	sstr.clear();
	Drop_StringAddStyle(psr,sstr,valiable);
	return sstr;
}

/*
double def_decode_value(int ID, double defaultval, const TCHAR* valname)
{
	
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.DropImportData(ID,
		(tstring)valname);
	if(psr == NULL) return defaultval;

	double res;
	TCHAR* e;
	res = _tcstod(psr->topscript().c_str(),&e);
	if(!(psr->topscript().length() != 0))
	{
		OnAssert(_T(__FILE__),__LINE__, false , sg_pDungeonSystem->DataBase.GetSampleDroping(ID)->FullNameBase().c_str(),
			((tstring)_T(" > ")+valname+_T(" に　空欄　が設定されています。")).c_str());
	}
	if(!((_tcslen(e) == 0) && ( !(res == 0 && _tcslen(e) == 0) || (psr->topscript() == _T("0")))))
	{
		OnAssert(_T(__FILE__),__LINE__, false , sg_pDungeonSystem->DataBase.GetSampleDroping(ID)->FullNameBase().c_str(),
			((tstring)_T(" > ")+valname+_T(" に　解釈不能な数値　が設定されています。")).c_str());
	}
	return res;
	
}
*/

/*
int def_decode_value_size(int ID, const TCHAR* valname)
{
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.DropImportData(ID,
		(tstring)valname);
	if(psr == NULL) return 0;

	return psr->script().size();
}
*/
pcCharacter cDroping::装備者_攻撃用()
{
	return 装備者();//仕様変更でとりあえず装備してたら返すように
}
pcCharacter cDroping::装備者_防御用()
{
	return 装備者();//仕様変更でとりあえず装備してたら返すように
}
pcCharacter cDroping::装備者()
{
	if(Parent())
	{
		pcEquipment pcequ = boost::dynamic_pointer_cast<cEquipment>(Parent());
		if(pcequ == NULL) return NULLCHARA;
		if(pcequ->装備者() != NULL)
		{
			return pcequ->装備者();
		}
	}
	return NULLCHARA;
}
bool cDroping::装備されている_攻撃用()
{
	pcCharacter pchara = 装備者();
	if(pchara != NULL)
	{
		if(pchara->attackequipment)
		{
			if(pchara->attackequipment == me() || pchara->attackequipment == Parent())
			{
				return true;
			}
		}
	}
	return false;
};
bool cDroping::装備されている_防御用()
{
	pcCharacter pchara = 装備者();
	if(pchara != NULL)
	{
		if(pchara->defenseequipment)
		{
			if(pchara->defenseequipment == me() || pchara->defenseequipment == Parent())
			{
				return true;
			}
		}
	}
	return false;
};
bool cDroping::能力発動条件満たしている_攻撃用()
{
	pcCharacter chara = 装備者_攻撃用();
	if(chara == NULL) {
		chara = 装備者_防御用();
	}
	if(chara != NULL) {
		cValiableField val;
		val.charas.dim(変数_対象者) = chara;
		val.drops.dim(変数_対象落ち物) = me();
		sg_pDungeonSystem->CutInM().CutIn(chara, 能力発動条件満たしている_攻撃用_タイミング, val);
		if(val.doubles.exist(変数_汎用ブール))
		{
			return val.doubles.val(変数_汎用ブール);
		}
	}

	return 装備されている_攻撃用();
}
bool cDroping::能力発動条件満たしている_防御用()
{
	pcCharacter chara = 装備者_攻撃用();
	if(chara == NULL) {
		chara = 装備者_防御用();
	}
	if(chara != NULL) {
		cValiableField val;
		val.charas.dim(変数_対象者) = chara;
		val.drops.dim(変数_対象落ち物) = me();
		sg_pDungeonSystem->CutInM().CutIn(chara, 能力発動条件満たしている_防御用_タイミング, val);
		if(val.doubles.exist(変数_汎用ブール))
		{
			return val.doubles.val(変数_汎用ブール);
		}
	}

	return 装備されている_防御用();
}
bool cDroping::装備マーク攻撃()
{
	pcCharacter chara = 装備者_攻撃用();
	if(chara == NULL) {
		chara = 装備者_防御用();
	}
	if(chara != NULL) {
		cValiableField val;
		val.charas.dim(変数_対象者) = chara;
		val.drops.dim(変数_対象落ち物) = me();
		sg_pDungeonSystem->CutInM().CutIn(chara, 装備マーク攻撃_タイミング, val);
		if(val.doubles.exist(変数_汎用ブール))
		{
			return val.doubles.val(変数_汎用ブール);
		}
	}

	return 装備されている_攻撃用();
}
bool cDroping::装備マーク防御()
{
	pcCharacter chara = 装備者_攻撃用();
	if(chara == NULL) {
		chara = 装備者_防御用();
	}
	if(chara != NULL) {
		cValiableField val;
		val.charas.dim(変数_対象者) = chara;
		val.drops.dim(変数_対象落ち物) = me();
		sg_pDungeonSystem->CutInM().CutIn(chara, 装備マーク防御_タイミング, val);
		if(val.doubles.exist(変数_汎用ブール))
		{
			return val.doubles.val(変数_汎用ブール);
		}
	}

	return 装備されている_防御用();
}
void cDroping::CutIn(タイミング timing, cValiableField& valiable)
{
	if(timing == 被攻撃接近直後_タイミング)
	{

		if(valiable.intsets[変数_属性].count(攻撃属性::アイテム破壊))
		{
			if(属性.count(落ち物属性::アイテム) && 被破壊可() && 落ちてる())
			{
				sg_pDungeonSystem->落ち物破壊要請(me(),true);
			}
		}
	
	}

	if(timing == 移動フェイズ終了_タイミング)
	{
		if(踏みフラグ_)
		{
			踏みフラグ_ = 0;
			乗る();
		}
		跳ね返し無効フラグ = 0;
	}
}

void cDroping::longExplanationExtraBase(std::vector<StyleString> &explanations)
{
	//cDroping::longExplanationExtraBase(explanations);
	
	if(能力スロット())
	{
		explanations.push_back(StyleString(_T("")));
		
		explanations.back() += g_Lang(_T("御霊Expresstions"));
		explanations.back() += 能力説明Caption();
		explanations.back() += _T("\n");
		explanations.back() += 能力説明();
	}
	#ifdef _UNRELEASE
	//アンリリースモードのみ
	explanations.push_back(StyleString(_T("")));

	explanations.back() += _T("出現率係数(レア度)");
	explanations.back() += _T(" ");
	explanations.back() += レア度();
	explanations.back() += _T("\n");

	explanations.back() += _T("値段基礎値");
	explanations.back() += _T(" ");
	explanations.back() += 値段基礎値();
	explanations.back() += _T("\n");

	int i;
	int size = 効果量基礎_定義サイズ();
	for(i=0;i<size;i++)
	{
		explanations.back() += _T("効果量基礎");
		explanations.back() += i;
		explanations.back() += _T(" ");
		explanations.back() += 効果量基礎(i);
		explanations.back() += _T("\n");
	}
	explanations.back() += _T("効果量状態倍率");
	explanations.back() += _T(" ");
	explanations.back() += 効果量状態倍率();
	explanations.back() += _T("\n");

	explanations.back() += _T("効果量係数");
	explanations.back() += _T(" ");
	explanations.back() += 効果量係数();
	explanations.back() += _T("\n");

	explanations.back() += _T("命中力基礎 ");
	explanations.back() += 命中力基礎();
	explanations.back() += _T("\n");

	explanations.back() += _T("命中力 ");
	explanations.back() += 命中力();
	explanations.back() += _T("\n");

	#endif
}
double cDroping::初期初期残りスロット()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(ID(), (tstring)_T("初期残りスロット"), 5);
}

double cDroping::初期残りスロット()
{
	
	cValiableField val;
	val.doubles.dim(変数_汎用ボーナス_定数) = 0.0;
	val.doubles.dim(変数_汎用不変定数) = 初期初期残りスロット();
	sg_pDungeonSystem->CutInM().CutIn(me(), 初期残りスロット計算時_タイミング, val);
	
	return val.doubles.dim(変数_汎用不変定数) + val.doubles.dim(変数_汎用ボーナス_定数);
}

double cDroping::残りスロット()
{
	int slot = 初期残りスロット();
	int i;
	int size = includedItem.size();
	for(i=0;i<size;i++)
	{
		slot -= includedItem[i]->能力スロット();
	}
	return slot;
}


double cDroping::能力スロット()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(ID(), (tstring)_T("能力スロット"), 0);
}
bool cDroping::御霊状態()
{
	if(Parent() != NULL)
	{
		return Parent()->属性.count(落ち物属性::装備品);
	}

	return false;

}
pcDroping cDroping::拠り代()
{
	if(御霊状態())
	{
		return Parent();
	}
	else
	{
		return me();
	}
}
bool cDroping::特殊能力あり()
{
	return 属性.count(落ち物属性::装備品) && !includedItem.empty();

}
bool cDroping::恒常能力()
{
	return (bool)sg_pDungeonSystem->DataBase.DropImportData_Value(ID(), (tstring)_T("恒常能力"), 0);
}
void cDroping::能力(const タイミング timing, cValiableField& valiable)
{

}
tstring cDroping::能力名基礎()
{
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.DropImportLanguage(ID(),
		(tstring)_T("能力名基礎"));
	if(psr == NULL) return _T("Nothing");
	return psr->topscript();
}
tstring cDroping::能力略名基礎()
{
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.DropImportLanguage(ID(),
		(tstring)_T("能力略名基礎"));
	if(psr == NULL) return _T("Nothing");
	return psr->topscript();
}
StyleString cDroping::能力名Full()
{
	StyleString sstring;
	map<tstring, StyleString> valiable;
	
	valiable[_T("Ability")] = 能力名基礎();

	g_Lang(_T("Equip能力名Full"), sstring, valiable);
	return sstring;
}
StyleString cDroping::能力略名Full()
{
	StyleString sstring;
	map<tstring, StyleString> valiable;
	
	valiable[_T("Ability")] = 能力略名基礎();

	g_Lang(_T("Equip能力略名Full"), sstring, valiable);

	if(御霊状態()) return sstring;
	else return _T("(_") + sstring + _T("_)");
}
StyleString cDroping::能力説明Caption()
{
	if(能力スロット() <= 0) return _T("");
	StyleString sstr;
	map<tstring, StyleString> valiable;
	valiable[_T("Slot")] = setStyle((int)能力スロット());
	valiable[_T("SlotExpression")] = 能力スロット表現();
	valiable[_T("Ability")] = 能力名Full();
	valiable[_T("Icon")] = 能力略名Full();

	int i, size = 効果量基礎_定義サイズ();
	for(i=0;i<size;i++)
	{
		能力効果量推定テキスト(i,sstr);
		valiable[(_T("Value")+setStyle(i)).c_str()] = sstr;
	}
	sstr.clear();
	g_Lang(_T("Equip能力説明Caption"), sstr, valiable);


	if(御霊状態())
	{
		sstr.setSameColor(ITEM_MITAMA_COLOR);
	}
	else
	{
		sstr.setSameColor(0xFFAAFFFF);
	}
	return sstr;
}
StyleString cDroping::能力スロット表現()
{
	return g_Lang(_T("Equip能力スロット表現"));
}
StyleString cDroping::能力説明()
{
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.DropImportLanguage(ID(),
		(tstring)_T("能力説明"));
	if(psr == NULL) return NoDataString();
	
	map<tstring,StyleString> valiable;
	StyleString sstr;

	int i, size = 効果量基礎_定義サイズ();
	for(i=0;i<size;i++)
	{
		能力効果量推定テキスト(i,sstr);
		valiable[(_T("Value")+setStyle(i)).c_str()] = sstr;
	}

	sstr.clear();
	Drop_StringAddStyle(psr,sstr,valiable);
	return sstr;
}
//----------------------------------------------------
//cDropingNothing
//----------------------------------------------------

StyleString cDropingNothing::FullName()
{
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.DropImportLanguage(ID(),
		(tstring)_T("FullNameBase"));
	if(psr == NULL) return _T("Nothing");
	return psr->topscript();
}

StyleString cDropingNothing::ShortName()
{
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.DropImportLanguage(ID(),
		(tstring)_T("ShortNameBase"));
	if(psr == NULL) return _T("Nothing");
	return psr->topscript();
}
