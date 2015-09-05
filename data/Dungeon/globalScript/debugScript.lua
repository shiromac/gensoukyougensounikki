
function floorInitProcess()


end

function dungeonProcess()
	--outputDebugString("dungeonProcess_start\n")
	if isUnRelease() then
        control = controllerInput()
        if control: smartdash().on and control: dash().on and control: diagon().justOn then
            goToNextFloor()
        end
        if control: smartdash().on and control: dash().on and control: miniMap().justOn then
            CharacterLevelUp(PlayerCharacter(),1)
        end
        if control: smartdash().on and control: dash().on and control: attack().justOn then
            vec_chara = allCharacterList()
            charalistsize = vec_chara: size()
            for i = 0, charalistsize-1 do
                if (vec_chara:at(i).forse == CHARACTER_FORSE_ENEMY) then
                    CharacterForsedDamage(vec_chara:at(i),100)
                end
            end
        end
        
        if control: smartdash().on and control: dash().on and control: turn().justOn then
            fileName_ = _T("effect\\sukima.png")
            texRange_ = TextureRangeRect(0,0,1,1)
            color_ = Color(255,255,255,255)
            placeX_ = PlayerCharacter().placeX
            placeY_ = PlayerCharacter().placeY
            effect_generalFog(placeX_, placeY_, fileName_, texRange_, color_, 100, 1, 1, 100, 30)
        end
        --[[
        if control: smartdash().on and control: dash().on and control: turn().justOn then
            if (g_setClearResultSkipFlag) then
                g_setClearResultSkipFlag = false
                messageWithStringStyle(StyleString(_T("ClearResultSkipFlag: false\n")))
            else
                g_setClearResultSkipFlag = true
                messageWithStringStyle(StyleString(_T("ClearResultSkipFlag: true\n")))
            end
            setClearResultSkipFlag(g_setClearResultSkipFlag)
        end
        ]]
    end
end


function dungeonTurnProcess()
	--outputDebugString("dungeonTurnProcess_start\n")

end