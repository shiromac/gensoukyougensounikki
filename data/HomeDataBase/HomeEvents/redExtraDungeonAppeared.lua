function homeEvent()
	if( isDungeonCleared(_T("red_Dungeon")) )then
		if(not isAppearedDungeon(_T("red_extra_Dungeon")) ) then

            stackDramaThread(coroutine.create(function()
                    
                clearMassage()
                str_map = map_tstring_StyleString()
                storyEventMessageLoop(_T("redExtraDungeonOpenEvent_"), str_map, nil)
                clearMassage()
                
                appearDungeon(_T("red_extra_Dungeon"))
                
                return true;
            end ))
		end
	end
end