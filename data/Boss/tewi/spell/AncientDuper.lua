-- cCharacter boss: boss character
-- value spellTurnCount
-- value spellRestTurn
-- value spellTurnLimitCount
-- value ValiableConstant1
-- value ValiableConstant2
-- value ValiableConstant3
-- value ValiableConstant4

action_no = 0
action_danmaku = 1
action_hakkyo_danmaku = 2
action_kakutou = 3
action_hakkyo_kakutou = 4
action_nomove_tame = 5
action_nomove_koutyoku = 6

imageColor1 = Color(255,255,128,128)
imageColor2 = Color(255,255,0,0)

function kawawatariAnime(plandlist)

    local first_place = Coordinate(boss.placeX, boss.placeY)
    local now_place = Coordinate(0, 0)
    hab = AnimeChip_Hab(PositionPerGrid(first_place.x, first_place.y, 0))
    
    jump_chip_life = 16
    local size = table.maxn(plandlist)
    local chara_anime = AnimeChip_Chara(boss,jump_chip_life*size)
    hab:addAnimeChip(chara_anime)

    for i = 1, size do
        local next_place = Coordinate(plandlist[i].place.x, plandlist[i].place.y) - first_place
        chara_anime.position: addCascade(PositionPerGrid(now_place.x, now_place.y, 0), PositionPerGrid((now_place.x+next_place.x)/2, (now_place.y+next_place.y)/2, 2), PositionPerGrid(next_place.x, next_place.y, 0), jump_chip_life)
        -- sound
        local sound = AnimeChip_Sound("jump.wav",PositionPerGrid(plandlist[i].place.x,plandlist[i].place.y,0),1.0)
        hab:addAnimeChip(sound)
        sound: setDelay(jump_chip_life*i)
        now_place = next_place
    end

    setAnimation_wait(hab)

end
function kawawatariDo()
    local you_place = Coordinate(boss.placeX, 25)
    local plandlist = {}
    local aspect = nil
    local pland = nil
    if(boss.placeY > MAPHEIGHT/2)then
        aspect = ASPECT_UP
        pland = LandformPlace(you_place.x, you_place.y)
    end
    local you_place = Coordinate(boss.placeX, 15)
    if(boss.placeY <= MAPHEIGHT/2)then
        aspect = ASPECT_DOWN
        pland = LandformPlace(you_place.x, you_place.y)
    end
    local count = 1
    local result = nil
    while true do
        local findflag = false
        if(LandformNearby(pland, aspect, 1):attribute() == LANDFORM_ATTRIBUTE_FLOOR)then
            pland = LandformNearby(pland, aspect, 1)
            plandlist[count] = pland          
            result = 1
            -- success
            break
        end
        if(LandformNearby(pland, aspect, 1).mountedCharacter)then
            pland = LandformNearby(pland, aspect, 1)
            findflag = true
        end
        if(LandformNearby(pland, aspect+1, 1).mountedCharacter)then
            pland = LandformNearby(pland, aspect+1, 1)
            findflag = true
        end
        if(LandformNearby(pland, aspect-1, 1).mountedCharacter)then
            pland = LandformNearby(pland, aspect-1, 1)
            findflag = true
        end
        if(findflag == false)then
            pland = LandformNearby(pland, aspect, 1)
            plandlist[count] = pland          
            result = 0
            -- fail
            break
        end
        plandlist[count] = pland;
        count = count + 1
        if(count > 50)then
            break
        end
    end
    
    if(result)then
        CharacterTurn(boss, aspect)
        kawawatariAnime(plandlist)

        if(result == 1)then
            pland = getSafeSetCharacterLandform(pland, boss)
            CharacterWarp(boss, pland)
			str_map = map_tstring_StyleString()
			str_map:setValue(_T("Chara"), boss:fullName() )
			utility_localizedMessage(_T("Boss_Tewi_AncientDuper_success"),str_map)
        elseif(result == 0)then
            CharacterForsedDamage(boss, boss.maxHP* 0.35)
            str_map = map_tstring_StyleString()
            str_map:setValue(_T("Chara"), boss:fullName() )
			utility_localizedMessage(_T("Boss_Tewi_AncientDuper_fail"),str_map)
        end
        
        local size = table.maxn(plandlist)
        for i = 1, size do
            if(plandlist[i].mountedCharacter)then
                CharacterForsedDamage(plandlist[i].mountedCharacter, 10)
                coroutine.yield(false)
            end
        end
        
    end
    return true;
end
function kawawatariJudge()
    local you_place = Coordinate(boss.placeX, 25)
    if(boss.placeY == you_place.y)then
        local pland = LandformPlace(you_place.x, you_place.y)
        if(LandformNearby(pland,ASPECT_UP,1).mountedCharacter) and (LandformNearby(pland,ASPECT_UP,2).mountedCharacter)then
            return true;
        end
        local pland = LandformPlace(you_place.x-1, you_place.y)
        if(LandformNearby(pland,ASPECT_UP,1).mountedCharacter) and (LandformNearby(pland,ASPECT_UP,2).mountedCharacter)then
            return true;
        end
        local pland = LandformPlace(you_place.x+1, you_place.y)
        if(LandformNearby(pland,ASPECT_UP,1).mountedCharacter) and (LandformNearby(pland,ASPECT_UP,2).mountedCharacter)then
            return true;
        end
    end
    local you_place = Coordinate(boss.placeX, 15)
    if(boss.placeY == you_place.y)then
        local pland = LandformPlace(you_place.x, you_place.y)
        if(LandformNearby(pland,ASPECT_DOWN,1).mountedCharacter) and (LandformNearby(pland,ASPECT_DOWN,2).mountedCharacter)then
            return true;
        end
        local pland = LandformPlace(you_place.x+1, you_place.y)
        if(LandformNearby(pland,ASPECT_DOWN,1).mountedCharacter) and (LandformNearby(pland,ASPECT_DOWN,2).mountedCharacter)then
            return true;
        end
        local pland = LandformPlace(you_place.x-1, you_place.y)
        if(LandformNearby(pland,ASPECT_DOWN,1).mountedCharacter) and (LandformNearby(pland,ASPECT_DOWN,2).mountedCharacter)then
            return true;
        end
    end

    return false
end


--valueFunction = function(Landform)
function selectMovePlace(chara, valueFunction)
    local value = {}
    local state = {}
    local lotate = random()*8
    for a = 0, 7 do
        if(CharacterCanMove(chara,a))then
            state[a] = LandformNearby(LandformCharacterPosition(chara), a+lotate, 1)
            value[a] = valueFunction(state[a])
        end
    end
    a = 8
    state[a] = LandformCharacterPosition(chara)
    value[a] = valueFunction(state[a])
 
    local maxvalue = nil
    local action = nil
    for a = 0, 8 do
        if(value[a])then
            if(maxvalue == nil or maxvalue < value[a])then
                maxvalue = value[a]
                action = a
            end
        end
    end
    
    if(action)then
        return state[action]
    end
    return LandformEdge()
end

function placeValue(land)
    local charalist = allCharacterList()
    local size = charalist: size()
    local value = 0;
    for i = 0, size -1 do
        local enemy = charalist: at(i)
        if(isEnemyCharacter(boss, enemy))then
            local me_place = Coordinate(land.place.x, land.place.y)
            local you_place = Coordinate(enemy.placeX, enemy.placeY)
            if((me_place-you_place):dif() <= 1 and not enemy:isEqualTo(boss))then
                value = value-10
                if(enemy:isEqualTo(PlayerCharacter()))then
                    value = value-10
                end
            end
        end
    end
    
    if(isSafeSetCharacterLandform(land) == false and not land:isEqualTo(LandformCharacterPosition(boss)))then
        value = value - 30
    end
    local me_place = Coordinate(land.place.x, land.place.y)
    if(PlayerCharacter().placeY > MAPHEIGHT/2)then
        local you_place = Coordinate(MAPWIDTH/2, 25)
        value = value - (me_place - you_place): dif()*2
        value = value - me_place.y
    else
        local you_place = Coordinate(MAPWIDTH/2, 15)
        value = value - (me_place - you_place): dif()*2
        value = value + me_place.y
    end
    return value
end


function selectAction(turn)
    if(spellRestTurn / spellTurnLimitCount > 0.3)then
        turn_amari = turn % 10
        if(turn_amari == 3)then
            return action_nomove_tame
        elseif(turn_amari == 4)then
            return action_kakutou
        elseif(turn_amari == 5)then
            return action_nomove_koutyoku
        elseif(turn_amari == 1)then
            return action_danmaku
        end
    else
        --hakkyo
        turn_amari = turn % 8
        if(turn_amari == 2 or turn_amari == 4)then
            return action_nomove_tame
        elseif(turn_amari == 3 or turn_amari == 5)then
            return action_hakkyo_kakutou
        elseif(turn_amari == 7)then
            return action_hakkyo_danmaku
        end
    end
    return action_no;
end

function bossMoveTargetLandform(pland)
    --outputDebugString("bossMoveTargetLandform");
    local me_place = Coordinate(boss.placeX, boss.placeY)
    local you_place = Coordinate(PlayerCharacter().placeX, PlayerCharacter().placeY)
    if(me_place.y > MAPHEIGHT/2)then
        if(you_place.y > MAPHEIGHT/2)then
            return selectMovePlace(boss, placeValue);
        else
            return LandformEdge()
        end
    else
        if(you_place.y > MAPHEIGHT/2)then
            return LandformEdge()
        else
            return selectMovePlace(boss, placeValue);
        end
    end
    return selectMovePlace(boss, placeValue);
end

function AISkillChoise(valiable)
    --outputDebugString("AISkillChoise");
    local me_place = Coordinate(boss.placeX, boss.placeY)
    local you_place = Coordinate(PlayerCharacter().placeX, PlayerCharacter().placeY)
    if(me_place.y > MAPHEIGHT/2)then
        if(you_place.y > MAPHEIGHT/2)then
            didAction = false
            stackDramaThread(coroutine.create( function()
                if(kawawatariJudge())then
                    stackDramaThread(coroutine.create( kawawatariDo))
                    coroutine.yield(false)
                end
                return true;
            end))
        end
    else
        if(you_place.y < MAPHEIGHT/2)then
            stackDramaThread(coroutine.create( function()
                if(kawawatariJudge())then
                    stackDramaThread(coroutine.create( kawawatariDo))
                    coroutine.yield(false)
                end
                return true;
            end))
        end
    end
    action = selectAction(spellTurnCount)
    if (action ~= action_no) then
        return YES;
    else
        return NO;
    end
end

function skillEffect(valiable)
    --outputDebugString("skillEffect");

    action = selectAction(spellTurnCount)
    if action == action_danmaku then
        return YES;
    elseif action == action_kakutou then
        kakutou_setting(valiable,false);
        return YES;
    elseif action == action_hakkyo_kakutou then
        kakutou_setting(valiable,true);
        return YES;
    elseif action == action_nomove_tame then
        effect_boss_charge(boss.placeX,boss.placeY,imageColor1,imageColor2);
        return NO;--special
    elseif action == action_nomove_koutyoku then
        return NO;--special
    end
    
    return YES;
end

function passiveSkill(timing,valiable)
    --outputDebugString("passiveSkill");
    if(timing == Timing.action_end)then

    end
end

function putUpBarrage()
    --outputDebugString("putUpBarrage");
    action = selectAction(spellTurnCount)
    if (action == action_danmaku or action == action_hakkyo_danmaku) then
        danmaku_setting()
    end
end

trapSpring = {}

function startSpell()
    --outputDebugString("startSpell");
    for i = 1, ValiableConstant4 do
        spawnFloorNativeItem(LandformRandom_Random())
    end
    for x = 8, 32 do
        for y = 16, 24 do
            mapTransformForseLandChange(LandformPlace(x,y),LANDFORM_ATTRIBUTE_WATER)
        end
    end
    mapTransformArrangement()
    trapSpring[0] = MakeDroping(9001, LandformPlace(20,8))
    trapSpring[1] = MakeDroping(9001, LandformPlace(20,32))
    
    	
	effect_Smoke1(boss.placeX, boss.placeY)
	local bossland = LandformPlace(20,8+4)
	stackFunction(function(enemy, land) CharacterWarp(enemy, land) end, boss, bossland)
	effect_Smoke1(bossland.place.x, bossland.place.y)

end

function endSpell()
    --outputDebugString("endSpell");
    for x = 8, 32 do
        for y = 16, 24 do
            mapTransformForseLandChange(LandformPlace(x,y),LANDFORM_ATTRIBUTE_FLOOR)
        end
    end
    mapTransformArrangement()
    DropingBreak(trapSpring[0])
    DropingBreak(trapSpring[1])
end

-----------------------------------------------------------------------
function danmaku_setting()
    effect_boss_focus(boss.placeX, boss.placeY);
    
    --enemy spown
    enemy = spawnFloorNativeEnemy(LandformRandom_RoomRandom_OutOfSight_Safe(LandformCharacterPosition(PlayerCharacter())),CHARACTER_FORSE_ENEMY)
    if(enemy)then
        CharacterCureMental(enemy)
    end
    
    for k = 1, ValiableConstant3 do
        local trap_place = LandformRandom_Random()
        effect_Smoke1(trap_place.place.x, trap_place.place.y)
        trap = spawnFloorNativeTrap(trap_place)
        if trap then
            trap: discover()
        end
    end
    
    
    for i = 0, MAPHEIGHT-1 do

        aspect = ASPECT_LEFT + (i%2)*4;
        pland = landformSearchObstacleWithRange(LandformPlace(MAPWIDTH/2,i), aspect+4, MAPWIDTH,true,false,false,false);
        pland = LandformNearby(pland,aspect,1);

        if(isSafeSetCharacterLandform(pland) or pland: attribute() == LANDFORM_ATTRIBUTE_WATER)then
        
            effect_Smoke1(pland.place.x, pland.place.y)

            color = BulletColor.orange;
            type = bulletType.straight;
            speed = 1
            
            if(pland: attribute() == LANDFORM_ATTRIBUTE_WATER)then
            
                color = BulletColor.cyan;
                type = bulletType.straight;
                speed = 0
            end

            bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, speed, type, color, aspect);
        end


    end
        
end

function kakutou_setting(valiable, isHakkyou)
    --effect
    pland = LandformCharacterPosition(PlayerCharacter());
    if(pland)then
        coo = Coordinate(boss.placeX - pland.place.x, boss.placeY - pland.place.y)
        
        effect_boss_emission(boss.placeX, boss.placeY,imageColor1,imageColor2);
    
        if(coo:dif() <= 1)then
            local me_place = Coordinate(boss.placeX , boss.placeY)
            local you_place = nil
            if(me_place.y > MAPHEIGHT/2)then
                you_place = Coordinate(MAPWIDTH/2, 25)
            else
                you_place = Coordinate(MAPWIDTH/2, 15)
            end
            local aspect = (you_place - me_place):getAspect()
            CharacterThrow(boss, boss, LandformNearby(pland, aspect, 2), ValiableConstant1, YES, 2.0);
        else
            enemy = spawnFloorNativeEnemy(LandformNearby(LandformCharacterPosition(boss),random()*8,1),CHARACTER_FORSE_ENEMY)
            if(enemy)then
                CharacterCureMental(enemy)
            end
            --[[
            for a = 0,7 do
                pland = LandformNearby(LandformCharacterPosition(boss), boss.aspect + a, 1)
                effect_Smoke1(pland.place.x, pland.place.y)
                color = BulletColor.magenta;
                type = bulletType.homing;
                bulletSummon( pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, type, color, boss.aspect + a);
            end
            ]]
        end
    end
end

