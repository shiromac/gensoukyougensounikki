#include "stdafx.h"

#include "cCommandDroping.h"


pcDroping DropingFuncToDroping(pcDropingFunc pdfunc)
{
	return pcDroping(new cCommandDroping(pdfunc));
}


cCommandDroping::cCommandDroping(void)
{
}
cCommandDroping::cCommandDroping(pcDropingFunc pdfunc)
{
	pdropingfunc = pdfunc;
}
cCommandDroping::~cCommandDroping(void)
{
}

StyleString cCommandDroping::shortExplanation()
{
	if(pdropingfunc->LastObject()!=NULL)
		return pdropingfunc->LastObject()->shortExplanation();
	return _T("");
}
StyleString cCommandDroping::longExplanation()
{
	if(pdropingfunc->LastObject()!=NULL)
		return pdropingfunc->LastObject()->longExplanation();
	return _T("");
}
void cCommandDroping::DrawIcon(cRenderDevice *pDev,int x,int y)
{
	if(pdropingfunc->LastObject()!=NULL)
	{
		pdropingfunc->LastObject()->m_color = m_color;
		pdropingfunc->LastObject()->DrawIcon(pDev,x,y);
	}
}
void cCommandDroping::DrawStateIcon(cRenderDevice *pDev,int x,int y)
{
	if(pdropingfunc->LastObject()!=NULL)
	{
		pdropingfunc->LastObject()->m_color = m_color;
		pdropingfunc->LastObject()->DrawStateIcon(pDev,x,y);
	}
}
void cCommandDroping::DrawStateIconSub(cRenderDevice *pDev,int x,int y)
{
	if(pdropingfunc->LastObject()!=NULL)
	{
		pdropingfunc->LastObject()->m_color = m_color;
		pdropingfunc->LastObject()->DrawStateIconSub(pDev,x,y);
	}
}
int cCommandDroping::RerenderText(SpriteText & sprite,int MaxLetterLength)
{
	if(pdropingfunc->LastObject()!=NULL)
		return pdropingfunc->LastObject()->RerenderText(sprite,MaxLetterLength);
	return false;
}
//メニューで選択時に最初に呼ばれる
int cCommandDroping::Action(cRenderDevice *pDev)
{
	return pdropingfunc->Action(pDev);
}

int cCommandDroping::Getsmollview(pcGameWindow& smollview)
{
	if(pdropingfunc->LastObject()!=NULL)
		return pdropingfunc->LastObject()->Getsmollview(smollview);
	return false;
}

bool cCommandDroping::内包落ち物対象可()
{
	if(pdropingfunc->LastObject() == NULL) return false; 
	return pdropingfunc->LastObject()->内包落ち物対象可() && pdropingfunc->LastObject()->includedItem.size() > 0;
}
int cCommandDroping::内包落ち物サイズ()
{
	if(pdropingfunc->LastObject() == NULL) return 0; 
	return pdropingfunc->LastObject()->内包落ち物サイズ();
}
vector<pcDroping> cCommandDroping::内包落ち物対象リスト()
{
	vector<pcDroping> vsubpDroping;

	if(pdropingfunc->LastObject() && pdropingfunc->LastObject()->includedItem.size() > 0)
	{

		pcDroping pobject = pdropingfunc->LastObject();
		int i,size = pobject->includedItem.size();
		for(i=0;i<size;i++)
		{
			
			//新規
			pcDropingFunc pcdf = pcDropingFunc(new cDropingFunc);
			pcdf->Subject = pdropingfunc->Subject;
			pcdf->Verb = pdropingfunc->Verb;
			pcdf->Objects = pdropingfunc->Objects;
			pcdf->Objects.pop_back();
			pcdf->Objects.push_back(pobject->includedItem[i]);
			//パック
			vsubpDroping.push_back(DropingFuncToDroping(pcdf));
		}

		
	}

	return vsubpDroping;
}


//-------------------------------------------------------------------


cObjectiveDroping::cObjectiveDroping(pcDroping surface, pcCommand action)
{
	surface_ = surface;
	action_ = action;
}
cObjectiveDroping::~cObjectiveDroping(void)
{
}

StyleString cObjectiveDroping::shortExplanation()
{
	if(action_)
	{
		StyleString sstr = action_->shortExplanationText();
		if(sstr.length() > 0)
		{
			return sstr;
		}
	}
	if(surface_!=NULL)
		return surface_->shortExplanation();
	return _T("");
}
StyleString cObjectiveDroping::longExplanation()
{
	if(surface_!=NULL)
		return surface_->longExplanation();
	return _T("");
}
void cObjectiveDroping::DrawIcon(cRenderDevice *pDev,int x,int y)
{
	if(surface_!=NULL)
	{
		surface_->m_color = m_color;
		surface_->DrawIcon(pDev,x,y);
	}
}
void cObjectiveDroping::DrawStateIcon(cRenderDevice *pDev,int x,int y)
{
	if(surface_!=NULL)
	{
		surface_->m_color = m_color;
		surface_->DrawStateIcon(pDev,x,y);
	}
}
void cObjectiveDroping::DrawStateIconSub(cRenderDevice *pDev,int x,int y)
{
	if(surface_!=NULL)
	{
		surface_->m_color = m_color;
		surface_->DrawStateIconSub(pDev,x,y);
	}
}
int cObjectiveDroping::RerenderText(SpriteText & sprite,int MaxLetterLength)
{
	if(surface_!=NULL)
		return surface_->RerenderText(sprite,MaxLetterLength);
	return false;
}
//メニューで選択時に最初に呼ばれる
int cObjectiveDroping::Action(cRenderDevice *pDev)
{
	return action_->Action(pDev);
}

int cObjectiveDroping::Getsmollview(pcGameWindow& smollview)
{
	if(surface_!=NULL)
		return surface_->Getsmollview(smollview);
	return false;
}

bool cObjectiveDroping::内包落ち物対象可()
{
	if(surface_ == NULL) return false; 
	return surface_->内包落ち物対象可() && surface_->includedItem.size() > 0;
}
int cObjectiveDroping::内包落ち物サイズ()
{
	if(surface_ == NULL) return 0; 
	return surface_->内包落ち物サイズ();
}
vector<pcDroping> cObjectiveDroping::内包落ち物対象リスト()
{
	vector<pcDroping> vsubpDroping;

	if(surface_ && surface_->includedItem.size() > 0)
	{

		pcDroping pobject = surface_;
		int i,size = pobject->includedItem.size();
		for(i=0;i<size;i++)
		{
			
			//新規
			pcDroping pcommanddrop = action_->includeObjectiveDroping(pobject->includedItem[i]);
			//パック
			if(pcommanddrop)
			{
				vsubpDroping.push_back(pcommanddrop);
			}
		}

		
	}

	return vsubpDroping;
}