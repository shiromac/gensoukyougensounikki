-- when equip a spell
-- valiable
-- Number : PlaceX, PlaceY, direction, imageColor1, imageColor2
-- Chara : player
life = 60
section1 = 0.15
section2 = 0.75 --catch
section3 = 0.1
handheight = 0.7
hab = AnimeChip_Hab(PositionPerGrid(PlaceX, PlaceY, 0))
spell_perticle_anime = AnimeChip_Perticle("effect\\spellcard.png",D3DXVECTOR2(32,32),life)
hab:addAnimeChip(spell_perticle_anime)
spell_perticle_anime:setWorldRotation(direction)
spell_perticle_anime.color: addCascade(Color(imageColor1),life*section1)
spell_perticle_anime.color: addCascade(Color(imageColor1),life*section2)
spell_perticle_anime.color: addCascade(Color(0,0,0,0),life*section3)
spell_perticle_anime.rotationAfter: addCascade(0,life*section1)
spell_perticle_anime.rotationAfter: addCascade(0,360*4,life*section2)
spell_perticle_anime.drawMode: setDefault(DrawMode_addition)
spell_perticle_anime.position: addCascade(PositionPerGrid(0.2,0,handheight),life*section1)
spell_perticle_anime.position: addCascade(PositionPerGrid(0.2,0,handheight),PositionPerGrid(0,0,7.5),PositionPerGrid(-0.2,0,handheight),life*section2)
spell_perticle_anime.position: addCascade(PositionPerGrid(0,0,0),1)
spell_perticle_anime2 = AnimeChip_Perticle("effect\\spellcard.png",D3DXVECTOR2(32,32),life)
spell_perticle_anime2: setDelay(1)
hab:addAnimeChip(spell_perticle_anime2)
spell_perticle_anime2:setWorldRotation(direction)
spell_perticle_anime2.color: addCascade(Color(imageColor2),life*section1)
spell_perticle_anime2.color: addCascade(Color(imageColor2),life*section2)
spell_perticle_anime2.color: addCascade(Color(0,0,0,0),life*section3)
spell_perticle_anime2.rotationAfter:addCascade(0,life*section1)
spell_perticle_anime2.rotationAfter:addCascade(0,360*4,life*section2)
spell_perticle_anime2.drawMode:setDefault(DrawMode_addition)
spell_perticle_anime2.position:addCascade(PositionPerGrid(0.2,0,handheight),life*section1)
spell_perticle_anime2.position:addCascade(PositionPerGrid(0.2,0,handheight),PositionPerGrid(0,0,7.5),PositionPerGrid(-0.2,0,handheight),life*section2)
spell_perticle_anime2.position:addCascade(PositionPerGrid(0,0,0),1)

catch_perticle_anime = AnimeChip_Perticle("effect\\basic\\ring_radio.png",D3DXVECTOR2(16,16),life*(section3))
hab:addAnimeChip(catch_perticle_anime)
catch_perticle_anime:setWorldRotation(direction)
catch_perticle_anime:setDelay(life*(section1+section2))
catch_perticle_anime.color:addCascade(Color(0,255,255,255),Color(255*2,255,255,255),Color(0,255,255,255),life*section3)
catch_perticle_anime.scaleX:addCascade(1,3,life*section3)
catch_perticle_anime.scaleY:addCascade(1,3,life*section3)
catch_perticle_anime.drawMode:setDefault(DrawMode_addition)
catch_perticle_anime.position:setDefault(PositionPerGrid(-0.2,0,handheight))

life2 = 30
hab_spell = AnimeChip_Hab(PositionPerGrid(0, 0, 2.5))
spell_perticle_anime: addNextChip(hab_spell)
spell2_perticle_anime = AnimeChip_Perticle("effect\\spellcard.png",D3DXVECTOR2(32,32),life2)
hab_spell: addAnimeChip(spell2_perticle_anime)
spell2_perticle_anime: setWorldRotation(direction)
spell2_perticle_anime.color: addCascade(Color(imageColor1),Color(imageColor1),Color(0,0,0,0),life2)
spell2_perticle_anime.scaleX: addCascade(1,3,life*section3)
spell2_perticle_anime.scaleY: addCascade(1,3,life*section3)
spell2_perticle_anime.drawMode: setDefault(DrawMode_addition)
spell2_perticle_anime.position: setDefault(PositionPerGrid(0,0,0))

spell2_perticle_anime2 = AnimeChip_Perticle("effect\\spellcard.png",D3DXVECTOR2(32,32),life2)
hab_spell: addAnimeChip(spell2_perticle_anime2)
spell2_perticle_anime2:setWorldRotation(direction)
spell2_perticle_anime2.color: addCascade(Color(imageColor2),Color(imageColor2),Color(0,0,0,0),life2)
spell2_perticle_anime2.scaleX: addCascade(1,4,life*section3)
spell2_perticle_anime2.scaleY: addCascade(1,4,life*section3)
spell2_perticle_anime2.drawMode: setDefault(DrawMode_addition)
spell2_perticle_anime2.position: setDefault(PositionPerGrid(0,0,0))

chara_anime = AnimeChip_Chara(player,life)
hab:addAnimeChip(chara_anime)
chara_anime:setWorldRotation(direction)
--chara_anime.pitching:addCascade(0,life*2)
--chara_anime.pitching:addCascade(0,360,life)
chara_anime.yawing:addCascade(0,life*0.75)
chara_anime.yawing:addCascade(0,360,life*0.25)

setAnimation_wait(hab)
