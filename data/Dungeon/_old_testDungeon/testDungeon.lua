

function testFunction()
--[[
	str_map = map_tstring_StyleString()
    utility_storyMessage(_T("blueDungeonEvent1_1"),str_map)
    messageWait()
    coroutine.yield(false);
    utility_storyMessage(_T("blueDungeonEvent1_2"),str_map)
    messageWait()
    coroutine.yield(false);
    utility_storyMessage(_T("blueDungeonEvent1_3"),str_map)
    messageWait()
    coroutine.yield(false);
    utility_storyMessage(_T("blueDungeonEvent1_4"),str_map)
    messageWait()
    coroutine.yield(false);
    utility_storyMessage(_T("blueDungeonEvent1_5"),str_map)
    messageWait()
    return true;
    ]]
    
   	str_map = map_tstring_StyleString()
    utility_storyMessage(_T("blueDungeonEvent1_1"),str_map)
    messageWait()
    
    local land = LandformCharacterPosition(PlayerCharacter())
    lazerSummon(land, ASPECT_DOWN, 20, 1, CHARACTER_FORSE_ENEMY, Color(255,255,0,0),  2, 10, false, true)

    
    local player_land = LandformCharacterPosition(PlayerCharacter())
    local mob_land = LandformNearby(player_land,ASPECT_UP,1)
    local charaA = MakeCharacter(4006, 0, CHARACTER_FORSE_OTHER, mob_land)

    charaA: isDead()
    --CharacterMove(charaA,ASPECT_UP)
    moveWithAnime(charaA, ASPECT_UP, 1, false, 1.0, true);
    
    coroutine.yield(false);
    
    utility_storyMessage(_T("blueDungeonEvent1_1"),str_map)
    messageWait()
        
    coroutine.yield(false);
    
    jumpWithAnime(charaA, ASPECT_LEFT, 1, 1, false, 1.0, true);
    
    coroutine.yield(false);
    
    jumpWithAnime(charaA, ASPECT_RIGHT, 1, 1, true, 1.0, true);
    
    coroutine.yield(false);
    
    jumpWithAnime(charaA, ASPECT_UP, 0, 2, false, 1.0, true);
    
    coroutine.yield(false);
    fadeBlack(60)
    moveWithAnime(charaA, ASPECT_UP, 10, false, 1.0, true);

    coroutine.yield(false);
    
    messageWait()
    
    coroutine.yield(false);
    
    fadeWithColor(Color(255,255,30,30), 60)
    messageWait()
    coroutine.yield(false);
    
    fadeOut(60)
    moveWithAnime(charaA, ASPECT_DOWN, 10, false, 1.0, true);
    coroutine.yield(false);
    
    moveWithAnime(charaA, ASPECT_DOWN, 1, false, 1.0, true);
    coroutine.yield(false);
    return true;
end
function floorInitProcess()
	--outputDebugString("floorInitProcess_start\n")

    stackDramaThread(coroutine.create(testFunction))
    
    
		
	bossLand = LandformPlace(20,15);
	emeny = MakeCharacter(5002, 0, CHARACTER_FORSE_ENEMY, bossLand)
	
	
	startBossSpell(emeny)
	
	
	CharacterMatchless(PlayerCharacter(),300)
	CharacterLevelUp(PlayerCharacter(),99)
end

function dungeonProcess()
	--outputDebugString("dungeonProcess_start\n")
	if(random() < 0.005)then
	    local randomIndex = math.floor(random()*3)
		if(randomIndex == 0)then
	        PlayBGM(_T(""),_T("talk1_main.wav"))
	    elseif(randomIndex == 1)then
	        PlayBGM(_T("boss_intro.wav"),_T("boss_main.wav"))
	    elseif(randomIndex == 2)then
	        PlayBGM(_T("title.wav"),_T("title_roop.wav")) 
	    end
	end
end


function dungeonTurnProcess()
	--outputDebugString("dungeonTurnProcess_start\n")
	--[[
    messageWithStringStyle(StyleString(_T("a \nbb \nccc \ndddd \n")))
    messageWithStringStyle(StyleString(_T("a \nbb \nccc \ndddd \n")))
    messageWithStringStyle(StyleString(_T("a \nbb \nccc \ndddd \n")))
    messageWithStringStyle(StyleString(_T("a \nbb \nccc \ndddd \n")))
    messageWithStringStyle(StyleString(_T("1 \n")))
    messageWithStringStyle(StyleString(_T("a \nbb \nccc \ndddd \n")))
    messageWithStringStyle(StyleString(_T("a \nbb \nccc \ndddd \n")))
    messageWithStringStyle(StyleString(_T("a \nbb \nccc \ndddd \n")))
    messageWithStringStyle(StyleString(_T("a \nbb \nccc \ndddd \n")))
    messageWithStringStyle(StyleString(_T("2 \n")))
    messageWithStringStyle(StyleString(_T("a \nbb \nccc \ndddd \n")))
    messageWithStringStyle(StyleString(_T("a \nbb \nccc \ndddd \n")))
    messageWithStringStyle(StyleString(_T("a \nbb \nccc \ndddd \n")))
    messageWithStringStyle(StyleString(_T("a \nbb \nccc \ndddd \n")))
    messageWithStringStyle(StyleString(_T("3 \n")))
    messageWithStringStyle(StyleString(_T("a \nbb \nccc \ndddd \n")))
    messageWithStringStyle(StyleString(_T("a \nbb \nccc \ndddd \n")))
    messageWithStringStyle(StyleString(_T("a \nbb \nccc \ndddd \n")))
    messageWithStringStyle(StyleString(_T("a \nbb \nccc \ndddd \n")))
    messageWithStringStyle(StyleString(_T("0 \n")))
    messageWithStringStyle(StyleString(_T("a \nbb \nccc \ndddd \n")))
    messageWithStringStyle(StyleString(_T("a \nbb \nccc \ndddd \n")))
    messageWithStringStyle(StyleString(_T("a \nbb \nccc \ndddd \n")))
    messageWithStringStyle(StyleString(_T("a \nbb \nccc \ndddd \n")))
    ]]
end