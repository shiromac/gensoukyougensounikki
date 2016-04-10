#include "stdafx.h"

#include "cDropingDistribution.h"

#include "GameObjects.h"

#include "utility/debug.h"

static const int EquipmentGroupID = 0;
static const int BulletGroupID = 1;
static const int SpellGroupID = 2;
static const int FoodGroupID = 3;
static const int DrinkGroupID = 4;
static const int BookGroupID = 5;
static const int BoxGroupID = 6;
static const int MoneyBagGroupID = 7;
static const int OtherGroupID = 8;

cDropingDistribution::cDropingDistribution(void)
{
	setItemGroupPos(
		10,//Equipment
		2,//Bullet
		4,//Spell
		4,//Food
		4,//Drink
		8,//Book
		5,//Box
		8,//MoneyBag
		0//Other
		);
}

cDropingDistribution::~cDropingDistribution(void)
{

}

int cDropingDistribution::setItemGroupPos(		
		double Equipment,
		double Bullet,
		double Spell,
		double Food,
		double Drink,
		double Book,
		double Box,
		double MoneyBag,
		double Other
		)
{
	clear();

	setnewGroup(Equipment);
	setnewGroup(Bullet);
	setnewGroup(Spell);
	setnewGroup(Food);
	setnewGroup(Drink);
	setnewGroup(Book);
	setnewGroup(Box);
	setnewGroup(MoneyBag);
	setnewGroup(Other);

	_Equipment = Equipment;
	_Bullet = Bullet;
	_Spell = Spell;
	_Food = Food;
	_Drink = Drink;
	_Book = Book;
	_Box = Box;
	_MoneyBag = MoneyBag;
	_Other = Other;

	return true;
}

int cDropingDistribution::getItemGroupPos(	
		double* Equipment,
		double* Bullet,
		double* Spell,
		double* Food,
		double* Drink,
		double* Book,
		double* Box,
		double* MoneyBag,
		double* Other
		) const
{
	if(Group.size() != 9) {
		return false;
	}

	*Equipment = _Equipment;
	*Bullet = _Bullet;
	*Spell = _Spell;
	*Food = _Food;
	*Drink = _Drink;
	*Book = _Book;
	*Box = _Box;
	*MoneyBag = _MoneyBag;
	*Other = _Other;

	return true;
}

int cDropingDistribution::changeItemGroupPos(	
		double Equipment,
		double Bullet,
		double Spell,
		double Food,
		double Drink,
		double Book,
		double Box,
		double MoneyBag,
		double Other
		)
{
	if(Group.size() != 9)
	{
		setItemGroupPos(
			Equipment,
			Bullet,
			Spell,
			Food,
			Drink,
			Book,
			Box,
			MoneyBag,
			Other);
		return true;
	}

	cDiscreteProbability::clear();

	setoldGroup(Equipment,EquipmentGroupID);
	setoldGroup(Bullet,BulletGroupID);
	setoldGroup(Spell,SpellGroupID);
	setoldGroup(Food,FoodGroupID);
	setoldGroup(Drink,DrinkGroupID);
	setoldGroup(Book,BookGroupID);
	setoldGroup(Box,BoxGroupID);
	setoldGroup(MoneyBag,MoneyBagGroupID);
	setoldGroup(Other,OtherGroupID);

	_Equipment = Equipment;
	_Bullet = Bullet;
	_Spell = Spell;
	_Food = Food;
	_Drink = Drink;
	_Book = Book;
	_Box = Box;
	_MoneyBag = MoneyBag;
	_Other = Other;

	return true;
}

int cDropingDistribution::get(double random_value) const
{
	return cDiscreteProbabilityGrouped::get(random_value);
}

int cDropingDistribution::clear()
{
	return cDiscreteProbabilityGrouped::clear();
}

bool cDropingDistribution::isExistDropItemID(const int ID)
{
#define def_isExistDropItemID_groupIDcheck(group) \
	if(index == group##GroupID) {\
		if(_##group <= 0) {\
			continue;\
		}\
	}

	int index, size = Group.size();
	for(index = 0; index < size; index++) {
		def_isExistDropItemID_groupIDcheck(Equipment)
		else def_isExistDropItemID_groupIDcheck(Bullet)
		else def_isExistDropItemID_groupIDcheck(Spell)
		else def_isExistDropItemID_groupIDcheck(Food)
		else def_isExistDropItemID_groupIDcheck(Drink)
		else def_isExistDropItemID_groupIDcheck(Book)
		else def_isExistDropItemID_groupIDcheck(Box)
		else def_isExistDropItemID_groupIDcheck(MoneyBag)
		else def_isExistDropItemID_groupIDcheck(Other)

		std::map<double, int>::iterator itr = Group[index].begin();
		for(;itr != Group[index].end(); itr++) {
			if(itr->second == ID) {
				return true;
			}
		}
	}
	return false;
}

int cDropingDistribution::set(int ID, double power)
{

	pcDroping pdrop = sg_pDungeonSystem->DataBase.GetSampleDroping(ID);
	if(pdrop == NULL)
	{
		OnAssert(_T(__FILE__),__LINE__, false, setStyle(ID).c_str(),_T(" 存在しないIDのアイテムを出現分布に挿入しようとしました。"));
		return false;
	}

	switch(ID/1000)
	{
	case CDROPING_cEquipment_BASE_ID_NUM/1000:
	//Equipment
		cDiscreteProbabilityGrouped::set(0,
			pdrop->レア度() *power,
			ID);

		break;
	case CDROPING_cBullet_BASE_ID_NUM/1000:
	//Equipment
		cDiscreteProbabilityGrouped::set(1,
			pdrop->レア度() *power,
			ID);

		break;
	case CDROPING_cSpell_BASE_ID_NUM/1000:
	//Equipment
		cDiscreteProbabilityGrouped::set(2,
			pdrop->レア度() *power,
			ID);

		break;
	case CDROPING_cFood_BASE_ID_NUM/1000:
	//Equipment
		cDiscreteProbabilityGrouped::set(3,
			pdrop->レア度() *power,
			ID);

		break;
	case CDROPING_cDrink_BASE_ID_NUM/1000:
	//Equipment
		cDiscreteProbabilityGrouped::set(4,
			pdrop->レア度() *power,
			ID);

		break;
	case CDROPING_cBook_BASE_ID_NUM/1000:
	//Equipment
		cDiscreteProbabilityGrouped::set(5,
			pdrop->レア度() *power,
			ID);

		break;
	case CDROPING_cBox_BASE_ID_NUM/1000:
	//Equipment
		cDiscreteProbabilityGrouped::set(6,
			pdrop->レア度() *power,
			ID);

		break;
	case CDROPING_cMoneyBag_BASE_ID_NUM/1000:
	//Equipment
		cDiscreteProbabilityGrouped::set(7,
			pdrop->レア度() *power,
			ID);

		break;

		/*
	case CDROPING_cOther_BASE_ID_NUM/1000:
	//Equipment

		break;
		*/
	default:
		cDiscreteProbabilityGrouped::set(8,
			pdrop->レア度() *power,
			ID);

		break;
	}

	return true;
}

int cDropingDistribution::set(tstring name, double power)
{

	return set(sg_pDungeonSystem->DataBase.GetSampleDroping(name)->ID(),
		power);
}

int cDropingDistribution::set(vector<int> IDs, double power)
{
	int i;
	for(i=0;i<IDs.size();i++)
	{
		set(IDs[i],power);
	}
	return true;
}
	