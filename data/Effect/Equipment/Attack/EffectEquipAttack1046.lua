-- 1046 イカリ

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
sound = AnimeChip_Sound("swing1046.wav",PositionPerGrid(toPlaceX,toPlaceY,0),1.0)
hab:addAnimeChip(sound)
sound: setDelay(8)

--
hab_parallel = AnimeChip_Hab(PositionPerGrid(toPlaceX, toPlaceY, 0))

effectfLife = 16
-- =============================================================
-- イカリ
ikari11 = AnimeChip_Perticle("effect\\equip\\ikari.png",D3DXVECTOR2(70,60), effectfLife)
ikari11.color: addCascade(Color(170,0,0,0), effectfLife)
ikari11.drawMode: setDefault(DrawMode_normal)
ikari11.rotationBefore: addCascade(direction, effectfLife)
--ikari11.scaleX:	addCascade(1, 1, 1, 0.1, effectfLife)
--ikari11.scaleY:	addCascade(1, 1, 1, 0.1, effectfLife)
ikari11:setWorldRotation(direction)
ikari11: setDelay(2)
--ari11.position: addCascade( PositionPerGrid(0, 0, 0.5), effectfLife)
ikari11.position: addCascade(PositionPerGrid(0, 0.6, 0.6), PositionPerGrid(0, -0.2, 0.6), PositionPerGrid(0, -0.2, 0.6), PositionPerGrid(0, -0.2, 0.6), 14)

-- コピーして加算合成
--effectfLife = effectfLife+6
ikari12 = AnimeChip_Perticle(ikari11)
ikari12.color: clear()
ikari12.color: addCascade(Color(130,130,130,130), effectfLife)
ikari12.drawMode: setDefault(DrawMode_addition)
ikari12: setDelay(3)
ikari12: setLife(effectfLife+6)
ikari12.scaleX:	addCascade(1, effectfLife)
ikari12.scaleY:	addCascade(1, effectfLife)
ikari12.scaleX:	addCascade(1, 0.1, 6)
ikari12.scaleY:	addCascade(1, 0.1, 6)
--ikari12.color: clear()
ikari12.position: addCascade(PositionPerGrid(0, 0, 0.6), PositionPerGrid(0, 1.2, 0.6), 6)

hab_parallel: addAnimeChip(ikari11);
hab_parallel: addAnimeChip(ikari12);
-- =============================================================

-- =============================================================
-- 水しぶき
--[[
mizu = { }
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

	mizu[k] = AnimeChip_Bound("effect\\equip\\particle_mizu.png", D3DXVECTOR2(32,32), PositionPerGrid(x_speed, 0, 7+random()*3), effectlife)
	mizu[k].scaleX:	addCascade(1, 0, effectlife)
	mizu[k].scaleY:	addCascade(1, 0, effectlife)
	mizu[k].color: addCascade(Color(210,240,240,250), effectlife)
	mizu[k].gravity: addCascade(15, effectlife)
	mizu[k].position: addCascade(PositionPerGrid(-0.3+(0.6*random()), -0.2+ (0.6*random()), 0.3), effectlife)
	mizu[k]:setWorldRotation(direction)
	mizu[k].rotationBefore: addCascade(0, random()*360, effectlife)
	mizu[k]: setDelay(10)
	
	hab_parallel: addAnimeChip(mizu[k]);
end
--]]
-- =============================================================



setAnimation_parallel(hab_parallel)
setAnimation_wait(hab)