/******************************************************//*!
	@file SoundBufferItem.h
	@brief 曲・効果音再生クラスが書かれたヘッダファイル
	@bug ストリーム再生時にノイズが起こるバグがあったので確認すること
**********************************************************/

#include "SoundDecodeItem.h"
#include <dsound.h>

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")

///////////////////////////////////////////////////////////
///	@brief 曲・効果音再生クラス
///////////////////////////////////////////////////////////
class SoundBufferItem 
{
public:
	SoundBufferItem();
	virtual ~SoundBufferItem();
private:
	static const int	SPLITNUM = 4; // ストリーム再生バッファの分割数
public:
	//! ストリーム再生時の再生識別子
	enum StreamStateEnum {
		STOP, //!< 停止中
		PLAY = 1, //!< 再生中
		PAUSE = 2, //!< 一時停止中
		DESTROY = 4, //!< データがない
		LOOP = 16, //!< ループ再生
		PLAYSTART = 32 //!< 再生開始
	};
private:
	//! フェードインやフェードアウトに使用する構造体
	struct tag_Time {
		LONG svalue; //!< 開始値
		LONG evalue; //!< 終了値
		DWORD time; //!< 実行期間
		DWORD beforetime; //!< 前回の処理が終わった時間
		DWORD st; //!< 記録開始した時間
	};
	//! 音を再生するのに必要な変数がある構造体
	struct SoundBufferInfo {
		DSBUFFERDESC desc;
		tag_Time volume; //!< ボリューム調整用構造体
	};
	//! ストリーム再生する際に必要となる構造体
	struct StreamInfo {
		bool streamflag; // ストリーム再生するか、否か
		int streambufsize; //!< ストリーム再生用バッファのサイズ
		int state; //!< 再生識別子(StreamStateEnum)
		int readindex; //!< 分割されたバッファの読み込み位置
		int writeindex; //!< 分割されたバッファの書き込み位置
		DWORD readcursor; //!< 読み込み位置
		DWORD writecursor; //!< 書き込み位置
		DWORD writerest; //!< 書き込み残りバッファ数
		int stock; //!< 書き込み済みバッファ数
		char* restbuf; //!< 書き込み時にあふれたものを受け入れるバッファ
	};
	//! ストリーム再生する際に受け渡す時に使う構造体
	struct ThreadInfo {
		LPDIRECTSOUND8			lpDS;
		LPDIRECTSOUNDBUFFER		lpSB;
		SoundBufferInfo			SBInfo;
		StreamInfo				streaminfo;
		SoundDecodeItem*		pdecoder; //!< デコーダクラスのポインタ
		int						playnum; //!< 同時使用数
	};
private:
	ThreadInfo	m_ti;
	HANDLE		m_hThread; // ストリーム再生用スレッドのハンドル
private:
	void createSoundBuffer(DWORD bufsize, WAVEFORMATEX* pwfx);
	void createStreamThread();
	void deleteStreamThread();
	static void streamVolume(ThreadInfo* pti);
	static void streamSeekStart(ThreadInfo* pti);
	static void streamload(char* buf, DWORD offset, DWORD bufsize, ThreadInfo* pti);
	static unsigned WINAPI streamThread(void *p);
	bool load(DWORD bufsize);
public:
	void init(LPDIRECTSOUND8 lpDS, SoundDecodeItem* pdecoder);
	bool load_filename(tstring filename, DWORD bufsize = 0);
	bool load_handle(HANDLE handle, DWORD bufsize = 0);
	bool load_pointer(void* p, DWORD bufsize = 0);
	void changeVolume(LONG volume, DWORD time);
	void play(bool loopflag = false, bool startflag = true, bool overlapflag = true);
	void stop();
	void pause();
	void destroy();
	int getState();
};