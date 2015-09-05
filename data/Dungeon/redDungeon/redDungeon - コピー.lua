

enemyPool = {}
landPool = {}
function remiliaStartEvent()

		
	bossLand = LandformPlace(20,15);
	emeny = MakeCharacter(5000, 0, CHARACTER_FORSE_ENEMY, bossLand)
	enemyPool["boss"] = emeny
		
	CharacterTurnNow(emeny, ASPECT_UP)
	

	if storyEventEnable() then
	
		PlayBGM(_T(""),_T("talk1_main.wav"))
		
		bossplace = bossLand.place
		
		cameraMoveParallel(bossplace.x, bossplace.y, 2,0.25,0.75)
		
		clearMassage()
		str_map = map_tstring_StyleString()
		utility_storyMessage(_T("redDungeonBoss1"),str_map)
		messageWait()
		
		CharacterTurnNow(PlayerCharacter(), ASPECT_UP)
		
		plplace = Coordinate(PlayerCharacter().placeX, PlayerCharacter().placeY)
		
		--cameraMoveParallel(plplace.x,plplace.y, 2,0.25,0.75)
		utility_storyMessage(_T("redDungeonBoss2"),str_map)
		messageWait()
		
		--cameraMoveParallel(bossplace.x, bossplace.y ,2,0.25,0.75)
		utility_storyMessage(_T("redDungeonBoss3"),str_map)
		messageWait()
		
		--cameraMoveParallel(plplace.x,plplace.y, 2,0.25,0.75)
		utility_storyMessage(_T("redDungeonBoss4"),str_map)
		messageWait()
		
		--cameraMoveParallel(bossplace.x, bossplace.y ,2,0.25,0.75)
		utility_storyMessage(_T("redDungeonBoss5"),str_map)
		messageWait()
		
		--cameraMoveParallel(plplace.x,plplace.y, 2,0.25,0.75)
		utility_storyMessage(_T("redDungeonBoss6"),str_map)
		messageWait()
	end
	
	CharacterTurn(emeny, ASPECT_DOWN)
	
	if storyEventEnable() then
		--cameraMoveParallel(bossplace.x, bossplace.y ,2,0.25,0.75)
		utility_storyMessage(_T("redDungeonBoss7"),str_map)
		messageWait()
	end
	
	PlayBGM(_T("boss_intro.wav"),_T("boss_main.wav"))
	
	if storyEventEnable() then
		cameraMoveParallel(plplace.x,plplace.y, 2,0.75,1)
		utility_storyMessage(_T("redDungeonBoss8"),str_map)
		messageWait()
	end

	--damege to start
	stackFunction(function() CharacterForsedDamage(enemyPool["boss"],1) end)
end

function eventBossEnd(object)
	boss = getCharacterStorage("boss")
	remiliaEndEvent(boss)
end

function remiliaEndEvent(boss)

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

		outputDebugString("before bossLand = LandformCharacterPosition(boss)\n")
		bossLand = LandformCharacterPosition(boss)
		outputDebugString("after bossLand = LandformCharacterPosition(boss)\n")
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
end

function startSpell(index)
	outputDebugString("startSpell_start\n")
	if(index == 1)then
	--[[
		pcLandform pland = sg_pDungeonSystem->Map().Land(MAPWIDTH/2,MAPHEIGHT/2);
		
		EffectFunctions::煙エフェクト1(boss()->足元地形()->placeX, boss()->足元地形()->placeY);
		sg_pDungeonSystem->ワープ要請(boss(),pland);
		EffectFunctions::煙エフェクト1(pland->placeX,pland->placeY);

		for(i=0;i<4;i++)
		{
			sg_pDungeonSystem->キャラクター生成(5001, 0, CHARACTER_FORSE_ENEMY, 
				sg_pDungeonSystem->隣接地形(pland, ASPECT_UP+i*2, 1));
		}
		]]

		effect_Smoke1(boss.placeX, boss.placeY)
		stackFunction(function() CharacterWarp(boss, LandformPlace(20,20)) end)
		effect_Smoke1(boss.placeX, boss.placeY)
	
		if storyEventEnable() then
			cameraMove(20, 20, 2,0.25,0.75)
		end
		
	
		chara = MakeCharacter(5001, 0, CHARACTER_FORSE_ENEMY, LandformPlace(21,20))
		enemyPool["fran1"] = chara
		effect_Smoke1(21,20)
		
		if storyEventEnable() then
			cameraMoveParallel(21,20, 1,0.25,0.75)

			clearMassage()
			str_map = map_tstring_StyleString()
			utility_storyMessage(_T("redDungeonBossEvent1"),str_map)
			messageWait()
		end
		
		
		chara = MakeCharacter(5001, 0, CHARACTER_FORSE_ENEMY, LandformPlace(19,20))
		enemyPool["fran2"] = chara
		effect_Smoke1(19,20)
			
		
		chara = MakeCharacter(5001, 0, CHARACTER_FORSE_ENEMY, LandformPlace(20,21))
		enemyPool["fran3"] = chara
		effect_Smoke1(20,21)
			

		chara = MakeCharacter(5001, 0, CHARACTER_FORSE_ENEMY, LandformPlace(20,19))
		enemyPool["fran4"] = chara
		effect_Smoke1(20,19)
		
		if storyEventEnable() then
			cameraMove(20, 20, 2,0.75,0.1)

			clearMassage()
			str_map = map_tstring_StyleString()
			utility_storyMessage(_T("redDungeonBossEvent2"),str_map)
			messageWait()

			cameraMove(PlayerCharacter().placeX, PlayerCharacter().placeY, 2,0.75,1)

			clearMassage()
			str_map = map_tstring_StyleString()
			utility_storyMessage(_T("redDungeonBossEvent3"),str_map)
			messageWait()

		end
		
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
		
	end
end

function pickedUpSpellEvent()

	if storyEventEnable() then
		str_map = map_tstring_StyleString()
		utility_storyMessage(_T("redDungeonSpell1"),str_map)
		messageWait()
		
		utility_storyMessage(_T("redDungeonSpell2"),str_map)
		messageWait()
	end
end

function floorInitProcess()
	--outputDebugString("floorInitProcess_start\n")

	if floorLevel() == 10 then

		remiliaStartEvent()
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
		if floorLevel() == 1 then
			clearMassage()
			str_map = map_tstring_StyleString()
			utility_storyMessage(_T("redDungeonStart"),str_map)
			messageWait()
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