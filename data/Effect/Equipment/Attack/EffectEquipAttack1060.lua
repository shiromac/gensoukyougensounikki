--1060 エレキテルの羽衣

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
sound = AnimeChip_Sound("swing1060.wav",PositionPerGrid(toPlaceX,toPlaceY,0),1.0)
hab:addAnimeChip(sound)
sound: setDelay(fumikiri_life*0.5+1)

--
hab_parallel = AnimeChip_Hab(PositionPerGrid(toPlaceX, toPlaceY, 0))

-- =============================================================
-- ドリルの軌跡
life_swing = 10
belt_point_num = 40
color_swing = Color(150,250,250,150)

s_okuRate = 0.9
s_temaeRate = -0.4

aniBelt11 = AnimeChip_Belt("effect\\equip\\sword_swing.png",PositionPerGrid(0, 0, 0),PositionPerGrid(0, 0, 0),life_swing)
aniBelt11:setDelay(4)
aniBelt11.drawRange: addCascade(D3DXVECTOR2(1,1),D3DXVECTOR2(1,belt_point_num-1),D3DXVECTOR2(1,belt_point_num-1),D3DXVECTOR2(belt_point_num-1,belt_point_num-1),life_swing)
aniBelt11.color: addCascade(color_swing, life_swing)
aniBelt11.position: addCascade(PositionPerGrid(0, -0.5, 0.7), 16)
aniBelt11:setWorldRotation(direction)
aniBelt11.drawMode: setDefault(DrawMode_normal)
aniBelt11.textureRange: setDefault(TextureRangeRect(1,0,0,1))

for k = 0, belt_point_num do
	-- swing sita
	angle =  ( (k/belt_point_num)*190 + 0) * MATH_PI/180
	sinSita = sin( angle )
	cosSita = cos( angle )
	
	-- baseVector
	angle =  ( (k/belt_point_num)*1440 + 180) * -MATH_PI/180
	pos = PositionPerGrid(-0.25*sin( angle )*sinSita, 1.0*(belt_point_num-k)/belt_point_num, 0.2*cos( angle )*cosSita)
	
	-- rotation
	rotAxis = PositionPerGrid(0, 1.0, 0)	
	oku	= pos + ( pos * sinSita* s_okuRate )
	temae	= pos + ( pos * sinSita* s_temaeRate )
	aniBelt11.rightEdge: push_back(VariationValue_PositionPerGrid(temae))
	aniBelt11.leftEdge: push_back(VariationValue_PositionPerGrid(oku))
	
end

-- コピーして加算合成
aniBelt12 = AnimeChip_Belt(aniBelt11);
aniBelt12.drawMode: setDefault(DrawMode_addition)
aniBelt13 = AnimeChip_Belt(aniBelt11);
aniBelt13: setDelay(8)
aniBelt14 = AnimeChip_Belt(aniBelt13);
aniBelt14.drawMode: setDefault(DrawMode_addition)

hab_parallel: addAnimeChip(aniBelt11);
hab_parallel: addAnimeChip(aniBelt12);
hab_parallel: addAnimeChip(aniBelt13);
hab_parallel: addAnimeChip(aniBelt14);
-- =============================================================



-- =============================================================
-- ビリビリ
--[[
belt_point_num = 10
color_swing = Color(240,250,100,250)

aniDenki11 = AnimeChip_Belt("effect\\equip\\sword_swing.png",PositionPerGrid(0, 0, 0),PositionPerGrid(0, 0, 0),life_swing)
aniDenki11:setDelay(4)
aniDenki11.drawRange: addCascade(D3DXVECTOR2(1,1),D3DXVECTOR2(1,belt_point_num-1),D3DXVECTOR2(1,belt_point_num-1),D3DXVECTOR2(belt_point_num-1,belt_point_num-1),life_swing)
aniDenki11.color: addCascade(color_swing, life_swing)
aniDenki11.position: addCascade(PositionPerGrid(0, -0.5, 0.7), 16)
aniDenki11:setWorldRotation(direction)
aniDenki11.drawMode: setDefault(DrawMode_normal)
aniDenki11.textureRange: setDefault(TextureRangeRect(1,0,0,1))

for k = 0, belt_point_num do
	-- swing sita
	r = random()
	
	-- baseVector
	angle =  ( (k/belt_point_num)*180 + 0) * -MATH_PI/180
	pos = PositionPerGrid(random(), random(), random())
	
	-- rotation
	rotAxis = PositionPerGrid(0, 1.0, 1.0)	
	oku	= pos + ( pos * sinSita* s_okuRate )
	temae	= pos + ( pos * sinSita* s_temaeRate )
	aniDenki11.rightEdge: push_back(VariationValue_PositionPerGrid(temae))
	aniDenki11.leftEdge: push_back(VariationValue_PositionPerGrid(oku))
	
end

-- コピーして加算合成
hab_parallel: addAnimeChip(aniBelt11);

--]]
-- =============================================================

setAnimation_parallel(hab_parallel)
setAnimation_wait(hab)