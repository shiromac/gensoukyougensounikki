--1037 鎖分銅

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
sound = AnimeChip_Sound("swing1037.wav",PositionPerGrid(toPlaceX,toPlaceY,0),1.0)
hab:addAnimeChip(sound)
sound: setDelay(fumikiri_life*0.5+1)

--
hab_parallel = AnimeChip_Hab(PositionPerGrid(toPlaceX, toPlaceY, 0))

syougeki_life = 12
-- =============================================================
-- 衝撃波 青
syougeki11 = AnimeChip_Perticle("effect\\basic\\ring_radio.png",D3DXVECTOR2(64,64), syougeki_life)
syougeki11.color: addCascade(Color(250,30,30,255), syougeki_life)
syougeki11.drawMode: setDefault(DrawMode_normal)
syougeki11.rotationBefore: addCascade(360, 0, syougeki_life)
syougeki11.scaleX:	addCascade(0.1, 1, syougeki_life)
syougeki11.scaleY:	addCascade(0.1, 1, syougeki_life)
syougeki11: setDelay(2)
syougeki11.position: addCascade( PositionPerGrid(0.5*random() - 0.25, 0.5*random() - 0.25, 0.5), syougeki_life)

-- コピーして加算合成
syougeki12 = AnimeChip_Perticle(syougeki11)
syougeki12: setDelay(3)
syougeki12.color: clear()
syougeki12.color: addCascade(Color(100,100,100,255), syougeki_life)
syougeki12.drawMode: setDefault(DrawMode_addition)

hab_parallel: addAnimeChip(syougeki11);
hab_parallel: addAnimeChip(syougeki12);
-- =============================================================

-- =============================================================
-- 衝撃波 赤
syougeki_life = 10
syougeki21 = AnimeChip_Perticle("effect\\basic\\ring_radio.png",D3DXVECTOR2(64,64), syougeki_life)
syougeki21.color: addCascade(Color(250,255,30,30), syougeki_life)
syougeki21.drawMode: setDefault(DrawMode_normal)
syougeki21.rotationBefore: addCascade(360, 0, syougeki_life)
syougeki21.scaleX:	addCascade(0.1, 1, syougeki_life)
syougeki21.scaleY:	addCascade(0.1, 1, syougeki_life)
syougeki21: setDelay(7)
syougeki21.position: addCascade( PositionPerGrid(0.5*random() - 0.25, 0.5*random() - 0.25, 0.5), syougeki_life)

-- コピーして加算合成
syougeki22 = AnimeChip_Perticle(syougeki21)
syougeki22: setDelay(8)
syougeki22.color: clear()
syougeki22.color: addCascade(Color(100,255,100,100), syougeki_life)
syougeki22.drawMode: setDefault(DrawMode_addition)

hab_parallel: addAnimeChip(syougeki21);
hab_parallel: addAnimeChip(syougeki22);
-- =============================================================

-- =============================================================
-- 衝撃波 黄
syougeki_life = 8
syougeki31 = AnimeChip_Perticle("effect\\basic\\ring_radio.png",D3DXVECTOR2(64,64), syougeki_life)
syougeki31.color: addCascade(Color(250,255,255,30), syougeki_life)
syougeki31.drawMode: setDefault(DrawMode_normal)
syougeki31.rotationBefore: addCascade(360, 0, syougeki_life)
syougeki31.scaleX:	addCascade(0.1, 1, syougeki_life)
syougeki31.scaleY:	addCascade(0.1, 1, syougeki_life)
syougeki31: setDelay(12)
syougeki31.position: addCascade( PositionPerGrid(0.5*random() - 0.25, 0.5*random() - 0.25, 0.5), syougeki_life)

-- コピーして加算合成
syougeki32 = AnimeChip_Perticle(syougeki31)
syougeki32: setDelay(13)
syougeki32.color: clear()
syougeki32.color: addCascade(Color(100,255,255,100), syougeki_life)
syougeki32.drawMode: setDefault(DrawMode_addition)

hab_parallel: addAnimeChip(syougeki31);
hab_parallel: addAnimeChip(syougeki32);
-- =============================================================


setAnimation_parallel(hab_parallel)
setAnimation_wait(hab)