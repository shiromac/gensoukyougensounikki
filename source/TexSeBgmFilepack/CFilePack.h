#pragma once

//#include "CFilePack.h"

//=====================================================
// ファイルをパックするクラス　				CFilePack
// パックしたファイルを読み込むクラス		CFilePackLoad
//=====================================================

#include <tchar.h>
#include <vector>
#include <map>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "../gameMainSystem/cRenderBackend.h"

using namespace std;

#define BIT_REVERSE(c)		( c = ~c)
#define BIT_CHANGE(c, k)	( c = ~c + k)
#define BIT_RECHANGE(c, k)	( c = ~(c - k))


//----------------------------------------------------
// ゴミ情報を書き込む関数
//----------------------------------------------------
static void WriteGomiData(FILE *fp, long beginSEEK, int WriteByte)
{
	fseek(fp, beginSEEK, SEEK_SET);
	for(int g=0; g<WriteByte; g++)
	{
		// ランダムな値
		char Gomi = rand() % 255;
		fwrite(&Gomi, sizeof(char), 1, fp);
	}
}


//1-16	byte	0-4byteに共通暗号キー
//1-16	byte	4-8byteにファイル数

// ファイルヘッダ （実際のデータは16byteから）

//1-16byte	1-4byteにファイル名文字数、4～ ファイル名
//16-32byte	余白 (ファイル名が長いときに使用)
//32-48byte	余白
//48-64byte	48-52 ファイルサイズ、52-56 SEEK位置

// ファイルヘッダサイズ
#define FILEHEADER_SIZE 256

	struct LoadFileHeader
	{
		basic_string<TCHAR>	fName;		// ファイル名
		int	fSize;			// サイズ
		int	fPlace;			// SEEK位置
	};

	struct SaveFileHeader
	{
		basic_string<TCHAR>	fName;		// ファイル名
		basic_string<TCHAR>	fSavename;	// パックファイルに保存する名前
		int	fSize;			// サイズ
		int	fPlace;			// SEEK位置
	};

//================================================================================
//
// テクスチャファイルをパックするクラス　CFilePack
//
//================================================================================
class CFilePack
{

private:

	vector<SaveFileHeader> m_Headers;
	int ChangeKey;

public:

	CFilePack() {}
	virtual ~CFilePack() {}

	//=====================================================
	// パックするファイル追加
	//=====================================================
	bool AddFile(const TCHAR *FileName, const TCHAR *SaveName)
	{
		FILE *fp;
		_tfopen_s(&fp, FileName, _T("rb"));
		if(fp == NULL) 
		{
			printf("%s が見つかりません。スキップします\n", FileName);
#ifndef __EMSCRIPTEN__
			Sleep(1000);
#endif
			return false;
		}
		SaveFileHeader fh;
		fh.fName = FileName;		// ファイル名保存保存
		fh.fSavename = SaveName;

		/* ファイルサイズを調査 */ 
		fseek(fp, 0, SEEK_END);
		long Size = ftell(fp);
		if(Size < 0) Size = 0; 

		fh.fSize = (int)Size;		// サイズ保存
		fh.fPlace = 0;				// 仮のSEEK位置

		m_Headers.push_back(fh);

		fclose(fp);
		return true;
	}

	//=====================================================
	// パックファイルを作成
	//=====================================================
	void OutPutPackFile(const TCHAR *PackFileName)
	{
		FILE *fp;
		_tfopen_s(&fp, PackFileName, _T("wb"));
		if(fp == NULL) return;

		// とりあえずヘッダ部はゴミで埋めておいて後から上書きすれば楽でいいんじゃね？
		fseek(fp, 0, SEEK_SET);
		int HeaderEndSeek = 16 + m_Headers.size()*FILEHEADER_SIZE;
		WriteGomiData(fp, 0, HeaderEndSeek);
		fseek(fp, 0, SEEK_SET);		// ちゃんとSEEKは戻しておく


		//----------------------------------------------------------------
		// ここから実際の必要データを書く

		ChangeKey = rand() % 255;				// ランダムキーは256通り
		fwrite(&ChangeKey, sizeof(int), 1, fp);	// 暗号化キーを書き込む

		int buf = (int)m_Headers.size();
		BIT_CHANGE(buf, ChangeKey);			// 反転
		fwrite(&buf, sizeof(int), 1, fp);	// ファイル数を書き込む

		fseek(fp, 16, SEEK_SET);			// ファイルヘッダは16バイト目から。

		// ファイル数が決まっているので、SEEK位置を全部求める
		m_Headers[0].fPlace = ( (int)m_Headers.size() * FILEHEADER_SIZE) + 16;	// 初めのファイル位置
		for(unsigned int i=1; i<m_Headers.size(); i++)
		{
			m_Headers[i].fPlace = m_Headers[i-1].fPlace + m_Headers[i-1].fSize;
		}

		// ヘッダを全て書き込む
		for(unsigned int i=0; i<m_Headers.size(); i++)
		{
			fseek(fp, (FILEHEADER_SIZE*i)+16, SEEK_SET);

			int FileNameLength = m_Headers[i].fSavename.length();
			BIT_CHANGE(FileNameLength, ChangeKey);			// 反転
			fwrite(&FileNameLength, sizeof(int), 1, fp);	// ヘッダ情報(ファイルの文字数)を書き込む
			BIT_CHANGE(FileNameLength, ChangeKey);			// 反転　また使うので元に戻す

			//fwrite(m_Headers[i].fSavename.c_str(), sizeof(char), FileNameLength, fp);	// ヘッダ情報(ファイル名)を書き込む

			// ファイル名を反転して書き込む
			for(int num=0; num<FileNameLength; num++)
			{
				// １文字ずつね･･･
				TCHAR fbuf = m_Headers[i].fSavename[num];
				BIT_CHANGE(fbuf, ChangeKey);
				fwrite(&fbuf, sizeof(TCHAR), 1, fp);	// ヘッダ情報(ファイル名)を書き込む
			}

			// 続いてファイルサイズとSEEK位置
			fseek(fp, 16 + (FILEHEADER_SIZE*(i+1)) - 16, SEEK_SET);

			int TempfSize = m_Headers[i].fSize;
			int TempfSeek = m_Headers[i].fPlace;
			BIT_CHANGE(TempfSize, ChangeKey);			// 反転
			BIT_CHANGE(TempfSeek, ChangeKey);			// 反転
			fwrite(&TempfSize, sizeof(int), 1, fp);		// ヘッダ情報(サイズ)を書き込む
			fwrite(&TempfSeek, sizeof(int), 1, fp);		// ヘッダ情報(SEEK位置)を書き込む
		}


		// 実際のデータを書き込む
		for(unsigned int i=0; i<m_Headers.size(); i++)
		{
			fseek(fp, m_Headers[i].fPlace, SEEK_SET);
			FILE *Writefp;
			_tfopen_s(&Writefp, m_Headers[i].fName.c_str(), _T("rb"));
			if(Writefp == NULL) return;

			int FileLength = (int)m_Headers[i].fSize;
			

			for(int end=0; end<FileLength; end++)
			{
				// ※
				// ここでビット反転すればデータ部の暗号化もできるかも？
				unsigned char data = 0;
				fread(&data, 1, 1, Writefp);
				fwrite(&data, 1, 1, fp);
			}
			fclose(Writefp);

		}

		fclose(fp);
	}

};

//================================================================================
//
// パックしたファイルを読み込むクラス		CFilePackLoad
//
//================================================================================
class CFilePackLoad
{

private:
	cRenderDevice* m_Device;
	basic_string<TCHAR> PackFileName;
	int ChangeKey;

public:

	map<basic_string<TCHAR>, LoadFileHeader> Files;

	CFilePackLoad() {}
	virtual ~CFilePackLoad() {}

	//=====================================================
	// パックファイルを読み込んで、ヘッダ情報だけを抽出
	//=====================================================
	bool LoadPackFile(const TCHAR* FileName, cRenderDevice* inDevice=NULL)
	{
		Files.clear();

		m_Device = inDevice;
		FILE *fp;
		_tfopen_s(&fp, FileName, _T("rb") );
		if(fp == NULL)
		{
#ifdef __EMSCRIPTEN__
			PackFileName = FileName;
			return false;
#else
			#ifdef _UNRELEASE
			printf("ファイルが見つかりません");
			MessageBox(NULL, _T("ファイルが見つかりません"), 0, 0);
			#endif
			exit(0);
			return false;
#endif
		}

		PackFileName = FileName;

		fread(&ChangeKey, sizeof(int), 1, fp);	// 暗号化キーを読み込む

		int FileNum = 0;
		fread(&FileNum, sizeof(int), 1, fp);	// まず、格納されているファイル数を得る
		BIT_RECHANGE(FileNum, ChangeKey);					// 反転

		for(int i=0; i<FileNum; i++)
		{
			fseek(fp, 16+(FILEHEADER_SIZE*i), SEEK_SET);	// ヘッダの位置に移動

			int FileNameLength = 0;
			TCHAR FileName[96] = {0};
			int Filesize = 0;
			int FileSeek = 0;

			fread(&FileNameLength, sizeof(int), 1, fp);
			BIT_RECHANGE(FileNameLength, ChangeKey);		// 反転

			//fread(&FileName, sizeof(char), FileNameLength, fp);
			
			// ファイル名を反転したのを読み込む
			for(int num=0; num<FileNameLength; num++)
			{
				// １文字ずつね･･･
				TCHAR fbuf;
				fread(&fbuf, sizeof(TCHAR), 1, fp);
				BIT_RECHANGE(fbuf, ChangeKey);				// 反転
				FileName[num] = fbuf;
			}

			fseek(fp, 16 + (FILEHEADER_SIZE*(i+1)) - 16, SEEK_SET);

			fread(&Filesize, sizeof(int), 1, fp);
			fread(&FileSeek, sizeof(int), 1, fp);
			BIT_RECHANGE(Filesize, ChangeKey);		// 反転
			BIT_RECHANGE(FileSeek, ChangeKey);		// 反転

			LoadFileHeader fh;
			fh.fName = FileName;
			fh.fSize = Filesize;
			fh.fPlace = FileSeek;

			Files.insert( pair<basic_string<TCHAR>, LoadFileHeader> (fh.fName, fh) );
		}

		fclose(fp);
		return true;

	}


	//=====================================================
	// 欲しいテクスチャの部分を読み込み、テクスチャ作成
	//=====================================================
	bool ReadFileData(const TCHAR* FileName, vector<BYTE>& data)
	{
		data.clear();

		basic_string<TCHAR> theKey(FileName);
		map<basic_string<TCHAR>, LoadFileHeader>::iterator p = Files.find(theKey);
		if(p == Files.end())
		{
			return false;
		}

		FILE *fp;
		_tfopen_s(&fp, PackFileName.c_str(), _T("rb") );
		if(fp == NULL)
		{
			return false;
		}

		data.resize(p->second.fSize);
		fseek(fp, p->second.fPlace, SEEK_SET);
		size_t readSize = 0;
		if(!data.empty())
		{
			readSize = fread( &data[0], sizeof(BYTE), p->second.fSize, fp);
		}
		fclose(fp);

		if(readSize != static_cast<size_t>(p->second.fSize))
		{
			data.clear();
			return false;
		}

		return true;
	}

	cRenderTexture* GetTextureFile(const TCHAR* FileName, cRenderTextureFormat format=C_RENDER_TEXTURE_FORMAT_A8R8G8B8, DWORD colorkey=0x00000000)
	{
		cRenderTexture* pTex = NULL;
		vector<BYTE> fileData;
		if(ReadFileData(FileName, fileData) && !fileData.empty())
		{
			cRenderCreateTextureFromMemory(m_Device, &fileData[0], static_cast<UINT>(fileData.size()), format, colorkey, &pTex);
		}

		return pTex;
	}

	//=====================================================
	// パックされているサウンドのSEEK位置を得る オブジェクトは作りません
	// SEとBGM用
	//=====================================================
	void GetSoundSeek(const TCHAR* FileName, int *pfSeek)
	{
		basic_string<TCHAR> theKey(FileName);
		map<basic_string<TCHAR>, LoadFileHeader>::iterator p = Files.find(theKey);
		if(p == Files.end())
		{
			*pfSeek = -1;
			return;
		}
		else
		{
			// ファイル名発見した↓
			// ヘッダはもう読んであるので再度読む必要なし
			*pfSeek = p->second.fPlace;
			return;
		}
	}

	//=====================================================
	// パック内のファイルのサイズとSEEKを得る オブジェクトは作りません
	// ファイルサイズも知りたい人向け
	//=====================================================
	bool GetFileSeekAndSize(const TCHAR* FileName, int *pfSeek, int *pfSize)
	{
		basic_string<TCHAR> theKey(FileName);
		map<basic_string<TCHAR>, LoadFileHeader>::iterator p = Files.find(theKey);
		if(p == Files.end())
		{
			*pfSeek = -1;
			*pfSize = 0;
			return false;
		}
		else
		{
			// ファイル名発見した↓
			// ヘッダはもう読んであるので再度読む必要なし
			*pfSeek = p->second.fPlace;
			*pfSize = p->second.fSize;
			return true;
		}
	}

};