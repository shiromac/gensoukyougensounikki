#include "stdafx.h"
#include "SoundDecodeItemWav.h"

SoundDecodeItemWav::SoundDecodeItemWav()
{
	m_hmmioIn = NULL;
}

SoundDecodeItemWav::~SoundDecodeItemWav()
{
	WaveClose();
}

bool SoundDecodeItemWav::loadHeader()
{
	HRESULT hr;
	if(FAILED(hr = WaveOpen()))
	{
		MyDebug::outputDebugString(TEXT("[SoundDecodeItemWav::loadHeader] %s WAVEOPEN FAILED \n"), m_loadinfo.filename.c_str());
		return false;
	}
	if(m_decodeinfo.wfx.wFormatTag != 1)
	{
		OutputDebugString(TEXT("[SoundDecodeItemWav::loadHeader] WAVフォーマットが違う \n"));
		return false;
	}
	return true;
}

void SoundDecodeItemWav::loadDataInit()
{
	// バッファを作成
	if(m_loadinfo.loadbufsize)
	{
		m_decodeinfo.buf = new char[m_loadinfo.loadbufsize];
		m_decodeinfo.bufsize = m_loadinfo.loadbufsize;
	}
	else
	{
		m_decodeinfo.buf = new char[m_chankIn.cksize];
		m_decodeinfo.bufsize = m_chankIn.cksize;
	}
	memset(m_decodeinfo.buf, 0, m_decodeinfo.bufsize);
}

void SoundDecodeItemWav::loadDataStart()
{
	WaveReadStart();
}

void SoundDecodeItemWav::loadDataEnd()
{
	WaveReadEnd();
}

void SoundDecodeItemWav::loadDataClose()
{
	WaveClose();
}

void SoundDecodeItemWav::loadDataReStart()
{
	if(mmioAscend(m_hmmioIn, &m_chankIn, 0))
		return;
	LONG byte2 = mmioSeek(m_hmmioIn, m_chankInRIFF.dwDataOffset + sizeof(FOURCC), SEEK_SET);
	m_chankIn.ckid = mmioFOURCC('d','a','t','a');
	if(mmioDescend(m_hmmioIn, &m_chankIn, &m_chankInRIFF, MMIO_FINDCHUNK))
		return;
	WaveReadStart();
}

int SoundDecodeItemWav::loadData()
{
	char* buf = m_decodeinfo.buf;
	DWORD bufsize = m_decodeinfo.bufsize;
	DWORD readsize = 0;

	if(FAILED(WaveRead(bufsize, buf, &readsize)))
		return 0;
	return readsize;
}

HRESULT SoundDecodeItemWav::WaveOpen()
{
	WAVEFORMATEX* pwfx;
	DWORD wfxsize;

	// ファイルのオープン
	if(m_loadinfo.fileflag)
	{
		if(!(m_hmmioIn = mmioOpen((TCHAR*)m_loadinfo.filename.c_str(), NULL, MMIO_ALLOCBUF | MMIO_READ)))
		{
			WaveClose();
			return E_FAIL;
		}
	}
	else if(m_loadinfo.filehandle)
	{
		MMIOINFO mmioinfo = {0};
		mmioinfo.pchBuffer = NULL;
		mmioinfo.fccIOProc = FOURCC_DOS;
		mmioinfo.adwInfo[0] = (DWORD)m_loadinfo.filehandle;
		if(!(m_hmmioIn = mmioOpen(NULL, &mmioinfo, MMIO_ALLOCBUF | MMIO_READ)))
		{
			WaveClose();
			return E_FAIL;
		}
	}
	// オープンしたファイルが目的の形式のものか調べる
	if(mmioDescend(m_hmmioIn, &m_chankInRIFF, NULL, 0))
	{
		WaveClose();
		return E_FAIL;
	}
	// ファイルの形式の確認
	if( (m_chankInRIFF.ckid != FOURCC_RIFF) || (m_chankInRIFF.fccType != mmioFOURCC('W','A','V','E')) )
	{
		WaveClose();
		return E_FAIL;
	}
	// 次はデータのフォーマットを取得
	m_chankIn.ckid = mmioFOURCC('f','m','t',' ');
	if(mmioDescend(m_hmmioIn, &m_chankIn, &m_chankInRIFF, MMIO_FINDCHUNK))
	{
		WaveClose();
		return E_FAIL;
	}
	// チャンクのサイズがPCMWAVEFORMAT構造体と一緒か確認
	if(m_chankIn.cksize < (LONG)sizeof(PCMWAVEFORMAT))
	{
		WaveClose();
		return E_FAIL;
	}
	pwfx = (WAVEFORMATEX*)malloc(m_chankIn.cksize);
	wfxsize = (m_chankIn.cksize > sizeof(WAVEFORMATEX)) ? sizeof(WAVEFORMATEX) : m_chankIn.cksize;
	if(mmioRead(m_hmmioIn, (HPSTR)pwfx, m_chankIn.cksize) != m_chankIn.cksize)
	{
		WaveClose();
		free(pwfx);
		return E_FAIL;
	}
	// チャンクから抜けます
	if(mmioAscend(m_hmmioIn, &m_chankIn, 0))
	{
		WaveClose();
		free(pwfx);
		return E_FAIL;
	}
	// データチャンクの読み込み
	if(-1 == mmioSeek(m_hmmioIn, m_chankInRIFF.dwDataOffset + sizeof(FOURCC), SEEK_SET))
	{
		WaveClose();
		free(pwfx);
		return E_FAIL;
	}
	m_chankIn.ckid = mmioFOURCC('d','a','t','a');
	if(mmioDescend(m_hmmioIn, &m_chankIn, &m_chankInRIFF, MMIO_FINDCHUNK))
	{
		WaveClose();
		free(pwfx);
		return E_FAIL;
	}
	// wfxをコピー
	memcpy(&m_decodeinfo.wfx, pwfx, wfxsize);
	free(pwfx);
	return S_OK;
}

HRESULT SoundDecodeItemWav::WaveReadStart()
{
	// バッファに直接アクセスする許可をもらう
	if (mmioGetInfo(m_hmmioIn, &m_mmiodata, 0))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT SoundDecodeItemWav::WaveReadEnd()
{
	// バッファに直接アクセスし終わったことを伝える
	if (mmioSetInfo(m_hmmioIn, &m_mmiodata, 0))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT SoundDecodeItemWav::WaveRead(DWORD size, char* buf, DWORD* out_readsize)
{
	DWORD cbDataIn = size;

	*out_readsize = 0;	
	if (cbDataIn > m_chankIn.cksize) 
		cbDataIn = m_chankIn.cksize;
	m_chankIn.cksize -= cbDataIn;
	// データを書き出す
	for (DWORD cT = 0; cT < cbDataIn; cT++)
	{
		if (m_mmiodata.pchNext == m_mmiodata.pchEndRead)
		{
			// バッファを更新
			if (mmioAdvance(m_hmmioIn, &m_mmiodata, MMIO_READ))
			{
				return E_FAIL;
			}
			// 最後なので終了
			if (m_mmiodata.pchNext == m_mmiodata.pchEndRead)
			{
				*out_readsize = cT;
				return S_OK;
			}
		}
		*( (BYTE *)buf + cT ) = *( (BYTE *)m_mmiodata.pchNext );
		m_mmiodata.pchNext++;
	}
	*out_readsize = cbDataIn;
	return S_OK;
}

void SoundDecodeItemWav::WaveClose()
{
	if(m_hmmioIn)
	{
		if(m_loadinfo.filehandle)
			mmioClose(m_hmmioIn, MMIO_FHOPEN);
		else
			mmioClose(m_hmmioIn, 0);
		m_hmmioIn = NULL;
	}
}

bool SoundDecodeItemWav::checkload(char c1, char c2, char c3, char c4)
{
	if(c1 == 'w' && c2 == 'a' && c3 == 'v')
		return true;
	return false;
}

SoundDecodeItem* SoundDecodeItemWav::copy()
{
	SoundDecodeItem* psi = new SoundDecodeItemWav();
	if(m_loadinfo.fileflag)
		psi->setLoadInfo_Filename(m_loadinfo.filename, m_loadinfo.loadbufsize);
	else if(m_loadinfo.filehandle)
		psi->setLoadInfo_Handle(m_loadinfo.filehandle, m_loadinfo.loadbufsize);
	else if(m_loadinfo.filep)
		psi->setLoadInfo_Pointer((void*)m_loadinfo.filep, m_loadinfo.loadbufsize);
	return psi;
}

void SoundDecodeItemWav::destory()
{
	SoundDecodeItem::destory();
	WaveClose();
	m_loadinfo.clear();
}