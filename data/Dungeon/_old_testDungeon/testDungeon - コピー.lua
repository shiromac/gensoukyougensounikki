

enemyPool = {}
landPool = {}

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
    end
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

	bossLand = LandformPlace(20,10);
	emeny = MakeCharacter(5002, 0, CHARACTER_FORSE_ENEMY, bossLand)
	enemyPool["boss"] = emeny
	
	local eirin
	local eirinLand
	if storyEventEnable() then
	    eirinLand = LandformPlace(20,15);
	    eirin = MakeCharacter(4007, 0, CHARACTER_FORSE_OTHER, eirinLand)
	    cameraMoveParallel(eirinLand.place.x, eirinLand.place.y, 2, 0.5, 1)
		
		clearMassage()
		str_map = map_tstring_StyleString()
		utility_storyMessage(_T("blueDungeonBoss1"),str_map)
		messageWait()
		utility_storyMessage(_T("blueDungeonBoss2"),str_map)
		messageWait()
		utility_storyMessage(_T("blueDungeonBoss3"),str_map)
		messageWait()
		utility_storyMessage(_T("blueDungeonBoss4"),str_map)
		messageWait()
		utility_storyMessage(_T("blueDungeonBoss5"),str_map)
		messageWait()
		utility_storyMessage(_T("blueDungeonBoss6"),str_map)
		messageWait()
		utility_storyMessage(_T("blueDungeonBoss7"),str_map)
		messageWait()
		utility_storyMessage(_T("blueDungeonBoss8"),str_map)
		messageWait()
		utility_storyMessage(_T("blueDungeonBoss9"),str_map)
		messageWait()
		utility_storyMessage(_T("blueDungeonBoss10"),str_map)
		messageWait()
		utility_storyMessage(_T("blueDungeonBoss11"),str_map)
		messageWait()
		utility_storyMessage(_T("blueDungeonBoss12"),str_map)
		messageWait()
		--
		moveOutAnime(eirin,LandformPlace(20,9),1,false)

		moveInAnime(emeny,LandformPlace(20,15),1,false)
    else
	    stackFunction(CharacterWarp,emeny ,LandformPlace(20,15))
        stackFunction(CharacterVisualRefresh,emeny)
	end
	

    if storyEventEnable() then	
        

		--
		utility_storyMessage(_T("blueDungeonBoss13"),str_map)
		messageWait()
		utility_storyMessage(_T("blueDungeonBoss14"),str_map)
		messageWait()
		cameraMove(PlayerCharacter().placeX, PlayerCharacter().placeY, 2, 0.5, 1)
		
		stackFunction(CharacterForseGoAway,eirin)
	end
		
	CharacterTurnNow(emeny, ASPECT_UP)
	

	if storyEventEnable() then
	
	end
	
	CharacterTurn(emeny, ASPECT_DOWN)

	--damege to start
	stackFunction(function() CharacterForsedDamage(enemyPool["boss"],1) end)
end
--deligatemethod
function eventBossEnd(object)
	boss = getCharacterStorage("boss")
	tewiEndEvent(boss)
end

function tewiEndEvent(boss)

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
	if storyEventEnable() then
	    local bossLand = LandformCharacterPosition(boss);
	    turnCharacterForCharacter(PlayerCharacter(),boss)
	    turnCharacterForCharacter(boss,PlayerCharacter())
	    cameraMoveParallel(bossLand.place.x, bossLand.place.y, 1, 0.5, 1)
		clearMassage()
		str_map = map_tstring_StyleString()
		utility_storyMessage(_T("blueDungeonBoss15"),str_map)
		messageWait()
		utility_storyMessage(_T("blueDungeonBoss16"),str_map)
		messageWait()
		utility_storyMessage(_T("blueDungeonBoss17"),str_map)
		messageWait()
		utility_storyMessage(_T("blueDungeonBoss18"),str_map)
		effect_Smoke1(bossLand.place.x, bossLand.place.y)
		CharacterForseGoAway(boss)
		
		local eirinLand = findNearEventLand(bossLand,12,ASPECT_LEFT,1)
		local eirin = MakeCharacter(4007, 0, CHARACTER_FORSE_OTHER, eirinLand)
		turnCharacterForCharacter(eirin,PlayerCharacter())
		effect_Smoke1(eirin.placeX, eirin.placeY)
	    cameraMoveParallel(eirinLand.place.x, eirinLand.place.y, 1, 0.5, 1)
		messageWait()
		utility_storyMessage(_T("blueDungeonBoss19"),str_map)
		messageWait()
		utility_storyMessage(_T("blueDungeonBoss20"),str_map)
		messageWait()
		utility_storyMessage(_T("blueDungeonBoss21"),str_map)
		messageWait()
		utility_storyMessage(_T("blueDungeonBoss22"),str_map)
		messageWait()
		utility_storyMessage(_T("blueDungeonBoss23"),str_map)
		messageWait()
		utility_storyMessage(_T("blueDungeonBoss24"),str_map)
		messageWait()
		utility_storyMessage(_T("blueDungeonBoss25"),str_map)
		messageWait()
	end
	
	goToNextFloor()
end


function eventBossEnd(object)
	boss = getCharacterStorage("boss")
	tewiEndEvent(boss)
end

function startSpell(index)
	outputDebugString("startSpell_start\n")
	
end


function floorInitProcess()
	--outputDebugString("floorInitProcess_start\n")

    tewiStartEvent()

	if floorLevel() == 15 then

		tewiStartEvent()
	end
	
	if storyEventEnable() then
		if floorLevel() == 8 then
			clearMassage()
			str_map = map_tstring_StyleString()
			utility_storyMessage(_T("redDungeonIntoToshokan"),str_map)
			messageWait()
		end
		if floorLevel() == 5 then
			clearMassage()
			str_map = map_tstring_StyleString()
			utility_storyMessage(_T("redDungeonIntoKoumakan"),str_map)
			messageWait()
		end
		if floorLevel() == 4 then
			uwasaEvent()
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