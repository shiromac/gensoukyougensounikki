function floorInitProcess()
	--outputDebugString("floorInitProcess_start\n")

	if floorLevel() == 1 then
		--outputDebugString("point 1\n")
		emeny = MakeCharactor(2000, 1, CHARACTER_FORSE_ENEMY, LandformPlace(25,27))
		CharacterCureMental(emeny)
		CharacterSwingAir(emeny, TURN_FOREVER)
		CharacterInIce(emeny, TURN_FOREVER)
		
		--outputDebugString("point 2\n")
		emeny = MakeCharactor(2000, 1, CHARACTER_FORSE_ENEMY, LandformPlace(26,22))
		CharacterCureMental(emeny)
		CharacterSwingAir(emeny, TURN_FOREVER)
		CharacterInIce(emeny, TURN_FOREVER)
		
		--outputDebugString("point 3\n")
		emeny = MakeCharactor(2000, 1, CHARACTER_FORSE_ENEMY, LandformPlace(21,11))
		CharacterCureMental(emeny)
		CharacterSwingAir(emeny, TURN_FOREVER)
		CharacterInIce(emeny, TURN_FOREVER)
		CharacterPoor(emeny, TURN_FOREVER)

		--outputDebugString("point 4\n")
	end
	--outputDebugString("point 5\n")

	return

end

function dungeonProcess()
	--outputDebugString("dungeonProcess_start\n")
end

function dungeonTurnProcess()
	--outputDebugString("dungeonTurnProcess_start\n")

	if savable_freeFlags(): empty() == 0 then
		outputDebugString("savable_freeFlags(): resize(40,0)")
		savable_freeFlags(): resize(40,NO)
	end

	--outputDebugString("1")
	save_frag_index = 1
	if savable_freeFlags(): at(save_frag_index) == NO then
		savable_freeFlags(): setAt(save_frag_index,YES);
		str_map = map_tstring_StyleString()
		ValueString = setStyle( buttonIndex(buttonIndex_ATTACK), _T("%.0f") ,StyleString_DEFAULT_COLOR,1,1,0)
		str_map: insert(pair_tstring_StyleString(_T("Button"),ValueString))
		utility_storyMessage(_T("tutorialStart"),str_map)
		messageWait()
	end

    --Attack Tutorial
    save_frag_index = 2
	if savable_freeFlags(): at(save_frag_index) == NO then
		if landformInFrontOfCharacter(PlayerCharacter(),1).mountedCharacter then
			savable_freeFlags(): setAt(save_frag_index,YES);
			str_map = map_tstring_StyleString()
			ValueString = setStyle( buttonIndex(buttonIndex_ATTACK), _T("%.0f") ,StyleString_DEFAULT_COLOR,1,1,0)
			str_map: insert(pair_tstring_StyleString(_T("Button"),ValueString))
			clearMassage()
			utility_storyMessage(_T("tutorialAttack"),str_map)
			messageWait()
		end
	end

    --Turn Tutorial
    save_frag_index = 3
	if savable_freeFlags(): at(save_frag_index) == NO then
	    frag = NO
	    for a = 1, 7 do
	        canAttack = isCharacterCanNeighberAttack(PlayerCharacter(), PlayerCharacter().aspect + a, 1, NO, NO) == true
	        if landformAroundOfCharacter(PlayerCharacter(),a,1).mountedCharacter and canAttack then
	            frag = YES
	            break
	        end
        end
		if frag == YES then
			savable_freeFlags(): setAt(save_frag_index,YES);
			str_map = map_tstring_StyleString()
			ValueString = setStyle( buttonIndex(buttonIndex_TURN), _T("%.0f") ,StyleString_DEFAULT_COLOR,1,1,0)
			str_map: insert(pair_tstring_StyleString(_T("Button"),ValueString))
			clearMassage()
			utility_storyMessage(_T("tutorialTurn"),str_map)
			messageWait()
		end
	end
	
	--Dash Tutorial
    save_frag_index = 4
	if savable_freeFlags(): at(save_frag_index) == NO then
		if totalTurnCount() >= 100 then
			savable_freeFlags(): setAt(save_frag_index,YES);
			str_map = map_tstring_StyleString()
			ValueString = setStyle( buttonIndex(buttonIndex_DASH), _T("%.0f") ,StyleString_DEFAULT_COLOR,1,1,0)
			str_map: insert(pair_tstring_StyleString(_T("Button"),ValueString))
			clearMassage()
			utility_storyMessage(_T("tutorialDash"),str_map)
			messageWait()
		end
	end
	
	--SmartDash Tutorial
    save_frag_index = 5
	if savable_freeFlags(): at(save_frag_index) == NO then
		if totalTurnCount() >= 200 then
			savable_freeFlags(): setAt(save_frag_index,YES);
			str_map = map_tstring_StyleString()
			ValueString = setStyle( buttonIndex(buttonIndex_SDASH), _T("%.0f") ,StyleString_DEFAULT_COLOR,1,1,0)
			str_map: insert(pair_tstring_StyleString(_T("Button"),ValueString))
			clearMassage()
			utility_storyMessage(_T("tutorialSmartDash"),str_map)
			messageWait()
		end
	end
	
	--LastSpell Tutorial
    save_frag_index = 6
	if savable_freeFlags(): at(save_frag_index) == NO then
		if totalTurnCount() >= 1200 then
			savable_freeFlags(): setAt(save_frag_index,YES);
			str_map = map_tstring_StyleString()
			clearMassage()
			utility_storyMessage(_T("tutorialLastSpell"),str_map)
			messageWait()
		end
	end
	
	--Item Tutorial
    save_frag_index = 7
	if savable_freeFlags(): at(save_frag_index) == NO then
	    frag = NO
	    for a = 0, 7 do
	        droping = landformAroundOfCharacter(PlayerCharacter(),a,1).mountedDroping
	        if droping then
	            if droping.attribute: count(DROPING_ATTRIBUTE_ITEM) > 0 then
	                frag = YES
	                break
	            end
	        end
        end
		if frag == YES then
			savable_freeFlags(): setAt(save_frag_index,YES);
			str_map = map_tstring_StyleString()
			ValueString = setStyle( buttonIndex(buttonIndex_DASH), _T("%.0f") ,StyleString_DEFAULT_COLOR,1,1,0)
			str_map: insert(pair_tstring_StyleString(_T("Button"),ValueString))
			clearMassage()
			utility_storyMessage(_T("tutorialItemDiscover"),str_map)
			messageWait()
		end
	end
	
	--ItemGet Tutorial
    save_frag_index = 8
	if savable_freeFlags(): at(save_frag_index) == NO then
		if PlayerCharacter().holdItem: size() > 1 then
			savable_freeFlags(): setAt(save_frag_index,YES);
			str_map = map_tstring_StyleString()
			ValueString = setStyle( buttonIndex(buttonIndex_MENU), _T("%.0f") ,StyleString_DEFAULT_COLOR,1,1,0)
			str_map: insert(pair_tstring_StyleString(_T("Button"),ValueString))
			clearMassage()
			utility_storyMessage(_T("tutorialItemGet"),str_map)
			messageWait()
		end
	end
	
	--Trap Tutorial
    save_frag_index = 9
	if savable_freeFlags(): at(save_frag_index) == NO then
	    trap = landformAroundOfCharacter(PlayerCharacter(),0,0).mountedDroping 
		if trap and trap.attribute: count(DROPING_ATTRIBUTE_TRAP) > 0 then
			savable_freeFlags(): setAt(save_frag_index,YES);
			str_map = map_tstring_StyleString()
			clearMassage()
			utility_storyMessage(_T("tutorialOnTrap"),str_map)
			messageWait()
		end
	end
end

