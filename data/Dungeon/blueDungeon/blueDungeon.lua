

enemyPool = {}
landPool = {}

function sukima_eff(posX,posY,posZ,frame)
    posX = posX+5;
    posY = posY+5;
    
    hab = AnimeChip_Hab(PositionPerGrid(posX, posY, 0))
    
    eff_obj = AnimeChip_Perticle("effect\\sukima.png",D3DXVECTOR2(256,256), frame)
    --scale_cascading--
    eff_obj.scaleX: addCascade(0,0.95,10)
    eff_obj.scaleY: addCascade(0,5)
    eff_obj.scaleX: addCascade(0.95,1,frame-20)
    eff_obj.scaleY: addCascade(0.95,1,frame-15)
    eff_obj.scaleX: addCascade(1,0,10)
    --eff_obj.scaleY: addCascade(1,0,10)
    --color_cascading--   
    eff_obj.color: addCascade(Color(0,255,255,255),Color(127,255,255,255), 10)
    eff_obj.color: addCascade(Color(127,255,255,255),Color(255,255,255,255), frame-12)
    eff_obj.color: addCascade(Color(255,255,255,255),Color(0,255,255,255),2)
    
    eff_obj.position: addCascade(PositionPerGrid(0,0,posZ),frame-5)
    eff_obj.position: addCascade(PositionPerGrid(0,0,posZ),PositionPerGrid(0,-2,posZ),5)    
    
    eff_obj.rotationAfter: addCascade(0,90,10)
    eff_obj.rotationAfter: addCascade(0,frame-10)   
    
    --set to hab--
    hab: addAnimeChip(eff_obj);        
    setAnimation_parallel(hab)
    return true
end


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
function uwasaEvent()
    if storyEventEnable() then
        local player_land = LandformCharacterPosition(PlayerCharacter())
        local mob_land = findNearEventLand(player_land,5,ASPECT_RIGHT,1)
        local charaA = MakeCharacter(4006, 0, CHARACTER_FORSE_OTHER, mob_land)
        local charaB = MakeCharacter(4006, 0, CHARACTER_FORSE_OTHER, mob_land)
        effect_Smoke1(charaA.placeX, charaA.placeY)
        effect_Smoke1(charaB.placeX, charaB.placeY)
        turnCharacterForCharacter(charaA,charaB)
        turnCharacterForCharacter(charaB,charaA)
        turnCharacterForCharacter(PlayerCharacter(),charaA)
        cameraMoveParallel(charaA.placeX, charaA.placeY, 1.5,0.5,0.75)
        clearMassage()
		str_map = map_tstring_StyleString()
		utility_storyMessage(_T("blueDungeonEvent1_1"),str_map)
		messageWait()
		utility_storyMessage(_T("blueDungeonEvent1_2"),str_map)
		messageWait()
		utility_storyMessage(_T("blueDungeonEvent1_3"),str_map)
		messageWait()
		utility_storyMessage(_T("blueDungeonEvent1_4"),str_map)
		messageWait()
		utility_storyMessage(_T("blueDungeonEvent1_5"),str_map)
		messageWait()
		cameraMove(player_land.place.x, player_land.place.y, 1.5,0.5,0.75)
		CharacterForseGoAway(charaA)
		effect_Smoke1(charaA.placeX, charaA.placeY)
		CharacterForseGoAway(charaB)
		effect_Smoke1(charaB.placeX, charaB.placeY)
		utility_storyMessage(_T("blueDungeonEvent1_6"),str_map)
		messageWait()
		clearMassage()
    end
    
    return true
end


function moveOutAnime(chara, end_land, speed, paralleled)

    local first_place = Coordinate(chara.placeX, chara.placeY)
    local first_land = LandformCharacterPosition(chara)
    local end_place = Coordinate(end_land.place.x, end_land.place.y)

    local hab = AnimeChip_Hab(PositionPerGrid(first_place.x, first_place.y, 0))
    local aspect = (end_place - first_place): getAspect()
    local basic_move_life = 16*(first_place - end_place):dif()/speed
    local chara_anime = AnimeChip_Chara(chara, basic_move_life)
    hab:addAnimeChip(chara_anime)

    --chara_anime: setWorldRotation(chara.aspect*45)
    chara_anime.position: addCascade(PositionPerGrid(0, 0, 0), PositionPerGrid(end_place.x - first_place.x, end_place.y - first_place.y, 0), basic_move_life)
    chara_anime.yawing: addCascade((aspect - chara.aspect)*45 , basic_move_life)
    
    if paralleled then
        setAnimation_parallel(hab)
    else
        setAnimation_wait(hab)
        stackFunction(CharacterWarp, chara, end_land)
		stackFunction(CharacterVisualRefresh, chara)
    end
end
function moveInAnime(chara, end_land, speed, paralleled)
    
    stackFunction(CharacterWarp,chara,end_land)
	stackFunction(CharacterVisualRefresh,chara)
    
    local first_place = Coordinate(chara.placeX, chara.placeY)
    local first_land = LandformCharacterPosition(chara)
    local end_place = Coordinate(end_land.place.x, end_land.place.y)

    local hab = AnimeChip_Hab(PositionPerGrid(first_place.x, first_place.y, 0))
    local aspect = (end_place - first_place): getAspect()
    local basic_move_life = 16*(first_place - end_place):dif()/speed
    local chara_anime = AnimeChip_Chara(chara, basic_move_life)
    hab:addAnimeChip(chara_anime)

    --chara_anime: setWorldRotation(chara.aspect*45)
    chara_anime.position: addCascade(PositionPerGrid(- end_place.x + first_place.x, - end_place.y + first_place.y, 0), PositionPerGrid(0, 0, 0), basic_move_life)
    chara_anime.yawing: addCascade((aspect - chara.aspect)*45 , basic_move_life)
    
    if paralleled then
        setAnimation_parallel(hab)
    else
        setAnimation_wait(hab)
    end
end
function tewiStartEvent()

	bossLand = LandformPlace(20,15);
	emeny = MakeCharacter(5002, 0, CHARACTER_FORSE_ENEMY, bossLand)
	enemyPool["boss"] = emeny
	CharacterTurnNow(PlayerCharacter(), ASPECT_UP)
	
coroutine.yield(false)
	local eirin
	local eirinLand
	local mairinLand
	local mairinLandTrue
    local tewi
	local tewiLand
	if storyEventEnable() then
        str_map = map_tstring_StyleString()
        fadeBlack(1)
	    coroutine.yield(false)
	    eirinLand = LandformPlace(20,20);
	    
        mairinLandTrue = LandformCharacterPosition(PlayerCharacter());
        mairinLand = LandformPlace(20,30);
	    
	    eirin = MakeCharacter(4007, 0, CHARACTER_FORSE_OTHER, eirinLand)
		PlayBGM(_T(""),_T("talk1_main.wav"))

        moveWithAnimeToLand(PlayerCharacter(),mairinLand,false,10,false)
        cameraMove(20,25,10,1,1)
        setHidingMiniMap(true)
        coroutine.yield(false)
        -----go_action-----------------------------------------------------------------------------------
        
        moveWithAnime(PlayerCharacter(), ASPECT_UP, 4, false, 1, false)      
        fadeOut(60)     
        coroutine.yield(false)
		clearMassage()

		
		storyEventMessageLoopRange(_T("blueDungeonBoss_"), 1, 1, str_map, nil)
		moveWithAnime(eirin, ASPECT_DOWN, 4, false, 1, false)
		coroutine.yield(false)
	    --cameraMoveParallel(eirinLand.place.x, eirinLand.place.y, 2, 0.25, 0.75)
		
		storyEventMessageLoopRange(_T("blueDungeonBoss_"), 2, 4, str_map, nil)
		
	    --cameraMoveParallel(mairinLand.place.x, mairinLand.place.y, 2, 0.25, 0.75)
	    
		storyEventMessageLoopRange(_T("blueDungeonBoss_"), 5, 9, str_map, nil)
		
	    --cameraMoveParallel(eirinLand.place.x, eirinLand.place.y, 2, 0.25, 0.75)
	    
        moveWithAnime(eirin, ASPECT_UP, 1, false, 0.7, true)
        coroutine.yield(false)
		storyEventMessageLoopRange(_T("blueDungeonBoss_"), 10, 10, str_map, nil)
		CharacterTurn(PlayerCharacter(),ASPECT_DOWN)
		storyEventMessageLoopRange(_T("blueDungeonBoss_"), 11, 11, str_map, nil)
		CharacterTurn(PlayerCharacter(),ASPECT_UP)
        moveWithAnime(eirin, ASPECT_DOWN, 1, false, 0.7, true)
        coroutine.yield(false)		
	    --cameraMoveParallel(mairinLand.place.x, mairinLand.place.y, 2, 0.25, 0.75)
	    StopBGM()
		storyEventMessageLoopRange(_T("blueDungeonBoss_"), 12, 13, str_map, nil)
		PlayBGM(_T("talk_strain_intro.wav"),_T("talk_strain_main.wav"))
	    --cameraMoveParallel(eirinLand.place.x, eirinLand.place.y, 3, 0.7, 1.0)
	    
		storyEventMessageLoopRange(_T("blueDungeonBoss_"), 14, 15, str_map, nil)
		
	    --cameraMoveParallel(mairinLand.place.x, mairinLand.place.y, 2, 0.25, 0.75)
	    
		storyEventMessageLoopRange(_T("blueDungeonBoss_"), 16, 19, str_map, nil)

        ---tewi_comes_in-------------------------------------------------------------------------------------------------------
        tewiLand = LandformPlace(14,26);
        tewi = MakeCharacter(4009, 0, CHARACTER_FORSE_OTHER, tewiLand)
        coroutine.yield(false)
        --moveWithAnime(tewi, ASPECT_LEFT, 3, false, 1, false)
        moveWithAnime(tewi, ASPECT_RIGHT, 4, false, 1, false)
        coroutine.yield(false)
        turnCharacterForCharacter(PlayerCharacter(),tewi)
        coroutine.yield(false)
	    --cameraMoveParallel(eirinLand.place.x, eirinLand.place.y, 2, 0.25, 0.75)
        storyEventMessageLoopRange(_T("blueDungeonBoss_"), 20, 22, str_map, nil)

        ---eirin_goes_away--------------------------------------------------------------------------------------------------------
	    moveWithAnime(eirin, ASPECT_UP, 2, false, 1, false)
  		coroutine.yield(false)
	    moveWithAnime(eirin, ASPECT_UP, 2, false, 1, false)
        --coroutine.yield(false)        
		---tewi_traces_eirin-------------------------------------------------------------------------------------------------------
		moveWithAnime(tewi, ASPECT_UPRIGHT, 2, false, 1, false)
		coroutine.yield(false)
		moveWithAnime(eirin, ASPECT_UP, 2, false, 1, false)
        CharacterTurn(tewi,ASPECT_UP)
        turnCharacterForCharacter(PlayerCharacter(),tewi)
		coroutine.yield(false)
        CharacterForseGoAway(eirin)
		coroutine.yield(false)
		storyEventMessageLoopRange(_T("blueDungeonBoss_"), 23, 23, str_map, nil)
        coroutine.yield(false)
        CharacterTurn(tewi,ASPECT_DOWN)        
        coroutine.yield(false)
        storyEventMessageLoopRange(_T("blueDungeonBoss_"), 24, 25, str_map, nil)				
    else
	    stackFunction(CharacterWarp,emeny ,LandformPlace(20,15))
        stackFunction(CharacterVisualRefresh,emeny)

        cameraMove(20,15,10,1,1)
        coroutine.yield(false)
        CharacterTurn(PlayerCharacter(),ASPECT_UP)
        PlayBGM(_T("boss_no_event_intro.wav"),_T("boss_main.wav"))
        coroutine.yield(false)
        effect_Wait(20,15,60)
        cameraMove(PlayerCharacter().placeX,PlayerCharacter().placeY,3,1,1)
        coroutine.yield(false)   
        
        
	end
    ----boss_biginning------------------------------------------------------------------------------------------------------------	
coroutine.yield(false)


	
    if storyEventEnable() then	
		--cameraMove(PlayerCharacter().placeX, PlayerCharacter().placeY, 2, 0.5, 1)
      	PlayBGM(_T("boss_intro.wav"),_T("boss_main.wav"))       
		storyEventMessageLoopRange(_T("blueDungeonBoss_"), 26, 26, str_map, nil)
		coroutine.yield(false)		
	    clearMassage()
	end
		
	CharacterTurnNow(emeny, ASPECT_UP)
	

	if storyEventEnable() then
	fadeWhite(30)
    --effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,30)
    
    --jumpWithAnimeToLand(boss,bossLand,1.0,true,0.3,false)
    coroutine.yield(false)
    CharacterForseGoAway(tewi)
    coroutine.yield(false)
    jumpWithAnimeToLand(PlayerCharacter(),mairinLand,1.0,true,0.3,false)
    cameraMove(PlayerCharacter().placeX,PlayerCharacter().placeY,1,1,1)
    setHidingMiniMap(false)
    coroutine.yield(false)
    cameraMove(PlayerCharacter().placeX, PlayerCharacter().placeY, 2, 0.5, 1)
    coroutine.yield(false)
    --PlayBGM(_T("boss_intro.wav"),_T("boss_main.wav"))
    
    in_frame = 30
    fadeOut(in_frame)
    effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,in_frame)
    coroutine.yield(false)
	
	
	end
	
	
	
	CharacterTurn(emeny, ASPECT_DOWN)
    clearMassage()
    
coroutine.yield(false)
	--damege to start
	startBossSpell(enemyPool["boss"])
	
return true
end

function tewiEndEvent(boss)

    CharacterCureMental(PlayerCharacter())
    CharacterCurePhysical(PlayerCharacter())
    CharacterCureMagic(PlayerCharacter())
    CharacterCureSpeed(PlayerCharacter())
    
    boss = enemyPool["boss"]
	vec_chara = allCharacterList()
	charalistsize = vec_chara: size()
	for i = 0, charalistsize-1 do
		if (vec_chara:at(i).forse == CHARACTER_FORSE_ENEMY) then
			if(vec_chara:at(i):ID() ~= 5002) then
				--not tewi
				CharacterForseGoAway(vec_chara:at(i))
			end
		end
	end

coroutine.yield(false)
	if storyEventEnable() then
	    fadeWhite(30)
	    effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,30)
	    CharacterForseGoAway(boss)
        setHidingMiniMap(true)
        coroutine.yield(false)
        eraseFloorDropings()
   	    local bossLand = LandformPlace(20,13)
	    local tewi = MakeCharacter(4009, 0, CHARACTER_FORSE_OTHER, bossLand)  
        local playerLand = LandformPlace(20,15)
  	    --moveWithAnimeToLand(tewi,bossLand,false,10,false)
  	    moveWithAnimeToLand(PlayerCharacter(),playerLand,false,10,false)	    
        coroutine.yield(false)

	    turnCharacterForCharacter(tewi,PlayerCharacter())
	    turnCharacterForCharacter(PlayerCharacter(),tewi)
	    coroutine.yield(false)
	    --cameraMoveParallel(bossLand.place.x, bossLand.place.y, 1, 0.5, 1)
	    cameraMove(playerLand.place.x, playerLand.place.y-1, 10, 0.5, 1)
	    coroutine.yield(false)
	    local erinland = findNearEventLand(bossLand,5,ASPECT_RIGHT,1)
		
	    PlayBGM(_T(""),_T("talk1_main.wav"))
        coroutine.yield(false)
        ---go_action-------------------------------------------------------------------------------------------
        fadeOut(60)
		clearMassage()
		str_map = map_tstring_StyleString()
		storyEventMessageLoopRange(_T("blueDungeonAfterBoss_"), 1, 8, str_map, nil)
        coroutine.yield(false)
		
		local erin = MakeCharacter(4007, 0, CHARACTER_FORSE_FRIEND, erinland)
		--effect_Smoke1(erin.placeX, erin.placeY)
		moveWithAnime(erin, ASPECT_LEFT, 3, false, 1.0, true)
coroutine.yield(false)

		storyEventMessageLoopRange(_T("blueDungeonAfterBoss_"), 9, 9, str_map, nil)
        coroutine.yield(false)
   		turnCharacterForCharacter(tewi,erin)       
		storyEventMessageLoopRange(_T("blueDungeonAfterBoss_"), 10, 11, str_map, nil)
        coroutine.yield(false)
        turnCharacterForCharacter(PlayerCharacter(),erin)
        turnCharacterForCharacter(erin,PlayerCharacter())
coroutine.yield(false)
	    --cameraMoveParallel(playerLand.place.x, playerLand.place.y, 1000, 0.5, 1)
		storyEventMessageLoopRange(_T("blueDungeonAfterBoss_"), 12, 13, str_map, nil)
coroutine.yield(false)
		--effect_Smoke1(bossLand.place.x, bossLand.place.y)
		--CharacterForseGoAway(boss)
	    --cameraMoveParallel(erinland.place.x, erinland.place.y, 1000, 0.5, 1)
		storyEventMessageLoopRange(_T("blueDungeonAfterBoss_"), 14, 14, str_map, nil)
coroutine.yield(false)
	    --cameraMoveParallel(playerLand.place.x, playerLand.place.y, 1000, 0.5, 1)
		storyEventMessageLoopRange(_T("blueDungeonAfterBoss_"), 15, 15, str_map, nil)
coroutine.yield(false)
	    --cameraMoveParallel(erinland.place.x, erinland.place.y, 1, 0.5, 1)
		storyEventMessageLoopRange(_T("blueDungeonAfterBoss_"), 16, 28, str_map, nil)
coroutine.yield(false)
        --cameraMove(playerLand.place.x, playerLand.place.y, 1, 0.5, 1)
        
        turnCharacterForCharacter(erin,tewi)
        coroutine.yield(false)       
		storyEventMessageLoopRange(_T("blueDungeonAfterBoss_"), 29, 32, str_map, nil)
        coroutine.yield(false)
        CharacterTurn(PlayerCharacter(),ASPECT_DOWN)         
        coroutine.yield(false)
		storyEventMessageLoopRange(_T("blueDungeonAfterBoss_"), 33, 34, str_map, nil)
        coroutine.yield(false)
        CharacterTurn(PlayerCharacter(),ASPECT_UP)         
        coroutine.yield(false)
		storyEventMessageLoopRange(_T("blueDungeonAfterBoss_"), 35, 36, str_map, nil)
        coroutine.yield(false)
        CharacterTurn(tewi,ASPECT_DOWN)         
        coroutine.yield(false)
		storyEventMessageLoopRange(_T("blueDungeonAfterBoss_"), 37, 37, str_map, nil)
coroutine.yield(false)

        clearMassage()
coroutine.yield(false)        
        --tewi_and_meiling_go_home------------------------------------------------------------------------------
        --CharacterTurn(PlayerCharacter(),ASPECT_DOWN)
        CharacterTurn(erin,ASPECT_DOWN)
        moveWithAnime(tewi, ASPECT_DOWNLEFT, 1, false, 1.0, true)
        coroutine.yield(false)  
        moveWithAnime(tewi, ASPECT_DOWN, 3, false, 1.0, true)
coroutine.yield(false)       
        CharacterTurn(tewi,ASPECT_UP)
        CharacterTurn(PlayerCharacter(),ASPECT_DOWN)                 
coroutine.yield(false)
        jumpWithAnime(PlayerCharacter(),ASPECT_DOWN, 0, 0.7, false, 1.0, true)
coroutine.yield(false)
        moveWithAnime(PlayerCharacter(), ASPECT_DOWN, 10, false, 1.0, true)
        moveWithAnime(tewi, ASPECT_DOWN, 9, false, 1.0, true)
        coroutine.yield(false)     


        --ayayayayaya------------------------------------------------------------------------------------------
        local aya_pos = LandformPlace(13,12)
        local aya = MakeCharacter(9018, 0, CHARACTER_FORSE_ENEMY, aya_pos)
        StopBGM()
        CharacterTurn(aya,ASPECT_DOWNRIGHT)
        coroutine.yield(false)  
        cameraMove(13,13, 1, 0.5, 1)
        coroutine.yield(false)
        CharacterTurn(erin,ASPECT_LEFT)
        storyEventMessageLoop(_T("blueDungeonAyaya_"), str_map, nil)                
        coroutine.yield(false)
        moveWithAnime(aya, ASPECT_LEFT, 5, false, 1.2, true)
        CharacterTurn(erin,ASPECT_LEFT)
        coroutine.yield(false)
        
        --yukari_x_eirin------------------------------------------------------------------------------------------        
        cameraMove(erin.placeX-2,erin.placeY, 1, 0.5, 1)
        coroutine.yield(false)
        storyEventMessageLoop(_T("blueDungeonBoss2_"), str_map, nil)
        coroutine.yield(false)
        clearMassage()        
        sukima_eff(erin.placeX-2,erin.placeY,1,60)
        effect_Wait(erin.placeX-2,erin.placeY,30)
        local yukari = MakeCharacter(9045, 0, CHARACTER_FORSE_FRIEND, LandformPlace(erin.placeX-2,erin.placeY))
        coroutine.yield(false)
        effect_Wait(erin.placeX-2,erin.placeY,70)

        
        --effect_Smoke1(yukari.placeX,yukari.placeY)
        coroutine.yield(false)
        PlayBGM(_T(""),_T("talk2_main.wav"))
        --storyEventMessageLoop(_T("blueDungeonBoss3_"), str_map, nil)

        CharacterTurn(yukari,ASPECT_RIGHT)
        coroutine.yield(false)

		storyEventMessageLoopRange(_T("blueDungeonBoss3_"), 1, 1, str_map, nil)
coroutine.yield(false)
        moveWithAnime(yukari, ASPECT_LEFT, 2, false, 0.3, true)
        coroutine.yield(false)        
		storyEventMessageLoopRange(_T("blueDungeonBoss3_"), 2, 5, str_map, nil)
coroutine.yield(false)
        CharacterTurn(yukari,ASPECT_RIGHT)

storyEventMessageLoopRange(_T("blueDungeonBoss3_"), 6, 8, str_map, nil)
        coroutine.yield(false)

        CharacterTurn(yukari,ASPECT_LEFT)
        coroutine.yield(false)

storyEventMessageLoopRange(_T("blueDungeonBoss3_"), 9, 11, str_map, nil)
        coroutine.yield(false)
        moveWithAnime(erin, ASPECT_LEFT, 1, false, 0.3, true)        
        coroutine.yield(false)                
        erin:setStopStamp(true)
		storyEventMessageLoopRange(_T("blueDungeonBoss3_"), 12, 12, str_map, nil)
coroutine.yield(false)
        clearMassage()
        yukari:setStopStamp(true)
        effect_Wait(erin.placeX-2,erin.placeY,30)
        coroutine.yield(false)
        CharacterTurn(yukari,ASPECT_UP)
        coroutine.yield(false)
        effect_Wait(erin.placeX-2,erin.placeY,30)
        CharacterTurn(yukari,ASPECT_RIGHT)
        coroutine.yield(false)
        effect_Wait(erin.placeX-2,erin.placeY,30)
		storyEventMessageLoopRange(_T("blueDungeonBoss3_"), 13, 13, str_map, nil)
coroutine.yield(false)

        coroutine.yield(false)

	end
	clearMassage()
	goToNextFloor()
return true
end

--deligatemethod
function eventBossEnd(object)
	enemyPool["boss"] = getCharacterStorage("boss")
    stackDramaThread(coroutine.create(tewiEndEvent))
end

function startSpell(index)
	outputDebugString("startSpell_start\n")

end


function floorInitProcess()
	--outputDebugString("floorInitProcess_start\n")

    --tewiStartEvent()

	if floorLevel() == 15 then

        stackDramaThread(coroutine.create(tewiStartEvent))
	end
	
	if storyEventEnable() then
		if floorLevel() == 4 then
		    stackDramaThread(coroutine.create(uwasaEvent))
		end
		
	end
	--outputDebugString("point 5\n")

	return

end

function dungeonProcess()
	--outputDebugString("dungeonProcess_start\n")
end


function dungeonTurnProcess()
	--outputDebugString("dungeonTurnProcess_start\n")

end