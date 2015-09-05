-- 数値型 PlaceX : 罠のX位置（マス）
-- 数値型 PlaceY : 罠のY位置（マス）
-- 数値型 direction : 被害者の向き（０～３６０）被害者がいない場合はー３６０
-- pcCharacter型 sufferer : 被害者（いない場合もあるがいない場合で

hab = AnimeChip_Hab(PositionPerGrid(PlaceX, PlaceY, 0))

-- sound
sound = AnimeChip_Sound("slowdown.wav",PositionPerGrid(PlaceX,PlaceY,0),1.0)
hab:addAnimeChip(sound)
sound: setDelay(5)

-- 
texRange_ = TextureRangeRect(0,0,1,1)
color_ = Color(255,255,255,255)
placeX_ = charaPos(PlaceX)
placeY_ = charaPos(PlaceY) - 0.5
effect_generalFog(placeX_, placeY_, _T("effect\\trap\\turu1.png"), texRange_, color_, 64, 0.2, 0.2, 40, 0)
effect_generalFog(placeX_, placeY_, _T("effect\\trap\\turu2.png"), texRange_, color_, 64, 0.2, 0.15, 40, 0)
effect_WaitPararellCritical(placeX_, placeY_, 30)


setAnimation_wait(hab)