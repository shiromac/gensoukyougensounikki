#pragma once


class cGameObject;
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
typedef boost::shared_ptr<cGameObject> pcGameObject;
typedef boost::weak_ptr<cGameObject> wpcGameObject;

#include "utility/ValiableField/SafePointerObject.h"
#include "cDrawingObject.h"
#include "utility/4Dvector.h"
#include "utility/StyleString.h"
#include <tstring_ph.h>


using namespace std;

class cGameObject
{
public:
	cGameObject(void);
public:
	virtual ~cGameObject(void);

	virtual inline StyleString FullName(){return _T("gameobject");};
	virtual inline StyleString FullNameSub(){return _T("gameobject");};
	virtual inline StyleString ShortName(){return _T("gameobject");};
	virtual inline StyleString ShortNameSub(){return _T("gameobject");};
	virtual inline tstring usefulName(){return _T("gameobject");};

	IDirect3DTexture9* m_pTexture;

	SafePointerObject<c4DVector> visibleplace;
	int placeX;
	int placeY;
	c4DVector mapForcus;

	int aspect;//方向
	SharedValiable_likeInt<int> visibleaspect;//方向


	SharedValiable_likeDouble<double> opaque;//描画時の不透明度(0~1)//視界にあるかどうか

	//描画されるべきか
	SharedValiable_likeInt<int> pri_onland_DrawFlag;


	virtual int ID(){return -1;};

	virtual void Init(IDirect3DDevice9 *pDev){};
	virtual void Draw(IDirect3DDevice9 *pDev){};
	virtual void Process(IDirect3DDevice9 *pDev){};

	//-----------------------------------------
	//特殊ドロー系

	//方向つき攻撃ドロー（主に矢と魔法）
	virtual void aspectedAttackDraw(IDirect3DDevice9 *pDev){Draw(pDev);};

};
