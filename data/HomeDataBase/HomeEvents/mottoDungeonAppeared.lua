function black_back(posX,posY,frame)

    local bg = EventBackground(0,_T("event\\black_back.png"), posX, posY, 2)

    coroutine.yield(false)
    cameraMove(posX,posY,10,1,1)
    bg:fadeIn(frame)
    coroutine.yield(false)

    return bg
end

function mottoDungeonAppeared()
            StopBGM()
            fadeBlack(1)
            setHiddenInterface(true);
            setHidingMiniMap(true)
            coroutine.yield(false)
            fairy = searchOneCharacterOnFloor(4000)
            hina = searchOneCharacterOnFloor(4001)
            marisa = searchOneCharacterOnFloor(4002)
            nitori = searchOneCharacterOnFloor(4005)
            akyu = searchOneCharacterOnFloor(4008)
            tewi = searchOneCharacterOnFloor(1003)
            
            str_map = map_tstring_StyleString()
            clearMassage()
           
            moveWithAnime(marisa, ASPECT_DOWN, 1, false, 1.0, true);
            coroutine.yield(false)

       	    appearDungeon(_T("99F_Dungeon"))
            coroutine.yield(false)
            --go_action-------------------------------------------------------------------------------
         
            --effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,30)

          
		    --storyEventMessageLoop(_T("mottoDungeonOpenEvent1_"), str_map, nil)
		    storyEventMessageLoopRange(_T("mottoDungeonOpenEvent1_"),1,3, str_map, nil)
            clearMessage()
            effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,5)
            coroutine.yield(false)
            
		    storyEventMessageLoopRange(_T("mottoDungeonOpenEvent1_"),4,4, str_map, nil)	
            clearMessage()
            coroutine.yield(false)
            
		    storyEventMessageLoopRange(_T("mottoDungeonOpenEvent1_"),5,8, str_map, nil)		    
            clearMessage()
            coroutine.yield(false)
		    storyEventMessageLoopRange(_T("mottoDungeonOpenEvent1_"),9,9, str_map, nil)		    
            clearMessage()
            effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,60)
            coroutine.yield(false)

            PlayBGM(_T(""),_T("talk1_main.wav"))  
            coroutine.yield(false)
            PlayBGM(_T(""),_T("base_main.wav"))
            fadeOut(60)
            coroutine.yield(false)
            ------------------------------------------------------------------------------------------
  


		    storyEventMessageLoop(_T("mottoDungeonOpenEvent2_"), str_map, nil)
		    --exciting_akyu--
		    moveWithAnime(akyu, ASPECT_DOWNLEFT, 1, false, 1.0, true);            
            coroutine.yield(false)
		    moveWithAnimeToLand(akyu,LandformPlace(PlayerCharacter().placeX+1, PlayerCharacter().placeY+1),false,1.0,false)
            coroutine.yield(false)	    
            turnCharacterForCharacter(PlayerCharacter(),akyu)
            turnCharacterForCharacter(akyu,PlayerCharacter())
            turnCharacterForCharacter(marisa,akyu)
            turnCharacterForCharacter(nitori,akyu)
            turnCharacterForCharacter(fairy,akyu)
            coroutine.yield(false)
            jumpWithAnime(akyu,ASPECT_UPLEFT, 0, 0.5, true, 1.0, true)
            coroutine.yield(false)
            jumpWithAnime(akyu,ASPECT_UPLEFT, 0, 0.5, true, 1.0, true)
            coroutine.yield(false)
            ------------------------------------------------------------------------------------------
		    
   		    storyEventMessageLoop(_T("mottoDungeonOpenEvent3_"), str_map, nil)
            coroutine.yield(false)            
            clearMassage()
       
            setHiddenInterface(false)
            setHidingMiniMap(false)     
            setMapValue(savable_globalFlags(),_T("EventDidFlag:mottoDungeonAppeared"), YES)
            coroutine.yield(false)            
    return true
end

function homeEvent()
    if(isDungeonCleared(_T("last_Dungeon")))then
        if( getMapValue(savable_globalFlags(),_T("EventDidFlag:homeAfterCredit"), YES ) == YES )then
            if( getMapValue(savable_globalFlags(),_T("EventDidFlag:mottoDungeonAppeared"), NO ) == NO )then
        
                stackDramaThread(coroutine.create(mottoDungeonAppeared))

            end
        end
    end
end