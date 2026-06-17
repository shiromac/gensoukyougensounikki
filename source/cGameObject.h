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

	cRenderTexture* m_pTexture;

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

	virtual void Init(cRenderDevice *pDev){};
	virtual void Draw(cRenderDevice *pDev){};
	virtual void Process(cRenderDevice *pDev){};

	//-----------------------------------------
	//特殊ドロー系

	//方向つき攻撃ドロー（主に矢と魔法）
	virtual void aspectedAttackDraw(cRenderDevice *pDev){Draw(pDev);};

};
