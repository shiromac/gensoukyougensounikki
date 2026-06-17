#pragma once
#include "cPlayerChara.h"
#include "ceaiActive.h"



class cOtherChara;
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
typedef boost::shared_ptr<cOtherChara> pcOtherChara;
typedef boost::weak_ptr<cOtherChara> wpcOtherChara;

class cOtherChara :
	public cPlayerChara
{
public:
	cOtherChara(void);
public:
	virtual ~cOtherChara(void);

	virtual pcEnemyAI Get_kindofAI(){return pcEnemyAI(new ceaiNoMotion);};

	virtual bool ‚·‚êˆá‚¢‹–‰Â(pcCharacter pchara);

	virtual void OptionDraw(cRenderDevice *pDev){};

	virtual int HaveEXP(){return ExpTable(LV);};
	
	virtual int canTalk();

	virtual int TalkEvent();
};



#define CCHARACTER_cOtherChara_Event_BASE_ID_NUM 9000
//newŠÖ”’è‹`
#define DEF_FUNC_new_cOtherChara_Event_Class(z, n, data) \
	pcCharacter def_GetInstanceOf##data##_ID_##n()\
{\
	return pcCharacter(new data(n + CCHARACTER_cOtherChara_Event_BASE_ID_NUM));\
};

class cOtherChara_Event : public cOtherChara
{
public:
	cOtherChara_Event(int ID);
public:
	virtual ~cOtherChara_Event(void);
	
	virtual inline int ID(){return _realID;};

	//—Bˆê–¼
	virtual tstring uniqueName();//{return _T("cCharacter_undefined");};
	
	//Œ©‚½–Ú
	virtual tstring surfaceName();//{return usefulName();};

	//ƒLƒƒƒ‰‚Ì•ûŒü‚ÌŒ^
	virtual int GetTex_aspect_type();

public:
	int simulationID(){return _simulationID;};
	void setID(int ID);
private:
	int _simulationID;
	int _realID;
};