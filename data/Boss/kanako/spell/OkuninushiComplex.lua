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

waveInterval=25;
Orb = nil;

function bossMoveTargetLandform()
    --outputDebugString("bossMoveTargetLandform");
    return LandformPlace(20,12);
end

function AISkillChoise(valiable)
    if Orb
    then
        if not Orb:isDead()
        then
            pland = LandformPlace(Orb.placeX, Orb.placeY);
            if(pland)
            then
                coo = Coordinate(boss.placeX - pland.place.x, boss.placeY - pland.place.y)
                if(coo:dif() <= 1)
                then
                    return YES;
                end
            end   
        end 
    end
    
    if (spellTurnCount % waveInterval) == 0
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
end

function OrbPower()
    effect_boss_emission(Orb.placeX, Orb.placeY,imageColor1,imageColor2);
    eraseDanmaku(boss);
    coroutine.yield(false)

    pland = LandformNearby(LandformPlace(boss.placeX, boss.placeY),ASPECT_DOWN,1);
    effect_Smoke1(pland.place.x, pland.place.y);
    pland = LandformNearby(pland,ASPECT_DOWN,1);
    CharacterCureMental(spawnFloorNativeEnemy(pland,CHARACTER_FORSE_ENEMY));
    coroutine.yield(false)

    pland = LandformNearby(LandformPlace(boss.placeX, boss.placeY),ASPECT_LEFT,1);
    effect_Smoke1(pland.place.x, pland.place.y);
    pland = LandformNearby(pland,ASPECT_DOWN,1);
    CharacterCureMental(spawnFloorNativeEnemy(pland,CHARACTER_FORSE_ENEMY));
    coroutine.yield(false)
    
    pland = LandformNearby(LandformPlace(boss.placeX, boss.placeY),ASPECT_RIGHT,1);
    effect_Smoke1(pland.place.x, pland.place.y);
    pland = LandformNearby(pland,ASPECT_DOWN,1);
    CharacterCureMental(spawnFloorNativeEnemy(pland,CHARACTER_FORSE_ENEMY));
    coroutine.yield(false)
    
    return true;
end

function skillEffect(valiable)
    if(spellRestTurn / spellTurnLimitCount < 0.4) or (boss.HP / boss.maxHP < 0.4)
    then
        waveInterval = 15
    end
    --outputDebugString("skillEffect");
    if Orb
    then
        if not Orb:isDead()
        then
            pland = LandformPlace(Orb.placeX, Orb.placeY);
            if(pland)
            then
                coo = Coordinate(boss.placeX - pland.place.x, boss.placeY - pland.place.y)
                if(coo:dif() <= 1)
                then
                    stackDramaThread(coroutine.create(OrbPower));
                end
            end   
        end 
    end
    --danmaku_setting()
    if (spellTurnCount % waveInterval) == 0
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
    if(spellRestTurn / spellTurnLimitCount < 0.6) or (boss.HP / boss.maxHP < 0.6)
    then
        if(spellTurnCount%10 == 0)then
            CharacterCureMental(spawnFloorNativeEnemy(LandformRandom_RoomRandom_OutOfSight_Safe(LandformCharacterPosition(PlayerCharacter())),CHARACTER_FORSE_ENEMY));
        end
    else
        if(spellTurnCount%12 == 0)then
            CharacterCureMental(spawnFloorNativeEnemy(LandformRandom_RoomRandom_OutOfSight_Safe(LandformCharacterPosition(PlayerCharacter())),CHARACTER_FORSE_ENEMY));
        end
    end    
end



function startSpell()
    --outputDebugString("startSpell");
    --for i = 1, ValiableConstant4 do
    --    spawnFloorNativeItem(LandformRandom_Random())
    --end
    for x = 8, 32 do
        for y = 28, 32 do
            mapTransformForseLandChange(LandformPlace(x,y),LANDFORM_ATTRIBUTE_WATER)
        end
    end
    mapTransformArrangement();
    pland = LandformPlace(PlayerCharacter().placeX, PlayerCharacter().placeY);
    --CharacterWarp(PlayerCharacter(), LandformPlace(20,25));
    if(pland)then
        coo = Coordinate(boss.placeX - pland.place.x, boss.placeY - pland.place.y)
        
        effect_boss_emission(boss.placeX, boss.placeY,imageColor1,imageColor2);

        if(coo:dif() <= 8)
        then
            CharacterThrow(boss,PlayerCharacter(), LandformPlace(20, 30), 0, YES, 2.0);
        end
    end
    
    for i = 1, ValiableConstant4 do
        spawnFloorNativeItem(LandformRandom_Random())
        spawnFloorNativeTrap(LandformRandom_Random())
    end
end

function endSpell()
    eraseDanmaku(boss);
    
    for x = 8, 32 do
        for y = 28, 32 do
            mapTransformForseLandChange(LandformPlace(x,y),LANDFORM_ATTRIBUTE_FLOOR)
        end
    end
    mapTransformArrangement()
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
function danmaku_setting()
    if(spellTurnCount % waveInterval == 1)then
        Orb = bulletSummon(LandformPlace(20, 28), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.yellow, ASPECT_UP);
    
        bulletSummon(LandformPlace(9, 28), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.cyan, ASPECT_UP);
        bulletSummon(LandformPlace(10, 28), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.cyan, ASPECT_UP);
        bulletSummon(LandformPlace(11, 28), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.cyan, ASPECT_UP);

        bulletSummon(LandformPlace(13, 28), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.cyan, ASPECT_UP);
        bulletSummon(LandformPlace(14, 28), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.cyan, ASPECT_UP);
        bulletSummon(LandformPlace(15, 28), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.cyan, ASPECT_UP);
    
        bulletSummon(LandformPlace(19, 28), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.cyan, ASPECT_UP);
        bulletSummon(LandformPlace(21, 28), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.cyan, ASPECT_UP);

        bulletSummon(LandformPlace(24, 28), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.cyan, ASPECT_UP);
        bulletSummon(LandformPlace(25, 28), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.cyan, ASPECT_UP);
        bulletSummon(LandformPlace(26, 28), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.cyan, ASPECT_UP);

        bulletSummon(LandformPlace(29, 28), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.cyan, ASPECT_UP);
        bulletSummon(LandformPlace(30, 28), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.cyan, ASPECT_UP);
        bulletSummon(LandformPlace(31, 28), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.cyan, ASPECT_UP);
   
        for i = 8,32 do
            wavecolor = BulletColor.blue;
            wavecolor2 = BulletColor.cyan;
            if(i % 5 == 2) then 
                wavecolor = BulletColor.cyan;
                wavecolor2 = BulletColor.blue;
            elseif (i % 5 == 3) then 
                wavecolor = BulletColor.cyan;
                wavecolor2 = BulletColor.blue;
            end
            bulletSummon(LandformPlace(i, 29), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, wavecolor, ASPECT_UP);
            bulletSummon(LandformPlace(i, 30), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, wavecolor, ASPECT_UP);
            bulletSummon(LandformPlace(i, 31), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, wavecolor, ASPECT_UP);
            bulletSummon(LandformPlace(i, 32), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, wavecolor, ASPECT_UP);
        end
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

