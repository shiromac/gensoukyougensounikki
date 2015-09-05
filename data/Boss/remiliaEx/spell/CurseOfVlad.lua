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
daikibo_dannmaku_pool = {}
function danmaku_setting()
    
    local danmaku_num = #daikibo_dannmaku_pool
    for i = 1, danmaku_num do
        local bullet = table.remove(daikibo_dannmaku_pool)
        CharacterForseGoAway(bullet)
    end

    effect_boss_focus(boss.placeX, boss.placeY);
    
    local playerLand = LandformCharacterPosition(PlayerCharacter())
    
    --enemy spown
    CharacterCureMental(spawnFloorNativeEnemy(LandformRandom_RoomRandom_OutOfSight_Safe(LandformCharacterPosition(PlayerCharacter())),CHARACTER_FORSE_ENEMY))
    --nibai!
    CharacterCureMental(spawnFloorNativeEnemy(LandformRandom_RoomRandom_OutOfSight_Safe(LandformCharacterPosition(PlayerCharacter())),CHARACTER_FORSE_ENEMY))
    coroutine.yield(false)
    for i = 0, 31 do
    
        local visibleFlag = false;
        for k = 0, 7 do
        
            x = math.pow(i/32.0 -1, 2);
            jiku = PositionPerGrid(0,0,1)
            v = PositionPerGrid(i*ValiableConstant3 + x*ValiableConstant3*2, 0, 0)
            kaiten = (k*45+i*8)/57.3;
            D3DXVec3Rotate(v, jiku, kaiten, v)
            v.x = v.x + boss.placeX;
            v.y = v.y + boss.placeY ;
            pland = LandformPlace(v.x, v.y);
            if(isSafeSetCharacterLandform(pland))then
            
                --EffectFunctions::
                effect_Smoke1(pland.place.x, pland.place.y)
                
                color = BulletColor.red;
                type = bulletType.straight;
                if(i%4 == 0)then
                    color = BulletColor.magenta;
                    type = bulletType.homing;
                end
                local bullet = bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, type, color, boss.aspect + k + i/2);
                table.insert(daikibo_dannmaku_pool, bullet)
                
                if ((pland.place - playerLand.place): dif() <= 4) then
                    visibleFlag = true
                end
                
            end
        end
        
        if (visibleFlag) then
            effect_Wait(playerLand.place.x, playerLand.place.y, 7)
            coroutine.yield(false)
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
    
        if(coo:dif() >= 20)then
            CharacterThrow(boss,boss,pland,ValiableConstant1, YES, 2.0);
        elseif(coo:dif() <= 1)then
            for a = 0,7 do
                pland = landformAroundOfCharacter(boss,a,1);
                if(pland.mountedCharacter)then
                    pcharafuki = pland.mountedCharacter;
                    CharacterThrow(boss,pcharafuki, LandformNearby(pland, boss.aspect + a, 7), ValiableConstant1, YES, 2.0);
                end
            end
        else
            for i = 0,2 do
                pland = landformAroundOfCharacter(boss,i*2 -2,1);
                if(isSafeSetCharacterLandform(pland))then
      
                    color = BulletColor.red;
                    type = bulletType.straight;
                    speed = 0;
                    if(isHakkyou)then
                        speed = 1;
                    end
                    
                    effect_Smoke1(pland.place.x, pland.place.y)
 
                    bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, speed, type, color, boss.aspect);
                end
            end
        end
    end
end

