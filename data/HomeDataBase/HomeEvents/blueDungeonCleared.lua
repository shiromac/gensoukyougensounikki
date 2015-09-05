
--[[function storyEventMessage(massageKeyPrefix, stringkeyMap)
    local index = 1
    while
        local massageKey = _T(massageKeyPrefix..index)
        if isExistStoryMessage(massageKey) then
            break
        end
		utility_storyMessage(massageKey,stringkeyMap)
		messageWait();
        index = index + 1
    end
end]]


--[[
function homeEvent()
	if( isDungeonCleared(_T("blue_Dungeon")) )then
		--if (getMapValue(savable_globalFlags(),_T("EventDidFlag:blueDungeonCleard"),NO)) == NO then
			--setMapValue(savable_globalFlags(),_T("EventDidFlag:blueDungeonCleard"), YES)
			
			keibimeido = nil
			vec_chara = allCharacterList()
			charalistsize = vec_chara: size()
			for i = 0, charalistsize-1 do
				if(vec_chara:at(i):ID() == 4000) then
					--keibi meido
					keibimeido = vec_chara:at(i)
				end
			end
			
			turnCharacterForCharacter(keibimeido,PlayerCharacter())
			turnCharacterForCharacter(PlayerCharacter(),keibimeido)
			
			--generate_tewi
			--tewi = MakeCharacter(2048, 0, CHARACTER_FORSE_FRIEND, LandformPlace(PlayerCharacter().placeX,PlayerCharacter().placeY-2))
			
			--cameraMoveParallel(keibimeido.placeX, keibimeido.placeY, 1,0.25,0.75)
			
			clearMassage()
			str_map = map_tstring_StyleString()
			storyEventMessageLoop(_T("blueDungeonCloseEvent_"), str_map, nil)
			clearMassage()
			
			--cameraMove(PlayerCharacter().placeX, PlayerCharacter().placeY, 1.5,0.75, 0.9)
--end
end
end	
]]	
