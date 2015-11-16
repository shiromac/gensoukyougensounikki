#pragma once

#include "cCommand.h"

#include "Language.h"

#include <vector>
#include <list>
#include <tstring_ph.h>


using namespace std;

#define DEF_COMMANDCLASS(name,deftext) \
class cCommand##name## : public cCommand\
{\
public:\
	cCommand##name##(void){caption = deftext;};\
	cCommand##name##(StyleString s){caption = s;};\
	virtual ~cCommand##name##(void){};\
	virtual int Action(IDirect3DDevice9 *pDev);\


	

DEF_COMMANDCLASS(_menuItem,g_Lang(_T("道具")))
	virtual StyleString shortExplanationText();
};

DEF_COMMANDCLASS(_menuFoot,g_Lang(_T("足元")))
	virtual StyleString shortExplanationText();
};

DEF_COMMANDCLASS(_menuStates,g_Lang(_T("状態")))
	virtual StyleString shortExplanationText();
};

DEF_COMMANDCLASS(_menuOther,g_Lang(_T("その他")))
	virtual StyleString shortExplanationText();
};

DEF_COMMANDCLASS(_menuOther_MessageHistory,g_Lang(_T("メッセージ履歴")))
	virtual StyleString shortExplanationText();
};

DEF_COMMANDCLASS(_menuOther_Summon,_T("デバック召喚"))
	virtual StyleString shortExplanationText();
};

DEF_COMMANDCLASS(_menuOther_Summon_Chara,_T("キャラ名前"))
	virtual StyleString shortExplanationText();
	int chara_ID;
};

DEF_COMMANDCLASS(_menuOther_Summon_Chara_Class,_T("クラス"))
	virtual StyleString shortExplanationText();
	int chara_ID;
	int CLASS;
	int Forse;
};

DEF_COMMANDCLASS(_menuOther_Make,_T("デバック生成"))
	virtual StyleString shortExplanationText();
};

DEF_COMMANDCLASS(_menuOther_Make_randomDrop,_T("ランダムアイテム生成"))
	virtual StyleString shortExplanationText();
};

DEF_COMMANDCLASS(_menuOther_Make_Drop,_T("落ち物名前"))
	virtual StyleString shortExplanationText();
	int Drop_ID;
};

DEF_COMMANDCLASS(_menuOther_Make_Drop_Natural,_T("自然生成"))
	virtual StyleString shortExplanationText();
	int Drop_ID;
};

DEF_COMMANDCLASS(_menuOther_Make_Drop_State,_T("状態"))
	virtual StyleString shortExplanationText();
	int Drop_ID;
	int state;
};

DEF_COMMANDCLASS(_menuOther_Make_Drop_State_Quality,_T("修正値"))
	virtual StyleString shortExplanationText();
	int Drop_ID;
	int state;
	int quality;
};

DEF_COMMANDCLASS(_menuOther_Get,_T("デバッグ取得"))
	virtual StyleString shortExplanationText();
};

DEF_COMMANDCLASS(_menuOther_Get_EquipUp,_T("装備品熟練度加算"))
	virtual StyleString shortExplanationText();
};

DEF_COMMANDCLASS(_menuOther_Get_MoneyUp,_T("所持金加算"))
	virtual StyleString shortExplanationText();
};

DEF_COMMANDCLASS(_menuOther_Get_Level10Up,_T("Lv10加算"))
	virtual StyleString shortExplanationText();
};

DEF_COMMANDCLASS(_menuOther_Get_MaxHP100Up,_T("最大Hp100加算"))
	virtual StyleString shortExplanationText();
};

DEF_COMMANDCLASS(_menuOther_String,_T("文言一覧"))
	virtual StyleString shortExplanationText();
};
/*
DEF_COMMANDCLASS(_menuOther_String_AllItemShortExplanation,_T("アイテム簡易説明一斉表示"))
	virtual StyleString shortExplanationText();
};
DEF_COMMANDCLASS(_menuOther_String_AllItemShortExplanation_ID,_T("アイテム名"))
	virtual StyleString shortExplanationText();
	int Drop_ID;
};
*/
DEF_COMMANDCLASS(_menuOther_String_AllItemLongExplanation,_T("アイテム説明一斉表示"))
	virtual StyleString shortExplanationText();
};
DEF_COMMANDCLASS(_menuOther_String_AllItemLongExplanation_ID,_T("アイテム名"))
	virtual StyleString shortExplanationText();
	int Drop_ID;
};
DEF_COMMANDCLASS(_menuOther_String_AllMassage,_T("ストーリー一覧"))
	virtual StyleString shortExplanationText();
};
DEF_COMMANDCLASS(_menuOther_String_AllMassage_Massage,_T("ストーリー"))
	virtual StyleString shortExplanationText();
	tstring massage_title;
};

DEF_COMMANDCLASS(_menuOther_ReImport,_T("外部データ再読み込み"))
	virtual StyleString shortExplanationText();
};
DEF_COMMANDCLASS(_menuOther_ReImportGraphic,_T("画像データ再読み込み"))
	virtual StyleString shortExplanationText();
};

DEF_COMMANDCLASS(_menuOther_ImportDataCheck,_T("外部読み込み形式エラーチェック"))
	virtual StyleString shortExplanationText();
};

DEF_COMMANDCLASS(_menuOther_RetireChoice,g_Lang(_T("あきらめる")))
	virtual StyleString shortExplanationText();
};
DEF_COMMANDCLASS(_menuOther_RetireChoice_Yes,g_Lang(_T("本当にあきらめる")))
	virtual StyleString shortExplanationText();
};
DEF_COMMANDCLASS(_menuOther_RetireChoice_No,g_Lang(_T("あきらめない")))
	virtual StyleString shortExplanationText();
};
DEF_COMMANDCLASS(_menuOther_ReturnTitle,g_Lang(_T("タイトルに戻る")))
	//virtual StyleString shortExplanationText();
};

namespace CommandContinue{
	int CommandFunc_Continue(cCommandDelegated& command);
	int CommandFunc_ContinueAndSuspend(cCommandDelegated& command);
	int CommandFunc_Retire(cCommandDelegated& command);
	int CommandFunc_Retire_Retire(cCommandDelegated& command);
	int CommandFunc_Retire_Restart(cCommandDelegated& command);
}