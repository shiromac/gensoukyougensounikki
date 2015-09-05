function homeEvent()
    if(isNotGoodTerminateInBase())then

        str_map = map_tstring_StyleString()
		storyEventMessageLoop(_T("homeResetEvent_"), str_map, nil)
        clearMassage()

    end
end
