
#include "AnimeFunctions.h"
#include "cDungeonSystem.h"
#include "AnimeChips.h"

class caWarp;
typedef boost::shared_ptr<caWarp> pcaWarp;
class caWarp :
	public cAnimation
{
public:
	caWarp(pcCharacter pchara, pcLandform toLand, int aspect, bool neverTurn, double speed, bool omission):
	  _chara(pchara), _toLand(toLand), _aspect(aspect), _initited(false) {

		cCoordinate first_place(pchara->placeX, pchara->placeY);
		pcLandform fromLand = pchara->足元地形();
		cCoordinate end_place(toLand->place.x, toLand->place.y);
		double basic_move_life = 16*(first_place - end_place).dif()/speed;
		_chara_anime = pAnimeChip_Chara(new AnimeChip_Chara(pchara, basic_move_life));
		_chara_anime->Init(_chara_anime);

		_chara_anime->setAlwaysOnScreen(!omission);
		_chara_anime->position.addCascade(PositionPerGrid(0, 0, 0), PositionPerGrid(end_place.x - first_place.x, end_place.y - first_place.y, 0), basic_move_life);
		//_chara_anime->position.addCascade(- PositionPerGrid(end_place.x - first_place.x, end_place.y - first_place.y, 0), PositionPerGrid(0, 0, 0), basic_move_life);
		//chara_anime.yawing: addCascade((aspect - chara.aspect)*45 , basic_move_life)

		//pAnimeChip_Chara chara_anime(new AnimeChip_Chara(pchara, basic_move_life));
		//hab->addAnimeChip(chara_anime);
	
	};

	caWarp(pcCharacter pchara, pcLandform toLand, int aspect, pAnimeChip_Chara chara_anime):
	  _chara(pchara), _toLand(toLand), _aspect(aspect), _initited(false) {
		_chara_anime = chara_anime;
	};
public:
	virtual ~caWarp(void){};


	virtual int process(cRenderDevice *pDev){
		if(!_initited) {
			if(sg_pDungeonSystem->ワープ地形移動可(_chara,_toLand)) {
				sg_pDungeonSystem->方向転換要請_主人公専用(_chara,_aspect);
			}
			_initited = true;
		}
		_chara_anime->process(pDev);
		
		if(_chara_anime->End) {
			if(sg_pDungeonSystem->ワープ地形移動可(_chara,_toLand)) {
				sg_pDungeonSystem->ワープ要請(_chara,_toLand);
			}
			End = true;
		}

		return false;
	};
	virtual int Draw(cRenderDevice *pDev){
		return _chara_anime->Draw(pDev);
	}
	virtual bool isOnScreen(){return _chara_anime->isOnScreen();};
	virtual bool isOnScreenNow(){return _chara_anime->isOnScreenNow();};
	bool _initited;
	pcCharacter _chara;
	pcLandform _toLand;
	int _aspect;
	pAnimeChip_Chara _chara_anime;
};


namespace AnimeFunctions
{

bool moveWithAnimeToLand(pcCharacter pchara, pcLandform toLand, bool neverTurn, double speed, bool omission)
{
	if(pchara == NULL) {
		return false;
	}
	if(speed <= 0) {
		return false;
	}
	
	if(!sg_pDungeonSystem->ワープ地形移動可(pchara,toLand))
	{
		return false;
	}

	
    cCoordinate first_place(pchara->placeX, pchara->placeY);
    pcLandform fromLand = pchara->足元地形();
    cCoordinate end_place(toLand->place.x, toLand->place.y);
	int aspect = pchara->aspect;
	if(neverTurn == false)
	{
		aspect = (end_place - first_place).GetAspect();
	}
	//pAnimeChip_Hab hab(new AnimeChip_Hab(PositionPerGrid(first_place.x, first_place.y, 0)));
    //int aspect = (end_place - first_place).GetAspect();
    //double basic_move_life = 16*(first_place - end_place).dif()/speed;
    //pAnimeChip_Chara chara_anime(new AnimeChip_Chara(pchara, basic_move_life));
	//hab->addAnimeChip(chara_anime);

	//chara_anime->setAlwaysOnScreen(!omission);
    //chara_anime->position.addCascade(PositionPerGrid(0, 0, 0), PositionPerGrid(end_place.x - first_place.x, end_place.y - first_place.y, 0), basic_move_life);
    //chara_anime->position.addCascade(- PositionPerGrid(end_place.x - first_place.x, end_place.y - first_place.y, 0), PositionPerGrid(0, 0, 0), basic_move_life);
    //chara_anime.yawing: addCascade((aspect - chara.aspect)*45 , basic_move_life)
	

	pcaWarp moveAnime(new caWarp(pchara, toLand, aspect, neverTurn, speed, omission));
	//sg_pDungeonSystem->AnimationManager().AddAnime_parallel(chara_anime);
	sg_pDungeonSystem->AnimationManager().AddAnime_parallelCritical(moveAnime);

	/*
    local first_place = Coordinate(chara.placeX, chara.placeY)
    local first_land = LandformCharacterPosition(chara)
    local end_place = Coordinate(end_land.place.x, end_land.place.y)

    local hab = AnimeChip_Hab(PositionPerGrid(first_place.x, first_place.y, 0))
    local aspect = (end_place - first_place): getAspect()
    local basic_move_life = 16*(first_place - end_place):dif()/speed
    local chara_anime = AnimeChip_Chara(chara, basic_move_life)
    hab:addAnimeChip(chara_anime)

    --chara_anime: setWorldRotation(chara.aspect*45)
    chara_anime.position: addCascade(PositionPerGrid(0, 0, 0), PositionPerGrid(end_place.x - first_place.x, end_place.y - first_place.y, 0), basic_move_life)
    chara_anime.yawing: addCascade((aspect - chara.aspect)*45 , basic_move_life)
    
    if paralleled then
        setAnimation_parallel(hab)
    else
        setAnimation_wait(hab)
        stackFunction(CharacterWarp, chara, end_land)
		stackFunction(CharacterVisualRefresh, chara)
    end
	*/
	return false;
}
bool moveWithAnime(pcCharacter pchara, int aspect, int distance, bool neverTurn, double speed, bool omission)
{
	if(pchara == NULL)
	{
		return false;
	}
	return moveWithAnimeToLand(pchara, sg_pDungeonSystem->隣接地形(pchara->足元地形(),aspect,distance), neverTurn, speed, omission);
}

bool jumpWithAnimeToLand(pcCharacter pchara, pcLandform toLand, double height, bool neverTurn, double speed, bool omission)
{
	if(pchara == NULL) {
		return false;
	}
	if(speed <= 0) {
		return false;
	}
	
	if(!sg_pDungeonSystem->ワープ地形移動可(pchara,toLand) && toLand->pOnChar != pchara)
	{
		return false;
	}
	
    cCoordinate first_place(pchara->placeX, pchara->placeY);
    pcLandform fromLand = pchara->足元地形();
    cCoordinate end_place(toLand->place.x, toLand->place.y);
	int aspect = pchara->aspect;
	if(neverTurn == false)
	{
		aspect = (end_place - first_place).GetAspect();
	}

	double basic_move_life = (sqrt(height)*24)/speed;
	pAnimeChip_Chara chara_anime = pAnimeChip_Chara(new AnimeChip_Chara(pchara, basic_move_life));
	chara_anime->Init(chara_anime);

	chara_anime->setAlwaysOnScreen(!omission);
	cCoordinate diffPlace = end_place - first_place;
	chara_anime->position.addCascade(PositionPerGrid(0, 0, 0), PositionPerGrid(diffPlace.x/2.0, diffPlace.y/2.0, height*2), PositionPerGrid(diffPlace.x, diffPlace.y, 0), basic_move_life);
	//_chara_anime->position.addCascade(- PositionPerGrid(end_place.x - first_place.x, end_place.y - first_place.y, 0), PositionPerGrid(0, 0, 0), basic_move_life);
	//chara_anime.yawing: addCascade((aspect - chara.aspect)*45 , basic_move_life)


	pcaWarp moveAnime(new caWarp(pchara, toLand, aspect, chara_anime));
	sg_pDungeonSystem->AnimationManager().AddAnime_parallelCritical(moveAnime);

	return true;
}
bool jumpWithAnime(pcCharacter pchara, int aspect, int distance, double height, bool neverTurn, double speed, bool omission)
{
	if(pchara == NULL)
	{
		return false;
	}
	return jumpWithAnimeToLand(pchara, sg_pDungeonSystem->隣接地形(pchara->足元地形(),aspect,distance), height, neverTurn, speed, omission);

}


}















