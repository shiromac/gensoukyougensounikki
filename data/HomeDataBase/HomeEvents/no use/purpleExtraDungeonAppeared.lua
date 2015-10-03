function homeEvent()
	if( isDungeonCleared(_T("purple_Dungeon")) )then
		if(not isAppearedDungeon(_T("purple_extra_Dungeon")) ) then

            stackDramaThread(coroutine.create(function()
                
                clearMassage()
                str_map = map_tstring_StyleString()
                storyEventMessageLoop(_T("purpleExtraDungeonOpenEvent_"), str_map, nil)
                clearMassage()
                
                appearDungeon(_T("purple_extra_Dungeon"))
			
                return true;
			end ))
		end
	end
end
