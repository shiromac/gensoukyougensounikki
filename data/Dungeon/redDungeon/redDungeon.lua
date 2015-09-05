

enemyPool = {}
landPool = {}
function remiliaStartEvent()


    fadeBlack(1)
    setHidingMiniMap(true)
    effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,5)
    cameraMove(talkposX,talkposY,10,1,1)
    local destRemi = LandformPlace(talkposX,talkposY-2)
    local destPC = LandformPlace(talkposX,talkposY+3)
    local remi = MakeCharacter(9004, 0, CHARACTER_FORSE_ENEMY, destRemi)
    moveWithAnimeToLand(PlayerCharacter(),destPC,false,10,false)
    PlayBGM(_T(""),_T("talk1_main.wav"))
    coroutine.yield(false)

    CharacterTurnNow(remi, ASPECT_UP)

   	str_map = map_tstring_StyleString()

    -----go_action-----------------------------------------------------------------------------------
    in_frame = 60
    fadeOut(in_frame)
    --effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,in_frame)		
				
	clearMassage()

	CharacterTurnNow(PlayerCharacter(), ASPECT_UP)

	plplace = Coordinate(PlayerCharacter().placeX, PlayerCharacter().placeY)
    moveWithAnime(PlayerCharacter(), ASPECT_UP, 2, false, 1, false)
    coroutine.yield(false)
    
    effect_Wait(talkposX,talkposY,30)
				
    storyEventMessageLoopRange(_T("redDungeonBoss_"), 1, 2, str_map, nil)
    coroutine.yield(false)
    CharacterTurn(remi, ASPECT_DOWN)
    coroutine.yield(false)
	--damege to start
	

	storyEventMessageLoopRange(_T("redDungeonBoss_"), 3, 7, str_map, nil)
    coroutine.yield(false)
    StopBGM()
    coroutine.yield(false)
	storyEventMessageLoopRange(_T("redDungeonBoss_"), 8, 8, str_map, nil)
    coroutine.yield(false)		
    PlayBGM(_T("talk_strain_intro.wav"),_T("talk_strain_main.wav"))
    coroutine.yield(false)   
	storyEventMessageLoopRange(_T("redDungeonBoss_"), 9, 14, str_map, nil)
    coroutine.yield(false)	
	PlayBGM(_T("boss_intro.wav"),_T("boss_main.wav"))
    coroutine.yield(false)   
	storyEventMessageLoopRange(_T("redDungeonBoss_"), 15, 15, str_map, nil)
    coroutine.yield(false)
	
    fadeWhite(30)
    jumpWithAnimeToLand(PlayerCharacter(),pcLand,1.0,true,0.3,false)
    cameraMove(PlayerCharacter().placeX,PlayerCharacter().placeY,1,1,1)

    coroutine.yield(false)
    
    in_frame = 30
    fadeOut(in_frame)
    effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,in_frame)

    coroutine.yield(false)   
    CharacterForseGoAway(remi)
    setHidingMiniMap(false)
    coroutine.yield(false)    
    startBossSpell(boss)
    return true
end


function remiliaEndEvent()
    CharacterCureMental(PlayerCharacter())
    CharacterCurePhysical(PlayerCharacter())
    CharacterCureMagic(PlayerCharacter())
    CharacterCureSpeed(PlayerCharacter())
    -----red_After---------------------set_talk_studio-----------------------------------------------
    fadeWhite(30)
    coroutine.yield(false)
    eraseFloorDropings()
    setHidingMiniMap(true)
    effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,30)
    cameraMove(talkposX,talkposY,10,1,1)
    CharacterForseGoAway(boss)

    coroutine.yield(false)
	vec_chara = allCharacterList()
	charalistsize = vec_chara: size()
	for i = 0, charalistsize-1 do
		if (vec_chara:at(i).forse == CHARACTER_FORSE_ENEMY) then
			if(vec_chara:at(i):ID() ~= 5000) then
				--not remilia
				CharacterForseGoAway(vec_chara:at(i))
			end
		end
	end
 
   coroutine.yield(false)
	str_map = map_tstring_StyleString()
    
    local remiLand = LandformPlace(talkposX,talkposY-2)
    local destPC = LandformPlace(talkposX,talkposY)
    local remi = MakeCharacter(9004, 0, CHARACTER_FORSE_ENEMY, remiLand)
    moveWithAnimeToLand(PlayerCharacter(),destPC,false,10,false)
    coroutine.yield(false)
    CharacterTurn(remi,ASPECT_DOWN)
    CharacterTurn(PlayerCharacter(),ASPECT_UP)



    
    PlayBGM(_T(""),_T("talk1_main.wav"))
    coroutine.yield(false)

	in_frame = 60    
    fadeOut(in_frame)
    coroutine.yield(false)
    
    playerLand = LandformCharacterPosition(PlayerCharacter())
	playerPlace = playerLand.place
	aspectcoo = Coordinate(playerLand.place - bossLand.place)

    coroutine.yield(false)
    sakuyaLand = landformAroundOfCharacter(remi,3,1);
	sakuyaLand2 = landformAroundOfCharacter(PlayerCharacter(),ASPECT_LEFT,2);
    ayaLand = landformAroundOfCharacter(PlayerCharacter(),3,8);

	sakuyaPlace2 = sakuyaLand.place
	aspectcoo2 = Coordinate(sakuyaLand2.place - playerLand.place)

    -----go_action-----------------------------------------------------------------------------------
    in_frame = 60
    fadeOut(in_frame)
		
    coroutine.yield(false)

	storyEventMessageLoop(_T("redDungeonBossEnd1_"), str_map, nil)

    coroutine.yield(false)
    
	landPool["sakuya"] = sakuyaLand;
	sakuyaPlace = sakuyaLand.place
	local sakuya = MakeCharacter(4003, 0, CHARACTER_FORSE_FRIEND, landPool["sakuya"])
	effect_Smoke1(sakuya.placeX, sakuya.placeY)
	
    coroutine.yield(false)
    cameraMove(sakuya.placeX, sakuya.placeY,0.7,1,1)

    coroutine.yield(false)
	enemyPool["sakuya"] = sakuya
    turnCharacterForCharacter(sakuya,remi)
    turnCharacterForCharacter(remi,sakuya)
    coroutine.yield(false)		

	storyEventMessageLoop(_T("redDungeonBossEnd2_"), str_map, nil)
    coroutine.yield(false)
    clearMassage()
    turnCharacterForCharacter(remi,PlayerCharacter())
    cameraMove(talkposX, talkposY,0.7,1,1)
    coroutine.yield(false)
--	storyEventMessageLoop(_T("redDungeonBossEnd3_"), str_map, nil)
    storyEventMessageLoopRange(_T("redDungeonBossEnd3_"),1,1, str_map, nil)
    coroutine.yield(false)
    --remilia out
	--effect_Smoke1(boss.placeX, boss.placeY)
	--CharacterForseGoAway(remi)
    remi:setStopStamp(true)
    jumpWithAnime(remi,ASPECT_RIGHT, 10, 3, false, 0.32, true)
    CharacterTurn(PlayerCharacter(),ASPECT_RIGHT)
    CharacterTurn(sakuya,ASPECT_RIGHT)
    coroutine.yield(false)
    storyEventMessageLoopRange(_T("redDungeonBossEnd3_"),2,2, str_map, nil)
    coroutine.yield(false)   
    clearMassage()

    coroutine.yield(false)
    landPool["sakuya2"] = sakuyaLand2;
	effect_Smoke1(enemyPool["sakuya"].placeX, enemyPool["sakuya"].placeY)
	CharacterWarp(enemyPool["sakuya"], landPool["sakuya2"])
	effect_Smoke1(enemyPool["sakuya"].placeX, enemyPool["sakuya"].placeY)

    coroutine.yield(false)
    turnCharacterForCharacter(sakuya,PlayerCharacter())
--	CharacterTurn(enemyPool["sakuya"], aspectcoo2:getAspect()+4)
--	CharacterTurn(PlayerCharacter(), aspectcoo2:getAspect())
    coroutine.yield(false)
	cameraMove(enemyPool["sakuya"].placeX+1, enemyPool["sakuya"].placeY, 1,0.25,0.75)
    turnCharacterForCharacter(PlayerCharacter(),sakuya)
    coroutine.yield(false)
	storyEventMessageLoop(_T("redDungeonBossEnd4_"), str_map, nil)	

    coroutine.yield(false)
	landPool["aya"] = ayaLand;
	enemyPool["aya"] = MakeCharacter(4004, 0, CHARACTER_FORSE_FRIEND, landPool["aya"])
	aspectcoo3 = Coordinate(landPool["aya"].place - playerLand.place)
	CharacterTurn(enemyPool["aya"], aspectcoo3:getAspect()+4)
	ayaPlace = ayaLand.place
	cameraMove(ayaPlace.x, ayaPlace.y, 0.5,0,0.8)
    coroutine.yield(false)
	storyEventMessageLoop(_T("redDungeonBossEnd5_"), str_map, nil)
    coroutine.yield(false)
	clearMassage()
    coroutine.yield(false)

    --storyEventMessageLoop(_T("greenDungeonAfterBoss_"), str_map, nil)
	
	goToNextFloor()
	
	return true;
end

function frandreAppear()
    local boss = enemyPool["boss"]

    effect_Smoke1(boss.placeX, boss.placeY)
    CharacterWarp(boss, LandformPlace(20,20))
    
    
    coroutine.yield(false)

    CharacterVisualRefresh(boss)
    effect_Smoke1(boss.placeX, boss.placeY)
    coroutine.yield(false)

    if storyEventEnable() then
        cameraMove(20, 20, 2,0.25,0.75)
    end
    
    coroutine.yield(false)

    chara = MakeCharacter(5001, 0, CHARACTER_FORSE_ENEMY, LandformPlace(21,20))
    enemyPool["fran1"] = chara
    effect_Smoke1(21,20)
    
    if storyEventEnable() then
        cameraMoveParallel(21,20, 1,0.25,0.75)

        clearMassage()
        str_map = map_tstring_StyleString()
        
	    storyEventMessageLoopRange(_T("redDungeonBossEvent1_"), 1, 1, str_map, nil)
    end
    
    coroutine.yield(false)
    
    chara = MakeCharacter(5001, 0, CHARACTER_FORSE_ENEMY, LandformPlace(19,20))
    enemyPool["fran2"] = chara
    effect_Smoke1(19,20)
        
    
    chara = MakeCharacter(5001, 0, CHARACTER_FORSE_ENEMY, LandformPlace(20,21))
    enemyPool["fran3"] = chara
    effect_Smoke1(20,21)
        

    chara = MakeCharacter(5001, 0, CHARACTER_FORSE_ENEMY, LandformPlace(20,19))
    enemyPool["fran4"] = chara
    effect_Smoke1(20,19)
    
coroutine.yield(false)

    if storyEventEnable() then
        cameraMove(20, 20, 2,0.75,0.1)

        clearMassage()
        str_map = map_tstring_StyleString()
	    storyEventMessageLoopRange(_T("redDungeonBossEvent1_"), 2, 2, str_map, nil)
coroutine.yield(false)
        cameraMove(PlayerCharacter().placeX, PlayerCharacter().placeY, 2,0.75,1)

	    storyEventMessageLoopRange(_T("redDungeonBossEvent1_"), 3, 3, str_map, nil)

    end
    
coroutine.yield(false)
    effect_Smoke1(enemyPool["fran1"].placeX, enemyPool["fran1"].placeY)
    landPool["fran1"] = LandformRandom_Random()
    stackFunction(function(enemy, land) CharacterWarp(enemy, land) end, enemyPool["fran1"], landPool["fran1"])
    effect_Smoke1(landPool["fran1"].place.x, landPool["fran1"].place.y)
    
    effect_Smoke1(enemyPool["fran2"].placeX, enemyPool["fran2"].placeY)
    landPool["fran2"] = LandformRandom_Random()
    stackFunction(function() CharacterWarp(enemyPool["fran2"], landPool["fran2"]) end)
    effect_Smoke1(landPool["fran2"].place.x, landPool["fran2"].place.y)
    
    effect_Smoke1(enemyPool["fran3"].placeX, enemyPool["fran3"].placeY)
    landPool["fran3"] = LandformRandom_Random()
    stackFunction(function() CharacterWarp(enemyPool["fran3"], landPool["fran3"]) end)
    effect_Smoke1(landPool["fran3"].place.x, landPool["fran3"].place.y)
    
    effect_Smoke1(enemyPool["fran4"].placeX, enemyPool["fran4"].placeY)
    landPool["fran4"] = LandformRandom_Random()
    stackFunction(function() CharacterWarp(enemyPool["fran4"], landPool["fran4"]) end)
    effect_Smoke1(landPool["fran4"].place.x, landPool["fran4"].place.y)
    clearMassage()
return true
end

function bossEndEvent()
    
    if storyEventEnable() then
        remiliaEndEvent()
    end
    coroutine.yield(false)
    goToNextFloor()
	return true
end

function eventBossEnd(object)
	local boss_sender = getCharacterStorage("boss")
	if(boss_sender: ID() == 5000)then--remilia
	    enemyPool["boss"] = boss_sender
        stackDramaThread(coroutine.create(bossEndEvent))
	end
end


function eventFiveOfAKindBefore(object)
	local boss_sender = getCharacterStorage("boss")
	if(boss_sender: ID() == 5000)then--remilia
	    enemyPool["boss"] = boss_sender
        stackDramaThread(coroutine.create(frandreAppear))
	end
end

function bossStartEvent()
    ------------------------common_initialization_for_boss_event----------
    bossposX = 20
    bossposY = 15
	bossLand = LandformPlace(bossposX,bossposY);
	pcLand = LandformPlace(PlayerCharacter().placeX,PlayerCharacter().placeY)
	str_map = map_tstring_StyleString()

    talkposX = (bossposX + PlayerCharacter().placeX)/2
    talkposY = (bossposY + PlayerCharacter().placeY)/2

    --------------------------------------------------------------------------
    
    --call_boss#5000:remillia---
    boss = MakeCharacter(5000, 0, CHARACTER_FORSE_ENEMY, bossLand)    
    --stackDramaThread(coroutine.create(greenBossBefore))
    if storyEventEnable() then
        remiliaStartEvent()
    else
        cameraMove(bossposX,bossposY,10,1,1)
        coroutine.yield(false)
        PlayBGM(_T("boss_no_event_intro.wav"),_T("boss_main.wav"))
        coroutine.yield(false)
        CharacterTurn(PlayerCharacter(),ASPECT_UP)
        effect_Wait(bossposX,bossposY,60)
        cameraMove(PlayerCharacter().placeX,PlayerCharacter().placeY,3,1,1)
        coroutine.yield(false)        

        startBossSpell(boss)
    end
    
	
	return true
end

function pickedUpSpellEvent()

	if storyEventEnable() then
	    storyEventMessageLoop(_T("redDungeonSpell_"), map_tstring_StyleString(), nil)
	end
end

function floorInitProcess()
	--outputDebugString("floorInitProcess_start\n")

	if floorLevel() == 8 then

        stackDramaThread(coroutine.create(bossStartEvent))
	end
	
	if storyEventEnable() then
		if floorLevel() == 6 then
			clearMassage()
	        storyEventMessageLoop(_T("redDungeonIntoToshokan_"), map_tstring_StyleString(), nil)
	        clearMassage()
		end
		if floorLevel() == 4 then
			clearMassage()
	        storyEventMessageLoop(_T("redDungeonIntoKoumakan_"), map_tstring_StyleString(), nil)
	        clearMassage()
		end
		if floorLevel() == 1 then
			clearMassage()
	        storyEventMessageLoop(_T("redDungeonStart_"), map_tstring_StyleString(), nil)
	        clearMassage()
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
	if (getMapValue(savable_localFlags(),_T("spellPickedup"),NO) == NO) then

		itemsize = PlayerCharacter().holdItem: size()
		for i = 0,itemsize-1 do
			if( PlayerCharacter().holdItem:at(i). attribute: count(DROPING_ATTRIBUTE_SPELL) ~= 0) then
				setMapValue(savable_localFlags(),_T("spellPickedup"),YES)
				--nextState
				pickedUpSpellEvent()
				break
			end
		end
		
	end
end