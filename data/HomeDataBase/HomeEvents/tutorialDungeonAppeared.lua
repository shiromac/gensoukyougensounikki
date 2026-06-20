function tutorialMessageLeadWait()
    effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY, 20)
end

function tutorialDungeonAppeared()
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
					
	local str_map = map_tstring_StyleString()
	for i = 1, 6 do
		    tutorialMessageLeadWait()
		    utility_storyMessage(_T("tutorialDungeonBeforeEvent_"..i),str_map)
	end
	tutorialMessageLeadWait()
	clearMassage()
	cameraMove(PlayerCharacter().placeX, PlayerCharacter().placeY, 1.5,0.75, 0.9)
			
	appearDungeon(_T("tutorial_Dungeon"))
    
    return true
end

function homeEvent()
	if( isDungeonCleared(_T("intro_Dungeon")) )then
		if(not isAppearedDungeon(_T("tutorial_Dungeon")) ) then

        stackDramaThread(coroutine.create(tutorialDungeonAppeared))
			
		end
	end
end
