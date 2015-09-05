function homeEvent()
	if( isDungeonCleared(_T("last_Dungeon")) )then
		if(not isAppearedDungeon(_T("ten_match_Dungeon")) ) then

            stackDramaThread(coroutine.create(function()
                
                clearMassage()
                str_map = map_tstring_StyleString()
                storyEventMessageLoop(_T("tenMatchDungeonOpenEvent_"), str_map, nil)
                clearMassage()
                
                appearDungeon(_T("ten_match_Dungeon"))
			
                return true;
			end ))
		end
	end
end
