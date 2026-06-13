#include "DirectSoundStream.h"


LPDIRECTSOUNDBUFFER DirectSoundStream::m_pDSStreamBuffer=NULL;
std::vector<PlayMusicInfo>	DirectSoundStream::vecInfo;
HANDLE	DirectSoundStream::hNotifyEvent		= NULL;
HANDLE	DirectSoundStream::hNotifyThread	= NULL;
BYTE*	DirectSoundStream::m_pbWavData		= NULL;
DWORD	DirectSoundStream::m_dwCurWavRead	= 0;
DWORD	DirectSoundStream::m_dwNumNotify	= DIRECTSOUND_MUSIC_STREAMBUFFER_SECTIONNUM;
DWORD	DirectSoundStream::BufSectionSize	= DIRECTSOUND_MUSIC_STREAMBUFFER_SECTIONSIZE;
int		DirectSoundStream::SectionID		= 0;
boost::interprocess::interprocess_semaphore DirectSoundStream::DSStreamBuffer_semaphore(1);
//------------------------------------------------------------
// コンストラクタ
//------------------------------------------------------------
DirectSoundStream::DirectSoundStream()
{
}

//------------------------------------------------------------
// デストラクタ
//------------------------------------------------------------
DirectSoundStream::~DirectSoundStream()
{
	SAFE_DELETEARRAY(m_pbWavData)
	SAFE_RELEASE(m_pDSStreamBuffer);

	//HandleClose();
	// ハンドルはクローズしてしまうとイベントなど通知が無効になるので
	// プログラム終了時にクローズします
}

//========================================
// ハンドルクローズ
//========================================
void DirectSoundStream::HandleClose(void)
{
	CloseHandle(hNotifyEvent);
	CloseHandle(hNotifyThread);

	SAFE_DELETEARRAY(m_pbWavData)
	SAFE_RELEASE(m_pDSStreamBuffer);
}

//========================================
// 初期化
//========================================
void DirectSoundStream::Create(TCHAR* FileName, int fSeek)
{

	DSStreamBuffer_semaphore.wait();

	SAFE_RELEASE(m_pDSStreamBuffer);
	SAFE_DELETEARRAY(m_pbWavData);
	SectionID		= 0;
	m_dwCurWavRead	= 0;
	vecInfo.clear();

	PlayMusicInfo MusicInfo;
	if( !GetMusicDataInfo(&MusicInfo, FileName, fSeek) ) { DSStreamBuffer_semaphore.post(); return; }
	vecInfo.push_back(MusicInfo);

	// DirectSoundセカンダリーバッファー作成
	DSBUFFERDESC dsbd;
	ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
	dsbd.dwSize		= sizeof(DSBUFFERDESC);
	dsbd.dwFlags	= DSBCAPS_GETCURRENTPOSITION2//ストリーミングのときは特に正確な再生カーソルを得るためにこのフラグは必須
						| DSBCAPS_CTRLPOSITIONNOTIFY //通知能力
						| DSBCAPS_LOCSOFTWARE	// バッファをハードウェアのメモリ上に置くのではなく、システムメモリを使う
											// Notifyが正しく発行されない対策
						/*| DSBCAPS_CTRLPAN */| DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLFX | DSBCAPS_GLOBALFOCUS;

	dsbd.dwBufferBytes		= ( BufSectionSize*m_dwNumNotify );//
	dsbd.guid3DAlgorithm	= DS3DALG_DEFAULT;	// 予約済み
	dsbd.lpwfxFormat		= &MusicInfo.format;

	if( FAILED( m_pDSound->CreateSoundBuffer( &dsbd, &m_pDSStreamBuffer, NULL ) ) )
	{
		//#if MESSAGE_BOX_APPEAR
		//MessageBox(0,"セカンダリバッファー作成失敗","",MB_OK);
		//#endif
		DSStreamBuffer_semaphore.post();
		return;
	}

	//波形データを書き込む準備として、セカンダリーバッファーをロックする
	void* pBuffer = NULL;
	DWORD dwBufferSize = 0;

	if( FAILED(m_pDSStreamBuffer->Lock( 0, (BufSectionSize*m_dwNumNotify), &pBuffer, &dwBufferSize, NULL, NULL, 0) ) )
	{
		DSStreamBuffer_semaphore.post();
		return;
	}

	FILE* fp;
	_tfopen_s(&fp, FileName, _T("rb"));
	if(NULL==fp)
	{
		//#if MESSAGE_BOX_APPEAR
		//MessageBox(0,"ファイルがありません","",MB_OK);
		//#endif
		DSStreamBuffer_semaphore.post();
		return;
	}
	fseek(fp, MusicInfo.wavOffSet, SEEK_SET);

	// 最初なのでWAVデータを書き込む
	// 通知位置が先頭なので、念のため先頭から「2区画分」だけデータを埋めておく
	//※※※※※※※※※※※※※※※※※※※

	BYTE* pbWavData	=	new BYTE[ BufSectionSize*2 ];	//2区間分のメモリを確保、そのアドレスを構造体内のメンバに代入
	fread(pbWavData, 1, BufSectionSize*2, fp);
	//そして波形データをセカンダリバッファーにコピーする
	for( DWORD i = 0; i < BufSectionSize*2; i++ )  	*((BYTE*)pBuffer+i) = *((BYTE*) pbWavData+i);

	fclose(fp);
	SAFE_DELETEARRAY( pbWavData );

	m_pDSStreamBuffer->Unlock( pBuffer, BufSectionSize*2, NULL, 0 );

	m_dwCurWavRead += BufSectionSize;	// 2区画読み込んだけど、1区画分だけ進めておく
	SectionID++;						// 2区画読み込んだけど、1区画分だけ進めておく

	LPDIRECTSOUNDNOTIFY pDSNotify = NULL;
	if( FAILED( m_pDSStreamBuffer->QueryInterface( IID_IDirectSoundNotify, (VOID**)&pDSNotify ) ) )
	{
		DSStreamBuffer_semaphore.post();
		return;
	}

	DSBPOSITIONNOTIFY* pPosNotify = new DSBPOSITIONNOTIFY[ m_dwNumNotify ];

	for( DWORD i = 0; i < m_dwNumNotify; i++ )
    {
        pPosNotify[i].dwOffset     = (BufSectionSize * i) ;
        pPosNotify[i].hEventNotify = hNotifyEvent;
    }
    if( FAILED( pDSNotify->SetNotificationPositions( m_dwNumNotify, pPosNotify)))
    {
        SAFE_RELEASE( pDSNotify );
        SAFE_DELETEARRAY( pPosNotify );
        DSStreamBuffer_semaphore.post();
        return;
    }

	if(m_pDSStreamBuffer != NULL) m_pDSStreamBuffer->SetVolume(m_volume);
	m_pbWavData	= new BYTE[BufSectionSize];	// 作業領域
    SAFE_DELETEARRAY( pPosNotify );
	SAFE_RELEASE( pDSNotify );


	DSStreamBuffer_semaphore.post();
}

//====================================================
// サウンドを変える 突然変わるよ
//====================================================
void DirectSoundStream::ChangeBGM(TCHAR* FileName, int fSeek)
{

	Stop();
	Create(FileName, fSeek);
	Play();
}

//=======================================================================
//	BGMを追加
//=======================================================================
void DirectSoundStream::AttendBGM(TCHAR* FileName, int fSeek)
{
	PlayMusicInfo info;
	if( !GetMusicDataInfo(&info, FileName, fSeek) )	return;
	vecInfo.push_back(info);
}

//=============================================================
// ボリュームを設定 0～100%
//==============================================================
void DirectSoundStream::SetVolume_ByRate(int volParsentage)
{
	int rate = volParsentage;
	if(rate > 100) rate = 100;
	else if(rate < 0) rate = 0;

	if(rate == 0)	m_volume = -10000;
	else
		m_volume = (DIRECTSOUND_VOLUME_MINIMUM + DIRECTSOUND_VOLUME_MAX) + (-DIRECTSOUND_VOLUME_MINIMUM/100)*rate;

	if(m_pDSStreamBuffer != NULL) m_pDSStreamBuffer->SetVolume(m_volume);
}
//=============================================================
// リュームを直接細かく設定　0が最大音量
//==============================================================
void DirectSoundStream::SetVolume(long Volume)
{
	m_volume = Volume;
	if(m_volume > DIRECTSOUND_VOLUME_MAX)	m_volume = DIRECTSOUND_VOLUME_MAX;
	else if(Volume < -10000)				m_volume = -10000;

	if(m_pDSStreamBuffer != NULL) m_pDSStreamBuffer->SetVolume(m_volume);
}


//################################################################
// ストリームバッファにデータを書き込む
//################################################################
void DirectSoundStream::WriteStream(void)
{
	if(m_pDSStreamBuffer == NULL)
	{
		return;
	}

	{
		DWORD dwCurrentPlayCursor;
		DWORD dwCurrentWriteCursor;

		if(DS_OK == m_pDSStreamBuffer->GetCurrentPosition( &dwCurrentPlayCursor, &dwCurrentWriteCursor ))
		{
			DWORD nextCursolMin = (SectionID + m_dwNumNotify - 1)%m_dwNumNotify * BufSectionSize;
			/*
			_TCHAR TempString[256] = _T("");
			wsprintf(TempString, _T("nextCursol = ((SectionID+2)%m_dwNumNotify) * BufSectionSize: %x = ((%x+2)％ %x) * %x, dwCurrentPlayCursor:%x , dwCurrentWriteCursor:%x \n"), nextCursol, SectionID, m_dwNumNotify, BufSectionSize, dwCurrentPlayCursor, dwCurrentWriteCursor );
			OutputDebugString( TempString);
			*/
			if(dwCurrentPlayCursor < nextCursolMin)
			{
				return;
			}
		}
		else
		{
			return;
		}
	}

	void*	pv		= NULL;
	DWORD	dwSize	= 0;
	DWORD	StartOffSet	= SectionID * BufSectionSize;

	// 読み込みカーソルがWAVの最後に到達したので、2回に分けて書き込む
	if(m_dwCurWavRead + BufSectionSize > vecInfo[0].wavSize)
	{
		/*
		_TCHAR TempString[256] = _T("");
		wsprintf(TempString, _T("SECOND fs : pv, vecInfo[0].FileName, vecInfo[0].wavOffSet, m_dwCurWavRead, BufSectionSize, dwSize, SectionID = %08x, %s, %x, %x, %x, %x, %x \n"), (DWORD)pv, vecInfo[0].FileName, vecInfo[0].wavOffSet, m_dwCurWavRead, BufSectionSize, dwSize, SectionID );
		OutputDebugString( TempString);
		*/
		// 2箇所に分けて読み込む
		// ・「次のバッファ書き込み位置　～　Wavデータの最後まで」　分のデータ
		// ・「Wavデータの最初　～　次の通知が発生する間隔まで」　分のデータ
		DWORD NokoriWavSize = (vecInfo[0].wavSize - m_dwCurWavRead);

		if (DS_OK == m_pDSStreamBuffer->Lock(StartOffSet, NokoriWavSize, &pv ,&dwSize, NULL, NULL, 0) )
		{
			memcpy(pv, ReadWavFile(vecInfo[0].FileName, vecInfo[0].wavOffSet, m_dwCurWavRead, dwSize), dwSize);
			m_pDSStreamBuffer->Unlock(pv, dwSize, NULL, NULL);
		}

		// 2回目の読み込み
		StartOffSet		+= NokoriWavSize;
		m_dwCurWavRead	= 0;

		if(vecInfo.size() > 1)
		{
			// 一番先頭の要素を削除
			std::vector<PlayMusicInfo>::iterator it = vecInfo.begin();
			vecInfo.erase(it);
		}

		if (DS_OK == m_pDSStreamBuffer->Lock(StartOffSet, (BufSectionSize - NokoriWavSize), &pv ,&dwSize, NULL, NULL, 0) )
		{
			memcpy(pv, ReadWavFile(vecInfo[0].FileName, vecInfo[0].wavOffSet, m_dwCurWavRead, dwSize), dwSize);
			m_pDSStreamBuffer->Unlock(pv, dwSize, NULL, NULL);
		}

		SectionID++;
		m_dwCurWavRead += dwSize;	//読み込みカーソルを進める
		/*
		wsprintf(TempString, _T("SECOND es : pv, vecInfo[0].FileName, vecInfo[0].wavOffSet, m_dwCurWavRead, BufSectionSize, dwSize, SectionID = %08x, %s, %x, %x, %x, %x, %x \n"), (DWORD)pv, vecInfo[0].FileName, vecInfo[0].wavOffSet, m_dwCurWavRead, BufSectionSize, dwSize, SectionID );
		OutputDebugString( TempString);
		*/
	}
	else
	{
		/*
		_TCHAR TempString[256] = _T("");
		wsprintf(TempString, _T("FIRST  fs : pv, vecInfo[0].FileName, vecInfo[0].wavOffSet, m_dwCurWavRead, BufSectionSize, dwSize, SectionID = %08x, %s, %x, %x, %x, %x, %x \n"), (DWORD)pv, vecInfo[0].FileName, vecInfo[0].wavOffSet, m_dwCurWavRead, BufSectionSize, dwSize, SectionID );
		OutputDebugString( TempString);
		*/
		if (DS_OK == m_pDSStreamBuffer->Lock(StartOffSet, BufSectionSize, &pv, &dwSize, NULL, NULL, 0))
		{
			memcpy(pv, ReadWavFile(vecInfo[0].FileName, vecInfo[0].wavOffSet, m_dwCurWavRead, dwSize), dwSize);
			m_pDSStreamBuffer->Unlock(pv, dwSize, NULL, NULL);//バグStartOffset=0,dwSize=32768,pv=0x03a90050,m_pDSStreamBuffer->Iunknown=0x00000003
		}

		SectionID++;
		m_dwCurWavRead += BufSectionSize;	//読み込みカーソルを進める
		/*
		wsprintf(TempString, _T("FIRST  es : pv, vecInfo[0].FileName, vecInfo[0].wavOffSet, m_dwCurWavRead, BufSectionSize, dwSize, SectionID = %08x, %s, %x, %x, %x, %x, %x \n"), (DWORD)pv, vecInfo[0].FileName, vecInfo[0].wavOffSet, m_dwCurWavRead, BufSectionSize, dwSize, SectionID );
		OutputDebugString( TempString);
		*/
	}

	if( SectionID >= m_dwNumNotify)	SectionID = 0;
	return;
}

//################################################################
// wavデータを読み込む
//################################################################
BYTE* DirectSoundStream::ReadWavFile(TCHAR* FileName, DWORD wavDataOffSet, DWORD ReadCur, DWORD ReadSize)
{
	FILE* fp;
	_tfopen_s(&fp, FileName, _T("rb"));
	fseek(fp, (wavDataOffSet+ReadCur), SEEK_SET);
	fread(m_pbWavData, 1, ReadSize, fp);
	fclose(fp);

	return m_pbWavData;
}

//################################################################
// スレッド関数 一定時間ごとにシグナルになるのでそこでデータ変更
//################################################################
DWORD WINAPI DirectSoundStream::NotifyProc(LPVOID lpParameter)
{
    MSG msg;
    DWORD dwResult;
    BOOL boDone = false;
    while( !boDone )
    {
        dwResult = MsgWaitForMultipleObjects(1, &hNotifyEvent, FALSE, INFINITE, QS_ALLEVENTS );
        switch( dwResult )
        {
			//シグナル状態にある。（つまり、再生カーソルが通知位置を通過した）
            case WAIT_OBJECT_0:

				DSStreamBuffer_semaphore.wait();
				WriteStream();
				DSStreamBuffer_semaphore.post();
				break;
            case WAIT_OBJECT_0+1:
                while( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
                {
                    if( msg.message == WM_QUIT )
                        boDone = true;
                }
                break;
        }
    }
    return 0;
}

//---------------------------------------------------------------
// ストリーミングに必要なBGMの情報を得る
//---------------------------------------------------------------
bool DirectSoundStream::GetMusicDataInfo(PlayMusicInfo *pInfo, TCHAR* FileName, int fSeek)
{
	HMMIO hMmio=NULL;		//WindowsマルチメディアAPIのハンドル
	WAVEFORMATEX wfex;		//WAVのフォーマット 例）16ビット、44100Hz、ステレオなど
	MMCKINFO ckInfo;		//チャンク情報
	MMCKINFO riffckInfo;	//最上部チャンク（RIFFチャンク）保存用
	DWORD dwWavSize = 0;	//WAVファイル内　WAVデータのサイズ

	//WAVファイル内のヘッダー情報（音データ以外）の確認と読み込み
	hMmio = mmioOpen( FileName, NULL, MMIO_ALLOCBUF | MMIO_READ );
	if(NULL==hMmio)
	{
/*		#if MESSAGE_BOX_APPEAR
		MessageBox(0,"ファイルがありません","",MB_OK);
		#endif	*/
		return false;
	}
	mmioSeek(hMmio, fSeek, SEEK_SET);

	//ファイルポインタをRIFFチャンクの先頭にセットする
	if( ( MMSYSERR_NOERROR != mmioDescend( hMmio, &riffckInfo, NULL, 0 ) ) )
	{
/*		#if MESSAGE_BOX_APPEAR
		MessageBox(0,"最初のチャンク（最上部チャンク)に進入できません","",MB_OK);
		#endif	*/
		return false;
	}
	//チャンク情報によりWAVファイルかどうか確認する
	if( (riffckInfo.ckid != mmioFOURCC('R', 'I', 'F', 'F')) || (riffckInfo.fccType != mmioFOURCC('W', 'A', 'V', 'E') ) )
	{
/*		#if MESSAGE_BOX_APPEAR
		MessageBox(0,"これはWAVファイルではありません"," lol ",MB_OK);
		#endif	*/
		return false;
	}
	// ファイルポインタを'f' 'm' 't' ' ' チャンクにセットする
	ckInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if( MMSYSERR_NOERROR != mmioDescend( hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK ) )
	{
		//#if MESSAGE_BOX_APPEAR
		//MessageBox(0,"fmt チャンクが見つかりません","",MB_OK);
		//#endif
		return false;
	}
	//フォーマットを読み込む
	if( mmioRead( hMmio, (HPSTR)&wfex, sizeof(PCMWAVEFORMAT)) == -1 )
	{
		//#if MESSAGE_BOX_APPEAR
		//MessageBox(0,"WAVフォーマットの読み込み失敗","",MB_OK);
		//#endif
		return false;
	}
	if( MMSYSERR_NOERROR != mmioAscend( hMmio, &ckInfo, 0 ) )
    {
		//#if MESSAGE_BOX_APPEAR
		//MessageBox(0,"Wav読み込みエラー","",MB_OK);
		//#endif
		return false;
    }
	// WAVファイル内の音データの読み込み
	ckInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
    if( MMSYSERR_NOERROR != mmioDescend( hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK ) )
	{
		//#if MESSAGE_BOX_APPEAR
		//MessageBox(0,"dataチャンクが見つかりません","",MB_OK);
		//#endif
		return false;
	}

	pInfo->format		= wfex;
	pInfo->FileName		= FileName;
	pInfo->FileSeek		= fSeek;
	pInfo->wavOffSet	= ckInfo.dwDataOffset;
	pInfo->wavSize		= ckInfo.cksize;

	// これが無いせいでリークしてた('A`)
	mmioClose( hMmio, MMIO_FHOPEN );
	return true;
}

