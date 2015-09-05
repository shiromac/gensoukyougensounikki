--悔悟の棒

-- 数値型 fromPlaceX : 攻撃者のX位置（マス）
-- 数値型 fromPlaceY : 攻撃者のY位置（マス）
-- 数値型 toPlaceX : 攻撃先のX位置（マス）
-- 数値型 toPlaceY : 攻撃先のY位置（マス）
-- 数値型 direction : 攻撃者の向き（０～３６０）
-- 数値型 distance : 攻撃者と攻撃先の距離（マス）
-- pcCharacter型 attacker : 攻撃者のキャラクター


hab = AnimeChip_Hab(PositionPerGrid(toPlaceX, toPlaceY, 0))


-- fumikiri
fumikiri_life = 16
fumikomi_distance = sqrt((fromPlaceX - toPlaceX)*(fromPlaceX - toPlaceX) + (fromPlaceY - toPlaceY)*(fromPlaceY - toPlaceY))/ distance 

chara_anime = AnimeChip_Chara(attacker,fumikiri_life)
hab:addAnimeChip(chara_anime)
chara_anime: setWorldRotation(direction)
chara_anime.position: addCascade(PositionPerGrid(0, 0, 0), PositionPerGrid(0, -0.1, 0), PositionPerGrid(0, -0.1, 0), fumikiri_life*0.5)
chara_anime.position: addCascade(PositionPerGrid(0, -0.1, 0), PositionPerGrid(0, -0.8*fumikomi_distance, 0), PositionPerGrid(0, -0.4*fumikomi_distance, 0), PositionPerGrid(0, 0, 0), fumikiri_life*0.5)
chara_anime.yawing: addCascade(0, 45, fumikiri_life*0.5)
chara_anime.yawing: addCascade(45, -90, 0, fumikiri_life*0.5)
--

-- sound
sound = AnimeChip_Sound("swing1031.wav",PositionPerGrid(toPlaceX,toPlaceY,0),1.0)
hab:addAnimeChip(sound)
sound: setDelay(fumikiri_life*0.8)

--
hab_parallel = AnimeChip_Hab(PositionPerGrid(toPlaceX, toPlaceY, 0))


--life_swing = 16
belt_point_num = 16
--color_swing = Color(255,255,255,255)

--belt_anime = AnimeChip_Belt("effect\\basic\\ball_simple.png",PositionPerGrid(0.1, 0, 0),PositionPerGrid( -0.1, 0, 0),life_swing)
--belt_anime.drawRange: addCascade(D3DXVECTOR2(1,1),D3DXVECTOR2(1,belt_point_num-1),D3DXVECTOR2(1,belt_point_num-1),D3DXVECTOR2(belt_point_num-1,belt_point_num-1),life_swing)
--belt_anime.color: addCascade(color_swing, life_swing)
--belt_anime.position: addCascade(PositionPerGrid(0, 1, 0.7), life_swing)
--belt_anime:setWorldRotation(direction)
--belt_anime.drawMode: setDefault(DrawMode_normal)
--belt_anime.textureRange: setDefault(TextureRangeRect(1,0,0,1))

 

--for k = 0, belt_point_num do
	
	--vec =PositionPerGrid(0, 0, 1)
	--radian = ((k/belt_point_num)+0.5)*70*MATH_PI/180
	--vec =PositionPerGrid(0, -sin(radian), cos(radian))

	--turn_matrix = D3DXMATRIX()
	--D3DXMatrixRotationX(turn_matrix, ((k/belt_point_num)+0.4)*70*MATH_PI/180 )
 	--D3DXVec3TransformCoord(vec,vec,turn_matrix)

	--rotAxis = PositionPerGrid(1, 0, 0)
	--angle = ((k/belt_point_num)+0.4)*70*MATH_PI/180
	--D3DXVec3Rotate( vec, rotAxis, angle, vec )




	--posl = vec
	--posr = vec*1.1
	--posr = PositionPerGrid(3,0,0)

	--pos = PositionPerGrid(0.6, 0, 0)
	--rotAxis = PositionPerGrid(k/20, -k/10, 1)
	--angle =  ( (k/belt_point_num)*180 - 220 ) * MATH_PI/180
	--D3DXVec3Rotate( pos, rotAxis, angle, pos )



	--belt_anime.rightEdge: push_back(VariationValue_PositionPerGrid(posl))
	--belt_anime.leftEdge: push_back(VariationValue_PositionPerGrid(posr))

--end

stick = {}

for i=0, 10 
do
	stick[i]= {}


	for j=0, 3 
	do
		stick[i][j] = AnimeChip_Perticle("effect\\basic\\ring_out.png",D3DXVECTOR2(15,15), 16);
		stick[i][j]:setWorldRotation(direction)
		stick[i][j].color: addCascade(Color(200,255,255,255), 16)
		stick[i][j]:setDelay(j)

		for k = 0, belt_point_num do
	
			radian = (((k+i*0.1)/belt_point_num)+0.5)*80*MATH_PI/180
			vec =PositionPerGrid(0, 1.2-1.4*sin(radian*0.8), 0.7+1.6*cos(radian))
	
			stick[i][j].position: addCascade(vec, 1)
			stick[i][j]. scaleX: addCascade((k/belt_point_num), 1)
			stick[i][j]. scaleY: addCascade(0.3, 1)

		end
	
	hab_parallel: addAnimeChip(stick[i][j]);
	
	end
end


--hab_parallel: addAnimeChip(belt_anime);


setAnimation_parallel(hab_parallel)
setAnimation_wait(hab)

