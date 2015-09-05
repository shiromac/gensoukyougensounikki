#include "stdafx.h"

#include "cSoundManager.h"
#include "sound/SoundDecodeItemWav.h" // <- Wav形式の読み込みを行う
#include "gameMainSystem/filemanage/cAutoSaveClass.h"

#define SOUNDFOLDER _EXELOCATION _T("sound\\")

#define SOUNDSEPACKNAME (SOUNDFOLDER _T("data1"))
#define SOUNDBGMPACKNAME (SOUNDFOLDER _T("data2"))

const int SOUNDGROUP_SE = 0;
const int SOUNDGROUP_MUSIC = 1;
static const int SOUND_STREAMBUFFERSIZE	= 8192 * 8; // 8192 * 8はストリームバッファのサイズ。この数字で大丈夫だと思う。

cSoundManager::cSoundManager()
{
	soundFileMap.clear();
}

cSoundManager::~cSoundManager()
{
	soundFileMap.clear();
	DirectSoundStream::HandleClose();
}

int cSoundManager::Init(HWND hWnd)
{
	//m_sound.init(hWnd);
	//m_sound.insertSoundItem(new SoundDecodeItemWav());

	
	//======================================
	// サウンドクラスの初期化
	// 必ず最初にこのコードで初期化
	DirectSound::InitSound(hWnd);		// ←これは主にSEで使うクラス
	DirectSoundStream::hNotifyEvent		= CreateEvent( NULL, FALSE, FALSE, NULL );
	DirectSoundStream::hNotifyThread	= CreateThread( NULL, 0 , DirectSoundStream::NotifyProc, hWnd, 0 , 0);
	SetThreadPriority(DirectSoundStream::hNotifyThread, THREAD_PRIORITY_TIME_CRITICAL);


#ifdef _UNRELEASE
//#ifdef _DEBUG
	/*
	{
		CFilePack SEpack;
		cScriptReader SR(SOUNDFOLDER _T("SEpass.id"));
		SR.load();

		vector<tstring> passes;
		getPass(SR.pdata(),SR.scriptdir(),passes);

		int i;
		for(i=0;i<passes.size();i++)
		{
			SEpack.AddFile(passes[i].c_str(),passes[i].c_str());
		}
		SEpack.OutPutPackFile(SOUNDSEPACKNAME);
	}


	{
		CFilePack BGMpack;
		cScriptReader SR(SOUNDFOLDER _T("BGMpass.id"));
		SR.load();

		vector<tstring> passes;
		getPass(SR.pdata(),SR.scriptdir(),passes);

		int i;
		for(i=0;i<passes.size();i++)
		{
			BGMpack.AddFile(passes[i].c_str(),passes[i].c_str());
		}
		BGMpack.OutPutPackFile(SOUNDBGMPACKNAME);
	}
	*/
//#endif//_DEBUG
#else

#endif

	FilePackSE_.LoadPackFile(SOUNDSEPACKNAME, /*pD3DDevice*/ NULL);
	FilePackBGM_.LoadPackFile(SOUNDBGMPACKNAME, /*pD3DDevice*/ NULL);
	
		
	return true;
}

int cSoundManager::clearSoundEffect()
{

	m_sound.releaseSB();
	soundFileMap.clear();
	Index2File.clear();
	se_exits.clear();
	return true;
}

int cSoundManager::resetVolume(void)
{
	if(playingBGM != _T("")) bgm.SetVolume_ByRate(BGMVolume_);
	return true;
}

int cSoundManager::getSoundEffectFromFile(const TCHAR* name)
{
	TCHAR path[128] = _T("");
	TCHAR filename[128] = _T("");	
	
	int IndexNum = -1;
	
	_stprintf(path,_T("%s%s%s"),SOUNDFOLDER,_T("SE\\"),name);
	_stprintf(filename,_T("%s"),name);



	map<tstring, int>::iterator itr;
	map<tstring, int>::iterator itrEnd = soundFileMap.end();


	itr = soundFileMap.find(path);
	if(itrEnd != itr)//pathをkeyとするものがある(複重)
	{
		//確保リストから参照
		IndexNum = itr->second;

		return IndexNum;
	}
/*

	IndexNum = soundFileMap.size();
	if(-1 == m_sound.loadSound(path,0,SOUNDGROUP_SE))
	{//失敗
		//再試行//直下
		if(-1 == m_sound.loadSound(filename,0,SOUNDGROUP_SE))
		{//さらに失敗
			return -1;
		}
	}
*/
	int Seek = -1;
	FilePackSE_.GetSoundSeek(path, &Seek);
	if(Seek != -1)
	{
		pDirectSound pds = pDirectSound(new DirectSound);
		pds->Create(SOUNDSEPACKNAME, Seek);
	
		IndexNum = soundFileMap.size();
		se_exits.insert(pair<int,pDirectSound>(IndexNum,pds));

		if (IndexNum != -1)
		{

			//確保リストに追加
			soundFileMap.insert( pair<tstring, int>(path, IndexNum));
			Index2File.insert( pair<int, tstring>(IndexNum, path));
		}

	}
	return IndexNum;
}

int cSoundManager::playSoundEffect(int Index)
{
	if(Index == -1)
	{
		return false;
	}
	if(SEVolume_ != 0)
	{
		//m_sound.play(Index2File[Index],1,0,1);
		se_exits[Index]->SetVolume_ByRate(SEVolume_);
		se_exits[Index]->Play();
	}
	return true;
}

int cSoundManager::getplayBGMFromFile(const TCHAR* beginName, const TCHAR* repeatName)
{
	
	TCHAR path[128] = _T("");
	TCHAR pathroop[128] = _T("");
	//TCHAR filename[128] = _T("");	
	
	int IndexNum = -1;
	
	_stprintf(path,_T("%s%s%s"),SOUNDFOLDER,_T("music\\"),beginName);
	_stprintf(pathroop,_T("%s%s%s"),SOUNDFOLDER,_T("music\\"),repeatName);
	//_stprintf(filename,_T("%s"),name);

	tstring nextBGM = beginName;
	nextBGM += _T(";");
	nextBGM += repeatName;
	if(playingBGM != nextBGM)
	{
		playingBGM.clear();
		playingBGM = beginName;
		playingBGM += _T(";");
		playingBGM += repeatName;
		stopBGM();
	}
	else
	{
		return 0;
	}
	/*
	if(-1 == m_sound.loadSound(path,SOUND_STREAMBUFFERSIZE,SOUNDGROUP_MUSIC))
	{//失敗
		return -1;
	}


	m_sound.play(path,0,1,0);

	*/

	//if(BGMVolume_ > 0)//省略すると場ぐる
	{
		int Seek = -1;
		FilePackBGM_.GetSoundSeek(path, &Seek);
		if(Seek != -1)
		{
			bgm.Create(SOUNDBGMPACKNAME, Seek);
		
		}
		Seek = -1;
		if(repeatName != _T(""))
		{
			FilePackBGM_.GetSoundSeek(pathroop, &Seek);
			if(Seek != -1)
			{
				bgm.AttendBGM(SOUNDBGMPACKNAME, Seek);
			}
		}
		bgm.SetVolume_ByRate(BGMVolume_);
		bgm.Play();
	}
	return 0;
}
//鳴りきりBGMをBGMを一旦止めてファイル名からロードしてならす。失敗した場合-1を返す。
int cSoundManager::getplaySEwithoutBGMFromFile(const TCHAR* beginName)
{
	return 0;
}
	
int cSoundManager::stopBGM()
{

	//m_sound.stopLayer(SOUNDGROUP_MUSIC);

	bgm.Stop();
	return 0;
}

void getPass(const pcScriptRLayer layer, const tstring dir, vector<tstring>& passes)
{
	int i;
	for(i=0;i<layer->script().size();i++)
	{
		if(layer->script()[i] != _T(""))
		{
			passes.push_back(dir + layer->script()[i]);
		}
	}
	
	map<tstring,pcScriptRLayer>::iterator itr = layer->membermap().begin();
	for(i=0;itr!=layer->membermap().end();itr++)
	{
		getPass(itr->second, dir + itr->first + _T("\\"), passes);
	}
}