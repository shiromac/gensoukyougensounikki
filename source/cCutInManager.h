#pragma once

#include "cDroping.h"
#include "cCharacter.h"
#include "cLandform.h"

#include "cAttackinformation.h"

#include "utility\valiableField\cValiableField.h"

class cEvent;
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
typedef boost::shared_ptr<cEvent> pcEvent;
typedef boost::weak_ptr<cEvent> wpcEvent;

#include <set>
class cCutInManager
{
public:
	cCutInManager(void);
public:
	virtual ~cCutInManager(void);


	/*
	double CutInValue(pcCharacter pchara,int timing);
	double CutInValue(pcDroping pdrop,int timing);
	double CutInValue(pcLandform pland,int timing);


	int CutInEvent(pcCharacter pchara,int timing);
	int CutInEvent(pcDroping pdrop,int timing);
	int CutInEvent(pcLandform pland,int timing);


	void CutInAttack(pcCharacter pchara,int timing,cAttackinformation & attinfo);
	void CutInAttack(pcDroping pdrop,int timing,cAttackinformation & attinfo);
	void CutInAttack(pcLandform pland,int timing,cAttackinformation & attinfo);
*/

	virtual void CutInForAllObject(タイミング timing, cValiableField& valiable);
	inline void CutInForAllObject(タイミング timing){
		cValiableField val;
		CutInForAllObject(timing, val);
	};

	virtual void CutInForAllChara(タイミング timing, cValiableField& valiable);
	virtual void CutInForAllDrop(タイミング timing, cValiableField& valiable);
	//virtual void CutInForAllLand(タイミング timing, cValiableField& valiable);
	virtual void CutInForAllEvent(タイミング timing, cValiableField& valiable);
	inline void CutInForAllChara(タイミング timing){
		cValiableField val;
		CutInForAllChara(timing, val);
	};
	inline void CutInForAllDrop(タイミング timing){
		cValiableField val;
		CutInForAllDrop(timing, val);
	};
	//inline void CutInForAllLand(タイミング timing){
	//	cValiableField val;
	//	CutInForAllLand(timing, val);
	//};
	inline void CutInForAllEvent(タイミング timing){
		cValiableField val;
		CutInForAllEvent(timing, val);
	};

	virtual void CutIn(pcCharacter pchara,タイミング timing, cValiableField& valiable);
	virtual void CutIn(pcDroping pdrop,タイミング timing, cValiableField& valiable);
	virtual void CutIn(pcLandform pland,タイミング timing, cValiableField& valiable);
	virtual void CutIn(pcEvent pevent,タイミング timing, cValiableField& valiable);
	inline void CutIn(pcCharacter pchara,タイミング timing){
		cValiableField val;
		CutIn(pchara, timing, val);
	};
	inline void CutIn(pcDroping pdrop,タイミング timing){
		cValiableField val;
		CutIn(pdrop, timing, val);
	};
	inline void CutIn(pcLandform pland,タイミング timing){
		cValiableField val;
		CutIn(pland, timing, val);
	};
	inline void CutIn(pcEvent pevent,タイミング timing){
		cValiableField val;
		CutIn(pevent, timing, val);
	};

	//自分
	virtual void CutIn(タイミング timing, cValiableField& valiable);


	virtual void clearMemCutIn();
protected:
	virtual void CutIn_pri(pcCharacter pchara,タイミング timing, cValiableField& valiable);
	virtual void CutIn_pri(pcDroping pdrop,タイミング timing, cValiableField& valiable);
	virtual void CutIn_pri(pcLandform pland,タイミング timing, cValiableField& valiable);
	virtual void CutIn_pri(pcEvent pevent,タイミング timing, cValiableField& valiable);

	virtual void memCutIn(pcDroping pointer, タイミング timing);
	virtual void memCutIn(pcCharacter pointer, タイミング timing);
	virtual void memCutIn(pcLandform pointer, タイミング timing);
	virtual void memCutIn(pcEvent pointer, タイミング timing);

	virtual void erasememCutIn(pcDroping pointer, タイミング timing);
	virtual void erasememCutIn(pcCharacter pointer, タイミング timing);
	virtual void erasememCutIn(pcLandform pointer, タイミング timing);
	virtual void erasememCutIn(pcEvent pointer, タイミング timing);


	virtual bool ExistCutIn(pcDroping pointer, タイミング timing);
	virtual bool ExistCutIn(pcCharacter pointer, タイミング timing);
	virtual bool ExistCutIn(pcLandform pointer, タイミング timing);
	virtual bool ExistCutIn(pcEvent pointer, タイミング timing);

	
	std::set<std::pair<pcDroping,タイミング>> droppointerpool_;
	std::set<std::pair<pcCharacter,タイミング>> charapointerpool_;
	std::set<std::pair<pcLandform,タイミング>> landpointerpool_;
	std::set<std::pair<pcEvent,タイミング>> eventpointerpool_;

	int count_;
};