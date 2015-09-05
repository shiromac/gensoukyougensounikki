--1001 íZåï
-- êîílå^ fromPlaceX : çUåÇé“ÇÃXà íuÅiÉ}ÉXÅj
-- êîílå^ fromPlaceY : çUåÇé“ÇÃYà íuÅiÉ}ÉXÅj
-- êîílå^ toPlaceX : çUåÇêÊÇÃXà íuÅiÉ}ÉXÅj
-- êîílå^ toPlaceY : çUåÇêÊÇÃYà íuÅiÉ}ÉXÅj
-- êîílå^ direction : çUåÇé“ÇÃå¸Ç´ÅiÇOÅ`ÇRÇUÇOÅj
-- êîílå^ distance : çUåÇé“Ç∆çUåÇêÊÇÃãóó£ÅiÉ}ÉXÅj
-- êîílå^ isValidAttack : çUåÇÇ™ï«Ç…å¸Ç©Ç¡ÇƒÇ¢ÇΩÇÁÇOÅAÇªÇ§Ç≈Ç»ÇØÇÍÇŒÇP
-- pcCharacterå^ attacker : çUåÇé“ÇÃÉLÉÉÉâÉNÉ^Å[


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
sound = AnimeChip_Sound("swing1001.wav",PositionPerGrid(toPlaceX,toPlaceY,0),1.0)
hab:addAnimeChip(sound)
sound: setDelay(fumikiri_life*0.5+1)

--
hab_parallel = AnimeChip_Hab(PositionPerGrid(toPlaceX, toPlaceY, 0))


LifeSwing1 = 8
LifeSwing2 = 8
belt_point_num = 30
color_swing = Color(200,150,150,250)

s_okuRate = 0.2
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
aniBelt12.drawMode: setDefault(DrawMode_addition)
--aniBelt13 = AnimeChip_Belt(aniBelt11);
--aniBelt13: setDelay(8)
hab_parallel: addAnimeChip(aniBelt11);
hab_parallel: addAnimeChip(aniBelt12);
--hab_parallel: addAnimeChip(aniBelt13);


--############################
-- second swing
--############################
aniBelt21 = AnimeChip_Belt("effect\\equip\\sword_swing.png",PositionPerGrid(0, 0, 0),PositionPerGrid(0, 0, 0), LifeSwing2)
aniBelt21: setDelay(10)
aniBelt21.drawRange: addCascade(D3DXVECTOR2(1,1),D3DXVECTOR2(1,belt_point_num-1),D3DXVECTOR2(1,belt_point_num-1),D3DXVECTOR2(belt_point_num-1,belt_point_num-1), LifeSwing2)
aniBelt21.color: addCascade(color_swing, LifeSwing2)
aniBelt21.position: addCascade(PositionPerGrid(0, 1.3, 0.6), LifeSwing2)
aniBelt21:setWorldRotation(direction)
aniBelt21.drawMode: setDefault(DrawMode_normal)
aniBelt21.textureRange: setDefault(TextureRangeRect(1,0,0,1))

for k = 0, belt_point_num do
	-- swing sita
	angle =  ( (k/belt_point_num)*180 + 0) * MATH_PI/180
	sinSita = sin( angle )
	
	-- baseVector
	pos = PositionPerGrid(-1, 0, 0.6)
	angle =  ( (k/belt_point_num)*100 + 40) * MATH_PI/180
		
	-- rotation
	rotAxis = PositionPerGrid(0.7, 0, 1)
	D3DXVec3Rotate( pos, rotAxis, angle, pos )
			
	oku	= pos + ( pos * sinSita* s_okuRate )
	temae	= pos + ( pos * sinSita* s_temaeRate )
	aniBelt21.rightEdge: push_back(VariationValue_PositionPerGrid(temae))
	aniBelt21.leftEdge: push_back(VariationValue_PositionPerGrid(oku))
	
	--aniBelt21.rightEdge: push_back(VariationValue_PositionPerGrid(pos*0.5))
	--aniBelt21.leftEdge: push_back(VariationValue_PositionPerGrid(pos))
end

aniBelt22 = AnimeChip_Belt(aniBelt21);
aniBelt22.drawMode: setDefault(DrawMode_addition)
--aniBelt23 = AnimeChip_Belt(aniBelt11);
--aniBelt23: setDelay(8)
hab_parallel: addAnimeChip(aniBelt21);
hab_parallel: addAnimeChip(aniBelt22);
--hab_parallel: addAnimeChip(aniBelt23);




setAnimation_parallel(hab_parallel)
setAnimation_wait(hab)