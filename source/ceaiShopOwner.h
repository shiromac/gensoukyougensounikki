#pragma once

#include "cEnemyAI.h"

class ceaiShopOwner
	:public cEnemyAI
{
public:
	ceaiShopOwner(void);
public:
	virtual ~ceaiShopOwner(void);

	virtual int Init(pcCharacter self);

	virtual int Move();
	virtual int Attack();



	virtual int Request(int phase);


};

//店システム
#define DEF_COMMANDCLASS(name,deftext) \
class cCommand##name## : public cCommand\
{\
public:\
	cCommand##name##(void){caption = deftext;};\
	cCommand##name##(StyleString s){caption = s;};\
	virtual ~cCommand##name##(void){};\
	virtual int Action(cRenderDevice *pDev);\


DEF_COMMANDCLASS(_ShopBuy,g_Lang(_T("商品を買う")))
	cCommand_ShopBuy(pcCharacter pchara,StyleString s){caption = s;powner_ = pchara;};
	virtual StyleString shortExplanationText();
	pcCharacter	powner_;
};
DEF_COMMANDCLASS(_ShopSell,g_Lang(_T("床のアイテムを売る")))
	cCommand_ShopSell(pcCharacter pchara,StyleString s){caption = s;powner_ = pchara;};
	virtual StyleString shortExplanationText();
	pcCharacter	powner_;
};
DEF_COMMANDCLASS(_ShopSellHold,g_Lang(_T("持ち物を売る")))
	cCommand_ShopSellHold(pcCharacter pchara,StyleString s){caption = s;powner_ = pchara;};
	virtual StyleString shortExplanationText();
	pcCharacter	powner_;
};
DEF_COMMANDCLASS(_ShopBuyHold,g_Lang(_T("店主の持ち物を買う")))
	cCommand_ShopBuyHold(pcCharacter pchara,StyleString s){caption = s;powner_ = pchara;};
	virtual StyleString shortExplanationText();
	pcCharacter	powner_;
};
DEF_COMMANDCLASS(_ShopChargeMoney,g_Lang(_T("後払い金を請求する")))
	virtual StyleString shortExplanationText();
};
DEF_COMMANDCLASS(_ShopExit,g_Lang(_T("店を出る")))
	virtual StyleString shortExplanationText();
};