/******************************************************//*!
	@file SoundDecodeItem.h
	@brief 曲・効果音デコード用基底クラスが書かれたヘッダファイル
**********************************************************/
#pragma once

#include <windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
using namespace std;

///////////////////////////////////////////////////////////
///	@brief 曲・効果音デコード用基底クラス
///////////////////////////////////////////////////////////
class SoundDecodeItem {
public:
	SoundDecodeItem();
	virtual ~SoundDecodeItem();
protected:
	//! デコードする際の読み込み用構造体
	struct LoadInfo {
		bool	fileflag; //!< ファイル名で読み込みかどうか
		tstring	filename; //!< デコードする音楽ファイル名
		HANDLE	filehandle; //!< デコードする音楽ファイル先のハンドル
		char*	filep; //!< デコードする音楽ファイル先のポインタ
		DWORD	loadbufsize; //!< ストリーム再生する場合のバッファサイズ（0の場合、全読み込み
		//! 初期化関数
		void clear() {
			fileflag = false;
			filename = TEXT("");
			filehandle = NULL;
			filep = NULL;
			loadbufsize = 0;
		};
	};
	//! デコードする際に使用する変数まとめ構造体
	struct DecodeInfo {
		WAVEFORMATEX	wfx; //!< フォーマット構造体
		char*			buf; //!< 書き込み用バッファ
		DWORD			bufsize; //!< 書き込み用バッファサイズ
	};
protected:
	DecodeInfo			m_decodeinfo; //!< デコード用変数まとめ構造地亜
	LoadInfo			m_loadinfo; //!< デコードする際の読み込み用構造体
public:
	void setLoadInfo_Filename(tstring filename, DWORD loadbufsize = 0);
	void setLoadInfo_Handle(HANDLE handle, DWORD loadbufsize = 0);
	void setLoadInfo_Pointer(void* filep, DWORD loadbufsize = 0);
	void getHeader(WAVEFORMATEX** out_wfx);
	void setHeader(WAVEFORMATEX& in_wfx);
	void getData(char** buffer, DWORD& buffersize);
	virtual void destory();
	virtual SoundDecodeItem* copy() = 0;
	virtual bool loadHeader() = 0;
	virtual void loadDataInit() = 0;
	virtual void loadDataStart() = 0;
	virtual void loadDataEnd() = 0;
	virtual void loadDataClose() = 0;
	virtual void loadDataReStart() = 0;
	virtual int loadData() = 0;
	virtual bool checkload(char c1, char c2, char c3, char c4) = 0;
};