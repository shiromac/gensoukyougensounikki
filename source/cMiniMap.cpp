#include "stdafx.h"


#include "cMiniMap.h"

#include "utility/debug.h"

#include "cLandform.h"

#define MINIMAPSIZE 8

#define MINIMAPTRANS 140
#define MINIMAPTRANS2 192



#define MINIMAPTEXNAME _T("map\\minimapchip.png")

#define MINIMAPTEXLANDNAME _T("map\\minimapchip2.png")

#define MINIMAPTEXHEIGHT 64
#define MINIMAPTEXWIDTH 32


#define MINIMAPDRAWCENTERX (366)
#define MINIMAPDRAWCENTERY (260)

cMiniMap::cMiniMap(void)
{
	RefreshMiniMapFlag = true;
	ClearMiniMapFlag = false;
	MiniMapdrawed_list.assign(MAPWIDTH * MAPHEIGHT, false);
	MiniMapMapped_list_store.assign(MAPWIDTH * MAPHEIGHT, false);
	ViewFlag = true;
}

cMiniMap::~cMiniMap(void)
{

}

void cMiniMap::Init(IDirect3DDevice9 *pDev)
{
	

	ResetMiniMap();

	//ミニマップ範囲初期化
	MiniMapRange.setLTRB(0,0,0,0);
	m_pTexture_miniMapchip = g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev,MINIMAPTEXNAME);
	
	m_pTexture_miniMapchip_land =  g_GameEnv.m_GlobalResourse->getTextureFromFile(pDev,MINIMAPTEXLANDNAME);

	
}

pcLandform cMiniMap::Land(const int x,const int y)
{
	return sg_pDungeonSystem->Map().Land(x,y);
}

pcLandform cMiniMap::Land(const cCoordinate& cood)
{
	return sg_pDungeonSystem->Map().Land(cood);
}


int cMiniMap::DrawMiniMap(IDirect3DDevice9 *pDev)
{
	int i;




	if(RefreshMiniMapFlag)
	{
		ReDrawMiniMap(pDev);

		//地形描画
		DrawMiniMap_Land(pDev);

		RefreshMiniMapFlag = false;
	}


	if(!ViewFlag)
	{
		//ターゲットをバックに
		g_GameEnv.m_Screen->setRenderTarget(pDev,SCREEN_BACKGROUND);

		return true;
	}



	//ターゲットをバックに
	g_GameEnv.m_Screen->setRenderTarget(pDev,SCREEN_BACKGROUND);


	
	
	//案内
	if(sg_pDungeonSystem->s_AltaofminimapViewprocess().on)
	{



		//バックを暗く
		cDrawingObject DO;
		DO.setTexture(NULL,0,0);
		DO.m_color.ARGB(188,0,0,0);
		/*
		DO.CenterX = MAPDRAWCENTERX;
		DO.CenterY = MAPDRAWCENTERY;
		DO.Width = MAPTEXBOXSIZE*MAPTEXPOWER*9;
		DO.Height = MAPTEXBOXSIZE*MAPTEXPOWER*9;
		*/
		DO.CenterX = SCREEN_X/2;
		DO.CenterY = SCREEN_Y/2;
		DO.Width = SCREEN_X;
		DO.Height = SCREEN_Y;
		DO.Draw(pDev);


	}

	


	//ターゲットをバックに
	g_GameEnv.m_Screen->setRenderTarget(pDev,SCREEN_BACKGROUND);

	cDrawingObject screenDO;
	screenDO.setTexture((g_GameEnv.m_Screen->m_pLayerTexture),g_GameEnv.m_Screen->TEXTURESIZE,g_GameEnv.m_Screen->TEXTURESIZE);
	screenDO.m_TexRange.setLTRB(0,0,MINIMAPSIZE*(double)(MAPWIDTH +1)/g_GameEnv.m_Screen->TEXTURESIZE, MINIMAPSIZE*(double)(MAPHEIGHT +1)/g_GameEnv.m_Screen->TEXTURESIZE);
	screenDO.Width = MINIMAPSIZE*(MAPWIDTH +1);
	screenDO.Height = MINIMAPSIZE*(MAPHEIGHT +1);
	screenDO.setLeft( (-MiniMapRange.Left())*MINIMAPSIZE + MINIMAPDRAWCENTERX - MINIMAPSIZE*MAPWIDTH/2 - MINIMAPSIZE/2);
	screenDO.setTop( (-MiniMapRange.CenterY)*MINIMAPSIZE + MINIMAPDRAWCENTERY /*- MINIMAPSIZE*MAPHEIGHT/2*/ - MINIMAPSIZE/2);
	screenDO.Draw(pDev);
		
	//オブジェクト描画
	DrawMiniMap_Object(pDev);

	//ガイド系
	cCoordinate aspectco, place, now;
	int aspect;
	aspect = sg_pDungeonSystem->pPlayerChara()->aspect;
	aspectco.SetAspect(aspect);
	now.x = sg_pDungeonSystem->pPlayerChara()->placeX;
	now.y = sg_pDungeonSystem->pPlayerChara()->placeY;

	if(sg_pDungeonSystem->isInputWaiting())
	{
		MiniMapDrawSub(pDev,now.x,now.y,3*(sg_pDungeonSystem->count_minimap()/30),0);
		int aspect = safeAspect(sg_pDungeonSystem->pPlayerChara()->aspect);
		MiniMapDrawSub(pDev,now.x,now.y,aspect % 4, aspect/4 + 6);	
	}


	//案内
	if(sg_pDungeonSystem->s_AltaofminimapViewprocess().on)
	{

		
		pcLandform pland;

		place = now;
		for(i=1;i<=32;i++)
		{
			aspectco.SetAspect(aspect);
			place = place + aspectco;
			if(sg_pDungeonSystem->Map().Land(place)->iswall() && MiniMapMapped_store_edge(place,safeAspect(aspect-4 +!(aspect % 2))))
			{
				if(aspect % 2)
				{
					if(sg_pDungeonSystem->壁反射判定且検索(pland,aspect,sg_pDungeonSystem->Map().Land(place),aspect))
					{
						place = pland->place;
						aspectco.SetAspect(aspect);
						place = place + aspectco;
					}
					else
					{
						break;
					}
				}
				else
				{
					if(sg_pDungeonSystem->壁曲がり判定且検索(pland,aspect,sg_pDungeonSystem->Map().Land(place),aspect))
					{
						place = pland->place;
						aspectco.SetAspect(aspect);
						place = place + aspectco;
					}
					else
					{
						break;
					}
				}
			}
			

			if(i == 10)
				MiniMapDrawSub(pDev,place.x,place.y,3,4);
			else
				MiniMapDrawSub(pDev,place.x,place.y,2,4);
		}

		aspect = sg_pDungeonSystem->pPlayerChara()->aspect;
		aspectco.SetAspect(aspect);
		now.x = sg_pDungeonSystem->pPlayerChara()->placeX;
		now.y = sg_pDungeonSystem->pPlayerChara()->placeY;

		for(i=1;i<=32;i++)
		{
			place = now + aspectco*i;
			if(i == 10)
				MiniMapDrawSub(pDev,place.x,place.y,1,3);
			else
				MiniMapDrawSub(pDev,place.x,place.y,0,3);
		}


		MiniMapDrawSub(pDev,now.x,now.y,3*(sg_pDungeonSystem->count_minimap()/30),0);	

	}

	//ターゲットをバックに
	g_GameEnv.m_Screen->setRenderTarget(pDev,SCREEN_BACKGROUND);


	return true;
}

int cMiniMap::ReDrawMiniMap(IDirect3DDevice9 *pDev)
{

	cColor c;
	c.ARGB(0,0,0,0);


	//ターゲットをレイヤに
	g_GameEnv.m_Screen->setRenderTarget(pDev,SCREEN_LAYER);

	if(ClearMiniMapFlag)
	{
		g_GameEnv.m_Screen->Clear(pDev,c);
		ClearMiniMapFlag = false;
	}


	return true;
}


int cMiniMap::DrawMiniMap_Land(IDirect3DDevice9 *pDev)
{
	//ターゲットをレイヤに
	g_GameEnv.m_Screen->setRenderTarget(pDev,SCREEN_LAYER);

	m_DO.setTexture(m_pTexture_miniMapchip_land);


	//不透明度
	int trans,trans2;
	if(g_pPlayerInput()->miniMap().on)
	{
		trans = 255;
		trans2 = 255;
	}
	else
	{
		trans = MINIMAPTRANS*sqrt(sg_pDungeonSystem->EV_mapThickness/6.0);
		trans2 = MINIMAPTRANS2*sqrt(sg_pDungeonSystem->EV_mapThickness/6.0);
	}
	
	int boxsize = MINIMAPSIZE;//枡の大きさ
	cCoordinate lefttop;//左上
	
	int x,y;
	for(x = 0; x < MAPWIDTH;x++)
	{
		for(y = 0; y < MAPHEIGHT;y++)
		{
			lefttop.x = (x)*boxsize + boxsize/2;
			lefttop.y = (y)*boxsize + boxsize/2;
			//lefttop.x = (x-MiniMapRange.CenterX)*boxsize + MAPDRAWCENTERX - boxsize/2;
			//lefttop.y = (y-MiniMapRange.CenterY)*boxsize + MAPDRAWCENTERY - boxsize/2;
			pcLandform land = Land(x,y);		

			int setflag = 0;
			if(land->kind_surface() == MAPKIND_FLOOR)
			{	//床
				if((land->mapped & MAPPED_SEE) && !(MiniMapdrawed(x,y) & MAPPED_SEE) && MiniMapMapped_store(x,y))
				{

					m_DO.setLTRB(lefttop.x,lefttop.y, lefttop.x + boxsize, lefttop.y + boxsize);
					int vi = 0, ui = 2;
					double ver = (double)MINIMAPSIZE/MINIMAPTEXHEIGHT;//縦の単位
					double hor = (double)MINIMAPSIZE/MINIMAPTEXWIDTH;//横の単位
					m_DO.m_TexRange.setLTRB( (ui)*hor,		(vi)*ver, 
								(ui+1)*hor, (vi+1)*ver);
					m_DO.m_color.ARGB(trans*0.85,20,60,170);
					m_DO.Draw(pDev);
					
					setflag |= MAPPED_SEE;
				}
				if((land->mapped & MAPPED_JUSTCHECK) && !(MiniMapdrawed(x,y) & MAPPED_JUSTCHECK) && MiniMapMapped_store(x,y))
				{

					m_DO.setLTRB(lefttop.x,lefttop.y, lefttop.x + boxsize, lefttop.y + boxsize);
					int vi = 1, ui = 2;
					double ver = (double)MINIMAPSIZE/MINIMAPTEXHEIGHT;//縦の単位
					double hor = (double)MINIMAPSIZE/MINIMAPTEXWIDTH;//横の単位
					m_DO.m_TexRange.setLTRB( (ui)*hor,		(vi)*ver, 
								(ui+1)*hor, (vi+1)*ver);
					m_DO.m_color.ARGB(trans*0.9,30,80,200);
					m_DO.Draw(pDev);
					
					setflag |= MAPPED_JUSTCHECK;
				}
			}

			if((land->mapped & MAPPED_SEE) && !MiniMapdrawed(x,y) && MiniMapMapped_store(x,y))
			{

				setflag |= 1;

				if(land->kind_surface() == MAPKIND_FLOOR2)
				{	//床通路

					m_DO.setLTRB(lefttop.x,lefttop.y, lefttop.x + boxsize, lefttop.y + boxsize);
					int vi = 0, ui = 3;
					double ver = (double)MINIMAPSIZE/MINIMAPTEXHEIGHT;//縦の単位
					double hor = (double)MINIMAPSIZE/MINIMAPTEXWIDTH;//横の単位
					m_DO.m_TexRange.setLTRB( (ui)*hor,		vi*ver, 
								(ui+1)*hor, (vi+1)*ver);
					m_DO.m_color.ARGB(trans,90,00,210);
					m_DO.Draw(pDev);

				}
				else if(land->kind_surface() == MAPKIND_WATER)
				{	//水
					m_DO.m_color.ARGB(trans2,70,240,255);


					int a;
					for(a=1;a<8;a+=2)
					{
						if(!MiniMapdrawed_edge(x,y,a))
						{//まだかかれてない
							DrawMiniMap_Land_Sub(pDev,lefttop,
								x,y,
								0,2,
								MAPKIND_WATER,-1,
								0.5,0.5,
								a);
						}
					}

				}
				
				else if(land->kind_surface() == MAPKIND_WALL)
				{	//壁
					m_DO.m_color.ARGB(trans2,230,230,230);
				
					int a;
					for(a=1;a<8;a+=2)
					{
						if(!MiniMapdrawed_edge(x,y,a))
						{
							DrawMiniMap_Land_Sub(pDev,lefttop,
								x,y,
								0,0,
								MAPKIND_WALL,MAPKIND_WALLEGDE,
								0.5,0.5,
								a);
						}
					}

				}

				else if(land->kind_surface() == MAPKIND_ABYSS)
				{	//奈落
					m_DO.m_color.ARGB(trans2,190,190,255);
				
					int a;
					for(a=1;a<8;a+=2)
					{
						if(!MiniMapdrawed_edge(x,y,a))
						{
							DrawMiniMap_Land_Sub(pDev,lefttop,
								x,y,
								0,2,
								MAPKIND_ABYSS,-1,
								0.5,0.5,
								a);
						}
					}

				}
				

				int a;
				for(a=0;a<8;a++)
				{
					cCoordinate coo;
					coo.SetAspect(a);
					coo.x += x;
					coo.y += y;
					pcLandform edgeland = Land(coo);

					lefttop.x = (coo.x)*boxsize + boxsize/2;
					lefttop.y = (coo.y)*boxsize + boxsize/2;

					if(edgeland->kind_surface() == MAPKIND_WALL)
					{
						m_DO.m_color.ARGB(trans2,230,230,230);
						
						if(a % 2 == 0)
						{//斜めではない
							int as;
							as = safeAspect(a+4+1);
							if(!MiniMapdrawed_edge(coo.x,coo.y,as) && MiniMapMapped_store_edge(coo.x,coo.y,as))
							{
								DrawMiniMap_Land_Sub(pDev,lefttop,
									coo.x,coo.y,
									0,0,
									MAPKIND_WALL,MAPKIND_WALLEGDE,
									0.5,0.5,
									as);		
							}
							as = safeAspect(a+4-1);
							if(!MiniMapdrawed_edge(coo.x,coo.y,as) && MiniMapMapped_store_edge(coo.x,coo.y,as))
							{
								DrawMiniMap_Land_Sub(pDev,lefttop,
									coo.x,coo.y,
									0,0,
									MAPKIND_WALL,MAPKIND_WALLEGDE,
									0.5,0.5,
									as);		
							}
						}
						else
						{
							int as;
							as = safeAspect(a+4);
							if(!MiniMapdrawed_edge(coo.x,coo.y,as) && MiniMapMapped_store_edge(coo.x,coo.y,as))
							{
								DrawMiniMap_Land_Sub(pDev,lefttop,
									coo.x,coo.y,
									0,0,
									MAPKIND_WALL,MAPKIND_WALLEGDE,
									0.5,0.5,
									as);		
							}
						}
					}
					else if(edgeland->kind_surface() == MAPKIND_WATER)
					{	//水
						m_DO.m_color.ARGB(trans2,70,240,255);


						if(a % 2 == 0)
						{//斜めではない
							int as;
							as = safeAspect(a+4+1);
							if(!MiniMapdrawed_edge(coo.x,coo.y,as) && MiniMapMapped_store_edge(coo.x,coo.y,as))
							{
								DrawMiniMap_Land_Sub(pDev,lefttop,
									coo.x,coo.y,
									0,2,
									MAPKIND_WATER,-1,
									0.5,0.5,
									as);		
							}
							as = safeAspect(a+4-1);
							if(!MiniMapdrawed_edge(coo.x,coo.y,as) && MiniMapMapped_store_edge(coo.x,coo.y,as))
							{
								DrawMiniMap_Land_Sub(pDev,lefttop,
									coo.x,coo.y,
									0,2,
									MAPKIND_WATER,-1,
									0.5,0.5,
									as);		
							}
						}
						else
						{
							int as;
							as = safeAspect(a+4);
							if(!MiniMapdrawed_edge(coo.x,coo.y,as) && MiniMapMapped_store_edge(coo.x,coo.y,as))
							{
								DrawMiniMap_Land_Sub(pDev,lefttop,
									coo.x,coo.y,
									0,2,
									MAPKIND_WATER,-1,
									0.5,0.5,
									as);		
							}
						}
						//MiniMapdrawed(x,y) = false;
							
					}
					else if(edgeland->kind_surface() == MAPKIND_ABYSS)
					{	//奈落
						m_DO.m_color.ARGB(trans2,190,190,255);


						if(a % 2 == 0)
						{//斜めではない
							int as;
							as = safeAspect(a+4+1);
							if(!MiniMapdrawed_edge(coo.x,coo.y,as) && MiniMapMapped_store_edge(coo.x,coo.y,as))
							{
								DrawMiniMap_Land_Sub(pDev,lefttop,
									coo.x,coo.y,
									0,2,
									MAPKIND_ABYSS,-1,
									0.5,0.5,
									as);		
							}
							as = safeAspect(a+4-1);
							if(!MiniMapdrawed_edge(coo.x,coo.y,as) && MiniMapMapped_store_edge(coo.x,coo.y,as))
							{
								DrawMiniMap_Land_Sub(pDev,lefttop,
									coo.x,coo.y,
									0,2,
									MAPKIND_ABYSS,-1,
									0.5,0.5,
									as);		
							}
						}
						else
						{
							int as;
							as = safeAspect(a+4);
							if(!MiniMapdrawed_edge(coo.x,coo.y,as) && MiniMapMapped_store_edge(coo.x,coo.y,as))
							{
								DrawMiniMap_Land_Sub(pDev,lefttop,
									coo.x,coo.y,
									0,2,
									MAPKIND_ABYSS,-1,
									0.5,0.5,
									as);		
							}
						}
						//MiniMapdrawed(x,y) = false;
							
					}
				}


			}

			MiniMapdrawed(x,y) |= setflag;
			
			/*
			else if(land->kind_surface() == MAPKIND_WALL)
			{	//壁
				m_DO.m_color.ARGB(trans2,230,230,230);
			
				int a;
				for(a=1;a<8;a+=2)
				{
					if(!MiniMapdrawed_edge(x,y,a) && MiniMapMapped_store_edge(x,y,a))
					{
						DrawMiniMap_Land_Sub(pDev,lefttop,
							x,y,
							0,0,
							MAPKIND_WALL,MAPKIND_WALLEGDE,
							0.5,0.5,
							a);

						
					}
				}

				//MiniMapdrawed(x,y) = false;
				
			}
			else if(land->kind_surface() == MAPKIND_WATER)
			{	//水
				m_DO.m_color.ARGB(trans2,70,240,255);


				int a;
				for(a=1;a<8;a+=2)
				{
					if(!MiniMapdrawed_edge(x,y,a) && MiniMapMapped_store_edge(x,y,a))
					{
						DrawMiniMap_Land_Sub(pDev,lefttop,
							x,y,
							0,2,
							MAPKIND_WATER,-1,
							0.5,0.5,
							a);

						
					}
				}
				//MiniMapdrawed(x,y) = false;
				
			}
			
				*/


			/*
			int a;
			for(a=0;a<8;a++)ffffffffffff
			{
				cCoordinate coo;
				coo.SetAspect(a);
				int newx = x + coo.x;
				int newy = y + coo.y;

				int aspect2 = 4;
				if((a+aspect2) % 2 != 1) aspect2 = 3;
				if(!MiniMapdrawed_edge(newx,newy,a+aspect2) && MiniMapMapped_store_edge(newx,newy,a+aspect2))
				{

					lefttop.x = (newx)*boxsize + boxsize/2;
					lefttop.y = (newy)*boxsize + boxsize/2;

					if(land->kind_surface() == MAPKIND_WALL)
					{	//壁
						m_DO.m_color.ARGB(trans2,255,255,255);
						DrawMiniMap_Land_Sub(pDev,lefttop,
							newx,newy,
							0,0,
							MAPKIND_WALL,MAPKIND_WALLEGDE,
							0.5,0.5,
							a+aspect2);
					}
					else if(land->kind_surface() == MAPKIND_WATER)
					{	//水
						m_DO.m_color.ARGB(trans2,50,200,255);

						DrawMiniMap_Land_Sub(pDev,lefttop,
							newx,newy,
							0,2,
							MAPKIND_WATER,-1,
							0.5,0.5,
							a+aspect2);
					}
				}
			}
			*/

			//land->minimapDrawed = true;
			
			


		
		}
	}



	return true;
}
int cMiniMap::DrawMiniMap_Land_Sub(IDirect3DDevice9 *pDev,cCoordinate lefttop,
		const int x,const int y,//マップ座標
		const int baseXi,const int baseYi,//テクスチャ座標基礎
		const int first_land,const int second_land,//テクスチャ座標基礎
		const double versep,const double horsep,//テクスチャ座標分割
		int aspect)
{
	int key1,key2,key3;
	int kind1,kind2,kind3;
	int xi, yi;


	int boxsize = MINIMAPSIZE;//枡の大きさ

	double ver = (double)MINIMAPSIZE/MINIMAPTEXHEIGHT;//縦の単位
	double hor = (double)MINIMAPSIZE/MINIMAPTEXWIDTH;//横の単位


	aspect = safeAspect(aspect);
	if(aspect == 7)
	{
		//左上
		kind1 = Land(x,y)->neighbor(ASPECT_LEFT);
		kind2 = Land(x,y)->neighbor(ASPECT_UP);
		kind3 = Land(x,y)->neighbor(ASPECT_UPLEFT);
		key1 = (kind1 == first_land);
		key2 = (kind2 == first_land);
		key3 = (kind3 == first_land);
		key1 |= (kind1 == second_land);
		key2 |= (kind2 == second_land);
		key3 |= (kind3 == second_land);


		if((key1&&key2) && key3)
		{
			return true;
			//xi = 2 + baseXi;
			//yi = 0 + baseYi;
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
	}
	else if(aspect == 1)
	{

		//右上
		kind1 = Land(x,y)->neighbor(ASPECT_RIGHT);
		kind2 = Land(x,y)->neighbor(ASPECT_UP);
		kind3 = Land(x,y)->neighbor(ASPECT_UPRIGHT);
		key1 = (kind1 == first_land);
		key2 = (kind2 == first_land);
		key3 = (kind3 == first_land);
		key1 |= (kind1 == second_land);
		key2 |= (kind2 == second_land);
		key3 |= (kind3 == second_land);

		if((key1&&key2) && key3)
		{
			return true;
			//xi = 2 + baseXi;
			//yi = 0 + baseYi;
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
	}
	else if(aspect == 5)
	{

		//左下
		kind1 = Land(x,y)->neighbor(ASPECT_LEFT);
		kind2 = Land(x,y)->neighbor(ASPECT_DOWN);
		kind3 = Land(x,y)->neighbor(ASPECT_DOWNLEFT);
		key1 = (kind1 == first_land);
		key2 = (kind2 == first_land);
		key3 = (kind3 == first_land);
		key1 |= (kind1 == second_land);
		key2 |= (kind2 == second_land);
		key3 |= (kind3 == second_land);

		if((key1&&key2) && key3)
		{
			return true;
			//xi = 2 + baseXi;
			//yi = 0 + baseYi;
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
	}
	else if(aspect == 3)
	{

		//右下
		kind1 = Land(x,y)->neighbor(ASPECT_RIGHT);
		kind2 = Land(x,y)->neighbor(ASPECT_DOWN);
		kind3 = Land(x,y)->neighbor(ASPECT_DOWNRIGHT);
		key1 = (kind1 == first_land);
		key2 = (kind2 == first_land);
		key3 = (kind3 == first_land);
		key1 |= (kind1 == second_land);
		key2 |= (kind2 == second_land);
		key3 |= (kind3 == second_land);

		if((key1&&key2) && key3)
		{
			return true;
			//xi = 2 + baseXi;
			//yi = 0 + baseYi;
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
	}

	return true;
}

int cMiniMap::DrawMiniMap_Object(IDirect3DDevice9 *pDev)
{
	pcLandform land;
	pcDroping pdrop;
	pcCharacter pchara;
	
	int size = MiniMapObjectdraw_list.size();
	int i,x,y,ui,vi;
	for(i=0;i<size;i++)
	{
		GetMiniMapObjectdraw(i,x,y,ui,vi);
		MiniMapDrawSub(pDev,x,y,ui,vi);
	}

	sg_pDungeonSystem->minimapDrawDelegate(pDev, miniMapLeftTop(), MINIMAPSIZE, miniMapObjectAlpha());
/*
	int i,x,y;
	for(i=0;i<sg_pDungeonSystem->DropList().size();i++)
	{
		pdrop = sg_pDungeonSystem->DropList()[i];
		x = pdrop->placeX;
		y = pdrop->placeY;
		land = Land(x,y);
		if(land->pOnDrop )
		{
			if( land->pOnDrop->discover)
			{//見えてる
				cItem* pItem;
				if(pItem = dynamic_cast<cItem*>(land->pOnDrop.get()))
				{//アイテム
					MiniMapDrawSub(pDev,x,y,0,1);
				}
			}
		}
		else
		{
			//int temp = 0;//era-
		}
	}
	for(i=0;i<sg_pDungeonSystem->CharaList().size();i++)
	{
		pchara = sg_pDungeonSystem->CharaList()[i];
		x = pchara->placeX;
		y = pchara->placeY;
		land = Land(x,y);
		if(land->pOnChar)
		{
			if(land->pOnChar->Forse == CHARACTER_FORSE_ENEMY)
			{//敵
				MiniMapDrawSub(pDev,x,y,1,0);
			}
			else if(land->pOnChar->Forse == CHARACTER_FORSE_FRIEND)
			{//味方
				//if(land->pOnChar == sg_pDungeonSystem->pPlayerChara())
				//{//プレイヤー

					//MiniMapDrawSub(pDev,x,y,0,0);
				//}
				//else
				//{
					MiniMapDrawSub(pDev,x,y,0,0);
				//}
			}

		}
		else
		{
			//int temp = 0;//era-
		}
	}
*/
	return true;
}
	

//描いたかどうか
int& cMiniMap::MiniMapdrawed(int x, int y)
{
	if(x < 0) x = 0;
	else if(x >= MAPWIDTH) x = MAPWIDTH-1;
	if(y < 0) y = 0;
	else if(y >= MAPHEIGHT) y = MAPHEIGHT-1;
	return MiniMapdrawed_list[y*MAPWIDTH + x];
}
int& cMiniMap::MiniMapdrawed(const cCoordinate& cood)
{
	return MiniMapdrawed(cood.x, cood.y);
}

int cMiniMap::MiniMapdrawed_edge(const int x,const int y,int aspect)
{
	aspect = safeAspect(aspect);
	if(aspect % 2 != 1) OnAssert(_T(__FILE__),__LINE__,false,_T("cMiniMap::MiniMapdrawed_edge　aspectが不正な値です"));
	cCoordinate coo;
	int i;
	int flag = false;
	for(i=aspect-1;i<=aspect+1;i++)
	{
		coo.SetAspect(i);
		if(x + coo.x < 0 || x + coo.x >= MAPWIDTH
			|| y + coo.y < 0 || y + coo.y >= MAPHEIGHT) continue;
		flag |= MiniMapdrawed(x + coo.x, y + coo.y);
	}

	return flag | MiniMapdrawed(x,y);
}
int cMiniMap::MiniMapdrawed_edge(const cCoordinate& cood,int aspect)
{
	return MiniMapdrawed_edge(cood.x, cood.y, aspect);
}

//マッピングされたかどうか
int& cMiniMap::MiniMapMapped_store(int x, int y)
{
	if(x < 0) x = 0;
	else if(x >= MAPWIDTH) x = MAPWIDTH-1;
	if(y < 0) y = 0;
	else if(y >= MAPHEIGHT) y = MAPHEIGHT-1;
	return MiniMapMapped_list_store[y*MAPWIDTH + x];
}
int& cMiniMap::MiniMapMapped_store(const cCoordinate& cood)
{
	return MiniMapMapped_store(cood.x, cood.y);
}

int cMiniMap::MiniMapMapped_store_edge(const int x,const int y,int aspect)
{
	aspect = safeAspect(aspect);
	if(aspect % 2 != 1) OnAssert(_T(__FILE__),__LINE__,false,_T("cMiniMap::MiniMapdrawed_edge　aspectが不正な値です"));
	cCoordinate coo;
	int i;
	int flag = false;
	for(i=aspect-1;i<=aspect+1;i++)
	{
		coo.SetAspect(i);
		if(x + coo.x < 0 || x + coo.x >= MAPWIDTH
			|| y + coo.y < 0 || y + coo.y >= MAPHEIGHT) continue;
		flag |= MiniMapMapped_store(x + coo.x, y + coo.y);
	}

	return flag || MiniMapMapped_store(x,y);
}
int cMiniMap::MiniMapMapped_store_edge(const cCoordinate& cood,int aspect)
{
	return MiniMapMapped_store_edge(cood.x, cood.y, aspect);
}

void cMiniMap::MiniMapMapped_list_refresh()
{
	int i;
	int x,y;
	int size = MiniMapMappedAndNoDraw_list.size();
	for(i = 0; i < size;i++)
	{
		GetMiniMapMappedAndNoDraw(i,x,y);
		if(x < 0 || x >= MAPWIDTH || y < 0 || y >= MAPHEIGHT ) continue;
		MiniMapMapped_store(x,y) = true;
	}
	MiniMapMappedAndNoDraw_list.clear();
}

cCoordinate cMiniMap::miniMapLeftTop()
{
	return cCoordinate(-MiniMapRange.Left()*MINIMAPSIZE + MINIMAPDRAWCENTERX - MINIMAPSIZE*MAPWIDTH/2,
					-MiniMapRange.CenterY*MINIMAPSIZE + MINIMAPDRAWCENTERY);
}
int cMiniMap::miniMapObjectAlpha()
{
	if(g_pPlayerInput()->miniMap().on)
	{
		return 255;
	}
	else
	{
		return 186*sqrt(sg_pDungeonSystem->EV_mapThickness/6.0);
	}
}


int cMiniMap::MiniMapDrawSub(IDirect3DDevice9 *pDev,int x,int y,int ui,int vi)
{
	int boxsize = MINIMAPSIZE;//枡の大きさ
	cCoordinate lefttop = miniMapLeftTop();//左上
	
	chip_DO.setTexture(m_pTexture_miniMapchip,MINIMAPTEXWIDTH,MINIMAPTEXHEIGHT);
	
	lefttop.x += x*boxsize;
	lefttop.y += y*boxsize;

	chip_DO.setLTRB(lefttop.x,lefttop.y, lefttop.x + boxsize, lefttop.y + boxsize);
	
	double vsize = 0.125;
	double usize = 0.25;
	chip_DO.m_TexRange.setLTRB(ui*usize,vi*vsize,(ui+1)*usize,(vi+1)*vsize);
	
	chip_DO.m_color.alpha = miniMapObjectAlpha();
	chip_DO.Draw(pDev);
	return true;
}







//ミニマップを一度消して書き直すフラグをたてる。
int cMiniMap::ResetMiniMap()
{


	RefreshMiniMap();
	
	int x,y;
	for(x = 0; x < MAPWIDTH;x++)
	{
		for(y = 0; y < MAPHEIGHT;y++)
		{
			//Land(x,y)->minimapDrawed = false;
			MiniMapdrawed(x,y) = false;
		}
	}
	ClearMiniMapFlag = true;




	//g_GameEnv.m_Screen->m_LayerClear = true;

	return true;
}

//ミニマップを書き直す
int cMiniMap::RefreshMiniMap()
{

	

	RefreshMiniMapFlag = true;
	

	
	StoreAllObjectdraw_list();


	MiniMapMapped_list_refresh();

	//g_GameEnv.m_Screen->m_LayerClear = true;

	return true;
}


//指定位置を通過済みマッピングする
int cMiniMap::mapping_just(const int x, const int y)
{
	private_mapping_just(x,y);
	return true;
}

//マッピング
int cMiniMap::mapping(const int x, const int y)
{
	int a;
	cCoordinate cood;

	pointmapping(x,y);

	for(a=0;a<8;a++)
	{
		cood.SetAspect(a);
		cood += Land(x,y)->place;
		if(Land(cood)->through(0,0,0))
		{
			private_mapping(cood.x,cood.y);
		}
		if(Land(cood)->pOnDrop)
			sg_pDungeonSystem->アイテムマッピング(Land(cood)->pOnDrop);		
	}
	return true;
}
//マッピング
void cMiniMap::private_mapping(const int x,const int y)
{
	Land(x,y)->mapped |= MAPPED_SEE;
	PushBackMiniMapMappedAndNoDraw(x,y);
}
//マッピング歩いた
void cMiniMap::private_mapping_just(const int x,const int y)
{
	Land(x,y)->mapped |= MAPPED_JUSTCHECK;
	PushBackMiniMapMappedAndNoDraw(x,y);
}

//指定位置のみマッピングする
int cMiniMap::pointmapping(const int x, const int y)
{

	private_mapping(x,y);

	if(Land(x,y)->pOnDrop)
		sg_pDungeonSystem->アイテムマッピング(Land(x,y)->pOnDrop);
		

	minimapextend(x,y);

	return true;
}
//ミニマップ表示範囲拡張
int cMiniMap::minimapextend(const int x, const int y)
{
	//double MiniMapRangeSize = MiniMapRange.Width*MiniMapRange.Height;


	if(MiniMapRange.Left() > x)MiniMapRange.formLeft(x);
	if(MiniMapRange.Top() > y)MiniMapRange.formTop(y);
	if(MiniMapRange.Right() < x)MiniMapRange.formRight(x);
	if(MiniMapRange.Bottom() < y)MiniMapRange.formBottom(y);

	/*
	if(MiniMapRangeSize != MiniMapRange.Width*MiniMapRange.Height)
	{
		//ResetMiniMap();
	}
	*/
	return true;
}
//全マッピングする
int cMiniMap::Allmapping()
{
	int x,y;
	for(x = 0; x < MAPWIDTH;x++)
	{
		for(y = 0; y < MAPHEIGHT;y++)
		{
			private_mapping(x,y);
			if(Land(x,y)->pOnDrop)
				sg_pDungeonSystem->アイテムマッピング(Land(x,y)->pOnDrop);
			MiniMapRange.setLTRB(0,0,MAPWIDTH,MAPHEIGHT);
			
		}
	}

	
	//ResetMiniMap();
	
	return true;
}
//ミニマップを消す
int cMiniMap::EraseMiniMap(int FocusX, int FocusY)
{

	MiniMapMapped_list_refresh();

	int x,y;
	for(x = 0; x < MAPWIDTH;x++)
	{
		for(y = 0; y < MAPHEIGHT;y++)
		{
			Land(x,y)->mapped = false;
			//Land(x,y)->minimapDrawed = false;
			MiniMapdrawed(x,y) = false;
			MiniMapMapped_store(x,y) = false;

			if(Land(x,y)->pOnDrop)
				Land(x,y)->pOnDrop->disappear();
			MiniMapRange.setLTRB(FocusX,FocusY,FocusX,FocusY);
		}
	}


	ResetMiniMap();

	return true;
}

//ミニマップ表示する
void cMiniMap::ViewOn()
{
	//g_GameEnv.m_Screen->m_LayerEnable = true;
	//g_GameEnv.m_Screen->m_OffScreenEnable = true;
	ViewFlag = true;
}

//ミニマップ表示消す
void cMiniMap::ViewOff()
{
	//g_GameEnv.m_Screen->m_LayerEnable = false;
	//g_GameEnv.m_Screen->m_OffScreenEnable = false;
	ViewFlag = false;
}



//全オブジェクトリストストア
void cMiniMap::StoreAllObjectdraw_list()
{

	MiniMapObjectdraw_list.clear();

	pcLandform land;
	pcDroping pdrop;
	pcCharacter pchara;
	


	int i,x,y;
	for(i=0;i<sg_pDungeonSystem->DropList().size();i++)
	{
		pdrop = sg_pDungeonSystem->DropList()[i];
		if(!pdrop->落ちてる())continue;
		x = pdrop->placeX;
		y = pdrop->placeY;
		land = Land(x,y);
		if(land->pOnDrop )
		{
			if( land->pOnDrop->isDiscover())
			{//見えてる
 				if(land->pOnDrop->属性.count(落ち物属性::アイテム))
				{//アイテム
					PushBackMiniMapObjectdraw(x,y,0,1);
				}
				else if(land->pOnDrop->属性.count(落ち物属性::罠))
				{//トラップ
					PushBackMiniMapObjectdraw(x,y,1,1);
				}
				else if(land->pOnDrop->属性.count(落ち物属性::階段))
				{//階段
					PushBackMiniMapObjectdraw(x,y,2,1);
				}
				else if(land->pOnDrop->属性.count(落ち物属性::火柱))
				{//火柱
					PushBackMiniMapObjectdraw(x,y,0,4);
				}
				minimapextend(x,y);
			}
		}
		else
		{
			assert( !_T("アイテムの位置情報と地形のアイテム情報が一致しません。") );
		}
	}
	for(i=0;i<sg_pDungeonSystem->CharaList().size();i++)
	{
		pchara = sg_pDungeonSystem->CharaList()[i];
		x = pchara->placeX;
		y = pchara->placeY;
		land = Land(x,y);
		if(land->pOnChar)
		{
			if(land->pOnChar->Forse == CHARACTER_FORSE_ENEMY)
			{//敵
				if(!sg_pDungeonSystem->キャラクター目視可(land->pOnChar)) continue;
				if(land->pOnChar->CharaAttribute().count(キャラ属性::弾幕))
				{
					PushBackMiniMapObjectdraw(x,y,0,5);
				}
				else
				{
					PushBackMiniMapObjectdraw(x,y,1,0);
					
					if(land->pOnChar->CLASS)
					{
						if(land->pOnChar->CLASS < 5)
						{
							PushBackMiniMapObjectdraw(x,y,land->pOnChar->CLASS-1,2);
						}
						else
						{
							PushBackMiniMapObjectdraw(x,y,3,3);
						}
					}
				}
				if(land->pOnChar->CharaAttribute().count(キャラ属性::ボス))
				{
					PushBackMiniMapObjectdraw(x,y,2,5);
				}
			}
			else if(land->pOnChar->Forse == CHARACTER_FORSE_FRIEND)
			{//味方
				//if(land->pOnChar == sg_pDungeonSystem->pPlayerChara())
				//{//プレイヤー

					//PushBackMiniMapObjectdraw(x,y,0,0);
				//}
				//else
				//{
					PushBackMiniMapObjectdraw(x,y,0,0);
				//}
			}
			else
			{//ソノタ
				if(!sg_pDungeonSystem->キャラクター目視可(land->pOnChar)) continue;
				//if(land->pOnChar == sg_pDungeonSystem->pPlayerChara())
				//{//プレイヤー

					//PushBackMiniMapObjectdraw(x,y,0,0);
				//}
				//else
				//{
					PushBackMiniMapObjectdraw(x,y,2,0);
				//}
			}
			int aspect = safeAspect( land->pOnChar->aspect);
			PushBackMiniMapObjectdraw(x,y,aspect % 4, 6 + aspect/4);

			minimapextend(x,y);
		}
		else
		{
			assert( !_T("キャラの位置情報と地形のキャラ情報が一致しません。") );
		}
	}


}

void cMiniMap::PushBackMiniMapObjectdraw(const int x,const int y,const int ui,const int vi)
{
	MiniMapObjectdraw_list.push_back( ((x&0xFF)<<24 | (y&0xFF)<<16 | (ui&0xFF)<<8 | (vi&0xFF)) );
}

void cMiniMap::GetMiniMapObjectdraw(const int index,int &x,int &y,int &ui,int &vi)
{
	x = 0xFF&(MiniMapObjectdraw_list[index] >> 24);
	y = 0xFF&(MiniMapObjectdraw_list[index] >> 16);
	ui = 0xFF&(MiniMapObjectdraw_list[index] >> 8);
	vi = 0xFF&(MiniMapObjectdraw_list[index]);
}

void cMiniMap::PushBackMiniMapMappedAndNoDraw(const unsigned int x,const unsigned int y)
{
	MiniMapMappedAndNoDraw_list.push_back( ((x&0xFF)<<24 | (y&0xFF)<<16 ));
}

void cMiniMap::GetMiniMapMappedAndNoDraw(const int index,int &x,int &y)
{
	x = 0xFF&(MiniMapMappedAndNoDraw_list[index] >> 24);
	y = 0xFF&(MiniMapMappedAndNoDraw_list[index] >> 16);
}
