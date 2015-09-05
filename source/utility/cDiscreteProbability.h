#pragma once
#include <set>
#include <map>
#include <vector>
using namespace std;

class cDiscreteProbability
	:protected map<double, int>
{
public:
	cDiscreteProbability(void);
public:
	virtual ~cDiscreteProbability(void);

protected:
	double Sum;

	multimap<double, int> memory;
	set<int> includes;
public:

	virtual inline double getSum(){return Sum;};

	//値域[0 ~ 1)
	virtual int get(double random_value) const;

	//要素のレア度上位から[Rarity]の割合の要素（切捨て）のみからランダム（低速）
	virtual int getRare(double random_value, double Rarity) const;
	virtual void changeRareDP(double Rarity);//レアＤＰに変化させる

	virtual int set(double pos, int value);
	virtual int set(double pos, vector<int> values);

	virtual int clear();

	virtual bool empty(){return map<double, int>::empty();};
};

class cDiscreteProbabilityGrouped
	:protected cDiscreteProbability
{
public:
	cDiscreteProbabilityGrouped(void);
public:
	virtual ~cDiscreteProbabilityGrouped(void);

protected:
	

	vector<cDiscreteProbability> Group;
public:

	virtual inline double getSum(){return Sum;};

	//値域[0 ~ 1)
	virtual int get(double random_value) const;
	virtual int getGroup(double& randomOutput, double random_value) const;

	//要素のレア度上位から[Rarity]の割合の要素（切捨て）のみからランダム（低速）
	virtual int getRare(double random_value, double Rarity) const;
	virtual void changeRareDP(double Rarity);//レアＤＰに変化させる

	//グループを返す
	virtual int setnewGroup(double pos);
	virtual int set(int group, double pos, int value);


	virtual int clear();

protected:
	virtual int setoldGroup(double pos, int groupindex);

};
