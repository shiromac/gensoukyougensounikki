enemyPool = {}

function bossStartEvent()
	local bossLand = LandformPlace(20,15);
	--local boss = MakeCharacter(5000, 0, CHARACTER_FORSE_ENEMY, bossLand)
	
	
	
	--startBossSpell(boss)
	return true
end


function bossEndEvent()
    local boss = enemyPool["boss"];
    

	goToNextFloor()
	return true
end

--delegate
function eventBossEnd(object)
	local boss_sender = getCharacterStorage("boss")
	--if(boss_sender: ID() == 5000)then--remilia
	    enemyPool["boss"] = boss_sender
	    --bossEndEvent()
        stackDramaThread(coroutine.create(bossEndEvent))
	--end
end

--requisite
function floorInitProcess()
	--outputDebugString("floorInitProcess_start\n")
	
	CharacterDisguise(PlayerCharacter(), 1000)

	if floorLevel() == 1 then
        bossStartEvent()
        --stackDramaThread(coroutine.create(bossStartEvent))
	end
	return
end

--requisite
function dungeonProcess()
	--outputDebugString("dungeonProcess_start\n")
end

--requisite
function dungeonTurnProcess()
	--outputDebugString("dungeonTurnProcess_start\n")

end