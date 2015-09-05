#pragma once
#include "cgameobject.h"
#include "utility/SpriteText.h"

#include "cMessageWindow.h"
#include "cDroping.h"

#include <vector>
#include <set>
using namespace std;


class cMessageDroping :
	public cDroping
{
public:
	cMessageDroping(void);
	cMessageDroping(psstring pstr);
public:
	virtual ~cMessageDroping(void);

	virtual inline tstring IconFileName(){return _T("item\\システムアイコン.png");};
	virtual inline int IconFileIndexX(){return 0;};
	virtual inline int IconFileIndexY(){return 0;};

	//消去処理の準備
	virtual void Release(){};

	virtual StyleString shortExplanation();
	virtual StyleString longExplanation();


	virtual int RerenderText(SpriteText & sprite,int MaxLetterLength);

	//メニューで選択時に最初に呼ばれる
	virtual int Action(IDirect3DDevice9 *pDev);

	psstring pmessagestr;

};

