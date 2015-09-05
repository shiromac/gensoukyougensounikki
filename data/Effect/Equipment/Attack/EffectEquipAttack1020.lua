--1020 人切り包丁
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
sound = AnimeChip_Sound("swing1020.wav",PositionPerGrid(toPlaceX,toPlaceY,0),1.0)
hab:addAnimeChip(sound)
sound: setDelay(fumikiri_life*0.5+1)

--
hab_parallel = AnimeChip_Hab(PositionPerGrid(toPlaceX, toPlaceY, 0))


LifeSwing1 = 8
LifeSwing2 = 8
belt_point_num = 30
color_swing = Color(230,250,30,30)

s_okuRate = 0.5
s_temaeRate = -0.2


--############################
-- first swing
--############################
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
	pos = PositionPerGrid(1, 0, 0.6)
	angle =  ( (k/belt_point_num)*100 + 40) * -MATH_PI/180
		
	-- rotation
	rotAxis = PositionPerGrid(-0.7, 0, 1)
	D3DXVec3Rotate( pos, rotAxis, angle, pos )
	
	oku	= pos + ( pos * sinSita* s_okuRate )
	temae	= pos + ( pos * sinSita* s_temaeRate )
	aniBelt11.rightEdge: push_back(VariationValue_PositionPerGrid(temae))
	aniBelt11.leftEdge: push_back(VariationValue_PositionPerGrid(oku))
end

aniBelt12 = AnimeChip_Belt(aniBelt11);
--aniBelt12.drawMode: setDefault(DrawMode_addition)
--aniBelt13 = AnimeChip_Belt(aniBelt11);
--aniBelt13: setDelay(8)
hab_parallel: addAnimeChip(aniBelt11);
hab_parallel: addAnimeChip(aniBelt12);
--hab_parallel: addAnimeChip(aniBelt13);



chi = { }
effectlife = 30
count = 0
for k = 0, 3
do
	x_speed = (random()-0.5)*2

	if x_speed < 0
	then
		count = count+1
	end

	if count > 2
	then 
		x_speed = x_speed * -1
	else
		if k - count > 2
		then
			x_speed = x_speed * -1
		end
	end

	eLife = 0.5 * effectlife * random() + 10

	chi[k] = AnimeChip_Bound("effect\\basic\\splash.png", D3DXVECTOR2(32,32), PositionPerGrid(x_speed, 0, 1+random()*3), eLife)
	chi[k].color: addCascade(Color(140,255,40,40), eLife)
	chi[k].scaleX:	addCascade(random()*2 + 2, 0.5, eLife)
	chi[k].scaleY:	addCascade(random()*2 + 2, 0.5, eLife)
	chi[k].gravity: addCascade(10, effectlife)
	chi[k].position: addCascade(PositionPerGrid(0,0,0.4), eLife)
	chi[k].rotationBefore: addCascade(random()*360, eLife)
	chi[k]: setDelay(fumikiri_life*0.5)
	hab_parallel: addAnimeChip(chi[k]);	
end



setAnimation_parallel(hab_parallel)
setAnimation_wait(hab)