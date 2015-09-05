--itirin

-- 数値型 fromPlaceX : 攻撃者のX位置（マス）
-- 数値型 fromPlaceY : 攻撃者のY位置（マス）
-- 数値型 toPlaceX : 攻撃先のX位置（マス）
-- 数値型 toPlaceY : 攻撃先のY位置（マス）
-- 数値型 distanceGrid
-- 数値型 direction : 攻撃者の向き（０～３６０）
-- pcCharacter型 attacker : 攻撃者のキャラクター

hab = AnimeChip_Hab(PositionPerGrid(fromPlaceX, fromPlaceY, 0))
distance = sqrt((fromPlaceX-toPlaceX)*(fromPlaceX-toPlaceX) + (fromPlaceY-toPlaceY)*(fromPlaceY-toPlaceY))

life_kobusi = 3*distance+3

life = life_kobusi
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
sound = AnimeChip_Sound("attack_punch1.wav",PositionPerGrid(toPlaceX,toPlaceY,0),1.0)
hab:addAnimeChip(sound)
sound: setDelay(fumikiri_life-1)

--
hab_parallel = AnimeChip_Hab(PositionPerGrid(fromPlaceX, fromPlaceY, 0))


--kobusi
kobusi = AnimeChip_Perticle("effect\\mob\\kobusi.png",D3DXVECTOR2(100,100),life_kobusi)
kobusi: setWorldRotation(direction)
kobusi.rotationBefore: setDefault(direction)
kobusi.position: addCascade(PositionPerGrid(0,0,0.8), PositionPerGrid(0,-distance+0.5,0.8), PositionPerGrid(0,-distance+0.2,0.8), life_kobusi)

hab_parallel:addAnimeChip(kobusi)

setAnimation_parallel(hab_parallel)
setAnimation_wait(hab)
