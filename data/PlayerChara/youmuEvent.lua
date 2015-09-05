KAMAE_SEME = 1;
KAMAE_UKE = 2;
g_kamae_mode = KAMAE_SEME;

function canTalk()
    return true
end
function talkEvent()

    turnCharacterForCharacter(me, PlayerCharacter())
    
    textCommandString = localizeString(_T("youmu_command_caption_changePlayerCharacter_caption"))
    textCommandStringExpranation = localizeString(_T("youmu_command_caption_changePlayerCharacter_ShortExplanation"))
    command = LuaFunctionCommand(textCommandString, changePlayerCharacterToMairin, textCommandStringExpranation)
    selectWindow = SelectWindow()
    selectWindow: addCommand(command)
    selectWindow: init(12,1)
    selectWindow: setLeft(menuDefaultPositionLeft(1))
    selectWindow: setTop(menuDefaultPositionTop(1))
    selectWindow: showWindow()

    menuRerenderAllWindows();

end
function changePlayerCharacterToMairin(freeValiable)
    if not (PlayerCharacter(): ID() == 1004)then
        swapPlayerCharacter(me)
    end
    menuDismissAllWindows()
end
function cutIn(timing, valiable)

    if (timing == Timing.equipment_equip_before) then
        if(valiable.doubles: val_str("both_hands") == YES)then
            valiable.doubles: setVal(ValiableIndex.generic_bool, NO)--cannot equip
            menuDismissAllWindows()
            str_map = map_tstring_StyleString()
            str_map: insert( pair_tstring_StyleString( _T("Chara"), me: fullName() ))
            utility_localizedMessage(_T("youmu_equipment_both_hands_fail"), str_map)
        end
    end
    if (timing == Timing.equipment_attack_mitama_enable) then
        if(g_kamae_mode == KAMAE_SEME)then
            if(valiable.drops: val(ValiableIndex.targetdrop))then
                if(valiable.drops: val(ValiableIndex.targetdrop): isEquipedForAttack() or valiable.drops: val(ValiableIndex.targetdrop).mitamaAbilityConstancy)then
                    valiable.doubles: dim(ValiableIndex.generic_bool, YES)
                end
            end
        elseif(g_kamae_mode == KAMAE_UKE)then
            valiable.doubles: dim(ValiableIndex.generic_bool, NO)
        end
    end
    if (timing == Timing.equipment_defence_mitama_enable) then
        if(g_kamae_mode == KAMAE_SEME)then
            valiable.doubles: dim(ValiableIndex.generic_bool, NO)
        elseif(g_kamae_mode == KAMAE_UKE)then
            valiable.doubles: dim(ValiableIndex.generic_bool, YES)
        end
    end
    if (timing == Timing.equipment_attack_mark_enable) then
        if(g_kamae_mode == KAMAE_SEME)then
            valiable.doubles: dim(ValiableIndex.generic_bool, YES)
        elseif(g_kamae_mode == KAMAE_UKE)then
            valiable.doubles: dim(ValiableIndex.generic_bool, NO)
        end
    end
    if (timing == Timing.equipment_defence_mark_enable) then
        if(g_kamae_mode == KAMAE_SEME)then
            valiable.doubles: dim(ValiableIndex.generic_bool, NO)
        elseif(g_kamae_mode == KAMAE_UKE)then
            valiable.doubles: dim(ValiableIndex.generic_bool, YES)
        end
    end
    if (timing == Timing.swap_equipment_before) then
        valiable.doubles: setVal(ValiableIndex.generic_bool, NO)
        changeKamae()
    end
    if (timing == Timing.command_varb_title) then
        if (valiable.tstrings: val_str("Verb") == localizeString(_T("youmu_command_equip_attack_key")):tstr() )then
            if(g_kamae_mode == KAMAE_SEME)then
                valiable.tstrings: dim_str("DisplayVerb", localizeString(_T("youmu_command_equip_attack_seme")):tstr())
            elseif(g_kamae_mode == KAMAE_UKE)then
                valiable.tstrings: dim_str("DisplayVerb", localizeString(_T("youmu_command_equip_attack_uke")):tstr())
            end
        elseif (valiable.tstrings: val_str("Verb") == localizeString(_T("youmu_command_equip_defence_key")):tstr() )then
            if(g_kamae_mode == KAMAE_SEME)then
                valiable.tstrings: dim_str("DisplayVerb", localizeString(_T("youmu_command_equip_defence_seme")):tstr())
            elseif(g_kamae_mode == KAMAE_UKE)then
                valiable.tstrings: dim_str("DisplayVerb", localizeString(_T("youmu_command_equip_defence_uke")):tstr())
            end
        end
    end
    if (timing == Timing.command_varb_shortExplanationText) then
        local str_map = map_tstring_StyleString()
        if(PlayerCharacter().attackEquipment)then
            str_map: insert( pair_tstring_StyleString( _T("attackEquipment"), PlayerCharacter().attackEquipment: fullName() ))
        else
            str_map: insert( pair_tstring_StyleString( _T("attackEquipment"), StyleString(_T("")) ))
        end
        if(PlayerCharacter().defenseEquipment)then
            str_map: insert( pair_tstring_StyleString( _T("defenseEquipment"), PlayerCharacter().defenseEquipment: fullName() ))
        else
            str_map: insert( pair_tstring_StyleString( _T("defenseEquipment"), StyleString(_T("")) ))
        end
        if(valiable.drops: val_str("Subject"))then
            str_map: insert( pair_tstring_StyleString( _T("Subject"), valiable.drops: val_str("Subject"): fullName() ))
        else
            str_map: insert( pair_tstring_StyleString( _T("Subject"), StyleString(_T("")) ))
        end
        if (valiable.tstrings: val_str("Verb") == localizeString(_T("youmu_command_equip_attack_key")):tstr() )then
            valiable.styleStrings: dim_str("shortExplanationText", localizeString(_T("youmu_command_equip_attack_shortExplanation"), str_map) )
        elseif (valiable.tstrings: val_str("Verb") == localizeString(_T("youmu_command_equip_defence_key")):tstr() )then
            valiable.styleStrings: dim_str("shortExplanationText", localizeString(_T("youmu_command_equip_defence_shortExplanation"), str_map) )
        elseif (valiable.tstrings: val_str("Verb") == localizeString(_T("youmu_command_unequip_key")):tstr() )then
            valiable.styleStrings: dim_str("shortExplanationText", StyleString(_T("")) )
        elseif (valiable.tstrings: val_str("Verb") == localizeString(_T("youmu_command_equip_key")):tstr() )then
            valiable.styleStrings: dim_str("shortExplanationText", StyleString(_T("")) )
        end
    end
end

function normalAttack()
    if(g_kamae_mode == KAMAE_SEME)then
        local attackCount = 0
        if(me.attackEquipment)then
            CharacterNormalAttack(me)
            attackCount = attackCount + 1
        end
        CharacterForceEquipmentSwap(me)
        if(me.attackEquipment)then
            CharacterNormalAttack(me)
            attackCount = attackCount + 1
        end
        CharacterForceEquipmentSwap(me)
        if(attackCount == 0)then
            return false;
        else
            return true;
        end
    elseif(g_kamae_mode == KAMAE_UKE)then
        return false;
    end
end

function charaSkillCommond()
    textCommandString = localizeString(_T("youmu_skill_command_title"))
    str_map = map_tstring_StyleString()
    if(g_kamae_mode == KAMAE_SEME)then
        str_map: insert( pair_tstring_StyleString( _T("Kamae"), localizeString(_T("youmu_skill_seme_kamae")) ))
    elseif(g_kamae_mode == KAMAE_UKE)then
        str_map: insert( pair_tstring_StyleString( _T("Kamae"), localizeString(_T("youmu_skill_uke_kamae")) ))
    end
    textCommandStringExpranation = localizeString(_T("youmu_skill_command_shortExplanation"), str_map)
    local command = LuaFunctionCommand(textCommandString, changeKamae, textCommandStringExpranation)
    return command
end

function changeKamae()
    menuDismissAllWindows()
    str_map = map_tstring_StyleString()
    str_map: insert( pair_tstring_StyleString( _T("Chara"), me: shortName() ))
    if(g_kamae_mode == KAMAE_SEME)then
        g_kamae_mode = KAMAE_UKE
        utility_localizedMessage(_T("youmu_skill_uke_kamae_message"), str_map)
        effect_powerUp(me.placeX, me.placeY, 190)
    elseif(g_kamae_mode == KAMAE_UKE)then
        g_kamae_mode = KAMAE_SEME
        utility_localizedMessage(_T("youmu_skill_seme_kamae_message"), str_map)
        effect_powerUp(me.placeX, me.placeY, 10)
    end
    turnEndPlayerCharacter()
end

function calcuAttackPower(valiable)
    attack_equipment_power = valiable.doubles: val_str("attack_equipment_power")
    player_attack_power = valiable.doubles: val_str("player_attack_power")
    player_attack_equipment_power_magnification = valiable.doubles: val_str("player_attack_equipment_power_magnification")
    
    if(g_kamae_mode == KAMAE_SEME)then
        return player_attack_power + attack_equipment_power*player_attack_equipment_power_magnification;
    elseif(g_kamae_mode == KAMAE_UKE)then
        return player_attack_power;
    end
    return player_attack_power + attack_equipment_power*player_attack_equipment_power_magnification;
end

function calcuDefencePower(valiable)
    defence_equipment_power = valiable.doubles: val_str("defence_equipment_power")
    player_defence_power = valiable.doubles: val_str("player_defence_power")
    player_defence_equipment_power_magnification = valiable.doubles: val_str("player_defence_equipment_power_magnification")
    
    if(g_kamae_mode == KAMAE_SEME)then
        return player_defence_power;
    elseif(g_kamae_mode == KAMAE_UKE)then
        return player_defence_power + defence_equipment_power*player_defence_equipment_power_magnification;
    end
    return player_defence_power + defence_equipment_power*player_defence_equipment_power_magnification;
end

function calcuExtraAttackPower(valiable)
    attack_equipment_power = valiable.doubles: val_str("attack_equipment_power")
    player_attack_equipment_power_magnification = valiable.doubles: val_str("player_attack_equipment_power_magnification")
    defence_equipment_power = valiable.doubles: val_str("defence_equipment_power")
    player_defence_equipment_power_magnification = valiable.doubles: val_str("player_defence_equipment_power_magnification")
    
    if(g_kamae_mode == KAMAE_SEME)then
        return 0;
    elseif(g_kamae_mode == KAMAE_UKE)then
        if attack_equipment_power > defence_equipment_power then
            return (attack_equipment_power - defence_equipment_power)*0.5;
        else
            return 0;
        end
    end
    return 0;
end

function calcuExtraDefencePower(valiable)
    attack_equipment_power = valiable.doubles: val_str("attack_equipment_power")
    player_attack_equipment_power_magnification = valiable.doubles: val_str("player_attack_equipment_power_magnification")
    defence_equipment_power = valiable.doubles: val_str("defence_equipment_power")
    player_defence_equipment_power_magnification = valiable.doubles: val_str("player_defence_equipment_power_magnification")
    
    if(g_kamae_mode == KAMAE_SEME)then
        return 0;
    elseif(g_kamae_mode == KAMAE_UKE)then
        return defence_equipment_power * player_defence_equipment_power_magnification;
    end
    return 0;
end

function calcuNoAppreciationExtraAttackPower(valiable)
    attack_equipment_power_base = valiable.doubles: val_str("attack_equipment_power_base")
    player_attack_equipment_power_magnification = valiable.doubles: val_str("player_attack_equipment_power_magnification")
    defence_equipment_power_base = valiable.doubles: val_str("defence_equipment_power_base")
    player_defence_equipment_power_magnification = valiable.doubles: val_str("player_defence_equipment_power_magnification")
    
    if(g_kamae_mode == KAMAE_SEME)then
        return 0;
    elseif(g_kamae_mode == KAMAE_UKE)then
        if attack_equipment_power_base > defence_equipment_power_base then
            return (attack_equipment_power_base - defence_equipment_power_base)*0.5;
        else
            return 0;
        end
    end
    return 0;
end

function calcuNoAppreciationExtraDefencePower(valiable)
    attack_equipment_power_base = valiable.doubles: val_str("attack_equipment_power_base")
    player_attack_equipment_power_magnification = valiable.doubles: val_str("player_attack_equipment_power_magnification")
    defence_equipment_power_base = valiable.doubles: val_str("defence_equipment_power_base")
    player_defence_equipment_power_magnification = valiable.doubles: val_str("player_defence_equipment_power_magnification")
    
    if(g_kamae_mode == KAMAE_SEME)then
        return 0;
    elseif(g_kamae_mode == KAMAE_UKE)then
        return defence_equipment_power_base * player_defence_equipment_power_magnification;
    end
    return 0;
end