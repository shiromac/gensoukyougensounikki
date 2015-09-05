-- cCharacter boss: boss character

enemyPool = {}
landPool = {}


function giveUpEvent()
	addCharacterStorage("boss",boss)
	eventNotification("eventBossEnd","","boss")
	--[[
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
	if storyEventEnable() then

		
		bossLand = LandformCharacterPosition(boss)
		bossPlace = bossLand.place
		playerLand = LandformCharacterPosition(PlayerCharacter())
		playerPlace = playerLand.place
		aspectcoo = Coordinate(playerLand.place - bossLand.place)
		CharacterTurn(boss, aspectcoo:getAspect())
		CharacterTurn(PlayerCharacter(), aspectcoo:getAspect()+4)
		

		cameraMoveParallel(bossPlace.x, bossPlace.y, 2,0.25,0.75)
	
		clearMassage()
		str_map = map_tstring_StyleString()
		utility_storyMessage(_T("redDungeonBossEnd1"),str_map)
		messageWait()
		
		cameraMoveParallel(playerPlace.x, playerPlace.y, 2,0.25,0.75)
		utility_storyMessage(_T("redDungeonBossEnd2"),str_map)
		messageWait()
	
		cameraMoveParallel(bossPlace.x, bossPlace.y, 2,0.25,0.75)
		utility_storyMessage(_T("redDungeonBossEnd3"),str_map)
		messageWait()
			
		sakuyaLand = landformAroundOfCharacter(boss,3,1);
		for i = 0,7 do
			if(isSafeSetCharacterLandform(sakuyaLand))then
				break;
			end
			sakuyaLand = landformAroundOfCharacter(boss,3+i,1);
		end
		landPool["sakuya"] = sakuyaLand;
		sakuyaPlace = sakuyaLand.place
		emeny = MakeCharacter(4003, 0, CHARACTER_FORSE_FRIEND, landPool["sakuya"])
		effect_Smoke1(emeny.placeX, emeny.placeY)
		
		enemyPool["sakuya"] = emeny
		CharacterTurn(emeny, aspectcoo:getAspect())
		
		cameraMoveParallel(sakuyaPlace.x, sakuyaPlace.y, 2,0.25,0.75)
		utility_storyMessage(_T("redDungeonBossEnd4"),str_map)
		messageWait()
	
		cameraMoveParallel(bossPlace.x, bossPlace.y, 2,0.25,0.75)
		utility_storyMessage(_T("redDungeonBossEnd5"),str_map)
		messageWait()
		
		utility_storyMessage(_T("redDungeonBossEnd6"),str_map)
		messageWait()

		utility_storyMessage(_T("redDungeonBossEnd7"),str_map)
		messageWait()
		
		--remilia out
		CharacterForseGoAway(boss)
		effect_Smoke1(bossPlace.x, bossPlace.y)
		
		cameraMoveParallel(playerPlace.x, playerPlace.y, 2,0.25,0.75)
		utility_storyMessage(_T("redDungeonBossEnd8"),str_map)
		messageWait()
		
		sakuyaLand2 = landformAroundOfCharacter(PlayerCharacter(),0,2);
		for i = 0,7 do
			if(isSafeSetCharacterLandform(sakuyaLand2))then
				break;
			end
			sakuyaLand2 = landformAroundOfCharacter(PlayerCharacter(),0+i,2);
		end
		landPool["sakuya2"] = sakuyaLand2;
		effect_Smoke1(enemyPool["sakuya"].placeX, enemyPool["sakuya"].placeY)
		stackFunction(function() CharacterWarp(enemyPool["sakuya"], landPool["sakuya2"]) end)
		effect_Smoke1(enemyPool["sakuya"].placeX, enemyPool["sakuya"].placeY)
		sakuyaPlace2 = sakuyaLand.place
		aspectcoo2 = Coordinate(sakuyaLand2.place - playerLand.place)
		CharacterTurn(enemyPool["sakuya"], aspectcoo2:getAspect()+4)
		CharacterTurn(PlayerCharacter(), aspectcoo2:getAspect())
		
		utility_storyMessage(_T("redDungeonBossEnd9"),str_map)
		messageWait()
		utility_storyMessage(_T("redDungeonBossEnd10"),str_map)
		messageWait()
		utility_storyMessage(_T("redDungeonBossEnd11"),str_map)
		messageWait()
		utility_storyMessage(_T("redDungeonBossEnd12"),str_map)
		messageWait()
		utility_storyMessage(_T("redDungeonBossEnd13"),str_map)
		messageWait()
		utility_storyMessage(_T("redDungeonBossEnd14"),str_map)
		messageWait()
		utility_storyMessage(_T("redDungeonBossEnd15"),str_map)
		messageWait()
		
		ayaLand = landformAroundOfCharacter(PlayerCharacter(),3,8);
		for i = 0,7 do
			if(isSafeSetCharacterLandform(ayaLand))then
				break;
			end
			ayaLand = landformAroundOfCharacter(PlayerCharacter(),3+i,8);
		end
		landPool["aya"] = ayaLand;
		enemyPool["aya"] = MakeCharacter(4004, 0, CHARACTER_FORSE_FRIEND, landPool["aya"])
		aspectcoo3 = Coordinate(landPool["aya"].place - playerLand.place)
		CharacterTurn(enemyPool["aya"], aspectcoo3:getAspect()+4)
		ayaPlace = ayaLand.place
		cameraMoveParallel(ayaPlace.x, ayaPlace.y, 0.3,0.1,0.1)
		utility_storyMessage(_T("redDungeonBossEnd16"),str_map)
		messageWait()
		utility_storyMessage(_T("redDungeonBossEnd17"),str_map)
		messageWait()
		utility_storyMessage(_T("redDungeonBossEnd18"),str_map)
		messageWait()
	end
	
	goToNextFloor()
	]]
end

function startSpell(index)
	outputDebugString("startSpell_start\n")
	if(index == 1)then

	    addCharacterStorage("boss",boss)
	    eventNotification("eventFiveOfAKindBefore","","boss")
	end
end

function endSpell(index)

	if storyEventEnable() then

	end
end