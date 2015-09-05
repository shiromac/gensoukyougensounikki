#pragma once
#include "csaveclass.h"

class cAutoSaveClass :
	public cSaveClass
{
public:
	cAutoSaveClass(void);
public:
	virtual ~cAutoSaveClass(void);
	virtual void Init(tstring filename);

	//----------------------------------------------
	//ここにポインターを追加してvaliableReserve内でReserveすると自動セーブするデータのポインタを作ることが出来る。
	//このクラスをメンバに持つクラスにポインタのインスタンスを作成し、Reserveしてもよいが順序にはくれぐれも注意


	//
	//----------------------------------------------
protected:
	virtual void setData2Valiable();
	virtual void valiableReserve();
public:
	void Reserve(SByte*& val){pp_char_.push_back(&val);};
	void Reserve(int*& val){pp_int_.push_back(&val);};
	void Reserve(double*& val){pp_double_.push_back(&val);};
	void Reserve(tstring*& val){pp_str_.push_back(&val);};
	void Reserve(pcSaveClass val){p_csc_.push_back(val);};

	void Reserve(std::vector<SByte>*& val){pp_v_char_.push_back(&val);};
	void Reserve(std::vector<int>*& val){pp_v_int_.push_back(&val);};
	void Reserve(std::vector<double>*& val){pp_v_double_.push_back(&val);};
	void Reserve(std::vector<tstring>*& val){pp_v_str_.push_back(&val);};
	void Reserve(std::vector<pcSaveClass>& val){p_v_csc_.push_back(val);};
protected:
	std::vector<SByte**> pp_char_;
	std::vector<int**> pp_int_;
	std::vector<double**> pp_double_;
	std::vector<tstring**> pp_str_;
	std::vector<pcSaveClass> p_csc_;

	std::vector<std::vector<SByte>**> pp_v_char_;
	std::vector<std::vector<int>**> pp_v_int_;
	std::vector<std::vector<double>**> pp_v_double_;
	std::vector<std::vector<tstring>**> pp_v_str_;
	std::vector<std::vector<pcSaveClass>> p_v_csc_;
};
