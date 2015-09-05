-- 数値型 PlaceX : 罠のX位置（マス）
-- 数値型 PlaceY : 罠のY位置（マス）
-- 数値型 direction : 被害者の向き（０～３６０）被害者がいない場合はー３６０
-- pcCharacter型 sufferer : 被害者（いない場合もあるがいない場合で


hab = AnimeChip_Hab(PositionPerGrid(PlaceX, PlaceY, 0))

-- sound
sound = AnimeChip_Sound("fission.wav",PositionPerGrid(PlaceX,PlaceY,0),1.0)
sound: setDelay(50)
hab:addAnimeChip(sound)

effectlife = 80

masicCircle = AnimeChip_Perticle("effect\\magiccircle.png",D3DXVECTOR2(256,256), effectlife)
masicCircle.color: addCascade(Color(0,255,100,50),Color(255*2,255,255,50),Color(0,255,100,50), effectlife)
masicCircle.drawMode: setDefault(DrawMode_addition)

masicCircle.rotationBefore: addCascade(50, 180, effectlife)

hab: addAnimeChip(masicCircle)


setAnimation_wait(hab)