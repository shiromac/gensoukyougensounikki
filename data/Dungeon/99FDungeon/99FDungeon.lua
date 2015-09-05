

enemyPool = {}
landPool = {}

function lastFloorEvent()

    local str_map = map_tstring_StyleString()
    
    if (PlayerCharacter(): ID() == 1002)then
    --mailing
        CharacterTurn(PlayerCharacter(),ASPECT_UP)
        coroutine.yield(false)
	    storyEventMessageLoop(_T("99FDungeonBladeEventMeiling1_"), str_map, nil)
	    clearMessage()
        coroutine.yield(false)
        cameraMove(10,20,2,1,1)
        coroutine.yield(false)
        effect_Wait(10,20,60)
        coroutine.yield(false)
        cameraMove(PlayerCharacter().placeX, PlayerCharacter().placeY,2,1,1)
    	storyEventMessageLoop(_T("99FDungeonBladeEventMeiling2_"), str_map, nil)
    	clearMessage()
        coroutine.yield(false)
    end
    if (PlayerCharacter(): ID() == 1003)then
    --tewi
        CharacterTurn(PlayerCharacter(),ASPECT_UP)
        coroutine.yield(false)
	    storyEventMessageLoop(_T("99FDungeonBladeEventTewi1_"), str_map, nil)
	    clearMessage()
        coroutine.yield(false)
        cameraMove(10,20,2,1,1)
        coroutine.yield(false)
        effect_Wait(10,20,60)
        coroutine.yield(false)
        cameraMove(PlayerCharacter().placeX, PlayerCharacter().placeY,2,1,1)
    	storyEventMessageLoop(_T("99FDungeonBladeEventTewi2_"), str_map, nil)
    	clearMessage()
        coroutine.yield(false)
    end
    if (PlayerCharacter(): ID() == 1004)then
    --youmu
        CharacterTurn(PlayerCharacter(),ASPECT_UP)
        coroutine.yield(false)
	    storyEventMessageLoop(_T("99FDungeonBladeEventYoumu1_"), str_map, nil)
	    clearMessage()
        coroutine.yield(false)
        cameraMove(10,20,2,1,1)
        coroutine.yield(false)
        effect_Wait(10,20,60)
        coroutine.yield(false)
        cameraMove(PlayerCharacter().placeX, PlayerCharacter().placeY,2,1,1)
    	storyEventMessageLoop(_T("99FDungeonBladeEventYoumu2_"), str_map, nil)
    	clearMessage()
        coroutine.yield(false)
    end
    return true
end

function floorInitProcess()
	--outputDebugString("floorInitProcess_start\n")

    --tewiStartEvent()

	if floorLevel() == 100 then
	
	    local item = MakeDroping(1021, LandformPlace(10,20))
	    DropingStateForsedMakeGood(item)
	    if storyEventEnable() then
            stackDramaThread(coroutine.create(lastFloorEvent))
        end
	end
	

end

function dungeonProcess()
	--outputDebugString("dungeonProcess_start\n")
end


function dungeonTurnProcess()
	--outputDebugString("dungeonTurnProcess_start\n")

end