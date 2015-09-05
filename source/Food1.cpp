#include "stdafx.h"
#include "Food1.h"

#include "cDungeonSystem.h"
#include "cAttackinformation.h"

//------------------------------------------------------------------------------
//普通の大きさ
//------------------------------------------------------------------------------
int cFood_Normal::効果(pcCharacter pchara, int recover, int extend)
{
	//sg_pDungeonSystem->AnimationManager().
	//	Anime_PlaySE(_T("recover.wav"),pchara->足元地形()->place);

	if(pchara->Stomach + 0.99 > pchara->MaxStomach)
	{
		sg_pDungeonSystem->最大満腹度拡張要請(pchara,extend);	
	}
	else
	{
		sg_pDungeonSystem->満腹度回復要請(pchara,recover);	
	}

	sg_pDungeonSystem->元気要請(pchara,効果量(2),効果量(3));
	sg_pDungeonSystem->頑強要請(pchara,効果量(2),効果量(3));

	return true;
}
int cFood_Normal::食べる_効果_通常(pcCharacter pchara)
{
	return 効果(pchara
		, 効果量(0)//回復量
		, 効果量(1)//HP拡張量
		);
}
int cFood_Normal::食べる_効果_祝福(pcCharacter pchara)
{
	return 食べる_効果_通常(pchara);
}
void cFood_Normal::CutIn(タイミング timing, cValiableField& valiable)
{
	cFood::CutIn(timing,valiable);

	if(timing == 食べ物劣化_タイミング)
	{
		//if(quality() == 0)
		//	sg_pDungeonSystem->アイテム劣化(me(),0);
		//else
		//{
			pcDroping pdrop2 = sg_pDungeonSystem->落ち物生成_設置なし(4003);
			sg_pDungeonSystem->落ち物変化(me(),pdrop2);
		//}
		valiable.doubles.val(変数_汎用ブール) = 1;
	}

}
//------------------------------------------------------------------------------
//大きめ
//------------------------------------------------------------------------------
int cFood_Big::効果(pcCharacter pchara, int recover, int extend)
{
	//sg_pDungeonSystem->AnimationManager().
	//	Anime_PlaySE(_T("recover.wav"),pchara->足元地形()->place);

	if(pchara->Stomach + 0.99 > pchara->MaxStomach)
	{
		sg_pDungeonSystem->最大満腹度拡張要請(pchara,extend);	
	}
	else
	{
		sg_pDungeonSystem->満腹度回復要請(pchara,recover);	
	}
	sg_pDungeonSystem->元気要請(pchara,効果量(2),効果量(3));
	sg_pDungeonSystem->頑強要請(pchara,効果量(2),効果量(3));
	return true;
}
int cFood_Big::食べる_効果_通常(pcCharacter pchara)
{
	return 効果(pchara
		, 効果量(0)//回復量
		, 効果量(1)//HP拡張量
		);
}
int cFood_Big::食べる_効果_祝福(pcCharacter pchara)
{
	return 食べる_効果_通常(pchara);
}
void cFood_Big::CutIn(タイミング timing, cValiableField& valiable)
{
	cFood::CutIn(timing,valiable);

	if(timing == 食べ物劣化_タイミング)
	{
		//if(quality() == 0)
		//	sg_pDungeonSystem->アイテム劣化(me(),0);
		//else
		//{
			pcDroping pdrop2 = sg_pDungeonSystem->落ち物生成_設置なし(4003);
			sg_pDungeonSystem->落ち物変化(me(),pdrop2);
		//}
		valiable.doubles.val(変数_汎用ブール) = 1;
	}

}
//------------------------------------------------------------------------------
//巨大
//------------------------------------------------------------------------------
int cFood_Large::効果(pcCharacter pchara, int recover, int extend)
{
	//sg_pDungeonSystem->AnimationManager().
	//	Anime_PlaySE(_T("recover.wav"),pchara->足元地形()->place);


		sg_pDungeonSystem->満腹度回復要請(pchara,recover);	
		sg_pDungeonSystem->最大満腹度拡張要請(pchara,extend);	
	sg_pDungeonSystem->元気要請(pchara,効果量(2),効果量(3));
	sg_pDungeonSystem->頑強要請(pchara,効果量(2),効果量(3));
	return true;
}
int cFood_Large::食べる_効果_通常(pcCharacter pchara)
{
	return 効果(pchara
		, 効果量(0)//回復量
		, 効果量(1)//HP拡張量
		);
}
int cFood_Large::食べる_効果_祝福(pcCharacter pchara)
{
	return 食べる_効果_通常(pchara);
}
void cFood_Large::CutIn(タイミング timing, cValiableField& valiable)
{
	cFood::CutIn(timing,valiable);

	if(timing == 食べ物劣化_タイミング)
	{
		//if(quality() == 0)
		//	sg_pDungeonSystem->アイテム劣化(me(),0);
		//else
		//{
			pcDroping pdrop2 = sg_pDungeonSystem->落ち物生成_設置なし(4003);
			sg_pDungeonSystem->落ち物変化(me(),pdrop2);
		//}
		valiable.doubles.val(変数_汎用ブール) = 1;
	}


}

//------------------------------------------------------------------------------
//くさった
//------------------------------------------------------------------------------
int cFood_Bad::効果(pcCharacter pchara, int recover, int extend)
{
	//sg_pDungeonSystem->AnimationManager().
	//	Anime_PlaySE(_T("recover.wav"),pchara->足元地形()->place);


	if(pchara->Stomach + 0.99 > pchara->MaxStomach)
	{
		sg_pDungeonSystem->最大満腹度拡張要請(pchara,extend);	
	}
	else
	{
		sg_pDungeonSystem->満腹度回復要請(pchara,recover);	
	}

	int index = 7*random();
	switch(index)
	{
	case 0:
		sg_pDungeonSystem->脱力要請(pchara,効果量(3),効果量(5));
		break;
	case 1:
		sg_pDungeonSystem->軟弱要請(pchara,効果量(4),効果量(5));
		break;
	case 2:
		sg_pDungeonSystem->泥酔要請(pchara,効果量(2));
		break;
	case 3:
		sg_pDungeonSystem->眠り要請(pchara,効果量(2));
		break;
	case 4:
		sg_pDungeonSystem->貧乏要請(pchara,効果量(2));
		break;
	case 5:
		sg_pDungeonSystem->レベルダウン(pchara,1);
		break;
	default:
		;
	}

	return true;
}
int cFood_Bad::食べる_効果_通常(pcCharacter pchara)
{
	return 効果(pchara
		, 効果量(0)//回復量
		, 効果量(1)//HP拡張量
		);
}
int cFood_Bad::食べる_効果_祝福(pcCharacter pchara)
{
	return 食べる_効果_通常(pchara);
}

//------------------------------------------------------------------------------
//焼けた
//------------------------------------------------------------------------------
int cFood_Toast::効果(pcCharacter pchara, int recover, int extend)
{
	//sg_pDungeonSystem->AnimationManager().
	//	Anime_PlaySE(_T("recover.wav"),pchara->足元地形()->place);


	if(pchara->Stomach + 0.99 > pchara->MaxStomach)
	{
		sg_pDungeonSystem->最大満腹度拡張要請(pchara,extend);	
	}
	else
	{
		sg_pDungeonSystem->満腹度回復要請(pchara,recover);	
	}
	sg_pDungeonSystem->回復要請(pchara,効果量(2));
	return true;
}
int cFood_Toast::食べる_効果_通常(pcCharacter pchara)
{
	return 効果(pchara
		, 効果量(0)//回復量
		, 効果量(1)//HP拡張量
		);
}
int cFood_Toast::食べる_効果_祝福(pcCharacter pchara)
{
	return 食べる_効果_通常(pchara);
}



//------------------------------------------------------------------------------
//こげた
//------------------------------------------------------------------------------
int cFood_Burned::効果(pcCharacter pchara, int recover)
{
	//sg_pDungeonSystem->AnimationManager().
	//	Anime_PlaySE(_T("recover.wav"),pchara->足元地形()->place);

		
	sg_pDungeonSystem->満腹度回復要請(pchara,recover);	
	
	return true;
}
int cFood_Burned::食べる_効果_通常(pcCharacter pchara)
{
	return 効果(pchara
		, 効果量(0)//回復量
		);
}
int cFood_Burned::食べる_効果_祝福(pcCharacter pchara)
{
	return 食べる_効果_通常(pchara);
}


//------------------------------------------------------------------------------
//こげた
//------------------------------------------------------------------------------
int cFood_Black::効果(pcCharacter pchara, int reduse, int damage)
{
	//sg_pDungeonSystem->AnimationManager().
	//	Anime_PlaySE(_T("recover.wav"),pchara->足元地形()->place);



	sg_pDungeonSystem->満腹度減少要請(pchara,reduse);

	multiset<攻撃属性::攻撃属性> 属性;

	sg_pDungeonSystem->攻撃接近(攻撃作成(
		投擲者(),//攻撃者
		pchara->足元地形(),//場所
		damage,//攻撃力
		true,//定数ダメージ
		属性,
		true
		));
	
	return true;
}
int cFood_Black::食べる_効果_通常(pcCharacter pchara)
{
	return 効果(pchara
		, 効果量(0)//減少量
		, 効果量(1)//ダメージ
		);
}
int cFood_Black::食べる_効果_祝福(pcCharacter pchara)
{
	return 食べる_効果_通常(pchara);
}


//------------------------------------------------------------------------------
//イモ
//------------------------------------------------------------------------------
int cFood_Potate::効果(pcCharacter pchara, int recover)
{
	//sg_pDungeonSystem->AnimationManager().
	//	Anime_PlaySE(_T("recover.wav"),pchara->足元地形()->place);

	sg_pDungeonSystem->満腹度回復要請(pchara,recover);	

	return true;
}
int cFood_Potate::食べる_効果_通常(pcCharacter pchara)
{
	return 効果(pchara
		, 効果量(0)//回復量
		);
}
int cFood_Potate::食べる_効果_祝福(pcCharacter pchara)
{
	return 食べる_効果_通常(pchara);
}
void cFood_Potate::CutIn(タイミング timing, cValiableField& valiable)
{
	cFood::CutIn(timing,valiable);

	if(timing == 食べ物劣化_タイミング)
	{
		//if(quality() == 0)
		//	sg_pDungeonSystem->アイテム劣化(me(),0);
		//else
		//{
			pcDroping pdrop2 = sg_pDungeonSystem->落ち物生成_設置なし(4014);
			sg_pDungeonSystem->落ち物変化(me(),pdrop2);
		//}
		valiable.doubles.val(変数_汎用ブール) = 1;
	}

}