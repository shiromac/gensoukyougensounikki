#include "stdafx.h"

#include "Gameobjects.h"

#include "EffectFunctions.h"
#include "FindUtility.h"
#include "cDungeonSystem.h"
#include "cSaveStore.h"

#include "cFloor.h"
#include "utility/debug.h"
#include "cDropingDistribution.h"
#define THROW_RANGE 10
#define MAGIC_RANGE 100

int cDungeonSystem::罠自然湧きID(int outputID[3], int flag)
{
	unsigned int trapIDandEtc = pDungeon()->pfloor(FloorLevel())->trapDP().get(random());

	outputID[0] = trapIDandEtc>>16;
	outputID[1] = (trapIDandEtc>>8)&0xFF;
	outputID[2] = (trapIDandEtc)&0xFF;

	return (outputID[0] != 0);
}
int cDungeonSystem::アイテム自然湧きID(int outputID[3], int flag)
{
	cDropingDistribution iDP = sg_pDungeonSystem->pFloor()->dropitemDP();

	outputID[0] = iDP.get(random());
	//if(!outputID[0])
	//{
	//	OnAssert(_T(__FILE__),__LINE__,false,_T("アイテム自然湧きIDエラー"));
	//}
	return (outputID[0] != 0);
}
int cDungeonSystem::アイテム店湧きID(int outputID[3], int flag)
{
	cDropingDistribution iDP = sg_pDungeonSystem->pFloor()->shopitemDP();

	outputID[0] = iDP.get(random());
	//if(!outputID[0])
	//{
	//	OnAssert(_T(__FILE__),__LINE__,false,_T("アイテム自然湧きIDエラー"));
	//}
	return (outputID[0] != 0);
}
int cDungeonSystem::アイテムレア湧きID(int outputID[3], double Rarity, int flag)
{
	cDropingDistribution iDP = sg_pDungeonSystem->pFloor()->dropitemDP();

	iDP.changeRareDP(Rarity);
	outputID[0] = iDP.get(random());
	//if(!outputID[0])
	//{
	//	OnAssert(_T(__FILE__),__LINE__,false,_T("アイテム自然湧きIDエラー"));
	//}
	return (outputID[0] != 0);
}
int cDungeonSystem::アイテムレア種別湧きID(int outputID[3], double Rarity,
							double Equipment,
							double Bullet,
							double Spell,
							double Food,
							double Drink,
							double Book,
							double Box,
							double MoneyBag,
							double Other,
							int flag)
{
	cDropingDistribution newDP = sg_pDungeonSystem->pFloor()->dropitemDP();
	newDP.changeItemGroupPos(Equipment,
							Bullet,
							Spell,
							Food,
							Drink,
							Book,
							Box,
							MoneyBag,
							Other);
	newDP.changeRareDP(Rarity);
	outputID[0] = newDP.get(random());

	return (outputID[0] != 0);
}

int cDungeonSystem::敵自然湧きID(int outputID[3], int flag)
{
	//敵自然湧き
	unsigned int enemyIDandPower = pDungeon()->pfloor(FloorLevel())->enemyDP().get(random());
	if(enemyIDandPower)
	{
		outputID[0] = enemyIDandPower>>16;
		outputID[1] = (enemyIDandPower>>8)&0xFF;
		outputID[2] = (enemyIDandPower)&0xFF;


		return true;
	}
	return false;
}

//初出現する時に使う。
int cDungeonSystem::初期設置(pcCharacter go, pcLandform land, int firstsetting)
{
	pcLandform l;
	if(go->CharaAttribute().count(キャラ属性::弾幕))
	{
		l = land;
		if(l == NULLLAND)
		{
			return false;
		}
		if(!キャラ配置安全(l,go))
		{
			return false;
		}
	}
	else
	{
		l = 配置安全地形(land,go,firstsetting);
		if(l == NULLLAND)
		{
			return false;
		}
	}
	go->placeX = l->placeX;
	go->placeY = l->placeY;
	go->visibleplace.set(l->place.x, l->place.y, 0, 0);


	pushbackCharaList(go);

	l->pOnChar = go;

	go->opaque = sg_pDungeonSystem->キャラクター目視可(go);
	//AnimationManager().Anime_DisplayChange(
	//		&(go->opaque),
	//		1);

	return true;
}
int cDungeonSystem::初期設置(pcDroping go, pcLandform land, int flexible, int firstsetting)
{
	pcLandform l;
	if(flexible)
	{
		l = 配置安全地形(land,go,firstsetting);
	}
	else
	{
		if(落ち物配置安全(land))
		{
			l = land;
		}
		else
		{
			return false;
		}
	}

	if(l == NULLLAND)
	{
		return false;
	}


	setDropingOnLand(go,l);

	//c4DVector visiplc(go->placeX,go->placeY,0,0);
	//AnimationManager().Anime_DisplayChange(&(go->visibleplace),
	//						visiplc);

	go->placeX = l->placeX;
	go->placeY = l->placeY;
	go->visibleplace.set(l->place.x, l->place.y, 0, 0);

	//l->pOnDrop = go;

	return true;
}
int cDungeonSystem::初期設置_大切(pcDroping go, pcLandform land)
{
	int i;
	for(i=0;i<10000;i++)
	{
		if(初期設置(go,land,0))
		{
			return true;
		}
		//配置できなかったときランダム配置
		land = RandomPlace();
	
	}

	int x,y;
	for(x=0;x<MAPWIDTH;x++)
	{
		for(y=0;y<MAPHEIGHT;y++)
		{
			pcLandform l;
			l = Map().Land(x,y);
			if(land->through(0,0,0))
			{
				if(land->pOnDrop == NULL || (land->pOnDrop != NULL && !(land->pOnDrop->大切())))
				{
					//おいてあるアイテムを廃棄してでも設置する。
					releaseDroping(l);
					初期設置(go,l);
					return true;
				}
			}
		}
	}

	OnAssert(_T(__FILE__),__LINE__,false,_T("重要オブジェクトの配置に失敗しました。ゲームの続行を保障できません。設定を確認してください。<error> Cannot put an important object on land."));

	return false;
}

int cDungeonSystem::アイテム合成(vector<pcDroping> vpdrop, int mitamaFlag, int isyuFrag, int cocktailFlag)
{


	int i,k;
	for(i=0;i<vpdrop.size();i++)
	{
		for(k=i+1;k<vpdrop.size();k++)
		{
			if(mitamaFlag)
			{//御霊合成
				if(vpdrop[i]->属性.count(落ち物属性::装備品))
				{
					int flag = 0;

					if( vpdrop[k]->属性.count(落ち物属性::装備品) )
					{
						vpdrop[i]->quality() += vpdrop[k]->quality();
						vpdrop[k]->quality() = 0;

						int st = 0;

						if(vpdrop[i]->state() == cDroping::STATE_CURSE) st--;
						if(vpdrop[k]->state() == cDroping::STATE_CURSE) st--;

						if(vpdrop[i]->state() == cDroping::STATE_GOOD) st++;
						if(vpdrop[k]->state() == cDroping::STATE_GOOD) st++;

						if(st < 0) vpdrop[i]->state() = cDroping::STATE_CURSE;
						else if(st > 1) vpdrop[i]->state() = cDroping::STATE_GOOD;
						else vpdrop[i]->state() = cDroping::STATE_NORMAL;

						flag = 1;
					}
					
					{
						pcEquipment pequip = boost::dynamic_pointer_cast<cEquipment>(vpdrop[i]);
						pcEquipment pequip2 = boost::dynamic_pointer_cast<cEquipment>(vpdrop[k]);
						if(pequip && pequip2)
						{
							pequip->熟練度カウント加算(pequip2->合成時減少済みproficiency());
							pequip2->熟練度リセット();
							flag = 1;
						}
					}

					if(vpdrop[i]->残りスロット() >= vpdrop[k]->能力スロット())
					{
						if(vpdrop[k]->能力スロット() != 0 || !vpdrop[k]->includedItem.empty())
						{//合成可能
							/*
							落ち物入れ要請(vpdrop[i],vpdrop[k]);
							落ち物入れ子展開要請(vpdrop[k]);
							*/
							int roop;
							if(vpdrop[k]->能力スロット() != 0) 落ち物入れ要請(vpdrop[i],落ち物生成_設置なし(vpdrop[k]->ID(),vpdrop[k]->dpack.integer));
							for(roop = 0;roop < vpdrop[k]->includedItem.size();roop++)
							{//はみ出た
								落ち物入れ要請(vpdrop[i],落ち物生成_設置なし(vpdrop[k]->includedItem[roop]->ID(),vpdrop[k]->includedItem[roop]->dpack.integer));
							}
							vpdrop[k]->shopitemflag() = cDroping::SHOPFLAG_CANNOTYAKU;//厄システム挿入不可
							落ち物破壊要請(vpdrop[k]);

							for(roop = 0;vpdrop[i]->残りスロット() < 0 && !vpdrop[i]->includedItem.empty();roop++)
							{//はみ出た
								落ち物破壊要請(vpdrop[i]->includedItem.back());
								if(roop > 10000)
								{
									OnAssert(_T(__FILE__),__LINE__,false,_T("異常に大きなループが確認されました。強制ブレイクします。"));
									break;
								}
							}
							

							flag = 1;
						}

					}
					if(flag)
					{
						if(vpdrop[k]->能力スロット() == 0 || vpdrop[k]->Parent() != vpdrop[i]) 落ち物破壊要請(vpdrop[k]);
						識別(vpdrop[i],0);
						vpdrop.erase(vpdrop.begin()+k);
						return アイテム合成(vpdrop,mitamaFlag,isyuFrag,cocktailFlag);
					}
				}
			}

			//同種合成
			if(vpdrop[i]->ID() == vpdrop[k]->ID()
				&&(
				vpdrop[i]->属性.count(落ち物属性::飲み物)
				|| vpdrop[i]->属性.count(落ち物属性::食べ物)
				|| vpdrop[i]->属性.count(落ち物属性::読み物)
				|| vpdrop[i]->属性.count(落ち物属性::スペカ)
				))
			{
				if(abs(vpdrop[i]->quality()) < vpdrop[i]->修正値最大値())
				{
					vpdrop[i]->quality() += vpdrop[k]->quality() + 1/vpdrop[i]->効果量修正値倍率();
					vpdrop[k]->quality() = 0;

					vpdrop[i]->修正値丸め();

					int st = 0;

					if(vpdrop[i]->state() == cDroping::STATE_CURSE) st--;
					if(vpdrop[k]->state() == cDroping::STATE_CURSE) st--;

					if(vpdrop[i]->state() == cDroping::STATE_GOOD) st++;
					if(vpdrop[k]->state() == cDroping::STATE_GOOD) st++;

					if(st < 0) vpdrop[i]->state() = cDroping::STATE_CURSE;
					else if(st > 1) vpdrop[i]->state() = cDroping::STATE_GOOD;
					else vpdrop[i]->state() = cDroping::STATE_NORMAL;


					落ち物破壊要請(vpdrop[k]);
					識別(vpdrop[i],0);
					vpdrop.erase(vpdrop.begin()+k);
					return アイテム合成(vpdrop,mitamaFlag,isyuFrag,cocktailFlag);
				}
			}
		}
	}


	return true;
}

int cDungeonSystem::落ち物落下(pcDroping pdrop, pcLandform pland)
{
	pcLandform dropLand;

	
	if(pland->pOnDrop != NULL)
	{
		if(pland->pOnDrop->上に落ちる(pdrop))
		{
			return false;
		}
	}

	if(pdrop->broken)
	{
		return false;
	}


	dropLand = 落下可能地点検索(pland,pdrop);

	if(dropLand == NULL)
	{
		removedroping(pdrop,true);

		map<tstring, StyleString> valiable;
		valiable[_T("Drop")] = pdrop->FullName();
		g_Langメッセージ(_T("落ちもの消滅メッセージ"),valiable);
		
		//メッセージ(pdrop->FullName() + _T(" はどこかに消えてしまった。\n"));
		return false;
	}
	else
	{
		//描画変更
		AnimationManager().Anime_DisplayChange(&(pdrop->pri_onland_DrawFlag), TRUE);
	

		if(dropLand != pland)
		{
			c4DVector start,end;
			//落ちアニメーション
			start.set(pland->placeX,pland->placeY,0,0);
			end.set(dropLand->placeX,dropLand->placeY,0,0);
			AnimationManager().Anime_Throw(pdrop,start,end,ROUTE_HIGH);
		}
		setDropingOnLand(pdrop,dropLand);
		c4DVector visiplc(pdrop->placeX,pdrop->placeY,0,0);
		AnimationManager().Anime_DisplayChange(&(pdrop->visibleplace),
				visiplc);

		
		cValiableField val;
		CutInM().CutIn(pdrop,落ち物落下直後時_タイミング,val);
		
		pdrop->再設置();

		return true;
	}
}

int cDungeonSystem::落ち物設置(pcDroping pdrop, pcLandform pland, bool anime_pararell)
{
	pcLandform dropLand;

	if(pdrop->broken)
	{
		return false;
	}

	dropLand = 落下可能地点検索(pland,pdrop);

	if(dropLand == NULL)
	{
		removedroping(pdrop,true);
		map<tstring, StyleString> valiable;
		valiable[_T("Drop")] = pdrop->FullName();
		g_Langメッセージ(_T("落ちもの消滅メッセージ"),valiable);
		
		//メッセージ(pdrop->FullName() + _T(" はどこかに消えてしまった。\n"));
		return false;
	}
	else
	{
		//描画変更
		AnimationManager().Anime_DisplayChange(&(pdrop->pri_onland_DrawFlag), true);
		if(dropLand != pland)
		{
			c4DVector visiplc(pdrop->placeX,pdrop->placeY,0,0);
			AnimationManager().Anime_DisplayChange(&(pdrop->visibleplace),
				visiplc);

			c4DVector start,end;
			//落ちアニメーション
			start.set(pland->placeX,pland->placeY,0,0);
			end.set(dropLand->placeX,dropLand->placeY,0,0);
			
			if(anime_pararell)
			{
				AnimationManager().Anime_Throw_pararell(pdrop,start,end,ROUTE_HIGH);
			}
			else
			{
				AnimationManager().Anime_Throw(pdrop,start,end,ROUTE_HIGH);
			}
		
		}
		setDropingOnLand(pdrop,dropLand);

		c4DVector visiplc(pdrop->placeX,pdrop->placeY,0,0);
		AnimationManager().Anime_DisplayChange(&(pdrop->visibleplace),
				visiplc);

		pdrop->再設置();

		return true;
	}
}
int cDungeonSystem::落ち物落し物(pcDroping pdrop, pcLandform pland)
{
	//所持から開放
	pdrop = releaseDroping(pdrop);
	return 落ち物設置(pdrop, pland, true);
}

int cDungeonSystem::落ち物ワープ(pcDroping pdrop, pcLandform pland)
{
	pcLandform dropLand;

	if(pdrop->broken)
	{
		return false;
	}

	dropLand = 落下可能地点検索(pland,pdrop);
	if(dropLand == NULL)
	{
		removedroping(pdrop,true);

		map<tstring, StyleString> valiable;
		valiable[_T("Drop")] = pdrop->FullName();
		g_Langメッセージ(_T("落ちもの消滅メッセージ"),valiable);
		
		//メッセージ(pdrop->FullName() + _T(" はどこかに消えてしまった。\n"));
		return false;
	}
	else
	{
		/*
		if(dropLand != pland)
		{
			c4DVector start,end;
			//落ちアニメーション
			start.set(pland->placeX,pland->placeY,0,0);
			end.set(dropLand->placeX,dropLand->placeY,0,0);
			AnimationManager().Anime_Throw(pdrop,start,end,ROUTE_HIGH);
		}
		*/

		//地形から引き離す
		if(pdrop->落ちてる() && !pdrop->内包されている()) releaseDroping(pdrop->現在地形());
		//所持から引き離す
		releaseDroping(pdrop);

		setDropingOnLand(pdrop,dropLand);

		c4DVector visiplc(pdrop->placeX,pdrop->placeY,0,0);
		AnimationManager().Anime_DisplayChange(&(pdrop->visibleplace),
				visiplc);

		return true;
	}
}
int cDungeonSystem::落ち物変化(pcDroping frompdrop, pcDroping topdrop)
{

	落ち物すり替え(frompdrop,topdrop);


	落ち物破壊要請(frompdrop);
	return true;
}
int cDungeonSystem::落ち物すり替え(pcDroping frompdrop, pcDroping topdrop)
{

	if(frompdrop->isDiscover()) topdrop->discover();
	topdrop->placeX = frompdrop->placeX;
	topdrop->placeY = frompdrop->placeY;
	

	AnimationManager().Anime_DisplayChange(&(topdrop->visibleplace), frompdrop->visibleplace);
	AnimationManager().Anime_DisplayChange(&(topdrop->pri_onland_DrawFlag), frompdrop->pri_onland_DrawFlag);
	AnimationManager().Anime_DisplayChange(&(topdrop->opaque), frompdrop->opaque);

	if(frompdrop->落ちてる() && !frompdrop->内包されている())
	{
		pcLandform pland = frompdrop->現在地形();
		releaseDroping(pland);
		//強制的に落ち物を床に設置する。
		setDropingOnLand(topdrop, pland);

		c4DVector visiplc(topdrop->placeX,topdrop->placeY,0,0);
		AnimationManager().Anime_DisplayChange(&(topdrop->visibleplace),
				visiplc);
	}
	else
	{
		pcCharacter pchara = frompdrop->Holder();

		if(pchara == NULL)
		{//浮いてるかも
			pcDroping parentdrop = frompdrop->Parent();
			topdrop->Holder_weakpointer = wpcCharacter(pchara);
			if(parentdrop == NULL)
			{//浮いてる
				//落ち物破壊要請(frompdrop);
				OnAssert(_T(__FILE__),__LINE__,false,_T("落ち物変化の関数の仕様外の使い方をしています。"));
				return false;
			}
			else
			{//箱の中とか
				topdrop->Parent_weakpointer = wpcDroping(parentdrop);
				vector<pcDroping>::iterator itr = find(parentdrop->includedItem.begin(), parentdrop->includedItem.end(), frompdrop);
				if(itr != parentdrop->includedItem.end())
				{
					*itr = topdrop;
				}
				else
				{
					OnAssert(_T(__FILE__),__LINE__,false,_T("アイテム内包関係が壊れています。"));
				}	
			}

		}
		else
		{//もってる
			pcDroping parentdrop = frompdrop->Parent();
			topdrop->Holder_weakpointer = wpcCharacter(pchara);
			if(parentdrop == NULL)
			{//じか
				vector<pcDroping>::iterator itr = find(pchara->holdItem.begin(), pchara->holdItem.end(), frompdrop);
				if(itr != pchara->holdItem.end())
				{
					*itr = topdrop;
				}
				else
				{
					OnAssert(_T(__FILE__),__LINE__,false,_T("アイテム所持関係が壊れています。"));
				}
			}
			else
			{//箱の中とか
				topdrop->Parent_weakpointer = wpcDroping(parentdrop);
				vector<pcDroping>::iterator itr = find(parentdrop->includedItem.begin(), parentdrop->includedItem.end(), frompdrop);
				if(itr != parentdrop->includedItem.end())
				{
					*itr = topdrop;
				}
				else
				{
					OnAssert(_T(__FILE__),__LINE__,false,_T("アイテム内包関係が壊れています。"));
				}	
			}
		}
	}


	releaseDroping(frompdrop);
	//落ち物破壊要請(frompdrop);
	return true;
}

int cDungeonSystem::落ち物破壊要請(pcDroping pdrop, int lossFlag)
{
	if(pdrop == NULL) return false;

	if(pdrop->被破壊可())
	{

		cValiableField val;
		sg_pDungeonSystem->CutInM().CutIn(pdrop,被破壊直前_タイミング,val);

		removedroping(pdrop, lossFlag);
		//描画変更（消える）
		sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(&(pdrop->pri_onland_DrawFlag), false);

		return true;
	}

	return false;
}

int cDungeonSystem::落ち物非破壊非設置放棄要請(pcDroping pdrop)
{
	if(pdrop == NULL) return false;
	pcDroping reldrop = releaseDroping(pdrop);
	
	return true;
}

int cDungeonSystem::足元拾得要請(pcCharacter pchara)
{

	pcDroping pdrop = pchara->足元();
	if(pdrop == NULL) return false;

	if(落ち物まとめ要請(pdrop, pchara))
	{
		return true;
	}
	else if(落ち物拾得可(pdrop, pchara))
	{
		pickupDropingfromStep(pchara);
		
		cValiableField val;
		val.drops.dim(変数_対象落ち物) = pdrop;
		sg_pDungeonSystem->CutInM().CutIn(pchara,拾い直後_タイミング,val);
		
		//描画変更（消える）
		sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(&(pdrop->pri_onland_DrawFlag), false);

		return true;
	}
	return false;
}
int cDungeonSystem::ころび要請(pcCharacter pchara, int DropItemNum, int mustFlag, int breakFlag)
{
	if(pchara == NULL) return false;
	int i,size;


	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
	if(!mustFlag) CutInM().CutIn(pchara,ころび直前_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール) || mustFlag)
	{

		vector<pcDroping> vpdrop = FindUtility::装備品以外の表面手持ちアイテム店売り除く(pchara);

		size = DropItemNum;
		for(i=0;i<size;i++)
		{
			if(vpdrop.empty()) break;
		
			int index = vpdrop.size()*random();
			pcDroping pdrop = vpdrop[index];
			
			pcDroping reldrop = releaseDroping(pdrop);
				
			if(reldrop != NULL)
			{
				落ち物設置(reldrop,pchara->足元地形(), !(size-i == 1));

				if(breakFlag)
				{
					cValiableField valf;
					CutInM().CutIn(reldrop,被衝撃_弱_タイミング,valf);
				}
				
			}

			vpdrop.erase(vpdrop.begin()+index);
		}
		return true;
	}
	return false;
}

int cDungeonSystem::遠隔拾得要請(pcCharacter pchara, pcDroping pdrop)
{
	if(pdrop == NULL) return false;

	if(落ち物まとめ要請(pdrop, pchara))
	{
		return true;
	}
	else if(落ち物拾得可(pdrop, pchara))
	{

		pickupDroping(pchara, pdrop);
		//描画変更（消える）
		sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(&(pdrop->pri_onland_DrawFlag), FALSE);

	
		return true;
	}
	return false;
}

int cDungeonSystem::遠隔拾得要請_持ち物余白無考慮(pcCharacter pchara, pcDroping pdrop)
{
	if(pdrop == NULL) return false;

	if(落ち物まとめ要請(pdrop, pchara))
	{
		return true;
	}
	else if(落ち物拾得可_持ち物余白無考慮(pdrop, pchara))
	{

		pickupDroping(pchara, pdrop);
		//描画変更（消える）
		sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(&(pdrop->pri_onland_DrawFlag), FALSE);

	
		return true;
	}
	return false;
}

int cDungeonSystem::泥棒要請(pcCharacter psubjectchara, pcDroping pdrop, int massageFlag, int mustflag)
{
	if(psubjectchara == NULL) return false;
	if(pdrop == NULL) return false;
	int i,size;

	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
	
	cCoordinate nowco(psubjectchara->placeX,psubjectchara->placeY);
	cCoordinate youco;
			
	pcCharacter pobjectchara;
	if((pobjectchara = pdrop->Holder()) != NULL)
	{//誰かの持ち物
	
		if(!mustflag) CutInM().CutIn(pobjectchara,泥棒直前_タイミング,valf);
		youco.Set(pobjectchara->placeX,pobjectchara->placeY);

	}
	else
	{
		youco.Set(pdrop->placeX,pdrop->placeY);
	}
	CutInM().CutIn(pdrop,泥棒直前_タイミング,valf);
	
	if(valf.doubles.val(変数_汎用ブール) || mustflag)
	{
		方向転換要請(psubjectchara, (youco - nowco).GetAspect());

		pcDroping reldrop = releaseDroping(pdrop);
			
		if(reldrop != NULL)
		{
			if(遠隔拾得要請(psubjectchara,reldrop))
			{
				if(massageFlag)
				{
					EffectFunctions::煙エフェクト1(youco.x, youco.y);
				}

				//描画変更
				sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(&(reldrop->pri_onland_DrawFlag), TRUE);

				c4DVector start,end;
				//投げアニメーション1
				start.set(youco.x ,youco.y, THROW_BASE_HEIGHT,0);
				end.set(nowco.x, nowco.y, THROW_BASE_HEIGHT,0);
				AnimationManager().Anime_Throw(reldrop,start,end,ROUTE_HIGH);
				
				//描画変更
				sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(&(reldrop->pri_onland_DrawFlag), FALSE);
				
				if(massageFlag)
				{
					map<tstring, StyleString> valiable;
					valiable[_T("Chara")] = psubjectchara->FullName();
					valiable[_T("Item")] = pdrop->FullName();
					g_Langメッセージ(_T("アイテム泥棒メッセージ"),valiable);

				}
				return true;
			}

		}


	}
	if(massageFlag)
	{
		map<tstring, StyleString> valiable;
		valiable[_T("Chara")] = psubjectchara->FullName();
		valiable[_T("Item")] = pdrop->FullName();
		g_Langメッセージ(_T("アイテム泥棒失敗メッセージ"),valiable);
	}
	return false;
}

int cDungeonSystem::アイテム弾き飛ばし要請(pcCharacter psubjectchara, pcDroping pdrop, int distanse, int massageFlag)
{
	if(pdrop == NULL) return false;

	if(装備強制はずし(pdrop,false))
	{
		pcCharacter pobjectchara = pdrop->Holder();
		pdrop = releaseDroping(pdrop);
		if(pdrop == NULL) return false;

		int aspect = psubjectchara->aspect;
		pcLandform fromland = pobjectchara->足元地形();
		pcLandform land = 射程内障害物検索(fromland,aspect,distanse,true,false,false,false);

		if(land->iswall()) land = 隣接地形(land,aspect+4,1);
		pcLandform pland = land;
		pcLandform evepland = fromland;

		//描画変更
		sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(&(pdrop->pri_onland_DrawFlag), TRUE);

		c4DVector start,end;
		//投げアニメーション1
		start.set(evepland->placeX ,evepland->placeY ,THROW_BASE_HEIGHT,0);
		end.set(pland->placeX,pland->placeY,THROW_BASE_HEIGHT,0);
		AnimationManager().Anime_Throw(pdrop,start,end,ROUTE_HIGH);
		
		落ち物落下(pdrop, land);

		if(massageFlag)
		{
			map<tstring, StyleString> valiable;
			valiable[_T("Chara")] = psubjectchara->ShortName();
			valiable[_T("Drop")] = pdrop->FullName();
			g_Langメッセージ(_T("アイテム弾き飛ばしメッセージ"),valiable);
					
		}

		return true;
	}

	return false;
}
int cDungeonSystem::落ち物まとめ要請(pcDroping pdrop, pcCharacter pchara)
{
	int i;
	if(pdrop == NULL) return false;

	if(pdrop->被まとめ要請(pchara))
	{
		return true;		
	}

	for(i=0;i<pchara->holdItem.size();i++)
	{

		if(pchara->holdItem[i]->まとめ要請(pdrop))
		{//まとめ成功

			return true;
		}
	}

	return false;
}

int cDungeonSystem::主人公お金拾得要請(pcDroping pdrop)
{
	if(pdrop->属性.count(落ち物属性::お金))
	{
		主人公お金増減(pdrop->quality());

		map<tstring, StyleString> valiable;
		valiable[_T("Drop")] = pdrop->FullName();
		g_Langメッセージ(_T("お金拾得メッセージ"),valiable);
		
		//メッセージ(pdrop->FullName() + _T(" 拾った。\n"));

		return true;
	}

	return false;
}
int cDungeonSystem::主人公お金増減(int value)
{
	GameScreenInterface.money.value += value;

	if(GameScreenInterface.money.value > 主人公お金所持量最大() ) GameScreenInterface.money.value = 主人公お金所持量最大();
	
	AnimationManager().Anime_DisplayChange(
		&GameScreenInterface.dispmoney,
		GameScreenInterface.money);
	return true;
}
int cDungeonSystem::主人公お金所持量()
{
	return GameScreenInterface.money.value;
}
int cDungeonSystem::主人公お金所持量最大()
{
	return 999999999;
}
bool cDungeonSystem::落ち物拾得可(pcDroping pdrop, pcCharacter pchara)
{
	return 落ち物拾得可_持ち物余白無考慮(pdrop,pchara) && pchara->持ち物余白あり();
}
bool cDungeonSystem::落ち物拾得可_持ち物余白無考慮(pcDroping pdrop, pcCharacter pchara)
{
	if(pdrop == NULL) return false;
	if(pchara == NULL) return false;

	if(pdrop->Holder() != NULL)
	{//持ってる人がいる。
		return false;
	}

	if(pdrop->Parent() != NULL && ! pdrop->倉庫の中())
	{//何処かに入ってる。
		return false;
	}

	cValiableField valiable;
	valiable.doubles.dim(変数_汎用ブール) = pdrop->被拾得可() && pchara->拾得可();
	valiable.charas.dim(変数_対象者) = pchara;
	valiable.drops.dim(変数_対象落ち物) = pdrop;
	CutInM().CutIn(pchara,アイテム拾得判定時_タイミング, valiable);

	return valiable.doubles.val(変数_汎用ブール);

}
int cDungeonSystem::無条件投擲要請(pcCharacter subjectchara, pcLandform pland, pcDroping pdrop, int aspect, int option, int throwrange)
{
	

	if(option != DRAWTYPE_ATTACKDRAW && subjectchara == pPlayerChara())
	{
		map<tstring, StyleString> valiable;
		valiable[_T("Drop")] = pdrop->FullName();
		g_Langメッセージ(_T("落ちもの投擲メッセージ"),valiable);

		//メッセージ(pdrop->FullName()+_T("を投げた。\n"));
	}

	//描画変更
	sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(&(pdrop->pri_onland_DrawFlag), TRUE);



	if(throwrange < 0) throwrange = THROW_RANGE;

	int himanage = bool(option & THROW_HIMANAGE);

	if(フロア飛ばず状態()) throwrange = 0;

	if(himanage)
	{
		cValiableField val;
		val.doubles.dim(変数_汎用ブール) = pdrop->投擲貫通();
		val.doubles.dim(変数_汎用実数) = throwrange;
		CutInM().CutIn(subjectchara,ひまなげ投擲貫通判定時_タイミング, val);

		himanageThrowDroping(subjectchara, pland, pdrop, aspect, val.doubles.val(変数_汎用実数), val.doubles[変数_汎用ブール], option);
	}
	else
	{
		cValiableField val;
		val.doubles.dim(変数_汎用ブール) = pdrop->投擲貫通();
		val.doubles.dim(変数_汎用実数) = throwrange;
		CutInM().CutIn(subjectchara,投擲貫通判定時_タイミング, val);

		ThrowDroping(subjectchara, pland, pdrop, aspect, val.doubles.val(変数_汎用実数), val.doubles[変数_汎用ブール], option);
	}
	return true;

	
	
	return false;

}

int cDungeonSystem::投擲要請(pcCharacter pchara, pcDroping pdrop, int option, int throwrange)
{

	if(落ち物投擲可(pchara, pdrop))
	{
		return 無条件投擲要請(pchara, pchara->足元地形(), pdrop, pchara->aspect, option, throwrange);
	}

	
	return false;
}
int cDungeonSystem::吹き飛ばし要請(pcCharacter subjectchara, pcDroping pdrop, int aspect,int distance)
{
	ThrowDroping(subjectchara, pdrop->現在地形(),pdrop,aspect,distance,0);
	return true;
}
int cDungeonSystem::引き寄せ要請(pcCharacter subjectchara, pcDroping pdrop, int aspect,int distance)
{
	ThrowDroping(subjectchara, pdrop->現在地形(),pdrop,aspect,distance,0,DRAWTYPE_NORMAL, 1);
	return true;
}
int cDungeonSystem::魔法発射要請(pcCharacter pchara, pcDroping pdrop)
{
		//落ちてるとき取得可稼動か調べる
	if( (pdrop->落ちてる() && 落ち物拾得可_持ち物余白無考慮(pdrop, pchara))
		|| !pdrop->落ちてる() )
	{

		MagicShooting(pchara, pdrop);
		return true;

	}
	
	
	return false;
}
	


int cDungeonSystem::足元設置要請(pcCharacter pchara, pcDroping pdrop)
{

	if(落ち物放棄可(pchara, pdrop))
	{
		pcLandform pland = pchara->足元地形();
		if(落ち物設置可(pdrop, pland))
		{
			//開放
			pcDroping reldrop = releaseDroping(pdrop);
			
			if(reldrop != NULL)
			{

				setDropingOnLand(reldrop, pland);
				//描画変更
				sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(&(pdrop->pri_onland_DrawFlag), true);

				c4DVector visiplc(pdrop->placeX,pdrop->placeY,0,0);
					AnimationManager().Anime_DisplayChange(&(pdrop->visibleplace),
						visiplc);

				reldrop->再設置();

				return true;
			
			}
		}
	}

	return false;
}

bool cDungeonSystem::落ち物放棄可(pcCharacter pchara, pcDroping pdrop)
{
	if(pchara == NULL) return true;
	if(pdrop == NULL) return false;
	return pdrop->被放棄可() && pchara->持ち物放棄可();
}
bool cDungeonSystem::落ち物投擲可(pcCharacter pchara, pcDroping pdrop)
{
	if(pchara == NULL) return true;
	if(pdrop == NULL) return false;

	//落ちてるとき取得可稼動か調べる
	if( (pdrop->落ちてる() && 落ち物拾得可_持ち物余白無考慮(pdrop, pchara))
		|| !pdrop->落ちてる() )
	{
		if(落ち物放棄可(pchara, pdrop))
		{

			return pdrop->被投擲可() && pchara->持ち物投擲可();

		}
	}
	return false;
}
bool cDungeonSystem::落ち物設置可(pcDroping pdrop, pcLandform pland)
{
	return 落ち物設置可_アイテム判定無し(pdrop,pland) && (pland->pOnDrop == NULL);
}
bool cDungeonSystem::落ち物設置可_アイテム判定無し(pcDroping pdrop, pcLandform pland)
{
	if(pland == NULL) return false;
	if(pdrop == NULL) return false;
	return pdrop->被設置可() && pland->設置可();
}
int cDungeonSystem::足元交換要請(pcCharacter pchara, pcDroping pdrop)
{
	if(落ち物放棄可(pchara, pdrop))
	{
		pcLandform pland = pchara->足元地形();
		if(落ち物設置可_アイテム判定無し(pdrop, pland) )
		{
			if(!pdrop->内包されている())
			{

				if(落ち物まとめ要請(pchara->足元(), pchara))
				{
					足元設置要請(pchara, pdrop);
					return true;
				}
				else if(落ち物拾得可_持ち物余白無考慮(pchara->足元(), pchara))
				{
					//描画変更（消える）
					sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(&(pdrop->pri_onland_DrawFlag), false);

					exchangeDropingfromStep(pchara, pdrop);
					
					pdrop->再設置();
					return true;
				}

			}
		}
	}
	return false;
}
int cDungeonSystem::ItemSize(vector<pcDroping> vpdrop)
{
	int i,size = vpdrop.size();
	int mass = 0;
	for(i=0;i<size;i++)
	{
		mass += vpdrop[i]->size();
	}
	return mass;
}

int cDungeonSystem::落ち物入れ要請(pcDroping box, pcDroping pdrop)
{
	if(box->Holder() != NULL)
	{
		if(pdrop->落ちてる())
		{
			if(落ち物拾得可_持ち物余白無考慮(pdrop,box->Holder()))
			{
				return putInto(box,pdrop);
			}
		}
		else
		{
			return putInto(box,pdrop);
		}
	}
	else
	{
		return putInto(box,pdrop);
	}
	return false;
}
//入れる側の入れ子を展開する
int cDungeonSystem::落ち物入れ子展開要請(pcDroping pdrop)
{
	pcDroping box = pdrop->Parent();
	pcCharacter receiver = pdrop->Holder();

	if(box != NULL)
	{
		int i;
		vector<pcDroping> vpdrop = pdrop->includedItem;
		for(i=0;i<vpdrop.size();i++)
		{
			落ち物出し要請(vpdrop[i],box);
		}
		return true;
	}
	if(receiver != NULL)
	{
		int i;
		vector<pcDroping> vpdrop = pdrop->includedItem;
		for(i=0;i<vpdrop.size();i++)
		{
			落ち物出し要請(vpdrop[i],receiver);
		}
		return true;
	}
	return true;
}
int cDungeonSystem::落ち物出し要請(pcDroping pdrop, pcCharacter receiver)
{
	return putOut(pdrop,receiver);
}
int cDungeonSystem::落ち物出し要請(pcDroping pdrop, pcDroping receiver)
{
	return putOut(pdrop,receiver);
}
int cDungeonSystem::落ち物ばら撒き要請(pcDroping box)
{

	return AlldropOut(box);
}
int cDungeonSystem::落ち物入れ替え要請(pcDroping pdrop, pcDroping pdrop2)
{
	//描画変更（消える）
	AnimationManager().Anime_DisplayChange(&(pdrop2->pri_onland_DrawFlag), FALSE);
	//描画変更（消える）
	AnimationManager().Anime_DisplayChange(&(pdrop->pri_onland_DrawFlag), FALSE);

	return changeDrop(pdrop, pdrop2);
}
int cDungeonSystem::倉庫入れ要請(vector<pcDroping>& store, const int storecapacity, pcDroping pdrop)
{
	//if(落ち物放棄可(pPlayerChara(), pdrop))
	{

		if(ItemSize(store) < storecapacity)
		{
			//描画変更（消える）
			sg_pDungeonSystem->AnimationManager().Anime_DisplayChange(&(pdrop->pri_onland_DrawFlag), false);

			return storeInto(store,pdrop);
		}
		
	}
	return false;

}
int cDungeonSystem::倉庫出し要請(vector<pcDroping>& store, pcDroping pdrop)
{
	if(落ち物拾得可(pdrop, pPlayerChara()))
	{

		return storeOut(store,pdrop);

	}
	return false;
}
int cDungeonSystem::アイテム倉庫行き要請(pcDroping pdrop)
{
	if(pdrop->大切() || !pdrop->属性.count(落ち物属性::アイテム)){
		return false;
	}
	sg_pDungeonSystem->アイテム店フラグ(pdrop, cDroping::SHOPFLAG_SHOULDYAKU);
	sg_pDungeonSystem->アイテム割引フラグ(pdrop, cDroping::SHOPDISCOUNTFLAG_FREE);
	sg_pDungeonSystem->厄システム挿入要請(sg_pDungeonSystem->pSaveQuest,pdrop);
	sg_pDungeonSystem->落ち物破壊要請(pdrop);
	return true;
}

int cDungeonSystem::厄システム挿入要請(pcSaveQuest pQuest, pcDroping pdrop)
{
	if(pdrop->shopitemflag() == cDroping::SHOPFLAG_YAKU
		|| pdrop->shopitemflag() == cDroping::SHOPFLAG_SHOP
		|| pdrop->shopitemflag() == cDroping::SHOPFLAG_CANNOTYAKU
		|| pdrop->倉庫の中())
	{//すでに挿入されてる || 売り物 || 厄システム挿入不可 || 倉庫の中
		return false;
	}
	if(pdrop->shopitemflag() == cDroping::SHOPFLAG_SHOULDYAKU)
	{
		アイテム店フラグ(pdrop, cDroping::SHOPFLAG_YAKU);
		pQuest->hinaPickItem.push_back(pdrop);
		return true;
	}
	pcEquipment pcEqu = boost::dynamic_pointer_cast<cEquipment>(pdrop);
	if(pcEqu != NULL)
	{
		if(pcEqu->熟練度() >= 100)
		{
			アイテム店フラグ(pdrop, cDroping::SHOPFLAG_YAKU);
			pQuest->hinaPickItem.push_back(pdrop);
			return true;
		}
	}
	pcBox pcb = boost::dynamic_pointer_cast<cBox>(pdrop);
	if(pcb != NULL)
	{
		厄システム挿入要請(pQuest, pcb->includedItem);
	}



	return false;
}
void cDungeonSystem::厄システム挿入要請(pcSaveQuest pQuest, vector<pcDroping>& vpdrop)
{
	int i,size = vpdrop.size();
	for(i=0;i<size;i++)
	{
		厄システム挿入要請(pQuest, vpdrop[i]);
	}
}
int cDungeonSystem::厄ポイント冒険評価(int gameClearFlag, int homeFlag, pcSaveQuest pQuest, pcCharacter pPlayer)
{
	int value = 0;
	int i, size = pQuest->hinaPickItem.size();
	for(i=0;i<size;i++)
	{
		value += pQuest->hinaPickItem[i]->厄ポイント単体売値();
	}

	if(!gameClearFlag)
	{//ゲームオーバー
		vector<pcDroping> vdrop = pPlayer->holdItem;
		int i, money = 0,size = vdrop.size();
		for(i=0;i<size;i++)
		{
			money += vdrop[i]->内包アイテム込み売却受け取り値();
		}
		value += money / 10;
	}

	if(!homeFlag) value += cDungeon::culcuscore(pQuest)/20;
	return value;
}
int cDungeonSystem::厄システム整理()
{
	int i, size = pSaveData->pSaveStore()->hinaItem.size();
	for(i=0;i<size;i++)
	{
		releaseDroping(pSaveData->pSaveStore()->hinaItem[i]);
		pSaveData->pSaveStore()->hinaItem[i]->broken = false;
	}

	//多すぎたらまえからカット
	for(i=0;i<size;i++)
	{
		if(pSaveData->pSaveStore()->hinaItem.size() <= 30) break;
		pSaveData->pSaveStore()->hinaItem.erase(pSaveData->pSaveStore()->hinaItem.begin());
	}
	return true;
}

//強制的に持ち物を投げる
void cDungeonSystem::ThrowDroping(pcCharacter subjectchara, pcLandform fromland, pcDroping pdrop, int aspect ,int distance, int passthrough, int option, int nearstop)
{
	if(pdrop->落ちてる() && !pdrop->内包されている())
	{//落ちてる。
		releaseDroping(pdrop->現在地形());
	}
	releaseDroping(pdrop);

	
	int drawtype = bool(option & DRAWTYPE_ATTACKDRAW);
	int himanage = bool(option & THROW_HIMANAGE);

	cValiableField val;
	val.doubles.dim(変数_汎用実数) = 0;
	val.doubles.dim(変数_汎用実数2) = 0;
	//CutInM().CutIn(subjectchara,投擲反射回数計算時_タイミング,val);
	CutInM().CutIn(subjectchara,投擲反射回数計算時_タイミング,val);
	CutInM().CutIn(pdrop,投擲反射回数計算時ドロップ専用_タイミング,val);
	int refrection = val.doubles[変数_汎用実数];
	int magari = val.doubles[変数_汎用実数2];
	int throwroute = himanage;


	AnimationManager().Anime_DisplayChange(
		&(pdrop->visibleaspect),
		aspect);
	pdrop->discover();


	if(aspect % 2 == 0)//斜めじゃない
	refrection = magari;

	int count;
	for(count=0;count<refrection+1;count++)
	{

		pcLandform land;
		if(pdrop->大切())
		{
			land = 射程内障害物検索(fromland,aspect,distance,true,true,true,true);
			land = 隣接地形(land,aspect+4,1);
		}
		else
		{
			if(himanage)
			{
				land = 隣接地形(fromland,aspect,distance);
			}
			else
			{
				land = 射程内障害物検索(fromland,aspect,distance,!passthrough,!passthrough,false);
				if(nearstop)
				{
					land = 隣接地形(land,aspect+4,1);
				}
			}
		}


		//投げ関係変数セット
		pdrop->跳ね返し無効フラグ = false;
		pdrop->投擲者_weakpointer = wpcCharacter(subjectchara);
		pdrop->aspect = aspect;

		cCoordinate coo;
		coo.SetAspect(aspect);

		c4DVector start,end;

		pcLandform pland,evepland;
		if(himanage)
		{//ひまなげ
			pland = land;
			evepland = fromland;

				
			//投げアニメーション1
			start.set(evepland->placeX + coo.x*0.3,evepland->placeY + coo.y*0.3,THROW_BASE_HEIGHT,0);
			end.set(pland->placeX,pland->placeY,THROW_BASE_HEIGHT,0);
			AnimationManager().Anime_Throw(pdrop,start,end,ROUTE_HIGH,drawtype);
			
		}
		else
		{
			pland = fromland;
			evepland = pland;//直前地形
			if(pland != land && (pland->place - land->place).dif() > 1)
			{
				pland = 隣接地形(pland,pdrop->aspect,1);
				//投げアニメーション1
				start.set(evepland->placeX + coo.x*0.3,evepland->placeY + coo.y*0.3,THROW_BASE_HEIGHT,0);
				end.set(pland->placeX,pland->placeY,THROW_BASE_HEIGHT,0);
				AnimationManager().Anime_Throw(pdrop,start,end,ROUTE_LOW,drawtype);
			}


			for(;pland != land;)
			{
				int res;


				if(pland->pOnDrop != NULL)
				{
							
					//位置修正（上を通過は位置修正してからじゃないと機能しないかも。）
					pdrop->placeX = pland->placeX; pdrop->placeY = pland->placeY;
					res = pland->pOnDrop->上を通過(pdrop);//何かあったらtrue;
					if(res && !passthrough)
					{//何かあったかつ貫通でない
						land = pland;
						break;
					}
				}

				if(passthrough)
				{
					if(pland->pOnChar != NULL)
					{//キャラにぶつかった
						c4DVector start,end;
						
						//位置修正（衝突は位置修正してからじゃないと機能しない。）
						pdrop->placeX = pland->pOnChar->placeX; pdrop->placeY = pland->pOnChar->placeY;
						
						投擲攻撃接近(pland->pOnChar,pdrop,passthrough);

					}
				}

				evepland = pland;
				pland = 隣接地形(pland,pdrop->aspect,1);
				distance--;


				//投げアニメーション2
				start.set(evepland->placeX,evepland->placeY,THROW_BASE_HEIGHT,0);
				end.set(pland->placeX,pland->placeY,THROW_BASE_HEIGHT,0);
				AnimationManager().Anime_Throw(pdrop,start,end,ROUTE_LOW,drawtype);

				if(pland == Map().Land(-1,-1)) break;
			}

		}
			
		if(passthrough)
		{
			if(pland->pOnChar != NULL && subjectchara != pland->pOnChar)
			{//キャラにぶつかった
				c4DVector start,end;
				
				//位置修正（衝突は位置修正してからじゃないと機能しない。）
				pdrop->placeX = pland->pOnChar->placeX; pdrop->placeY = pland->pOnChar->placeY;
				
				投擲攻撃接近(pland->pOnChar,pdrop,passthrough);

			}

			map<tstring, StyleString> valiable;
			valiable[_T("Drop")] = pdrop->FullName();
			g_Langメッセージ(_T("落ちもの消滅メッセージ"),valiable);
		
			落ち物破壊要請(pdrop);
		}


		if(pdrop->broken)
		{


			//位置修正（衝突は位置修正してからじゃないと機能しない。）
			pdrop->placeX = land->placeX; pdrop->placeY = land->placeY;
		
			return;
		}


		
		if(land->iswall())
		{//壁にぶつかった

			land = Map().Land(land->place-coo);
			c4DVector start,end;

			//投げアニメーション3
			start.set(land->placeX ,land->placeY ,THROW_BASE_HEIGHT,0);
			end.set(land->placeX + coo.x*0.5, land->placeY + coo.y*0.5,THROW_BASE_HEIGHT,0);
			AnimationManager().Anime_Throw(pdrop,start,end,ROUTE_LOW,drawtype);



			if(count < refrection && !passthrough)
			{//反射回数残りあり
				if(aspect%2)
				{//斜め撃ち
					pcLandform landwall = Map().Land(land->place+coo);
				
					pcLandform outLand;
					int outAspect;
					if(壁反射判定且検索(outLand, outAspect, landwall, aspect))
					{//反射した
						int Normalaspect = safeAspect((aspect+outAspect)/2+2);//反射面
						
						fromland = outLand;
						pdrop->visibleaspect = aspect = outAspect;
						
						continue;
					}
				}
				else
				{
					pcLandform landwall = Map().Land(land->place+coo);
				
					pcLandform outLand;
					int outAspect;
					if(壁曲がり判定且検索(outLand, outAspect, landwall, aspect))
					{//反射した
						int Normalaspect = safeAspect((aspect+outAspect)/2+2);//反射面
						
						fromland = outLand;
						pdrop->visibleaspect = aspect = outAspect;
						
						continue;
					}
				}
			}
			
			//落ちアニメーション
			start.set(land->placeX + coo.x*0.5,land->placeY + coo.y*0.5,THROW_BASE_HEIGHT,0);
			end.set(land->placeX,land->placeY,0,0);
			AnimationManager().Anime_Throw(pdrop,start,end,ROUTE_HIGH);


			
			//位置修正（衝突は位置修正してからじゃないと機能しない。）
			pdrop->placeX = land->placeX; pdrop->placeY = land->placeY;

			//カットイン
			cValiableField val;
			CutInM().CutIn(pdrop,被衝撃_強_タイミング,val);


			落ち物落下(pdrop, land);
			
			break;
		}
		else if(land->pOnChar != NULL && subjectchara != land->pOnChar)
		{//キャラにぶつかった
			c4DVector start,end;


			
			//位置修正（衝突は位置修正してからじゃないと機能しない。）
			pdrop->placeX = land->pOnChar->placeX; pdrop->placeY = land->pOnChar->placeY;
			
			投擲攻撃接近(land->pOnChar,pdrop);

			break;
		}
		else
		{//ぶつからなかった
			c4DVector start,end;
			
			//落ちアニメーション
			start.set(land->placeX,land->placeY,THROW_BASE_HEIGHT,0);
			end.set(land->placeX,land->placeY,0,0);
			AnimationManager().Anime_Throw(pdrop,start,end,ROUTE_HIGH);

			//位置修正（衝突は位置修正してからじゃないと機能しない。）
			pdrop->placeX = land->placeX; pdrop->placeY = land->placeY;
			
			落ち物落下(pdrop, land);

			break;
		}

		/*
		//描画変更
		AnimationManager().Anime_DisplayChange(&(pdrop->pri_onland_DrawFlag),
			(pdrop->Holder() == NULL && pdrop->Parent() == NULL && !pdrop->broken));
		*/

	}

	return;
}
	
//強制的に持ち物を投げる
void cDungeonSystem::himanageThrowDroping(pcCharacter subjectchara, pcLandform fromland, pcDroping pdrop, int aspect ,int distance, int passthrough, int option, int nearstop)
{
	if(pdrop->落ちてる() && !pdrop->内包されている())
	{//落ちてる。
		releaseDroping(pdrop->現在地形());
	}
	releaseDroping(pdrop);

	
	int drawtype = bool(option & DRAWTYPE_ATTACKDRAW);
	int himanage = bool(option & THROW_HIMANAGE);


	cValiableField val;
	val.doubles.dim(変数_汎用実数) = 0;
	//CutInM().CutIn(subjectchara,投擲反射回数計算時_タイミング,val);
	//CutInM().CutIn(pdrop,投擲反射回数計算時_タイミング,val);
	int refrection = val.doubles[変数_汎用実数];

	int throwroute = himanage;
	

	AnimationManager().Anime_DisplayChange(
		&(pdrop->visibleaspect),
		aspect);
	pdrop->discover();



	int count;
	for(count=0;count<refrection+1;count++)
	{

		pcLandform land;
		if(pdrop->大切())
		{
			land = 射程内障害物検索(fromland,aspect,distance,true,true,true,true);
			land = 隣接地形(land,aspect+4,1);
		}
		else
		{
			if(THROW_HIMANAGESTRAIGHT & option)
			{
				land = 隣接地形(fromland,aspect,distance);
					
			}
			else
			{
				int size = sg_pDungeonSystem->CharaList().size();
				multimap<int,pcCharacter> sortMap;
				int i;
				int maxForcevalue = 0;
				pcCharacter pchara;
				cCoordinate coo;

				for(i=0;i<size;i++)
				{//敵キャラリスト作成
					pchara = sg_pDungeonSystem->CharaList()[i];

					coo.x = pchara->placeX - subjectchara->placeX;
					coo.y = pchara->placeY - subjectchara->placeY;
					if(sg_pDungeonSystem->キャラクター敵対判定(subjectchara,pchara)
						&& coo.dif() <= distance
						&& aspect_AngleAspect(coo.GetAspect(),aspect) < 2)
					{
						if(maxForcevalue < pchara->被攻撃優先度())
						{//優先度高い
							maxForcevalue = pchara->被攻撃優先度();
							sortMap.clear();
						}
						else if(maxForcevalue > pchara->被攻撃優先度())
						{//優先度低い
							continue;
						}
						sortMap.insert(pair<int,pcCharacter>(-coo.dif(),pchara));
					}
				}

				multimap<int,pcCharacter>::iterator itr = sortMap.begin();
				if(sortMap.size())
				{//最も近い敵をサーチする。
					for(;itr != sortMap.end();itr++)
					{

						land = itr->second->足元地形();
					}
				}
				else
				{
					//land = 隣接地形(fromland,aspect,distance);
					land = 射程内障害物検索(fromland,aspect,distance,true,true,false,false);
				
				}
			}
		}


		//投げ関係変数セット
		pdrop->跳ね返し無効フラグ = false;
		pdrop->投擲者_weakpointer = wpcCharacter(subjectchara);
		pdrop->aspect = aspect;

		cCoordinate coo;
		coo.SetAspect(aspect);

		c4DVector start,end;

		pcLandform pland,evepland;



		pland = land;
		evepland = fromland;
		//投げアニメーション1
		start.set(evepland->placeX + coo.x*0.3,evepland->placeY + coo.y*0.3,THROW_BASE_HEIGHT,0);
		end.set(pland->placeX,pland->placeY,0,0);
		AnimationManager().Anime_Throw(pdrop,start,end,ROUTE_HIGH,drawtype,1.4);


	
		if(land->pOnChar != NULL && subjectchara != land->pOnChar)
		{//キャラにぶつかった
			c4DVector start,end;


			
			//位置修正（衝突は位置修正してからじゃないと機能しない。）
			pdrop->placeX = land->pOnChar->placeX; pdrop->placeY = land->pOnChar->placeY;
			
			val.clear();
			CutInM().CutIn(pdrop,落ち物ひまなげ落下直後時_タイミング,val);

			投擲攻撃接近(land->pOnChar,pdrop);

			break;
		}
		else if(land->pOnDrop != NULL)
		{//落ち物にぶつかった

			//位置修正（衝突は位置修正してからじゃないと機能しない。）
			pdrop->placeX = land->placeX; pdrop->placeY = land->placeY;
				
			val.clear();
			CutInM().CutIn(pdrop,落ち物ひまなげ落下直後時_タイミング,val);
			//pland->pOnDrop->上に落ちる(pdrop);

			落ち物落下(pdrop, land);
			
			
			if(pdrop->broken)
			{
				return;
			}
		
		}
		else
		{

			//位置修正（衝突は位置修正してからじゃないと機能しない。）
			pdrop->placeX = land->placeX; pdrop->placeY = land->placeY;
			

			val.clear();
			CutInM().CutIn(pdrop,落ち物ひまなげ落下直後時_タイミング,val);

			if(pdrop->broken)
			{


				//位置修正（衝突は位置修正してからじゃないと機能しない。）
				pdrop->placeX = land->placeX; pdrop->placeY = land->placeY;
			
				return;
			}


			落ち物落下(pdrop, land);

			break;
		}


		/*
		//描画変更
		AnimationManager().Anime_DisplayChange(&(pdrop->pri_onland_DrawFlag),
			(pdrop->Holder() == NULL && pdrop->Parent() == NULL && !pdrop->broken));
		*/

	}

	//カットイン
	val.clear();
	CutInM().CutIn(pdrop,被衝撃_強_タイミング,val);

	return;
}

//強制的に魔法を発射する
int cDungeonSystem::MagicShooting(pcCharacter pchara, pcDroping pdrop)
{
	pcBook pbook = boost::dynamic_pointer_cast<cBook>(pdrop);
	if(pbook == NULL)
	{
		return false;
	}



	//投げ関係変数セット
	//pdrop->跳ね返し無効フラグ = false;
	pdrop->投擲者_weakpointer = wpcCharacter(pchara);


	//魔法生成//魔法->本にした
	/*
	cMagic *magic;
	pcDroping packedmagic = pcDroping(magic = new cMagic);
	magic->objectDrop = pdrop;
	*/
	cCoordinate coo;

	

	pcLandform startland = pchara->足元地形();
	int aspect = pbook->visibleaspect = pchara->aspect;
	
	AnimationManager().Anime_DisplayChange(
		&(pbook->visibleaspect),
		aspect);

	coo.SetAspect(aspect);

	vector<pcLandform> landpass;

	c4DVector start;
	start.set(pchara->placeX + coo.x*0.3,pchara->placeY + coo.y*0.3,THROW_BASE_HEIGHT,0);
	

	int count;
	for(count=0;count<(pbook->反射回数()+1);count++)
	{

		pcLandform land = 射程内障害物検索(landpass, startland, aspect, MAGIC_RANGE, true, pbook->効果対象_キャラ(), pbook->効果対象_アイテム());
				//射程内障害物検索(pcLandform start, int aspect, int range, bool stopWall, bool stopChara, bool stopDrop)
		
		for(;;)
		{
			if(pbook->効果対象_アイテム() && land != 射程内障害物検索(landpass, startland, aspect, MAGIC_RANGE, true, pbook->効果対象_キャラ(), false))
			{//アイテムのせいでとまった
				if(land->pOnDrop != NULL && !land->pOnDrop->isDiscover())
				{//見えてない
					land = 射程内障害物検索(landpass, land, aspect, MAGIC_RANGE, true, pbook->効果対象_キャラ(), pbook->効果対象_アイテム());
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}

		if(land->iswall())
		{//壁にぶつかった

			c4DVector end;

			//投げアニメーション
			end.set(land->placeX - coo.x*0.5,land->placeY - coo.y*0.5,THROW_BASE_HEIGHT,0);
			AnimationManager().Anime_Throw(pdrop,start,end,ROUTE_LOW,DRAWTYPE_ATTACKDRAW);


			if(aspect%2 && count < pbook->反射回数())
			{//斜め撃ち且一回目
			
				pcLandform outLand;
				int outAspect;
				if(壁反射判定且検索(outLand, outAspect, land, aspect))
				{//反射した
					int Normalaspect = safeAspect((aspect+outAspect)/2+2);//反射面
					
					startland = outLand;
					pbook->visibleaspect = aspect = outAspect;
					
					coo.SetAspect(aspect);
					start.set(startland->placeX + coo.x*0.5,startland->placeY + coo.y*0.5,THROW_BASE_HEIGHT,0);
					
					pbook->反射エフェクト(start,Normalaspect);


					continue;
				}
			}
			
			pbook->効果(NULLCHARA,NULLDROP,landpass,aspect);
			
			break;
		}
		else if(land->pOnChar != NULL)
		{//キャラにぶつかった
			c4DVector end;

			//投げアニメーション
			end.set(land->placeX,land->placeY,THROW_BASE_HEIGHT,0);
			AnimationManager().Anime_Throw(pdrop,start,end,ROUTE_LOW,DRAWTYPE_ATTACKDRAW);

			

			pbook->aspect = aspect;
			魔法接近(land->pOnChar,pdrop);
			pbook->効果(NULLCHARA,NULLDROP,landpass,aspect);

			break;
		}
		else if(land->pOnDrop != NULL)
		{//アイテムにぶつかった
			c4DVector end;

			//投げアニメーション
			end.set(land->placeX,land->placeY,THROW_BASE_HEIGHT,0);
			AnimationManager().Anime_Throw(pdrop,start,end,ROUTE_LOW,DRAWTYPE_ATTACKDRAW);


			pbook->効果(NULLCHARA,land->pOnDrop,landpass,aspect);
			break;
		}
		else
		{//ぶつからなかった
			c4DVector end;

			//投げアニメーション
			end.set(land->placeX,land->placeY,THROW_BASE_HEIGHT,0);
			AnimationManager().Anime_Throw(pdrop,start,end,ROUTE_LOW,DRAWTYPE_ATTACKDRAW);
			
			pbook->効果(NULLCHARA,NULLDROP,landpass,aspect);

			break;
		}
	}

	return true;
}
	


//強制的に足元から持ち物を拾う
int cDungeonSystem::pickupDropingfromStep(pcCharacter pchara)
{
	pcDroping pdrop;
	
	//地形から引き離す
	pdrop = releaseDroping(pchara->足元地形());

	haveDroping(pchara, pdrop);
	

	return true;
}

//強制的に持ち物を拾う
int cDungeonSystem::pickupDroping(pcCharacter pchara, pcDroping pdrop)
{	
	//地形から引き離す
	if(pdrop->落ちてる() && !pdrop->内包されている())
		pdrop = releaseDroping(pdrop->現在地形());
	else
		pdrop = releaseDroping(pdrop);

	haveDroping(pchara, pdrop);
	

	return true;
}


//強制的に足元と持ち物を交換する
int cDungeonSystem::exchangeDropingfromStep(pcCharacter pchara, pcDroping pdrop)
{
	pcDroping pstepdrop,phavedrop;


	//地形から引き離す
	pstepdrop = releaseDroping(pchara->足元地形());

	pcCharacter holder = pdrop->Holder();
	pcDroping parent = pdrop->Parent();
	//所持から引き離す
	phavedrop = releaseDroping(pdrop);


	//足元に置く
	setDropingOnLand(phavedrop, pchara->足元地形());

	//所持
	pstepdrop->Holder_weakpointer = wpcCharacter(holder);
	pstepdrop->Parent_weakpointer = wpcDroping(parent);
	

	
	if(parent != NULL)
	{
		parent->includedItem.push_back(pstepdrop);
	}
	else
	{
		holder->holdItem.push_back(pstepdrop);
	}

	return true;
}


//強制的に持ち物を持つ
int cDungeonSystem::haveDroping(pcCharacter pchara, pcDroping pdrop)
{

	//ホールダー設定
	pdrop->Holder_weakpointer = wpcCharacter(pchara);

	//アイテムを持つ。
	pchara->holdItem.push_back(pdrop);


	
	return true;
}
//ドロップリストに追加する
//持ち込みアイテム、階をまたぐアイテムにつかう
int cDungeonSystem::insertDroplist(vector<pcDroping> pdropvec, int appreciating)
{
	int i;
	int size = pdropvec.size();
	for(i=0;i<size;i++)
	{
		pdropvec[i]->discover();
		if(pdropvec[i]->shopitemflag() == cDroping::SHOPFLAG_SHOP)
		{
			pdropvec[i]->shopitemflag() = cDroping::SHOPFLAG_NONE;
		}
		if(appreciating) 識別(pdropvec[i],false);
		pushbackDropList(pdropvec[i]);
		if(!(pdropvec[i]->includedItem.empty())) insertDroplist(pdropvec[i]->includedItem, appreciating);
	}
	return true;
}
//強制的に落ち物を床に設置する。
int cDungeonSystem::setDropingOnLand(pcDroping pdrop, pcLandform pland)
{

	pland->pOnDrop = pdrop;
	
	//最後に設置されたアイテムを保存するためのもの
	if(pland->RoomIndex != -1)
	{
		Map().RoomList[pland->RoomIndex]->plastputland = pland;
	}

	pdrop->placeX = pland->placeX;
	pdrop->placeY = pland->placeY;

	//ポインタリンクはずす
	pdrop->Holder_weakpointer = wpcCharacter(NULLCHARA);
	pdrop->Parent_weakpointer = wpcDroping(NULLDROP);



	//AnimationManager().Anime_DisplayChange(&(pdrop->opaque), pdrop->isDiscover());
	//AnimationManager().Anime_DisplayChange(&(pdrop->pri_onland_DrawFlag), pdrop->is_onland_DrawFlag());
	
	//セット
	//pushbackDropList(pdrop);
	
	//アイテム強制マッピング(pdrop);

	return true;
}


//強制的に落ち物を地形から引き離す
pcDroping cDungeonSystem::releaseDroping(pcLandform pland)
{
	if(pland->pOnDrop != NULL)
	{
		pcDroping pdrop = pland->pOnDrop;
		
		


		pland->pOnDrop = NULLDROP;

		//リストから消去
		//vector<pcDroping>::iterator itr = find(DropList().begin(), DropList().end(), pdrop);
		//if(itr != DropList().end()) DropList().erase(itr);
		/*
		vector<pcDroping>::iterator itr = DropList().begin();
		for(;itr != DropList().end();itr++)
		{
			if(*itr == pdrop) break;
		}
		if(itr != DropList().end()) DropList().erase(itr);
		*/
		return pdrop;
	}
	else
	{
		return NULLDROP;
	}

}




//強制的に落ち物を所持から引き離す
pcDroping cDungeonSystem::releaseDroping(pcDroping pdrop)
{
	//所持者判定
	pcCharacter pchara = pdrop->Holder();
	//所持者削除
	pdrop->Holder_weakpointer = wpcCharacter(NULLCHARA);
				

	//入れ子判定
	pcDroping pparent = pdrop->Parent();
	//入れ子削除
	pdrop->Parent_weakpointer = wpcDroping(NULLDROP);
	

	//所持者から離す
	if(pchara != NULL)
	{
		vector<pcDroping>::iterator pdpitr = pchara->holdItem.begin();
		for(;pdpitr != pchara->holdItem.end();)
		{
			if((*pdpitr) == pdrop)
			{//検索ヒット //しなくてもいい
				//削除
				pchara->holdItem.erase(pdpitr);
				
				break;
			}
			pdpitr++;
		}
	
	}

	//入れ子から離す
	if(pparent != NULL)
	{
		vector<pcDroping>::iterator pdpitr = pparent->includedItem.begin();
		for(;pdpitr != pparent->includedItem.end();)
		{
			if((*pdpitr) == pdrop)
			{//検索ヒット //しなかったらおかしいわけだが
				//削除
				pparent->includedItem.erase(pdpitr);
				
				break;
			}
			pdpitr++;
		}
	}


	pdrop->Release();

	return pdrop;
}

//フロアの落ち物を除去する。
void cDungeonSystem::allremovefloorDroping()
{
	
	int x,y;

	/*
	vector<pcDroping> templist = DropList();
	
	vector<pcDroping>::iterator itr = templist.begin();
	for(;itr!=templist.end();itr++)
	{
		(*itr)->broken = true;
		releaseDroping((*itr)->現在地形());
		
	}
	*/

	/*
	for(x=0;x<MAPWIDTH;x++)
	{
		for(y=0;y<MAPHEIGHT;y++)
		{
			if(Map().Land(x,y)->pOnDrop)Map().Land(x,y)->pOnDrop->broken = true;
		}
	}
	for(x=0;x<MAPWIDTH;x++)
	{
		for(y=0;y<MAPHEIGHT;y++)
		{
			releaseDroping(Map().Land(x,y));
		}
	}
	*/


	/*
	for(x=0;x<MAPWIDTH;x++)
	{
		for(y=0;y<MAPHEIGHT;y++)
		{
			removedropingnoanime(Map().Land(x,y)->pOnDrop);
		}
	}
	*/

	
	DropList().clear();
	visibleDropList().clear();


}

//落ち物を除去する。
void cDungeonSystem::removedroping(pcDroping pdrop, int lossFlag)
{

	removedropingnoanime(pdrop,lossFlag);

	
}
void cDungeonSystem::removedropingnoanime(pcDroping pdrop, int lossFlag)
{
	if(pdrop == NULL) return;


	if(pdrop->broken) return;

	pcLandform eveland;

	//壊れているフラグ
	pdrop->broken = cDroping::BREAKFLAG_NORMAL + lossFlag*cDroping::BREAKFLAG_LOSS;




	eveland = Map().Land(pdrop->placeX, pdrop->placeY);


	//前にいた場所のポインタリンクをはずす。
	if(eveland != NULL && eveland->pOnDrop == pdrop)
	{//落ちてる。
		releaseDroping(eveland);
	}



	//所持からポインタリンクをはずす。
	releaseDroping(pdrop);

	//厄システム
	厄システム挿入要請(pSaveQuest, pdrop);


	vector<pcDroping>::iterator itr = find(DropList().begin(), DropList().end(), pdrop);
	if(itr != DropList().end()) DropList().erase(itr);
	/*
	vector<pcDroping>::iterator itr = DropList().begin();
	for(;itr != DropList().end();itr++)
	{
		if(*itr == pdrop) break;
	}
	if(itr != DropList().end()) DropList().erase(itr);
	*/
}


//強制的に入れる
int cDungeonSystem::putInto(pcDroping box, pcDroping pdrop)
{
	if(box == pdrop) return false;
	if(box == NULL || pdrop == NULL) return false;
	if(pdrop->落ちてる())
	{//落ちてる

		//地面から開放
		pdrop = releaseDroping(pdrop->現在地形());
	
	}
	else
	{//持ってる

		//所持から開放
		pdrop = releaseDroping(pdrop);
	
	}

	putInto_simple(box, pdrop);

	return true;
}
//強制的に入れる
int cDungeonSystem::putInto_simple(pcDroping box, pcDroping pdrop)
{
	if(box == pdrop) return false;
	if(box == NULL || pdrop == NULL) return false;

	//パレント設定
	pdrop->Parent_weakpointer = wpcDroping(box);

	//ホールダー設定
	//pdrop->Holder_weakpointer = wpcCharacter(box->Holder());
	pdrop->Holder_weakpointer = wpcCharacter(NULLCHARA);

	//アイテムを持つ。
	box->includedItem.push_back(pdrop);

	return true;
}

//強制的に出す
int cDungeonSystem::putOut(pcDroping pdrop, pcCharacter receiver)
{
	//所持から開放
	pdrop = releaseDroping(pdrop);
	
	//pdrop->Holder_weakpointer = wpcCharacter(receiver);

	
	haveDroping(receiver, pdrop);


	return true;
}
//強制的に出す
int cDungeonSystem::putOut(pcDroping pdrop, pcDroping receiver)
{
	//所持から開放
	pdrop = releaseDroping(pdrop);
	
	putInto(receiver, pdrop);


	return true;
}

//強制的に中身を全部ばら撒く
int cDungeonSystem::AlldropOut(pcDroping box)
{

	for(;box->includedItem.size();)
	{
		pcDroping pdrop = box->includedItem[0];

		//所持から開放
		pdrop = releaseDroping(pdrop);
	
		if(box->includedItem.empty())
		{//ラスト
			落ち物落下( pdrop, box->現在地形());
		}
		else
		{
			落ち物設置( pdrop, box->現在地形(), true);
		}
	}


	return true;
}

//強制的に入れ替えする
int cDungeonSystem::changeDrop(pcDroping pdrop, pcDroping pdrop2)
{
	pcDroping pboxdrop,phavedrop;



	pcCharacter boxholder = pdrop2->Holder();
	pcDroping boxparent = pdrop2->Parent();
	pcLandform boxland = pdrop2->現在地形();
	//所持から引き離す
	pboxdrop = releaseDroping(pdrop2);



	pcCharacter holder = pdrop->Holder();
	pcDroping parent = pdrop->Parent();
	pcLandform land = pdrop->現在地形();
	//所持から引き離す
	phavedrop = releaseDroping(pdrop);



	//所持
	pboxdrop->Holder_weakpointer = wpcCharacter(holder);
	pboxdrop->Parent_weakpointer = wpcDroping(parent);
	if(parent != NULL)
	{
		parent->includedItem.push_back(pboxdrop);
	}
	else if(holder != NULL)
	{
		holder->holdItem.push_back(pboxdrop);
	}
	else
	{
		//足元に置く
		setDropingOnLand(pboxdrop, land);
	}


	//所持
	phavedrop->Holder_weakpointer = wpcCharacter(boxholder);
	phavedrop->Parent_weakpointer = wpcDroping(boxparent);
	if(boxparent != NULL)
	{
		boxparent->includedItem.push_back(phavedrop);
	}
	else if(boxholder != NULL)
	{
		boxholder->holdItem.push_back(phavedrop);
	}
	else
	{
		//足元に置く
		setDropingOnLand(phavedrop, boxland);
	}

	return true;
}

//強制的に入れる
int cDungeonSystem::storeInto(vector<pcDroping>& store, pcDroping pdrop)
{

	if(pdrop->落ちてる() && !pdrop->内包されている())
	{//落ちてる
		//地面から開放
		pdrop = releaseDroping(pdrop->現在地形());
	}
	else
	{//持ってる
		//所持から開放
		pdrop = releaseDroping(pdrop);
	}
	//パレント設定
	pdrop->Parent_weakpointer = wpcDroping(NULLDROP);
	pdrop->Holder_weakpointer = wpcCharacter(NULLCHARA);

	//アイテムを持つ。
	store.push_back(pdrop);

	return true;
}
//強制的に出す
int cDungeonSystem::storeOut(vector<pcDroping>& store, pcDroping pdrop)
{
	//入れ子から離す
	vector<pcDroping>::iterator pdpitr = store.begin();
	if(pdrop->内包されている())
	{
		releaseDroping(pdrop);
	}
	else
	{
		for(;pdpitr != store.end();)
		{
			if((*pdpitr) == pdrop)
			{//検索ヒット //しなかったらおかしいわけだが
				//削除
				store.erase(pdpitr);
				
				break;
			}
			pdpitr++;
		}
	}
	haveDroping(pPlayerChara(), pdrop);
	return true;
}
int cDungeonSystem::storeOut(vector<pcDroping>& store, pcDroping pdrop, pcLandform pland)
{
	//入れ子から離す
	vector<pcDroping>::iterator pdpitr = store.begin();
	for(;pdpitr != store.end();)
	{
		if((*pdpitr) == pdrop)
		{//検索ヒット //しなかったらおかしいわけだが
			//削除
			store.erase(pdpitr);
			
			break;
		}
		pdpitr++;
	}

	if(pdrop->broken)
	{
		return false;
	}

	pcLandform dropLand = 落下可能地点検索(pland,pdrop);

	setDropingOnLand(pdrop,dropLand);
	return true;
}