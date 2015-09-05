-- when equip a spell
-- valiable
-- Number : PlaceX, PlaceY, direction, imageColor1, imageColor2
-- Chara : player
imageColor1_color = Color(imageColor1)
imageColor1_color.alpha = 128
imageColor2_color = Color(imageColor2)
imageColor2_color.alpha = 128
life = 60
section1 = 0.10
section2 = 0.80 --catch
section3 = 0.10
handheight = 0.7

--
life3 = 50
belt_hab = AnimeChip_Hab(PositionPerGrid(PlaceX, PlaceY, 0))
for i = 1, 20 do

	kaiten = random()*40-20

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
		pos = PositionPerGrid(k/5.0,0,0)

		D3DXVec3TransformCoord(pos,pos,turn_matrix)
		
        belt_anime.leftEdge: push_back(VariationValue_PositionPerGrid(pos))
		belt_anime.rightEdge: push_back(VariationValue_PositionPerGrid(pos))

	end

	belt_hab: addAnimeChip(belt_anime);

end


ring_perticle_anime = AnimeChip_Perticle("effect\\basic\\ring_out.png",D3DXVECTOR2(64,64),10)
belt_hab: addAnimeChip(ring_perticle_anime)
ring_perticle_anime:setDelay(life)
ring_perticle_anime:setWorldRotation(direction)
ring_perticle_anime.color: addCascade(imageColor1_color,imageColor2_color,Color(0,0,0,0),10)
ring_perticle_anime.scaleX:addCascade(1,10,10)
ring_perticle_anime.scaleY:addCascade(1,10,10)
ring_perticle_anime.drawMode: setDefault(DrawMode_addition)
ring_perticle_anime.position: setDefault(PositionPerGrid(0,0,0.5))

setAnimation_parallel(belt_hab)
--

hab = AnimeChip_Hab(PositionPerGrid(PlaceX, PlaceY, 0))
spell_perticle_anime = AnimeChip_Perticle("effect\\spellcard.png",D3DXVECTOR2(32,32),life*(section1+section2))
hab:addAnimeChip(spell_perticle_anime)
spell_perticle_anime:setWorldRotation(direction)
spell_perticle_anime.color: addCascade(imageColor2_color,life*section1)
spell_perticle_anime.color: addCascade(imageColor2_color,life*section2)
spell_perticle_anime.color: addCascade(Color(0,0,0,0),life*section3)
spell_perticle_anime.rotationAfter: addCascade(0,life*section1)
spell_perticle_anime.rotationAfter: addCascade(0,360*4,life*section2)
spell_perticle_anime.drawMode: setDefault(DrawMode_normal)
spell_perticle_anime.position: addCascade(PositionPerGrid(0.2,0,handheight),life*section1)
spell_perticle_anime.position: addCascade(PositionPerGrid(0.2,0,handheight),PositionPerGrid(0,0,6.5),PositionPerGrid(-0.2,0,handheight),life*section2)
spell_perticle_anime.position: addCascade(PositionPerGrid(0, 0, 2.5),life*section3)
spell_perticle_anime2 = AnimeChip_Perticle(spell_perticle_anime)
hab:addAnimeChip(spell_perticle_anime2)
spell_perticle_anime2.drawMode: setDefault(DrawMode_addition)

spell_perticle_anime3 = AnimeChip_Perticle(spell_perticle_anime)
spell_perticle_anime3.color: clear()
spell_perticle_anime3.color: addCascade(imageColor1_color,life*section1)
spell_perticle_anime3.color: addCascade(imageColor1_color,life*section2)
spell_perticle_anime3.color: addCascade(Color(0,0,0,0),life*section3)
spell_perticle_anime2: setDelay(2)
hab:addAnimeChip(spell_perticle_anime3)

spell_perticle_anime4 = AnimeChip_Perticle(spell_perticle_anime3)
spell_perticle_anime4.drawMode: setDefault(DrawMode_addition)
hab:addAnimeChip(spell_perticle_anime4)

catch_perticle_anime = AnimeChip_Perticle("effect\\basic\\ring_radio.png",D3DXVECTOR2(16,16),life*(section3))
hab:addAnimeChip(catch_perticle_anime)
catch_perticle_anime:setWorldRotation(direction)
catch_perticle_anime:setDelay(life*(section1+section2))
catch_perticle_anime.color:addCascade(Color(0,255,255,255),Color(255*2,255,255,255),Color(0,255,255,255),life*section3)
catch_perticle_anime.scaleX:addCascade(1,3,life*section3)
catch_perticle_anime.scaleY:addCascade(1,3,life*section3)
catch_perticle_anime.drawMode:setDefault(DrawMode_addition)
catch_perticle_anime.position:setDefault(PositionPerGrid(-0.2,0,handheight))

chara_anime = AnimeChip_Chara(player,life)
hab:addAnimeChip(chara_anime)
chara_anime:setWorldRotation(direction)
--chara_anime.pitching:addCascade(0,life*2)
--chara_anime.pitching:addCascade(0,360,life)
chara_anime.yawing:addCascade(0,life*0.75)
chara_anime.yawing:addCascade(0,360,life*0.25)

setAnimation_wait(hab)

life2 = 30
hab_spell = AnimeChip_Hab(PositionPerGrid(PlaceX, PlaceY, 2.5))
spell2_perticle_anime = AnimeChip_Perticle("effect\\spellcard.png",D3DXVECTOR2(32,32),life2)
hab_spell: addAnimeChip(spell2_perticle_anime)
spell2_perticle_anime: setWorldRotation(direction)
spell2_perticle_anime.color: addCascade(imageColor1_color,imageColor2_color,Color(0,0,0,0),life2)
spell2_perticle_anime.scaleX: addCascade(1,3,life2)
spell2_perticle_anime.scaleY: addCascade(1,3,life2)
spell2_perticle_anime.drawMode: setDefault(DrawMode_normal)
spell2_perticle_anime.position: setDefault(PositionPerGrid(0,0,0))

spell2_perticle_anime2 = AnimeChip_Perticle(spell2_perticle_anime)
hab_spell: addAnimeChip(spell2_perticle_anime2)
spell2_perticle_anime2.drawMode: setDefault(DrawMode_addition)

setAnimation_parallel(hab_spell)




