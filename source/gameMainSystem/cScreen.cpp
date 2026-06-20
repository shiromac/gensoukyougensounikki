#include "../stdafx.h"

#include "cScreen.h"
#include "../utility/cColor.h"


namespace
{
void SetScreenPointSampler(cRenderDevice* pDevice, DWORD sampler)
{
	cRenderSetPointSampler(pDevice, sampler);
}
}

cScreen::cScreen(void)
{
}

cScreen::~cScreen(void)
{
}

void cScreen::initScreen(cRenderDevice* pDevice)
{
	cResourseManage* RManager = g_GameEnv.m_GlobalResourse;
	//バックバッファの保存
	m_pBackBufferSurface = RManager->getBackBuffer(pDevice);
	
	//オフスクリーンの生成。
	m_pOffScreenTexture = *RManager->makeTexture(pDevice,TEXTURESIZE,TEXTURESIZE);
	//(m_pOffScreenTexture)->GetSurfaceLevel(0, &m_pOffScreenSurface);
	RManager->getSurfaceFromTexture(m_pOffScreenTexture, m_pOffScreenSurface);
	
	//ターゲットをオフスクリーンに
	setRenderTarget(pDevice, SCREEN_OFFSCREEN);

	//レイヤーの生成。
	m_pLayerTexture = *RManager->makeTexture(pDevice,TEXTURESIZE,TEXTURESIZE);
	//(m_pLayerTexture)->GetSurfaceLevel(0, &m_pLayerSurface);
	RManager->getSurfaceFromTexture(m_pLayerTexture, m_pLayerSurface);
	
	//及び設定
	m_LayerEnable = false;
	m_LayerMode = ALPHA_MODE_ADDITION;
	m_LayerColor.ARGB(255,128,0,0);
	
	//ミニレイヤーの生成
	m_pMiniTexture[0] = *RManager->makeTexture(pDevice,TEXTURESIZE/MINIPROPOTION ,TEXTURESIZE/MINIPROPOTION);
	//(m_pMiniTexture[0])->GetSurfaceLevel(0, &m_pMiniSurface[0]);
	RManager->getSurfaceFromTexture(m_pMiniTexture[0], m_pMiniSurface[0]);
	//m_pMiniTexture[1] = RManager->makeTexture(pDevice,TEXTURESIZE/MINIPROPOTION ,TEXTURESIZE/MINIPROPOTION);
	//m_pMiniTexture[1]->GetSurfaceLevel(0, &m_pMiniSurface[1]);
	//m_pMiniSurface = RManager->getSurfaceFromTexture(m_pMiniTexture);
	//及び設定
	m_MiniEnable = false;
	m_MiniMode = ALPHA_MODE_ADDITION;
	m_LayerColor.ARGB(255,0,128,0);
	m_MiniMagnify = 1;
	m_MiniRotation = 0;
	m_MiniBlur = false;//ブラー
	m_MiniBlurValue = 0.7;//ブラー値
	//スクリーンサイズ指定
	setLTRB(0,0,SCREEN_X,SCREEN_Y);

	m_OffScreenEnable = true;
}

void cScreen::Draw(cRenderDevice* pDevice)
{



	//HRESULT hr;
	cRenderSetViewport(pDevice, SCREEN_X, SCREEN_Y);
	SetScreenPointSampler(pDevice, 0);
	SetScreenPointSampler(pDevice, 1);

	/*
	//クリア
	setRenderTarget(pDevice, SCREEN_BACKGROUND);
	pDevice->Clear( 0, NULL, D3DCLEAR_TARGET,//|D3DCLEAR_ZBUFFER,
		0xFF000000, 1.0f, 0 );
	*/

	//m_MiniMagnify = 1.2;
	//m_MiniBlur = true;

	cRectObj tex_map;

	tex_map.setLTRB(0.5 / TEXTURESIZE, 0.5 / TEXTURESIZE, (double)(SCREEN_X+0.5)/TEXTURESIZE, (double)(SCREEN_Y+0.5)/TEXTURESIZE);
	float tmLeft = (float)tex_map.Left();
	float tmTop = (float)tex_map.Top();
	float tmRight = (float)tex_map.Right();
	float tmBottom = (float)tex_map.Bottom();

	VERTEX2D VxData[] = {
			{ (float) Left(), (float)   Top(), 0, 1, tmLeft, tmTop },
			{ (float)Right(), (float)   Top(), 0, 1, tmRight, tmTop },
			{ (float) Left(), (float)Bottom(), 0, 1, tmLeft, tmBottom },
			{ (float)Right(), (float)Bottom(), 0, 1, tmRight, tmBottom }
		};
	VERTEX2D_COLORED VxDataColor[] = {
			{ 0.0f				, 0.0f				, 0, 1, m_LayerColor.D3Dcolor(), tmLeft, tmTop },
			{ (float)SCREEN_X	, 0.0f				, 0, 1, m_LayerColor.D3Dcolor(), tmRight, tmTop },
			{ 0.0f				, (float)SCREEN_Y	, 0, 1, m_LayerColor.D3Dcolor(), tmLeft, tmBottom },
			{ (float)SCREEN_X	, (float)SCREEN_Y	, 0, 1, m_LayerColor.D3Dcolor(), tmRight, tmBottom }
		};

	cRectObj miniScreen;
	miniScreen = *this;
	miniScreen.CenterX = miniScreen.CenterY = TEXTURESIZE/MINIPROPOTION/2;
	miniScreen.Width = SCREEN_X/MINIPROPOTION;
	miniScreen.Height = SCREEN_Y/MINIPROPOTION;
	float msLeft = (float)miniScreen.Left();
	float msTop = (float)miniScreen.Top();
	float msRight = (float)miniScreen.Right();
	float msBottom = (float)miniScreen.Bottom();
	//テクスチャ領域
	float mstLeft = (float)miniScreen.Left()/(TEXTURESIZE/MINIPROPOTION);
	float mstTop = (float)miniScreen.Top()/(TEXTURESIZE/MINIPROPOTION);
	float mstRight = (float)miniScreen.Right()/(TEXTURESIZE/MINIPROPOTION);
	float mstBottom = (float)miniScreen.Bottom()/(TEXTURESIZE/MINIPROPOTION);


	VERTEX2D VxDataToMini[] = {
			{ msLeft	, msTop,	0, 1, tmLeft, tmTop },
			{ msRight	, msTop,	0, 1, tmRight, tmTop },
			{ msLeft	, msBottom,	0, 1, tmLeft, tmBottom },
			{ msRight	, msBottom,	0, 1, tmRight, tmBottom }
		};
	VERTEX2D_COLORED MiniBlack[] = {
			{ msLeft	, msTop	,	0, 1,cRenderColorARGB((int)(255*(1-m_MiniBlurValue)),0,0,0), 0.0f		 , 0.0f },
			{ msRight	, msTop	,	0, 1,cRenderColorARGB((int)(255*(1-m_MiniBlurValue)),0,0,0), 0.0f		 , 0.0f },
			{ msLeft	, msBottom,	0, 1,cRenderColorARGB((int)(255*(1-m_MiniBlurValue)),0,0,0), 0.0f		 , 0.0f },
			{ msRight	, msBottom,	0, 1,cRenderColorARGB((int)(255*(1-m_MiniBlurValue)),0,0,0), 0.0f		 , 0.0f },
	};


	//拡大
	miniScreen.Width /= m_MiniMagnify;
	miniScreen.Height /= m_MiniMagnify;
	miniScreen.CenterX += 0.5;
	miniScreen.CenterY += 0.5;
	//テクスチャ領域
	mstLeft = (float)miniScreen.Left()/(TEXTURESIZE/MINIPROPOTION);
	mstTop = (float)miniScreen.Top()/(TEXTURESIZE/MINIPROPOTION);
	mstRight = (float)miniScreen.Right()/(TEXTURESIZE/MINIPROPOTION);
	mstBottom = (float)miniScreen.Bottom()/(TEXTURESIZE/MINIPROPOTION);

	

	VERTEX2D_COLORED VxDataMIniToBG[] = {
			{ (float) Left(), (float)   Top(), 0, 1, m_MiniColor.D3Dcolor(), mstLeft 	, mstTop },
			{ (float)Right(), (float)   Top(), 0, 1, m_MiniColor.D3Dcolor(), mstRight	, mstTop },
			{ (float) Left(), (float)Bottom(), 0, 1, m_MiniColor.D3Dcolor(), mstLeft	, mstBottom },
			{ (float)Right(), (float)Bottom(), 0, 1, m_MiniColor.D3Dcolor(), mstRight , mstBottom }
		};

	VERTEX2D_COLORED VxDataBlack[] = {
			{ (float) Left(), (float)   Top(), 0, 1, 0xFF000000, tmLeft, tmTop },
			{ (float)Right(), (float)   Top(), 0, 1, 0xFF000000, tmRight, tmTop },
			{ (float) Left(), (float)Bottom(), 0, 1, 0xFF000000, tmLeft, tmBottom },
			{ (float)Right(), (float)Bottom(), 0, 1, 0xFF000000, tmRight, tmBottom }
		};
	

	
	if(m_OffScreenEnable)
	{

		//通常の処理


		//ターゲットを背景に
		setRenderTarget(pDevice, SCREEN_BACKGROUND);
		//テクスチャをオフスクリーンに
		cRenderSetTexture(pDevice, 0, (m_pOffScreenTexture));
		// 頂点ＦＶＦを設定
		cRenderSetTexturedVertexFormat(pDevice);

		//アルファ合成を可に
		cRenderSetAlphaBlendEnabled(pDevice, true);

		// ポリゴンを描画
		cRenderDrawTriangleStrip(pDevice, 2, VxData);

	}




	if(m_LayerEnable)
	{
		/*

		if(m_LayerMode != ALPHA_MODE_ANTIADDITION)
		{
			//アルファモード設定
			setAlphaMode(pDevice, m_LayerMode);
			//ターゲットをオフスクリーンに
			setRenderTarget(pDevice, SCREEN_OFFSCREEN);
			//テクスチャをレイヤーに
			pDevice->SetTexture(0, m_pLayerTexture);

			// 頂点ＦＶＦを設定
			pDevice->SetFVF( VERTEX2D_COLORED::FVF );

			// 頂点色を乗算する
			pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
			pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_CURRENT );
			


			// ポリゴンを描画
			pDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, VxDataColor, sizeof(VERTEX2D_COLORED) );

			//そのまま全部描画しちゃう
			//
			//
							//ターゲットを背景に
			setRenderTarget(pDevice, SCREEN_BACKGROUND);
			//テクスチャをオフスクリーンに
			pDevice->SetTexture(0, m_pOffScreenTexture);
			// 頂点ＦＶＦを設定
			pDevice->SetFVF( VERTEX2D::FVF );

			//アルファ合成を不可に
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

			// ポリゴンを描画
			pDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, VxData, sizeof(VERTEX2D) );


	
		}
		else//m_LayerMode == ALPHA_MODE_ANTIADDITION
		{	//alpha255標準？

			//減算っぽいしょり

			//アルファモードをノーマル固定設定
			setAlphaMode(pDevice, ALPHA_MODE_NORMAL);
			//ターゲットを背景に
			setRenderTarget(pDevice, SCREEN_BACKGROUND);
			//テクスチャ1をオフスクリーンに
			pDevice->SetTexture(0, m_pOffScreenTexture);
			//テクスチャ2をレイヤーに
			pDevice->SetTexture(1, m_pLayerTexture);

			// 頂点ＦＶＦを設定
			pDevice->SetFVF( VERTEX2D_COLORED::FVF );

			pDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
			pDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 0 );

			// 頂点色を乗算する
			pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
			pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			
			pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
			pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
			
			// さらにレイヤーを乗算する？
			pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
			pDevice->SetTextureStageState( 1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			pDevice->SetTextureStageState( 1, D3DTSS_ALPHAARG2, D3DTA_CURRENT );

			pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_MODULATE );
			pDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			pDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
			

			//http://msdn.microsoft.com/library/ja/default.asp?url=/library/ja/directx9_c/directx/graphics/reference/d3d/enums/d3dtexturestagestatetype.asp

			// ポリゴンを描画
			pDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, VxDataColor, sizeof(VERTEX2D_COLORED) );
			//テクスチャ2をNULLに
			pDevice->SetTexture(1, NULL);

		}

		*/

		//アルファモード設定
		setAlphaMode(pDevice, m_LayerMode);
		//ターゲットをバックに
		setRenderTarget(pDevice, SCREEN_BACKGROUND);
		//テクスチャをレイヤーに
		cRenderSetTexture(pDevice, 0, (m_pLayerTexture));

		// 頂点ＦＶＦを設定
		cRenderSetColoredVertexFormat(pDevice);

		// 頂点色を乗算する
		cRenderSetTextureColorMode(pDevice, 0, C_RENDER_TEXTURE_COLOR_MODULATE);

		// ポリゴンを描画
		cRenderDrawColoredTriangleStrip(pDevice, 2, VxDataColor);



		if(m_LayerMode == ALPHA_MODE_ANTIADDITION)
		{
			// テクスチャを設定
			cRenderSetTexture(pDevice, 0, NULL);
			
			g_GameEnv.m_Screen->setRenderTarget(pDevice, SCREEN_OFFSCREEN);
			
			// 頂点ＦＶＦを設定
			cRenderSetColoredVertexFormat(pDevice);


			setAlphaMode(pDevice, ALPHA_MODE_ADDITION);


			cRenderDrawColoredTriangleStrip(pDevice, 2, VxDataBlack);

		}


	}


	if(m_MiniEnable)
	{
		//保存
		m_MiniRotation = 0;
		/*
				//ビューポート設定(左上)
		vp.X = 0;
		vp.Y = 0;
		vp.Width = SCREEN_X/MINIPROPOTION +MINIPROPOTION;
		vp.Height = SCREEN_Y/MINIPROPOTION +MINIPROPOTION;
		vp.MinZ = 0.0f;
		vp.MaxZ = 1.0f;
		//ビューポートセット
		pDevice->SetViewport(&vp);
*/

		if (m_MiniBlur)
		{
			//ターゲットをミニレイヤーに
			cRenderSetRenderTarget(pDevice, 0, m_pMiniSurface[m_MiniRotation]);
			//テクスチャをNULLに
			cRenderSetTexture(pDevice, 0, NULL);
			// 頂点ＦＶＦを設定
			cRenderSetColoredVertexFormat(pDevice);

			//アルファ合成を通常に
			setAlphaMode(pDevice, ALPHA_MODE_NORMAL);

			// ポリゴンを描画
			cRenderDrawColoredTriangleStrip(pDevice, 2, MiniBlack);

			//テクスチャをオフスクリーンに
			cRenderSetTexture(pDevice, 0, (m_pOffScreenTexture));

		
			// 頂点ＦＶＦを設定
			cRenderSetTexturedVertexFormat(pDevice);

			//アルファ合成を加算に//加算縛り
			setAlphaMode(pDevice, ALPHA_MODE_ADDITION);

			// ポリゴンを描画
			cRenderDrawTriangleStrip(pDevice, 2, VxDataToMini);
			
		}
		else
		{
			//ターゲットをミニレイヤーに
			cRenderSetRenderTarget(pDevice, 0, m_pMiniSurface[m_MiniRotation]);
			
			//くりあ
			cRenderClearTarget(pDevice, cRenderColorARGB(255,0,0,0));

			//テクスチャをオフスクリーンに
			cRenderSetTexture(pDevice, 0, (m_pOffScreenTexture));

		
			// 頂点ＦＶＦを設定
			cRenderSetTexturedVertexFormat(pDevice);

			//アルファ合成を不可に
			cRenderSetAlphaBlendEnabled(pDevice, false);

			// ポリゴンを描画
			cRenderDrawTriangleStrip(pDevice, 2, VxDataToMini);
			
			//m_MiniRotation = !m_MiniRotation;//反転
		}
		
	}


	

	if(m_MiniEnable)
	{
/*
		//クリア
		pDevice->SetRenderTarget(0, m_pMiniSurface);
		pDevice->Clear( 0, NULL, D3DCLEAR_TARGET,//|D3DCLEAR_ZBUFFER,
			cRenderColorARGB(255,255,255,255), 1.0f, 0 );
*/
		m_MiniRotation = 0;

		cRenderSetViewport(pDevice, SCREEN_X, SCREEN_Y);
		SetScreenPointSampler(pDevice, 0);
		SetScreenPointSampler(pDevice, 1);

		//アルファモード設定
		setAlphaMode(pDevice, m_MiniMode);

		//ターゲットを背景に
		setRenderTarget(pDevice, SCREEN_BACKGROUND);
		//テクスチャをミニレイヤーに
		cRenderSetTexture(pDevice, 0, (m_pMiniTexture[m_MiniRotation]));

		// 頂点ＦＶＦを設定
		cRenderSetColoredVertexFormat(pDevice);

		// 頂点色を乗算する
		cRenderSetTextureColorMode(pDevice, 0, C_RENDER_TEXTURE_COLOR_MODULATE);

		// ポリゴンを描画
		cRenderDrawColoredTriangleStrip(pDevice, 2, VxDataMIniToBG);
	}

	//なぜかこれ入れないとばぐる
		//cRenderSetTexturedVertexFormat(pDevice);
}

void cScreen::Clear(cRenderDevice* pDevice, cColor& c)
{


	cRenderClearTarget(pDevice, c.D3Dcolor());


}

void cScreen::setRenderTarget(cRenderDevice* pDevice, int screen_ID)
{
	//ターゲットをオフスクリーンに
	if(screen_ID == SCREEN_OFFSCREEN)
		cRenderSetRenderTarget(pDevice, 0, m_pOffScreenSurface);

	//ターゲットをレイヤーに
	else if(screen_ID == SCREEN_LAYER)
		cRenderSetRenderTarget(pDevice, 0, m_pLayerSurface);

	//ターゲットをレイヤーに
	else if(screen_ID == SCREEN_BACKGROUND)
		cRenderSetRenderTarget(pDevice, 0, (m_pBackBufferSurface));

}

void cScreen::setAlphaMode(cRenderDevice* pDevice, int alpha_mode)
{
	cRenderSetAlphaBlendEnabled(pDevice, true);
	cRenderSetBlendOperation(pDevice, C_RENDER_BLEND_OPERATION_ADD);
	switch(alpha_mode)
	{
	case ALPHA_MODE_ADDITION:
		cRenderSetBlendFactors(pDevice, C_RENDER_BLEND_FACTORS_ADDITION);
		break;
	case ALPHA_MODE_ANTIADDITION:
		cRenderSetBlendFactors(pDevice, C_RENDER_BLEND_FACTORS_ANTIADDITION);
		break;
	case ALPHA_MODE_NORMAL:
		cRenderSetBlendFactors(pDevice, C_RENDER_BLEND_FACTORS_NORMAL);
		break;
	case ALPHA_MODE_HALFADDITION:
		cRenderSetBlendFactors(pDevice, C_RENDER_BLEND_FACTORS_HALFADDITION);
		break;
	}
}
void cScreen::DebugDraw(cRenderDevice* pDevice)
{
	cRenderSetViewport(pDevice, SCREEN_X, SCREEN_Y);
	SetScreenPointSampler(pDevice, 0);
	SetScreenPointSampler(pDevice, 1);

	double h = Height/3;

	VERTEX2D VxData[] = {
			{ (float)(Right() - h), (float)   Top()	  , 0, 1, 0.0f, 0.0f },
			{ (float)Right()	  , (float)   Top()	  , 0, 1, 1.0f, 0.0f },
			{ (float)(Right() - h), (float)(Top() + h), 0, 1, 0.0f, 1.0f },
			{ (float)Right()	  , (float)(Top() + h), 0, 1, 1.0f, 1.0f }
		};

	VERTEX2D_COLORED VxData2[] = {
			{ (float)(Right() - h), (float)(Top() + h), 0, 1,m_LayerColor.D3Dcolor(), 0.0f, 0.0f },
			{ (float)Right()	  , (float)(Top() + h), 0, 1,m_LayerColor.D3Dcolor(), 1.0f, 0.0f },
			{ (float)(Right() - h), (float)(Top() + h*2), 0, 1,m_LayerColor.D3Dcolor(), 0.0f, 1.0f },
			{ (float)Right()	  , (float)(Top() + h*2), 0, 1,m_LayerColor.D3Dcolor(), 1.0f, 1.0f }
		};

	VERTEX2D_COLORED VxData3[] = {
			{ (float)(Right() - h), (float)(Top() + h*2), 0, 1,m_MiniColor.D3Dcolor(), 0.0f, 0.0f },
			{ (float)Right()	  , (float)(Top() + h*2), 0, 1,m_MiniColor.D3Dcolor(), 1.0f, 0.0f },
			{ (float)(Right() - h), (float)(Top() + h*3), 0, 1,m_MiniColor.D3Dcolor(), 0.0f, 1.0f },
			{ (float)Right()	  , (float)(Top() + h*3), 0, 1,m_MiniColor.D3Dcolor(), 1.0f, 1.0f }
		};

	//アルファ合成を不可に
	cRenderSetAlphaBlendEnabled(pDevice, false);
	// 頂点ＦＶＦを設定
	cRenderSetTexturedVertexFormat(pDevice);

	//ターゲットを背景に
	setRenderTarget(pDevice, SCREEN_BACKGROUND);

	cRenderSetTexture(pDevice, 0, (m_pOffScreenTexture));
	// ポリゴンを描画
	cRenderDrawTriangleStrip(pDevice, 2, VxData);

	// 頂点色を乗算する
	cRenderSetTextureAlphaCurrent(pDevice, 0);
	// 頂点ＦＶＦを設定
	cRenderSetColoredVertexFormat(pDevice);

	cRenderSetTexture(pDevice, 0, (m_pLayerTexture));
	// ポリゴンを描画
	cRenderDrawColoredTriangleStrip(pDevice, 2, VxData2);

	cRenderSetTexture(pDevice, 0, (m_pMiniTexture[m_MiniRotation]));
	// ポリゴンを描画
	cRenderDrawColoredTriangleStrip(pDevice, 2, VxData3);


}
