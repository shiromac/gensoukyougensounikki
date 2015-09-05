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

LazerCountMax = 15

bossFixPlaceX=20;
bossFixPlaceY=12; 

function bossMoveTargetLandform()
    --outputDebugString("bossMoveTargetLandform");
    return LandformPlace(bossFixPlaceX,bossFixPlaceY);
end

function AISkillChoise(valiable)
    if (spellTurnCount % LazerCountMax) == 0
    then
        return YES;
    else
        coo = Coordinate(boss.placeX - PlayerCharacter().placeX, boss.placeY - PlayerCharacter().placeY);
        if(coo:dif() <= 1)
        then
            return YES;
        else
            if(boss.placeX == bossFixPlaceX) and (boss.placeY == bossFixPlaceY)
            then
                return YES;
            else    
                return NO;
            end    
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
    CharacterTurnNow(boss, aspectOfBetweenCharacters(boss, PlayerCharacter()));
    --danmaku_setting()
    if (spellTurnCount % LazerCountMax) == 0
    then
        kakutou_setting(valiable);
    else
        AIshortCut_NomoveAttack(boss,valiable);    
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
    --outputDebugString("startSpell");
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
function LazerMove(timeCount, x)
    pland = landformSearchObstacleWithRange(LandformPlace(x,MAPHEIGHT/2), ASPECT_UP, MAPHEIGHT,true,false,false,false);
    pland = LandformNearby(pland,ASPECT_DOWN,1);
    lazerSummon(pland, ASPECT_DOWN, 50, 15, CHARACTER_FORSE_ENEMY, Color(255,255,81,137), 1, timeCount, false, true);
end

function danmaku_setting()
    lazerCount = (spellTurnCount % LazerCountMax);
    if lazerCount == 0    
    then
        effect_boss_focus(boss.placeX, boss.placeY);
    end


    if (spellTurnCount % 6) == 0 and (spellTurnCount % LazerCountMax) ~= 0
    then
            bulletLand = landformSearchObstacleWithRange(LandformPlace(boss.placeX, boss.placeY), (boss.aspect + 3) % 8, 1,true,false,false,false);
            bullet_aspect = (LandformCharacterPosition(PlayerCharacter()).place - bulletLand.place): getAspect()
            bulletSummon(bulletLand, ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.homing, BulletColor.green, bullet_aspect);
            
            bulletLand = landformSearchObstacleWithRange(LandformPlace(boss.placeX, boss.placeY), (boss.aspect + 5) % 8, 1,true,false,false,false);
            bullet_aspect = (LandformCharacterPosition(PlayerCharacter()).place - bulletLand.place): getAspect()
            bulletSummon(bulletLand, ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.homing, BulletColor.green, bullet_aspect);
    end
    
    if  (spellTurnCount % 6) == 0 and ((spellRestTurn / spellTurnLimitCount < 0.4) or (boss.HP / boss.maxHP < 0.4))
    then
            bulletLand = landformSearchObstacleWithRange(LandformPlace(boss.placeX, boss.placeY), (boss.aspect + 2) % 8, 2,true,false,false,false);
            bullet_aspect = (LandformCharacterPosition(PlayerCharacter()).place - bulletLand.place): getAspect()
            bulletSummon(bulletLand, ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.homing, BulletColor.green, bullet_aspect);
            
            bulletLand = landformSearchObstacleWithRange(LandformPlace(boss.placeX, boss.placeY), (boss.aspect + 6) % 8, 2,true,false,false,false);
            bullet_aspect = (LandformCharacterPosition(PlayerCharacter()).place - bulletLand.place): getAspect()
            bulletSummon(bulletLand, ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.homing, BulletColor.green, bullet_aspect);
    end
    
    if lazerCount <= 10
    then
        LazerMove( 1 + (lazerCount / 5), 8 + lazerCount);
        LazerMove( 1 + (lazerCount / 5), 32 - lazerCount);
    end
    --if (spellTurnCount % 7) == 0
    --then
    --        bulletSummon(LandformPlace(boss.placeX - 3, boss.placeY), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.blue, ASPECT_DOWN);
    --        bulletSummon(LandformPlace(boss.placeX + 3, boss.placeY), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.blue, ASPECT_DOWN);
    --end
   
--   for i = 8, 18 do
--        turnCount = (spellTurnCount % 28) - i + 8;
--        LazerMove(turnCount, i);
--    end
    
--    for i = 22, 32 do
--        turnCount = (spellTurnCount % 28) + i - 32;
--        LazerMove(turnCount, i);
--    end
    
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

