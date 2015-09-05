#include "cDataConverter.h"
#include "cSaveClass.h"

//using namespace fileManage;
using namespace std;

union UIntInByte
{
	unsigned int ui;
	SByte c[4];
};
const unsigned int unsigned_int_size = sizeof(unsigned int);

union IntInByte
{
	int i;
	SByte c[4];
};
const unsigned int int_size = sizeof(int);



int cDataConverter::VecVecC2VecC(const std::vector<std::vector<SByte>>& vvc, std::vector<SByte>& vc)
{
	unsigned int ui,uii,culsize, allsize = 0;
	
	//解析
	UIntInByte msize;//vectorの数
	msize.ui = vvc.size();
	allsize += unsigned_int_size;

	vector<UIntInByte> dindex;//data[x]の開始アドレス
	vector<UIntInByte> csize;//vector<vector>の内部データ数
	
	dindex.resize(msize.ui);
	csize.resize(msize.ui);

	allsize += unsigned_int_size * msize.ui;
	
	for(ui=0;ui<msize.ui;ui++)
	{
		dindex[ui].ui = allsize;
		allsize += csize[ui].ui = vvc[ui].size();
	}

	//格納
	vc.resize(allsize);
	culsize = 0;
	for(ui=0;ui<unsigned_int_size;(ui++,culsize++))
	{//msize
		vc[culsize] = msize.c[ui];
	}

	for(uii=0;uii<msize.ui;uii++)
	{
		for(ui=0;ui<unsigned_int_size;(ui++,culsize++))
		{//dindex
			vc[culsize] = dindex[uii].c[ui];
		}
	}
	
	for(uii=0;uii<msize.ui;uii++)
	{
		for(ui=0;ui<csize[uii].ui;(ui++,culsize++))
		{//csize
			vc[culsize] = vvc[uii][ui];
		}
	}

	return SUCCESS;
}
int cDataConverter::VecC2VecVecC(const std::vector<SByte>& vc, std::vector<std::vector<SByte>>& vvc)
{
	unsigned int ui,uii,culsize, datasize = 0, allsize = vc.size();
	UIntInByte msize;//vectorの数
	vector<UIntInByte> csize;//vector<vector>の内部データ数
	vector<UIntInByte> dindex;//data[x]の開始アドレス
	//msize.ui = 0;

	if(allsize < unsigned_int_size) return ERROR_FM_CONVERT_V2VV_SHORTDATA;
	
	culsize = 0;
	for(ui=0;ui<unsigned_int_size;(ui++,culsize++))
	{//msizeの復元
		msize.c[ui] = vc[culsize];
	}
	
	if(allsize < culsize + unsigned_int_size * msize.ui) return ERROR_FM_CONVERT_V2VV_SHORTDATA;
	
	vvc.resize(msize.ui);
	dindex.resize(msize.ui+1);
	csize.resize(msize.ui);


	for(uii=0;uii<msize.ui;uii++)
	{
		for(ui=0;ui<unsigned_int_size;(ui++,culsize++))
		{//dindex
			dindex[uii].c[ui] = vc[culsize];
		}
	}
	dindex[uii].ui = allsize;

	for(uii=0;uii<msize.ui;uii++)
	{//csize
		csize[uii].ui = dindex[uii+1].ui - dindex[uii].ui;
		datasize += csize[uii].ui;
	}

	if(allsize < culsize + datasize) return ERROR_FM_CONVERT_V2VV_SHORTDATA;
	
	for(uii=0;uii<msize.ui;uii++)
	{
		vvc[uii].resize(csize[uii].ui);

		for(ui=0;ui<csize[uii].ui;(ui++,culsize++))
		{//csize
			vvc[uii][ui] = vc[culsize];
		}
	}

	return SUCCESS;
}



//変換

//tstring変換//tstringはvec<SByte>に近いけどあえてこっちで//サイズ変更により動きません
/*
int cDataConverter::Convert2VecC(const std::vector<tstring>& v_str, std::vector<SByte>& vc)
{
	unsigned int ui,uii,msize = v_str.size();
	vc.resize(msize);
	vector<vector<SByte>> vvc_str;
	vvc_str.resize(msize);

	for(ui=0;ui<msize;ui++)
	{
		vvc_str[ui].assign(v_str[ui].begin(),v_str[ui].end());
	}
	VecVecC2VecC(vvc_str,vc);

	return SUCCESS;
}
int cDataConverter::DecodeVecC(const std::vector<SByte>& vc, std::vector<tstring>& v_str)
{
	int error = 0;
	vector<vector<SByte>> vvc_str;

	error = VecC2VecVecC(vc,vvc_str);
	if(error) return error;

	unsigned int ui,uii,msize = vvc_str.size();
	v_str.resize(msize);

	for(ui=0;ui<msize;ui++)
	{
		v_str[ui].assign(vvc_str[ui].begin(),vvc_str[ui].end());
	}

	return SUCCESS;
}
*/
int cDataConverter::ConvertT2VecC(const tstring& str, std::vector<SByte>& vc)
{
	vector<TCHAR> vtchar(str.begin(),str.end());

	Convert2VecC(vtchar,vc);

	return SUCCESS;
}
int cDataConverter::DecodeVecC2T(const std::vector<SByte>& vc, tstring& str)
{
	str.clear();
	int error = 0;
	vector<TCHAR> vtchar;

	DecodeVecC(vc,vtchar);

	str.assign(vtchar.begin(),vtchar.end());

	//str.push_back(_T('\0'));
	
	return SUCCESS;
}
int cDataConverter::ConvertT2VecC(const bool& tmpl, std::vector<SByte>& vc)
{
	vc.resize(1);
	vc[0] = tmpl;
	return SUCCESS;
}
int cDataConverter::DecodeVecC2T(const std::vector<SByte>& vc, bool& tmpl)
{
	if(vc.size() < 1) return ERROR_FM_LOAD_DECODE_ERROR;
	tmpl = vc[0];
	return SUCCESS;
}

//DecodeVecCはデータが不適切だと失敗するので注意
//pcSaveClass変換
int cDataConverter::Convert2VecC(const std::vector<pcSaveClass>& v_pcsc, std::vector<SByte>& vc)
{
	unsigned int ui,uii,msize = v_pcsc.size();
	vc.resize(msize);
	vector<vector<SByte>> vvc_pcsc;
	vvc_pcsc.resize(msize);

	for(ui=0;ui<msize;ui++)
	{
		v_pcsc[ui]->convertData(vvc_pcsc[ui]);
	}
	VecVecC2VecC(vvc_pcsc,vc);

	return SUCCESS;
}
	
int cDataConverter::DecodeVecC(const std::vector<SByte>& vc, std::vector<pcSaveClass>& v_pcsc)
{
	int error = 0;
	vector<vector<SByte>> vvc_pcsc;

	error = VecC2VecVecC(vc,vvc_pcsc);
	if(error) return error;

	unsigned int ui,uii,msize = vvc_pcsc.size();
	v_pcsc.resize(msize);

	for(ui=0;ui<msize;ui++)
	{
		v_pcsc[ui] = pcSaveClass(new cSaveClass);
		error |= v_pcsc[ui]->decodeData(vvc_pcsc[ui]);
	}

	if(error) return error;

	return SUCCESS;
}


/*
int cDataConverter::Convert2VecC(std::vector<int>& v_int, std::vector<SByte>& vc)
{
	unsigned int ui,msize = v_int.size() * sizeof(int);
	vc.resize(msize);
	SByte* first = (SByte*)&v_int[0];

	for(ui=0;ui<msize;ui++)
	{
		vc[ui] = first[ui];
	}

	return SUCCESS;
}
int cDataConverter::DecodeVecC(std::vector<SByte>& vc, std::vector<int>& v_int)
{
	unsigned int ui,msize = vc.size() / sizeof(int);
	v_int.resize(msize);
	int* first = (int*)&vc[0];

	for(ui=0;ui<msize;ui++)
	{
		v_int[ui] = first[ui];
	}

	return SUCCESS;
}
*/