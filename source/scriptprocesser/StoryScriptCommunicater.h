#pragma once

class StoryScriptCommunicater;

class cCommand;
#include <boost/shared_ptr.hpp>
typedef boost::shared_ptr<cCommand> pcCommand;

#include <vector>

class StoryScriptCommunicater
{

public:

	StoryScriptCommunicater();

	virtual ~StoryScriptCommunicater();
protected:

	enum StoryScriptCommunicater_type{
		TYPE_CHOICE = 0,
	};

	enum StoryScriptCommunicater_ResoltOfCommandList{
		ResoltCommandList_CANCELED = 0,
		ResoltCommandList_SELECT_0,
		ResoltCommandList_SELECT_1,
		ResoltCommandList_SELECT_2,
		ResoltCommandList_SELECT_3,
		ResoltCommandList_SELECT_4,
		ResoltCommandList_SELECT_5,
		ResoltCommandList_SELECT_6,
		ResoltCommandList_SELECT_7,
		ResoltCommandList_SELECT_8,
		ResoltCommandList_SELECT_9,
	};
public:

	void showCommandList(std::vector<pcCommand> commands);

	StoryScriptCommunicater_ResoltOfCommandList getResoltOfCommandList();

};