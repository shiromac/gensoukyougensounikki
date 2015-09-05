#ifndef __SPRITETEXT_H__
#define __SPRITETEXT_H__


//======================================================================
//このソースコードはTWINTAIL LOVERS(http://www.twin-tail.jp/)さんが作成したものを
//すき焼き(http://www.c3.club.kyutech.ac.jp/~sukiyaki/)が改造したものです。
//勝手にソースコードを改変することを許可します。
//======================================================================

#include <windows.h>
#include <d3dx9.h>
#include <d3d9.h>


#include "StyleString.h"


#define MAXBUFFSIZE 2048

//=============================================================
//このクラスは２バイト文字をサポートする描画クラスです。
//ただしDrawStringより低速です。
//=============================================================
class SpriteText
{
public:
	SpriteText();
	~SpriteText();
protected:
	BYTE m_byAlphaTbl[65];
	LPDIRECT3DTEXTURE9 m_lpTex;
	DWORD m_dwWidth, m_dwHeight;
	HFONT m_hFont, m_hOldFont;
	DWORD m_dwTexSize;
	HDC m_hdc;
	D3DXVECTOR2 m_fSize;
	DWORD m_dwX;
	DWORD m_dwY;

	DWORD m_dwBackX;	//改行するときに必要。
	DWORD m_fMargin;	//改行マージン
	DWORD m_ReturnWidth; //これ以上行が長くなると強制改行

	int m_Weight;	//太さ


	float m_fRot;
	MAT2 m_mat;
	HWND m_hwnd;
	LONG lWidth;
	LONG lHeight;
	bool m_AntiAlias;	//アンチエイリアスフラグ
	BYTE m_a;
	DWORD m_rgb;
	bool m_Over;	//上書きフラグ
protected:
	DWORD GetFontBuffer( long code, GLYPHMETRICS *pgm, LPBYTE *lpData );
	long GetCharCode( UByte *str );
	void SetDrawText(TCHAR* tstring);
	bool IsZenkaku( UByte c );
	void UpDate();
	void DrawChar(unsigned int code,D3DLOCKED_RECT &TexRect,TEXTMETRIC &tm);

	int GetcharWidth(unsigned int code);
public:
	DWORD GetX();
	DWORD GetY();
	LPDIRECT3DTEXTURE9 GetTex();
	bool Init( LPDIRECT3DDEVICE9 lpDev,long num, DWORD w, DWORD h);
	void UnInit( void );
	void BeginText(const TCHAR* face, DWORD size );
	void EndText( void );
	void SetColor(DWORD color );
	void SetSize(D3DXVECTOR2 &vec2Size);
	void SetRot(float fRot);

	void SetReturnWidth(DWORD width);
	void SetReturnFontMargin(DWORD margin);
	void SetReturnBaseLine(DWORD BaseLine);
	int GetTextWidth(const TCHAR* tstring);

	//テキスト幅を枠内に入るよう補正する
	void FitTextWidth(StyleString& sstr, int Width);


	void DrawText( DWORD x, DWORD y, TCHAR* tstring);
	void DrawText(const TCHAR* tstring);

	void DrawText(tstring str);
	void DrawText(StyleString str);

	void CleatText( void );
	void SetAntiAlias(bool inAntiFlag);
	void SetWeight(int inWeight);
	void SetCursolPos(int x,int y);
	void NL(DWORD dwBackX);
	DWORD GetSize();
	void SetOver(bool);
};

#endif
