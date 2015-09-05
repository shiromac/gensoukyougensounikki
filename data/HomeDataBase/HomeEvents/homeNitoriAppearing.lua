function homeEvent()
    if(globalPlayerLavel() >= 5)then
        
        if( getMapValue(savable_globalFlags(),_T("EventDidFlag:homeNitoriAppearing"), NO ) == NO )then
            
            stackDramaThread(coroutine.create(function()

            if(PlayerCharacter().placeX==19)then

        	--cameraMove(PlayerCharacter().placeX, PlayerCharacter().placeY, 1.5,0.75, 0.9)
            cameraMoveParallel(12,14,0.6,0,1)
            moveWithAnime(PlayerCharacter(), ASPECT_LEFT, 1, false, 0.8, false)
            coroutine.yield(false)
            moveWithAnime(PlayerCharacter(), ASPECT_UPLEFT, 2, false, 0.8, false)
            coroutine.yield(false)
            moveWithAnime(PlayerCharacter(), ASPECT_LEFT, 4, false, 0.8, false)			
            coroutine.yield(false)
            end

            setMapValue(savable_globalFlags(),_T("EventDidFlag:homeNitoriAppearing"), YES)

            effect_Smoke1(8,16);
            Nitori = MakeCharacter(4005, 0, CHARACTER_FORSE_OTHER, LandformPlace(8,16))
                
            aspectcoo = Coordinate(Nitori.placeX - PlayerCharacter().placeX, Nitori.placeY - PlayerCharacter().placeY)
            turnCharacterForCharacter(PlayerCharacter(),Nitori)
            turnCharacterForCharacter(Nitori,PlayerCharacter())            
--            CharacterTurn(PlayerCharacter(), aspectcoo:getAspect())
--            CharacterTurn(Nitori, aspectcoo:getAspect()+4)
            
            cameraMoveParallel(Nitori.placeX, Nitori.placeY, 1,0.25,0.75)
                    
            str_map = map_tstring_StyleString()
            clearMassage()
            
		    storyEventMessageLoop(_T("homeNitoriAppearEvent_"), str_map, nil)
            clearMassage()
            
            cameraMove(PlayerCharacter().placeX, PlayerCharacter().placeY, 1.5,0.75, 0.9)
            
            return true;
            end ))
            
        end
    end
end
