function isPlayerCharacterAccordingStartRegulation(errorMassage)
--errorMassage output
    return true;--NoRegulation
end

function StartRegulationMassage()
--return message
    stringMap = map_tstring_StyleString()
    return localizeString(_T("DungeonRegulation_NoRegulation_FreeCharacter"), stringMap)
end

function isRegureCharacter(character)
    return true--FreeChara
end
