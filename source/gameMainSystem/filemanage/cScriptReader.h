#pragma once

#include "fileManage_ph.h"

#include <tstring_ph.h>
using namespace std;
#include <vector>

class cScriptReader
{
public:
	cScriptReader(void);
	cScriptReader(tstring scriptname);
	void constsub();
public:
	virtual ~cScriptReader(void);
public:
	//基本インターフェイス
	virtual int save(void);
	virtual int load(void);

	virtual int savepacked(void);
	virtual int loadpacked(void);

	virtual int loadAndpacked(void);//アンリリースモード

	virtual std::vector<tstring> & codes(){return codes_;};

	virtual void setscriptname(const tstring scriptname){scriptname_ = scriptname;};
	virtual const tstring & scriptname(){return scriptname_;};
	virtual const tstring & scriptdir(){return scriptdir_;};
	virtual const tstring & filename(){return scriptfilename_;};
public:
	//**拡張インターフェイス
	virtual pcScriptRLayer pdata(){return pcsl_;};
	virtual cScriptRLayer& data(){return *pcsl_;};

	virtual int simpleload(void);//デコードしないロード

	virtual void addErrorMassage(tstring em){errormassage_.push_back(em);};
	
	virtual std::vector<tstring>& errormassage(){return errormassage_;};
	virtual int outputerrormassage(tstring& filename);

protected:
	//**拡張内部インターフェイス

	//初期デコード
	virtual int decode(void);
	virtual int decode_roop( std::vector<tstring>& codes, std::vector<pcScriptRLayer>& vlayer, tstring& codename, unsigned int flag);
	//scriptname_からscriptdir_の計算
	virtual void makedirpass(tstring& filename);


	virtual void addError(pcScriptRLayer layer, tstring& em);
	virtual void addError(pcScriptRLayer layer, tstring& errorfile, int line, tstring& errorcode, tstring& em);
protected:
	//**拡張内部インターフェイス内関数
	virtual int decode_newAndDownLayer(std::vector<pcScriptRLayer>& vlayer, tstring& name);
	virtual int decode_UpLayer(std::vector<pcScriptRLayer>& vlayer);
	virtual int decode_FileOpen(std::vector<pcScriptRLayer>& vlayer, tstring& name, unsigned int flag);
	virtual int decode_newDefineMacro(std::vector<pcScriptRLayer>& vlayer, std::vector<tstring>& codes, std::vector<tstring>::iterator& itrstr, tstring& name, int &line);
	virtual int decode_Macro(std::vector<pcScriptRLayer>& vlayer, tstring& name, unsigned int flag);
	
	virtual int decode_Comment(std::vector<pcScriptRLayer>& vlayer, std::vector<tstring>& codes, std::vector<tstring>::iterator& itrstr, tstring& name, int &line);

	virtual int decode_Reference(std::vector<pcScriptRLayer>& vlayer, tstring& name);

	virtual int decode_Matrix(std::vector<pcScriptRLayer>& vlayer, tstring& sepcode);

	virtual int decode_LuaScript(std::vector<pcScriptRLayer>& vlayer, tstring& filepass, tstring& filename);

	virtual int decode_LuaDependScript(std::vector<pcScriptRLayer>& vlayer, tstring& filepass, tstring& filename);

protected:
	//基本メンバ

	std::vector<tstring> codes_;

	//このファイルのパス
	tstring scriptname_;

	//このファイルのディレクトリ
	tstring scriptdir_;

	//このファイルのファイルネーム
	tstring scriptfilename_;

	pcFileManager pcfm_;
protected:
	//**拡張メンバ
	//レイヤー
	pcScriptRLayer pcsl_;
	std::vector<tstring> errormassage_;

	//エラーコード用
	//unsigned line_;
	//tstring codename_;
protected:
	tstring matrix_sepcode_;
};

