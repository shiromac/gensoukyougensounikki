#pragma once

#include "fileManage_ph.h"

#include <vector>

//vector<SByte>とバイナリファイルのインターフェイス
//詳しいエラー未実装11/03
//バグ修正　11/20


class cFileManager
{
public:
	cFileManager(void);
public:
	virtual ~cFileManager(void);

	//フラグ指定モード
	virtual int saveFile(const tstring& filename, const unsigned flag, std::vector<SByte>& data);
	virtual int loadFile(const tstring& filename, const unsigned flag, std::vector<SByte>& data);

	virtual int saveFile(const tstring& filename, const unsigned flag, std::vector<tstring>& data);
	virtual int loadFile(const tstring& filename, const unsigned flag, std::vector<tstring>& data);
public:
	//返り値：成功したら SUCCESS (0)
	static int getLastWriteTime(const tstring& filename, FileTime* lastWriteTime);
	
	//返り値：成功したら SUCCESS (0)
	//oldとnewは同一でも良い
	//oldLastWriteTimeは参照にしないこと
	static int compareLastWriteTime(const tstring& filename, const FileTime oldLastWriteTime, FileTime* newLastWriteTime, bool* isRenew);
public:
	//フラグ指定しないモード
	inline int saveFile(const tstring& filename, std::vector<SByte>& data, unsigned flag = std::ios_base::binary | std::ios_base::out | std::ios_base::trunc)
	{return saveFile(filename, flag, data);};
	inline int loadFile(const tstring& filename, std::vector<SByte>& data, unsigned flag = std::ios_base::binary | std::ios_base::in)
	{return loadFile(filename, flag, data);};

	inline int saveFile(const tstring& filename, std::vector<tstring>& data, unsigned flag = std::ios_base::out | std::ios_base::trunc)
	{return saveFile(filename, flag, data);};
	inline int loadFile(const tstring& filename, std::vector<tstring>& data, unsigned flag = std::ios_base::in)
	{return loadFile(filename, flag, data);};

	virtual int saveEncryptFile(const tstring& filename, std::vector<SByte>& data);//勝手に.cdatをつける！
	virtual int loadEncryptFile(const tstring& filename, std::vector<SByte>& data);//勝手に.cdatをつける！

protected:

	virtual int decode_Unicode(const std::vector<SByte>& data, std::vector<tstring>& stringdata, const int endian);
	virtual int decode_SJIS(const std::vector<SByte>& data, std::vector<tstring>& stringdata);

};
