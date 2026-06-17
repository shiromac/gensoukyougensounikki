#pragma once

#include "../utility/cRectObj.h"
#include "../utility/cColor.h"
#include "cRenderBackend.h"

#define LAYERNUM_MAX (2)

//環境に唯一作られる
class cScreen : public cRectObj
{
public:
	cScreen(void);
public:
	virtual ~cScreen(void);

	void initScreen(cRenderDevice* pDevice);

	//描画！
	void Draw(cRenderDevice* pDevice);
	void Clear(cRenderDevice* pDevice, cColor& c);

	void DebugDraw(cRenderDevice* pDevice);
	
	//	レンダーターゲットをせってい。
	void setRenderTarget(cRenderDevice* pDevice, int screen_ID);

	//	アルファブレンディングモードをせってい。
	void setAlphaMode(cRenderDevice* pDevice, int screen_ID);

private:
	cRenderSurface*		m_pBackBufferSurface;//元の画面のサーフェイス

public:
	static const int TEXTURESIZE = 1024;//テクスチャのサイズ

	cRenderTexture*		m_pOffScreenTexture;
	cRenderSurface*		m_pOffScreenSurface;//オフスクリーンのサーフェイス
	boolean					m_OffScreenEnable;//オフスクリーンを使用するかどうかフラグ


	cRenderTexture*		m_pLayerTexture;
	cRenderSurface*		m_pLayerSurface;//レイヤーのサーフェイス
	boolean					m_LayerEnable;//使用するフラグ
	boolean					m_LayerClear;//毎回クリアするフラグ

	int						m_LayerMode;
	cColor					m_LayerColor;


private:
	cRenderTexture*		m_pMiniTexture[2];
	cRenderSurface*		m_pMiniSurface[2];//レイヤーのサーフェイス
public:
	boolean					m_MiniEnable;//使用するフラグ
	int						m_MiniMode;
	cColor					m_MiniColor;
	double					m_MiniMagnify;//拡大率
	
	boolean					m_MiniBlur;//ブラー
	double					m_MiniBlurValue;//ブラー値

	int						m_MiniRotation;
	static const int MINIPROPOTION = 8;//何分の一か
	
	
};
const int SCREEN_OFFSCREEN = 0;
const int SCREEN_LAYER = 1;
const int SCREEN_BACKGROUND = 2;

const int ALPHA_MODE_ADDITION = 0;
const int ALPHA_MODE_ANTIADDITION = 1;
const int ALPHA_MODE_NORMAL = 2;
const int ALPHA_MODE_HALFADDITION = 3;

