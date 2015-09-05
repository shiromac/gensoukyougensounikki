-- ”’lŒ^ PlaceX : ã©‚ÌXˆÊ’uiƒ}ƒXj
-- ”’lŒ^ PlaceY : ã©‚ÌYˆÊ’uiƒ}ƒXj
-- ”’lŒ^ direction : ”íŠQÒ‚ÌŒü‚«i‚O`‚R‚U‚Oj”íŠQÒ‚ª‚¢‚È‚¢ê‡‚Í[‚R‚U‚O
-- pcCharacterŒ^ sufferer : ”íŠQÒi‚¢‚È‚¢ê‡‚à‚ ‚é‚ª‚¢‚È‚¢ê‡‚Å


effect_life = 50

fase1 = 0.5
fase2 = 0.2
fase3 = 0.3

hab = AnimeChip_Hab(PositionPerGrid(PlaceX, PlaceY, 0))


-- sound

sound = AnimeChip_Sound("trap9024.wav",PositionPerGrid(PlaceX,PlaceY,0),1.0)
hab:addAnimeChip(sound)
sound: setDelay(0)

chara = AnimeChip_Chara(sufferer, effect_life)
chara.pitching: addCascade(0, 360, effect_life* fase1)

chara.position: addCascade(PositionPerGrid(0,0,0), PositionPerGrid(0,0,3), PositionPerGrid(0,0,0), effect_life* fase1)
chara.position: addCascade(PositionPerGrid(0,0,0), PositionPerGrid(0,0,1), PositionPerGrid(0,0,0), effect_life* fase2)
hab:addAnimeChip(chara)

setAnimation_parallel(hab)
effect_WaitPararellCritical(charaPos(PlaceX), charaPos(PlaceY), effect_life*(fase1+fase2))