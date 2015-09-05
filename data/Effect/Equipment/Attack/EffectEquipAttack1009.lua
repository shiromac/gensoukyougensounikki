--ódêÓ

-- êîílå^ fromPlaceX : çUåÇé“ÇÃXà íuÅiÉ}ÉXÅj
-- êîílå^ fromPlaceY : çUåÇé“ÇÃYà íuÅiÉ}ÉXÅj
-- êîílå^ toPlaceX : çUåÇêÊÇÃXà íuÅiÉ}ÉXÅj
-- êîílå^ toPlaceY : çUåÇêÊÇÃYà íuÅiÉ}ÉXÅj
-- êîílå^ direction : çUåÇé“ÇÃå¸Ç´ÅiÇOÅ`ÇRÇUÇOÅj
-- êîílå^ distance : çUåÇé“Ç∆çUåÇêÊÇÃãóó£ÅiÉ}ÉXÅ
-- êîílå^ isValidAttack : çUåÇÇ™ï«Ç…å¸Ç©Ç¡ÇƒÇ¢ÇΩÇÁÇOÅAÇªÇ§Ç≈Ç»ÇØÇÍÇŒÇPj
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
--chara_anime.yawing: addCascade(0, -45, fumikiri_life*0.5)
--chara_anime.yawing: addCascade(-45, 90, 0, fumikiri_life*0.5)
--

-- sound
sound = AnimeChip_Sound("swing1009.wav",PositionPerGrid(toPlaceX,toPlaceY,0),1.0)
hab:addAnimeChip(sound)
sound: setDelay(10)

--
hab_parallel = AnimeChip_Hab(PositionPerGrid(toPlaceX, toPlaceY, 0))


----- Nway Bullet
AllFlame = 24
Nway = 4
bfly = { }

for k = 0, Nway do
	-- äOë§ÇÃíeñã
	angle = ( k / Nway )*200 - (200/2)
	endPos =  PositionPerGrid(0, -0.8, 0.6)
	
	turn_matrix = D3DXMATRIX()
	D3DXMatrixRotationZ(turn_matrix, (angle * MATH_PI/180) )
	D3DXVec3TransformCoord(endPos, endPos, turn_matrix)
	
	colorBase = Color(130,(255*random()), (255*random()), (255*random()))
	
	bfly[k] = AnimeChip_Perticle("effect\\equip\\butterfly.png", D3DXVECTOR2(24,18), AllFlame)
	bfly[k]:setDelay(4)
	bfly[k]:setWorldRotation(direction)
	bfly[k].rotationBefore: setDefault(direction + 0.5*angle)
	bfly[k].color: addCascade(colorBase, AllFlame)
	bfly[k].position: addCascade(PositionPerGrid(0, 0.4, 0.6), endPos, AllFlame)
	bfly[k].drawMode: setDefault(DrawMode_addition)
	bfly[k].textureRange: setDefault(TextureRangeRect(1,0,0,1))
	
	hab_parallel: addAnimeChip(bfly[k])
end

for k = 0, Nway-1 do
	-- ì‡ë§ÇÃíeñã
	angle = ( k / (Nway-1) )*200 - (200/2)

	endPos = PositionPerGrid(0, -0.8, 0.6)
	D3DXMatrixRotationZ(turn_matrix, (angle * MATH_PI/180) )
	D3DXVec3TransformCoord(endPos, endPos, turn_matrix)
	endPos = endPos*0.5
	
	colorBase = Color(130,(255*random()), (255*random()), (255*random()))
	
	bfly[Nway+k] = AnimeChip_Perticle("effect\\equip\\butterfly.png", D3DXVECTOR2(24,18), AllFlame)
	bfly[Nway+k]:setDelay(4)
	bfly[Nway+k]:setWorldRotation(direction)
	bfly[Nway+k].rotationBefore: setDefault(direction + 0.5*angle)
	bfly[Nway+k].color: addCascade(colorBase, AllFlame)
	bfly[Nway+k].position: addCascade(PositionPerGrid(0, 0.4, 0.6), endPos, AllFlame)
	bfly[Nway+k].drawMode: setDefault(DrawMode_addition)
	bfly[Nway+k].textureRange: setDefault(TextureRangeRect(1,0,0,1))

	hab_parallel: addAnimeChip(bfly[Nway+k]);
end

-- â∫ínóp
for k =  0, Nway*2 do
	bfly[Nway*2 + k] = AnimeChip_Perticle(bfly[k])
	bfly[k].drawMode: setDefault(DrawMode_normal)
	hab_parallel: addAnimeChip(bfly[Nway*2 + k])
end



--[[
color_swing = Color(200,200,200,200)

bfly = AnimeChip_Perticle("effect\\equip\\butterfly.png", D3DXVECTOR2(20,20), 16)
bfly:setDelay(4)
bfly:setWorldRotation(direction)
bfly.rotationBefore: addCascade(direction, 16)
bfly.color: addCascade(colorBase, 16)
bfly.position: addCascade(PositionPerGrid(0, 0.4, 0.6), PositionPerGrid(0, -0.5, 0.6), 16)
bfly.drawMode: setDefault(DrawMode_normal)
bfly.textureRange: setDefault(TextureRangeRect(1,0,0,1))

hab_parallel: addAnimeChip(bfly);
]]


setAnimation_parallel(hab_parallel)
setAnimation_wait(hab)
