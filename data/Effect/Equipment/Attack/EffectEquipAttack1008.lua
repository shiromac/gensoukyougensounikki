--1008 大鎌

-- 数値型 fromPlaceX : 攻撃者のX位置（マス）
-- 数値型 fromPlaceY : 攻撃者のY位置（マス）
-- 数値型 toPlaceX : 攻撃先のX位置（マス）
-- 数値型 toPlaceY : 攻撃先のY位置（マス）
-- 数値型 direction : 攻撃者の向き（０～３６０）
-- 数値型 distance : 攻撃者と攻撃先の距離（マス）
-- 数値型 isValidAttack : 攻撃が壁に向かっていたら０、そうでなければ１
-- pcCharacter型 attacker : 攻撃者のキャラクター


hab = AnimeChip_Hab(PositionPerGrid(toPlaceX, toPlaceY, 0))


-- fumikiri
fumikiri_life = 16
fumikomi_distance = isValidAttack * sqrt((fromPlaceX - toPlaceX)*(fromPlaceX - toPlaceX) + (fromPlaceY - toPlaceY)*(fromPlaceY - toPlaceY))/ distance 

chara_anime = AnimeChip_Chara(attacker,fumikiri_life)
hab:addAnimeChip(chara_anime)
chara_anime: setWorldRotation(direction)
chara_anime.position: addCascade(PositionPerGrid(0, 0, 0), PositionPerGrid(0, -0.1, 0), PositionPerGrid(0, -0.1, 0), fumikiri_life*0.5)
chara_anime.position: addCascade(PositionPerGrid(0, -0.1, 0), PositionPerGrid(0, -0.8*fumikomi_distance, 0), PositionPerGrid(0, -0.4*fumikomi_distance, 0), PositionPerGrid(0, 0, 0), fumikiri_life*0.5)
chara_anime.yawing: addCascade(0, 45, fumikiri_life*0.5)
chara_anime.yawing: addCascade(45, -90, 0, fumikiri_life*0.5)
--

-- sound
sound = AnimeChip_Sound("swing1008.wav",PositionPerGrid(toPlaceX,toPlaceY,0),1.0)
hab:addAnimeChip(sound)
sound: setDelay(fumikiri_life*0.5+1)

--
hab_parallel = AnimeChip_Hab(PositionPerGrid(toPlaceX, toPlaceY, 0))


kama = AnimeChip_Perticle("effect\\equip\\nagekama.png",D3DXVECTOR2(80,80), 16)
kama.color: addCascade(Color(255,230,230,230), 16)
kama.drawMode: setDefault(DrawMode_normal)
kama:setWorldRotation(direction)
	
kama.position: addCascade(PositionPerGrid(0, 1, 2), PositionPerGrid(0, -1.8, 1), PositionPerGrid(0, 1, 0), 16)
kama.rotationAfter: addCascade(1200, 0, 16)
kama: setDelay(4)

-- コピーして加算合成
-- 残像を描画する
kama12 = AnimeChip_Perticle(kama);
kama12.drawMode: setDefault(DrawMode_addition)
kama12.color: clear()
kama12.color: addCascade(Color(70,250,250,250), 16)
kama12: setDelay(5)

kama13 = AnimeChip_Perticle(kama12);
kama13: setDelay(7)

hab_parallel: addAnimeChip(kama);
hab_parallel: addAnimeChip(kama12);
hab_parallel: addAnimeChip(kama13);

-- =============================================================


setAnimation_parallel(hab_parallel)
setAnimation_wait(hab)