--[[
	cValiableField val;
	double temp;
	val.doubles.dim(tstring(_T("PlaceX")),temp = pchara->placeX);
	val.doubles.dim(tstring(_T("PlaceY")),temp = pchara->placeY);
	cCoordinate coo;
	coo.SetAspect(aspect);
	val.doubles.dim(tstring(_T("toPlaceX")),temp = pchara->placeX + coo.x);
	val.doubles.dim(tstring(_T("toPlaceY")),temp = pchara->placeY + coo.y);
	val.doubles.dim(tstring(_T("direction")),temp = pchara->aspect*45);
	val.doubles.dim(tstring(_T("aspect")),temp = (double)aspect);
	val.charas.dim(tstring(_T("player")),pchara);
]]

hab_parallel = AnimeChip_Hab(PositionPerGrid(PlaceX, PlaceY, 0))

-- fumikiri
fumikiri_life = 12
fumikomi_distance = sqrt((PlaceX - toPlaceX)*(PlaceX - toPlaceX) + (PlaceY - toPlaceY)*(PlaceY - toPlaceY)) 

chara_anime = AnimeChip_Chara(player,fumikiri_life)
hab_parallel:addAnimeChip(chara_anime)
chara_anime: setWorldRotation(direction)
chara_anime.position: addCascade(PositionPerGrid(0, 0, 0), PositionPerGrid(0, -0.2*fumikomi_distance, 0), PositionPerGrid(0, -1*fumikomi_distance, 0), fumikiri_life)
--
--[[
effectlife = 10
ring = AnimeChip_Perticle("effect\\basic\\ring_out.png",D3DXVECTOR2(60,60), effectlife)
ring.color: addCascade(Color(0,200,200,200),Color(200,255,255,255),Color(0,200,200,200), effectlife)
ring.drawMode: setDefault(DrawMode_normal)

ring.scaleX: addCascade(1, 4, 4, effectlife)
ring.scaleY: addCascade(1, 4, 4, effectlife)
ring.rotationAfter: addCascade(random()*360, effectlife)
ring: setDelay(fumikiri_life)

hab_parallel:addAnimeChip(ring)
]]
--

setAnimation_parallel(hab_parallel)
