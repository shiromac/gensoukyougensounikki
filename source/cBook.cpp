#include "stdafx.h"

#include "cBook.h"


#include "Language.h"


#include "gameMainSystem/filemanage/cScriptRLayer.h"
#include "scriptprocesser/DropsScript.h"

#define DEF_ATTACKICON_INDEXSIZE 8.0


#define RUNOUTGOOD_POSSIBILITY (0.4)

cBook::cBook(void)
{

}

cBook::~cBook(void)
{
}

void cBook::Init(IDirect3DDevice9 *pDev,pcDroping self)
{
	cItem::Init(pDev,self);
	m_pTexture_AttackIcon = g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev, AttackIconFileName().c_str());
}

/*
int cBook::修正値識別済み()
{
	return cItem::修正値識別済み() || (usecountminus() != 0);
}
*/


StyleString cBook::識別未識別混合道具名()
{
	if(大局識別済み())
		return FullNameBase();
	else
	{
		if(NamingCaption() == _T(""))
			return sg_pDungeonSystem->DataBase.未識別名(ID());
		else
			return g_Lang(_T("本:")) + NamingCaption();
	}
}
	

/*
cDiscreteProbability cBook::stateBeginDP(int difficulty)
{
	cDiscreteProbability DP;
	DP.set(10,cDroping::STATE_NORMAL);
	DP.set(1,cDroping::STATE_GOOD);
	return DP;
}
*/
/*
cDiscreteProbability cBook::qualityBeginDP(int difficulty)
{
	cDiscreteProbability DP;
	DP.set(1,3);
	DP.set(2,4);
	DP.set(2,5);
	DP.set(1,6);
	return DP;
}
*/

void cBook::aspectedAttackDraw(IDirect3DDevice9 *pDev)
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
	DO.colorblendmode = cDrawableObject::COLOR_BLEND_ADDITION;//加算
	DO.m_color.inputD3Dcolor(imagecolor());
	DO.m_color.green *= 0.5*DO.m_color.green/255;
	DO.m_color.red *= 0.5*DO.m_color.red/255;
	DO.m_color.blue *= 0.5*DO.m_color.blue/255;
	DO.m_color.alpha = opaque*DO.m_color.alpha;
		
	DO.Draw(pDev);
}
int cBook::GetAttackIcon(cDrawingObject& DO)
{
	
	DO.setTexture(m_pTexture_AttackIcon,AttackIconFileSize(),AttackIconFileSize());
	DO.m_TexRange.setLTRB(AttackIconFileIndexX()/DEF_ATTACKICON_INDEXSIZE, visibleaspect/DEF_ATTACKICON_INDEXSIZE,
					(AttackIconFileIndexX()+1)/DEF_ATTACKICON_INDEXSIZE, (visibleaspect+1)/DEF_ATTACKICON_INDEXSIZE);

	return true;
}


StyleString cBook::QualityCaption()
{
	//------------------------------------
	//修正値

	return _T("[") + setStyle(quality()) + _T("]");

}
StyleString cBook::ExtraHatString()
{
	StyleString ehs = cItem::ExtraHatString();

	if(cannotread())
	{
		ehs += g_Lang(_T("ページ開かずCaption"));
	}
	return ehs;
}


#define DEF_BookAttributeExpresstions魔法難度(str) \
if(魔法難度() == str){\
explanations.back() += g_Lang(_T("魔法難度")_T(#str)_T("Expresstions"));}


void cBook::longExplanationExtraBase(std::vector<StyleString> &explanations)
{
	cItem::longExplanationExtraBase(explanations);

	explanations.push_back(StyleString(_T("")));

	explanations.back() += g_Lang(_T("BookAttributeExpresstions"));

	DEF_BookAttributeExpresstions魔法難度(1)
	DEF_BookAttributeExpresstions魔法難度(2)
	DEF_BookAttributeExpresstions魔法難度(3)
	DEF_BookAttributeExpresstions魔法難度(4)
	DEF_BookAttributeExpresstions魔法難度(5)

	#ifdef _UNRELEASE
	//アンリリースモードのみ
	explanations.push_back(StyleString(_T("")));

	explanations.back() += _T("読み時神力切れ確率 ");
	explanations.back() += 読み時神力切れ確率();
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


double cBook::読み時神力切れ確率()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		ID(),(tstring)_T("読み時神力切れ確率"),デフォルト読み時神力切れ確率());
}
double cBook::デフォルト読み時神力切れ確率()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		(tstring)_T("Book基本値"),(tstring)_T("デフォルト読み時神力切れ確率"),RUNOUTGOOD_POSSIBILITY);
}
double cBook::水時濡れ確率()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		(tstring)_T("Book基本値"),(tstring)_T("水時濡れ確率"),0.1);
}
StyleString cBook::OptionCaption()
{
	if(!修正値識別済み() && usecountminus())
	{
		return _T("(") + setStyle(usecountminus()) + _T(")");
	}
	else
	{
		return _T("");
	}
}


int cBook::GetmenuCaption(vector<tstring> &CaptionList)
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

	CaptionList.push_back(_T("読む"));


	CaptionList.push_back(_T("投げる"));


	
	if(Holder() != NULL && Holder()->足元() == NULL)
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
int cBook::魔法難度()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(ID(), (tstring)_T("魔法難度"), 1);
}

void cBook::CutIn(タイミング timing, cValiableField& valiable)
{
	cItem::CutIn(timing,valiable);

	if(timing == 被攻撃接近直後_タイミング)
	{
		if(valiable.intsets.val(変数_属性).count(攻撃属性::水)
			&& random() < 水時濡れ確率() && !属性.count(落ち物属性::耐濡れ)
			&& !cannotread())
		{
			cValiableField newvaliable;
			newvaliable.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
			sg_pDungeonSystem->CutInM().CutIn(Holder(),本濡れ直前_タイミング,newvaliable);
			if(newvaliable.doubles.val(変数_汎用ブール))
			{
				map<tstring, StyleString> val;
				val[_T("Item")] = FullName();
				g_Langメッセージ(_T("Book水付与メッセージ"),val);
				cannotread() = 1;
			}
		}
	}
	else if(timing == フロア開始_タイミング)
	{
		cannotread() = 0;
	}
}

int cBook::異常状態解除()
{
	if (cannotread()) {
		map<tstring, StyleString> val;
		val[_T("Item")] = FullName();
		g_Langメッセージ(_T("Book水解消メッセージ"),val);
		cannotread() = 0;
		return true;
	}
	return false;
}
/*
//コマンドを解決する;
int cBook::FireCommand(IDirect3DDevice9 *pDev, tstring verb, vector<pcDroping> &ObjectList)
{

	if(verb == _T("読む"))
	{
		読む(pDev);
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
int cBook::読む(IDirect3DDevice9 *pDev)
{
	int result = 0;


	if(state() == cDroping::STATE_NORMAL)
	{
		result = 読む_メッセージ_通常(sg_pDungeonSystem->pPlayerChara());
	}
	else if(state() == cDroping::STATE_CURSE)
	{
		result = 読む_メッセージ_呪い(sg_pDungeonSystem->pPlayerChara());
	}
	else if(state() == cDroping::STATE_GOOD)
	{
		result = 読む_メッセージ_祝福(sg_pDungeonSystem->pPlayerChara());
	}

	if(result)
	{//使用に成功した
		sg_pDungeonSystem->メニューを閉じる();
		sg_pDungeonSystem->ターンエンド();
	


		if(quality() > 0)
		{//残量あり
			溜めエフェクト(sg_pDungeonSystem->pPlayerChara());
			result = 読み要請(sg_pDungeonSystem->pPlayerChara());
		}
		else
		{//残量なし
			//効果なし
			弾切れエフェクト(sg_pDungeonSystem->pPlayerChara());
			
			if(!修正値識別済み())
			{
				sg_pDungeonSystem->修正のみ識別(me(),false);
			}
			result = true;
		}

	}

	return true;
}
int cBook::読み要請(pcCharacter pchara)
{

	int result = 0;
	if(state() == cDroping::STATE_NORMAL)
	{
		result = 読む_効果_通常(pchara);
	}
	else if(state() == cDroping::STATE_CURSE)
	{
		result = 読む_効果_呪い(pchara);
	}
	else if(state() == cDroping::STATE_GOOD)
	{
		result = 読む_効果_祝福(pchara);
	}

	if(result)
	{//使用に成功した
		

		if(state() == cDroping::STATE_GOOD)
		{
			stategoodRunoutprocess();
		}

	}

	return result;
}
*/

int cBook::読む_効果_通常(pcCharacter pchara)
{
	if(cannotread())
	{
		return false;
	}
	pcDroping newpdrop = sg_pDungeonSystem->落ち物生成_設置なし(ID(),dpack.integer);

	//ひとつ
	newpdrop->quality() = 0;

	//ひとつ減る
	if(state() != cDroping::STATE_GOOD)
	{
		quality()--;
		usecountminus()--;
	}
	sg_pDungeonSystem->魔法発射要請(pchara,newpdrop);
	sg_pDungeonSystem->状態のみ識別(me(),0);
	return true;
}
int cBook::読む_メッセージ_通常(pcCharacter pchara)
{

	return 読むメッセージ定型(pchara);

}

int cBook::読む_効果_呪い(pcCharacter pchara)
{
	//効果なし
	return false;
}
int cBook::読む_メッセージ_呪い(pcCharacter pchara)
{
	sg_pDungeonSystem->メッセージ
			(g_Lang(_T("Book呪いによる読み不可メッセージ")));
	sg_pDungeonSystem->状態のみ識別(me(),false);
	return false;
}

int cBook::読む_効果_祝福(pcCharacter pchara)
{
	return 読む_効果_通常(pchara);
}
int cBook::読む_メッセージ_祝福(pcCharacter pchara)
{
	return 読む_メッセージ_通常(pchara);
}


int cBook::読むメッセージ定型(pcCharacter pchara)
{
	if(cannotread())
	{
		sg_pDungeonSystem->メッセージ(g_Lang(_T("Book濡れによる読み不可メッセージ")));
		return false;
	}

	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("cast.wav"),pchara->足元地形()->place);
	
	map<tstring, StyleString> valiable;
	valiable[_T("Item")] = FullName();
	g_Langメッセージ(_T("Book読むメッセージ"),valiable);
	//sg_pDungeonSystem->メッセージ
	//		(FullName()+g_Lang(_T("Book読むメッセージ")));
	return true;
}

int cBook::衝突(pcCharacter pchara)
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

int cBook::効果(pcCharacter pchara, pcDroping pdrop, vector<pcLandform> vpland, int aspect,int flag)
{

	return true;
}
multiset<攻撃属性::攻撃属性> cBook::攻撃属性()
{
	multiset<攻撃属性::攻撃属性> temp;
	sg_pDungeonSystem->DataBase.def_decode_Attackattri(sg_pDungeonSystem->DataBase.DropImportData(ID(),(tstring)_T("攻撃属性")), temp);
	return temp;
}


int cBook::衝突時効果(pcCharacter pchara)
{
	vector<pcLandform> vpland;
	return 効果(pchara,
		NULLDROP,
		vpland,
		aspect
		);


}



int cBook::衝突時_メッセージ(pcCharacter pchara)
{

	//sg_pDungeonSystem->メッセージ((FullName()+_T(" が当たった。\n")));

	return true;
}



bool cBook::stategoodRunoutprocess()
{
	if(読み時神力切れ確率() > random())
	{
		map<tstring, StyleString> valiable;
		valiable[_T("Item")] = FullName();
		g_Langメッセージ(_T("Book神力切れメッセージ"),valiable);
		
		state() = cDroping::STATE_NORMAL;	

		return true;
	}

	return false;
}




//エフェクト
int cBook::溜めエフェクト(pcCharacter pchara)
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



	
	//加算
	caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

	//色
	caec_model.ColorBase.inputD3Dcolor(imagecolor());
	caec_model.Alpha.set(0,2,2,0);


	
	//表示時間
	caec_model.life = 10;
	caec_model.hidingTime = 0;	

	//チップに追加
	pcAnimationChipManager_chip pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
	caec->addAnimechip(pcac);


	caec_model.hidingTime = 6;	

	//チップに追加
	pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
	caec->addAnimechip(pcac);


	caec_model.hidingTime = 12;	

	//チップに追加
	pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
	caec->addAnimechip(pcac);

	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));



	//-----------------------------------------------
	//魔方陣
	//-----------------------------------------------
	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->place.set(pchara->placeX,pchara->placeY,0,0);

	//レイヤー2
	caec->LayerLevel = LAYERLEVEL_EXTRAEFFECT2;

	//テクスチャセット
	caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
		sg_pDungeonSystem->pDevice_D3D,_T("effect\\magiccircle.png")),512,512);

	//テクスチャ座標セット
	caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


	//描画基本位置
	coo.SetAspect(pchara->aspect);
	//center.set(pchara->placeX ,pchara->placeY ,0,0);
	//center = sg_pDungeonSystem->Map().realvisibleplace(center);
	//描画位置
	//caec_model.Centerbegin = center;
	//caec_model.Centerend = center;
	caec_model.AbsolutePlaceFlag = false;
	caec_model.place.set(pchara->placeX +coo.x*0.2,pchara->placeY +coo.y*0.2,0.25,0);



	//基本サイズ
	caec_model.DO.Width = 196;
	caec_model.DO.Height = 196;



	
	
	//シェイプオプション
	caec_model.ScaleX.set(1,1,1,1);
	caec_model.ScaleY.set(0.75,0.75,1,1);
	caec_model.Rotation.set(100,180,1,1);
	//caec_model.Rotation2.set(100,180,1,1);



	//表示時間
	caec_model.life = 30;
	caec_model.hidingTime = 5;	
	
	//加算
	caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

	//色
	caec_model.ColorBase.inputD3Dcolor(imagecolor());
	caec_model.Alpha.set(0,1,1,0);


	
	//チップに追加
	pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
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
		sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ring_wind.png")),SIZE_BASICTEX,SIZE_BASICTEX);


	
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
	caec_model.DO.Width = 128;
	caec_model.DO.Height = 128;



	
	
	//シェイプオプション
	caec_model.ScaleX.set(0.2,1,2,1);
	caec_model.ScaleY.set(0.1,0.5,1,0.5);
	caec_model.Rotation.set(0,360,10,1);
	caec_model.Rotation2.set(pchara->aspect*45,pchara->aspect*45,1,1);



	//表示時間
	caec_model.life = 20;
	caec_model.hidingTime = 0;	
	
	//加算
	caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

	//色
	caec_model.ColorBase.inputD3Dcolor(imagecolor());
	caec_model.Alpha.set(0,2,2,0);


	
	//チップに追加
	pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
	caec->addAnimechip(pcac);

	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("shoot.wav"),pchara->足元地形()->place);


	return sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));


}

int cBook::反射エフェクト(c4DVector place, int aspect)
{
	//c4DVector center;
	cCoordinate coo;
	caEchip_Basic caec_model;
	pcAnimationChipManager_chip pcac;

	pcaECustom caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->place = place;
	
	//レイヤー１
	caec->LayerLevel = LAYERLEVEL_EXTRAEFFECT1;

	//アンカーとして空アニメを追加
	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));



	//-----------------------------------------------
	//衝撃
	//-----------------------------------------------
	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->place = place;

	//レイヤー１
	caec->LayerLevel = LAYERLEVEL_EXTRAEFFECT1;

	//テクスチャセット
	caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
		sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ring_out.png")),SIZE_BASICTEX,SIZE_BASICTEX);

	//テクスチャ座標セット
	caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


	//描画基本位置
	coo.SetAspect(aspect);
	//center.set(place.x,place.y,0.25,0);
	//center = sg_pDungeonSystem->Map().visibleplace(center);
	//描画位置
	//caec_model.Centerbegin = center;
	//caec_model.Centerend = center;
	caec_model.AbsolutePlaceFlag = false;
	caec_model.place.set(place.x,place.y,0.25,0);



	//基本サイズ
	caec_model.DO.Width = 128;
	caec_model.DO.Height = 128;



	
	
	//シェイプオプション
	caec_model.ScaleX.set(0,1,1,1);
	caec_model.ScaleY.set(0,0.5,0.5,0.5);
	//caec_model.Rotation.set(0,180,10,1);
	caec_model.Rotation2.set(aspect*45,aspect*45,1,1);



	//表示時間
	caec_model.life = 10;
	caec_model.hidingTime = 0;	
	
	//加算
	caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

	//色
	caec_model.ColorBase.inputD3Dcolor(imagecolor());
	caec_model.Alpha.set(0,2,2,0);


	
	//チップに追加
	pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
	caec->addAnimechip(pcac);



	return sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));


}
int cBook::弾切れエフェクト(pcCharacter pchara)
{


	pcaECustom caec;
	cCoordinate coo;
	c4DVector center;
	pcAnimationChipManager_chip pcac;

	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->place.set(pchara->placeX,pchara->placeY,0,0);
	
	//レイヤー２
	caec->LayerLevel = LAYERLEVEL_EXTRAEFFECT2;
	
	caEchip_Basic caec_model;

	//テクスチャセット
	caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
		sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ball_smoke.png")),SIZE_BASICTEX,SIZE_BASICTEX);

	//テクスチャ座標セット
	caec_model.DO.m_TexRange.setLTRB(0,0,1,1);


	//描画基本位置
	coo.SetAspect(pchara->aspect);
	//center.set(pchara->placeX +coo.x*0.2,pchara->placeY +coo.y*0.2,0.25,0);
	//center = sg_pDungeonSystem->Map().visibleplace(center);

	caec_model.AbsolutePlaceFlag = 0; caec_model.place.set(pchara->placeX +coo.x*0.2,pchara->placeY +coo.y*0.2,0.25,0);




	//基本サイズ
	caec_model.DO.Width = 128;
	caec_model.DO.Height = 128;


	//描画位置
	caec_model.Centerbegin = center;
	caec_model.Centerend = center;

	caec_model.Centerend.y -= 32;
	
	
	//シェイプオプション
	caec_model.ScaleX.set(0,2,2,0);
	caec_model.ScaleY.set(0,2,2,0);
	//caec_model.Rotation.set(0,0,0,0);
	caec_model.Rotation2.set(rand()*360/RAND_MAX,rand()*360/RAND_MAX,1,0.5);



	
	//加算
	caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;

	//色
	caec_model.ColorBase.inputD3Dcolor(0xFFFFFFFF);
	caec_model.Alpha.set(0,2,1,0);


	
	//表示時間
	caec_model.life = 20;
	caec_model.hidingTime = 0;	

	//チップに追加
	pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
	caec->addAnimechip(pcac);


	caec_model.hidingTime = 4;
	caec_model.Rotation2.set(rand()*360/RAND_MAX,rand()*360/RAND_MAX,1,0.5);
	caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

	//チップに追加
	pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
	caec->addAnimechip(pcac);


	caec_model.hidingTime = 8;
	caec_model.Rotation2.set(rand()*360/RAND_MAX,rand()*360/RAND_MAX,1,0.5);
	caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;

	//チップに追加
	pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
	caec->addAnimechip(pcac);

	return sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));

}


//イメージカラー
unsigned long cBook::imagecolorConcrete()
{
	pcScriptRLayer psr;
	psr = sg_pDungeonSystem->DataBase.DropImportData(ID(),
		(tstring)_T("imagecolorConcrete"));
	if(psr == NULL) return 0xFFFFFFFF;
	TCHAR** end = NULL;
	return _tcstoul(psr->topscript().c_str(), end, 16);
}

unsigned long cBook::imagecolor()
{
	if(大局識別済み()) return imagecolorConcrete();
	else return 0xFFAADDFF;
}