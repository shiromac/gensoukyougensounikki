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

imageColor1 = Color(255,0,0,225)
imageColor2 = Color(255,0,128,255)

danmaku_phase_first = 1
danmaku_phase_middle = 2
danmaku_phase_last = 3

danmakuMap = {}

snakeMap={}

bombInterval = 8;

Warpflag = false;

BossCurrentHP = 0;
BossStop = 0;

tmpX = 0;
tmpY = 0;

danmaku_phase = danmaku_phase_first

danmakuWay = 0;

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
    if (Warpflag == true)
    then
        return YES;
    elseif BossStop > 0
    then
        return YES;
    end
    
    return NO;
end

function skillEffect(valiable)
    if(Warpflag ~= false)
    then
        CharacterTurnNow(boss, aspectOfBetweenCharacters(boss, PlayerCharacter()));

        blt = bulletSummon(LandformPlace(tmpx, tmpy), ValiableConstant2*1.5, CHARACTER_FORSE_ENEMY, 0, bulletType.homing, BulletColor.black, boss.aspect);
        tbl = { bullet = blt, turn = bombInterval - 3};
        table.insert(danmakuMap, tbl);
        
        pland=landformSearchObstacleWithRange(LandformPlace(boss.placeX,boss.placeY), boss.aspect, 5, true, false, false, false)

        blt = bulletSummon(pland, ValiableConstant2*1.5, CHARACTER_FORSE_ENEMY, 0, bulletType.homing, BulletColor.black, boss.aspect);
        tbl = { bullet = blt, turn = 0};
        table.insert(danmakuMap, tbl);
        
        Warpflag = false;
        BossStop = 6;
    else    
        BossStop = BossStop - 1;
        AIshortCut_NomoveAttack(boss,valiable);
    end
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
            effect_Smoke1(warpland.place.x, warpland.place.y)
            CharacterWarp(boss, warpland);
            --CharacterTurn(boss,ASPECT_DOWN);
            
            
            
            Warpflag = true;
        end   
    end
end

function putUpBarrage()
    if (bossStateClinic > 0)
    then
        cureState();
    else
        cureStateCheck();    
    end
    
    local num = #danmakuMap;

    for i=1, num do
        if danmakuMap[num - i + 1]
        then
            tmpbullet = danmakuMap[num - i + 1].bullet;
            tmpturn = danmakuMap[num - i + 1].turn;
            
            if tmpbullet
            then
                if(tmpbullet:isDead())
                then
                    table.remove(danmakuMap, num - i + 1);
                else
                    tmpturn = (tmpturn + 1) % bombInterval;
                    danmakuMap[num - i + 1].turn = tmpturn;
                    if((tmpturn % bombInterval) == (bombInterval - 2))
                    then
                        CharacterSurprise( tmpbullet ,1);
                    end
                    
                    if((tmpturn % bombInterval) == (bombInterval - 1))
                    then
                        bltland = LandformPlace(tmpbullet.placeX, tmpbullet.placeY);
                        distance = 3;
                        baku_color = Color(255,255,84,41);
                        lazerSummon(LandformNearby(bltland, ASPECT_DOWN,1), ASPECT_DOWN,distance, 30, CHARACTER_FORSE_ENEMY, baku_color, 0, 0, true, true);
                        lazerSummon(LandformNearby(bltland, ASPECT_UP,1), ASPECT_UP,distance, 30, CHARACTER_FORSE_ENEMY, baku_color, 0, 0, true, true);
                        lazerSummon(LandformNearby(bltland, ASPECT_LEFT,1), ASPECT_LEFT,distance, 30, CHARACTER_FORSE_ENEMY, baku_color, 0, 0, true, true);
                        lazerSummon(LandformNearby(bltland, ASPECT_RIGHT,1), ASPECT_RIGHT,distance, 30, CHARACTER_FORSE_ENEMY, baku_color, 0, 0, true, true);
                        
                        lazerSummon(LandformNearby(bltland, ASPECT_DOWNLEFT,1), ASPECT_DOWNLEFT,distance, 30, CHARACTER_FORSE_ENEMY, baku_color, 0, 0, true, true);
                        lazerSummon(LandformNearby(bltland, ASPECT_UPRIGHT,1), ASPECT_UPRIGHT,distance, 30, CHARACTER_FORSE_ENEMY, baku_color, 0, 0, true, true);
                        lazerSummon(LandformNearby(bltland, ASPECT_UPLEFT,1), ASPECT_UPLEFT,distance, 30, CHARACTER_FORSE_ENEMY, baku_color, 0, 0, true, true);
                        lazerSummon(LandformNearby(bltland, ASPECT_DOWNRIGHT,1), ASPECT_DOWNRIGHT,distance, 30, CHARACTER_FORSE_ENEMY, baku_color, 0, 0, true, true);
                        CharacterTurnNow(danmakuMap[num - i + 1].bullet, aspectOfBetweenCharacters(danmakuMap[num - i + 1].bullet, PlayerCharacter()));
                    end
                end
            end    
        end
    end
    
    num = #snakeMap;
    for i=1, num do
        if snakeMap[num - i + 1]
        then
            tmpbullet = snakeMap[num - i + 1].bullet;
            tmpturn = snakeMap[num - i + 1].turn;
            
            if tmpbullet
            then
                if(tmpbullet:isDead())
                then
                    table.remove(snakeMap, num - i + 1);
                else 
                    if(spellTurnCount - tmpturn > 3)
                    then
                        CharacterTurnNow(snakeMap[num - i + 1].bullet, aspectOfBetweenCharacters(snakeMap[num - i + 1].bullet, PlayerCharacter()));
                        table.remove(snakeMap, num - i + 1);
                    end
                end
            end    
        end
    end
    
    if(spellTurnCount%3 == 0)    
    then   
        stackDramaThread(coroutine.create(danmaku_setting));    
    end
    
    if(spellTurnCount%10 == 0)then
       CharacterCureMental(spawnFloorNativeEnemy(LandformRandom_RoomRandom_OutOfSight_Safe(LandformCharacterPosition(PlayerCharacter())),CHARACTER_FORSE_ENEMY));
    end
end

function snakeBullet(danmakuAspect)
    pland = landformSearchObstacleWithRange(LandformPlace(boss.placeX, boss.placeY), danmakuAspect, 4,true,false,false,false);
    blt = bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.blue, danmakuAspect);
    tbl = { bullet = blt, turn = spellTurnCount};
    table.insert(snakeMap, tbl);
    coroutine.yield(false);
    pland = landformSearchObstacleWithRange(pland, (danmakuAspect + 4) % 8, 1,true,false,false,false);
    blt = bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.cyan, danmakuAspect);
    tbl = { bullet = blt, turn = spellTurnCount};
    table.insert(snakeMap, tbl); 
    coroutine.yield(false);
    pland = landformSearchObstacleWithRange(pland, (danmakuAspect + 4) % 8, 1,true,false,false,false);
    blt = bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.cyan, danmakuAspect);
    tbl = { bullet = blt, turn = spellTurnCount};
    table.insert(snakeMap, tbl);
    coroutine.yield(false);
    pland = landformSearchObstacleWithRange(pland, (danmakuAspect + 4) % 8, 1,true,false,false,false);
    blt = bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.blue, danmakuAspect);
    tbl = { bullet = blt, turn = spellTurnCount};
    table.insert(snakeMap, tbl);
    coroutine.yield(false);
end

function snakeDanmaku(bulletWay)
    if(bulletWay == 0)
    then
        snakeBullet((boss.aspect + 1) % 8);
    elseif(bulletWay == 1)
    then
        snakeBullet((boss.aspect + 6) % 8);
    elseif(bulletWay == 2)
    then
        snakeBullet((boss.aspect + 2) % 8);
    else
        snakeBullet((boss.aspect + 7) % 8);
    end
end

function danmaku_setting()
    snakeDanmaku(danmakuWay);
    
    if(spellRestTurn / spellTurnLimitCount < 0.4) or (boss.HP / boss.maxHP < 0.4)
    then
        snakeDanmaku((danmakuWay+3)%4);
    end

    danmakuWay = (danmakuWay + 1) % 4;
    
    return true;
end


function startSpell()
    --outputDebugString("startSpell");
    BossCurrentHP = boss.HP;
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