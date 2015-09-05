-- ”’lŒ^ PlaceX : ã©‚ÌXˆÊ’uiƒ}ƒXj
-- ”’lŒ^ PlaceY : ã©‚ÌYˆÊ’uiƒ}ƒXj
-- ”’lŒ^ direction : ”íŠQÒ‚ÌŒü‚«i‚O`‚R‚U‚Oj”íŠQÒ‚ª‚¢‚È‚¢ê‡‚Í[‚R‚U‚O
-- pcCharacterŒ^ sufferer : ”íŠQÒi‚¢‚È‚¢ê‡‚à‚ ‚é‚ª‚¢‚È‚¢ê‡‚Å


color_white = Color(255,255,255,255)
color_white_trans = Color(0,255,255,255)

hab = AnimeChip_Hab(PositionPerGrid(PlaceX, PlaceY, 0))

effectlife = 30

-- sound
sound = AnimeChip_Sound("confuse.wav",PositionPerGrid(PlaceX,PlaceY,0),1.0)

hab:addAnimeChip(sound)
sound: setDelay(effectlife*fase1)


ring = AnimeChip_Perticle("effect\\basic\\ring_out.png",D3DXVECTOR2(64, 64), effectlife)
ring.scaleX: addCascade(1,1,1,10, effectlife)
ring.scaleY = ring.scaleX
ring.color: addCascade(color_white_trans, color_white, color_white_trans, effectlife)
ring.drawMode: setDefault(DrawMode_substraction)
hab: addAnimeChip(ring)



setAnimation_parallel(hab)
effect_WaitPararellCritical(charaPos(PlaceX), charaPos(PlaceY), effectlife/2)