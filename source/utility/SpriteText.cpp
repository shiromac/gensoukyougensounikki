//======================================================================
//このソースコードはTWINTAIL LOVERS(http://www.twin-tail.jp/)さんが作成したものを
//すき焼き(http://www.c3.club.kyutech.ac.jp/~sukiyaki/)が改造したものです。
//勝手にソースコードを改変することを許可します。
//======================================================================



#include "SpriteText.h"
#include <stdio.h>
#include <assert.h>

#include "debug.h"

//======================================================================
//
//		コンストラクタ
//
//======================================================================
SpriteText::SpriteText()
{
	m_hFont = m_hOldFont = NULL;
	m_lpTex = NULL;
	m_fRot = 0.0f;
	m_fSize = D3DXVECTOR2(1.0f,1.0f);
	m_AntiAlias = 1;
	m_Weight = FW_REGULAR;

	m_dwBackX = 0;
	m_fMargin = 0;

	m_Over = 1;
	
	UpDate();
}

//======================================================================
//
//		デストラクタ
//
//======================================================================
SpriteText::~SpriteText()
{
	UnInit();
}

//======================================================================
//
//		初期化
//
//======================================================================
bool SpriteText::Init( LPDIRECT3DDEVICE9 lpDev,long num, DWORD w, DWORD h)
{
	m_lpTex = NULL;
	
	HRESULT hr;
	long i;


	for ( i = 0; i <= 64; i++ )
	{
		m_byAlphaTbl[i] = (BYTE)( i*255/64 );
	}



	hr = D3DXCreateTexture( lpDev, w, h, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_lpTex );
	if(hr != S_OK)
	{
		return 1;
	}

	D3DSURFACE_DESC desc;
	m_lpTex->GetLevelDesc( 0, &desc );
	
	m_dwWidth = desc.Width;
	m_dwHeight = desc.Height;

	//*************************
	//追加
	m_ReturnWidth = m_dwWidth;


	return true;
}

//======================================================================
//
//		開放
//
//======================================================================
void SpriteText::UnInit( void )
{
	if(m_lpTex != NULL)
	{
		m_lpTex->Release();
		m_lpTex = NULL;
	}
}




//======================================================================
//
//		作成したテクスチャを取得します。
//
//======================================================================
LPDIRECT3DTEXTURE9 SpriteText::GetTex()
{
	return m_lpTex;
}




//======================================================================
//
//		テクスチャクリア
//
//======================================================================
void SpriteText::CleatText( void )
{

	D3DLOCKED_RECT TexRect;
	m_fRot = 0.0f;
	m_fSize = D3DXVECTOR2(1.0f,1.0f);

	if(m_lpTex != NULL)
	{
		HRESULT hr = m_lpTex->LockRect( 0, &TexRect, NULL, 0 );
		if SUCCEEDED( hr )
		{
			memset( TexRect.pBits, 0x00, TexRect.Pitch*m_dwHeight );
			m_lpTex->UnlockRect( 0 );
		}
	}
}

//======================================================================
//
//		テキスト使用開始
//
//======================================================================
void SpriteText::BeginText(const TCHAR* face, DWORD size )
{
	m_hwnd = GetActiveWindow();
	m_hdc = GetDC( m_hwnd );
	m_dwTexSize = size;

	//============================================
	//		フォント生成
	//============================================
	m_hFont = CreateFont(
				size,						// フォント高さ
				0,							// 文字幅
				0,							// テキストの角度	
				0,							// ベースラインとｘ軸との角度
				m_Weight,					// フォントの重さ（太さ）
				FALSE,						// イタリック体
				FALSE,						// アンダーライン
				FALSE,						// 打ち消し線
				SHIFTJIS_CHARSET,			// 文字セット
				OUT_TT_PRECIS,				// 出力精度
				CLIP_DEFAULT_PRECIS,		// クリッピング精度
				PROOF_QUALITY,				// 出力品質
				FIXED_PITCH | FF_MODERN,	// ピッチとファミリー
				face						// 書体名
			);

	m_hOldFont = (HFONT)SelectObject( m_hdc, m_hFont );


}

//======================================================================
//
//		テキスト使用終了
//
//======================================================================
void SpriteText::EndText( void )
{
	DeleteObject( SelectObject( m_hdc, m_hOldFont ) );
	ReleaseDC( m_hwnd, m_hdc );
}


//======================================================================
//
//		テキスト色
//
//======================================================================
void SpriteText::SetColor( D3DCOLOR color )
{
	m_a = (BYTE)((color & 0xFF000000) >> 24);
	m_rgb = color & 0x00FFFFFF;
}

//=======================================================================
//
//		最後に描画した文字の X 座標の位置を取得します
//
//=======================================================================
DWORD SpriteText::GetX()
{
	return m_dwX;
}

//=======================================================================
//
//		最後に描画した文字の Y 座標の位置を取得します
//
//=======================================================================
DWORD SpriteText::GetY()
{
	return m_dwY;
}


//======================================================================
//
//		テキスト描画・・・一番最後に描画した位置の次の位置から描画
//
//======================================================================
void SpriteText::DrawText(const TCHAR* tstring)
{
	TCHAR Buffer[MAXBUFFSIZE];
	//_vstprintf( Buffer, tstring, (TCHAR* )(&tstring + 1) );
	_tcscpy( Buffer, tstring);
	SetDrawText(Buffer);
}

//**********************************************************************
//
//		テキスト描画・・・一番最後に描画した位置の次の位置から描画
//
//**********************************************************************
void SpriteText::DrawText(tstring str)
{
	DrawText(str.c_str());
}
//**********************************************************************
//
//		テキスト描画・・・一番最後に描画した位置の次の位置から描画
//
//**********************************************************************
void SpriteText::DrawText(StyleString str)
{
	int i;
	vector<expandedString> estr = str.exportString();
	for(i=0;i<estr.size();i++)
	{
		SetColor(estr[i].color);
		SetSize(estr[i].size);
		if(estr[i].flag & STYLES_POSX_FLAG)
		{
			m_dwX = (estr[i].flag & STYLES_POSX_MASK) >> STYLES_POSX_FLOOR;
		}
		if(estr[i].flag & STYLES_POSY_FLAG)
		{
			m_dwY = (estr[i].flag & STYLES_POSY_MASK) >> STYLES_POSY_FLOOR;
		}


		DrawText((tstring)estr[i]);
	}

	SetColor(0xFFFFFFFF);
	D3DXVECTOR2 v(1.0f,1.0f);
	SetSize(v);
}

//======================================================================
//
//		テキスト描画・・・座標指定することにより指定位置に書き込むことができます。
//
//======================================================================
void SpriteText::DrawText( DWORD x, DWORD y, TCHAR* tstring)
{
	TCHAR Buffer[MAXBUFFSIZE];
	m_dwX = x;
	m_dwY = y;
	//_vstprintf( Buffer, tstring, (TCHAR* )(&tstring + 1) );
	_tcscpy( Buffer, tstring);
	SetDrawText(Buffer);
}

//======================================================================
//
//		1文字のみ描画します。
//
//======================================================================
void SpriteText::DrawChar(unsigned int code,D3DLOCKED_RECT &TexRect,TEXTMETRIC &tm)
{
	GLYPHMETRICS gm;
	ABC abc;

	//==================================================
	// フォントバッファ格納
	//==================================================
	LPBYTE lpFont = NULL;

	// フォントバッファ取得
	DWORD DataSize = GetFontBuffer( code, &gm, &lpFont );
	if(lpFont == NULL) return;

	// フォント幅取得
	GetCharABCWidths( m_hdc, code, code, &abc );

	LPBYTE p2 = lpFont;


	// 描画位置を進める
	/*
	int a = (abc.abcA)?(2):(0);	//文字の左淵の大きさ
	int b = abc.abcB;			//文字の大きさ
	int c = (abc.abcC)?(2):(0);	//文字の右淵の大きさ
	*/
	int a;	//文字の左淵の大きさ
	int b;	//文字の大きさ
	int c;	//文字の右淵の大きさ

	a = abc.abcA;	//文字の左淵の大きさ
	b = abc.abcB;	//文字の大きさ
	c = abc.abcC;	//文字の右淵の大きさ

	//**************************************************
	//追加点　強制改行
	//**************************************************
	if(m_dwX + m_fSize.x * (c + a + gm.gmBlackBoxX) > m_ReturnWidth) NL(m_dwBackX);

	m_dwX += m_fSize.x * (a);




	// サイズ取得
	lWidth = (long)gm.gmBlackBoxX;
	lHeight = (long)gm.gmBlackBoxY;
	


	if(m_dwX + lWidth > m_dwWidth)
	{
		lWidth = m_dwWidth - m_dwX;
	}
					
	if(m_dwY + lHeight > m_dwHeight)
	{
		lHeight = m_dwHeight - m_dwY;
	}


	



	//==================================================
	// 描画先サーフェイスのポインタ
	//==================================================
	LPDWORD p1 = (LPDWORD)TexRect.pBits;
	DWORD pitch = TexRect.Pitch / 4;





	// 転送先のサーフェイスの始点


	//==================================================
	//上書き判定
	//==================================================


	/*
	if(m_Over)
	{
		LPDWORD pt = (LPDWORD)TexRect.pBits;
		pt += m_dwX + (m_dwY) * pitch;
		int width = (int)(m_fSize.x * (2 + 2) + gm.gmBlackBoxX);
		int height = tm.tmHeight + tm.tmExternalLeading * 2;
		for ( long y = 0; y <  height; y++ )
		{
			memset(pt, 0x00, width * sizeof(DWORD));
			pt += pitch;
		}
		
	}
	*/
	if(m_Over)
	{
		LPDWORD pt = (LPDWORD)TexRect.pBits;
		pt += m_dwX + (m_dwY) * pitch;
		int width = (int)(m_fSize.x * (a + c) + gm.gmBlackBoxX);
		int height = tm.tmHeight + tm.tmExternalLeading * 2;
		if(m_dwHeight > m_dwY)
		{
			for ( long y = 0; y <  height; y++ )
			{
				memset(pt, 0x00, width * sizeof(DWORD));
				pt += pitch;
			}
		}
	}
	//==================================================
	// Ｙ開始位置取得
	//==================================================
	int sy = tm.tmAscent  - gm.gmptGlyphOrigin.y;

	int add = m_dwX + (m_dwY + sy) * pitch;

	if(add < 0)
	{
		add = 0;
	}
	p1 = (LPDWORD)TexRect.pBits;
	p1 += add;




	//==================================================
	// 転送
	//==================================================
	// フォントピッチ
	DWORD fontPitch;
	if(m_AntiAlias)
	{
		if(gm.gmBlackBoxY == 0) fontPitch = INT_MAX & ~0x03;
		else fontPitch = (DataSize / gm.gmBlackBoxY) & ~0x03;
		
		for ( long y = 0; y < lHeight ; y++ )
		{

			for ( long x = 0; x < lWidth; x++ )
			{
				DWORD a1 = (DWORD)((m_byAlphaTbl[p2[x]]) * m_a / 255) << 24;
				DWORD dwColor = m_rgb | a1;
				if((dwColor & 0xff000000) != 0x00000000)
				{
					p1[x] = dwColor; 
				}
			}
			p1 += pitch;
			p2 += fontPitch;
		}
	}
	else
	{
		LPBYTE lpCurrent; // 現在チェック中のBYTE位置
		fontPitch = ( ( gm.gmBlackBoxX + 0x1f ) & ~0x1f ) / 8; // 1行分のバイト数
		BYTE byMask; // ビットマスク

		for (int y = 0; y < lHeight; y++)
		{


			byMask = 0x80; // マスク値初期化
			lpCurrent = p2; // 現在位置を行頭に
			for (int x = 0; x < lWidth; x++ )
			{
				if ( !byMask ) 
				{
					byMask = 0x80;
					lpCurrent++;
				}
				if ( *lpCurrent & byMask )
				{
					DWORD dwColor = m_rgb | (DWORD)(m_a) << 24;
					if((dwColor & 0xff000000) != 0x00000000)
					{
						p1[x] = dwColor; 
					}
				}
				byMask >>= 1;
			}
			p2 += fontPitch;
			p1 += pitch; 
		}
	}


	


	//m_dwX += (DWORD)(m_fSize.x * (a + c) + gm.gmBlackBoxX);
	m_dwX += (DWORD)(m_fSize.x * (c) + gm.gmBlackBoxX);
				


	// バッファ開放
	if ( lpFont != NULL )
	{
		delete [] lpFont;
		lpFont = NULL;
	}
}

//======================================================================
//
//		アンチエイリアスシング処理のフラグ	大きい文字ならＯＮで小さい文字ならOFFがいいです。
//
//======================================================================
void SpriteText::SetAntiAlias(bool inAntiFlag)
{
	m_AntiAlias = inAntiFlag;
}
void SpriteText::SetWeight(int inWeight)
{
	m_Weight = inWeight;
}

void SpriteText::SetCursolPos(int x,int y)
{
	m_dwX = x;
	m_dwY = y;
}

void SpriteText::SetOver(bool inOver)
{
	m_Over = inOver;
}


DWORD SpriteText::GetSize()
{
	return m_dwTexSize;
}
void SpriteText::SetDrawText(TCHAR* Buffer)
{
	//===========================================================
	// テクスチャロック
	//===========================================================
	D3DLOCKED_RECT TexRect;
	HRESULT hr = m_lpTex->LockRect( 0, &TexRect, NULL, 0 );
	if FAILED( hr ) return;
	// 文字長
	DWORD len = (DWORD)_tcslen( Buffer );

	// フォント高さ取得-
	TEXTMETRIC tm;
	GetTextMetrics( m_hdc, &tm );


	//m_dwBackX = m_dwX;	//開業するときに必要。
	for(DWORD i = 0; i < len; i++)
	{
		//==================================================
		// 文字コード
		//==================================================
		long code = Buffer[i];

		// 全角
		//**************************************************
		//追加点　Unicode
		//**************************************************
#ifdef UNICODE
		
#else
		if ( IsZenkaku( Buffer[i] ) )
		{
			// Shift-JISコード
			code = GetCharCode( (UByte *)&Buffer[i++] );
		}
#endif
		switch ( code )
		{
		case _T('\n'):
			NL(m_dwBackX);
			break;
		//**************************************************
		//追加点　スペース
		//**************************************************
		case _T(' '):
			m_dwX += (m_dwTexSize* m_fSize.x)/2 ;
			break;
		case _T('_'):
			m_dwX += (m_dwTexSize* m_fSize.x)/4;
			break;
		default:
			DrawChar(code,TexRect,tm);
			break;
		}
	}

	m_lpTex->UnlockRect( 0 );
}

//************************************************************************
//
//		文字の幅を得ます
//
//************************************************************************
int SpriteText::GetTextWidth(const TCHAR* tstring)
{
	// 文字長
	DWORD len = (DWORD)_tcslen( tstring );

	int width = 0;
	int maxwidth = 0;

	for(DWORD i = 0; i < len; i++)
	{
		//==================================================
		// 文字コード
		//==================================================
		long code = tstring[i];

		// 全角
		//**************************************************
		//追加点　Unicode
		//**************************************************
#ifdef UNICODE
		
#else
		if ( IsZenkaku( tstring[i] ) )
		{
			// Shift-JISコード
			code = GetCharCode( (UByte *)&tstring[i++] );
		}
#endif
		switch ( code )
		{
		case _T('\n'):
			maxwidth = max(maxwidth,width);
			width = 0;
			break;
		//**************************************************
		//追加点　スペース
		//**************************************************
		case _T(' '):
			width += (m_dwTexSize* m_fSize.x)/2 ;
			break;
		case _T('_'):
			width += (m_dwTexSize* m_fSize.x)/4;
			break;
		default:
			width += GetcharWidth(code);
			break;
		}
	}

	return maxwidth = max(maxwidth,width);
}
int SpriteText::GetcharWidth(unsigned int code)
{
	GLYPHMETRICS gm;
	ABC abc;

	//==================================================
	// フォントバッファ格納
	//==================================================
	LPBYTE lpFont = NULL;

	// フォントバッファ取得
	DWORD DataSize = GetFontBuffer( code, &gm, &lpFont );
	if(lpFont == NULL) return 0;

	// フォント幅取得
	GetCharABCWidths( m_hdc, code, code, &abc );

	// バッファ開放
	if ( lpFont != NULL )
	{
		delete [] lpFont;
		lpFont = NULL;
	}

	return (DWORD)((int)(m_fSize.x * abc.abcA) + (m_fSize.x * abc.abcC) + gm.gmBlackBoxX);
}


void SpriteText::NL(DWORD dwBackX)
{
	m_dwY += (DWORD)(m_fSize.y * m_dwTexSize + m_fMargin);
	m_dwX = dwBackX;
}
//************************************************************************
//
//		文字の幅をWidth内におさめるよう縮小します。もとは（1.0,1.0）のサイズの文字とします
//
//************************************************************************
void SpriteText::FitTextWidth(StyleString& sstr, int Width)
{
	D3DXVECTOR2 vec(1.0,1.0);
	sstr.setSameSize(vec);
	int textwidth = GetTextWidth(sstr.c_str());
	if( textwidth > Width)
	{	
		vec.x = (double)Width/textwidth; vec.y = 1.0f;
		sstr.setSameSize(vec);
	}
}

//=======================================================================
//
//		行列を更新します。
//
//=======================================================================
void SpriteText::UpDate()
{
	float fCos = cos(m_fRot);
	float fSin = sin(m_fRot);

    long m11 = (long)(m_fSize.x * fCos * 65536.0);	long m12 = (long)(m_fSize.y * fSin * 65536.0);
    long m21 = (long)(-m_fSize.x * fSin * 65536.0);	long m22 = (long)(m_fSize.y * fCos * 65536.0);

	m_mat.eM11 = *( (FIXED *)&m11 );	m_mat.eM12 = *( (FIXED *)&m12 );
	m_mat.eM21 = *( (FIXED *)&m21 );	m_mat.eM22 = *( (FIXED *)&m22 );

}

//=======================================================================
//
//		大きさを更新します。
//
//=======================================================================
void SpriteText::SetSize(D3DXVECTOR2 &vec2Size)
{
	m_fSize = vec2Size;
	UpDate();
}

//=======================================================================
//
//		オブジェクトを回転させます。
//
//=======================================================================
void SpriteText::SetRot(float fRot)
{
	m_fRot = fRot;
	UpDate();
}

//************************************************************************
//
//		強制改行する行の長さを設定します
//
//************************************************************************
void SpriteText::SetReturnWidth(DWORD width)
{
	m_ReturnWidth = width;
}


//************************************************************************
//
//		強制改行したときの基準線を設定します
//
//************************************************************************
void SpriteText::SetReturnBaseLine(DWORD BaseLine)
{
	m_dwBackX = BaseLine;
}

//************************************************************************
//
//		強制改行したときの改行マージンを設定します
//
//************************************************************************
void SpriteText::SetReturnFontMargin(DWORD margin)
{
	m_fMargin = margin;
}

//==========================================================================
//
//			フォントバッファ取得
//
//==========================================================================
DWORD SpriteText::GetFontBuffer( long code, GLYPHMETRICS *pgm, LPBYTE *lpData)
{
	ZeroMemory( pgm, sizeof(GLYPHMETRICS) );

	//=============================================================
	// フォントデータ取得
	//=============================================================

	int theBitMap = GGO_BITMAP;
	if(m_AntiAlias)
	{
		theBitMap = GGO_GRAY8_BITMAP;
	}

	// 16階調のアンチェリフォント
	DWORD Size;
	// バッファサイズ受信
	Size = GetGlyphOutline( m_hdc, code, theBitMap, pgm, 0, NULL, &m_mat );
	
	if( Size == GDI_ERROR ){
		//エラー
		*lpData = NULL;
		return Size;
	}
	// バッファ取得
	*lpData = new BYTE [Size ];
	// バッファにフォント受信
	GetGlyphOutline( m_hdc, code, theBitMap, pgm, Size, *lpData, &m_mat );

	return Size;
}

//==========================================================================
//
//			キャラコード取得
//
//==========================================================================
long SpriteText::GetCharCode( UByte *str )
{
	long str0 = str[0];
	long str1 = str[1];
 	return str0 << 8 | str1; 
}



//==========================================================================
//
//		全角チェック
//
//==========================================================================
bool SpriteText::IsZenkaku( UByte c )
{
#ifdef UNICODE
	return true;
#else
	if ( c < 0x81 ) return false;//制御コード、半角英数字
	if ( c < 0xA0 ) return true;
	if ( c < 0xE0 ) return false;//半角カナ
	if ( c < 0xFF ) return true;

	return false;
#endif
}
