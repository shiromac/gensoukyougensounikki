#pragma once
#include "cAnimation.h"
#include "utility/SpriteText.h"

#include <vector>
#include <list>
#include <tstring_ph.h>


using namespace std;


class caFade :
	public cAnimation
{
public:
	caFade(void);
public:
	virtual ~caFade(void);


	virtual int process(cRenderDevice *pDev);
	virtual int Draw(cRenderDevice *pDev);
	virtual int Init(int OutorIn);

	virtual bool isOnScreen();

	double count;
	double speed;

	int inorout;
	SpriteText m_SpriteText;
	
	const static int ANIME_FADE_STEP_OUT = 32;
	const static int ANIME_FADE_STEP_IN = 180;
	const static int ANIME_FADE_IN = 0;
	const static int ANIME_FADE_OUT = 1;
};

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<caFade> pcaFade;

class caFadeGotoDungeon :
	public cAnimation
{
public:
	caFadeGotoDungeon(void);
public:
	virtual ~caFadeGotoDungeon(void);


	virtual int process(cRenderDevice *pDev);
	virtual int Draw(cRenderDevice *pDev);

	virtual bool isOnScreen();

	double count;
	double speed;

	tstring DungeonID;
	map<tstring,int> privateFlags;
	tstring savefileName;

	const static int ANIME_FADE_STEP_OUT = 32;

private:
	int mode_;
};

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<caFadeGotoDungeon> pcaFadeGotoDungeon;

class cSaveQuest;
typedef boost::shared_ptr<cSaveQuest> pcSaveQuest;

class caFadeContinueDungeon :
	public cAnimation
{
public:
	caFadeContinueDungeon(void);
public:
	virtual ~caFadeContinueDungeon(void);


	virtual int process(cRenderDevice *pDev);
	virtual int Draw(cRenderDevice *pDev);

	virtual bool isOnScreen();

	double count;
	double speed;

	pcSaveQuest continueQuest;

	const static int ANIME_FADE_STEP_OUT = 32;

private:
	int mode_;
};
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<caFadeContinueDungeon> pcaFadeContinueDungeon;


