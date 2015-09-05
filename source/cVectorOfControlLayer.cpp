#include "stdafx.h"
#include "cVectorOfControlLayer.h"


cVectorOfControlLayer::cVectorOfControlLayer(void)
{



}

cVectorOfControlLayer::~cVectorOfControlLayer(void)
{

}

pcControlLayer cVectorOfControlLayer::Last()
{
	if(size() <= 0) return pcControlLayer((cControlLayer *)NULL);
	return at(size()-1);
}

void cVectorOfControlLayer::process(IDirect3DDevice9 *pDev)
{
	if(!empty())
	{
		//最も新しいものだけに処理を行う
		Last()->process(pDev);
		
		if(g_pPlayerInput()->cancel().justOn)
		{
			
			cancel();
		}

		if(g_pPlayerInput()->menu().justOn)
		{
			
			allCancel();
		}

		if(Last() != NULL)
		{

					//フォーカスの処理
			if(Last())
				Last()->DrawedAndNotProcessingFlag = false;
			
			if(Last()->PleasePop)
			{
				pop_back();
				sg_pDungeonSystem->メニューリレンダ();
			}
		}
	}
}


void cVectorOfControlLayer::cancel()
{
	if(Last() == NULL) return;
	if(Last()->notCancelToPop) return;

	if(Last()->WindowList[0] != NULL)
	{
		Last()->WindowList[0]->playsound_cancel();
	}	
	
	pop_back();

	sg_pDungeonSystem->メニューリレンダ();
}



void cVectorOfControlLayer::allCancel()
{
	if(Last() == NULL) return;
	if(Last()->notCancelToPop) return;

	if(Last()->WindowList[0] != NULL)
	{
		Last()->WindowList[0]->playsound_cancel();
	}	
	
	AllRelease();
}


int cVectorOfControlLayer::AllRelease()
{
	while(size())
	{
	
		pop_back();
	}
	return true;
}
