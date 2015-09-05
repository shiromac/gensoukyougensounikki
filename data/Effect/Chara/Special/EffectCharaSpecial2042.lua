--onryou

-- 数値型 fromPlaceX : 攻撃者のX位置（マス）
-- 数値型 fromPlaceY : 攻撃者のY位置（マス）
-- 数値型 toPlaceX : 攻撃先のX位置（マス）
-- 数値型 toPlaceY : 攻撃先のY位置（マス）
-- 数値型 direction : 攻撃者の向き（０～３６０）
-- pcCharacter型 attacker : 攻撃者のキャラクター

lightColor = Color();
lightColor: HSV(120*random()*120,1,1)
lightTrasrate = Color(lightColor);
lightTrasrate.alpha = 0;
darkColor = Color();
darkColor: HSV(random()*360,1,0.4)

hab = AnimeChip_Hab(PositionPerGrid(toPlaceX, toPlaceY, 0))
distance = sqrt((fromPlaceX-toPlaceX)*(fromPlaceX-toPlaceX) + (fromPlaceY-toPlaceY)*(fromPlaceY-toPlaceY))

life = 16*sqrt(distance)
-- fumikiri
fumikiri_life = life
fumikomi_distance = 1

chara_anime = AnimeChip_Chara(attacker,fumikiri_life)
hab:addAnimeChip(chara_anime)
chara_anime: setWorldRotation(direction)
chara_anime.position: addCascade(PositionPerGrid(0, 0, 0), PositionPerGrid(0, 0, 0.3), PositionPerGrid(0, 0, 1), PositionPerGrid(0, 0, 0), fumikiri_life)
--
--[[
-- sound
sound = AnimeChip_Sound("bom.wav",PositionPerGrid(toPlaceX,toPlaceY,0),1.0)
hab:addAnimeChip(sound)
sound: setDelay(fumikiri_life-1)
--]]
--
hab_parallel = AnimeChip_Hab(PositionPerGrid(toPlaceX, toPlaceY, 0))


life_ball = life
life_belt = 16

difX = fromPlaceX - toPlaceX;
difY = fromPlaceY - toPlaceY;

belt_point_num = 5
belt_num = 5
--belt

for i = 0, belt_num do
    
    rotation_start = random()*360;
    lightColor = Color();
    lightColor: HSV(120*random()+180,0.9,0.9)

    aniBelt11 = AnimeChip_Belt("effect\\basic\\ball_smoke.png",PositionPerGrid(0, 0, 0.9),PositionPerGrid(0, 0, 0.5),life_belt)
    aniBelt11:setDelay(random()*8)
    aniBelt11.drawRange: addCascade(D3DXVECTOR2(1,1),D3DXVECTOR2(1,belt_point_num-1),D3DXVECTOR2(1,belt_point_num-1),D3DXVECTOR2(belt_point_num-1,belt_point_num-1),life_belt)
    aniBelt11.color: addCascade(lightColor, life_belt)
    aniBelt11.position: addCascade(PositionPerGrid(0, 0, 0), life_belt)
    --aniBelt11:setWorldRotation(direction)
    aniBelt11.drawMode: setDefault(DrawMode_normal)
    aniBelt11.textureRange: setDefault(TextureRangeRect(1,1,0,0))

    for k = 0, belt_point_num do
        turn_matrix = D3DXMATRIX()
        pos = PositionPerGrid(0.5, 0, 0)	-- ベースの位置ベクトル(先端位置)
        rotAxis = PositionPerGrid(0, 0, 1.0)	-- 回転軸
        
        angle =  ( (k/belt_point_num)*320 + rotation_start ) * -MATH_PI/180	-- 回転角度
        D3DXVec3Rotate( pos, rotAxis, angle, pos )			-- 位置ベクトルを回転
        
        process_value = (k/belt_point_num)
        
        pos.x = pos.x + difX *(1-process_value) + (random()*2 -1)*(1-process_value)*process_value*2
        pos.y = pos.y + difY *(1-process_value) + (random()*2 -1)*(1-process_value)*process_value*2
        
        aniBelt11.rightEdge: push_back(VariationValue_PositionPerGrid(pos))	-- 手前
        aniBelt11.leftEdge: push_back(VariationValue_PositionPerGrid(pos))	-- 奥
    end
    
    hab_parallel: addAnimeChip(aniBelt11)

end

setAnimation_parallel(hab_parallel)
setAnimation_wait(hab)
