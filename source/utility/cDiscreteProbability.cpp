#include "cDiscreteProbability.h"

cDiscreteProbability::cDiscreteProbability(void)
{
	Sum = 0;
}

cDiscreteProbability::~cDiscreteProbability(void)
{
}

int cDiscreteProbability::set(double pos, int value)
{
	//Sumëùâ¡
	Sum += pos;
	insert(pair<double, int>(Sum,value));


	if(pos > 0)
		memory.insert(pair<double, int>(pos,value));

	includes.insert(value);

	return true;
}
int cDiscreteProbability::set(double pos, vector<int> values)
{
	int i;
	for(i=0;i<values.size();i++)
	{
		set(pos, values[i]);
	}
	return true;
}
	

int cDiscreteProbability::get(double random_value) const
{
	if(!size()) return 0;//ãÛ

	double find_value = random_value*Sum;

	map<double, int>::const_iterator itr;
 
	itr = upper_bound(find_value);


	if(itr == end()) return (--itr)->second;
	
	return itr->second;
}
int cDiscreteProbability::getRare(double random_value, double Rarity) const
{
	cDiscreteProbability DP = *this;
	DP.changeRareDP(Rarity);
	return DP.get(random_value);
}
void cDiscreteProbability::changeRareDP(double Rarity)
{
	if(memory.empty()) return;
	Rarity = max<double>(Rarity,0);
	Rarity = min<double>(Rarity,1);
	int size = memory.size();
	int position = size*Rarity;

	int i;

	multimap<double, int>::iterator itr = memory.begin();
	for(i=0;i<position;itr++,i++)
	{
		if(itr==memory.end())
		{
			itr--;
			break;
		}
	}
	multimap<double, int>::iterator itrposi;
	double find_value = itr->first;
	itrposi = memory.lower_bound(find_value);//à»è„ÇÃç≈èâÇÃíl
	if(itrposi==memory.begin())
	{//óvëfÇ»Çµ
		itrposi = memory.upper_bound(find_value);//âzÇ¶ÇÈç≈èâÇÃíl
	}
	if(itrposi==memory.begin())
	{//ÇªÇÍÇ≈Ç‡óvëfÇ»Çµ(Ç†ÇËÇ¶Ç»Ç¢ÇÕÇ∏)
		return;
	}
	
	cDiscreteProbability DP;
	itr = memory.begin();
	for(;itr!=itrposi;itr++)
	{
		if(itr==memory.end()) break;

		DP.set(itr->first,itr->second);
	}

	*this = DP;
}

int cDiscreteProbability::clear()
{
	Sum = 0;

	map<double, int>::clear();

	return true;
}

//------------------------------------------
//cDiscreteProbabilityGrouped
//------------------------------------------

cDiscreteProbabilityGrouped::cDiscreteProbabilityGrouped(void)
{
	Sum = 0;
}

cDiscreteProbabilityGrouped::~cDiscreteProbabilityGrouped(void)
{
}


int cDiscreteProbabilityGrouped::getGroup(double& randomOutput, double random_value) const
{
	double find_value = random_value*Sum;

	map<double, int>::const_iterator itr, tempitr;
 
	itr = upper_bound(find_value);


	if(itr == end()) itr--;

	double low,high;

	if(itr == begin())
	{
		low = 0;
		high = itr->first;
	}
	else
	{
		tempitr = itr;
		tempitr--;
		low = tempitr->first;
		high = itr->first;
	}

	randomOutput = (find_value - low)/(high-low);
	
	return itr->second;
}
//ÉOÉãÅ[ÉvÇï‘Ç∑
int cDiscreteProbabilityGrouped::setnewGroup(double pos)
{
	//Sumëùâ¡
	Sum += pos;


	Group.push_back(cDiscreteProbability());

	insert(pair<double, int>(Sum,Group.size()-1));

	return Group.size()-1;
}
int cDiscreteProbabilityGrouped::setoldGroup(double pos, int groupindex)
{
	//Sumëùâ¡
	Sum += pos;
	insert(pair<double, int>(Sum, groupindex));
	return Group.size()-1;
}

int cDiscreteProbabilityGrouped::set(int group, double pos, int value)
{
	if(group < 0 || Group.size() <= group) return false;


	Group[group].set(pos,value);


	return true;
}

//ílàÊ[0 ~ 1)
int cDiscreteProbabilityGrouped::get(double random_value) const
{
	double newpos;
	int groupindex = getGroup(newpos,random_value);
	return Group[groupindex].get(newpos);
}

int cDiscreteProbabilityGrouped::clear()
{
	Sum = 0;

	Group.clear();


	cDiscreteProbability::clear();

	return true;
}
int cDiscreteProbabilityGrouped::getRare(double random_value, double Rarity) const
{
	cDiscreteProbabilityGrouped DP = *this;
	DP.changeRareDP(Rarity);
	return DP.get(random_value);
}
void cDiscreteProbabilityGrouped::changeRareDP(double Rarity)//ÉåÉAÇcÇoÇ…ïœâªÇ≥ÇπÇÈ
{
	int i;
	for(i=0;i<Group.size();i++)
	{
		Group[i].changeRareDP(Rarity);
	}
}