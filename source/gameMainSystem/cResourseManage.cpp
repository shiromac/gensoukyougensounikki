#include "../stdafx.h"

#include "cResourseManage.h"





cResourseManage::cResourseManage(void)
{
	pFilePackLoad_ = NULL;
}
cResourseManage::cResourseManage(CFilePackLoad* pFilePackLoad)
{
	pFilePackLoad_ = pFilePackLoad;
}

cResourseManage::~cResourseManage(void)
{
	deleteD3DResourse();
	//--------------------------------------------
	//ここまで定型
	//--------------------------------------------

}

void cResourseManage::deleteD3DResourse()
{
	//要素が増えるごとに追加してく
	deleteSurface();
	deleteTexture();
	//deleteXfile();


}
void cResourseManage::deleteTexture()
{


	//テクスチャを全部開放

	map<tstring, IDirect3DTexture9*>::iterator itr    = m_pTextureM.begin();
	map<tstring, IDirect3DTexture9*>::iterator itrEnd = m_pTextureM.end();
	// 要素をすべて削除
	for( ; itr != itrEnd ; itr++ )
	{
		if ( itr->second != NULL )
		{

			itr->second->Release();//強制開放

			/*
			サーフェイスを開放するとテクスチャが消失する？
			ので片方だけ開放命令
			*/

			itr->second = NULL;
		}
	}
	m_pTextureM.clear();//掃除

	deleteOnlyTextureD();

}
void cResourseManage::deleteOnlyTextureD()
{
	int i,size = m_pTextureD.size();
	for(i=0;i<size;i++)
	{
		if ( (m_pTextureD[i]) != NULL)
		{
			(m_pTextureD[i])->Release();//強制開放

			(m_pTextureD[i]) = NULL;
		}
	}
	m_pTextureD.clear();//掃除
	
}
void cResourseManage::ResetedDevice()
{
	deleteSurface();
	deleteOnlyTextureD();


}
void cResourseManage::deleteSurface()
{
	int i;

	//サーフェイスを全部開放
	for(i=0 ;i < (int)m_pSurfaceV.size(); i++)
	{
		if ( (m_pSurfaceV[i]) != NULL)
		{
			(m_pSurfaceV[i])->Release() ;//強制開放

			(m_pSurfaceV[i]) = NULL;
		}
	}
	m_pSurfaceV.clear();//掃除
	
}

void cResourseManage::deleteXfile()
{
	//int i;
/*
	//Xfileを全部開放
	for(i=0 ;i < (int)m_pXfileV.size(); i++)
	{
		if ( m_pXfileV[i] != NULL )
		{
			delete[] m_pXfileV[i];
			m_pXfileV[i] = NULL;
		}
	}
	m_pXfileV.clear();//掃除
	*/
}

void cResourseManage::deleteThis(vector<void*>* pSomethingV)
{
	int i;

	//全部開放
	for(i=0 ;i < (int)pSomethingV->size(); i++)
	{
		if ( (*pSomethingV)[i] != NULL )
		{
			delete (*pSomethingV)[i];
			(*pSomethingV)[i] = NULL;
		}
	}
	(*pSomethingV).clear();//掃除
	
}

//ファイルからテクスチャを読み込む
IDirect3DTexture9* cResourseManage::getTextureFromFile(IDirect3DDevice9* pDevice,const TCHAR* name)
{
	HRESULT hr;
	IDirect3DTexture9 *pTexture = NULL;
	TCHAR path[128] = _T("");
	//--------------------------------------------------------------
	// テクスチャ読み込み
	//--------------------------------------------------------------

	//hr = D3DXCreateTextureFromFile( pDevice, name, &pTexture );



	_stprintf(path,_T("%s%s"),GRAPHICFOLDER,name);



	map<tstring, IDirect3DTexture9*>::iterator itr;
	map<tstring, IDirect3DTexture9*>::iterator itrEnd = m_pTextureM.end();


	itr = m_pTextureM.find(path);
	if(itrEnd != itr)//pathをkeyとするものがある(複重)
	{
		//確保リストから参照
		pTexture = itr->second;

		return pTexture;
	}


	//持って無いからロード
	if(pFilePackLoad_ != NULL)
	{
		pTexture = pFilePackLoad_->GetTextureFile(path);
		if(pTexture != NULL)
		{	
			//確保リストに追加
			m_pTextureM.insert( pair<tstring, IDirect3DTexture9*>(path, pTexture));

			return pTexture;
		}

	}


	hr = D3DXCreateTextureFromFileEx(
		pDevice,
		path,	//画像
		0, 0, 0, 0,
		D3DFMT_A8B8G8R8,
		D3DPOOL_MANAGED,
		D3DX_FILTER_LINEAR,
		D3DX_FILTER_LINEAR,
		0,
		NULL,
		NULL,
		&pTexture);

	
	if FAILED( hr ) 
	{//再試行//直下
		hr = D3DXCreateTextureFromFileEx(
			pDevice,
			name,	//画像
			0, 0, 0, 0,
			D3DFMT_A8B8G8R8,
			D3DPOOL_MANAGED,
			D3DX_FILTER_LINEAR,
			D3DX_FILTER_LINEAR,
			0,
			NULL,
			NULL,
			&pTexture);

		int i=0;
	}

	if FAILED( hr ) 
	{
		if (pTexture != NULL)
		{	//保険
			pTexture->Release();
			pTexture = NULL;
		}
		tstring ods = _T("");
		ods = ods + _T("テクスチャの読み込みに失敗\"") + name + _T("\"\n");
		OutputDebugString( (ods.c_str()) );
		return NULL;
	}
	if (pTexture != NULL)
	{

		//確保リストに追加
		m_pTextureM.insert( pair<tstring, IDirect3DTexture9*>(path, pTexture));

	}

	return pTexture;
}

//テクスチャ作成
IDirect3DTexture9** cResourseManage::makeTexture(IDirect3DDevice9* pDevice, int width, int height)
{
	HRESULT hr;
	IDirect3DTexture9 *pTexture = NULL;
	//--------------------------------------------------------------
	// テクスチャ読み込み
	//--------------------------------------------------------------
	hr = pDevice->CreateTexture(width, height, 0, D3DUSAGE_RENDERTARGET,
			 D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pTexture, NULL);
	
	if FAILED( hr ) 
	{
		if (pTexture != NULL)
		{	//保険
			pTexture->Release();
			pTexture = NULL;
		}
		OutputDebugString( _T("テクスチャの作成に失敗\n") );
		return NULL;
	}
	if (pTexture != NULL)
	{
		/*
		map<tstring, IDirect3DTexture9*>::iterator itr;

		TCHAR tmpName[64] = _T("");

		_stprintf(tmpName,_T("%s%d"),_T("\\\\\\origin"),m_pTextureM.size());
		
		//確保リストに追加
		m_pTextureM.insert( pair<tstring, IDirect3DTexture9*>(tmpName, pTexture));
		//内部で作成したテクスチャは常にオリジナルとなる。
		*/
		m_pTextureD.push_back(pTexture);
		return &(m_pTextureD.back());
	}

	return NULL;
}

//テクスチャのポインタからサーフェイスを読み込んでポインタを返す。失敗した場合ＮＵＬＬを返す。
bool cResourseManage::getSurfaceFromTexture(IDirect3DTexture9*& pTexture, IDirect3DSurface9*& pSurface)
{
	HRESULT hr;
	pSurface = NULL;

	hr = pTexture->GetSurfaceLevel(0, &pSurface);
	if FAILED( hr ) 
	{
		if (pSurface != NULL)
		{	//保険
			pSurface->Release();
			pSurface = NULL;
		}
		ERROR_RETURN( _T("バックバッファの取得に失敗") );
		return false;
	}
	if (pSurface != NULL)
	{
		//確保リストに追加
		m_pSurfaceV.push_back(pSurface);

		return true;
	}
	return false;
}

//バックバッファのサーフェイスを読み込んでポインタを返す。失敗した場合ＮＵＬＬを返す。
IDirect3DSurface9* cResourseManage::getBackBuffer(IDirect3DDevice9* pDevice)
{
	HRESULT hr;
	IDirect3DSurface9* pBackBuffer = NULL;

	hr = pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	if FAILED( hr ) 
	{
		if (pBackBuffer != NULL)
		{	//保険
			pBackBuffer->Release();
			pBackBuffer = NULL;
		}
		OutputDebugString( _T("バックバッファの取得に失敗") );
		return NULL;
	}
	if (pBackBuffer != NULL)
	{
		//確保リストに追加
		m_pSurfaceV.push_back(pBackBuffer);

		return m_pSurfaceV.back();
	}
	return NULL;
}

