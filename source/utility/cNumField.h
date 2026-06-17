#pragma once

#include "cColor.h"
#include "4DVector.h"

#include "../gameMainSystem/cRenderBackend.h"

#include <vector>
using namespace std;

class cNumField
{
public:
	cNumField(void);
public:
	virtual ~cNumField(void);

	virtual void Draw(cRenderDevice *pDev);
	virtual void Init(cRenderDevice *pDev);

	int distance;
	c4DVector position;

	int FontHeightSize;
	int FontWidthSize;

	cColor color;
	int colorBlendMode;
	int drawMode;

	int Fontindex;
	
	virtual void GetFigure(vector<int>& figures);

	int arrangeX;
	int arrangeY;

	enum PositionArrangeX
	{
		ARRANGEX_LEFT = -1,
		ARRANGEX_CENTER = 0,
		ARRANGEX_RIGHT = 1,
	};
	enum PositionArrangeY
	{
		ARRANGEY_TOP = -1,
		ARRANGEY_CENTER = 0,
		ARRANGEY_BOTTOM = 1,
	};

protected:
	
protected:
	cRenderTexture* pTexture_;
	//0123456789-×

public:

	double Value_;
	//int * watchTarget_int;//入っているアドレスの実値を監視。
	//double * watchTarget_double;
};

#define CNF_MAX_FIGURE 32