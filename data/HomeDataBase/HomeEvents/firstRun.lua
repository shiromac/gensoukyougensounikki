function homeEvent()
	if( not isDungeonCleared(_T("intro_Dungeon")) )then
		gotoTempDungeon(_T("intro_Dungeon"))
	end
end
