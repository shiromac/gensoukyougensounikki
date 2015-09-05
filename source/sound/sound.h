//_ 
//_ sound.h
//_ 08/07/08
//_ ver 1.04
//_ [ver1.00(??/??/??)] 完成
//_ [ver1.01(??/??/??)] エコークラスを作る。
//_ [ver1.02(??/??/??)] mp3に微妙な対応をする。
//_ [ver1.03(07/03/08)] mp3大体対応する。
//_ [ver1.04(08/07/08)] ファイルを分割　ストリーム再生対応　ポインタ対応
//_ [ver1.05(10/01/31)] tstring対応

#pragma once

#include <windows.h>
#include <mmreg.h>
#include <dsound.h>
#include <vector>
#include <string>
#include <map>
#include "SoundBufferItem.h"

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")

using namespace std;

#define UNKNOWN_VOLUME	1

class Sound
{
public:
	Sound();
	virtual ~Sound();
private:
	struct SOUNDBUFFERINFO {
		LONG			pan;
		LONG			volume;
		DWORD			frq;
		WAVEFORMATEX	wfx;
		DSBCAPS			dsbc;
		DSBUFFERDESC	desc;
	};
private:
	// DirectSound8オブジェクトのポインタを格納する
	LPDIRECTSOUND8				m_lpDS;
	// アプリケーションのプライマリバッファとして機能する
	// サウンドバッファである。
	LPDIRECTSOUNDBUFFER				m_lpDSP;
	vector<SoundDecodeItem*>		m_sditems;
	map<tstring, SoundBufferItem*>	m_sbitems;
	multimap<int, SoundBufferItem*>	m_mapsbitems;	// 音量の一括管理などに使用する
private:
	// もう使わなくなったので捨て。
	void release();
	int loadSoundbyHandle(HANDLE handle, char* str4, DWORD bufsize, int layer, tstring filename);
public:
	enum {
		NOT_FILE,
		PLAY,
		STOP
	};
public:
	// プライマリバッファまで初期化
	int init(HWND hWnd);
	void insertSoundItem(SoundDecodeItem* psi);
	void play(tstring id, bool startflag = false, bool loopflag = false, bool overlapflag = true);
	void stop(tstring id);
	void changeVolume(tstring id, LONG volume, DWORD time);
	void changePan(tstring id, LONG pan);
	void change_speed(tstring id, float speed);
	DWORD checktime(tstring id);
	void settime(tstring id, DWORD time);
	int getstate(tstring id);
	void pause(tstring id);
	DWORD getBuffersize(tstring id);
	// 音を読み込む
	int loadSound(tstring filename, DWORD bufsize, int layer, tstring datname = TEXT(""));
	void releaseSB();
	// レイヤーごとに処理する
	void stopLayer(int layer);
	void changeVolumeLayer(int layer, LONG volume, DWORD time);
};
