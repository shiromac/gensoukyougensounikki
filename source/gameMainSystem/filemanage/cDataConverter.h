#pragma once

#include <vector>
#include <map>

#include "fileManage_ph.h"
#include <tstring_ph.h>


//----------------------------------
//2GBを超えるデータはエラーを起こします

//VecC2VecVecC
//DecodeVecVecC
//DecodeVecCはデータが不適切だと失敗するので注意
	
//vector<SByte>とvector<vector<SByte>>の変換機
//---仕様---
//vector<vector<SByte>> data;
//の時
//SByte[4] msize = data.size()
//SByte[4] dindex[0] = data[0]の開始アドレス
//...
//SByte[4] dindex[msize-1] = data[msize-1]の開始アドレス
//SByte[csize[0]] = data[0];
//...
//SByte[csize[msize-1]] = data[msize-1];
//のSByte列に変換する

namespace cDataConverter
{
	//VecC2VecVecCはデータが不適切だと失敗するので注意
	int VecVecC2VecC(const std::vector<std::vector<SByte>>& vvc, std::vector<SByte>& vc);
	int VecC2VecVecC(const std::vector<SByte>& vc, std::vector<std::vector<SByte>>& vvc);


	//DecodeVecCはデータが不適切だと失敗するので注意
	//tstring変換
	int ConvertT2VecC(const tstring& str, std::vector<SByte>& vc);
	int DecodeVecC2T(const std::vector<SByte>& vc, tstring& str);
	inline int BackDecodeVecC2T(tstring& str,const std::vector<SByte>& vc)
	{return DecodeVecC2T(vc,str);}

	//int Convert2VecC(const std::vector<tstring>& v_str, std::vector<SByte>& vc);
	//int DecodeVecC(const std::vector<SByte>& vc, std::vector<tstring>& v_str);

	//DecodeVecCはデータが不適切だと失敗するので注意
	//pcSaveClass変換
	int Convert2VecC(const std::vector<pcSaveClass>& v_pcsc, std::vector<SByte>& vc);
	int DecodeVecC(const std::vector<SByte>& vc, std::vector<pcSaveClass>& v_pcsc);
	inline int BackDecodeVecC( std::vector<pcSaveClass>& v_pcsc,const std::vector<SByte>& vc)
	{return DecodeVecC(vc,v_pcsc);}

	//Ｔは実データのみ対応
	//変換
	template <class T> int Convert2VecC(const std::vector<T>& v_tmpl, std::vector<SByte>& vc)
	{
		unsigned int ui,msize = v_tmpl.size() * sizeof(T);
		vc.resize(msize);
		if(msize)
		{
			SByte* first = (SByte*)&v_tmpl[0];

			for(ui=0;ui<msize;ui++)
			{
				vc[ui] = first[ui];
			}
		}
		return SUCCESS;
	};

	template <class T> int DecodeVecC(const std::vector<SByte>& vc, std::vector<T>& v_tmpl)
	{
		unsigned int ui,msize = vc.size() / sizeof(T);
		v_tmpl.resize(msize);
			
		if(msize)
		{
			T* first = (T*)&vc[0];

			for(ui=0;ui<msize;ui++)
			{
				v_tmpl[ui] = first[ui];
			}
		}

		return SUCCESS;
	};
	template <class T> inline int BackDecodeVecC(std::vector<T>& v_tmpl,const std::vector<SByte>& vc)
	{return DecodeVecC(vc,v_tmpl);}

	//Ｔは実データのみ対応
	//変換
	template <class T> int ConvertT2VecC(const T& tmpl, std::vector<SByte>& vc)
	{
		unsigned int ui,msize = sizeof(T);
		vc.resize(msize);
		if(msize)
		{
			SByte* first = (SByte*)&tmpl;

			for(ui=0;ui<msize;ui++)
			{
				vc[ui] = first[ui];
			}
		}
		return SUCCESS;
	};

	template <class T> int DecodeVecC2T(const std::vector<SByte>& vc, T& tmpl)
	{
		int msize = sizeof(T);
		if(vc.size() < msize) return ERROR_FM_LOAD_DECODE_ERROR;

		T* first = (T*)&vc[0];

		tmpl = *first;
			
		return SUCCESS;
	};
	template <class T> inline int BackDecodeVecC2T(T& tmpl, const std::vector<SByte>& vc)
	{return DecodeVecC2T(vc,tmpl);}
	int ConvertT2VecC(const bool& tmpl, std::vector<SByte>& vc);
	int DecodeVecC2T(const std::vector<SByte>& vc, bool& tmpl);


	//Ｔは実データのみ対応
	//変換
	template <class T> int Convert2VecVecC(const std::vector<std::vector<T>>& vv_tmpl, std::vector<std::vector<SByte>>& vvc)
	{
		unsigned int ui,msize = vv_tmpl.size();
		vvc.resize(msize);

		for(ui=0;ui<msize;ui++)
		{
			Convert2VecC(vv_tmpl[ui], vvc[ui]);
		}

		return SUCCESS;
	};

	//DecodeVecVecCはデータが不適切だと失敗するので注意
	template <class T> int DecodeVecVecC(const std::vector<std::vector<SByte>>& vvc, std::vector<std::vector<T>>& vv_tmpl)
	{
		int error = 0;
		unsigned int ui,msize = vvc.size();
		vv_tmpl.resize(msize);

		for(ui=0;ui<msize;ui++)
		{
			error |= DecodeVecC(vvc[ui], vv_tmpl[ui]);
		}

		if(error) return error;

		return SUCCESS;
	};
	template <class T> inline int BackDecodeVecVecC( std::vector<std::vector<T>>& vv_tmpl, const std::vector<std::vector<SByte>>& vvc)
	{return DecodeVecVecC(vvc,vv_tmpl);}

	//Ｔはあらゆるクラスに対応
	//変換
	template <class T> int ConvertVT2VecVecC(const std::vector<T>& v_tmpl, std::vector<std::vector<SByte>>& vc)
	{
		unsigned int ui,msize = v_tmpl.size();
		vc.resize(msize);
		if(msize)
		{
			for(ui=0;ui<msize;ui++)
			{
				ConvertT2VecC(v_tmpl[ui],vc[ui]);
			}
		}
		return SUCCESS;
	};

	template <class T> int DecodeVecVecC2VT(const std::vector<std::vector<SByte>>& vc, std::vector<T>& v_tmpl)
	{
		unsigned int ui,msize = vc.size() / sizeof(T);
		v_tmpl.resize(msize);
			
		if(msize)
		{
			for(ui=0;ui<msize;ui++)
			{
				DecodeVecC2T(vc[ui],v_tmpl[ui]);
			}
		}

		return SUCCESS;
	};
	template <class T> inline int BackDecodeVecVecC2VT(std::vector<T>& v_tmpl, const std::vector<std::vector<SByte>>& vc)
	{return DecodeVecVecC2VT(vc,v_tmpl);}

	//Ｔはあらゆるクラスに対応
	//変換
	template <class T> int ConvertVT2VecVecC2VecC(const std::vector<T>& v_tmpl, std::vector<SByte>& vc)
	{
		unsigned int ui,msize = v_tmpl.size();
		std::vector<std::vector<SByte>> vvc;
		vvc.resize(msize);
		if(msize)
		{
			for(ui=0;ui<msize;ui++)
			{
				ConvertT2VecC(v_tmpl[ui],vvc[ui]);
			}
		}
		VecVecC2VecC(vvc,vc);
		return SUCCESS;
	};

	template <class T> int DecodeVecC2VecVecC2VT(const std::vector<SByte>& vc, std::vector<T>& v_tmpl)
	{
		std::vector<std::vector<SByte>> vvc;
		VecC2VecVecC(vc,vvc);
		unsigned int ui,msize = vvc.size();
		v_tmpl.resize(msize);

		if(msize)
		{
			for(ui=0;ui<msize;ui++)
			{
				DecodeVecC2T(vvc[ui],v_tmpl[ui]);
			}
		}
		
		return SUCCESS;
	};

	template <class T> inline int BackDecodeVecC2VecVecC2VT(std::vector<T>& v_tmpl, const std::vector<SByte>& vc)
	{return DecodeVecC2VecVecC2VT(vc,v_tmpl);}


	//Ｔはあらゆるクラスに対応
	//変換
	template <class T> int ConvertVVT2VecVecVecC2VecC(const std::vector<std::vector<T>>& vv_tmpl, std::vector<SByte>& vc)
	{
		unsigned int ui,msize = vv_tmpl.size();
		std::vector<std::vector<SByte>> vvc;
		vvc.resize(msize);
		if(msize)
		{
			for(ui=0;ui<msize;ui++)
			{
				ConvertVT2VecVecC2VecC(vv_tmpl[ui],vvc[ui]);
			}
		}
		VecVecC2VecC(vvc,vc);
		return SUCCESS;
	};

	template <class T> int DecodeVecC2VecVecVecC2VVT(const std::vector<SByte>& vc, std::vector<std::vector<T>>& vv_tmpl)
	{
		std::vector<std::vector<SByte>> vvc;
		VecC2VecVecC(vc,vvc);
		unsigned int ui,msize = vvc.size();
		vv_tmpl.resize(msize);

		if(msize)
		{
			for(ui=0;ui<msize;ui++)
			{
				DecodeVecC2VecVecC2VT(vvc[ui],vv_tmpl[ui]);
			}
		}
		
		return SUCCESS;
	};

	template <class T> inline int BackDecodeVecC2VecVecVecC2VVT(std::vector<std::vector<T>>& vv_tmpl, const std::vector<SByte>& vc)
	{return DecodeVecC2VecVecVecC2VVT(vc,vv_tmpl);}

	//Ｔはあらゆるクラスに対応
	//変換
	template <class T1, class T2> int ConvertMTT2VecVecC2VecC(const std::map<T1,T2>& map_tmpl, std::vector<SByte>& vc)
	{
		unsigned int ui,msize = map_tmpl.size();
		std::vector<std::vector<SByte>> vvc;
		std::map<T1,T2>::const_iterator itr = map_tmpl.begin();
		vvc.resize(msize*2);
		if(msize)
		{
			for(ui=0;ui<msize;ui++)
			{
				ConvertT2VecC(itr->first,vvc[ui*2]);
				ConvertT2VecC(itr->second,vvc[ui*2+1]);
				itr++;
			}
		}
		VecVecC2VecC(vvc,vc);
		return SUCCESS;
	};

	template <class T1, class T2> int DecodeVecC2VecVecC2MTT(const std::vector<SByte>& vc, std::map<T1,T2>& map_tmpl)
	{
		std::vector<std::vector<SByte>> vvc;
		VecC2VecVecC(vc,vvc);
		unsigned int ui,msize = vvc.size()/2;

		if(msize)
		{
			T1 t1;
			T2 t2;
			for(ui=0;ui<msize;ui++)
			{

				DecodeVecC2T(vvc[ui*2],t1);
				DecodeVecC2T(vvc[ui*2+1],t2);
				map_tmpl.insert(pair<T1,T2>(t1,t2));
			}
		}
		
		return SUCCESS;
	};

	template <class T1, class T2> inline int BackDecodeVecC2VecVecC2MTT(std::map<T1,T2>& map_tmpl, const std::vector<SByte>& vc)
	{return DecodeVecC2VecVecC2MTT(vc,map_tmpl);}

	//Ｔはあらゆるクラスに対応
	//変換
	template <class T1, class T2> int ConvertMTVT2VecVecC2VecC(const std::map<T1,std::vector<T2>>& map_tmpl, std::vector<SByte>& vc)
	{
		unsigned int ui,msize = map_tmpl.size();
		std::vector<std::vector<SByte>> vvc;
		std::map<T1,std::vector<T2>>::const_iterator itr = map_tmpl.begin();
		vvc.resize(msize*2);
		if(msize)
		{
			for(ui=0;ui<msize;ui++)
			{
				ConvertT2VecC(itr->first,vvc[ui*2]);
				ConvertVT2VecVecC2VecC(itr->second,vvc[ui*2+1]);
				itr++;
			}
		}
		VecVecC2VecC(vvc,vc);
		return SUCCESS;
	};

	template <class T1, class T2> int DecodeVecC2VecVecC2MTVT(const std::vector<SByte>& vc, std::map<T1,std::vector<T2>>& map_tmpl)
	{
		std::vector<std::vector<SByte>> vvc;
		VecC2VecVecC(vc,vvc);
		unsigned int ui,msize = vvc.size()/2;

		if(msize)
		{
			T1 t1;
			std::vector<T2> t2;
			for(ui=0;ui<msize;ui++)
			{

				DecodeVecC2T(vvc[ui*2],t1);
				DecodeVecC2VecVecC2VT(vvc[ui*2+1],t2);
				map_tmpl.insert(pair<T1,std::vector<T2>>(t1,t2));
			}
		}
		
		return SUCCESS;
	};

	template <class T1, class T2> inline int BackDecodeVecC2VecVecC2MTVT(std::map<T1,std::vector<T2>>& map_tmpl, const std::vector<SByte>& vc)
	{return DecodeVecC2VecVecC2MTVT(vc,map_tmpl);}
/*
	int Convert2VecC(std::vector<int>& v_int, std::vector<SByte>& vc);
	int DecodeVecC(std::vector<SByte>& vc, std::vector<int>& v_int);
*/
};




