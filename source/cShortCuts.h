#pragma once

#include <tstring_ph.h>
#include "utility/StyleString.h"

class cShortCutsManager;
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
typedef boost::shared_ptr<cShortCutsManager> pcShortCutsManager;
typedef boost::weak_ptr<cShortCutsManager> wpcShortCutsManager;

class cCommand;
typedef boost::shared_ptr<cCommand> pcCommand;
typedef boost::weak_ptr<cCommand> wpcCommand;
class cDroping;
typedef boost::shared_ptr<cDroping> pcDroping;
class cDropingFunc;
typedef boost::shared_ptr<cDropingFunc> pcDropingFunc;
typedef boost::weak_ptr<cDropingFunc> wpcDropingFunc;
class cGameWindow;
typedef boost::shared_ptr<cGameWindow> pcGameWindow;


enum eShortCuts_index{
	eShortCuts_index_Up = 0,
	eShortCuts_index_UpRight,
	eShortCuts_index_Right,
	eShortCuts_index_DownRight,
	eShortCuts_index_Down,
	eShortCuts_index_DownLeft,
	eShortCuts_index_Left,
	eShortCuts_index_UpLeft,
	eShortCuts_index_Neutral,
	eShortCuts_index_Num,
	eShortCuts_index_NoIndex,
};
class cShortCutsManager
{
protected:
	//-------------------------------------------
	class cShortCut;
	typedef boost::shared_ptr<cShortCut> pcShortCut;
	typedef boost::weak_ptr<cShortCut> wpcShortCut;
	class cShortCut
	{
	protected:
		pcCommand pcommand_;
		pcDroping pDroping_;
		pcDropingFunc pDropingFunc_;
		bool autoFunction_;
	protected:
		virtual void init(pcCommand pcommand);
		virtual void init(pcDroping pdroping);

		static bool canParentShortCut(pcDroping pparent);
	public:
		//コンストラクタ
		cShortCut(pcCommand pcommand, bool autoFunction);
		cShortCut(pcDroping pdroping, bool autoFunction);
		virtual ~cShortCut();
		
		bool autoFunction(){return autoFunction_;};
	public:
		//ショートカット喪失で成立不能
		virtual bool lostShortCut();
		//ショートカット実行可能
		virtual bool canShortCut();
		//ショートカット実行
		virtual bool doShortCut();
	public:
		//サブジェクト取得
		virtual pcDroping Subject();
		//動詞取得
		virtual tstring DisplayVerb();
		//説明取得
		virtual StyleString ShortExplanation();
	public:
		//ショートカット挿入可能
		static bool canAddShortcut(pcCommand pcommand);
		static bool canAddShortcut(pcDroping pdroping);

	};
	//-------------------------------------------


public:
	cShortCutsManager(void);
	virtual ~cShortCutsManager(void);
protected:
	int inittedFlag_;
	pcShortCut shortcut_[eShortCuts_index_Num];
	pcGameWindow shortcutWindow_[eShortCuts_index_Num];
	pcGameWindow shortcutTempWindow_;
	pcDroping shortcutTempDrop_;
	void check(eShortCuts_index index);
	void pri_init();

	pcShortCut makeAutoShortCut(pcDroping source);
	pcShortCut makeAutoShortCutWithVerb(pcDroping source, const vector<tstring>& CaptionList, const tstring& verb);

	void tryAddShortcutForSource(pcDroping source);
	void addShortcutToEmpty(pcShortCut shortcut);

	virtual void pri_setShortcutText(pcGameWindow pwindow, pcDroping pdrop, const StyleString& Verb, bool autoFunction);
	
public:
	void addShortcut(eShortCuts_index index, pcCommand pcommand, bool autoFunction = false);
	void addShortcut(eShortCuts_index index, pcDroping pdroping, bool autoFunction = false);
	void doShortcut(eShortCuts_index index);
	bool isShortcutAutomatic(eShortCuts_index index);

	void eraseShortcut(eShortCuts_index index);
	
	void resetShortcutText(eShortCuts_index index);
	void resetAllShortcutText();
	
	void refreshAutomaticShortCut(vector<pcDroping>& itemList);

	
	void setShortcutTempText(pcDroping pdrop, const StyleString& Verb);
	
	StyleString ShortExplanation(eShortCuts_index index);

	

	bool canAddShortcut(pcCommand pcommand);
	bool canAddShortcut(pcDroping pdroping);

	void drawWindow(eShortCuts_index forcus);
	
	void drawTempWindow(eShortCuts_index forcus);
};