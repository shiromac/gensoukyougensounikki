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

imageColor1 = Color(255,0,255,0)
imageColor2 = Color(255,30,128,30)

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

rootPoint = 0;
sizeofField = 6;
fieldTurn = 0;

enemySpawnFunc = {}

surviveEnemy = {}

spawnOrder = 1;

bossStateClinic = 0;

danmaku_phase = danmaku_phase_first

danmakuMap = {}

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
    move_target_aspect = (aspectOfBetweenCharacters(boss, PlayerCharacter()) + 4) % 8;
    pland = landformSearchObstacleWithRange(LandformPlace(boss.placeX,boss.placeY), move_target_aspect, MAPHEIGHT, true, false, false, false);
    pland = LandformNearby(pland,((move_target_aspect+6)% 8),1);
    coo = Coordinate(rootPoint.place.x - pland.place.x, rootPoint.place.y - pland.place.y);

    return LandformNearby(pland,((move_target_aspect+4)% 8),(coo:dif() - sizeofField - 2));
end

function AISkillChoise(valiable)
    return NO;
end

function skillEffect(valiable) 
    AIshortCut_NomoveAttack(boss,valiable);
    
    return NO;
end

function passiveSkill(timing,valiable)
    if(Warpflag ~= true)
    then
        if(BossCurrentHP > boss.HP)
        then
            BossCurrentHP = boss.HP
            tmpx = boss.placeX;
            tmpy = boss.placeY;
            warpland = LandformRandom_Random();
            effect_Smoke1(warpland.place.x, warpland.place.y)
            CharacterWarp(boss, warpland);
            CharacterTurn(boss,ASPECT_DOWN);
      
            Warpflag = true;
        end   
    end
end

function eraseField()
    local num = #danmakuMap;

    for i=1, num do
        if danmakuMap[num - i + 1]
        then
            CharacterForseGoAway(danmakuMap[num - i + 1])
            table.remove(danmakuMap, num - i + 1);
        end
    end
end
        
function battleField()
    eraseField(boss);
    num = sizeofField*2 - 2;
    local i = 1;
    effect_Smoke1(rootPoint.place.x - sizeofField + i, rootPoint.place.y - sizeofField + 1);
    local bullet = bulletSummon(LandformPlace(rootPoint.place.x - sizeofField + i, rootPoint.place.y - sizeofField + 1), ValiableConstant2, CHARACTER_FORSE_ENEMY, -2, bulletType.straight, BulletColor.green, ASPECT_DOWNRIGHT);
    table.insert(danmakuMap, bullet);
    effect_Smoke1(rootPoint.place.x - sizeofField + i, rootPoint.place.y + sizeofField - 1);
    bullet = bulletSummon(LandformPlace(rootPoint.place.x - sizeofField + i, rootPoint.place.y + sizeofField - 1), ValiableConstant2, CHARACTER_FORSE_ENEMY, -2, bulletType.straight, BulletColor.green, ASPECT_UPRIGHT);
    table.insert(danmakuMap, bullet);
    
    i = sizeofField*2 - 1;
    effect_Smoke1(rootPoint.place.x - sizeofField + i, rootPoint.place.y - sizeofField + 1);
    bullet = bulletSummon(LandformPlace(rootPoint.place.x - sizeofField + i, rootPoint.place.y - sizeofField + 1), ValiableConstant2, CHARACTER_FORSE_ENEMY, -2, bulletType.straight, BulletColor.green, ASPECT_DOWNLEFT);
    table.insert(danmakuMap, bullet);
    effect_Smoke1(rootPoint.place.x - sizeofField + i, rootPoint.place.y + sizeofField - 1);
    bullet = bulletSummon(LandformPlace(rootPoint.place.x - sizeofField + i, rootPoint.place.y + sizeofField - 1), ValiableConstant2, CHARACTER_FORSE_ENEMY, -2, bulletType.straight, BulletColor.green, ASPECT_UPLEFT);
    table.insert(danmakuMap, bullet);
    
    for i=2,num do
        effect_Smoke1(rootPoint.place.x - sizeofField + i, rootPoint.place.y - sizeofField + 1);
        bullet = bulletSummon(LandformPlace(rootPoint.place.x - sizeofField + i, rootPoint.place.y - sizeofField + 1), ValiableConstant2, CHARACTER_FORSE_ENEMY, -2, bulletType.straight, BulletColor.black, ASPECT_DOWN);
        table.insert(danmakuMap, bullet);
        effect_Smoke1(rootPoint.place.x - sizeofField + i, rootPoint.place.y + sizeofField - 1);
        bullet = bulletSummon(LandformPlace(rootPoint.place.x - sizeofField + i, rootPoint.place.y + sizeofField - 1), ValiableConstant2, CHARACTER_FORSE_ENEMY, -2, bulletType.straight, BulletColor.black, ASPECT_UP);
        table.insert(danmakuMap, bullet); 
   end
    
    num = sizeofField*2 - 3;
    for i=1,num do
        effect_Smoke1(rootPoint.place.x - sizeofField + 1,rootPoint.place.y - sizeofField + 1 + i);
        bullet = bulletSummon(LandformPlace(rootPoint.place.x - sizeofField + 1,rootPoint.place.y - sizeofField + 1 + i), ValiableConstant2, CHARACTER_FORSE_ENEMY, -2, bulletType.straight, BulletColor.black, ASPECT_RIGHT);
        table.insert(danmakuMap, bullet);
        effect_Smoke1(rootPoint.place.x + sizeofField - 1,rootPoint.place.y - sizeofField + 1 + i);
        bullet = bulletSummon(LandformPlace(rootPoint.place.x + sizeofField - 1,rootPoint.place.y - sizeofField + 1 + i), ValiableConstant2, CHARACTER_FORSE_ENEMY, -2, bulletType.straight, BulletColor.black, ASPECT_LEFT);
        table.insert(danmakuMap, bullet);
    end
end

function putUpBarrage()
    if (bossStateClinic > 0)
    then
        cureState();
    else
        cureStateCheck();    
    end 
    
    stackDramaThread(coroutine.create(danmaku_setting));
    
    --if(spellTurnCount%10 == 0)then
        --CharacterCureMental(spawnFloorNativeEnemy(LandformRandom_RoomRandom_OutOfSight_Safe(LandformCharacterPosition(PlayerCharacter())),CHARACTER_FORSE_ENEMY));
    --end
end



function startSpell()
    --outputDebugString("startSpell");
    BossCurrentHP = boss.HP;
    --effect_Smoke1(warpland.place.x, warpland.place.y)
    --CharacterWarp(boss, warpland)
    CharacterTurn(boss,ASPECT_DOWN)

    CharacterWarp(PlayerCharacter(), LandformPlace(20,20));
    rootPoint = LandformPlace(20,20);

    randomArray = {1,2,3,4,5,6,7,8};

    for i=1,7
    do
        randomnum = math.floor( random() * (9 - i)) + i;
        randomArray[i], randomArray[randomnum] = randomArray[randomnum], randomArray[i];
    end

    enemySpawnFunc[randomArray[1]] =enemy1;
    enemySpawnFunc[randomArray[2]] =enemy2;
    enemySpawnFunc[randomArray[3]] =enemy3;
    enemySpawnFunc[randomArray[4]] =enemy5;
    enemySpawnFunc[randomArray[5]] =enemy6;
    enemySpawnFunc[randomArray[6]] =enemy7;
    enemySpawnFunc[randomArray[7]] =enemy8;
    enemySpawnFunc[randomArray[8]] =enemy9;
    --enemySpawnFunc[randomArray[9]] =enemy9;
    

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

function enemySpawn(enemyCount)
    enemyWay=math.floor( (random() * 4) ) * 2;
    pland = landformSearchObstacleWithRange(rootPoint, enemyWay, MAPHEIGHT, true, false, false, false);
    coo = Coordinate(rootPoint.place.x - pland.place.x, rootPoint.place.y - pland.place.y);
    
    while (coo:dif() < sizeofField)
    do
        enemyWay=math.floor( (random() * 4) ) * 2;
        pland = landformSearchObstacleWithRange(rootPoint, enemyWay, MAPHEIGHT, true, false, false, false);
        coo = Coordinate(rootPoint.place.x - pland.place.x, rootPoint.place.y - pland.place.y);
    end

    enemyPlace={};
    
    enemyPlace[1] = LandformNearby(rootPoint, enemyWay , sizeofField - 2);
    enemyPlace[2] = LandformNearby(enemyPlace[1], (enemyWay + 2) % 8, 2);
    enemyPlace[3] = LandformNearby(enemyPlace[1], (enemyWay + 6) % 8, 2);
    enemyPlace[4] = LandformNearby(enemyPlace[1], (enemyWay + 2) % 8, 4);
    enemyPlace[5] = LandformNearby(enemyPlace[1], (enemyWay + 6) % 8, 4);

    enemySpawnFunc[enemyCount](enemyPlace, enemyWay);
end

function enemyRingIN()
    enemyWay=math.floor( (random() * 4) ) * 2;
    pland = landformSearchObstacleWithRange(rootPoint, enemyWay, MAPHEIGHT, true, false, false, false);
    coo = Coordinate(rootPoint.place.x - pland.place.x, rootPoint.place.y - pland.place.y);
    
    while (coo:dif() < sizeofField)
    do
        enemyWay=math.floor( (random() * 4) ) * 2;
        pland = landformSearchObstacleWithRange(rootPoint, enemyWay, MAPHEIGHT, true, false, false, false);
        coo = Coordinate(rootPoint.place.x - pland.place.x, rootPoint.place.y - pland.place.y);
    end

    enemyPlace={};
    
    enemyPlace[1] = LandformNearby(rootPoint, enemyWay , sizeofField - 2);
    enemyPlace[2] = LandformNearby(enemyPlace[1], (enemyWay + 2) % 8, 2);
    enemyPlace[3] = LandformNearby(enemyPlace[1], (enemyWay + 6) % 8, 2);
    enemyPlace[4] = LandformNearby(enemyPlace[1], (enemyWay + 2) % 8, 4);
    enemyPlace[5] = LandformNearby(enemyPlace[1], (enemyWay + 6) % 8, 4);
    placeNum = 1;
    num = #surviveEnemy;
    for i=1, num
    do
        coo = Coordinate(rootPoint.place.x - surviveEnemy[i].placeX, rootPoint.place.y - surviveEnemy[i].placeY);
        if (coo:dif() > sizeofField)
        then
            CharacterWarp(surviveEnemy[i], enemyPlace[placeNum]);
            placeNum = placeNum + 1;
            
            if(placeNum > 5)
            then
                    enemyWay2=(enemyWay + 4) % 8;
                    pland = landformSearchObstacleWithRange(rootPoint, enemyWay2, MAPHEIGHT, true, false, false, false);
                    coo = Coordinate(rootPoint.place.x - pland.place.x, rootPoint.place.y - pland.place.y);
                    
                    if(coo:dif() < sizeofField)
                    then
                        enemyWay2=(enemyWay + 2) % 8;
                        pland = landformSearchObstacleWithRange(rootPoint, enemyWay2, MAPHEIGHT, true, false, false, false);
                        coo = Coordinate(rootPoint.place.x - pland.place.x, rootPoint.place.y - pland.place.y);
                    elseif(coo:dif() < sizeofField)
                    then
                        enemyWay2=(enemyWay + 6) % 8;
                        pland = landformSearchObstacleWithRange(rootPoint, enemyWay2, MAPHEIGHT, true, false, false, false);
                        coo = Coordinate(rootPoint.place.x - pland.place.x, rootPoint.place.y - pland.place.y);
                    end

                    enemyPlace={};
                    
                    enemyPlace[1] = LandformNearby(rootPoint, enemyWay2 , sizeofField - 2);
                    enemyPlace[2] = LandformNearby(enemyPlace[1], (enemyWay2 + 2) % 8, 2);
                    enemyPlace[3] = LandformNearby(enemyPlace[1], (enemyWay2 + 6) % 8, 2);
                    enemyPlace[4] = LandformNearby(enemyPlace[1], (enemyWay2 + 2) % 8, 4);
                    enemyPlace[5] = LandformNearby(enemyPlace[1], (enemyWay2 + 6) % 8, 4);
                    placeNum = 1;
            end
        end
    end
end

function danmaku_setting()

    num = #surviveEnemy;
    
    for i=1, num do
        if surviveEnemy[num - i + 1]
        then
            if (surviveEnemy[num - i + 1]:isDead())
            then    
                table.remove(surviveEnemy, num - i + 1);
            end    
        end
    end

    num = #surviveEnemy 

    if (num == 0)
    then
        CharacterForsedDamage(boss, boss.maxHP * 0.2);
    end

    player = PlayerCharacter();
    coo = Coordinate(rootPoint.place.x - player.placeX, rootPoint.place.y - player.placeY);
    
    if (coo:dif() >= sizeofField)
    then
        rootPoint = LandformPlace(player.placeX,player.placeY);
        fieldTurn = 0;
        enemyRingIN();
    end
    
    enemyRingIN();
    
    if(fieldTurn%3==0)
    then
        battleField();
    end
    
    --coroutine.yield(false);
    
    coo = Coordinate(rootPoint.place.x - boss.placeX, rootPoint.place.y - boss.placeY);
    
    if (coo:dif() <= sizeofField)
    then
        warpland = LandformRandom_Random();
        coo = Coordinate(rootPoint.place.x - warpland.place.x, rootPoint.place.y - warpland.place.y);
        
        while coo:dif() > sizeofField do
            warpland = LandformRandom_Random();
            coo = Coordinate(rootPoint.place.x - warpland.place.x, rootPoint.place.y - warpland.place.y);
        end
        
        effect_Smoke1(boss.placeX, boss.placeY);
        CharacterWarp(boss, warpland);
    end
    
    coroutine.yield(false);
    
    if(spellRestTurn / spellTurnLimitCount <= (1.2 - spawnOrder * 0.2)) or (boss.HP / boss.maxHP < (1.2 - spawnOrder * 0.2))
    then        
        enemySpawn(spawnOrder);
        spawnOrder = spawnOrder + 1;
    end
    
    fieldTurn = (fieldTurn + 1) % 3;
    
    if(spellTurnCount % 2) == 0
    then
        local pland = landformSearchObstacleWithRange(rootPoint, ASPECT_UP, MAPHEIGHT*2,true,false,false,false);
        pland = LandformNearby(pland, ASPECT_DOWNLEFT,1);
        bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.green, ASPECT_DOWNLEFT);
        
        pland = landformSearchObstacleWithRange(rootPoint, ASPECT_LEFT, MAPHEIGHT*2,true,false,false,false);
        pland = LandformNearby(pland, ASPECT_DOWNRIGHT,1);
        bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.green, ASPECT_DOWNRIGHT);

        pland = landformSearchObstacleWithRange(rootPoint, ASPECT_DOWN, MAPHEIGHT*2,true,false,false,false);
        pland = LandformNearby(pland, ASPECT_UPRIGHT,1);
        bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.green, ASPECT_UPRIGHT);

        pland = landformSearchObstacleWithRange(rootPoint, ASPECT_RIGHT, MAPHEIGHT*2,true,false,false,false);
        pland = LandformNearby(pland, ASPECT_UPLEFT,1);
        bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.green, ASPECT_UPLEFT);
    end
    
    CharacterTurnNow(boss, aspectOfBetweenCharacters(boss, PlayerCharacter())); 
    
    return true;
end

----------------------------------------------------
function enemy1(enemyPlace, enemyWay)
    num=1;
    if(spellRestTurn / spellTurnLimitCount < 0.4) or (boss.HP / boss.maxHP < 0.4)
    then
        newEnemy = MakeCharacter(2035, 2, CHARACTER_FORSE_ENEMY, enemyPlace[1]);--orin
        table.insert(surviveEnemy, newEnemy);
        num=2;
    end
    
    newEnemy = MakeCharacter(2012, 2, CHARACTER_FORSE_ENEMY, enemyPlace[num]);--yomu
    table.insert(surviveEnemy, newEnemy);
    
    newEnemy = MakeCharacter(2013, 2, CHARACTER_FORSE_ENEMY, enemyPlace[num+1]);--chiruno
    table.insert(surviveEnemy, newEnemy);
end

function enemy2(enemyPlace, enemyWay)
    num=1;
    if(spellRestTurn / spellTurnLimitCount < 0.4) or (boss.HP / boss.maxHP < 0.4)
    then
        newEnemy = MakeCharacter(2035, 2, CHARACTER_FORSE_ENEMY, enemyPlace[1]);--orin
        num=2;
    end
    newEnemy = MakeCharacter(2011, 2, CHARACTER_FORSE_ENEMY, enemyPlace[num]);--yuyuko
    table.insert(surviveEnemy, newEnemy);
    newEnemy = MakeCharacter(2067, 2, CHARACTER_FORSE_ENEMY, enemyPlace[num+1]);--kanako
    table.insert(surviveEnemy, newEnemy);
    newEnemy = MakeCharacter(2047, 2, CHARACTER_FORSE_ENEMY, enemyPlace[num+2]);--kaguya
    table.insert(surviveEnemy, newEnemy);
end

function enemy3(enemyPlace, enemyWay)
    num=1;
    if(spellRestTurn / spellTurnLimitCount < 0.4) or (boss.HP / boss.maxHP < 0.4)
    then
        newEnemy = MakeCharacter(2035, 2, CHARACTER_FORSE_ENEMY, enemyPlace[1]);--orin
        num=2;
    end
    newEnemy = MakeCharacter(2038, 2, CHARACTER_FORSE_ENEMY, enemyPlace[num]);--koisi
    table.insert(surviveEnemy, newEnemy);
    newEnemy = MakeCharacter(2075, 2, CHARACTER_FORSE_ENEMY, enemyPlace[num+1]);--kene
    table.insert(surviveEnemy, newEnemy);
end

function enemy4(enemyPlace, enemyWay)
    num=1;
    if(spellRestTurn / spellTurnLimitCount < 0.4) or (boss.HP / boss.maxHP < 0.4)
    then
        newEnemy = MakeCharacter(2035, 2, CHARACTER_FORSE_ENEMY, enemyPlace[1]);--orin
        num=2;
    end
    newEnemy = MakeCharacter(2041, 2, CHARACTER_FORSE_ENEMY, enemyPlace[num]);--alice
    table.insert(surviveEnemy, newEnemy);
    newEnemy = MakeCharacter(2036, 2, CHARACTER_FORSE_ENEMY, enemyPlace[num+1]);--yamame
    table.insert(surviveEnemy, newEnemy);
end

function enemy5(enemyPlace, enemyWay)
    num=1;
    if(spellRestTurn / spellTurnLimitCount < 0.4) or (boss.HP / boss.maxHP < 0.4)
    then
        newEnemy = MakeCharacter(2035, 2, CHARACTER_FORSE_ENEMY, enemyPlace[1]);--orin
        num=2;
    end
    newEnemy = MakeCharacter(2072, 2, CHARACTER_FORSE_ENEMY, enemyPlace[num]);--3gatsu sunny milk
    table.insert(surviveEnemy, newEnemy);
    newEnemy = searchOneCharacterOnFloor(2073);--lunachild
    if(newEnemy)
    then
        table.insert(surviveEnemy, newEnemy);
    end 
    newEnemy = searchOneCharacterOnFloor(2074);--star sapphire
    if(newEnemy)
    then
        table.insert(surviveEnemy, newEnemy);
    end 
    newEnemy = MakeCharacter(2054, 2, CHARACTER_FORSE_ENEMY, enemyPlace[num+1]);--runasa
    table.insert(surviveEnemy, newEnemy);
    newEnemy = searchOneCharacterOnFloor(2052);--meruran
    if(newEnemy)
    then
        table.insert(surviveEnemy, newEnemy);
    end 
    newEnemy = searchOneCharacterOnFloor(2053);--ririka
    if(newEnemy)
    then
        table.insert(surviveEnemy, newEnemy);
    end       
    newEnemy = MakeCharacter(2029, 2, CHARACTER_FORSE_ENEMY, enemyPlace[num+2]);--tenko
    table.insert(surviveEnemy, newEnemy);
    newEnemy = MakeCharacter(2005, 2, CHARACTER_FORSE_ENEMY, enemyPlace[num+3]);--huran
    table.insert(surviveEnemy, newEnemy);
end

function enemy6(enemyPlace, enemyWay)
    num=1;
    if(spellRestTurn / spellTurnLimitCount < 0.4) or (boss.HP / boss.maxHP < 0.4)
    then
        newEnemy = MakeCharacter(2035, 0, CHARACTER_FORSE_ENEMY, enemyPlace[1]);--orin
        num=2;
    end
    newEnemy = MakeCharacter(2051, 0, CHARACTER_FORSE_ENEMY, enemyPlace[num]);--misstia
    table.insert(surviveEnemy, newEnemy);
    newEnemy = MakeCharacter(2034, 0, CHARACTER_FORSE_ENEMY, enemyPlace[num+1]);--kisume
    table.insert(surviveEnemy, newEnemy);
    newEnemy = MakeCharacter(2043, 0, CHARACTER_FORSE_ENEMY, enemyPlace[num+2]);--chen
    table.insert(surviveEnemy, newEnemy);
    newEnemy = MakeCharacter(2007, 0, CHARACTER_FORSE_ENEMY, enemyPlace[num+3]);--rumia
    table.insert(surviveEnemy, newEnemy);
end

function enemy7(enemyPlace, enemyWay)
    num=1;
    if(spellRestTurn / spellTurnLimitCount < 0.4) or (boss.HP / boss.maxHP < 0.4)
    then
        newEnemy = MakeCharacter(2035, 2, CHARACTER_FORSE_ENEMY, enemyPlace[1]);--orin
        num=2;
    end
    newEnemy = MakeCharacter(2055, 2, CHARACTER_FORSE_ENEMY, enemyPlace[num]);--medi
    table.insert(surviveEnemy, newEnemy);
    newEnemy = MakeCharacter(2030, 2, CHARACTER_FORSE_ENEMY, enemyPlace[num+1]);--parusi
    table.insert(surviveEnemy, newEnemy);
    newEnemy = MakeCharacter(2056, 2, CHARACTER_FORSE_ENEMY, enemyPlace[num+2]);--yuka
    table.insert(surviveEnemy, newEnemy);
end

function enemy8(enemyPlace, enemyWay)
    num=1;
    if(spellRestTurn / spellTurnLimitCount < 0.4) or (boss.HP / boss.maxHP < 0.4)
    then
        newEnemy = MakeCharacter(2035, 2, CHARACTER_FORSE_ENEMY, enemyPlace[1]);--orin
        num=2;
    end
    newEnemy = MakeCharacter(2032, 2, CHARACTER_FORSE_ENEMY, enemyPlace[num]);--momiji
    table.insert(surviveEnemy, newEnemy);
    newEnemy = MakeCharacter(2022, 2, CHARACTER_FORSE_ENEMY, enemyPlace[num+1]);--sanae
    table.insert(surviveEnemy, newEnemy);
end

function enemy9(enemyPlace, enemyWay)
    num=1;
    if(spellRestTurn / spellTurnLimitCount < 0.4) or (boss.HP / boss.maxHP < 0.4)
    then
        newEnemy = MakeCharacter(2035, 2, CHARACTER_FORSE_ENEMY, enemyPlace[1]);--orin
        num=2;
    end
    newEnemy = MakeCharacter(2018, 2, CHARACTER_FORSE_ENEMY, enemyPlace[num]);--aya
    table.insert(surviveEnemy, newEnemy);
    newEnemy = MakeCharacter(2027, 2, CHARACTER_FORSE_ENEMY, enemyPlace[num+1]);--suica
    table.insert(surviveEnemy, newEnemy);
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