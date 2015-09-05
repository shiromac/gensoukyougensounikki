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

skillCount=0;
distance=6;

lazerCount=0
lazerThreshold=45

repairMap={}
repairIndex=1;

bossStateClinic = 0;

--valueFunction = function(Landform)

function bossMoveTargetLandform()
    --outputDebugString("bossMoveTargetLandform");
    return LandformEdge();
end

function AISkillChoise(valiable)
    if(skillCount < 0)
    then
        return YES;
    elseif(skillCount > 8)
    then
        if(isValidShootToCharactor(boss, PlayerCharacter(), distance))
        then
            return YES;
        else
            skillCount = skillCount + 1; 
            return NO;
        end
    end
    
    skillCount = skillCount + 1;
    
    return NO;
end

function skillEffect(valiable)
    if(skillCount < 0)
    then
        gensezan()
        skillCount=0;
        return YES;
    end

    if(skillCount > 8)
    then
        if(isValidShootToCharactor(boss, PlayerCharacter(), distance))
        then
            CharacterTurnNow(boss, aspectOfBetweenCharacters(boss, PlayerCharacter()));
            skillCount = -1;
            effect_boss_charge(boss.placeX,boss.placeY,imageColor1,imageColor2);
            
            if(spellRestTurn / spellTurnLimitCount < 0.4) or (boss.HP / boss.maxHP < 0.4)
            then
                gensezan()
                skillCount=0;
            end
            return YES;
        end
    end
      
    return NO;
end

function passiveSkill(timing,valiable)
 
end

function putUpBarrage()
    if (bossStateClinic > 0)
    then
        cureState();
    else
        cureStateCheck();    
    end
    
    stackDramaThread(coroutine.create(danmaku_setting))
    if(spellTurnCount%10 == 0)then
        CharacterCureMental(spawnFloorNativeEnemy(LandformRandom_RoomRandom_OutOfSight_Safe(LandformCharacterPosition(PlayerCharacter())),CHARACTER_FORSE_ENEMY));
    end
end

function startSpell()
    repairMap[1] = {};
    repairMap[2] = {};
    repairMap[3] = {};
    repairMap[4] = {};
    
    for i = 1, ValiableConstant4 do
        spawnFloorNativeItem(LandformRandom_Random())
    end
end

function endSpell()
    --outputDebugString("endSpell");
    for i=1,4 do
        while repairMap[i][1]
        do
            mapTransformForseLandChange(repairMap[i][1],LANDFORM_ATTRIBUTE_FLOOR);
            table.remove(repairMap[i], 1)
        end
    end    
    eraseDanmaku(boss);
end

-----------------------------------------------------------------------

function gensezan()    
    while repairMap[(repairIndex % 5) + 1][1]
    do
        mapTransformForseLandChange(repairMap[(repairIndex % 5) + 1][1],LANDFORM_ATTRIBUTE_FLOOR);
        table.remove(repairMap[(repairIndex % 5) + 1], 1)
    end

    pland=landformSearchObstacleWithRange(LandformPlace(boss.placeX,boss.placeY), boss.aspect, distance+1, true, false, false, false)
    pland = LandformNearby(pland,((boss.aspect+4)% 8),1)
    lazerSummon(landformAroundOfCharacter(boss, 0, 1), boss.aspect,distance, 40, CHARACTER_FORSE_ENEMY, Color(255,255,255,137), 0, 0, true, true);
    
    
    cland=LandformPlace(boss.placeX,boss.placeY)

    local index=1;
    while 1
    do
        repairMap[repairIndex][index] = cland;
        index=index+1;
        mapTransformForseLandChange(cland,LANDFORM_ATTRIBUTE_WATER)
        cland=LandformNearby(cland,boss.aspect,1)
        
        if (pland.place.x == cland.place.x) and (pland.place.y == cland.place.y)
        then
            break
        end
    end
    
    repairIndex=(repairIndex % 5) + 1;
    
    mapTransformArrangement()
    
    CharacterWarp(boss, pland)
    --dis=distance;
    --pland = landformAroundOfCharacter(boss,0,dis);
    --while(pland.mountedCharacter)
    --do
    --    dis=dis+1;
    --    pland = landformAroundOfCharacter(boss,0,dis);
    --end

    --lazerSummon(landformAroundOfCharacter(boss, 0, 1), boss.aspect, dis, 40, CHARACTER_FORSE_ENEMY, Color(255,255,255,137), 0, 0, true, true);
    --CharacterWarp(boss, pland)
end

function danmaku_setting()
    pland=LandformPlace(boss.placeX,boss.placeY);
    sland=LandformNearby(pland,boss.aspect,1);
    if ((spellTurnCount % 4) ~= 0)
    then
        bulletSummon(sland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.straight, BulletColor.red, boss.aspect);
    end
    
    if ((spellTurnCount % 2) == 1)
    then
        bulletSummon(LandformNearby(sland,(boss.aspect+2)%8,1), ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.straight, BulletColor.red, (boss.aspect+1)%8);
        bulletSummon(LandformNearby(sland,(boss.aspect+6)%8,1), ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.straight, BulletColor.red, (boss.aspect+7)%8);
        bulletSummon(LandformNearby(pland,(boss.aspect+2)%8,2), ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.straight, BulletColor.red, (boss.aspect+1)%8);
        bulletSummon(LandformNearby(pland,(boss.aspect+6)%8,2), ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.straight, BulletColor.red, (boss.aspect+7)%8);
        sland=LandformNearby(pland,(boss.aspect+4)%8,1);
        bulletSummon(LandformNearby(sland,(boss.aspect+2)%8,3), ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.straight, BulletColor.red, (boss.aspect+1)%8);
        bulletSummon(LandformNearby(sland,(boss.aspect+6)%8,3), ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.straight, BulletColor.red, (boss.aspect+7)%8);
         
   end
    
    len = {1,2,3,4,5,6,7,8,8,9,9,9,8,8,7,6,5,4,3,2,1}
    if (lazerCount % (lazerThreshold*2)) < lazerThreshold
    then
        for i = 1, 21 
        do
            if(i-13+(lazerCount % lazerThreshold) >= 8) and  (i-13+(lazerCount % lazerThreshold) <= 32)
            then
                lazerSummon(LandformPlace(8, i-13+(lazerCount % lazerThreshold)), ASPECT_RIGHT, len[i], 40, CHARACTER_FORSE_ENEMY, Color(255,0,0,255), 0, 1, false, true);
            end    
        end
    else
        for i = 1, 21 
        do
            if(i-13+(lazerCount % lazerThreshold) >= 8) and  (i-13+(lazerCount % lazerThreshold) <= 32)
            then
                lazerSummon(LandformPlace(32, i-13+(lazerCount % lazerThreshold)), ASPECT_LEFT, len[i], 40, CHARACTER_FORSE_ENEMY, Color(255,0,0,255), 0, 1, false, true);
            end    
        end
    end
    
    lazerCount=lazerCount+1;
    return true;
end

function cureStateCheck()
    if(CharacterIsMentalAberration(boss) == YES) or (CharacterIsPhysicalAberration(boss) == YES) 
        or (CharacterIsMagicAberration(boss) == YES) or (CharacterIsSpeedAberration(boss) == YES)
    then
        bossStateClinic = 2
    end

end


function cureState()
    if(bossStateClinic == 1 )
    then
        CharacterCureMental(boss);
        CharacterCurePhysical(boss);
        CharacterCureMagic(boss);
        CharacterCureSpeed(boss);    
    end 
    
    bossStateClinic = bossStateClinic - 1;
end