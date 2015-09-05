///////////////////////////////////////////////////////////
/// Stream読み込みできるサウンドクラスの使い方を教えるためのcppファイルです。
/// 注意その１. stdafx.hに#include "Utility_K.h"を付け加えてください。
///////////////////////////////////////////////////////////
#include "stdafx.h"
#include <iostream>
#include <tchar.h>
#include "sound.h" // <- サウンドクラスを使うときはsound.hをインクルードしてください
#include "SoundDecodeItemWav.h" // <- Wav形式の読み込みを行う

HWND GetWindowHandle();

// マクロ
static const int SOUNDLAYER_BGM	= 0; 
static const int SOUNDLAYER_SE = 1;
static const int SOUND_STREAMBUFFERSIZE	= 8192 * 8; // 8192 * 8はストリームバッファのサイズ。この数字で大丈夫だと思う。

int main()
{
	HWND hWnd = GetWindowHandle();
	Sound sound;
	// 一応メモリリークチェック
	MyDebug::CheckMemoryLeak();
	// ---ここから開始---
	// 1. 初期化をします。ウィンドウハンドルを渡してください。
	// 初期化は1回だけでいいので、ウィンドウを作ったあとぐらいに初期化してください。
	sound.init(hWnd);
	// 2. Wav形式の読み込みを行う。この行は必須なので、init()の後にこのまま書いてください。
	sound.insertSoundItem(new SoundDecodeItemWav());

	// 初期化作業はこれで終了。次からサウンドファイルの読み込みについて

	// 3. サウンドファイルを読み込みます。
	// BGMを読み込みます
	sound.loadSound(TEXT("sound/testbgm.wav"), SOUND_STREAMBUFFERSIZE, SOUNDLAYER_BGM); 
	sound.loadSound(TEXT("sound/testbgm2.wav"), SOUND_STREAMBUFFERSIZE, SOUNDLAYER_BGM); 
	// SEを読み込みます
	sound.loadSound(TEXT("sound/testse.wav"), 0, SOUNDLAYER_SE); // SEの時はストリーミングする必要はないので、バッファサイズを0にしてください。
	sound.loadSound(TEXT("sound/testse2.wav"), 0, SOUNDLAYER_SE);
	sound.play(TEXT("sound/testbgm.wav"), true, true);
	// ループ変数
	int bgmvolume = 0;
	int sevolume = 0;
	while(1) // 出来る事
	{
		cout << "キー入力 > ";
		char ch = getchar();
		fflush(stdin);
		if(ch == 'q') // 終了
		{
			cout << "終了します\n";
			break;
		}
		else if(ch == 'z') // 効果音1を鳴らす
		{
			sound.play(TEXT("sound/testse.wav"), true);
			cout << "効果音1を再生\n";
		}
		else if(ch == 'x') // 効果音2を鳴らす
		{
			sound.play(TEXT("sound/testse2.wav"), true);
			cout << "効果音2を再生\n";
		}
		else if(ch == '1') // 曲を再生（鳴っている場合は最初からにしない
		{
			sound.play(TEXT("sound/testbgm.wav"), true, true, false);
			cout << "testbgm.wavを再生（鳴っている場合はそのまま\n";
		}
		else if(ch == '2') // 曲を最初から再生
		{
			sound.play(TEXT("sound/testbgm.wav"), true, true, true);
			cout << "testbgm.wavを最初から再生\n";
		}
		else if(ch == '3') // 曲を停止
		{
			sound.stop(TEXT("sound/testbgm.wav"));
			cout << "testbgm.wavを停止\n";
		}
		else if(ch == '4') // 曲の音量調節(即座
		{
			bgmvolume = (bgmvolume) ? 0 : -1000;
			sound.changeVolume(TEXT("sound/testbgm.wav"), bgmvolume, 0);
			cout << "testbgm.wavの音量を" << bgmvolume << "にしました\n";
		}
		else if(ch == '5') // 曲の音量調節(フェード
		{
			const int ms = 200; // ミリ秒単位
			bgmvolume = (bgmvolume) ? 0 : -1000;
			sound.changeVolume(TEXT("sound/testbgm.wav"), bgmvolume, ms);
			cout << "testbgm.wavの音量を" << ms << "ミリ秒かけて" << bgmvolume << "にしました\n";
		}
		else if(ch == '6') // レイヤ単位で音量調節
		{
			sevolume = (sevolume) ? 0 : -1000;
			sound.changeVolumeLayer(SOUNDLAYER_SE, sevolume, 0);
			cout << "seレイヤの音量を" << sevolume << "にしました\n";
		}
		else if(ch == '7') // レイヤ単位で停止
		{
			cout << "一時的にbgm2を流します\n";
			sound.play(TEXT("sound/testbgm2.wav"), true, true);
			cout << "...3 second wait...\n";
			Sleep(3000);
			sound.stopLayer(SOUNDLAYER_BGM);
			cout << "bgmレイヤを停止しました\n";
		}
	}
	// 4. 終了処理はsoundクラスがなくなればやってくれる
	// しかし、メモリ確保のためにBGMを削除したい場合、releaseSB()を行ってください。
	return 0;
}

/// ここから下は関係ない。
HWND GetWindowHandle()
{
	HWND windowhandle = NULL;
	const int MY_BUFSIZE = 1024;		// コンソール ウィンドウのタイトル用のバッファサイズ
    TCHAR pszNewWindowTitle[MY_BUFSIZE]; // 作成されるウィンドウのタイトルが入ります
    TCHAR pszOldWindowTitle[MY_BUFSIZE]; // 元のウィンドウタイトルが入ります

    // 現在のウィンドウタイトルを取得します
	GetConsoleTitle(pszOldWindowTitle, MY_BUFSIZE);
    // 独自に、ウィンドウの新規タイトルをフォーマットします
	// 複数のウィンドウが同じタイトルを持つ可能性があるため、ユニークにします
    _stprintf_s(pszNewWindowTitle, MY_BUFSIZE, TEXT("%d/%d"), GetTickCount(), GetCurrentProcessId());
    // 現在のウィンドウタイトルを変更します
    SetConsoleTitle(pszNewWindowTitle);
    // ウィンドウタイトルのアップデートを確実なものにさせます
    Sleep(40);
    // ウィンドウの新規タイトルを探しにいきます
    windowhandle = FindWindow(NULL, pszNewWindowTitle);
    // 元のウィンドウタイトルへ戻します
    SetConsoleTitle(pszOldWindowTitle);
	return windowhandle;
}