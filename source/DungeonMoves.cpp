#include "stdafx.h"

#include "cDungeonSystem.h"
#include "EffectFunctions.h"
#include "caFade.h"

#include "cFloor.h"
#include "Event1.h"
#include "GameIdiom.h"

int cDungeonSystem::方向転換要請(pcCharacter pchara, int aspect)
{
	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//方向転換成功フラグ
	CutInM().CutIn(pchara,方向転換直前_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール))
	{
		pchara->aspect = aspect;

		AnimationManager().Anime_DisplayChange(
			&(pchara->visibleaspect),
			aspect);

		return true;
	}
	return false;
}

//即方向転換する。主人公のターン専用
int cDungeonSystem::方向転換要請_主人公専用(pcCharacter pchara, int aspect)
{
	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//方向転換成功フラグ
	CutInM().CutIn(pchara,方向転換直前_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール))
	{

		pchara->aspect = aspect;

		pchara->visibleaspect = aspect;

		return true;

	}
	return false;

}


//移動できたら移動しない
int cDungeonSystem::泥棒確認移動可_主人公専用(pcCharacter pchara, int moveaspect)
{
	int aspect = moveaspect;
	int moveflag = 1;
	int preroomindex = pchara->足元地形()->RoomIndex;
	if(店請求金額(店長(preroomindex)) > 0
		&& !泥棒状態()
		&& 隣接地形移動可( pchara,aspect)
		&& 隣接地形( pchara->足元地形(),aspect,1)->RoomIndex != preroomindex
		)
	{//泥棒しようとしている
		pcCharacter powner = 店長(preroomindex);
		int a;
		for(a=0;a<8;a++)
		{
			if(キャラ視点方角地形(pchara,a,1)->pOnChar && キャラ視点方角地形(pchara,a,1)->pOnChar == powner)
			{
				/*
				valf.doubles.val(変数_汎用ブール) *= 0;
				map<tstring, StyleString> val;
				g_Langメッセージ(_T("店未払いメッセージ"),val);
				*/
				moveflag = 0;
				キャラ視点方角地形(pchara,a,1)->pOnChar->TalkEvent();
				break;
			}
		}

		if(a==8)
		{//店主が隣接していない
			moveflag = 0;
			方向転換要請(pchara, aspect);
			店退出確認();
		}
	}
	return moveflag;

}
//移動できたら移動する
int cDungeonSystem::移動要請(pcCharacter pchara, int moveaspect, int badyaspect)
{
	if(pchara->Condition.混乱状態である())
	{
		badyaspect = moveaspect = random()*8;
	}

	if(badyaspect == -1) badyaspect = moveaspect;

	int preroomindex = pchara->足元地形()->RoomIndex;
	if(隣接地形移動可(pchara, moveaspect))
	{//移動できる

		cValiableField valf;
		valf.doubles.dim(変数_汎用ブール) = 1;//移動成功フラグ
		CutInM().CutIn(pchara,通常移動直前_タイミング,valf);


		if(valf.doubles.val(変数_汎用ブール))
		{
			movecharacter(pchara, moveaspect, badyaspect);


			int nextroomindex = pchara->足元地形()->RoomIndex;

			pcDroping pdrop;
			if((pdrop = pchara->足元()) != NULL)
			{//なんか落ちてる
				pdrop->乗る_記録();
			}


			if(pchara == pPlayerChara())
			{//主人公

				cValiableField valf;
				valf.doubles.dim(変数_汎用実数) = 1.0;
				CutInM().CutIn(pchara,敵起こしやすさ倍率計算時_タイミング,valf);

				Map().mapping_just(pchara->placeX, pchara->placeY);

				if(preroomindex != nextroomindex)
				{
					if(nextroomindex != -1)
					{//部屋入る

						//宴会発覚(nextroomindex);
					
						vector<pcLandform> landlist = Map().RoomList[nextroomindex]->allLandformList();
						int size = landlist.size();
						int i;
						for(i=0;i<size;i++)
						{
							CutInM().CutIn(landlist[i]->pOnChar,主人公部屋入室_タイミング,valf);
							//CutInM().CutIn(landlist[i]->pOnDrop,主人公部屋入室_タイミング,valf);
						}
					}
					else
					{//部屋出る
					
					}
				}

				int a;
				for(a=0;a<8;a++)
				{
					CutInM().CutIn(隣接地形(pchara->足元地形(), a, 1)->pOnChar,主人公隣接_タイミング,valf);
				}

				valf.clear();
				valf.lands.dim(変数_移動場所) = pchara->足元地形();
				CutInM().CutIn(pchara,主人公通常移動直後_タイミング,valf);
			}


			return true;

		}
	}
	
	//移動できなかった
	方向転換要請(pchara, badyaspect);
	return false;
	

}


int cDungeonSystem::すれ違い要請(pcCharacter pchara, int moveaspect, int badyaspect)
{

	if(badyaspect == -1) badyaspect = moveaspect;

	pcCharacter pchara2 = 隣接地形(pchara->足元地形(),moveaspect,1)->pOnChar;
	if(pchara2 == NULL) return false;
	
	int preroomindex = pchara->足元地形()->RoomIndex;
	if(隣接地形移動可地形のみ(pchara, moveaspect) && 隣接地形移動可地形のみ(pchara2, moveaspect+4) && pchara2->すれ違い許可(pchara))
	{//移動できる

		if(pchara == pPlayerChara())
		{
			DashSwitch() = 0;
		}

		passmovecharacter(pchara, pchara2, moveaspect, badyaspect);

		強制ターンエンド要請(pchara2);

		return true;
		/*
		int nextroomindex = pchara->足元地形()->RoomIndex;

		pcDroping pdrop;
		if((pdrop = pchara->足元()) != NULL)
		{//なんか落ちてる
			pdrop->乗る_記録();
		}


		if(pchara == pPlayerChara())
		{//主人公
			if(preroomindex != nextroomindex)
			{
				if(nextroomindex != -1)
				{//部屋入る
					vector<pcLandform> landlist = Map().RoomList[nextroomindex]->allLandformList();
					int size = landlist.size();
					int i;
					for(i=0;i<size;i++)
					{
						CutInM().CutIn(landlist[i],主人公部屋入室_タイミング);
					}
				}
				else
				{//部屋出る
				
				}
			}

			int a;
			for(a=0;a<8;a++)
			{
				CutInM().CutIn(隣接地形(pchara->足元地形(), a, 1),主人公隣接_タイミング);
			}

		}

		return true;
		*/
	}
	else 
	{//移動できなかった
		方向転換要請(pchara, badyaspect);
		return false;
	}

	return false;
}

//キャラクターが隣接地形に移動できるかどうか
bool cDungeonSystem::隣接地形移動可(pcCharacter pchara, int aspect)
{
	cValiableField valf;
	pcLandform land;
	cCoordinate placeaspect;
	placeaspect.SetAspect(aspect);
	land = Map().Land(pchara->placeX + placeaspect.x, pchara->placeY + placeaspect.y);
	valf.doubles.dim(変数_汎用ブール) = 1;//許可
	valf.charas.dim(変数_移動者) = pchara;
	CutInM().CutIn(land->pOnDrop,隣接地形被移動可判定_タイミング,valf);

	return valf.doubles.val(変数_汎用ブール) && isCanNeighberInto(pchara, aspect);
}
//地形のみ見る
bool cDungeonSystem::隣接地形移動可地形のみ(pcCharacter pchara, int aspect)
{
	return isCanNeighberThrough(pchara, aspect);
}

//地形とキャラ両方見る
bool cDungeonSystem::ワープ地形移動可(pcCharacter pchara, pcLandform pland)
{

	return pland->caninto(0,0,0);
}



bool cDungeonSystem::isCanNeighberInto(pcCharacter pchara, int aspect)
{

	return isCanNeighberInto(pchara->足元地形(), aspect, pchara->水上歩行(), pchara->壁中歩行(), pchara->空中歩行());

}
bool cDungeonSystem::isCanNeighberInto(pcLandform pland, int aspect, int waterwalking, int wallwalking, int airwalking)
{

	pcLandform land;
	cCoordinate placeaspect;

	placeaspect.SetAspect(aspect);

	land = Map().Land(pland->placeX + placeaspect.x, pland->placeY + placeaspect.y);




	if(abs(placeaspect.x)+abs(placeaspect.y) > 1)
	{//斜め
		pcLandform sublandX,sublandY;
	
		sublandX = Map().Land(pland->placeX + placeaspect.x, pland->placeY);
		sublandY = Map().Land(pland->placeX, pland->placeY + placeaspect.y);

		land = Map().Land(pland->placeX + placeaspect.x, pland->placeY + placeaspect.y);
		
		if(land->caninto(waterwalking, wallwalking, airwalking) && sublandX->diagonthrough(waterwalking, wallwalking, airwalking) && sublandY->diagonthrough(waterwalking, wallwalking, airwalking)) return true;
	}
	else
	{

		if(land->caninto(waterwalking, wallwalking, airwalking)) return true;
	
	}

	

	return false;
}
bool cDungeonSystem::isCanNeighberThrough(pcLandform pland, int aspect, int waterwalking, int wallwalking, int airwalking)
{

	pcLandform land;
	cCoordinate placeaspect;

	placeaspect.SetAspect(aspect);

	land = Map().Land(pland->placeX + placeaspect.x, pland->placeY + placeaspect.y);




	if(abs(placeaspect.x)+abs(placeaspect.y) > 1)
	{//斜め
		pcLandform sublandX,sublandY;
	
		sublandX = Map().Land(pland->placeX + placeaspect.x, pland->placeY);
		sublandY = Map().Land(pland->placeX, pland->placeY + placeaspect.y);

		land = Map().Land(pland->placeX + placeaspect.x, pland->placeY + placeaspect.y);
		
		if(land->through(waterwalking,wallwalking, airwalking) && sublandX->diagonthrough(waterwalking,wallwalking, airwalking) && sublandY->diagonthrough(waterwalking,wallwalking, airwalking)) return true;
	}
	else
	{

		if(land->through(waterwalking,wallwalking, airwalking)) return true;
	
	}

	return false;

}
bool cDungeonSystem::isCanNeighberThrough(pcCharacter pchara, int aspect)
{
	return isCanNeighberThrough(pchara->足元地形(), aspect, pchara->水上歩行(), pchara->壁中歩行(), pchara->空中歩行());
}
int cDungeonSystem::ワープ要請(pcCharacter pchara, pcLandform pland)
{
	pland = 落下可能地点検索(pland,pchara);

	if(pland == NULL) return false;

	if( ワープ地形移動可(pchara, pland))
	{//移動できる

		warpcharacter(pchara, pland);


		/*
		pcDroping pdrop;
		if((pdrop = pchara->足元()) != NULL)
		{//なんか落ちてる
			pdrop->乗る();
		}
		*/

		return true;
	}

	return false;
}
int cDungeonSystem::キャラ落下(pcCharacter pchara, pcLandform pland, int dropcollition)
{
	pcLandform charaLand;

	unlinkcharacter(pchara);
	
	if(pland->pOnChar == NULL)
	{
		if(dropcollition && pland->pOnDrop != NULL)
		{
			linkcharacter(pchara, pland);
			if((pland->pOnDrop->上に落ちる(pchara)))
			{	
				//位置修正
				
				return false;
			}
		}
	}
	
	if(pchara->死亡())
	{
		//位置修正
		linkcharacter(pchara, pland);
		return false;
	}


	charaLand = 落下可能地点検索(pland,pchara);

	if(charaLand == NULL)
	{
		//位置修正
		linkcharacter(pchara, pland);//仮設置

		GameIdiom::高飛び(pchara,pland);
		
		//メッセージ(pdrop->FullName() + _T(" はどこかに消えてしまった。\n"));
		return false;
	}
	else
	{
		if(charaLand != pland)
		{

			c4DVector start,end;
			//落ちアニメーション
			start.set(pland->placeX,pland->placeY,0,0);
			end.set(charaLand->placeX,charaLand->placeY,0,0);
			AnimationManager().Anime_Throw(pchara,start,end,ROUTE_HIGH);


		}
		//位置修正
		linkcharacter(pchara, charaLand);//設置
		return true;
	}
}
int cDungeonSystem::分身要請(pcCharacter pchara)
{
	pcLandform pland;
	int a;

	for(a=0;a<8;a++)
	{
		pland = sg_pDungeonSystem->キャラ視点方角地形(pchara,a,1);

		if(pland->caninto(0,0,0)) break;
	}
	if(a == 8) return false;//空きなし

	pcCharacter pcharanew = キャラクター生成(pchara->ID(),pchara->CLASS,pchara->Forse,pland);
	
	if(pcharanew != NULL)
	{
		pcharanew->Stomach = pchara->Stomach;
	
		AnimationManager().
				Anime_PlaySE(_T("fission.wav"),pPlayerChara()->足元地形()->place);
		キャラ落下(pcharanew,pchara->足元地形(),false);
		pcharanew->Condition.とても強い刺激();
		pcharanew->Condition.眠り追加(0);
	}
	
	return true;


}
int cDungeonSystem::アイテム隠れ(pcCharacter pchara, pcDroping pdrop)
{
	if(pchara->死亡()) return false;
	//リンクはずし

	cEvent_ItemHidding::pNewEvent(cEvent::PRIORITY_HIGH,pchara,pdrop)->startEvent();

	stockInCharacter(pchara);

	return true;
}

int cDungeonSystem::アイテム隠れ解除(pcCharacter pchara, pcLandform pland)
{
	if(pchara->死亡()) return false;
	//リンクはずし

	if(stockOntCharacter(pchara, pland))
	{

		cValiableField valf;
		valf.lands.dim(変数_攻撃場所) = pland;
		CutInM().CutIn(pchara,アイテム隠れ解除直後時_タイミング,valf);

		return true;
	}
	return false;
}

int cDungeonSystem::吹き飛ばし要請(pcCharacter subjectchara, pcCharacter pchara, int aspect,int distance,int collisionpower)
{
	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
	CutInM().CutIn(pchara,吹き飛ばし判定時_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール) && !フロア飛ばず状態())
	{
		pchara->Condition.とても強い刺激();
		ThrowCharacter(subjectchara,pchara,aspect,distance,collisionpower);
		return true;
	}
	return false;
}
int cDungeonSystem::引き寄せ要請(pcCharacter subjectchara, pcCharacter pchara, int aspect,int distance)
{
	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
	CutInM().CutIn(pchara,引き寄せ判定時_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール) && !フロア飛ばず状態())
	{
		ThrowCharacter(subjectchara,pchara,aspect,distance,0,1);
		return true;
	}
	return false;
}
int cDungeonSystem::場所替え要請(pcCharacter subjectchara, pcCharacter pchara)
{
	SwapCharacter(subjectchara,pchara);
	return true;
}
int cDungeonSystem::投げ飛ばし要請(pcCharacter subjectchara, pcCharacter pchara, pcLandform toland, int collisionpower, int dropcollition, double speed)
{
	cValiableField valf;
	valf.doubles.dim(変数_汎用ブール) = 1;//効果発揮フラグ
	CutInM().CutIn(pchara,投げ飛ばし判定時_タイミング,valf);
	if(valf.doubles.val(変数_汎用ブール) && !フロア飛ばず状態())
	{
		pchara->Condition.とても強い刺激();
		JumpCharacter(subjectchara,pchara,toland,collisionpower,dropcollition, speed);
		return true;
	}
	return false;
}

//射程内の障害物を検索する。障害物がなかった場合最大射程を返す。
pcLandform cDungeonSystem::キャラ飛ばし障害物検索(pcLandform start, int aspect, int range, bool Wallthrough)
{
	
	pcLandform land = start;

	if(range < 0) range = max(MAPWIDTH,MAPHEIGHT);

	cCoordinate now = start->place,vec;
	vec.SetAspect(aspect);

	int i;
	for(i=0;i<range;i++)
	{
		now += vec;
		land = Map().Land(now);
		

		if(!(land->place == now))
		{//端っこ
			return Map().Land(now-vec);
		}

		if(!isCanNeighberInto(land, aspect, 1, Wallthrough, 1))
		{
			return land;
		}
	
	}

	return land;
}

//強制的にキャラを吹き飛ばし
void cDungeonSystem::ThrowCharacter(pcCharacter subjectchara, pcCharacter pchara, int aspect,int distance,int collisionpower,int nearstop)
{

	if(pchara->死亡()) return;
	//リンクはずし
	pcLandform evepland = unlinkcharacter(pchara);

	
	/*
	//描画変更（消える）
	AnimationManager().Anime_DisplayChange(&(pdrop->pri_onland_DrawFlag), FALSE);
	*/
	if(evepland == NULL ) return;

	pcLandform land = 射程内障害物検索(evepland,aspect,distance,true,true,false);

	if(nearstop)
	{
		land = 隣接地形(land,aspect+4,1);
	}

	cCoordinate coo;
	coo.SetAspect(aspect);

	c4DVector start,end;

	pcLandform pland = evepland;//直前地形

	for(;pland != land;)
	{
		int res;


		if(pland->pOnDrop != NULL)
		{
					
			//位置修正（上を通過は位置修正してからじゃないと機能しないかも。）
			linkcharacter(pchara,pland);
			res = pland->pOnDrop->上を通過(pchara);//何かあったらtrue;
			if(res)
			{
				land = pland;
				unlinkcharacter(pchara);
				break;
			}
			unlinkcharacter(pchara);
		}

		evepland = pland;
		pland = 隣接地形(pland,aspect,1);

		//投げアニメーション2
		start.set(evepland->placeX,evepland->placeY,0,0);
		end.set(pland->placeX,pland->placeY,0,0);
		AnimationManager().Anime_Throw(pchara,start,end,ROUTE_LOW,DRAWTYPE_NORMAL);

		if(pland == Map().Land(-1,-1)) break;
	}


	if(pchara->死亡())
	{
		return;
	}


	
	if(land->iswall())
	{//壁にぶつかった

		land = Map().Land(land->place-coo);
		c4DVector start,end;

		//位置修正（衝突は位置修正してからじゃないと機能しない。）
		linkcharacter(pchara,land);

		//if(collisionpower)
		{

			//投げアニメーション3
			start.set(land->placeX ,land->placeY ,0,0);
			end.set(land->placeX + coo.x*0.5,land->placeY + coo.y*0.5,0,0);
			AnimationManager().Anime_Throw(pchara,start,end,ROUTE_LOW,DRAWTYPE_NORMAL);




			//落ちアニメーション
			start.set(land->placeX + coo.x*0.5,land->placeY + coo.y*0.5,0,0);
			end.set(land->placeX,land->placeY,0,0);
			AnimationManager().Anime_Throw(pchara,start,end,ROUTE_HIGH);
			


		}

		

		//カットイン
		//cValiableField val;
		//CutInM().CutIn(pchara,被衝撃_強_タイミング,val);

		//位置修正（衝突は位置修正してからじゃないと機能しない。）
		linkcharacter(pchara,land);

		キャラ落下(pchara, land);
		land = pchara->足元地形();

		if(pchara->死亡())
		{
			return;
		}
		if(collisionpower)
		{
			multiset<攻撃属性::攻撃属性> 属性;
			攻撃接近(攻撃作成(
				subjectchara,//攻撃者
				land,//場所
				collisionpower,//攻撃力
				true,//定数ダメージ
				属性,
				true
				));
		}

	}
	else if(land->pOnChar != NULL)
	{//キャラにぶつかった
		pcLandform collitionland = land;

		land = Map().Land(land->place-coo);
		c4DVector start,end;


		//位置修正（衝突は位置修正してからじゃないと機能しない。）
		linkcharacter(pchara,land);
		//if(collisionpower)
		{

			//投げアニメーション3
			start.set(land->placeX ,land->placeY ,0,0);
			end.set(land->placeX + coo.x*0.5,land->placeY + coo.y*0.5,0,0);
			AnimationManager().Anime_Throw(pchara,start,end,ROUTE_LOW,DRAWTYPE_NORMAL);



			//落ちアニメーション
			start.set(land->placeX + coo.x*0.5,land->placeY + coo.y*0.5,0,0);
			end.set(land->placeX,land->placeY,0,0);
			AnimationManager().Anime_Throw(pchara,start,end,ROUTE_HIGH);

		}

		
		//位置修正（衝突は位置修正してからじゃないと機能しない。）
		linkcharacter(pchara,land);

		//カットイン
		//cValiableField val;
		//CutInM().CutIn(pchara,被衝撃_強_タイミング,val);


		キャラ落下(pchara, land);
		land = pchara->足元地形();
		
		if(collisionpower)
		{

			multiset<攻撃属性::攻撃属性> 属性;
			攻撃接近(攻撃作成(
				subjectchara,//攻撃者
				land,//場所
				collisionpower,//攻撃力
				true,//定数ダメージ
				属性,
				true
				));
			
			攻撃接近(攻撃作成(
				subjectchara,//攻撃者
				collitionland,//場所
				collisionpower,//攻撃力
				true,//定数ダメージ
				属性,
				true
				));
				
		}
	}
	else
	{//ぶつからなかった
		c4DVector start,end;
		

		//位置修正（衝突は位置修正してからじゃないと機能しない。）
		linkcharacter(pchara,land);

		キャラ落下(pchara, land);
	}

	/*
	//描画変更
	AnimationManager().Anime_DisplayChange(&(pdrop->pri_onland_DrawFlag),
		(pdrop->Holder() == NULL && pdrop->Parent() == NULL && !pdrop->broken));
	*/

	return;
}

void cDungeonSystem::JumpCharacter(pcCharacter subjectchara, pcCharacter pchara, pcLandform toland, int collisionpower, int dropcollition, double speed)
{

	if(toland == Map().Land(-1,-1)) return;


	//リンクはずし
	pcLandform evepland = unlinkcharacter(pchara);

	
	/*
	//描画変更（消える）
	AnimationManager().Anime_DisplayChange(&(pdrop->pri_onland_DrawFlag), FALSE);
	*/


	pcLandform land = toland;

	c4DVector start,end;
	//投げアニメーション
	start.set(evepland->placeX,evepland->placeY,0,0);
	end.set(land->placeX,land->placeY,0,0);
	AnimationManager().Anime_Throw(pchara,start,end,ROUTE_HIGH,DRAWTYPE_NORMAL,speed);



	if(land->iswall())
	{//壁にぶつかった


		キャラ落下(pchara, land, dropcollition);
		land = pchara->足元地形();

		if(pchara->死亡())
		{
			return;
		}
		if(collisionpower && subjectchara != pchara)
		{
			multiset<攻撃属性::攻撃属性> 属性;
			攻撃接近(攻撃作成(
				subjectchara,//攻撃者
				land,//場所
				collisionpower,//攻撃力
				true,//定数ダメージ
				属性,
				true
				));
		}

		return;
	}
	else if(land->pOnChar != NULL)
	{//キャラにぶつかった

		
		キャラ落下(pchara, land);

		pcLandform collitionland = pchara->足元地形();

		if(collisionpower)
		{

			multiset<攻撃属性::攻撃属性> 属性;
			攻撃接近(攻撃作成(
				subjectchara,//攻撃者
				land,//場所
				collisionpower,//攻撃力
				true,//定数ダメージ
				属性,
				true
				));
			
			if(subjectchara != pchara)
			{
				攻撃接近(攻撃作成(
					subjectchara,//攻撃者
					collitionland,//場所
					collisionpower,//攻撃力
					true,//定数ダメージ
					属性,
					true
					));
			}
		}

		return;
	}
	else
	{//ぶつからなかった

		

		//位置修正（衝突は位置修正してからじゃないと機能しない。）
		linkcharacter(pchara,land);

		キャラ落下(pchara, land);

		return;
	}

	/*
	//描画変更
	AnimationManager().Anime_DisplayChange(&(pdrop->pri_onland_DrawFlag),
		(pdrop->Holder() == NULL && pdrop->Parent() == NULL && !pdrop->broken));
	*/
	linkcharacter(pchara,land);
	return;
}

void cDungeonSystem::SwapCharacter(pcCharacter subjectchara, pcCharacter pchara)
{
	//リンクはずし
	pcLandform evepland = unlinkcharacter(subjectchara);
	//リンクはずし
	pcLandform topland = unlinkcharacter(pchara);
	

	linkcharacter(pchara, evepland);
	linkcharacter(subjectchara,topland);
	if(evepland->pOnDrop != NULL)
	{
		evepland->pOnDrop->上に落ちる(pchara);
	}

	c4DVector start,end;
	//落ちアニメーション
	start.set(evepland->placeX,evepland->placeY,0,0);
	end = start;
	AnimationManager().Anime_Throw(pchara,start,end,ROUTE_LOW);
	//落ちアニメーション
	start.set(topland->placeX,topland->placeY,0,0);
	end = start;
	AnimationManager().Anime_Throw(subjectchara,start,end,ROUTE_LOW);
}

//キャラを除去する。
void cDungeonSystem::removecharacter(pcCharacter pchara)
{
	pcLandform eveland;


	//死んでるフラグでもある
	pchara->HP = 0;

	//前にいた場所のポインタリンクをはずす。
	eveland = unlinkcharacter(pchara);


	vector<pcCharacter>::iterator itr = find(CharaList().begin(), CharaList().end(), pchara);
	if(itr != CharaList().end()) CharaList().erase(itr);
	/*
	vector<pcCharacter>::iterator itr = CharaList().begin();
	for(;itr != CharaList().end();itr++)
	{
		if(*itr == pchara) break;
	}
	if(itr != CharaList().end()) CharaList().erase(itr);
	*/

	if(pchara == pPlayerChara())
	{//ゲームオーバー
		KnockOutHero();
	}
}

//キャラクターを地面リンクから開放する
pcLandform cDungeonSystem::unlinkcharacter(pcCharacter pchara)
{
	pcLandform eveland;
	eveland = Map().Land(pchara->placeX, pchara->placeY);

	//前にいた場所のポインタリンクをはずす。
	if(eveland->pOnChar == pchara) eveland->pOnChar = NULLCHARA;
	else
	{
		//assert( !_T("キャラの位置情報と地形のキャラ情報が一致しません。") );
		return NULLLAND;
	}

	return eveland;
}
//キャラクターを地面リンクする//いた場所を返す。
void cDungeonSystem::linkcharacter(pcCharacter pchara, pcLandform pland)
{
	if(pchara->足元地形()->pOnChar == pchara) unlinkcharacter(pchara);
	//位置修正
	pchara->placeX = pland->placeX; pchara->placeY = pland->placeY;
	

	pland->pOnChar = pchara;//設置

}


//フロアのキャラを除去する。
void cDungeonSystem::allremovefloorcharacter()
{
	int i,k;
	vector<pcCharacter>::iterator itr = CharaList().begin();
	for(;itr != CharaList().end();itr++)
	{

		/*
		if(CharaList()[i] == sg_pDungeonSystem->pPlayerChara())
		{
			continue;
		}
		
		for(k=0;pFriendChares.size();k++)
		{
			if(CharaList()[i] == sg_pDungeonSystem->pPlayerChara())
			{
				continue;
			}
		}
		*/

		//前にいた場所のポインタリンクをはずす。
		unlinkcharacter(*itr);

	}

	CharaList().clear();
	visibleCharaList().clear();
}

//キャラクターを地面リンクから開放して対比リストに入れる
void cDungeonSystem::stockInCharacter(pcCharacter pchara)
{
	vector<pcCharacter>::iterator itr = find(CharaList().begin(),CharaList().end(),pchara);
	if(itr == CharaList().end())
	{
		return;
	}

	unlinkcharacter(pchara);

	CharaList().erase(itr);

	stockCharaList().push_back(pchara);

	AnimationManager().Anime_DisplayChange(&(pchara->opaque),0);

}
//キャラクターを地面リンクから開放して対比リストに入れる
bool cDungeonSystem::stockOntCharacter(pcCharacter pchara, pcLandform pland)
{
	vector<pcCharacter>::iterator itr = find(stockCharaList().begin(), stockCharaList().end(), pchara);
	if(itr == stockCharaList().end())
	{
		return false;
	}

	pcLandform l = 配置安全地形(pland,pchara,1);
	if(l == NULLLAND)
	{
		return false;
	}
	
	pchara->placeX = l->placeX;
	pchara->placeY = l->placeY;
	pchara->visibleplace.set(l->place.x, l->place.y, 0, 0);


	AnimationManager().Anime_DisplayChange(
			&(pchara->opaque),
			1);

	l->pOnChar = pchara;

	CharaList().push_back(pchara);
	stockCharaList().erase(itr);

	return true;
}


//移動を実行する（強制的に）
void cDungeonSystem::movecharacter(pcCharacter pchara, int moveaspect, int badyaspect)
{
	pcLandform eveland;
	pcLandform goland;



	cCoordinate aspectcoo;
	aspectcoo.SetAspect(moveaspect);

	//前にいた場所のポインタリンクをはずす。
	eveland = unlinkcharacter(pchara);



	//移動
	pchara->placeX += aspectcoo.x;
	pchara->placeY += aspectcoo.y;

	方向転換要請(pchara,badyaspect);

	
	//今の場所のコピー
	goland = Map().Land(pchara->placeX, pchara->placeY);
	


	//今の場所のポインタリンクをつける。
	//goland->pOnChar = pchara;
	linkcharacter(pchara,goland);

	AnimationManager().AddMove(pchara,eveland,goland,pchara->aspect);

}
void cDungeonSystem::passmovecharacter(pcCharacter pchara, pcCharacter pchara2, int moveaspect, int badyaspect)
{
	pcLandform eveland;
	pcLandform goland;



	cCoordinate aspectcoo;
	aspectcoo.SetAspect(moveaspect);

	//前にいた場所のポインタリンクをはずす。
	eveland = unlinkcharacter(pchara);
	unlinkcharacter(pchara2);



	//移動
	pchara->placeX += aspectcoo.x;
	pchara->placeY += aspectcoo.y;
	pchara->aspect = badyaspect;
	
	//移動
	pchara2->placeX -= aspectcoo.x;
	pchara2->placeY -= aspectcoo.y;
	pchara2->aspect = safeAspect(moveaspect+4);

	//今の場所のコピー
	goland = Map().Land(pchara->placeX, pchara->placeY);
	


	//今の場所のポインタリンクをつける。
	//goland->pOnChar = pchara;
	linkcharacter(pchara,goland);
	linkcharacter(pchara2,eveland);

	AnimationManager().AddMove(pchara,eveland,goland,badyaspect);
	AnimationManager().AddMove(pchara2,goland,eveland,moveaspect+4);

}

//ワープ的に動く
void cDungeonSystem::warpcharacter(pcCharacter pchara, pcLandform pland)
{
	pcLandform eveland;

	//前にいた場所のポインタリンクをはずす。
	eveland = unlinkcharacter(pchara);


	//移動
	pchara->placeX = pland->placeX;
	pchara->placeY = pland->placeY;



	//今の場所のポインタリンクをつける。
	pland->pOnChar = pchara;

	
	AnimationManager().Anime_DisplayChange(
		&(pchara->visibleplace),
		c4DVector(pchara->placeX, pchara->placeY, 0, 0));
	
	//AnimationManager().AddMove(pchara,eveland,pland,pchara->aspect);
}

//次のフロアに移動
void cDungeonSystem::movefloor()
{
	//時間計測
	time_SumFrameCount() += time_FrameCount();
	time_Sumtime() += Floortime();

	sg_pDungeonSystem->FloorLevel()++;
	if(sg_pDungeonSystem->FloorLevel() > pDungeon()->MaxFloor())
	{
		GameClear();
	}
	else
	{
		resetfloor();



	}
}

int cDungeonSystem::フロア更新要請()
{
	if(!拠点フラグ())
	{
		CutInM().CutInForAllChara(フロア終了_タイミング);
	}

	pcaFade panime = pcaFade(new caFade);


	if(sg_pDungeonSystem->FloorLevel() < pDungeon()->MaxFloor())
	{
		panime->Init(caFade::ANIME_FADE_OUT);
		AnimationManager().AddAnime(panime);
	}

	AnimationManager().Anime_DisplayChange(
		&(floormoveswitch()),
		true);



	return true;
}




/*
//キャラを消す。(不要)
void cDungeonSystem::キャラ消去_アニメ用(pcCharacter pchara)
{

	
	vector<pcCharacter>::iterator itr = visibleCharaList().begin();
	for(;itr != visibleCharaList().end();itr++)
	{
		if(*itr == pchara) break;
	}
	if(itr != visibleCharaList().end()) visibleCharaList().erase(itr);

}
*/
