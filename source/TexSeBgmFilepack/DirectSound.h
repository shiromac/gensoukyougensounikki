#pragma once

//#include "DirectSound.h"

//################################################################
// DirectSoundで音を鳴らす
// ボリュームは　-10000 ～ 0 の範囲が使えるが-3000くらいでもう完全に聞こえない
//################################################################


#include <stdio.h>
#include <dsound.h>
#include "GameCommon.h"

#define DIRECTSOUND_VOLUME_MAX 0
#define DIRECTSOUND_VOLUME_MINIMUM -3000	// 最小は-10000だが、もうこのくらいで聞こえないはず


class DirectSound
{
protected:

	static LPDIRECTSOUND8 m_pDSound;	// DirectSoundオブジェクト
	static bool m_Init_Flag;			// 初期化したか？
	static HWND m_hWnd;
	long m_volume;						// 音量
	LPDIRECTSOUNDBUFFER8		m_pDSBuffer;	//DirectSoundセカンダリーバッファー

private:

	static int m_class_count;					// このオブジェクトが作られた数

public:
    DirectSound();
    virtual ~DirectSound();

public:

	// ===========================================================================
	// =======必須設定関数========================================================

	// 初期化　一回どこかで呼べばOK。次からはLoadSound関数だけ呼んで設定すればいい
	static HRESULT InitSound(HWND);
	
	// =======================================================================
	// =======設定関数========================================================

	// サウンドを読み込む
	virtual void Create(TCHAR* szFileName, int fSeek=0);

	// ボリュームを設定
	virtual void SetVolume_ByRate(int volParsentage=100);

	// ボリュームを直接細かく設定　0が最大音量
	virtual void SetVolume(long Volume=0);

	virtual void Play(bool isContine=false, bool isLoop=false)
	{
		if(m_pDSBuffer != NULL)
		{
			if(!isContine)	m_pDSBuffer->SetCurrentPosition(0);
			if(isLoop)		m_pDSBuffer->Play(NULL,	0,	DSBPLAY_LOOPING);
			else			m_pDSBuffer->Play(NULL,	0,	NULL);
		}
	}
	virtual void Stop(void)
	{
		if(m_pDSBuffer != NULL)	m_pDSBuffer->Stop();
	}

protected:
	void Free();

};