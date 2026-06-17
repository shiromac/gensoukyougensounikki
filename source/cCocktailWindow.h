#pragma once
#include "cgamewindow.h"
#include "cCommand.h"

#include <vector>
#include <map>
using namespace std;

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cCommand> pcCommand;

class cCocktailWindow;
typedef boost::shared_ptr<cCocktailWindow> pcCocktailWindow;

class cCocktailWindowDelegateBase;
typedef boost::shared_ptr<cCocktailWindowDelegateBase> pcCocktailWindowDelegateBase;

class cCocktailWindow :
	public cSelectWindow
{
public:
	cCocktailWindow(void);

	//cCocktailWindowÇÕècâ°ÇÃéwíËÇ™ïsâ¬î\
	virtual void Init(cRenderDevice *pDev, int letterXnum, int letterYnum);
	virtual void Init(cRenderDevice *pDev, pcDroping pbox);
	virtual void Init(cRenderDevice *pDev, vector<pcDroping> materialList, vector<int> materials, bool unappreciatedCooktailable);

	virtual int ReRendarText();
	
	void setDelegate(cCocktailWindowDelegateBase* delegateObject){delegateObject_ = pcCocktailWindowDelegateBase(delegateObject);};

protected:
	virtual int TextDraw(cRenderDevice *pDev);
	virtual int CursorDraw(cRenderDevice *pDev);
	//virtual int pageDraw(cRenderDevice *pDev);

	virtual int RerenderDraw(cRenderDevice *pDev);

	vector<pcDroping> materialList_;
	pcDroping pbox_;

	vector<int> materials_;
	vector<int> firstMaterials_;

	bool unappreciatedCooktailable_;

	cRenderTexture* m_pTexture_Multiselect;


	int cursolIndexXMax;
	int cursolIndexX;
	int cursolIndexYMax;
	int cursolIndexY;


	virtual StyleString shortExplanationString(cRenderDevice *pDev);
	pcGameWindow pMaterialW_;
	pcGameWindow pFromCaptionW_;
	pcGameWindow pToCaptionW_;
	pcSelectWindow pFromSelectW_;
	pcSelectWindow pToSelectW_;

	pcCocktailWindowDelegateBase delegateObject_;

public:
	virtual ~cCocktailWindow(void);


	virtual int process(cRenderDevice *pDev);
	virtual int Draw(cRenderDevice *pDev);

	virtual int pageIndexMaxF(){return pageIndexMax;};
};

class cCocktailWindowDelegateBase {
public:
	cCocktailWindowDelegateBase(){};
	virtual ~cCocktailWindowDelegateBase(){};
	virtual void didFinishCocktail(const vector<pcDroping>& materialList, const vector<pcDroping>& resultCocktails) = 0;
	virtual void didCancelCocktail() = 0;
};

template < class T >
class cCocktailWindowDelegate : public cCocktailWindowDelegateBase {
public:
	cCocktailWindowDelegate(boost::shared_ptr<T> delegateObject)
	{
		_delegateObject = boost::weak_ptr<T>(delegateObject);
	};
	virtual ~cCocktailWindowDelegate(){};

	virtual void didFinishCocktail(const vector<pcDroping>& materialList, const vector<pcDroping>& resultCocktails)
	{
		if(_delegateObject.lock()) {
			_delegateObject.lock()->didFinishCocktail(materialList, resultCocktails);
		}
	};
	virtual void didCancelCocktail()
	{
		if(_delegateObject.lock()) {
			_delegateObject.lock()->didCancelCocktail();
		}
	};

protected:
	boost::weak_ptr<T> _delegateObject;
};