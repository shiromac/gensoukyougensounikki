function homeEvent()
    if(PlayerCharacter().holdItem: size() > 0 or walletMoney() > 0)then
        
        if( getMapValue(savable_globalFlags(),_T("EventDidFlag:homeMarisaAppearing"), NO ) == NO )then
            
            stackDramaThread(coroutine.create(function()

                setMapValue(savable_globalFlags(),_T("EventDidFlag:homeMarisaAppearing"), YES)

                effect_Smoke1(11,15);
                marisa = MakeCharacter(4002, 0, CHARACTER_FORSE_OTHER, LandformPlace(11,15))
                    
                aspectcoo = Coordinate(marisa.placeX - PlayerCharacter().placeX, marisa.placeY - PlayerCharacter().placeY)
                CharacterTurn(PlayerCharacter(), aspectcoo:getAspect())
                CharacterTurn(marisa, aspectcoo:getAspect()+4)
                
                cameraMoveParallel(marisa.placeX, marisa.placeY, 1,0.25,0.75)
                        
                str_map = map_tstring_StyleString()
                item = MakeDropingAndPick(3029, PlayerCharacter())
                str_map: insert( pair_tstring_StyleString( _T("Item"), item: fullName() ))
                clearMassage()
                
                storyEventMessageLoop(_T("homeMarisaAppearEvent_"), str_map, nil)
                clearMassage()
                
                cameraMove(PlayerCharacter().placeX, PlayerCharacter().placeY, 1.5,0.75, 0.9)

                return true;
            end ))

        end
    end
end
