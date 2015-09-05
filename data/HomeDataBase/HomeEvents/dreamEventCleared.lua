function dreamEventClose()
            --[[
            if(PlayerCharacter(): ID() == 1002)
             then
             pal = searchOneCharacterOnFloor(1003)
             else 
             pal = searchOneCharacterOnFloor(1002)
            end

            --pal_walks_to_near_by_pc--
            dest_pal = LandformPlace(PlayerCharacter().placeX+1, PlayerCharacter().placeY)
            moveWithAnimeToLand(pal,dest_pal,false,1.0,false)
            coroutine.yield(false)
]]

			clearMassage()
			str_map = map_tstring_StyleString()
            storyEventMessageLoop(_T("greenAndPurpleOpenWake_"), str_map, nil)
            --storyEventMessageLoopRange(_T("greenDungeonCloseEvent_"),2,6, str_map, nil)

			clearMassage()
            return true
end



--[[
function homeEvent()
	if( isDungeonCleared(_T("blue_Dungeon")) )then
		if (getMapValue(savable_globalFlags(),_T("EventDidFlag:dreamDungeonCleard"),NO)) == NO then
			setMapValue(savable_globalFlags(),_T("EventDidFlag:dreamDungeonCleard"), YES)
			
            stackDramaThread(coroutine.create(dreamEventClose))
            appearDungeon(_T("green_Dungeon"))
			appearDungeon(_T("purple_Dungeon"))
			--cameraMove(PlayerCharacter().placeX, PlayerCharacter().placeY, 1.5,0.75, 0.9)
        end
    end
end		
]]