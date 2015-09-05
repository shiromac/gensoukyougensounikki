function effect_test(posX,posY,posZ)
    posX = posX+5;
    posY = posY+5;

    hab = AnimeChip_Hab(PositionPerGrid(posX, posY, posZ))

    effect_time = 20

    eff_obj = AnimeChip_Perticle("effect\\equip\\wind_green.png",D3DXVECTOR2(80,80), effect_time)
    --eff_obj = AnimeChip_Perticle("effect\\sukima.png",D3DXVECTOR2(80,80), effect_time)
        
    --color_cascading--
    eff_obj.color: addCascade(Color(0,255,255,255),Color(255,255,255,255), 2)
    eff_obj.color: addCascade(Color(255,255,255,255), 16)
    eff_obj.color: addCascade(Color(255,255,255,255),Color(0,255,255,255),2)
    --move_cascading--
    eff_obj.position: addCascade(PositionPerGrid(0,0,0),PositionPerGrid(2,0,0),20)
    
    --set to hab--
    hab: addAnimeChip(eff_obj);    

    --fire--  
    --setAnimation_parallel(hab)
    setAnimation_wait(hab)
    return true
end

function sukima_eff(posX,posY,posZ,frame)
    posX = posX+5;
    posY = posY+5;
    
    hab = AnimeChip_Hab(PositionPerGrid(posX, posY, 0))
    
    eff_obj = AnimeChip_Perticle("effect\\sukima.png",D3DXVECTOR2(256,256), frame)
    --scale_cascading--
    eff_obj.scaleX: addCascade(0,0.95,10)
    eff_obj.scaleY: addCascade(0,5)
    eff_obj.scaleX: addCascade(0.95,1,frame-20)
    eff_obj.scaleY: addCascade(0.95,1,frame-15)
    eff_obj.scaleX: addCascade(1,0,10)
    --eff_obj.scaleY: addCascade(1,0,10)
    --color_cascading--   
    eff_obj.color: addCascade(Color(0,255,255,255),Color(127,255,255,255), 10)
    eff_obj.color: addCascade(Color(127,255,255,255),Color(255,255,255,255), frame-12)
    eff_obj.color: addCascade(Color(255,255,255,255),Color(0,255,255,255),2)
    
    eff_obj.position: addCascade(PositionPerGrid(0,0,posZ),frame)   
--    eff_obj.position: addCascade(PositionPerGrid(0,0,posZ),PositionPerGrid(0,0,0),10)    
    
    eff_obj.rotationAfter: addCascade(0,90,10)
    eff_obj.rotationAfter: addCascade(0,frame-10)

    
    
    --set to hab--
    hab: addAnimeChip(eff_obj);    
    
    setAnimation_parallel(hab)
    return true
end

function sukima(posX,posY,posZ,frame)
    
    sukima_eff(posX,posY,posZ,frame)
    --[[
    bg = EventBackground(0,_T("effect\\sukima.png"), posX, posY, 2)
    coroutine.yield(false)
    --bg: fadeOut(0)
    bg: fadeIn(10)
    coroutine.yield(false)
    sukima_eff(posX,posY,0,frame)  
    effect_Wait(posX,posY,frame-15)
    coroutine.yield(false)
    bg: fadeOut(2)
    coroutine.yield(false)
    ]]--
    return true
end

function tewi_tired()
	clearMassage()
    str_map = map_tstring_StyleString()

    tewi = PlayerCharacter()
    meiling = searchOneCharacterOnFloor(1002)
    
    
    turnCharacterForCharacter(tewi,meiling)
    storyEventMessageLoop(_T("LastDungeonOpenEventTewi1_"), str_map, nil)
    coroutine.yield(false)
    
    cameraMove(meiling.placeX,meiling.placeY, 0.7,0,1) 
    storyEventMessageLoop(_T("LastDungeonOpenEventTewi2_"), str_map, nil)
    coroutine.yield(false)
    cameraMove(tewi.placeX,tewi.placeY, 0.7,0,1)
    coroutine.yield(false)
	clearMassage()
    return true
end

function lastDungeonOpen()
            fairy = searchOneCharacterOnFloor(4000)
            hina = searchOneCharacterOnFloor(4001)
            marisa = searchOneCharacterOnFloor(4002)
            nitori = searchOneCharacterOnFloor(4005)
            akyu = searchOneCharacterOnFloor(4008)
            
            if(PlayerCharacter(): ID() == 1002)
             then
             meiling = PlayerCharacter()
             tewi = searchOneCharacterOnFloor(1003)
             else 
             tewi = PlayerCharacter()
             meiling = searchOneCharacterOnFloor(1002)
             
             tewi_tired()
          
            end
            
            --[[
            --pal_walks_to_near_by_pc--
            dest_pal = LandformPlace(PlayerCharacter().placeX+1, PlayerCharacter().placeY)
            moveWithAnimeToLand(pal,dest_pal,false,1.0,false)
            coroutine.yield(false)
            ]]

            local alice = MakeCharacter(4011, 0, CHARACTER_FORSE_FRIEND, LandformPlace(PlayerCharacter().placeX,PlayerCharacter().placeY-2))

			clearMassage()
			str_map = map_tstring_StyleString()
			
			turnCharacterForCharacter(PlayerCharacter(),alice)
			
			
            storyEventMessageLoop(_T("LastDungeonOpenEvent1_"), str_map, nil)
            StopBGM()
            PlayBGM(_T(""),_T("talk2_main.wav"))            
            storyEventMessageLoop(_T("LastDungeonOpenEvent2_"), str_map, nil)
            --PlayBGM(_T("talk_strain_intro.wav"),_T("talk_strain_main.wav"))
            
            storyEventMessageLoop(_T("LastDungeonOpenEvent3_"), str_map, nil)
                        
            
            --yukari_comes_in---------------------------------------------------------------
            PlayBGM(_T("talk_strain_intro.wav"),_T("talk_strain_main.wav"))
            coroutine.yield(false)
            local yukariX = PlayerCharacter().placeX-2
            local yukariY = PlayerCharacter().placeY-2
            sukima(yukariX,yukariY,1,30)
            effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,10)
            coroutine.yield(false)
            local yukari = MakeCharacter(9045, 0, CHARACTER_FORSE_FRIEND, LandformPlace(yukariX,yukariY))
            effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,20)
            
            storyEventMessageLoop(_T("LastDungeonOpenEvent4_"), str_map, nil)
            
            effect_test(yukari.placeX,yukari.placeY,0)
            
            --sukima(yukari.placeX,yukari.placeY,0,60)
            coroutine.yield(false)
            turnCharacterForCharacter(alice,yukari)
            coroutine.yield(false)
            jumpWithAnime(alice,ASPECT_RIGHT, 2, 0.7, true, 1.0, true)
            coroutine.yield(false)
            
            sukima(alice.placeX,alice.placeY,1,30)
            
            effect_Wait(alice.placeX, alice.placeY,15)
            CharacterForseGoAway(alice)
            effect_Wait(alice.placeX, alice.placeY,15)
            coroutine.yield(false)
            
            --jumpWithAnime(PlayerCharacter(),ASPECT_DOWN, 0, 0.7, false, 1.0, true)
                
            --alice_have_been_exiled---------------------------------------------------------------
            PlayBGM(_T(""),_T("talk1_main.wav"))
            storyEventMessageLoop(_T("LastDungeonOpenEvent5_"), str_map, nil)

            sukima(yukari.placeX,yukari.placeY,1,30)
            effect_Wait(yukari.placeX,yukari.placeY,15)
            CharacterForseGoAway(yukari)
            effect_Wait(yukari.placeX,yukari.placeY,15)
            coroutine.yield(false)

            PlayBGM(_T(""),_T("base_main.wav"))
            storyEventMessageLoop(_T("LastDungeonOpenEvent6_"), str_map, nil)
            coroutine.yield(false)
     	    setMapValue(savable_globalFlags(),_T("EventDidFlag:lastDungronOpen"), YES)
            appearDungeon(_T("last_Dungeon"))                   
			clearMassage()
            return true
end

function homeEvent()
    if (getMapValue(savable_globalFlags(),_T("EventDidFlag:greenDungeonCleard"),NO)) == YES then
	    if( isDungeonCleared(_T("purple_Dungeon")) )then
    		if (getMapValue(savable_globalFlags(),_T("EventDidFlag:lastDungronOpen"),NO)) == NO then	    

                hina = searchOneCharacterOnFloor(4001)
                marisa = searchOneCharacterOnFloor(4002)
                nitori = searchOneCharacterOnFloor(4005)
                akyu = searchOneCharacterOnFloor(4008)

                if(hina and marisa and nitori and akyu) then
	                if(PlayerCharacter(): ID() == 1002)then  
                        stackDramaThread(coroutine.create(lastDungeonOpen))
                        --meiling = PlayerCharacter()                
                    else
                    --tewi = PlayerCharacter()
                    --meiling = searchOneCharacterOnFloor(1002)
                    stackDramaThread(coroutine.create(tewi_tired))
                    end
                end
            end
		end
	end
end
