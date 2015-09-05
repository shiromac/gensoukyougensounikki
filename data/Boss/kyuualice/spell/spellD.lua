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

imageColor1 = Color(255,255,0,225)
imageColor2 = Color(255,128,128,128)

danmaku_phase_first = 1
danmaku_phase_middle = 2
danmaku_phase_last = 3

rollingMode = 0;
danmaku_state = rollingMode;

bossRootPoint = nil;

danmaku_turn = 8;
danmaku_cycle = 16;

targetLandNow = nil;


function bossMoveTargetLandform()
    if danmaku_state == rollingMode
    then
        coo = Coordinate(boss.placeX - targetLandNow.place.x, boss.placeY - targetLandNow.place.y);
        if(coo:dif() <= 1)
        then
            randnum = math.floor( (random() * 5) ) - 2;
            pland = LandformNearby(bossRootPoint,0,randnum);
            if(targetLandNow.place.x > bossRootPoint.place.x)
            then
                targetLandNow = LandformNearby(pland,6,6);
            else    
                targetLandNow = LandformNearby(pland,2,6);
            end    
        end
        
        return targetLandNow;    
    else
        return LandformEdge();
    end    
end

function AISkillChoise(valiable)
    return NO;
end

function skillEffect(valiable)

    AIshortCut_NomoveAttack(boss,valiable);
    
    
    return YES;
end

function passiveSkill(timing,valiable)
    --CharacterTurn(boss,ASPECT_DOWN);
end


function putUpBarrage()
    cureState(); 
    
    if(CharacterIsSpeedAberration(PlayerCharacter()) == NO)
    then
        CharacterSpeedUp(PlayerCharacter(), 999);
    end
    
    --if(spellTurnCount%10 == 0)then
        stackDramaThread(coroutine.create(danmaku_setting));
    --end
    
    if(spellTurnCount%8 == 0)then
        CharacterCureMental(spawnFloorNativeEnemy(LandformRandom_RoomRandom_OutOfSight_Safe(LandformCharacterPosition(PlayerCharacter())),CHARACTER_FORSE_ENEMY));
    end
    
    if(spellTurnCount%4 == 0)then
        spawnEnemy();
    end
    
    if(spellRestTurn / spellTurnLimitCount < 0.4)
    then
        if(spellTurnCount%3 == 0)then
            spawnEnemy();
        end
    end
end



function startSpell()
    eraseDanmaku(boss);
    --outputDebugString("startSpell");
    bossRootPoint = LandformPlace(20, 16);
    randnum = math.floor( (random() * 5) ) - 2;
    pland = LandformNearby(bossRootPoint,0,randnum);
    randnum = math.floor( (random() * 2) );
    if randnum == 1
    then
        targetLandNow = LandformNearby(pland,2,6);
    else
        targetLandNow = LandformNearby(pland,6,6);
    end
   
    CharacterWarp(boss, LandformPlace(20,16));
    CharacterTurn(boss,ASPECT_DOWN)
    
    CharacterMatchless(boss, 999);    

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

function danmaku_setting()  
    rollingBullet(danmaku_turn);
    rollingLazer(danmaku_turn, imageColor1);
    rollingLazer((danmaku_turn+4)%8, imageColor2);
    danmaku_turn = (danmaku_turn + 1) % danmaku_cycle;
    
    if(spellRestTurn / spellTurnLimitCount < 0.4)
    then
        rollingBullet((danmaku_turn+2)%8);
    end
    return true;
end    

function rollingLazer(turn, lazer_color)
    if (turn == 0) or (turn == 8)
    then
        return
    end
    
    if turn >= 8
    then
        pland = LandformNearby(LandformPlace(boss.placeX, boss.placeY), turn%8,1);
        lazerSummon(pland, turn%8,MAPHEIGHT*2, 40, CHARACTER_FORSE_ENEMY, lazer_color, 2, 7, false, true);
   else
        pland = LandformNearby(LandformPlace(boss.placeX, boss.placeY), (16-turn)%8,1);
        lazerSummon(pland, (16-turn)%8,MAPHEIGHT*2, 40, CHARACTER_FORSE_ENEMY, lazer_color, 2, 7, false, true);
    end
end

function rollingBullet(turn)
    if( (turn - 1) % 2) == 1
    then
        bullet_color1 = BulletColor.black;
        bullet_color2 = BulletColor.magenta;
    else
        bullet_color1 = BulletColor.magenta;
        bullet_color2 = BulletColor.black;
    end
    
    if(turn % 2) == 1
    then    
        pland = LandformNearby(LandformPlace(boss.placeX, boss.placeY), ASPECT_DOWN,1);
        bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.homing, bullet_color1, ASPECT_DOWN);
        pland = LandformNearby(LandformPlace(boss.placeX, boss.placeY), ASPECT_LEFT,1);
        bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.homing, bullet_color2, ASPECT_LEFT);    
        pland = LandformNearby(LandformPlace(boss.placeX, boss.placeY), ASPECT_UP,1);
        bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.homing, bullet_color1, ASPECT_UP);    
        pland = LandformNearby(LandformPlace(boss.placeX, boss.placeY), ASPECT_RIGHT,1);
        bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.homing, bullet_color2, ASPECT_RIGHT);    
    else
        pland = LandformNearby(LandformPlace(boss.placeX, boss.placeY), ASPECT_DOWNLEFT,1);
        bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.homing, bullet_color1, ASPECT_DOWNLEFT);
        pland = LandformNearby(LandformPlace(boss.placeX, boss.placeY), ASPECT_UPLEFT,1);
        bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.homing, bullet_color2, ASPECT_UPLEFT);    
        pland = LandformNearby(LandformPlace(boss.placeX, boss.placeY), ASPECT_UPRIGHT,1);
        bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.homing, bullet_color1, ASPECT_UPRIGHT);    
        pland = LandformNearby(LandformPlace(boss.placeX, boss.placeY), ASPECT_DOWNRIGHT,1);
        bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.homing, bullet_color2, ASPECT_DOWNRIGHT);     
    end

    if(turn >= 4) 
    then
        bullet_color1 = BulletColor.black;
        bullet_color2 = BulletColor.magenta;
    else
        bullet_color1 = BulletColor.magenta;
        bullet_color2 = BulletColor.black;
    end

    if(turn % 4) == 0
    then
        pland = LandformNearby(LandformPlace(boss.placeX, boss.placeY), ASPECT_DOWN,3);
        bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.homing, bullet_color1, ASPECT_DOWN);
        pland = LandformNearby(LandformPlace(boss.placeX, boss.placeY), ASPECT_LEFT,3);
        bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.homing, bullet_color2, ASPECT_LEFT);    
        pland = LandformNearby(LandformPlace(boss.placeX, boss.placeY), ASPECT_UP,3);
        bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.homing, bullet_color1, ASPECT_UP);    
        pland = LandformNearby(LandformPlace(boss.placeX, boss.placeY), ASPECT_RIGHT,3);
        bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.homing, bullet_color2, ASPECT_RIGHT);    
    elseif(turn % 4) == 1
    then
        pland = LandformNearby(LandformPlace(boss.placeX, boss.placeY), ASPECT_DOWN,3);
        pland = LandformNearby(pland, ASPECT_LEFT,1);    
        bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.homing, bullet_color1, ASPECT_DOWNLEFT);    
        pland = LandformNearby(LandformPlace(boss.placeX, boss.placeY), ASPECT_LEFT,3);
        pland = LandformNearby(pland, ASPECT_UP,1);
        bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.homing, bullet_color2, ASPECT_UPLEFT);    
        pland = LandformNearby(LandformPlace(boss.placeX, boss.placeY), ASPECT_UP,3);
        pland = LandformNearby(pland, ASPECT_RIGHT,1);
        bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.homing, bullet_color1, ASPECT_UPRIGHT);    
        pland = LandformNearby(LandformPlace(boss.placeX, boss.placeY), ASPECT_RIGHT,3);
        pland = LandformNearby(pland, ASPECT_DOWN,1);
        bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.homing, bullet_color2, ASPECT_DOWNRIGHT);       
    elseif(turn % 4) == 2
    then
        pland = LandformNearby(LandformPlace(boss.placeX, boss.placeY), ASPECT_DOWNLEFT,2);
        bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.homing, bullet_color1, ASPECT_DOWNLEFT);    
        pland = LandformNearby(LandformPlace(boss.placeX, boss.placeY), ASPECT_UPLEFT,2);
        bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.homing, bullet_color2, ASPECT_UPLEFT);    
        pland = LandformNearby(LandformPlace(boss.placeX, boss.placeY), ASPECT_UPRIGHT,2);
        bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.homing, bullet_color1, ASPECT_UPRIGHT);    
        pland = LandformNearby(LandformPlace(boss.placeX, boss.placeY), ASPECT_DOWNRIGHT,2);
        bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.homing, bullet_color2, ASPECT_DOWNRIGHT);    
    else
        pland = LandformNearby(LandformPlace(boss.placeX, boss.placeY), ASPECT_DOWN,1);
        pland = LandformNearby(pland, ASPECT_LEFT,3);    
        bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.homing, bullet_color1, ASPECT_DOWNLEFT);    
        pland = LandformNearby(LandformPlace(boss.placeX, boss.placeY), ASPECT_LEFT,1);
        pland = LandformNearby(pland, ASPECT_UP,3);
        bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.homing, bullet_color2, ASPECT_UPLEFT);    
        pland = LandformNearby(LandformPlace(boss.placeX, boss.placeY), ASPECT_UP,1);
        pland = LandformNearby(pland, ASPECT_RIGHT,3);
        bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.homing, bullet_color1, ASPECT_UPRIGHT);    
        pland = LandformNearby(LandformPlace(boss.placeX, boss.placeY), ASPECT_RIGHT,1);
        pland = LandformNearby(pland, ASPECT_DOWN,3);
        bulletSummon(pland, ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.homing, bullet_color2, ASPECT_DOWNRIGHT);      
    end
end

function cureState()
    if(CharacterIsMentalAberration(boss) == YES) or (CharacterIsPhysicalAberration(boss) == YES) 
        or (CharacterIsMagicAberration(boss) == YES) or (CharacterIsSpeedAberration(boss) == YES)
    then
        CharacterCureMental(boss);
        CharacterCurePhysical(boss);
        CharacterCureMagic(boss);
        CharacterCureSpeed(boss); 
    end

end



function spawnEnemy()
    local randnum = math.floor( (random() * 4) );
    local pland = LandformRandom_RoomRandom_OutOfSight_Safe(LandformCharacterPosition(PlayerCharacter()));
    
    if(randnum == 0)
    then
        MakeCharacter(2027, 2, CHARACTER_FORSE_ENEMY, pland);
    elseif(randnum == 1)   
    then
        MakeCharacter(2000, 3, CHARACTER_FORSE_ENEMY, pland);
    elseif(randnum == 2)  
    then
        MakeCharacter(2012, 2, CHARACTER_FORSE_ENEMY, pland);
    else
        MakeCharacter(2033, 2, CHARACTER_FORSE_ENEMY, pland);
    end
end

function spawnEnemyHakkyo()
    local randnum = math.floor( (random() * 4) );
    local pland = LandformRandom_RoomRandom_OutOfSight_Safe(LandformCharacterPosition(PlayerCharacter()));
    
    if(randnum == 0)
    then
        MakeCharacter(2027, 2, CHARACTER_FORSE_ENEMY, pland);
    elseif(randnum == 1)   
    then
        MakeCharacter(2021, 2, CHARACTER_FORSE_ENEMY, pland);
    elseif(randnum == 2)  
    then
        MakeCharacter(2012, 2, CHARACTER_FORSE_ENEMY, pland);
    else
        MakeCharacter(2033, 2, CHARACTER_FORSE_ENEMY, pland);
    end
end