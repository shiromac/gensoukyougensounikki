sstr = StyleString()
if( getMapValue(savable_globalFlags(),_T("EventDidFlag:homeHinaAppearing"), NO ) == YES )then
    textSetPath(sstr, "after")
else
    textSetPath(sstr, "before")
end
textPrint(sstr)
