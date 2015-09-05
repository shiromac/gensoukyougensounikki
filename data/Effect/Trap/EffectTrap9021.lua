-- ”’lŒ^ PlaceX : ã©‚ÌXˆÊ’uiƒ}ƒXj
-- ”’lŒ^ PlaceY : ã©‚ÌYˆÊ’uiƒ}ƒXj
-- ”’lŒ^ direction : ”íŠQÒ‚ÌŒü‚«i‚O`‚R‚U‚Oj”íŠQÒ‚ª‚¢‚È‚¢ê‡‚Í[‚R‚U‚O
-- pcCharacterŒ^ sufferer : ”íŠQÒi‚¢‚È‚¢ê‡‚à‚ ‚é‚ª‚¢‚È‚¢ê‡‚Å

color_white = Color(255*2,128,128,128)
color_white_trans = Color(0,128,128,128)


hab = AnimeChip_Hab(PositionPerGrid(PlaceX, PlaceY, 0))

-- sound
sound = AnimeChip_Sound("fission.wav",PositionPerGrid(PlaceX,PlaceY,0),1.0)
sound: setDelay(0)
hab:addAnimeChip(sound)

effectlife = 80

for i = 1, 16 do
    speed = random()*20
    local position = D3DXVec3XYPlaneRotate(PositionPerGrid(speed,0,20+random()*12), radianForDegree(360*random()))

    trap = AnimeChip_Bound("effect\\trap\\trap_model.png",D3DXVECTOR2(52,52), position, effectlife)
    trap.gravity: setDefault(9.8*7)
    trap.floorHeight: setDefault(0)
    trap: setDelay(i*2)
    local x = math.floor(random()*4)
    local y = math.floor(random()*4)
    trap.textureRange: setDefault(TextureRangeRect(x/4,y/4,(x+1)/4,(y+1)/4))
    trap.color: addCascade(color_white, color_white, color_white, color_white_trans, effectlife)
    hab: addAnimeChip(trap)
end


setAnimation_parallel(hab)
effect_WaitPararellCritical(charaPos(PlaceX), charaPos(PlaceY), effectlife/2)