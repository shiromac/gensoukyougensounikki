--1042 壊れたお守り

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
sound = AnimeChip_Sound("swing1042.wav",PositionPerGrid(toPlaceX,toPlaceY,0),1.0)
hab:addAnimeChip(sound)
sound: setDelay(fumikiri_life*0.5+1)

--
hab_parallel = AnimeChip_Hab(PositionPerGrid(toPlaceX, toPlaceY, 0))


-- =============================================================
-- 雛の体当たり
hina = AnimeChip_Perticle("effect\\equip\\hina.png",D3DXVECTOR2(46,46), 16)
hina.color: addCascade(Color(255,255,120,255), 16)
hina.drawMode: setDefault(DrawMode_normal)
hina:setWorldRotation(direction)
hina: setDelay(0)

hina.textureRange:  addCascade(TextureRangeRect(0, 0, 0.25, 0.125), 2)
hina.textureRange:  addCascade(TextureRangeRect(0, 0.125*1, 0.25, 0.125*2), 2)
hina.textureRange:  addCascade(TextureRangeRect(0, 0.125*2, 0.25, 0.125*3), 2)
hina.textureRange:  addCascade(TextureRangeRect(0, 0.125*3, 0.25, 0.125*4), 2)
hina.textureRange:  addCascade(TextureRangeRect(0, 0.125*4, 0.25, 0.125*5), 2)
hina.textureRange:  addCascade(TextureRangeRect(0, 0.125*5, 0.25, 0.125*6), 2)
hina.textureRange:  addCascade(TextureRangeRect(0, 0.125*6, 0.25, 0.125*7), 2)
hina.textureRange:  addCascade(TextureRangeRect(0, 0.125*7, 0.25, 0.125*8), 2)
	
hina.position: addCascade(PositionPerGrid(0, 0.6, 0.7), PositionPerGrid(0, 0.0, 0.6), PositionPerGrid(0, 0.1, 1), 16)

-- コピーして加算合成
-- 残像を描画する
hina12 = AnimeChip_Perticle(hina);
hina12.color: clear()
hina12.color: addCascade(Color(140,60,0,60), 16)
hina12: setDelay(8)
hina13 = AnimeChip_Perticle(hina12);
hina13: setDelay(12)

hab_parallel: addAnimeChip(hina);
hab_parallel: addAnimeChip(hina12);
hab_parallel: addAnimeChip(hina13);
-- =============================================================


-- =============================================================
-- 煙
smoke = { }
effectlife = 20
count = 0
for k = 0, 8
do
	x_speed = (random()-0.5)*3

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

	smoke[k] = AnimeChip_Bound("effect\\basic\\ball_antismoke.png", D3DXVECTOR2(40,40), PositionPerGrid(x_speed, 0, 7+random()*3), effectlife)
	smoke[k].scaleX:	addCascade(1, 0.1, effectlife)
	smoke[k].scaleY:	addCascade(1, 0.1, effectlife)
	smoke[k].color: addCascade(Color(100,200,0,200), effectlife)
	smoke[k].gravity: addCascade(25, effectlife)
	smoke[k].position: addCascade(PositionPerGrid(0,0,0.1), effectlife)
	smoke[k].rotationBefore: addCascade(turn, 0, effectlife)
	smoke[k]: setDelay(14)
	
	hab_parallel: addAnimeChip(smoke[k]);
end
-- =============================================================


setAnimation_parallel(hab_parallel)
setAnimation_wait(hab)