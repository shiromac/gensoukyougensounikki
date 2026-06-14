#include "stdafx.h"

#include "cTrap.h"

#include "cDungeonSystem.h"

#include "GameIdiom.h"

#include "Language.h"

#include "AnimeChips.h"

#include "scriptprocesser/AnimationScript.h"

#define ITEM_CAPTION_MAX_LENGTH (40.0)


cTrap::cTrap(void)
{
	Fired = 0;
}

cTrap::~cTrap(void)
{
}

StyleString cTrap::FullName()
{
	StyleString FullN = 識別未識別混合道具名();

	FullN += QualityCaption();


	FullN.setSameColor(TRAP_COLOR);


	if(FullN.length() > ITEM_CAPTION_MAX_LENGTH)
	{
		D3DXVECTOR2 size(ITEM_CAPTION_MAX_LENGTH/FullN.length(),1);
		FullN.setSameSize(size);
	}

	return FullN;
}
/*
//レア度(出現確率基礎値)
double cTrap::レア度()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		ID(),(tstring)_T("レア度"),RARITY_F);
}
*/
//発動率(%)
double cTrap::発動率()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		ID(),(tstring)_T("発動率"),0);
}
double cTrap::発動率_発見済み()
{
	double pos = sg_pDungeonSystem->DataBase.DropImportData_Value(
		ID(),(tstring)_T("発動率"),0);
	if(stepcount() <= 1) pos *= 0;
	else if(stepcount() <= 2) pos *= 0.25;
	else pos *= 0.25;
	//else pos *= 0.5;

	return pos;
}

//故障率(%)
double cTrap::故障率()
{
	return sg_pDungeonSystem->DataBase.DropImportData_Value(
		ID(),(tstring)_T("故障率"),0);
}



void cTrap::DataBeginOptimize(int difficulty)
{
	state() = stateBeginDP(difficulty).get(random());
	quality() = qualityBeginDP(difficulty).get(random());
	
	disappear();//隠す

	AppreciationBeginOptimize(difficulty);
}
void cTrap::AppreciationBeginOptimize(int difficulty)
{
	allAppreciate();
}
void cTrap::visibleReset()
{
	cDroping::visibleReset();
	if(setflag())
	{
		setExtraMaterialColor(255,255,128,128); 
	}
}


cDiscreteProbability cTrap::stateBeginDP(int difficulty)
{
	cDiscreteProbability DP;
	DP.set(1,cDroping::STATE_NORMAL);
	return DP;
}

cDiscreteProbability cTrap::qualityBeginDP(int difficulty)
{
	cDiscreteProbability DP;
	DP.set(1,0);
	return DP;
}


StyleString cTrap::QualityCaption()
{
	//------------------------------------
	//修正値
	if(quality() > 0)	
	{//正
		return _T("+") + setStyle(quality());
	}
	else if(quality() < 0)
	{//負
		return setStyle(quality());
	}
	else
	{
		return _T("");
	}
}

StyleString cTrap::FullNameSub()
{
	return setStyle(_T("No FullNameSub cItem"),0xFFFF0000);
}

StyleString cTrap::ShortName()
{
	return setStyle(_T("No ShortName cItem"),0xFFFF0000);
}

StyleString cTrap::ShortNameSub()
{
	return setStyle(_T("No ShortNameSub cItem"),0xFFFF0000);
}

int cTrap::OnMapping()
{
	if(sg_pDungeonSystem->pPlayerChara()->Condition.目薬状態() || !罠用_効果目標か(sg_pDungeonSystem->pPlayerChara()))
	{
		return discover();
	}
	return false;
}

bool cTrap::被拾得可()
{
	return !罠用_効果目標か(sg_pDungeonSystem->pPlayerChara()) && !setflag();
}

int cTrap::GetmenuCaption(vector<tstring> &CaptionList)
{

	if(!罠用_効果目標か(sg_pDungeonSystem->pPlayerChara()))
	{
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
	
	}

	if(倉庫の中())
	{
		CaptionList.push_back(_T("説明"));
		CaptionList.push_back(_T("倉庫から出す"));
		return true;
	}

	if(落ちてる()) {
		CaptionList.push_back(_T("踏む"));
	}

	if(!罠用_効果目標か(sg_pDungeonSystem->pPlayerChara()))
	{
		CaptionList.push_back(_T("投げる"));
	}

	if(Holder() != NULL && Holder()->足元() == NULL)
	{
		CaptionList.push_back(_T("置く"));
	}

	if(Holder() != NULL && Holder()->足元() != NULL
		&& !罠用_効果目標か(sg_pDungeonSystem->pPlayerChara()))
	{//落ちてる
		if(!内包されている()) CaptionList.push_back(_T("交換"));
	}

	CaptionList.push_back(_T("説明"));


	return true;
}

/*
//コマンドを解決する;
int cTrap::FireCommand(IDirect3DDevice9 *pDev, tstring verb, vector<pcDroping> &ObjectList)
{

	if(verb == _T("踏む"))
	{
		踏む(pDev);
	}



	if(verb == _T("説明")) 説明(pDev);


	return true;
}


int cTrap::踏む(IDirect3DDevice9 *pDev)
{
	return 踏み要請();
}
int cTrap::踏み要請()
{
	int result = 発動();

	sg_pDungeonSystem->メニューを閉じる();
	sg_pDungeonSystem->ターンエンド();

	return result;
}
*/
bool cTrap::罠用_効果目標か(pcCharacter pchara)
{
	wpcCharacter wchara = wpcCharacter(pchara);
	if(罠用_効果目標か_一時記憶_.find(wchara) == 罠用_効果目標か_一時記憶_.end()){
		bool isTarget = (pchara == sg_pDungeonSystem->pPlayerChara());
		cValiableField valiable;
		valiable.doubles.dim(変数_汎用ブール) = isTarget;
		valiable.charas.dim(変数_対象者) = pchara;
		valiable.drops.dim(変数_対象落ち物) = me();
		sg_pDungeonSystem->CutInM().CutIn(sg_pDungeonSystem->pPlayerChara(),罠対象キャラ判定時_タイミング, valiable);
		sg_pDungeonSystem->CutInM().CutIn(pchara,罠対象キャラ判定時_タイミング, valiable);
		return 罠用_効果目標か_一時記憶_[wchara] = valiable.doubles.val(変数_汎用ブール);
	}
	else {
		return 罠用_効果目標か_一時記憶_[wchara];
	}
}


int cTrap::乗る()
{
	pcCharacter onChara = 乗りキャラ();
	if(!罠用_効果目標か(onChara))
	{
		if(onChara == sg_pDungeonSystem->pPlayerChara())
		{
			cDroping::乗る();
		}
		return false;
	}
	else
	{
		int isdiscovered = isDiscover();

		if(onChara == sg_pDungeonSystem->pPlayerChara())
		{
			sg_pDungeonSystem->アイテム強制マッピング(me());
			sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(&(opaque), 1);
		}

		//踏みエフェクト();

		//踏みカウント増加
		stepcount()++;

		map<tstring, StyleString> valiable;
		valiable[_T("Item")] = FullName();
		g_Langメッセージ(_T("Trap踏みメッセージ"),valiable);
		//sg_pDungeonSystem->メッセージ(FullName() +_T("__を_踏んでしまった！\n"));

		cValiableField valf;
		valf.doubles.dim(変数_汎用実数) = 1.0;
		sg_pDungeonSystem->CutInM().CutIn(乗りキャラ(),罠発動率計算時_タイミング,valf);
		double posp = valf.doubles[変数_汎用実数];

		if(!sg_pDungeonSystem->キャラクター敵対判定(sg_pDungeonSystem->pPlayerChara(), onChara))
		{
			if(!isdiscovered && random()*100 <= 発動率()*posp)
			{
				return 発動要請();
			}
			else if(random()*100 <= 発動率_発見済み()*posp)
			{
				return 発動要請();
			}
			else
			{
				g_Langメッセージ(_T("Trap踏み不発メッセージ"),valiable);
				//sg_pDungeonSystem->メッセージ(_T("しかし、 ")+ FullName() +_T(" は発動しなかった。\n"));
				return false;
			}
		}
		else
		{
			return 発動要請();
		}
	}
}

int cTrap::上に落ちる(pcDroping pdrop)//pdropを破壊したいときtrue;
{
	sg_pDungeonSystem->アイテム強制マッピング(me());//発見
	sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(&(opaque), 1);


	return 発動要請(pdrop);
}
int cTrap::上に落ちる(pcCharacter pchara)//charaに何か起こったらtrue;
{
	sg_pDungeonSystem->アイテム強制マッピング(me());
	sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(&(opaque), 1);
		

	//踏みエフェクト();

	//踏みカウント増加
	stepcount()++;

	return 発動要請();
}
int cTrap::発動要請()
{
	/*
	cValiableField val;
	val.doubles.dim(変数_汎用ブール) = 0.0;
	sg_pDungeonSystem->CutInM().CutIn(me(),罠発動専用_タイミング,val);
	return val.doubles.val(変数_汎用ブール);
	*/
	if(!Fired)
	{
		//ここでカチッて音がする
		踏みエフェクト();

		map<tstring, StyleString> valiable;
		valiable[_T("Item")] = FullName();
		g_Langメッセージ(_T("Trap発動メッセージ"),valiable);
		//sg_pDungeonSystem->メッセージ(FullName() + _T(" が発動した！\n"));

		return 発動(乗りキャラ());
	}
	else
	{
		return false;
	}
}
int cTrap::発動要請(pcCharacter pchara)
{
	if(!Fired)
		return 発動(pchara);
	else
		return false;
}
int cTrap::発動要請(pcDroping pdrop)
{
	/*
	cValiableField val;
	val.doubles.dim(変数_汎用ブール) = 0.0;
	sg_pDungeonSystem->CutInM().CutIn(me(),罠発動専用_タイミング,val);
	return val.doubles.val(変数_汎用ブール);
	*/
	if(!Fired)
		return 発動(pdrop);
	else
		return false;
}

int cTrap::発動(pcCharacter pchara)
{
	sg_pDungeonSystem->アイテム強制マッピング(me());
	Fired = 1;
	int result = 0;
	if(pchara == NULL)
	{
		result = 効果();
	}
	else
	{
		result = 効果(pchara);
	}

	if(random()*100 <= 故障率())
	{
		故障();
		map<tstring, StyleString> valiable;
		valiable[_T("Item")] = FullName();
		g_Langメッセージ(_T("Trap故障メッセージ"),valiable);
		//sg_pDungeonSystem->メッセージ(FullName() + _T(" は壊れた。\n"));
	}

	
	return result;
}
int cTrap::発動(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	sg_pDungeonSystem->アイテム強制マッピング(me());
	Fired = 1;
	int result = 効果(pdrop);

	if(random()*100 <= 故障率())
	{
		map<tstring, StyleString> valiable;
		valiable[_T("Item")] = FullName();
		g_Langメッセージ(_T("Trap故障メッセージ"),valiable);
		//sg_pDungeonSystem->メッセージ(FullName() + _T(" は壊れた。\n"));
		故障();
	}

	
	return result;
}
int cTrap::衝突(pcCharacter pchara)
{

	multiset<攻撃属性::攻撃属性> 属性;

	if(投擲者() == NULL)
	{
		投擲者_weakpointer = wpcCharacter(NULLCHARA);
	}


	発動(pchara);
	
	sg_pDungeonSystem->落ち物破壊要請(me(),1);

	return true;
}
int cTrap::再設置()
{

	setflag() = true;
	return true;
}
int cTrap::効果()
{
	sg_pDungeonSystem->メッセージ((StyleString)_T("cTrap::効果　効果が設定されていません。\n"));

	return false;
}
int cTrap::効果(pcCharacter pchara)
{
	sg_pDungeonSystem->メッセージ((StyleString)_T("cTrap::効果　効果が設定されていません。\n"));

	return false;
}
int cTrap::効果(pcDroping pdrop)//pdropを破壊したかったらtrue;
{
	sg_pDungeonSystem->メッセージ((StyleString)_T("cTrap::効果　効果が設定されていません。\n"));

	return false;
}

int cTrap::故障()
{
	sg_pDungeonSystem->落ち物破壊要請(me());
	return true;
}

void cTrap::CutIn(タイミング timing, cValiableField& valiable)
{
	cDroping::CutIn(timing,valiable);

	/*
	if(timing == 罠発動専用_タイミング)
	{
		valiable.doubles.val(変数_汎用ブール) = 発動();
	}
	*/

	/*
	if(timing == アクション終了_タイミング)
	{//一挙動に一回しか発動しないちょっとおもい
		Fired = 0;
	}
	*/
	if(timing == 攻撃フェイズ終了_タイミング)
	{//一フェイズに一回しか発動しない
		Fired = 0;
	}
}
void cTrap::踏みエフェクト()
{
	/*
	pcaECustom caec;
	caEchip_Basic caec_model;
	c4DVector center;
	pcAnimationChipManager_chip pcac;

	cCoordinate coo(placeX,placeY);
	sg_pDungeonSystem->AnimationManager().
		Anime_PlaySE(_T("trap.wav"),coo);
		*/

	pcCharacter pchara = 乗りキャラ();
	if(Holder() != NULL)
	{
		pchara = Holder();
	}
	cValiableField val;
	double temp;
	if(pchara != NULL)
	{
		val.doubles.dim(tstring(_T("PlaceX")),temp = pchara->placeX);
		val.doubles.dim(tstring(_T("PlaceY")),temp = pchara->placeY);
	}
	else
	{
		val.doubles.dim(tstring(_T("PlaceX")),temp = placeX);
		val.doubles.dim(tstring(_T("PlaceY")),temp = placeY);
	}
	val.charas.dim(tstring(_T("sufferer")),pchara);

	int outputaspect = -360;
	if(pchara != NULL)
	{
		outputaspect = pchara->aspect * 360 / 8;
	}
	val.doubles.dim(tstring(_T("direction")),temp = outputaspect*45);


	AnimationScript_AddAnimation(
		sg_pDungeonSystem->DataBase.EffectImport_Trap(),
		_T("EffectTrapBase.lua"),
		val);

	bool didAnime = AnimationScript_AddAnimation(
		sg_pDungeonSystem->DataBase.EffectImport_Trap(),
		sg_pDungeonSystem->DataBase.EffectImport_Name_Trap(ID()),
		val);	


	//テストコード
	/*
	int life = 15;
	pAnimeChip_Perticle chip = AnimeChip_Perticle::new_shared_ptr(life);

	chip->position.addCascade(PositionPerGrid(placeX,placeY,0),life);
	chip->setTexture(_T("effect\\basic\\ring_out.png"));
	chip->scaleX.addCascade(0,	1,	life);
	chip->scaleY.addCascade(0,	0.75,life);
	chip->setPerticleSize(D3DXVECTOR2(512,512));
	chip->color.addCascade(cColor(0,255,255,255),cColor(255,255,255,255),cColor(0,255,255,255),life);
	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(chip));

	pAnimeChip_Chara chara = AnimeChip_Chara::new_shared_ptr(乗りキャラ(),life*8);
	chara->scaleX.addCascade(1,3,1,life);
	chara->scaleY.addCascade(1,life);
	chara->scaleY.addCascade(1,3,1,life);
	chara->pitching.addCascade(0,life*2);
	chara->pitching.addCascade(0,360,life);
	chara->yawing.addCascade(0,life*3);
	chara->yawing.addCascade(0,360,life);
	chara->position.addCascade(PositionPerGrid(0,0,0),life*4);
	chara->position.addCascade(PositionPerGrid(0,0,0),PositionPerGrid(1,0,0),life);
	chara->position.addCascade(PositionPerGrid(1,0,0),PositionPerGrid(0,1,0),life);
	chara->position.addCascade(PositionPerGrid(0,1,0),PositionPerGrid(0,0,1),life);
	chara->position.addCascade(PositionPerGrid(0,0,1),PositionPerGrid(0,0,0),life);
	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(chara));
	*/

	/*-
	//-----------------------------------------------
	//衝撃
	//-----------------------------------------------
	caec = pcaECustom(new caECustom);
	//描画位置通知
	caec->place.set(placeX,placeY,0,0);

	//レイヤー
	caec->LayerLevel = LAYERLEVEL_NORMAL;

	//テクスチャセット
	caec_model.DO.setTexture(g_GameEnv.m_GlobalResourse->getTextureFromFile(
		sg_pDungeonSystem->pDevice_D3D,_T("effect\\basic\\ring_out.png")),256,256);



	//描画基本位置
	center.set(placeX ,placeY ,0,0);
	center = sg_pDungeonSystem->Map().realvisibleplace(center);




	//基本サイズ
	caec_model.DO.Width = 512;
	caec_model.DO.Height = 512;


	//描画位置
	caec_model.Centerbegin = center;
	caec_model.Centerend = center;

	//シェイプオプション
	caec_model.ScaleX.set(1,1,0,1);
	caec_model.ScaleY.set(0.75,0.75,0,1);
	caec_model.Rotation.set(0,0,1,1);
	//caec_model.Rotation2.set(100,180,1,1);


	//表示時間
	caec_model.life = 15;
	caec_model.hidingTime = 0;	
	
	//加算
	caec_model.DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;

	//色
	caec_model.ColorBase.inputD3Dcolor(0xFFFFFFFF);
	caec_model.Alpha.set(0,1,1,0);
	
	//チップに追加
	pcac = pcAnimationChipManager_chip(new caEchip_Basic(caec_model));
	caec->addAnimechip(pcac);

	sg_pDungeonSystem->AnimationManager().AddAnime(boost::static_pointer_cast<cAnimation>(caec));
	*/
}