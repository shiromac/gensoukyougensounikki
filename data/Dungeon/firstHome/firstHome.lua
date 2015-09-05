
enemyPool = {}
--[[
function marisaAppear()
	emeny = MakeCharacter(2001, 0, CHARACTER_FORSE_ENEMY, LandformPlace(20,17))
	enemyPool["marisa"] = emeny
end

function ninaAppear()

	emeny = MakeCharacter(2001, 0, CHARACTER_FORSE_FRIEND, LandformPlace(20,17))
	enemyPool["hina"] = emeny
end
]]
function GyougiEvent()
    local keibimeido = nil
    vec_chara = allCharacterList()
    charalistsize = vec_chara: size()
    for i = 0, charalistsize-1 do
        if(vec_chara:at(i):ID() == 4000) then
            --keibi meido
            keibimeido = vec_chara:at(i)
            break
        end
    end
    turnCharacterForCharacter(keibimeido,PlayerCharacter())
    cameraMove(keibimeido.placeX, keibimeido.placeY, 1.5,0.5,1)
    clearMassage()
    str_map = map_tstring_StyleString()
    utility_storyMessage(_T("homeOtherGyougiEvent1"),str_map)
    messageWait()
    clearMassage()
    cameraMove(PlayerCharacter().placeX, PlayerCharacter().placeY, 1.5,0.5,1)
end
function floorInitProcess()
	--outputDebugString("firstHome_floorInitProcess_start\n")

	vec_item = allDropingList()
	itemsize = vec_item: size()
	
	outputDebugValue(itemsize)
	for i = 0,itemsize-1 do
		--outputDebugValue(vec_item:at(i): ID())
		if( vec_item:at(i): ID() == 11002) then
			--outputDebugString("vec_item\n")
			--exit
			DropingBreak(vec_item:at(i))
		end
	end
	
	MakeDroping(11002, LandformPlace(10,9))

	--outputDebugString("point 5\n")


    if( getMapValue(savable_globalFlags(),_T("EventDidFlag:homeMarisaAppearing"),NO) == YES )then
		MakeCharacter(4002, 0, CHARACTER_FORSE_OTHER, LandformPlace(11,15))
    end
    if( getMapValue(savable_globalFlags(),_T("EventDidFlag:homeHinaAppearing"),NO) == YES )then
		MakeCharacter(4001, 0, CHARACTER_FORSE_OTHER, LandformPlace(13,10))
    end
    if( getMapValue(savable_globalFlags(),_T("EventDidFlag:homeNitoriAppearing"),NO) == YES )then
		MakeCharacter(4005, 0, CHARACTER_FORSE_OTHER, LandformPlace(8,16))
    end
    if( getMapValue(savable_globalFlags(),_T("EventDidFlag:homeAkyuAppearing"),NO) == YES )then
		MakeCharacter(4008, 0, CHARACTER_FORSE_OTHER, LandformPlace(18,13))
    end
	--MakeCharacter(4008, 0, CHARACTER_FORSE_OTHER, LandformPlace(18,13))
	if not(PlayerCharacter(): ID() == 1002)then
	    MakeCharacter(1002, 0, CHARACTER_FORSE_FRIEND, LandformPlace(14,17))
	end
	if not(PlayerCharacter(): ID() == 1003) and getMapValue(savable_globalFlags(),_T("CharacterPlayableFlag:Tewi"),NO) == YES  then
	    MakeCharacter(1003, 0, CHARACTER_FORSE_FRIEND, LandformPlace(15,17))
	end
	if not(PlayerCharacter(): ID() == 1004) and getMapValue(savable_globalFlags(),_T("CharacterPlayableFlag:Youmu"),NO) == YES then
	    MakeCharacter(1004, 0, CHARACTER_FORSE_FRIEND, LandformPlace(16,17))
	end
	--[[
	if isUnRelease() then
		if not(PlayerCharacter(): ID() == 1002)then
	        MakeCharacter(1002, 0, CHARACTER_FORSE_FRIEND, LandformPlace(14,17))
        end
        if not(PlayerCharacter(): ID() == 1003)then
            MakeCharacter(1003, 0, CHARACTER_FORSE_FRIEND, LandformPlace(15,17))
        end
	end
	]]
    return

end

function dungeonProcess()
	--outputDebugString("dungeonProcess_start\n")
end

countForGyougiEvent = 0
function dungeonTurnProcess()
	--outputDebugString("dungeonTurnProcess_start\n")

    if(PlayerCharacter(): ID() == 1002 and PlayerCharacter().placeX == 10 and PlayerCharacter().placeY == 14)then
        countForGyougiEvent = countForGyougiEvent + 1
        if(countForGyougiEvent >= 3 and random() > 0.7)then
            GyougiEvent()
            countForGyougiEvent = -20
        end
    end
end

