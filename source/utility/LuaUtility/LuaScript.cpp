
#include "../stdafx.h"
#include "LuaScript.h"
#include "cFileManager.h"
#include <cDataConverter.h>

cFileManager g_fileManager;

LuaScript::LuaScript(void):isFlexible_(false)
{
#ifdef _UNRELEASE
	setFlexible(true);
#endif
	lastWriteTime_.dwHighDateTime = 0;
	lastWriteTime_.dwLowDateTime = 0;
}

LuaScript::~LuaScript(void)
{

}

int LuaScript::loadFile(const tstring& filename)
{
	if(isFlexible_)
	{
		cFileManager::getLastWriteTime(filename_, &lastWriteTime_);
	}
	filename_ = filename;
	return (g_fileManager.loadFile(filename, buffer_) == SUCCESS);
}
int LuaScript::savedata(std::vector<SByte>& data)
{
	
	std::vector<std::vector<SByte>> vvc;
	vvc.resize(4);
	vvc[0] = buffer_;
	cDataConverter::ConvertT2VecC(filename_, vvc[1]);
	cDataConverter::ConvertT2VecC(lastWriteTime_.dwHighDateTime, vvc[2]);
	cDataConverter::ConvertT2VecC(lastWriteTime_.dwLowDateTime, vvc[3]);
	cDataConverter::VecVecC2VecC(vvc,data);
	

	//data = buffer_;
	return true;
}
int LuaScript::loaddata(const std::vector<SByte>& data)
{
	
	std::vector<std::vector<SByte>> vvc;
	cDataConverter::VecC2VecVecC(data,vvc);
	buffer_ = vvc[0];
	cDataConverter::BackDecodeVecC2T(filename_, vvc[1]);
	cDataConverter::BackDecodeVecC2T(lastWriteTime_.dwHighDateTime, vvc[2]);
	cDataConverter::BackDecodeVecC2T(lastWriteTime_.dwLowDateTime, vvc[3]);
	

	//buffer_ = data;
	
	return true;
}
void LuaScript::setFlexible(const int flexible)
{
	isFlexible_ = flexible;
}
const pLuaString LuaScript::buffer()
{
	if(isFlexible_)
	{
		bool isRenew;
		if(cFileManager::compareLastWriteTime(filename_, lastWriteTime_, &lastWriteTime_, &isRenew) != SUCCESS)
		{
			return (pLuaString)&(*buffer_.begin());
		}

		if(isRenew)
		{
			buffer_.clear();
			loadFile(filename_);
		}
	}

	return (pLuaString)&(*buffer_.begin());
	
}
const size_t LuaScript::buffersize()
{
	if(isFlexible_)
	{
		bool isRenew;
		if(cFileManager::compareLastWriteTime(filename_, lastWriteTime_, &lastWriteTime_, &isRenew) != SUCCESS)
		{
			return buffer_.size();
		}

		if(isRenew)
		{
			buffer_.clear();
			loadFile(filename_);
		}

	}

	return buffer_.size();

}

int ConvertT2VecC(const pLuaScript& tmpl, std::vector<SByte>& data)
{
	tmpl->savedata(data);
	return SUCCESS;
}
int DecodeVecC2T(const std::vector<SByte>& data, pLuaScript& tmpl)
{
	tmpl = pLuaScript(new LuaScript);
	tmpl->loaddata(data);
	return SUCCESS;
}