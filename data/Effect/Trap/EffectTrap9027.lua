-- 数値型 PlaceX : 罠のX位置（マス）
-- 数値型 PlaceY : 罠のY位置（マス）
-- 数値型 direction : 被害者の向き（０～３６０）被害者がいない場合はー３６０
-- pcCharacter型 sufferer : 被害者（いない場合もあるがいない場合で


bound_life = 50
hab = AnimeChip_Hab(PositionPerGrid(PlaceX, PlaceY, 0))

-- sound
sound = AnimeChip_Sound("trap9027.wav",PositionPerGrid(PlaceX,PlaceY,0),1.0)
hab:addAnimeChip(sound)
sound: setDelay(0)

-- wait用
bound_anime = AnimeChip_Bound("effect\\equip\\particle_dero.png", D3DXVECTOR2(0,0), PositionPerGrid(0,0,0) ,bound_life)
bound_anime.color: addCascade(Color(0,10,130,10), bound_life)
hab: addAnimeChip(bound_anime)


setAnimation_wait(hab)