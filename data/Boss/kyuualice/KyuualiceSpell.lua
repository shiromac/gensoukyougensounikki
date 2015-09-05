-- cCharacter boss: boss character

enemyPool = {}
landPool = {}


function giveUpEvent()
    effect_boss_defeat(boss.placeX, boss.placeY)
	addCharacterStorage("boss",boss)
	eventNotification("eventBossEnd","","boss")
	
end

function startSpell(index)
	outputDebugString("startSpell_start\n")
	if(index == 1)then

	end
end

function endSpell(index)
	if(index == 3)then
	    addCharacterStorage("boss",boss)
	    eventNotification("eventBossWillStartLastWard","","boss")
	end
end