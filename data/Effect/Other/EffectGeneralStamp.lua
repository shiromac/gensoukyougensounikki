--[[
	cValiableField val;
	double temp;
	val.doubles.dim(tstring(_T("placeX")),temp = placeX);
	val.doubles.dim(tstring(_T("placeY")),temp = placeY);

	val.tstrings.dim(tstring(_T("fileName")),fileName);

	val.doubles.dim(tstring(_T("burnColor_D3Dcolor")),temp = burnColor.D3Dcolor());

	val.doubles.dim(tstring(_T("texRengeX1")),temp = texRenge.x);
	val.doubles.dim(tstring(_T("texRengeY1")),temp = texRenge.y);
	val.doubles.dim(tstring(_T("texRengeX2")),temp = texRenge.z);
	val.doubles.dim(tstring(_T("texRengeY2")),temp = texRenge.w);

	val.doubles.dim(tstring(_T("size")),temp = size);

	val.doubles.dim(tstring(_T("duration")),temp = duration);
	
	val.doubles.dim(tstring(_T("delay")),temp = delay);
]]
cascade1 = 0.1
cascade2 = 0.5
cascade2_1 = 0.25
cascade2_2 = 0.25
cascade3 = 0.4
burnColor = Color(burnColor_D3Dcolor)
burnColor_trans = Color(burnColor)
burnColor_trans.alpha = 0
textureRangeRect = TextureRangeRect(texRengeX1, texRengeY1, texRengeX2, texRengeY2)


hab_parallel = AnimeChip_Hab(PositionPerGrid(placeX, placeY, 0))

mainPerticle = AnimeChip_Perticle(string(fileName), D3DXVECTOR2(size,size), duration)
mainPerticle: setDelay(delay)
subPerticle = AnimeChip_Perticle(mainPerticle)

mainPerticle.color: addCascade(burnColor_trans, burnColor, duration*cascade1)
mainPerticle.color: addCascade(burnColor, duration*cascade2)
mainPerticle.color: addCascade(burnColor, burnColor_trans, duration*cascade3)

mainPerticle.scaleX: addCascade(3, 3, 1, duration*cascade1)
mainPerticle.scaleX: addCascade(1, duration*cascade2)
mainPerticle.scaleX: addCascade(1, duration*cascade3)

mainPerticle.scaleY = mainPerticle.scaleX

mainPerticle.drawMode: setDefault(DrawMode_normal)

subPerticle.color: addCascade(burnColor_trans, duration*cascade1)
subPerticle.color: addCascade(burnColor, burnColor_trans, duration*cascade2_1)

subPerticle.scaleX: addCascade(1, duration*cascade1)
subPerticle.scaleX: addCascade(1, 3, 3, duration*cascade2_1)

subPerticle.scaleY = subPerticle.scaleX

subPerticle.drawMode: setDefault(DrawMode_addition)
--[[
	smoke[k].scaleX: addCascade(0.2, 2, 1, 2, effectlife/2)
	smoke[k].scaleY: addCascade(0.2, 2, 1, 2, effectlife/2)
	smoke[k].rotationAfter: addCascade(random()*360, random()*360, effectlife/2)
	smoke[k]: setDelay(16*(0.5+0.03*k))

	pos = PositionPerGrid((1+k/smoke_num)*random()*0.2,0,0.8)
	
	smoke[k].position: addCascade(pos ,effectlife)
	]]
hab_parallel: addAnimeChip(mainPerticle)
hab_parallel: addAnimeChip(subPerticle)
	
setAnimation_parallel(hab_parallel)
