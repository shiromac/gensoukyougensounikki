#pragma once
#include "cdroping.h"

#include "utility/cDiscreteProbability.h"

#define CDROPING_cEquipment_BASE_ID_NUM 1000
#define CDROPING_cBullet_BASE_ID_NUM 2000
#define CDROPING_cSpell_BASE_ID_NUM 3000
#define CDROPING_cFood_BASE_ID_NUM 4000
#define CDROPING_cDrink_BASE_ID_NUM 5000
#define CDROPING_cBook_BASE_ID_NUM 6000
#define CDROPING_cBox_BASE_ID_NUM 7000
#define CDROPING_cMoneyBag_BASE_ID_NUM 8000



class cItem;
#include <boost/pointer_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
typedef boost::shared_ptr<cItem> pcItem;
typedef boost::weak_ptr<cItem> wpcItem;

class cItem :
	public cDroping
{
public:
	cItem(void);
public:
	virtual ~cItem(void);

	//データパックを初期最適化する。アイテムを初期設置するときに一度だけ使う。
	virtual void DataBeginOptimize(int difficulty);

	virtual void AppreciationBeginOptimize(int difficulty);

	virtual cDiscreteProbability stateBeginDP(int difficulty);
	virtual cDiscreteProbability qualityBeginDP(int difficulty);

	virtual StyleString FullName();
	virtual StyleString FullNameSub();
	virtual StyleString ShortName();
	virtual StyleString ShortNameSub();


	virtual StyleString 識別未識別混合道具名(){return FullNameBase();};

	virtual StyleString ExtraHatString();



	virtual StyleString QualityCaption();
	virtual StyleString MemoCaption();
	virtual StyleString NamingCaption();
	virtual StyleString OptionCaption(){return _T("");};

	virtual bool 被箱入れ可(){return true;};


	virtual void longExplanationExtra(std::vector<StyleString> &explanations);
	virtual void longExplanationExtraBase(std::vector<StyleString> &explanations);



	virtual bool 被拾得可(){return true;};
	virtual bool 売却可(){return true;};

	virtual void setattribute()
	{
		属性.insert(落ち物属性::物体);	
		属性.insert(落ち物属性::アイテム);
		属性追加();
	};
	virtual void 属性追加(){};
};
