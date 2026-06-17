#include "stdafx.h"

#include "cDrawingObject.h"


namespace
{
void SetPointSampler(cRenderDevice* pDev, DWORD sampler)
{
	cRenderSetPointSampler(pDev, sampler);
}
}


//-------------------------------------------------
//cDrawableObject
//-------------------------------------------------
cDrawableObject::cDrawableObject(void)
{
	m_pTexture = NULL;
	m_color.ARGB(255,255,255,255);
	m_TexRange.setLTRB(0,0,1,1);
	AddingDraw = DRAW_MODE_NORMAL;
	colorblendmode = COLOR_BLEND_MULTIPLE;
}
cDrawableObject::~cDrawableObject(void)
{
}
bool cDrawableObject::culculateRealTexRange()
{
	double w = m_TexSizeX * m_TexRange.Width;
	double h = m_TexSizeY * m_TexRange.Height;
	double l = m_TexSizeX * m_TexRange.Left();
	double t = m_TexSizeY * m_TexRange.Top();
	

	m_RealTexRange.setLTRB((l+0.49)/m_TexSizeX,
							(t+0.49)/m_TexSizeY,
							(l+w+0.49)/m_TexSizeX,
							(t+h+0.49)/m_TexSizeY);

	return true;
}
void cDrawableObject::setTexture(cRenderTexture* pTexture,int TexSizeX, int TexSizeY)
{
	m_pTexture = pTexture;
	m_TexSizeX = TexSizeX;
	m_TexSizeY = TexSizeY;
}
void cDrawableObject::setTexture(cRenderTexture* pTexture)
{
	m_pTexture = pTexture;
	if(pTexture == NULL) return;

	cRenderGetTextureSize(pTexture, m_TexSizeX, m_TexSizeY);

}

int cDrawableObject::SetRenderMode(cRenderDevice *pDev)
{

	// 頂点ＦＶＦを設定
	cRenderSetColoredVertexFormat(pDev);

	cRenderSetTextureCoordIndex(pDev, 0, 0);
	SetPointSampler(pDev, 0);



	if(colorblendmode == COLOR_BLEND_MULTIPLE)
	{
		// 頂点色を乗算する
		cRenderSetTextureColorMode(pDev, 0, C_RENDER_TEXTURE_COLOR_MODULATE);
	}
	else if(colorblendmode == COLOR_BLEND_ADDITION)
	{
		// 頂点色を加算する
		cRenderSetTextureColorMode(pDev, 0, C_RENDER_TEXTURE_COLOR_ADD);
	}
	else if(colorblendmode == COLOR_BLEND_FILL)
	{
		// 頂点色で塗りつぶす
		cRenderSetTextureColorMode(pDev, 0, C_RENDER_TEXTURE_COLOR_FILL);
	}
	/*
	pDev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	pDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	pDev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	*/
	if(AddingDraw == DRAW_MODE_ADDITION)
	{//加算
		cRenderSetBlendFactors(pDev, C_RENDER_BLEND_FACTORS_ADDITION);
		cRenderSetBlendOperation(pDev, C_RENDER_BLEND_OPERATION_ADD);
	}
	else if(AddingDraw == DRAW_MODE_NORMAL)
	{//通常
		cRenderSetBlendFactors(pDev, C_RENDER_BLEND_FACTORS_NORMAL);
		cRenderSetBlendOperation(pDev, C_RENDER_BLEND_OPERATION_ADD);
	}
	else if(AddingDraw == DRAW_MODE_SUBSTRACTION)
	{//減算だが、可能か？
		cRenderSetBlendFactors(pDev, C_RENDER_BLEND_FACTORS_ADDITION);
		cRenderSetBlendOperation(pDev, C_RENDER_BLEND_OPERATION_REVERSE_SUBTRACT);
	}
	else//
	{//
		cRenderSetBlendFactors(pDev, C_RENDER_BLEND_FACTORS_NORMAL);
		cRenderSetBlendOperation(pDev, C_RENDER_BLEND_OPERATION_ADD);
	}


	//アルファ合成を可に
	cRenderSetAlphaBlendEnabled(pDev, true);

	return true;
}










//-------------------------------------------------
//cDrawingObject
//-------------------------------------------------
cDrawingObject::cDrawingObject(void)
{
	Rotation = 0.0;
	ScaleX = 1.0;
	ScaleY = 1.0;
	Rotation2 = 0.0;

}

cDrawingObject::~cDrawingObject(void)
{
}




int cDrawingObject::Draw(cRenderDevice *pDev)
{
	int i;

	CenterX , CenterY;

	c4DVector v[4];
	c4DVector vCentor;

	vCentor.set(CenterX,CenterY,0,0);

	(v[0]).set(Left() - vCentor.x, Top() - vCentor.y,0,0);
	(v[1]).set(Right() - vCentor.x, Top() - vCentor.y,0,0);
	(v[2]).set(Left() - vCentor.x, Bottom() - vCentor.y,0,0);
	(v[3]).set(Right() - vCentor.x, Bottom() - vCentor.y,0,0);

	if(Rotation != 0.0)
	{
		for(i=0;i<4;i++)
		{
			(v[i]).turn(Rotation);
		}
	}

	if(ScaleX != 1.0)
	{
		for(i=0;i<4;i++)
		{
			(v[i]).x *= ScaleX;
		}
	}

	if(ScaleY != 1.0)
	{
		for(i=0;i<4;i++)
		{
			(v[i]).y *= ScaleY;
		}
	}

	if(Rotation2 != 0.0)
	{
		for(i=0;i<4;i++)
		{
			(v[i]).turn(Rotation2);
		}
	}

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
		cRenderSetTexture(pDev, 0, m_pTexture);
	}
	else
	{
		m_RealTexRange.setLTRB(0,0,0,0);
		//テクスチャない
		cRenderSetTexture(pDev, 0, NULL);
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


	cRenderDrawColoredTriangleStrip(pDev, 2, VxData);

	return true;
}


	//基礎しかサポートし無い高速ドロー
int cDrawingObject::EasyDraw(cRenderDevice *pDev)
{
	int i;



	float l = Left();
	float r = Right();
	float t = Top();
	float b = Bottom();



	m_RealTexRange.setLTRB(0,0,0,0);
	//テクスチャない
	cRenderSetTexture(pDev, 0, NULL);
	

	VERTEX2D_COLORED VxData[] = {
		{l, t, 0, 1.0, m_color.D3Dcolor(), 0, 0},
		{r, t, 0, 1.0, m_color.D3Dcolor(), 0, 0},
		{l, b, 0, 1.0, m_color.D3Dcolor(), 0, 0},
		{r, b, 0, 1.0, m_color.D3Dcolor(), 0, 0}
	};



	// 頂点ＦＶＦを設定
	cRenderSetColoredVertexFormat(pDev);

	cRenderSetTextureCoordIndex(pDev, 0, 0);
	SetPointSampler(pDev, 0);


	
	// 頂点色を乗算する
	cRenderSetTextureColorMode(pDev, 0, C_RENDER_TEXTURE_COLOR_MODULATE);
	


	cRenderSetBlendFactors(pDev, C_RENDER_BLEND_FACTORS_NORMAL);


	//アルファ合成を可に
	cRenderSetAlphaBlendEnabled(pDev, true);

	cRenderDrawColoredTriangleStrip(pDev, 2, VxData);

	return true;
}
