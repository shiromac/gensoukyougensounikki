--天狗団扇

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
sound = AnimeChip_Sound("swing1027.wav",PositionPerGrid(toPlaceX,toPlaceY,0),1.0)
hab:addAnimeChip(sound)
sound: setDelay(fumikiri_life*0.5+1)

--
hab_parallel = AnimeChip_Hab(PositionPerGrid(toPlaceX, toPlaceY, 0))


life_swing = 8
belt_point_num = 30
color_swing = Color(128,230,120,50)
--color_swing = Color(128,100,200,100)

--基本ベルト
belt_anime = AnimeChip_Belt("effect\\equip\\sword_swing.png",PositionPerGrid(0, 0, 0),PositionPerGrid(0, 0, 1),life_swing)
belt_anime: setDelay(fumikiri_life*0.5)
belt_anime.drawRange: addCascade(D3DXVECTOR2(1,1),D3DXVECTOR2(1,belt_point_num-1),D3DXVECTOR2(1,belt_point_num-1),D3DXVECTOR2(belt_point_num-1,belt_point_num-1),life_swing)
belt_anime.color: addCascade(color_swing, life_swing)
belt_anime.position: addCascade(PositionPerGrid(0, 0.8*fumikomi_distance, 0), life_swing)
belt_anime:setWorldRotation(direction)
belt_anime.drawMode: setDefault(DrawMode_normal)
--テクスチャどういう風に重ねてるの
belt_anime.textureRange: setDefault(TextureRangeRect(1,0,0,1))

base_trun = random()*360

for k = 0, belt_point_num do
	turn_matrix = D3DXMATRIX()

	pos = PositionPerGrid(0.6,0,0)

	D3DXMatrixRotationZ(turn_matrix, ((k/belt_point_num)*300+90)*MATH_PI/180 )
	D3DXVec3TransformCoord(pos,pos,turn_matrix)

	D3DXMatrixRotationY(turn_matrix, 30*MATH_PI/(180) )
	D3DXVec3TransformCoord(pos,pos,turn_matrix)

	D3DXMatrixRotationZ(turn_matrix, 5*MATH_PI/(180) )
	D3DXVec3TransformCoord(pos,pos,turn_matrix)

	belt_anime.leftEdge: push_back(VariationValue_PositionPerGrid(pos))
	belt_anime.rightEdge: push_back(VariationValue_PositionPerGrid(pos))

end

life_wind = 10

wind_perticle_anime = AnimeChip_Perticle("effect\\equip\\wind_green.png",D3DXVECTOR2(96,96),life_wind)
hab_parallel: addAnimeChip(wind_perticle_anime)
wind_perticle_anime: setWorldRotation(direction)
wind_perticle_anime.color: addCascade(Color(0,255,255,255),Color(512,255,255,255),Color(0,255,255,255),life_wind)
wind_perticle_anime.scaleX: addCascade(2,0.5,life_wind)
wind_perticle_anime.scaleY: addCascade(0,1,life_wind)
wind_perticle_anime.rotationBefore: setDefault(-90)
wind_perticle_anime.rotationAfter: setDefault(direction)
wind_perticle_anime.position: addCascade(PositionPerGrid(0, 1.5, 0.7),PositionPerGrid(0, -0.5, 0.7),life_wind)
wind_perticle_anime.drawMode: setDefault(DrawMode_addition)
wind_perticle_anime.position: setDefault(PositionPerGrid(0,0,0))


hab_parallel: addAnimeChip(belt_anime);
setAnimation_parallel(hab_parallel)
setAnimation_wait(hab)
