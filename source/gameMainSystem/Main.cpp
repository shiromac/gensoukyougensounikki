
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
#include <string.h>
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#else
#include <windows.h>
#include <mmsystem.h>
#endif





//=============================================================================
// LIBRARY
//=============================================================================
#ifndef __EMSCRIPTEN__
#pragma comment ( lib, "winmm.lib" )
#pragma comment ( lib, "d3d9.lib" )
#ifdef _DEBUG
	#pragma comment ( lib, "d3dx9.lib" )
#else
	#pragma comment ( lib, "d3dx9.lib" )
#endif
#endif


//=============================================================================
// DEFINE
//=============================================================================
#ifdef __EMSCRIPTEN__
#define ERR_BOX(MSG)		((void)0)
#else
#define ERR_BOX(MSG)		::MessageBox( App.hWindow, MSG, _T("ERROR"), MB_OK|MB_ICONERROR )
#endif


//=============================================================================
// STRUCT
//=============================================================================
struct APPLICATION
{
	HWND					hWindow;			///< ウィンドウハンドル
#ifndef __EMSCRIPTEN__
	WNDCLASS				WndClass;			///< ウィンドウクラス
	MSG						Msg;				///< メッセージ
#endif
	eBootMode				BootMode;			///< デバイス起動モード
	cRenderInterface			*pDirect3D;			///< Direct3Dオブジェクト
	cRenderDevice			*pDevice;			///< Direct3Dデバイス
	cRenderDisplayMode		DisplayMode;		///< ディスプレイモード
	cRenderPresentParameters	PresentParameter;	///< プレゼンテーションパラメーター

	unsigned long			OldTime;			///< 前のフレームの時間
	unsigned long			NowTime;			///< 今のフレームの時間
	float					fFrameTime;			///< フレーム差分時間
	
	float					FPSFrame;			///< FPS計測用
	unsigned long			FPSCount;			///< FPSカウント
/*	unsigned long			DrawFPSCount;		///< 描画FPSカウント
*/
	TimeControl				timeCr;

	BOOL					IsActive;			///< アクティブか否かのフラグ

#ifndef __EMSCRIPTEN__
	_TCHAR					CurrentDir[512];	///< カレントディレクトリ

	HANDLE					hMutex;				///< ミューテックス
#endif
};


//=============================================================================
// VARIABLE
//=============================================================================
static APPLICATION App;


//=============================================================================
// PROTOTYPE
//=============================================================================
#ifndef __EMSCRIPTEN__
static LRESULT CALLBACK WinProc( HWND hWnd, UINT msg, UINT wParam, LONG lParam );
static bool WindowCreate( HINSTANCE hInst );
#endif
static bool Direct3DInitialize( void );
static void Direct3DFinalize( void );
static bool Direct3DScreenRefresh( void );

static bool GameInitialize( void );
static void GameFinalize( void );
static void GameSetupFrameTimer( void );
static bool GameFrame( void );

#ifndef __EMSCRIPTEN__
static BOOL InitMutex( void );
static BOOL ExitMutex( void );
#else
static void BrowserPrepareSaveFs( void );
static int BrowserIsSaveFsReady( void );
static bool BrowserInitializeGame( void );
static void BrowserShutdownGame( void );
static void BrowserMainLoop( void );
#endif
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
#ifndef __EMSCRIPTEN__
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

	if ( !GameInitialize() ) goto EXIT;
	GameSetupFrameTimer();
	
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
				cPlatformSleepMilliseconds(100);
			}
			else
			*/
			{
				
				if ( !GameFrame() )
				{
					::PostMessage( App.hWindow, WM_CLOSE, 0, 0 );
				}
			}
		}
	}

EXIT:
	//---------------------------------------------------
	// ユーザーデータ開放
	//---------------------------------------------------
	GameFinalize();

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
#else
static bool BrowserGameStarted = false;
static bool BrowserDirect3DStarted = false;
static bool BrowserGameInitialized = false;

static void BrowserPrepareSaveFs( void )
{
	EM_ASM({
		if (typeof FS === 'undefined' || typeof IDBFS === 'undefined') {
			Module['ggnSaveReady'] = 1;
			return;
		}

		try {
			FS.mkdir('/save');
		} catch (e) {
		}

		try {
			if (!Module['ggnSaveMounted']) {
				FS.mount(IDBFS, {}, '/save');
				Module['ggnSaveMounted'] = 1;
			}
		} catch (e) {
			if (typeof console !== 'undefined') console.error('save mount failed', e);
			Module['ggnSaveReady'] = 1;
			return;
		}

		Module['ggnSaveReady'] = 0;
		FS.syncfs(true, function(err) {
			if (err && typeof console !== 'undefined') console.error('save load failed', err);
			Module['ggnSaveReady'] = 1;
		});
	});
}

static int BrowserIsSaveFsReady( void )
{
	return EM_ASM_INT({
		return Module['ggnSaveReady'] ? 1 : 0;
	});
}

static bool BrowserInitializeGame( void )
{
	if ( !Direct3DInitialize() ) return false;
	BrowserDirect3DStarted = true;

	if ( !GameInitialize() )
	{
		Direct3DFinalize();
		BrowserDirect3DStarted = false;
		return false;
	}
	BrowserGameInitialized = true;
	GameSetupFrameTimer();
	return true;
}

static void BrowserShutdownGame( void )
{
	if ( BrowserGameInitialized )
	{
		GameFinalize();
		BrowserGameInitialized = false;
	}
	if ( BrowserDirect3DStarted )
	{
		Direct3DFinalize();
		BrowserDirect3DStarted = false;
	}
}

static void BrowserMainLoop( void )
{
	if ( !BrowserGameStarted )
	{
		if ( !BrowserIsSaveFsReady() ) return;
		if ( !BrowserInitializeGame() )
		{
			BrowserShutdownGame();
			emscripten_cancel_main_loop();
			return;
		}
		BrowserGameStarted = true;
	}

	if ( !GameFrame() )
	{
		BrowserShutdownGame();
		emscripten_cancel_main_loop();
	}
}

int main( int argc, char** argv )
{
	(void)argc;
	(void)argv;

	memset( &App, 0x00, sizeof(APPLICATION) );

	BrowserPrepareSaveFs();
	emscripten_set_main_loop( BrowserMainLoop, 0, 1 );
	return 0;
}
#endif

static bool GameInitialize( void )
{
	g_GameEnv.init(App.pDevice);
	g_GameEnv.m_Screen->initScreen(App.pDevice);
	g_GameEnv.m_Input.InitInput(App.hWindow);
	g_GameEnv.m_SoundManager.Init(App.hWindow);

	return SceneInitialize( App.pDevice );
}

static void GameFinalize( void )
{
	SceneFinalize();
}

static void GameSetupFrameTimer( void )
{
	App.NowTime = cPlatformGetMilliseconds();
	App.FPSCount = 0;
	App.OldTime = cPlatformGetMilliseconds();

	App.timeCr.OnFrameSkip(true);
	App.timeCr.SetFPS(60);
	App.timeCr.SetmaxSkip(3);
}

static bool GameFrame( void )
{
	App.NowTime = cPlatformGetMilliseconds();
	App.fFrameTime = ((float)(App.NowTime - App.OldTime)) * 0.001f;
	App.OldTime = App.NowTime;

	App.timeCr.TimeRegular();

	g_GameEnv.m_Input.UpdateInput();
	g_GameEnv.m_SceneManage->SceneCheck(App.pDevice);
	g_GameEnv.m_SceneManage->m_NowScene->SceneProcess(App.pDevice);

	if(App.timeCr.GetDrawFlag())
	{
		if ( Direct3DScreenRefresh() )
		{
			SceneRender( App.pDevice, App.fFrameTime );
		}
		else
		{
			return false;
		}

		if(g_GameEnv.AppEnd) return false;
	}

	App.FPSCount++;
	App.FPSFrame += App.fFrameTime;
	if ( App.FPSFrame >= 1.0f )
	{
#if defined(_UNRELEASE) && !defined(__EMSCRIPTEN__)
		_TCHAR Temp[64] = _T("");
		wsprintf( Temp, _T("%s [%ld/%ld/S]"), APPLICATION_NAME, App.timeCr.GetFrameRate(), App.FPSCount);
		::SetWindowText( App.hWindow, Temp );
#endif
		App.FPSFrame = 0.0f;
		App.FPSCount = 0;
	}

	return true;
}
#ifndef __EMSCRIPTEN__
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
			cPlatformBeginTimerPeriod(1);
		}
		return 0;
	//-------------------------------------------------
	// ウィンドウクローズ
	//-------------------------------------------------
	case WM_CLOSE:
		{
			cPlatformEndTimerPeriod(1);
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

#endif


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
#if defined(_UNRELEASE) || defined(__EMSCRIPTEN__)
#else
	if( MessageBox( App.hWindow, _T("FullScreen\nフルスクリーンで起動しますか？"), _T("FullScreen"), MB_YESNO | MB_DEFBUTTON2 | MB_ICONQUESTION) == IDYES )
	{
		Fullscreen = true;
	}
#endif

	cRenderResult hr;

	//---------------------------------------------------
	// Direct3D オブジェクトを作成
	//---------------------------------------------------
	App.pDirect3D = cRenderCreateInterface();
	if ( App.pDirect3D == NULL )
	{
		ERR_BOX( _T("Direct3Dの作成に失敗しました") );
		return false;
	}

	//---------------------------------------------------
	// 現在の画面モードを取得
	//---------------------------------------------------
	hr = cRenderGetAdapterDisplayMode(App.pDirect3D, &App.DisplayMode);
	if( cRenderFailed(hr) )
	{
		ERR_BOX( _T("画面モードの取得に失敗しました") );
		return false;
	}

	//---------------------------------------------------
	// 性能チェック
	//---------------------------------------------------
	cRenderCaps Caps;
	hr = cRenderGetDeviceCaps(App.pDirect3D, &Caps);
	if( cRenderFailed(hr) )
	{
		ERR_BOX( _T("デバイスの性能が取得できませんでした") );
		return false;
	}

	//---------------------------------------------------
	// Direct3D 初期化パラメータの設定
	//---------------------------------------------------
	cRenderInitializePresentParameters(&App.PresentParameter, App.hWindow, Fullscreen, App.DisplayMode, SCREEN_X, SCREEN_Y);

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
		if( !cRenderCreateReferenceSoftwareDevice(App.pDirect3D, App.hWindow, &App.PresentParameter, &App.pDevice) )
		{
			ERR_BOX( _T("Direct3Dデバイスの生成に失敗しました") );
			return false;
		}
		break;
	//------------------------------------------------------- SoftwareTnLデバイスの生成
	case BOOT_MODE_SOFTWARE:
		// SOFTWARE HAL
		if( !cRenderCreateHalSoftwareDevice(App.pDirect3D, App.hWindow, &App.PresentParameter, &App.pDevice) )
		{
			// REFERENCE RASTERIZE
			if( !cRenderCreateReferenceSoftwareDevice(App.pDirect3D, App.hWindow, &App.PresentParameter, &App.pDevice) )
			{
				ERR_BOX( _T("Direct3Dデバイスの生成に失敗しました") );
				return false;
			}
		}
		break;
	//------------------------------------------------------- HardwareTnLデバイスの生成
	case BOOT_MODE_HARDWARE:
		// HARDWARE T&L
		if( !cRenderCreateHalHardwareDevice(App.pDirect3D, App.hWindow, &App.PresentParameter, &App.pDevice) )
		{
			// SOFTWARE HAL
			if( !cRenderCreateHalSoftwareDevice(App.pDirect3D, App.hWindow, &App.PresentParameter, &App.pDevice) )
			{
				// REFERENCE RASTERIZE
				if( !cRenderCreateReferenceSoftwareDevice(App.pDirect3D, App.hWindow, &App.PresentParameter, &App.pDevice) )
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
		if( !cRenderCreateHalMixedDevice(App.pDirect3D, App.hWindow, &App.PresentParameter, &App.pDevice) )
		{
			// SOFTWARE HAL
			if( !cRenderCreateHalSoftwareDevice(App.pDirect3D, App.hWindow, &App.PresentParameter, &App.pDevice) )
			{
				// REFERENCE RASTERIZE
				if( !cRenderCreateReferenceSoftwareDevice(App.pDirect3D, App.hWindow, &App.PresentParameter, &App.pDevice) )
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
	cRenderClearTarget(App.pDevice, cRenderColorRGB(128,128,192));

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
		cRenderRelease(App.pDevice);
	}

	// オブジェクト開放
	if ( App.pDirect3D != NULL )
	{
		cRenderRelease(App.pDirect3D);
	}
}


//=============================================================================
/**	Direct3D画面更新<BR>
	画面の更新処理を行います。

*/
//=============================================================================
bool Direct3DScreenRefresh( void )
{
	cRenderResult presentResult = cRenderPresent(App.pDevice);

	if(cRenderIsDriverInternalError(presentResult))
	{
		return false;
	}

	if(cRenderIsDeviceLost(presentResult))
	{
#ifndef __EMSCRIPTEN__
		while ( ::PeekMessage( &App.Msg, 0, 0, 0, PM_NOREMOVE ) )
		{
			if(!(GetMessage( &App.Msg, 0, 0, 0 ))) return false;
			::TranslateMessage( &App.Msg );
			::DispatchMessage( &App.Msg );
		}
#endif

		cRenderResult deviceState = cRenderTestCooperativeLevel(App.pDevice);
		if(cRenderIsDeviceLost(deviceState))
		{
			cPlatformSleepMilliseconds(100);
		}
		else if(cRenderIsDeviceNotReset(deviceState))
		{
			if ( !SceneReset( App.pDevice ) )
			{
				return false;
			}

			cRenderResult resetResult = cRenderResetDevice(App.pDevice, &App.PresentParameter);
			if(cRenderIsDeviceLost(resetResult))
			{
				return false;
			}
			if(cRenderFailed(resetResult))
			{
				return false;
			}

			SceneRestore( App.pDevice );
		}
	}

	return true;
}
