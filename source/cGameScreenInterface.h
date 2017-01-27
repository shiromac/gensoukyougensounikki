
#pragma once
#include "cControl.h"
#include "cPlayerChara.h"
#include "cMoney.h"
#include "utility/SpriteText.h"

#include "cParameterGage.h"
#include "utility\valiableField\SafePointerObject.h"

class GensouGageView;
typedef boost::shared_ptr<GensouGageView> pGensouGageView;

class cGameScreenInterface :
	public cControl
{
public:
	cGameScreenInterface(void);
public:
	virtual ~cGameScreenInterface(void);

	virtual int Init(IDirect3DDevice9 *pDev, pcCharacter phero);

	virtual int process(IDirect3DDevice9 *pDev);
	
	virtual int Draw(IDirect3DDevice9 *pDev);
	virtual int RefreshDraw(IDirect3DDevice9 *pDev);
	virtual int Resetdisp(IDirect3DDevice9 *pDev);
public:	
	//実際の値
	pcCharacter hero;
	cMoney money;
	int FloorLevel;

	
	//表示の値
	SharedValiable_likeInt<int> disphero_HP;
	SharedValiable_likeInt<int> disphero_MHP;
	SharedValiable_likeInt<int> disphero_LV;
	SharedValiable_likeDouble<double> disphero_Stomach;	
	SharedValiable_likeDouble<double> disphero_MaxStomach;
	SafePointerObject<set<int>> disphero_emotion;
	SafePointerObject<cMoney> dispmoney;
	SharedValiable_likeInt<int> dispFloorLevel;

protected:
	virtual int isHeroPchange(IDirect3DDevice9 *pDev);
	//一時記憶
	pcCharacter heroCopy;
	cMoney moneyCopy;
	int FloorLevelCopy;

public:	

	virtual int menuPosWidthByLevel();
	virtual int menuPosHeightByLevel();

	virtual int menuPosTop(int level);
	virtual int menuPosLeft(int level);
	virtual int menuPosRight(int level);
	virtual int menuPosBottom(int level);
	
	virtual int menuPosCenterX();
	virtual int menuPosCenterY();

	virtual cRectObj menuPosParentOfControlLayer(const pcControlLayer& childControlLayer);

	SpriteText text;

protected:
	cParameterGage HPGage;
	cParameterGage STGage;
	pGensouGageView gensouGage;
public:	
	//終了フラグ
	bool End;
};


