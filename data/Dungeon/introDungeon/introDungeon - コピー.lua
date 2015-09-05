
state_attackTutorial = 1
state_itemTutorial = 2
state_itemTutorial2 = 3
state_itemTutorial3 = 4
state_introEndEvent = 5

enemyPool = {}

function introSelfIntroEvent()
	clearMassage()
	str_map = map_tstring_StyleString()
	ValueString = setStyle( buttonIndex(buttonIndex_DECIDE), _T("%.0f") ,StyleString_DEFAULT_COLOR,1,1,0)
	str_map: insert(pair_tstring_StyleString(_T("Button"),ValueString))
	utility_storyMessage(_T("introSelfIntro1"),str_map)
	messageWait()

	str_map: clear()
	utility_storyMessage(_T("introSelfIntro2"),str_map)
	messageWait()

	cameraMove(20,17,1,1,1)
	function tempfunction()
		emeny = MakeCharactor(2001, 0, CHARACTER_FORSE_ENEMY, LandformPlace(20,17))
		enemyPool["first"] = emeny
		CharacterCureMental(emeny)
		CharacterSwingAir(emeny, TURN_FOREVER)
	end
	stackFunction(tempfunction)

	effect_Smoke1(20,17)

	utility_storyMessage(_T("introSelfIntro3"),str_map)
	messageWait()

	cameraMoveParallel(20,20,1,0.25,0.75)
	ValueString = setStyle( buttonIndex(buttonIndex_ATTACK), _T("%.0f") ,StyleString_DEFAULT_COLOR,1,1,0)
	str_map: insert(pair_tstring_StyleString(_T("Button"),ValueString))
	utility_storyMessage(_T("introSelfIntro4"),str_map)
	messageWait()
end

function introItemUseEvent()
	clearMassage()

	cameraMove(20,17,1,1,1)
	function tempfunction1()
		emeny = MakeCharactor(2001, 0, CHARACTER_FORSE_ENEMY, LandformPlace(20,17))
		enemyPool["second"] = emeny
		CharacterCureMental(emeny)
	end
	stackFunction(tempfunction1)
	effect_Smoke1(20,17)

	cameraMove(17,20,1.5,1,1)
	function tempfunction2()
		emeny = MakeCharactor(2001, 0, CHARACTER_FORSE_ENEMY, LandformPlace(17,20))
		enemyPool["third"] = emeny
		CharacterCureMental(emeny)
	end
	stackFunction(tempfunction2)
	effect_Smoke1(17,20)

	cameraMove(23,20,1.5,1,1)
	function tempfunction3()
		emeny = MakeCharactor(2001, 0, CHARACTER_FORSE_ENEMY, LandformPlace(23,20))
		enemyPool["fourth"] = emeny
		CharacterCureMental(emeny)
	end
	stackFunction(tempfunction3)
	effect_Smoke1(23,20)

	cameraMove(20,20,1.5,1,1)

	str_map = map_tstring_StyleString()
	utility_storyMessage(_T("introItemUse1"),str_map)
	messageWait()

	cameraMove(PlayerCharacter():placeX(),PlayerCharacter():placeY(),2,1,1)

	str_map: clear()
	utility_storyMessage(_T("introItemUse2"),str_map)
	messageWait()

	cameraMove(PlayerCharacter():placeX(),PlayerCharacter():placeY()+1,2,1,1)
	function tempfunction4()
		MakeDroping(5029, LandformPlace(PlayerCharacter():placeX(),PlayerCharacter():placeY()+1))
	end
	stackFunction(tempfunction4)
	effect_Smoke1(PlayerCharacter():placeX(),PlayerCharacter():placeY()+1)

	str_map: clear()
	utility_storyMessage(_T("introItemUse3"),str_map)
	messageWait()

	cameraMove(PlayerCharacter():placeX(),PlayerCharacter():placeY(),1,0.25,0.75)

end

function introItemUseEvent2()
	clearMassage()
	str_map: clear()
	ValueString = setStyle( buttonIndex(buttonIndex_MENU), _T("%.0f") ,StyleString_DEFAULT_COLOR,1,1,0)
	str_map: insert(pair_tstring_StyleString(_T("Button"),ValueString))
	utility_storyMessage(_T("introItemUse4"),str_map)
	messageWait()
end

function introEndEvent()
	clearMassage()
	str_map = map_tstring_StyleString()
	utility_storyMessage(_T("introEndEvent1"),str_map)
	messageWait()

	utility_storyMessage(_T("introEndEvent2"),str_map)
	messageWait()

	utility_storyMessage(_T("introEndEvent3"),str_map)
	messageWait()

	utility_storyMessage(_T("introEndEvent4"),str_map)
	messageWait()

	utility_storyMessage(_T("introEndEvent5"),str_map)
	messageWait()

	utility_storyMessage(_T("introEndEvent6"),str_map)
	messageWait()
end

function floorInitProcess()
	--outputDebugString("floorInitProcess_start\n")

	if floorLevel() == 1 then

		introSelfIntroEvent()
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
			introItemUseEvent()
			setMapValue(savable_localFlags(),_T("state"),state_itemTutorial)
			return
		end

		if (getMapValue(savable_localFlags(),_T("attackTutorial_reTutorial"),0) >= 8) then
			clearMassage()
			str_map = map_tstring_StyleString()
			ValueString = setStyle( buttonIndex(buttonIndex_DECIDE), _T("%.0f") ,StyleString_DEFAULT_COLOR,1,1,0)
			str_map: insert(pair_tstring_StyleString(_T("Button"),ValueString))
			utility_storyMessage(_T("introAttackSupport"),str_map)
			messageWait()
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
			introEndEvent()
			setMapValue(savable_localFlags(),_T("state"),state_introEndEvent)
			return
		end

		if (getMapValue(savable_localFlags(),_T("itemTutorial_reTutorial"),0) >= 3) then
			clearMassage()
			str_map = map_tstring_StyleString()
			ValueString = setStyle( buttonIndex(buttonIndex_MENU), _T("%.0f") ,StyleString_DEFAULT_COLOR,1,1,0)
			str_map: insert(pair_tstring_StyleString(_T("Button"),ValueString))
			utility_storyMessage(_T("introItemSupport"),str_map)
			messageWait()
			setMapValue(savable_localFlags(),_T("itemTutorial_reTutorial"), 0)
		end
		setMapValue(savable_localFlags(),_T("itemTutorial_reTutorial"), getMapValue(savable_localFlags(),_T("itemTutorial_reTutorial"),0) +1 )

	elseif (getMapValue(savable_localFlags(),_T("state"),NO) == state_itemTutorial3) then
	
		if (enemyPool["second"]:isDead() and enemyPool["third"]:isDead() and enemyPool["fourth"]:isDead()) then
			--nextState
			introEndEvent()
			setMapValue(savable_localFlags(),_T("state"),state_introEndEvent)
		end
	end

	if (getMapValue(savable_localFlags(),_T("state"),NO) == state_introEndEvent) then

		vDroping = PlayerCharacter().holdItem
		size = vDroping: size()
		for i = 0, size-1 do
			DropingBreak(vDroping: at(i))
		end
		goToNextFloor()
		return
	end

	if (PlayerCharacter().HP < 10) then
		clearMassage()
		str_map = map_tstring_StyleString()
		utility_storyMessage(_T("introHPrecover"),str_map)
		messageWait()
		stackFunction(function() CharacterHPRecovery(PlayerCharacter(),15) end)
	end
end

