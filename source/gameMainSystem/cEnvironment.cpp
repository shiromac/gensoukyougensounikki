#include "../stdafx.h"
#include "cEnvironment.h"
#include "filemanage/cScriptReader.h"
#include "filemanage/cScriptRLayer.h"
#include "cScreen.h"

cEnvironment g_GameEnv; 


cEnvironment::cEnvironment(void)
{
	int i;
	
	m_Screen = new cScreen;
	m_SceneManage = new cSceneManager;

	pmTextureVforGlobalIndex.resize(MAX_TEX_INDEXSIZE);
	for(i=0;i<MAX_TEX_INDEXSIZE;i++)
	{
		pmTextureVforGlobalIndex[i] = NULL;
	}
	AppEnd = false;


}

cEnvironment::~cEnvironment(void)
{
	//RELEASE(m_Resourse);
	RELEASE(m_GlobalResourse);
	RELEASE(m_Screen);
	RELEASE(m_SceneManage);


	pmTextureVforGlobalIndex.clear();
}
bool cEnvironment::ResetDevice(LPDIRECT3DDEVICE9 pDev)//デバイスのリセット
{
	m_GlobalResourse->ResetedDevice();

	return true;
}
void cEnvironment::init(LPDIRECT3DDEVICE9 pDev)
{
	
#if defined(_UNRELEASE) && defined(GGN_REPACK_GRAPHICS_ON_START)
	CFilePack graphicpack;
	cScriptReader SR(GRAPHICFOLDER _T("graphicpass.id"));
	SR.load();

	vector<tstring> passes;
	getPass(SR.pdata(),SR.scriptdir(),passes);

	int i;
	for(i=0;i<passes.size();i++)
	{
		graphicpack.AddFile(passes[i].c_str(),passes[i].c_str());
	}
	graphicpack.OutPutPackFile(GRAPHICFOLDER _T("graphicpack"));

#else

#endif

	FilePackLoad_.LoadPackFile(GRAPHICFOLDER _T("graphicpack"),pDev);

	//リソース確保
	m_GlobalResourse = new cResourseManage(&FilePackLoad_);
	
	//m_GlobalResourse = new cResourseManage();
}

