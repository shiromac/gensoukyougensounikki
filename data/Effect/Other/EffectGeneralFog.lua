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
	
	val.doubles.dim(tstring(_T("generateRadius")),temp = generateRadius);
	val.doubles.dim(tstring(_T("generateRatio")),temp = generateRatio);
	
	val.doubles.dim(tstring(_T("delay")),temp = delay);
]]
cascade1 = 0.5
cascade3 = 0.5

particle_life = 20

burnColor = Color(burnColor_D3Dcolor)
burnColor_trans = Color(burnColor)
burnColor_trans.alpha = 0
textureRangeRect = TextureRangeRect(texRengeX1, texRengeY1, texRengeX2, texRengeY2)


hab_parallel = AnimeChip_Hab(PositionPerGrid(placeX, placeY, 0))

for i = 0, generateRatio*duration, 1 do
    mainPerticle = AnimeChip_Perticle(string(fileName), D3DXVECTOR2(size,size), duration)
    mainPerticle: setDelay(delay + duration*random())
    
    mainPerticle.color: addCascade(burnColor_trans, burnColor, burnColor, particle_life*cascade1)
    mainPerticle.color: addCascade(burnColor, burnColor, burnColor_trans, particle_life*cascade3)
    
    local randomValue = random()
    local position = D3DXVec3XYPlaneRotate(PositionPerGrid(generateRadius*(1 -randomValue*randomValue),0,0), radianForDegree(360*random()))
    mainPerticle.position: addCascade(position, particle_life)

    mainPerticle.rotationBefore: addCascade(360*random(), particle_life)
    
    mainPerticle.scaleX: addCascade(0.8, 1.2, particle_life)
    mainPerticle.scaleY = mainPerticle.scaleX

    mainPerticle.drawMode: setDefault(DrawMode_normal)

    hab_parallel: addAnimeChip(mainPerticle)
end
	
setAnimation_parallel(hab_parallel)
