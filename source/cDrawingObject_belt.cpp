#include "stdafx.h"

#include "cDrawingObject_belt.h"


#include <boost/preprocessor.hpp>

#include <boost/preprocessor/repetition/repeat.hpp>


#define DEFOULT_ACCURACY (16)
#define ACCURACY_MAX 128




#define DEF_VERTEX_DEF(a, n, text) \
{(float)(v[n].x),\
(float)(v[n].y),\
(float)(v[n].z),\
1.0,\
m_color.D3Dcolor(),\
(float)(r[n].x),\
(float)(r[n].y)},


cDrawingObject_belt::cDrawingObject_belt(void)
{
	Accuracy = DEFOULT_ACCURACY;
}

cDrawingObject_belt::~cDrawingObject_belt(void)
{
}

int cDrawingObject_belt::Draw(IDirect3DDevice9 *pDev)
{
	int i;

	int accur = max(2,min(Accuracy,ACCURACY_MAX));

	int vertex_num = accur*2;


	c4DVector v[ACCURACY_MAX];
	c4DVector r[ACCURACY_MAX];
	c4DVector vCentor[2];

	vCentor[0] = startingpoints[0];
	vCentor[1] = startingpoints[1];

	vCentor[0].x += CenterX;
	vCentor[1].x += CenterX;

	vCentor[0].y += CenterY;
	vCentor[1].y += CenterY;

	for(i=0;i<vertex_num;i++)
	{
		double d = (i/2)/(double)(accur-1);
		double point = start*(1-d) + end*d;

		v[i] = Tracks[i%2].get(point) + vCentor[i%2];
	
		v[i].y -= v[i].z/2;
		v[i].z = 0.0;
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

		for(i=0;i<vertex_num;i++)
		{
			double d = (i/2)/(double)(accur-1);
			double point = m_RealTexRange.Left()*(1-d) + m_RealTexRange.Right()*d;
			r[i].set(point,m_RealTexRange.Top()*((i+1)%2) + m_RealTexRange.Bottom()*((i)%2) ,0,0);
		}

	}
	else
	{
		m_RealTexRange.setLTRB(0,0,0,0);
		//テクスチャない
		pDev->SetTexture(0, NULL);
	}

	VERTEX2D_COLORED VxData[] = {
		BOOST_PP_REPEAT(ACCURACY_MAX, DEF_VERTEX_DEF, nil)
		{}
	};


	SetRenderMode(pDev);


	pDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, (accur-1)*2 , VxData, sizeof(VERTEX2D_COLORED) );

	return true;


}
