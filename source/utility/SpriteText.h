#ifndef __SPRITETEXT_H__
#define __SPRITETEXT_H__


//======================================================================
//このソースコードはTWINTAIL LOVERS(http://www.twin-tail.jp/)さんが作成したものを
//すき焼き(http://www.c3.club.kyutech.ac.jp/~sukiyaki/)が改造したものです。
//勝手にソースコードを改変することを許可します。
//======================================================================

#include "../gameMainSystem/cRenderBackend.h"


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
	cRenderTexture* m_lpTex;
	DWORD m_dwWidth, m_dwHeight;
	cRenderTextContext m_textContext;
	DWORD m_dwTexSize;
	cRenderVector2 m_fSize;
	DWORD m_dwX;
	DWORD m_dwY;

	DWORD m_dwBackX;	//改行するときに必要。
	DWORD m_fMargin;	//改行マージン
	DWORD m_ReturnWidth; //これ以上行が長くなると強制改行

	int m_Weight;	//太さ


	float m_fRot;
	cRenderTextTransform m_textTransform;
	long lWidth;
	long lHeight;
	bool m_AntiAlias;	//アンチエイリアスフラグ
	BYTE m_a;
	DWORD m_rgb;
	bool m_Over;	//上書きフラグ
protected:
	DWORD GetFontBuffer( long code, cRenderGlyphMetrics *pgm, BYTE **lpData );
	long GetCharCode( UByte *str );
	void SetDrawText(TCHAR* tstring);
	bool IsZenkaku( UByte c );
	void UpDate();
	void DrawChar(unsigned int code,cRenderLockedRect &TexRect,const cRenderTextMetrics &tm);

	int GetcharWidth(unsigned int code);
public:
	DWORD GetX();
	DWORD GetY();
	cRenderTexture* GetTex();
	bool Init( cRenderDevice* lpDev,long num, DWORD w, DWORD h);
	void UnInit( void );
	void BeginText(const TCHAR* face, DWORD size );
	void EndText( void );
	void SetColor(DWORD color );
	void SetSize(const cRenderVector2 &vec2Size);
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
