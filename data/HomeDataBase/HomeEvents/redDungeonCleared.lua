function homeEvent()
	if( isDungeonCleared(_T("red_Dungeon")) )then
		if (getMapValue(savable_globalFlags(),_T("EventDidFlag:redDungeonCleard"),NO)) == NO then
			setMapValue(savable_globalFlags(),_T("EventDidFlag:redDungeonCleard"), YES)
			
			keibimeido = nil
			vec_chara = allCharacterList()
			charalistsize = vec_chara: size()
			for i = 0, charalistsize-1 do

				if(vec_chara:at(i):ID() == 4000) then
					--keibi meido
					keibimeido = vec_chara:at(i)

				end
			end
			
			aspectcoo = Coordinate(keibimeido.placeX - PlayerCharacter().placeX, keibimeido.placeY - PlayerCharacter().placeY)
			CharacterTurn(PlayerCharacter(), aspectcoo:getAspect())
			CharacterTurn(keibimeido, aspectcoo:getAspect()+4)
			
			cameraMoveParallel(keibimeido.placeX, keibimeido.placeY, 1,0.25,0.75)
			
			clearMassage()
			str_map = map_tstring_StyleString()
			storyEventMessageLoop(_T("redDungeonCloseEvent_"), str_map, nil)
			clearMassage()
			
			--cameraMove(PlayerCharacter().placeX, PlayerCharacter().placeY, 1.5,0.75, 0.9)
			
		end
	end
end
