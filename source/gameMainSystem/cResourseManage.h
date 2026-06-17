#pragma once
#include "cRenderBackend.h"
#include <vector>
#include "filemanage/tstring_ph.h"
#include "../utility/common.h"

#include "../TexSeBgmFilepack/CFilePack.h"

#include <map>
using namespace std;
#define GRAPHICFOLDER _EXELOCATION _T("graphic\\")

inline tstring g_GraphicAssetDirectory()
{
#ifdef __EMSCRIPTEN__
	return tstring(_T("/assets/graphic/"));
#else
	return tstring(GRAPHICFOLDER);
#endif
}

inline tstring g_GraphicAssetPath(const tstring& filename)
{
	return g_GraphicAssetDirectory() + filename;
}

inline tstring g_GraphicAssetPath(const TCHAR* filename)
{
	return g_GraphicAssetPath(tstring(filename));
}


//各シーンのとき唯一Environmentのなかに作られる（確保される）。
//グローバルで唯一Environmentのなかに作られる（確保される）。
//全ての動的に確保されるメモリのポインタはこのクラスに保存される
class cResourseManage
{
public:
	cResourseManage(void);
	cResourseManage(CFilePackLoad* pFilePackLoad);
public:
	virtual ~cResourseManage(void);



	void ResetedDevice();
private:
	map<tstring, cRenderTexture*> m_pTextureM;

	vector<cRenderTexture*> m_pTextureD;
	void deleteTexture();
	void deleteOnlyTextureD();
private:
	vector<cRenderSurface*> m_pSurfaceV;
	void deleteSurface();

private:
	//vector<D3DXMATERIAL*> m_pXfileV;
	void deleteXfile();
protected:
	//パックテクスチャ
	CFilePackLoad* pFilePackLoad_;

	//定型
	void deleteD3DResourse();

	//ベクトル開放
	//pSomethingVは基本的にD3Dオブジェクトにしない。
	void deleteThis(vector<void*>* pSomethingV);//NO D3DObj

public:



	//ファイルネームからテクスチャを読み込んでポインタを返す。失敗した場合ＮＵＬＬを返す。
	cRenderTexture* getTextureFromFile(cRenderDevice* pDevice, const TCHAR* name);

	//新規テクスチャを作成してポインタを返す。失敗した場合ＮＵＬＬを返す。
	cRenderTexture** makeTexture(cRenderDevice* pDevice, int width, int height);

public:

	//テクスチャのポインタからサーフェイスを読み込んでポインタを返す。失敗した場合ＮＵＬＬを返す。
	bool getSurfaceFromTexture(cRenderTexture*& pTexture, cRenderSurface*& pSurface);

	//バックバッファのサーフェイスを読み込んでポインタを返す。失敗した場合ＮＵＬＬを返す。
	cRenderSurface* getBackBuffer(cRenderDevice* pDevice);


	//Xファイル読み込み
	//D3DXMATERIAL*

};
