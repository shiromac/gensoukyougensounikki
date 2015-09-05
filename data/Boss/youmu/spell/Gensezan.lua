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

imageColor1 = Color(255,53,201,70)
imageColor2 = Color(255,200,220,220)


mediceneBullet = vector_cCharacter()

skillCount=0;
distance=6;

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
    for i = 1, ValiableConstant4 do
        spawnFloorNativeItem(LandformRandom_Random())
    end
end

function endSpell()
    eraseDanmaku(boss);
    --outputDebugString("endSpell");
end

-----------------------------------------------------------------------

function gensezan()
    pland=landformSearchObstacleWithRange(LandformPlace(boss.placeX,boss.placeY), boss.aspect, distance+1, true, false, false, false)
    pland = LandformNearby(pland,((boss.aspect+4)% 8),1)
    lazerSummon(landformAroundOfCharacter(boss, 0, 1), boss.aspect,distance, 40, CHARACTER_FORSE_ENEMY, Color(255,255,255,137), 0, 0, true, true);
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
    pland = LandformRandom_RoomRandom_OutOfSight_Safe(LandformCharacterPosition(PlayerCharacter()));
    pland = landformSearchObstacleWithRange(pland, ASPECT_LEFT, MAPHEIGHT*2,true,false,false,false);
    pland = LandformNearby(pland,ASPECT_RIGHT,1);
    bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.red, ASPECT_RIGHT);

    pland = LandformRandom_RoomRandom_OutOfSight_Safe(LandformCharacterPosition(PlayerCharacter()));
    pland = landformSearchObstacleWithRange(pland, ASPECT_RIGHT, MAPHEIGHT*2,true,false,false,false);
    pland = LandformNearby(pland,ASPECT_LEFT,1);
    y = random() * 25 + 8;
    bulletSummon(LandformPlace(32, y), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.red, ASPECT_LEFT);
    --bulletSummon(LandformPlace(32, 32), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.red, ASPECT_LEFT);
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