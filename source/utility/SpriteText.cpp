//======================================================================
//このソースコードはTWINTAIL LOVERS(http://www.twin-tail.jp/)さんが作成したものを
//すき焼き(http://www.c3.club.kyutech.ac.jp/~sukiyaki/)が改造したものです。
//勝手にソースコードを改変することを許可します。
//======================================================================



#include "SpriteText.h"
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <string.h>

#include "debug.h"

//======================================================================
//
//		コンストラクタ
//
//======================================================================
SpriteText::SpriteText()
{
	m_lpTex = NULL;
	m_fRot = 0.0f;
	m_fSize = cRenderVector2(1.0f,1.0f);
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
bool SpriteText::Init( cRenderDevice* lpDev,long num, DWORD w, DWORD h)
{
	m_lpTex = NULL;
	
	long i;


	for ( i = 0; i <= 64; i++ )
	{
		m_byAlphaTbl[i] = (BYTE)( i*255/64 );
	}



	if(!cRenderCreateManagedTexture(lpDev, w, h, &m_lpTex))
	{
		return 1;
	}

	int textureWidth = 0;
	int textureHeight = 0;
	cRenderGetTextureSize(m_lpTex, textureWidth, textureHeight);
	
	m_dwWidth = textureWidth;
	m_dwHeight = textureHeight;

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
		 cRenderRelease(m_lpTex);
	}
}




//======================================================================
//
//		作成したテクスチャを取得します。
//
//======================================================================
cRenderTexture* SpriteText::GetTex()
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

	cRenderLockedRect TexRect;
	m_fRot = 0.0f;
	m_fSize = cRenderVector2(1.0f,1.0f);

	if(m_lpTex != NULL)
	{
		if(cRenderLockTexture(m_lpTex, TexRect))
		{
			memset( TexRect.pBits, 0x00, TexRect.Pitch*m_dwHeight );
			cRenderUnlockTexture(m_lpTex);
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
	m_dwTexSize = size;
	cRenderBeginText(m_textContext, face, size, m_Weight);
}

//======================================================================
//
//		テキスト使用終了
//
//======================================================================
void SpriteText::EndText( void )
{
	cRenderEndText(m_textContext);
}


//======================================================================
//
//		テキスト色
//
//======================================================================
void SpriteText::SetColor( DWORD color )
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
	cRenderVector2 v(1.0f,1.0f);
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
void SpriteText::DrawChar(unsigned int code,cRenderLockedRect &TexRect,const cRenderTextMetrics &tm)
{
	cRenderGlyphMetrics gm;
	cRenderGlyphABC abc;

	//==================================================
	// フォントバッファ格納
	//==================================================
	BYTE* lpFont = NULL;

	// フォントバッファ取得
	DWORD DataSize = GetFontBuffer( code, &gm, &lpFont );
	if(lpFont == NULL) return;

	// フォント幅取得
	if(!cRenderGetGlyphABC(m_textContext, code, abc))
	{
		delete [] lpFont;
		return;
	}

	BYTE* p2 = lpFont;


	// 描画位置を進める
	/*
	int a = (abc.a)?(2):(0);	//文字の左淵の大きさ
	int b = abc.b;			//文字の大きさ
	int c = (abc.c)?(2):(0);	//文字の右淵の大きさ
	*/
	int a;	//文字の左淵の大きさ
	int b;	//文字の大きさ
	int c;	//文字の右淵の大きさ

	a = abc.a;	//文字の左淵の大きさ
	b = abc.b;	//文字の大きさ
	c = abc.c;	//文字の右淵の大きさ

	//**************************************************
	//追加点　強制改行
	//**************************************************
	if(m_dwX + m_fSize.x * (c + a + gm.blackBoxX) > m_ReturnWidth) NL(m_dwBackX);

	m_dwX += m_fSize.x * (a);




	// サイズ取得
	lWidth = (long)gm.blackBoxX;
	lHeight = (long)gm.blackBoxY;
	DWORD advance = (DWORD)(m_fSize.x * (c) + gm.blackBoxX);
	if(m_dwX >= m_dwWidth || m_dwY >= m_dwHeight || lWidth <= 0 || lHeight <= 0)
	{
		m_dwX += advance;
		delete [] lpFont;
		return;
	}
	


	if(m_dwX + lWidth > m_dwWidth)
	{
		lWidth = m_dwWidth - m_dwX;
	}
					
	if(m_dwY + lHeight > m_dwHeight)
	{
		lHeight = m_dwHeight - m_dwY;
	}
	if(lWidth <= 0 || lHeight <= 0)
	{
		m_dwX += advance;
		delete [] lpFont;
		return;
	}


	



	//==================================================
	// 描画先サーフェイスのポインタ
	//==================================================
	DWORD* p1 = (DWORD*)TexRect.pBits;
	DWORD pitch = TexRect.Pitch / 4;





	// 転送先のサーフェイスの始点


	//==================================================
	//上書き判定
	//==================================================


	/*
	if(m_Over)
	{
		DWORD* pt = (DWORD*)TexRect.pBits;
		pt += m_dwX + (m_dwY) * pitch;
		int width = (int)(m_fSize.x * (2 + 2) + gm.blackBoxX);
		int height = tm.height + tm.externalLeading * 2;
		for ( long y = 0; y <  height; y++ )
		{
			memset(pt, 0x00, width * sizeof(DWORD));
			pt += pitch;
		}
		
	}
	*/
	if(m_Over)
	{
		DWORD* pt = (DWORD*)TexRect.pBits;
		pt += m_dwX + (m_dwY) * pitch;
		int width = (int)(m_fSize.x * (a + c) + gm.blackBoxX);
		int height = tm.height + tm.externalLeading * 2;
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
	int sy = tm.ascent  - gm.glyphOriginY;

	int add = m_dwX + (m_dwY + sy) * pitch;

	if(add < 0)
	{
		add = 0;
	}
	p1 = (DWORD*)TexRect.pBits;
	p1 += add;




	//==================================================
	// 転送
	//==================================================
	// フォントピッチ
	DWORD fontPitch;
	if(m_AntiAlias)
	{
		if(gm.blackBoxY == 0) fontPitch = INT_MAX & ~0x03;
		else fontPitch = (DataSize / gm.blackBoxY) & ~0x03;
		
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
		BYTE* lpCurrent; // 現在チェック中のBYTE位置
		fontPitch = ( ( gm.blackBoxX + 0x1f ) & ~0x1f ) / 8; // 1行分のバイト数
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


	


	//m_dwX += (DWORD)(m_fSize.x * (a + c) + gm.blackBoxX);
	m_dwX += advance;
				


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
	cRenderLockedRect TexRect;
	if(!cRenderLockTexture(m_lpTex, TexRect)) return;
	// 文字長
	DWORD len = (DWORD)_tcslen( Buffer );

	// フォント高さ取得-
	cRenderTextMetrics tm;
	if(!cRenderGetTextMetrics(m_textContext, tm))
	{
		cRenderUnlockTexture(m_lpTex);
		return;
	}


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

	cRenderUnlockTexture(m_lpTex);
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
			if(width > maxwidth) maxwidth = width;
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

	if(width > maxwidth) maxwidth = width;
	return maxwidth;
}
int SpriteText::GetcharWidth(unsigned int code)
{
	cRenderGlyphMetrics gm;
	cRenderGlyphABC abc;

	//==================================================
	// フォントバッファ格納
	//==================================================
	BYTE* lpFont = NULL;

	// フォントバッファ取得
	DWORD DataSize = GetFontBuffer( code, &gm, &lpFont );
	if(lpFont == NULL) return 0;

	// フォント幅取得
	if(!cRenderGetGlyphABC(m_textContext, code, abc))
	{
		delete [] lpFont;
		return 0;
	}

	// バッファ開放
	if ( lpFont != NULL )
	{
		delete [] lpFont;
		lpFont = NULL;
	}

	return (DWORD)((int)(m_fSize.x * abc.a) + (m_fSize.x * abc.c) + gm.blackBoxX);
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
	cRenderVector2 vec(1.0,1.0);
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
	cRenderBuildTextTransform(m_textTransform, m_fSize.x, m_fSize.y, m_fRot);
}

//=======================================================================
//
//		大きさを更新します。
//
//=======================================================================
void SpriteText::SetSize(const cRenderVector2 &vec2Size)
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
DWORD SpriteText::GetFontBuffer( long code, cRenderGlyphMetrics *pgm, BYTE **lpData)
{
	return cRenderGetGlyphBitmap(m_textContext, code, m_AntiAlias, m_textTransform, *pgm, lpData);
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
