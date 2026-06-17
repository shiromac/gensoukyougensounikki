
//=============================================================================
/** @file
    @brief		アプリーケーション
    @author		Noriyuki Lee
    @since		2003.10.24
*/
//=============================================================================

#ifndef ___WINMAIN_H___
#define ___WINMAIN_H___


#include "../resource.h"
//=============================================================================
// DEFINE
//=============================================================================
#define APPLICATION_NAME		_T("ggn")	///< アプリケーション名
#define SCREEN_X				(800)						///< 画面横サイズ
#define SCREEN_Y				(600)						///< 画面縦サイズ

#define MINIMUM_FPS				(50)						///< FPSがこの値以下にならないようにする

#ifdef __EMSCRIPTEN__
#define ERROR_RETURN(str)		{ return false; }
#else
#define ERROR_RETURN(str)		{ ::MessageBox( NULL, str, _T("Error"), MB_OK|MB_ICONERROR ); return false; }
#endif


//=============================================================================
// ENUM
//=============================================================================
enum eBootMode
{
	BOOT_MODE_MIXED,
	BOOT_MODE_HARDWARE,
	BOOT_MODE_SOFTWARE,
	BOOT_MODE_REF,
};


#endif // ___WINMAIN_H___

