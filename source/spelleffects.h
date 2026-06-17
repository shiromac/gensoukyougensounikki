#pragma once

#include "cAnimation.h"

//-------------------------------------------------
//スペルエフェクト群
//-------------------------------------------------
namespace SpellEffects
{

	void EffectcSpell_ID_0(const double placeX,const double placeY);
	void EffectcSpell_ID_1(const double placeX,const double placeY);

	void EffectcSpell_ID_4(const double placeX,const double placeY, const int aspect);

	void EffectcSpell_ID_6(const double placeX,const double placeY, const int aspect);

	void EffectcSpell_ID_22(const double placeX,const double placeY, const int time);















};

class caESpell_ID_1 :
	public cAnimation
{
public:
	caESpell_ID_1(int num);
public:
	virtual ~caESpell_ID_1(void);

	virtual int process(cRenderDevice *pDev);
	virtual int Draw(cRenderDevice *pDev);

	virtual int Init();
protected:
	virtual int preprocess(cRenderDevice *pDev);
	int preprocessed;
	int num_;
	class firefly
	{
	public:
		c4DVector position;
		double angle;
		double acangle;
		int flag;
		int flag2;
		double lightpower;
		double cicle;
		int life;
		int lifecount;
	};

	vector<firefly> fireflys;

	int life;
public:

};

