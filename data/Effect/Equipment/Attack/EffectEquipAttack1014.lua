--イナバの杵

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
sound = AnimeChip_Sound("swing1014.wav",PositionPerGrid(toPlaceX,toPlaceY,0),1.0)
hab:addAnimeChip(sound)
sound: setDelay(10)

--
hab_parallel = AnimeChip_Hab(PositionPerGrid(toPlaceX, toPlaceY, 0))

-- =============================================================
-- 剣の軌跡
life_swing = 16
belt_point_num = 16

stick = {}

for i=0, 20 
do
	stick[i]= {}


	for j=0, 5 
	do
		stick[i][j] = AnimeChip_Perticle("effect\\basic\\ring_out.png",D3DXVECTOR2(50,50), 12)
		stick[i][j]:setWorldRotation(direction)
		stick[i][j].color: addCascade(Color(50,210,163,17), 12)
		stick[i][j]:setDelay(j)

		for k = 0, belt_point_num do
	
			radian = (((k+i*0.05)/belt_point_num)+0.5)*90*MATH_PI/180
			vec =PositionPerGrid(0, 0.9-0.9*sin(radian*0.8), 0.7+1.6*cos(radian))
	
			stick[i][j].position: addCascade(vec, 1)
			stick[i][j]. scaleX: addCascade(((k+10)/belt_point_num)*0.8, 1)
			stick[i][j]. scaleY: addCascade(0.3, 1)

		end
	
	hab_parallel: addAnimeChip(stick[i][j]);
	
	end
end

shock= AnimeChip_Perticle("effect\\basic\\ring_out.png",D3DXVECTOR2(50,50), 8)
shock.position:addCascade(PositionPerGrid(0, 0, 0), 8)
shock.scaleX: addCascade(1, 2.5, 8)
shock.scaleY: addCascade(0.3, 1.3, 8)
shock:setDelay(12)
hab_parallel: addAnimeChip(shock);
-- =============================================================


setAnimation_parallel(hab_parallel)
setAnimation_wait(hab)