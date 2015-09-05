-- cCharacter 型 player : プレイヤーキャラクター


emeny = MakeCharactor(2000, 1, CHARACTER_FORSE_ENEMY, LandformPlace(25,27))
CharacterCureMental(emeny)
CharacterSwingAir(emeny, TURN_FOREVER)
CharacterInIce(emeny, TURN_FOREVER)


emeny = MakeCharactor(2000, 1, CHARACTER_FORSE_ENEMY, LandformPlace(26,22))
CharacterCureMental(emeny)
CharacterSwingAir(emeny, TURN_FOREVER)
CharacterInIce(emeny, TURN_FOREVER)

emeny = MakeCharactor(2000, 1, CHARACTER_FORSE_ENEMY, LandformPlace(21,11))
CharacterCureMental(emeny)
CharacterSwingAir(emeny, TURN_FOREVER)
CharacterInIce(emeny, TURN_FOREVER)
CharacterPoor(emeny, TURN_FOREVER)

DropingQualityForsedMakeValue(item, 10)