#include "../../stdafx.h"
#include "cSaveClass.h"
#include "cFileManager.h"
#include "cDataConverter.h"
//using namespace fileManage;

cSaveClass::cSaveClass(void)
{
	savename_ = _T("");

	constsub();
}
cSaveClass::cSaveClass(tstring savename)
{
	savename_ = savename;

	constsub();
}
void cSaveClass::constsub()
{
	pcfm_ = pcFileManager(new cFileManager);
}

cSaveClass::~cSaveClass(void)
{
}

int cSaveClass::convertData(std::vector<SByte> & vc)
{

	std::vector<std::vector<SByte>> v_packdata;
	std::vector<std::vector<SByte>> vv_data;
	std::vector<SByte> v_data;

	//SByte
	cDataConverter::VecVecC2VecC(vv_char_,v_data);
	v_packdata.push_back(v_data);

	//int
	cDataConverter::Convert2VecVecC(vv_int_,vv_data);
	cDataConverter::VecVecC2VecC(vv_data,v_data);
	v_packdata.push_back(v_data);

	//double
	cDataConverter::Convert2VecVecC(vv_double_,vv_data);
	cDataConverter::VecVecC2VecC(vv_data,v_data);
	v_packdata.push_back(v_data);

	//str
	cDataConverter::Convert2VecVecC(vv_str_,vv_data);
	cDataConverter::VecVecC2VecC(vv_data,v_data);
	v_packdata.push_back(v_data);

	//pcsc
	cDataConverter::Convert2VecVecC(vv_pcsc_,vv_data);
	cDataConverter::VecVecC2VecC(vv_data,v_data);
	v_packdata.push_back(v_data);

	cDataConverter::VecVecC2VecC(v_packdata,vc);

	return SUCCESS;
}
int cSaveClass::decodeData(const std::vector<SByte> & vc)
{
	
	int error = 0;
	std::vector<std::vector<SByte>> v_packdata;
	std::vector<std::vector<SByte>> vv_data;
	std::vector<SByte> v_data;


	error |= cDataConverter::VecC2VecVecC(vc,v_packdata);
	
	if(v_packdata.size() < 5) return error | ERROR_FM_LOAD_DECODE_ERROR;
	error |= cDataConverter::VecC2VecVecC(v_packdata[0],vv_char_);

	error |= cDataConverter::VecC2VecVecC(v_packdata[1],vv_data);
	error |= cDataConverter::DecodeVecVecC(vv_data,vv_int_);

	error |= cDataConverter::VecC2VecVecC(v_packdata[2],vv_data);
	error |= cDataConverter::DecodeVecVecC(vv_data,vv_double_);

	error |= cDataConverter::VecC2VecVecC(v_packdata[3],vv_data);
	error |= cDataConverter::DecodeVecVecC(vv_data,vv_str_);

	error |= cDataConverter::VecC2VecVecC(v_packdata[4],vv_data);
	error |= cDataConverter::DecodeVecVecC(vv_data,vv_pcsc_);

	if(error) return error | ERROR_FM_LOAD_DECODE_ERROR;
	
	return SUCCESS;
}

int cSaveClass::save(void)
{
	
	if(savename_ == _T("")) return ERROR_FM_FILENAME_NULL;
	int error = 0;

	std::vector<SByte> savedata;


	convertData(savedata);


#ifdef _UNRELEASE
	error |= pcfm_->saveFile(savename_,savedata);
#else
	error |= pcfm_->saveEncryptFile(savename_,savedata);
#endif

	if(error) return error;

	return SUCCESS;
}
int cSaveClass::load(void)
{
	
	if(savename_ == _T("")) return ERROR_FM_FILENAME_NULL;
	int error = 0;

	std::vector<SByte> loaddata;

#ifdef _UNRELEASE
	error |= pcfm_->loadFile(savename_,loaddata);
#else
	error |= pcfm_->loadEncryptFile(savename_,loaddata);
#endif

	if(error) return error;

	error |= decodeData(loaddata);
	if(error) return error;

	return SUCCESS;
}