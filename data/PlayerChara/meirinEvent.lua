function canTalk()
    return true
end
function talkEvent()

    turnCharacterForCharacter(me, PlayerCharacter())
    
    textCommandString = localizeString(_T("meirin_command_caption_changePlayerCharacter_caption"))
    textCommandStringExpranation = localizeString(_T("meirin_command_caption_changePlayerCharacter_ShortExplanation"))
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
    if not (PlayerCharacter(): ID() == 1002)then
        swapPlayerCharacter(me)
    end
    menuDismissAllWindows()
end