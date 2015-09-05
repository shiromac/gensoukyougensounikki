#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include "filemanage/tstring_ph.h"

#include "../TexSeBgmFilepack/CFilePack.h"

#include <map>
using namespace std;
#define GRAPHICFOLDER _EXELOCATION _T("graphic\\")


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
	map<tstring, IDirect3DTexture9*> m_pTextureM;

	vector<IDirect3DTexture9*> m_pTextureD;
	void deleteTexture();
	void deleteOnlyTextureD();
private:
	vector<IDirect3DSurface9*> m_pSurfaceV;
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
	IDirect3DTexture9* getTextureFromFile(IDirect3DDevice9* pDevice, const TCHAR* name);

	//新規テクスチャを作成してポインタを返す。失敗した場合ＮＵＬＬを返す。
	IDirect3DTexture9** makeTexture(IDirect3DDevice9* pDevice, int width, int height);

public:

	//テクスチャのポインタからサーフェイスを読み込んでポインタを返す。失敗した場合ＮＵＬＬを返す。
	bool getSurfaceFromTexture(IDirect3DTexture9*& pTexture, IDirect3DSurface9*& pSurface);

	//バックバッファのサーフェイスを読み込んでポインタを返す。失敗した場合ＮＵＬＬを返す。
	IDirect3DSurface9* getBackBuffer(IDirect3DDevice9* pDevice);


	//Xファイル読み込み
	//D3DXMATERIAL*

};
