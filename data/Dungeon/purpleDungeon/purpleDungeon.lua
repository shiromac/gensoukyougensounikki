enemyPool = {}

function purpleBossBefore()

    --------------------------set_talk_studio-----------------------------------------------
    setHidingMiniMap(true)
    fadeBlack(1)
    effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,5)    
    cameraMove(talkposX,talkposY,10,1,1)
    local destBoss = LandformPlace(talkposX,talkposY-2)
    local destPC = LandformPlace(talkposX,talkposY+3)
    local yuyuko = MakeCharacter(9011, 0, CHARACTER_FORSE_ENEMY, destBoss)
    --moveWithAnimeToLand(yuyuko,destBoss,false,10,false)
    moveWithAnimeToLand(PlayerCharacter(),destPC,false,10,false)
    CharacterTurn(yuyuko,ASPECT_DOWN)
    CharacterTurn(PlayerCharacter(),ASPECT_UP)

    PlayBGM(_T(""),_T("talk1_main.wav"))

    coroutine.yield(false)

    -----go_action-----------------------------------------------------------------------------------
    in_frame = 60
    fadeOut(in_frame)
    --effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,in_frame)
    
    destPC = LandformPlace(talkposX,talkposY)
    moveWithAnimeToLand(PlayerCharacter(),destPC,false,1,false)
    coroutine.yield(false)
    
	--storyEventMessageLoop(_T("purpleDungeonBossBefore1_"), str_map, nil)
	storyEventMessageLoopRange(_T("purpleDungeonBossBefore1_"), 1, 5, str_map, nil)
    coroutine.yield(false)
    StopBGM()
	storyEventMessageLoopRange(_T("purpleDungeonBossBefore1_"), 6, 6, str_map, nil)
    coroutine.yield(false)
    PlayBGM(_T(""),_T("talk2_main.wav"))
	moveWithAnime(yuyuko, ASPECT_RIGHT, 1, false, 0.3, true)
	turnCharacterForCharacter(PlayerCharacter(),yuyuko)
    coroutine.yield(false)		
	
    storyEventMessageLoop(_T("purpleDungeonBossBefore2_"), str_map, nil)
    coroutine.yield(false)
    cameraMoveParallel(PlayerCharacter().placeX,PlayerCharacter().placeY,0.07,0.5,0.5)
    moveWithAnime(yuyuko, ASPECT_DOWNRIGHT, 1, false, 0.3, true)
    turnCharacterForCharacter(PlayerCharacter(),yuyuko)
	coroutine.yield(false)
    moveWithAnime(yuyuko, ASPECT_DOWN, 1, false, 0.3, true)
    coroutine.yield(false)
    turnCharacterForCharacter(PlayerCharacter(),yuyuko)
    coroutine.yield(false)
    storyEventMessageLoop(_T("purpleDungeonBossBefore3_"), str_map, nil)
    coroutine.yield(false)
    cameraMoveParallel(PlayerCharacter().placeX,PlayerCharacter().placeY+2,0.07,0.5,0.5)
    moveWithAnime(yuyuko, ASPECT_DOWN, 2, false, 0.3, true)
    coroutine.yield(false)
    turnCharacterForCharacter(PlayerCharacter(),yuyuko)
    coroutine.yield(false)
    --moveWithAnime(yuyuko, ASPECT_DOWNLEFT, 1, false, 0.3, true)
    coroutine.yield(false)
    turnCharacterForCharacter(PlayerCharacter(),yuyuko)
    coroutine.yield(false)	
    --turnCharacterForCharacter(yuyuko,PlayerCharacter())
    --storyEventMessageLoop(_T("purpleDungeonBossBefore4_"), str_map, nil)
    storyEventMessageLoopRange(_T("purpleDungeonBossBefore4_"), 1, 2, str_map, nil)
    turnCharacterForCharacter(yuyuko,PlayerCharacter())
    storyEventMessageLoopRange(_T("purpleDungeonBossBefore4_"), 3, 3, str_map, nil)
    moveWithAnime(yuyuko, ASPECT_LEFT, 2, false, 0.3, true)
    coroutine.yield(false)
    turnCharacterForCharacter(PlayerCharacter(),yuyuko)
    turnCharacterForCharacter(yuyuko,PlayerCharacter())
    coroutine.yield(false)
    StopBGM()
    coroutine.yield(false)
    storyEventMessageLoop(_T("purpleDungeonBossBefore5_"), str_map, nil)
	PlayBGM(_T("talk_strain_intro.wav"),_T("talk_strain_main.wav"))
	coroutine.yield(false)
	
	--youmu_comes_in--
	--local yuyuko = boss
    youmu_pos = LandformPlace(PlayerCharacter().placeX,PlayerCharacter().placeY-4)
    --5004:youmu--
	youmu = MakeCharacter(9012, 0, CHARACTER_FORSE_ENEMY, youmu_pos)
	--boss = MakeCharacter(5002, 0, CHARACTER_FORSE_ENEMY, bossLand)
	coroutine.yield(false)
	cameraMove(PlayerCharacter().placeX,PlayerCharacter().placeY-2,0.5,0.75, 0.9)
	coroutine.yield(false)
    turnCharacterForCharacter(PlayerCharacter(),boss)
    coroutine.yield(false)
    --yuyuko_go_away--
	moveWithAnime(yuyuko, ASPECT_DOWN, 2, false, 1.0, true)
	coroutine.yield(false)
	CharacterForseGoAway(yuyuko)
	coroutine.yield(false)
	
    storyEventMessageLoop(_T("purpleDungeonBossBefore6_"), str_map, nil)
    --storyEventMessageLoopRange(_T("greenDungeonCloseEvent_"),2,6, str_map, nil)
    PlayBGM(_T("boss_intro.wav"),_T("boss_main.wav"))
    coroutine.yield(false)
    storyEventMessageLoop(_T("purpleDungeonBossBefore7_"), str_map, nil)
    coroutine.yield(false)
    fadeWhite(30)
    --effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,30)
    
    --jumpWithAnimeToLand(boss,bossLand,1.0,true,0.3,false)
    CharacterForseGoAway(youmu)
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
end--purpleBossBefore---

function purpleBossAfter()
    CharacterCureMental(PlayerCharacter())
    CharacterCurePhysical(PlayerCharacter())
    CharacterCureMagic(PlayerCharacter())
    CharacterCureSpeed(PlayerCharacter())
    eraseEnemys()
    -----purple_After---------------------set_talk_studio-----------------------------------------------
    fadeWhite(30)
    setHidingMiniMap(true)
    eraseFloorDropings()
    coroutine.yield(false)
    effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,30)
    cameraMove(talkposX,talkposY,10,1,1)
    local youmu_pos = LandformPlace(talkposX,talkposY-2)
    local youmu = MakeCharacter(9012, 0, CHARACTER_FORSE_ENEMY, youmu_pos)
    --moveWithAnimeToLand(boss,destBoss,false,10,false)
    CharacterForseGoAway(boss)
    local destPC = LandformPlace(talkposX,talkposY)
    moveWithAnimeToLand(PlayerCharacter(),destPC,false,10,false)
    coroutine.yield(false)
    CharacterTurn(youmu,ASPECT_DOWN)
    CharacterTurn(PlayerCharacter(),ASPECT_UP)
    
    PlayBGM(_T(""),_T("talk1_main.wav"))
    coroutine.yield(false)
    -----go_action-----------------------------------------------------------------------------------
    in_frame = 60    
    fadeOut(in_frame)
    
    local yuyuko_pos = LandformPlace(PlayerCharacter().placeX+7,PlayerCharacter().placeY)
    local yuyuko = MakeCharacter(9011, 0, CHARACTER_FORSE_ENEMY, yuyuko_pos)
    local aya_pos = LandformPlace(PlayerCharacter().placeX-10,PlayerCharacter().placeY-10)
    local aya = MakeCharacter(9018, 0, CHARACTER_FORSE_ENEMY, aya_pos)

    moveWithAnime(yuyuko, ASPECT_LEFT, 5, false, 0.5, true)
    coroutine.yield(false)
    CharacterTurn(yuyuko,ASPECT_UPLEFT)
    CharacterTurn(youmu,ASPECT_DOWNRIGHT)
    coroutine.yield(false)
    --storyEventMessageLoop(_T("purpleDungeonAfterBoss1_"), str_map, nil)
    storyEventMessageLoopRange(_T("purpleDungeonAfterBoss1_"), 1, 6, str_map, nil)
    coroutine.yield(false)
    jumpWithAnime(youmu,ASPECT_DOWN,0, 0.5, true, 1, false);
    coroutine.yield(false)    
    storyEventMessageLoopRange(_T("purpleDungeonAfterBoss1_"), 7, 7, str_map, nil)
    coroutine.yield(false)
    CharacterTurn(youmu,ASPECT_DOWN) 
    coroutine.yield(false)
    storyEventMessageLoopRange(_T("purpleDungeonAfterBoss1_"), 8, 11, str_map, nil)
    coroutine.yield(false)    
    jumpWithAnime(PlayerCharacter(),ASPECT_UP,0, 0.5, true, 1, false);
    coroutine.yield(false)
    storyEventMessageLoopRange(_T("purpleDungeonAfterBoss1_"), 12,13 , str_map, nil)
    coroutine.yield(false)    
    CharacterTurn(PlayerCharacter(),ASPECT_RIGHT)    
    coroutine.yield(false)    
    storyEventMessageLoopRange(_T("purpleDungeonAfterBoss1_"), 14,14, str_map, nil)
    coroutine.yield(false)
    CharacterTurn(PlayerCharacter(),ASPECT_UP)
    coroutine.yield(false)
    storyEventMessageLoopRange(_T("purpleDungeonAfterBoss1_"), 15,17, str_map, nil)
    coroutine.yield(false)
    CharacterTurn(yuyuko,ASPECT_LEFT)
    coroutine.yield(false)
    storyEventMessageLoopRange(_T("purpleDungeonAfterBoss1_"), 18,18, str_map, nil)
    coroutine.yield(false)
    CharacterTurn(youmu,ASPECT_DOWNRIGHT)
    coroutine.yield(false)
    storyEventMessageLoopRange(_T("purpleDungeonAfterBoss1_"), 19,19, str_map, nil)
    coroutine.yield(false)
    CharacterTurn(yuyuko,ASPECT_UPLEFT)
    coroutine.yield(false)
    storyEventMessageLoopRange(_T("purpleDungeonAfterBoss1_"), 20,21, str_map, nil)
    coroutine.yield(false)        
    CharacterTurn(yuyuko,ASPECT_LEFT)
    CharacterTurn(PlayerCharacter(),ASPECT_LEFT)
    moveWithAnime(youmu, ASPECT_LEFT, 7, false, 1.0, true)

    coroutine.yield(false)
    CharacterForseGoAway(youmu)
    coroutine.yield(false)
    CharacterTurn(PlayerCharacter(),ASPECT_RIGHT)
    storyEventMessageLoop(_T("purpleDungeonAfterBoss2_"), str_map, nil)
    --clearMassage()
    coroutine.yield(false)
    CharacterTurn(yuyuko,ASPECT_DOWN)
    moveWithAnime(PlayerCharacter(), ASPECT_DOWN, 6, false, 1, true)
    clearMassage()
    coroutine.yield(false)    
    CharacterTurn(aya,ASPECT_DOWNRIGHT)
    cameraMove(aya.placeX,aya.placeY,0.5,0.5,0.9)
    coroutine.yield(false)
    storyEventMessageLoop(_T("purpleDungeonAfterBoss3_"), str_map, nil)
    coroutine.yield(false)
    setHidingMiniMap(false)
    goToNextFloor()
    
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
    
    --call_boss#5004:youmu---
    boss = MakeCharacter(5004, 0, CHARACTER_FORSE_ENEMY, bossLand)    
    --stackDramaThread(coroutine.create(purpleBossBefore))
    if storyEventEnable() then
        purpleBossBefore()
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


function bossEndEvent()
    --local boss = enemyPool["boss"];
    
    if storyEventEnable() then
        purpleBossAfter()
    end
    coroutine.yield(false)
    goToNextFloor()
	return true
end



--deligate method
function eventBossEnd(object)
	enemyPool["boss"] = getCharacterStorage("boss")
    stackDramaThread(coroutine.create(bossEndEvent))
end

function startSpell(index)
	outputDebugString("startSpell_start\n")

end


function floorInitProcess()
	--outputDebugString("floorInitProcess_start\n")
	if floorLevel() == 15 then
        stackDramaThread(coroutine.create(bossStartEvent))
	end
end

function dungeonProcess()
	--outputDebugString("dungeonProcess_start\n")
end


function dungeonTurnProcess()
	--outputDebugString("dungeonTurnProcess_start\n")

end