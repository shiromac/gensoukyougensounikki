function homeEvent()
    if(isDungeonCleared(_T("red_Dungeon")))then
        
        if( getMapValue(savable_globalFlags(),_T("EventDidFlag:homeAkyuAppearing"), NO ) == NO )then
            
            -- make first of all
            local akyu = MakeCharacter(4008, 0, CHARACTER_FORSE_OTHER, LandformPlace(18,13))
                
            stackDramaThread(coroutine.create(function()
            
                setMapValue(savable_globalFlags(),_T("EventDidFlag:homeAkyuAppearing"), YES)

                effect_Smoke1(18,13);
                    
                aspectcoo = Coordinate(akyu.placeX - PlayerCharacter().placeX, akyu.placeY - PlayerCharacter().placeY)
                CharacterTurn(PlayerCharacter(), aspectcoo:getAspect())
                CharacterTurn(akyu, aspectcoo:getAspect()+4)
                
                cameraMoveParallel(akyu.placeX, akyu.placeY, 1,0.25,0.75)
                        
                str_map = map_tstring_StyleString()
                clearMassage()
                
                storyEventMessageLoop(_T("homeAkyuAppearEvent_"), str_map, nil)
                clearMassage()
                
                cameraMove(PlayerCharacter().placeX, PlayerCharacter().placeY, 1.5,0.75, 0.9)
                
                return true;
            end ))

        end
    end
end
