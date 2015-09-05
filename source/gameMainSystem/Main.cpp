
//=============================================================================
/** @file
    @brief		アプリーケーション
    @author		Noriyuki Lee
    @since		2003.10.24
*/
//=============================================================================


//=============================================================================
// INCLUDE
//=============================================================================
#include "../stdafx.h"


#include <stdio.h>
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <mmsystem.h>





//=============================================================================
// LIBRARY
//=============================================================================
#pragma comment ( lib, "winmm.lib" )
#pragma comment ( lib, "d3d9.lib" )
#ifdef _DEBUG
	#pragma comment ( lib, "d3dx9.lib" )
#else
	#pragma comment ( lib, "d3dx9.lib" )
#endif


//=============================================================================
// DEFINE
//=============================================================================
#define ERR_BOX(MSG)		::MessageBox( App.hWindow, MSG, _T("ERROR"), MB_OK|MB_ICONERROR )


//=============================================================================
// STRUCT
//=============================================================================
struct APPLICATION
{
	HWND					hWindow;			///< ウィンドウハンドル
	WNDCLASS				WndClass;			///< ウィンドウクラス
	MSG						Msg;				///< メッセージ
	eBootMode				BootMode;			///< デバイス起動モード
	IDirect3D9				*pDirect3D;			///< Direct3Dオブジェクト
	IDirect3DDevice9		*pDevice;			///< Direct3Dデバイス
	D3DDISPLAYMODE			DisplayMode;		///< ディスプレイモード
	D3DPRESENT_PARAMETERS	PresentParameter;	///< プレゼンテーションパラメーター

	unsigned long			OldTime;			///< 前のフレームの時間
	unsigned long			NowTime;			///< 今のフレームの時間
	float					fFrameTime;			///< フレーム差分時間
	
	float					FPSFrame;			///< FPS計測用
	unsigned long			FPSCount;			///< FPSカウント
/*	unsigned long			DrawFPSCount;		///< 描画FPSカウント
*/
	TimeControl				timeCr;

	BOOL					IsActive;			///< アクティブか否かのフラグ

	_TCHAR					CurrentDir[512];	///< カレントディレクトリ

	HANDLE					hMutex;				///< ミューテックス
};


//=============================================================================
// VARIABLE
//=============================================================================
static APPLICATION App;


//=============================================================================
// PROTOTYPE
//=============================================================================
static LRESULT CALLBACK WinProc( HWND hWnd, UINT msg, UINT wParam, LONG lParam );
static bool WindowCreate( HINSTANCE hInst );
static bool Direct3DInitialize( void );
static void Direct3DFinalize( void );
static bool Direct3DScreenRefresh( void );

static BOOL InitMutex( void );
static BOOL ExitMutex( void );

//=============================================================================
/**	ウィンドウズメイン関数<BR>
	一般的なコンソールアプリでのmain()関数のWindows版です。

	@param hInst	[in] インスタンスのハンドル
	@param hPrev	[in] Win3.1時代の名残（NULL)
	@param Cmd		[in] コマンドラインが格納された文字列へのポインタ
	@param Show		[in] ウィンドウをどのように表示するかの指定

	@return		終了コード
*/
//=============================================================================
int WINAPI WinMain( HINSTANCE hInst, HINSTANCE prev, LPSTR cmd, int show )
{
	#ifdef _DEBUG
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	
	//---------------------------------------------------
	// 初期化
	//---------------------------------------------------
	::FillMemory( &App, sizeof(APPLICATION), 0x00 );

	//---------------------------------------------------
	// カレントディレクトリ設定
	//---------------------------------------------------
	::GetModuleFileName( hInst, (App.CurrentDir), sizeof(App.CurrentDir) );
	long i = (long)_tcslen( App.CurrentDir );
	while ( App.CurrentDir[i] != _T('\\') ) if ( --i < 0 ) break;
	App.CurrentDir[i+1] = _T('\0');
	::SetCurrentDirectory( App.CurrentDir );
	
	//---------------------------------------------------
	// 多重起動チェック
	//---------------------------------------------------
	if( InitMutex() )
	{
		ExitMutex();
		return (int)App.Msg.wParam;
	}

	//---------------------------------------------------
	// ウィンドウ作成
	//---------------------------------------------------
	if ( !WindowCreate( hInst ) ) return 0;

	//---------------------------------------------------
	// Direct3D初期化
	//---------------------------------------------------
	if ( !Direct3DInitialize() ) goto EXIT;

	//---------------------------------------------------
	// 環境クラス初期化
	//---------------------------------------------------
	g_GameEnv.init(App.pDevice);

	//---------------------------------------------------
	// スクリーンクラス初期化
	//---------------------------------------------------
	g_GameEnv.m_Screen->initScreen(App.pDevice);

	//---------------------------------------------------
	// インプットクラス初期化
	//---------------------------------------------------
	g_GameEnv.m_Input.InitInput(App.hWindow);

	//---------------------------------------------------
	// サウンドクラス初期化
	//---------------------------------------------------
	g_GameEnv.m_SoundManager.Init(App.hWindow);

	//---------------------------------------------------
	// ユーザー初期化処理
	//---------------------------------------------------
	if ( !SceneInitialize( App.pDevice ) ) goto EXIT;

	/*
	//---------------------------------------------------
	// フレーム時間初期化
	//---------------------------------------------------
	*/
	App.NowTime = ::timeGetTime();
	App.FPSCount = 0;
	App.OldTime = ::timeGetTime();

	App.timeCr.OnFrameSkip(true);
	App.timeCr.SetFPS(60);
	App.timeCr.SetmaxSkip(3);
	
	//---------------------------------------------------
	// メインループ
	//---------------------------------------------------
	for ( ; ; )
	{
		//--------------------------------------------
		// メッセージ処理
		//--------------------------------------------
		if ( ::PeekMessage( &App.Msg, 0, 0, 0, PM_REMOVE ) )
		{
			if ( App.Msg.message == WM_QUIT ) break;
			::TranslateMessage( &App.Msg );
			::DispatchMessage( &App.Msg );

			//OutputDebugString(_T("メッセージ\n"));
		}
		//--------------------------------------------
		// 他
		//--------------------------------------------
		else
		{
			/*
			if ( !App.IsActive )
			{
				::Sleep( 100 );
			}
			else
			*/
			{
				
				static const float FrameTimeLimit = 1.0f / (float)MINIMUM_FPS;

				//-----------------------------------
				// フレーム時間取得
				//-----------------------------------

				App.NowTime = ::timeGetTime();
				
				//App.SaveTime += (App.OldTime + 1000/MINIMUM_FPS) - App.NowTime;//理想-現実=余り時間
				
				App.fFrameTime = ((float)(App.NowTime - App.OldTime)) * 0.001f;
				App.OldTime = App.NowTime;
				/*
				//-----------------------------------
				// 一定値以上なら描画を飛ばす
				//-----------------------------------
				//if ( App.fFrameTime <= FrameTimeLimit )
				//{
				if (App.SaveTime >= 0)//貯金あり
				{
					//-----------------------------------
					// 画面更新
					//-----------------------------------
					if ( Direct3DScreenRefresh() )
					{
						// シーンの描画
						SceneRender( App.pDevice, App.fFrameTime );
						App.DrawFPSCount++;
					}
					else
					{
						// 糸冬
						::PostMessage( App.hWindow, WM_CLOSE, 0, 0 );
					}
				}
				*/
				App.timeCr.TimeRegular();       //フレーム制御

				//-----------------------------------
				// インプット更新
				//-----------------------------------
				g_GameEnv.m_Input.UpdateInput();


				g_GameEnv.m_SceneManage->SceneCheck(App.pDevice);
				//ここでシーン本実行
				g_GameEnv.m_SceneManage->m_NowScene->SceneProcess(App.pDevice);



				if(App.timeCr.GetDrawFlag())   //描画命令
				{
					//-----------------------------------
					// 画面更新
					//-----------------------------------
					if ( Direct3DScreenRefresh() )
					{
						// シーンの描画
						SceneRender( App.pDevice, App.fFrameTime );
						
					}
					else
					{
						// 糸冬
						::PostMessage( App.hWindow, WM_CLOSE, 0, 0 );
					}
					
					if(g_GameEnv.AppEnd)::PostMessage( App.hWindow, WM_CLOSE, 0, 0 );
					 
				}
				/*
				//-----------------------------------
				// ダミーでもいいからちと眠らす
				//-----------------------------------
				//if (App.SaveTime > 1000/MINIMUM_FPS)//貯金ありすぎ
				{
					unsigned long tempNowTime = ::timeGetTime();
					unsigned long tempOldTime = ::timeGetTime();
					for(;App.SaveTime > 1000/MINIMUM_FPS;)
					{
						::Sleep( 1 );
						App.SaveTime -= (tempNowTime = ::timeGetTime()) - (tempOldTime);
						tempOldTime = ::timeGetTime();
					}
				}

				*/
				//-----------------------------------
				// FPS加算
				//-----------------------------------
				App.FPSCount++;
				
				//-----------------------------------
				// FPS算出用時間判定
				//-----------------------------------
				App.FPSFrame += App.fFrameTime;
				if ( App.FPSFrame >= 1.0f )
				{
#ifdef _UNRELEASE

					_TCHAR Temp[64] = _T("");
					//_stprintf( Temp, _T("%s [%ld/%ld/S]"), APPLICATION_NAME, App.DrawFPSCount, App.FPSCount);
					wsprintf( Temp, _T("%s [%ld/%ld/S]"), APPLICATION_NAME, App.timeCr.GetFrameRate(), App.FPSCount);
					
					::SetWindowText( App.hWindow, Temp );
#endif
					
					// データクリア
					App.FPSFrame = 0.0f;
					App.FPSCount = 0;
					//App.DrawFPSCount = 0;
				}
			}
		}
	}

EXIT:
	//---------------------------------------------------
	// ユーザーデータ開放
	//---------------------------------------------------
	SceneFinalize();

	//---------------------------------------------------
	// Direct3Dの開放
	//---------------------------------------------------
	Direct3DFinalize();
//_CrtDumpMemoryLeaks();

	//---------------------------------------------------
	// Mutexの開放
	//---------------------------------------------------
	ExitMutex();

	//---------------------------------------------------
	// おしまい
	//---------------------------------------------------
	return (int)App.Msg.wParam;
}

BOOL InitMutex()
{
	App.hMutex = ::CreateMutex(NULL, FALSE, APPLICATION_NAME);
	if(::GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL, _T("二重起動は許されません。"), APPLICATION_NAME, MB_ICONERROR | MB_TOPMOST | MB_OK);
		return TRUE;
	}
	return FALSE;
}
 
BOOL ExitMutex()
{
	::ReleaseMutex(App.hMutex);
	::CloseHandle(App.hMutex);
	return FALSE;
} 

//=============================================================================
/**	ウィンドウズメッセージ関数<BR>
	ウィンドウのイベントメッセージを受け取るための関数です。

	@param hWnd			[in] メッセージを発行したウィンドウハンドル
	@param msg			[in] メッセージ
	@param wParam		[in] メッセージ用パラメーター
	@param lParam		[in] メッセージ用パラメーター

	@return		終了コード
*/
//=============================================================================
LRESULT CALLBACK WinProc( HWND hWnd, UINT msg, UINT wParam, LONG lParam )
{
	switch( msg )
	{
	//-------------------------------------------------
	// ウィンドウフォーカス移動処理
	//-------------------------------------------------
	case WM_ACTIVATEAPP:
		{
			App.IsActive = (wParam != 0);
		}
		return 0;
	//-------------------------------------------------
	// キー処理
	//-------------------------------------------------
	case WM_KEYDOWN:
		{
			switch ( wParam )
			{
			//--------------------------- エスケープで終了
			case VK_ESCAPE:
				
#ifdef _UNRELEASE

				/*
				if( MessageBox( hWnd, _T("Quit Game\nゲームを終了しますか？"), _T("Quit"), MB_YESNO ) == IDNO )
				{
					return 0; // 閉じないためにはDefWindowProc()に処理させてはならない
				}
				*/
                ::SendMessage( hWnd, WM_CLOSE, 0, 0 );
#endif
				
				break;
			}
		}
		return 0;
	//-------------------------------------------------
	// ウィンドウ生成
	//-------------------------------------------------
	case WM_CREATE:
		{
			::timeBeginPeriod( 1 );
		}
		return 0;
	//-------------------------------------------------
	// ウィンドウクローズ
	//-------------------------------------------------
	case WM_CLOSE:
		{
			::timeEndPeriod( 1 );
			::SendMessage( hWnd, WM_DESTROY, 0, 0 );
		}
		return 0;
	//-------------------------------------------------
	// 終了時
	//-------------------------------------------------
	case WM_DESTROY:
		{
			::PostQuitMessage( 0 );
		}
		return 0;
	}

	return ::DefWindowProc( hWnd, msg, wParam, lParam );
}


//=============================================================================
/**	ウィンドウ作成<BR>
	ウィンドウの作成を行います。

	@param hInst	[in] インスタンスハンドル

	@retval			true : 成功
	@retval			false : 失敗
*/
//=============================================================================
bool WindowCreate( HINSTANCE hInst )
{
	//---------------------------------------------------
	// ウィンドウクラス登録
	//---------------------------------------------------
	App.WndClass.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	App.WndClass.lpfnWndProc	= WinProc; 
	App.WndClass.cbClsExtra		= 0;
	App.WndClass.cbWndExtra		= 0;
	App.WndClass.hInstance		= hInst;
	App.WndClass.hIcon			= NULL;
	App.WndClass.hCursor		= ::LoadCursor( NULL, IDC_ARROW );
	App.WndClass.hbrBackground	= (HBRUSH)::GetStockObject( BLACK_BRUSH );
	App.WndClass.lpszMenuName	= NULL;
	App.WndClass.lpszClassName	= APPLICATION_NAME;

	if ( ::RegisterClass( &App.WndClass ) == NULL ) return false;

	//---------------------------------------------------
	// ウィンドウサイズ取得
	//---------------------------------------------------
	long WinX = SCREEN_X + ::GetSystemMetrics(SM_CXEDGE) + ::GetSystemMetrics(SM_CXBORDER) + ::GetSystemMetrics(SM_CXDLGFRAME);
	long WinY = SCREEN_Y + ::GetSystemMetrics(SM_CYEDGE) + ::GetSystemMetrics(SM_CYBORDER) + ::GetSystemMetrics(SM_CYDLGFRAME) + ::GetSystemMetrics(SM_CYCAPTION);

	//---------------------------------------------------
	// ウィンドウ生成
	//---------------------------------------------------
	App.hWindow = ::CreateWindowEx(
							0,
							APPLICATION_NAME,
							APPLICATION_NAME,
							WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
							(::GetSystemMetrics(SM_CXSCREEN) - WinX) / 2,
							(::GetSystemMetrics(SM_CYSCREEN) - WinY) / 2,
							WinX,
							WinY,
							NULL,
							NULL,
							hInst,
							NULL );

	if ( App.hWindow == NULL ) return false;

	return true;
}


//=============================================================================
/**	Direct3D初期化<BR>
	Direct3Dの初期化を行います。

	@retval			true : 成功
	@retval			false : 失敗
*/
//=============================================================================
bool Direct3DInitialize( void )
{
	bool Fullscreen = false;
#ifdef _UNRELEASE
#else
	if( MessageBox( App.hWindow, _T("FullScreen\nフルスクリーンで起動しますか？"), _T("FullScreen"), MB_YESNO | MB_DEFBUTTON2 | MB_ICONQUESTION) == IDYES )
	{
		Fullscreen = true;
	}
#endif

	HRESULT hr;

	//---------------------------------------------------
	// Direct3D オブジェクトを作成
	//---------------------------------------------------
	App.pDirect3D = Direct3DCreate9( D3D_SDK_VERSION );
	if ( App.pDirect3D == NULL )
	{
		ERR_BOX( _T("Direct3Dの作成に失敗しました") );
		return false;
	}

	//---------------------------------------------------
	// 現在の画面モードを取得
	//---------------------------------------------------
	hr = App.pDirect3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &App.DisplayMode );
	if FAILED( hr )
	{
		ERR_BOX( _T("画面モードの取得に失敗しました") );
		return false;
	}

	//---------------------------------------------------
	// 性能チェック
	//---------------------------------------------------
	D3DCAPS9 Caps;
	hr = App.pDirect3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &Caps );
	if FAILED( hr )
	{
		ERR_BOX( _T("デバイスの性能が取得できませんでした") );
		return false;
	}

	//---------------------------------------------------
	// Direct3D 初期化パラメータの設定
	//---------------------------------------------------
	ZeroMemory( &App.PresentParameter, sizeof(D3DPRESENT_PARAMETERS) );

	// 画面情報
	App.PresentParameter.BackBufferCount			= 1;
	//App.PresentParameter.Flags						= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	App.PresentParameter.Flags						= 0;
	App.PresentParameter.SwapEffect					= D3DSWAPEFFECT_DISCARD;
	App.PresentParameter.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
	App.PresentParameter.PresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;
	App.PresentParameter.MultiSampleType			= D3DMULTISAMPLE_NONE;
	App.PresentParameter.MultiSampleQuality			= D3DMULTISAMPLE_NONE;

	// ウインドウ : 現在の画面モードを使用
	App.PresentParameter.hDeviceWindow				= App.hWindow;
	App.PresentParameter.Windowed					= !Fullscreen;
	App.PresentParameter.BackBufferWidth			= SCREEN_X;
	App.PresentParameter.BackBufferHeight			= SCREEN_Y;
	App.PresentParameter.BackBufferFormat			= App.DisplayMode.Format;

	// Ｚバッファの自動作成
	App.PresentParameter.EnableAutoDepthStencil		= FALSE;
	App.PresentParameter.AutoDepthStencilFormat		= D3DFMT_D16;

	//---------------------------------------------------
	// 性能に応じて処理を行う
	//---------------------------------------------------
	App.BootMode = SceneCheck( &Caps );

	//---------------------------------------------------
	// デバイスの生成
	//---------------------------------------------------
	switch ( App.BootMode )
	{
	//------------------------------------------------------- Refデバイスの生成
	case BOOT_MODE_REF:
		// REFERENCE RASTERIZE
		if FAILED( App.pDirect3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, App.hWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &App.PresentParameter, &App.pDevice ) )
		{
			ERR_BOX( _T("Direct3Dデバイスの生成に失敗しました") );
			return false;
		}
		break;
	//------------------------------------------------------- SoftwareTnLデバイスの生成
	case BOOT_MODE_SOFTWARE:
		// SOFTWARE HAL
		if FAILED( App.pDirect3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, App.hWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &App.PresentParameter, &App.pDevice ) )
		{
			// REFERENCE RASTERIZE
			if FAILED( App.pDirect3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, App.hWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &App.PresentParameter, &App.pDevice ) )
			{
				ERR_BOX( _T("Direct3Dデバイスの生成に失敗しました") );
				return false;
			}
		}
		break;
	//------------------------------------------------------- HardwareTnLデバイスの生成
	case BOOT_MODE_HARDWARE:
		// HARDWARE T&L
		if FAILED( App.pDirect3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, App.hWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, &App.PresentParameter, &App.pDevice ) )
		{
			// SOFTWARE HAL
			if FAILED( App.pDirect3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, App.hWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &App.PresentParameter, &App.pDevice ) )
			{
				// REFERENCE RASTERIZE
				if FAILED( App.pDirect3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, App.hWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &App.PresentParameter, &App.pDevice ) )
				{
					ERR_BOX( _T("Direct3Dデバイスの生成に失敗しました") );
					return false;
				}
			}
		}
		break;
	//------------------------------------------------------- SoftwareTnL and HardwareTnLデバイスの生成
	case BOOT_MODE_MIXED:
		// MIXED T&L
		if FAILED( App.pDirect3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, App.hWindow, D3DCREATE_MIXED_VERTEXPROCESSING, &App.PresentParameter, &App.pDevice ) )
		{
			// SOFTWARE HAL
			if FAILED( App.pDirect3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, App.hWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &App.PresentParameter, &App.pDevice ) )
			{
				// REFERENCE RASTERIZE
				if FAILED( App.pDirect3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, App.hWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &App.PresentParameter, &App.pDevice ) )
				{
					ERR_BOX( _T("Direct3Dデバイスの生成に失敗しました") );
					return false;
				}
			}
		}
		break;
	}

	//---------------------------------------------------
	// シーンクリア
	//---------------------------------------------------
	App.pDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(128,128,192), 0, 0 );

	return true;
}


//=============================================================================
/**	Direct3D開放<BR>
	Direct3Dの開放を行います。

*/
//=============================================================================
void Direct3DFinalize( void )
{
	// デバイス開放
	if ( App.pDevice != NULL )
	{
		App.pDevice->Release();
		App.pDevice = NULL;
	}

	// オブジェクト開放
	if ( App.pDirect3D != NULL )
	{
		App.pDirect3D->Release();
		App.pDirect3D = NULL;
	}
}


//=============================================================================
/**	Direct3D画面更新<BR>
	画面の更新処理を行います。

*/
//=============================================================================
bool Direct3DScreenRefresh( void )
{
	switch ( App.pDevice->Present( NULL, NULL, NULL, NULL ) )
	{
	//---------------------------- これ出たらもうダメポ
	case D3DERR_DRIVERINTERNALERROR:
		{
			return false;
		}
		break;

	//---------------------------- デバイス消失状態
	case D3DERR_DEVICELOST:
		{
			// １秒まっとけ
			//::Sleep( 1000 );
			while ( ::PeekMessage( &App.Msg, 0, 0, 0, PM_NOREMOVE ) )
			{
				if(!(GetMessage( &App.Msg, 0, 0, 0 ))) return false;
				::TranslateMessage( &App.Msg );
				::DispatchMessage( &App.Msg );
			}
			// デバイス状態チェック
			switch ( App.pDevice->TestCooperativeLevel() )
			{
			//---------------- デバイス消失
			case D3DERR_DEVICELOST:
				{
					::Sleep( 100 );
				}
				break;

			//---------------- デバイスリセットＯＫ
			case D3DERR_DEVICENOTRESET:
				{
					
					// シーンリセット
					if ( !SceneReset( App.pDevice ) )
					{
						// こけたら終わる
						return false;
					}

					// デバイスのリセット
					HRESULT hr = App.pDevice->Reset( &App.PresentParameter );
					if(hr == D3DERR_DEVICELOST)
					{
						return false;
					}
					if FAILED( hr )
					{
						// こけたら終わる
						return false;
					}

					// シーン復旧
					SceneRestore( App.pDevice );
					
				}
				break;
			}
		}
		break;
	}

	return true;
}

