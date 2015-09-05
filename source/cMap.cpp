#include "stdafx.h"


#include "cMap.h"


#include "cMapChipRule.h"
#include "debug.h"


#define VISIBLEBOXNUM 4

#define MAPTEXHEIGHT 512
#define MAPTEXWIDTH 512


#define MINIMAPTEXNAME _T("map\\minimapchip.png")
#define MINIMAPTEXHEIGHT 32
#define MINIMAPTEXWIDTH 32

#define MAPTEXVERSEP 6
#define MAPTEXHORSEP 16


#define MAPDARKTEXNAME _T("map\\dark2.png")
#define MAPDARKTEXHEIGHT 512
#define MAPDARKTEXWIDTH 256




cMap::cMap(void)
{
	additinal_visibleForcusX_ = 0;
	additinal_visibleForcusY_ = 0;
}

cMap::~cMap(void)
{
	RoomList.clear();
}

void cMap::Init(IDirect3DDevice9 *pDev)
{
	int x,y;

	for(y=0;y<MAPHEIGHT;y++)
	{
		for(x=0;x<MAPWIDTH;x++)
		{
			cLandform* l;
			l = new cLandform(x,y);

			l->setAttribute(MAPKIND_FLOOR);
			if(x==0||y==0||x==MAPWIDTH-1||y==MAPHEIGHT-1)
				l->setAttribute(MAPKIND_WALLEGDE);//特殊壁(掘れない)

			LandListOfSqr.push_back(pcLandform(l));
		}
	}
	EdgeLand = pcLandform(new cLandform);
	EdgeLand->setAttribute(MAPKIND_WALLEGDE);


	ResetMiniMap();

	Minimap.Init(pDev);
		
	m_pTexture_Dark =  g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev,MAPDARKTEXNAME);
	m_pTexture_Decochip = NULL;

	landrandam = LandListOfSqr;
	random_shuffle(landrandam.begin(),landrandam.end(),random_int);

	skyDecoAlpha_.assign((MAPHEIGHT+1)*(MAPWIDTH+1)+1,1.0);
}
void cMap::process()
{
	int i,size = LandListOfSqr.size();
	for(i=0;i<size;i++)
	{
		LandListOfSqr[i]->process_decoration();
	}

	mapChipAnimeAll->process();
}
void cMap::setChipset(const tstring& chipsetID)
{
	IDirect3DTexture9* pTex = g_GameEnv.m_GlobalResourse->getTextureFromFile(sg_pDungeonSystem->pDevice_D3D,
		(sg_pDungeonSystem->DataBase.MapChipFormImportDataMapchipFile(chipsetID)).c_str());
	if(pTex == NULL)
	{
		m_pTexture_Mapchip = 
			g_GameEnv.m_GlobalResourse->getTextureFromFile(sg_pDungeonSystem->pDevice_D3D, _T("map\\紅魔館セット.png"));
	}
	else
	{
		m_pTexture_Mapchip = pTex;
	}

	pTex = g_GameEnv.m_GlobalResourse->getTextureFromFile(sg_pDungeonSystem->pDevice_D3D,
		(sg_pDungeonSystem->DataBase.MapChipFormImportDataMapchipDecoFile(chipsetID)).c_str());
	if(pTex == NULL)
	{
		m_pTexture_Decochip = 
			g_GameEnv.m_GlobalResourse->getTextureFromFile(sg_pDungeonSystem->pDevice_D3D, _T("map\\紅魔館セットDeco.png"));
	}
	else
	{
		m_pTexture_Decochip = pTex;
	}

	mapChipAnimeAll = sg_pDungeonSystem->DataBase.MapImportDataChipAnimeAll(chipsetID);
	if(mapChipAnimeAll == NULL)
	{
		OnAssert(_T(__FILE__),__LINE__,false,_T("error chipset not found. チップセットが見つかりません: "),chipsetID.c_str());
	}
	mapChipAnimeAll->Init();

	vector<pcMapChipRule>& mapChipRules = sg_pDungeonSystem->DataBase.MapImportDataChipDecoRule(chipsetID);
	if(DecorationNum_.empty()) DecorationNum_.resize(mapChipRules.size(),0);

}
void cMap::DecorationAll(const tstring& chipsetID)
{
	//std::basic_ofstream<TCHAR> ofs(_T("debug.txt"), ios::app);/**/

	cRandomGen genran;
	genran.init_genrand(sg_pDungeonSystem->random_Seed());

	vector<pcMapChipRule>& mapChipRules = sg_pDungeonSystem->DataBase.MapImportDataChipDecoRule(chipsetID);
	

	int Numcount,MaxMatchingSize;
	int i,k;
	for(i=0;i<mapChipRules.size();i++)
	{
		//ofs << _T("in DecorationAll mapChipRules:") << i << _T(" Name:") << mapChipRules[i]->ruleName() << endl<TCHAR>;/**/

		Numcount = 0;
		MaxMatchingSize = (MAPWIDTH - mapChipRules[i]->sizeX()+1)*(MAPHEIGHT - mapChipRules[i]->sizeY()+1);
		for(k=0;k<landrandam.size();k++)
		{
			if(landrandam[k]->placeX+mapChipRules[i]->sizeX() > MAPWIDTH || landrandam[k]->placeY+mapChipRules[i]->sizeY() > MAPHEIGHT) continue;
			if(Numcount > mapChipRules[i]->checkPer()*MaxMatchingSize) break;
			if(DecorationNum_[i] >= mapChipRules[i]->maxNum()) break;

			Numcount++;
			
			if(mapChipRules[i]->appearPos() <= genran.random()) continue;//確率


			if(mapChipRules[i]->Matching(landrandam[k]->placeX, landrandam[k]->placeY))
			{
				pcMapChipObject pdeco = pcMapChipObject(new cMapChipObject);
				pdeco->Init(mapChipRules[i],landrandam[k]->placeX,landrandam[k]->placeY,DecorationObject.size());
				pdeco->SetDecoration();
				
				DecorationObject.push_back(pdeco);
				//ここに挿入処理
				DecorationNum_[i]++;
			}
			
		}
	}
}
void cMap::ReDecoration(const tstring& chipsetID, vector<pcLandform> changeLand)
{
	int i,size = changeLand.size();
	for(i=0;i<size;i++)
	{
		const set<int> setint = changeLand[i]->getDecorationConditionID();
		set<int>::const_iterator itr = setint.begin();
		set<int>::const_iterator itrend = setint.end();
		for(itr;itr!=itrend;itr++)
		{
			if(!DecorationObject[*itr]->CheckDecoration())
			{
				DecorationObject[*itr]->DeleteDecoration();//消すけどDecorationObjectからは削除しない。
			}
		}
	}
	
	DecorationAll(chipsetID);
}
double& cMap::skyDecoAlpha(int x,int y)
{
	if(x < 0 || x >= MAPWIDTH+1 || y < 0 || y >= MAPHEIGHT+1) return skyDecoAlpha_[(MAPWIDTH+1)*(MAPHEIGHT+1)];
	return skyDecoAlpha_[y*(MAPWIDTH+1)+x];
}

//再初期化
void cMap::Renew(IDirect3DDevice9 *pDev)
{

	//ResetMiniMap();
	//Minimap.Init(pDev);

	Minimap.EraseMiniMap(sg_pDungeonSystem->pPlayerChara()->placeX,sg_pDungeonSystem->pPlayerChara()->placeY);
}

//地形を描く
int cMap::Draw(IDirect3DDevice9 *pDev)
{
	int x,y;

	m_DO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;
	for(x = floor(visibleForcusX()) - VISIBLEBOXNUM; x < visibleForcusX() + VISIBLEBOXNUM +1;x++)
	{
		for(y = floor(visibleForcusY()) - VISIBLEBOXNUM; y < visibleForcusY() + VISIBLEBOXNUM +1;y++)
		{
			Drawchip(pDev, x, y);
		}
	}


	return true;
}
//地面の飾りを描く
int cMap::DrawDecoLand(IDirect3DDevice9 *pDev)
{
	int x,y;
	for(x = floor(visibleForcusX()) - VISIBLEBOXNUM; x < visibleForcusX() + VISIBLEBOXNUM +1;x++)
	{
		for(y = floor(visibleForcusY()) - VISIBLEBOXNUM; y < visibleForcusY() + VISIBLEBOXNUM +1;y++)
		{
			DrawDeco(pDev, x, y, cMap::DECORATIONLAYER_LAND1);
			DrawDeco(pDev, x, y, cMap::DECORATIONLAYER_LAND2);
			DrawDeco(pDev, x, y, cMap::DECORATIONLAYER_LAND3);
		}
	}
	return true;
}
//地面のグリッドを書く
int cMap::DrawGridLand(IDirect3DDevice9 *pDev,const double alpha, const double forcus_alpha )
{
	IDirect3DTexture9* gridTex = g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev,_T("interface\\grid.png"));
	//グリッド描画
	cDrawingObject DO,DOback;
	DO.setTexture(gridTex);
	DOback.setTexture(gridTex);

	int boxsize = MAPTEXBOXSIZE*MAPTEXPOWER;//枡の大きさ

	DO.Width = DO.Height = boxsize;
	DO.m_TexRange.setLTRB(0,0,
						1,1);
	DO.AddingDraw = cDrawableObject::DRAW_MODE_SUBSTRACTION;

	DOback.Width = DOback.Height = boxsize;
	DOback.m_TexRange.setLTRB(0,0,
						1,1);
	DOback.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;
	DOback.m_color.ARGB(128,128,128,128);
	DOback.m_color.alpha = 160*min(1.0,alpha);

	int x,y;
	for(x = floor(visibleForcusX()) - VISIBLEBOXNUM; x < visibleForcusX() + VISIBLEBOXNUM +1;x++)
	{
		for(y = floor(visibleForcusY()) - VISIBLEBOXNUM; y < visibleForcusY() + VISIBLEBOXNUM +1;y++)
		{
			DO.CenterX = (x-visibleForcusX())*boxsize + MAPDRAWCENTERX;
			DO.CenterY = (y-visibleForcusY())*boxsize + MAPDRAWCENTERY;
		
			DOback.CenterX = (x-visibleForcusX())*boxsize + MAPDRAWCENTERX;
			DOback.CenterY = (y-visibleForcusY())*boxsize + MAPDRAWCENTERY;
			DOback.Draw(pDev);
			
			if(Land(x,y)->RoomIndex == -1)
			{
				//通路
				DO.m_color.ARGB(255,24,84,24);
				DO.m_color.alpha = 255*alpha;
				DO.Draw(pDev);
			}
			else
			{
				if(Land(x,y)->mapped & cMiniMap::MAPPED_JUSTCHECK)
				{
					DO.m_color.ARGB(255,74,30,30);
					DO.m_color.alpha = 255*alpha;
					DO.Draw(pDev);
				}
				else
				{
					DO.m_color.ARGB(255,74,74,30);
					DO.m_color.alpha = 255*alpha;
					DO.Draw(pDev);
				}
			}
		}
	}

	DO.AddingDraw = cDrawableObject::DRAW_MODE_ADDITION;
	DO.m_color.ARGB(255,120,120,120);
	DO.m_color.alpha = 255*forcus_alpha;
	int i;
	for(i = 0; i < VISIBLEBOXNUM +1;i++)
	{
		pcLandform pland = sg_pDungeonSystem->キャラ前地形(sg_pDungeonSystem->pPlayerChara(), i);
		x = pland->placeX;
		y = pland->placeY;
		DO.CenterX = (x-visibleForcusX())*boxsize + MAPDRAWCENTERX;
		DO.CenterY = (y-visibleForcusY())*boxsize + MAPDRAWCENTERY;
		DO.Draw(pDev);
	}
	return true;
}
//地面のグリッドを書く
int cMap::DrawGridDrop(IDirect3DDevice9 *pDev,const double alpha)
{
	IDirect3DTexture9* gridTex = g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev,_T("interface\\grid4.png"));
	IDirect3DTexture9* gridTex_trap = g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev,_T("interface\\grid3.png"));
	//グリッド描画
	cDrawingObject DO;
	DO.setTexture(gridTex);

	int boxsize = MAPTEXBOXSIZE*MAPTEXPOWER;//枡の大きさ

	DO.Width = DO.Height = boxsize;
	DO.m_TexRange.setLTRB(0,0,
						1,1);
	//DO.AddingDraw = cDrawableObject::DRAW_MODE_SUBSTRACTION;


	int x,y;
	for(x = floor(visibleForcusX()) - VISIBLEBOXNUM; x < visibleForcusX() + VISIBLEBOXNUM +1;x++)
	{
		for(y = floor(visibleForcusY()) - VISIBLEBOXNUM; y < visibleForcusY() + VISIBLEBOXNUM +1;y++)
		{

			if(Land(x,y)->pOnDrop && Land(x,y)->pOnDrop->isDiscover())
			{
				DO.CenterX = (x-visibleForcusX())*boxsize + MAPDRAWCENTERX;
				DO.CenterY = (y-visibleForcusY())*boxsize + MAPDRAWCENTERY;
				if(Land(x,y)->pOnDrop->属性.count(落ち物属性::罠))
				{
					DO.m_color.ARGB(255,240,100,240);
					DO.m_color.alpha = 200*alpha;
					DO.setTexture(gridTex_trap);
					DO.Draw(pDev);
				}
				else if(Land(x,y)->pOnDrop->属性.count(落ち物属性::アイテム))
				{
					DO.m_color.ARGB(255,100,220,240);
					DO.m_color.alpha = 192*alpha;
					DO.setTexture(gridTex);
					DO.Draw(pDev);
				}

			}
			
		}
	}
	return true;
}
//地面のグリッドを書く
int cMap::DrawGridChara(IDirect3DDevice9 *pDev,const double alpha)
{
	IDirect3DTexture9* gridTex = g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev,_T("interface\\grid2.png"));
	//グリッド描画
	cDrawingObject DO;
	DO.setTexture(gridTex);

	int boxsize = MAPTEXBOXSIZE*MAPTEXPOWER;//枡の大きさ

	DO.Width = DO.Height = boxsize;
	DO.m_TexRange.setLTRB(0,0,
						1,1);
	//DO.AddingDraw = cDrawableObject::DRAW_MODE_SUBSTRACTION;

	DO.setTexture(gridTex);

	int x,y;
	for(x = floor(visibleForcusX()) - VISIBLEBOXNUM; x < visibleForcusX() + VISIBLEBOXNUM +1;x++)
	{
		for(y = floor(visibleForcusY()) - VISIBLEBOXNUM; y < visibleForcusY() + VISIBLEBOXNUM +1;y++)
		{

			if(Land(x,y)->pOnChar && sg_pDungeonSystem->キャラクター目視可(Land(x,y)->pOnChar))
			{
				DO.CenterX = (x-visibleForcusX())*boxsize + MAPDRAWCENTERX;
				DO.CenterY = (y-visibleForcusY())*boxsize + MAPDRAWCENTERY;
				if(Land(x,y)->pOnChar->Forse == CHARACTER_FORSE_FRIEND)
				{
					DO.m_color.ARGB(255,30,250,30);
					DO.m_color.alpha = 192*alpha;
				}
				else if(Land(x,y)->pOnChar->Forse == CHARACTER_FORSE_ENEMY)
				{
					DO.m_color.ARGB(255,250,30,60);
					DO.m_color.alpha = 192*alpha;

				}
				else if(Land(x,y)->pOnChar->Forse == CHARACTER_FORSE_OTHER)
				{
					DO.m_color.ARGB(255,220,220,60);
					DO.m_color.alpha = 192*alpha;

				}
				DO.Draw(pDev);
			}
			
		}
	}
	return true;
}
/*
double function_sigmoidtic(double d)
{
	return 1.0/(1.0+exp(-10*(d-0.5)));
}
*/
double function_0to1(double d)
{
	if(d > 1.0) return 1.0;
	if(d < 0.15) return 0.15;
	return d;
}
//空の飾りを描く
int cMap::DrawDecoSky(IDirect3DDevice9 *pDev)
{
	int x,y;
	const double speed = 0.08;
	const double alpha = -0.00;

	
	for(x = floor(visibleForcusX()) - VISIBLEBOXNUM -1 ; x < visibleForcusX() + VISIBLEBOXNUM +2;x++)
	{
		for(y = floor(visibleForcusY()) - VISIBLEBOXNUM -1 ; y < visibleForcusY() + VISIBLEBOXNUM +2;y++)
		{
			
			skyDecoAlpha(x,y) = 1*speed*0.4 + (1-speed*0.4)*skyDecoAlpha(x,y);
			pcLandform pland = Land(x,y);
			if(pland->pOnChar != NULL && sg_pDungeonSystem->キャラクター目視可(pland->pOnChar))
			{
				skyDecoAlpha(x,y-1) = alpha*speed + (1-speed)*skyDecoAlpha(x,y-1);
				skyDecoAlpha(x+1,y-1) = alpha*speed + (1-speed)*skyDecoAlpha(x+1,y-1);
				skyDecoAlpha(x,y) = alpha*speed + (1-speed)*skyDecoAlpha(x,y);
				skyDecoAlpha(x+1,y) = alpha*speed + (1-speed)*skyDecoAlpha(x+1,y);
				skyDecoAlpha(x,y+1) = alpha*speed + (1-speed)*skyDecoAlpha(x,y+1);
				skyDecoAlpha(x+1,y+1) = alpha*speed + (1-speed)*skyDecoAlpha(x+1,y+1);
			}
			else if(pland->pOnDrop != NULL && pland->pOnDrop->isDiscover())
			{
				skyDecoAlpha(x,y) = alpha*speed + (1-speed)*skyDecoAlpha(x,y);
				skyDecoAlpha(x+1,y) = alpha*speed + (1-speed)*skyDecoAlpha(x+1,y);
				skyDecoAlpha(x,y+1) = alpha*speed + (1-speed)*skyDecoAlpha(x,y+1);
				skyDecoAlpha(x+1,y+1) = alpha*speed + (1-speed)*skyDecoAlpha(x+1,y+1);
			}
			/*
			pcLandform pland = Land(x,y);
			if(pland->pOnChar != NULL && sg_pDungeonSystem->キャラクター目視可(pland->pOnChar))
			{
				skyDecoAlpha(x,y-1) = alpha*speed + (1-speed)*skyDecoAlpha(x,y);
				skyDecoAlpha(x+1,y-1) = alpha*speed + (1-speed)*skyDecoAlpha(x+1,y);
				skyDecoAlpha(x,y) = alpha*speed + (1-speed)*skyDecoAlpha(x,y);
				skyDecoAlpha(x+1,y) = alpha*speed + (1-speed)*skyDecoAlpha(x+1,y);
				skyDecoAlpha(x,y+1) = alpha*speed + (1-speed)*skyDecoAlpha(x,y+1);
				skyDecoAlpha(x+1,y+1) = alpha*speed + (1-speed)*skyDecoAlpha(x+1,y+1);
			}
			else if(pland->pOnDrop != NULL && pland->pOnDrop->isDiscover())
			{
				skyDecoAlpha(x,y) = alpha*speed + (1-speed)*skyDecoAlpha(x,y);
				skyDecoAlpha(x+1,y) = alpha*speed + (1-speed)*skyDecoAlpha(x+1,y);
				skyDecoAlpha(x,y+1) = alpha*speed + (1-speed)*skyDecoAlpha(x,y+1);
				skyDecoAlpha(x+1,y+1) = alpha*speed + (1-speed)*skyDecoAlpha(x+1,y+1);
			}
			*/
			//何故かバグるので使用不可
			/*
			skyDecoAlpha(x,y) = function_0to1(skyDecoAlpha(x,y)+speed/3);
		
			pcLandform pland = Land(x,y);
			if(pland->pOnChar != NULL && sg_pDungeonSystem->キャラクター目視可(pland->pOnChar))
			{
				skyDecoAlpha(x,y-1) = function_0to1(skyDecoAlpha(x,y-1)-speed);
				skyDecoAlpha(x+1,y-1) = function_0to1(skyDecoAlpha(x+1,y-1)-speed);
				skyDecoAlpha(x,y) = function_0to1(skyDecoAlpha(x,y)-speed);
				skyDecoAlpha(x+1,y) = function_0to1(skyDecoAlpha(x+1,y)-speed);
				skyDecoAlpha(x,y+1) = function_0to1(skyDecoAlpha(x,y+1)-speed);
				skyDecoAlpha(x+1,y+1) = function_0to1(skyDecoAlpha(x+1,y+1)-speed);
			}
			else if(pland->pOnDrop != NULL && pland->pOnDrop->isDiscover())
			{
				skyDecoAlpha(x,y) = function_0to1(skyDecoAlpha(x,y)-speed);
				skyDecoAlpha(x+1,y) = function_0to1(skyDecoAlpha(x+1,y)-speed);
				skyDecoAlpha(x,y+1) = function_0to1(skyDecoAlpha(x,y+1)-speed);
				skyDecoAlpha(x+1,y+1) = function_0to1(skyDecoAlpha(x+1,y+1)-speed);
			}
			*/
		}
	}

	m_DO.vm_color.resize(4);

	for(x = floor(visibleForcusX()) - VISIBLEBOXNUM; x < visibleForcusX() + VISIBLEBOXNUM +1;x++)
	{
		for(y = floor(visibleForcusY()) - VISIBLEBOXNUM; y < visibleForcusY() + VISIBLEBOXNUM +1;y++)
		{
			DrawDeco(pDev, x, y, cMap::DECORATIONLAYER_SKY);
			DrawDeco(pDev, x, y, cMap::DECORATIONLAYER_SKY2);
		}
	}

	m_DO.vm_color.clear();
	return true;
}


//暗闇描く
int cMap::DrawDark(IDirect3DDevice9 *pDev, pcCharacter hero)
{

	m_DO.AddingDraw = cDrawableObject::DRAW_MODE_NORMAL;

	if(sg_pDungeonSystem->フロア明かり状態ビジブル()) return true;

	if(abs(hero->visibleplace.x - floor(hero->visibleplace.x + 0.5)) < 0.01
		&&abs(hero->visibleplace.y - floor(hero->visibleplace.y + 0.5)) < 0.01)
	{
		pcLandform heroplace = Land(hero->visibleplace.x, hero->visibleplace.y);

		DrawDarkSub(pDev, hero, heroplace, 1.0);
	}
	else
	{
		cCoordinate ascoo;
		ascoo.SetAspect(hero->visibleaspect);

		c4DVector vTo = hero->visibleplace, vFrom = hero->visibleplace;
		//推測される行き先と
		//推測される元の位置
		if(ascoo.x != 0)
		{
			vTo.x *= ascoo.x;
			vTo.x += 1;
			vTo.x = floor(vTo.x);
			vTo.x *= ascoo.x;

			vFrom.x *= ascoo.x;
			vFrom.x = floor(vFrom.x);
			vFrom.x *= ascoo.x;
		}
		else
		{
			vTo.x = floor(vTo.x + 0.5);//四捨五入
			vFrom.x = floor(vFrom.x + 0.5);//四捨五入
		}
		
		if(ascoo.y != 0)
		{
			vTo.y *= ascoo.y;
			vTo.y += 1;
			vTo.y = floor(vTo.y);
			vTo.y *= ascoo.y;

			vFrom.y *= ascoo.y;
			vFrom.y = floor(vFrom.y);
			vFrom.y *= ascoo.y;
		}
		else
		{
			vTo.y = floor(vTo.y + 0.5);//四捨五入
			vFrom.y = floor(vFrom.y + 0.5);//四捨五入
		}



		pcLandform Toplace = Land(vTo.x, vTo.y);
		pcLandform Fromplace = Land(vFrom.x, vFrom.y);
		
		if( Toplace->RoomIndex == Fromplace->RoomIndex)
		{//部屋が同じなら
			DrawDarkSub(pDev, hero, Fromplace, 1.0);
		}
		else
		{
			c4DVector vAspect = vTo - vFrom;
			c4DVector vAspect2 = hero->visibleplace - vFrom;

			double bias = (vAspect*vAspect2/(abs(vAspect.x)+abs(vAspect.y)));

			DrawDarkSub(pDev, hero, Fromplace, 1-bias);
			DrawDarkSub(pDev, hero, Toplace, bias);
		}
	}


	return true;
}

int cMap::DrawDarkSub(IDirect3DDevice9 *pDev, pcCharacter  hero, pcLandform heroplace, double opaque)
{


	int boxsize = MAPTEXBOXSIZE*MAPTEXPOWER;//枡の大きさ
	double ver = (double)boxsize/MAPDARKTEXHEIGHT;//縦の単位
	double hor = (double)boxsize/MAPDARKTEXWIDTH;//横の単位
	cCoordinate lefttop,rightbottom;//左上
	m_DO.setTexture(m_pTexture_Dark, MAPDARKTEXWIDTH, MAPDARKTEXHEIGHT);
	m_DO.m_color.ARGB(160*opaque,255,255,255);


	int xi = 0;
	int yi = 4;
	if(heroplace->RoomIndex == -1 || sg_pDungeonSystem->pPlayerChara()->Condition.鳥目状態())
	{//通路
		int sqrlefttopx = (hero->visibleplace.x-visibleForcusX()-1)*boxsize + MAPDRAWCENTERX - boxsize;
		int sqrlefttopy = (hero->visibleplace.y-visibleForcusY()-1)*boxsize + MAPDRAWCENTERY - boxsize - boxsize/4;


		m_DO.setLTRB(sqrlefttopx,sqrlefttopy, sqrlefttopx + boxsize*4, sqrlefttopy + boxsize*4);
		m_DO.m_TexRange.setLTRB( xi*hor, yi*ver, (xi+4)*hor, (yi+4)*ver);
		m_DO.Draw(pDev);

		xi = 0;
		yi = 0;
		m_DO.m_TexRange.setLTRB( xi*hor, yi*ver, (xi+0.99)*hor, (yi+0.99)*ver);

		lefttop.x = MAPDRAWCENTERX - VISIBLEBOXNUM*boxsize - boxsize/2;
		lefttop.y = MAPDRAWCENTERY - VISIBLEBOXNUM*boxsize - boxsize/2;
		rightbottom.x = MAPDRAWCENTERX + VISIBLEBOXNUM*boxsize + boxsize/2;
		rightbottom.y = sqrlefttopy;
		m_DO.setLTRB(lefttop.x,lefttop.y, rightbottom.x, rightbottom.y);
		m_DO.Draw(pDev);

		lefttop.x = MAPDRAWCENTERX - VISIBLEBOXNUM*boxsize - boxsize/2;
		lefttop.y = sqrlefttopy;
		rightbottom.x = sqrlefttopx;
		rightbottom.y = sqrlefttopy + boxsize*4;
		m_DO.setLTRB(lefttop.x,lefttop.y, rightbottom.x, rightbottom.y);
		m_DO.Draw(pDev);

		lefttop.x = sqrlefttopx + boxsize*4;
		lefttop.y = sqrlefttopy;
		rightbottom.x = MAPDRAWCENTERX + VISIBLEBOXNUM*boxsize + boxsize/2;
		rightbottom.y = sqrlefttopy + boxsize*4;
		m_DO.setLTRB(lefttop.x,lefttop.y, rightbottom.x, rightbottom.y);
		m_DO.Draw(pDev);

		lefttop.x = MAPDRAWCENTERX - VISIBLEBOXNUM*boxsize - boxsize/2;
		lefttop.y = sqrlefttopy + boxsize*4;
		rightbottom.x = MAPDRAWCENTERX + VISIBLEBOXNUM*boxsize + boxsize/2;
		rightbottom.y = MAPDRAWCENTERY + VISIBLEBOXNUM*boxsize + boxsize/2;
		m_DO.setLTRB(lefttop.x,lefttop.y, rightbottom.x, rightbottom.y);
		m_DO.Draw(pDev);
		return true;
		
		/*
		int sqrlefttopx = (hero->visibleplace.x-visibleForcusX()-1)*boxsize + MAPDRAWCENTERX - boxsize/2;
		int sqrlefttopy = (hero->visibleplace.y-visibleForcusY()-1)*boxsize + MAPDRAWCENTERY - boxsize/2 - boxsize/4;
		
		m_DO.setLTRB(sqrlefttopx,sqrlefttopy, sqrlefttopx + boxsize*3, sqrlefttopy + boxsize*3);
		m_DO.m_TexRange.setLTRB( xi*hor, yi*ver, (xi+3)*hor, (yi+3)*ver);
		m_DO.Draw(pDev);

		xi = 0;
		yi = 0;
		m_DO.m_TexRange.setLTRB( xi*hor, yi*ver, (xi+0.99)*hor, (yi+0.99)*ver);

		lefttop.x = MAPDRAWCENTERX - VISIBLEBOXNUM*boxsize - boxsize/2;
		lefttop.y = MAPDRAWCENTERY - VISIBLEBOXNUM*boxsize - boxsize/2;
		rightbottom.x = MAPDRAWCENTERX + VISIBLEBOXNUM*boxsize + boxsize/2;
		rightbottom.y = sqrlefttopy;
		m_DO.setLTRB(lefttop.x,lefttop.y, rightbottom.x, rightbottom.y);
		m_DO.Draw(pDev);

		lefttop.x = MAPDRAWCENTERX - VISIBLEBOXNUM*boxsize - boxsize/2;
		lefttop.y = sqrlefttopy;
		rightbottom.x = sqrlefttopx;
		rightbottom.y = sqrlefttopy + boxsize*3;
		m_DO.setLTRB(lefttop.x,lefttop.y, rightbottom.x, rightbottom.y);
		m_DO.Draw(pDev);

		lefttop.x = sqrlefttopx + boxsize*3;
		lefttop.y = sqrlefttopy;
		rightbottom.x = MAPDRAWCENTERX + VISIBLEBOXNUM*boxsize + boxsize/2;
		rightbottom.y = sqrlefttopy + boxsize*3;
		m_DO.setLTRB(lefttop.x,lefttop.y, rightbottom.x, rightbottom.y);
		m_DO.Draw(pDev);

		lefttop.x = MAPDRAWCENTERX - VISIBLEBOXNUM*boxsize - boxsize/2;
		lefttop.y = sqrlefttopy + boxsize*3;
		rightbottom.x = MAPDRAWCENTERX + VISIBLEBOXNUM*boxsize + boxsize/2;
		rightbottom.y = MAPDRAWCENTERY + VISIBLEBOXNUM*boxsize + boxsize/2;
		m_DO.setLTRB(lefttop.x,lefttop.y, rightbottom.x, rightbottom.y);
		m_DO.Draw(pDev);
		return true;
		*/
	}
	

	int x,y;

	int a,b,c,d;

	int e,f;

	vector<vector<UByte>> memo(VISIBLEBOXNUM*2+5);
	/*
	for(x = visibleForcusX() - VISIBLEBOXNUM; x < visibleForcusX() + VISIBLEBOXNUM +3;x++)
	{//暗闇以外
		memo[x- (visibleForcusX() - VISIBLEBOXNUM)].resize(VISIBLEBOXNUM*2+5, 1);
	}
	*/
	for(x = 0; x < memo.size();x++)
	{//暗闇以外
		memo[x].resize(VISIBLEBOXNUM*2+5, 1);
	}
	for(x = visibleForcusX() - VISIBLEBOXNUM -1; x < visibleForcusX() + VISIBLEBOXNUM +1 +1;x++)
	{//暗闇以外
		for(y = visibleForcusY() - VISIBLEBOXNUM -1; y < visibleForcusY() + VISIBLEBOXNUM +1 +1;y++)
		{
			f = isOnSight(heroplace,Land(x,y)) && !Land(x,y)->getDarkflag();
			memo[(x+1)- (visibleForcusX() - VISIBLEBOXNUM )][(y+1)- (visibleForcusY() - VISIBLEBOXNUM )] &= f;
			memo[(x+1)- (visibleForcusX() - VISIBLEBOXNUM )+1][(y+1)- (visibleForcusY() - VISIBLEBOXNUM )] &= f;
			memo[(x+1)- (visibleForcusX() - VISIBLEBOXNUM )][(y+1)- (visibleForcusY() - VISIBLEBOXNUM )+1] &= f;
			memo[(x+1)- (visibleForcusX() - VISIBLEBOXNUM )+1][(y+1)- (visibleForcusY() - VISIBLEBOXNUM )+1] &= f;
				
		}
	}

	for(x = visibleForcusX() - VISIBLEBOXNUM; x < visibleForcusX() + VISIBLEBOXNUM +1;x++)
	{//暗闇以外
		for(y = visibleForcusY() - VISIBLEBOXNUM; y < visibleForcusY() + VISIBLEBOXNUM +1;y++)
		{
			if(!isOnDisplay(x,y)) continue;
			/*
			if(isOnSameRoom(heroplace,Land(x,y))) continue;//描画無し

			lefttop.x = (x-visibleForcusX())*boxsize + MAPDRAWCENTERX - boxsize/2;
			lefttop.y = (y-visibleForcusY())*boxsize + MAPDRAWCENTERY - boxsize/2;

			m_DO.setLTRB(lefttop.x,lefttop.y, lefttop.x + boxsize, lefttop.y + boxsize);
			
			a = (isOnSameRoom(heroplace,Land(x-1,y))||isOnSameRoom(heroplace,Land(x-1,y-1))||isOnSameRoom(heroplace,Land(x,y-1)));
			b = (isOnSameRoom(heroplace,Land(x,y-1))||isOnSameRoom(heroplace,Land(x+1,y-1))||isOnSameRoom(heroplace,Land(x+1,y)));
			c = (isOnSameRoom(heroplace,Land(x+1,y))||isOnSameRoom(heroplace,Land(x+1,y+1))||isOnSameRoom(heroplace,Land(x,y+1)));
			d = (isOnSameRoom(heroplace,Land(x,y+1))||isOnSameRoom(heroplace,Land(x-1,y+1))||isOnSameRoom(heroplace,Land(x-1,y)));
				
			*/

			lefttop.x = (x-visibleForcusX())*boxsize + MAPDRAWCENTERX - boxsize/2;
			lefttop.y = (y-visibleForcusY())*boxsize + MAPDRAWCENTERY - boxsize/2;
			lefttop.x = lefttop.x/2*2;
			lefttop.y = lefttop.y/2*2;

			m_DO.setLTRB(lefttop.x,lefttop.y, lefttop.x + boxsize, lefttop.y + boxsize);
			
			a = memo[(x+1)- (visibleForcusX() - VISIBLEBOXNUM)][(y+1)- (visibleForcusY() - VISIBLEBOXNUM)];
			b = memo[(x+1)- (visibleForcusX() - VISIBLEBOXNUM)+1][(y+1)- (visibleForcusY() - VISIBLEBOXNUM)];
			c = memo[(x+1)- (visibleForcusX() - VISIBLEBOXNUM)+1][(y+1)- (visibleForcusY() - VISIBLEBOXNUM)+1];
			d = memo[(x+1)- (visibleForcusX() - VISIBLEBOXNUM)][(y+1)- (visibleForcusY() - VISIBLEBOXNUM)+1];
			
			int xi = a+(b<<1);
			int yi = d+(c<<1);

			m_DO.m_TexRange.setLTRB( xi*hor, yi*ver, (xi+1)*hor, (yi+1)*ver);
			m_DO.Draw(pDev);
		}
	}
	return true;
}
int cMap::DrawDeco(IDirect3DDevice9 *pDev, const int x, const int y, const int decoLayer)
{
	dChipTexPos pos = Land(x,y)->getDecorationchip(decoLayer);
	if(pos == -1) return false;

	m_DO.setTexture(m_pTexture_Decochip);
	m_DO.m_color.ARGB(255,255,255,255);

	if(decoLayer == cMap::DECORATIONLAYER_SKY || decoLayer == cMap::DECORATIONLAYER_SKY2 )
	{
		m_DO.vm_color[0].ARGB(skyDecoAlpha(x,y)*255,255,255,255);
		m_DO.vm_color[1].ARGB(skyDecoAlpha(x+1,y)*255,255,255,255);
		m_DO.vm_color[2].ARGB(skyDecoAlpha(x,y+1)*255,255,255,255);
		m_DO.vm_color[3].ARGB(skyDecoAlpha(x+1,y+1)*255,255,255,255);
	}
	else
	{
		m_DO.m_color.ARGB(255,255,255,255);
	}

	int boxsize = MAPTEXBOXSIZE*MAPTEXPOWER;//枡の大きさ
	cCoordinate lefttop;//左上
	lefttop.x = (x-visibleForcusX())*boxsize + MAPDRAWCENTERX - boxsize/2;
	lefttop.y = (y-visibleForcusY())*boxsize + MAPDRAWCENTERY - boxsize/2;

	double ver = (double)MAPTEXBOXSIZE/MAPTEXHEIGHT;//縦の単位
	double hor = (double)MAPTEXBOXSIZE/MAPTEXWIDTH;//横の単位


	int u = (pos & 0xF0)>>4;
	int v = pos & 0x0F;

	m_DO.m_TexRange.setLTRB(u*ver, v*hor,
		(u+1)*ver, (v+1)*hor);

	m_DO.AddingDraw = (cDrawableObject::DrawMode)Land(x,y)->getDecorationDrawMode(decoLayer);

	m_DO.setLTRB(lefttop.x, 		lefttop.y,
		lefttop.x + boxsize, lefttop.y + boxsize);

	m_DO.Draw(pDev);

	return true;
}
//画面上にある
bool cMap::isOnDisplay(double x, double y, int margin)
{

	return (x - VISIBLEBOXNUM -1 -margin < visibleForcusX() && visibleForcusX() < x + VISIBLEBOXNUM +1 +margin  &&
			y - VISIBLEBOXNUM -1 -margin < visibleForcusY() && visibleForcusY() < y + VISIBLEBOXNUM +1 +margin
			);
}

double& cMap::additinal_visibleForcusX()
{
	return additinal_visibleForcusX_;
}
double& cMap::additinal_visibleForcusY()
{
	return additinal_visibleForcusY_;
}

const double cMap::visibleForcusX()
{
	return visibleForcusX_ + additinal_visibleForcusX_;
}
const double cMap::visibleForcusY()
{
	return visibleForcusY_ + additinal_visibleForcusY_;
}
void cMap::setVisibleForcusX(double x)
{
	visibleForcusX_ = x;
}
void cMap::setVisibleForcusY(double y)
{
	visibleForcusY_ = y;
}
double cMap::ForcusX()
{
	return sg_pDungeonSystem->pPlayerChara()->placeX;
}
double cMap::ForcusY()
{
	return sg_pDungeonSystem->pPlayerChara()->placeY;
}

//同じ部屋か
bool cMap::isOnSameRoom(pcLandform la, pcLandform lb)
{//通路の場合同じ地形か
	if(la->RoomIndex == -1)
	{
		return (la == lb);
	}
	else
	{
		return (la->RoomIndex == lb->RoomIndex);
	}
}
//同じ部屋もしくは出口または隣接
bool cMap::isOnSameRoomOrExit(pcLandform from, pcLandform to)
{//通路の場合同じ地形か
	cCoordinate aspect;
	int i;
	for(i=0;i<8;i++)
	{
		aspect.SetAspect(i);
		if(isOnSameRoom(from,Land(to->place+aspect)))
		{
			return true;
		}
	}

	if(isOnSameRoom(from,to))
	{
		return true;
	}
	return false;
}
bool cMap::isOnSight(pcLandform from, pcLandform to, int sightrange, int torime)
{
	int i;
	cCoordinate aspect,fromplace,toplace;
	toplace = to->place;
	fromplace = from->place;

	if((toplace-fromplace).dif() <= sightrange) return true;

	if(torime) return false;

	if(to->getDarkflag()) return false;

	for(i=0;i<8;i++)
	{
		aspect.SetAspect(i);
		if(isOnSameRoom(from,Land(toplace+aspect)))
		{
			return true;
		}
	}

	if(isOnSameRoom(from,Land(toplace)))
	{
		return true;
	}
	return false;
}

c4DVector cMap::visibleplace(c4DVector place)
{
	int boxsize = MAPTEXBOXSIZE*MAPTEXPOWER;//枡の大きさ

	return c4DVector((place.x-visibleForcusX())*boxsize + MAPDRAWCENTERX,
		   (place.y-visibleForcusY())*boxsize + MAPDRAWCENTERY - place.z*boxsize/2,
		   0,0);
}
//画面上主人公の移動を考慮した実際の位置
c4DVector cMap::realvisibleplace(c4DVector place)
{
	int boxsize = MAPTEXBOXSIZE*MAPTEXPOWER;//枡の大きさ

	return c4DVector((place.x-visibleForcusX())*boxsize + MAPDRAWCENTERX,
		   (place.y-visibleForcusY())*boxsize + MAPDRAWCENTERY - place.z*boxsize/2,
		   0,0);
}

//チップを一マス描く
int cMap::Drawchip(IDirect3DDevice9 *pDev, const int x, const int y)
{
	//if(!isOnDisplay(x,y)) return false;

	m_DO.setTexture(m_pTexture_Mapchip, MAPTEXWIDTH, MAPTEXHEIGHT);
	m_DO.m_color.ARGB(255,255,255,255);

	int boxsize = MAPTEXBOXSIZE*MAPTEXPOWER;//枡の大きさ
	cCoordinate lefttop;//左上
	lefttop.x = (x-visibleForcusX())*boxsize + MAPDRAWCENTERX - boxsize/2;
	lefttop.y = (y-visibleForcusY())*boxsize + MAPDRAWCENTERY - boxsize/2;

	double ver = (double)MAPTEXBOXSIZE/MAPTEXHEIGHT;//縦の単位
	double hor = (double)MAPTEXBOXSIZE/MAPTEXWIDTH;//横の単位

	if(Land(x,y)->kind_surface_visible() == MAPKIND_FLOOR)
	{	//床
		double versep = (double)16/MAPTEXBOXSIZE;
		double horsep = (double)16/MAPTEXBOXSIZE;
		int baseX = mapChipAnimeAll->getFrame(cMapChipAnimeAll::MAPCHIP_ROOM)*4,
			baseY = cMapChipAnimeAll::MAPCHIP_ROOM*2;

		DrawchipSub(pDev,lefttop,
		x,y,
		baseX,baseY,
		MAPKIND_FLOOR,-1,
		versep,horsep);
	}
	if(Land(x,y)->kind_surface_visible() == MAPKIND_FLOOR2)
	{	//通路床
		double versep = (double)16/MAPTEXBOXSIZE;
		double horsep = (double)16/MAPTEXBOXSIZE;
		int baseX = mapChipAnimeAll->getFrame(cMapChipAnimeAll::MAPCHIP_AISLE)*4,
			baseY = cMapChipAnimeAll::MAPCHIP_AISLE*2;

		DrawchipSub(pDev,lefttop,
		x,y,
		baseX,baseY,
		MAPKIND_FLOOR2,-1,
		versep,horsep);

	}
	if(Land(x,y)->kind_surface_visible() == MAPKIND_WATER)
	{	//水
		double versep = (double)16/MAPTEXBOXSIZE;
		double horsep = (double)16/MAPTEXBOXSIZE;
		int baseX = mapChipAnimeAll->getFrame(cMapChipAnimeAll::MAPCHIP_WATER)*4,
			baseY = cMapChipAnimeAll::MAPCHIP_WATER*2;

		DrawchipSub(pDev,lefttop,
		x,y,
		baseX,baseY,
		MAPKIND_WATER,-1,
		versep,horsep);

	}
	if(Land(x,y)->kind_surface_visible() == MAPKIND_WALL)
	{	//壁
		double versep = (double)MAPTEXVERSEP/MAPTEXBOXSIZE;
		double horsep = (double)MAPTEXHORSEP/MAPTEXBOXSIZE;
		int baseX = mapChipAnimeAll->getFrame(cMapChipAnimeAll::MAPCHIP_WALL)*4,
			baseY = cMapChipAnimeAll::MAPCHIP_WALL*2;

		DrawchipSub(pDev,lefttop,
		x,y,
		baseX,baseY,
		MAPKIND_WALL,MAPKIND_WALLEGDE,
		versep,horsep);

	}

	return true;
}
int cMap::DrawchipSub(IDirect3DDevice9 *pDev,cCoordinate lefttop,//描画位置
		const int x,const int y,//マップ座標
		const int baseXi,const int baseYi,//テクスチャ座標基礎
		const int first_land,const int second_land,//テクスチャ座標基礎
		const double versep,const double horsep)//テクスチャ座標分割数
{
	//double versep = (double)MAPTEXVERSEP/MAPTEXBOXSIZE;
	//double horsep = (double)MAPTEXHORSEP/MAPTEXBOXSIZE;
	int key1,key2,key3;
	int kind1,kind2,kind3;
	int xi, yi;


	int boxsize = MAPTEXBOXSIZE*MAPTEXPOWER;//枡の大きさ

	double ver = (double)MAPTEXBOXSIZE/MAPTEXHEIGHT;//縦の単位
	double hor = (double)MAPTEXBOXSIZE/MAPTEXWIDTH;//横の単位


	//左上
	kind1 = Land(x,y)->visibleneighbor(ASPECT_LEFT);
	kind2 = Land(x,y)->visibleneighbor(ASPECT_UP);
	kind3 = Land(x,y)->visibleneighbor(ASPECT_UPLEFT);
	key1 = (kind1 == first_land);
	key2 = (kind2 == first_land);
	key3 = (kind3 == first_land);
	key1 |= (kind1 == second_land);
	key2 |= (kind2 == second_land);
	key3 |= (kind3 == second_land);


	if((key1&&key2) && key3)
	{
		xi = 2 + baseXi;
		yi = 0 + baseYi;
	}
	else
	{
		xi = key1 + baseXi;
		yi = key2 + baseYi;
	}
	m_DO.setLTRB(lefttop.x, 		lefttop.y,
		lefttop.x + boxsize*horsep, lefttop.y + boxsize*versep);

	m_DO.m_TexRange.setLTRB( xi*hor,		yi*ver,
							(xi+horsep)*hor, (yi+versep)*ver);
	m_DO.Draw(pDev);

	//右上
	kind1 = Land(x,y)->visibleneighbor(ASPECT_RIGHT);
	kind2 = Land(x,y)->visibleneighbor(ASPECT_UP);
	kind3 = Land(x,y)->visibleneighbor(ASPECT_UPRIGHT);
	key1 = (kind1 == first_land);
	key2 = (kind2 == first_land);
	key3 = (kind3 == first_land);
	key1 |= (kind1 == second_land);
	key2 |= (kind2 == second_land);
	key3 |= (kind3 == second_land);

	if((key1&&key2) && key3)
	{
		xi = 2 + baseXi;
		yi = 0 + baseYi;
	}
	else
	{
		xi = !key1 + baseXi;
		yi = key2 + baseYi;
	}

	m_DO.setLTRB(lefttop.x + boxsize*horsep, lefttop.y,
		lefttop.x + boxsize, lefttop.y + boxsize*versep);

	m_DO.m_TexRange.setLTRB( (xi+horsep)*hor,		yi*ver, 
							(xi+1)*hor, (yi+versep)*ver);
	m_DO.Draw(pDev);

	//左下
	kind1 = Land(x,y)->visibleneighbor(ASPECT_LEFT);
	kind2 = Land(x,y)->visibleneighbor(ASPECT_DOWN);
	kind3 = Land(x,y)->visibleneighbor(ASPECT_DOWNLEFT);
	key1 = (kind1 == first_land);
	key2 = (kind2 == first_land);
	key3 = (kind3 == first_land);
	key1 |= (kind1 == second_land);
	key2 |= (kind2 == second_land);
	key3 |= (kind3 == second_land);

	if((key1&&key2) && key3)
	{
		xi = 2 + baseXi;
		yi = 0 + baseYi;
	}
	else
	{
		xi = key1 + baseXi;
		yi = !key2 + baseYi;
	}

	m_DO.setLTRB(lefttop.x, 		lefttop.y + boxsize*versep,
		lefttop.x + boxsize*horsep, lefttop.y + boxsize);

	m_DO.m_TexRange.setLTRB( xi*hor,		(yi+versep)*ver,
							(xi+horsep)*hor, (yi+1)*ver);
	m_DO.Draw(pDev);

	//右下
	kind1 = Land(x,y)->visibleneighbor(ASPECT_RIGHT);
	kind2 = Land(x,y)->visibleneighbor(ASPECT_DOWN);
	kind3 = Land(x,y)->visibleneighbor(ASPECT_DOWNRIGHT);
	key1 = (kind1 == first_land);
	key2 = (kind2 == first_land);
	key3 = (kind3 == first_land);
	key1 |= (kind1 == second_land);
	key2 |= (kind2 == second_land);
	key3 |= (kind3 == second_land);

	if((key1&&key2) && key3)
	{
		xi = 2 + baseXi;
		yi = 0 + baseYi;
	}
	else
	{
		xi = !key1 + baseXi;
		yi = !key2 + baseYi;
	}
	m_DO.setLTRB(lefttop.x + boxsize*horsep, lefttop.y + boxsize*versep,
		lefttop.x + boxsize, lefttop.y + boxsize);

	m_DO.m_TexRange.setLTRB( (xi+horsep)*hor,(yi+versep)*ver, 
							(xi+1)*hor, (yi+1)*ver);
	m_DO.Draw(pDev);

	return true;
}

//ミニマップを描く
int cMap::DrawMiniMap(IDirect3DDevice9 *pDev)
{
	return Minimap.DrawMiniMap(pDev);
}


//マッピング
int cMap::mapping_just(const int x, const int y)
{
	return Minimap.mapping_just(x,y);
}
//マッピング
int cMap::mapping(const int x, const int y)
{
	return Minimap.mapping(x,y);
}

//全マッピングする
int cMap::Allmapping()
{
	return Minimap.Allmapping();
}
//ミニマップを消す
int cMap::EraseMiniMap(int FocusX, int FocusY)
{
	return Minimap.EraseMiniMap(FocusX,FocusY);
}

//ミニマップを書き直すフラグをたてる。
int cMap::ResetMiniMap()
{
	return Minimap.ResetMiniMap();
}
	//ミニマップを書き直すフラグをたてる。
int cMap::RefreshMiniMap()
{
	return Minimap.RefreshMiniMap();
}
	

//指定位置のみマッピングする
int cMap::pointmapping(const int x, const int y)
{
	return Minimap.pointmapping(x,y);
}

//マップの表示をリセット（主人公の周りだけ）
void cMap::MapvisibleReset()
{
	int x,y;

	for(x = visibleForcusX() - VISIBLEBOXNUM; x < visibleForcusX() + VISIBLEBOXNUM +1;x++)
	{
		for(y = visibleForcusY() - VISIBLEBOXNUM; y < visibleForcusY() + VISIBLEBOXNUM +1;y++)
		{
			Land(x, y)->visibleReset();
		}
	}
}

//マップの表示をリセット
void cMap::AllMapvisibleReset()
{
	int x,y;
	for(y=0;y<MAPHEIGHT;y++)
	{
		for(x=0;x<MAPWIDTH;x++)
		{
			Land(x, y)->visibleReset();
		}
	}
	EdgeLand->visibleReset();

	additinal_visibleForcusX_ = 0;
	additinal_visibleForcusY_ = 0;
}

//隣接マスデータ格納（全マス）
int cMap::resetAllLandNeighbor()
{
	int x,y;
	for(x=0;x<MAPWIDTH;x++)
	{
		for(y=0;y<MAPHEIGHT;y++)
		{
			resetLandNeighbor(x,y);
		}
	}
	resetLandNeighbor(-1,-1);
	return true;
}

//隣接マスデータ格納
int cMap::resetLandNeighbor(const int x,const int y)
{
	/*不要
	int a;
	cCoordinate place,aspect;

	place.x = x;
	place.y = y;

	for(a=0;a<8;a++)
	{
		aspect.SetAspect(a);
		aspect += place;

		Land(place)[a] = Land(aspect)->getAttribute();
	}

	*/
	return true;
}