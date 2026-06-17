#include "stdafx.h"

#include "cDrawingObject_Rod.h"


//-------------------------------------------------
//cDrawingObject_Rod
//-------------------------------------------------
cDrawingObject_Rod::cDrawingObject_Rod(void)
{
	startX = 0.0;
	startY = 0.0;
	endX = 0.0;
	endY = 0.0;
	halfWidth = 10.0;
}

cDrawingObject_Rod::~cDrawingObject_Rod(void)
{
}



int cDrawingObject_Rod::Draw(cRenderDevice *pDev)
{

	/*

	cCoordinate coo;
	coo.SetAspect(aspect_);
	int i;
	
	c4DVector point(coo.x*(-visibleWidth_/2), coo.y*(-visibleWidth_/2), 0, 0);
	c4DVector point2(point);
	point.turn(-45);
	point2.turn(45);
	point.x += land_->placeX;
	point.y += land_->placeY;
	point2.x += land_->placeX;
	point2.y += land_->placeY;
	beltEx_.rightStartpoint() = sg_pDungeonSystem->Map().visibleplace(point);
	beltEx_.leftStartpoint() = sg_pDungeonSystem->Map().visibleplace(point2);
	

	beltEx_.Draw(sg_pDungeonSystem->pDevice_D3D);
	*/
	int i;

	c4DVector start(startX,startY, 0,0);
	c4DVector end(endX,endY, 0,0);
	if(start == end)
	{
		CenterX = start.x;
		CenterY = start.y;
		Width = halfWidth*2;
		Height = halfWidth*2;

		c4DVector v[4];
		c4DVector vCentor;

		vCentor.set(CenterX,CenterY,0,0);

		(v[0]).set(Left() - vCentor.x, Top() - vCentor.y,0,0);
		(v[1]).set(Right() - vCentor.x, Top() - vCentor.y,0,0);
		(v[2]).set(Left() - vCentor.x, Bottom() - vCentor.y,0,0);
		(v[3]).set(Right() - vCentor.x, Bottom() - vCentor.y,0,0);

		for(i=0;i<4;i++)
		{
			(v[i]) += vCentor;
		}



		if(m_pTexture != NULL)
		{
			//----------------------------------------
			//テクスチャ座標ズレ修正のためのルーチン
			//動作確認
			//----------------------------------------
			culculateRealTexRange();
			
			//テクスチャあり
			pDev->SetTexture(0, m_pTexture);
		}
		else
		{
			m_RealTexRange.setLTRB(0,0,0,0);
			//テクスチャない
			pDev->SetTexture(0, NULL);
		}

		unsigned long color[4];
		if(vm_color.size() >= 4)
		{
			color[0] = vm_color[0].D3Dcolor();
			color[1] = vm_color[1].D3Dcolor();
			color[2] = vm_color[2].D3Dcolor();
			color[3] = vm_color[3].D3Dcolor();
		}
		else
		{
			color[0] = m_color.D3Dcolor();
			color[1] = m_color.D3Dcolor();
			color[2] = m_color.D3Dcolor();
			color[3] = m_color.D3Dcolor();
		}

		VERTEX2D_COLORED VxData[] = {
			{(float)(v[0].x), (float)(v[0].y), (float)(v[0].z), 1.0, color[0], (float)(m_RealTexRange.Left()), (float)(m_RealTexRange.Top())},
			{(float)(v[1].x), (float)(v[1].y), (float)(v[1].z), 1.0, color[1], (float)(m_RealTexRange.Right()), (float)(m_RealTexRange.Top())},
			{(float)(v[2].x), (float)(v[2].y), (float)(v[2].z), 1.0, color[2], (float)(m_RealTexRange.Left()), (float)(m_RealTexRange.Bottom())},
			{(float)(v[3].x), (float)(v[3].y), (float)(v[3].z), 1.0, color[3], (float)(m_RealTexRange.Right()), (float)(m_RealTexRange.Bottom())}
		};



		SetRenderMode(pDev);


		pDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, VxData, sizeof(VERTEX2D_COLORED) );

	}
	else
	{
		CenterX = start.x;
		CenterY = start.y;
		c4DVector backV = unit(end - start)*(-halfWidth);
		c4DVector leftV = backV;
		leftV.turn90();

		Width = halfWidth*2;
		Height = halfWidth*2;

		c4DVector v[8];

		v[0] = start + backV - leftV;
		v[1] = start + backV + leftV;
		v[2] = start		 - leftV;
		v[3] = start		 + leftV;
		v[4] = end			 - leftV;
		v[5] = end			 + leftV;
		v[6] = end	 - backV - leftV;
		v[7] = end	 - backV + leftV;


		if(m_pTexture != NULL)
		{
			//----------------------------------------
			//テクスチャ座標ズレ修正のためのルーチン
			//動作確認
			//----------------------------------------
			culculateRealTexRange();
			
			//テクスチャあり
			pDev->SetTexture(0, m_pTexture);
		}
		else
		{
			m_RealTexRange.setLTRB(0,0,0,0);
			//テクスチャない
			pDev->SetTexture(0, NULL);
		}

		unsigned long color[8];
		if(vm_color.size() >= 4)
		{
			color[0] = vm_color[0].D3Dcolor();
			color[1] = vm_color[1].D3Dcolor();
			color[2] = vm_color[0].D3Dcolor();
			color[3] = vm_color[1].D3Dcolor();
			color[4] = vm_color[2].D3Dcolor();
			color[5] = vm_color[3].D3Dcolor();
			color[6] = vm_color[2].D3Dcolor();
			color[7] = vm_color[3].D3Dcolor();
		}
		else
		{
			int i;
			for(i=0;i<8;i++)
			{
				color[i] = m_color.D3Dcolor();
			}
		}

		VERTEX2D_COLORED VxData[] = {
			{(float)(v[0].x), (float)(v[0].y), (float)(v[0].z), 1.0, color[0], (float)(m_RealTexRange.Left()), (float)(m_RealTexRange.Top())},
			{(float)(v[1].x), (float)(v[1].y), (float)(v[1].z), 1.0, color[1], (float)(m_RealTexRange.Right()), (float)(m_RealTexRange.Top())},

			{(float)(v[2].x), (float)(v[2].y), (float)(v[2].z), 1.0, color[2], (float)(m_RealTexRange.Left()), (float)(m_RealTexRange.CenterY)},
			{(float)(v[3].x), (float)(v[3].y), (float)(v[3].z), 1.0, color[3], (float)(m_RealTexRange.Right()), (float)(m_RealTexRange.CenterY)},

			{(float)(v[4].x), (float)(v[4].y), (float)(v[4].z), 1.0, color[4], (float)(m_RealTexRange.Left()), (float)(m_RealTexRange.CenterY)},
			{(float)(v[5].x), (float)(v[5].y), (float)(v[5].z), 1.0, color[5], (float)(m_RealTexRange.Right()), (float)(m_RealTexRange.CenterY)},

			{(float)(v[6].x), (float)(v[6].y), (float)(v[6].z), 1.0, color[6], (float)(m_RealTexRange.Left()), (float)(m_RealTexRange.Bottom())},
			{(float)(v[7].x), (float)(v[7].y), (float)(v[7].z), 1.0, color[7], (float)(m_RealTexRange.Right()), (float)(m_RealTexRange.Bottom())}
		};



		SetRenderMode(pDev);


		pDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 6, VxData, sizeof(VERTEX2D_COLORED) );

	}

	return true;
}


	//基礎しかサポートし無い高速ドロー
int cDrawingObject_Rod::EasyDraw(cRenderDevice *pDev)
{
	return Draw(pDev);
}