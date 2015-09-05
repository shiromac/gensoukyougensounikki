function homeEvent()
	if( isDungeonCleared(_T("red_Dungeon")) )then
		if(not isAppearedDungeon(_T("blue_Dungeon")) ) then

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
			
			cameraMoveParallel(keibimeido.placeX, keibimeido.placeY, 1,0.25,0.75)
					
			clearMassage()
			str_map = map_tstring_StyleString()
			storyEventMessageLoop(_T("blueDungeonOpenEvent_"), str_map, nil)
			clearMassage()
			
			cameraMove(PlayerCharacter().placeX, PlayerCharacter().placeY, 1.5,0.75, 0.9)
			
			appearDungeon(_T("blue_Dungeon"))
		end
	end
end
