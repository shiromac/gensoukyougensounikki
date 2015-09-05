function canTalk()
    return true
end
function talkEvent()
    turnCharacterForCharacter(me, PlayerCharacter())
    
    selectWindow = SelectWindow()
    
    local textCommandString = localizeString(_T("akyu_command_extraDungeons_caption"))
    local textCommandStringExpranation = localizeString(_T("akyu_command_extraDungeons_shortExplanation"))
    local command = LuaFunctionCommand(textCommandString, selectDungeonCommand_Action, textCommandStringExpranation)
    selectWindow: addCommand(command)
    
    local textCommandString = localizeString(_T("akyu_command_tips_caption"))
    local textCommandStringExpranation = localizeString(_T("akyu_command_tips_shortExplanation"))
    local command = LuaFunctionCommand(textCommandString, categoryTipsCommand_Action, textCommandStringExpranation)
    selectWindow: addCommand(command)

    selectWindow: init(12,selectWindow: commandSize())
    selectWindow: setLeft(menuDefaultPositionLeft(1))
    selectWindow: setTop(menuDefaultPositionTop(1))
    selectWindow: showWindow()

    menuRerenderAllWindows();

end
function selectDungeonCommand_Action()
    local dungeons = vector_tstring()
	dungeons: push_back(_T("red_extra_Dungeon"))
	dungeons: push_back(_T("blue_extra_Dungeon"))
	dungeons: push_back(_T("green_extra_Dungeon"))
	dungeons: push_back(_T("purple_extra_Dungeon"))
	dungeons: push_back(_T("99F_Dungeon"))
	dungeons: push_back(_T("ten_match_Dungeon"))
	dungeons: push_back(_T("no_food_Dungeon"))
	dungeons: push_back(_T("no_equip_Dungeon"))
	showSelectDungeonWindow(dungeons)
end
function categoryTipsCommand_Action(freeValiable)
    local localizeLayer = localizeDataLayer()
    local GameTipsLayer = localizeLayer: member(_T("GameTips"))
    local CategoryListLayer = GameTipsLayer: member(_T("CategoryList"))
    local CategoryList = CategoryListLayer: script()

    local selectWindow = SelectWindow()

    for i = 0, CategoryList: size() -1 do
    
        textCategoryString = StyleString(CategoryList: at(i))
        textCommandStringExpranation = localizeString(_T(""))
    
        command = LuaFunctionCommand(textCategoryString, tipsListCommand_Action, textCommandStringExpranation)
        --local freeValiable = map_tstring_object();
        command.freeValiable: setValue(_T("category"), CategoryList: at(i))
        --command.freeValiable = freeValiable;
        selectWindow: addCommand(command)
    end

    local rawSize = math.min(10,selectWindow: commandSize())

    selectWindow: init(12,rawSize)
    selectWindow: setLeft(menuDefaultPositionLeft(2))
    selectWindow: setTop(menuDefaultPositionTop(2))
    selectWindow: showWindow()

    menuRerenderAllWindows();
end
function tipsListCommand_Action(freeValiable)
    local localizeLayer = localizeDataLayer()
    local GameTipsLayer = localizeLayer: member(_T("GameTips"))
    local Cagegory = freeValiable: value(_T("category"))
    local CategoryLayer = GameTipsLayer: member(Cagegory)
    local keyList = CategoryLayer: memberKeyList()

    local selectWindow = SelectWindow()

    local tipsLayers = {}
    for i = 0, keyList: size() -1 do
        local tipsLayer = CategoryLayer: member(keyList: at(i))
        if(isConditionAppearingOfGameTips(tipsLayer))then
            table.insert(tipsLayers, tipsLayer)
        end
    end
    
    table.sort(tipsLayers, compareTipsLayer)
    
    for index, tipsLayer in pairs(tipsLayers) do
        local textTipsString = StyleString(tipsLayer: name())
        local textCommandStringExpranation = localizeString(_T(""))--localizeString(_T("tipsCommandExpranation"))
        
        local flagString = flagStringOfGameTipsDidReadFlag(tipsLayer)
        if( getMapValue(savable_globalFlags(),flagString, NO ) == NO )then
            textTipsString = localizeString(_T("GameTips_New")) + textTipsString
        end
    
        command = LuaFunctionCommand(textTipsString, tipsCommand_Action, textCommandStringExpranation)
        --local freeValiable = map_tstring_object();
        command.freeValiable: setValue(_T("tipsLayer"), tipsLayer)
        --command.freeValiable = freeValiable;
        selectWindow: addCommand(command)
    end
    
    if #tipsLayers == 0 then
        local textTipsString = localizeString(_T("GameTips_NoTips"))
        local textCommandStringExpranation = StyleString(_T(""))
        command = LuaFunctionCommand(textTipsString, nothingCommand_Action, textCommandStringExpranation)
        selectWindow: addCommand(command)
    end
    
    --[[
    for i = 0, keyList: size() -1 do
    
        textTipsString = StyleString(keyList: at(i))
        textCommandStringExpranation = localizeString(_T("tipsCommandExpranation"))
    
        command = LuaFunctionCommand(textTipsString, tipsCommand_Action, textCommandStringExpranation)
        --local freeValiable = map_tstring_object();
        command.freeValiable: setValue(_T("tipsKey"), keyList: at(i))
        command.freeValiable: setValue(_T("CategoryLayer"), CategoryLayer)
        --command.freeValiable = freeValiable;
        selectWindow: addCommand(command)
    end
    ]]


    local rawSize = math.min(10,selectWindow: commandSize())
    selectWindow: init(12,rawSize)
    selectWindow: setLeft(menuDefaultPositionLeft(3))
    selectWindow: setTop(menuDefaultPositionTop(3))
    selectWindow: showWindow()

    menuRerenderAllWindows();
end
function nothingCommand_Action(freeValiable)
    --nothing
end
function tipsPriority(tipsLayter)
    local priorityLayer = tipsLayter: member(_T("priority"))
    local topScript = priorityLayer: topScript()
    local priorityString = string(topScript)
    local priority = tonumber(priorityString)
    return priority
end
function compareTipsLayer(a,b)
	return (tipsPriority(a) < tipsPriority(b))
end
	
function tipsCommand_Action(freeValiable)
    local tipsLayer = freeValiable: value(_T("tipsLayer"))
    local bodyLayer = tipsLayer: member(_T("body"))
    
    local gameWindow = GameWindow()
    
    local flagString = flagStringOfGameTipsDidReadFlag(tipsLayer)
    if( getMapValue(savable_globalFlags(),flagString, NO ) == NO )then
        setMapValue(savable_globalFlags(),flagString, YES)
    end
    for i = 1, 100 do
    
        local pageLayer = bodyLayer: member(_T(i..""))
        if(pageLayer == nil)then
            break
        end
        local text = StyleString()
        LoadStringStyle(pageLayer, text)
        gameWindow.vText: resize(i)
        gameWindow.vText: setAt(i-1, text)
    
    end


    gameWindow: init(28,16)
    gameWindow. centerX = (SCREEN_SIZE_X/2)
    gameWindow: setTop(menuDefaultPositionTop(1))
    gameWindow: showWindow()

    menuRerenderAllWindows();
end
function flagStringOfGameTipsDidReadFlag(tipsLayer)
    local IDLayer = tipsLayer: member(_T("ID"))
    local flagString = _T("GameTipsDidReadFlag:")
    flagString = flagString + IDLayer: topScript()
    return flagString
end
function isConditionAppearingOfGameTips(tipsLayer)
    local conditionLayer = tipsLayer: member(_T("condition"))
    local conditions = conditionLayer: script()
    local condition = true
    for i = 0, conditions: size() -1 do
    
        local conditionFlag = conditions: at(i)
        if(conditionFlag: length() > 0)then
            if(getMapValue(savable_globalFlags(),conditionFlag, NO ) == NO)then
                condition = false
                break
            end
        end
    end
    return condition
end