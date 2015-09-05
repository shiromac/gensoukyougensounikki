#pragma once

#include "fileManage_ph.h"

#include <vector>




//継承用セーブデータクラス?
//
//
//

//load
//decodeDataはデータが不適切だと失敗するので注意
class cSaveClass
{
public:
	cSaveClass(void);
	cSaveClass(tstring savename);
	void constsub();
public:
	virtual ~cSaveClass(void);


	virtual int save(void);
	virtual int load(void);

	virtual int convertData(std::vector<SByte> & vc);
	virtual int decodeData(const std::vector<SByte> & vc);
public:
	tstring & savename(){return savename_;};
	std::vector<std::vector<SByte>> & vv_char(){return vv_char_;};
	std::vector<std::vector<int>> & vv_int(){return vv_int_;};
	std::vector<std::vector<double>> & vv_double(){return vv_double_;};
	std::vector<std::vector<tstring>> & vv_str(){return vv_str_;};
	std::vector<std::vector<pcSaveClass>> & vv_pcsc(){return vv_pcsc_;};
protected:
	tstring savename_;
	std::vector<std::vector<SByte>> vv_char_;
	std::vector<std::vector<int>> vv_int_;
	std::vector<std::vector<double>> vv_double_;
	std::vector<std::vector<tstring>> vv_str_;
	std::vector<std::vector<pcSaveClass>> vv_pcsc_;

	pcFileManager pcfm_;
};
