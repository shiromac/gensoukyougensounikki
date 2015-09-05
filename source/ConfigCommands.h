

#include "cCommand.h"

//パッドコンフィグ
class cCommandConfigPadConfig : public cCommand
{
public:
	cCommandConfigPadConfig(void){caption = g_Lang(_T("パッドコンフィグ"));};
	cCommandConfigPadConfig(tstring s){caption = s;};
	virtual ~cCommandConfigPadConfig(void){};
	virtual int Action(IDirect3DDevice9 *pDev);
};
void MenuPopupConfigPadConfig(cVectorOfControlLayer& vectorOfControlLayer, IDirect3DDevice9 *pDev);

//BGM音量コンフィグ
class cCommandConfigBGMVolConfig : public cCommand
{
public:
	cCommandConfigBGMVolConfig(void){caption = g_Lang(_T("BGM音量コンフィグ"));};
	cCommandConfigBGMVolConfig(tstring s){caption = s;};
	virtual ~cCommandConfigBGMVolConfig(void){};
	virtual int Action(IDirect3DDevice9 *pDev);
	virtual StyleString shortExplanationText();
};
void MenuPopupConfigBGMVolConfig(cVectorOfControlLayer& vectorOfControlLayer, IDirect3DDevice9 *pDev, StyleString& caption);
//SE音量コンフィグ
class cCommandConfigSEVolConfig : public cCommand
{
public:
	cCommandConfigSEVolConfig(void){caption = g_Lang(_T("SE音量コンフィグ"));};
	cCommandConfigSEVolConfig(tstring s){caption = s;};
	virtual ~cCommandConfigSEVolConfig(void){};
	virtual int Action(IDirect3DDevice9 *pDev);
	virtual StyleString shortExplanationText();
};
void MenuPopupConfigSEVolConfig(cVectorOfControlLayer& vectorOfControlLayer, IDirect3DDevice9 *pDev, StyleString& caption);

//プレイヤーサイン変更
class cCommandConfigSignsConfig : public cCommand
{
public:
	cCommandConfigSignsConfig(void){caption = g_Lang(_T("プレイヤーサイン変更"));};
	cCommandConfigSignsConfig(tstring s){caption = s;};
	virtual ~cCommandConfigSignsConfig(void){};
	virtual int Action(IDirect3DDevice9 *pDev);
	virtual StyleString shortExplanationText();
};
void MenuPopupConfigSignsConfig(cVectorOfControlLayer& vectorOfControlLayer, IDirect3DDevice9 *pDev, StyleString& caption);

//文字描画の精細さコンフィグ
class cCommandConfigCharEdged : public cCommand
{
public:
	cCommandConfigCharEdged(void){caption = g_Lang(_T("文字描画の精細さ"));};
	cCommandConfigCharEdged(tstring s){caption = s;};
	virtual ~cCommandConfigCharEdged(void){};
	virtual int Action(IDirect3DDevice9 *pDev);
	virtual StyleString shortExplanationText();
};
void MenuPopupConfigCharEdged(cVectorOfControlLayer& vectorOfControlLayer, IDirect3DDevice9 *pDev, StyleString& caption);

//文字描画の精細さコンフィグ
class cCommandConfigMapThickness : public cCommand
{
public:
	cCommandConfigMapThickness(void){caption = g_Lang(_T("ミニマップ表示の濃さ"));};
	cCommandConfigMapThickness(tstring s){caption = s;};
	virtual ~cCommandConfigMapThickness(void){};
	virtual int Action(IDirect3DDevice9 *pDev);
	virtual StyleString shortExplanationText();
};
void MenuPopupConfigMapThickness(cVectorOfControlLayer& vectorOfControlLayer, IDirect3DDevice9 *pDev, StyleString& caption);



//コンフィグ
class cCommandConfigConfig : public cCommand
{
public:
	cCommandConfigConfig(void){caption = g_Lang(_T("コンフィグ"));};
	cCommandConfigConfig(tstring s){caption = s;};
	virtual ~cCommandConfigConfig(void){};
	virtual int Action(IDirect3DDevice9 *pDev);
};
void MenuPopupConfigConfig(cVectorOfControlLayer& vectorOfControlLayer, IDirect3DDevice9 *pDev);

