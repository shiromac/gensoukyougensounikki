#include "cFileManager.h"

#include "../../utility/ATLcompati/CString.h"
#ifndef __EMSCRIPTEN__
#include <windows.h>
#else
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <emscripten/emscripten.h>
#endif

using namespace std;

using namespace ATLcompati;

const int CHARCODE_UNKNOWN = 0;
const int CHARCODE_SJIS = 1;
const int CHARCODE_UNICODE_BE = 2;
const int CHARCODE_UNICODE_LE = 3;
#ifdef __EMSCRIPTEN__
static void AppendUtf8CodePoint(string& output, unsigned long code)
{
	if(code <= 0x7f)
	{
		output.push_back(static_cast<char>(code));
	}
	else if(code <= 0x7ff)
	{
		output.push_back(static_cast<char>(0xc0 | ((code >> 6) & 0x1f)));
		output.push_back(static_cast<char>(0x80 | (code & 0x3f)));
	}
	else if(code <= 0xffff)
	{
		output.push_back(static_cast<char>(0xe0 | ((code >> 12) & 0x0f)));
		output.push_back(static_cast<char>(0x80 | ((code >> 6) & 0x3f)));
		output.push_back(static_cast<char>(0x80 | (code & 0x3f)));
	}
	else
	{
		output.push_back('?');
	}
}

static string TStringToUtf8(const tstring& value)
{
	string output;
	for(size_t i = 0; i < value.size(); i++)
	{
		if(sizeof(TCHAR) == 1)
		{
			output.push_back(static_cast<char>(value[i]));
		}
		else
		{
			AppendUtf8CodePoint(output, static_cast<unsigned long>(value[i]));
		}
	}
	return output;
}

static string TStringToBrowserPath(const tstring& value)
{
	string output = TStringToUtf8(value);
	for(size_t i = 0; i < output.size(); i++)
	{
		if(output[i] == '\\') output[i] = '/';
	}
	return output;
}

static void OpenBrowserInputFile(ifstream& stream, const tstring& filename, unsigned flag)
{
	string path = TStringToBrowserPath(filename);
	stream.open(path.c_str(), static_cast<std::ios_base::openmode>(flag));
}

static void OpenBrowserOutputFile(ofstream& stream, const tstring& filename, unsigned flag)
{
	string path = TStringToBrowserPath(filename);
	stream.open(path.c_str(), static_cast<std::ios_base::openmode>(flag));
}

static void BrowserSyncPersistentStorage()
{
	EM_ASM({
		if (typeof FS === 'undefined' || !Module['ggnSaveMounted']) return;
		if (Module['ggnSaveSyncRunning']) {
			Module['ggnSaveSyncAgain'] = 1;
			return;
		}

		var runSync = function() {
			Module['ggnSaveSyncRunning'] = 1;
			FS.syncfs(false, function(err) {
				if (err && typeof console !== 'undefined') console.error('save sync failed', err);
				Module['ggnSaveSyncRunning'] = 0;
				if (Module['ggnSaveSyncAgain']) {
					Module['ggnSaveSyncAgain'] = 0;
					runSync();
				}
			});
		};
		runSync();
	});
}
#endif
#ifdef UNICODE
static tstring SjisToTString(const string& line)
{
	if(line.empty()) return tstring();

#ifndef __EMSCRIPTEN__
	int length = ::MultiByteToWideChar(932, 0, line.c_str(), static_cast<int>(line.size()), NULL, 0);
	if(length <= 0)
	{
		CStringW fallback(line.c_str());
		return fallback.GetString();
	}

	tstring output;
	output.resize(length);
	::MultiByteToWideChar(932, 0, line.c_str(), static_cast<int>(line.size()), &output[0], length);
	return output;
#else
	tstring output;
	output.reserve(line.size());
	for(size_t i = 0; i < line.size(); i++)
	{
		output.push_back(static_cast<unsigned char>(line[i]));
	}
	return output;
#endif
}
#endif

//#include <atlstr.h>
//using namespace ATL;

cFileManager::cFileManager(void)
{
}

cFileManager::~cFileManager(void)
{
}

static void EnsureParentDirectory(const tstring& filename)
{
	size_t pos = filename.find_last_of(_T("\\/"));
	if(pos == tstring::npos) return;

	tstring directory = filename.substr(0, pos);
	if(directory.empty()) return;

#ifdef __EMSCRIPTEN__
	string browserDirectory = TStringToBrowserPath(directory);
	size_t start = 0;
	while(start < browserDirectory.size())
	{
		size_t next = browserDirectory.find('/', start);
		if(next == string::npos) break;
		if(next > 0)
		{
			string partial = browserDirectory.substr(0, next);
			if(!partial.empty()) mkdir(partial.c_str(), 0777);
		}
		start = next + 1;
	}
	mkdir(browserDirectory.c_str(), 0777);
#else
	for(size_t i = 0; i < directory.size(); i++)
	{
		if(directory[i] == _T('/')) directory[i] = _T('\\');
	}

	size_t start = 0;
	while(start < directory.size())
	{
		size_t next = directory.find(_T('\\'), start);
		if(next == tstring::npos) break;
		if(next > 0)
		{
			tstring partial = directory.substr(0, next);
			if(!(partial.size() == 2 && partial[1] == _T(':')))
			{
				::CreateDirectory(partial.c_str(), NULL);
			}
		}
		start = next + 1;
	}

	::CreateDirectory(directory.c_str(), NULL);
#endif
}
int cFileManager::saveFile(const tstring& filename, unsigned flag, std::vector<SByte>& data)
{//バイナリ
	EnsureParentDirectory(filename);
	#ifdef __EMSCRIPTEN__
	ofstream ofs;
	OpenBrowserOutputFile(ofs, filename, flag);
#else
	ofstream ofs(filename.c_str(), flag);
#endif
	
	if(ofs)
	{
		vector<SByte>::iterator itr = data.begin();
		for(;itr!=data.end();itr++)
		{
			ofs.put(*itr);
		}

		ofs.close();
#ifdef __EMSCRIPTEN__
		BrowserSyncPersistentStorage();
#endif
	}
	else
	{//開けられない無い！
		return ERROR_FM_FILE_ERROR;
	}

	return 0;
}
int cFileManager::loadFile(const tstring& filename, unsigned flag, std::vector<SByte>& data)
{//バイナリ
    #ifdef __EMSCRIPTEN__
    ifstream ifs;
    OpenBrowserInputFile(ifs, filename, flag);
#else
    ifstream ifs(filename.c_str(), flag);
#endif

	
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

	EnsureParentDirectory(truefilename);
	#ifdef __EMSCRIPTEN__
	ofstream ofs;
	OpenBrowserOutputFile(ofs, truefilename, flag);
#else
	ofstream ofs(truefilename.c_str(), flag);
#endif

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
#ifdef __EMSCRIPTEN__
		BrowserSyncPersistentStorage();
#endif
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

	#ifdef __EMSCRIPTEN__
	ifstream ifs;
	OpenBrowserInputFile(ifs, truefilename, flag);
#else
	ifstream ifs(truefilename.c_str(), flag);
#endif

	
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
	EnsureParentDirectory(filename);
	#ifdef __EMSCRIPTEN__
	ofstream ofs;
	OpenBrowserOutputFile(ofs, filename, flag);
#else
	oftstream ofs(filename.c_str(), flag);
#endif
	
	if(ofs)
	{
		vector<tstring>::iterator itr = data.begin();
		for(;itr!=data.end();itr++)
		{
			#ifdef __EMSCRIPTEN__
			ofs << TStringToUtf8(*itr);
#else
			ofs << *itr;
#endif
		}

		ofs.close();
#ifdef __EMSCRIPTEN__
		BrowserSyncPersistentStorage();
#endif
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

	tstring line;

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
		TCHAR wchar = 0;
		int newlineflag = 0;
		wchar = ( ((UByte)data[i]) << firstbit | ((UByte)data[i+1]) << secondbit);
		
		
		if(wchar == (TCHAR)0x000D)
		{//改行コードCR
			newlineflag = 1;
		}
		else if(wchar == (TCHAR)0x000A)
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
				line.push_back(_T('\n'));
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
				line.push_back(_T('\n'));
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
				stringdata.push_back(SjisToTString(line));
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
				stringdata.push_back(SjisToTString(line));
#else
				stringdata.push_back(line);
#endif

				line.clear();
			}

	return 0;

}

int cFileManager::getLastWriteTime(const tstring& filename, FileTime* lastWriteTime)
{
#ifdef __EMSCRIPTEN__
	struct stat status;
	string path = TStringToBrowserPath(filename);
	if(stat(path.c_str(), &status) != 0)
	{
		return ERROR_FM_FILE_ERROR;
	}

	unsigned long long value = static_cast<unsigned long long>(status.st_mtime);
	lastWriteTime->dwLowDateTime = static_cast<unsigned long>(value & 0xffffffffUL);
	lastWriteTime->dwHighDateTime = static_cast<unsigned long>((value >> 32) & 0xffffffffUL);
	return SUCCESS;
#else
	WIN32_FIND_DATA findData;
	HANDLE hFile = FindFirstFile(filename.c_str(), &findData);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return ERROR_FM_FILE_ERROR;
	}
	else
	{
		FindClose(hFile);
		FileTimeToLocalFileTime(&findData.ftLastWriteTime, reinterpret_cast<FILETIME*>(lastWriteTime));
		return SUCCESS;
	}
#endif
}

int cFileManager::compareLastWriteTime(const tstring& filename, const FileTime oldLastWriteTime, FileTime* newLastWriteTime, bool* isRenew)
{
#ifdef __EMSCRIPTEN__
	int error = getLastWriteTime(filename, newLastWriteTime);
	if(error != SUCCESS) return error;
	*isRenew = (oldLastWriteTime.dwHighDateTime != newLastWriteTime->dwHighDateTime
		|| oldLastWriteTime.dwLowDateTime != newLastWriteTime->dwLowDateTime);
	return SUCCESS;
#else
	WIN32_FIND_DATA findData;
	FILETIME fileTime;
	unsigned long lastWriteTime_low_new = 0;
	HANDLE hFile = FindFirstFile(filename.c_str(), &findData);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return ERROR_FM_FILE_ERROR;
	}
	else
	{
		FindClose(hFile);
		FileTimeToLocalFileTime(&findData.ftLastWriteTime, reinterpret_cast<FILETIME*>(newLastWriteTime));
		if(oldLastWriteTime.dwHighDateTime != newLastWriteTime->dwHighDateTime
			|| oldLastWriteTime.dwLowDateTime != newLastWriteTime->dwLowDateTime)
		{
			*isRenew = true;
		}
		else
		{
			*isRenew = false;
		}

		return SUCCESS;
	}
#endif
}
