#include "stdafx.h"

#include "cCondition.h"

#include "cStateDroping.h"

#include "EffectFunctions.h"

#include "utility/cNumField.h"

#include <boost/pointer_cast.hpp>

//処理の流れ
/*
1.	異常状態に掛かる
		異常状態を付与
			アニメ更新

		（主人公の番）アニメ更新


	残りターンをマイナスする
		異常状態を消去（自然回復）

	手順がくる（動けるタイミング）


2.	異常状態に掛かる
		異常状態を付与
			アニメ更新


	異常状態を解除する
		異常状態を消去
			アニメ更新


*/
#define TURN_NOTBAD -1

#define SPEED_SUPERSLOW -2
#define SPEED_SLOW -1
#define SPEED_NORMAL 0
#define SPEED_DOUBLE 1
#define SPEED_TRIPLE 2

const double cCondition::healthfitnessRecoverPerHP = 0.1f;
const double cCondition::sicknessDamegePerHP = 0.1f;

cCondition::cCondition(void)
{
	/*
	currentspeed = defaultspeed = SPEED_NORMAL;
	slow_count = 0;
	speed_turn_count = 0;
	HP_oddstock = 0;
	stomach_msg_count = STOMACH_MSG_FULL;
	sleep_turn_count = 0;
	heavysleep_turnmemo = 0;
	currentpoison = 0;
	poison_turn_count = 0;
	currentpoison = 0;
	poison_turn_count = 0;
	fuuin_turn_count = 0;
	karaburi_turn_count = 0;
	yarisugosi_turn_count = 0;
	binbou_turn_count = 0;
	koorituke_turn_count = 0;
	sasoi_turn_count = 0;
	migawari_turn_count = 0;
	muteki_turn_count = 0;
	gitai_turn_count = 0;
	*/
	anotherSurfaceID = -1;
	anotherSurfaceIDchange = -1;
}

cCondition::~cCondition(void)
{
}
#define DEF_condition_chipvector_init(type,emotion) \
	p_condition_chipvector_.push_back( &type##Chip );\
	type##Chip.init(type,emotion,pself)
	
#define DEF_condition_chipvector_initNULL(type,emotion) \
	p_condition_chipvector_.push_back( &type##Chip );\
	type##Chip.init(異常状態_初期化無し,emotion,pself)

bool cCondition::init(pcCharacter pself)
{
	weak_pself = wpcCharacter(pself);


	//pself->emotion.clear();

	DEF_condition_chipvector_init(空腹,EMOTION_HUNGER);
	/*
	DEF_condition_chipvector_init(超鈍足,EMOTION_UTSU);
	DEF_condition_chipvector_init(鈍足,EMOTION_UTSU);
	DEF_condition_chipvector_init(等速,EMOTION_NULL);
	DEF_condition_chipvector_init(二倍速,EMOTION_NULL);
	DEF_condition_chipvector_init(三倍速,EMOTION_NULL);
	*/

	DEF_condition_chipvector_initNULL(速度,EMOTION_NULL);

	DEF_condition_chipvector_initNULL(眠り,EMOTION_ZZZ);

	/*
	DEF_condition_chipvector_init(バクスイ,EMOTION_ZZZ);
	DEF_condition_chipvector_init(浅い居眠り,EMOTION_ZZZ);
	DEF_condition_chipvector_init(深い居眠り,EMOTION_ZZZ);
	*/

	DEF_condition_chipvector_init(びっくり,EMOTION_BIKKURI);びっくりChip.setOverWrite(false);
	DEF_condition_chipvector_init(金縛り,EMOTION_BIRIBIRI);金縛りChip.setOverWrite(false);
	DEF_condition_chipvector_init(泥酔,EMOTION_GURUGURU);泥酔Chip.setOverWrite(false);
	DEF_condition_chipvector_init(空振り,EMOTION_KARABURI);空振りChip.setOverWrite(false);
	DEF_condition_chipvector_init(やりすごし,EMOTION_NULL);やりすごしChip.setOverWrite(true);
	DEF_condition_chipvector_init(貧乏,EMOTION_BINBOU);貧乏Chip.setOverWrite(false);
	DEF_condition_chipvector_init(氷付け,EMOTION_KOORITUKE);氷付けChip.setOverWrite(false);
	DEF_condition_chipvector_init(死の誘い,EMOTION_NULL);
	DEF_condition_chipvector_init(みがわり,EMOTION_MIGAWARI);
	DEF_condition_chipvector_init(変身,EMOTION_NULL);
	DEF_condition_chipvector_init(擬態,EMOTION_GITAI);擬態Chip.setOverWrite(true);
	DEF_condition_chipvector_init(狂乱,EMOTION_KYOURAN);狂乱Chip.setOverWrite(false);
	DEF_condition_chipvector_init(嫉妬,EMOTION_SHITTO);嫉妬Chip.setOverWrite(true);
	DEF_condition_chipvector_init(臆病,EMOTION_OKUBYOU);臆病Chip.setOverWrite(false);
	DEF_condition_chipvector_init(鳥目,EMOTION_TORIME);鳥目Chip.setOverWrite(false);
	DEF_condition_chipvector_init(無意識,EMOTION_PUNPUN);無意識Chip.setOverWrite(true);
	DEF_condition_chipvector_init(健康,EMOTION_POWAPOWA);健康Chip.setOverWrite(false);
	DEF_condition_chipvector_init(病気,EMOTION_SEKIMEN);病気Chip.setOverWrite(false);

	DEF_condition_chipvector_initNULL(脱力元気,EMOTION_NULL);
	DEF_condition_chipvector_initNULL(軟弱頑強,EMOTION_NULL);
	
	DEF_condition_chipvector_init(無敵,EMOTION_MUTEKI);無敵Chip.setOverWrite(true);
	DEF_condition_chipvector_init(目薬,EMOTION_NULL);
	DEF_condition_chipvector_init(封印,EMOTION_FUUIN);封印Chip.setOverWrite(false);

	//currentspeed = defaultspeed = SPEED_NORMAL;
	/*
	slow_count = 0;
	speed_turn_count = 0;
	HP_oddstock = 0;
	stomach_msg_count = STOMACH_MSG_FULL;
	sleep_turn_count = 0;
	heavysleep_turnmemo = 0;
	currentpoison = 0;
	poison_turn_count = 0;
	currentpoison = 0;
	poison_turn_count = 0;
	fuuin_turn_count = 0;
	karaburi_turn_count = 0;
	yarisugosi_turn_count = 0;
	binbou_turn_count = 0;
	koorituke_turn_count = 0;
	sasoi_turn_count = 0;
	migawari_turn_count = 0;
	muteki_turn_count = 0;
	gitai_turn_count = 0;
	kyouran_turn_count = 0;
	okubyou_turn_count = 0;
	shitto_turn_count = 0;
	torime_turn_count = 0;
	*/

	/*
	anotherSurfaceID = -1;
	sasoi_turn_count_visible_ = 0;
	sasoi_visual_counter = 0;
	*/
	return true;
}

pcCharacter cCondition::anotherSurface()
{
	return sg_pDungeonSystem->DataBase.GetSampleCharacter(anotherSurfaceID);
}
void cCondition::CutIn(タイミング timing, cValiableField& valiable)
{



	if(timing == 被攻撃直後時_タイミング)
	{
		とても強い刺激();
	}

	if(封印状態())
	{
		std::map<tstring, StyleString > val;
		if(timing == 読み直前_タイミング)
		{

			g_Langメッセージ(_T("封印読み不可メッセージ"),val);
			valiable.doubles[変数_汎用ブール] = false;
			sg_pDungeonSystem->メニューを閉じる();
			
		}
		else if(timing == 宣言直前_タイミング)
		{
			g_Langメッセージ(_T("封印宣言不可メッセージ"),val);
			valiable.doubles[変数_汎用ブール] = false;
			sg_pDungeonSystem->メニューを閉じる();
		}
		else if(timing == スキル直前_タイミング)
		{
			g_Langメッセージ(_T("スキル宣言不可メッセージ"),val);
			valiable.doubles[変数_汎用ブール] = false;
			sg_pDungeonSystem->メニューを閉じる();
		}
	}


	if(空振り状態() && timing == 直接攻撃直前_タイミング)
	{
		valiable.doubles[変数_命中力] = 0;
	}

	if(やりすごし状態() && 
		(timing == 直接攻撃直前_タイミング
		|| timing == 被攻撃直後時_タイミング
		))
		//アイテムを使用しても解除されない|| (timing == アイテム使用直後_タイミング && yarisugosi_turnmemo != yarisugosi_turn_count)))
	{
		やりすごし解除();
	}
	
	if(氷付け状態())
	{
		if(timing == 通常移動直前_タイミング)
		{
			valiable.doubles[変数_汎用ブール] = 0;//移動失敗
			氷付けで移動失敗();
		}
		else if(timing == 被攻撃直後時_タイミング)
		{
			if(valiable.intsets[変数_属性].count(攻撃属性::火))
			{
				氷付け追加(-1);
			}
		
		}
	}

	if(貧乏状態() && timing == 方向転換直前_タイミング)
	{
		valiable.doubles[変数_汎用ブール] = 0;//方向転換失敗
	}

	if(みがわり状態() && timing == 被攻撃優先度計算時_タイミング)
	{
		valiable.doubles[変数_汎用実数] += 100;
	}

	if(擬態状態() && timing == 被攻撃直後時_タイミング)
	{
		擬態追加(-1);
	}

	if(目薬状態() && timing == フロア終了_タイミング)
	{
		目薬解除();
	}

	if(嫉妬状態() && timing == 攻撃力計算時_タイミング)
	{
		valiable.doubles.val(変数_攻撃力ボーナス_倍率) += 1.0;
	}

	if(無意識状態() && timing == 攻撃力計算時_タイミング)
	{
		valiable.doubles.val(変数_攻撃力ボーナス_倍率) += 1.0;
	}

	if(狂乱状態())
	{
		if(timing == 攻撃力計算時_タイミング)
		{
			valiable.doubles.val(変数_攻撃力ボーナス_倍率) += 0.5;
		}
		if(timing == 防御力計算時_タイミング)
		{
			valiable.doubles.val(変数_防御力ボーナス_倍率) += 0.5;
		}
	}

	if(臆病状態() && timing == 防御力計算時_タイミング)
	{
		valiable.doubles.val(変数_防御力ボーナス_倍率) += 1.0;
	}

	if(病気状態() && timing == 回復直前_タイミング)
	{
		if(self() == sg_pDungeonSystem->pPlayerChara() && valiable.doubles.val(変数_メッセージフラグ)) {
			std::map<tstring, StyleString > val;
			g_Langメッセージ(_T("病気回復不可メッセージ"), val);
		}
		病気Chip.setAvoidHealVolume(病気Chip.avoidHealVolume() + valiable.doubles.val(変数_汎用実数));
		valiable.doubles[変数_汎用ブール] = false;
	}

	if(健康状態() && timing == ターン終了_タイミング) {
		sg_pDungeonSystem->回復要請(self(), ceil(self()->MHP*healthfitnessRecoverPerHP), false);
	}
}

//---------------------------------
//異常状態が自然に直ったときの特殊処理
bool cCondition::cure_back_natural(異常状態 type)
{
	

	return true;
}
//異常状態を無理やり治したときの特殊処理
bool cCondition::cure_back_force(異常状態 type)
{
	//アニメ
	refresh_emotion_anime();

	return cure_back_natural(type);
}
//異常状態になったときの特殊処理
bool cCondition::go_bad(異常状態 type)
{
	//アニメ
	refresh_emotion_anime();

	return true;
}

//説明
void cCondition::MakeExplainWindow(IDirect3DDevice9 *pDev)
{
	pcControlLayer pccl;
	pcListWindow pclw;

	//新規コントロールレイヤー
	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));
	pccl->Init(pDev);

	pccl->WindowList.push_back(pclw = pcListWindow(new cListWindow));

	pcDroping pdrop;

	//説明挿入
	explain(pclw);
	int i;
	for(i=0;i<pclw->pDroplist.size();i++)
	{
		pclw->pDroplist[i]->Init(pDev,pclw->pDroplist[i]);
	}

	if(pclw->pDroplist.size())
	{//なにかある
		
	}
	else
	{//なにもない
		pclw->pDroplist.push_back(pcDroping(new cDropingNothing));
	}

	
	pclw->Init(pDev, 20, 10);
	pclw->setLeft(sg_pDungeonSystem->GameScreenInterface.menuPosLeft(2));
	pclw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(2));

}

#define DEF_CONDITION_EXPLAIN(condi) \
if(count(condi))\
{\
	pclw->pDroplist.push_back(pcDroping(new cStateDroping(\
		g_Lang(((tstring)_T(#condi) + _T("キャプション")).c_str(),val),\
		g_Lang(((tstring)_T(#condi) + _T("ShortExplanation")).c_str(),val)\
		)));\
}

#define DEF_CONDITION_EXPLAIN_FLOOR(condi) \
if(sg_pDungeonSystem->condi##状態())\
{\
	pclw->pDroplist.push_back(pcDroping(new cStateDroping(\
		g_Lang(((tstring)_T(#condi) + _T("キャプション")).c_str(),val),\
		g_Lang(((tstring)_T(#condi) + _T("ShortExplanation")).c_str(),val)\
		)));\
}

void cCondition::explain(pcListWindow& pclw)
{
	StyleString exp;
	if(empty())
	{
		pclw->pDroplist.push_back(pcDroping(new cStateDroping(
			g_Lang(_T("正常キャプション")), //_T("正常\n"),
			g_Lang(_T("正常ShortExplanation"))//_T("正常です。\n")
			//_T("---特に心配することもないでしょう。")
			)));
	}

	std::map<tstring, StyleString > val;

	int i;
	
	異常状態 type;
	int size = (int)p_condition_chipvector_.size();
	for(i=0;i<size;i++)
	{
		p_condition_chipvector_[i]->setConditionExplain(pclw);

	}
	

	DEF_CONDITION_EXPLAIN_FLOOR(フロア飛ばず)
	/*
	DEF_CONDITION_EXPLAIN(空腹)

	val[_T("Turn")] = setStyle(speed_turn_count + 1);
	DEF_CONDITION_EXPLAIN(超鈍足)
	DEF_CONDITION_EXPLAIN(鈍足)
	DEF_CONDITION_EXPLAIN(二倍速)
	DEF_CONDITION_EXPLAIN(三倍速)

	val[_T("Turn")] = setStyle(deisui_turn_count + 1);
	DEF_CONDITION_EXPLAIN(泥酔)

	val[_T("Turn")] = setStyle(poison_turn_count + 1);
	val[_T("Num")] = setStyle(currentpoison);
	DEF_CONDITION_EXPLAIN(脱力)
	val[_T("Num")] = setStyle(-currentpoison);
	DEF_CONDITION_EXPLAIN(元気)

	val[_T("Turn")] = setStyle(poison_turn_count + 1);
	val[_T("Num")] = setStyle(currentpoison);
	DEF_CONDITION_EXPLAIN(軟弱)
	val[_T("Num")] = setStyle(-currentpoison);
	DEF_CONDITION_EXPLAIN(頑強)

	DEF_CONDITION_EXPLAIN(目薬)

	val[_T("Turn")] = setStyle(fuuin_turn_count + 1);
	DEF_CONDITION_EXPLAIN(封印)

	val[_T("Turn")] = setStyle(karaburi_turn_count + 1);
	DEF_CONDITION_EXPLAIN(空振り)

	val[_T("Turn")] = setStyle(yarisugosi_turn_count + 1);
	DEF_CONDITION_EXPLAIN(やりすごし)

	val[_T("Turn")] = setStyle(binbou_turn_count + 1);
	DEF_CONDITION_EXPLAIN(貧乏)

	val[_T("Turn")] = setStyle(koorituke_turn_count + 1);
	DEF_CONDITION_EXPLAIN(氷付け)

	val[_T("Turn")] = setStyle(sasoi_turn_count);
	DEF_CONDITION_EXPLAIN(死の誘い)

	val[_T("Turn")] = setStyle(migawari_turn_count + 1);
	DEF_CONDITION_EXPLAIN(みがわり)

	val[_T("Turn")] = setStyle(muteki_turn_count + 1);
	DEF_CONDITION_EXPLAIN(無敵)
	
	val[_T("Turn")] = setStyle(gitai_turn_count + 1);
	DEF_CONDITION_EXPLAIN(擬態)

	val[_T("Turn")] = setStyle(torime_turn_count + 1);
	DEF_CONDITION_EXPLAIN(鳥目)

	val[_T("Turn")] = setStyle(shitto_turn_count + 1);
	DEF_CONDITION_EXPLAIN(嫉妬)

	val[_T("Turn")] = setStyle(okubyou_turn_count + 1);
	DEF_CONDITION_EXPLAIN(臆病)
*/

	/*

	if(count(空腹))
	{////////１　　　５　　　　10　　　　15　　　　20//
		pclw->pDroplist.push_back(pcDroping(new cStateDroping(
			g_Lang(_T("空腹キャプション")), //_T("空腹   : ターン毎にHPが減っていく。\n"),
			g_Lang(_T("空腹ShortExplanation"))//_T("お腹が減って倒れそうです。\n")
			//_T("---ターンが経つごとにひとつずつHPが減っていきます。\n")
			//_T("---食べ物や飲み物で満腹度を回復すると治ります。")
			)));
	}

	if(count(超鈍足))
	{///////////１　　　５　　　　10　　　　15　　　　20//
		pclw->pDroplist.push_back(pcDroping(new cStateDroping(
			g_Lang(_T("超鬱キャプション")), //_T("超鬱   : ３ターンに一度しか行動できない。\n"),
			g_Lang(_T("超鬱ShortExplanation"))//_T("とても欝で何もやる気がおきません。\n")
			//_T("---３ターンに一度しか行動できません。\n")
			//_T("---時間がたつと治ります。")
			)));
	}

	if(count(鈍足))
	{
		pclw->pDroplist.push_back(pcDroping(new cStateDroping(
			g_Lang(_T("鬱キャプション")), //_T("鬱     : ２ターンに一度しか行動できない。\n"),
			g_Lang(_T("鬱ShortExplanation"))//_T("欝でやる気がおきません。\n")
			//_T("---２ターンに一度しか行動できません。\n")
			//_T("---時間がたつと治ります。")
			)));
	}

	if(count(二倍速))
	{
		pclw->pDroplist.push_back(pcDroping(new cStateDroping(
			g_Lang(_T("倍速キャプション")), //_T("倍速   : １ターンに二度行動できる。\n"),
			g_Lang(_T("倍速ShortExplanation"))//_T("とても元気でてきぱきうごけます。\n")
			//_T("---１ターンに二度行動できます。\n")
			//_T("---時間がたつと治ります。")
			)));
	}

	if(count(三倍速))
	{
		pclw->pDroplist.push_back(pcDroping(new cStateDroping(
			g_Lang(_T("三倍速キャプション")), //_T("三倍速 : １ターンに三度行動できる。\n"),
			g_Lang(_T("三倍速ShortExplanation"))//_T("非常に元気で俊敏にうごけます。\n")
			//_T("---１ターンに三度行動できます。\n")
			//_T("---時間がたつと治ります。")
			)));
	}
*/


}



pcCharacter cCondition::self()
{
	return weak_pself.lock();
}
bool cCondition::refresh_emotion()
{
	int i;
	異常状態 type;
	int size = p_condition_chipvector_.size();
	//エモーションを全消しする
	for(i=0;i<size;i++)
	{
		p_condition_chipvector_[i]->erase_emotion();
	}

	//エモーションを付加する
	for(i=0;i<size;i++)
	{
		p_condition_chipvector_[i]->set_emotion();
	}
	return true;
}
bool cCondition::refresh_predict_emotion()
{
	int i;
	異常状態 type;

	int size = p_condition_chipvector_.size();
	//エモーションを全消しする
	for(i=0;i<size;i++)
	{
		p_condition_chipvector_[i]->erase_emotion();
	}

	//エモーションを付加する
	for(i=0;i<size;i++)
	{
		p_condition_chipvector_[i]->set_predict_emotion();
	}
	return true;
}
//エモーションアニメを更新する
bool cCondition::refresh_emotion_anime()
{
	//アニメ
	sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(
		&self()->visibleemotion,
		self()->emotion);
	//アニメ
	sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(
		&anotherSurfaceID,
		anotherSurfaceIDchange);

	return true;
}
bool cCondition::process()
{

	int i;
	/*
	異常状態 type;
	for(i=0;i<CONDITION_NUM;i++)
	{
		type = static_cast<異常状態>(i);
		Chip(type).process();
	}
	*/

	int size = p_condition_chipvector_.size();
	//エモーションを全消しする
	for(i=0;i<size;i++)
	{
		p_condition_chipvector_[i]->process();
	}

/*
	process_speed();
	process_sleep();
	process_bikkuri();
	process_kanasibari();
	process_deisui();
	process_poison();
	process_poison();
	process_fuuin();
	process_karaburi();
	process_yarisugosi();
	process_binbou();
	process_koorituke();
	process_sasoi();
	process_migawari();
	process_muteki();
	process_gitai();
	process_kyouran();
	process_okubyou();
	process_shitto();
	process_torime();
	//process_stomach();
*/

	if(self()->Forse == CHARACTER_FORSE_OTHER && sg_pDungeonSystem->キャラクター敵対判定(self(),sg_pDungeonSystem->pPlayerChara()))
		self()->emotion.insert(EMOTION_IKARI);
	else
		self()->emotion.erase(EMOTION_IKARI);
	
	self()->conditionprocess();
	return true;
}

bool cCondition::Draw(IDirect3DDevice9 *pDev)
{
	if(死の誘い状態() && 死の誘いChip.sasoi_turn_count_visible())
	{
		死の誘いChip.Draw_sasoi(pDev);
	}

	return true;
}


bool cCondition::飛ばし危険()
{
	cValiableField valiable;
	valiable.doubles.dim(変数_汎用ブール) = 0;
	sg_pDungeonSystem->CutInM().CutIn(self(), 飛ばし危険判定時_タイミング, valiable);
	
	return (count(空腹)) || count(死の誘い) || valiable.doubles.val(変数_汎用ブール);
}
bool cCondition::行動不能である()
{
	return 眠りで行動不能である() || びっくりで行動不能である() || 金縛りで行動不能である();
}
bool cCondition::行動不能である_visual()
{
	return self()->visibleemotion.count(EMOTION_ZZZ) || self()->visibleemotion.count(EMOTION_BIRIBIRI)
		|| self()->visibleemotion.count(EMOTION_BIKKURI);
}
bool cCondition::混乱状態である()
{
	return 泥酔で混乱状態である() || みがわり状態();
}
bool cCondition::透明状態である()
{
	return (擬態状態()
		|| やりすごし状態());
}
bool cCondition::無敵状態である()
{
	return やりすごし状態() || 無敵状態();
}


//---------------------------------
//眠り関係
//---------------------------------
bool cCondition::眠りcConditionChip::process()
{
	if(count(眠り) || count(バクスイ))
	{//眠り状態異常
		sleep_turn_count--;
		if(turn_off_judge(sleep_turn_count))
		{//眠り状態異常切れ


			if(self() == sg_pDungeonSystem->pPlayerChara())
			{
				
				map<tstring, StyleString> valiable;
	
				valiable[_T("Chara")] = sg_pDungeonSystem->pPlayerChara()->ShortName();
				g_Langメッセージ(_T("眠り元通りメッセージ"),valiable);
			}

			if(count(バクスイ))
			{//バクスイの反動
				cure_back_natural(バクスイ);
				sg_pDungeonSystem->速度増加要請(self(),heavysleep_turnmemo);
			}
			else if(count(バクスイ))
			{
				cure_back_natural(眠り);
			}
			erase(眠り);
			erase(バクスイ);

			sleep_turn_count = TURN_NOTBAD;



			//process_emotion_sleep();
		}
	}

	return true;
	/*
	if(count(眠り) || count(バクスイ))
	{//眠り状態異常
		sleep_turn_count--;
		if(sleep_turn_count < 0)
		{//眠り状態異常切れ


			if(self() == sg_pDungeonSystem->pPlayerChara())
			{
				
				map<tstring, StyleString> valiable;
	
				valiable[_T("Chara")] = sg_pDungeonSystem->pPlayerChara()->ShortName();
				g_Langメッセージ(_T("眠り元通りメッセージ"),valiable);
			}

			if(count(バクスイ))
			{//バクスイの反動
				sg_pDungeonSystem->速度増加要請(self(),heavysleep_turnmemo);
			}
			erase(眠り);
			erase(バクスイ);

			sleep_turn_count = TURN_NOTBAD;

			process_emotion_sleep();
		}
	}

	return true;

	*/
}

bool cCondition::眠りcConditionChip::set_emotion()
{//エモーション処理
	/*
	if(self()->emotion.count(EMOTION_ZZZ) != 眠りで行動不能である())
	{
		if(眠りで行動不能である())
		{
			self()->emotion.insert(EMOTION_ZZZ);
		}
		else
		{
			self()->emotion.erase(EMOTION_ZZZ);
		}

		sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(
			&self()->visibleemotion,
			self()->emotion);
	}
	return true;
	*/
	//if(self()->emotion.count(EMOTION_ZZZ) && 眠りで行動不能である())
	//{
		if(眠りで行動不能である())
		{
			self()->emotion.insert(EMOTION_ZZZ);
		}

	//}
	return true;
}

		
//敵用の次の行動の時のエモーションを予測してセットする関数
bool cCondition::眠りcConditionChip::set_predict_emotion()
{
	if(count(眠り) || count(バクスイ))
	{//眠り状態異常
		int tempcount = sleep_turn_count;
		tempcount--;
		if(!turn_off_judge(tempcount))
		{
			self()->emotion.insert(EMOTION_ZZZ);
		}
	}
	else if(count(深い居眠り) || count(浅い居眠り))
	{
		self()->emotion.insert(EMOTION_ZZZ);
	}
	return true;
}

//エモーションを消す関数
bool cCondition::眠りcConditionChip::erase_emotion()
{
	self()->emotion.erase(EMOTION_ZZZ);
	return true;
}
bool cCondition::眠りcConditionChip::眠り追加(int turn)
{
	if(turn < 0)
	{//効果切れ
		sleep_turn_count = TURN_NOTBAD;
		erase(眠り);
		self()->emotion.erase(EMOTION_ZZZ);
		cure_back_force(眠り);
	}
	else
	{
		if(!count(眠り))
		{
			sleep_turn_count = max(sleep_turn_count,turn);
			insert(眠り);
			self()->emotion.insert(EMOTION_ZZZ);
			go_bad(眠り);

			
		}
	}
	
	return true;
}
bool cCondition::眠りcConditionChip::バクスイ追加(int turn)
{
	if(turn < 0)
	{//効果切れ
		sleep_turn_count = TURN_NOTBAD;
		heavysleep_turnmemo = TURN_NOTBAD;
		erase(バクスイ);
		self()->emotion.erase(EMOTION_ZZZ);
		cure_back_force(バクスイ);
	}
	else
	{
		sleep_turn_count = turn;
		heavysleep_turnmemo = turn;
		insert(バクスイ);
		self()->emotion.insert(EMOTION_ZZZ);
		go_bad(バクスイ);
	}

	return true;
}
bool cCondition::眠りcConditionChip::浅い居眠り追加()
{
	erase(深い居眠り);
	insert(浅い居眠り);
	self()->emotion.insert(EMOTION_ZZZ);
	go_bad(浅い居眠り);
	return true;
}
bool cCondition::眠りcConditionChip::深い居眠り追加()
{
	erase(浅い居眠り);
	insert(深い居眠り);
	self()->emotion.insert(EMOTION_ZZZ);
	go_bad(深い居眠り);
	return true;
}
bool cCondition::とても強い刺激()
{//直接攻撃
	erase(浅い居眠り);
	erase(深い居眠り);
	金縛りChip.set_off();
	self()->emotion.erase(EMOTION_ZZZ);
	cure_back_force(異常状態_初期化無し);
	return true;
}
void cCondition::damagedHP()
{
	空腹Chip.resetLastDamageTurnCount();
}
bool cCondition::眠りcConditionChip::強い刺激(double power)
{//隣接
	if(count(深い居眠り))
	{
		if(random() < 深い居眠り強い刺激浅い居眠り遷移確率() * power)
		{
			浅い居眠り追加();
		}
	}
	
	if(count(浅い居眠り))
	{
		if(random() < 浅い居眠り強い刺激覚醒遷移確率() * power)
		{
			erase(浅い居眠り);
			erase(深い居眠り);
			self()->emotion.erase(EMOTION_ZZZ);
			sg_pDungeonSystem->強制ターンエンド要請(self());
		}
	}

	return true;
}
bool cCondition::眠りcConditionChip::弱い刺激(double power)
{//部屋進入
	if(count(深い居眠り))
	{
		if(random() < 深い居眠り弱い刺激浅い居眠り遷移確率() * power)
		{
			浅い居眠り追加();
		}
	}
	
	if(count(浅い居眠り))
	{
		if(random() < 浅い居眠り弱い刺激覚醒遷移確率() * power)
		{
			erase(浅い居眠り);
			erase(深い居眠り);
			self()->emotion.erase(EMOTION_ZZZ);
			sg_pDungeonSystem->強制ターンエンド要請(self());
		}
	}

	return true;
}
double cCondition::眠りcConditionChip::深い居眠り強い刺激浅い居眠り遷移確率()
{
	return sg_pDungeonSystem->DataBase.CharaImportData_Value((tstring)_T("Enemy基本値"),(tstring)_T("深い居眠り強い刺激浅い居眠り遷移確率"),0.5);
}
double cCondition::眠りcConditionChip::深い居眠り弱い刺激浅い居眠り遷移確率()
{
	return sg_pDungeonSystem->DataBase.CharaImportData_Value((tstring)_T("Enemy基本値"),(tstring)_T("深い居眠り弱い刺激浅い居眠り遷移確率"),0.1);
}
double cCondition::眠りcConditionChip::浅い居眠り強い刺激覚醒遷移確率()
{
	return sg_pDungeonSystem->DataBase.CharaImportData_Value((tstring)_T("Enemy基本値"),(tstring)_T("浅い居眠り強い刺激覚醒遷移確率"),1.0);
}
double cCondition::眠りcConditionChip::浅い居眠り弱い刺激覚醒遷移確率()
{
	return sg_pDungeonSystem->DataBase.CharaImportData_Value((tstring)_T("Enemy基本値"),(tstring)_T("浅い居眠り弱い刺激覚醒遷移確率"),0.5);
}
bool cCondition::眠りcConditionChip::眠りで行動不能である()
{
	return count(眠り) || count(バクスイ) || count(浅い居眠り) || count(深い居眠り);
}
/*
//---------------------------------
//びっくり関係
//---------------------------------
bool cCondition::process_bikkuri()
{
	if(count(びっくり))
	{//眠り状態異常
		bikkuri_turn_count--;
		if(bikkuri_turn_count < 0)
		{//眠り状態異常切れ


			if(self() == sg_pDungeonSystem->pPlayerChara())
			{
				
				map<tstring, StyleString> valiable;
	
				valiable[_T("Chara")] = sg_pDungeonSystem->pPlayerChara()->ShortName();
				g_Langメッセージ(_T("びっくり元通りメッセージ"),valiable);
			}

			erase(びっくり);

			bikkuri_turn_count = TURN_NOTBAD;

			process_emotion_bikkuri();
		}
	}

	return true;

}

bool cCondition::process_emotion_bikkuri()
{//エモーション処理
	if(self()->emotion.count(EMOTION_BIKKURI) != びっくりで行動不能である())
	{
		if(びっくりで行動不能である())
		{
			self()->emotion.insert(EMOTION_BIKKURI);
		}
		else
		{
			self()->emotion.erase(EMOTION_BIKKURI);
		}

		sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(
			&self()->visibleemotion,
			self()->emotion);
	}
	return true;
}
bool cCondition::びっくり追加(int turn)
{
	bool res = Chip(びっくり).turn_on(turn,self());
	Chip(びっくり).set_emotion(self());
	return res;
	
	//if(turn < 0)
	//{//効果切れ
	//	bikkuri_turn_count = TURN_NOTBAD;
	//	erase(びっくり);
	//}
	//else
	//{
	//	if(!count(びっくり))
	//	{
	//		bikkuri_turn_count = turn;
	//		insert(びっくり);
	//	}
	//}
	//process_emotion_bikkuri();
	//return true;
	//
}
bool cCondition::びっくりで行動不能である()
{
	return count(びっくり);
}
//---------------------------------
//金縛り関係
//---------------------------------
bool cCondition::process_kanasibari()
{
	if(count(金縛り))
	{//金縛り状態異常
		kanasibari_turn_count--;
		if(kanasibari_turn_count < 0)
		{//金縛り状態異常切れ


			if(self() == sg_pDungeonSystem->pPlayerChara())
			{
				
				map<tstring, StyleString> valiable;
	
				valiable[_T("Chara")] = sg_pDungeonSystem->pPlayerChara()->ShortName();
				g_Langメッセージ(_T("金縛り元通りメッセージ"),valiable);
			}

			erase(金縛り);

			kanasibari_turn_count = TURN_NOTBAD;

			process_emotion_kanasibari();
		}
	}

	return true;

}

bool cCondition::process_emotion_kanasibari()
{//エモーション処理
	if(self()->emotion.count(EMOTION_BIRIBIRI) != 金縛りで行動不能である())
	{
		if(金縛りで行動不能である())
		{
			self()->emotion.insert(EMOTION_BIRIBIRI);
		}
		else
		{
			self()->emotion.erase(EMOTION_BIRIBIRI);
		}

		sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(
			&self()->visibleemotion,
			self()->emotion);
	}
	return true;
}
bool cCondition::金縛り追加(int turn)
{
	if(turn < 0)
	{//効果切れ
		kanasibari_turn_count = turn;
		erase(金縛り);
	}
	else
	{
		if(!count(金縛り))
		{
			kanasibari_turn_count = turn;
			insert(金縛り);
		}
	}
	process_emotion_kanasibari();
	return true;
}
bool cCondition::金縛りで行動不能である()
{
	return count(金縛り);
}

//---------------------------------
//泥酔関係
//---------------------------------
bool cCondition::process_deisui()
{
	if(count(泥酔))
	{//泥酔状態異常
		deisui_turn_count--;
		if(deisui_turn_count < 0)
		{//泥酔状態異常切れ


			if(self() == sg_pDungeonSystem->pPlayerChara())
			{
				
				map<tstring, StyleString> valiable;
	
				valiable[_T("Chara")] = sg_pDungeonSystem->pPlayerChara()->ShortName();
				g_Langメッセージ(_T("泥酔元通りメッセージ"),valiable);
			}

			erase(泥酔);

			deisui_turn_count = TURN_NOTBAD;

			process_emotion_deisui();
		}
	}

	return true;

}

bool cCondition::process_emotion_deisui()
{//エモーション処理
	if(self()->emotion.count(EMOTION_GURUGURU) != 泥酔で混乱状態である())
	{
		if(泥酔で混乱状態である())
		{
			self()->emotion.insert(EMOTION_GURUGURU);
		}
		else
		{
			self()->emotion.erase(EMOTION_GURUGURU);
		}

		sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(
			&self()->visibleemotion,
			self()->emotion);
	}
	return true;
}
bool cCondition::泥酔追加(int turn)
{
	if(turn < 0)
	{//効果切れ
		deisui_turn_count = -1;
		erase(泥酔);
	}	
	else
	{
		if(!count(泥酔))
		{
			deisui_turn_count = turn;
			insert(泥酔);
		}
	}
	process_emotion_deisui();
	return true;
}
bool cCondition::泥酔で混乱状態である()
{
	return count(泥酔);
}
*/
//----------------------------------------------
//満腹度関係
//----------------------------------------------
//define at _T("cCondition.h")
//#define STOMACH_MSG_FULL 0
//#define STOMACH_MSG_AFEW 1
//#define STOMACH_MSG_FEW 2
//#define STOMACH_MSG_ENPTY1 3
//#define STOMACH_MSG_ENPTY2 4
//#define STOMACH_MSG_ENPTY3 5

bool cCondition::process_stomach()
{
	return 空腹Chip.process();
}
int cCondition::空腹cConditionChip::init(異常状態 type, int emotion, pcCharacter pchara)
{
	cConditionChipVirtual::init(type,emotion,pchara);
	HP_oddstock = 0;
	stomach_msg_count = 0;
	lastDamageTurnCount = pchara->HP自然回復開始ターン();
	return true;
};
bool cCondition::空腹cConditionChip::process()
{
	//if(self() == sg_pDungeonSystem->pPlayerChara())
	//{



		if((self()->空腹ProcessFlag() & 空腹ProcessFlag_空腹))
		{

			
			if(self()->Stomach <= 0 && stomach_msg_count == STOMACH_MSG_ENPTY3)
			{//お腹すいた
				if(count(空腹) == 0)
				{//空腹で無い
					insert(空腹);//空腹に
					
					self()->emotion.insert(EMOTION_HUNGER);
					go_bad(空腹);
				}
				
			}
			else
			{//お腹すいてない
				if(count(空腹) != 0)
				{//空腹
					erase(空腹);//空腹でなくする
					
					self()->emotion.erase(EMOTION_HUNGER);
					cure_back_natural(空腹);
				}

			}


			int now_stomach = 0;

			if(self()->Stomach > 20) now_stomach = STOMACH_MSG_FULL;
			else if(self()->Stomach > 10) now_stomach = STOMACH_MSG_AFEW;
			else if(self()->Stomach > 0 ) now_stomach = STOMACH_MSG_FEW;
			else if(self()->Stomach <= 0 &&
				stomach_msg_count == STOMACH_MSG_FEW) now_stomach = STOMACH_MSG_ENPTY1;
			else if(self()->Stomach <= 0 &&
				stomach_msg_count == STOMACH_MSG_ENPTY1) now_stomach = STOMACH_MSG_ENPTY2;
			else if(self()->Stomach <= 0 &&
				stomach_msg_count == STOMACH_MSG_ENPTY2) now_stomach = STOMACH_MSG_ENPTY3;
			else now_stomach = STOMACH_MSG_ENPTY3;

			if(stomach_msg_count >= now_stomach)
			{//お腹が膨れた
				stomach_msg_count = now_stomach;
			}
			else
			{//お腹がすいた
				stomach_msg_count = now_stomach;
				
				pcPlayerChara pplchara = boost::dynamic_pointer_cast<cPlayerChara>(sg_pDungeonSystem->pPlayerChara());
				if (pplchara != NULL)
				{
					sg_pDungeonSystem->AnimationManager().
						Anime_PlaySE(_T("hunger.wav"),sg_pDungeonSystem->pPlayerChara()->足元地形()->place);

					sg_pDungeonSystem->メッセージ(pplchara->uniqueMessage(_T("空腹"),now_stomach));
					if(now_stomach >= STOMACH_MSG_ENPTY1)
					{//最終通告
						//sg_pDungeonSystem->AnimationManager().Anime_MessageWait();
					}
				}
			}


		}



	//}



	//if(self()->Forse == CHARACTER_FORSE_FRIEND)
	//{//友軍
		if(self()->雑魚属性() && count(病気))
		{
			double damege = ceil(self()->HP*sicknessDamegePerHP);
			if(self()->HP == damege) {
				damege = self()->HP-1;
			}
			if(damege > 0) {
				sg_pDungeonSystem->強制ダメージ要請(self(), damege, false, true);
			}
		}
		if((self()->空腹ProcessFlag() & 空腹ProcessFlag_空腹))
		{
			if(!sg_pDungeonSystem->拠点フラグ())
			{
				if(count(空腹))
				{//お腹すいた
					sg_pDungeonSystem->強制ダメージ要請(self(), 1, false, false);
					
				}
				else
				{
					cValiableField valf;
					valf.doubles.dim(変数_汎用ボーナス_倍率) = 1;
					sg_pDungeonSystem->CutInM().CutIn(self(),自然満腹度減少量計算時_タイミング,valf);
					
					//最小値
					valf.doubles.val(変数_汎用ボーナス_倍率) = max(valf.doubles.val(変数_汎用ボーナス_倍率),0.5);

					sg_pDungeonSystem->満腹度減少要請(self(), 0.1*valf.doubles.val(変数_汎用ボーナス_倍率), false);
				}
			}
		}
		if(!count(空腹) && (self()->空腹ProcessFlag() & 空腹ProcessFlag_回復))
		{
			lastDamageTurnCount++;
			if(lastDamageTurnCount > self()->HP自然回復開始ターン() && !count(病気)) {
				//ココにダメージを受けたら一定時間回復しない処理を入れる
				cValiableField valf;
				valf.doubles.dim(変数_回復力ボーナス_倍率) = 1;
				sg_pDungeonSystem->CutInM().CutIn(self(),自然回復量計算時_タイミング,valf);
		

				double recover_d = max(self()->MHP*self()->HP自然回復割合()*valf.doubles.val(変数_回復力ボーナス_倍率),self()->HP自然回復最低保障値()) + HP_oddstock;//0.5を下限
				int recover = recover_d;
				HP_oddstock = recover_d - recover;
				if (sg_pDungeonSystem->回復要請(self(),recover,false)) 
				{
					cValiableField valf;
					valf.doubles.dim(変数_汎用ボーナス_倍率) = 1;
					sg_pDungeonSystem->CutInM().CutIn(self(),自然満腹度減少量計算時_タイミング,valf);
					valf.doubles.val(変数_汎用ボーナス_倍率) = max(valf.doubles.val(変数_汎用ボーナス_倍率),0.5);//最小値
					sg_pDungeonSystem->満腹度減少要請(self(), 0.4*valf.doubles.val(変数_汎用ボーナス_倍率), false);
				}
			}
			if(lastDamageTurnCount > GAME_TURN_GAMEOVER) {
				lastDamageTurnCount = GAME_TURN_GAMEOVER;
			}
		}
		
	//}
	return true;
}

bool cCondition::空腹cConditionChip::set_emotion()
{
	if(count(空腹))
	{
		self()->emotion.insert(EMOTION_HUNGER);
	}
	return true;
}

bool cCondition::空腹cConditionChip::set_predict_emotion()
{
	if(count(空腹))
	{
		self()->emotion.insert(EMOTION_HUNGER);
	}
	return true;
}

bool cCondition::空腹cConditionChip::erase_emotion()
{
	self()->emotion.erase(EMOTION_HUNGER);
	return true;
}

void cCondition::空腹cConditionChip::resetLastDamageTurnCount()
{
	lastDamageTurnCount = 0;
}

//----------------------------------------------
//速度関係
//----------------------------------------------
int cCondition::速度cConditionChip::init(異常状態 type, int emotion, pcCharacter pchara)
{
	cCondition::cConditionChipVirtual::init(type,emotion,pchara);
	currentspeed = defaultspeed = SPEED_NORMAL;
	speed_turn_count = TURN_NOTBAD;
	slow_count = 0;
	return true;
}
		
bool cCondition::速度cConditionChip::process()
{


	if(currentspeed != defaultspeed)
	{//速度状態異常
		speed_turn_count--;
		if(turn_off_judge(speed_turn_count))
		{//速度状態異常切れ

			currentspeed = defaultspeed;

			if(self() == sg_pDungeonSystem->pPlayerChara())
			{
				sg_pDungeonSystem->メッセージ(g_Lang(_T("速度元通りメッセージ")));//_T("速度が元にもどった。\n"));
			}


			slow_count = 0;
			speed_turn_count = TURN_NOTBAD;
			
			//speed_refresh();
			//速度表記初期化
			erase(超鈍足);
			erase(鈍足);
			erase(二倍速);
			erase(三倍速);

			cure_back_natural(異常状態_初期化無し);

			//設定
			switch(currentspeed)
			{
			case SPEED_SUPERSLOW:
				insert(超鈍足); break;
			case SPEED_SLOW:
				insert(鈍足); break;
			case SPEED_DOUBLE:
				insert(二倍速); break;
			case SPEED_TRIPLE:
				insert(三倍速); break;
			}

			if(count(超鈍足) || count(鈍足))
			{
				slow_count = -currentspeed;//鈍足カウント
			}


		}
		

	}

	if(count(超鈍足) || count(鈍足))
	{//鈍足系
		slow_count--;
		if(slow_count < 0) slow_count = -currentspeed;//鈍足カウント初期化
	}
		
	return true;
}

bool cCondition::速度cConditionChip::set_emotion()
{
	if(count(超鈍足) || count(鈍足))
	{
		if(defaultspeed != currentspeed) self()->emotion.insert(EMOTION_UTSU);

		if(count(超鈍足))
		{
			self()->emotion.insert(EMOTION_SUBINFO_STEP_ATHIRD);
		}
		else
		{
			self()->emotion.insert(EMOTION_SUBINFO_STEP_AHALF);
		}
	}
	else if(count(二倍速) || count(三倍速))
	{
		if(count(二倍速))
		{
			self()->emotion.insert(EMOTION_SUBINFO_STEP_DOUBLE);
		}
		else
		{
			self()->emotion.insert(EMOTION_SUBINFO_STEP_TRIPLE);
		}
	}
	return true;
}

bool cCondition::速度cConditionChip::set_predict_emotion()
{
	/*
	if(speed_turn_count < 0)
	{//速度状態異常切れ

		currentspeed = defaultspeed;

		if(self() == sg_pDungeonSystem->pPlayerChara())
		{
			sg_pDungeonSystem->メッセージ(g_Lang(_T("速度元通りメッセージ")));//_T("速度が元にもどった。\n"));
		}


		slow_count = TURN_NOTBAD;
		speed_turn_count = TURN_NOTBAD;
		
		//speed_refresh();
		//速度表記初期化
		erase(超鈍足);
		erase(鈍足);
		erase(二倍速);
		erase(三倍速);

		//設定
		switch(currentspeed)
		{
		case SPEED_SUPERSLOW:
			insert(超鈍足); break;
		case SPEED_SLOW:
			insert(鈍足); break;
		case SPEED_DOUBLE:
			insert(二倍速); break;
		case SPEED_TRIPLE:
			insert(三倍速); break;
		}

		if(count(超鈍足) || count(鈍足))
		{
			slow_count = -currentspeed;//鈍足カウント
		}

	}
	*/


	int temp = speed_turn_count;
	temp--;
	if(!turn_off_judge(temp) || currentspeed == defaultspeed)
	{
		if(count(超鈍足) || count(鈍足))
		{
			if(defaultspeed != currentspeed) self()->emotion.insert(EMOTION_UTSU);

			if(count(超鈍足))
			{
				self()->emotion.insert(EMOTION_SUBINFO_STEP_ATHIRD);
			}
			else
			{
				self()->emotion.insert(EMOTION_SUBINFO_STEP_AHALF);
			}
		}
		else if(count(二倍速) || count(三倍速))
		{
			if(count(二倍速))
			{
				self()->emotion.insert(EMOTION_SUBINFO_STEP_DOUBLE);
			}
			else
			{
				self()->emotion.insert(EMOTION_SUBINFO_STEP_TRIPLE);
			}
		}
	}
	return true;
}

bool cCondition::速度cConditionChip::erase_emotion()
{
	//エモーション表示更新
	self()->emotion.erase(EMOTION_UTSU);


	self()->emotion.erase(EMOTION_SUBINFO_STEP_ATHIRD);
	self()->emotion.erase(EMOTION_SUBINFO_STEP_AHALF);
	self()->emotion.erase(EMOTION_SUBINFO_STEP_DOUBLE);
	self()->emotion.erase(EMOTION_SUBINFO_STEP_TRIPLE);
	return true;
}

int cCondition::速度cConditionChip::speed_refresh()
{
	
	//速度表記初期化
	erase(超鈍足);
	erase(鈍足);
	erase(二倍速);
	erase(三倍速);

	//設定
	switch(currentspeed)
	{
	case SPEED_SUPERSLOW:
		insert(超鈍足); break;
	case SPEED_SLOW:
		insert(鈍足); break;
	case SPEED_DOUBLE:
		insert(二倍速); break;
	case SPEED_TRIPLE:
		insert(三倍速); break;
	}
	

	//エモーション表示更新
	self()->emotion.erase(EMOTION_UTSU);


	self()->emotion.erase(EMOTION_SUBINFO_STEP_ATHIRD);
	self()->emotion.erase(EMOTION_SUBINFO_STEP_AHALF);
	self()->emotion.erase(EMOTION_SUBINFO_STEP_DOUBLE);
	self()->emotion.erase(EMOTION_SUBINFO_STEP_TRIPLE);
	
	if(count(超鈍足) || count(鈍足))
	{
		if(defaultspeed != currentspeed) self()->emotion.insert(EMOTION_UTSU);
		slow_count = -currentspeed+1;//鈍足カウント

		if(count(超鈍足))
		{
			self()->emotion.insert(EMOTION_SUBINFO_STEP_ATHIRD);
		}
		else
		{
			self()->emotion.insert(EMOTION_SUBINFO_STEP_AHALF);
		}
	}
	else if(count(二倍速) || count(三倍速))
	{
		if(count(二倍速))
		{
			self()->emotion.insert(EMOTION_SUBINFO_STEP_DOUBLE);
		}
		else
		{
			self()->emotion.insert(EMOTION_SUBINFO_STEP_TRIPLE);
		}
	}
	else
	{

	}
	

	/*
	//アニメ
	sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(
		&self()->visibleemotion,
		self()->emotion);
	*/

	return true;
}


bool cCondition::速度cConditionChip::デフォルト速度設定(異常状態 speed)
{
	switch(speed)
	{
	case 超鈍足:
		currentspeed = defaultspeed = SPEED_SUPERSLOW; break;
	case 鈍足:
		currentspeed = defaultspeed = SPEED_SLOW; break;
	case 等速:
		currentspeed = defaultspeed = SPEED_NORMAL; break;
	case 二倍速:
		currentspeed = defaultspeed = SPEED_DOUBLE; break;
	case 三倍速:
		currentspeed = defaultspeed = SPEED_TRIPLE; break;
	}

	go_bad(異常状態_初期化無し);
	speed_refresh();//これがないと初期鈍足が実装できない
	return true;
}

bool cCondition::速度cConditionChip::速度増加(int turn)
{
	currentspeed++;
	if(currentspeed > SPEED_TRIPLE) currentspeed = SPEED_TRIPLE;
	
	speed_turn_count = turn;
	slow_count = 0;
	
	go_bad(異常状態_初期化無し);
	speed_refresh();
	return true;
}
bool cCondition::速度cConditionChip::速度減少(int turn)
{
	currentspeed--;
	if(currentspeed < SPEED_SUPERSLOW) currentspeed = SPEED_SUPERSLOW;

	speed_turn_count = turn;

	if(self()->雑魚属性() && currentspeed <= SPEED_SLOW)
	{
		sg_pDungeonSystem->強制ターンエンド要請(self());
	}

	go_bad(異常状態_初期化無し);
	speed_refresh();
	return true;
}

bool cCondition::速度cConditionChip::鈍足で行動不能である()
{
	return slow_count;
}

bool cCondition::速度cConditionChip::鈍足以下である()
{
	if(currentspeed <= SPEED_SLOW)
	{
		return true;
	}
	return false;
}

bool cCondition::速度cConditionChip::二倍速以上である()
{
	if(currentspeed >= SPEED_DOUBLE &&
		(self()->Forse == CHARACTER_FORSE_FRIEND || !self()->Condition.封印状態())) return true;
	return false;
}

bool cCondition::速度cConditionChip::三倍速である()
{
	if(currentspeed >= SPEED_TRIPLE &&
		(self()->Forse == CHARACTER_FORSE_FRIEND || !self()->Condition.封印状態())) return true;
	return false;
}

//----------------------------------------------
//脱力関係
//----------------------------------------------	
bool cCondition::脱力元気cConditionChip::process()
{

	if(currentpoison != 0)
	{//速度状態異常
		poison_turn_count--;
		if(turn_off_judge(poison_turn_count))
		{//速度状態異常切れ
			currentpoison = 0;

			
			if(self() == sg_pDungeonSystem->pPlayerChara())
			{
				map<tstring, StyleString> valiable;
				valiable[_T("Chara")] = sg_pDungeonSystem->pPlayerChara()->ShortName();
				sg_pDungeonSystem->メッセージ(g_Lang(_T("脱力元通りメッセージ"),valiable));//_T("速度が元にもどった。\n"));
			}

			poison_turn_count = TURN_NOTBAD;
			erase(脱力);
			erase(元気);
			cure_back_natural(異常状態_初期化無し);
		}
	}

	return true;
}

//主人公用の現在のエモーションをセットする関数
bool cCondition::脱力元気cConditionChip::set_emotion()
{

	if(!turn_off_judge(poison_turn_count))
	{
		if(currentpoison > 0)
		{//速度状態異常
			self()->emotion.insert(EMOTION_DOKU);
		}
		else if(currentpoison < 0)
		{//速度状態異常
			self()->emotion.insert(EMOTION_GENKI);
		}
		
	}
	return true;
}
		
//敵用の次の行動の時のエモーションを予測してセットする関数
bool cCondition::脱力元気cConditionChip::set_predict_emotion()
{
	int temp = poison_turn_count;
	temp--;
	if(!turn_off_judge(temp))
	{
		if(currentpoison > 0)
		{//速度状態異常
			self()->emotion.insert(EMOTION_DOKU);
		}
		else if(currentpoison < 0)
		{//速度状態異常
			self()->emotion.insert(EMOTION_GENKI);
		}
	}
	return true;
}

//エモーションを消す関数
bool cCondition::脱力元気cConditionChip::erase_emotion()
{
	self()->emotion.erase(EMOTION_DOKU);
	self()->emotion.erase(EMOTION_GENKI);
	return true;
}

bool cCondition::脱力元気cConditionChip::脱力初期化()
{
	if(abs(currentpoison) > 0)
	{
		currentpoison = 0;

		if(self() == sg_pDungeonSystem->pPlayerChara())
		{
			map<tstring, StyleString> valiable;
			valiable[_T("Chara")] = sg_pDungeonSystem->pPlayerChara()->ShortName();
			sg_pDungeonSystem->メッセージ(g_Lang(_T("脱力元通りメッセージ"),valiable));//_T("速度が元にもどった。\n"));
		}

		poison_turn_count = TURN_NOTBAD;


		erase(脱力);
		erase(元気);

		cure_back_natural(異常状態_初期化無し);
		//poison_refresh();

		return true;
	}
	return false;
}
/*
int cCondition::脱力元気cConditionChip::poison_refresh()
{
	//速度表記初期化
	erase(脱力);
	erase(元気);

	//設定
	if(currentpoison > 0)
	{
		insert(脱力);
	}
	else if(currentpoison < 0)
	{
		insert(元気);
	}



	//エモーション表示更新
	self()->emotion.erase(EMOTION_DOKU);
	self()->emotion.erase(EMOTION_GENKI);

	if(count(脱力))
	{
		self()->emotion.insert(EMOTION_DOKU);
	}
	else if( count(元気))
	{
		self()->emotion.insert(EMOTION_GENKI);
	}

	//アニメ
	sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(
		&self()->visibleemotion,
		self()->emotion);


	return true;
}
*/

bool cCondition::脱力元気cConditionChip::脱力追加(int power,int turn)
{
	//if(currentpoison < 0) currentpoison = 0;
	int prepoison = currentpoison;
	if(currentpoison < 0)
	{
		currentpoison = 0;
	}
	currentpoison += power;
	if(currentpoison > Maxpoison()) currentpoison = Maxpoison();
	
	if(poison_turn_count <= 0) poison_turn_count = turn;
	else poison_turn_count = min(poison_turn_count,turn);
	

	erase(脱力);
	erase(元気);
	if (currentpoison > 0)
	{
		insert(脱力);
	}
	else if (currentpoison < 0)
	{
		insert(元気);
	}

	if (prepoison < 0 && currentpoison >= 0)
	{
		cure_back_force(元気);
	}
	if (prepoison <= 0 && currentpoison > 0)
	{
		go_bad(脱力);
	}


	//poison_refresh();
	return true;
}
bool cCondition::脱力元気cConditionChip::元気追加(int power,int turn)
{
	//if(currentpoison > 0) currentpoison = 0;
	int prepoison = currentpoison;
	if(currentpoison > 0)
	{
		currentpoison = 0;
	}
	currentpoison -= power;
	if(currentpoison < -Maxpoison()) currentpoison = -Maxpoison();

	if(poison_turn_count <= 0) poison_turn_count = turn;
	else poison_turn_count = min(poison_turn_count,turn);
	

	erase(脱力);
	erase(元気);
	if(currentpoison > 0)
	{
		insert(脱力);
	}
	else if (currentpoison < 0)
	{
		insert(元気);
	}

	if(prepoison > 0 && currentpoison <= 0)
	{
		cure_back_force(脱力);
	}
	if(prepoison >= 0 && currentpoison < 0)
	{
		go_bad(元気);
	}
	

	//poison_refresh();
	return true;
}
double cCondition::脱力元気cConditionChip::力係数()
{
	if(currentpoison < 0)
	{//元気
		return 1.0 + -currentpoison/(double)Maxpoison();
	}
	else if(currentpoison > 0)
	{//脱力
		return 1.0 + -currentpoison/(double)Maxpoison();
	}
	return 1.0;
}
void cCondition::脱力元気cConditionChip::setConditionExplainType_Valiable(std::map<tstring, StyleString>& val)
{
	val[_T("Turn")] = setStyle(turncount() + 1);
	val[_T("Num")] = setStyle(abs(currentpoison));
}
//----------------------------------------------
//軟弱関係
//----------------------------------------------	
bool cCondition::軟弱頑強cConditionChip::process()
{

	if(currentweak != 0)
	{//速度状態異常
		weak_turn_count--;
		if(turn_off_judge(weak_turn_count))
		{//速度状態異常切れ
			currentweak = 0;

			
			if(self() == sg_pDungeonSystem->pPlayerChara())
			{
				map<tstring, StyleString> valiable;
				valiable[_T("Chara")] = sg_pDungeonSystem->pPlayerChara()->ShortName();
				sg_pDungeonSystem->メッセージ(g_Lang(_T("軟弱元通りメッセージ"),valiable));//_T("速度が元にもどった。\n"));
			}

			weak_turn_count = TURN_NOTBAD;
			erase(軟弱);
			erase(頑強);

			cure_back_natural(異常状態_初期化無し);
		}
	}

	return true;
}

//主人公用の現在のエモーションをセットする関数
bool cCondition::軟弱頑強cConditionChip::set_emotion()
{
	if(!turn_off_judge(weak_turn_count))
	{
		if(currentweak > 0)
		{//速度状態異常
			self()->emotion.insert(EMOTION_NANJAKU);
		}
		else if(currentweak < 0)
		{//速度状態異常
			self()->emotion.insert(EMOTION_GANKYOU);
		}
		
	}
	return true;
}
		
//敵用の次の行動の時のエモーションを予測してセットする関数
bool cCondition::軟弱頑強cConditionChip::set_predict_emotion()
{
	int temp = weak_turn_count;
	temp--;
	if(!turn_off_judge(temp))
	{
		if(currentweak > 0)
		{//速度状態異常
			self()->emotion.insert(EMOTION_NANJAKU);
		}
		else if(currentweak < 0)
		{//速度状態異常
			self()->emotion.insert(EMOTION_GANKYOU);
		}
		
	}
	return true;
}

//エモーションを消す関数
bool cCondition::軟弱頑強cConditionChip::erase_emotion()
{
	self()->emotion.erase(EMOTION_NANJAKU);
	self()->emotion.erase(EMOTION_GANKYOU);
	return true;
}

bool cCondition::軟弱頑強cConditionChip::軟弱初期化()
{
	if(abs(currentweak) > 0)
	{
		currentweak = 0;

		if(self() == sg_pDungeonSystem->pPlayerChara())
		{
			map<tstring, StyleString> valiable;
			valiable[_T("Chara")] = sg_pDungeonSystem->pPlayerChara()->ShortName();
			sg_pDungeonSystem->メッセージ(g_Lang(_T("軟弱元通りメッセージ"),valiable));//_T("速度が元にもどった。\n"));
		}

		weak_turn_count = TURN_NOTBAD;

		erase(脱力);
		erase(元気);

		cure_back_force(異常状態_初期化無し);


		//weak_refresh();

		return true;
	}
	return false;
}
/*
int cCondition::軟弱頑強cConditionChip::weak_refresh()
{
	//速度表記初期化
	erase(軟弱);
	erase(頑強);

	//設定
	if(currentweak > 0)
	{
		insert(軟弱);
	}
	else if(currentweak < 0)
	{
		insert(頑強);
	}



	//エモーション表示更新
	self()->emotion.erase(EMOTION_DOKU);
	self()->emotion.erase(EMOTION_GENKI);

	if(count(軟弱))
	{
		self()->emotion.insert(EMOTION_DOKU);
	}
	else if( count(頑強))
	{
		self()->emotion.insert(EMOTION_GENKI);
	}

	//アニメ
	sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(
		&self()->visibleemotion,
		self()->emotion);


	return true;
}
*/

bool cCondition::軟弱頑強cConditionChip::軟弱追加(int power,int turn)
{
	//if(currentweak < 0) currentweak = 0;
	int preweak = currentweak;
	if(currentweak < 0)
	{
		currentweak = 0;
	}
	currentweak += power;
	if(currentweak > Maxweak()) currentweak = Maxweak();
	
	if(weak_turn_count <= 0) weak_turn_count = turn;
	else weak_turn_count = min(weak_turn_count,turn);
	
	erase(軟弱);
	erase(頑強);
	if(currentweak > 0)
	{
		insert(軟弱);
	}
	else if (currentweak < 0)
	{
		insert(頑強);
	}

	if(preweak < 0 && currentweak >= 0)
	{
		cure_back_force(頑強);
	}
	if(preweak <= 0 && currentweak > 0)
	{
		go_bad(軟弱);
	}

	//weak_refresh();
	return true;
}
bool cCondition::軟弱頑強cConditionChip::頑強追加(int power,int turn)
{
	//if(currentweak > 0) currentweak = 0;
	int preweak = currentweak;
	if(currentweak > 0)
	{
		currentweak = 0;
	}
	currentweak -= power;
	if(currentweak < -Maxweak()) currentweak = -Maxweak();

	if(weak_turn_count <= 0) weak_turn_count = turn;
	else weak_turn_count = min(weak_turn_count,turn);
	
	erase(軟弱);
	erase(頑強);
	if(currentweak > 0)
	{
		insert(軟弱);
	}
	else if (currentweak < 0)
	{
		insert(頑強);
	}

	if(preweak < 0 && currentweak >= 0)
	{
		cure_back_force(頑強);
	}
	if(preweak <= 0 && currentweak > 0)
	{
		go_bad(軟弱);
	}

	//weak_refresh();
	return true;
}
double cCondition::軟弱頑強cConditionChip::守係数()
{
	if(currentweak < 0)
	{//頑強
		return 1.0 + -currentweak/(double)Maxweak();
	}
	else if(currentweak > 0)
	{//軟弱
		return 1.0 + -currentweak/(double)Maxweak();
	}
	return 1.0;
}
void cCondition::軟弱頑強cConditionChip::setConditionExplainType_Valiable(std::map<tstring, StyleString>& val)
{
	val[_T("Turn")] = setStyle(turncount() + 1);
	val[_T("Num")] = setStyle(abs(currentweak));
}
//----------------------------------------------
//目薬関係
//----------------------------------------------	
bool cCondition::目薬追加()
{
	目薬Chip.set_on();
	return true;
}
bool cCondition::目薬解除()
{
	目薬Chip.set_off();
	return true;
}
/*
//----------------------------------------------
//封印関係
//----------------------------------------------	
bool cCondition::process_fuuin()
{
	if(封印状態())
	{//状態異常
		fuuin_turn_count--;
		if(fuuin_turn_count < 0)
		{//状態異常切れ


			if(self() == sg_pDungeonSystem->pPlayerChara())
			{
				sg_pDungeonSystem->メッセージ(g_Lang(_T("封印元通りメッセージ")));//_T("速度が元にもどった。\n"));
			}

			fuuin_turn_count = TURN_NOTBAD;
			
			fuuin_refresh();
		}
	}

	return true;
}

int cCondition::fuuin_refresh()
{

	//速度表記初期化
	erase(封印);


	//設定
	if(fuuin_turn_count > 0)
	{
		insert(封印);
	}

	//エモーション表示更新

	self()->emotion.erase(EMOTION_FUUIN);

	if(封印状態())
	{
		self()->emotion.insert(EMOTION_FUUIN);
	}


	//アニメ
	sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(
		&self()->visibleemotion,
		self()->emotion);


	return true;
}

bool cCondition::封印追加(int turn)
{
	if(turn < 0)
	{//効果切れ
		fuuin_turn_count = -1;
	}
	else
	{
		if(!count(封印))
		{
			fuuin_turn_count = turn-1;
		}
	}
	return fuuin_refresh();
}
//----------------------------------------------
//空振り関係
//----------------------------------------------	
bool cCondition::process_karaburi()
{
	if(空振り状態())
	{//状態異常
		karaburi_turn_count--;
		if(karaburi_turn_count < 0)
		{//状態異常切れ


			if(self() == sg_pDungeonSystem->pPlayerChara())
			{
				sg_pDungeonSystem->メッセージ(g_Lang(_T("空振り元通りメッセージ")));//_T("速度が元にもどった。\n"));
			}

			karaburi_turn_count = TURN_NOTBAD;
			
			karaburi_refresh();
		}
	}

	return true;
}

int cCondition::karaburi_refresh()
{

	//速度表記初期化
	erase(空振り);


	//設定
	if(karaburi_turn_count > 0)
	{
		insert(空振り);
	}

	//エモーション表示更新

	self()->emotion.erase(EMOTION_KARABURI);

	if(空振り状態())
	{
		self()->emotion.insert(EMOTION_KARABURI);
	}


	//アニメ
	sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(
		&self()->visibleemotion,
		self()->emotion);


	return true;
}

bool cCondition::空振り追加(int turn)
{
	if(turn < 0)
	{//効果切れ
		karaburi_turn_count = -1;
	}
	else
	{
		if(!count(空振り))
		{
			karaburi_turn_count = turn-1;
		}
	}
	return karaburi_refresh();
}
*/
//----------------------------------------------
//やりすごし関係
//----------------------------------------------
//主人公用の現在のエモーションをセットする関数
bool cCondition::やりすごしcConditionChip::set_emotion()
{
	if(count(やりすごし))
	{
		self()->Condition.anotherSurfaceIDchange = 3000;
	}
	return cConditionChipTurn::set_emotion();
};

//敵用の次の行動の時のエモーションを予測してセットする関数
bool cCondition::やりすごしcConditionChip::set_predict_emotion()
{
	if(count(やりすごし))
	{
		self()->Condition.anotherSurfaceIDchange = 3000;
	}
	return cConditionChipTurn::set_predict_emotion();
};

//エモーションを消す関数
bool cCondition::やりすごしcConditionChip::erase_emotion()
{
	//self()->Condition.anotherSurfaceIDchange = -1;
	self()->Condition.anotherSurfaceIDchange = -1;
	return cConditionChipTurn::erase_emotion();
};
//turnターン異常状態になる。
bool cCondition::やりすごしcConditionChip::turn_on(int turn)
{
	
	if(turn >= 0)
	{
		if(overWrite() || !count(type()))
		{
			turn_count_ = turn;
			//異常状態になったときの特殊処理
			insert(type());
			
			pcCharacter pchara = sg_pDungeonSystem->DataBase.GetSampleCharacter(3000);
			if(pchara!=NULL) pchara->Init(sg_pDungeonSystem->pDevice_D3D,pchara);
				
			if( EMOTION_NULL != emotion())
			{
				self()->emotion.insert(emotion());
				self()->Condition.anotherSurfaceID = 3000;
			}
			go_bad(type());	
			return true;
		}
	}
	else
	{
		turn_count_ = TURN_NOTBAD;
		//直ったときの特殊処理
		erase(type());
		if( EMOTION_NULL != emotion())
		{
			self()->emotion.erase(emotion());
			self()->Condition.anotherSurfaceID = -1;
		}
		cure_back_force(type());
		やりすごし解除効果();
		return false;
	}
	return false;
}
bool cCondition::やりすごしcConditionChip::やりすごし解除効果()
{

	if(self() == sg_pDungeonSystem->pPlayerChara())
	{
		vector<pcLandform> outputlist;
		sg_pDungeonSystem->キャラクター隣接地形リスト(self(), outputlist);
		int i;
		for(i=0;i<outputlist.size();i++)
		{
			if(outputlist[i]->pOnChar != NULL && sg_pDungeonSystem->キャラクター敵対判定(sg_pDungeonSystem->pPlayerChara(),outputlist[i]->pOnChar))
				sg_pDungeonSystem->びっくり要請(outputlist[i]->pOnChar,1,0);
		}
	}
	return true;
}
/*
bool cCondition::process_yarisugosi()
{
	if(やりすごし状態())
	{//状態異常
		yarisugosi_turn_count--;
		if(yarisugosi_turn_count < 0)
		{//状態異常切れ


			やりすごし解除();

		}
	}

	return true;
}

int cCondition::yarisugosi_refresh()
{

	//速度表記初期化
	erase(やりすごし);


	//設定
	if(yarisugosi_turn_count > 0)
	{
		insert(やりすごし);
	}

	//エモーション表示更新

	//self()->emotion.erase(EMOTION_KARABURI);

	int anotherSurfaceIDchange = -1;

	if(やりすごし状態())
	{
		anotherSurfaceIDchange = 3000;
	}


	//アニメ
	sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(
		&anotherSurfaceID,
		anotherSurfaceIDchange);


	return true;
}

bool cCondition::やりすごし追加(int turn)
{
	pcCharacter pchara = sg_pDungeonSystem->DataBase.GetSampleCharacter(3000);
	if(pchara!=NULL) pchara->Init(sg_pDungeonSystem->pDevice_D3D,pchara);
	yarisugosi_turnmemo = yarisugosi_turn_count = turn;
	return yarisugosi_refresh();
}
bool cCondition::やりすごし解除()
{

	やりすごしChip.set_off();

	if(self() == sg_pDungeonSystem->pPlayerChara())
	{
		vector<pcLandform> outputlist;
		sg_pDungeonSystem->キャラクター隣接地形リスト(self(), outputlist);
		int i;
		for(i=0;i<outputlist.size();i++)
		{
			if(outputlist[i]->pOnChar != NULL && sg_pDungeonSystem->キャラクター敵対判定(sg_pDungeonSystem->pPlayerChara(),outputlist[i]->pOnChar))
				sg_pDungeonSystem->びっくり要請(outputlist[i]->pOnChar,1,0);
		}
	}
	return true;
}
*/
/*
//----------------------------------------------
//貧乏関係
//----------------------------------------------	
bool cCondition::process_binbou()
{
	if(貧乏状態())
	{//状態異常
		binbou_turn_count--;
		if(binbou_turn_count < 0)
		{//状態異常切れ


			if(self() == sg_pDungeonSystem->pPlayerChara())
			{
				sg_pDungeonSystem->メッセージ(g_Lang(_T("貧乏元通りメッセージ")));//_T("速度が元にもどった。\n"));
			}

			binbou_turn_count = TURN_NOTBAD;
			
			binbou_refresh();
		}
	}

	return true;
}

int cCondition::binbou_refresh()
{

	//速度表記初期化
	erase(貧乏);


	//設定
	if(binbou_turn_count > 0)
	{
		insert(貧乏);
	}

	//エモーション表示更新

	self()->emotion.erase(EMOTION_BINBOU);

	if(貧乏状態())
	{
		self()->emotion.insert(EMOTION_BINBOU);
	}


	//アニメ
	sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(
		&self()->visibleemotion,
		self()->emotion);


	return true;
}

bool cCondition::貧乏追加(int turn)
{
	if(turn < 0)
	{//効果切れ
		binbou_turn_count = -1;
	}
	else
	{
		if(!count(貧乏))
		{
			binbou_turn_count = turn-1;
		}
	}
	return binbou_refresh();
}
//----------------------------------------------
//氷付け関係
//----------------------------------------------	
bool cCondition::process_koorituke()
{
	if(氷付け状態())
	{//状態異常
		koorituke_turn_count--;
		if(koorituke_turn_count < 0)
		{//状態異常切れ


			if(self() == sg_pDungeonSystem->pPlayerChara())
			{
				sg_pDungeonSystem->メッセージ(g_Lang(_T("氷付け元通りメッセージ")));//_T("速度が元にもどった。\n"));
			}

			koorituke_turn_count = TURN_NOTBAD;
			
			koorituke_refresh();
		}
	}

	return true;
}

int cCondition::koorituke_refresh()
{

	//速度表記初期化
	erase(氷付け);


	//設定
	if(koorituke_turn_count > 0)
	{
		insert(氷付け);
	}

	//エモーション表示更新

	self()->emotion.erase(EMOTION_KOORITUKE);

	if(氷付け状態())
	{
		self()->emotion.insert(EMOTION_KOORITUKE);
	}


	//アニメ
	sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(
		&self()->visibleemotion,
		self()->emotion);


	return true;
}

bool cCondition::氷付け追加(int turn)
{
	if(turn < 0)
	{//効果切れ
		koorituke_turn_count = -1;
	}
	else
	{
		if(!count(氷付け))
		{
			koorituke_turn_count = turn;
		}
	}
	return koorituke_refresh();
}
*/
bool cCondition::氷付けで移動失敗()
{
	if(self() == sg_pDungeonSystem->pPlayerChara())
	{
		std::map<tstring, StyleString > val;
		g_Langメッセージ(_T("氷付けで移動失敗メッセージ"),val);
		sg_pDungeonSystem->ターンエンド();
		EffectFunctions::氷付けエフェクト_短(self()->placeX,self()->placeY);
	}
	
	return true;
}

//----------------------------------------------
//死の誘い関係
//----------------------------------------------	
bool cCondition::死の誘いcConditionChip::process()
{
	if(is_on())
	{//状態異常
		sasoi_turn_count--;
		if(turn_off_judge(sasoi_turn_count))
		{//状態異常切れ
			erase(死の誘い);
			cure_back_force(死の誘い);
			
			if(sasoi_subject_.lock() != NULL && !sasoi_subject_.lock()->死亡()
				|| sasoi_subject_.lock() == NULL)
			{
				if(self() == sg_pDungeonSystem->pPlayerChara())
				{
					sg_pDungeonSystem->メッセージ(g_Lang(_T("死の誘い発動メッセージ")));//_T("速度が元にもどった。\n"));
				}

				
				sg_pDungeonSystem->即死要請(self());
			}
			sasoi_turn_count = TURN_NOTBAD;
			
			//sasoi_refresh();
		}
		if(sasoi_subject_.lock() != NULL && sasoi_subject_.lock()->死亡())
		{//相手が死亡
			sasoi_turn_count = 0;
			erase(死の誘い);
			cure_back_force(死の誘い);

			if(self() == sg_pDungeonSystem->pPlayerChara())
			{
				sg_pDungeonSystem->メッセージ(g_Lang(_T("死の誘い元通りメッセージ")));//_T("速度が元にもどった。\n"));
			}
			
		}
		//sasoi_refresh();
	}

	return true;
}
bool cCondition::死の誘いcConditionChip::Draw_sasoi(IDirect3DDevice9 *pDev)
{
	cNumField nf;
	nf.Init(pDev);

	nf.FontHeightSize = 32;
	nf.FontWidthSize = 16;

	nf.Fontindex = 3;
	sasoi_visual_counter += 0.02;
	nf.color.ARGB(128+sin(sasoi_visual_counter)*64,255,40,20);

	nf.Value_ = sasoi_turn_count_visible_;
	
	int CenterX = self()->GetDrawDifference_dotX()*MAPTEXPOWER + 
		MAPDRAWCENTERX + MAPTEXBOXSIZE*MAPTEXPOWER*(self()->visibleplace.x - self()->mapForcus.x) + 16;
	
	int CenterY = self()->GetDrawDifference_dotY()*MAPTEXPOWER + 
		-self()->GetDrawHeadtall_dotY()*MAPTEXPOWER +
		MAPDRAWCENTERY + MAPTEXBOXSIZE*MAPTEXPOWER*(self()->visibleplace.y - self()->mapForcus.y) -32	
		+ MAPTEXBOXSIZE*MAPTEXPOWER/2;
	nf.position.set(CenterX,CenterY,0,0);

	nf.Draw(pDev);
	return true;
}
bool cCondition::死の誘いcConditionChip::set_emotion()
{
	/*

	//速度表記初期化
	erase(死の誘い);


	//設定
	if(sasoi_turn_count > 0)
	{
		insert(死の誘い);
	}

	//エモーション表示更新

	//self()->emotion.erase(EMOTION_KOORITUKE);

	if(死の誘い状態())
	{
		//self()->emotion.insert(EMOTION_KOORITUKE);
	}
*/

	sasoi_turn_count_visible_ = sasoi_turn_count;
		
/*
	if(count(死の誘い))
	{
		self()->emotion.insert(EMOTION_KOORITUKE);
	}
*/
	//アニメ
	//sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(
	//	&sasoi_turn_count_visible_,
	//	sasoi_turn_count);


	return true;
}
bool cCondition::死の誘いcConditionChip::set_predict_emotion()
{
/*
	if(count(死の誘い))
	{
		self()->emotion.insert(EMOTION_KOORITUKE);
	}
*/

	//アニメ
	//sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(
	//	&sasoi_turn_count_visible_,
	//	sasoi_turn_count);


	return true;
}
bool cCondition::死の誘いcConditionChip::erase_emotion()
{

	self()->emotion.erase(EMOTION_KOORITUKE);


	return true;
}

bool cCondition::死の誘いcConditionChip::turn_on(int turn)
{
	if(turn < 0)
	{//効果切れ
		sasoi_turn_count = -1;
		erase(死の誘い);
		cure_back_force(死の誘い);

	}
	else
	{
		sasoi_turn_count = turn;
		insert(死の誘い);
		go_bad(死の誘い);

	}

	return true;
	
}
bool cCondition::死の誘いcConditionChip::死の誘い追加(int turn, pcCharacter sasoi_subject)
{
	sasoi_subject_ = wpcCharacter(sasoi_subject);

	return turn_on(turn);
}

//----------------------------------------------
//みがわり関係
//----------------------------------------------	
bool cCondition::みがわりcConditionChip::process()
{
	if(is_on())
	{//状態異常
		migawari_turn_count--;
		if(turn_off_judge(migawari_turn_count))
		{//状態異常切れ


			みがわり解除();

		}
	}

	return true;
}
/*
int cCondition::みがわりcConditionChip::migawari_refresh()
{

	//速度表記初期化
	erase(みがわり);


	//設定
	if(migawari_turn_count > 0)
	{
		insert(みがわり);
	}

	//エモーション表示更新

	self()->emotion.erase(EMOTION_MIGAWARI);
	int anotherSurfaceIDchange = -1;


	if(みがわり状態())
	{
		self()->emotion.insert(EMOTION_MIGAWARI);
		anotherSurfaceIDchange = migawari_surfaceID;
	}


	//アニメ
	sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(
		&anotherSurfaceID,
		anotherSurfaceIDchange);
	//アニメ
	sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(
		&self()->visibleemotion,
		self()->emotion);

	return true;
}
*/

//主人公用の現在のエモーションをセットする関数
bool cCondition::みがわりcConditionChip::set_emotion()
{
	if(count(みがわり))
	{
		self()->emotion.insert(EMOTION_MIGAWARI);
		self()->Condition.anotherSurfaceIDchange = migawari_surfaceID;
	}
	return true;
}
//敵用の次の行動の時のエモーションを予測してセットする関数
bool cCondition::みがわりcConditionChip::set_predict_emotion()
{
	int temp = migawari_turn_count;
	temp--;
	if(!turn_off_judge(temp))
	{
		self()->emotion.insert(EMOTION_MIGAWARI);
		self()->Condition.anotherSurfaceIDchange = migawari_surfaceID;
	}
	return true;
}		
//エモーションを消す関数
bool cCondition::みがわりcConditionChip::erase_emotion()
{
	self()->emotion.erase(EMOTION_MIGAWARI);
	self()->Condition.anotherSurfaceIDchange = -1;
	return true;
}
bool cCondition::みがわりcConditionChip::turn_on(int turn)
{
	if(turn < 0)
	{//効果切れ
		migawari_turn_count = -1;
		erase(みがわり);
		cure_back_force(みがわり);
	}
	else
	{
		migawari_turn_count = turn;
		insert(みがわり);
		go_bad(みがわり);
	}

	return true;
	/*
	migawari_turn_count = turn;
	if(pchara!=NULL)
	{

		//pcCharacter pchara2 = sg_pDungeonSystem->DataBase.GetSampleCharacter(migawari_surfaceID);
		//if(pchara2!=NULL) pchara2->Init(sg_pDungeonSystem->pDevice_D3D,pchara2);
	}
	//return migawari_refresh();
	return true;
	*/
}
bool cCondition::みがわりcConditionChip::みがわり追加(int turn, pcCharacter pchara)
{
	if(pchara == NULL)
	{
		migawari_surfaceID = 0;
	}
	else
	{
		migawari_surfaceID = pchara->ID();
	}
	return turn_on(turn);
}
bool cCondition::みがわりcConditionChip::みがわり解除()
{

	if(self() == sg_pDungeonSystem->pPlayerChara())
	{
		sg_pDungeonSystem->メッセージ(g_Lang(_T("みがわり元通りメッセージ")));//_T("速度が元にもどった。\n"));
	}

	migawari_turn_count = 0;
	erase(みがわり);
	cure_back_force(みがわり);
	//migawari_refresh();
	return true;
}

//----------------------------------------------
//変身関係
//----------------------------------------------	
bool cCondition::変身cConditionChip::process()
{
	if(is_on())
	{//状態異常
		hensin_turn_count--;
		if(turn_off_judge(hensin_turn_count))
		{//状態異常切れ


			変身解除();

		}
	}

	return true;
}

//主人公用の現在のエモーションをセットする関数
bool cCondition::変身cConditionChip::set_emotion()
{
	if(count(変身))
	{
		self()->Condition.anotherSurfaceIDchange = hensin_surfaceID;
	}
	return true;
}
//敵用の次の行動の時のエモーションを予測してセットする関数
bool cCondition::変身cConditionChip::set_predict_emotion()
{
	int temp = hensin_turn_count;
	temp--;
	if(!turn_off_judge(temp))
	{
		self()->Condition.anotherSurfaceIDchange = hensin_surfaceID;
	}
	return true;
}		
//エモーションを消す関数
bool cCondition::変身cConditionChip::erase_emotion()
{
	self()->Condition.anotherSurfaceIDchange = -1;
	return true;
}
bool cCondition::変身cConditionChip::turn_on(int turn)
{
	if(turn < 0)
	{//効果切れ
		hensin_turn_count = -1;
		erase(変身);
		cure_back_force(変身);
	}
	else
	{
		hensin_turn_count = turn;
		insert(変身);
		go_bad(変身);
	}

	return true;
}
bool cCondition::変身cConditionChip::変身追加(int turn, pcCharacter pchara)
{
	if(pchara == NULL)
	{
		hensin_surfaceID = 0;
	}
	else
	{
		hensin_surfaceID = pchara->ID();
	}
	return turn_on(turn);
}
bool cCondition::変身cConditionChip::変身解除()
{

	if(self() == sg_pDungeonSystem->pPlayerChara())
	{
		sg_pDungeonSystem->メッセージ(g_Lang(_T("変身元通りメッセージ")));//_T("速度が元にもどった。\n"));
	}

	hensin_turn_count = 0;
	erase(変身);
	cure_back_force(変身);
	//hensin_refresh();
	return true;
}
/*
//----------------------------------------------
//無敵関係
//----------------------------------------------	
bool cCondition::process_muteki()
{
	if(無敵状態())
	{//状態異常
		muteki_turn_count--;
		if(muteki_turn_count < 0)
		{//状態異常切れ


			if(self() == sg_pDungeonSystem->pPlayerChara())
			{
				sg_pDungeonSystem->メッセージ(g_Lang(_T("無敵元通りメッセージ")));//_T("速度が元にもどった。\n"));
			}

			muteki_turn_count = 0;
			
			muteki_refresh();
		}
	}

	return true;
}

int cCondition::muteki_refresh()
{

	//速度表記初期化
	erase(無敵);


	//設定
	if(muteki_turn_count > 0)
	{
		insert(無敵);
	}

	//エモーション表示更新

	self()->emotion.erase(EMOTION_MUTEKI);

	if(無敵状態())
	{
		self()->emotion.insert(EMOTION_MUTEKI);
	}


	//アニメ
	sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(
		&self()->visibleemotion,
		self()->emotion);


	return true;
}

bool cCondition::無敵追加(int turn)
{
	if(turn < 0)
	{//効果切れ
		muteki_turn_count = -1;
	}
	else
	{
		muteki_turn_count = turn;
	}
	return muteki_refresh();
}
//----------------------------------------------
//擬態関係
//----------------------------------------------	
bool cCondition::process_gitai()
{
	if(擬態状態())
	{//状態異常
		gitai_turn_count--;
		if(gitai_turn_count < 0)
		{//状態異常切れ


			if(self() == sg_pDungeonSystem->pPlayerChara())
			{
				sg_pDungeonSystem->メッセージ(g_Lang(_T("擬態元通りメッセージ")));//_T("速度が元にもどった。\n"));
			}

			gitai_turn_count = TURN_NOTBAD;
			
			gitai_refresh();
		}
	}

	return true;
}

int cCondition::gitai_refresh()
{

	//速度表記初期化
	erase(擬態);


	//設定
	if(gitai_turn_count > 0)
	{
		insert(擬態);
	}

	//エモーション表示更新

	self()->emotion.erase(EMOTION_GITAI);

	if(擬態状態())
	{
		self()->emotion.insert(EMOTION_GITAI);
	}


	//アニメ
	sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(
		&self()->visibleemotion,
		self()->emotion);


	return true;
}

bool cCondition::擬態追加(int turn)
{
	if(turn < 0)
	{//効果切れ
		gitai_turn_count = -1;
	}
	else
	{
		gitai_turn_count = turn;
	}
	return gitai_refresh();
}

//----------------------------------------------
//狂乱関係
//----------------------------------------------	
bool cCondition::process_kyouran()
{
	if(狂乱状態())
	{//状態異常
		kyouran_turn_count--;
		if(kyouran_turn_count < 0)
		{//状態異常切れ


			if(self() == sg_pDungeonSystem->pPlayerChara())
			{
				map<tstring, StyleString> valiable;
				valiable[_T("Chara")] = sg_pDungeonSystem->pPlayerChara()->ShortName();
			
				sg_pDungeonSystem->メッセージ(g_Lang(_T("狂乱元通りメッセージ"),valiable));//_T("速度が元にもどった。\n"));
			}

			kyouran_turn_count = TURN_NOTBAD;
			
			kyouran_refresh();
		}
	}

	return true;
}

int cCondition::kyouran_refresh()
{

	//速度表記初期化
	erase(狂乱);


	//設定
	if(kyouran_turn_count > 0)
	{
		insert(狂乱);
	}

	//エモーション表示更新

	self()->emotion.erase(EMOTION_KYOURAN);

	if(狂乱状態())
	{
		self()->emotion.insert(EMOTION_KYOURAN);
	}


	//アニメ
	sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(
		&self()->visibleemotion,
		self()->emotion);


	return true;
}

bool cCondition::狂乱追加(int turn)
{
	if(turn < 0)
	{//効果切れ
		kyouran_turn_count = -1;
	}
	else
	{
		if(!count(狂乱))
		{
			kyouran_turn_count = turn;
		}
	}
	return kyouran_refresh();
}

//----------------------------------------------
//臆病関係
//----------------------------------------------	
bool cCondition::process_okubyou()
{
	if(臆病状態())
	{//状態異常
		okubyou_turn_count--;
		if(okubyou_turn_count < 0)
		{//状態異常切れ


			if(self() == sg_pDungeonSystem->pPlayerChara())
			{
				map<tstring, StyleString> valiable;
				valiable[_T("Chara")] = sg_pDungeonSystem->pPlayerChara()->ShortName();
			
				sg_pDungeonSystem->メッセージ(g_Lang(_T("臆病元通りメッセージ"),valiable));//_T("速度が元にもどった。\n"));
			}

			okubyou_turn_count = TURN_NOTBAD;
			
			okubyou_refresh();
		}
	}

	return true;
}

int cCondition::okubyou_refresh()
{

	//速度表記初期化
	erase(臆病);


	//設定
	if(okubyou_turn_count > 0)
	{
		insert(臆病);
	}

	//エモーション表示更新

	self()->emotion.erase(EMOTION_OKUBYOU);

	if(臆病状態())
	{
		self()->emotion.insert(EMOTION_OKUBYOU);
	}


	//アニメ
	sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(
		&self()->visibleemotion,
		self()->emotion);


	return true;
}

bool cCondition::臆病追加(int turn)
{
	if(turn < 0)
	{//効果切れ
		okubyou_turn_count = -1;
	}
	else
	{
		if(!count(臆病))
		{
			okubyou_turn_count = turn;
		}
	}
	return okubyou_refresh();
}
//----------------------------------------------
//嫉妬関係
//----------------------------------------------	
bool cCondition::process_shitto()
{
	if(嫉妬状態())
	{//状態異常
		shitto_turn_count--;
		if(shitto_turn_count < 0)
		{//状態異常切れ


			if(self() == sg_pDungeonSystem->pPlayerChara())
			{
				map<tstring, StyleString> valiable;
				valiable[_T("Chara")] = sg_pDungeonSystem->pPlayerChara()->ShortName();
			
				sg_pDungeonSystem->メッセージ(g_Lang(_T("嫉妬元通りメッセージ"),valiable));//_T("速度が元にもどった。\n"));
			}

			shitto_turn_count = TURN_NOTBAD;
			
			shitto_refresh();
		}
	}

	return true;
}

int cCondition::shitto_refresh()
{

	//速度表記初期化
	erase(嫉妬);


	//設定
	if(shitto_turn_count > 0)
	{
		insert(嫉妬);
	}

	//エモーション表示更新

	self()->emotion.erase(EMOTION_SHITTO);

	if(嫉妬状態())
	{
		self()->emotion.insert(EMOTION_SHITTO);
	}


	//アニメ
	sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(
		&self()->visibleemotion,
		self()->emotion);


	return true;
}

bool cCondition::嫉妬追加(int turn)
{
	if(turn < 0)
	{//効果切れ
		shitto_turn_count = -1;
	}
	else
	{
		shitto_turn_count = turn;
	}
	return shitto_refresh();
}
//----------------------------------------------
//鳥目関係
//----------------------------------------------	
bool cCondition::process_torime()
{
	if(鳥目状態())
	{//状態異常
		torime_turn_count--;
		if(torime_turn_count < 0)
		{//状態異常切れ


			if(self() == sg_pDungeonSystem->pPlayerChara())
			{
				map<tstring, StyleString> valiable;
				valiable[_T("Chara")] = sg_pDungeonSystem->pPlayerChara()->ShortName();
			
				sg_pDungeonSystem->メッセージ(g_Lang(_T("鳥目元通りメッセージ"),valiable));//_T("速度が元にもどった。\n"));
			}

			torime_turn_count = TURN_NOTBAD;
			
			torime_refresh();
		}
	}

	return true;
}

int cCondition::torime_refresh()
{

	//速度表記初期化
	erase(鳥目);


	//設定
	if(torime_turn_count > 0)
	{
		insert(鳥目);
	}

	//エモーション表示更新

	self()->emotion.erase(EMOTION_TORIME);

	if(鳥目状態())
	{
		self()->emotion.insert(EMOTION_TORIME);
	}


	//アニメ
	sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(
		&self()->visibleemotion,
		self()->emotion);


	return true;
}

bool cCondition::鳥目追加(int turn)
{
	if(turn < 0)
	{//効果切れ
		torime_turn_count = -1;
	}
	else
	{
		if(!count(鳥目))
		{
			torime_turn_count = turn;
		}
	}
	return torime_refresh();
}
*/

//---------------------------------------------------------
		//
//---------------------------------------------------------
// cCondition::cConditionChipVirtual
int cCondition::cConditionChipVirtual::count(異常状態 state){return self()->Condition.count(state);};
void cCondition::cConditionChipVirtual::insert(異常状態 state){self()->Condition.insert(state);};
void cCondition::cConditionChipVirtual::erase(異常状態 state){self()->Condition.erase(state);};
void cCondition::cConditionChipVirtual::setConditionExplain(pcListWindow& pclw)
{
	setConditionExplainType(pclw, type_);
}
void cCondition::cConditionChipVirtual::setConditionExplainType(pcListWindow& pclw, 異常状態 type)
{


	std::map<tstring, StyleString > val;

	int i;

	if(count(type))
	{
		setConditionExplainType_Valiable(val);

		pclw->pDroplist.push_back(pcDroping(new cStateDroping(\
			g_Lang((cCondition::condition_str_[type] + _T("キャプション")).c_str(),val),
			g_Lang((cCondition::condition_str_[type] + _T("ShortExplanation")).c_str(),val)
			)));
	}
}
void cCondition::cConditionChipVirtual::setConditionExplainType_Valiable(std::map<tstring, StyleString>& val)
{
	val[_T("Turn")] = setStyle(turncount() + 1);
}

//異常状態が自然に直ったときの特殊処理
bool cCondition::cConditionChipVirtual::cure_back_natural(異常状態 type)
{
	return self()->Condition.cure_back_natural(type);
}

//異常状態を無理やり治したときの特殊処理
bool cCondition::cConditionChipVirtual::cure_back_force(異常状態 type)
{
	return self()->Condition.cure_back_force(type);
}

//異常状態になったときの特殊処理
bool cCondition::cConditionChipVirtual::go_bad(異常状態 type)
{
	return self()->Condition.go_bad(type);
}

//---------------------------------------------------------
// cCondition::cConditionChipTurn
int cCondition::cConditionChipTurn::init(異常状態 type, int emotion, pcCharacter pchara)
{
	cCondition::cConditionChipVirtual::init(type,emotion,pchara);
	overWrite_ = true;
	turn_count_ = -1;
	return true;
}
bool cCondition::cConditionChipTurn::process()
{

	if(is_on())
	{//状態異常
		turn_count_--;
		if(turn_off_judge(turn_count_))
		{//状態異常切れ

			if(self() == sg_pDungeonSystem->pPlayerChara())
			{
				map<tstring, StyleString> valiable;
				valiable[_T("Chara")] = sg_pDungeonSystem->pPlayerChara()->ShortName();
				
				sg_pDungeonSystem->メッセージ(g_Lang((cCondition::Chipstr(type()) + _T("元通りメッセージ")).c_str(),valiable));//_T("速度が元にもどった。\n"));
			}

			turn_count_ = TURN_NOTBAD;
			erase(type());
			if( EMOTION_NULL != emotion())
			{
				self()->emotion.erase(emotion());
			}
			//直ったときの特殊処理
			cure_back_natural(type());
		}
	}
	return true;
}


//turnターン異常状態になる。
bool cCondition::cConditionChipTurn::turn_on(int turn)
{
	
	if(turn >= 0)
	{
		if(overWrite() || !count(type()))
		{
			turn_count_ = turn;
			//異常状態になったときの特殊処理
			insert(type());
			if( EMOTION_NULL != emotion())
			{
				self()->emotion.insert(emotion());
			}
			go_bad(type());	
			return true;
		}
	}
	else
	{
		turn_count_ = TURN_NOTBAD;
		//直ったときの特殊処理
		erase(type());
		if( EMOTION_NULL != emotion())
		{
			self()->emotion.erase(emotion());
		}
		cure_back_force(type());
		return false;
	}
	return false;
}
//turnターン異常状態になる。
bool cCondition::cConditionChipTurn::set_off()
{
	erase(type());
	if( EMOTION_NULL != emotion())
	{
		self()->emotion.erase(emotion());
	}
	cure_back_force(type());
	return turn_on(-1);
}

int cCondition::cConditionChipTurn::turncount()
{
	return turn_count_;
}


//エモーションを消す関数
bool cCondition::cConditionChipTurn::erase_emotion()
{
	if( EMOTION_NULL != emotion())
	{
		self()->emotion.erase(emotion());
	}
	return true;
}
//主人公用の現在のエモーションをセットする関数
bool cCondition::cConditionChipTurn::set_emotion()
{
	//エモーション表示更新
	if(!turn_off_judge(turn_count_) && EMOTION_NULL != emotion())
	{
		self()->emotion.insert(emotion());
	}
	return true;
}

//敵用の次の行動の時のエモーションを予測してセットする関数
bool cCondition::cConditionChipTurn::set_predict_emotion()
{

	int temp = turn_count_;
	temp--;
	//エモーション表示更新
	if(!turn_off_judge(temp) && EMOTION_NULL != emotion())
	{
		self()->emotion.insert(emotion());
	}
	return true;
}
//---------------------------------------------------------

bool cCondition::健康cConditionChip::go_bad(異常状態 type) {
	self()->Condition.病気追加(-1);
	return cConditionChipTurn::go_bad(type);
}

void cCondition::病気cConditionChip::become_healthfitness(int value) {
	double recoverValue = ceil(self()->MHP * healthfitnessRecoverPerHP);
	int turn = ceil(_avoidHealVolume / recoverValue);
	self()->Condition.健康追加(turn);
}

bool cCondition::病気cConditionChip::cure_back_natural(異常状態 type) {
	if(_avoidHealVolume > 0){
		become_healthfitness(_avoidHealVolume);
	}
	return cConditionChipTurn::cure_back_natural(type);
}

bool cCondition::病気cConditionChip::cure_back_force(異常状態 type){
	if(_avoidHealVolume > 0){
		become_healthfitness(_avoidHealVolume);
	}
	return cConditionChipTurn::cure_back_force(type);
}

bool cCondition::病気cConditionChip::go_bad(異常状態 type) {
	setAvoidHealVolume(0);
	self()->Condition.健康追加(-1);
	return cConditionChipTurn::go_bad(type);
}

//---------------------------------------------------------
// cCondition::cConditionChipOnOff

int cCondition::cConditionChipOnOff::init(異常状態 type, int emotion, pcCharacter pchara)
{
	cCondition::cConditionChipVirtual::init(type,emotion,pchara);

	on_ = false;
	return true;
}
bool cCondition::cConditionChipOnOff::process()
{
	return true;
}

//異常状態になる。
bool cCondition::cConditionChipOnOff::set_on()
{
	insert(type());
	if(EMOTION_NULL != emotion())
	{
		self()->emotion.insert(emotion());
	}
	go_bad(type());	
	on_ = true;
	return true;
}
//異常状態になる。
bool cCondition::cConditionChipOnOff::set_off()
{
	erase(type());
	if(EMOTION_NULL != emotion())
	{
		self()->emotion.erase(emotion());
	}
	cure_back_force(type());

	on_ = false;
	return true;
}
/*
int cCondition::cConditionChipOnOff::turncount()
{
	return GAME_TURN_GAMEOVER;
}
*/

//エモーションを消す関数
bool cCondition::cConditionChipOnOff::erase_emotion()
{
	if( EMOTION_NULL != emotion())
	{
		self()->emotion.erase(emotion());
	}
	return true;
}
//主人公用の現在のエモーションをセットする関数
bool cCondition::cConditionChipOnOff::set_emotion()
{
	//エモーション表示更新
	if(is_on() && EMOTION_NULL != emotion())
	{
		self()->emotion.insert(emotion());
	}
	return true;
}

//敵用の次の行動の時のエモーションを予測してセットする関数
bool cCondition::cConditionChipOnOff::set_predict_emotion()
{
	//エモーション表示更新
	if(is_on() && EMOTION_NULL != emotion())
	{
		self()->emotion.insert(emotion());
	}
	return true;
}


//---------------------------------------------------------
//
//------------------------------------------------------------------
//インタフェース
//チップ参照関数
/*
cCondition::cConditionChipVirtual& cCondition::Chip(異常状態 type)
{
	//cCondition::cConditionChip temp;
	return *(p_condition_chipvector_[type]);
}
*/

#define DEF_InitChipstr_set(type) \
	condition_str_[type] = _T(#type)
//チップのSTRを初期化する。ゲーム開始時に一度だけ呼ぶ。

tstring cCondition::condition_str_[CONDITION_NUM];
void cCondition::InitChipstr()
{
	DEF_InitChipstr_set(異常状態_初期化無し);

	DEF_InitChipstr_set(空腹);

	DEF_InitChipstr_set(超鈍足);
	DEF_InitChipstr_set(鈍足);
	DEF_InitChipstr_set(等速);
	DEF_InitChipstr_set(二倍速);
	DEF_InitChipstr_set(三倍速);

	DEF_InitChipstr_set(眠り);
	DEF_InitChipstr_set(バクスイ);
	DEF_InitChipstr_set(浅い居眠り);
	DEF_InitChipstr_set(深い居眠り);

	DEF_InitChipstr_set(びっくり);

	DEF_InitChipstr_set(金縛り);

	DEF_InitChipstr_set(泥酔);
	DEF_InitChipstr_set(空振り);
	DEF_InitChipstr_set(やりすごし);
	DEF_InitChipstr_set(貧乏);
	DEF_InitChipstr_set(氷付け);

	DEF_InitChipstr_set(死の誘い);
	DEF_InitChipstr_set(みがわり);
	DEF_InitChipstr_set(擬態);

	DEF_InitChipstr_set(狂乱);
	DEF_InitChipstr_set(嫉妬);
	DEF_InitChipstr_set(臆病);
	DEF_InitChipstr_set(鳥目);
	
	DEF_InitChipstr_set(健康);
	DEF_InitChipstr_set(病気);

	DEF_InitChipstr_set(脱力);
	DEF_InitChipstr_set(病気);

	DEF_InitChipstr_set(軟弱);
	DEF_InitChipstr_set(頑強);

	DEF_InitChipstr_set(無敵);

	DEF_InitChipstr_set(目薬);
	DEF_InitChipstr_set(封印);

}

const tstring&cCondition::Chipstr(異常状態 type)
{
	return condition_str_[type];
}
