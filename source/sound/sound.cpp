#include "stdafx.h"
//_
//_ sound.cpp
//_ 07/03/08
//_ ver1.06

#include "sound.h"
//ライブラリの追加
#pragma comment(lib, "winmm.lib")

Sound::Sound()
{
	CoInitialize(NULL);
	m_lpDS = NULL;
	m_lpDSP = NULL;
}

Sound::~Sound()
{
	release();
	CoUninitialize();
}

void Sound::release()
{
	releaseSB();
	if(m_lpDSP)
	{
		m_lpDSP->Release();
		m_lpDSP = NULL;
	}
	if(m_lpDS)
	{
		m_lpDS->Release();
		m_lpDS = NULL;
	}

	for(unsigned int i = 0; i < m_sditems.size(); i++)
		delete m_sditems[i];
}

void Sound::releaseSB()
{
	map<tstring, SoundBufferItem*>::iterator itr = m_sbitems.begin();
	while(itr != m_sbitems.end())
	{
		delete itr->second;
		itr++;
	}
	m_sbitems.clear();
	m_mapsbitems.clear();
}

int Sound::init(HWND hWnd)
{
	// DirectSound8のオブジェクトを作ります。
	if(FAILED(DirectSoundCreate8(NULL, &m_lpDS, NULL)))
	{
		return -1;
	}
	// DirectSoundの初期化を行う。
	// １．「協調レベル」を設定。
	if(FAILED(m_lpDS->SetCooperativeLevel(hWnd, DSSCL_PRIORITY)))
	{
		return -1;
	}
	// ２．プライマリバッファを作成
	DSBUFFERDESC desc;
	ZeroMemory(&desc, sizeof(DSBUFFERDESC));
	desc.dwSize = sizeof(DSBUFFERDESC);
	desc.dwFlags = DSBCAPS_PRIMARYBUFFER;
	if(FAILED(m_lpDS->CreateSoundBuffer(&desc, &m_lpDSP, NULL)))
	{
		return -1;
	}
	return 0;
}

int Sound::loadSoundbyHandle(HANDLE handle, char* str4, DWORD bufsize, int layer, tstring filename)
{
	for(unsigned int i = 0; i < m_sditems.size(); i++)
	{
		char c1 = *(str4);
		char c2 = *(str4 + 1);
		char c3 = *(str4 + 2);
		char c4 = *(str4 + 3);
		if(m_sditems[i]->checkload(c1, c2, c3, c4))
		{
			DWORD curpos = SetFilePointer(handle, 0, 0, FILE_CURRENT);
			m_sditems[i]->setLoadInfo_Handle(handle);
			if(m_sditems[i]->loadHeader())
			{
				// ファイル位置を戻す
				SetFilePointer(handle, curpos, 0, FILE_BEGIN);
				m_sditems[i]->destory();
				SoundBufferItem* sb;
				sb = new SoundBufferItem();
				sb->init(m_lpDS, m_sditems[i]->copy());
				sb->load_handle(handle, bufsize);
				m_sbitems.insert(pair<tstring, SoundBufferItem*>(filename, sb));
				m_mapsbitems.insert(pair<int, SoundBufferItem*>(layer, sb));
				return 0;
			}
		}
	}
	return -1;
}

int Sound::loadSound(tstring filename, DWORD bufsize, int layer, tstring datname)
{
	if(m_sbitems.find(filename) != m_sbitems.end()) // ファイルを既に読み込んでいる際
		return 0;
	// まだ明確に分類法が分からないために拡張子で分ける
	/// @todo : 分類法を明らかにする
	tstring ext = MyFile::GetExtension(filename);

	for(unsigned int i = 0; i < m_sditems.size(); i++)
	{
		char c1 = (ext.size() == 0) ? 0 : ext[0];
		char c2 = (ext.size() <= 1) ? 0 : ext[1];
		char c3 = (ext.size() <= 2) ? 0 : ext[2];
		char c4 = (ext.size() <= 3) ? 0 : ext[3];
		if(m_sditems[i]->checkload(c1, c2, c3, c4))
		{
			if(!datname.empty()) // datがある場合
			{
				HANDLE handle;
				if(MyFile::FilePack::GetHandle(datname, filename, &handle)) // ファイルが存在するならば
				{
					char str4[4] = {c1, c2, c3, c4};
					return loadSoundbyHandle(handle, str4, bufsize, layer, filename);
				}
			}
			else
			{
				m_sditems[i]->setLoadInfo_Filename(filename);
				if(m_sditems[i]->loadHeader())
				{
					m_sditems[i]->destory();
					SoundBufferItem* sb;
					sb = new SoundBufferItem();
					sb->init(m_lpDS, m_sditems[i]->copy());
					sb->load_filename(filename, bufsize);
					m_sbitems.insert(pair<tstring, SoundBufferItem*>(filename, sb));
					m_mapsbitems.insert(pair<int, SoundBufferItem*>(layer, sb));
					return 0;
				}
			}
		}
	}
	return -1;
}

///////////////////////////////////////////////////////////
///	@brief 曲・効果音再生
///	@param[in] loopflag ループするかどうか
///	@param[in] startflag 最初から再生するかどうか
///	@param[in] overlapflag ループ時に読み込んでいても再生回数だけ増やす
///////////////////////////////////////////////////////////
void Sound::play(tstring id, bool startflag, bool loopflag, bool overlapflag)
{
	map<tstring, SoundBufferItem*>::iterator itr = m_sbitems.find(id);
	if(itr != m_sbitems.end())
		itr->second->play(loopflag, startflag, overlapflag);
}

void Sound::pause(tstring id)
{
	map<tstring, SoundBufferItem*>::iterator itr = m_sbitems.find(id);
	if(itr != m_sbitems.end())
		itr->second->pause();
}

void Sound::stop(tstring id)
{
	map<tstring, SoundBufferItem*>::iterator itr = m_sbitems.find(id);
	if(itr != m_sbitems.end())
		itr->second->stop();
}

void Sound::change_speed(tstring id, float speed)
{
}

void Sound::changePan(tstring id, LONG pan)
{
}

void Sound::changeVolume(tstring id, LONG volume, DWORD time)
{
	map<tstring, SoundBufferItem*>::iterator itr = m_sbitems.find(id);
	if(itr != m_sbitems.end())
		itr->second->changeVolume(volume, time);
}

DWORD Sound::checktime(tstring id)
{
	return 0;
}

void Sound::settime(tstring id, DWORD time)
{
}

DWORD Sound::getBuffersize(tstring id)
{
	return 0;
}

int Sound::getstate(tstring id)
{
	map<tstring, SoundBufferItem*>::iterator itr = m_sbitems.find(id);
	if(itr != m_sbitems.end())
		itr->second->getState();
	return -1;
}

void Sound::insertSoundItem(SoundDecodeItem* psi)
{
	m_sditems.push_back(psi);
}

void Sound::stopLayer(int layer)
{
	pair<multimap<int, SoundBufferItem*>::iterator, 
		multimap<int, SoundBufferItem*>::iterator> itrpair = m_mapsbitems.equal_range(layer);
	for(multimap<int, SoundBufferItem*>::iterator itr = itrpair.first; itr != itrpair.second; itr++)
		itr->second->stop();
}

void Sound::changeVolumeLayer(int layer, LONG volume, DWORD time)
{
	pair<multimap<int, SoundBufferItem*>::iterator, 
		multimap<int, SoundBufferItem*>::iterator> itrpair = m_mapsbitems.equal_range(layer);
	for(multimap<int, SoundBufferItem*>::iterator itr = itrpair.first; itr != itrpair.second; itr++)
		itr->second->changeVolume(volume, time);
}
