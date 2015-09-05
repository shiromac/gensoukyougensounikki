-- ”’lŒ^ PlaceX : ã©‚ÌXˆÊ’uiƒ}ƒXj
-- ”’lŒ^ PlaceY : ã©‚ÌYˆÊ’uiƒ}ƒXj
-- ”’lŒ^ direction : ”íŠQÒ‚ÌŒü‚«i‚O`‚R‚U‚Oj”íŠQÒ‚ª‚¢‚È‚¢ê‡‚Í[‚R‚U‚O
-- pcCharacterŒ^ sufferer : ”íŠQÒi‚¢‚È‚¢ê‡‚à‚ ‚é‚ª‚¢‚È‚¢ê‡‚Å


bound_life = 50
hab = AnimeChip_Hab(PositionPerGrid(PlaceX, PlaceY, 0))

-- sound
sound = AnimeChip_Sound("trap9027.wav",PositionPerGrid(PlaceX,PlaceY,0),1.0)
hab:addAnimeChip(sound)
sound: setDelay(0)

-- wait—p
--bound_anime = AnimeChip_Bound("effect\\equip\\particle_dero.png", D3DXVECTOR2(0,0), PositionPerGrid(0,0,0) ,bound_life)
--bound_anime.color: addCascade(Color(0,10,130,10), bound_life)
--hab: addAnimeChip(bound_anime)
--effect_generalStamp(placeX_, placeY_, fileName_, texRange_, color_, 100, 100)

texRange_ = TextureRangeRect(0,0,1,1)
color_ = Color(255,255,255,100)
color2_ = Color(50,255,255,100)
placeX_ = charaPos(PlaceX)
placeY_ = charaPos(PlaceY) - 0.5
effect_generalFog(placeX_, placeY_, _T("effect\\basic\\ball_smoke.png"), texRange_, color2_, 64, 3, 0.15, 60, 0)
effect_generalStamp(placeX_, placeY_, _T("effect\\trap\\bongo_a.png"), texRange_, color_, 160, 100, 0)
effect_WaitPararellCritical(placeX_, placeY_, 30)


setAnimation_wait(hab)