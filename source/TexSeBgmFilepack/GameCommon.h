#pragma once

//#include "GameCommon.h"

//#################################################
// 全ファイルからの共通要素
//#################################################


#include <windows.h>
//#include "resource.h"
#include <tchar.h>

#define WINDOW_NAME		"TexSeBgm"	// ウィンドウ名
#define CLIENT_WIDTH	200					// クライアント領域のデフォルトの幅, フルスクリーンの場合は水平方向解像度
#define	CLIENT_HEIGHT	40					// クライアント領域のデフォルトの高さ, フルスクリーンの場合は垂直方向解像度



// ログを吐き出すか
#define LOGPUTOUT 1

// メッセージボックスを出すか
#define MESSAGE_BOX_APPEAR 1

// メモリの解放
#define SAFE_DELETE(p)			{ if(p) { delete (p);	(p)=NULL; } }
#define SAFE_DELETEARRAY(p)		{ if(p) { delete [](p);	(p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
