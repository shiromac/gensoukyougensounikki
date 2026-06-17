#pragma once
#include "cdrawingobject.h"
#include "utility/SpriteText.h"
#include "utility/cColor.h"
#include "cControl.h"

#include <tstring_ph.h>


using namespace std;


#define WINDOWTEXPATH _T("skin\\window.png")
#define CURSORTEXPATH _T("skin\\cursor.png")
#define PAGETEXPATH _T("skin\\page.png")

#define GWFONTSIZE (23)
#define GWFONTOVERMARGIN (3)
#define GWFONTUNDERMARGIN (3)

const int GWFONTWEIGHT = 500;

#define WINDOWTEXSIZE (128)
#define EDGEWIDTH (16)
#define EDGE_VERTICALSPACE (8)
#define PAGETEXSIZE (32)

//#define MAXBUFFSIZE 4096


#define SOUND_SELECTDECIDE _T("select_decidion.wav")
#define SOUND_SELECTCANCEL _T("select_cancel.wav")
#define SOUND_SELECTMOVE _T("select_move.wav")



//x以上の最小の2^n
inline int largerMinPow2(int x)
{
	int i;
	for(i=0;i<64;i++)
	{
		if(!((x-1) >> i)) break;
	}
	x = 1<<i;//tw以上の最小の2^n
	return x;
};

class cGameWindow :
	public cControl
{
public:
	cGameWindow(void);
public:
	virtual ~cGameWindow(void);

	//特殊インターフェイス
	int closeLayer;

public:

	//処理するかどうか
	int enable;

	virtual void Init(cRenderDevice *pDev, int letterXnum, int letterYnum);
	virtual void Init(int letterXnum, int letterYnum);
	virtual void InitBasic(cRenderDevice *pDev, int letterXnum, int letterYnum);

	inline int merginheight(){return GWFONTOVERMARGIN + GWFONTUNDERMARGIN;};

	inline int sideEdgeWith(){return EDGEWIDTH+EDGE_VERTICALSPACE;};

	inline int lineheight(){return GWFONTSIZE + merginheight();};

	inline int linetop(int line){return lineheight()*line + GWFONTOVERMARGIN;};

	cDrawingObject m_DO;

	cColor color;

	int letterXlength;
	int letterYlength;

protected:

	int soundIndex_decide;
	int soundIndex_cancel;
	int soundIndex_move;


	cRenderTexture* m_pTexture_Window;
	cRenderTexture* m_pTexture_Cursor;

	virtual int WindowDraw(cRenderDevice *pDev);
	virtual int TextDraw(cRenderDevice *pDev);

	virtual int TextDrawSub(cRenderDevice *pDev);

	virtual int pageDraw(cRenderDevice *pDev);


	SpriteText m_pageText;
	int m_pageAnimeCount;
	
	cRenderTexture* m_pTexture_Page;


	SpriteText m_SpriteText;
	bool preRendar;


protected:
	//共通ルーチン
	//void DrawAddShadowDO(cDrawingObject& DO, cRenderDevice *pDev);
public:

	//virtual StyleString& Text(){return ((vText.empty())? _T("") : vText[0]);};
	virtual StyleString& Text(){return vText[0];};
	virtual StyleString& Text(int page){return vText.at(page);};
	std::vector<StyleString> vText;//サイズ１以上を保障
	virtual int ReRendarText();

	virtual int process(cRenderDevice *pDev);


	virtual StyleString shortExplanationString(cRenderDevice *pDev){return _T("");};


	virtual int pageIndexMaxF(){return vText.size();};
	int pageIndex;


	virtual int Draw(cRenderDevice *pDev);


	virtual void playsound_decide();
	virtual void playsound_cancel();
	virtual void playsound_move();

	//ユーティリティ
	void FitTextWidthToWindow(StyleString& sstr);
};

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cGameWindow> pcGameWindow;


//共通ルーチン
void DrawAddShadowDO(cDrawingObject& DO, cRenderDevice *pDev);
void DrawAddShadowDO_mini(cDrawingObject& DO, cRenderDevice *pDev);
