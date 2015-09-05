--1033 お払い棒

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
fumikomi_distance = isValidAttack * 1.3 * sqrt((fromPlaceX - toPlaceX)*(fromPlaceX - toPlaceX) + (fromPlaceY - toPlaceY)*(fromPlaceY - toPlaceY))/ distance 

chara_anime = AnimeChip_Chara(attacker,fumikiri_life)
hab:addAnimeChip(chara_anime)
chara_anime: setWorldRotation(direction)
chara_anime.position: addCascade(PositionPerGrid(0, 0, 0), PositionPerGrid(0, -0.1, 0), PositionPerGrid(0, -0.1, 0), fumikiri_life*0.5)
chara_anime.position: addCascade(PositionPerGrid(0, -0.1, 0), PositionPerGrid(0, -0.8*fumikomi_distance, 0), PositionPerGrid(0, -0.4*fumikomi_distance, 0), PositionPerGrid(0, 0, 0), fumikiri_life*0.5)
chara_anime.yawing: addCascade(0, 45, fumikiri_life*0.5)
--chara_anime.yawing: addCascade(45, -90, 0, fumikiri_life*0.5)


-- sound
sound = AnimeChip_Sound("swing1033.wav",PositionPerGrid(toPlaceX,toPlaceY,0),1.0)
hab:addAnimeChip(sound)
sound: setDelay(fumikiri_life*0.5+1)

--
hab_parallel = AnimeChip_Hab(PositionPerGrid(toPlaceX, toPlaceY, 0))



--###########################
-- Swing
--###########################
LifeSwing1 = 6
belt_point_num = 30
color_swing = Color(200,150,150,150)
s_okuRate = 0.1
s_temaeRate = -0.2
aniBelt11 = AnimeChip_Belt("effect\\equip\\sword_swing.png",PositionPerGrid(0, 0, 0),PositionPerGrid(0, 0, 0), LifeSwing1)
aniBelt11: setDelay(2)
aniBelt11.drawRange: addCascade(D3DXVECTOR2(1,1),D3DXVECTOR2(1,belt_point_num-1),D3DXVECTOR2(1,belt_point_num-1),D3DXVECTOR2(belt_point_num-1,belt_point_num-1), LifeSwing1)
aniBelt11.color: addCascade(color_swing, LifeSwing1)
aniBelt11.position: addCascade(PositionPerGrid(0, 1.3, 0.6), LifeSwing1)
aniBelt11:setWorldRotation(direction)
aniBelt11.drawMode: setDefault(DrawMode_normal)
aniBelt11.textureRange: setDefault(TextureRangeRect(1,0,0,1))

for k = 0, belt_point_num do
	-- swing sita
	angle =  ( (k/belt_point_num)*180 + 0) * MATH_PI/180
	sinSita = sin( angle )

	-- baseVector
	pos = PositionPerGrid(0, 0, 1)
	angle =  ( (k/belt_point_num)*50 + 40) * -MATH_PI/180
		
	-- rotation
	rotAxis = PositionPerGrid(-1, 0, 0)
	D3DXVec3Rotate( pos, rotAxis, angle, pos )
	
	oku	= pos + ( pos * sinSita* s_okuRate )
	temae	= pos + ( pos * sinSita* s_temaeRate )
	aniBelt11.rightEdge: push_back(VariationValue_PositionPerGrid(temae))
	aniBelt11.leftEdge: push_back(VariationValue_PositionPerGrid(oku))
end
aniBelt12 = AnimeChip_Belt(aniBelt11);
aniBelt12.drawMode: setDefault(DrawMode_addition)
hab_parallel: addAnimeChip(aniBelt11);
hab_parallel: addAnimeChip(aniBelt12);

--###########################
-- Onmyo ball
--###########################
ball = { }
effectlife = 20
kMax = 5
for k = 0, kMax
do
	v = PositionPerGrid(1.0, 0, 0)
	angle = ( (k/kMax)*360 - 180) * MATH_PI/180
	v.x = 2.5 * sin( angle )
	v.y = 1.5 * cos( angle )

	ball[k] = AnimeChip_Bound("effect\\equip\\onmyoball.png", D3DXVECTOR2(25,25), v, effectlife)
	ball[k].color: addCascade(Color(230,250,250,250), LifeSwing1)
	ball[k].scaleX:	addCascade(1.5, 0, effectlife)
	ball[k].scaleY:	addCascade(1.5, 0, effectlife)
	ball[k].gravity: addCascade(25, effectlife)
	ball[k].position: addCascade(PositionPerGrid(0,0,0.5), effectlife)
	ball[k].rotationBefore: addCascade(360, 0, effectlife)
	ball[k]: setDelay(8)
	hab_parallel: addAnimeChip(ball[k]);
end

--###########################
-- shock motion
--###########################
ex = AnimeChip_Bound("effect\\basic\\ring_radio.png", D3DXVECTOR2(70,70), PositionPerGrid(0, 0, 0), 4)
ex.position: addCascade(PositionPerGrid(0,0,0.5), effectlife)
ex.rotationBefore: addCascade(100, 0, effectlife)
ex: setDelay(8)
hab_parallel: addAnimeChip(ex);




setAnimation_parallel(hab_parallel)
setAnimation_wait(hab)
