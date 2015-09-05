#pragma once

#include "sound/sound.h"
#include "TexSeBgmFilepack/CFilePack.h"
#include "TexSeBgmFilepack/DirectSound.h"
#include "TexSeBgmFilepack/DirectSoundStream.h"
#include "gameMainSystem/filemanage/cScriptRLayer.h"

#include <vector>
#include "gameMainSystem/filemanage/tstring_ph.h"


#include <map>
using namespace std;

#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<DirectSound> pDirectSound;

class cSoundManager
{
public:
	cSoundManager(void);
public:
	virtual ~cSoundManager(void);

	//初期化
	int Init(HWND hWnd);

	//サウンドをファイル名からロードしてナンバーを返す。失敗した場合-1を返す。
	int getSoundEffectFromFile(const TCHAR* name);
	int playSoundEffect(int Index);

	//BGMをファイル名からロードしてならす。失敗した場合-1を返す。同時にならせるBGMは１つだけ
	int getplayBGMFromFile(const TCHAR* beginName, const TCHAR* repeatName);
	//BGMをファイル名からロードしてならす。失敗した場合-1を返す。同時にならせるBGMは１つだけ
	int getplayBGMFromFile(const TCHAR* repeatName){return getplayBGMFromFile(repeatName,_T(""));};
	
	//鳴りきりBGMをBGMを一旦止めてファイル名からロードしてならす。失敗した場合-1を返す。
	int getplaySEwithoutBGMFromFile(const TCHAR* beginName);
	
	//BGMを止める
	int stopBGM();

	//きれいさっぱり忘れる
	int clearSoundEffect();

	void setSEVolume(int vol){SEVolume_ = vol;resetVolume();};
	void setBGMVolume(int vol){BGMVolume_ = vol;resetVolume();};
	int getSEVolume(void){return SEVolume_;};
	int getBGMVolume(void){return BGMVolume_;};

	int resetVolume(void);

	int* pSEVolume(void){return &SEVolume_;};
	int* pBGMVolume(void){return &BGMVolume_;};
protected:
	int SEVolume_;
	int BGMVolume_;

	Sound m_sound;

	map<tstring,int> soundFileMap;
	map<int,tstring> Index2File;

	tstring playingBGM;


protected:
	//パックテクスチャ
	CFilePackLoad FilePackSE_;
	CFilePackLoad FilePackBGM_;

	DirectSoundStream bgm;
	map<int,pDirectSound> se_exits;
};
void getPass(const pcScriptRLayer layer, const tstring dir, vector<tstring>& passes);
