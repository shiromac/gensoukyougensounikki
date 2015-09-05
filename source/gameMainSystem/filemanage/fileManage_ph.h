#pragma once
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "tstring_ph.h"


//namespace fileManage
//{
	#define BOOST_DEFINE_SHERED_WEEK_PTR_OF(classname) \
	class classname;\
	typedef boost::shared_ptr<classname> p##classname;\
	typedef boost::weak_ptr<classname> wp##classname;

	BOOST_DEFINE_SHERED_WEEK_PTR_OF(cFileManager)
	#define NULLOFcFileManager (pcFileManager((cFileManager*)(NULL)))

	//BOOST_DEFINE_SHERED_WEEK_PTR_OF(cDataConverter)
	//#define NULLOFcDataConverter (pcDataConverter((cDataConverter*)(NULL)))

	BOOST_DEFINE_SHERED_WEEK_PTR_OF(cSaveClass)
	#define NULLOFcSaveClass (pcSaveClass((cSaveClass*)(NULL)))

	BOOST_DEFINE_SHERED_WEEK_PTR_OF(cScriptReader)
	#define NULLOFcScriptReader (pcScriptReader((cScriptReader*)(NULL)))

	BOOST_DEFINE_SHERED_WEEK_PTR_OF(cScriptRLayer)
	#define NULLOFcScriptRLayer (pcScriptRLayer((cScriptRLayer*)(NULL)))

	BOOST_DEFINE_SHERED_WEEK_PTR_OF(LuaScript)
	#define NULLOFLuaScript (pLuaScript((LuaScript*)(NULL)))

	//エラー出力系
	const int SUCCESS = 0;
	const int ERROR_FM_CONVERT_V2VV_SHORTDATA = (1<<16);//CONVERT_V2VVでエラー(主にdataの長さが不足)
	const int ERROR_FM_FILENAME_NULL = (1<<15);//ファイルネームでエラー(ファイルネーム未設定)
	const int ERROR_FM_LOAD_DECODE_ERROR = (1<<14);//CONVERT_V2VVでエラー(ロードデコードでエラー)
	const int ERROR_FM_FILE_ERROR = (1<<13);//FILEでエラー
	

	const int VERROR = 1;//汎用エラー
	//}

	#define SUBSTITUTION_L2R(L,R) (R)=(L)
	#define SUBSTITUTION_R2L(L,R) (L)=(R)

	struct FileTime {
    unsigned long dwLowDateTime;
    unsigned long dwHighDateTime;
};