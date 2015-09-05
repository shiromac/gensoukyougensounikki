function isPlayerCharacterAccordingStartRegulation(errorMassage)
--errorMassage output
    return true;--NoRegulation
end

function StartRegulationMassage()
--return message
    stringMap = map_tstring_StyleString()
    stringMap: setValue(_T("chara"), getSumpleCharacter(1003): fullName())
    return localizeString(_T("DungeonRegulation_NoRegulation"), stringMap)
end

function isRegureCharacter(character)
    idNumber = character: ID()
    if (idNumber == 1003) then
        --mairin
        return true;
    end
    return false;
end
