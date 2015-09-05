--[[
	val.doubles.dim(tstring(_T("placeX")),temp = placeX);
	val.doubles.dim(tstring(_T("placeY")),temp = placeY);

	val.doubles.dim(tstring(_T("part")),temp = part);
	
    luabind::class_<EffectFunctions::SpellEffectPart_class>("SpellEffectPart")
    .enum_("constants")
    [
        luabind::value("main", EffectFunctions::SpellEffectPart_class::SpellEffectPart_Main),
        luabind::value("effected", EffectFunctions::SpellEffectPart_class::SpellEffectPart_Effected),
        luabind::value("notEffected", EffectFunctions::SpellEffectPart_class::SpellEffectPart_NotEffected)
    ]
]]

if(SpellEffectPart.main == part)then
    hab = AnimeChip_Hab(PositionPerGrid(PlaceX, PlaceY, 0))
    
    life = 30
    
    color_white = Color(255,255,255,255)
    color_white_trans = Color(0,255,255,255)

    left_wing = AnimeChip_Perticle("effect\\basic\\square_gradation.png",D3DXVECTOR2(1024,1024),life)
    left_wing.color: addCascade(color_white_trans, color_white, color_white_trans, life)
    
    left_wing.scaleX:addCascade(0.1, 0.2, life)
    left_wing.scaleY:addCascade(1, life)

    left_wing.rotationBefore: setDefault(180)
    
    left_wing.drawMode: setDefault(DrawMode_addition)
    
    left_wing.position: addCascade(PositionPerGrid(0,0,0), PositionPerGrid(0,0,0), PositionPerGrid(-5,0,0), life)
    
    right_wing = AnimeChip_Perticle(left_wing)
    right_wing.drawMode: setDefault(DrawMode_substraction)
    
    right_wing.rotationBefore: setDefault(0)
    right_wing.position: clear()
    right_wing.position: addCascade(PositionPerGrid(0,0,0), PositionPerGrid(0,0,0), PositionPerGrid(5,0,0), life)
    
    hab: addAnimeChip(left_wing)
    hab: addAnimeChip(right_wing)
    setAnimation_parallel(hab)

    placeX_ = charaPos(placeX)
    placeY_ = charaPos(placeY)
    effect_Wait(placeX_, placeY_, life - 5)
elseif(SpellEffectPart.effected == part)then
    fileName_ = _T("effect\\spell\\yuuzai.png")
    texRange_ = TextureRangeRect(0,0,1,1)
    color_ = Color(255,255,255,255)
    placeX_ = charaPos(placeX)
    placeY_ = charaPos(placeY) - 0.5
    effect_generalStamp(placeX_, placeY_, fileName_, texRange_, color_, 100, 80, 0)
    effect_WaitPararellCritical(placeX_, placeY_, 70)
    effect_lightning(placeX_, placeY_);
elseif(SpellEffectPart.notEffected == part)then
    fileName_ = _T("effect\\spell\\muzai.png")
    texRange_ = TextureRangeRect(0,0,1,1)
    color_ = Color(255,255,255,255)
    placeX_ = charaPos(placeX)
    placeY_ = charaPos(placeY) - 0.5
    effect_generalStamp(placeX_, placeY_, fileName_, texRange_, color_, 100, 80, 0)
    effect_WaitPararellCritical(placeX_, placeY_, 70)
end
--
