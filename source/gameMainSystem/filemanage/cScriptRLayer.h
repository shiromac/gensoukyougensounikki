#pragma once


#include "fileManage_ph.h"
#include <map>
#include <vector>
#include "tstring_ph.h"



class cScriptRLayer
{
public:
	cScriptRLayer(void);
public:
	virtual ~cScriptRLayer(void);

	std::vector<tstring>& script(){return script_;};
	
	tstring topscript(){
		if(script_.empty()) return _T("");
		else return script_[0];
	};
	
	int savedata(std::vector<SByte>& data);
	int loaddata(const std::vector<SByte>& data, pcScriptRLayer me);

	std::vector<tstring>*& messageOutputTo(){return messageOutputTo_;};
	void AllsetmessageOutputTo(std::vector<tstring>* pmessageOutputTo);//下層に伝播する

	//メンバ系
	pcScriptRLayer pmember(const tstring& label);
	cScriptRLayer& member(const tstring& label);
	cScriptRLayer& operator[](const tstring& label){return member(label);};//ホントは**なんだけども便宜上これで
	
	//階層をパスの形式で移動する(1階層しか動かない)
	pcScriptRLayer path2Layer(const tstring& path);

	//階層をさかのぼってメンバを検索する
	pcScriptRLayer findpmember(const tstring& label);
	//パレント系
	pcScriptRLayer pparent();
	cScriptRLayer& parent();

	//マクロ
	std::vector<tstring>* pmacro(const tstring& label);
	//階層をさかのぼってマクロを検索する
	std::vector<tstring>* findpmacro(const tstring& label);

	//コンテンツ
	pLuaScript pcontent(const tstring& label);
	void getLuaScriptKeys(std::vector<tstring>& keys);

	//名前
	virtual tstring& name(){return name_;};
	virtual tstring nameAllPass();

	pcScriptRLayer addMember(const tstring& label, pcScriptRLayer parent);
	int eraseMember(const tstring& label);

	std::vector<tstring>* addMacro(const tstring& label);
	int eraseMacro(const tstring& label);

	int addContent(const tstring& label, pLuaScript content);
	int eraseContent(const tstring& label);

	std::map<tstring,pcScriptRLayer> & membermap(){return member_;};
	std::vector<tstring> memberKeyList();
protected:
	std::vector<tstring> script_;//データ
	std::map<tstring,pcScriptRLayer> member_;//子メンバ
	std::map<tstring,pLuaScript> content_;//特殊コンテンツ
	wpcScriptRLayer parent_;//親メンバ

	tstring name_;

	//マクロ
	std::map<tstring,std::vector<tstring>> macro_;//マクロ
	

	std::vector<tstring>* messageOutputTo_;
};

//ヌル参照用ダミー
static cScriptRLayer NULLrefScriptRLayer;

//デコードユーティリティ
void afterdecode_table(std::vector<tstring> & script,const TCHAR* delim, pcScriptRLayer ptargetsr);


int ConvertT2VecC(const pcScriptRLayer& tmpl, std::vector<SByte>& data);
int DecodeVecC2T(const std::vector<SByte>& data, pcScriptRLayer& tmpl);
inline int BackDecodeVecC2T(pcScriptRLayer& tmpl, const std::vector<SByte>& data)
{return DecodeVecC2T(data, tmpl);};
