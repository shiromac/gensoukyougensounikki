#pragma once

#include "cAnimation.h"
#include "cLandform.h"
#include "cDroping.h"

#include "caEchipS.h"
#include "gameMainSystem/main.h"

#include <vector>
#include <list>
#include <tstring_ph.h>


using namespace std;



#define CAESPELL_BACKNUM 32

class caEspell;
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<caEspell> pcaEspell;

//-------------------------------------------------
//スペル
//-------------------------------------------------
class caEspell :
	public cAnimationChipManager
{
public:
	caEspell(int x = 0,int y = 0,int z = 0, int layer = 0);
public:
	virtual ~caEspell(void);


	virtual int process(cRenderDevice *pDev);
	virtual int Draw(cRenderDevice *pDev);
	

	virtual int Init();
protected:
	virtual int preprocess(cRenderDevice *pDev);
	int preprocessed;

public:

	virtual bool isOnScreen();


	tstring SpellName;





	unsigned long color1;
	unsigned long color2;

protected:
	int m_count;

	c4DVector leftbottom;


	caEchip_Text SpellName_Echip;
	cDrawingObject backDO;
	cDrawingObject underDO;

	c4DVector backdif[CAESPELL_BACKNUM];

protected:
	static const int BOXSIZE = (64);

	static const int POSITION_BEGIN_Y = (SCREEN_Y/2+BOXSIZE);
	static const int POSITION_BEGIN_X = (SCREEN_X/2-BOXSIZE*4.2);
	static const int POSITION_END_Y = (SCREEN_Y/2-BOXSIZE*2.8);
};


class cCharacter;
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
typedef boost::shared_ptr<cCharacter> pcCharacter;
typedef boost::weak_ptr<cCharacter> wpcCharacter;


//-------------------------------------------------
//ボススペル
//-------------------------------------------------
class caBossEspell;
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<caBossEspell> p;
class caBossEspell :
	public caEspell
{
public:
	caBossEspell(pcCharacter boss, tstring spellName, unsigned long lcolor1, unsigned long lcolor2);
public:
	virtual ~caBossEspell(void);


	virtual int process(cRenderDevice *pDev);
	virtual int Draw(cRenderDevice *pDev);
	

	virtual int Init();

	virtual pcCharacter boss(){return boss_.lock();};

	void setRestHPper(double restHPper){restHPper_ = restHPper;};
	void setTurn(int turn, bool isBig);
	void setRestSpellNum(int num){restSpellNum_ = num;};
protected:
	virtual int preprocess(cRenderDevice *pDev);

	wpcCharacter boss_;

	cDrawingObject spellDO;
	cDrawingObject hpDO;

	double restHPper_;
	int restSpellNum_;
	int turn_;
	cNumField spellTurn_chip;


public:

protected:
	double hp_tinkle;

	c4DVector rightbottom;

	static const int POSITION_BEGIN_Y = (SCREEN_Y/2+BOXSIZE);
	static const int POSITION_BEGIN_X = (SCREEN_X/2+BOXSIZE*3.6);
	static const int POSITION_END_Y = (SCREEN_Y/2-BOXSIZE*3.2);
};


