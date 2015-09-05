placeX = 12
placeY = 11
function homeYoumuAppearEvent()
            -- make first of all
    --local placeX = 12
    --local placeY = 11
    --local youmu = MakeCharacter(1004, 0, CHARACTER_FORSE_FRIEND, LandformPlace(placeX,placeY))
    str_map = map_tstring_StyleString()
    clearMassage()
                
    effect_Smoke1(placeX,placeY);
                               
    cameraMoveParallel(youmu.placeX, youmu.placeY, 1,0.25,0.75)
                        
    turnCharacterForCharacter(youmu,PlayerCharacter())
    turnCharacterForCharacter(PlayerCharacter(),youmu)
    storyEventMessageLoop(_T("homeYoumuAppearEvent_"), str_map, nil)
coroutine.yield(false)
    clearMassage()
coroutine.yield(false)
    storyEventMessageLoopRange(_T("homeYoumuExplain_"),1,1, str_map, nil)
coroutine.yield(false)
    clearMassage()
coroutine.yield(false)
    storyEventMessageLoopRange(_T("homeYoumuExplain_"),2,2, str_map, nil)
coroutine.yield(false)
    clearMassage()
coroutine.yield(false)
    storyEventMessageLoopRange(_T("homeYoumuExplain_"),3,4, str_map, nil)
coroutine.yield(false)
    clearMassage()
coroutine.yield(false)
    storyEventMessageLoopRange(_T("homeYoumuExplain_"),5,5, str_map, nil)
coroutine.yield(false)
    clearMassage()
               
    cameraMove(PlayerCharacter().placeX, PlayerCharacter().placeY, 1.5,0.75, 0.9)
                
    setMapValue(savable_globalFlags(),_T("CharacterPlayableFlag:Youmu"), YES)
    setMapValue(savable_globalFlags(),_T("EventDidFlag:homeYoumuAppearing"), YES)                

    return true

end

function homeEvent()
	if(isAppearedDungeon(_T("99F_Dungeon")) ) then
       if(PlayerCharacter(): ID() == 1002)then  
           if( getMapValue(savable_globalFlags(),_T("EventDidFlag:homeYoumuAppearing"), NO ) == NO )then
                youmu = MakeCharacter(1004, 0, CHARACTER_FORSE_FRIEND, LandformPlace(placeX,placeY))
                stackDramaThread(coroutine.create(homeYoumuAppearEvent))
           end
       end
	end
end