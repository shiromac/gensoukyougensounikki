#include "StoryScriptCommunicater.h"

#include "../cDungeonSystem.h"

#include "../cCommand.h"

#include "../cControlLayer.h"
#include "../cSelectWindow.h"
#include "../cGameWindow.h"


StoryScriptCommunicater::StoryScriptCommunicater()
{

}

StoryScriptCommunicater::~StoryScriptCommunicater()
{

}

void showCommandList(vector<pcCommand> commands)
{

	pcControlLayer pccl;
	pcSelectWindow pcsw;
	pcGameWindow pcgw;

	sg_pDungeonSystem->menuControlLayerV().push_back(pccl = pcControlLayer(new cControlLayer));

	pccl->Init(sg_pDungeonSystem->pDevice_D3D);
	pccl->WindowList.push_back(pcsw = pcSelectWindow(new cSelectWindow));
	pccl->WindowList.push_back(pcgw = pcGameWindow(new cGameWindow));


	int i,size = commands.size();
	for(i=0;i<size;i++)
	{
		pcsw->commandList.push_back(commands[i]);
	}
	pcsw->Init(sg_pDungeonSystem->pDevice_D3D, 3, pcsw->commandList.size());
	pcsw->setRight(sg_pDungeonSystem->GameScreenInterface.menuPosRight(3));
	pcsw->setTop(sg_pDungeonSystem->GameScreenInterface.menuPosTop(4));

	pcsw->playsound_decide();
}
