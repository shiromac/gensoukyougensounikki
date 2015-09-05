#pragma once

//################################################################
// ストリーミング再生をする 主にBGMのみ
// オブジェクトは複数作っても同時再生は出来ない
// ファイルパックされたデータのファイル名と目的のBGMのSeek位置を指定する

// ※[重要]※
// 連続して次のBGM再生にはWAVのフォーマットは統一しないといけない
//################################################################

/*
一度どこかで初期化さえすればあとはファイル名を読み込めばOK

DirectSound::InitSound(hWnd);
DirectSoundStream::hNotifyEvent = CreateEvent( NULL, FALSE, FALSE, NULL );
DirectSoundStream::hNotifyThread = CreateThread( NULL, 0 , DirectSoundStream::NotifyProc, DirectSoundStream::GetHWnd(), 0 , 0);

*/

#include "DirectSound.h"
#include <vector>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>

#define	DIRECTSOUND_MUSIC_STREAMBUFFER_SECTIONNUM	3			// バッファ分割数
#define	DIRECTSOUND_MUSIC_STREAMBUFFER_SECTIONSIZE	(1024*32)	// (1KB * 32)


//※※※※※※※※※※※※※※※※※※※
// ストリーミングに必要なDATA情報
//※※※※※※※※※※※※※※※※※※※
class PlayMusicInfo
{
public:
	WAVEFORMATEX	format;
	TCHAR*			FileName;
	int				FileSeek;
	DWORD			wavSize;
	DWORD			wavOffSet;
	PlayMusicInfo() : FileSeek(0), wavSize(0), wavOffSet(0)	{}
	virtual ~PlayMusicInfo()	{}
};

class DirectSoundStream : public DirectSound
{

public:

	static HANDLE hNotifyEvent;		//イベントのハンドル
	static HANDLE hNotifyThread;	//スレッドのハンドル

protected:

	static std::vector<PlayMusicInfo>		vecInfo;	// 流すBGMデータのコンテナ
	static BYTE* m_pbWavData;		// 作業領域。1時事的にココに読み込んでバッファにコピーする
	static DWORD m_dwCurWavRead;	// WAVデータをどれだけ読み込んだか。WAV読み込みカーソル
	static DWORD m_dwNumNotify;		// 通知回数。バッファの分割数
	static DWORD BufSectionSize;	// 1区間のサイズ。通知を発生する間隔のバイトサイズ
	static int	SectionID;			// 区間ID
	static LPDIRECTSOUNDBUFFER m_pDSStreamBuffer;	//DirectSoundセカンダリーバッファー
	static boost::interprocess::interprocess_semaphore
      DSStreamBuffer_semaphore;

public:
    DirectSoundStream();
    virtual ~DirectSoundStream();

public:

	virtual void Create(TCHAR* FileName, int fSeek);		// 初期化
	void AttendBGM(TCHAR* FileName, int fSeek);			// BGMを追加
	void ChangeBGM(TCHAR* FileName, int fSeek);			// サウンドを変える  突然変わるよ

	virtual void SetVolume_ByRate(int volParsentage);
	virtual void SetVolume(long Volume);

	// スレッド関数 一定時間ごとにシグナルになるのでそこでデータ変更
	// スレッドが自動で呼び出すので、オブジェクトから呼び出すの禁止
	static DWORD WINAPI NotifyProc(LPVOID);

	// ハンドルクローズ
	static void HandleClose(void);

	//=============================================================
	// バッファを再生。ストリーミングバッファは、環状バッファであるので、
	// ループで再生しないとうまく行かない
	//==============================================================
	virtual void Play(bool NoSetting=false, bool NoSetting2=false)
	{
		if(m_pDSStreamBuffer != NULL)
		{
			m_pDSStreamBuffer->Play(NULL,0,DSBPLAY_LOOPING);
		}
	}
	virtual void Stop(void)
	{
		if(m_pDSStreamBuffer != NULL)	m_pDSStreamBuffer->Stop();
	}

	// =======================================================================

protected:

	// ストリームバッファにデータを書き込む
	static void WriteStream(void);

	// wavデータを読み込む
	static BYTE* ReadWavFile(TCHAR* FileName, DWORD wavDataOffSet, DWORD ReadCur, DWORD ReadSize);

	// ストリーミングに必要なBGMの情報を得る
	bool GetMusicDataInfo(PlayMusicInfo *pInfo, TCHAR* FileName, int fSeek); 


};