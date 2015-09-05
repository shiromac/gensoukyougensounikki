--1049 §Œä–_

-- ”’lŒ^ fromPlaceX : UŒ‚Ò‚ÌXˆÊ’uiƒ}ƒXj
-- ”’lŒ^ fromPlaceY : UŒ‚Ò‚ÌYˆÊ’uiƒ}ƒXj
-- ”’lŒ^ toPlaceX : UŒ‚æ‚ÌXˆÊ’uiƒ}ƒXj
-- ”’lŒ^ toPlaceY : UŒ‚æ‚ÌYˆÊ’uiƒ}ƒXj
-- ”’lŒ^ direction : UŒ‚Ò‚ÌŒü‚«i‚O`‚R‚U‚Oj
-- ”’lŒ^ distance : UŒ‚Ò‚ÆUŒ‚æ‚Ì‹——£iƒ}ƒXj
-- ”’lŒ^ isValidAttack : UŒ‚‚ª•Ç‚ÉŒü‚©‚Á‚Ä‚¢‚½‚ç‚OA‚»‚¤‚Å‚È‚¯‚ê‚Î‚P
-- pcCharacterŒ^ attacker : UŒ‚Ò‚ÌƒLƒƒƒ‰ƒNƒ^[


hab = AnimeChip_Hab(PositionPerGrid(toPlaceX, toPlaceY, 0))


-- fumikiri
fumikiri_life = 16
fumikomi_distance = isValidAttack * sqrt((fromPlaceX - toPlaceX)*(fromPlaceX - toPlaceX) + (fromPlaceY - toPlaceY)*(fromPlaceY - toPlaceY))/ distance 

chara_anime = AnimeChip_Chara(attacker,fumikiri_life)
hab:addAnimeChip(chara_anime)
chara_anime: setWorldRotation(direction)
chara_anime.position: addCascade(PositionPerGrid(0, 0, 0), PositionPerGrid(0, -0.1, 0), PositionPerGrid(0, -0.1, 0), fumikiri_life*0.5)
chara_anime.position: addCascade(PositionPerGrid(0, -0.1, 0), PositionPerGrid(0, -0.8*fumikomi_distance, 0), PositionPerGrid(0, -0.4*fumikomi_distance, 0), PositionPerGrid(0, 0, 0), fumikiri_life*0.5)
chara_anime.yawing: addCascade(0, 45, fumikiri_life*0.5)
chara_anime.yawing: addCascade(45, -90, 0, fumikiri_life*0.5)
--

-- sound
sound = AnimeChip_Sound("swing1049.wav",PositionPerGrid(toPlaceX,toPlaceY,0),1.0)
hab:addAnimeChip(sound)
sound: setDelay(fumikiri_life*0.5+1)

--
hab_parallel = AnimeChip_Hab(PositionPerGrid(toPlaceX, toPlaceY, 0))


syougeki_life = 16
-- =============================================================
-- ÕŒ‚”g ŠO‘¤
syougeki11 = AnimeChip_Perticle("effect\\basic\\ball_simple.png",D3DXVECTOR2(180,180), syougeki_life)
syougeki11.color: addCascade(Color(200,180,180,180), Color(200,180,180,180), Color(0,180,180,180), syougeki_life)
syougeki11.drawMode: setDefault(DrawMode_normal)
syougeki11.rotationBefore: addCascade(360*random(), 0, syougeki_life)
syougeki11.scaleX:	addCascade(0.1, 1, syougeki_life)
syougeki11.scaleY:	addCascade(0.1, 1, syougeki_life)
--syougeki11:setWorldRotation(direction)
syougeki11: setDelay(2)
syougeki11.position: addCascade( PositionPerGrid(0, 0, 0.5), syougeki_life)

-- ƒRƒs[‚µ‚Ä‰ÁZ‡¬
syougeki12 = AnimeChip_Perticle(syougeki11)
syougeki12: setDelay(2)
syougeki12.color: clear()
syougeki12.color: addCascade(Color(255,255,150,30), Color(120,255,150,30), syougeki_life)
syougeki12.scaleX: clear()
syougeki12.scaleY: clear()
syougeki12.scaleX:	addCascade(0.1, 0.7, syougeki_life)
syougeki12.scaleY:	addCascade(0.1, 0.7, syougeki_life)

syougeki12.drawMode: setDefault(DrawMode_addition)

hab_parallel: addAnimeChip(syougeki11);
hab_parallel: addAnimeChip(syougeki12);
-- =============================================================

syougeki_life = 16
-- =============================================================
-- ÕŒ‚”g ‰ñ“]1,2
syougeki21 = AnimeChip_Perticle("effect\\basic\\ring_nagi.png",D3DXVECTOR2(80,80), syougeki_life)
syougeki21.color: addCascade(Color(200,250,80,30), Color(0,250,80,30), syougeki_life)
syougeki21.drawMode: setDefault(DrawMode_normal)
syougeki21.rotationBefore: addCascade(0, 720, syougeki_life)
syougeki21.scaleX:	addCascade(0.6, 1, syougeki_life)
syougeki21.scaleY:	addCascade(0.6, 1, syougeki_life)
--syougeki21:setWorldRotation(direction)
syougeki21: setDelay(2)
syougeki21.position: addCascade( PositionPerGrid(0, 0, 0.5), syougeki_life)

-- ƒRƒs[‚µ‚Ä‰ÁZ‡¬
syougeki22 = AnimeChip_Perticle(syougeki21)
syougeki22: setDelay(2)
syougeki22.color: clear()
syougeki22.color: addCascade(Color(255,255,130,30), Color(0,255,130,30), syougeki_life)
syougeki22.drawMode: setDefault(DrawMode_addition)

hab_parallel: addAnimeChip(syougeki21);
hab_parallel: addAnimeChip(syougeki22);
-- =============================================================

syougeki_life = 8
-- =============================================================
-- ÕŒ‚”g ŠO‘¤
syougeki31 = AnimeChip_Perticle("effect\\basic\\ring_radio.png",D3DXVECTOR2(100,100), syougeki_life)
syougeki31.color: addCascade(Color(120,255,120,0),  syougeki_life)
syougeki31.drawMode: setDefault(DrawMode_normal)
syougeki31.rotationBefore: addCascade(360, 0, syougeki_life)
syougeki31.scaleX:	addCascade(0.5, 1, syougeki_life)
syougeki31.scaleY:	addCascade(0.5, 1, syougeki_life)
--syougeki11:setWorldRotation(direction)
syougeki31: setDelay(2)
syougeki31.position: addCascade( PositionPerGrid(0, 0, 0.5), syougeki_life)

-- ƒRƒs[‚µ‚Ä‰ÁZ‡¬
syougeki32 = AnimeChip_Perticle(syougeki31)
syougeki32: setDelay(2)
syougeki32.color: clear()
syougeki32.color: addCascade(Color(120,200,120,30), Color(0,200,120,30), syougeki_life)
syougeki32.scaleX: clear()
syougeki32.scaleY: clear()
syougeki32.scaleX:	addCascade(0.1, 0.9, syougeki_life)
syougeki32.scaleY:	addCascade(0.1, 0.9, syougeki_life)

syougeki32.drawMode: setDefault(DrawMode_addition)

hab_parallel: addAnimeChip(syougeki31);
hab_parallel: addAnimeChip(syougeki32);
-- =============================================================


setAnimation_parallel(hab_parallel)
setAnimation_wait(hab)