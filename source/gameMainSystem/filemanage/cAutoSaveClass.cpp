#include "cAutoSaveClass.h"

cAutoSaveClass::cAutoSaveClass(void)
{

}
cAutoSaveClass::~cAutoSaveClass(void)
{
	save();
}

void cAutoSaveClass::Init(tstring filename)
{
	valiableReserve();
	setData2Valiable();

	savename_ = filename;
	load();
}

void cAutoSaveClass::valiableReserve()
{
	//----------------------------------------------
	//ポインターをReserveすると自動セーブする。



	//
	//----------------------------------------------
}


void cAutoSaveClass::setData2Valiable()
{
	unsigned int i;
	vv_char_.resize(pp_v_char_.size() + 1);
	vv_int_.resize(pp_v_int_.size() + 1);
	vv_double_.resize(pp_v_double_.size() + 1);
	vv_str_.resize(pp_v_str_.size() + 1);

	vv_char_[0].resize(pp_char_.size());
	for(i=0;i<vv_char_[0].size();i++)
	{
		*(pp_char_[i]) = &(vv_char_[0][i]);
	}

	vv_int_[0].resize(pp_int_.size());
	for(i=0;i<vv_int_[0].size();i++)
	{
		*(pp_int_[i]) = &(vv_int_[0][i]);
	}

	vv_double_[0].resize(pp_double_.size());
	for(i=0;i<vv_double_[0].size();i++)
	{
		*(pp_double_[i]) = &(vv_double_[0][i]);
	}

	vv_str_[0].resize(pp_str_.size());
	for(i=0;i<vv_str_[0].size();i++)
	{
		*(pp_str_[i]) = &(vv_str_[0][i]);
	}


	for(i=0;i<pp_v_char_.size();i++)
	{
		*(pp_v_char_[i]) = &(vv_char_[i+1]);
	}

	for(i=0;i<pp_v_int_.size();i++)
	{
		*(pp_v_int_[i]) = &(vv_int_[i+1]);
	}

	for(i=0;i<pp_v_double_.size();i++)
	{
		*(pp_v_double_[i]) = &(vv_double_[i+1]);
	}

	for(i=0;i<pp_v_str_.size();i++)
	{
		*(pp_v_str_[i]) = &(vv_str_[i+1]);
	}

}