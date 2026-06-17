
#include "stdafx.h"

#include "cControlLayer.h"

#define MessageWINDOW_CENTERX (SCREEN_X/2)
#define MessageWINDOW_CENTERY (SCREEN_Y - (GWFONTSIZE+GWFONTOVERMARGIN+GWFONTUNDERMARGIN)*2 - EDGEWIDTH)


cControlLayer::cControlLayer(void)
{
	DrawedAndNotProcessingFlag = false;

	InputFreeFlag = false;

	notCancelToPop = false;

	PleasePop = false;

	countStringWindow = pcGameWindow((cGameWindow*)NULL);

	sscounter = -1;
}

cControlLayer::~cControlLayer(void)
{
	WindowList.clear();
}

int cControlLayer::Init(cRenderDevice *pDev)
{
	shortExplanationWindow = pcGameWindow(new cGameWindow);
	shortExplanationWindow->Init(pDev,28,4);
	shortExplanationWindow->CenterX = MessageWINDOW_CENTERX;
	shortExplanationWindow->CenterY = MessageWINDOW_CENTERY;

	return true;
}


int cControlLayer::process(cRenderDevice *pDev)
{
	int i;




	shortExplanationprocess(pDev);

	for(i=0;i<WindowList.size();i++)
	{
		if((WindowList[i])->enable)
			(WindowList[i])->process(pDev);
		
	}

	for(i=0;i<WindowList.size();i++)
	{
		if((WindowList[i])->closeLayer)
		{
			PleasePop = true;
		}
	}

	return true;
}

int cControlLayer::shortExplanationprocess(cRenderDevice *pDev)
{
	StyleString sestr;
	int i;
	sscounter--;
	if(sscounter < 0)
	{
		sscounter = 6;
		for(i=0;i<WindowList.size();i++)
		{
			if((WindowList[i])->enable)
				sestr += (WindowList[i])->shortExplanationString(pDev);
		}

		if(sestr != shortExplanationText)
		{
			shortExplanationText = sestr;
			shortExplanationWindow->Text() = sestr;
			shortExplanationWindow->ReRendarText();
		}
	}
	return true;
}

int cControlLayer::Draw(cRenderDevice *pDev)
{
	int i;


	/*
	if(smollviewWindow != NULL)
	{
		smollviewWindow->Draw(pDev);
	}
	*/

	if(DrawedAndNotProcessingFlag)
	{
		for(i=0;i<WindowList.size();i++)
		{
			(WindowList[i])->color.ARGB(200,128,128,128);
		}


	}
	else
	{
		if(shortExplanationText != _T(""))
		{
			shortExplanationWindow->Draw(pDev);
		}

		if(countStringWindow != NULL)
		{
			countStringWindow->Draw(pDev);
		}

		if(smollviewWindow != NULL)
		{
			smollviewWindow->color.ARGB(200,255,255,255);
			
		}

		for(i=0;i<WindowList.size();i++)
		{
			(WindowList[i])->color.ARGB(255,255,255,255);
		}
	}
	

	for(i=0;i<WindowList.size();i++)
	{
		(WindowList[i])->Draw(pDev);
	}

	if(!DrawedAndNotProcessingFlag)
	{
		if(smollviewWindow != NULL)
		{
			smollviewWindow->Draw(pDev);
		}
	}

	DrawedAndNotProcessingFlag = true;
	return true;
}