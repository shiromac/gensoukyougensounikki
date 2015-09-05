ItemNum = 10

function isPlayerCharacterAccordingStartRegulation(errorMassage)
--errorMassage output
    if itemCount(PlayerCharacter().holdItem) > 10 then
        stringMap = map_tstring_StyleString()
        stringMap: setValue(_T("ItemNum"), StyleString(_T(ItemNum.."")))
        errorMassage: addstring(localizeString(_T("DungeonRegulation_AnyItemFreeManey_FreeCharacter_ErrorMessage"), stringMap))
        return false;
    end
    return true;
end

function StartRegulationMassage()
--return message
    stringMap = map_tstring_StyleString()
    stringMap: setValue(_T("ItemNum"), StyleString(_T(ItemNum.."")))
    return localizeString(_T("DungeonRegulation_AnyItemFreeManey_FreeCharacter"), stringMap)
end

function isRegureCharacter(character)
    return true--FreeChara
end
