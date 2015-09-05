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

redius = 0.4
-- sound
sound = AnimeChip_Sound("confuse.wav",PositionPerGrid(PlaceX,PlaceY,0),1.0)

hab:addAnimeChip(sound)
sound: setDelay(effectlife*fase1)


sword = AnimeChip_Perticle("effect\\trap\\sword_icon.png",D3DXVECTOR2(52,52), effectlife)
sword.color: addCascade(color_white, color_white, color_white, color_white_trans, effectlife)
sword.drawMode: setDefault(DrawMode_normal)
sword.position: addCascade(PositionPerGrid(redius,0,0), effectlife*fase1)
for i = 0, effectlife*fase2-1, 1 do
    local position = D3DXVec3XYPlaneRotate(PositionPerGrid(redius,0,0), radianForDegree(i*360*1.5/(effectlife*fase2)))
    sword.position: addCascade(position, 1)
end
sword.position: addCascade(PositionPerGrid(-redius,0,0), effectlife*fase3)
hab: addAnimeChip(sword)

shield = AnimeChip_Perticle("effect\\trap\\shield_icon.png",D3DXVECTOR2(52,52), effectlife)
shield.drawMode: setDefault(DrawMode_normal)
shield.color: addCascade(color_white, color_white, color_white, color_white_trans, effectlife)
shield.position: addCascade(PositionPerGrid(-redius,0,0), effectlife*fase1)
for i = 0, effectlife*fase2-1, 1 do
    local position = D3DXVec3XYPlaneRotate(PositionPerGrid(-redius,0,0), radianForDegree(i*360*1.5/(effectlife*fase2)))
    shield.position: addCascade(position, 1)
end
shield.position: addCascade(PositionPerGrid(redius,0,0), effectlife*fase3)
hab: addAnimeChip(shield)

setAnimation_parallel(hab)
effect_WaitPararellCritical(charaPos(PlaceX), charaPos(PlaceY), effectlife/2)