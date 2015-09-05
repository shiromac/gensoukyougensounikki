#pragma once


#include "cCharacter.h"

#include "cListWindow.h"

#include <set>
#include <vector>
using namespace std;


//----モジュール要求----
//主人公用の現在のエモーションをセットする関数
//敵用の未来のエモーションをセットする関数
//を提供する。
//
//----詳細設計要求----
//主人公用の現在のエモーションをセットする関数を
//ポリもーフィズムで提供する
//提供された関数は配列で走査できる
//異常状態とその文字列は一対一対応できる。
//その状態かどうかはcount()で判定することが出来る。
//対外的には専用インターフェイスが存在する。

enum 異常状態
{
	異常状態_初期化無し,

	空腹,

	超鈍足,
	鈍足,
	等速,
	二倍速,
	三倍速,

	眠り,
	バクスイ,
	浅い居眠り,
	深い居眠り,

	びっくり,

	金縛り,

	泥酔,
	空振り,
	やりすごし,
	貧乏,
	氷付け,

	死の誘い,
	みがわり,
	擬態,
	変身,

	狂乱,
	嫉妬,
	臆病,
	鳥目,
	無意識,

	脱力,
	元気,

	軟弱,
	頑強,

	無敵,

	目薬,
	封印,
	CONDITION_NUM
};

//-----------------------------
//空腹
//-----------------------------
#define STOMACH_MSG_FULL 0
#define STOMACH_MSG_AFEW 1
#define STOMACH_MSG_FEW 2
#define STOMACH_MSG_ENPTY1 3
#define STOMACH_MSG_ENPTY2 4
#define STOMACH_MSG_ENPTY3 5

class cCondition :
	protected set<int>
{
public:
	cCondition(void);
public:
	virtual ~cCondition(void);
	virtual bool init(pcCharacter pself);

	//説明
	virtual void MakeExplainWindow(IDirect3DDevice9 *pDev);
	virtual void explain(pcListWindow& pclw);
	virtual void CutIn(タイミング timing, cValiableField& valiable);

	virtual pcCharacter self();
protected:
	wpcCharacter weak_pself;	
public:

	//set<int> Set;//異常状態

	//一ターンに一度実行されることになってます
	virtual bool process();

	//エモーションをセットする(主人公用と割り込み用)
	virtual bool refresh_emotion();

	//エモーションをセットする(主人公以外用)
	virtual bool refresh_predict_emotion();

	//エモーションアニメを更新する
	virtual bool refresh_emotion_anime();

	virtual bool Draw(IDirect3DDevice9 *pDev);
	
	SharedValiable_likeInt<int> anotherSurfaceID;
	int anotherSurfaceIDchange;

	virtual pcCharacter anotherSurface();
	//---------------------------------
	//共通
	//---------------------------------

	virtual bool 飛ばし危険();

	//ただし鈍足で行動不能であるを除く
	virtual bool 行動不能である();
	//ただし鈍足で行動不能であるを除く
	virtual bool 行動不能である_visual();
	//正常に動けません
	virtual bool 混乱状態である();

	//見えません
	virtual bool 透明状態である();

	//無敵
	virtual bool 無敵状態である();

	virtual bool とても強い刺激();//直接攻撃

	//---------------------------------------------
	//Chip
	class cConditionChipVirtual
	{
	private://隠蔽しましょう
		異常状態 type_;
		int emotion_;
		wpcCharacter wpchara_;
	public:
		//デフォルトコンストラクタ
		cConditionChipVirtual(){};
		//cConditionChipVirtual(異常状態 type, int emotion) = 0;
	public:
		virtual ~cConditionChipVirtual(void){};

		virtual int init(異常状態 type, int emotion, pcCharacter pchara)
		{
			type_ = type;
			emotion_ = emotion;
			wpchara_ = wpcCharacter(pchara);
			return true;
		};

		inline pcCharacter self(void){return wpchara_.lock();};

	protected:
		void setConditionExplainType(pcListWindow& pclw, 異常状態 type);
		virtual void setConditionExplainType_Valiable(std::map<tstring, StyleString>& val);

	public:
		//ゲッター
		int emotion(){return emotion_;};
		異常状態 type(){return type_;};

		//毎ターンの処理
		virtual bool process() = 0;

		//エモーションを消す関数
		virtual bool erase_emotion() = 0;

		//主人公用の現在のエモーションをセットする関数
		virtual bool set_emotion() = 0;
		
		//敵用の次の行動の時のエモーションを予測してセットする関数
		virtual bool set_predict_emotion() = 0;
				
		//
		virtual int count(異常状態 state);
		virtual void insert(異常状態 state);
		virtual void erase(異常状態 state);


		virtual void setConditionExplain(pcListWindow& pclw);


		//残りターンを出力する
		virtual int turncount(){return 9999;};
		//残りターンを出力する
		virtual int is_on(){return count(type_);};



		//異常状態が自然に直ったときの特殊処理
		bool cure_back_natural(異常状態 type);

		//異常状態を無理やり治したときの特殊処理
		bool cure_back_force(異常状態 type);

		//異常状態になったときの特殊処理
		bool go_bad(異常状態 type);
	};
	friend cConditionChipVirtual;

	//-------------------------------------------
	//ターンでだんだん下がる系
	class cConditionChipTurn
		: public cConditionChipVirtual
	{
	public:
		//デフォルトコンストラクタ
		cConditionChipTurn()//:
			//cConditionChipVirtual(異常状態_初期化無し, 0),
			//turn_count_(0)
		{};
		/*
		cConditionChipTurn(異常状態 type, int emotion):
			cConditionChipVirtual(type, emotion),
			turn_count_(emotion)
		{};
		*/
	public:
		virtual ~cConditionChipTurn(void){};
		virtual int init(異常状態 type, int emotion, pcCharacter pchara);
		
	protected://隠蔽しましょう
		int turn_count_;
		int overWrite_;
	protected:
		virtual bool turn_off_judge(const int param){return (param < 0);};
	public:
		

		//毎ターンの処理
		virtual bool process();

		//主人公用の現在のエモーションをセットする関数
		virtual bool set_emotion();
		
		//敵用の次の行動の時のエモーションを予測してセットする関数
		virtual bool set_predict_emotion();
		
		//エモーションを消す関数
		virtual bool erase_emotion();

		//ターンをセットする
		virtual bool turn_on(int turn);

		//オフ
		virtual bool set_off();


		//残りターンを出力する
		virtual int turncount();

		//重ね上書きされる
		virtual bool overWrite(){return overWrite_;};
		virtual void setOverWrite(bool overWrite){overWrite_ = overWrite;};

	};


	//-------------------------------------------
	//オンオフのみ系
	class cConditionChipOnOff
		: public cConditionChipVirtual
	{
	public:
		//デフォルトコンストラクタ
		cConditionChipOnOff()//:
			//cConditionChipVirtual(異常状態_初期化無し, 0),
			//on_(false)
		{};
		/*
		cConditionChipOnOff(異常状態 type, int emotion):
			cConditionChipVirtual(type, emotion),
			on_(false)
		{};
		*/
	public:
		virtual ~cConditionChipOnOff(void){};
		virtual int init(異常状態 type, int emotion, pcCharacter pchara);
		
	private://隠蔽しましょう

		bool on_;
	public:
		//毎ターンの処理
		virtual bool process();

		//エモーションを消す関数
		virtual bool erase_emotion();

		//主人公用の現在のエモーションをセットする関数
		virtual bool set_emotion();
		
		//敵用の次の行動の時のエモーションを予測してセットする関数
		virtual bool set_predict_emotion();

		//オン
		virtual bool set_on();
		
		//オフ
		virtual bool set_off();
		
	};

	//---------------------------------
	//インタフェース
	//---------------------------------
	//チップ参照関数
	//cConditionChipVirtual* Chip(異常状態 type);
	static const tstring&  Chipstr(異常状態 type);
	//チップのSTRを初期化する。ゲーム開始時に一度だけ呼ぶ。
	static void InitChipstr();

	//---------------------------------
	//メンバ
	//---------------------------------
protected:
	vector<cConditionChipVirtual*> p_condition_chipvector_;
	static tstring condition_str_[CONDITION_NUM];

	//cConditionChip 空腹Chip;
	//cConditionChip 超鈍足Chip;
	//cConditionChip 鈍足Chip;
	//cConditionChip 等速Chip;
	//cConditionChip 二倍速Chip;
	//cConditionChip 三倍速Chip;
	//cConditionChip 速度Chip;

	//cConditionChipTurn 眠りChip;
	//cConditionChipTurn バクスイChip;
	//cConditionChip 浅い居眠りChip;
	//cConditionChip 深い居眠りChip;
	//cConditionChipTurn びっくりChip;
	//cConditionChipTurn 金縛りChip;
	//cConditionChipTurn 泥酔Chip;
	//cConditionChipTurn 空振りChip;
	//cConditionChipTurn やりすごしChip;
	//cConditionChipTurn 貧乏Chip;
	//cConditionChipTurn 氷付けChip;
	//cConditionChipTurn 死の誘いChip;
	//cConditionChipTurn みがわりChip;
	//cConditionChipTurn 擬態Chip;
	//cConditionChipTurn 狂乱Chip;
	//cConditionChipTurn 嫉妬Chip;
	//cConditionChipTurn 臆病Chip;
	//cConditionChipTurn 鳥目Chip;
	//cConditionChipTurn 脱力Chip;
	//cConditionChipTurn 元気Chip;
	//cConditionChipTurn 軟弱Chip;
	//cConditionChipTurn 頑強Chip;
	//cConditionChipTurn 無敵Chip;
	//cConditionChipTurn 目薬Chip;
	//cConditionChipTurn 封印Chip;

	//---------------------------------
	//例外処理
	//---------------------------------
	//異常状態が自然に直ったときの例外処理
	bool cure_back_natural(異常状態 type);
	//異常状態を無理やり治したときの例外処理
	bool cure_back_force(異常状態 type);

	//異常状態になったときの例外処理
	bool go_bad(異常状態 type);

	//---------------------------------
	//省略マクロ
	//---------------------------------
	#define DEF_CONDITION_PREPROCESS_turnon_追加(type) \
	virtual bool type##追加(int turn){\
		return type##Chip.turn_on(turn);};

	#define DEF_CONDITION_PREPROCESS_seton_追加(type) \
	virtual bool type##追加(){\
		return type##Chip.set_on();};

	#define DEF_CONDITION_PREPROCESS_count_verb(type,verb) \
	virtual bool type##verb(){\
		return count(type);};

	//---------------------------------
	//満腹度関係
	//---------------------------------
protected:
	virtual bool process_stomach();
public:
	enum 空腹ProcessFlags
	{
		空腹ProcessFlag_無し = 0,
		空腹ProcessFlag_空腹 = 1<<0,
		空腹ProcessFlag_回復 = 1<<1,
		空腹ProcessFlag_空腹回復 = 空腹ProcessFlag_空腹 | 空腹ProcessFlag_回復,
	};
protected:
	//int stomach_msg_count;
	//double HP_oddstock;

	class 空腹cConditionChip:
		public cConditionChipVirtual
	{
	private:
		int stomach_msg_count;
		double HP_oddstock;
	public:
		//デフォルトコンストラクタ
		空腹cConditionChip()//:
			//cConditionChipVirtual(異常状態_初期化無し, 0),
			//stomach_msg_count(0),
			//HP_oddstock(0.0)
		{};

		virtual int init(異常状態 type, int emotion, pcCharacter pchara)
		{
			cConditionChipVirtual::init(type,emotion,pchara);
			HP_oddstock = 0;
			stomach_msg_count = 0;
			return true;
		};
		/*
		空腹cConditionChip(異常状態 type, int emotion):
			cConditionChipVirtual(type, emotion),	
			stomach_msg_count(0),
			HP_oddstock(0.0)
		{};
		*/

		//毎ターンの処理
		virtual bool process();

		//主人公用の現在のエモーションをセットする関数
		virtual bool set_emotion();
		
		//敵用の次の行動の時のエモーションを予測してセットする関数
		virtual bool set_predict_emotion();

		//エモーションを消す関数
		virtual bool erase_emotion();

	} 空腹Chip;
protected:
	//---------------------------------
	//速度関係
	//---------------------------------
	//virtual bool process_speed();

	
	class 速度cConditionChip:
		public cConditionChipVirtual
	{
	private:
		int currentspeed;
		int defaultspeed;
		int speed_turn_count;
		int slow_count;

	protected:
		
		virtual int speed_refresh();

		virtual bool turn_off_judge(const int param){return (param < 0);};
	
	public:
		//デフォルトコンストラクタ
		速度cConditionChip()//:
			//cConditionChipVirtual(異常状態_初期化無し, 0)
		{};
		virtual int init(異常状態 type, int emotion, pcCharacter pchara);

		//毎ターンの処理
		virtual bool process();

		//主人公用の現在のエモーションをセットする関数
		virtual bool set_emotion();
		
		//敵用の次の行動の時のエモーションを予測してセットする関数
		virtual bool set_predict_emotion();

		//エモーションを消す関数
		virtual bool erase_emotion();

		//残りターンを出力する
		virtual int turncount(){return speed_turn_count;};

		virtual void setConditionExplain(pcListWindow& pclw)
		{
			setConditionExplainType(pclw, 超鈍足);
			setConditionExplainType(pclw, 鈍足);
			setConditionExplainType(pclw, 二倍速);
			setConditionExplainType(pclw, 三倍速);
		}
		//
		virtual bool デフォルト速度設定(異常状態 speed);

		virtual bool 速度増加(int turn);
		virtual bool 速度減少(int turn);

		virtual bool 鈍足で行動不能である();
		virtual bool 鈍足以下である();
		virtual bool 二倍速以上である();
		virtual bool 三倍速である();

		virtual int 速度度数(){return currentspeed;};
		virtual int デフォルト速度度数(){return defaultspeed;};
		virtual int 速度残りターン数(){return speed_turn_count;};
	} 速度Chip;

public:
	virtual bool デフォルト速度設定(異常状態 speed){return 速度Chip.デフォルト速度設定(speed);};

	virtual bool 速度増加(int turn){return 速度Chip.速度増加(turn);};
	virtual bool 速度減少(int turn){return 速度Chip.速度減少(turn);};

	virtual bool 鈍足で行動不能である(){return 速度Chip.鈍足で行動不能である();};
	virtual bool 鈍足以下である(){return 速度Chip.鈍足以下である();};
	virtual bool 二倍速以上である(){return 速度Chip.二倍速以上である();};
	virtual bool 三倍速である(){return 速度Chip.三倍速である();};

	virtual int 速度度数(){return 速度Chip.速度度数();};
	virtual int デフォルト速度度数(){return 速度Chip.デフォルト速度度数();};
	virtual int 速度残りターン数(){return 速度Chip.速度残りターン数();};
	//---------------------------------
	//眠り関係
	//---------------------------------
protected:
	//virtual bool process_sleep();
	//virtual bool process_emotion_sleep();//エモーション処理
protected:

	class 眠りcConditionChip:
		public cConditionChipVirtual
	{
	private:
		int sleep_turn_count;
		int heavysleep_turnmemo;
	protected:
		virtual bool turn_off_judge(const int param){return (param < 0);};
	public:
		//デフォルトコンストラクタ
		眠りcConditionChip()
		{};
		virtual int init(異常状態 type, int emotion, pcCharacter pchara)
		{
			cConditionChipVirtual::init(type,emotion,pchara);
			sleep_turn_count = -1;
			heavysleep_turnmemo = -1;
			return true;
		};
		virtual void setConditionExplain(pcListWindow& pclw)
		{
			setConditionExplainType(pclw, 眠り);
			setConditionExplainType(pclw, バクスイ);
			setConditionExplainType(pclw, 浅い居眠り);
			setConditionExplainType(pclw, 深い居眠り);
		}

		//毎ターンの処理
		virtual bool process();

		//主人公用の現在のエモーションをセットする関数
		virtual bool set_emotion();
		
		//敵用の次の行動の時のエモーションを予測してセットする関数
		virtual bool set_predict_emotion();

		//エモーションを消す関数
		virtual bool erase_emotion();

		//
		virtual bool 眠り追加(int turn);
		virtual bool バクスイ追加(int turn);
		virtual bool 浅い居眠り追加();
		virtual bool 深い居眠り追加();


		virtual bool 強い刺激(double power);//隣接
		virtual bool 弱い刺激(double power);//部屋進入


		virtual double 深い居眠り強い刺激浅い居眠り遷移確率();
		virtual double 深い居眠り弱い刺激浅い居眠り遷移確率();
		virtual double 浅い居眠り強い刺激覚醒遷移確率();
		virtual double 浅い居眠り弱い刺激覚醒遷移確率();
		virtual bool 眠りで行動不能である();
	} 眠りChip;
public:
	virtual bool 眠り追加(int turn){return 眠りChip.眠り追加(turn);};
	virtual bool バクスイ追加(int turn){return 眠りChip.バクスイ追加(turn);};
	virtual bool 浅い居眠り追加(){return 眠りChip.浅い居眠り追加();};
	virtual bool 深い居眠り追加(){return 眠りChip.深い居眠り追加();};


	virtual bool 強い刺激(double power){return 眠りChip.強い刺激(power);};//隣接
	virtual bool 弱い刺激(double power){return 眠りChip.弱い刺激(power);};//部屋進入


	//virtual double 深い居眠り強い刺激浅い居眠り遷移確率();
	//virtual double 深い居眠り弱い刺激浅い居眠り遷移確率();
	//virtual double 浅い居眠り強い刺激覚醒遷移確率();
	//virtual double 浅い居眠り弱い刺激覚醒遷移確率();

	virtual bool 眠りで行動不能である(){return 眠りChip.眠りで行動不能である();};

	//---------------------------------
	//びっくり関係
	//---------------------------------
protected:
	//virtual bool process_bikkuri();
	//virtual bool process_emotion_bikkuri();//エモーション処理
	//int bikkuri_turn_count;
public:

	cConditionChipTurn びっくりChip;
	DEF_CONDITION_PREPROCESS_turnon_追加(びっくり)
	DEF_CONDITION_PREPROCESS_count_verb(びっくり,で行動不能である)

	//---------------------------------
	//金縛り関係
	//---------------------------------
protected:
	//virtual bool process_kanasibari();
	//virtual bool process_emotion_kanasibari();//エモーション処理
	//int kanasibari_turn_count;
public:

	cConditionChipTurn 金縛りChip;
	DEF_CONDITION_PREPROCESS_turnon_追加(金縛り)
	DEF_CONDITION_PREPROCESS_count_verb(金縛り,で行動不能である)

	//---------------------------------
	//泥酔関係
	//---------------------------------
protected:
	//virtual bool process_deisui();
	//virtual bool process_emotion_deisui();//エモーション処理
	//int deisui_turn_count;
public:

	cConditionChipTurn 泥酔Chip;
	DEF_CONDITION_PREPROCESS_turnon_追加(泥酔)
	DEF_CONDITION_PREPROCESS_count_verb(泥酔,状態)
	DEF_CONDITION_PREPROCESS_count_verb(泥酔,で混乱状態である)
	//---------------------------------
	//脱力関係
	//---------------------------------
protected:
	//virtual bool process_poison();
	//int currentpoison;
	//int poison_turn_count;
	//virtual int Maxpoison(){return 10;};

	//virtual int poison_refresh();

public:
	class 脱力元気cConditionChip:
		public cConditionChipVirtual
	{
	private:
		int currentpoison;
		int poison_turn_count;
		virtual int Maxpoison(){return 10;};
	protected:
		virtual bool turn_off_judge(const int param){return (param < 0);};
		virtual void setConditionExplainType_Valiable(std::map<tstring, StyleString>& val);

	public:
		//デフォルトコンストラクタ
		脱力元気cConditionChip()//:
			//cConditionChipVirtual(異常状態_初期化無し, 0),
			//stomach_msg_count(0),
			//HP_oddstock(0.0)
		{};

		virtual int init(異常状態 type, int emotion, pcCharacter pchara)
		{
			cConditionChipVirtual::init(type,emotion,pchara);
			currentpoison = 0;
			poison_turn_count = -1;
			return true;
		};
		/*
		脱力元気cConditionChip(異常状態 type, int emotion):
			cConditionChipVirtual(type, emotion),	
			stomach_msg_count(0),
			HP_oddstock(0.0)
		{};
		*/

		//毎ターンの処理
		virtual bool process();

		//主人公用の現在のエモーションをセットする関数
		virtual bool set_emotion();
		
		//敵用の次の行動の時のエモーションを予測してセットする関数
		virtual bool set_predict_emotion();

		//エモーションを消す関数
		virtual bool erase_emotion();

		//残りターンを出力する
		virtual int turncount(){return poison_turn_count;};

		virtual void setConditionExplain(pcListWindow& pclw)
		{
			setConditionExplainType(pclw, 脱力);
			setConditionExplainType(pclw, 元気);
		}

		virtual bool 脱力追加(int power,int turn);
		virtual bool 元気追加(int power,int turn);
		virtual bool 脱力初期化();
		virtual double 力係数();
		virtual int 力度数(){return -currentpoison;};
		virtual int 力残りターン数(){return poison_turn_count;};
	} 脱力元気Chip;

	virtual bool 脱力追加(int power,int turn){return 脱力元気Chip.脱力追加(power,turn);};
	virtual bool 元気追加(int power,int turn){return 脱力元気Chip.元気追加(power,turn);};
	virtual bool 脱力初期化(){return 脱力元気Chip.脱力初期化();};
	virtual double 力係数(){return 脱力元気Chip.力係数();};
	virtual int 力度数(){return 脱力元気Chip.力度数();};
	virtual int 力残りターン数(){return 脱力元気Chip.力残りターン数();};
	//---------------------------------
	//軟弱関係
	//---------------------------------
protected:
	//virtual bool process_weak();
	//int currentweak;
	//int weak_turn_count;
	//virtual int Maxweak(){return 10;};

	//virtual int weak_refresh();

public:
	class 軟弱頑強cConditionChip:
		public cConditionChipVirtual
	{
	private:
		int currentweak;
		int weak_turn_count;
		virtual int Maxweak(){return 10;};

	protected:
		virtual bool turn_off_judge(const int param){return (param < 0);};
		virtual void setConditionExplainType_Valiable(std::map<tstring, StyleString>& val);

	public:
		//デフォルトコンストラクタ
		軟弱頑強cConditionChip()//:
			//cConditionChipVirtual(異常状態_初期化無し, 0),
			//stomach_msg_count(0),
			//HP_oddstock(0.0)
		{};

		virtual int init(異常状態 type, int emotion, pcCharacter pchara)
		{
			cConditionChipVirtual::init(type,emotion,pchara);
			currentweak = 0;
			weak_turn_count = -1;
			return true;
		};
		/*
		軟弱頑強cConditionChip(異常状態 type, int emotion):
			cConditionChipVirtual(type, emotion),	
			stomach_msg_count(0),
			HP_oddstock(0.0)
		{};
		*/

		//毎ターンの処理
		virtual bool process();

		//主人公用の現在のエモーションをセットする関数
		virtual bool set_emotion();
		
		//敵用の次の行動の時のエモーションを予測してセットする関数
		virtual bool set_predict_emotion();

		//エモーションを消す関数
		virtual bool erase_emotion();

		//残りターンを出力する
		virtual int turncount(){return weak_turn_count;};

		virtual void setConditionExplain(pcListWindow& pclw)
		{
			setConditionExplainType(pclw, 軟弱);
			setConditionExplainType(pclw, 頑強);
		}

		virtual bool 軟弱追加(int power,int turn);
		virtual bool 頑強追加(int power,int turn);
		virtual bool 軟弱初期化();
		virtual double 守係数();
		virtual int 守度数(){return -currentweak;};
		virtual int 守残りターン数(){return weak_turn_count;};
	} 軟弱頑強Chip;

	virtual bool 軟弱追加(int power,int turn){return 軟弱頑強Chip.軟弱追加(power,turn);};
	virtual bool 頑強追加(int power,int turn){return 軟弱頑強Chip.頑強追加(power,turn);};
	virtual bool 軟弱初期化(){return 軟弱頑強Chip.軟弱初期化();};
	virtual double 守係数(){return 軟弱頑強Chip.守係数();};
	virtual int 守度数(){return 軟弱頑強Chip.守度数();};
	virtual int 守残りターン数(){return 軟弱頑強Chip.守残りターン数();};


	//---------------------------------
	//目薬関係
	//---------------------------------
protected:
	cConditionChipOnOff 目薬Chip;
public:
	virtual bool 目薬追加();
	virtual bool 目薬解除();
	inline bool 目薬状態(){return count(目薬);};
	//---------------------------------
	//封印関係
	//---------------------------------
protected:
	//virtual bool process_fuuin();
	//int fuuin_turn_count;

	//virtual int fuuin_refresh();
	cConditionChipTurn 封印Chip;
public:
	DEF_CONDITION_PREPROCESS_turnon_追加(封印)
	DEF_CONDITION_PREPROCESS_count_verb(封印,状態)
	//---------------------------------
	//空振り関係
	//---------------------------------
protected:
	//virtual bool process_karaburi();
	//int karaburi_turn_count;

	//virtual int karaburi_refresh();

	cConditionChipTurn 空振りChip;
public:
	DEF_CONDITION_PREPROCESS_turnon_追加(空振り)
	DEF_CONDITION_PREPROCESS_count_verb(空振り,状態)


	//---------------------------------
	//やりすごし関係
	//---------------------------------
protected:
	//virtual bool process_yarisugosi();
	//int yarisugosi_turn_count;
	//int yarisugosi_turnmemo;
	//virtual int yarisugosi_refresh();
	class やりすごしcConditionChip:
		public cConditionChipTurn
	{
	
	protected:
		
	public:
		//デフォルトコンストラクタ
		やりすごしcConditionChip()//:
			//cConditionChipVirtual(異常状態_初期化無し, 0),
			//stomach_msg_count(0),
			//HP_oddstock(0.0)
		{};

		virtual int init(異常状態 type, int emotion, pcCharacter pchara)
		{
			cConditionChipTurn::init(type,emotion,pchara);
			return true;
		};	
		//主人公用の現在のエモーションをセットする関数
		virtual bool set_emotion();
		
		//敵用の次の行動の時のエモーションを予測してセットする関数
		virtual bool set_predict_emotion();
		
		//エモーションを消す関数
		virtual bool erase_emotion();
		
		//ターンをセットする
		virtual bool turn_on(int turn);
		virtual bool やりすごし解除効果();
	} やりすごしChip;
public:
	//DEF_CONDITION_PREPROCESS_turnon_追加(やりすごし)
	//DEF_CONDITION_PREPROCESS_count_verb(やりすごし,状態)

	DEF_CONDITION_PREPROCESS_turnon_追加(やりすごし)
		virtual bool やりすごし解除(){return やりすごしChip.set_off();};
	DEF_CONDITION_PREPROCESS_count_verb(やりすごし,状態)

	//---------------------------------
	//貧乏関係
	//---------------------------------
protected:
	//virtual bool process_binbou();
	//int binbou_turn_count;
	//virtual int binbou_refresh();

	cConditionChipTurn 貧乏Chip;
public:

	
	DEF_CONDITION_PREPROCESS_turnon_追加(貧乏)
	DEF_CONDITION_PREPROCESS_count_verb(貧乏,状態)
	//---------------------------------
	//氷付け関係
	//---------------------------------
protected:
	//virtual bool process_koorituke();
	//int koorituke_turn_count;
	//virtual int koorituke_refresh();
	cConditionChipTurn 氷付けChip;

	virtual bool 氷付けで移動失敗();

public:


	DEF_CONDITION_PREPROCESS_turnon_追加(氷付け)
	DEF_CONDITION_PREPROCESS_count_verb(氷付け,状態)
	//---------------------------------
	//死の誘い関係
	//---------------------------------
protected:
	//virtual bool process_sasoi();
	//int sasoi_turn_count;
	//int sasoi_turn_count_visible_;
	//double sasoi_visual_counter;
	//virtual bool Draw_sasoi(IDirect3DDevice9 *pDev);
	//virtual int sasoi_refresh();
	//wpcCharacter sasoi_subject_;

	class 死の誘いcConditionChip:
		public cConditionChipVirtual
	{
	
	protected:
		
		int sasoi_turn_count;
		int sasoi_turn_count_visible_;
		double sasoi_visual_counter;
		
		
		wpcCharacter sasoi_subject_;

		virtual bool turn_off_judge(const int param){return (param < 0);};
	public:
		//デフォルトコンストラクタ
		死の誘いcConditionChip()//:
			//cConditionChipVirtual(異常状態_初期化無し, 0),
			//stomach_msg_count(0),
			//HP_oddstock(0.0)
		{};

		virtual int init(異常状態 type, int emotion, pcCharacter pchara)
		{
			cConditionChipVirtual::init(type,emotion,pchara);
			sasoi_turn_count = -1;
			sasoi_turn_count_visible_ = -1;
			sasoi_visual_counter = 0;
			sasoi_subject_ = wpcCharacter(pcCharacter((cCharacter*)NULL));
			return true;
		};

		//毎ターンの処理
		virtual bool process();

		//主人公用の現在のエモーションをセットする関数
		virtual bool set_emotion();

		//敵用の次の行動の時のエモーションを予測してセットする関数
		virtual bool set_predict_emotion();
		
		//エモーションを消す関数
		virtual bool erase_emotion();

	private:
		//ターンをセットする
		virtual bool turn_on(int turn);
	public:
		virtual bool Draw_sasoi(IDirect3DDevice9 *pDev);

		virtual bool 死の誘い追加(int turn, pcCharacter sasoi_subject);

		int sasoi_turn_count_visible(){return sasoi_turn_count_visible_;};
	} 死の誘いChip;
public:
	virtual bool 死の誘い追加(int turn, pcCharacter sasoi_subject)
		{return 死の誘いChip.死の誘い追加(turn,sasoi_subject);};
	inline bool 死の誘い状態(){return count(死の誘い);};

	//---------------------------------
	//みがわり関係
	//---------------------------------
protected:
	//virtual bool process_migawari();
	//int migawari_turn_count;
	//int migawari_surfaceID;
	//virtual int migawari_refresh();

	class みがわりcConditionChip:
		public cConditionChipVirtual
	{
	
	protected:
		int migawari_turn_count;
		int migawari_surfaceID;

		virtual bool turn_off_judge(const int param){return (param < 0);};
	public:
		//デフォルトコンストラクタ
		みがわりcConditionChip()//:
			//cConditionChipVirtual(異常状態_初期化無し, 0),
			//stomach_msg_count(0),
			//HP_oddstock(0.0)
		{};
		~みがわりcConditionChip(){};

		virtual int init(異常状態 type, int emotion, pcCharacter pchara)
		{
			cConditionChipVirtual::init(type,emotion,pchara);
			migawari_turn_count = -1;
			migawari_surfaceID = 0;
			return true;
		};

		//毎ターンの処理
		virtual bool process();

		//主人公用の現在のエモーションをセットする関数
		virtual bool set_emotion();

		//敵用の次の行動の時のエモーションを予測してセットする関数
		virtual bool set_predict_emotion();
		
		//エモーションを消す関数
		virtual bool erase_emotion();

		//ターンをセットする
		virtual bool turn_on(int turn);

		virtual bool みがわり追加(int turn, pcCharacter pchara);
		virtual bool みがわり解除();

	} みがわりChip;
public:
	virtual bool みがわり追加(int turn, pcCharacter pchara)
		{return みがわりChip.みがわり追加(turn, pchara);};
	virtual bool みがわり解除(){return みがわりChip.みがわり解除();};
	inline bool みがわり状態(){return count(みがわり);};
	
	//---------------------------------
	//変身関係
	//---------------------------------
protected:

	class 変身cConditionChip:
		public cConditionChipVirtual
	{
	
	protected:
		int hensin_turn_count;
		int hensin_surfaceID;

		virtual bool turn_off_judge(const int param){return (param < 0);};
	public:
		//デフォルトコンストラクタ
		変身cConditionChip()//:
			//cConditionChipVirtual(異常状態_初期化無し, 0),
			//stomach_msg_count(0),
			//HP_oddstock(0.0)
		{};
		~変身cConditionChip(){};

		virtual int init(異常状態 type, int emotion, pcCharacter pchara)
		{
			cConditionChipVirtual::init(type,emotion,pchara);
			hensin_turn_count = -1;
			hensin_surfaceID = 0;
			return true;
		};

		//毎ターンの処理
		virtual bool process();

		//主人公用の現在のエモーションをセットする関数
		virtual bool set_emotion();

		//敵用の次の行動の時のエモーションを予測してセットする関数
		virtual bool set_predict_emotion();
		
		//エモーションを消す関数
		virtual bool erase_emotion();

		//ターンをセットする
		virtual bool turn_on(int turn);

		virtual bool 変身追加(int turn, pcCharacter pchara);
		virtual bool 変身解除();

	} 変身Chip;
public:
	virtual bool 変身追加(int turn, pcCharacter pchara)
		{return 変身Chip.変身追加(turn, pchara);};
	virtual bool 変身解除(){return 変身Chip.変身解除();};
	inline bool 変身状態(){return count(変身);};
	
	//---------------------------------
	//無敵関係
	//---------------------------------
protected:
	//virtual bool process_muteki();
	//int muteki_turn_count;
	//virtual int muteki_refresh();

	cConditionChipTurn 無敵Chip;
public:
	DEF_CONDITION_PREPROCESS_turnon_追加(無敵)
	DEF_CONDITION_PREPROCESS_count_verb(無敵,状態)

	//---------------------------------
	//擬態関係
	//---------------------------------
protected:
	//virtual bool process_gitai();
	//int gitai_turn_count;
	//virtual int gitai_refresh();
	cConditionChipTurn 擬態Chip;
public:
	DEF_CONDITION_PREPROCESS_turnon_追加(擬態)
	DEF_CONDITION_PREPROCESS_count_verb(擬態,状態)

	//---------------------------------
	//狂乱関係
	//---------------------------------
protected:
	//virtual bool process_kyouran();
	//int kyouran_turn_count;
	//virtual int kyouran_refresh();
	cConditionChipTurn 狂乱Chip;
public:
	DEF_CONDITION_PREPROCESS_turnon_追加(狂乱)
	DEF_CONDITION_PREPROCESS_count_verb(狂乱,状態)

	//---------------------------------
	//臆病関係
	//---------------------------------
protected:
	//virtual bool process_okubyou();
	//int okubyou_turn_count;
	//virtual int okubyou_refresh();
	cConditionChipTurn 臆病Chip;
public:
	DEF_CONDITION_PREPROCESS_turnon_追加(臆病)
	DEF_CONDITION_PREPROCESS_count_verb(臆病,状態)

	//---------------------------------
	//嫉妬関係
	//---------------------------------
protected:
	//virtual bool process_shitto();
	//int shitto_turn_count;
	//virtual int shitto_refresh();
	cConditionChipTurn 嫉妬Chip;
public:
	DEF_CONDITION_PREPROCESS_turnon_追加(嫉妬)
	DEF_CONDITION_PREPROCESS_count_verb(嫉妬,状態)

	//---------------------------------
	//鳥目関係
	//---------------------------------
protected:
	//virtual bool process_torime();
	//int torime_turn_count;
	//virtual int torime_refresh();
	cConditionChipTurn 鳥目Chip;
public:
	DEF_CONDITION_PREPROCESS_turnon_追加(鳥目)
	DEF_CONDITION_PREPROCESS_count_verb(鳥目,状態)

	//---------------------------------
	//無意識関係
	//---------------------------------
protected:
	//virtual bool process_shitto();
	//int shitto_turn_count;
	//virtual int shitto_refresh();
	cConditionChipTurn 無意識Chip;
public:
	DEF_CONDITION_PREPROCESS_turnon_追加(無意識)
	DEF_CONDITION_PREPROCESS_count_verb(無意識,状態)

};

//#include <boost/shared_ptr.hpp>
//typedef boost::shared_ptr<cCondition> pcCondition;
