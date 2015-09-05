--toramaru

-- êîílå^ fromPlaceX : çUåÇé“ÇÃXà íuÅiÉ}ÉXÅj
-- êîílå^ fromPlaceY : çUåÇé“ÇÃYà íuÅiÉ}ÉXÅj
-- êîílå^ toPlaceX :
-- êîílå^ toPlaceY :
-- êîílå^ distanceGrid
-- êîílå^ direction : çUåÇé“ÇÃå¸Ç´ÅiÇOÅ`ÇRÇUÇOÅj
-- pcCharacterå^ attacker : çUåÇé“ÇÃÉLÉÉÉâÉNÉ^Å[

hab = AnimeChip_Hab(PositionPerGrid(fromPlaceX, fromPlaceY, 0))
distance = sqrt((fromPlaceX-toPlaceX)*(fromPlaceX-toPlaceX) + (fromPlaceY-toPlaceY)*(fromPlaceY-toPlaceY))

life_yari = 3*distance+3

life = life_yari
-- fumikiri
fumikiri_life = life
fumikomi_distance = 1

chara_anime = AnimeChip_Chara(attacker,fumikiri_life)
hab:addAnimeChip(chara_anime)
chara_anime: setWorldRotation(direction)
chara_anime.position: addCascade(PositionPerGrid(0, 0, 0), PositionPerGrid(0, -0.1, 0), PositionPerGrid(0, -0.1, 0), fumikiri_life*0.5)
chara_anime.position: addCascade(PositionPerGrid(0, -0.1, 0), PositionPerGrid(0, -0.8*fumikomi_distance, 0), PositionPerGrid(0, -0.4*fumikomi_distance, 0), PositionPerGrid(0, 0, 0), fumikiri_life*0.5)
chara_anime.yawing: addCascade(0, 45, fumikiri_life*0.5)
chara_anime.yawing: addCascade(45, -90, 0, fumikiri_life*0.5)
--

-- sound
sound = AnimeChip_Sound("attack_swing1.wav",PositionPerGrid(toPlaceX,toPlaceY,0),1.0)
hab:addAnimeChip(sound)
sound: setDelay(2)

--
hab_parallel = AnimeChip_Hab(PositionPerGrid(fromPlaceX, fromPlaceY, 0))


--zanngeki
zanngeki = AnimeChip_Perticle("effect\\mob\\yarihead.png",D3DXVECTOR2(32,32),life_yari)
zanngeki: setWorldRotation(direction)
zanngeki.rotationAfter: setDefault(direction)
zanngeki.color: addCascade(Color(255,255,255,255), life_yari)
zanngeki.scaleY: addCascade(1, distance*2, distance*2, life_yari)
zanngeki.position: addCascade(PositionPerGrid(0,0,0.8), PositionPerGrid(0,-distance*0.7,0.8), PositionPerGrid(0,-distance*0.7,0.8), life_yari)

hab_parallel:addAnimeChip(zanngeki)

zanngeki2 = AnimeChip_Perticle("effect\\basic\\ball_simple.png",D3DXVECTOR2(64,64),life_yari)
zanngeki2: setWorldRotation(direction)
zanngeki2.rotationAfter: setDefault(direction)
zanngeki2.color: addCascade(Color(188,200,255,255), life_yari)
zanngeki2.scaleY: addCascade(1, distance, life_yari)
zanngeki2.drawMode: setDefault(DrawMode_addition)
zanngeki2.position: addCascade(PositionPerGrid(0,0,0.8), PositionPerGrid(0,-distance,0.8), life_yari)

hab_parallel:addAnimeChip(zanngeki2)

setAnimation_parallel(hab_parallel)
setAnimation_wait(hab)
