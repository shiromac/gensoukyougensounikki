--toramaru

-- ”’lŒ^ fromPlaceX : UŒ‚Ò‚ÌXˆÊ’uiƒ}ƒXj
-- ”’lŒ^ fromPlaceY : UŒ‚Ò‚ÌYˆÊ’uiƒ}ƒXj
-- ”’lŒ^ toPlaceX :
-- ”’lŒ^ toPlaceY :
-- ”’lŒ^ angle :
-- ”’lŒ^ distanceGrid
-- ”’lŒ^ direction : UŒ‚Ò‚ÌŒü‚«i‚O`‚R‚U‚Oj
-- pcCharacterŒ^ attacker : UŒ‚Ò‚ÌƒLƒƒƒ‰ƒNƒ^[

hab = AnimeChip_Hab(PositionPerGrid(toPlaceX, toPlaceY, 0))
distance = sqrt((fromPlaceX-toPlaceX)*(fromPlaceX-toPlaceX) + (fromPlaceY-toPlaceY)*(fromPlaceY-toPlaceY))

life_shutter = 3*distance+3

life = life_shutter
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


--zanngeki
shutter = AnimeChip_Perticle("effect\\mob\\shutter.png",D3DXVECTOR2(92,92),life_shutter)
shutter.rotationAfter: setDefault(angle+90)
shutter.color: addCascade(Color(0,255,255,255), Color(512,255,255,255), Color(512,255,255,255), Color(0,255,255,255), life_shutter)
shutter.position: addCascade(PositionPerGrid(0,0,0.8), life_shutter)

hab_parallel:addAnimeChip(shutter)

shutter2 = AnimeChip_Perticle("effect\\mob\\shutter.png",D3DXVECTOR2(92,92),life_shutter)
shutter2.rotationAfter: setDefault(angle+90)
shutter2.color: addCascade(Color(0,255,255,255), Color(0,255,255,255), Color(512,255,255,255), Color(0,255,255,255), life_shutter)
shutter2.drawMode: setDefault(DrawMode_addition)
shutter2.colorBlendMode: setDefault(ColorBlendMode_fill)
shutter2.position: addCascade(PositionPerGrid(0,0,0.8), life_shutter)

hab_parallel:addAnimeChip(shutter2)

setAnimation_parallel(hab_parallel)
setAnimation_wait(hab)
