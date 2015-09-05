#pragma once

//mt19937arをパックしたクラス


#include <vector>

class cRandomGen
{
public:
	cRandomGen(void);
public:
	virtual ~cRandomGen(void);

	void init_genrand(unsigned long s);
	void init_by_array(unsigned long init_key[], int key_length);

	//状態を保存する//vector<unsigned long>[627]
	void saveState(std::vector<unsigned long>& output);
	void loadState(std::vector<unsigned long>& input);

	// generates a random number on [0,1)-real-interval
	inline double random(){return genrand_real2();};
	inline double operator()(){return random();};


	//Range[min,max)
	inline double random_range(double min,double max)
	{
		return min + genrand_real2()*(max - min);
	};

	


	// generates a random number on [0,1]-real-interval
	double genrand_real1(void);
	// generates a random number on [0,1)-real-interval
	double genrand_real2(void);
	// generates a random number on (0,1)-real-interval
	double genrand_real3(void);
	// generates a random number on [0,1) with 53-bit resolution
	double genrand_res53(void);


private:
	std::vector<unsigned long> mt; /* the array for the state vector  */
	unsigned long mti; /* mti==N+1 means mt[N] is not initialized */
    unsigned long mag01[2];
 

public:

	long genrand_int31(void);
	unsigned long genrand_int32(void);

};
