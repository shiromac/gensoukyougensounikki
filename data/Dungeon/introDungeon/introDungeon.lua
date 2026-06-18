
state_attackTutorial = 1
state_itemTutorial = 2
state_itemTutorial2 = 3
state_itemTutorial3 = 4
state_introEndEvent = 6

enemyPool = {}

function storyEventMessageLoopNoWait(keyPrefix, StringValiable, intervalFunction)
    local index = 1
    while true do
        local massageKey = keyPrefix .. tostring(index)
        if not isExistStoryMessage(massageKey) then
            break
        end
        utility_storyMessage(massageKey, StringValiable)
        if intervalFunction then
            intervalFunction()
        end
        index = index + 1
    end
end

function storyEventMessageLoopRangeNoWait(keyPrefix, firstIndex, lastIndex, StringValiable, intervalFunction)
    local index = firstIndex
    while index <= lastIndex do
        local massageKey = keyPrefix .. tostring(index)
        if not isExistStoryMessage(massageKey) then
            break
        end
        utility_storyMessage(massageKey, StringValiable)
        if intervalFunction then
            intervalFunction()
        end
        index = index + 1
    end
end
firstPositionX, firstPositionY = 20, 17

function findNearEventLand_valueFunction(fromland, toland, distance, aspect, margin)
    local targetland = LandformNearby(fromland, aspect, distance)
    local targetdistance = (targetland.place - toland.place):dif()
    local fromdistance = (toland.place - fromland.place):dif()
    local margin_enable_count = 0
    local toland_enable = 0
    if not toland: isEqualTo(LandformEdge()) and isSafeSetCharacterLandform(toland) and canLandformSeeLandform(fromland, toland) then
        toland_enable = 1
    end
    for x = targetland.place.x - margin, targetland.place.x + margin do
        for y = targetland.place.y - margin, targetland.place.y + margin do
            local templand = LandformPlace(x,y)
            if not templand: isEqualTo(LandformEdge()) and isSafeSetCharacterLandform(templand) and canLandformSeeLandform(fromland, templand) then
                margin_enable_count = margin_enable_count + 1
            end
        end
    end
    return -targetdistance -math.abs(fromdistance - distance)*100 +margin_enable_count*10000 +toland_enable*1000000
end

function findNearEventLand(fromland, distance, aspect, margin)
    local templand = LandformNearby(fromland, aspect, distance)
    local landlist = vector_cLandform()
    for x = 0 , MAPWIDTH-1 do
        for y = 0 , MAPHEIGHT-1 do
            local templand = LandformPlace(x,y)
            if( not templand: isEqualTo(LandformEdge()))then
                landlist: push_back(templand)
            end
        end
    end
    local value = {}
    local state = {}
    local maxvalue = nil
    local action = nil
    local landsize = landlist: size();
    for i = 0, landsize-1 do
        local tempvalue = findNearEventLand_valueFunction(fromland, landlist: at(i), distance, aspect, margin)
        if(maxvalue == nil or maxvalue < tempvalue)then
            maxvalue = tempvalue
            action = i
        end
    end
    
    if(action)then
        return landlist: at(action)
    end
    return LandformEdge()
end

function introSelfIntroEvent()

    fadeBlack(1)
    setHidingMiniMap(true)
    effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,5)
    coroutine.yield(false)--drama continue

    eventNameKeyPrefix = _T("introSelfIntro_");
	clearMassage()
	str_map = map_tstring_StyleString()

    fadeOut(240)
    effect_Wait(PlayerCharacter().placeX,PlayerCharacter().placeY,60)
    coroutine.yield(false)--drama continue
    moveWithAnime(PlayerCharacter(), ASPECT_LEFT, 1, false, 1, false)
    coroutine.yield(false)--drama continue
    effect_Wait(PlayerCharacter().placeX,PlayerCharacter().placeY,60)
    moveWithAnime(PlayerCharacter(), ASPECT_LEFT, 1, false, 1, false)
    coroutine.yield(false)--drama continue
    effect_Wait(PlayerCharacter().placeX,PlayerCharacter().placeY,60)
    coroutine.yield(false)--drama continue

    moveWithAnime(PlayerCharacter(), ASPECT_RIGHT, 1, false, 1, false)
    coroutine.yield(false)--drama continue
    effect_Wait(PlayerCharacter().placeX,PlayerCharacter().placeY,60)
    coroutine.yield(false)--drama continue
    moveWithAnime(PlayerCharacter(), ASPECT_RIGHT, 1, false, 1, false)
    coroutine.yield(false)--drama continue
    effect_Wait(PlayerCharacter().placeX,PlayerCharacter().placeY,60)
    coroutine.yield(false)--drama continue
	
    storyEventMessageLoopRangeNoWait(eventNameKeyPrefix, 1, 1, str_map, nil)	
    jumpWithAnime(PlayerCharacter(),ASPECT_RIGHT,1, 0.5, true, 1, false);
    coroutine.yield(false)--drama continue
    jumpWithAnime(PlayerCharacter(),ASPECT_LEFT,1, 0.5, true, 1, false);
    coroutine.yield(false)--drama continue
    clearMassage()
    effect_Wait(PlayerCharacter().placeX,PlayerCharacter().placeY,60)
    coroutine.yield(false)--drama continue        
    moveWithAnime(PlayerCharacter(), ASPECT_LEFT, 2, false, 1, false)
    coroutine.yield(false)--drama continue
    storyEventMessageLoopRangeNoWait(eventNameKeyPrefix, 2, 2, str_map, nil)
    jumpWithAnime(PlayerCharacter(),ASPECT_RIGHT,1, 0.5, true, 1, false);
    coroutine.yield(false)--drama continue
    jumpWithAnime(PlayerCharacter(),ASPECT_LEFT,1, 0.5, true, 1, false);
    coroutine.yield(false)--drama continue
    clearMassage()
    effect_Wait(PlayerCharacter().placeX,PlayerCharacter().placeY,60)
    coroutine.yield(false)--drama continue	
    moveWithAnime(PlayerCharacter(), ASPECT_RIGHT, 2, false, 1, false)
    coroutine.yield(false)--drama continue	
    CharacterTurn(PlayerCharacter(),ASPECT_DOWN)
    
	storyEventMessageLoopRangeNoWait(eventNameKeyPrefix, 3, 5, str_map, nil)

	cameraMove(firstPositionX,firstPositionY,1,1,1)

	emeny = MakeCharacter(2001, 0, CHARACTER_FORSE_ENEMY, LandformPlace(firstPositionX,firstPositionY))
	enemyPool["first"] = emeny
	CharacterCureMental(emeny)
	CharacterSwingAir(emeny, TURN_FOREVER)
	effect_Smoke1(firstPositionX,firstPositionY)
	
    coroutine.yield(false)--drama continue

	storyEventMessageLoopRangeNoWait(eventNameKeyPrefix, 6, 6, str_map, nil)

	cameraMoveParallel(PlayerCharacter().placeX,PlayerCharacter().placeY,1,0.25,0.75)
	
	storyEventMessageLoopRangeNoWait(eventNameKeyPrefix, 7, 9, str_map, nil)
	
	return true--drama end
end

function introItemUseEvent()
	clearMassage()

	local str_map = map_tstring_StyleString()	
	storyEventMessageLoopNoWait(_T("introAfterSelf_"), str_map, nil)
	
    coroutine.yield(false)--drama continue
	cameraMove(20,17,1,1,1)
    --alice
	emeny = MakeCharacter(4011, 1, CHARACTER_FORSE_ENEMY, LandformPlace(20,17))
	enemyPool["second"] = emeny
	CharacterCureMental(emeny)

	effect_Smoke1(20,17)
	storyEventMessageLoopNoWait(_T("introBattleAlice_"), str_map, nil)
	
    coroutine.yield(false)--drama continue
    
    
    
	cameraMove(17,20,1.5,1,1)

	emeny = MakeCharacter(4012, 1, CHARACTER_FORSE_ENEMY, LandformPlace(17,20))
	enemyPool["third"] = emeny
	CharacterCureMental(emeny)
	effect_Smoke1(17,20)
	
    coroutine.yield(false)--drama continue

	cameraMove(23,20,1.5,1,1)

	emeny = MakeCharacter(4012, 1, CHARACTER_FORSE_ENEMY, LandformPlace(23,20))
	enemyPool["fourth"] = emeny
	CharacterCureMental(emeny)
	effect_Smoke1(23,20)
		
    coroutine.yield(false)--drama continue

	cameraMove(PlayerCharacter().placeX,PlayerCharacter().placeY,1.5,1,1)
	storyEventMessageLoopNoWait(_T("introItemUse_"), str_map, nil)

    coroutine.yield(false)--drama continue
    
    
	cameraMove(PlayerCharacter().placeX,PlayerCharacter().placeY+1,2,1,1)
    coroutine.yield(false)--drama continue
    
	MakeDroping(5029, LandformPlace(PlayerCharacter().placeX,PlayerCharacter().placeY+1))
	effect_Smoke1(PlayerCharacter().placeX,PlayerCharacter().placeY+1)
	
    coroutine.yield(false)--drama continue
    
    
	storyEventMessageLoopRangeNoWait(_T("introItemUse1_"), 1, 1, str_map, nil)
	
    coroutine.yield(false)--drama continue
    
	cameraMove(PlayerCharacter().placeX,PlayerCharacter().placeY,2,1,1)
	storyEventMessageLoopRangeNoWait(_T("introItemUse1_"), 2, 3, str_map, nil)

    return true--drama end

end

function introItemUseEvent2()
	clearMassage()
	local str_map = map_tstring_StyleString()	
	--ValueString = setStyle( buttonIndex(buttonIndex_MENU), _T("%.0f") ,StyleString_DEFAULT_COLOR,1,1,0)
	--str_map: insert(pair_tstring_StyleString(_T("Button"),ValueString))
	storyEventMessageLoopRangeNoWait(_T("introItemUse1_"), 4, 4, str_map, nil)
end

function introEndEvent()
	clearMassage()
	str_map = map_tstring_StyleString()
	storyEventMessageLoopNoWait(_T("introEndEvent_"),str_map, nil)
    coroutine.yield(false)--drama continue
    clearMassage()
    effect_Wait(PlayerCharacter().placeX,PlayerCharacter().placeY,30)
    coroutine.yield(false)--drama continue
    fadeBlack(20)
    StopBGM()    
    coroutine.yield(false)--drama continue
	storyEventMessageLoopNoWait(_T("introEndEvent1_"),str_map, nil)
	coroutine.yield(false)--drama continue
	setMapValue(savable_localFlags(),_T("introEndEventDone"),YES)
    return true--drama end
end

function floorInitProcess()
	--outputDebugString("floorInitProcess_start\n")

	if floorLevel() == 1 then

        stackDramaThread(coroutine.create(introSelfIntroEvent))
		setMapValue(savable_localFlags(),_T("state"),state_attackTutorial)
	end
	--outputDebugString("point 5\n")

	return

end

function dungeonProcess()
	--outputDebugString("dungeonProcess_start\n")
end


function dungeonTurnProcess()
	--outputDebugString("dungeonTurnProcess_start\n")

	if (getMapValue(savable_localFlags(),_T("state"),NO) == state_attackTutorial) then

		if (enemyPool["first"]:isDead()) then
			--nextState
            stackDramaThread(coroutine.create(introItemUseEvent))
			setMapValue(savable_localFlags(),_T("state"),state_itemTutorial)
			return
		end

		if (getMapValue(savable_localFlags(),_T("attackTutorial_reTutorial"),0) >= 8) then
			clearMassage()
			local str_map = map_tstring_StyleString()
			--ValueString = setStyle( buttonIndex(buttonIndex_DECIDE), _T("%.0f") ,StyleString_DEFAULT_COLOR,1,1,0)
			--str_map: insert(pair_tstring_StyleString(_T("Button"),ValueString))
	        storyEventMessageLoopNoWait(_T("introAttackSupport_"), str_map, nil)
			setMapValue(savable_localFlags(),_T("attackTutorial_reTutorial"), 0)
		end
		setMapValue(savable_localFlags(),_T("attackTutorial_reTutorial"), getMapValue(savable_localFlags(),_T("attackTutorial_reTutorial"),0) +1 )


	elseif (getMapValue(savable_localFlags(),_T("state"),NO) == state_itemTutorial) then

		if (PlayerCharacter().holdItem: size() >= 1) then
			--nextState
			introItemUseEvent2()
			setMapValue(savable_localFlags(),_T("state"),state_itemTutorial2)
			return
		end

	elseif (getMapValue(savable_localFlags(),_T("state"),NO) == state_itemTutorial2) then

		if (PlayerCharacter().holdItem: size() == 0) then
			--nextState
			setMapValue(savable_localFlags(),_T("state"),state_itemTutorial3)
			return
		end

		if (enemyPool["second"]:isDead() and enemyPool["third"]:isDead() and enemyPool["fourth"]:isDead()) then
			--skipState
			stackDramaThread(coroutine.create(introEndEvent))
			setMapValue(savable_localFlags(),_T("state"),state_introEndEvent)
			return
		end

		if (getMapValue(savable_localFlags(),_T("itemTutorial_reTutorial"),0) >= 3) then
			clearMassage()
			local str_map = map_tstring_StyleString()
			--ValueString = setStyle( buttonIndex(buttonIndex_MENU), _T("%.0f") ,StyleString_DEFAULT_COLOR,1,1,0)
			--str_map: insert(pair_tstring_StyleString(_T("Button"),ValueString))
	        storyEventMessageLoopNoWait(_T("introItemSupport_"), str_map, nil)
			setMapValue(savable_localFlags(),_T("itemTutorial_reTutorial"), 0)
		end
		setMapValue(savable_localFlags(),_T("itemTutorial_reTutorial"), getMapValue(savable_localFlags(),_T("itemTutorial_reTutorial"),0) +1 )

	elseif (getMapValue(savable_localFlags(),_T("state"),NO) == state_itemTutorial3) then
	
		if (enemyPool["second"]:isDead() and enemyPool["third"]:isDead() and enemyPool["fourth"]:isDead()) then
			--nextState
			stackDramaThread(coroutine.create(introEndEvent))
			setMapValue(savable_localFlags(),_T("state"),state_introEndEvent)
		end
		
	end

	if (getMapValue(savable_localFlags(),_T("state"),NO) == state_introEndEvent) and
		(getMapValue(savable_localFlags(),_T("introEndEventDone"),NO) == YES) then

		vDroping = PlayerCharacter().holdItem
		size = vDroping: size()
		for i = 0, size-1 do
			if (vDroping: at(i): ID() == 5029) then
				DropingBreak(vDroping: at(i))
				break
			end
		end
		goToNextFloor()
		return
	end

	if (PlayerCharacter().HP < PlayerCharacter().maxHP * 0.66) then
		clearMassage()
		str_map = map_tstring_StyleString()
	    storyEventMessageLoopNoWait(_T("introHPrecover_"), str_map, nil)
		stackFunction(function() CharacterHPRecovery(PlayerCharacter(),100) end)
	end
end

