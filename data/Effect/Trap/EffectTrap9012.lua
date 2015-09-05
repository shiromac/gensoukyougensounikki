-- ”’lŒ^ PlaceX : ã©‚ÌXˆÊ’uiƒ}ƒXj
-- ”’lŒ^ PlaceY : ã©‚ÌYˆÊ’uiƒ}ƒXj
-- ”’lŒ^ direction : ”íŠQÒ‚ÌŒü‚«i‚O`‚R‚U‚Oj”íŠQÒ‚ª‚¢‚È‚¢ê‡‚Í[‚R‚U‚O
-- pcCharacterŒ^ sufferer : ”íŠQÒi‚¢‚È‚¢ê‡‚à‚ ‚é‚ª‚¢‚È‚¢ê‡‚Å

color_white = Color(255*2,255,255,255)
color_white_trans = Color(0,255,255,255)

hab = AnimeChip_Hab(PositionPerGrid(PlaceX, PlaceY, 2))

effectlife = 60

fase1 = 0.2
fase2 = 0.5
fase3 = 0.3

speed = 0.8
-- sound
sound = AnimeChip_Sound("confuse.wav",PositionPerGrid(PlaceX,PlaceY,0),1.0)

hab:addAnimeChip(sound)
sound: setDelay(effectlife*fase1)


sword = AnimeChip_Bound("effect\\trap\\sword_icon.png",D3DXVECTOR2(52,52), PositionPerGrid(speed,0,5), effectlife)
sword.rotationBefore: addCascade(0, 90, effectlife)
sword.gravity: setDefault(9.8*5)
sword.floorHeight: setDefault(-1.5)
sword.color: addCascade(color_white, color_white, color_white, color_white_trans, effectlife)
hab: addAnimeChip(sword)

shield = AnimeChip_Bound("effect\\trap\\shield_icon.png",D3DXVECTOR2(52,52), PositionPerGrid(-speed,0,5), effectlife)
shield.rotationBefore: addCascade(0, -90, effectlife)
shield.gravity: setDefault(9.8*5)
shield.floorHeight: setDefault(-1.5)
shield.color: addCascade(color_white, color_white, color_white, color_white_trans, effectlife)
hab: addAnimeChip(shield)

setAnimation_parallel(hab)
effect_WaitPararellCritical(charaPos(PlaceX), charaPos(PlaceY), effectlife/2)