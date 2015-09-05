#pragma once
//リリースバージョンでない
//
//exeのロケーション→プリプロセッサマクロに変更
#ifdef _ARCHIVE
	#define _EXELOCATION _T(".\\")
#ifdef _DEBUG
	#define _UNRELEASE
#endif
#ifdef _LUADEBUG;
	#define _UNRELEASE
#endif
#else
	#define _EXELOCATION _T("..\\")
	#define _UNRELEASE
#endif

#define _LOGFOLDER _EXELOCATION _T("log\\")

#define RELEASE(pointer) if((pointer)!=NULL){delete(pointer);pointer=NULL;}
#define RELEASE_ARRAY(pointer) if((pointer)!=NULL){delete[](pointer);pointer=NULL;}
