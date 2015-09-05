--1019 神切

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
sound = AnimeChip_Sound("swing1019.wav",PositionPerGrid(toPlaceX,toPlaceY,0),1.0)
hab:addAnimeChip(sound)
sound: setDelay(fumikiri_life*0.5+1)

--
hab_parallel = AnimeChip_Hab(PositionPerGrid(toPlaceX, toPlaceY, 0))

-- =============================================================
-- 剣の軌跡
life_swing = 16
belt_point_num = 20
color_swing = Color(230,70,70,70)

s_okuRate = 0.5
s_temaeRate = -0.2

aniBelt11 = AnimeChip_Belt("effect\\equip\\sword_swing2.png",PositionPerGrid(0, 0, 0),PositionPerGrid(0, 0, 0),life_swing)
aniBelt11:setDelay(3)
aniBelt11.drawRange: addCascade(D3DXVECTOR2(1,1),D3DXVECTOR2(1,belt_point_num-1),D3DXVECTOR2(1,belt_point_num-1),D3DXVECTOR2(belt_point_num-1,belt_point_num-1),life_swing)
aniBelt11.color: addCascade(color_swing, life_swing)
aniBelt11.position: addCascade(PositionPerGrid(0, 1.1, 0.6), life_swing)
aniBelt11:setWorldRotation(direction)
aniBelt11.drawMode: setDefault(DrawMode_normal)
aniBelt11.textureRange: setDefault(TextureRangeRect(1,0,0,1))


for k = 0, belt_point_num do
	angle =  ( (k/belt_point_num)*180 + 0) * MATH_PI/180	-- sinSitaの値を作成するための角度
	sinSita = sin( angle )					-- kによって0～1で変化する値

	pos = PositionPerGrid(1, 0, 0.4)			-- ベースの位置ベクトル(先端位置)
	angle =  ( (k/belt_point_num)*60 + 60) * -MATH_PI/180	-- 回転角度
		
	rotAxis = PositionPerGrid(-0.7, 0, 1)		-- 回転軸
	D3DXVec3Rotate( pos, rotAxis, angle, pos )	-- 位置ベクトルを回転
	
	oku	= pos + ( pos * 1* s_okuRate )
	temae	= pos + ( pos * sinSita* s_temaeRate )
	aniBelt11.rightEdge: push_back(VariationValue_PositionPerGrid(temae))	-- 手前
	aniBelt11.leftEdge: push_back(VariationValue_PositionPerGrid(oku))	-- 奥
end

-- コピーして加算合成
aniBelt12 = AnimeChip_Belt(aniBelt11);
aniBelt12.color:clear()
aniBelt12.color: addCascade(Color(140,150,150,150), life_swing)
aniBelt12.drawMode: setDefault(DrawMode_addition)
aniBelt12:setDelay(1)

aniBelt13 = AnimeChip_Belt(aniBelt12);
aniBelt13.color:clear()
aniBelt13.color: addCascade(Color(230,50,50,50), life_swing)
aniBelt13:setDelay(8)

hab_parallel: addAnimeChip(aniBelt11);
hab_parallel: addAnimeChip(aniBelt12);
hab_parallel: addAnimeChip(aniBelt13);
-- =============================================================

-- =============================================================
-- 火花
spark = { }
sparkAdd = { }
effectlife = 10
hiMax = 40
for k = 0, hiMax
do
	x_speed = (random()-0.5)*3

	pos = PositionPerGrid(1, 0, 0.4)		-- ベースの位置ベクトル(先端位置)
	angle =  ( (k/hiMax)*80 + 50) * -MATH_PI/180	-- 回転角度
		
	rotAxis = PositionPerGrid(-0.7, 0, 1)		-- 回転軸
	D3DXVec3Rotate( pos, rotAxis, angle, pos )	-- 位置ベクトルを回転

	-- 火花が飛ぶ位置 オフセット有り
	hi	= pos + PositionPerGrid(0, 1.1 - 0.5*random(), 0.2)
	
	spark[k] = AnimeChip_Bound("effect\\basic\\square_gradation.png", D3DXVECTOR2(6,5), PositionPerGrid(x_speed, 0, 7+random()*3), effectlife)
	spark[k].scaleX:	addCascade(1, random(), effectlife)
	spark[k].scaleY:	addCascade(1, random(), effectlife)
	spark[k].color: addCascade(Color(240,235+20*random(),170+30*random(),30), effectlife)
	spark[k].gravity: addCascade(3, effectlife)
	spark[k].position: addCascade(hi, effectlife)
	spark[k].rotationBefore: addCascade(random()*360, 0, effectlife)
	spark[k]: setDelay((16/hiMax)*k)
	spark[k]:setWorldRotation(direction)
	
	x_speed = (random()-0.5)*3
	hi	= pos + PositionPerGrid(0, 1.1 - 0.5*random(), 0.2)
	
	sparkAdd[k] = AnimeChip_Bound("effect\\basic\\square_gradation.png", D3DXVECTOR2(6,5), PositionPerGrid(x_speed, 0, 7+random()*3), effectlife)
	sparkAdd[k].scaleX:	addCascade(1, random(), effectlife)
	sparkAdd[k].scaleY:	addCascade(1, random(), effectlife)
	sparkAdd[k].color: addCascade(Color(240,235+20*random(),170+30*random(),30), effectlife)
	sparkAdd[k].gravity: addCascade(3, effectlife)
	sparkAdd[k].position: addCascade(hi, effectlife)
	sparkAdd[k].rotationBefore: addCascade(random()*360, 0, effectlife)
	sparkAdd[k]: setDelay((16/hiMax)*k)
	sparkAdd[k]:setWorldRotation(direction)
	sparkAdd[k].drawMode: setDefault(DrawMode_addition)
	
	
	hab_parallel: addAnimeChip(spark[k]);
	hab_parallel: addAnimeChip(sparkAdd[k]);
end
-- =============================================================


setAnimation_parallel(hab_parallel)
setAnimation_wait(hab)