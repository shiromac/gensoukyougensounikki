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
    return LandformEdge();
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

        for i = 0, 2 do

            pland = LandformNearby(pland,aspect+6,5)

            for m = 1, 4 do
            
                mx = 5 - m
                
                summonpland = LandformNearby(pland,aspect+1,mx)
                
                if(isSafeSetCharacterLandform(summonpland))then

                    effect_Smoke1(summonpland.place.x, summonpland.place.y)

                    color = BulletColor.red;
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

