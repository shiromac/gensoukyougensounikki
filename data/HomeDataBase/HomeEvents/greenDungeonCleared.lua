
--[[function storyEventMessage(massageKeyPrefix, stringkeyMap)
    local index = 1
    while
        local massageKey = _T(massageKeyPrefix..index)
        if isExistStoryMessage(massageKey) then
            break
        end
		utility_storyMessage(massageKey,stringkeyMap)
		messageWait();
        index = index + 1
    end
end]]
function greenDungeonClose()
            
            if(PlayerCharacter(): ID() == 1002)
             then
             pal = searchOneCharacterOnFloor(1003)
             else 
             pal = searchOneCharacterOnFloor(1002)
            end

            --pal_walks_to_near_by_pc--
            --[[
            fadeBlack(1)
            coroutine.yield(false)
            ]]
            dest_pal = LandformPlace(PlayerCharacter().placeX+1, PlayerCharacter().placeY)
            --moveWithAnimeToLand(pal,dest_pal,false,1.0,false)
            moveWithAnime(pal,ASPECT_UP, 2, false, 5.0, true)
            coroutine.yield(false)
            moveWithAnime(pal,ASPECT_LEFT, 3, false, 1.0, true)
            coroutine.yield(false)
            moveWithAnime(pal,ASPECT_UPLEFT, 1, false, 1.0, true)
			--clearMassage()
			coroutine.yield(false)
			turnCharacterForCharacter(PlayerCharacter(),pal)
			turnCharacterForCharacter(pal,PlayerCharacter())
            coroutine.yield(false)

            --fadeOut(30)
            --effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,30)
            str_map = map_tstring_StyleString()
            storyEventMessageLoop(_T("greenDungeonCloseEvent1_"), str_map, nil)
            jumpWithAnime(pal, ASPECT_LEFT, 0, 0.7, false, 1.0, true)
            storyEventMessageLoop(_T("greenDungeonCloseEvent2_"), str_map, nil)
            --storyEventMessageLoop(_T("greenDungeonCloseEvent3_"), str_map, nil)
            CharacterTurn(PlayerCharacter(),ASPECT_LEFT)
            coroutine.yield(false) 
            storyEventMessageLoopRange(_T("greenDungeonCloseEvent3_"),1,1, str_map, nil)
            jumpWithAnime(pal, ASPECT_LEFT, 0, 0.7, false, 1.0, true)
            storyEventMessageLoopRange(_T("greenDungeonCloseEvent3_"),2,3, str_map, nil)
			turnCharacterForCharacter(PlayerCharacter(),pal)
			turnCharacterForCharacter(pal,PlayerCharacter())
 
            storyEventMessageLoopRange(_T("greenDungeonCloseEvent3_"),4,4, str_map, nil)

            coroutine.yield(false)           
   			setMapValue(savable_globalFlags(),_T("EventDidFlag:greenDungeonCleard"), YES)
			clearMassage()
            coroutine.yield(false)
            return true
end


function homeEvent()

	if( isDungeonCleared(_T("green_Dungeon")) )then
		if (getMapValue(savable_globalFlags(),_T("EventDidFlag:greenDungeonCleard"),NO)) == NO then

			
            stackDramaThread(coroutine.create(greenDungeonClose))
            --greenDungeonClose()
			
			--cameraMove(PlayerCharacter().placeX, PlayerCharacter().placeY, 1.5,0.75, 0.9)

    end
  end

end		
