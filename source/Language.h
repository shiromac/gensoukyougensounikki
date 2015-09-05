#pragma once

#include "utility/StyleString.h"

#include "gameMainSystem/filemanage/cScriptReader.h"

#include <common.h>

void g_InitSystemData(tstring & file);

const tstring IMPORTDATADIR = _EXELOCATION _T("data/");

tstring LANGUAGE_IMPORTDATADIR();

#include <map>

StyleString g_Lang(const TCHAR* name);
StyleString g_Lang(const TCHAR* name, std::map<tstring, StyleString > & valiable);

//高速
void g_Lang(const TCHAR* name, StyleString & output);
void g_Lang(const TCHAR* name, StyleString & output, std::map<tstring, StyleString > & valiable );

//sg_pDungeonSystem->メッセージを高速に実行する関数
int g_Langメッセージ(const TCHAR* name, std::map<tstring, StyleString > & valiable);

StyleString g_Story(const TCHAR* name);
StyleString g_Story(const TCHAR* name, std::map<tstring, StyleString > & valiable);

//高速
void g_Story(const TCHAR* name, StyleString & output);
void g_Story(const TCHAR* name, StyleString & output, std::map<tstring, StyleString > & valiable );

//sg_pDungeonSystem->メッセージを高速に実行する関数
int g_Storyメッセージ(const TCHAR* name, std::map<tstring, StyleString > & valiable);

tstring g_fontstyle();

extern cScriptReader g_systemSR;
