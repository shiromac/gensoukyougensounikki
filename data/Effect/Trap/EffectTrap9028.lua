-- 数値型 PlaceX : 罠のX位置（マス）
-- 数値型 PlaceY : 罠のY位置（マス）
-- 数値型 direction : 被害者の向き（０～３６０）被害者がいない場合はー３６０
-- pcCharacter型 sufferer : 被害者（いない場合もあるがいない場合で

hab = AnimeChip_Hab(PositionPerGrid(PlaceX, PlaceY, 0))

-- sound


-- 
fileName_ = _T("effect\\basic\\ring_smooth.png")
texRange_ = TextureRangeRect(0,0,1,1)
color_ = Color(128,150,255,155)
placeX_ = charaPos(PlaceX)
placeY_ = charaPos(PlaceY) - 0.5
effect_generalFog(placeX_, placeY_, fileName_, texRange_, color_, 30, 0.5, 0.5, 40, 0)
effect_WaitPararellCritical(placeX_, placeY_, 30)


setAnimation_wait(hab)