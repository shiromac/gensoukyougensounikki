g_droping_map = {}

g_boss = nil

function bossStartEvent()

    bossposX = g_boss.placeX
    bossposY = g_boss.placeY
	cameraMove(bossposX,bossposY,10,1,1)
    PlayBGM(_T("boss_no_event_intro.wav"),_T("boss_main.wav"))
    CharacterTurn(PlayerCharacter(),ASPECT_UP)
    effect_Wait(bossposX,bossposY,60)
    cameraMove(PlayerCharacter().placeX,PlayerCharacter().placeY,3,1,1)

    startBossSpell(g_boss)
	return true
end
function bossEndEvent()

    MakeDroping(10000, LandformRandom_RoomRandom_Safe())
    goToNextFloor()
	return true
end

--deligate method
function eventBossEnd(object)
	local boss = getCharacterStorage("boss")
    stackDramaThread(coroutine.create(bossEndEvent))
end

function startSpell(index)
	outputDebugString("startSpell_start\n")

end

function floorInitProcess()
	--outputDebugString("floorInitProcess_start\n")

	if floorLevel() % 2 == 1 then
        MakeDroping(10000, LandformRandom_RoomRandom_Safe())
	end
	
	if floorLevel() == 2 then
        g_boss = MakeCharacter(5007, 0, CHARACTER_FORSE_ENEMY, LandformPlace(20,10))
        bossStartEvent()
	elseif floorLevel() == 4 then
        g_boss = MakeCharacter(5009, 0, CHARACTER_FORSE_ENEMY, LandformPlace(20,10))
        bossStartEvent()
	elseif floorLevel() == 6 then
        g_boss = MakeCharacter(5010, 0, CHARACTER_FORSE_ENEMY, LandformPlace(20,10))
        bossStartEvent()
	elseif floorLevel() == 8 then
        g_boss = MakeCharacter(5011, 0, CHARACTER_FORSE_ENEMY, LandformPlace(20,10))
        bossStartEvent()
	elseif floorLevel() == 10 then
        g_boss = MakeCharacter(5013, 0, CHARACTER_FORSE_ENEMY, LandformPlace(20,10))
        bossStartEvent()
	end
	
	if floorLevel() == 11 then
	    local item1 = MakeDroping(7001, LandformRandom_RoomRandom_Safe())
	    local item2 = MakeDroping(7021, LandformRandom_RoomRandom_Safe())
	    DropingQualityForsedMakeValue(item1,10)
	    DropingQualityForsedMakeValue(item2,10)
	end
	
	return

end

function dungeonProcess()
	--outputDebugString("dungeonProcess_start\n")
end

function dungeonTurnProcess()
	
end

