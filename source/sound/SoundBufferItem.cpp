/******************************************************//*!
	@file SoundBufferItem.cpp
	@brief 曲・効果音再生クラスが書かれたソースファイル
**********************************************************/
#include "stdafx.h"
#include "SoundBufferItem.h"
#include <process.h>

///////////////////////////////////////////////////////////
///	@brief コンストラクタ
///////////////////////////////////////////////////////////
SoundBufferItem::SoundBufferItem()
{
	memset(&m_ti, 0, sizeof(ThreadInfo));
	m_hThread = NULL;
}

///////////////////////////////////////////////////////////
///	@brief デストラクタ、リソース開放も行う
///	@sa	destory	
///////////////////////////////////////////////////////////
SoundBufferItem::~SoundBufferItem()
{
	destroy();
	delete m_ti.pdecoder;
}

///////////////////////////////////////////////////////////
///	@brief 初期化
///	@param[in] lpDS DirectSound関連
///	@param[in] pdecoder デコーダ用のクラス
///////////////////////////////////////////////////////////
void SoundBufferItem::init(LPDIRECTSOUND8 lpDS, SoundDecodeItem* pdecoder)
{
	m_ti.lpDS = lpDS;
	m_ti.pdecoder = pdecoder;
}

///////////////////////////////////////////////////////////
///	@brief リソース開放
///
///	デストラクタで呼ばれるため、使った後に必ず書かなくてもよい。
///////////////////////////////////////////////////////////
void SoundBufferItem::destroy()
{
	deleteStreamThread();	
	m_ti.pdecoder->destory();
	if(m_ti.lpSB)
	{
		m_ti.lpSB->Release();
		m_ti.lpSB = NULL;
	}
	if(m_ti.streaminfo.restbuf)
		delete m_ti.streaminfo.restbuf;
}

///////////////////////////////////////////////////////////
///	@brief ロード関数
///	@param[in] bufsize ストリーム読み込み用バッファサイズ(0の場合、ストリーム再生は行わない)
///	@return ロード成功したかどうか
///////////////////////////////////////////////////////////
/// @todo : ロード失敗した時の処理も書きましょう
bool SoundBufferItem::load(DWORD bufsize)
{
	WAVEFORMATEX* pwfx;
	m_ti.pdecoder->getHeader(&pwfx);
	if(bufsize) // ストリーム再生する
	{
		m_ti.pdecoder->loadDataInit();
		m_ti.pdecoder->loadDataStart();
		createSoundBuffer(bufsize, pwfx);
		memset(&m_ti.streaminfo, 0, sizeof(StreamInfo));
		m_ti.streaminfo.streamflag = true;
		m_ti.streaminfo.streambufsize = bufsize;
		m_ti.streaminfo.state = STOP;
		m_ti.streaminfo.restbuf = new char[bufsize];
		createStreamThread();
	}
	else // ストリーム再生しない
	{
		char* buf = NULL;
		m_ti.pdecoder->loadDataInit();
		m_ti.pdecoder->loadDataStart();
		m_ti.pdecoder->loadData();
		m_ti.pdecoder->loadDataEnd();
		m_ti.pdecoder->loadDataClose();
		m_ti.pdecoder->getData(&buf, bufsize);
		createSoundBuffer(bufsize, pwfx);
		streamload(buf, 0, bufsize, &m_ti);
		m_ti.pdecoder->destory();
	}
	return true;
}

///////////////////////////////////////////////////////////
///	@brief ファイル名から読み込みする関数
///	@param[in] filename ファイル名
///	@param[in] bufsize ストリーム読み込み用バッファサイズ(0の場合、ストリーム再生は行わない)
///	@return ロード成功したかどうか
///////////////////////////////////////////////////////////
/// @todo : この下３つはpdecoderのm_loadinfoなどを使用すれば綺麗にまとまる気がする
bool SoundBufferItem::load_filename(tstring filename, DWORD bufsize)
{
	m_ti.pdecoder->setLoadInfo_Filename(filename, bufsize / SPLITNUM);
	if(!m_ti.pdecoder->loadHeader())
		return false;
	return load(bufsize);
}

///////////////////////////////////////////////////////////
///	@brief ファイルハンドルから読み込みする関数
///	@param[in] handle ファイルハンドル
///	@param[in] bufsize ストリーム読み込み用バッファサイズ(0の場合、ストリーム再生は行わない)
///	@return ロード成功したかどうか
///////////////////////////////////////////////////////////
bool SoundBufferItem::load_handle(HANDLE handle, DWORD bufsize)
{
	m_ti.pdecoder->setLoadInfo_Handle(handle, bufsize / SPLITNUM);
	if(!m_ti.pdecoder->loadHeader())
		return false;
	return load(bufsize);
}

///////////////////////////////////////////////////////////
///	@brief ポインタから読み込みする関数
///	@param[in] p 音楽データへのポインタ
///	@param[in] bufsize ストリーム読み込み用バッファサイズ(0の場合、ストリーム再生は行わない)
///	@return ロード成功したかどうか
///////////////////////////////////////////////////////////
bool SoundBufferItem::load_pointer(void *p, DWORD bufsize)
{
	m_ti.pdecoder->setLoadInfo_Pointer(p, bufsize / SPLITNUM);
	if(!m_ti.pdecoder->loadHeader())
		return false;
	return load(bufsize);
}

///////////////////////////////////////////////////////////
///	@brief DIRECTSOUNDBUFFERに書き込むクラス
///	@param[in] buf 書き込み元のデータポインタ
///	@param[in] offset 書き込む元のデータオフセット
///	@param[in] bufsize 書き込むデータサイズ
///	@param[in,out] pti スレッド用構造体
///////////////////////////////////////////////////////////
void SoundBufferItem::streamload(char* buf, DWORD offset, DWORD bufsize, ThreadInfo* pti)
{
	LPVOID pMem1,pMem2;
	DWORD size1,size2;
	LPDIRECTSOUNDBUFFER lpDSB = pti->lpSB;

	// 領域をロック
	if (FAILED(lpDSB->Lock(offset,bufsize,&pMem1,&size1,&pMem2,&size2,0)))
		return;
	// 書き込み
	memcpy(pMem1,buf,size1);
	if (size2)
		memcpy(pMem2,buf + size1,size2);
	// ロック解除
	if(FAILED(lpDSB->Unlock(pMem1,size1,pMem2,size2)))
		return;
}

///////////////////////////////////////////////////////////
///	@brief DIRECTSOUNDBUFFER作成
///	@param[in] bufsize 作成バッファサイズ
///	@param[in] pwfx 音楽フォーマットのポインタ
///////////////////////////////////////////////////////////
void SoundBufferItem::createSoundBuffer(DWORD bufsize, WAVEFORMATEX* pwfx)
{
	// descに書き込み
	memset(&m_ti.SBInfo.desc, 0, sizeof(DSBUFFERDESC));
	m_ti.SBInfo.desc.dwSize = sizeof(DSBUFFERDESC);
	m_ti.SBInfo.desc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS | DSBCAPS_LOCDEFER 
	| DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPAN;
	m_ti.SBInfo.desc.dwBufferBytes = bufsize;
	m_ti.SBInfo.desc.lpwfxFormat = pwfx;
	// バッファを作成
	if(FAILED(m_ti.lpDS->CreateSoundBuffer(&m_ti.SBInfo.desc, &m_ti.lpSB, NULL)))
		return;
}

///////////////////////////////////////////////////////////
///	@brief ストリーム再生用スレッド作成
///////////////////////////////////////////////////////////
void SoundBufferItem::createStreamThread()
{
	MyWindows::CreateThread(&m_hThread, &streamThread, (void*)&m_ti, THREAD_PRIORITY_NORMAL);
	return;
}

///////////////////////////////////////////////////////////
///	@brief ストリーム再生用スレッド消去
///////////////////////////////////////////////////////////
void SoundBufferItem::deleteStreamThread()
{
	m_ti.streaminfo.state = DESTROY;
	MyWindows::DestroyThread(m_hThread);
}

///////////////////////////////////////////////////////////
///	@brief ストリーム再生時のボリューム調整
///	@param[in,out] スレッド用構造体
///////////////////////////////////////////////////////////
void SoundBufferItem::streamVolume(ThreadInfo* pti)
{
	if(pti->SBInfo.volume.time) // ボリューム調整をする場合
	{
		double rate = 0;
		LONG nowvolume = 0;
		DWORD nowtime = timeGetTime() - pti->SBInfo.volume.st;
		DWORD onetime = timeGetTime() - pti->SBInfo.volume.beforetime;
		if(onetime > 100 || nowtime > pti->SBInfo.volume.time) // 100msごとにVolume変更
		{
			if(nowtime > pti->SBInfo.volume.time) // ボリューム調整時間が終わっていた場合
			{
				pti->SBInfo.volume.time = 0;
				rate = 1;
			}
			else // ボリューム調整する場合
				rate = (double)nowtime / pti->SBInfo.volume.time;
			nowvolume = (LONG)(pti->SBInfo.volume.svalue * (1 - rate) + pti->SBInfo.volume.evalue * rate);
			if(nowvolume == DSBVOLUME_MIN) // 現在のボリュームがミュートの場合
			{
				if(--pti->playnum == 0) // 再生数が１つだけの場合、再生をストップする
				{
					if(pti->streaminfo.streamflag)
					{
						pti->streaminfo.state = STOP;
					}
					else
					{
						pti->lpSB->Stop();
						pti->lpSB->SetCurrentPosition(0);
					}
				}
				pti->playnum = (pti->playnum > 0) ? pti->playnum : 0;
			}
			pti->lpSB->SetVolume(nowvolume);
			pti->SBInfo.volume.beforetime = timeGetTime();
		}
	}
}

/////////////////////////////////////////////////////////// 
///	@brief ストリーム再生時の初期化
///	@param[in] pti スレッド用構造体
///////////////////////////////////////////////////////////
void SoundBufferItem::streamSeekStart(ThreadInfo* pti)
{
	pti->streaminfo.readcursor = 0;
	pti->streaminfo.writecursor = 0;
	pti->streaminfo.readindex = 0;
	pti->streaminfo.writeindex = 0;
	pti->streaminfo.writerest = 0;
	pti->streaminfo.stock = 0;
	pti->pdecoder->loadDataEnd();
	pti->pdecoder->loadDataReStart();
	pti->lpSB->SetCurrentPosition(0);
}

///////////////////////////////////////////////////////////
///	@brief ストリーム再生用関数
///	@param[in] p スレッド用構造体(ThreadInfo*)
///////////////////////////////////////////////////////////
unsigned WINAPI SoundBufferItem::streamThread(void *p)
{
	ThreadInfo* pti = (ThreadInfo*)p;
	while(1)
	{
		const int STATUS = STOP | PLAY | PAUSE | DESTROY;
		int rindex = pti->streaminfo.readindex;
		DWORD splitbufsize = pti->streaminfo.streambufsize / SPLITNUM;
		DWORD rstatus;
		// カーソル位置を更新
		pti->lpSB->GetCurrentPosition(&pti->streaminfo.readcursor, NULL);
		// 書きこみを行う
		while((pti->streaminfo.state & PLAY) && !(pti->streaminfo.state & PLAYSTART) && pti->streaminfo.stock != SPLITNUM)
		{
			int windex = pti->streaminfo.writeindex;
			char* buf;
			DWORD readsize;
			DWORD bufsize;
			readsize = pti->pdecoder->loadData();
			if(readsize == 0) // 読み込み終了時
			{
				if(pti->streaminfo.state & LOOP) // ループ処理
				{
					pti->streaminfo.writecursor = 0;
					pti->pdecoder->loadDataEnd();
					pti->pdecoder->loadDataReStart();
				}
				else // 停止処理
				{
					pti->streaminfo.state = STOP;
					break;
				}
			}
			else
			{
				if(pti->streaminfo.writerest) // あまりバッファにたまってる曲を出す
				{
					DWORD rest = pti->streaminfo.writerest;
					streamload(pti->streaminfo.restbuf, splitbufsize * windex, rest, pti);
				}
				pti->pdecoder->getData(&buf, bufsize);
				if(readsize != bufsize) // 読み込みサイズとバッファサイズが違う場合
				{
					DWORD brest = pti->streaminfo.writerest;
					DWORD rest = pti->streaminfo.writerest;
					rest += readsize;
					if(rest > bufsize) // 残りがバッファサイズよりも大きい場合
					{
						streamload(buf, splitbufsize * windex + brest, bufsize - brest, pti);
						rest = rest - bufsize;
						memcpy(pti->streaminfo.restbuf, &buf[bufsize - brest], rest);
						pti->streaminfo.writeindex = (windex == SPLITNUM - 1) ? 0 : windex + 1;
						pti->streaminfo.writecursor += bufsize - brest;
						pti->streaminfo.stock++;
					}
					else
						memcpy(&pti->streaminfo.restbuf[brest], buf, readsize);
					pti->streaminfo.writerest = rest;
				}
				else
				{
					int writesize = bufsize - pti->streaminfo.writerest;
					int restsize = readsize - writesize;
					streamload(buf, splitbufsize * windex + pti->streaminfo.writerest, writesize, pti);
					memcpy(pti->streaminfo.restbuf, &buf[writesize], restsize);
					pti->streaminfo.writeindex = (windex == SPLITNUM - 1) ? 0 : windex + 1;
					pti->streaminfo.writecursor += readsize;
					pti->streaminfo.stock++;
				}
			}
		}
		Sleep(1);
		// 状態チェック
		switch(pti->streaminfo.state & STATUS)
		{
		case STOP: // 停止
			pti->lpSB->Stop();
			break;
		case PLAY: // 読み込みを行う
			pti->lpSB->GetStatus(&rstatus);
			if(pti->streaminfo.state & PLAYSTART)
			{
				streamSeekStart(pti);
				pti->streaminfo.state -= PLAYSTART;
			}
			else if(!(rstatus & DSBSTATUS_PLAYING))
			{
				pti->lpSB->Play(0, 0, DSBPLAY_LOOPING);
			}
			else // 読み込みカーソルからreadindexを更新
			{
				// Volume調整
				streamVolume(pti);
				int nowrindex = pti->streaminfo.readcursor / splitbufsize;
				if(rindex != nowrindex)
				{
					pti->streaminfo.readindex = nowrindex;
					pti->streaminfo.stock -= (nowrindex > rindex) ? nowrindex - rindex : nowrindex + SPLITNUM - rindex;
				}
			}
			break;
		case PAUSE: // 一時停止
			pti->lpSB->Stop();
			break;
		case DESTROY: // データ消去
			pti->pdecoder->loadDataEnd();
			pti->pdecoder->loadDataClose();
			pti->pdecoder->destory();
			return 0;
		}
	}
	return 0;
}

///////////////////////////////////////////////////////////
///	@brief 曲・効果音再生
///	@param[in] loopflag ループするかどうか
///	@param[in] startflag 最初から再生するかどうか
///	@param[in] overlapflag ループ時に読み込んでいても再生回数だけ増やす
///////////////////////////////////////////////////////////
void SoundBufferItem::play(bool loopflag, bool startflag, bool overlapflag)
{
	HRESULT hr;
	if(!overlapflag && m_ti.playnum != 0) // もう既に読み込んでいる
		return;
	/* macに渡すために、変な場所を修正
	if(loopflag) // ループ時に再生回数を増やす
		m_ti.playnum++;
	else // 再生回数は必ず1
	*/
	m_ti.playnum = 1;
	if(m_ti.streaminfo.streamflag) // ストリーム再生する場合
	{
		m_ti.streaminfo.state = PLAY;
		m_ti.streaminfo.state += (loopflag) ? LOOP : 0;
		m_ti.streaminfo.state += (startflag) ? PLAYSTART : 0;
	}
	else // 普通に再生する場合
	{
		if(startflag)
		{
			m_ti.lpSB->Stop();
			hr = m_ti.lpSB->SetCurrentPosition(0);
		}
		if(loopflag)
			hr = m_ti.lpSB->Play(0, 0, DSBPLAY_LOOPING);
		else 
			hr = m_ti.lpSB->Play(0, 0, 0);
		if(hr == DSERR_PRIOLEVELNEEDED)
			hr = 0;
	}
}

///////////////////////////////////////////////////////////
///	@brief 曲・効果音停止
///////////////////////////////////////////////////////////
void SoundBufferItem::stop()
{
	if(--m_ti.playnum == 0) // 再生数が0の場合、停止する
	{
		if(m_ti.streaminfo.streamflag)
		{
			m_ti.streaminfo.state = STOP;
		}
		else
		{
			m_ti.lpSB->Stop();
			m_ti.lpSB->SetCurrentPosition(0);
		}
	}
	m_ti.playnum = (m_ti.playnum > 0) ? m_ti.playnum : 0;
}

///////////////////////////////////////////////////////////
///	@brief 曲・効果音一時停止
///////////////////////////////////////////////////////////
void SoundBufferItem::pause()
{
	if(m_ti.streaminfo.streamflag)
	{
		m_ti.streaminfo.state = PAUSE;
	}
	else
		m_ti.lpSB->Stop();
}

///////////////////////////////////////////////////////////
///	@brief 再生状態取得
///	@return 状態[StreamStateEnum参考]
///	@sa StreamStateEnum
///////////////////////////////////////////////////////////
int SoundBufferItem::getState()
{
	return m_ti.streaminfo.state;
}

///////////////////////////////////////////////////////////
///	@brief ボリューム変更
///	@param volume 0～-10000の間
///	@param time 変更に要する時間(0の場合は即座に変更)
///////////////////////////////////////////////////////////
void SoundBufferItem::changeVolume(LONG volume, DWORD time)
{
	volume = (volume > DSBVOLUME_MAX) ? DSBVOLUME_MAX : volume;
	volume = (volume < DSBVOLUME_MIN) ? DSBVOLUME_MIN : volume;
	if(m_ti.streaminfo.streamflag && time) // 時間をかけて変更する
	{
		m_ti.lpSB->GetVolume(&m_ti.SBInfo.volume.svalue);
		m_ti.SBInfo.volume.evalue = volume;
		m_ti.SBInfo.volume.time = time;
		m_ti.SBInfo.volume.st = timeGetTime();
		m_ti.SBInfo.volume.beforetime = timeGetTime();
	}
	else // 即座に変更する
		m_ti.lpSB->SetVolume(volume);
}