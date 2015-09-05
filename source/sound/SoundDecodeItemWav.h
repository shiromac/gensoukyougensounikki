#pragma once 

#include "SoundDecodeItem.h"

class SoundDecodeItemWav : public SoundDecodeItem {
public:
	SoundDecodeItemWav();
	virtual ~SoundDecodeItemWav();
private:
	HMMIO				m_hmmioIn;
	MMCKINFO			m_chankInRIFF;
	MMCKINFO			m_chankIn;
	MMIOINFO			m_mmiodata;
private:
	HRESULT WaveOpen();
	HRESULT WaveReadStart();
	HRESULT WaveRead(DWORD size, char* buf, DWORD* out_readsize);
	HRESULT WaveReadEnd();
	void WaveClose();
public:
	virtual SoundDecodeItem* copy();
	virtual bool loadHeader();
	virtual int loadData();
	virtual void loadDataInit();
	virtual void loadDataStart();
	virtual void loadDataReStart();
	virtual void loadDataEnd();
	virtual void loadDataClose();
	virtual bool checkload(char c1, char c2, char c3, char c4);
	virtual void destory();
};