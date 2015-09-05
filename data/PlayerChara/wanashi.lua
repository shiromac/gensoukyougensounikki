-- cCharacter me: me character

function cutIn(timing, valiable)
    --[[
    if (timing == Timing.item_pickup_judge) then
        -- valiable.doubles: val(ValiableIndex.generic_bool) : enablePickUp
        -- valiable.drops: val(ValiableIndex.targetdrop) : item picked up
        -- valiable.charas: val(ValiableIndex.targetchara) : chara picked up
        pickUpItem = valiable.drops: val(ValiableIndex.targetdrop)
        
        if (pickUpItem.attribute: count(DROPING_ATTRIBUTE_TRAP) > 0) then
            valiable.doubles: setVal(ValiableIndex.generic_bool, YES)
        end
    
    end
    ]]
    if (timing == Timing.trap_target_judge) then
        -- valiable.doubles: val(ValiableIndex.generic_bool) : isTarget
        -- valiable.drops: val(ValiableIndex.targetdrop) : trap
        -- valiable.charas: val(ValiableIndex.targetchara) : chara picked up
        
        isTarget = isEnemyCharacter(me, valiable.charas: val(ValiableIndex.targetchara))
        if valiable.charas: val(ValiableIndex.targetchara): characterAttribute(): count(CharacterAttribute.barrage) > 0 then
            isTarget = false
        end
        isTargetValue = 0
        if (isTarget) then
            isTargetValue = 1
        end
        valiable.doubles: setVal(ValiableIndex.generic_bool, isTargetValue)
    end
    if (timing == Timing.experience_get_judge) then
        -- valiable.doubles: val(ValiableIndex.generic_bool) : get experience
        -- valiable.doubles: val(ValiableIndex.generic_value) : experience value
        -- valiable.charas: val(ValiableIndex.attacker) : character getting exp
        -- valiable.charas: val(ValiableIndex.defenser) : defeated character

        if ( valiable.charas: val(ValiableIndex.attacker) == nil ) then
            valiable.charas: setVal(ValiableIndex.attacker, PlayerCharacter())
            valiable.doubles: setVal(ValiableIndex.generic_bool, YES)
            --CharacterGetExperience(PlayerCharacter(), valiable.doubles: val(ValiableIndex.generic_value) )
        end
    end
end