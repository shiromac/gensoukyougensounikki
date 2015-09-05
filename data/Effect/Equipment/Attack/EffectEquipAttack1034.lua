--魔女箒

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
sound = AnimeChip_Sound("swing1034.wav",PositionPerGrid(toPlaceX,toPlaceY,0),1.0)
hab:addAnimeChip(sound)
sound: setDelay(fumikiri_life*0.5+1)

--
hab_parallel = AnimeChip_Hab(PositionPerGrid(toPlaceX, toPlaceY, 0))

star = { }
effectlife = 36
count = 0
for k = 0, 4
do
	x_speed = (random()-0.5)*5

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

	if x_speed < 0
	then
		turn = 360
	else
		turn = -360
	end

	star[k] = AnimeChip_Bound("effect\\basic\\star.png", D3DXVECTOR2(25,25), PositionPerGrid(x_speed, 0, 7+random()*3), effectlife)
	star[k].scaleX:	addCascade(1, 0.3, effectlife)
	star[k].scaleY:	addCascade(1, 0.3, effectlife)
	star[k].gravity: addCascade(25, effectlife)
	star[k].position: addCascade(PositionPerGrid(0,0,0.8), effectlife)
	star[k].rotationBefore: addCascade(turn, 0, effectlife)
	star[k]: setDelay(fumikiri_life*0.5)
end

star[0].color: addCascade(Color(255,255,0,0), effectlife)
star[1].color: addCascade(Color(255,100,255,100), effectlife)
star[2].color: addCascade(Color(255,100,100,255), effectlife)
star[3].color: addCascade(Color(255,255,200,0), effectlife)
star[4].color: addCascade(Color(255,255,0,200), effectlife)

smoke = {}

smoke_num = 5


for k = 0, smoke_num
do
	smoke[k] = AnimeChip_Perticle("effect\\basic\\ball_smoke.png",D3DXVECTOR2(60,60), effectlife/2)
--	smoke[k].textureRange: addCascade(TextureRangeRect(0, 0, 0.25, 0.25), TextureRangeRect(0.75, 0, 1, 0.25), 3)
--	smoke[k].textureRange: addCascade(TextureRangeRect(0, 0.25, 0.25, 0.5), TextureRangeRect(0.75, 0.25, 1, 0.5), 3)
--	smoke[k].textureRange: addCascade(TextureRangeRect(0, 0.5, 0.25, 0.75), TextureRangeRect(0.75, 0.5, 1, 0.75), 3)
--	smoke[k].textureRange: addCascade(TextureRangeRect(0, 0.75, 0.25, 1), TextureRangeRect(0.75, 0.75, 1, 1), 3)
	if random()*2 > 1 then
		smoke[k].color: addCascade(Color(0,255,255,128),Color(255*2,255,255,128),Color(0,255,255,128), effectlife/2)
		smoke[k].drawMode: setDefault(DrawMode_normal)
	else
		smoke[k].color: addCascade(Color(0,128,128,128),Color(255*2,128,128,128),Color(0,128,128,128), effectlife/2)
		smoke[k].drawMode: setDefault(DrawMode_normal)
	end
	smoke[k].scaleX: addCascade(1, 2, 1, effectlife/2)
	smoke[k].scaleY: addCascade(1, 2, 1, effectlife/2)
	smoke[k].rotationAfter: addCascade(random()*360, random()*360, effectlife/2)
	smoke[k]: setDelay(fumikiri_life*(0.5+0.03*k))

	pos = PositionPerGrid((1+k/smoke_num)*random()*0.2,0,0.5)
	turn_matrix = D3DXMATRIX()
	D3DXMatrixRotationZ(turn_matrix, (random()*MATH_PI*2))
	D3DXVec3TransformCoord(pos,pos,turn_matrix)

	
	smoke[k].position: addCascade(pos ,effectlife)
end

for k = 0, 4 
do
	hab_parallel: addAnimeChip(star[k]);	
end

for k = 0, smoke_num 
do
	hab_parallel: addAnimeChip(smoke[k]);
end


-- life_swing = 18
-- belt_point_num = 30
-- color_swing = Color(128,200,200,200)

-- belt_anime = AnimeChip_Belt("effect\\basic\\star.png",PositionPerGrid(0, -0.35, 0.5),PositionPerGrid(0, 0, 0),life_swing)
-- belt_anime: setDelay(fumikiri_life*0.5)
-- belt_anime.drawRange: addCascade(D3DXVECTOR2(1,1),D3DXVECTOR2(1,belt_point_num-1),D3DXVECTOR2(1,belt_point_num-1),D3DXVECTOR2(belt_point_num-1,belt_point_num-1),life_swing)
-- belt_anime.color: addCascade(color_swing, life_swing)
-- belt_anime.position: addCascade(PositionPerGrid(0, 0.6, 0.7), life_swing)
-- belt_anime:setWorldRotation(direction)
-- belt_anime.drawMode: setDefault(DrawMode_normal)
-- belt_anime.textureRange: setDefault(TextureRangeRect(1,0,0,1))

-- base_trun = random()*360


-- for k = 0, belt_point_num do
-- 	turn_matrix = D3DXMATRIX()

-- 	pos = PositionPerGrid(0,0,1)

-- 	D3DXMatrixRotationX(turn_matrix, ((k/belt_point_num)*300-75)*MATH_PI/180 )
-- 	D3DXVec3TransformCoord(pos,pos,turn_matrix)

-- 	D3DXMatrixRotationY(turn_matrix, 30*MATH_PI/(180) )
-- 	D3DXVec3TransformCoord(pos,pos,turn_matrix)

-- 	D3DXMatrixRotationZ(turn_matrix, 5*MATH_PI/(180) )
-- 	D3DXVec3TransformCoord(pos,pos,turn_matrix)
-- 	pos.x = pos.x*0.5

-- 	belt_anime.rightEdge: push_back(VariationValue_PositionPerGrid(pos*0.4))
-- 	belt_anime.leftEdge: push_back(VariationValue_PositionPerGrid(pos))

-- end
-- belt_anime2 = AnimeChip_Belt(belt_anime);
-- belt_anime2: setLeftEdgeStartingPositionalPoint(PositionPerGrid(0.0, -0.35, 0.5))
-- belt_anime2: setRightEdgeStartingPositionalPoint(PositionPerGrid(0.05, 0, 0))

-- belt_anime3 = AnimeChip_Belt(belt_anime);
-- belt_anime3.drawMode: setDefault(DrawMode_addition)
-- belt_anime3: setDelay(fumikiri_life*0.5+2)
-- belt_anime4 = AnimeChip_Belt(belt_anime2);
-- belt_anime4.drawMode: setDefault(DrawMode_addition)
-- belt_anime4: setDelay(fumikiri_life*0.5+2)

-- hab_parallel: addAnimeChip(belt_anime);
-- hab_parallel: addAnimeChip(belt_anime2);
-- hab_parallel: addAnimeChip(belt_anime3);
-- hab_parallel: addAnimeChip(belt_anime4);


setAnimation_parallel(hab_parallel)
setAnimation_wait(hab)
