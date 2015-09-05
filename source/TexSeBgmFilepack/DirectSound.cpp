#include "DirectSound.h"


bool				DirectSound::m_Init_Flag	= false;
LPDIRECTSOUND8		DirectSound::m_pDSound		= NULL;
int					DirectSound::m_class_count	= 0;
HWND				DirectSound::m_hWnd			= NULL;

//------------------------------------------------------------
// コンストラクタ・デストラクタ
//------------------------------------------------------------
DirectSound::DirectSound() : m_pDSBuffer(NULL)
{
	m_volume	= 0;
	m_class_count++;
}
DirectSound::~DirectSound()
{
	Free();
}

//=============================================================
// 開放
//=============================================================
void DirectSound::Free()
{
	SAFE_RELEASE(m_pDSBuffer);
	
	if(m_class_count == 1)
	{
		SAFE_RELEASE(m_pDSound);
		CoUninitialize();
		m_class_count--;
	}
	else
		m_class_count--;
	
}


//==============================================================
// HRESULT InitSound(HWND hWnd)
// DirectSoundを使えるようにする。沢山オブジェクト作っても、一度だけ呼べばOK
//==============================================================
HRESULT DirectSound::InitSound(HWND hWnd)
{	
	if(m_Init_Flag)		return S_FALSE;

	m_hWnd = hWnd;
	CoInitialize(NULL);
	
	//まず一番最初にすることはグローバル関数DirectSoundCreate8によりDirectSoundオブジェクトを生成すること   
	if( FAILED(DirectSoundCreate8(NULL,&m_pDSound,NULL)) )
   	{
		//#if LOGPUTOUT
		//	LogPutout("エラー！　サウンドカードが無い？？");
		//#endif
		//#if MESSAGE_BOX_APPEAR
		//	MessageBox(0,"サウンドカードが無い？？","エラー",MB_OK);
		//#endif

	    return S_FALSE;
    }
   // 協調レベルの設定
   if(FAILED(m_pDSound->SetCooperativeLevel(hWnd,DSSCL_EXCLUSIVE )))
   {
		//#if LOGPUTOUT
		//	LogPutout("エラー！　協調レベル設定失敗");
		//#endif
		//#if MESSAGE_BOX_APPEAR
		//	MessageBox(0,"協調レベル設定失敗","",MB_OK);
		//#endif

	   return S_FALSE;
   }
   m_Init_Flag = true;
  // #if LOGPUTOUT
		//LogPutout("DirectSound初期化成功");
  // #endif
   return S_OK;
}

//==============================================================
// wavファイルの中身をDirectSoundバッファーにコピーする
//==============================================================
void DirectSound::Create(TCHAR* szFileName, int fSeek)
{

	HMMIO hMmio=NULL;		//WindowsマルチメディアAPIのハンドル（Win32API）
	WAVEFORMATEX wfex;		//読み込むWAVファイルのフォーマット
	MMCKINFO ckInfo;		//フォーマットチャンク及びデータ（波形）チャンク情報用
	MMCKINFO riffckInfo;	//最上部（RIFF)チャンク情報用
	DWORD dwWavSize = 0;	//WAVファイル内　WAVデータのサイズ（WAVファイルはWAVデータで占められているので、ほぼファイルサイズと同一）

   //WAVファイル内のヘッダー情報（音データ以外）の確認と読み込み
   hMmio = mmioOpen( szFileName, NULL, MMIO_ALLOCBUF | MMIO_READ );

   //ファイルのSeekまで進める
   mmioSeek(hMmio, fSeek, SEEK_SET);

   //ファイルポインタをRIFFチャンクの先頭にセットする
   if( ( MMSYSERR_NOERROR != mmioDescend( hMmio, &riffckInfo, NULL, 0 ) ) )
   {
	  // #if MESSAGE_BOX_APPEAR
			//MessageBox(0,"最上部チャンクに進入できません\nファイル名が間違っている？","",MB_OK);
	  // #endif
	   return;
   }
   //チャンク情報によりWAVファイルかどうか確認する
   if( (riffckInfo.ckid != mmioFOURCC('R', 'I', 'F', 'F')) || (riffckInfo.fccType != mmioFOURCC('W', 'A', 'V', 'E') ) )
   {
	  // #if MESSAGE_BOX_APPEAR
			//MessageBox(0,"これはWAVファイルではありません"," lol ",MB_OK);
	  // #endif
	   return;
   }
   // ファイルポインタを'f' 'm' 't' ' ' チャンクにセットする
   ckInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
   if( MMSYSERR_NOERROR != mmioDescend( hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK ) )
   {
	  // #if MESSAGE_BOX_APPEAR
			//MessageBox(0,"fmtチャンクが見つかりません","",MB_OK);
	  // #endif
	   return;
   }
   //フォーマットを読み込む
   if( mmioRead( hMmio, (HPSTR) &wfex, 
                  sizeof(WAVEFORMATEX)) != sizeof(WAVEFORMATEX) )
   {
	  // #if MESSAGE_BOX_APPEAR
			//MessageBox(0,"WAVフォーマットの読み込み失敗","",MB_OK);
	  // #endif
   }
   //標準リニアPCMの確認。圧縮PCMやマルチチャンネル等は想定外
    if( wfex.wFormatTag != WAVE_FORMAT_PCM )
    {
		//#if MESSAGE_BOX_APPEAR
		//	MessageBox(0,"標準のリニアPCMフォーマットではありません","",MB_OK);
		//#endif
		return;
	}
	if( MMSYSERR_NOERROR != mmioAscend( hMmio, &ckInfo, 0 ) )
    {
        return;
    }
	// WAVファイル内の音データ(ヘッダー以外のデータ、波形データ)の読み込み
	ckInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
    if( MMSYSERR_NOERROR != mmioDescend( hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK ) )
	{
		//#if MESSAGE_BOX_APPEAR
		//	MessageBox(0,"dataチャンクが見つかりません","",MB_OK);
		//#endif
		return;
	}
	dwWavSize = ckInfo.cksize;
   // そのWAVフォーマットに沿ったDirectSoundセカンダリーバッファー作成
	DSBUFFERDESC dsbd;  
	ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLFX | DSBCAPS_GLOBALFOCUS;
	dsbd.dwBufferBytes = dwWavSize;
	dsbd.guid3DAlgorithm = DS3DALG_DEFAULT;
	dsbd.lpwfxFormat = &wfex;

   LPDIRECTSOUNDBUFFER ptmpBuffer = NULL;
   if( FAILED( m_pDSound->CreateSoundBuffer( &dsbd, &ptmpBuffer, NULL ) ) )
   {
	  // #if MESSAGE_BOX_APPEAR
			//MessageBox(0,"セカンダリバッファー作成失敗","",MB_OK);
	  // #endif
       return;
   }
   if(FAILED(ptmpBuffer->QueryInterface(IID_IDirectSoundBuffer8,(VOID**)&m_pDSBuffer)))
   {
		//#if MESSAGE_BOX_APPEAR
		//	MessageBox(0,"IDirectSoundBuffer8を取得できません","",MB_OK);
		//#endif
       return;
   }
   SAFE_RELEASE(ptmpBuffer);

   //波形データを書き込む準備として、セカンダリーバッファーをロックする
   VOID* pBuffer =NULL;
   DWORD dwBufferSize = 0; 
   if( FAILED(m_pDSBuffer->Lock( 0, dwWavSize,&pBuffer, &dwBufferSize,NULL, NULL, 0) ) )
   {
	   return;
   }
   //ロックしたバッファーサイズが実際の波形データより大きい場合。クラッシュ防止。
   DWORD dwSize = dwBufferSize;
   if( dwSize > ckInfo.cksize ) 
   {
	   dwSize = ckInfo.cksize;
   }
   //バッファーに音データを書き込む   
   FILE* fp;
   _tfopen_s(&fp, szFileName, _T("rb"));
   fseek(fp,ckInfo.dwDataOffset,SEEK_SET);
   BYTE* pWavData=new BYTE[ dwSize ];
   fread(pWavData, 1, dwSize, fp);//まずは、一時的な場所(pWavData)に波形データを読み込み・・・
   for( DWORD i = 0; i < dwSize; i++ )
   {
	   *((BYTE*)pBuffer+i) = *((BYTE*) pWavData+i); //・・・そして、一時的な場所の波形データをセカンダリバッファーにコピーする
   }
   fclose(fp);
   delete [] pWavData;//一時的な波形データを開放する
   m_pDSBuffer->Unlock( pBuffer, dwBufferSize, NULL, 0 );

   // これが無いせいでリークしてた('A`)
   mmioClose( hMmio, MMIO_FHOPEN );

   m_pDSBuffer->SetVolume(m_volume);
   return;
}


//=============================================================
// ボリュームを設定 0～100%
//==============================================================
void DirectSound::SetVolume_ByRate(int volParsentage)
{
	int rate = volParsentage;
	if(rate > 100) rate = 100;
	else if(rate < 0) rate = 0;

	if(rate == 0)	m_volume = -10000;
	else
		m_volume = (DIRECTSOUND_VOLUME_MINIMUM + DIRECTSOUND_VOLUME_MAX) + (-DIRECTSOUND_VOLUME_MINIMUM/100)*rate;

	if(m_pDSBuffer != NULL) m_pDSBuffer->SetVolume(m_volume);
}

//=============================================================
// リュームを直接細かく設定　0が最大音量
//==============================================================
void DirectSound::SetVolume(long Volume)
{
	m_volume = Volume;
	if(m_volume > DIRECTSOUND_VOLUME_MAX)	m_volume = DIRECTSOUND_VOLUME_MAX;
	else if(Volume < -10000)				m_volume = -10000;

	if(m_pDSBuffer != NULL) m_pDSBuffer->SetVolume(m_volume);
}

