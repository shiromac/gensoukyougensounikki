function purpleDungeonClose()
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
			
			fairy = searchOneCharacterOnFloor(4000)
			turnCharacterForCharacter(fairy,PlayerCharacter())
			turnCharacterForCharacter(PlayerCharacter(),fairy)
			cameraMove(fairy.placeX, fairy.placeY+1, 1,0.25,0.75)
			coroutine.yield(false)
            storyEventMessageLoop(_T("purpleDungeonClose_"), str_map, nil)
            --storyEventMessageLoopRange(_T("greenDungeonCloseEvent_"),2,6, str_map, nil)
            coroutine.yield(false)
            cameraMove(PlayerCharacter().placeX, PlayerCharacter().placeY,1,0.25,0.75)
			clearMassage()
			setMapValue(savable_globalFlags(),_T("EventDidFlag:purpleDungeonCleard"), YES)
            coroutine.yield(false)
            return true
end



function homeEvent()

	if( isDungeonCleared(_T("purple_Dungeon")) )then
		if (getMapValue(savable_globalFlags(),_T("EventDidFlag:purpleDungeonCleard"),NO)) == NO then

			
            stackDramaThread(coroutine.create(purpleDungeonClose))

			
			--cameraMove(PlayerCharacter().placeX, PlayerCharacter().placeY, 1.5,0.75, 0.9)
    end
   end
end		
