--針

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
chara_anime.position: addCascade(PositionPerGrid(0, 0, 0), PositionPerGrid(-0.1, 0, 0), PositionPerGrid(0.1, 0, 0), fumikiri_life*0.5)
chara_anime.position: addCascade(PositionPerGrid(0, -0.1, 0), PositionPerGrid(0, -0.8*fumikomi_distance, 0), PositionPerGrid(0, -0.4*fumikomi_distance, 0), PositionPerGrid(0, 0, 0), fumikiri_life*0.5)
chara_anime.yawing: addCascade(0, 45, fumikiri_life*0.5)
chara_anime.yawing: addCascade(45, -90, 0, fumikiri_life*0.5)
--

-- sound
sound = AnimeChip_Sound("swing1038.wav",PositionPerGrid(toPlaceX,toPlaceY,0),1.0)
hab:addAnimeChip(sound)
sound: setDelay(fumikiri_life*0.5+1)

--
hab_parallel = AnimeChip_Hab(PositionPerGrid(toPlaceX, toPlaceY, 0))

-- =============================================================
-- 剣の軌跡
function needle(position, delay)

	life_swing = 8
	belt_point_num = 20
	color_swing = Color(250,200,50,200)
	width = 0.1
	aniBelt11 = AnimeChip_Belt("effect\\basic\\ball_simple.png",PositionPerGrid(0, 0, 0),PositionPerGrid(0, 0, 0),life_swing)
	aniBelt11:setDelay(4 + delay)
	aniBelt11.drawRange: addCascade(D3DXVECTOR2(1,1),D3DXVECTOR2(1,belt_point_num-1),D3DXVECTOR2(1,belt_point_num-1),D3DXVECTOR2(belt_point_num-1,belt_point_num-1),life_swing)
	aniBelt11.color: addCascade(color_swing, life_swing)
	aniBelt11.position: addCascade(PositionPerGrid(0, 0.8, 0.6) + position, life_swing)
	aniBelt11:setWorldRotation(direction)
	aniBelt11.drawMode: setDefault(DrawMode_normal)
	aniBelt11.textureRange: setDefault(TextureRangeRect(1,0,0,1))
	
	for k = 0, belt_point_num do

		leftpoint = VariationValue_PositionPerGrid(PositionPerGrid(0,0,0))
		rightpoint = VariationValue_PositionPerGrid(PositionPerGrid(0,0,0))
		leftpoint: addCascade(PositionPerGrid(width, -k/belt_point_num*1.3, 0), PositionPerGrid(width/2, -k/belt_point_num*1.3, 0), life_swing)
		rightpoint: addCascade(PositionPerGrid(-width, -k/belt_point_num*1.3, 0), PositionPerGrid(-width/2, -k/belt_point_num*1.3, 0), life_swing)
		
		aniBelt11.rightEdge: push_back(rightpoint)	-- 手前
		aniBelt11.leftEdge: push_back(leftpoint)	-- 奥
	end
	
	-- コピーして加算合成
	aniBelt12 = AnimeChip_Belt(aniBelt11);
	aniBelt12.drawMode: setDefault(DrawMode_addition)
	aniBelt12.color: clear()
	aniBelt12.color: addCascade(Color(250,150,150,250), life_swing)
	aniBelt12: setLeftEdgeStartingPositionalPoint(PositionPerGrid(-0.05, 0, 0))
	aniBelt12: setRightEdgeStartingPositionalPoint(PositionPerGrid(0.05, 0, 0))
	hab_parallel: addAnimeChip(aniBelt11);
	hab_parallel: addAnimeChip(aniBelt12);
	
	
	hit_life = 6
	hitPerticle = AnimeChip_Perticle("effect\\basic\\ring_radio.png",D3DXVECTOR2(16,16),hit_life)
	hitPerticle.rotationBefore: setDefault(random()*360)
	hitPerticle.rotationAfter: setDefault(direction)
	hitPerticle:setWorldRotation(direction)
	hitPerticle.position: addCascade(PositionPerGrid(0, -0.3, 0.6) + position, hit_life)
	hitPerticle.scaleX: addCascade(1,4,hit_life)
	hitPerticle.scaleY: addCascade(0.5,2,hit_life)
	hitPerticle.color: addCascade(color_swing, hit_life)
	hitPerticle:setDelay(4+life_swing - 4 + delay)
	
	hitPerticle2 = AnimeChip_Perticle(hitPerticle);
	hitPerticle2.drawMode: setDefault(DrawMode_addition)
	hitPerticle2.color: clear()
	hitPerticle2.color: addCascade(Color(200,150,150,250), life_swing)
	
	
	hab_parallel: addAnimeChip(hitPerticle);
	hab_parallel: addAnimeChip(hitPerticle2);

end


needle( PositionPerGrid(0, 0, 0), 0)
needle( PositionPerGrid(0.25, 0.1, -0.1), 3)
needle( PositionPerGrid(-0.25, 0.2, -0.2), 6)
-- =============================================================


setAnimation_parallel(hab_parallel)
setAnimation_wait(hab)