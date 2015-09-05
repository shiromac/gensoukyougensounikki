function homeEvent()
    if(yakuPoint() > 1000)then
        
        if( getMapValue(savable_globalFlags(),_T("EventDidFlag:homeHinaAppearing"), NO ) == NO )then
        
        
            stackDramaThread(coroutine.create(function()
                
                setMapValue(savable_globalFlags(),_T("EventDidFlag:homeHinaAppearing"), YES)

                effect_Smoke1(13,10);
                Hina = MakeCharacter(4001, 0, CHARACTER_FORSE_OTHER, LandformPlace(13,10))
                    
                aspectcoo = Coordinate(Hina.placeX - PlayerCharacter().placeX, Hina.placeY - PlayerCharacter().placeY)
                CharacterTurn(PlayerCharacter(), aspectcoo:getAspect())
                CharacterTurn(Hina, aspectcoo:getAspect()+4)
                
                cameraMoveParallel(Hina.placeX, Hina.placeY, 1,0.25,0.75)
                        
                str_map = map_tstring_StyleString()
                clearMassage()
                storyEventMessageLoop(_T("homeHinaAppearEvent_"), str_map, nil)
                clearMassage()
                
                cameraMove(PlayerCharacter().placeX, PlayerCharacter().placeY, 1.5,0.75, 0.9)
                
                return true;
            end ))
            
        end
    end
end
