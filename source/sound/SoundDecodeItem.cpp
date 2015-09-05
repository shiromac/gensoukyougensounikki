/******************************************************//*!
	@file SoundDecodeItem.cpp
	@brief 曲・効果音デコード用基底クラスが書かれたソースファイル
**********************************************************/

#include "stdafx.h"
#include "SoundDecodeItem.h"

///////////////////////////////////////////////////////////
///	@brief コンストラクタ
///////////////////////////////////////////////////////////
SoundDecodeItem::SoundDecodeItem()
{
	m_loadinfo.clear();
	memset(&m_decodeinfo, 0, sizeof(DecodeInfo));
}

///////////////////////////////////////////////////////////
///	@brief デストラクタ、リソース開放も行う
///	@sa destory
///////////////////////////////////////////////////////////
SoundDecodeItem::~SoundDecodeItem()
{
	destory();
}

///////////////////////////////////////////////////////////
///	@brief ファイル名から読み込むことをセット
///	@param[in] filename ファイル名
///	@param[in] loadbufsize ストリーム用バッファサイズ(0の場合、全読み込み
///////////////////////////////////////////////////////////
void SoundDecodeItem::setLoadInfo_Filename(tstring filename, DWORD loadbufsize)
{
	m_loadinfo.clear();
	m_loadinfo.fileflag = true;
	m_loadinfo.loadbufsize = loadbufsize;
	m_loadinfo.filename = filename;
}

///////////////////////////////////////////////////////////
///	@brief ファイルハンドルから読み込むことをセット
///	@param[in] handle ファイルハンドル
///	@param[in] loadbufsize ストリーム用バッファサイズ(0の場合、全読み込み
///////////////////////////////////////////////////////////
void SoundDecodeItem::setLoadInfo_Handle(HANDLE handle, DWORD loadbufsize)
{
	m_loadinfo.clear();
	m_loadinfo.filehandle = handle;
	m_loadinfo.loadbufsize = loadbufsize;
}

///////////////////////////////////////////////////////////
///	@brief ファイルポインタから読み込むことをセット
///	@param[in] p ファイルポインタ
///	@param[in] loadbufsize ストリーム用バッファサイズ(0の場合、全読み込み
///////////////////////////////////////////////////////////
void SoundDecodeItem::setLoadInfo_Pointer(void* p, DWORD loadbufsize)
{
	m_loadinfo.clear();
	m_loadinfo.filep = (char*)p;
	m_loadinfo.loadbufsize = loadbufsize;
}

///////////////////////////////////////////////////////////
///	@brief フォーマット情報を取得
///	@param[out] out_wfx フォーマット構造体のダブルポインタ
///////////////////////////////////////////////////////////
void SoundDecodeItem::getHeader(WAVEFORMATEX** out_wfx)
{
	*out_wfx = &m_decodeinfo.wfx;
}

///////////////////////////////////////////////////////////
///	@brief フォーマット情報をセット
///	@param[out] in_wfx フォーマット構造体
///////////////////////////////////////////////////////////
void SoundDecodeItem::setHeader(WAVEFORMATEX &in_wfx)
{
	m_decodeinfo.wfx = in_wfx;
}

///////////////////////////////////////////////////////////
///	@brief バッファポインタとバッファサイズを取得
///	@param[out] buffer バッファポインタの保存先ポインタ
///	@param[out] buffersize バッファサイズ
///////////////////////////////////////////////////////////
void SoundDecodeItem::getData(char **buffer, DWORD& buffersize)
{
	*buffer = m_decodeinfo.buf;
	buffersize = m_decodeinfo.bufsize;
}

///////////////////////////////////////////////////////////
///	@brief リソース開放
///////////////////////////////////////////////////////////
void SoundDecodeItem::destory()
{
	if(m_decodeinfo.buf)
	{
		delete m_decodeinfo.buf;
		m_decodeinfo.buf = NULL;
	}
}
