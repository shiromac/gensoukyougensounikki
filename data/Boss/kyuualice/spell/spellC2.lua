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

imageColor1 = Color(255,255,0,255)
imageColor2 = Color(255,128,0,128)

danmaku_phase_first = 1
danmaku_phase_middle = 2
danmaku_phase_last = 3

danmakuMap = {}

bombInterval = 8;

Warpflag = false;

BossCurrentHP = 0;
BossStop = 0;

tmpX = 0;
tmpY = 0;

turnWay = 1;
startWay = 0;

lazerCount = 0;

danmaku_phase = danmaku_phase_first

bossStateClinic = 0;
----------------------------------------------------------------------------------------------------------------------
function bullet_baramaki(turn)

    --local step = 0;
    --do true do
     --   danmaku_setting_field(danmakuOddRythms(spellTurnCount), ASPECT_DOWN, 2, BulletColor.red)
       -- coroutine.yield(true)
        
        --for step = 1, 4 do
          --  coroutine.yield(false)
        --end
        --coroutine.yield(false)
        --coroutine.yield(false)
    --while
    
    return false
end
----------------------------------------------------------------------------------------------------------------------
function selectAction(turn)
    
    return action_no

    --turn_amari = turn % 10
    --if(turn_amari == 3)then
     --   return action_nomove_tame
    --elseif(turn_amari == 4)then
      --  return action_kakutou
    --elseif(turn_amari == 5)then
      --  return action_danmaku
    --end
    --return action_nomove_attack;
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
    return YES;
end

function skillEffect(valiable)
--[[
    if(Warpflag ~= false)
    then
        pland = LandformNearby(pland, ASPECT_DOWN,1);
        bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.straight, BulletColor.magenta, ASPECT_UP);


        pland = LandformNearby(pland, ASPECT_DOWNRIGHT,1);
        bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.straight, BulletColor.magenta, ASPECT_UPLEFT);

    end

    CharacterTurnNow(boss, ASPECT_DOWN);

    pland = landformAroundOfCharacter(boss,a,1);
    if(pland.mountedCharacter)then
        pcharafuki = pland.mountedCharacter;
        CharacterThrow(boss,pcharafuki, LandformNearby(pland, boss.aspect + a, 7), ValiableConstant1, YES, 2.0);
    end
    
    
    ]]--
    
    AIshortCut_NomoveAttack(boss,valiable);
    
    
    return YES;
end

function passiveSkill(timing,valiable)
    if(Warpflag ~= true)
    then
        if(BossCurrentHP > boss.HP)
        then
            BossCurrentHP = boss.HP
            tmpx = boss.placeX;
            tmpy = boss.placeY;
            effect_Smoke1(tmpx, tmpy);
            warpland = LandformRandom_RoomRandom_OutOfSight_Safe(LandformCharacterPosition(PlayerCharacter()));
            
            pland = landformSearchObstacleWithRange(warpland, ASPECT_UP, 3,true,false,false,false);
            coo = Coordinate(warpland.place.x - pland.place.x, warpland.place.y - pland.place.y);
            
            if(coo:dif() < 3)
            then    
                warpland = landformSearchObstacleWithRange(warpland, ASPECT_DOWN, 3 - coo:dif(),true,false,false,false);
            end
            
            pland = landformSearchObstacleWithRange(warpland, ASPECT_DOWN, 3,true,false,false,false);
            coo = Coordinate(warpland.place.x - pland.place.x, warpland.place.y - pland.place.y);
            
            if(coo:dif() < 3)
            then
                warpland = landformSearchObstacleWithRange(warpland, ASPECT_UP, 3 - coo:dif(),true,false,false,false);
            end
    
            pland = landformSearchObstacleWithRange(warpland, ASPECT_LEFT, 3,true,false,false,false);
            coo = Coordinate(warpland.place.x - pland.place.x, warpland.place.y - pland.place.y);
            
            if(coo:dif() < 3)
            then    
                warpland = landformSearchObstacleWithRange(warpland, ASPECT_RIGHT, 3 - coo:dif(),true,false,false,false);
            end
            
            pland = landformSearchObstacleWithRange(warpland, ASPECT_RIGHT, 3,true,false,false,false);
            coo = Coordinate(warpland.place.x - pland.place.x, warpland.place.y - pland.place.y);
            
            if(coo:dif() < 3)
            then
                warpland = landformSearchObstacleWithRange(warpland, ASPECT_LEFT, 3 - coo:dif(),true,false,false,false);
            end
        
            effect_Smoke1(warpland.place.x, warpland.place.y)
            CharacterWarp(boss, warpland);
            CharacterTurn(boss,ASPECT_DOWN);
      
            Warpflag = true;
        end   
    end
end

function barrier()
    table.insert(danmakuMap, bulletSummon(LandformPlace(boss.placeX + 1, boss.placeY - 2), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.green, ASPECT_LEFT));
    table.insert(danmakuMap, bulletSummon(LandformPlace(boss.placeX + 2, boss.placeY - 2), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.green, ASPECT_LEFT));
    table.insert(danmakuMap, bulletSummon(LandformPlace(boss.placeX + 2, boss.placeY - 1), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.green, ASPECT_UP));
    table.insert(danmakuMap, bulletSummon(LandformPlace(boss.placeX + 2, boss.placeY), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.green, ASPECT_UP));
    table.insert(danmakuMap, bulletSummon(LandformPlace(boss.placeX + 2, boss.placeY + 1), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.green, ASPECT_UP));
    table.insert(danmakuMap, bulletSummon(LandformPlace(boss.placeX + 2, boss.placeY + 2), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.green, ASPECT_UP));
    table.insert(danmakuMap, bulletSummon(LandformPlace(boss.placeX + 1, boss.placeY + 2), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.green, ASPECT_RIGHT));
    table.insert(danmakuMap, bulletSummon(LandformPlace(boss.placeX    , boss.placeY + 2), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.green, ASPECT_RIGHT));
    table.insert(danmakuMap, bulletSummon(LandformPlace(boss.placeX - 1, boss.placeY + 2), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.green, ASPECT_RIGHT));
    table.insert(danmakuMap, bulletSummon(LandformPlace(boss.placeX - 2, boss.placeY + 2), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.green, ASPECT_RIGHT));
    table.insert(danmakuMap, bulletSummon(LandformPlace(boss.placeX - 2, boss.placeY + 1), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.green, ASPECT_DOWN));
    table.insert(danmakuMap, bulletSummon(LandformPlace(boss.placeX - 2, boss.placeY), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.green, ASPECT_DOWN));
    table.insert(danmakuMap, bulletSummon(LandformPlace(boss.placeX - 2, boss.placeY - 1), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.green, ASPECT_DOWN));
    table.insert(danmakuMap, bulletSummon(LandformPlace(boss.placeX - 2, boss.placeY - 2), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.green, ASPECT_DOWN));
    table.insert(danmakuMap, bulletSummon(LandformPlace(boss.placeX - 1, boss.placeY - 2), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.green, ASPECT_LEFT));


    table.insert(danmakuMap, bulletSummon(LandformPlace(boss.placeX - 1, boss.placeY + 1), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.yellow, ASPECT_RIGHT));
    table.insert(danmakuMap, bulletSummon(LandformPlace(boss.placeX - 1, boss.placeY), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.yellow, ASPECT_DOWN));
    table.insert(danmakuMap, bulletSummon(LandformPlace(boss.placeX - 1, boss.placeY - 1), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.yellow, ASPECT_DOWN));
    table.insert(danmakuMap, bulletSummon(LandformPlace(boss.placeX    , boss.placeY - 1), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.yellow, ASPECT_LEFT));
    table.insert(danmakuMap, bulletSummon(LandformPlace(boss.placeX + 1, boss.placeY - 1), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.yellow, ASPECT_LEFT));
    table.insert(danmakuMap, bulletSummon(LandformPlace(boss.placeX + 1, boss.placeY), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.yellow, ASPECT_UP));
    table.insert(danmakuMap, bulletSummon(LandformPlace(boss.placeX + 1, boss.placeY + 1), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.yellow, ASPECT_UP));end

function repairBarrier()
    local num = #danmakuMap;

    for i=1, num do
        if danmakuMap[num - i + 1]
        then
            if(danmakuMap[num - i + 1]:isDead())
            then
                pland = LandformPlace(danmakuMap[num - i + 1].placeX, danmakuMap[num - i + 1].placeY);
                pland = LandformNearby(pland, danmakuMap[num - i + 1].aspect,1);
                bullet = bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.green, danmakuMap[num - i + 1].aspect);
                table.insert(danmakuMap, bullet);
                table.remove(danmakuMap, num - i + 1);
                
            end   
        end
    end
end

function turnBarrier()
    pland = LandformPlace(boss.placeX - 2, boss.placeY - 2);
    if(pland.mountedCharacter)then
        CharacterTurnNow(pland.mountedCharacter, ASPECT_DOWN);
    end

    pland = LandformPlace(boss.placeX - 1, boss.placeY - 1);
    if(pland.mountedCharacter)then
        CharacterTurnNow(pland.mountedCharacter, ASPECT_DOWN);
    end
    
    pland = LandformPlace(boss.placeX - 2, boss.placeY + 2);
    if(pland.mountedCharacter)then
        CharacterTurnNow(pland.mountedCharacter, ASPECT_RIGHT);
    end

    pland = LandformPlace(boss.placeX - 1, boss.placeY + 1);
    if(pland.mountedCharacter)then
        CharacterTurnNow(pland.mountedCharacter, ASPECT_RIGHT);
    end

    pland = LandformPlace(boss.placeX + 2, boss.placeY + 2);
    if(pland.mountedCharacter)then
        CharacterTurnNow(pland.mountedCharacter, ASPECT_UP);
    end

    pland = LandformPlace(boss.placeX + 1, boss.placeY + 1);
    if(pland.mountedCharacter)then
        CharacterTurnNow(pland.mountedCharacter, ASPECT_UP);
    end

    pland = LandformPlace(boss.placeX + 2, boss.placeY - 2);
    if(pland.mountedCharacter)then
        CharacterTurnNow(pland.mountedCharacter, ASPECT_LEFT);
    end

    pland = LandformPlace(boss.placeX + 1, boss.placeY - 1);
    if(pland.mountedCharacter)then
        CharacterTurnNow(pland.mountedCharacter, ASPECT_LEFT);
    end

end

function lazerLock(count)
    if(count == 4)
    then
        randnum = math.floor( (random() * 10) );
        if(randnum % 2)
        then
            turnWay=1;
        else
            turnWay=-1;
        end    
        
        startWay = math.floor( (random() * 8) );
    end
    
    player = PlayerCharacter();
    
    pland = landformSearchObstacleWithRange(LandformPlace(player.placeX,player.placeY), (startWay+4)%8, MAPHEIGHT*2,true,false,false,false);
    pland = LandformNearby(pland,startWay,1);

    lazerSummon(pland, startWay,MAPHEIGHT*2, 40, CHARACTER_FORSE_ENEMY, Color(137,137,137,137), 2, 3, true, true);
    
    startWay = (startWay + turnWay + 8) % 8;
end

function putUpBarrage()
    if (bossStateClinic > 0)
    then
        cureState();
    else
        cureStateCheck();    
    end    

    stackDramaThread(coroutine.create(danmaku_setting));    
    
    repairBarrier();
    
    turnBarrier();
    
    if(spellTurnCount%20 == 8)then
        lazerCount = 4;
    end
    
    if lazerCount > 0
    then
        lazerLock(lazerCount);
        lazerCount = lazerCount - 1;
    end
    
    if(spellTurnCount%10 == 0)then
        CharacterCureMental(spawnFloorNativeEnemy(LandformRandom_RoomRandom_OutOfSight_Safe(LandformCharacterPosition(PlayerCharacter())),CHARACTER_FORSE_ENEMY));
    end
end



function startSpell()
    --outputDebugString("startSpell");
    BossCurrentHP = boss.HP;
    warpland = LandformRandom_RoomRandom_OutOfSight_Safe(LandformCharacterPosition(PlayerCharacter()));
            
    pland = landformSearchObstacleWithRange(warpland, ASPECT_UP, 3,true,false,false,false);
    coo = Coordinate(warpland.place.x - pland.place.x, warpland.place.y - pland.place.y);
    
    if(coo:dif() < 3)
    then    
        warpland = landformSearchObstacleWithRange(warpland, ASPECT_DOWN, 3 - coo:dif(),true,false,false,false);
    end
    
    pland = landformSearchObstacleWithRange(warpland, ASPECT_DOWN, 3,true,false,false,false);
    coo = Coordinate(warpland.place.x - pland.place.x, warpland.place.y - pland.place.y);
    
    if(coo:dif() < 3)
    then
        warpland = landformSearchObstacleWithRange(warpland, ASPECT_UP, 3 - coo:dif(),true,false,false,false);
    end

    pland = landformSearchObstacleWithRange(warpland, ASPECT_LEFT, 3,true,false,false,false);
    coo = Coordinate(warpland.place.x - pland.place.x, warpland.place.y - pland.place.y);
    
    if(coo:dif() < 3)
    then    
        warpland = landformSearchObstacleWithRange(warpland, ASPECT_RIGHT, 3 - coo:dif(),true,false,false,false);
    end
    
    pland = landformSearchObstacleWithRange(warpland, ASPECT_RIGHT, 3,true,false,false,false);
    coo = Coordinate(warpland.place.x - pland.place.x, warpland.place.y - pland.place.y);
    
    if(coo:dif() < 3)
    then
        warpland = landformSearchObstacleWithRange(warpland, ASPECT_LEFT, 3 - coo:dif(),true,false,false,false);
    end

    effect_Smoke1(warpland.place.x, warpland.place.y)
    CharacterWarp(boss, warpland)
    CharacterTurn(boss,ASPECT_DOWN)

    Warpflag = true;

    for i = 1, ValiableConstant4 do
        spawnFloorNativeItem(LandformRandom_Random())
    end
end

function endSpell()
    eraseDanmaku(boss);
    --outputDebugString("endSpell");
end

-----------------------------------------------------------------------
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

-----------------------------------------------------------------------

function danmaku_setting()
    
    showerShot(aspectOfBetweenCharacters(boss, PlayerCharacter()));
    showerShot( (aspectOfBetweenCharacters(boss, PlayerCharacter()) + 2) % 8);
    showerShot( (aspectOfBetweenCharacters(boss, PlayerCharacter()) + 6) % 8);
    
    
    if(Warpflag ~= false)
    then
        local num = #danmakuMap;

        for i=1, num do
            if danmakuMap[num - i + 1]
            then
                
                table.remove(danmakuMap, num - i + 1);
            end
        end
        
        barrier();
        Warpflag = false;
    end
    
    return true;
end

function showerShot(danmaku_aspect)
    pland=landformSearchObstacleWithRange(LandformPlace(boss.placeX,boss.placeY), danmaku_aspect, MAPHEIGHT, true, false, false, false);
    pland = LandformNearby(pland,((danmaku_aspect+4)% 8),1);
    
    if ((spellTurnCount % 3) == 0)
    then
        danmaku_aspect = (danmaku_aspect + 4) % 8;
    elseif ((spellTurnCount % 3) == 1)
    then
        danmaku_aspect = (danmaku_aspect + 3) % 8;
    else
        danmaku_aspect = (danmaku_aspect + 5) % 8;
    end

    if(spellRestTurn / spellTurnLimitCount < 0.6) or (boss.HP / boss.maxHP < 0.6)
    then
        bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.straight, BulletColor.magenta, danmaku_aspect);
    else
        bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.magenta, danmaku_aspect);
    end
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