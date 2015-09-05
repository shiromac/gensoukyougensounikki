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


mediceneBullet = vector_cCharacter()


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
            if((me_place-you_place):dif() <= 5 and not enemy:isEqualTo(boss))then
                value = value - (6 - (me_place-you_place):dif())*10
                if(enemy:isEqualTo(PlayerCharacter()))then
                    value = value - (6 - (me_place-you_place):dif())*10
                end
            end
        end
    end
    
    local size = mediceneBullet: size()
    for i = 0, size -1 do
        local enemy = charalist: at(i)
        if(enemy and enemy.HP > 0)then
            local me_place = Coordinate(land.place.x, land.place.y)
            local you_place = Coordinate(enemy.placeX, enemy.placeY)
            value = value - (me_place-you_place):dif()
            
            if((me_place-you_place):dif() <= 5)then
                value = value + (6 - (me_place-you_place):dif())*2
            end
            
            if((me_place-you_place):dif() <= 2)then
                value = value + (3 - (me_place-you_place):dif())*7
            end
        end
    end
    if(isSafeSetCharacterLandform(land) == false and not land:isEqualTo(LandformCharacterPosition(boss)))then
        value = value - 100
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
        elseif(turn_amari == 8)then
            return action_nomove_tame
        elseif(turn_amari == 9)then
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

function bossMoveTargetLandform()
    --outputDebugString("bossMoveTargetLandform");
    return selectMovePlace(boss, placeValue);
end

function AISkillChoise(valiable)
    --outputDebugString("AISkillChoise");
    action = selectAction(spellTurnCount)
    if (action ~= action_no) then
        return YES;
    else
        return NO;
    end
end

function skillEffect(valiable)
    --outputDebugString("skillEffect");
    a = ValiableFieldChip_double()
    a:dim_str("a",2);
    outputDebugValue(a:val_str("a"));
    b = ValiableField()

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
        return YES;
    elseif action == action_nomove_koutyoku then
        return YES;
    end
    
    return YES;
end

function passiveSkill(timing,valiable)
    --outputDebugString("passiveSkill");
    if(timing == Timing.turn_end)then
        local size = mediceneBullet: size()
        for i = 0, size -1 do
            local enemy = mediceneBullet: at(i)
            if(enemy and enemy.HP > 0)then
                local me_place = Coordinate(boss.placeX, boss.placeY)
                local you_place = Coordinate(enemy.placeX, enemy.placeY)
                if((me_place-you_place):dif() <= 1)then
                    CharacterHPRecovery(boss,20)
        			str_map = map_tstring_StyleString()
			        str_map:setValue(_T("Chara"), boss:fullName() )
			        utility_localizedMessage(_T("Boss_Tewi_MedicineOfOnamuti_success"),str_map)
                end
            end
            CharacterTurn(enemy, random()*8)
        end
    end
end

function putUpBarrage()
    --outputDebugString("putUpBarrage");
    action = selectAction(spellTurnCount)
    if (action == action_danmaku or action == action_hakkyo_danmaku) then
        stackDramaThread(coroutine.create(danmaku_setting))
    end
end

function startSpell()
    --outputDebugString("startSpell");
    for i = 1, ValiableConstant4 do
        spawnFloorNativeItem(LandformRandom_Random())
    end
    
    for k = 1, 10 do
        local summonpland = LandformRandom_RoomRandom_Safe();
        if(isSafeSetCharacterLandform(summonpland))then

            effect_Smoke1(summonpland.place.x, summonpland.place.y)

            color = BulletColor.yellow;
            type = bulletType.homing;

            local bullet = bulletSummon(summonpland, 100, CHARACTER_FORSE_OTHER, -2, type, color, random()*8)
            mediceneBullet: push_back(bullet)
        end
    end
    	
	effect_Smoke1(boss.placeX, boss.placeY)
	local bossland = LandformPlace(20,4+8)
	stackFunction(function(enemy, land) CharacterWarp(enemy, land) end, boss, bossland)
	effect_Smoke1(bossland.place.x, bossland.place.y)

end

function endSpell()
    --outputDebugString("endSpell");
end

-----------------------------------------------------------------------

damakuPool = {}
function danmaku_setting()
    effect_boss_focus(boss.placeX, boss.placeY);
    
    --enemy spown
    CharacterCureMental(spawnFloorNativeEnemy(LandformRandom_RoomRandom_OutOfSight_Safe(LandformCharacterPosition(PlayerCharacter())),CHARACTER_FORSE_ENEMY))

    for k = 1, ValiableConstant3 do
        local trap_place = LandformRandom_Random()
        effect_Smoke1(trap_place.place.x, trap_place.place.y)
        trap = spawnFloorNativeTrap(trap_place)
        if trap then
            trap: discover()
        end
    end

    for k = 0, 3 do
    
        aspect = ASPECT_LEFT + k*2;
        pland = landformSearchObstacleWithRange(LandformCharacterPosition(boss), aspect+4, MAPWIDTH,true,false,false,false)

        pland = LandformNearby(pland,aspect+2,5*2)
        for i = 0, 4 do

            pland = LandformNearby(pland,aspect+6,5)

            for m = 1, 4 do
            
                mx = 5 - m
                
                summonpland = LandformNearby(pland,aspect+1,mx)
                
                if(isSafeSetCharacterLandform(summonpland))then

                    effect_Smoke1(summonpland.place.x, summonpland.place.y)

                    color = BulletColor.green;
                    type = bulletType.homing;

                    bulletSummon(summonpland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, type, color, aspect+1);
                end
            end

        end

    end

    return true;
end

function kakutou_setting(valiable, isHakkyou)
    --effect
    
    pland = valiable.lands:val(ValiableIndex.attackplace);
    if(pland)then
        coo = Coordinate(boss.placeX - pland.place.x, boss.placeY - pland.place.y)
        
        effect_boss_emission(boss.placeX, boss.placeY,imageColor1,imageColor2);
    
        if(coo:dif() <= 1)then
            CharacterThrow(boss, boss, LandformNearby(pland, boss.aspect, 4), ValiableConstant1, YES, 2.0);
        else
            for a = 0,7 do
                pland = LandformNearby(LandformCharacterPosition(boss), boss.aspect + a, 1)
                effect_Smoke1(pland.place.x, pland.place.y)
                color = BulletColor.magenta;
                type = bulletType.homing;
                bulletSummon( pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, type, color, boss.aspect + a);
            end
        end
    end
end

