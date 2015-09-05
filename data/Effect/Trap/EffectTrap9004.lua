-- ”’lŒ^ PlaceX : ã©‚ÌXˆÊ’uiƒ}ƒXj
-- ”’lŒ^ PlaceY : ã©‚ÌYˆÊ’uiƒ}ƒXj
-- ”’lŒ^ direction : ”íŠQÒ‚ÌŒü‚«i‚O`‚R‚U‚Oj”íŠQÒ‚ª‚¢‚È‚¢ê‡‚Í[‚R‚U‚O
-- pcCharacterŒ^ sufferer : ”íŠQÒi‚¢‚È‚¢ê‡‚à‚ ‚é‚ª‚¢‚È‚¢ê‡‚Å


bound_life = 50
hab = AnimeChip_Hab(PositionPerGrid(PlaceX, PlaceY, 0))


-- sound

sound = AnimeChip_Sound("trap9004.wav",PositionPerGrid(PlaceX,PlaceY,0),1.0)
hab:addAnimeChip(sound)
sound: setDelay(0)

bound_num = 36
for k = 0, bound_num do

	bound_anime = AnimeChip_Bound("effect\\equip\\particle_sakura.png", D3DXVECTOR2(32,32), PositionPerGrid(0,0,0) ,bound_life)

	bound_anime: setWorldRotation(random()*120)
	turn_matrix = D3DXMATRIX()
	pos = PositionPerGrid(0,0,3)

	D3DXMatrixRotationY(turn_matrix, (-(k/bound_num)*60 + 30)*MATH_PI/180 )
	D3DXVec3TransformCoord(pos,pos,turn_matrix)
	
	pos.z = pos.z*2
	bound_anime: setStartVector(pos)
	bound_anime.color: addCascade(Color(250,255,250,250), Color(0,255,255,255), bound_life)
	bound_anime.scaleX:	addCascade(1, 0.3, bound_life)
	bound_anime.scaleY:	addCascade(1, 0.3, bound_life)
	bound_anime.gravity: setDefault(6)
	bound_anime.rotationBefore: addCascade(360*random(), -360*random(), bound_life)

	hab: addAnimeChip(bound_anime)
end

--setAnimation_parallel(hab)
setAnimation_wait(hab)