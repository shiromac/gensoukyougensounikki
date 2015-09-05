enemyPool = {}


function lastBossBefore()
    --------------------------set_talk_studio-----------------------------------------------
    setHidingMiniMap(true)
    fadeBlack(1)
    effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,5)
    --PlayBGM(_T(""),_T("talk1_main.wav"))
    cameraMove(talkposX,talkposY,10,1,1)
    local destBoss = LandformPlace(talkposX,talkposY-1)
    local destPC_tmp = LandformPlace(talkposX,talkposY+7)
    local destPC = LandformPlace(talkposX,talkposY)
    local alice_mg = MakeCharacter(4011, 0, CHARACTER_FORSE_ENEMY, destBoss)
    
    alice_mg:setStopStamp(true)
    
    moveWithAnimeToLand(boss,destBoss,false,10,false)
    moveWithAnimeToLand(PlayerCharacter(),destPC_tmp,false,10,false)
    CharacterTurn(alice_mg,ASPECT_UP)
    CharacterTurn(PlayerCharacter(),ASPECT_UP)

    StopBGM()
    coroutine.yield(false)

    --boss = MakeCharacter(4011, 0, CHARACTER_FORSE_ENEMY, bossLand)    

    -----go_action-----------------------------------------------------------------------------------
    
    in_frame = 120    
    fadeOut(in_frame)
    
    moveWithAnimeToLand(PlayerCharacter(),destPC,false,1,false)
    coroutine.yield(false)
    
    storyEventMessageLoop(_T("beforeLastBoss1_"), str_map, nil)
    coroutine.yield(false)
    moveWithAnime(PlayerCharacter(), ASPECT_UP, 1, false, 1, false)
    coroutine.yield(false)
    
    storyEventMessageLoop(_T("beforeLastBoss2_"), str_map, nil)
    coroutine.yield(false)
    local aliceMasked = MakeCharacter(4011, 0, CHARACTER_FORSE_ENEMY, destPC_tmp)
    coroutine.yield(false)
    CharacterTurn(aliceMasked,ASPECT_UP)
    coroutine.yield(false)
    cameraMove(talkposX,talkposY+1,0.5,1,1)
    coroutine.yield(false)
    moveWithAnime(aliceMasked, ASPECT_UP, 4, false, 1, false)
    coroutine.yield(false)
    --storyEventMessageLoop(_T("beforeLastBoss3_"), str_map, nil)
    storyEventMessageLoopRange(_T("beforeLastBoss3_"), 1, 1, str_map, nil)
    coroutine.yield(false)
    CharacterTurn(PlayerCharacter(),ASPECT_DOWN)
    coroutine.yield(false)
    storyEventMessageLoopRange(_T("beforeLastBoss3_"), 2, 2, str_map, nil)
    coroutine.yield(false)
    moveWithAnime(PlayerCharacter(), ASPECT_DOWN, 1, false, 1, false)
    coroutine.yield(false)
    storyEventMessageLoopRange(_T("beforeLastBoss3_"), 3, 6, str_map, nil)
    coroutine.yield(false)
    cameraMove(talkposX,talkposY+2,0.5,1,1)
    coroutine.yield(false)
    --alice_CO_old_alice-------------------------------------------------------------------------------------------

    CharacterTurn(aliceMasked,ASPECT_DOWN)
    effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,60)

    local alice_CO_Land = LandformPlace(aliceMasked.placeX, aliceMasked.placeY)
    effect_Smoke1(aliceMasked.placeX, aliceMasked.placeY)
    coroutine.yield(false)
    CharacterForseGoAway(aliceMasked)
    coroutine.yield(false)
    local alice = MakeCharacter(4013, 0, CHARACTER_FORSE_ENEMY, alice_CO_Land)
    coroutine.yield(false)
    CharacterTurn(alice,ASPECT_DOWN)
    coroutine.yield(false)
    effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,60)
    coroutine.yield(false)
    CharacterTurn(alice,ASPECT_UP)
    coroutine.yield(false)
    
    storyEventMessageLoopRange(_T("beforeLastBoss4_"), 1, 2, str_map, nil)
    --storyEventMessageLoop(_T("beforeLastBoss4_"), str_map, nil)
    coroutine.yield(false)
    PlayBGM(_T(""),_T("talk1_main.wav"))
    coroutine.yield(false)
    PlayBGM(_T("alice_intro.wav"),_T("alice_main.wav"))
    storyEventMessageLoopRange(_T("beforeLastBoss4_"), 3, 5, str_map, nil)
    --PlayBGM(_T("ending_intro.wav"),_T(""))
    coroutine.yield(false)
    
    storyEventMessageLoop(_T("beforeLastBoss5_"), str_map, nil)
    coroutine.yield(false)
    clearMassage()
    coroutine.yield(false)
    
    moveWithAnime(alice, ASPECT_UPLEFT, 1, false, 0.7, false)
    coroutine.yield(false)
    cameraMoveParallel(talkposX,talkposY,0.5,1,1)
    moveWithAnimeToLand(alice,LandformPlace(alice_mg.placeX-1,alice_mg.placeY),false,0.7,false)
    coroutine.yield(false)
    CharacterTurn(alice,ASPECT_DOWN)
    coroutine.yield(false)
    --near_aliceMG----------------------------------------------------------------------
    
    CharacterTurn(PlayerCharacter(),ASPECT_UP)
    --turnCharacterForCharacter(PlayerCharacter(),yuyuko)
    coroutine.yield(false)  
        
    storyEventMessageLoop(_T("beforeLastBoss6_"), str_map, nil)
    coroutine.yield(false)    

    --aliceMG_is_gone_away------------------------------------------------------
    CharacterTurn(alice,ASPECT_RIGHT)
    coroutine.yield(false)        
    jumpWithAnime(alice,ASPECT_RIGHT,0, 0.3, true, 1, false);
    
    --jumpWithAnime(alice_mg,ASPECT_RIGHT,5, 1, true, 0.5, false);
    coroutine.yield(false)
    effect_Smoke1(alice_mg.placeX, alice_mg.placeY)
    
    CharacterForseGoAway(alice_mg)
    coroutine.yield(false)
    
    moveWithAnime(alice, ASPECT_RIGHT, 1, false, 1, false)
    coroutine.yield(false)
    CharacterTurn(alice,ASPECT_DOWN)
    coroutine.yield(false)
    --jumpWithAnime(pcCharacter pchara, int aspect, int distance, double height, bool neverTurn, double speed, bool omission);
    StopBGM()
    coroutine.yield(false)
    effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,30)
    coroutine.yield(false)
    storyEventMessageLoop(_T("beforeLastBoss7_"), str_map, nil)
    coroutine.yield(false)
    clearMassage()
    coroutine.yield(false)
    effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,60)
    coroutine.yield(false)

    PlayBGM(_T("talk3_intro.wav"),_T("talk3_main.wav"))

    storyEventMessageLoop(_T("beforeLastBoss8_"), str_map, nil)
    coroutine.yield(false)
    clearMassage() 
    coroutine.yield(false)
    StopBGM()
    effect_Wait(talkposX,talkposY,60)
    coroutine.yield(false)
    PlayBGM(_T("grimore_intro.wav"),_T("grimore_main.wav"))
    coroutine.yield(false)

    storyEventMessageLoop(_T("beforeLastBoss9_"), str_map, nil)
    coroutine.yield(false)
    -----------------------------------------------------------------------------------
    fadeWhite(30)
    --effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,30)
    
    --jumpWithAnimeToLand(boss,bossLand,1.0,true,0.3,false)
    CharacterForseGoAway(alice)
    
    jumpWithAnimeToLand(PlayerCharacter(),pcLand,1.0,true,0.3,false)
    cameraMove(PlayerCharacter().placeX,PlayerCharacter().placeY,1,1,1)
    setHidingMiniMap(false)
    coroutine.yield(false)
    
    --PlayBGM(_T("boss_intro.wav"),_T("boss_main.wav"))
    
    in_frame = 30
    fadeOut(in_frame)
    effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,in_frame)
    coroutine.yield(false)
    clearMassage()
    startBossSpell(boss)

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
    
    eff_obj.position: addCascade(PositionPerGrid(0,0,posZ),frame-5)
    eff_obj.position: addCascade(PositionPerGrid(0,0,posZ),PositionPerGrid(0,-2,posZ),5)    
    
    eff_obj.rotationAfter: addCascade(0,90,10)
    eff_obj.rotationAfter: addCascade(0,frame-10)   
    
    --set to hab--
    hab: addAnimeChip(eff_obj);        
    setAnimation_parallel(hab)
    return true
end

function staffRoll(posX,posY)
    posX = posX+5;
    posY = posY+5;

    img_dir_path = "event\\staff\\"
    img = { "kikaku.png",--1
            "gameDesign.png",--2
            "program.png",--3
            "programKyouryoku.png",--4
            "scenarioGenan.png",--5
            "scenario.png",--6
            "ensyutu.png",--7
            "dungeonDesign.png",--8
            "bossBattleDesign.png",--9
            "balanceTyousei.png",--10
            "graphic.png",--11
            "graphicGaibuStaff.png",--12
            "effectDesign.png",--13
            "ongaku.png",--14
            "koukaon.png",--15
            "packageDesign.png",--16
            "gensaku.png",--17
            "saisaku.png"}--18

    eff_time = 60*78+300--60frame*sec
    eff_time_remain = eff_time

    local i = 1;
    --local hab_i = 1
    local sizeX = 512
    local sizeY = 512
    local hab = {}
    local eff_time_per_perticle = 260--eff_time/num_of_perticle 
    local in_frame = 30
    local out_frame = 30

    for i = 1 , 18 do
        hab[i]= AnimeChip_Hab(PositionPerGrid(posX, posY, 0))         
        local eff = AnimeChip_Perticle(img_dir_path..img[i],D3DXVECTOR2(sizeX,sizeY), eff_time)        

        displaying_frame = eff_time_per_perticle-(in_frame+out_frame)

        if i == 18 then
            displaying_frame = eff_time_remain - (in_frame + out_frame + 30)
            out_frame = out_frame + 30
        else       
            eff_time_remain = eff_time_remain - eff_time_per_perticle
        end
  
        if (i ~= 1) then
        eff.color: addCascade(Color(0,255,255,255),Color(0,255,255,255), eff_time_per_perticle*(i-1))
        end
        eff.color: addCascade(Color(0,255,255,255),Color(255,255,255,255), in_frame)
        eff.color: addCascade(Color(255,255,255,255),Color(255,255,255,255), displaying_frame)
        eff.color: addCascade(Color(255,255,255,255),Color(0,255,255,255),out_frame)

        --set to hab--
        hab[i]: addAnimeChip(eff);        
        setAnimation_parallel(hab[i])
    end

    PlayBGM(_T("ending_intro.wav"),_T(""))

    return eff_time
end

function hakurei_event(posX,posY,bg)

    cameraMove(posX,posY-1,10,1,1)
    local reimu = MakeCharacter(9019, 0, CHARACTER_FORSE_ENEMY, LandformPlace(posX+1,posY))
    
    
    coroutine.yield(false)
    --bg: fadeOut(0)
    bg: fadeIn(1)
    coroutine.yield(false)  
    fadeOut(30)
    coroutine.yield(false)  
    ----go_action--
    
    effect_Wait(posX,posY,60)
    
    sukima_eff(posX-1,posY,0,60)  
    effect_Wait(posX,posY,50)
    local yukari = MakeCharacter(9045, 0, CHARACTER_FORSE_ENEMY, LandformPlace(posX-1,posY))
    coroutine.yield(false)
    effect_Wait(posX,posY,30)
    turnCharacterForCharacter(reimu,yukari)
    coroutine.yield(false)
    effect_Wait(posX,posY,30)
    coroutine.yield(false)
    turnCharacterForCharacter(yukari,reimu)  
    coroutine.yield(false)
    
    clearMassage()
    coroutine.yield(false)
    


    return true
end

function black_back(posX,posY,frame)
    local bg = EventBackground(cEvent.PRIORITY_MIDDLE,_T("event\\black_back.png"), posX, posY, 2)
    coroutine.yield(false)
    --cameraMove(posX,posY,10,1,1)
    bg:fadeIn(frame)
    coroutine.yield(false)
    return bg
end

function ED2(posX,posY,bg)
    local meiling_cameraposX = PlayerCharacter().placeX
    local meiling_cameraposY = PlayerCharacter().placeY

    fadeBlack(30)
    coroutine.yield(false)
    effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,60)
    coroutine.yield(false)
    cameraMove(posX,posY,10,1,1)    
    coroutine.yield(false)
    StopBGM()
    effect_Wait(posX,posY,60)
    coroutine.yield(false)
    local posX_s = posX+13
    local posY_s = posY
    local bg_shrine = EventBackground(cEvent.PRIORITY_LOW,_T("event\\hakurei_back.png"), posX_s, posY_s, 2)
    
--    black_back(posX_s,pos_s,1)
    coroutine.yield(false)    
    local bg_black1 = black_back(posX+2,posY,1)
    coroutine.yield(false)
    effect_Wait(posX,posY,30)

    PlayBGM(_T("lonealice_intro.wav"),_T("lonealice_main.wav"))  

    --CharacterForseGoAway(aliceMasked)
    --coroutine.yield(false)
    
    --alice_appears--------------------------------------------------------------------------

    local alice = MakeCharacter(4013, 0, CHARACTER_FORSE_ENEMY, LandformPlace(posX, posY)) 
    alice:setStopStamp(true)
    coroutine.yield(false)
    fadeOut(60)
    effect_Wait(posX,posY,60)
    coroutine.yield(false)
    
    -----------------------------------------------------------------------------------------
    storyEventMessageLoop(_T("ED2A_"), str_map, nil)     
    coroutine.yield(false)
    clearMassage()
    --diary_appears--------------------------------------------------------------------------    
    effect_Smoke1(posX+1,posY)
    MakeDroping(6001, LandformPlace(posX+1,posY))

    -----------------------------------------------------------------------------------------
    storyEventMessageLoop(_T("ED2B_"), str_map, nil)
    coroutine.yield(false)
    clearMassage()
    -----------------------------------------------------------------------------------------
    -----------------------------------------------------------------------------------------    
    storyEventMessageLoop(_T("ED2C_"), str_map, nil)
    coroutine.yield(false)
    clearMassage()
    -----------------------------------------------------------------------------------------
    fadeBlack(60)
    effect_Wait(posX,posY,60)

    coroutine.yield(false)
    local reimu = MakeCharacter(9019, 0, CHARACTER_FORSE_ENEMY, LandformPlace(posX_s, posY_s-1))
    local marisa = MakeCharacter(4002, 0, CHARACTER_FORSE_ENEMY, LandformPlace(posX_s+1, posY_s+1))
    cameraMove(posX_s,posY_s-1,10,1,1)
    bg_black1:fadeOut(1)
    coroutine.yield(false)
    fadeOut(60)   
    bg_shrine:fadeIn(60)
    --effect_Wait(posX_s,posY_s,60)
    coroutine.yield(false)



    -----------------------------------------------------------------------------------------
    storyEventMessageLoop(_T("ED2D_"), str_map, nil) 
    coroutine.yield(false)
    clearMassage()
    ---shrine_scene_w/o_alice----------------------------------------------------------------
    
    local posX_border = posX_s-3
    local posY_border = posY_s

    moveWithAnimeToLand(alice, LandformPlace(posX_border-2, posY_border), false, 5, false);

    coroutine.yield(false)
    bg_black1:fadeIn(60)
    cameraMove(posX_border+1,posY_border-1,0.15,0,1)
    CharacterTurn(alice,ASPECT_RIGHT)
    coroutine.yield(false)
    -----------------------------------------------------------------------------------------
    storyEventMessageLoop(_T("ED2E_"), str_map, nil)
    coroutine.yield(false)
    clearMassage()
    ---alice_mg moves to gensokyo------------------------------------------------------------
    CharacterTurn(alice,ASPECT_DOWN)
    coroutine.yield(false)
    effect_Wait(alice.placeX,alice.placeY,30)
    coroutine.yield(false)

    local alice_change_posX =  alice.placeX
    local alice_change_posY =  alice.placeY
    effect_Smoke1(alice_change_posX,alice_change_posY)
    MakeDroping(6001, LandformPlace(alice_change_posX, alice_change_posY))
    CharacterForseGoAway(alice)
    coroutine.yield(false)
    local alice_mg = MakeCharacter(4011, 0, CHARACTER_FORSE_ENEMY, LandformPlace(alice_change_posX, alice_change_posY))
    alice_mg:setStopStamp(true)
    coroutine.yield(false)
    effect_Wait(alice_change_posX,alice_change_posY,60)
    moveWithAnimeToLand(alice_mg, LandformPlace(posX_s-2,posY_s),true,0.3,false)
    coroutine.yield(false)
    alice_mg:setStopStamp(false)
    CharacterTurn(alice_mg,ASPECT_RIGHT)
    CharacterTurn(reimu,ASPECT_LEFT)
    CharacterTurn(marisa,ASPECT_LEFT)
    coroutine.yield(false)
    
    -----------------------------------------------------------------------------------------    
    storyEventMessageLoop(_T("ED2F_"), str_map, nil)
    coroutine.yield(false)
    clearMassage()
    ---grimore(as_alice)_is_out_of_gensokyo--------------------------------------------------------------------------------------

    --cameraMove(alice_change_posX,alice_change_posY,0.3,0,1)
    --coroutine.yield(false)

    effect_Smoke1(alice_change_posX,alice_change_posY)    
    coroutine.yield(false)
    eraseFloorDropings()
    coroutine.yield(false)
    local grimore = MakeCharacter(4013, 0, CHARACTER_FORSE_ENEMY, LandformPlace(alice_change_posX,alice_change_posY))
    CharacterTurn(grimore,ASPECT_RIGHT)
    coroutine.yield(false)

    -----------------------------------------------------------------------------------------  
    storyEventMessageLoop(_T("ED2G_"), str_map, nil)
    coroutine.yield(false)
    clearMassage()
    -----------------------------------------------------------------------------------------    
    -----------------------------------------------------------------------------------------    
    storyEventMessageLoop(_T("ED2H_"), str_map, nil)
    coroutine.yield(false)
    clearMassage()    
    ----mixed_gensokyo-------------------------------------------------------------------------------------
    
    local reimu_tbl={reimu}    
    local marisa_tbl={marisa}
--    reimu_tbl[1] = reimu
--    marisa_tbl[1] = marisa
    
    effect_Smoke1(posX_s-2, posY_s+2)
    reimu_tbl[2] = MakeCharacter(9019, 0, CHARACTER_FORSE_ENEMY, LandformPlace(posX_s-2, posY_s+2))    
    coroutine.yield(false)
    effect_Wait(posX_s,posY_s,60)    
    coroutine.yield(false)
    
    effect_Smoke1(posX_s+1, posY_s+2)
    marisa_tbl[2] = MakeCharacter(4002, 0, CHARACTER_FORSE_ENEMY, LandformPlace(posX_s+1, posY_s+2))
    effect_Wait(posX_s,posY_s,60)    
    coroutine.yield(false)
    
    --local reimu = MakeCharacter(9019, 0, CHARACTER_FORSE_ENEMY, LandformPlace(posX_s+1, posY_s+2))
    --local marisa = MakeCharacter(4002, 0, CHARACTER_FORSE_ENEMY, LandformPlace(posX_s, posY_s+3))
    
    -----------------------------------------------------------------------------------------
    storyEventMessageLoop(_T("ED2I_"), str_map, nil)
    coroutine.yield(false)
    clearMassage()           
    --erace_characters_other_than_alice---------------------------------------------------------------------------------------
    --[[
    effect_Smoke1(reimu_tbl[1].placeX,reimu_tbl[1].placeY);
    effect_Smoke1(reimu_tbl[2].placeX,reimu_tbl[2].placeY);
    effect_Smoke1(marisa_tbl[1].placeX,marisa_tbl[1].placeY);
    effect_Smoke1(marisa_tbl[2].placeX,marisa_tbl[2].placeY);
    coroutine.yield(false)  
    CharacterForseGoAway(reimu_tbl[1])
    CharacterForseGoAway(reimu_tbl[2])
    CharacterForseGoAway(marisa_tbl[1])    
    CharacterForseGoAway(marisa_tbl[2])    
    coroutine.yield(false)
    ]]
    effect_Wait(posX_border,posY_border,30)
    
    alice_mg:setStopStamp(true)
    grimore:setStopStamp(true)
--[[
    bg_shrine:fadeOut(60)
    local bg_black2 = black_back(posX_s,posY_s,60)
 ]]

    CharacterTurn(grimore,ASPECT_DOWN)
    CharacterTurn(alice_mg,ASPECT_UP)
    
    moveWithAnimeToLand(alice_mg,LandformPlace(posX_border-2,posY_border+1),true,0.4,false)
    moveWithAnimeToLand(grimore,LandformPlace(posX_border-2,posY_border-3),true,0.4,false)
    cameraMoveParallel(posX_border,posY_border-1,0.2,0,1)
    coroutine.yield(false)
    turnCharacterForCharacter(grimore,alice_mg)
    turnCharacterForCharacter(alice_mg,grimore)

    coroutine.yield(false)
    effect_Wait(posX_border,posY_border,60)
    coroutine.yield(false)


    turnCharacterForCharacter(grimore,alice_mg)
    turnCharacterForCharacter(alice_mg,grimore)    
    coroutine.yield(false)

    effect_Wait(posX_border,posY_border,60)
    coroutine.yield(false)
    
    -----------------------------------------------------------------------------------------
    storyEventMessageLoop(_T("ED2J_"), str_map, nil)
    coroutine.yield(false)    
    clearMassage()
    coroutine.yield(false)
    --yukari_comes_in-(near_by_grimore)--------------------------------------------------------------------------------------

    local yukariX = grimore.placeX+4
    local yukariY = grimore.placeY+3
    
    sukima_eff(yukariX,yukariY,0,60)  
    effect_Wait(yukariX,yukariY,50)
    
    local yukari = MakeCharacter(9045, 0, CHARACTER_FORSE_ENEMY, LandformPlace(yukariX,yukariY))
    coroutine.yield(false)
    effect_Wait(yukari.placeX,yukari.placeY,30)
    coroutine.yield(false)
    storyEventMessageLoop(_T("ED2K_"), str_map, nil)
    coroutine.yield(false)
    clearMassage()
    effect_Wait(yukari.placeX,yukari.placeY,60)
    coroutine.yield(false)
    moveWithAnimeToLand(yukari,LandformPlace(grimore.placeX+1,grimore.placeY+3),false,0.3,false)
--    coroutine.yield(false)
--    effect_Wait(yukari.placeX,yukari.placeY,60)
    coroutine.yield(false)
--    CharacterTurn(yukari,ASPECT_UP)
--    coroutine.yield(false)
    moveWithAnimeToLand(yukari,LandformPlace(grimore.placeX+1,grimore.placeY+1),false,0.3,false)  
    coroutine.yield(false)
    turnCharacterForCharacter(yukari,grimore)
    coroutine.yield(false)    
    -----------------------------------------------------------------------------------------
    storyEventMessageLoop(_T("ED2K2_"), str_map, nil)
    coroutine.yield(false)    
    clearMassage()
    coroutine.yield(false)
    --yukari_walks_to_alice_mg---------------------------------------------------------------------------------------    
    moveWithAnimeToLand(yukari,LandformPlace(alice_mg.placeX+1,alice.placeY-1),false,0.3,false)
    coroutine.yield(false)
    turnCharacterForCharacter(yukari,alice_mg)
    coroutine.yield(false)  
    effect_Wait(yukari.placeX,yukari.placeY,30)
    coroutine.yield(false)
    -----------------------------------------------------------------------------------------
    storyEventMessageLoop(_T("ED2L_"), str_map, nil)
    coroutine.yield(false)
    clearMassage()    
    coroutine.yield(false)
    -----------------------------------------------------------------------------------------    
    
    effect_Smoke1(reimu_tbl[1].placeX,reimu_tbl[1].placeY);
    effect_Smoke1(reimu_tbl[2].placeX,reimu_tbl[2].placeY);
    effect_Smoke1(marisa_tbl[1].placeX,marisa_tbl[1].placeY);
    effect_Smoke1(marisa_tbl[2].placeX,marisa_tbl[2].placeY);
    coroutine.yield(false)
    CharacterForseGoAway(reimu_tbl[1])
    CharacterForseGoAway(reimu_tbl[2])
    CharacterForseGoAway(marisa_tbl[1])    
    CharacterForseGoAway(marisa_tbl[2])    
    coroutine.yield(false)  
    
    bg_shrine:fadeOut(60)
    local bg_black2 = black_back(posX_s,posY_s,60)
    effect_Wait(yukari.placeX,yukari.placeY,60)
    coroutine.yield(false) 
    
    effect_Smoke1(grimore.placeX+4,grimore.placeY)
    local victim = MakeCharacter(1002, 0, CHARACTER_FORSE_ENEMY, LandformPlace(grimore.placeX+4,grimore.placeY))
    victim:setStopStamp(true)
    
    effect_Wait(yukari.placeX,yukari.placeY,30)    
    coroutine.yield(false)
    moveWithAnimeToLand(yukari,LandformPlace(victim.placeX,victim.placeY+2),false,0.3,false)
    coroutine.yield(false)
    CharacterTurn(yukari,ASPECT_UP)
    coroutine.yield(false)
    -----------------------------------------------------------------------------------------
    storyEventMessageLoop(_T("ED2M_"), str_map, nil)
    coroutine.yield(false)
    clearMassage()        
    coroutine.yield(false)
    -----------------------------------------------------------------------------------------
    effect_Smoke1(alice_mg.placeX+4,alice_mg.placeY);
    local justice = MakeCharacter(9019, 0, CHARACTER_FORSE_ENEMY, LandformPlace(alice_mg.placeX+4,alice_mg.placeY))
    coroutine.yield(false)
    justice:setStopStamp(true)
    coroutine.yield(false)
    CharacterTurn(justice,ASPECT_UP)       
    coroutine.yield(false)    
    moveWithAnimeToLand(yukari,LandformPlace(justice.placeX,justice.placeY-2),false,0.3,false)
    coroutine.yield(false)
    effect_Wait(yukari.placeX,yukari.placeY,30)
    coroutine.yield(false)
    CharacterTurn(yukari,ASPECT_RIGHT)
    coroutine.yield(false)    
    effect_Wait(yukari.placeX,yukari.placeY,30)
    coroutine.yield(false)
    turnCharacterForCharacter(yukari,justice)
    coroutine.yield(false)
    -----------------------------------------------------------------------------------------
    storyEventMessageLoop(_T("ED2N_"), str_map, nil)
    coroutine.yield(false)        
    clearMassage()
    coroutine.yield(false)
    moveWithAnimeToLand(yukari,LandformPlace(posX_border,posY_border-1),false,0.3,false)
    coroutine.yield(false)
    CharacterTurn(yukari,ASPECT_DOWN)

    effect_Wait(yukari.placeX, yukari.placeY,30)
    effect_Smoke1(alice_mg.placeX,alice_mg.placeY)
    effect_Smoke1(grimore.placeX,grimore.placeY)
    effect_Smoke1(victim.placeX,victim.placeY)
    effect_Smoke1(justice.placeX,justice.placeY)
    coroutine.yield(false)
    CharacterForseGoAway(alice_mg)
    CharacterForseGoAway(grimore)
    CharacterForseGoAway(victim)
    CharacterForseGoAway(justice)  
    StopBGM()
    --effect_Wait(yukari.placeX, yukari.placeY,30)
    coroutine.yield(false)
    yukari:setStopStamp(true)
    coroutine.yield(false)
    effect_Wait(yukari.placeX, yukari.placeY,60)
    coroutine.yield(false)

    CharacterTurn(yukari,ASPECT_DOWNLEFT)    
    coroutine.yield(false)
    effect_Wait(yukari.placeX, yukari.placeY,4)
    coroutine.yield(false)

    CharacterTurn(yukari,ASPECT_LEFT)    
    coroutine.yield(false)
    effect_Wait(yukari.placeX, yukari.placeY,4)
    coroutine.yield(false)

    CharacterTurn(yukari,ASPECT_UPLEFT)    
    coroutine.yield(false)
    effect_Wait(yukari.placeX, yukari.placeY,4)
    coroutine.yield(false)

    CharacterTurn(yukari,ASPECT_UP)    
    coroutine.yield(false)
    effect_Wait(yukari.placeX, yukari.placeY,60)
    coroutine.yield(false)
    
    storyEventMessageLoop(_T("soreha_"), str_map, nil)
    coroutine.yield(false)        
    clearMassage()
    effect_Wait(yukari.placeX, yukari.placeY,30)
    coroutine.yield(false)        
    fadeBlack(60)

    coroutine.yield(false)
    effect_Wait(yukari.placeX, yukari.placeY,120)
    coroutine.yield(false)

    CharacterForseGoAway(yukari)

    bg_black1:fadeOut(1)
    bg_black2:fadeOut(1)
--    cameraMove(meiling_cameraposX,meiling_cameraposY,10,1,1)
    coroutine.yield(false)

--[[    
    fadeOut(60)
    coroutine.yield(false)
    effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,60)
    coroutine.yield(false)
]]
    return true;
end

function lastBossAfter() 
    CharacterCureMental(PlayerCharacter())
    CharacterCurePhysical(PlayerCharacter())
    CharacterCureMagic(PlayerCharacter())
    CharacterCureSpeed(PlayerCharacter())
    eraseEnemys()
    -----last_After---------------------set_talk_studio-----------------------------------------------
    setHidingMiniMap(true)
    fadeWhite(30)
    coroutine.yield(false)
    effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,30)
    coroutine.yield(false)
    cameraMove(talkposX,talkposY,10,1,1)
    CharacterForseGoAway(boss)
    eraseFloorDropings()
    setHiddenInterface(true);
    --alice--goes-away
    local destBoss = LandformPlace(talkposX,talkposY-2)
    local destPC = LandformPlace(talkposX,talkposY)
    alice = MakeCharacter(4013, 0, CHARACTER_FORSE_ENEMY, destBoss)
    --moveWithAnimeToLand(alice,destBoss,false,10,false)
    moveWithAnimeToLand(PlayerCharacter(),destPC,false,10,false)
    coroutine.yield(false)
    CharacterTurn(alice,ASPECT_DOWN)
    CharacterTurn(PlayerCharacter(),ASPECT_UP)
    
    StopBGM()

    coroutine.yield(false)
    -----go_action-----------------------------------------------------------------------------------
    in_frame = 120    
    fadeOut(in_frame)
    coroutine.yield(false)
    effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,60)
    PlayBGM(_T(""),_T("talk3_main.wav"))   
    storyEventMessageLoop(_T("ED1_"), str_map, nil)
    coroutine.yield(false)        
    effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,60)
    clearMassage()
    coroutine.yield(false)
    --monologue--
    
    local posX =15
    local posY =24
    
    --bg_shrine = EventBackground(0,_T("event\\hakurei_back.png"), posX, posY, 2)
    
    --ED2(posX,posY,bg_shrine)---------------------------------------------------------------------------------------------------20140419-------------------------------------------
    ED2(posX,posY)
    clearMassage()
    coroutine.yield(false)

    effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,60)
    coroutine.yield(false) 
  
    --hakurei_event--
    PlayBGM(_T(""),_T("talk1_main.wav")) 

    posX =25
    posY =30
    bg_shrine = EventBackground(cEvent.PRIORITY_HIGH,_T("event\\hakurei_back.png"), posX, posY, 2)
    coroutine.yield(false)
    hakurei_event(posX,posY,bg_shrine); 
    storyEventMessageLoop(_T("ED3_"), str_map, nil)
    coroutine.yield(false)    

    clearMassage()
    fadeBlack(60)
    effect_Wait(posX,posY,60)
    coroutine.yield(false)
    effect_Wait(posX,posY,30)
    coroutine.yield(false)
    StopBGM()
    cameraMove(talkposX,talkposY,10,1,1)        
    coroutine.yield(false)
    effect_Wait(posX, posY,30)
    bg_shrine:fadeOut(10)
    fadeOut(30)
    PlayBGM(_T(""),_T("talk3_main.wav")) 
    coroutine.yield(false)

    --yamete--
    storyEventMessageLoop(_T("ED4_"), str_map, nil)
    coroutine.yield(false)

    --reimu_comes_in--
    local reimuLand = LandformPlace(talkposX+8,talkposY)
    local reimu = MakeCharacter(9019, 0, CHARACTER_FORSE_ENEMY, reimuLand)
    coroutine.yield(false)
    --moveWithAnime(reimu, ASPECT_LEFT, 3, false, 1, false)
    jumpWithAnime(reimu, ASPECT_LEFT, 5, 3, false, 1, false);
    

    coroutine.yield(false)
    turnCharacterForCharacter(PlayerCharacter(),reimu) 
    coroutine.yield(false)    

    storyEventMessageLoop(_T("ED5_"), str_map, nil)
    coroutine.yield(false)
    clearMassage()
    effect_Wait(talkposX,talkposY,30)   

    ---meiling_escapes--------------------------------------------------------
    moveWithAnime(PlayerCharacter(), ASPECT_LEFT, 1, true, 0.7, false)
    moveWithAnime(reimu, ASPECT_LEFT, 1, false, 0.7, false)
    coroutine.yield(false)
    effect_Wait(talkposX,talkposY,45)
    CharacterTurn(alice,ASPECT_LEFT)
    moveWithAnime(PlayerCharacter(), ASPECT_LEFT, 7, false, 1.5, false)
    moveWithAnime(reimu, ASPECT_LEFT, 7, false, 1.5, false)
    coroutine.yield(false)
--  effect_Wait(talkposX,talkposY,120)
    -----------------------------------------------------------
    sukima_eff(talkposX,talkposY,0,60)  
    effect_Wait(talkposX,talkposY,50)
    coroutine.yield(false)
    local yukari = MakeCharacter(9045, 0, CHARACTER_FORSE_ENEMY, LandformPlace(talkposX,talkposY))
    coroutine.yield(false)
    effect_Wait(talkposX,talkposY,30)
    coroutine.yield(false)   
    CharacterTurn(yukari,ASPECT_LEFT)
    coroutine.yield(false)
    effect_Wait(talkposX,talkposY,60)
    coroutine.yield(false)
    storyEventMessageLoop(_T("Negirai_"), str_map, nil)
    coroutine.yield(false)
    effect_Wait(talkposX,talkposY,30)
    coroutine.yield(false)
    clearMassage()
    coroutine.yield(false)    
    -----------------------------------------------------------
    fadeWhite(60)
    coroutine.yield(false)
    effect_Wait(talkposX,talkposY,100)   
    coroutine.yield(false)
    fadeBlack(60)
    PlayBGM(_T(""),_T("talk1_main.wav")) 
    effect_Wait(talkposX,talkposY,90)
    coroutine.yield(false)
    posX = 13
    local bg_black = black_back(posX,posY,60)
    cameraMove(posX,posY,10,1,1)    
    coroutine.yield(false)
    effect_Wait(talkposX,talkposY,120)
    coroutine.yield(false)
    storyEventMessageLoop(_T("ED6_"), str_map, nil)
    coroutine.yield(false)    
    clearMassage()
    CharacterForseGoAway(alice)
    local akyu = MakeCharacter(4008, 0, CHARACTER_FORSE_ENEMY, LandformPlace(posX,posY+1))    
    alice = MakeCharacter(4013, 0, CHARACTER_FORSE_ENEMY, LandformPlace(posX,posY-1))
    
    --akyu:setStopStamp(true)
    alice:setStopStamp(true)    
    fadeOut(60)
    effect_Wait(talkposX,talkposY,90)
    coroutine.yield(false)
    storyEventMessageLoop(_T("ED7_"), str_map, nil)
    coroutine.yield(false)
    clearMassage()
    coroutine.yield(false)
    storyEventMessageLoop(_T("ED8_"), str_map, nil)
    coroutine.yield(false)

    
    --set_field_for_staffroll--------------------------------------------------------
    clearMassage()
    coroutine.yield(false)
    
    --hide_characters--
    fadeBlack(120)
    StopBGM()
    effect_Wait(posX,posY,130)
    coroutine.yield(false)

    --set_position--    
    posX = 15
    posY = 15

    --erase_characters--
    CharacterForseGoAway(reimu)
    CharacterForseGoAway(akyu)
    CharacterForseGoAway(alice)
    coroutine.yield(false)
    
    bg_black:fadeOut(1)
    
    --move_meiling_and_camera--
    cameraMove(posX,posY,10,1,1)
    moveWithAnimeToLand(PlayerCharacter(),LandformPlace(posX+5,posY+2),false,10,false)
    coroutine.yield(false)
    
    --set_bg_black--
    bg_black = black_back(posX,posY,1)
    effect_Wait(posX,posY,20)
    coroutine.yield(false)
    
    --generate_casts--
    tewi = MakeCharacter(4013, 0, CHARACTER_FORSE_ENEMY, LandformPlace(posX+5,posY-1))
    alice = MakeCharacter(4009, 0, CHARACTER_FORSE_ENEMY, LandformPlace(posX+5,posY-2))
    coroutine.yield(false)
    
    --open--
    fadeOut(10)
    effect_Wait(posX,posY,20)
    coroutine.yield(false)    
    
    --skip_result_display--
    setClearResultSkipFlag(true);
    -------------------------------------------------------------------------

    local time_roll = staffRoll(posX,posY)
    local time_BGM = 60*81.5

    effect_Wait(posX,posY,time_BGM)
    coroutine.yield(false)
    StopBGM()
    coroutine.yield(false)
    effect_Wait(posX,posY,time_roll-time_BGM)
    coroutine.yield(false)

    effect_Wait(posX,posY,120) --yoin
    coroutine.yield(false)

    return true
end


function bossStartEvent()

    ------------------------common_initialization_for_boss_event----------
    bossposX = 20
    bossposY = 15
	bossLand = LandformPlace(bossposX,bossposY);
	pcLand = LandformPlace(PlayerCharacter().placeX,PlayerCharacter().placeY)
	str_map = map_tstring_StyleString()

    talkposX = (bossposX + PlayerCharacter().placeX)/2
    talkposY = (bossposY + PlayerCharacter().placeY)/2

    --------------------------------------------------------------------------
    
    --call_boss#4011:alice_mg/#5006:alice---    
    boss = MakeCharacter(5006, 0, CHARACTER_FORSE_ENEMY, bossLand)        
    if storyEventEnable() then
        lastBossBefore()
    else
        cameraMove(bossposX,bossposY,10,1,1)
        coroutine.yield(false)
        PlayBGM(_T("boss_no_event_intro.wav"),_T("boss_main.wav"))
        coroutine.yield(false)
        effect_Wait(bossposX,bossposY,60)
        cameraMove(PlayerCharacter().placeX,PlayerCharacter().placeY,3,1,1)
        coroutine.yield(false)        

        startBossSpell(boss)
    end	
    
	
	return true
end

function bossWillStartLastWard()
	local str_map = map_tstring_StyleString()

    cameraMove(boss.placeX,boss.placeY,10,1,1)    
    storyEventMessageLoop(_T("beforeLastWord1_"), str_map, nil)
    coroutine.yield(false)
    
    cameraMove(PlayerCharacter().placeX,PlayerCharacter().placeY,10,1,1)    
    storyEventMessageLoop(_T("beforeLastWord2_"), str_map, nil)
    coroutine.yield(false)
    
    storyEventMessageLoop(_T("beforeLastWord3_"), str_map, nil)
    coroutine.yield(false)
	return true
end


function bossEndEvent()
    --local boss = enemyPool["boss"];
    
    if storyEventEnable() then
        lastBossAfter()
    end
    coroutine.yield(false)
    goToNextFloor()
	return true
end


function bossWillStartLastWardEvent()
    --local boss = enemyPool["boss"];
    
    if storyEventEnable() then
        bossWillStartLastWard()
    end
    coroutine.yield(false)
	return true
end


--deligate method
function eventBossEnd(object)
	enemyPool["boss"] = getCharacterStorage("boss")
    stackDramaThread(coroutine.create(bossEndEvent))
end

--deligate method
function eventBossWillStartLastWard(object)
	enemyPool["boss"] = getCharacterStorage("boss")
    stackDramaThread(coroutine.create(bossWillStartLastWardEvent))
end

function startSpell(index)
	outputDebugString("startSpell_start\n")

end


function floorInitProcess()
	--outputDebugString("floorInitProcess_start\n")
	if floorLevel() == 30 then
        stackDramaThread(coroutine.create(bossStartEvent))
	end
end

function dungeonProcess()
	--outputDebugString("dungeonProcess_start\n")
end


function dungeonTurnProcess()
	--outputDebugString("dungeonTurnProcess_start\n")

end