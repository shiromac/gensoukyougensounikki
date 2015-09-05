--rethi

-- êîílå^ fromPlaceX : çUåÇé“ÇÃXà íuÅiÉ}ÉXÅj
-- êîílå^ fromPlaceY : çUåÇé“ÇÃYà íuÅiÉ}ÉXÅj
-- êîílå^ toPlaceX :
-- êîílå^ toPlaceY :
-- êîílå^ angle :
-- êîílå^ distanceGrid
-- êîílå^ direction : çUåÇé“ÇÃå¸Ç´ÅiÇOÅ`ÇRÇUÇOÅj
-- pcCharacterå^ attacker : çUåÇé“ÇÃÉLÉÉÉâÉNÉ^Å[

hab = AnimeChip_Hab(PositionPerGrid(toPlaceX, toPlaceY, 0))
distance = sqrt((fromPlaceX-toPlaceX)*(fromPlaceX-toPlaceX) + (fromPlaceY-toPlaceY)*(fromPlaceY-toPlaceY))

baseColor = Color(512,128,255,255)
baseColor_trans = Color(0,0,128,255)
life_snow = 30
life_snow_delay = 30
life_ice = 30
life_ice_delay = 20
life = 30
-- fumikiri
fumikiri_life = life
fumikomi_distance = 1

chara_anime = AnimeChip_Chara(attacker,fumikiri_life)
hab:addAnimeChip(chara_anime)
chara_anime.position: addCascade(PositionPerGrid(0, 0, 0), PositionPerGrid(0, 0, 0), PositionPerGrid(0, 0, 1), PositionPerGrid(0, 0, 0), fumikiri_life)
--
--[[
-- sound
sound = AnimeChip_Sound("attack_swing1.wav",PositionPerGrid(toPlaceX,toPlaceY,0),1.0)
hab:addAnimeChip(sound)
sound: setDelay(2)
]]
--
hab_parallel = AnimeChip_Hab(PositionPerGrid(toPlaceX, toPlaceY, 0))


--yuki
textureWidth = 0.25
textureHeight = 0.25
textureIndexY = 0
for i = 1, 8 do
    textureIndexX = math.floor(random()*3)
    snow = AnimeChip_Perticle("effect\\ice.png",D3DXVECTOR2(24,24),life_snow)
    snow.textureRange: setDefault(TextureRangeRect(textureIndexX * textureWidth, textureIndexY * textureHeight, (textureIndexX+1) * textureWidth, (textureIndexY+1) * textureHeight))
    rotate = random()*360
    snow.rotationAfter: addCascade(rotate, rotate + 60, life_snow)
    snow.color: addCascade(baseColor_trans, baseColor, baseColor, baseColor_trans, life_snow)
    snow.drawMode: setDefault(DrawMode_addition)
    snow.scaleX: addCascade(1, 1.5, life_snow)
    snow.scaleY: addCascade(1, 1.5, life_snow)
    snow: setDelay(life_snow_delay*random())
    deferent_end_X = (0.5- random())*0.6
    deferent_end_Y = (0.5- random())*0.6
    deferent_start_X = deferent_end_X + (0.5- random())*0.3
    deferent_start_Y = deferent_end_Y + (0.5- random())*0.3
    snow.position: addCascade(PositionPerGrid(deferent_start_X,deferent_start_Y,0), PositionPerGrid(deferent_start_X,deferent_start_Y,0), PositionPerGrid(deferent_end_X,deferent_end_Y,3), life_snow)
    hab_parallel:addAnimeChip(snow)
end

--hyou
textureWidth = 0.25
textureHeight = 0.25
textureIndexY = 1
for i = 1, 10 do
    textureIndexX = math.floor(random()*3)
    ice = AnimeChip_Perticle("effect\\ice.png",D3DXVECTOR2(16,16),life_ice)
    ice.textureRange: setDefault(TextureRangeRect(textureIndexX * textureWidth, textureIndexY * textureHeight, (textureIndexX+1) * textureWidth, (textureIndexY+1) * textureHeight))
    ice.rotationAfter: setDefault(random()*360)
    ice.color: addCascade(baseColor_trans, baseColor, baseColor, baseColor_trans, life_ice)
    ice.drawMode: setDefault(DrawMode_addition)
    ice.scaleX: addCascade(1, 2, life_snow)
    ice.scaleY: addCascade(1, 2, life_snow)
    ice: setDelay(life_ice_delay*random())
    deferent_end_X = (0.5- random())*0.6
    deferent_end_Y = (0.5- random())*0.6
    deferent_start_X = deferent_end_X + (0.5- random())*5
    deferent_start_Y = deferent_end_Y + (0.5- random())*5
    ice.position: addCascade(PositionPerGrid(deferent_start_X,deferent_start_Y,6), PositionPerGrid(deferent_end_X,deferent_end_Y,0), PositionPerGrid(deferent_end_X,deferent_end_Y,0), life_ice)
    hab_parallel:addAnimeChip(ice)
end

setAnimation_parallel(hab_parallel)
setAnimation_wait(hab)
