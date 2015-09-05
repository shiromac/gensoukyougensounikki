function homeEvent()
	if( isDungeonCleared(_T("blue_Dungeon")) )then
		if(not isAppearedDungeon(_T("no_equip_Dungeon")) ) then

            stackDramaThread(coroutine.create(function()
                
                clearMassage()
                str_map = map_tstring_StyleString()
                storyEventMessageLoop(_T("noEquipDungeonOpenEvent_"), str_map, nil)
                clearMassage()
                
                appearDungeon(_T("no_equip_Dungeon"))
			
                return true;
			end ))
		end
	end
end
