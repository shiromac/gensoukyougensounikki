#include "stdafx.h"

#include "cBullet.h"
#include "FireCommand.h"
#include "EffectFunctions.h"


#include "Language.h"

#include "cDungeonSystem.h"

#define DEF_ATTACKICON_INDEXSIZE 8.0


#define RUNOUTGOOD_POSSIBILITY (0.2)

cBullet::cBullet(void)
{

}

cBullet::~cBullet(void)
{
}

//消去処理の準備
void cBullet::Release()
{
	unequip();	

}

StyleString cBullet::識別未識別混合道具名()
{
	if(大局識別済み())
		return FullNameBase();
	else
	{
		if(NamingCaption() == _T(""))
			return sg_pDungeonSystem->DataBase.未識別名(ID());
		else
			return g_Lang(_T("弾:")) + NamingCaption();
	}
}

void cBullet::Init(cRenderDevice *pDev,pcDroping self)
{
	cItem::Init(pDev,self);
	m_pTexture_AttackIcon = g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev, AttackIconFileName().c_str());
}

int cBullet::値段補正値()
{
	return 値段基礎値()*(quantity());
}
/*
cDiscreteProbability cBullet::stateBeginDP(int difficulty)
{
	cDiscreteProbability DP;
	DP.set(10,cDroping::STATE_NORMAL);
	DP.set(1,cDroping::STATE_GOOD);
	return DP;
}
*/

/*
cDiscreteProbability cBullet::qualityBeginDP(int difficulty)
{
	cDiscreteProbability DP;

	int min = 0;
	int max = 19;
	int i;
	for(i=min;i<=max;i++)
	{
		DP.set(1,i);
	}
	return DP;
}
*/

#define DEF_BulletAttributeExpresstions(str) \
if(AAt.count(攻撃属性::str)){\
explanations.back() += g_Lang(_T(#str) _T("Expresstions"));}


void cBullet::longExplanationExtraBase(std::vector<StyleString> &explanations)
{
	cItem::longExplanationExtraBase(explanations);

	explanations.push_back(StyleString(_T("")));

	explanations.back() += g_Lang(_T("BulletAttributeExpresstions"));

	multiset<攻撃属性::攻撃属性> AAt = 攻撃属性();
	DEF_BulletAttributeExpresstions(爆発)
	DEF_BulletAttributeExpresstions(火)
	DEF_BulletAttributeExpresstions(冷気)
	DEF_BulletAttributeExpresstions(水)
	DEF_BulletAttributeExpresstions(電気)
	DEF_BulletAttributeExpresstions(気)
	DEF_BulletAttributeExpresstions(毒)

	#ifdef _UNRELEASE
	//アンリリースモードのみ
	explanations.push_back(StyleString(_T("")));

	explanations.back() += _T("加算攻撃力 ");
	explanations.back() += 加算攻撃力();
	explanations.back() += _T("\n");

	explanations.back() += _T("乗算攻撃力 ");
	explanations.back() += 乗算攻撃力();
	explanations.back() += _T("\n");


	explanations.back() += _T("属性 :");
	multiset<攻撃属性::攻撃属性> attr = 攻撃属性();
	multiset<攻撃属性::攻撃属性>::iterator itr = attr.begin();
	for(;itr != attr.end();itr++)
	{
		explanations.back() += _T(" ");
		explanations.back() += sg_pDungeonSystem->DataBase.attackattrI2S(*itr);
	}
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

void cBullet::DrawStateIconSub(cRenderDevice *pDev,int x,int y)
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

		DO.m_TexRange.setLTRB(0.5,0.25,0.75,0.5);
		DO.Draw(pDev);
	}

		
}


void cBullet::aspectedAttackDraw(cRenderDevice *pDev)
{
	cDrawingObject DO;
	GetAttackIcon(DO);



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
		
	DO.Draw(pDev);
}
int cBullet::GetAttackIcon(cDrawingObject& DO)
{
	
	DO.setTexture(m_pTexture_AttackIcon,AttackIconFileSize(),AttackIconFileSize());
	DO.m_TexRange.setLTRB(AttackIconFileIndexX()/DEF_ATTACKICON_INDEXSIZE, visibleaspect/DEF_ATTACKICON_INDEXSIZE,
					(AttackIconFileIndexX()+1)/DEF_ATTACKICON_INDEXSIZE, (visibleaspect+1)/DEF_ATTACKICON_INDEXSIZE);

	return true;
}


StyleString cBullet::QualityCaption()
{
	//------------------------------------
	//修正値
	if(quantity() == 1 )
	{
		//return _T("");
		return _T("×") + setStyle(quantity());
	}
	else if(quantity() > 1)
	{//正
		return _T("×") + setStyle(quantity());
	}
	else
	{//ありえないけど
		return setStyle(quality());
	}
}
double cBullet::効果量修正値倍率()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(ID(), (tstring)_T("効果量修正値倍率"), デフォルト効果量修正値倍率());
}
double cBullet::デフォルト効果量修正値倍率()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		(tstring)_T("Bullet基本値"),(tstring)_T("デフォルト効果量修正値倍率"),0.05);
}
double cBullet::架空攻撃力_定数()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		(tstring)_T("Bullet基本値"),(tstring)_T("架空攻撃力"),1.0, 0);
}
double cBullet::架空攻撃力_フロア係数()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		(tstring)_T("Bullet基本値"),(tstring)_T("架空攻撃力"),1.0, 1);
}
double cBullet::架空攻撃力_ダメージキャップ()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		(tstring)_T("Bullet基本値"),(tstring)_T("架空攻撃力_ダメージキャップ"),1.0, 0);
}
double cBullet::デフォルト効果量状態倍率_祝福()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		(tstring)_T("Bullet基本値"),(tstring)_T("デフォルト効果量状態倍率_祝福"),3.0);
}

double cBullet::撃ち時神力切れ確率()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(ID(), (tstring)_T("撃ち時神力切れ確率"), デフォルト撃ち時神力切れ確率());
}
double cBullet::デフォルト撃ち時神力切れ確率()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		(tstring)_T("Bullet基本値"),(tstring)_T("デフォルト撃ち時神力切れ確率"),RUNOUTGOOD_POSSIBILITY);
}

int cBullet::GetmenuCaption(vector<tstring> &CaptionList)
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
			CaptionList.push_back(_T("拾う"));

			if(sg_pDungeonSystem->pPlayerChara()->holdItem.size() >= sg_pDungeonSystem->pPlayerChara()->MaxholdNum())
			{
				CaptionList.push_back(_T("交換"));
			}
		}
	}
	else
	{
		if(装備されている())
		{
			CaptionList.push_back(_T("はずす"));
		}
		else
		{
			if(!内包されている())
				CaptionList.push_back(_T("装備"));

		}
	}

	CaptionList.push_back(_T("撃つ"));


	if(Holder() == NULL)
	{
		CaptionList.push_back(_T("投げる"));
	}
	else
	{
	
		//if(装備されている())
		//{
			
		//}
		//else
		{

		
			CaptionList.push_back(_T("投げる"));

			if(Holder()->足元() == NULL)
			{
				CaptionList.push_back(_T("置く"));
			}
			else
			{
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
/*
//コマンドを解決する;
int cBullet::FireCommand(cRenderDevice *pDev, tstring verb, vector<pcDroping> &ObjectList)
{

	if(verb == _T("撃つ"))
	{
		撃つ(pDev);
	}


	if(verb == _T("装備"))
	{
		装備(pDev);
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
int cBullet::撃つ(cRenderDevice *pDev)
{
	int result = 0;


	if(state() == cDroping::STATE_NORMAL)
	{
		result = 撃つ_メッセージ_通常(sg_pDungeonSystem->pPlayerChara());
	}
	else if(state() == cDroping::STATE_CURSE)
	{
		result = 撃つ_メッセージ_呪い(sg_pDungeonSystem->pPlayerChara());
	}
	else if(state() == cDroping::STATE_GOOD)
	{
		result = 撃つ_メッセージ_祝福(sg_pDungeonSystem->pPlayerChara());
	}


	result = 撃ち要請(sg_pDungeonSystem->pPlayerChara());


	if(result)
	{//使用に成功した
		sg_pDungeonSystem->メニューを閉じる();
		sg_pDungeonSystem->ターンエンド();
	}

	return true;
}
int cBullet::撃ち要請(pcCharacter pchara)
{

	int result = 0;
	if(state() == cDroping::STATE_NORMAL)
	{
		result = 撃つ_効果_通常(pchara);
	}
	else if(state() == cDroping::STATE_CURSE)
	{
		result = 撃つ_効果_呪い(pchara);
	}
	else if(state() == cDroping::STATE_GOOD)
	{
		result = 撃つ_効果_祝福(pchara);
	}

	if(result)
	{//使用に成功した
		
		if(quantity() <= 0)
		{
			sg_pDungeonSystem->落ち物破壊要請(me());
		}
		else
		{
			if(state() == cDroping::STATE_GOOD)
			{
				stategoodRunoutprocess();
			}
		}
	}

	return result;
}
*/

int cBullet::撃つ_効果_通常(pcCharacter pchara)
{
	//コピー
	pcDroping newpdrop = sg_pDungeonSystem->落ち物生成_設置なし(ID(),dpack.integer);

	sg_pDungeonSystem->アイテム店フラグ(newpdrop,cDroping::SHOPFLAG_NONE);//売り物でない
	sg_pDungeonSystem->アイテム割引フラグ(newpdrop, cDroping::SHOPDISCOUNTFLAG_NONE);
	//一本
	newpdrop->quality() = 0;



	本数消費();


	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("shoot.wav"),現在地形()->place);
	溜めエフェクト(pchara);

	int option;
	if(ひま投げ())
	{
		if(ひま投げ() == 2)
			option = THROW_HIMANAGE;
		else
			option = THROW_HIMANAGESTRAIGHT | THROW_HIMANAGE;
	}
	else
	{
		option = DRAWTYPE_ATTACKDRAW;
	}
	sg_pDungeonSystem->投擲要請(pchara,newpdrop,option,射程距離());
	return true;
}

int cBullet::本数消費()
{
	

	vector<pcDroping> vpdrop;
	if(Parent() != NULL)
	{//箱の中
		vpdrop = Parent()->includedItem;
	}
	else if(Holder() != NULL)
	{//箱の中
		vpdrop = Holder()->holdItem;
	}
	if(!vpdrop.empty())
	{
		pcDroping pdrop;//最後の同一ID//少なくとも自分
		int i;
		for(i=0;i<vpdrop.size();i++)
		{
			if(vpdrop[i]->ID() == ID() &&
				vpdrop[i]->state() == state())
			{
				pdrop = vpdrop[i];
			}
		}

		pdrop->quality()--;

		if(pdrop->quality() < 0)
		{
			sg_pDungeonSystem->落ち物破壊要請(pdrop);
		}
	}
	else
	{
		//一本減る
		quality()--;

		if(quantity() <= 0)
		{
			sg_pDungeonSystem->落ち物破壊要請(me());
		}
	}
	return true;
}

int cBullet::撃つ_メッセージ_通常(pcCharacter pchara)
{
	return 撃つメッセージ定型(pchara);
}

int cBullet::撃つ_効果_呪い(pcCharacter pchara)
{
	return 撃つ_効果_通常(pchara);
}
int cBullet::撃つ_メッセージ_呪い(pcCharacter pchara)
{
	return 撃つメッセージ定型(pchara);
}

int cBullet::撃つ_効果_祝福(pcCharacter pchara)
{
	return 撃つ_効果_通常(pchara);
}
int cBullet::撃つ_メッセージ_祝福(pcCharacter pchara)
{
	return 撃つ_メッセージ_通常(pchara);
}


int cBullet::撃つメッセージ定型(pcCharacter pchara)
{
	//sg_pDungeonSystem->AnimationManager().
	//	Anime_PlaySE(_T("drink.wav"),pchara->足元地形()->place);
	
	if(被拾得可())
	{
		map<tstring, StyleString> valiable;
		valiable[_T("Item")] = FullName();
		g_Langメッセージ(_T("Bullet撃ちメッセージ"),valiable);
	}
	else
	{
		map<tstring, StyleString> valiable;
		valiable[_T("Item")] = FullName();
		g_Langメッセージ(_T("アイテム拾う失敗拾得不可メッセージ"),valiable);
	}
		
	//sg_pDungeonSystem->メッセージ
	//		(FullName()+_T(" の中から 一つを撃った。\n"));
	return 被拾得可();
}


int cBullet::衝突(pcCharacter pchara)
{
	int result = 0;

	衝突時_メッセージ(pchara);


	result = 衝突時効果(pchara);


	if(result)
	{//使用に成功した
		sg_pDungeonSystem->落ち物破壊要請(me());
	}
	
	return result;
}

int cBullet::ダメージ効果(pcCharacter pchara, pcLandform pland, int power)
{

	//sg_pDungeonSystem->AnimationManager().
	//	Anime_PlaySE(_T("flame.wav"),pchara->足元地形()->place);
	//ここにエフェクト

	pcAttackinformation attackInfo = 攻撃作成(
			pchara,//攻撃者
			pland,//場所
			power,//攻撃力
			攻撃属性_定数ダメージ(),//定数ダメージ
			攻撃属性(),
			true);

	if(投擲者() == NULL)
	{
		//だいたい罠
		attackInfo->valiable.doubles.dim(変数_ダメージキャップ) = 架空攻撃力_ダメージキャップ();
	}

	sg_pDungeonSystem->攻撃接近(
			attackInfo
		);


	return true;
}
multiset<攻撃属性::攻撃属性> cBullet::攻撃属性()
{
	multiset<攻撃属性::攻撃属性> temp;
	sg_pDungeonSystem->DataBase.def_decode_Attackattri(sg_pDungeonSystem->DataBase.DropImportData(ID(),(tstring)_T("攻撃属性")), temp);
	temp.insert(攻撃属性::落ち物);
	return temp;
}
int cBullet::攻撃属性_定数ダメージ()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(ID(),(tstring)_T("攻撃属性_定数ダメージ"), 0);
}

int cBullet::威力計算()
{
	double charaattackpower = 0;
	if(投擲者() == NULL)
	{
		charaattackpower = 架空攻撃力_定数() + sg_pDungeonSystem->FloorLevel() * 架空攻撃力_フロア係数();
	}
	else
	{
		charaattackpower = 投擲者()->GetBulletAttackPower();
	}

	int power;
	if(state() == cDroping::STATE_CURSE)
	{
		power = (charaattackpower*乗算攻撃力()
			+ 加算攻撃力())*0.5*効果量係数();//半分
	}
	else
	{
		power = (charaattackpower*乗算攻撃力()
			+ 加算攻撃力())*効果量状態倍率()*効果量係数();
	}


	cValiableField valf;
	valf.doubles.dim(変数_攻撃力ボーナス_倍率) = 0;
	valf.doubles.dim(変数_攻撃力ボーナス_定数) = 0;
	sg_pDungeonSystem->CutInM().CutIn(投擲者(),射撃攻撃力計算時_タイミング, valf);


	return power * (1 + valf.doubles.val(変数_攻撃力ボーナス_倍率)) + valf.doubles.val(変数_攻撃力ボーナス_定数);
}
int cBullet::衝突時効果(pcCharacter pchara)
{
	return ダメージ効果(投擲者()
		, sg_pDungeonSystem->キャラ足元地形(pchara)//地形
		, 威力計算()//威力
		);
}
int cBullet::衝突時効果land(pcLandform pland)
{
	return ダメージ効果(投擲者()
		, pland//地形
		, 威力計算()//威力
		);
}

int cBullet::加算攻撃力()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		ID(),(tstring)_T("加算攻撃力"),1);
}
double cBullet::乗算攻撃力()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		ID(),(tstring)_T("乗算攻撃力"),1);
}
int cBullet::射程距離()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		ID(),(tstring)_T("射程距離"),10);
}
int cBullet::ひま投げ()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		ID(),(tstring)_T("ひま投げ"),0);
}
bool cBullet::撃ち返し爆弾()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		ID(),(tstring)_T("撃ち返し爆弾"),0);
}

int cBullet::衝突時_メッセージ(pcCharacter pchara)
{
	/*
	map<tstring, StyleString> valiable;
	valiable[_T("Item")] = FullName();
	g_Langメッセージ(_T("Bullet衝突時メッセージ"),valiable);
		
	//sg_pDungeonSystem->メッセージ((FullName()+_T(" が当たった。\n")));
	*/
	return true;
}

double cBullet::能力スロット()
{
	int maxslot = cItem::能力スロット();


	return ceil( maxslot*効果量係数()*能力効果量倍率());
}
double cBullet::能力効果量倍率()
{
	return 1/max(0.5,1+(まとめ最大数()-1)*効果量修正値倍率());
}




int cBullet::まとめ要請(pcDroping pdrop)
{
	pcBullet pcBlt = boost::dynamic_pointer_cast<cBullet>(pdrop);
	if(pcBlt == NULL) return false;
	if(ID() == pdrop->ID()
		&& me() != pdrop
		&& me()->state() == pdrop->state()
		&& !(pcBlt->撃ち返し爆弾() && pcBlt->bombSet())
		&& pdrop->shopitemflag() == shopitemflag())
	{//まとめられる
		return まとめる(pdrop);
	}
	else
	{
		return false;
	}
}
int cBullet::まとめる(pcDroping pdrop)
{
	int maxnum = まとめ最大数();

	if(quality() +1 == maxnum)
	{
		return false;
	}

	quality() += pdrop->quality()+1;


	int st = 0;

	if(state() == cDroping::STATE_CURSE) st--;
	if(pdrop->state() == cDroping::STATE_CURSE) st--;

	if(state() == cDroping::STATE_GOOD) st++;
	if(pdrop->state() == cDroping::STATE_GOOD) st++;

	if(st < 0) state() = cDroping::STATE_CURSE;
	else if(st > 1) state() = cDroping::STATE_GOOD;
	else state() = cDroping::STATE_NORMAL;



	if(quality()+1 <= maxnum)
	{
		sg_pDungeonSystem->落ち物破壊要請(pdrop);

		if(Holder() != NULL && Holder() == sg_pDungeonSystem->pPlayerChara())
		{
			map<tstring, StyleString> valiable;
			valiable[_T("ItemA")] = pdrop->FullName();
			valiable[_T("ItemB")] = FullName();
			g_Langメッセージ(_T("Bulletまとめメッセージ"),valiable);
			//sg_pDungeonSystem->メッセージ( pdrop->FullName() + _T("を拾って、 ") + FullName() + _T("にまとめた。\n"));
		}
	}
	else
	{
		int amari = quality()+1 - maxnum;
		quality() = maxnum-1;

	


		if(Holder() != NULL && Holder() == sg_pDungeonSystem->pPlayerChara())
		{
			map<tstring, StyleString> valiable;
			valiable[_T("ItemA")] = pdrop->FullName();
			valiable[_T("ItemB")] = FullName();
			g_Langメッセージ(_T("Bulletまとめ2メッセージ"),valiable);
			//sg_pDungeonSystem->メッセージ( pdrop->FullName() + _T("を拾って、 ") + FullName() + _T("にまとめた。\n"));
		}
		pdrop->quality() = amari-1;

		return false;
	}


			

	return true;
}
int cBullet::まとめ最大数()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		ID(),(tstring)_T("まとめ最大数"),20);
}

bool cBullet::stategoodRunoutprocess()
{
	if(RUNOUTGOOD_POSSIBILITY > random())
	{
		map<tstring, StyleString> valiable;
		valiable[_T("Item")] = FullName();
		g_Langメッセージ(_T("Bullet神力切れメッセージ"),valiable);

		//sg_pDungeonSystem->メッセージ(
		//	FullName() + _T(" の神力がなくなった！\n"));
		state() = cDroping::STATE_NORMAL;	

		return true;
	}

	return false;
}


/*
//-----------------------------------------------
//装備系
//-----------------------------------------------
int cBullet::装備(cRenderDevice *pDev)
{
	int result = 0;


	if(state() == cDroping::STATE_NORMAL)
	{
		result = 装備_メッセージ_通常(sg_pDungeonSystem->pPlayerChara());
	}
	else if(state() == cDroping::STATE_CURSE)
	{
		result = 装備_メッセージ_呪い(sg_pDungeonSystem->pPlayerChara());
	}
	else if(state() == cDroping::STATE_GOOD)
	{
		result = 装備_メッセージ_祝福(sg_pDungeonSystem->pPlayerChara());
	}


	result = 装備要請(sg_pDungeonSystem->pPlayerChara());


	if(result)
	{//使用に成功した
		sg_pDungeonSystem->メニューを閉じる();
		//sg_pDungeonSystem->ターンエンド();
	}

	return true;
}
int cBullet::装備要請(pcCharacter pchara)
{
	if(!装備可())
	{//装備できない
		return false;
	}


	int result = 0;
	if(state() == cDroping::STATE_NORMAL)
	{
		result = 装備_効果_通常(pchara);
	}
	else if(state() == cDroping::STATE_CURSE)
	{
		result = 装備_効果_呪い(pchara);
	}
	else if(state() == cDroping::STATE_GOOD)
	{
		result = 装備_効果_祝福(pchara);
	}

	if(result)
	{//装備に成功した
		
	}

	return result;
}
*/

int cBullet::装備_効果_通常(pcCharacter pchara)
{
	int result = 1;

	if(pchara->bulletequipment == NULLDROP)
	{//装備されて無い
		equip(pchara);//EQUIP_PART_ATTACKは意味なし
	}
	else
	{//装備されてる
		pcBullet pcBul;
		pcBul = boost::dynamic_pointer_cast<cBullet>(pchara->bulletequipment);
		if(pcBul != NULL)
		{
			result = Bullet装備はずし要請(pchara, pchara->bulletequipment);
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
	
	equip(pchara);


	装備メッセージ定型(pchara);
	return true;
}
int cBullet::装備_メッセージ_通常(pcCharacter pchara)
{
	
	return true;
}

int cBullet::装備_効果_呪い(pcCharacter pchara)
{
	return 装備_効果_通常(pchara);
}
int cBullet::装備_メッセージ_呪い(pcCharacter pchara)
{
	return 装備_メッセージ_通常(pchara);
}

int cBullet::装備_効果_祝福(pcCharacter pchara)
{
	return 装備_効果_通常(pchara);
}
int cBullet::装備_メッセージ_祝福(pcCharacter pchara)
{
	return 装備_メッセージ_通常(pchara);
}


int cBullet::装備メッセージ定型(pcCharacter pchara)
{
	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("equip.wav"),pchara->足元地形()->place);
		
	map<tstring, StyleString> valiable;
	valiable[_T("Item")] = FullName();
	g_Langメッセージ(_T("Bullet装備メッセージ"),valiable);


	//sg_pDungeonSystem->メッセージ
	//		(FullName()+_T(" を装備した。\n"));

	return true;
}

/*
int cBullet::はずす(cRenderDevice *pDev)
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


int cBullet::装備はずし要請(pcCharacter pchara)
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

int cBullet::はずす_効果_通常(pcCharacter pchara)
{
	unequip();



	return true;
}
int cBullet::はずす_メッセージ_通常(pcCharacter pchara)
{
	return はずすメッセージ定型(pchara);;
}

int cBullet::はずす_効果_呪い(pcCharacter pchara)
{
	return はずす_効果_通常(pchara);
}
int cBullet::はずす_メッセージ_呪い(pcCharacter pchara)
{
	return はずす_メッセージ_通常(pchara);
}

int cBullet::はずす_効果_祝福(pcCharacter pchara)
{
	return はずす_効果_通常(pchara);
}
int cBullet::はずす_メッセージ_祝福(pcCharacter pchara)
{
	return はずす_メッセージ_通常(pchara);
}


int cBullet::はずすメッセージ定型(pcCharacter pchara)
{
	//sg_pDungeonSystem->AnimationManager().
	//	Anime_PlaySE(_T("drink.wav"),pchara->足元地形()->place);
	
	map<tstring, StyleString> valiable;
	valiable[_T("Item")] = FullName();
	g_Langメッセージ(_T("Bullet装備はずすメッセージ"),valiable);

	//sg_pDungeonSystem->メッセージ
	//		(FullName()+_T(" をはずした。\n"));
	return true;
}

void cBullet::効果量推定テキスト(int index, StyleString& output)
{
	double qualityPower = 1.0;
	double statePower = 1.0;
	//if(修正値識別済み())
	//	qualityPower = 効果量係数();//効果量状態倍率を除く
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
		output.addstring((int)(効果量基礎(index)*qualityPower*statePower));
		return;
	}
}

//強制的に装備
int cBullet::equip(pcCharacter pchara)
{
	
	if(pchara->bulletequipment == NULLDROP)
	{
		pchara->bulletequipment = me();
		weak_pEquiper = wpcCharacter(pchara);
	}
	else
	{//別の装備されてる。
		return false;
	}


	return true;
}
//強制的に装備解除
int cBullet::unequip()
{

	if(装備されている())
	{

		装備者()->bulletequipment = NULLDROP;
		weak_pEquiper = wpcCharacter(NULLCHARA);

	}
	

	return true;
}
void cBullet::CutIn(タイミング timing, cValiableField& valiable)
{
	cItem::CutIn(timing,valiable);

	if(撃ち返し爆弾())
	{
		if(bombSet() && !broken)
		{
			if(timing == 被攻撃接近直後_タイミング)
			{
				爆弾爆発();
			}
		}
		else
		{
			if(timing == 被衝撃_強_タイミング 
				//&& sg_pDungeonSystem->落ち物設置可(me(),現在地形())
				&& 現在地形()->through(0,0,0)
				)
			{
				bombSet() = 1;
			}
		}
	}
}
int cBullet::上を通過(pcDroping pdrop)//pdropに何か起こったらtrue;
{
	if(撃ち返し爆弾() && bombSet() && !broken)
	{
		爆弾爆発();
	}
	return false;
}
int cBullet::上を通過(pcCharacter pchara)//pdropに何か起こったらtrue;
{
	if(撃ち返し爆弾() && bombSet() && !broken)
	{
		if(sg_pDungeonSystem->キャラクター敵対判定(投擲者(),pchara))
		{
			//衝突時効果(pchara);
			爆弾爆発();	
			return pchara->死亡();
		}
	}
	return false;
}
int cBullet::上に落ちる(pcDroping pdrop)
{
	if(撃ち返し爆弾() && bombSet() && !broken)
	{
		爆弾爆発();
	}
	return false;
}
int cBullet::上に落ちる(pcCharacter pchara)
{
	if(撃ち返し爆弾() && bombSet() && !broken)
	{
		if(sg_pDungeonSystem->キャラクター敵対判定(投擲者(),pchara))
		{
			//衝突時効果(pchara);
			爆弾爆発();	
			return pchara->死亡();
		}
	}
	return false;
}
int cBullet::乗る()
{
	pcCharacter pchara = 乗りキャラ();
	if(pchara != NULL)
	{
		if(撃ち返し爆弾() && bombSet() && !broken)
		{
			if(sg_pDungeonSystem->キャラクター敵対判定(投擲者(),pchara))
			{
				//衝突時効果(pchara);
				爆弾爆発();
				return pchara->死亡();
			}
		}
	}

	return cItem::乗る();
}
void cBullet::能力効果量推定テキスト(int index, StyleString& output)
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
		if(御霊状態()) output.addstring((int)ceil((効果量(index)*能力効果量倍率())));
		else output.addstring((int)ceil((効果量(index)*能力効果量倍率()/効果量状態倍率())));
		
		return;
	}
}
bool cBullet::爆弾爆発()
{
	
	if(!bombed())
	{
		bombed() = 1;
		EffectFunctions::爆発エフェクト_中(me()->placeX,me()->placeY,330);
	

		vector<pcLandform> vpland;
		vpland.push_back(現在地形());

		int a;
		for(a=0;a<8;a++)
		{
			vpland.push_back(sg_pDungeonSystem->隣接地形(現在地形(), a, 1));
		}

		int i;
		for(i=0;i<vpland.size();i++)
		{
			if(vpland[i]->pOnChar != NULL) 衝突時効果(vpland[i]->pOnChar);
			else 衝突時効果land(vpland[i]);
		}

		sg_pDungeonSystem->落ち物破壊要請(me());
		return true;
	}
	return false;
	
}


//エフェクト
int cBullet::溜めエフェクト(pcCharacter pchara)
{
	pcaECustom caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->place.set(pchara->placeX,pchara->placeY,0,0);
	
	//レイヤー１
	caec->LayerLevel = LAYERLEVEL_EXTRAEFFECT1;
	
	caEchip_Basic caec_model;

	//テクスチャセット
	caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
		sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ring_in.png")),SIZE_BASICTEX,SIZE_BASICTEX);

	//テクスチャ座標セット
	caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


	//描画基本位置
	cCoordinate coo;
	coo.SetAspect(pchara->aspect);
	//c4DVector center(pchara->placeX +coo.x*0.2,pchara->placeY +coo.y*0.2,0.25,0);
	//center = sg_pDungeonSystem->Map().realvisibleplace(center);
	//描画位置
	//caec_model.Centerbegin = center;
	//caec_model.Centerend = center;
	caec_model.AbsolutePlaceFlag = false;
	caec_model.place.set(pchara->placeX +coo.x*0.2,pchara->placeY +coo.y*0.2,0.25,0);



	//基本サイズ
	caec_model.DO.Width = 128;
	caec_model.DO.Height = 128;


	
	
	//シェイプオプション
	caec_model.ScaleX.set(1,0,1,0.5);
	caec_model.ScaleY.set(1,0,1,0.5);
	//caec_model.Rotation.set(0,0,0,0);
	//caec_model.Rotation2.set(0,0,0,0);



	//表示時間
	caec_model.life = 4;
	caec_model.hidingTime = 0;	
	
	//加算
	caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

	//色
	caec_model.ColorBase.ARGB(255,128,255,255);
	caec_model.Alpha.set(0,2,2,0);


	
	//チップに追加
	pcAnimationChipManager_chip pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
	caec->addAnimechip(pcac);



	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));



	//-----------------------------------------------
	//衝撃
	//-----------------------------------------------
	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->place.set(pchara->placeX,pchara->placeY,0,0);

	//レイヤー１
	caec->LayerLevel = LAYERLEVEL_EXTRAEFFECT1;

	//テクスチャセット
	caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
		sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ring_radio.png")),SIZE_BASICTEX,SIZE_BASICTEX);

	//テクスチャ座標セット
	caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


	//描画基本位置
	coo.SetAspect(pchara->aspect);
	//center.set(pchara->placeX +coo.x*0.2,pchara->placeY +coo.y*0.2,0.25,0);
	//center = sg_pDungeonSystem->Map().realvisibleplace(center);
	//描画位置
	//caec_model.Centerbegin = center;
	//caec_model.Centerend = center;
	caec_model.AbsolutePlaceFlag = false;
	caec_model.place.set(pchara->placeX +coo.x*0.2,pchara->placeY +coo.y*0.2,0.25,0);



	//基本サイズ
	caec_model.DO.Width = 256;
	caec_model.DO.Height = 64;


	//描画位置
	//caec_model.Centerbegin = center;
	//caec_model.Centerend = center;

	
	
	//シェイプオプション
	caec_model.ScaleX.set(0.1,1,0.4,1);
	caec_model.ScaleY.set(0.1,1,0.4,1);
	//caec_model.Rotation.set(0,0,1,1);
	caec_model.Rotation2.set(pchara->aspect*45,pchara->aspect*45,1,1);



	//表示時間
	caec_model.life = 8;
	caec_model.hidingTime = 0;	
	
	//加算
	caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

	//色
	caec_model.ColorBase.ARGB(255,128,255,255);
	caec_model.Alpha.set(0,2,2,0);


	
	//チップに追加
	pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
	caec->addAnimechip(pcac);



	return sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));


}