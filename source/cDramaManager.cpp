
#include "cDramaManager.h"

#include "cDrama.h"


cDramaManager::cDramaManager()
{

}

cDramaManager::~cDramaManager()
{
}

void cDramaManager::doDrama()
{
	if(dramaQueue_.empty())
	{
		return;
	}
	pcDrama pDrama = dramaQueue_.front();
	if(pDrama->doDrama())
	{
		dramaQueue_.pop();
	}
}
	
bool cDramaManager::stackDrama(pcDrama& pDrama)
{
	if(pDrama->isError())
	{
		return false;
	}
	dramaQueue_.push(pDrama);
	return true;
}

bool cDramaManager::emptyDrama()
{
	return dramaQueue_.empty();
}