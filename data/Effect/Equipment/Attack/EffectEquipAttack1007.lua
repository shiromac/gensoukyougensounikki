--1007 三日月戟

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
sound = AnimeChip_Sound("swing1007.wav",PositionPerGrid(toPlaceX,toPlaceY,0),1.0)
hab:addAnimeChip(sound)
sound: setDelay(fumikiri_life*0.5+1)

--
hab_parallel = AnimeChip_Hab(PositionPerGrid(toPlaceX, toPlaceY, 0))

-- =============================================================
-- 剣の軌跡
life_swing = 8
belt_point_num = 20
color_swing = Color(120,250,130,60)

aniBelt11 = AnimeChip_Belt("effect\\equip\\mikatuki.png",PositionPerGrid(0, 0, 0),PositionPerGrid(0, 0, 0),life_swing)
aniBelt11:setDelay(4)
aniBelt11.drawRange: addCascade(D3DXVECTOR2(1,1),D3DXVECTOR2(1,15-1),D3DXVECTOR2(1,belt_point_num-1),D3DXVECTOR2(belt_point_num-1,belt_point_num-1),life_swing)
aniBelt11.color: addCascade(color_swing, life_swing)
aniBelt11.position: addCascade(PositionPerGrid(0, 0.6, 0.6), life_swing)
aniBelt11:setWorldRotation(direction)
aniBelt11.drawMode: setDefault(DrawMode_normal)
aniBelt11.textureRange: setDefault(TextureRangeRect(1,0,0,1))

for k = 0, belt_point_num do
	pos = PositionPerGrid(1.2, 0, 0)	-- ベースの位置ベクトル(先端位置)
	rotAxis = PositionPerGrid(0, -0.2, 1.0)	-- 回転軸
	
	angle =  ( (k/belt_point_num)*170 + 0 ) * -MATH_PI/180	-- 回転角度
	D3DXVec3Rotate( pos, rotAxis, angle, pos )			-- 位置ベクトルを回転
	
	aniBelt11.rightEdge: push_back(VariationValue_PositionPerGrid(pos*0.3))	-- 手前
	aniBelt11.leftEdge: push_back(VariationValue_PositionPerGrid(pos))	-- 奥
end

-- コピーして加算合成
aniBelt12 = AnimeChip_Belt(aniBelt11);
aniBelt13 = AnimeChip_Belt(aniBelt11);
aniBelt13:setDelay(3)
aniBelt14 = AnimeChip_Belt(aniBelt13);
aniBelt14.drawMode: setDefault(DrawMode_addition)

hab_parallel: addAnimeChip(aniBelt11);
hab_parallel: addAnimeChip(aniBelt12);
hab_parallel: addAnimeChip(aniBelt13);
hab_parallel: addAnimeChip(aniBelt14);
-- =============================================================


setAnimation_parallel(hab_parallel)
setAnimation_wait(hab)