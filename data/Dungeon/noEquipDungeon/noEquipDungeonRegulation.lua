function isPlayerCharacterAccordingStartRegulation(errorMassage)
--errorMassage output
    if PlayerCharacter().holdItem: size() > 0 or walletMoney() > 0 then
        stringMap = map_tstring_StyleString()
        errorMassage: addstring(localizeString(_T("DungeonRegulation_NoItemNoManey_FreeCharacter_ErrorMessage"), stringMap))
        return false;
    end
    return true;
end

function StartRegulationMassage()
--return message
    stringMap = map_tstring_StyleString()
    return localizeString(_T("DungeonRegulation_NoItemNoManey_FreeCharacter"), stringMap)
end

function isRegureCharacter(character)
    return true--FreeChara
end
