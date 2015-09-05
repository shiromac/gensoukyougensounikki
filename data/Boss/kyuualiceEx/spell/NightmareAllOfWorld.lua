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
action_nomove_attack = 7

imageColor1 = Color(255,255,128,128)
imageColor2 = Color(255,255,0,0)

danmaku_phase_first = 1
danmaku_phase_middle = 2
danmaku_phase_last = 3

danmaku_phase = danmaku_phase_first

function selectAction(turn)
    turn_amari = turn % 10
    if(turn_amari == 3)then
        return action_nomove_tame
    elseif(turn_amari == 4)then
        return action_kakutou
    elseif(turn_amari == 5)then
        return action_danmaku
    end
    return action_nomove_attack;
end

function danmakuOddRythms(turn)
    turn_amari = turn % 20
    if(turn_amari > 10)then
        return 1
    else
        return 0;
    end
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
    elseif action == action_nomove_attack then 
        AIshortCut_NomoveAttack(boss,valiable)
        return YES;
    end
    
    return YES;
end

function passiveSkill(timing,valiable)
    --outputDebugString("passiveSkill");
    
end

function putUpBarrage()


    if(danmaku_phase == danmaku_phase_first)then
        if(spellTurnCount%2 == 1)then
        
            stackDramaThread(coroutine.create(function()
                danmaku_setting_field(danmakuOddRythms(spellTurnCount), ASPECT_DOWN, 0, BulletColor.red)
                return true
            end))
        end
        if(spellRestTurn / spellTurnLimitCount < 0.6) or (boss.HP / boss.maxHP < 0.6)then
            eraseDanmaku(boss)
            danmaku_phase = danmaku_phase_middle
            warpland = LandformNearby(landformSearchObstacleWithRange(LandformPlace(MAPWIDTH/2,MAPHEIGHT/2), ASPECT_DOWN, MAPWIDTH,true,false,false,false), ASPECT_DOWN+4, 1)
            effect_Smoke1(warpland.place.x, warpland.place.y)
            CharacterWarp(boss, warpland)
            CharacterTurn(boss,ASPECT_UP)
        end
        
    elseif(danmaku_phase == danmaku_phase_middle)then
        if(spellTurnCount%2 == 1)then
           
            stackDramaThread(coroutine.create(function()
                danmaku_setting_field(danmakuOddRythms(spellTurnCount), ASPECT_UP, 1, BulletColor.red)
                return true
            end))
        end
        if(spellRestTurn / spellTurnLimitCount < 0.3) or (boss.HP / boss.maxHP < 0.3)then
            eraseDanmaku(boss)
            danmaku_phase = danmaku_phase_last
            warpland = LandformNearby(landformSearchObstacleWithRange(LandformPlace(MAPWIDTH/2,MAPHEIGHT/2), ASPECT_UP, MAPWIDTH,true,false,false,false), ASPECT_UP+4, 1)
            effect_Smoke1(warpland.place.x, warpland.place.y)
            CharacterWarp(boss, warpland)
            CharacterTurn(boss,ASPECT_DOWN)
        end
    elseif(danmaku_phase == danmaku_phase_last)then
        if(spellTurnCount%2 == 1)then
        
            stackDramaThread(coroutine.create(function()
                danmaku_setting_field(danmakuOddRythms(spellTurnCount), ASPECT_DOWN, 2, BulletColor.red)
                return true
            end))
        end
    end
    --outputDebugString("putUpBarrage");
    action = selectAction(spellTurnCount)
    if (action == action_danmaku or action == action_hakkyo_danmaku) then
        stackDramaThread(coroutine.create(danmaku_setting_self))
    end
end



function startSpell()
    --outputDebugString("startSpell");

    warpland = LandformNearby(landformSearchObstacleWithRange(LandformPlace(MAPWIDTH/2,MAPHEIGHT/2), ASPECT_UP, MAPWIDTH,true,false,false,false), ASPECT_UP+4, 1)
    effect_Smoke1(warpland.place.x, warpland.place.y)
    CharacterWarp(boss, warpland)
    CharacterTurn(boss,ASPECT_DOWN)
    
    for i = 1, ValiableConstant4 do
        spawnFloorNativeItem(LandformRandom_Random())
    end
end

function endSpell()
    --outputDebugString("endSpell");
end

-----------------------------------------------------------------------
function danmaku_setting_field(odd, aspect, speed, color)

    for i = odd-2, MAPWIDTH-1, 2 do

        pland = landformSearchObstacleWithRange(LandformPlace(i,MAPHEIGHT/2), aspect+4, MAPWIDTH,true,false,false,false);
        pland = LandformNearby(pland,aspect,2);

        if(isSafeSetCharacterLandform(pland))then

            effect_Smoke1(pland.place.x, pland.place.y)

            type = bulletType.straight;

            bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, speed, type, color, aspect);
        end
    end
    
end
function danmaku_setting_self()
 
    effect_boss_focus(boss.placeX, boss.placeY);

    if(danmaku_phase == danmaku_phase_first)then
        danmaku_setting_field(danmakuOddRythms(spellTurnCount)+1, ASPECT_DOWN, 0, BulletColor.magenta)
    elseif(danmaku_phase == danmaku_phase_middle)then
        danmaku_setting_field(danmakuOddRythms(spellTurnCount)-1, ASPECT_UP, 1, BulletColor.magenta)
    elseif(danmaku_phase == danmaku_phase_last)then
        danmaku_setting_field(danmakuOddRythms(spellTurnCount)+1, ASPECT_DOWN, 2, BulletColor.magenta)
    end
    
    bulletSummon(LandformNearby(LandformCharacterPosition(boss),ASPECT_LEFT,1), ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.straight, BulletColor.magenta, ASPECT_LEFT);
    bulletSummon(LandformNearby(LandformCharacterPosition(boss),ASPECT_RIGHT,1), ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.straight, BulletColor.magenta, ASPECT_RIGHT);

    return true;
end

function kakutou_setting(valiable, isHakkyou)
    --effect
    
    pland = valiable.lands:val(ValiableIndex.attackplace);
    if(pland)then
        coo = Coordinate(boss.placeX - pland.place.x, boss.placeY - pland.place.y)
        
        effect_boss_emission(boss.placeX, boss.placeY,imageColor1,imageColor2);

        if(coo:dif() <= 1)then
            for a = 0,7 do
                pland = landformAroundOfCharacter(boss,a,1);
                if(pland.mountedCharacter)then
                    pcharafuki = pland.mountedCharacter;
                    CharacterThrow(boss,pcharafuki, LandformNearby(pland, boss.aspect + a, 7), ValiableConstant1, YES, 2.0);
                end
            end
        end
    end
end

