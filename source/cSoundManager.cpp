#include "stdafx.h"

#include "cSoundManager.h"
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif
#ifndef __EMSCRIPTEN__
#include "sound/SoundDecodeItemWav.h"
#include "gameMainSystem/filemanage/cAutoSaveClass.h"
#endif

#define SOUNDFOLDER _EXELOCATION _T("sound\\")

#define SOUNDSEPACKNAME (SOUNDFOLDER _T("data1"))
#define SOUNDBGMPACKNAME (SOUNDFOLDER _T("data2"))

const int SOUNDGROUP_SE = 0;
const int SOUNDGROUP_MUSIC = 1;
static const int SOUND_STREAMBUFFERSIZE	= 8192 * 8; // 8192 * 8はストリームバッファのサイズ。この数字で大丈夫だと思う。
#ifdef __EMSCRIPTEN__
namespace
{
	std::string BrowserAudioPath(const TCHAR* path)
	{
		std::string normalized = ggn_tchar_to_path(path);
		while(normalized.size() >= 2 && normalized[0] == '.' && normalized[1] == '/')
		{
			normalized.erase(0, 2);
		}
		return normalized;
	}

	void BrowserAudioEnsure()
	{
		EM_ASM((function() {
			if (Module.ggnAudio) return;

			var state = {
				context: null,
				masterGain: null,
				seGain: null,
				bgmGain: null,
				buffers: {},
				loading: {},
				resolvedPaths: {},
				errors: [],
				loadCount: 0,
				playCount: 0,
				bgmToken: 0,
				bgmSources: [],
				bgmPath: ''
			};

			function normalize(path) {
				path = (path || '').split(String.fromCharCode(92)).join('/');
				while (path.indexOf('./') === 0) path = path.substring(2);
				return path;
			}

			function context() {
				if (state.context) return state.context;
				var Ctor = window.AudioContext || window.webkitAudioContext;
				if (!Ctor) return null;
				state.context = new Ctor();
				state.masterGain = state.context.createGain();
				state.seGain = state.context.createGain();
				state.bgmGain = state.context.createGain();
				state.seGain.gain.value = 1;
				state.bgmGain.gain.value = 1;
				state.seGain.connect(state.masterGain);
				state.bgmGain.connect(state.masterGain);
				state.masterGain.connect(state.context.destination);
				return state.context;
			}

			function resume() {
				var ctx = context();
				if (ctx && ctx.state === 'suspended') ctx.resume();
			}

			['keydown', 'pointerdown', 'mousedown', 'touchstart'].forEach(function(name) {
				window.addEventListener(name, resume, { capture: true, once: false, passive: true });
			});

			function candidatesForPath(path) {
				if (path.indexOf('/music/') >= 0 && path.slice(-4).toLowerCase() === '.wav') {
					return [path.slice(0, -4) + '.m4a', path];
				}
				return [path];
			}

			function decodeCandidate(requestedPath, candidates, index) {
				var ctx = context();
				if (!ctx || index >= candidates.length) return Promise.resolve(null);
				var actualPath = candidates[index];
				return fetch(actualPath)
					.then(function(response) {
						if (!response.ok) {
							if (index + 1 < candidates.length) return decodeCandidate(requestedPath, candidates, index + 1);
							throw new Error('audio fetch ' + response.status + ': ' + actualPath);
						}
						return response.arrayBuffer().then(function(data) {
							return ctx.decodeAudioData(data);
						}).then(function(buffer) {
							state.buffers[requestedPath] = buffer;
							state.resolvedPaths[requestedPath] = actualPath;
							state.loading[requestedPath] = null;
							return buffer;
						});
					})
					.catch(function(error) {
						if (index + 1 < candidates.length) return decodeCandidate(requestedPath, candidates, index + 1);
						console.warn('[ggn audio]', error);
						state.errors.push(String(error));
						state.loading[requestedPath] = null;
						return null;
					});
			}

			function load(path) {
				path = normalize(path);
				if (!path) return Promise.resolve(null);
				if (state.buffers[path]) return Promise.resolve(state.buffers[path]);
				if (state.loading[path]) return state.loading[path];
				if (!context()) return Promise.resolve(null);
				state.loadCount++;
				state.loading[path] = decodeCandidate(path, candidatesForPath(path), 0);
				return state.loading[path];
			}

			function stopBgm() {
				state.bgmToken++;
				state.bgmSources.forEach(function(source) {
					try { source.stop(0); } catch (e) {}
				});
				state.bgmSources = [];
			}

			function startSource(buffer, gain, loop, when) {
				var ctx = context();
				if (!ctx || !buffer) return null;
				var source = ctx.createBufferSource();
				source.buffer = buffer;
				source.loop = !!loop;
				source.connect(gain);
				source.start(typeof when === 'number' ? when : 0);
				state.playCount++;
				return source;
			}

			Module.ggnAudio = state;
			Module.ggnAudioResume = resume;
			Module.ggnAudioEnsure = function() {
				return !!context();
			};
			Module.ggnAudioLoad = function(path) {
				return load(path);
			};
			Module.ggnAudioSetVolume = function(seVolume, bgmVolume) {
				context();
				var se = Math.max(0, Math.min(1, seVolume / 100));
				var bgm = Math.max(0, Math.min(1, bgmVolume / 100));
				if (state.seGain) state.seGain.gain.value = se;
				if (state.bgmGain) state.bgmGain.gain.value = bgm;
			};
			Module.ggnAudioPlaySe = function(path) {
				path = normalize(path);
				resume();
				load(path).then(function(buffer) {
					if (buffer) startSource(buffer, state.seGain, false, null);
				});
			};
			Module.ggnAudioStopBgm = stopBgm;
			Module.ggnAudioPlayBgm = function(beginPath, repeatPath) {
				beginPath = normalize(beginPath);
				repeatPath = normalize(repeatPath);
				stopBgm();
				state.bgmPath = beginPath + ';' + repeatPath;
				var token = state.bgmToken;
				resume();

				if (beginPath && repeatPath) {
					Promise.all([load(beginPath), load(repeatPath)]).then(function(buffers) {
						if (state.bgmToken !== token || !buffers[0] || !buffers[1]) return;
						var ctx = context();
						if (!ctx) return;
						var startAt = ctx.currentTime + 0.05;
						var introSource = startSource(buffers[0], state.bgmGain, false, startAt);
						var loopSource = startSource(buffers[1], state.bgmGain, true, startAt + buffers[0].duration);
						if (introSource) state.bgmSources.push(introSource);
						if (loopSource) state.bgmSources.push(loopSource);
					});
				} else if (beginPath) {
					load(beginPath).then(function(buffer) {
						if (state.bgmToken !== token || !buffer) return;
						var source = startSource(buffer, state.bgmGain, true, 0);
						if (source) state.bgmSources.push(source);
					});
				} else if (repeatPath) {
					load(repeatPath).then(function(buffer) {
						if (state.bgmToken !== token || !buffer) return;
						var source = startSource(buffer, state.bgmGain, true, 0);
						if (source) state.bgmSources.push(source);
					});
				}
			};
		})());
	}

	void BrowserAudioResume()
	{
		BrowserAudioEnsure();
		EM_ASM({
			if (Module.ggnAudioResume) Module.ggnAudioResume();
		});
	}

	void BrowserAudioSetVolume(int seVolume, int bgmVolume)
	{
		BrowserAudioEnsure();
		EM_ASM({
			if (Module.ggnAudioSetVolume) Module.ggnAudioSetVolume($0, $1);
		}, seVolume, bgmVolume);
	}

	void BrowserAudioPreloadPath(const TCHAR* path)
	{
		std::string normalized = BrowserAudioPath(path);
		BrowserAudioEnsure();
		EM_ASM({
			if (Module.ggnAudioLoad) Module.ggnAudioLoad(UTF8ToString($0));
		}, normalized.c_str());
	}

	void BrowserAudioPlaySePath(const TCHAR* path)
	{
		std::string normalized = BrowserAudioPath(path);
		EM_ASM({
			if (Module.ggnAudioPlaySe) Module.ggnAudioPlaySe(UTF8ToString($0));
		}, normalized.c_str());
	}

	void BrowserAudioPlayBgmPaths(const TCHAR* beginPath, const TCHAR* repeatPath)
	{
		std::string normalizedBegin = BrowserAudioPath(beginPath);
		std::string normalizedRepeat = BrowserAudioPath(repeatPath);
		EM_ASM({
			if (Module.ggnAudioPlayBgm) Module.ggnAudioPlayBgm(UTF8ToString($0), UTF8ToString($1));
		}, normalizedBegin.c_str(), normalizedRepeat.c_str());
	}

	void BrowserAudioStopBgm()
	{
		BrowserAudioEnsure();
		EM_ASM({
			if (Module.ggnAudioStopBgm) Module.ggnAudioStopBgm();
		});
	}
}
#endif

cSoundManager::cSoundManager()
{
	soundFileMap.clear();
}

cSoundManager::~cSoundManager()
{
	soundFileMap.clear();
#ifndef __EMSCRIPTEN__
	DirectSoundStream::HandleClose();
#endif
}

int cSoundManager::Init(cSoundWindowHandle hWnd)
{
#ifdef __EMSCRIPTEN__
	BrowserAudioEnsure();
#else
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
#endif
	
		
	return true;
}

int cSoundManager::clearSoundEffect()
{

#ifndef __EMSCRIPTEN__
	m_sound.releaseSB();
	se_exits.clear();
#endif
	soundFileMap.clear();
	Index2File.clear();
	return true;
}

int cSoundManager::resetVolume(void)
{
#ifdef __EMSCRIPTEN__
	BrowserAudioSetVolume(SEVolume_, BGMVolume_);
#else
	if(playingBGM != _T("")) bgm.SetVolume_ByRate(BGMVolume_);
#endif
	return true;
}

int cSoundManager::unlockAudio(void)
{
#ifdef __EMSCRIPTEN__
	BrowserAudioResume();
#endif
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
#ifdef __EMSCRIPTEN__
	IndexNum = soundFileMap.size();
	soundFileMap.insert( pair<tstring, int>(path, IndexNum));
	Index2File.insert( pair<int, tstring>(IndexNum, path));
	BrowserAudioPreloadPath(path);
	return IndexNum;
#else
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
#endif
}

int cSoundManager::playSoundEffect(int Index)
{
	if(Index == -1)
	{
		return false;
	}
#ifdef __EMSCRIPTEN__
	if(SEVolume_ != 0)
	{
		map<int,tstring>::iterator itr = Index2File.find(Index);
		if(itr != Index2File.end())
		{
			BrowserAudioSetVolume(SEVolume_, BGMVolume_);
			BrowserAudioPlaySePath(itr->second.c_str());
		}
	}
#else
	if(SEVolume_ != 0)
	{
		//m_sound.play(Index2File[Index],1,0,1);
		se_exits[Index]->SetVolume_ByRate(SEVolume_);
		se_exits[Index]->Play();
	}
#endif
	return true;
}

int cSoundManager::getplayBGMFromFile(const TCHAR* beginName, const TCHAR* repeatName)
{
	if(beginName == NULL) beginName = _T("");
	if(repeatName == NULL) repeatName = _T("");
	
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

#ifdef __EMSCRIPTEN__
	BrowserAudioSetVolume(SEVolume_, BGMVolume_);
	BrowserAudioPlayBgmPaths(
		_tcscmp(beginName, _T("")) != 0 ? path : _T(""),
		_tcscmp(repeatName, _T("")) != 0 ? pathroop : _T(""));
#else
	//if(BGMVolume_ > 0)//省略すると場ぐる
	{
		int Seek = -1;
		FilePackBGM_.GetSoundSeek(path, &Seek);
		if(Seek != -1)
		{
			bgm.Create(SOUNDBGMPACKNAME, Seek);
		
		}
		Seek = -1;
		if(_tcscmp(repeatName, _T("")) != 0)
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
#endif
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

#ifdef __EMSCRIPTEN__
	BrowserAudioStopBgm();
#else
	bgm.Stop();
#endif
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