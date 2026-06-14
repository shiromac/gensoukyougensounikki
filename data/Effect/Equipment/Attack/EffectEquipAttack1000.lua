--打刀

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
sound = AnimeChip_Sound("swing1000.wav",PositionPerGrid(toPlaceX,toPlaceY,0),1.0)
hab:addAnimeChip(sound)
sound: setDelay(fumikiri_life*0.5+1)

--
hab_parallel = AnimeChip_Hab(PositionPerGrid(toPlaceX, toPlaceY, 0))


life_swing = 8
belt_point_num = 30
color_swing = Color(128,200,200,200)

belt_anime = AnimeChip_Belt("effect\\equip\\sword_swing.png",PositionPerGrid(0, -0.35, 0.5),PositionPerGrid(0, 0, 0),life_swing)
belt_anime: setDelay(fumikiri_life*0.5)
belt_anime.drawRange: addCascade(D3DXVECTOR2(1,1),D3DXVECTOR2(1,belt_point_num-1),D3DXVECTOR2(1,belt_point_num-1),D3DXVECTOR2(belt_point_num-1,belt_point_num-1),life_swing)
belt_anime.color: addCascade(color_swing, life_swing)
belt_anime.position: addCascade(PositionPerGrid(0, 0.6, 0.7), life_swing)
belt_anime:setWorldRotation(direction)
belt_anime.drawMode: setDefault(DrawMode_normal)
belt_anime.textureRange: setDefault(TextureRangeRect(1,0,0,1))

base_trun = random()*360


for k = 0, belt_point_num do
	turn_matrix = D3DXMATRIX()

	pos = PositionPerGrid(0,0,1)

	D3DXMatrixRotationX(turn_matrix, ((k/belt_point_num)*300-75)*MATH_PI/180 )
	D3DXVec3TransformCoord(pos,pos,turn_matrix)

	D3DXMatrixRotationY(turn_matrix, 30*MATH_PI/(180) )
	D3DXVec3TransformCoord(pos,pos,turn_matrix)

	D3DXMatrixRotationZ(turn_matrix, 5*MATH_PI/(180) )
	D3DXVec3TransformCoord(pos,pos,turn_matrix)
	pos.x = pos.x*0.5

	belt_anime.rightEdge: push_back(VariationValue_PositionPerGrid(pos*0.4))
	belt_anime.leftEdge: push_back(VariationValue_PositionPerGrid(pos))

end
belt_anime2 = AnimeChip_Belt(belt_anime);
belt_anime2: setLeftEdgeStartingPositionalPoint(PositionPerGrid(0.0, -0.35, 0.5))
belt_anime2: setRightEdgeStartingPositionalPoint(PositionPerGrid(0.05, 0, 0))

belt_anime3 = AnimeChip_Belt(belt_anime);
belt_anime3.drawMode: setDefault(DrawMode_addition)
belt_anime3: setDelay(fumikiri_life*0.5+2)
belt_anime4 = AnimeChip_Belt(belt_anime2);
belt_anime4.drawMode: setDefault(DrawMode_addition)
belt_anime4: setDelay(fumikiri_life*0.5+2)

hab_parallel: addAnimeChip(belt_anime);
hab_parallel: addAnimeChip(belt_anime2);
hab_parallel: addAnimeChip(belt_anime3);
hab_parallel: addAnimeChip(belt_anime4);


setAnimation_parallel(hab_parallel)
setAnimation_wait(hab)
