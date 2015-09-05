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
        turn_amari = turn % 6
        if(turn_amari == 3)then
            return action_nomove_tame
        elseif(turn_amari == 4)then
            return action_kakutou
        elseif(turn_amari == 5)then
            return action_danmaku
        end
    else
        --hakkyo
        turn_amari = turn % 6
        if(turn_amari == 3)then
            return action_nomove_tame
        elseif(turn_amari == 4)then
            return action_kakutou
        elseif(turn_amari == 5 or turn_amari == 2)then
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
    
    --enemy spown
    if(spellTurnCount%10 == 0)then
        CharacterCureMental(spawnFloorNativeEnemy(LandformRandom_RoomRandom_OutOfSight_Safe(LandformCharacterPosition(PlayerCharacter())),CHARACTER_FORSE_ENEMY))
        --nibai!
        CharacterCureMental(spawnFloorNativeEnemy(LandformRandom_RoomRandom_OutOfSight_Safe(LandformCharacterPosition(PlayerCharacter())),CHARACTER_FORSE_ENEMY))
    end
    
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
function danmaku_setting()
    effect_boss_focus(boss.placeX, boss.placeY);

    for i = 0, MAPHEIGHT-1 do

        aspect = ASPECT_LEFT + (i%2)*4;
        pland = landformSearchObstacleWithRange(LandformPlace(MAPWIDTH/2,i), aspect+4, MAPWIDTH,true,false,false,false);
        pland = LandformNearby(pland,aspect,1);

        if(isSafeSetCharacterLandform(pland))then

            effect_Smoke1(pland.place.x, pland.place.y)

            color = BulletColor.green;
            type = bulletType.straight;

            bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, type, color, aspect);
        end
    end
    for i = 0, MAPWIDTH-1 do
    
        aspect = ASPECT_DOWN + (i%2)*4;
        pland = landformSearchObstacleWithRange(LandformPlace(i,MAPHEIGHT/2), aspect+4, MAPHEIGHT,true,false,false,false);
        pland = LandformNearby(pland,aspect,1);

        if(isSafeSetCharacterLandform(pland))then

            effect_Smoke1(pland.place.x, pland.place.y)
            
            color = BulletColor.green;
            type = bulletType.straight;
            
            bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, type, color, aspect);
        end
    end
    
    for i = 0, 7 do
    
        pland = LandformPlace(boss.placeX, boss.placeY);
        pland = LandformNearby(pland, i, 1);
        if(isSafeSetCharacterLandform(pland))then

            effect_Smoke1(pland.place.x, pland.place.y)
            
            color = BulletColor.magenta;
            type = bulletType.straight;

            bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, type, color, i);
            
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
                    speed = 0
                    if(isHakkyou)then
                        type = bulletType.homing;
                        speed = 1
                    end
                    effect_Smoke1(pland.place.x, pland.place.y)
 
                    bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, speed, type, color, boss.aspect);
                end
            end
        end
    end
end

