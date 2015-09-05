-- ”’lŒ^ PlaceX : ã©‚ÌXˆÊ’uiƒ}ƒXj
-- ”’lŒ^ PlaceY : ã©‚ÌYˆÊ’uiƒ}ƒXj
-- ”’lŒ^ direction : ”íŠQÒ‚ÌŒü‚«i‚O`‚R‚U‚Oj”íŠQÒ‚ª‚¢‚È‚¢ê‡‚Í[‚R‚U‚O
-- pcCharacterŒ^ sufferer : ”íŠQÒi‚¢‚È‚¢ê‡‚à‚ ‚é‚ª‚¢‚È‚¢ê‡‚Å


hab = AnimeChip_Hab(PositionPerGrid(PlaceX, PlaceY, 0))

-- sound
sound = AnimeChip_Sound("trap9010.wav",PositionPerGrid(PlaceX,PlaceY,0),1.0)
hab:addAnimeChip(sound)
sound: setDelay(0)

effectlife = 80
smoke = {}
smoke_num = 10
for k = 0, smoke_num
do
	smoke[k] = AnimeChip_Perticle("effect\\basic\\ball_smoke.png",D3DXVECTOR2(60,60), effectlife/2)
	smoke[k].color: addCascade(Color(0,200,200,255),Color(255*2,255,255,255),Color(0,200,200,255), effectlife/2)
	smoke[k].drawMode: setDefault(DrawMode_normal)

	smoke[k].scaleX: addCascade(0.2, 2, 1, 2, effectlife/2)
	smoke[k].scaleY: addCascade(0.2, 2, 1, 2, effectlife/2)
	smoke[k].rotationAfter: addCascade(random()*360, random()*360, effectlife/2)
	smoke[k]: setDelay(16*(0.5+0.03*k))

	pos = PositionPerGrid((1+k/smoke_num)*random()*0.2,0,0.8)
	turn_matrix = D3DXMATRIX()
	D3DXMatrixRotationZ(turn_matrix, (random()*MATH_PI*2))
	D3DXVec3TransformCoord(pos,pos,turn_matrix)
	
	smoke[k].position: addCascade(pos ,effectlife)
	hab: addAnimeChip(smoke[k])
end

setAnimation_wait(hab)