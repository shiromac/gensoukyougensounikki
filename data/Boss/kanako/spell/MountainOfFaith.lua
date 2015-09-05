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

imageColor1 = Color(255,200,45,67)
imageColor2 = Color(255,25,91,217)



function bossMoveTargetLandform()
    --outputDebugString("bossMoveTargetLandform");
    return LandformPlace(20,12);
end

function AISkillChoise(valiable)
    
    if (spellTurnCount % 10) == 0
    then
        return YES;
    else
        coo = Coordinate(boss.placeX - PlayerCharacter().placeX, boss.placeY - PlayerCharacter().placeY);
        if(coo:dif() <= 1)
        then
            return YES;
        else
            return NO;
        end    
    end
    --outputDebugString("AISkillChoise");
    --action = selectAction(spellTurnCount)
    --if (action ~= action_no) then
    --    return YES;
    --else
    --    return NO;
    --end
end

function skillEffect(valiable)
    --outputDebugString("skillEffect");
    
    --danmaku_setting()
    if (spellTurnCount % 10) == 0
    then
        kakutou_setting(valiable);
    else
        AIshortCut_NomoveAttack(boss,valiable)
    end
    return YES; 
end

function passiveSkill(timing,valiable)
    --outputDebugString("passiveSkill");
    --stackDramaThread(coroutine.create(danmaku_setting))
    --danmaku_setting()
end

function putUpBarrage()
    stackDramaThread(coroutine.create(danmaku_setting))
    if(spellTurnCount%12 == 0)then
        CharacterCureMental(spawnFloorNativeEnemy(LandformRandom_RoomRandom_OutOfSight_Safe(LandformCharacterPosition(PlayerCharacter())),CHARACTER_FORSE_ENEMY));
    end
end



function startSpell()
    --CharacterWarp(PlayerCharacter(), LandformPlace(20,25));
    pland = LandformPlace(PlayerCharacter().placeX, PlayerCharacter().placeY);
    if(pland)then
        coo = Coordinate(boss.placeX - pland.place.x, boss.placeY - pland.place.y)
        
        effect_boss_emission(boss.placeX, boss.placeY,imageColor1,imageColor2);

        if(coo:dif() <= 8)
        then
            CharacterThrow(boss,PlayerCharacter(), LandformPlace(20, 30), 0, YES, 2.0);
        end    
        --outputDebugString("startSpell");
        
    end 

    for i = 1, ValiableConstant4 do
        spawnFloorNativeItem(LandformRandom_Random())
        spawnFloorNativeTrap(LandformRandom_Random())
    end       
end

function endSpell()
    --outputDebugString("endSpell");
end

-----------------------------------------------------------------------
--function LazerMove(timeCount, x)
--    if turnCount >= 0 and turnCount < 18
--        then
--            pland = landformSearchObstacleWithRange(LandformPlace(x,MAPHEIGHT/2), ASPECT_UP, MAPHEIGHT,true,false,false,false);
--           
--            if turnCount < 5
--            then
--                pland = LandformNearby(pland,ASPECT_DOWN,1);
--                lazerSummon(pland, ASPECT_DOWN, turnCount * 2, 15, CHARACTER_FORSE_ENEMY, Color(255,255,81,137), 0, 0, false, true);
--            else 
--                    pland = LandformNearby(pland,ASPECT_DOWN,1 + (turnCount-5)  * 2);
--                    lazerSummon(pland, ASPECT_DOWN, 10, 15, CHARACTER_FORSE_ENEMY, Color(255,255,81,137), 0, 0, false, true);
--            end
--        end
--end
-----------------------------------------------------------------------
function quadro(x, y, aspect, color)
    if(color == BulletColor.green)
    then
        lazerSummon(LandformPlace(x,y), ASPECT_RIGHT, 4, 15, CHARACTER_FORSE_ENEMY, Color(255,81,255,137), 0, 0, false, true);
    elseif(color == BulletColor.blue)
    then
        lazerSummon(LandformPlace(x,y), ASPECT_RIGHT, 4, 15, CHARACTER_FORSE_ENEMY, Color(255,81,137,255), 0, 0, false, true);
    else
        lazerSummon(LandformPlace(x,y), ASPECT_RIGHT, 4, 15, CHARACTER_FORSE_ENEMY, Color(255,255,81,137), 0, 0, false, true);
    end
    
    
    bulletSummon(LandformPlace(x, y), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, color, aspect);
    bulletSummon(LandformPlace(x+1, y), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, color, aspect);
    bulletSummon(LandformPlace(x+2, y), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, color, aspect);
    bulletSummon(LandformPlace(x+3, y), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, color, aspect);
end

function danmaku_setting()
    randomnum = math.floor( random() * 2);
    if(spellTurnCount % 4 == 0)then
        quadro(14+randomnum, 8, ASPECT_DOWNRIGHT, BulletColor.green)
        quadro(8+randomnum, 13, ASPECT_DOWNRIGHT, BulletColor.blue)
    elseif (spellTurnCount % 4 == 1)then
        quadro(23-randomnum, 8, ASPECT_DOWNLEFT, BulletColor.green)
        quadro(29-randomnum, 13, ASPECT_DOWNLEFT, BulletColor.blue)
    elseif (spellTurnCount % 4 == 2)then
        quadro(8+randomnum, 8, ASPECT_DOWNRIGHT, BulletColor.green)
        quadro(8+randomnum, 18, ASPECT_DOWNRIGHT, BulletColor.red)
    elseif (spellTurnCount % 4 == 3)then
        quadro(29-randomnum, 8, ASPECT_DOWNLEFT, BulletColor.green)
        quadro(29-randomnum, 18, ASPECT_DOWNLEFT, BulletColor.red)
    end
    
    return true;
end

function kakutou_setting(valiable)
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

