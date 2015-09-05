--[[
	val.doubles.dim(tstring(_T("PlaceX")),temp = pchara->placeX);
	val.doubles.dim(tstring(_T("PlaceY")),temp = pchara->placeY);
	val.doubles.dim(tstring(_T("time")),temp = time);
]]
imageColor1_color = Color(255,100,100,255)
life = time

hab = AnimeChip_Hab(PositionPerGrid(PlaceX, PlaceY, 0))

ring_perticle_anime = AnimeChip_Perticle("effect\\magiccircle.png",D3DXVECTOR2(512,512),life)
hab: addAnimeChip(ring_perticle_anime)
ring_perticle_anime.color: addCascade(imageColor1_color,life)
ring_perticle_anime.scaleX:addCascade(1,0,life)
ring_perticle_anime.scaleY:addCascade(1,0,life)
ring_perticle_anime.rotationBefore:addCascade(0,360,life)
ring_perticle_anime.drawMode: setDefault(DrawMode_addition)
ring_perticle_anime.position: setDefault(PositionPerGrid(0,0,0))

setAnimation_parallel(hab)
--
