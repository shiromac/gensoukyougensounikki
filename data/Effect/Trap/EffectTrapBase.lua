
-- 数値型 PlaceX : 罠のX位置（マス）
-- 数値型 PlaceY : 罠のY位置（マス）
-- 数値型 direction : 被害者の向き（０～３６０）被害者がいない場合はー３６０
-- pcCharacter型 sufferer : 被害者（いない場合もあるがいない場合でも有効）

hab_parallel = AnimeChip_Hab(PositionPerGrid(PlaceX, PlaceY, 0))

sound = AnimeChip_Sound("trap.wav",PositionPerGrid(PlaceX,PlaceY,0),1.0)
hab_parallel: addAnimeChip(sound)

life = 15

ring_perticle_anime = AnimeChip_Perticle("effect\\basic\\ring_out.png",D3DXVECTOR2(512,512*0.75),10)
hab_parallel: addAnimeChip(ring_perticle_anime)
ring_perticle_anime:setWorldRotation(direction)
ring_perticle_anime.color: addCascade(Color(0,255,255,255),Color(255,255,255,255),Color(0,255,255,255),life)
ring_perticle_anime.scaleX:addCascade(0,0.7,life)
ring_perticle_anime.scaleY:addCascade(0,0.7,life)
ring_perticle_anime.drawMode: setDefault(DrawMode_addition)
ring_perticle_anime.position: setDefault(PositionPerGrid(0,0,0))


setAnimation_parallel(hab_parallel)
