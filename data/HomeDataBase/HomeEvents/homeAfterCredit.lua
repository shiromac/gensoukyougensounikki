function black_back(posX,posY,frame)

    local bg = EventBackground(0,_T("event\\black_back.png"), posX, posY, 2)

    coroutine.yield(false)
    cameraMove(posX,posY,10,1,1)
    bg:fadeIn(frame)
    coroutine.yield(false)

    return bg
end

function homeAfterCredit()
            StopBGM()
            setHiddenInterface(true);
            setHidingMiniMap(true)
            coroutine.yield(false)
            local alice_gri =  MakeCharacter(4013, 0, CHARACTER_FORSE_OTHER, LandformPlace(17,13))
            local alice_mg = MakeCharacter(4011, 0, CHARACTER_FORSE_OTHER, LandformPlace(9,14))
            fairy = searchOneCharacterOnFloor(4000)
            hina = searchOneCharacterOnFloor(4001)
            marisa = searchOneCharacterOnFloor(4002)
            nitori = searchOneCharacterOnFloor(4005)
            akyu = searchOneCharacterOnFloor(4008)
            --tewi = searchOneCharacterOnFloor(4009)
            tewi = searchOneCharacterOnFloor(1003)
            
            str_map = map_tstring_StyleString()
            clearMassage()
           
            --effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,30)
            CharacterTurn(PlayerCharacter(),ASPECT_DOWN)
            CharacterTurn(alice_mg,ASPECT_RIGHT)
            coroutine.yield(false)
            moveWithAnime(marisa,ASPECT_LEFT, 1, false, 0.5, true)
            moveWithAnime(nitori,ASPECT_UP,1, false, 0.7, true)           
            coroutine.yield(false)            
            turnCharacterForCharacter(marisa,alice_mg)
            turnCharacterForCharacter(alice_mg,marisa)
            effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,30)
            coroutine.yield(false)
                       
--            effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,30)

            moveWithAnime(nitori,ASPECT_UP,1, false, 0.7, true)
            moveWithAnime(PlayerCharacter(),ASPECT_RIGHT, 1, false, 0.7, true)
            coroutine.yield(false)
            moveWithAnime(PlayerCharacter(),ASPECT_DOWN, 1, false, 0.7, true)
            cameraMoveParallel(akyu.placeX,akyu.placeY, 0.4,0,1)        
            turnCharacterForCharacter(marisa,alice_mg)
            turnCharacterForCharacter(alice_mg,marisa)
            coroutine.yield(false)
            CharacterTurn(PlayerCharacter(),ASPECT_RIGHT)
            moveWithAnime(nitori,ASPECT_UPRIGHT,1, false, 0.7, true)
            moveWithAnime(hina,ASPECT_DOWNLEFT, 1, false, 0.5, true)
            moveWithAnime(tewi,ASPECT_UP, 2, false, 0.5, true)
            coroutine.yield(false)
            moveWithAnime(hina,ASPECT_LEFT, 1, false, 0.5, true)
            moveWithAnime(tewi,ASPECT_LEFT, 3, false, 0.5, true)
            coroutine.yield(false)

            
            coroutine.yield(false)
            

            alice_gri:setStopStamp(true)            
            akyu:setStopStamp(true)            
            
            coroutine.yield(false)

            bg = black_back(akyu.placeX, akyu.placeY,120)
            effect_Wait(akyu.placeX, akyu.placeY,150)
            
            --[[
            moveWithAnime(tewi,ASPECT_UP, 2, false, 0.5, true)
            coroutine.yield(false)
            moveWithAnime(tewi,ASPECT_LEFT, 3, false, 0.5, true)
            coroutine.yield(false)
            
            cameraMoveParallel(akyu.placeX, akyu.placeY, 1,0.25,0.75)
            ]]
          
		    storyEventMessageLoop(_T("AfterCredit_"), str_map, nil)
            coroutine.yield(false)            
            clearMassage()
            
            show_owari(akyu.placeX, akyu.placeY,600,150,180)
            effect_Wait(akyu.placeX, akyu.placeY,180)
            CharacterForseGoAway(akyu)
            CharacterForseGoAway(alice_gri)
            coroutine.yield(false)
            effect_Wait(akyu.placeX, akyu.placeY,420+120)
            setMapValue(savable_globalFlags(),_T("EventDidFlag:homeAfterCredit"), YES)
            coroutine.yield(false)
            gotoSceneTitle();
    return true
end

function show_owari(posX,posY,eff_time,in_frame,out_frame)
    posX = posX+5;
    posY = posY+5;

    img_owari = "event\\staff\\owari.png"
    
    local sizeX = 512
    local sizeY = 512

    local hab = AnimeChip_Hab(PositionPerGrid(posX, posY, 0.25))         
    local eff = AnimeChip_Perticle(img_owari,D3DXVECTOR2(sizeX,sizeY), eff_time)        

    displaying_frame = eff_time-(in_frame+out_frame)
        
    eff.color: addCascade(Color(0,255,255,255),Color(255,255,255,255), in_frame)
    eff.color: addCascade(Color(255,255,255,255),Color(255,255,255,255), displaying_frame)
    eff.color: addCascade(Color(255,255,255,255),Color(0,255,255,255),out_frame)

    hab: addAnimeChip(eff);        
    setAnimation_parallel(hab)

    return eff_time
end

function IFAppear()
    setHiddenInterface(false);
    return true
end

function homeEvent()
    if(isDungeonCleared(_T("last_Dungeon")))then        
        if( getMapValue(savable_globalFlags(),_T("EventDidFlag:homeAfterCredit"), NO ) == NO )then
            stackDramaThread(coroutine.create(homeAfterCredit))
        else
            stackDramaThread(coroutine.create(IFAppear))
        end
    end
end
