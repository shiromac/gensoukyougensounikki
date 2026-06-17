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

	map<tstring, cRenderTexture*>::iterator itr    = m_pTextureM.begin();
	map<tstring, cRenderTexture*>::iterator itrEnd = m_pTextureM.end();
	// 要素をすべて削除
	for( ; itr != itrEnd ; itr++ )
	{
		if ( itr->second != NULL )
		{

			cRenderRelease(itr->second);//強制開放

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
			cRenderRelease(m_pTextureD[i]);//強制開放

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
			cRenderRelease(m_pSurfaceV[i]) ;//強制開放

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
cRenderTexture* cResourseManage::getTextureFromFile(cRenderDevice* pDevice,const TCHAR* name)
{
	cRenderTexture *pTexture = NULL;
	tstring path = g_GraphicAssetPath(name);
	//--------------------------------------------------------------
	// テクスチャ読み込み
	//--------------------------------------------------------------

	//hr = D3DXCreateTextureFromFile( pDevice, name, &pTexture );







	map<tstring, cRenderTexture*>::iterator itr;
	map<tstring, cRenderTexture*>::iterator itrEnd = m_pTextureM.end();


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
		pTexture = pFilePackLoad_->GetTextureFile(path.c_str());
		if(pTexture != NULL)
		{
			//確保リストに追加
			m_pTextureM.insert( pair<tstring, cRenderTexture*>(path, pTexture));

			return pTexture;
		}

	}


	bool loaded = cRenderLoadTextureFromFile(pDevice, path.c_str(), &pTexture);


	if(!loaded)
	{//再失敗//仮
		loaded = cRenderLoadTextureFromFile(pDevice, name, &pTexture);

		int i=0;
	}

	if(!loaded)
	{
		if (pTexture != NULL)
		{	//保険
			cRenderRelease(pTexture);
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
		m_pTextureM.insert( pair<tstring, cRenderTexture*>(path, pTexture));

	}

	return pTexture;
}

//テクスチャ作成
cRenderTexture** cResourseManage::makeTexture(cRenderDevice* pDevice, int width, int height)
{
	cRenderTexture *pTexture = NULL;
	//--------------------------------------------------------------
	// テクスチャ読み込み
	//--------------------------------------------------------------
	bool created = cRenderCreateRenderTargetTexture(pDevice, width, height, &pTexture);

	if(!created)
	{
		if (pTexture != NULL)
		{	//保険
			cRenderRelease(pTexture);
			pTexture = NULL;
		}
		OutputDebugString( _T("テクスチャの作成に失敗\n") );
		return NULL;
	}
	if (pTexture != NULL)
	{
		/*
		map<tstring, cRenderTexture*>::iterator itr;

		TCHAR tmpName[64] = _T("");

		_stprintf(tmpName,_T("%s%d"),_T("\\\\\\origin"),m_pTextureM.size());

		//確保リストに追加
		m_pTextureM.insert( pair<tstring, cRenderTexture*>(tmpName, pTexture));
		//ここで作成したテクスチャは常にオリジナルとなる。
		*/
		m_pTextureD.push_back(pTexture);
		return &(m_pTextureD.back());
	}

	return NULL;
}

//テクスチャのポインタからサーフェイスを読み込んでポインタを返す。失敗した場合ＮＵＬＬを返す。
bool cResourseManage::getSurfaceFromTexture(cRenderTexture*& pTexture, cRenderSurface*& pSurface)
{
	pSurface = NULL;

	if(!cRenderGetSurfaceFromTexture(pTexture, &pSurface))
	{
		if (pSurface != NULL)
		{	//保険
			cRenderRelease(pSurface);
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
cRenderSurface* cResourseManage::getBackBuffer(cRenderDevice* pDevice)
{
	cRenderSurface* pBackBuffer = NULL;

	if(!cRenderGetBackBuffer(pDevice, &pBackBuffer))
	{
		if (pBackBuffer != NULL)
		{	//保険
			cRenderRelease(pBackBuffer);
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

