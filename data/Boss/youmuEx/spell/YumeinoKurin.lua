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

bossStateClinic = 0;

mediceneBullet = vector_cCharacter()

--valueFunction = function(Landform)

function bossMoveTargetLandform()
    local player = PlayerCharacter();
    local partner = searchOneCharacterOnFloor(5012);
    local boss2boss = Coordinate(boss.placeX - partner.placeX, boss.placeY - partner.placeY);
    local boss2player = Coordinate(boss.placeX - player.placeX, boss.placeY - player.placeY);
    local partner2player = Coordinate(partner.placeX - player.placeX, partner.placeY - player.placeY);

    if(boss2player:dif() > 3)
    then
        if(boss2boss:dif() < boss2player:dif() + partner2player:dif())
        then
            local target = landformAroundOfCharacter(player,(aspectOfBetweenCharacters(partner, boss)),3);
            local target2player = Coordinate(target.place.x - player.placeX, target.place.y - player.placeY);
            
            if(target2player:dif() <= boss2player:dif())
            then
                return LandformNearby(LandformPlace(player.placeX,player.placeY),aspectOfBetweenCharacters(partner, boss),3);
            end    
        end
    end
    
    return LandformEdge();
end

function AISkillChoise(valiable)
    if (spellTurnCount % 7) == 0
    then
        return YES;
    else
        return NO;
    end
end

function skillEffect(valiable)
    CharacterTurnNow(boss, aspectOfBetweenCharacters(boss, PlayerCharacter())); 
    lazerSummon(landformAroundOfCharacter(boss, 7, 1), (boss.aspect+2)%8,2, ValiableConstant3, CHARACTER_FORSE_ENEMY, Color(255,255,255,137), 0, 0, false, true);
    bulletSummon(landformAroundOfCharacter(boss, 7, 1), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.blue, boss.aspect);
    bulletSummon(landformAroundOfCharacter(boss, 0, 1), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.blue, boss.aspect);
    bulletSummon(landformAroundOfCharacter(boss, 1, 1), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.blue, boss.aspect);

    return YES;
end

function passiveSkill(timing,valiable)
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


function putUpBarrage()
    if (bossStateClinic > 0)
    then
        cureState();
    else
        cureStateCheck();    
    end    
    
    if(spellRestTurn / spellTurnLimitCount < 0.3) 
    then
        local player = PlayerCharacter();
        local boss2player = Coordinate(boss.placeX - player.placeX, boss.placeY - player.placeY);
        
        if(boss2player:dif() >= 2)
        then
            CharacterSpeedUp(boss, 0);
        end
    end
    
    if(boss.HP / boss.maxHP < 0.3)
    then
        local player = PlayerCharacter();
        local partner = searchOneCharacterOnFloor(5012);
        local partner2player = Coordinate(partner.placeX - player.placeX, partner.placeY - player.placeY);
        
        if(partner2player:dif() >= 2)
        then
            CharacterSpeedUp(partner, 0);
        end
    end
    
    stackDramaThread(coroutine.create(danmaku_setting))
    if(spellTurnCount%10 == 0)then
        CharacterCureMental(spawnFloorNativeEnemy(LandformRandom_RoomRandom_OutOfSight_Safe(LandformCharacterPosition(PlayerCharacter())),CHARACTER_FORSE_ENEMY));
        CharacterCureMental(spawnFloorNativeEnemy(LandformRandom_RoomRandom_OutOfSight_Safe(LandformCharacterPosition(PlayerCharacter())),CHARACTER_FORSE_ENEMY));
    end
end

function startSpell()
    bossLand = LandformRandom_RoomRandom_OutOfSight_Safe(LandformCharacterPosition(PlayerCharacter()));
    effect_Smoke1(bossLand.place.x, bossLand.place.y);
    CharacterWarp(boss, bossLand);
    bossLand = LandformPlace(28,16);
    effect_Smoke1(bossLand.place.x, bossLand.place.y);
    emeny = MakeCharacter(5012, 0, CHARACTER_FORSE_ENEMY, bossLand);
    emeny2 = MakeCharacter(5012, 0, CHARACTER_FORSE_ENEMY, bossLand);
    --MakeCharacter(5005, 0, CHARACTER_FORSE_ENEMY, LandformPlace(boss.placeX+1,boss.placeY))
    for i = 1, ValiableConstant4 do
        spawnFloorNativeItem(LandformRandom_Random())
    end
end

function endSpell()
    local partner = searchOneCharacterOnFloor(5012);
    
    if(partner)
    then
        CharacterForseGoAway(partner);
    end
    local partner = searchOneCharacterOnFloor(5012);
    
    if(partner)
    then
        CharacterForseGoAway(partner);
    end
    
    eraseDanmaku(boss);
    --outputDebugString("endSpell");
end

-----------------------------------------------------------------------

function danmaku_setting()
    return true;
end
