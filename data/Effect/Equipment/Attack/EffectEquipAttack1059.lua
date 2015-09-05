--1059 柄杓

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
sound = AnimeChip_Sound("swing1059.wav",PositionPerGrid(toPlaceX,toPlaceY,0),1.0)
hab:addAnimeChip(sound)
sound: setDelay(fumikiri_life*0.5+1)

--
hab_parallel = AnimeChip_Hab(PositionPerGrid(toPlaceX, toPlaceY, 0))

-- =============================================================
-- 水しぶき
mizu = { }
effectlife = 30
for k = 0, 16
do
	x_speed = (random()-0.5)*3

	mizu[k] = AnimeChip_Bound("effect\\equip\\particle_mizu.png", D3DXVECTOR2(24,24), PositionPerGrid(x_speed, 0, 7+random()*3), effectlife)
	mizu[k].scaleX:	addCascade(1, 0, effectlife)
	mizu[k].scaleY:	addCascade(1, 0, effectlife)
	mizu[k].color: addCascade(Color(180,255,255,255), effectlife)
	mizu[k].gravity: addCascade(35, effectlife)
	mizu[k].position: addCascade(PositionPerGrid(-0.3+(0.6*random()), -0.2+ (0.6*random()), 0.3), effectlife)
	mizu[k]:setWorldRotation(direction)
	mizu[k].rotationBefore: addCascade(random()*180, random()*180, effectlife)
	mizu[k]: setDelay(6)
	
	if( random() < 0.3 )
	then
		mizu[k].color: clear()
		mizu[k].color: addCascade(Color(140,130,130,130), effectlife)
		mizu[k].drawMode: setDefault(DrawMode_addition)	
	end
	
	hab_parallel: addAnimeChip(mizu[k]);
end
-- =============================================================

setAnimation_parallel(hab_parallel)
setAnimation_wait(hab)