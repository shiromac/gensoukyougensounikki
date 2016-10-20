#include "cScriptRLayer.h"

#include "../../utility/StyleString.h"

#include "cDataConverter.h"
#include <LuaUtility/LuaScript.h>

#include <assert.h>
using namespace std;
cScriptRLayer::cScriptRLayer(void)
{
}

cScriptRLayer::~cScriptRLayer(void)
{
}

pcScriptRLayer cScriptRLayer::pmember(const tstring& label)
{
	pcScriptRLayer pcsr;
	map<tstring,pcScriptRLayer>::iterator itr;
	itr = member_.find(label);

	if(itr == member_.end())
	{
		//assert(!_T("cScriptRLayer::pmember内で不適切な参照"));
		return NULLOFcScriptRLayer;
	}

	return itr->second;
};

pcScriptRLayer cScriptRLayer::pparent()
{
	return parent_.lock();
};
cScriptRLayer& cScriptRLayer::parent()
{
	pcScriptRLayer ppr = pparent();
	if(ppr == NULL)
	{
		return NULLrefScriptRLayer;
	}
	else
	{
		return *ppr;
	}
};

//階層をパスの形式で移動する
pcScriptRLayer cScriptRLayer::path2Layer(const tstring& path)
{
	if(path == _T(".."))
	{
		return pparent();
	}
	else
	{
		return pmember(path);
	}
}

cScriptRLayer& cScriptRLayer::member(const tstring& label)
{
	pcScriptRLayer pm = pmember(label);
	if(pm == NULLOFcScriptRLayer) return NULLrefScriptRLayer;
	return *pm;
}

pcScriptRLayer cScriptRLayer::findpmember(const tstring& label)
{
	pcScriptRLayer psr = pmember(label);
	pcScriptRLayer parentpsr;
	if(psr != NULL)
	{
		return psr;
	}

	parentpsr = pparent();

	for(;;)
	{
		if(parentpsr == NULL)
		{
			break;
		}

		psr = parentpsr->pmember(label);
		if(psr != NULL)
		{
			return psr;
		}
	
		parentpsr = parentpsr->pparent();
	}

	return NULLOFcScriptRLayer;
}

vector<tstring>* cScriptRLayer::pmacro(const tstring& label)
{

	map<tstring,vector<tstring>>::iterator itr;
	itr = macro_.find(label);

	if(itr == macro_.end())
	{
		//assert(!_T("cScriptRLayer::macro内で不適切な参照"));
		return NULL;
	}

	return &(itr->second);
}
vector<tstring>* cScriptRLayer::findpmacro(const tstring& label)
{
	pcScriptRLayer psr;
	vector<tstring>* pm = pmacro(label);

	if(pm != NULL)
	{
		return pm;
	}

	psr = pparent();

	for(;;)
	{
		if(psr == NULL)
		{
			break;
		}

		pm = psr->pmacro(label);
		if(pm != NULL)
		{
			return pm;
		}
	
		psr = psr->pparent();
	}

	return NULL;
}

pLuaScript cScriptRLayer::pcontent(const tstring& label)
{
	map<tstring,pLuaScript>::iterator itr;
	itr = content_.find(label);

	if(itr == content_.end())
	{
		//assert(!_T("cScriptRLayer::macro内で不適切な参照"));
		return NULLOFLuaScript;
	}

	return (itr->second);
}

pLuaScript cScriptRLayer::pdependContent(const tstring& label)
{
	map<tstring,pLuaScript>::iterator itr;
	itr = dependContent_.find(label);

	if(itr == dependContent_.end())
	{
		//assert(!_T("cScriptRLayer::macro内で不適切な参照"));
		return NULLOFLuaScript;
	}

	return (itr->second);
}

void cScriptRLayer::getDependLuaScriptKeys(std::vector<tstring>& keys)
{
	map<tstring,pLuaScript>::iterator itr = dependContent_.begin();
	for(;;)
	{
		if(itr == dependContent_.end())
		{
			return;
		}

		keys.push_back(itr->first);

		itr++;
	}

}
/*
pLuaScript cScriptRLayer::ptopcontent()
{
	if(content_.empty())
	{
		return NULLOFLuaScript;
	}

	std::map<tstring,pLuaScript>::iterator itr = content_.begin();
	itr = content_.find(itr->first);
	return (itr->second);
}
*/
vector<tstring>* cScriptRLayer::addMacro(const tstring& label)
{
	vector<tstring> temp;
	vector<tstring>* pvstr;
	if((macro_.insert(pair<tstring, vector<tstring>>(label,temp)).second) == false)
	{//しっぱい
		return NULL;
	}
	return pmacro(label);
}
int cScriptRLayer::eraseMacro(const tstring& label)
{
	if((macro_.erase(label)))
	{//成功
		return true;
	}
	return false;
}

int cScriptRLayer::addContent(const tstring& label, pLuaScript content)
{
	if((content_.insert(pair<tstring, pLuaScript>(label,content)).second) == false)
	{//しっぱい
		return false;
	}
	return true;
}
int cScriptRLayer::eraseContent(const tstring& label)
{
	if((content_.erase(label)))
	{//成功
		return true;
	}
	return false;
}

int cScriptRLayer::addDependContent(const tstring& label, pLuaScript content)
{
	if((dependContent_.insert(pair<tstring, pLuaScript>(label,content)).second) == false)
	{//しっぱい
		return false;
	}
	return true;
}
int cScriptRLayer::eraseDependContent(const tstring& label)
{
	if((dependContent_.erase(label)))
	{//成功
		return true;
	}
	return false;
}

pcScriptRLayer cScriptRLayer::addMember(const tstring& label, pcScriptRLayer parent)
{
	pcScriptRLayer pcsr = pcScriptRLayer(new cScriptRLayer);
	if((member_.insert(pair<tstring, pcScriptRLayer>(label,pcsr))).second == false)
	{//しっぱい
		return NULLOFcScriptRLayer;
	}
	pcsr->messageOutputTo() = messageOutputTo_;
	pcsr->parent_ = wpcScriptRLayer(parent);
	pcsr->name() = label;
	return pcsr;
}
int cScriptRLayer::eraseMember(const tstring& label)
{
	if((member_.erase(label)))
	{//成功
		return true;
	}
	return false;
}

tstring cScriptRLayer::nameAllPass()
{
	vector<pcScriptRLayer> vpcsr;
	tstring namepass;

	if(pparent() != NULLOFcScriptRLayer)
	{
		vpcsr.push_back(pparent());

		for(;;)
		{
			if(vpcsr.back()->pparent() == NULLOFcScriptRLayer)
			{
				break;
			}
			else
			{
				vpcsr.push_back(vpcsr.back()->pparent());
			}
		}
	}



	int i;
	for(i=vpcsr.size()-1; i>=0;i--)
	{
		namepass += vpcsr[i]->name();
		namepass += _T("/");
	}
	namepass += name();
	namepass += _T("/");
	return namepass;
}

void cScriptRLayer::getLuaScriptKeys(std::vector<tstring>& keys)
{
	vector<tstring>& vstr = script();
	vector<tstring>::iterator itrstr = vstr.begin();
	tstring::iterator itrchar;

	for(;itrstr!=vstr.end();itrstr++)
	{
		if(itrstr->empty()) continue;//空行

		itrchar = itrstr->begin();
		for(; itrchar != itrstr->end(); itrchar++)
		{//文字列の最初から
			if(*itrchar == _T(' ') || *itrchar == _T('\t'))
			{//ホワイトスペース類
				continue;
			}
			else
			{
				break;
			}
		}

		//先頭ホワイトスペース類を排除した




		for(; itrchar != itrstr->end();)
		{
			bool command = false;

			if(*itrchar == _T('#'))
			{//コマンド
				command = true;
				itrchar++;
				for(; itrchar != itrstr->end();)
				{
					
					if(*itrchar == _T('!'))
					{//スクリプト
						itrchar++; if(itrchar == itrstr->end()) break;
						tstring op(itrchar,itrstr->end());
						
						keys.push_back(op);
					}
					
					if(itrchar != itrstr->end()) itrchar++;
				}
			}

			if(!command)
			{//コマンド以外
				itrchar = itrstr->end(); break;
			}

			if(itrchar != itrstr->end()) itrchar++;
		}
		
	}

}

//デコードユーティリティ
void afterdecode_table(std::vector<tstring> & script,const TCHAR* delim, pcScriptRLayer ptargetsr)
{


	vector<vector<tstring>> vvstr;
	split(script,delim,vvstr);

	vector<tstring> columnstr;//列要素
	pcScriptRLayer prowsr;//行レイヤー
	
	if(vvstr.empty()) return;

	int x,y;
	for(x=0;x<vvstr[0].size();x++)
	{
		columnstr.push_back(trimstring(vvstr[0][x]));
	}

	for(y=1;y<vvstr.size();y++)
	{
		
		tstring rowstr = trimstring(vvstr[y][0]);
		if(rowstr != _T(""))
		{
			prowsr = ptargetsr->pmember(rowstr);
			if(prowsr == NULL)
			{
				prowsr = ptargetsr->addMember(rowstr,ptargetsr);
			}
		}
		

		int size = min(columnstr.size(),vvstr[y].size());
		for(x=1;x<size;x++)
		{
			tstring elementstr;
			trimstring(vvstr[y][x],elementstr);
			if(elementstr == _T("")) continue;
			if(columnstr[x].find(_T("$/")) == 0) continue;//コメント列

			pcScriptRLayer pelementsr = prowsr->pmember(columnstr[x]);
			if(pelementsr == NULL)
			{
				pelementsr = prowsr->addMember(columnstr[x],pelementsr);
			}

			if(elementstr.find(_T("$")) == 0)
			{
				/*if(elementstr.find("$*") == 0)
				{
					vector<tstring>* pvstr = prowsr->findpmacro(elementstr);
					if(pvstr != NULL)
						pelementsr->script().insert(pelementsr->script().end(),pvstr->begin(),pvstr->end());
				}
				else*/ if(elementstr.find(_T("$?")) == 0)
				{
					pcScriptRLayer pmemsr = prowsr->findpmember(tstring(elementstr,2,elementstr.size()-2));
					if(pmemsr != NULL)
						pelementsr->script().insert(pelementsr->script().end(),pmemsr->script().begin(),pmemsr->script().end());
					continue;
				}
				else if(elementstr.find(_T("$/")) == 0)
				{
					continue;
				}
			}

			pelementsr->script().push_back(elementstr);
		}
	}



}

int cScriptRLayer::savedata(std::vector<SByte>& data)
{
	vector<vector<SByte>> vvdata;
	vvdata.resize(5);

	cDataConverter::ConvertVT2VecVecC2VecC(script_,vvdata[0]);
	cDataConverter::ConvertMTT2VecVecC2VecC(member_,vvdata[1]);
	cDataConverter::ConvertT2VecC(name_,vvdata[2]);
	cDataConverter::ConvertMTVT2VecVecC2VecC(macro_,vvdata[3]);

	//追加
	cDataConverter::ConvertMTT2VecVecC2VecC(content_,vvdata[4]);

	cDataConverter::VecVecC2VecC(vvdata,data);

	return SUCCESS;
}
int cScriptRLayer::loaddata(const std::vector<SByte>& data, pcScriptRLayer me)
{
	
	vector<vector<SByte>> vvdata;
	cDataConverter::VecC2VecVecC(data,vvdata);
	if(vvdata.size() < 4) return ERROR_FM_LOAD_DECODE_ERROR;

	cDataConverter::BackDecodeVecC2VecVecC2VT(script_,vvdata[0]);
	cDataConverter::BackDecodeVecC2VecVecC2MTT(member_,vvdata[1]);
	cDataConverter::BackDecodeVecC2T(name_,vvdata[2]);
	cDataConverter::BackDecodeVecC2VecVecC2MTVT(macro_,vvdata[3]);
	//追加
	cDataConverter::BackDecodeVecC2VecVecC2MTT(content_,vvdata[4]);

	std::map<tstring,pcScriptRLayer>::iterator itr = member_.begin();
	for(; itr != member_.end(); itr++)
	{
		itr->second->parent_ = wpcScriptRLayer(me);
	}

	
	return SUCCESS;
}

void cScriptRLayer::AllsetmessageOutputTo(std::vector<tstring>* pmessageOutputTo)
{
	std::map<tstring,pcScriptRLayer>::iterator itr = member_.begin();
	messageOutputTo_ = pmessageOutputTo;
	for(; itr != member_.end(); itr++)
	{
		itr->second->AllsetmessageOutputTo(pmessageOutputTo);
	}
}
std::vector<tstring> cScriptRLayer::memberKeyList(){
	std::vector<tstring> keyList;
	std::map<tstring,pcScriptRLayer>::iterator itr = member_.begin();
	for(; itr != member_.end(); itr++)
	{
		keyList.push_back(itr->first);
	}
	return keyList;
}

int ConvertT2VecC(const pcScriptRLayer& tmpl, std::vector<SByte>& data)
{
	tmpl->savedata(data);
	return SUCCESS;
}
int DecodeVecC2T(const std::vector<SByte>& data, pcScriptRLayer& tmpl)
{
	tmpl = pcScriptRLayer(new cScriptRLayer);
	tmpl->loaddata(data,tmpl);
	return SUCCESS;
}
