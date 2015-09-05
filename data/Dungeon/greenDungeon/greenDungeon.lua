enemyPool = {}

function greenBossBefore()
    --------------------------set_talk_studio-----------------------------------------------
    fadeBlack(5)
    setHidingMiniMap(true)
    effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,5)
    cameraMove(talkposX,talkposY,10,1,1)
    local destBoss = LandformPlace(talkposX,talkposY-2)
    local destPC = LandformPlace(talkposX,talkposY+3)
    local kanako = MakeCharacter(9067, 0, CHARACTER_FORSE_ENEMY, destBoss)
--    moveWithAnimeToLand(boss,destBoss,false,10,false)
    moveWithAnimeToLand(PlayerCharacter(),destPC,false,10,false)

    PlayBGM(_T(""),_T("talk1_main.wav"))

    coroutine.yield(false)

    -----go_action-----------------------------------------------------------------------------------
    in_frame = 60
    fadeOut(in_frame)
    --effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,in_frame)
    
    destPC = LandformPlace(talkposX,talkposY)
    moveWithAnimeToLand(PlayerCharacter(),destPC,false,1,false)
    coroutine.yield(false)
	storyEventMessageLoop(_T("greenDungeonBossBefore1_"), str_map, nil)
	PlayBGM(_T("talk_strain_intro.wav"),_T("talk_strain_main.wav"))
	coroutine.yield(false)
    --storyEventMessageLoop(_T("greenDungeonBossBefore2_"), str_map, nil)
    storyEventMessageLoopRange(_T("greenDungeonBossBefore2_"),1,5, str_map, nil)
    coroutine.yield(false)
    PlayBGM(_T("boss_intro.wav"),_T("boss_main.wav"))
    coroutine.yield(false)
    storyEventMessageLoopRange(_T("greenDungeonBossBefore2_"),6,7, str_map, nil)
    coroutine.yield(false)

    fadeWhite(30)
    --effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,30)
    
    --jumpWithAnimeToLand(boss,bossLand,1.0,true,0.3,false)
    jumpWithAnimeToLand(PlayerCharacter(),pcLand,1.0,true,0.3,false)
    cameraMove(PlayerCharacter().placeX,PlayerCharacter().placeY,1,1,1)

    coroutine.yield(false)
    
    CharacterForseGoAway(kanako)
    
    in_frame = 30
    fadeOut(in_frame)
    effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,in_frame)

    setHidingMiniMap(false)
    coroutine.yield(false)        
    startBossSpell(boss)
    return true
end--greenBossBefore---

function greenBossAfter()
    CharacterCureMental(PlayerCharacter())
    CharacterCurePhysical(PlayerCharacter())
    CharacterCureMagic(PlayerCharacter())
    CharacterCureSpeed(PlayerCharacter())
    eraseEnemys()
    -----green_After---------------------set_talk_studio-----------------------------------------------
    fadeWhite(30)
    coroutine.yield(false)
    eraseFloorDropings()
    setHidingMiniMap(true)
    effect_Wait(PlayerCharacter().placeX, PlayerCharacter().placeY,30)
    cameraMove(talkposX,talkposY,10,1,1)
    CharacterForseGoAway(boss)
    
    local destBoss = LandformPlace(talkposX,talkposY-2)
    local destPC = LandformPlace(talkposX,talkposY)
    local kanako = MakeCharacter(9067, 0, CHARACTER_FORSE_ENEMY, destBoss)
    moveWithAnimeToLand(PlayerCharacter(),destPC,false,10,false)
    coroutine.yield(false)
    CharacterTurn(boss,ASPECT_DOWN)
    CharacterTurn(PlayerCharacter(),ASPECT_UP)
    
    PlayBGM(_T(""),_T("talk1_main.wav"))
    coroutine.yield(false)
    -----go_action-----------------------------------------------------------------------------------
    in_frame = 60    
    fadeOut(in_frame)
    storyEventMessageLoop(_T("greenDungeonAfterBoss_"), str_map, nil)
    
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
    
    --call_boss#5003:kanako---
    boss = MakeCharacter(5003, 0, CHARACTER_FORSE_ENEMY, bossLand)    
    --stackDramaThread(coroutine.create(greenBossBefore))
    if storyEventEnable() then
        greenBossBefore()
    else
        cameraMove(bossposX,bossposY,10,1,1)
        coroutine.yield(false)
        PlayBGM(_T("boss_no_event_intro.wav"),_T("boss_main.wav"))
        CharacterTurn(PlayerCharacter(),ASPECT_UP)
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
        greenBossAfter()
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