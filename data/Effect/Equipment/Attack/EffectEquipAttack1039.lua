--ミニ八卦炉

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
sound = AnimeChip_Sound("swing1039.wav",PositionPerGrid(toPlaceX,toPlaceY,0),1.0)
hab:addAnimeChip(sound)
sound: setDelay(fumikiri_life*0.5+1)

--
hab_parallel = AnimeChip_Hab(PositionPerGrid(toPlaceX, toPlaceY, 0))


-- =============================================================
-- 火
efLife = 32

texname = "effect\\fire\\firetex1mini.png"
r = random()
if r < 0.3
then
	texname = "effect\\fire\\firetex2mini.png"
elseif r < 0.6
then
	texname = "effect\\fire\\firetex3mini.png"
end





fire = AnimeChip_Perticle(texname, D3DXVECTOR2(256,256), efLife)
fire.color: addCascade(Color(100,20,20,20), Color(0,30,30,20), Color(0,30,30,20), efLife)
fire.drawMode: setDefault(DrawMode_normal)
fire:setWorldRotation(direction)
fire.scaleX:	addCascade(0.5+0.3*random(), 0.8+0.2*random(), efLife)
fire.scaleY:	addCascade(0.5+0.3*random(), 0.8+0.2*random(), efLife)
fire.rotationBefore: addCascade(360*random(), efLife)
fire.position: addCascade(PositionPerGrid(0, 0.6, 0.7), PositionPerGrid(0, -0.3, 0.6+ 0.2*random()), PositionPerGrid(0, -0.1, 1+ 0.2*random()), efLife)
fire: setDelay(2)

for i = 0, 4
do
	for j = 0, 4
	do
		fire.textureRange:  addCascade(TextureRangeRect(0.25*j, 0.25*i, 0.25*(j+1), 0.25*(i+1)), efLife/8)
	end
end
	

-- コピーして加算合成
-- 残像を描画する
fire12 = AnimeChip_Perticle(fire);
fire12.rotationBefore: clear()
fire12.rotationBefore: addCascade(360*random(), efLife)
fire12.color: clear()
fire12.color: addCascade(Color(200,255,90,60), Color(200,255,90,60), Color(0,180,90,60), efLife)
fire12.scaleX:clear()
fire12.scaleY:clear()
fire12.scaleX:	addCascade(0.5+0.3*random(), 0.8+0.2*random(), efLife)
fire12.scaleY:	addCascade(0.5+0.3*random(), 0.8+0.2*random(), efLife)
fire12.position:clear()
fire12.position: addCascade(PositionPerGrid(0, 0.0, 0.7), PositionPerGrid(0, -0.1 + 0.2*random(), 0.6), PositionPerGrid(0, 0.1 + 0.2*random(), 0.7), efLife)
fire12: setDelay(2)

fire13 = AnimeChip_Perticle(fire);
fire13.rotationBefore: clear()
fire13.rotationBefore: addCascade(360*random(), efLife)
fire13.color: clear()
fire13.color: addCascade(Color(250,255,130,20), Color(250,255,130,20), Color(0,255,130,20), efLife)
fire13.scaleX:clear()
fire13.scaleY:clear()
fire13.scaleX:	addCascade(0.5+0.3*random(), 0.8+0.2*random(), efLife)
fire13.scaleY:	addCascade(0.5+0.3*random(), 0.8+0.2*random(), efLife)
fire13.drawMode: setDefault(DrawMode_addition)
fire13: setDelay(3 - 3*random())

hab_parallel: addAnimeChip(fire);
hab_parallel: addAnimeChip(fire12);
hab_parallel: addAnimeChip(fire13);
-- =============================================================


setAnimation_parallel(hab_parallel)
setAnimation_wait(hab)