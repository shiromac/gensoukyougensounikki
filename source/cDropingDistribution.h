#pragma once

#include "utility/cDiscreteProbability.h"

#include <tstring_ph.h>


#include <vector>
using namespace std;

class cDropingDistribution
	:protected cDiscreteProbabilityGrouped
{
public:
	cDropingDistribution(void);
public:
	virtual ~cDropingDistribution(void);

protected:
		double _Equipment;
		double _Bullet;
		double _Spell;
		double _Food;
		double _Drink;
		double _Book;
		double _Box;
		double _MoneyBag;
		double _Other;
public:

	int setItemGroupPos(	
		double Equipment,
		double Bullet,
		double Spell,
		double Food,
		double Drink,
		double Book,
		double Box,
		double MoneyBag,
		double Other
		);

	int changeItemGroupPos(	
		double Equipment,
		double Bullet,
		double Spell,
		double Food,
		double Drink,
		double Book,
		double Box,
		double MoneyBag,
		double Other
		);

	virtual int get(double random_value) const;


	virtual int set(int ID, double power = 1);
	virtual int set(tstring name, double power = 1);


	virtual int set(vector<int> IDs, double power = 1);
	

	virtual void changeRareDP(double Rarity){return cDiscreteProbabilityGrouped::changeRareDP(Rarity);};//ƒŒƒA‚c‚o‚É•Ï‰»‚³‚¹‚é
	
	virtual int clear();

	int getItemGroupPos(	
		double* Equipment,
		double* Bullet,
		double* Spell,
		double* Food,
		double* Drink,
		double* Book,
		double* Box,
		double* MoneyBag,
		double* Other
		) const;

	bool isExistDropItemID(const int ID);
};
