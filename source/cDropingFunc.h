#pragma once
#include "cCommand.h"

#include "cDroping.h"

#include <vector>

#include <tstring_ph.h>


using namespace std;

class cDropingFunc :
	public cCommand
{
public:
	cDropingFunc(void);
	cDropingFunc(tstring str);
public:
	virtual ~cDropingFunc(void);


	pcDroping Subject;
	tstring Verb;
	tstring DisplayVerb();
	vector<pcDroping> Objects;

	
	virtual StyleString shortExplanationText();

	StyleString shortExplanation;


	//メニューの処理を行う。
	virtual int Action(cRenderDevice *pDev);


	//新規の目的語(オブジェクト)を選択するためのリストを作成する。
	virtual void MakeSetObjectMenu(cRenderDevice *pDev, vector<pcDroping> &Objects, bool MultiObj, StyleString info);
	virtual void MakeSetObjectMenu(cRenderDevice *pDev, vector<vector<pcDroping>> &Objectslist, bool MultiObj, vector<StyleString> infolist);



	//実際の処理を行う。
	virtual int Fire(cRenderDevice *pDev);


	//最新のオブジェクトを返す
	virtual pcDroping LastObject();

	
	//ショートカットに入れても良いかどうか
	virtual bool canIntoShortCut();

};

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cDropingFunc> pcDropingFunc;

class cStoreDroping :
	public cDroping
{
public:
	cStoreDroping(vector<pcDroping>& store);
public:
	virtual ~cStoreDroping(void);

	vector<pcDroping>& store_;
	int capacity_;
};
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cStoreDroping> pcStoreDroping;

class cShopDroping :
	public cDroping
{
public:
	cShopDroping(pcDroping pdrop, vector<tstring>& captions);
public:
	virtual ~cShopDroping(void);

	pcDroping item_;
	pcCharacter sellchara_;
	vector<tstring> captions_;

	virtual void Init(cRenderDevice *pDev,pcDroping self);

	virtual void DrawIcon(cRenderDevice *pDev,int x,int y){
		item_->m_color = m_color;
		item_->DrawIcon(pDev,x,y);};
	virtual void DrawStateIcon(cRenderDevice *pDev,int x,int y){item_->m_color = m_color;item_->DrawStateIcon(pDev,x,y);};
	virtual void DrawStateIconSub(cRenderDevice *pDev,int x,int y){item_->m_color = m_color;item_->DrawStateIconSub(pDev,x,y);};

	//virtual int RerenderText(SpriteText & sprite,int MaxLetterLength);

	virtual StyleString shortExplanation();
	virtual StyleString longExplanation();

	virtual StyleString FullName();
	virtual StyleString ShortName();
	virtual StyleString ListFullName();

	virtual void longExplanationExtra(std::vector<StyleString>& explanations){return item_->longExplanationExtra(explanations);};
	virtual int Getsmollview(pcGameWindow& smollview){return item_->Getsmollview(smollview);};

	virtual int ID(){return item_->ID();};

	//メニューで選択時に最初に呼ばれる
	virtual int Action(cRenderDevice *pDev);

	virtual StyleString ExtraHatString();

};
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cShopDroping> pcShopDroping;