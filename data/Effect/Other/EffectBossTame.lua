-- when equip a spell
-- valiable
-- Number : PlaceX, PlaceY, imageColor1, imageColor2
imageColor1_color = Color(imageColor1)
imageColor1_color.alpha = 128
imageColor2_color = Color(imageColor2)
imageColor2_color.alpha = 128
life = 20
section1 = 0.10
section2 = 0.80 --catch
section3 = 0.10
handheight = 0.7

--
life3 = 10
belt_hab = AnimeChip_Hab(PositionPerGrid(PlaceX, PlaceY, 0))
for i = 1, 20 do

	kaiten = 0--random()*40-20

	belt_anime = AnimeChip_Belt("effect\\basic\\ball_smoke.png",PositionPerGrid(0,0,0),PositionPerGrid(0,0, 0.3),life3)
	belt_anime.drawRange: addCascade(D3DXVECTOR2(29,29),D3DXVECTOR2(26,29),D3DXVECTOR2(26,29),D3DXVECTOR2(1,4),life3)
	if random() > 0.5 then
		belt_anime.color: setDefault(imageColor1_color)
	else
		belt_anime.color: setDefault(imageColor2_color)
	end

	belt_anime.drawMode: setDefault(DrawMode_addition)

	belt_anime: setDelay(random()*life3/2)

	base_trun = random()*360

	for k = 0, 30 do
		turn_matrix = D3DXMATRIX()
		D3DXMatrixRotationZ(turn_matrix, (k*kaiten+base_trun)/57.0 )
		pos = PositionPerGrid(k/6.0,0,0)

		D3DXVec3TransformCoord(pos,pos,turn_matrix)

		belt_anime.leftEdge: push_back(VariationValue_PositionPerGrid(pos))
		belt_anime.rightEdge: push_back(VariationValue_PositionPerGrid(pos))

	end

	belt_hab: addAnimeChip(belt_anime);

end


ring_perticle_anime = AnimeChip_Perticle("effect\\basic\\ring_in.png",D3DXVECTOR2(64,64),10)
belt_hab: addAnimeChip(ring_perticle_anime)
ring_perticle_anime:setDelay(life)
ring_perticle_anime.color: addCascade(imageColor1_color,imageColor2_color,Color(0,0,0,0),10)
ring_perticle_anime.scaleX:addCascade(10,1,10)
ring_perticle_anime.scaleY:addCascade(10,1,10)
ring_perticle_anime.drawMode: setDefault(DrawMode_addition)
ring_perticle_anime.position: setDefault(PositionPerGrid(0,0,0.5))

setAnimation_parallel(belt_hab)




