g_droping_map = {}

function tutorialMessageLeadWait()
    effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY, 20)
end

function floorInitProcess()
	--outputDebugString("floorInitProcess_start\n")

	if floorLevel() == 1 then
		MakeDroping(10000, LandformPlace(36,26))

		if storyEventEnable() then
			clearMassage()
			str_map = map_tstring_StyleString()
			tutorialMessageLeadWait()
			utility_storyMessage(_T("tutorialDungeonIntro"),str_map)
		end
		
		--Trap
		MakeDroping(9009, LandformPlace(18,0))
		MakeDroping(9009, LandformPlace(20,0))
		MakeDroping(9009, LandformPlace(21,0))
		MakeDroping(9009, LandformPlace(19,1))
		MakeDroping(9009, LandformPlace(22,1))
		g_droping_map["trap"] = MakeDroping(9009, LandformPlace(19,2))
		MakeDroping(9009, LandformPlace(21,2))
		MakeDroping(9009, LandformPlace(20,3))
		MakeDroping(9009, LandformPlace(21,3))
		MakeDroping(9009, LandformPlace(18,4))
		MakeDroping(9009, LandformPlace(22,4))
	end

	return

end

function dungeonProcess()
	--outputDebugString("dungeonProcess_start\n")
end

function dungeonTurnProcess()
	--outputDebugString("dungeonTurnProcess_start\n")
	player = PlayerCharacter()
	placeX = player.placeX;
	placeY = player.placeY;

	if (getMapValue(savable_localFlags(),_T("tutorialDungeonDush"),NO) == NO) and placeX == 2 and placeY == 34 then
		setMapValue(savable_localFlags(),_T("tutorialDungeonDush"), YES)

		if storyEventEnable() then
			clearMassage()
			str_map = map_tstring_StyleString()
			ValueString = setStyle( buttonIndex(buttonIndex_DASH), _T("%.0f") ,StyleString_DEFAULT_COLOR,1,1,0)
			str_map: insert(pair_tstring_StyleString(_T("Button"),ValueString))
			tutorialMessageLeadWait()
			utility_storyMessage(_T("tutorialDungeonDush"),str_map)
		end
	end

	if (getMapValue(savable_localFlags(),_T("tutorialDungeonTurn"),NO) == NO) and placeX == 2 and placeY == 9 then
		setMapValue(savable_localFlags(),_T("tutorialDungeonTurn"), YES)

		MakeCharacter(2001, 0, CHARACTER_FORSE_ENEMY, LandformPlace(1,9))
		effect_Smoke1(1,9)

		if storyEventEnable() then
			clearMassage()
			str_map = map_tstring_StyleString()
			ValueString = setStyle( buttonIndex(buttonIndex_TURN), _T("%.0f") ,StyleString_DEFAULT_COLOR,1,1,0)
			str_map: insert(pair_tstring_StyleString(_T("Button"),ValueString))
			tutorialMessageLeadWait()
			utility_storyMessage(_T("tutorialDungeonTurn"),str_map)
		end
	end
	
	if (getMapValue(savable_localFlags(),_T("tutorialDungeonDiagon"),NO) == NO) and placeX == 7 and placeY == 8 then
		setMapValue(savable_localFlags(),_T("tutorialDungeonDiagon"), YES)

		if storyEventEnable() then
			clearMassage()
			str_map = map_tstring_StyleString()
			ValueString = setStyle( buttonIndex(buttonIndex_DIAGON), _T("%.0f") ,StyleString_DEFAULT_COLOR,1,1,0)
			str_map: insert(pair_tstring_StyleString(_T("Button"),ValueString))
			tutorialMessageLeadWait()
			utility_storyMessage(_T("tutorialDungeonDiagon"),str_map)
		end
	end
	
	if (getMapValue(savable_localFlags(),_T("tutorialDungeonOnTrap"),NO) == NO) and placeX == 18 and placeY == 2 then
		setMapValue(savable_localFlags(),_T("tutorialDungeonOnTrap"), YES)

		g_droping_map["trap"]: discover()

		if storyEventEnable() then
			clearMassage()
			str_map = map_tstring_StyleString()
			tutorialMessageLeadWait()
			utility_storyMessage(_T("tutorialDungeonOnTrap1"),str_map)
			utility_storyMessage(_T("tutorialDungeonOnTrap2"),str_map)
		end
	end
	
	if (getMapValue(savable_localFlags(),_T("tutorialDungeonSmartDash"),NO) == NO) and placeX == 25 and placeY == 2 then
		setMapValue(savable_localFlags(),_T("tutorialDungeonSmartDash"), YES)


		if storyEventEnable() then
			clearMassage()
			str_map = map_tstring_StyleString()
			tutorialMessageLeadWait()
			utility_storyMessage(_T("tutorialDungeonSmartDash1"),str_map)
			ValueString = setStyle( buttonIndex(buttonIndex_SDASH), _T("%.0f") ,StyleString_DEFAULT_COLOR,1,1,0)
			str_map: insert(pair_tstring_StyleString(_T("Button"),ValueString))
			utility_storyMessage(_T("tutorialDungeonSmartDash2"),str_map)
		end
	end
	
	if (getMapValue(savable_localFlags(),_T("tutorialDungeonMessageLog"),NO) == NO) and placeX == 37 and placeY == 12 then
		setMapValue(savable_localFlags(),_T("tutorialDungeonMessageLog"), YES)


		if storyEventEnable() then
			clearMassage()
			str_map = map_tstring_StyleString()
			tutorialMessageLeadWait()
			utility_storyMessage(_T("tutorialDungeonMessageLog"),str_map)
		end
	end
	
	if (getMapValue(savable_localFlags(),_T("tutorialDungeonMap"),NO) == NO) and placeX == 24 and placeY == 16 then
		setMapValue(savable_localFlags(),_T("tutorialDungeonMap"), YES)


		if storyEventEnable() then
			clearMassage()
			str_map = map_tstring_StyleString()
			tutorialMessageLeadWait()
			utility_storyMessage(_T("tutorialDungeonMap1"),str_map)
			ValueString = setStyle( buttonIndex(buttonIndex_MAP), _T("%.0f") ,StyleString_DEFAULT_COLOR,1,1,0)
			str_map: insert(pair_tstring_StyleString(_T("Button"),ValueString))
			utility_storyMessage(_T("tutorialDungeonMap2"),str_map)
		end
	end
	
	
	if (getMapValue(savable_localFlags(),_T("tutorialDungeonStep"),NO) == NO) and placeX == 36 and placeY == 18 then
		setMapValue(savable_localFlags(),_T("tutorialDungeonStep"), YES)


		if storyEventEnable() then
			clearMassage()
			str_map = map_tstring_StyleString()
			tutorialMessageLeadWait()
			utility_storyMessage(_T("tutorialDungeonStep1"),str_map)
			utility_storyMessage(_T("tutorialDungeonStep2"),str_map)
		end
	end
	
	if (getMapValue(savable_localFlags(),_T("tutorialDungeonStair"),NO) == NO) and placeX == 36 and placeY == 24 then
		setMapValue(savable_localFlags(),_T("tutorialDungeonStair"), YES)


		if storyEventEnable() then
			clearMassage()
			str_map = map_tstring_StyleString()
			
			tutorialMessageLeadWait()
			utility_storyMessage(_T("tutorialDungeonStair"),str_map)
		end
	end
end

