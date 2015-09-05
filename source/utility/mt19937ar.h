#pragma once

double genrand_real2(void);

void init_genrand(unsigned long s);

inline double random()
{
	return genrand_real2();
};

//[0,max)
inline int random_int(int max)
{
	return genrand_real2()*max;
};


//Range[min,max)
inline double random_range(double min,double max)
{
	return min + genrand_real2()*(max - min);
};