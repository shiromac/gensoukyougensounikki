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

turnCount = 0;

bossStateClinic = 0;

bulletDamageConstant2 = 10
--valueFunction = function(Landform)

function bossMoveTargetLandform()
    local player = PlayerCharacter();
    local partner = searchOneCharacterOnFloor(5011);
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
    if (turnCount % 6) == 0
    then
        turnCount = turnCount+1;
        return YES;
    else
        turnCount = turnCount+1;
        return NO;
    end
end

function skillEffect(valiable)
    CharacterTurnNow(boss, aspectOfBetweenCharacters(boss, PlayerCharacter())); 
    lazerSummon(landformAroundOfCharacter(boss, 7, 1), (boss.aspect+2)%8,2, ValiableConstant3, CHARACTER_FORSE_ENEMY, Color(255,255,255,137), 0, 0, false, true);
    bulletSummon(landformAroundOfCharacter(boss, 7, 1), bulletDamageConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.cyan, boss.aspect);
    bulletSummon(landformAroundOfCharacter(boss, 0, 1), bulletDamageConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.cyan, boss.aspect);
    bulletSummon(landformAroundOfCharacter(boss, 1, 1), bulletDamageConstant2, CHARACTER_FORSE_ENEMY, 0, bulletType.straight, BulletColor.cyan, boss.aspect);

    return YES;
end

function cureStateCheck()
    if(CharacterIsMentalAberration(boss) == YES) or (CharacterIsPhysicalAberration(boss) == YES) 
        or (CharacterIsMagicAberration(boss) == YES) or (CharacterIsSpeedAberration(boss) == YES)
    then
        bossStateClinic = 4
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

function passiveSkill(timing,valiable)
    if(timing == Timing.turn_end)
    then
        if (bossStateClinic > 0)
        then
            cureState();
        else
            cureStateCheck();    
        end    
    end
end