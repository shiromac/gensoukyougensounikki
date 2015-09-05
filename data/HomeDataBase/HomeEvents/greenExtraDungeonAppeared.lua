function homeEvent()
	if( isDungeonCleared(_T("green_Dungeon")) )then
		if(not isAppearedDungeon(_T("green_extra_Dungeon")) ) then

            stackDramaThread(coroutine.create(function()
                
                clearMassage()
                str_map = map_tstring_StyleString()
                storyEventMessageLoop(_T("greenExtraDungeonOpenEvent_"), str_map, nil)
                clearMassage()
                
                appearDungeon(_T("green_extra_Dungeon"))
			
                return true;
			end ))
		end
	end
end
