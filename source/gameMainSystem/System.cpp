
//=============================================================================
/** @file
    @brief		シーン関係
    @author		Noriyuki Lee
    @since		2003.10.24
*/
//=============================================================================


//=============================================================================
// INCLUDE
//=============================================================================
#include "../stdafx.h"

//=============================================================================
// VARIABLE
//=============================================================================

//=============================================================================
// CONSTANT
//=============================================================================

//=============================================================================
/**	アダプター情報チェック<BR>
	アダプターの情報を調べて起動モードを選択します。

	@param pCaps	[in] Direct3Dアダプター情報

	@return		終了コード
*/
//=============================================================================
eBootMode SceneCheck( D3DCAPS9 *pCaps )
{
	return BOOT_MODE_HARDWARE;
}


//=============================================================================
/**	シーンの初期化<BR>
	シーンの初期化処理を行います。

	@param pDev		[in] Direct3Dデバイス

	@retval			true : 成功
	@retval			false : 失敗
*/
//=============================================================================
bool SceneInitialize( IDirect3DDevice9 *pDev )
{
	
	//リソースの確保の準備


	

	//--------------------------------------------
	//ここまで定型//というわけでもないので注意
	//--------------------------------------------


	
	return g_GameEnv.m_SceneManage->initialize(pDev);
}


//=============================================================================
/**	シーンの解放<BR>
	シーンの解放処理を行います。

*/
//=============================================================================
void SceneFinalize( void )
{
	//リソースの開放


	//RELEASE(g_GameEnv.m_Resourse);
	RELEASE(g_GameEnv.m_GlobalResourse);

	g_GameEnv.m_SceneManage->finalize();
	//--------------------------------------------
	//ここまで定型
	//--------------------------------------------





}


//=============================================================================
/**	シーンリセット処理<BR>
	シーンが破壊されたときのリセット処理を行います。

	@param pDev			[in] Direct3Dデバイス

	@retval			true : 成功
	@retval			false : 失敗
*/
//=============================================================================
bool SceneReset( IDirect3DDevice9 *pDev )
{
	g_GameEnv.ResetDevice(pDev);
	return true;
}


//=============================================================================
/**	シーンリストア<BR>
	デバイスが復旧されたあとのシーンの復旧をします。

	@param pDev			[in] Direct3Dデバイス

	@retval			true : 成功
	@retval			false : 失敗
*/
//=============================================================================
bool SceneRestore( IDirect3DDevice9 *pDev )
{
	
	g_GameEnv.m_Screen->initScreen(pDev);
	g_GameEnv.m_Screen->setAlphaMode(pDev,ALPHA_MODE_NORMAL);
	g_GameEnv.m_Screen->m_OffScreenEnable = false;
	g_GameEnv.m_Screen->m_LayerEnable = false;
	g_GameEnv.m_Screen->m_MiniEnable = false;

	g_GameEnv.m_Screen->setRenderTarget(pDev, SCREEN_BACKGROUND);

	if(sg_pDungeonSystem)
	{
		sg_pDungeonSystem->resetedDevice(pDev);
	}
	return true;
}


//=============================================================================
/**	シーンレンダリング<BR>
	シーンのレンダリング処理を行います。

	@param pDev			[in] Direct3Dデバイス
	@param fFrameTime	[in] 前フレームのフレーム時間

	@retval			true : 成功
	@retval			false : 失敗
*/
//=============================================================================
bool SceneRender( IDirect3DDevice9 *pDev, float fFrameTime )
{

	//------------------------------------------------------
	// 画面クリア
	//------------------------------------------------------
	pDev->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(128,128,192), 0, 0 );

	
	//カリングしない
	pDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//------------------------------------------------------
	// シーン描画開始
	//------------------------------------------------------
	if SUCCEEDED( pDev->BeginScene() )
	{
		cColor c;

		/*
		if(g_GameEnv.m_Screen->m_OffScreenEnable)
		{
			c.ARGB(255,0,0,0);
			g_GameEnv.m_Screen->setRenderTarget(pDev, SCREEN_OFFSCREEN);	
			g_GameEnv.m_Screen->Clear(pDev, c);
		}
		*/

		/*
		if(g_GameEnv.m_Screen->m_LayerEnable
			&& g_GameEnv.m_Screen->m_LayerClear)
		{
			c.ARGB(255,0,0,0);
			g_GameEnv.m_Screen->setRenderTarget(pDev, SCREEN_LAYER);	
			g_GameEnv.m_Screen->Clear(pDev, c);
			g_GameEnv.m_Screen->m_LayerClear = false;
		}*/
		

		c.ARGB(255,0,0,0);	
		g_GameEnv.m_Screen->setRenderTarget(pDev, SCREEN_BACKGROUND);
		g_GameEnv.m_Screen->Clear(pDev, c);

		//シーンのドロー
		g_GameEnv.m_SceneManage->m_NowScene->SceneDraw(pDev);

		//画面へのドロー
		g_GameEnv.m_Screen->Draw(pDev);

		//デバグドロー
		//g_GameEnv.m_Screen->DebugDraw(pDev);
		// シーン描画終了

		//システムドロー
		g_GameEnv.m_SceneManage->m_NowScene->SceneSystemDraw(pDev);

		



		pDev->EndScene();
	}

	

	return true;
}


