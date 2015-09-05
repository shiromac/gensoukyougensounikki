function homeEvent()
    if( isDungeonCleared(_T("intro_Dungeon")) ) then
        if( getMapValue(savable_globalFlags(),_T("EventDidFlag:playerLevelExplanation"), NO ) == NO )then
            if(globalPlayerLavel() > 1 or globalPlayerExp() > 0 ) then

                setMapValue(savable_globalFlags(),_T("EventDidFlag:playerLevelExplanation"), YES)
                
                clearMassage()
                str_map = map_tstring_StyleString()
                for i = 1, 3 do
                    utility_storyMessage(_T("playerLevelExplanation"..i),str_map)
                    messageWait()
                end
                clearMassage()
                
            end
        end
	end
end
