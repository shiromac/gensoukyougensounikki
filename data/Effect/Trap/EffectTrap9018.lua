-- ”’lŒ^ PlaceX : ã©‚ÌXˆÊ’uiƒ}ƒXj
-- ”’lŒ^ PlaceY : ã©‚ÌYˆÊ’uiƒ}ƒXj
-- ”’lŒ^ direction : ”íŠQÒ‚ÌŒü‚«i‚O`‚R‚U‚Oj”íŠQÒ‚ª‚¢‚È‚¢ê‡‚Í[‚R‚U‚O
-- pcCharacterŒ^ sufferer : ”íŠQÒi‚¢‚È‚¢ê‡‚à‚ ‚é‚ª‚¢‚È‚¢ê‡‚Å

color_white = Color(255,255,255,255)
color_white_trans = Color(0,255,255,255)

particle_life = 50
effect_life = 60
hab = AnimeChip_Hab(PositionPerGrid(PlaceX, PlaceY, 1))

-- sound
sound = AnimeChip_Sound("slowdown.wav",PositionPerGrid(PlaceX,PlaceY,0),1.0)
hab:addAnimeChip(sound)
sound: setDelay(5)

-- wait—p
--bound_anime = AnimeChip_Bound("effect\\equip\\particle_dero.png", D3DXVECTOR2(0,0), PositionPerGrid(0,0,0) ,bound_life)
--bound_anime.color: addCascade(Color(0,10,130,10), bound_life)
--hab: addAnimeChip(bound_anime)
--effect_generalStamp(placeX_, placeY_, fileName_, texRange_, color_, 100, 100)

yen = AnimeChip_Perticle("effect\\trap\\yen.png",D3DXVECTOR2(64,64), particle_life)
yen.color: addCascade(color_white_trans, color_white, color_white, color_white_trans, particle_life)
yen.drawMode: setDefault(DrawMode_normal)
yen.position: addCascade(PositionPerGrid(0,0,0), PositionPerGrid(0,0,5), particle_life)
yen.scaleX: addCascade(1, 1, 3, particle_life)
yen.scaleY: addCascade(1, 1, 3, particle_life)
hab: addAnimeChip(yen)

setAnimation_wait(hab)
effect_WaitPararellCritical(charaPos(PlaceX), charaPos(PlaceY), effect_life/2)