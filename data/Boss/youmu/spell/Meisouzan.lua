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

--valueFunction = function(Landform)

function bossMoveTargetLandform()
    pland = LandformPlace(PlayerCharacter().placeX, PlayerCharacter().placeY);
    if(pland)
    then
        coo = Coordinate(boss.placeX - pland.place.x, boss.placeY - pland.place.y)
    
        if(coo:dif() > 2)
        then
            target1=landformAroundOfCharacter(PlayerCharacter(),(aspectOfBetweenCharacters(boss, PlayerCharacter())+3)%8,1);
            target2=landformAroundOfCharacter(PlayerCharacter(),(aspectOfBetweenCharacters(boss, PlayerCharacter())+5)%8,1);
            
            dis1=(boss.placeX - target1.place.x)*(boss.placeX - target1.place.x) + (boss.placeY - target1.place.y)*(boss.placeY - target1.place.y);
            dis2=(boss.placeX - target2.place.x)*(boss.placeX - target2.place.x) + (boss.placeY - target2.place.y)*(boss.placeY - target2.place.y);
            
            if(dis1 > dis2)
            then   
                return target2;
            else
                return target1;
            end
        end
    end

    return LandformEdge();
end

function AISkillChoise(valiable)
    return skillEffectSub(valiable)
end

skillEffected = false;
function skillEffectSub(valiable)
    local result, returnValue = coroutine.resume(bossMainLoop)
    if returnValue == YES then
        skillEffected = true
    end
    return returnValue
end

gSwordTurnRight = true
gSwordAspectStart = ASPECT_DOWN
gSwordAspectDegree = 3
gSwordLength = 3
function eightBullet()
    local aspect = 0
    local bossLand = LandformCharacterPosition(boss)
    local playerLand = LandformCharacterPosition(PlayerCharacter())
    if (random() > 0.4) then
        gSwordTurnRight = true
    else
        gSwordTurnRight = false
    end
    gSwordAspectStart = (playerLand.place - bossLand.place): getAspect()
    local aspectTurn = - 1
    if gSwordTurnRight then
        aspectTurn = 1
    end
    
    local count = 0
    for count = 0 , 7 do
        if (count < gSwordAspectDegree) then
            bulletColor = BulletColor.magenta
        else
            bulletColor = BulletColor.cyan
        end
        local aspect = gSwordAspectStart + count * aspectTurn
        bulletSummon(LandformNearby(bossLand, aspect, 1), ValiableConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, bulletColor, aspect);
    end
end

function baramakiBullet(baramakiBulletLength)
    local bossLand = LandformCharacterPosition(boss)
    for aspect = 0 , 7 do
        local targetLand = LandformNearby(bossLand, aspect +2, 1)
        bulletSummon(LandformNearby(targetLand, aspect +4, baramakiBulletLength), ValiableConstant2, CHARACTER_FORSE_ENEMY, 1, bulletType.straight, BulletColor.green, aspect);
    end
end

function buttagiriLazer()
    local bossLand = LandformCharacterPosition(boss)
    local startLand = LandformNearby(bossLand, gSwordAspectStart, 0)
    effect_boss_focus(boss.placeX,boss.placeY);
    lazerSummon(startLand, gSwordAspectStart, 50, 40, CHARACTER_FORSE_ENEMY, Color(255,255,255,137), 2, 0, false, true);
end

function absoluteCrossProduct(a,b)
--Coordinate a, b
    return a.x*b.y - a.y*b.x
end
function absoluteCrossProduct(bossLand,targetLand,aspectStart, aspectDegree)
--Coordinate a, b
end

function buttagiriEffect(x,y)
    local toPlaceX = x +5
    local toPlaceY = y +5
    local hab = AnimeChip_Hab(PositionPerGrid(toPlaceX, toPlaceY, 0.3))
    local width = (gSwordLength*2+1)*64*0.9
    local life = 40
    local degreeSign = -1
    if (gSwordTurnRight) then
        degreeSign = 1
    end
    local degreeEnd = gSwordAspectStart * 45 - 90 + degreeSign * 45
    local degreeBegin = degreeEnd + 45*(gSwordAspectDegree-1 +0.5) * degreeSign
    local sword = AnimeChip_Perticle("effect\\meisouzan.png",D3DXVECTOR2(width,width), life)
    sword.color: addCascade(Color(0, 200, 50, 200) , Color(255, 200, 50, 200), life*0.1)
    sword.color: addCascade(Color(255, 200, 50, 200) , life*0.2)
    sword.color: addCascade(Color(255, 200, 50, 200) , Color(0, 200, 50, 200), life*0.7)
    sword.rotationBefore: addCascade(degreeBegin , degreeEnd, life*0.3)
    sword.rotationBefore: addCascade(degreeEnd , life*0.7)
    if (gSwordTurnRight) then
        sword.textureRange: setDefault(TextureRangeRect(0,1,1,0))
    end
    local sword2 = AnimeChip_Perticle(sword)
    sword2.drawMode: setDefault(DrawMode_addition)
    sword2.scaleX: addCascade(1.0, 1.2, 4)
    sword2.scaleX: setOutRangeMode(OutRangeMode_Wrap)
    sword2.scaleY: addCascade(1.0, 1.2, 4)
    sword2.scaleY: setOutRangeMode(OutRangeMode_Wrap)
    
    local sword_delay = AnimeChip_Perticle(sword)
    sword_delay: setDelay(5)
    local sword2_delay = AnimeChip_Perticle(sword2)
    sword2_delay: setDelay(5)
    hab: addAnimeChip(sword);
    hab: addAnimeChip(sword2);
    hab: addAnimeChip(sword_delay);
    hab: addAnimeChip(sword2_delay);
    setAnimation_parallel(hab)
end

function buttagiriSword()
    local bossLand = LandformCharacterPosition(boss)
    local startLand = LandformNearby(bossLand, gSwordAspectStart, 0)
    effect_boss_emission(boss.placeX,boss.placeY,imageColor1,imageColor2);
    buttagiriEffect(boss.placeX,boss.placeY)
    for x = -gSwordLength , gSwordLength do
        for y = -gSwordLength , gSwordLength do
            local targetLand = LandformPlace(bossLand.place.x + x, bossLand.place.y + y)
            --void attackHappen(pcCharacter pchara, pcLandform pland, double power, int constFlag, vector<int> attackAttributes, bool surelyHitFlag)
            attackAttributes = vector_int()
            attackAttributes: push_back(AttackAttribute.soul)
            local targetChara = targetLand.mountedCharacter
            local skipFlag = false
            if (targetChara) then
                if (targetChara: isEqualTo(boss)) then
                    skipFlag = true
                end
            end
            local startAspectCoordinate = Coordinate(0,0)
            local aspectCoordinate = Coordinate(x, y)
            startAspectCoordinate: setAspect(gSwordAspectStart)
            local degree = startAspectCoordinate: calcuDegree(aspectCoordinate)
            if not gSwordTurnRight then
                degree = -degree
            end
            if degree < -1.0 then
                skipFlag = true
            end
            if degree > (gSwordAspectDegree-1)*45 + 1.0 then
                skipFlag = true
            end
            distance_pow2 = x*x + y*y
            if (distance_pow2 > (gSwordLength+0.3)*(gSwordLength+0.3)) then
                skipFlag = true
            end
            if not(skipFlag) then
                attackHappen(boss, targetLand, 100, 0, attackAttributes, false)
            end
        end
    end
end

function youmuSlash()
    local distance = 5
    pland=landformSearchObstacleWithRange(LandformPlace(boss.placeX,boss.placeY), boss.aspect, distance+1, true, false, false, false)
    pland = LandformNearby(pland,((boss.aspect+4)% 8),1)
    lazerSummon(landformAroundOfCharacter(boss, 0, 1), boss.aspect,distance, 40, CHARACTER_FORSE_ENEMY, Color(255,255,255,137), 0, 0, true, true);
    CharacterWarp(boss, pland)
end



function isHakkyo()
    if (spellRestTurn / spellTurnLimitCount < 0.3) then
        return true
    end
    if (boss.HP / boss.maxHP < 0.3) then
        return true
    end
    return false
end
      
nextSkillEffectFunction = function () end
function bossMainLoopFunction()
    while true do
        nextSkillEffectFunction = eightBullet
        coroutine.yield(YES)
        coroutine.yield(NO)
        if not isHakkyo() then 
            coroutine.yield(NO)
        end
        
        nextSkillEffectFunction = (function ()
                if not isHakkyo() then 
                    gSwordAspectDegree = 3
                    gSwordLength = 3
                else
                    gSwordAspectDegree = 5
                    gSwordLength = 4
                end
                buttagiriLazer()
            end)
        coroutine.yield(YES)
        nextSkillEffectFunction = buttagiriSword
        coroutine.yield(YES)
        nextSkillEffectFunction = (function ()
                baramakiBullet(3)
            end)
        coroutine.yield(YES)
        nextSkillEffectFunction = (function ()
                baramakiBullet(4)
            end)
        coroutine.yield(YES)
        nextSkillEffectFunction = (function ()
                baramakiBullet(5)
                if isHakkyo() then 
                    effect_boss_charge(boss.placeX,boss.placeY,imageColor1,imageColor2);
                end
            end)
        coroutine.yield(YES)
        if not isHakkyo() then 
            coroutine.yield(NO)
        else
            nextSkillEffectFunction = (function ()
                youmuSlash()
            end)
            coroutine.yield(YES)
        end
        coroutine.yield(NO)
    end
end

function skillEffect(valiable)

    if(skillEffected)then
        nextSkillEffectFunction()
        skillEffected = false;
        return YES;
    end
    return NO;
end

function passiveSkill(timing,valiable)
end

function putUpBarrage()
    stackDramaThread(coroutine.create(danmaku_setting))
    if(spellTurnCount%10 == 0)then
        --CharacterCureMental(spawnFloorNativeEnemy(LandformRandom_RoomRandom_OutOfSight_Safe(LandformCharacterPosition(PlayerCharacter())),CHARACTER_FORSE_ENEMY));
    end
end

function startSpell()
    for i = 1, ValiableConstant4 do
        spawnFloorNativeItem(LandformRandom_Random())
    end
    --MakeCharacter(5005, 0, CHARACTER_FORSE_ENEMY, LandformPlace(boss.placeX+1,boss.placeY))
    bossMainLoop = coroutine.create(bossMainLoopFunction)
end

function endSpell()
    --outputDebugString("endSpell");
end

-----------------------------------------------------------------------

function danmaku_setting()
    return true;
end
