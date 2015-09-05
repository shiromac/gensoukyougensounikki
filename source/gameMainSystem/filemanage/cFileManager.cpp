#include "cFileManager.h"

#include "../../utility/ATLcompati/CString.h"
#include <windows.h>

using namespace std;

using namespace ATLcompati;

const int CHARCODE_UNKNOWN = 0;
const int CHARCODE_SJIS = 1;
const int CHARCODE_UNICODE_BE = 2;
const int CHARCODE_UNICODE_LE = 3;

//#include <atlstr.h>
//using namespace ATL;

cFileManager::cFileManager(void)
{
}

cFileManager::~cFileManager(void)
{
}

int cFileManager::saveFile(const tstring& filename, unsigned flag, std::vector<SByte>& data)
{//バイナリ
	ofstream ofs(filename.c_str(), flag);
	
	if(ofs)
	{
		vector<SByte>::iterator itr = data.begin();
		for(;itr!=data.end();itr++)
		{
			ofs.put(*itr);
		}

		ofs.close();
	}
	else
	{//開けられない無い！
		return ERROR_FM_FILE_ERROR;
	}

	return 0;
}
int cFileManager::loadFile(const tstring& filename, unsigned flag, std::vector<SByte>& data)
{//バイナリ
    ifstream ifs(filename.c_str(), flag);

	
	if(ifs)
	{
		ifs.seekg(0, std::ios::end );
		int size = ifs.tellg(); 
		ifs.seekg(0, std::ios::beg ); 


		data.resize(size);
		//SByte c;
		
		vector<SByte>::iterator itr = data.begin();
		for(;itr!=data.end();itr++)
		{//読み込み
			ifs.get(*itr);
		}
    }
	else
	{//開けられない無い！
		//もしかして「暗号」
		if(loadEncryptFile(filename,data) != SUCCESS)
		{
			return ERROR_FM_FILE_ERROR;
		}
		return 0;
	}

    ifs.close();  

	return 0;
}

const UByte CIPHERKEY[16] = {0x7F,0x43,0x5C,0x16,
							0x18,0x57,0xDF,0x09,
							0x46,0x4B,0x93,0x37,
							0x36,0xDE,0xF2,0xAC};
static unsigned int easyrandnext;
UByte easyrand(void)
{
	easyrandnext = easyrandnext * 1246501853 + 12347;
	return (unsigned int)(easyrandnext >> 16);
}
int cFileManager::saveEncryptFile(const tstring& filename, std::vector<SByte>& data)
{
	unsigned flag = std::ios_base::binary | std::ios_base::out | std::ios_base::trunc;
	
	tstring truefilename = filename;
	if(filename.substr( filename.size() - 5 ) != _T(".cdat"))
	{//拡張子cdatじゃないときはつける
		truefilename += _T(".cdat");
	}

	ofstream ofs(truefilename.c_str(), flag);

	SByte checksum = 0;
	SByte firstkey = rand() >> 2;
	SByte secondkey = rand() >> 2;
	easyrandnext = firstkey + secondkey << 8;

	if(ofs)
	{
		ofs.put(firstkey);
		ofs.put(secondkey);

		vector<SByte>::iterator itr = data.begin();
		for(;itr!=data.end();itr++)
		{
			checksum ^= *itr;
			ofs.put(CIPHERKEY[easyrand()&0x0F] ^ *itr);
		}

		easyrandnext = checksum;
		ofs.put(static_cast<SByte>(easyrand()));
		ofs.close();
	}
	else
	{
		return ERROR_FM_FILE_ERROR;
	}

	return 0;
}
int cFileManager::loadEncryptFile(const tstring& filename, std::vector<SByte>& data)
{
	unsigned flag = std::ios_base::binary | std::ios_base::in;

	tstring truefilename = filename;
	if(filename.substr( filename.size() - 5 ) != _T(".cdat"))
	{//拡張子cdatじゃないときはつける
		truefilename += _T(".cdat");
	}

	ifstream ifs(truefilename.c_str(), flag);

	
	if(ifs)
	{
		ifs.seekg(0, std::ios::end );
		int size = ifs.tellg(); 
		ifs.seekg(0, std::ios::beg ); 


		data.resize(size-3);
		//SByte c;

		SByte checksum = 0;
		SByte firstkey;
		SByte secondkey;
		SByte databyte;
		ifs.get(firstkey);
		ifs.get(secondkey);

		easyrandnext = firstkey + secondkey << 8;

		vector<SByte>::iterator itr = data.begin();
		for(;itr!=data.end();itr++)
		{//読み込み
			ifs.get(databyte);
			databyte ^= CIPHERKEY[easyrand()&0x0F];
			*itr = databyte;
			checksum ^= databyte;
		}

		SByte checksumfile;
		ifs.get(checksumfile);
		easyrandnext = checksum;
		if(checksumfile != static_cast<SByte>(easyrand()))
		{//壊れてるよ
			data.clear();
			return ERROR_FM_FILE_ERROR;
		}
    }
	else
	{//開けられない無い！
		//もしかして「生」　危険？
		//if(loadFile(filename,data) != SUCCESS)
		//{
			return ERROR_FM_FILE_ERROR;
		//}
		//return 0;
	}

    ifs.close();  

	return 0;
}


int cFileManager::saveFile(const tstring& filename, unsigned flag, std::vector<tstring>& data)
{
	oftstream ofs(filename.c_str(), flag);
	
	if(ofs)
	{
		vector<tstring>::iterator itr = data.begin();
		for(;itr!=data.end();itr++)
		{
			ofs << *itr;
		}

		ofs.close();
	}
	else
	{
		return ERROR_FM_FILE_ERROR;
	}

	return 0;
}


int cFileManager::loadFile(const tstring& filename, unsigned flag, std::vector<tstring>& data)
{
	std::vector<SByte> binalydata;
	int error = 0;
	if(error |= loadFile(filename.c_str(), binalydata)) return error;

	int charcode = CHARCODE_SJIS;
	if(binalydata.size() >= 2)
	{
		if(binalydata[0] == (SByte)0xFF && binalydata[1] == (SByte)0xFE)
		{
			charcode = CHARCODE_UNICODE_LE;
		}
		else if(binalydata[0] == (SByte)0xFE && binalydata[1] == (SByte)0xFF)
		{
			charcode = CHARCODE_UNICODE_BE;
		}
		else
		{
			charcode = CHARCODE_SJIS;
		}
	}
	//Unicode

	if(charcode == CHARCODE_UNICODE_LE || charcode == CHARCODE_UNICODE_BE)
	{
		decode_Unicode(binalydata, data, charcode);
	}
	else if(charcode == CHARCODE_SJIS)
	{
		decode_SJIS(binalydata, data);
	}
	else
	{
		return VERROR;
	}
	/*
	union
	{
		SByte line_t[1024];
		wchar_t line_wt[512];
	} lines;
	data.clear();

	while( !ifs.eof() ){
		
		//ifs.getline(lines.line_t,sizeof(lines.line_t));
		ifs.fread(lines.line_t,sizeof(lines.line_t));
		
		
		data.push_back((tstring)lines.line_wt+_T("\n"));
	}
*/


	return error;
}

int cFileManager::decode_Unicode(const std::vector<SByte>& data, std::vector<tstring>& stringdata, const int endian)
{
	unsigned int size = data.size();

	stringdata.clear();

	wstring line;

	//CHARCODE_UNICODE_BEの場合をデフォルトとする。
	int firstbit = 8;
	int secondbit = 0;
	if(endian == CHARCODE_UNICODE_LE)
	{
		firstbit = 0;
		secondbit = 8;
	}

	int i;

	//BOM飛ばして2から
	for(i=2;i<size;i+=2)
	{
		wchar_t wchar = 0;
		int newlineflag = 0;
		wchar = ( ((UByte)data[i]) << firstbit | ((UByte)data[i+1]) << secondbit);
		
		
		if(wchar == (wchar_t)0x000D)
		{//改行コードCR
			newlineflag = 1;
		}
		else if(wchar == (wchar_t)0x000A)
		{//改行コードLF
			newlineflag = 1;
		}
		else
		{
			line.push_back(wchar);
		}

		if(newlineflag)
		{
			if(!line.empty())
			{
				//line.push_back((wchar_t)0x0000);
				line.push_back(L'\n');
#ifdef UNICODE
				stringdata.push_back(line);
#else
				CStringA str(line.c_str());
				stringdata.push_back(str.GetString());
#endif

				line.clear();
			}
		}
	}

			if(!line.empty())
			{
				//line.push_back((wchar_t)0x0000);
				line.push_back(L'\n');
#ifdef UNICODE
				stringdata.push_back(line);
#else
				CStringA str(line.c_str());
				stringdata.push_back(str.GetString());
#endif

				line.clear();
			}

	return 0;
}

int cFileManager::decode_SJIS(const std::vector<SByte>& data, std::vector<tstring>& stringdata)
{
	unsigned int size = data.size();

	stringdata.clear();

	string line;

	int i;

	//BOM飛ばして2から
	for(i=0;i<size;i++)
	{
		int newlineflag = 0;
		
		if(data[i] == (char)0x0D)
		{//改行コードCR
			newlineflag = 1;
		}
		else if(data[i] == (char)0x0A)
		{//改行コードLF
			newlineflag = 1;
		}
		else
		{
			line.push_back(data[i]);
		}

		if(newlineflag)
		{
			if(!line.empty())
			{
				line.push_back('\n');
#ifdef UNICODE
				CStringW str(line.c_str());
				stringdata.push_back(str.GetString());
#else
				stringdata.push_back(line);
#endif

				line.clear();
			}
		}
	}

			if(!line.empty())
			{
				line.push_back('\n');
#ifdef UNICODE
				CStringW str(line.c_str());
				stringdata.push_back(str.GetString());
#else
				stringdata.push_back(line);
#endif

				line.clear();
			}

	return 0;

}

int cFileManager::getLastWriteTime(const tstring& filename, FileTime* lastWriteTime)
{
	// ファイル情報
	WIN32_FIND_DATA findData;

	// ファイル情報取得
	HANDLE hFile = FindFirstFile(filename.c_str(), &findData);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		// 失敗(不正パス)
		return ERROR_FM_FILE_ERROR;
	}
	else
	{
		// ファイル検索ハンドル閉じる
		FindClose(hFile);

		// 更新日時
		FileTimeToLocalFileTime(&findData.ftLastWriteTime, reinterpret_cast<FILETIME*>(lastWriteTime));
		return SUCCESS;
	}
}

int cFileManager::compareLastWriteTime(const tstring& filename, const FileTime oldLastWriteTime, FileTime* newLastWriteTime, bool* isRenew)
{
	// ファイル情報
	WIN32_FIND_DATA findData;

	// ファイル時間
	FILETIME fileTime;
	unsigned long lastWriteTime_low_new = 0;

	// ファイル情報取得
	HANDLE hFile = FindFirstFile(filename.c_str(), &findData);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		// 失敗(不正パス)
		return ERROR_FM_FILE_ERROR;
	}
	else
	{
		// 成功
		// ファイル検索ハンドル閉じる
		FindClose(hFile);

		// 更新日時
		FileTimeToLocalFileTime(&findData.ftLastWriteTime, reinterpret_cast<FILETIME*>(newLastWriteTime));
		if(oldLastWriteTime.dwHighDateTime != newLastWriteTime->dwHighDateTime
			|| oldLastWriteTime.dwLowDateTime != newLastWriteTime->dwLowDateTime)
		{
			//更新された
			*isRenew = true;
		}
		else
		{
			//更新されてない
			*isRenew = false;
		}

		return SUCCESS;
	}
}