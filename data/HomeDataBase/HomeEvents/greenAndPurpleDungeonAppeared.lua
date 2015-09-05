function blueDungeonClose()
            ---------------------------------------------------------------------------------------------------------------------------
			
            --generate_tewi
			tewi = MakeCharacter(4009, 0, CHARACTER_FORSE_FRIEND, LandformPlace(PlayerCharacter().placeX-1,PlayerCharacter().placeY-1))
            --storyEventMessageLoop(_T("blueDungeonCloseEvent_"), str_map, nil)
            
                        
            ---------------------------------------------------------------------------------------------------------------------------

            storyEventMessageLoopRange(_T("blueDungeonCloseEvent_"),1,1, str_map, nil)            

            --[[
            for i =0,10 do
                for j=0,7 do 
                    CharacterTurn(tewi,j)
                    effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,2)
                end
            end
            ]]

            tewi_drct = 1
            tewi_waitTime = 30
            CharacterTurn(tewi,tewi_drct)
            effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,tewi_waitTime)

                for tewi_drct=2,2 do 
                    CharacterTurn(tewi,tewi_drct)
                    effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,5)
                end   
         
            tewi_drct = 3
            tewi_waitTime = 30
            CharacterTurn(tewi,tewi_drct)
            effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,tewi_waitTime)
            
                for tewi_drct=4,6 do 
                    CharacterTurn(tewi,tewi_drct)
                    effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,5)
                end   
         
            tewi_drct = 7
            tewi_waitTime = 30
            CharacterTurn(tewi,tewi_drct)
            effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,tewi_waitTime)
            
            turnCharacterForCharacter(tewi,PlayerCharacter())

            storyEventMessageLoopRange(_T("blueDungeonCloseEvent_"),2,8, str_map, nil)            
            --storyEventMessageLoop(_T("blueDungeonCloseEvent_"), str_map, nil)
            
    
            
            --tewi_walk
            cameraMoveParallel(PlayerCharacter().placeX+2, PlayerCharacter().placeY+1, 0.5,0.75, 0.9)
            moveWithAnime(tewi, ASPECT_RIGHT, 2, false, 1.0, true)
            coroutine.yield(false)
            moveWithAnime(tewi, ASPECT_DOWNRIGHT, 2, false, 1.0, true);
            --moveWithAnime(tewi, 3, 2, false, 1.0, true)
            coroutine.yield(false)
            --stackDramaThread(coroutine.create(blueDungeonCloseTewiwalk(tewi)))
            --blueDungeonCloseTewiwalk(tewi)
            
            -----greenAndPurpleOpenEvent----------------------------------------------------------------------------------
            fairy = searchOneCharacterOnFloor(4000)
			--storyEventMessageLoop(_T("greenAndPurpleOpenEvent_"), str_map, nil)
            storyEventMessageLoopRange(_T("greenAndPurpleOpenEvent_"),1,1, str_map, nil)
                --moveWithAnime(tewi,ASPECT_UPRIGHT, 1, false, 1.0, true)
                --coroutine.yield(false)
                --turnCharacterForCharacter(fairy,PlayerCharacter())
			CharacterTurn(fairy,ASPECT_DOWN)
                --turnCharacterForCharacter(PlayerCharacter(),fairy)
            CharacterTurn(PlayerCharacter(),ASPECT_UP)
			    --turnCharacterForCharacter(tewi,fairy)

			--CharacterTurn(tewi,ASPECT_UPLEFT)
            tjp = LandformNearby(LandformCharacterPosition(tewi),ASPECT_UPLEFT,1)
            cameraMoveParallel(fairy.placeX+1, fairy.placeY+1, 1.5,0.5,0.75)
            moveWithAnimeToLand(tewi,tjp,false,1.0,false)
            --moveWithAnime(tewi,ASPECT_UPLEFT, 1, false, 1.0, true)
            coroutine.yield(false)
            storyEventMessageLoopRange(_T("greenAndPurpleOpenEvent_"),2,8, str_map, nil) 			
            coroutine.yield(false)          
           
            --jumpWithAnimeToLand(pcCharacter pchara, pcLandform toLand, double height, bool neverTurn, double speed, bool omission);
            jumpWithAnimeToLand(tewi,tjp,0.5,false,1.0,false)
            coroutine.yield(false)            
            jumpWithAnimeToLand(tewi,tjp,0.5,false,1.0,false)
            coroutine.yield(false)
            CharacterTurn(PlayerCharacter(),ASPECT_RIGHT)
            --[[
            jumpWithAnime(tewi, ASPECT_UPLEFT, 1, 1, false, 1.0, true)
            coroutine.yield(false)
            jumpWithAnime(tewi, ASPECT_UPLEFT, 1, 1, false, 1.0, true)
            coroutine.yield(false)
            clearMassage()
            ]]
            -------------
            
            storyEventMessageLoopRange(_T("greenAndPurpleOpenEvent_"),9,9, str_map, nil)
            coroutine.yield(false)
            jumpWithAnime(PlayerCharacter(), ASPECT_RIGHT, 0, 0.7, false, 1.0, true)
            ----------------
            storyEventMessageLoopRange(_T("greenAndPurpleOpenEvent_"),10,10, str_map, nil)
            coroutine.yield(false)
            CharacterTurn(PlayerCharacter(),ASPECT_UP)
            storyEventMessageLoopRange(_T("greenAndPurpleOpenEvent_"),11,17, str_map, nil)
            coroutine.yield(false)
            fadeBlack(180)
            effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,180)
            coroutine.yield(false)
            clearMassage()
            coroutine.yield(false)
            
            --fadeOut(180)
            effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,180)
            --coroutine.yield(false)
            
            CharacterForseGoAway(tewi)
            gotoTempDungeon(_T("event_dream_Dungeon"))
            
            return true
end

function meiling_lie(meiling,frame)

    hab = AnimeChip_Hab()

    local eff_obj = AnimeChip_Chara(meiling, frame)
    eff_obj.pitching: addCascade(90,frame)
--[[
    eff_obj.pitching: addCascade(90,30,0,30)
    eff_obj.yawing: addCascade(180,90)
    eff_obj.yawing: addCascade(180,270,315,360,90)
    ]]
    --eff_obj.yawing: addCascade(270,360,30)
    eff_obj.position: addCascade(PositionPerGrid(0, 0, -0.5),frame)
--    eff_obj.position: addCascade(PositionPerGrid(0.3, 0, -0.5),PositionPerGrid(0, 0, 0),30)

    --PositionPerGrid(posX, posY, posZ)
    --eff_obj.color: addCascade(Color(0,255,255,255),Color(255,255,255,255), 60)
    

    --set to hab--
    hab: addAnimeChip(eff_obj);    
    
    --fire--  
    setAnimation_parallel(hab)
    return true
end

function dreamEventClose()
    fadeBlack(1)
    setHidingMiniMap(true)
    effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,5)
	clearMassage()
	
	str_map = map_tstring_StyleString()
    PlayerCharacter():setStopStamp(true)
    moveWithAnimeToLand(PlayerCharacter(),LandformPlace(19,16),false,10,false)
    cameraMoveParallel(19,16, 10,1,0)
    coroutine.yield(false)
    CharacterTurn(PlayerCharacter(),ASPECT_DOWN)
    coroutine.yield(false)

    --------------------------------------------------------------------------

    meiling_lie(PlayerCharacter(),120)
    fadeOut(60)
    coroutine.yield(false)
    storyEventMessageLoop(_T("greenAndPurpleOpenWake_"), str_map, nil)
	clearMassage()
    coroutine.yield(false)
    PlayerCharacter():setStopStamp(false)
    coroutine.yield(false)
    return true
end

function homeEvent()
	if( isDungeonCleared(_T("blue_Dungeon")) )then
      if (getMapValue(savable_globalFlags(),_T("EventDidFlag:blueDungeonCleard"),NO)) == NO or (getMapValue(savable_globalFlags(),_T("CharacterPlayableFlag:Tewi"),NO)) == NO then
	 		setMapValue(savable_globalFlags(),_T("EventDidFlag:blueDungeonCleard"), YES)
            setMapValue(savable_globalFlags(),_T("CharacterPlayableFlag:Tewi"), YES)
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
			--turnCharacterForCharacter(PlayerCharacter(),keibimeido)
			
			--cameraMoveParallel(keibimeido.placeX, keibimeido.placeY, 1,0.25,0.75)
					
			clearMassage()
			str_map = map_tstring_StyleString()
            stackDramaThread(coroutine.create(blueDungeonClose))
            --blueDungeonClose()
     elseif (getMapValue(savable_globalFlags(),_T("EventDidFlag:dreamDungeonCleard"),NO)) == NO then
			setMapValue(savable_globalFlags(),_T("EventDidFlag:dreamDungeonCleard"), YES)
			
            stackDramaThread(coroutine.create(dreamEventClose))
            appearDungeon(_T("green_Dungeon"))
			appearDungeon(_T("purple_Dungeon"))
			--cameraMove(PlayerCharacter().placeX, PlayerCharacter().placeY, 1.5,0.75, 0.9)
     end
	end
end
