#include "Language.h"
#include "scriptprocesser/DropsScript.h"
#include "gameMainSystem/filemanage/cScriptRLayer.h"

#include "cDungeonSystem.h"

cScriptReader g_systemSR;

tstring LANGUAGE_IMPORTDATADIR()
{
	tstring str = _EXELOCATION _T("Language/");
	cScriptReader selectSR(str + _T("select.id"));
	selectSR.load();
	str += selectSR.data().topscript();
	return str;
}


void g_InitSystemData(tstring & file)
{
	g_systemSR.setscriptname(file);
	g_systemSR.load();
	g_systemSR.outputerrormassage((tstring)_T("ImportSystem_log.txt"));
}

tstring g_fontstyle()
{
	pcScriptRLayer psrl = g_systemSR.pdata()->pmember((tstring)_T("Fontstyle"));
	if(psrl == NULL)
	{
		return _T("ＭＳ Ｐゴシック");
	}
	else return psrl->topscript();
}


StyleString g_Lang(const TCHAR* name)
{
	StyleString sstr;
	g_Lang(name, sstr);
	return sstr;
}

void g_Lang(const TCHAR* name, StyleString & output)
{
	pcScriptRLayer psrl = g_systemSR.pdata()->pmember((tstring)_T("Language"));
	if(psrl == NULL)
	{
		output = name;
		return;
	}
	psrl = psrl->pmember((tstring)name);
	if(psrl == NULL)
	{
		output = name;
		return;
	}
	Drop_StringAddStyle(psrl, output);
}
void g_Lang(const TCHAR* name, StyleString & output, std::map<tstring, StyleString > & valiable)
{
	pcScriptRLayer psrl = g_systemSR.pdata()->pmember((tstring)_T("Language"));
	if(psrl == NULL)
	{
		output = name;
		return;
	}
	psrl = psrl->pmember((tstring)name);
	if(psrl == NULL)
	{
		output = name;
		return;
	}
	Drop_StringAddStyle(psrl, output, valiable);
}

StyleString g_Lang(const TCHAR* name, std::map<tstring, StyleString > & valiable)
{
	StyleString output;
	g_Lang(name,output,valiable);
	return output;
}

//sg_pDungeonSystem->メッセージを高速に実行する関数
int g_Langメッセージ(const TCHAR* name, std::map<tstring, StyleString > & valiable)
{
	StyleString output;
	g_Lang(name,output,valiable);
	return sg_pDungeonSystem->メッセージ(output);
}




StyleString g_Story(const TCHAR* name)
{
	StyleString sstr;
	g_Story(name, sstr);
	return sstr;
}

void g_Story(const TCHAR* name, StyleString & output)
{
	output.clear();
	pcScriptRLayer psrl = g_systemSR.pdata()->pmember((tstring)_T("Story"));
	if(psrl == NULL)
	{
		output = name;
		return;
	}
	psrl = psrl->pmember((tstring)name);
	if(psrl == NULL)
	{
		output = name;
		return;
	}
	Drop_StringAddStyle(psrl, output);
}
void g_Story(const TCHAR* name, StyleString & output, std::map<tstring, StyleString > & valiable)
{
	output.clear();
	pcScriptRLayer psrl = g_systemSR.pdata()->pmember((tstring)_T("Story"));
	if(psrl == NULL)
	{
		output = name;
		return;
	}
	psrl = psrl->pmember((tstring)name);
	if(psrl == NULL)
	{
		output = name;
		return;
	}
	Drop_StringAddStyle(psrl, output, valiable);
}

StyleString g_Story(const TCHAR* name, std::map<tstring, StyleString > & valiable)
{
	StyleString output;
	g_Story(name,output,valiable);
	return output;
}

//sg_pDungeonSystem->メッセージを高速に実行する関数
int g_Storyメッセージ(const TCHAR* name, std::map<tstring, StyleString > & valiable)
{
	StyleString output;
	g_Story(name,output,valiable);
	return sg_pDungeonSystem->メッセージ(output);
}
