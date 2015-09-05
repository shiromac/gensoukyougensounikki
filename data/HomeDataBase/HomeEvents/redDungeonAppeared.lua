function meiling_lie(meiling,frame)

    hab = AnimeChip_Hab()

    local eff_obj = AnimeChip_Chara(meiling, frame)
    eff_obj.pitching: addCascade(90,frame)
--[[
    eff_obj.pitching: addCascade(90,30,0,30)
    eff_obj.yawing: addCascade(180,90)
    eff_obj.yawing: addCascade(180,270,315,360,90)
    ]]
    --eff_obj.yawing: addCascade(270,360,30)
    eff_obj.position: addCascade(PositionPerGrid(0, 0, -0.5),frame)
--    eff_obj.position: addCascade(PositionPerGrid(0.3, 0, -0.5),PositionPerGrid(0, 0, 0),30)

    --PositionPerGrid(posX, posY, posZ)
    --eff_obj.color: addCascade(Color(0,255,255,255),Color(255,255,255,255), 60)
    

    --set to hab--
    hab: addAnimeChip(eff_obj);    
    
    --fire--  
    setAnimation_parallel(hab)
    return true
end

function black_back(posX,posY,frame)
    local bg = EventBackground(cEvent.PRIORITY_MIDDLE,_T("event\\black_back.png"), posX, posY, 2)
    coroutine.yield(false)
    --cameraMove(posX,posY,10,1,1)
    bg:fadeIn(frame)
    coroutine.yield(false)
    return bg
end

function redDungeonAppeared()
    fadeBlack(1)
    setHidingMiniMap(true)
    effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,5)
	clearMassage()
	
	str_map = map_tstring_StyleString()
    PlayerCharacter():setStopStamp(true)
    moveWithAnimeToLand(PlayerCharacter(),LandformPlace(19,16),false,10,false)


    coroutine.yield(false)
    sakuya = MakeCharacter(4003, 0, CHARACTER_FORSE_FRIEND, LandformPlace(PlayerCharacter().placeX,PlayerCharacter().placeY-2))
    CharacterTurn(PlayerCharacter(),ASPECT_DOWN)
	CharacterTurn(sakuya,ASPECT_DOWN)
    coroutine.yield(false)

    --------------------------------------------------------------------------

    meiling_lie(PlayerCharacter(),120)
    fadeOut(180)
    coroutine.yield(false)


			
	aspectcoo = Coordinate(sakuya.placeX - PlayerCharacter().placeX, sakuya.placeY - PlayerCharacter().placeY)
			
	--cameraMoveParallel(sakuya.placeX, sakuya.placeY, 1,0.25,0.75)
	CharacterSleep(PlayerCharacter(),1)
	clearMassage()

	meiling_lie(PlayerCharacter(),360)
    effect_Wait(13,14,60)
    cameraMoveParallel(18,14,0.3,0,1)
    effect_Wait(13,14,240)
    coroutine.yield(false)
	stackFunction(CharacterCureMental,PlayerCharacter())
	stackFunction(CharacterVisualRefresh,PlayerCharacter())			
    storyEventMessageLoop(_T("firstEvent1_"), str_map, nil)
    --meiling_wakes_up---------------------------------------------------------------------
    coroutine.yield(false)
    effect_Wait(13,14,30)
	CharacterTurn(PlayerCharacter(),ASPECT_UP)
    PlayerCharacter():setStopStamp(false)
    coroutine.yield(false)
    storyEventMessageLoop(_T("firstEvent2_"), str_map, nil)			
    --remillia_comes_in--------------------------------------------------------------------
    local bg = black_back(18,14,30)    
    effect_Wait(18,14,45)
    coroutine.yield(false)    
    local remi = MakeCharacter(9004, 0, CHARACTER_FORSE_ENEMY, LandformPlace(sakuya.placeX-1,sakuya.placeY))
    effect_Smoke1(sakuya.placeX-1,sakuya.placeY)
    coroutine.yield(false)
    effect_Wait(18,14,30)
    storyEventMessageLoop(_T("firstEvent3_"), str_map, nil)
    coroutine.yield(false)
    --remillia_goes_away--------------------------------------------------------------------
    effect_Wait(18,14,30)
    effect_Smoke1(sakuya.placeX-1,sakuya.placeY)
    CharacterForseGoAway(remi)
    coroutine.yield(false)
    bg:fadeOut(30)
    effect_Wait(18,14,30)
    coroutine.yield(false)    
    storyEventMessageLoop(_T("firstEvent4_"), str_map, nil)
    coroutine.yield(false)
    clearMassage()
	sound = AnimeChip_Sound("pickup.wav",PositionPerGrid(PlayerCharacter().placeX,PlayerCharacter().placeY,0),1.0)
    coroutine.yield(false)
    setAnimation_wait(sound)
    coroutine.yield(false)
    storyEventMessageLoop(_T("firstEvent5_"), str_map, nil)
    coroutine.yield(false)
	effect_Smoke1(19,14)
    coroutine.yield(false)
    CharacterForseGoAway(sakuya)
    coroutine.yield(false)
    storyEventMessageLoop(_T("firstEvent6_"), str_map, nil)
    coroutine.yield(false)
    clearMassage()
			
	--cameraMove(PlayerCharacter().placeX, PlayerCharacter().placeY, 1.5,0.75, 0.9)
    cameraMoveParallel(12,14,0.6,0,1)
    moveWithAnime(PlayerCharacter(), ASPECT_LEFT, 1, false, 0.8, false)
    coroutine.yield(false)
    moveWithAnime(PlayerCharacter(), ASPECT_UPLEFT, 2, false, 0.8, false)
    coroutine.yield(false)
    moveWithAnime(PlayerCharacter(), ASPECT_LEFT, 4, false, 0.8, false)			
    coroutine.yield(false)
	appearDungeon(_T("red_Dungeon"))

    return true
end

function homeEvent()
	if( isDungeonCleared(_T("intro_Dungeon")) )then
		if(not isAppearedDungeon(_T("red_Dungeon")) ) then			

        stackDramaThread(coroutine.create(redDungeonAppeared))
			
		end
	end
end
