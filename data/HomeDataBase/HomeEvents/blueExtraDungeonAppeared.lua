function homeEvent()
	if( isDungeonCleared(_T("blue_Dungeon")) )then
		if(not isAppearedDungeon(_T("blue_extra_Dungeon")) ) then

            stackDramaThread(coroutine.create(function()
                
                clearMassage()
                str_map = map_tstring_StyleString()
                storyEventMessageLoop(_T("blueExtraDungeonOpenEvent_"), str_map, nil)
                clearMassage()
                
                appearDungeon(_T("blue_extra_Dungeon"))
			
                return true;
			end ))
		end
	end
end
